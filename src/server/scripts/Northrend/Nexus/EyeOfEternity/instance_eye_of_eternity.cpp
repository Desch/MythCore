/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "eye_of_eternity.h"

class instance_eye_of_eternity : public InstanceMapScript
{
public:
    instance_eye_of_eternity() : InstanceMapScript("instance_eye_of_eternity", 616) { }

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new instance_eye_of_eternity_InstanceMapScript(pMap);
    }

    struct instance_eye_of_eternity_InstanceMapScript : public InstanceScript
    {
        instance_eye_of_eternity_InstanceMapScript(Map* pMap) : InstanceScript(pMap)
        {
            SetBossNumber(MAX_ENCOUNTER);

            vortexTriggers.clear();
            portalTriggers.clear();

            malygosGUID     = 0;
            lastPortalGUID  = 0;
            platformGUID    = 0;
            exitPortalGUID  = 0;
        };

        bool SetBossState(uint32 type, EncounterState state)
        {
            if(!InstanceScript::SetBossState(type, state))
                return false;

            if(type == DATA_MALYGOS_EVENT)
            {
                if(state == FAIL)
                {
                    for(std::list<uint64>::const_iterator itr_trigger = portalTriggers.begin(); itr_trigger != portalTriggers.end(); ++itr_trigger)
                    {
                        if(Creature* trigger = instance->GetCreature(*itr_trigger))
                        {
                            // just in case
                            trigger->RemoveAllAuras();
                            trigger->AI()->Reset();
                        }
                    }

                    SpawnGameObject(GO_FOCUSING_IRIS, focusingIrisPosition);
                    SpawnGameObject(GO_EXIT_PORTAL, exitPortalPosition);

                    if(GameObject* platform = instance->GetGameObject(platformGUID))
                        platform->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_DESTROYED);
                }
                else if(state == DONE)
                {
                    if(Creature* malygos = instance->GetCreature(malygosGUID))
                        malygos->SummonCreature(NPC_ALEXSTRASZA, 829.0679f, 1244.77f, 279.7453f, 2.32f);

                    SpawnGameObject(GO_EXIT_PORTAL, exitPortalPosition);

                    // we make the platform appear again because at the moment we don't support looting using a vehicle
                    if(GameObject* platform = instance->GetGameObject(platformGUID))
                        platform->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_DESTROYED);

                    if(GameObject* chest = instance->GetGameObject(chestGUID))
                        chest->SetRespawnTime(7*DAY);
                }
            }
            return true;
        }

        // There is no other way afaik...
        void SpawnGameObject(uint32 entry, Position& pos)
        {
            GameObject* pGo = new GameObject;
            if(!pGo->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT),entry, instance,
                PHASEMASK_NORMAL, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation(),
                0, 0, 0, 0, 120, GO_STATE_READY))
            {
                delete pGo;
                return;
            }

            instance->Add(pGo);
        }

        void OnGameObjectCreate(GameObject* pGo)
        {
            switch(pGo->GetEntry())
            {
                case GO_NEXUS_RAID_PLATFORM:
                    platformGUID = pGo->GetGUID();
                    break;
                case GO_FOCUSING_IRIS:
                    pGo->GetPosition(&focusingIrisPosition);
                    break;
                case GO_EXIT_PORTAL:
                    exitPortalGUID = pGo->GetGUID();
                    pGo->GetPosition(&exitPortalPosition);
                    break;
                case GO_ALEXSTRASZA_S_GIFT:
                case GO_ALEXSTRASZA_S_GIFT_2:
                    chestGUID = pGo->GetGUID();
                    break;
            }
        }

        void OnCreatureCreate(Creature* pCreature)
        {
            switch(pCreature->GetEntry())
            {
                case NPC_VORTEX_TRIGGER:
                    vortexTriggers.push_back(pCreature->GetGUID());
                    break;
                case NPC_MALYGOS:
                    malygosGUID = pCreature->GetGUID();
                    break;
                case NPC_PORTAL_TRIGGER:
                    portalTriggers.push_back(pCreature->GetGUID());
                    break;
            }
        }

        void ProcessEvent(WorldObject* pWO, uint32 eventId)
        {
            if(eventId == EVENT_FOCUSING_IRIS)
            {
                if(GameObject* go = pWO->ToGameObject())
                    go->Delete(); // this is not the best way.

                if(Creature* malygos = instance->GetCreature(malygosGUID))
                    malygos->GetMotionMaster()->MovePoint(4, 770.10f, 1275.33f, 267.23f); // MOVE_INIT_PHASE_ONE

                if(GameObject* exitPortal = instance->GetGameObject(exitPortalGUID))
                    exitPortal->Delete();
            }
        }

        void VortexHandling()
        {
            if(Creature* malygos = instance->GetCreature(malygosGUID))
            {
                std::list<HostileReference*> m_threatlist = malygos->getThreatManager().getThreatList();
                for(std::list<uint64>::const_iterator itr_vortex = vortexTriggers.begin(); itr_vortex != vortexTriggers.end(); ++itr_vortex)
                {
                    if(m_threatlist.empty())
                        return;

                    uint8 counter = 0;
                    if(Creature* pTrigger_Creature = instance->GetCreature(*itr_vortex))
                    {
                        // each trigger have to cast the spell to 5 players.
                        for(std::list<HostileReference*>::const_iterator itr = m_threatlist.begin(); itr!= m_threatlist.end(); ++itr)
                        {
                            if(counter >= 5)
                                break;

                            if(Unit* pTarget = (*itr)->getTarget())
                            {
                                Player* pPlayer = pTarget->ToPlayer();

                                if(!pPlayer || pPlayer->isGameMaster() || pPlayer->HasAura(SPELL_VORTEX_4))
                                    continue;

                                pPlayer->CastSpell(pTrigger_Creature, SPELL_VORTEX_4, true);
                                counter++;
                            }
                        }
                    }
                }
            }
        }

        void PowerSparksHandling()
        {
            bool next =  (lastPortalGUID == portalTriggers.back() || !lastPortalGUID ? true : false);

            for(std::list<uint64>::const_iterator itr_trigger = portalTriggers.begin(); itr_trigger != portalTriggers.end(); ++itr_trigger)
            {
                if(next)
                {
                    if(Creature* trigger = instance->GetCreature(*itr_trigger))
                    {
                        lastPortalGUID = trigger->GetGUID();
                        trigger->CastSpell(trigger, SPELL_PORTAL_OPENED, true);
                        return;
                    }
                }

                if(*itr_trigger == lastPortalGUID)
                    next = true;
            }
        }

        void SetData(uint32 data, uint32 /*value*/)
        {
            switch(data)
            {
                case DATA_VORTEX_HANDLING:
                    VortexHandling();
                    break;
                case DATA_POWER_SPARKS_HANDLING:
                    PowerSparksHandling();
                    break;
            }
        }

        uint64 GetData64(uint32 data)
        {
            switch(data)
            {
                case DATA_TRIGGER:
                    return vortexTriggers.front();
                case DATA_MALYGOS:
                    return malygosGUID;
                case DATA_PLATFORM:
                    return platformGUID;
            }

            return 0;
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << "E E " << GetBossSaveData();

            OUT_SAVE_INST_DATA_COMPLETE;
            return saveStream.str();
        }

        void Load(const char* str)
        {
            if(!str)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(str);

            char dataHead1, dataHead2;

            std::istringstream loadStream(str);
            loadStream >> dataHead1 >> dataHead2;

            if(dataHead1 == 'E' && dataHead2 == 'E')
            {
                for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                {
                    uint32 tmpState;
                    loadStream >> tmpState;
                    if(tmpState == IN_PROGRESS || tmpState > SPECIAL)
                        tmpState = NOT_STARTED;
                    SetBossState(i, EncounterState(tmpState));
                }

            } else OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }

        private:
            std::list<uint64> vortexTriggers;
            std::list<uint64> portalTriggers;
            uint64 malygosGUID;
            uint64 lastPortalGUID;
            uint64 platformGUID;
            uint64 exitPortalGUID;
            uint64 chestGUID;
            Position focusingIrisPosition;
            Position exitPortalPosition;
    };
};

void AddSC_instance_eye_of_eternity()
{
   new instance_eye_of_eternity();
}

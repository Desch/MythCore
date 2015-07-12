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

#include "ScriptPCH.h"
#include "oculus.h"

#define MAX_ENCOUNTER 4

class instance_oculus : public InstanceMapScript
{
public:
    instance_oculus() : InstanceMapScript("instance_oculus", 578) { }

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new instance_oculus_InstanceMapScript(pMap);
    }

    struct instance_oculus_InstanceMapScript : public InstanceScript
    {
        instance_oculus_InstanceMapScript(Map* pMap) : InstanceScript(pMap) { }

        void Initialize()
        {
            SetBossNumber(MAX_ENCOUNTER);

            drakosGUID      = 0;
            varosGUID       = 0;
            uromGUID        = 0;
            eregosGUID      = 0;
            eregosCacheGUID = 0;
            Spotlight       = 0;
            platformUrom    = 0;
            centrifugueConstructCounter = 0;

            azureDragonsList.clear();
            gameObjectList.clear();
        }

        void OnCreatureDeath(Creature* creature)
        {
            if(creature->GetEntry() != NPC_CENTRIFUGE_CONSTRUCT)
                return;

             DoUpdateWorldState(WORLD_STATE_CENTRIFUGE_CONSTRUCT_AMOUNT, --centrifugueConstructCounter);

             if(!centrifugueConstructCounter)
                if(Creature* varos = instance->GetCreature(varosGUID))
                    varos->RemoveAllAuras();
        }

        void OnPlayerEnter(Player* player)
        {
            if(GetBossState(DATA_DRAKOS_EVENT) == DONE && GetBossState(DATA_VAROS_EVENT) != DONE)
            {
                player->SendUpdateWorldState(WORLD_STATE_CENTRIFUGE_CONSTRUCT_SHOW, 1);
                player->SendUpdateWorldState(WORLD_STATE_CENTRIFUGE_CONSTRUCT_AMOUNT, centrifugueConstructCounter);
            } else {
                player->SendUpdateWorldState(WORLD_STATE_CENTRIFUGE_CONSTRUCT_SHOW, 0);
                player->SendUpdateWorldState(WORLD_STATE_CENTRIFUGE_CONSTRUCT_AMOUNT, 0);
            }
        }

        void ProcessEvent(WorldObject* /*unit*/, uint32 eventId)
        {
            if(eventId != EVENT_CALL_DRAGON)
                return;

            Creature* varos = instance->GetCreature(varosGUID);
            if(!varos)
                return;

            if(Creature* drake = varos->SummonCreature(NPC_AZURE_RING_GUARDIAN, varos->GetPositionX(), varos->GetPositionY(), varos->GetPositionZ()+40))
                drake->AI()->DoAction(ACTION_CALL_DRAGON_EVENT);
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch(creature->GetEntry())
            {
                case NPC_DRAKOS:
                    drakosGUID = creature->GetGUID();
                    break;
                case NPC_VAROS:
                    varosGUID = creature->GetGUID();
                    break;
                case NPC_UROM:
                    uromGUID = creature->GetGUID();
                    break;
                case NPC_EREGOS:
                    eregosGUID = creature->GetGUID();
                    break;
                case NPC_CENTRIFUGE_CONSTRUCT:
                    if(creature->isAlive())
                        DoUpdateWorldState(WORLD_STATE_CENTRIFUGE_CONSTRUCT_AMOUNT, ++centrifugueConstructCounter);
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* pGo)
        {
            switch(pGo->GetEntry())
            {
                case GO_EREGOS_CACHE_5N:
                    eregosCacheGUID = pGo->GetGUID();
                    break;
                case GO_EREGOS_CACHE_5H:
                    eregosCacheGUID = pGo->GetGUID();
                    break;
                case GO_SPOTLIGHT:
                    Spotlight = pGo->GetGUID();
                    break;
                case GO_DRAGON_CAGE_DOOR:
                    if(GetBossState(DATA_DRAKOS_EVENT) == DONE)
                        pGo->SetGoState(GO_STATE_ACTIVE);
                    else
                        pGo->SetGoState(GO_STATE_READY);
                    gameObjectList.push_back(pGo->GetGUID());
                    break;
            }
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if(!InstanceScript::SetBossState(type, state))
                return false;

            switch(type)
            {
                case DATA_DRAKOS_EVENT:
                    if(state == DONE)
                    {
                        DoUpdateWorldState(WORLD_STATE_CENTRIFUGE_CONSTRUCT_SHOW, 1);
                        DoUpdateWorldState(WORLD_STATE_CENTRIFUGE_CONSTRUCT_AMOUNT, centrifugueConstructCounter);
                        OpenCageDoors();
                    }
                    break;
                case DATA_VAROS_EVENT:
                    if(state == DONE)
                        DoUpdateWorldState(WORLD_STATE_CENTRIFUGE_CONSTRUCT_SHOW, 0);
                    break;
                case DATA_EREGOS_EVENT:
                    if(state == DONE)
                    {
                        if(GameObject* pChest = instance->GetGameObject(eregosCacheGUID))
                        {
                            if(pChest && !pChest->isSpawned())
                                pChest->SetRespawnTime(DAY);
                            else pChest->SetPhaseMask(1, true);
                        }
                    }
                    break;
            }

            return true;
        }

        void SetData(uint32 type, uint32 data)
        {
            switch(type)
            {
                case DATA_EREGOS_EVENT:
                    if(data == DONE)
                    {
                        if(GameObject* pChest = instance->GetGameObject(eregosCacheGUID))
                        {
                            if(pChest && !pChest->isSpawned())
                                pChest->SetRespawnTime(DAY);
                            else pChest->SetPhaseMask(1, true);
                        }
                    }
                    break;
                case DATA_UROM_PLATAFORM:
                    platformUrom = data;
                    break;
            }
        }

        uint32 GetData(uint32 type)
        {
            switch(type)
            {
                case DATA_UROM_PLATAFORM:              return platformUrom;
                case DATA_UROM_EVENT:                  return GetBossState(DATA_UROM_EVENT);
            }

            return 0;
        }

        uint64 GetData64(uint32 identifier)
        {
            switch(identifier)
            {
                case DATA_DRAKOS:                 return drakosGUID;
                case DATA_VAROS:                  return varosGUID;
                case DATA_UROM:                   return uromGUID;
                case DATA_EREGOS:                 return eregosGUID;
            }

            return 0;
        }

        void OpenCageDoors()
        {
            if(gameObjectList.empty())
                return;

            for(std::list<uint64>::const_iterator itr = gameObjectList.begin(); itr != gameObjectList.end(); ++itr)
            {
                if(GameObject* go = instance->GetGameObject(*itr))
                    go->SetGoState(GO_STATE_ACTIVE);
            }
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;
            std::ostringstream saveStream;
            saveStream << "T O " << GetBossSaveData();

            str_data = saveStream.str();
            OUT_SAVE_INST_DATA_COMPLETE;
            return str_data;
        }

        void Load(const char* in)
        {
            if(!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);
            char dataHead1, dataHead2;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2;

            if(dataHead1 == 'T' && dataHead2 == 'O')
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
        uint64 drakosGUID;
        uint64 varosGUID;
        uint64 uromGUID;
        uint64 eregosGUID;
        uint64 eregosCacheGUID;
        uint64 Spotlight;

        uint8 platformUrom;
        uint8 centrifugueConstructCounter;

        std::string str_data;
        std::list<uint64> gameObjectList;
        std::list<uint64> azureDragonsList;
    };
};

void AddSC_instance_oculus()
{
    new instance_oculus;
}
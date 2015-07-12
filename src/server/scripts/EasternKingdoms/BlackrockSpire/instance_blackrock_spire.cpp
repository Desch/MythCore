/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Copyright (C) 2012 SymphonyArt <http://symphonyart.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "blackrock_spire.h"

class instance_blackrock_spire : public InstanceMapScript
{
public:
    instance_blackrock_spire() : InstanceMapScript("instance_blackrock_spire", 229) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_blackrock_spireMapScript(map);
    }

    struct instance_blackrock_spireMapScript : public InstanceScript
    {
        instance_blackrock_spireMapScript(InstanceMap* map) : InstanceScript(map) { }

        uint32 Encounter[MAX_ENCOUNTER];
        std::string m_strInstData;
        uint64 HighlordOmokk;
        uint64 ShadowHunterVoshgajin;
        uint64 WarMasterVoone;
        uint64 MotherSmolderweb;
        uint64 UrokDoomhowl;
        uint64 QuartermasterZigris;
        uint64 GizrultheSlavener;
        uint64 Halycon;
        uint64 OverlordWyrmthalak;
        uint64 PyroguardEmberseer;
        uint64 WarchiefRendBlackhand;
        uint64 Gyth;
        uint64 TheBeast;
        uint64 GeneralDrakkisath;

        void Initialize()
        {
            SetBossNumber(MAX_ENCOUNTER);
            HighlordOmokk           = 0;
            ShadowHunterVoshgajin   = 0;
            WarMasterVoone          = 0;
            MotherSmolderweb        = 0;
            UrokDoomhowl            = 0;
            QuartermasterZigris     = 0;
            GizrultheSlavener       = 0;
            Halycon                 = 0;
            OverlordWyrmthalak      = 0;
            PyroguardEmberseer      = 0;
            WarchiefRendBlackhand   = 0;
            Gyth                    = 0;
            TheBeast                = 0;
            GeneralDrakkisath       = 0;
        }

        bool IsEncounterInProgress() const
        {
            for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            {
                if(Encounter[i] == IN_PROGRESS)
                    return true;
            }
            return false;
        }

        void OnCreatureCreate(Creature* pCreature)
        {
            switch(pCreature->GetEntry())
            {
                case NPC_OMOKK:
                    HighlordOmokk = pCreature->GetGUID();
                    break;
                case NPC_SHADOW_HUNTER_VOSHGAJIN:
                    ShadowHunterVoshgajin = pCreature->GetGUID();
                    break;
                case NPC_WARMASTER_VOONE:
                    WarMasterVoone = pCreature->GetGUID();
                    break;
                case NPC_MOTHER_SMOLDERWEB:
                    MotherSmolderweb = pCreature->GetGUID();
                    break;
                case NPC_UROK_DOOMHOWL:
                    UrokDoomhowl = pCreature->GetGUID();
                    break;
                case NPC_QUARTERMASTER_ZIGRIS:
                    QuartermasterZigris = pCreature->GetGUID();
                    break;
                case NPC_GIZRUL_THE_SLAVENER:
                    GizrultheSlavener = pCreature->GetGUID();
                    break;
                case NPC_HALYCON:
                    Halycon = pCreature->GetGUID();
                    break;
                case NPC_OVERLORD_WYRMTHALAK:
                    OverlordWyrmthalak = pCreature->GetGUID();
                    break;
                case NPC_PYROGAURD_EMBERSEER:
                    PyroguardEmberseer = pCreature->GetGUID();
                    break;
                case NPC_WARCHIEF_REND_BLACKHAND:
                    WarchiefRendBlackhand = pCreature->GetGUID();
                    break;
                case NPC_GYTH:
                    Gyth = pCreature->GetGUID();
                    break;
                case NPC_THE_BEAST:
                    TheBeast = pCreature->GetGUID();
                    break;
                case NPC_GENERAL_DRAKKISATH:
                    GeneralDrakkisath = pCreature->GetGUID();
                    break;
            }
         }

        void OnGameObjectCreate(GameObject* go)
        {
            switch(go->GetEntry())
            {
                case GO_WHELP_SPAWNER:
                    go->CastSpell(NULL, SPELL_SUMMON_ROOKERY_WHELP);
                    break;
            }
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if(!InstanceScript::SetBossState(type, state))
                return false;

            switch(type)
            {
                case DATA_OMOKK:
                case DATA_SHADOW_HUNTER_VOSHGAJIN:
                case DATA_WARMASTER_VOONE:
                case DATA_MOTHER_SMOLDERWEB:
                case DATA_UROK_DOOMHOWL:
                case DATA_QUARTERMASTER_ZIGRIS:
                case DATA_GIZRUL_THE_SLAVENER:
                case DATA_HALYCON:
                case DATA_OVERLORD_WYRMTHALAK:
                case DATA_PYROGAURD_EMBERSEER:
                case DATA_WARCHIEF_REND_BLACKHAND:
                case DATA_GYTH:
                case DATA_THE_BEAST:
                case DATA_GENERAL_DRAKKISATH:
                    break;
                default:
                    break;
            }

             return true;
        }

        uint64 GetData64(uint32 type)
        {
            switch(type)
            {
                case DATA_OMOKK:
                    return HighlordOmokk;
                case DATA_SHADOW_HUNTER_VOSHGAJIN:
                    return ShadowHunterVoshgajin;
                case DATA_WARMASTER_VOONE:
                    return WarMasterVoone;
                case DATA_MOTHER_SMOLDERWEB:
                    return MotherSmolderweb;
                case DATA_UROK_DOOMHOWL:
                    return UrokDoomhowl;
                case DATA_QUARTERMASTER_ZIGRIS:
                    return QuartermasterZigris;
                case DATA_GIZRUL_THE_SLAVENER:
                    return GizrultheSlavener;
                case DATA_HALYCON:
                    return Halycon;
                case DATA_OVERLORD_WYRMTHALAK:
                    return OverlordWyrmthalak;
                case DATA_PYROGAURD_EMBERSEER:
                    return PyroguardEmberseer;
                case DATA_WARCHIEF_REND_BLACKHAND:
                    return WarchiefRendBlackhand;
                case DATA_GYTH:
                    return Gyth;
                case DATA_THE_BEAST:
                    return TheBeast;
                case DATA_GENERAL_DRAKKISATH:
                    return GeneralDrakkisath;
            }

            return 0;
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << "B S " << GetBossSaveData();

            OUT_SAVE_INST_DATA_COMPLETE;
            return saveStream.str();
        }

        void Load(const char* strIn)
        {
            if(!strIn)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(strIn);

            char dataHead1, dataHead2;

            std::istringstream loadStream(strIn);
            loadStream >> dataHead1 >> dataHead2;

            if(dataHead1 == 'B' && dataHead2 == 'S')
            {
                for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                {
                    uint32 tmpState;
                    loadStream >> tmpState;
                    if(tmpState == IN_PROGRESS || tmpState > SPECIAL)
                        tmpState = NOT_STARTED;
                }
            }

            OUT_LOAD_INST_DATA_COMPLETE;
        }
    };
};

void AddSC_instance_blackrock_spire()
{
    new instance_blackrock_spire;
}

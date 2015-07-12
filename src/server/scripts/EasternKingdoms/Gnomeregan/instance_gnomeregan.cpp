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
#include "gnomeregan.h"

#define    MAX_ENCOUNTER  1

class instance_gnomeregan : public InstanceMapScript
{
public:
    instance_gnomeregan() : InstanceMapScript("instance_gnomeregan", 90) { }

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new instance_gnomeregan_InstanceMapScript(pMap);
    }

    struct instance_gnomeregan_InstanceMapScript : public InstanceScript
    {
        instance_gnomeregan_InstanceMapScript(Map* pMap) : InstanceScript(pMap) { }

        uint32 m_auiEncounter[MAX_ENCOUNTER];

        uint64 uiCaveInLeftGUID;
        uint64 uiCaveInRightGUID;

        uint64 uiBastmasterEmiShortfuseGUID;

        void Initialize()
        {
            memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

            uiCaveInLeftGUID                = 0;
            uiCaveInRightGUID               = 0;

            uiBastmasterEmiShortfuseGUID    = 0;
        }

        void Load(const char* in)
        {
            if(!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);

            std::istringstream loadStream(in);
            loadStream >> m_auiEncounter[0];

            for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            {
                if(m_auiEncounter[i] == IN_PROGRESS)
                    m_auiEncounter[i] = NOT_STARTED;
            }

            OUT_LOAD_INST_DATA_COMPLETE;
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch(creature->GetEntry())
            {
                case NPC_BLASTMASTER_EMI_SHORTFUSE: uiBastmasterEmiShortfuseGUID = creature->GetGUID(); break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch(go->GetEntry())
            {
                case GO_CAVE_IN_LEFT:
                    uiCaveInLeftGUID = go->GetGUID();
                    if(m_auiEncounter[0] == DONE || m_auiEncounter[0] == NOT_STARTED)
                        HandleGameObject(0, false, go);
                    break;
                case GO_CAVE_IN_RIGHT:
                    uiCaveInRightGUID = go->GetGUID();
                    if(m_auiEncounter[0] == DONE || m_auiEncounter[0] == NOT_STARTED)
                        HandleGameObject(0, false, go);
                    break;
            }
        }

        void SetData(uint32 uiType, uint32 uiData)
        {
            switch(uiType)
            {
                case TYPE_EVENT:
                    m_auiEncounter[0] = uiData;
                    if(uiData == DONE)
                        SaveToDB();
                    break;
            }
        }

        uint32 GetData(uint32 uiType)
        {
            switch(uiType)
            {
                case TYPE_EVENT:    return m_auiEncounter[0];
            }
            return 0;
        }

        uint64 GetData64(uint32 uiType)
        {
            switch(uiType)
            {
                case DATA_GO_CAVE_IN_LEFT:              return uiCaveInLeftGUID;
                case DATA_GO_CAVE_IN_RIGHT:             return uiCaveInRightGUID;
                case DATA_NPC_BASTMASTER_EMI_SHORTFUSE: return uiBastmasterEmiShortfuseGUID;
            }

            return 0;
        }
    };
};

void AddSC_instance_gnomeregan()
{
    new instance_gnomeregan;
}

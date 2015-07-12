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
#include "obsidian_sanctum.h"

#define MAX_ENCOUNTER     4

class instance_obsidian_sanctum : public InstanceMapScript
{
public:
    instance_obsidian_sanctum() : InstanceMapScript("instance_obsidian_sanctum", 615) { }

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new instance_obsidian_sanctum_InstanceMapScript(pMap);
    }

    struct instance_obsidian_sanctum_InstanceMapScript : public InstanceScript
    {
        instance_obsidian_sanctum_InstanceMapScript(Map* pMap) : InstanceScript(pMap) { }

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        uint64 m_uiSartharionGUID;
        uint64 m_uiTenebronGUID;
        uint64 m_uiShadronGUID;
        uint64 m_uiVesperonGUID;

        bool m_bTenebronKilled;
        bool m_bShadronKilled;
        bool m_bVesperonKilled;

        void Initialize()
        {
            memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

            m_uiSartharionGUID = 0;
            m_uiTenebronGUID   = 0;
            m_uiShadronGUID    = 0;
            m_uiVesperonGUID   = 0;

            m_bTenebronKilled = false;
            m_bShadronKilled = false;
            m_bVesperonKilled = false;
        }

        bool IsEncounterInProgress() const
        {
            for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if(m_auiEncounter[i] == IN_PROGRESS)
                    return true;

            return false;
        }

        void OnCreatureCreate(Creature* pCreature)
        {
            switch(pCreature->GetEntry())
            {
                case NPC_SARTHARION:
                    m_uiSartharionGUID = pCreature->GetGUID();
                    break;
                //three dragons below set to active state once created.
                //we must expect bigger raid to encounter main boss, and then three dragons must be active due to grid differences
                case NPC_TENEBRON:
                    m_uiTenebronGUID = pCreature->GetGUID();
                    pCreature->setActive(true);
                    break;
                case NPC_SHADRON:
                    m_uiShadronGUID = pCreature->GetGUID();
                    pCreature->setActive(true);
                    break;
                case NPC_VESPERON:
                    m_uiVesperonGUID = pCreature->GetGUID();
                    pCreature->setActive(true);
                    break;
                default:
                    break;
            }
        }

        void SetData(uint32 uiType, uint32 uiData)
        {
            switch(uiType)
            {
                case TYPE_SARTHARION_EVENT:
                    if(m_auiEncounter[0] != DONE)
                    m_auiEncounter[0] = uiData;
                    break;
                case TYPE_TENEBRON_PREKILLED:
                    m_auiEncounter[1] = DONE;
                    m_bTenebronKilled = true;
                    break;
                case TYPE_SHADRON_PREKILLED:
                    m_auiEncounter[2] = DONE;
                    m_bShadronKilled = true;
                    break;
                case TYPE_VESPERON_PREKILLED:
                    m_auiEncounter[3] = DONE;
                    m_bVesperonKilled = true;
                    break;
                default:
                    break;
            }

            if(uiData == DONE)
                SaveToDB();
        }

        uint32 GetData(uint32 uiType)
        {
            switch(uiType)
            {
                case TYPE_SARTHARION_EVENT:
                    return m_auiEncounter[0];
                case TYPE_TENEBRON_PREKILLED:
                    return m_bTenebronKilled && m_auiEncounter[1] == DONE;
                case TYPE_SHADRON_PREKILLED:
                    return m_bShadronKilled && m_auiEncounter[2] == DONE;
                case TYPE_VESPERON_PREKILLED:
                    return m_bVesperonKilled && m_auiEncounter[3] == DONE;
                default:
                    break;
            }
            return 0;
        }

        uint64 GetData64(uint32 uiData)
        {
            switch(uiData)
            {
                case DATA_SARTHARION:
                    return m_uiSartharionGUID;
                case DATA_TENEBRON:
                    return m_uiTenebronGUID;
                case DATA_SHADRON:
                    return m_uiShadronGUID;
                case DATA_VESPERON:
                    return m_uiVesperonGUID;
                default:
                    break;
            }
            return 0;
        }

        std::string GetSaveData()
        {
            std::ostringstream saveStream;
            saveStream << "O S ";
            for(int i = 0; i < MAX_ENCOUNTER; ++i)
                saveStream << m_auiEncounter[i] << " ";

            return saveStream.str();
        }

        void Load(const char * data)
        {
            std::istringstream loadStream(data);
            char dataHead1, dataHead2;
            loadStream >> dataHead1 >> dataHead2;
            std::string newdata = loadStream.str();

            uint32 buff;
            if(dataHead1 == 'O' && dataHead2 == 'S')
            {
                for(int i = 0; i < MAX_ENCOUNTER; ++i)
                {
                    loadStream >> buff;
                    m_auiEncounter[i]= buff;
                }
            }

            m_bTenebronKilled = (m_auiEncounter[1] == DONE);
            m_bShadronKilled = (m_auiEncounter[2] == DONE);
            m_bVesperonKilled = (m_auiEncounter[3] == DONE);

            for(int i = 0; i < MAX_ENCOUNTER; ++i)
                if(m_auiEncounter[i] != DONE)
                    m_auiEncounter[i] = NOT_STARTED;
        }
    };
};

void AddSC_instance_obsidian_sanctum()
{
    new instance_obsidian_sanctum;
}

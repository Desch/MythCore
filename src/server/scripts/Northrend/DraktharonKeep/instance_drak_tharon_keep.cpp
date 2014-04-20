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
#include "drak_tharon_keep.h"

#define MAX_ENCOUNTER     4

/* Drak'Tharon Keep encounters:
0 - Trollgore
1 - Novos
2 - King Dred
3 - Tharon Ja
*/

enum Creatures
{
    NPC_TROLLGORE                                 = 26630,
    NPC_NOVOS                                     = 26631,
    NPC_KING_DRED                                 = 27483,
    NPC_THARON_JA                                 = 26632
};

enum GameObjects
{
    GO_NOVOS_CRYSTAL_1                            = 189299,
    GO_NOVOS_CRYSTAL_2                            = 189300,
    GO_NOVOS_CRYSTAL_3                            = 189301,
    GO_NOVOS_CRYSTAL_4                            = 189302
};

class instance_drak_tharon : public InstanceMapScript
{
public:
    instance_drak_tharon() : InstanceMapScript("instance_drak_tharon", 600) { }

    struct instance_drak_tharon_InstanceScript : public InstanceScript
    {
        instance_drak_tharon_InstanceScript(Map* pMap) : InstanceScript(pMap) { }

        uint8 uiDredAchievCounter;

        uint64 uiTrollgore;
        uint64 uiNovos;
        uint64 uiDred;
        uint64 uiTharonJa;

        uint64 uiNovosCrystal1;
        uint64 uiNovosCrystal2;
        uint64 uiNovosCrystal3;
        uint64 uiNovosCrystal4;

        uint16 m_auiEncounter[MAX_ENCOUNTER];

        std::string str_data;

        void Initialize()
        {
            uiTrollgore = 0;
            uiNovos = 0;
            uiDred = 0;
            uiTharonJa = 0;
            uiNovosCrystal1 = 0;
            uiNovosCrystal2 = 0;
            uiNovosCrystal3 = 0;
            uiNovosCrystal4 = 0;
            uiDredAchievCounter = 0;
        }

        bool IsEncounterInProgress() const
        {
            for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if(m_auiEncounter[i] == IN_PROGRESS) return true;

            return false;
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch(go->GetEntry())
            {
                case GO_NOVOS_CRYSTAL_1:
                    uiNovosCrystal1 = go->GetGUID();
                    break;
                case GO_NOVOS_CRYSTAL_2:
                    uiNovosCrystal2 = go->GetGUID();
                    break;
                case GO_NOVOS_CRYSTAL_3:
                    uiNovosCrystal3 = go->GetGUID();
                    break;
                case GO_NOVOS_CRYSTAL_4:
                    uiNovosCrystal4 = go->GetGUID();
                    break;
            }
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch(creature->GetEntry())
            {
                case NPC_TROLLGORE:
                    uiTrollgore = creature->GetGUID();
                    break;
                case NPC_NOVOS:
                    uiNovos = creature->GetGUID();
                    break;
                case NPC_KING_DRED:
                    uiDred = creature->GetGUID();
                    break;
                case NPC_THARON_JA:
                    uiTharonJa = creature->GetGUID();
                    break;
            }
        }

        uint64 GetData64(uint32 identifier)
        {
            switch(identifier)
            {
                case DATA_TROLLGORE:          return uiTrollgore;
                case DATA_NOVOS:              return uiNovos;
                case DATA_DRED:               return uiDred;
                case DATA_THARON_JA:          return uiTharonJa;
                case DATA_NOVOS_CRYSTAL_1:    return uiNovosCrystal1;
                case DATA_NOVOS_CRYSTAL_2:    return uiNovosCrystal2;
                case DATA_NOVOS_CRYSTAL_3:    return uiNovosCrystal3;
                case DATA_NOVOS_CRYSTAL_4:    return uiNovosCrystal4;
            }

            return 0;
        }

        void SetData(uint32 type, uint32 data)
        {
            switch(type)
            {
                case DATA_TROLLGORE_EVENT:
                    m_auiEncounter[0] = data;
                    break;
                case DATA_NOVOS_EVENT:
                    m_auiEncounter[1] = data;
                    break;
                case DATA_DRED_EVENT:
                    m_auiEncounter[2] = data;
                    break;
                case DATA_THARON_JA_EVENT:
                    m_auiEncounter[3] = data;
                    break;
                case DATA_KING_DRED_ACHIEV:
                    uiDredAchievCounter = data;
                    break;
            }

            if(data == DONE)
            {
                SaveToDB();
            }
        }

        uint32 GetData(uint32 type)
        {
            switch(type)
            {
                case DATA_TROLLGORE_EVENT:    return m_auiEncounter[0];
                case DATA_NOVOS_EVENT:        return m_auiEncounter[1];
                case DATA_DRED_EVENT:         return m_auiEncounter[2];
                case DATA_THARON_JA_EVENT:    return m_auiEncounter[3];
                case DATA_KING_DRED_ACHIEV:   return uiDredAchievCounter;
            }
            return 0;
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::string str_data;

            std::ostringstream saveStream;
            saveStream << "D K " << m_auiEncounter[0] << " " << m_auiEncounter[1] << " "
                << m_auiEncounter[2] << " " << m_auiEncounter[3];

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
            uint16 data0, data1, data2, data3;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2 >> data3;

            if(dataHead1 == 'D' && dataHead2 == 'K')
            {
                m_auiEncounter[0] = data0;
                m_auiEncounter[1] = data1;
                m_auiEncounter[2] = data2;
                m_auiEncounter[3] = data3;

                for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if(m_auiEncounter[i] == IN_PROGRESS)
                        m_auiEncounter[i] = NOT_STARTED;
            } else OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }
    };

    InstanceScript* GetInstanceScript(InstanceMap *map) const
    {
        return new instance_drak_tharon_InstanceScript(map);
    }
};

void AddSC_instance_drak_tharon()
{
    new instance_drak_tharon;
}

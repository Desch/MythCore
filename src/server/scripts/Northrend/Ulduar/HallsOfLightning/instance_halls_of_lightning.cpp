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
#include "halls_of_lightning.h"

class instance_halls_of_lightning : public InstanceMapScript
{
public:
    instance_halls_of_lightning() : InstanceMapScript("instance_halls_of_lightning", 602) { }

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new instance_halls_of_lightning_InstanceMapScript(pMap);
    }

    struct instance_halls_of_lightning_InstanceMapScript : public InstanceScript
    {
        instance_halls_of_lightning_InstanceMapScript(Map* pMap) : InstanceScript(pMap) { }

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        uint64 m_uiGeneralBjarngrimGUID;
        uint64 m_uiIonarGUID;
        uint64 m_uiLokenGUID;
        uint64 m_uiVolkhanGUID;
        uint64 m_uiVolkhanAnvilGUID;
        uint64 m_uiBjarngrimDoorGUID;
        uint64 m_uiVolkhanDoorGUID;
        uint64 m_uiIonarDoorGUID;
        uint64 m_uiLokenDoorGUID;
        uint64 m_uiLokenGlobeGUID;

        void Initialize()
        {
            memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

            m_uiGeneralBjarngrimGUID = 0;
            m_uiVolkhanGUID          = 0;
            m_uiIonarGUID            = 0;
            m_uiLokenGUID            = 0;
            m_uiVolkhanAnvilGUID     = 0;
            m_uiBjarngrimDoorGUID    = 0;
            m_uiVolkhanDoorGUID      = 0;
            m_uiIonarDoorGUID        = 0;
            m_uiLokenDoorGUID        = 0;
            m_uiLokenGlobeGUID       = 0;
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch(creature->GetEntry())
            {
                case NPC_BJARNGRIM:
                    m_uiGeneralBjarngrimGUID = creature->GetGUID();
                    break;
                case NPC_VOLKHAN:
                    m_uiVolkhanGUID = creature->GetGUID();
                    break;
                case NPC_IONAR:
                    m_uiIonarGUID = creature->GetGUID();
                    break;
                case NPC_LOKEN:
                    m_uiLokenGUID = creature->GetGUID();
                    break;
                case NPC_VOLKHAN_ANVIL:
                    m_uiVolkhanAnvilGUID = creature->GetGUID();
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch(go->GetEntry())
            {
                case GO_BJARNGRIM_DOOR:
                    m_uiBjarngrimDoorGUID = go->GetGUID();
                    if(m_auiEncounter[0] == DONE)
                        go->SetGoState(GO_STATE_ACTIVE);
                    else
                        go->SetGoState(GO_STATE_READY);
                    break;
                case GO_VOLKHAN_DOOR:
                    m_uiVolkhanDoorGUID = go->GetGUID();
                    if(m_auiEncounter[1] == DONE)
                        go->SetGoState(GO_STATE_ACTIVE);
                    else
                        go->SetGoState(GO_STATE_READY);
                    break;
                case GO_IONAR_DOOR:
                    m_uiIonarDoorGUID = go->GetGUID();
                    if(m_auiEncounter[2] == DONE)
                        go->SetGoState(GO_STATE_ACTIVE);
                    else
                        go->SetGoState(GO_STATE_READY);
                    break;
                case GO_LOKEN_DOOR:
                    m_uiLokenDoorGUID = go->GetGUID();
                    if(m_auiEncounter[3] == DONE)
                        go->SetGoState(GO_STATE_ACTIVE);
                    else
                        go->SetGoState(GO_STATE_READY);
                    break;
                case GO_LOKEN_THRONE:
                    m_uiLokenGlobeGUID = go->GetGUID();
                    break;
            }
        }

        void SetData(uint32 uiType, uint32 uiData)
        {
            switch(uiType)
            {
                case TYPE_BJARNGRIM:
                    if(uiData == DONE)
                    {
                        if(GameObject* pDoor = instance->GetGameObject(m_uiBjarngrimDoorGUID))
                            pDoor->SetGoState(GO_STATE_ACTIVE);
                        GameObject* pGo = instance->GetGameObject(m_uiBjarngrimDoorGUID);
                        pGo->SetGoState(GO_STATE_ACTIVE);
                    }
                    m_auiEncounter[0] = uiData;
                    break;
                case TYPE_VOLKHAN:
                    if(uiData == DONE)
                        if(GameObject* pDoor = instance->GetGameObject(m_uiVolkhanDoorGUID))
                            pDoor->SetGoState(GO_STATE_ACTIVE);
                    m_auiEncounter[1] = uiData;
                    break;
                case TYPE_IONAR:
                    if(uiData == DONE)
                        if(GameObject* pDoor = instance->GetGameObject(m_uiIonarDoorGUID))
                            pDoor->SetGoState(GO_STATE_ACTIVE);
                    m_auiEncounter[2] = uiData;
                    break;
                case TYPE_LOKEN:
                    if(uiData == DONE)
                    {
                        DoUseDoorOrButton(m_uiLokenDoorGUID);
                        if(GameObject* pGlobe = instance->GetGameObject(m_uiLokenGlobeGUID))
                            pGlobe->SetGoState(GO_STATE_ACTIVE);
                    }
                    m_auiEncounter[3] = uiData;
                    break;
            }

            if(uiData == DONE)
                SaveToDB();
        }

        uint32 GetData(uint32 uiType)
        {
            switch(uiType)
            {
                case TYPE_BJARNGRIM:
                    return m_auiEncounter[0];
                case TYPE_VOLKHAN:
                    return m_auiEncounter[1];
                case TYPE_IONAR:
                    return m_auiEncounter[2];
                case TYPE_LOKEN:
                    return m_auiEncounter[3];
            }
            return 0;
        }

        uint64 GetData64(uint32 uiData)
        {
            switch(uiData)
            {
                case DATA_BJARNGRIM:
                    return m_uiGeneralBjarngrimGUID;
                case DATA_VOLKHAN:
                    return m_uiVolkhanGUID;
                case DATA_IONAR:
                    return m_uiIonarGUID;
                case DATA_LOKEN:
                    return m_uiLokenGUID;
                case DATA_VOLKHAN_ANVIL:
                    return m_uiVolkhanAnvilGUID;
            }
            return 0;
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;
            std::ostringstream saveStream;
            saveStream << "H L " << m_auiEncounter[0] << " " << m_auiEncounter[1] << " "
            << m_auiEncounter[2] << " " << m_auiEncounter[3];

            OUT_SAVE_INST_DATA_COMPLETE;
            return saveStream.str();
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

            if(dataHead1 == 'H' && dataHead2 == 'L')
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
};

void AddSC_instance_halls_of_lightning()
{
    new instance_halls_of_lightning;
}
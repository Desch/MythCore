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
#include "scarlet_monastery.h"

#define ENTRY_PUMPKIN_SHRINE    186267
#define ENTRY_HORSEMAN          23682
#define ENTRY_HEAD              23775
#define ENTRY_PUMPKIN           23694

#define MAX_ENCOUNTER 2

class instance_scarlet_monastery : public InstanceMapScript
{
public:
    instance_scarlet_monastery() : InstanceMapScript("instance_scarlet_monastery", 189) { }

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new instance_scarlet_monastery_InstanceMapScript(pMap);
    }

    struct instance_scarlet_monastery_InstanceMapScript : public InstanceScript
    {
        instance_scarlet_monastery_InstanceMapScript(Map* pMap) : InstanceScript(pMap) { }

        uint64 PumpkinShrineGUID;
        uint64 HorsemanGUID;
        uint64 HeadGUID;
        std::set<uint64> HorsemanAdds;

        uint64 MograineGUID;
        uint64 WhitemaneGUID;
        uint64 VorrelGUID;
        uint64 DoorHighInquisitorGUID;

        uint32 m_auiEncounter[MAX_ENCOUNTER];

        void Initialize()
        {
            memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

            PumpkinShrineGUID  = 0;
            HorsemanGUID = 0;
            HeadGUID = 0;
            HorsemanAdds.clear();

            MograineGUID = 0;
            WhitemaneGUID = 0;
            VorrelGUID = 0;
            DoorHighInquisitorGUID = 0;
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch(go->GetEntry())
            {
            case ENTRY_PUMPKIN_SHRINE: PumpkinShrineGUID = go->GetGUID();break;
            case 104600: DoorHighInquisitorGUID = go->GetGUID(); break;
            }
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch(creature->GetEntry())
            {
                case ENTRY_HORSEMAN:    HorsemanGUID = creature->GetGUID(); break;
                case ENTRY_HEAD:        HeadGUID = creature->GetGUID(); break;
                case ENTRY_PUMPKIN:     HorsemanAdds.insert(creature->GetGUID());break;
                case 3976: MograineGUID = creature->GetGUID(); break;
                case 3977: WhitemaneGUID = creature->GetGUID(); break;
                case 3981: VorrelGUID = creature->GetGUID(); break;
            }
        }

        void SetData(uint32 type, uint32 data)
        {
            switch(type)
            {
            case TYPE_MOGRAINE_AND_WHITE_EVENT:
                if(data == IN_PROGRESS)
                    DoUseDoorOrButton(DoorHighInquisitorGUID);
                if(data == FAIL)
                    DoUseDoorOrButton(DoorHighInquisitorGUID);

                m_auiEncounter[0] = data;
                break;
            case GAMEOBJECT_PUMPKIN_SHRINE:
                HandleGameObject(PumpkinShrineGUID, false);
                break;
            case DATA_HORSEMAN_EVENT:
                m_auiEncounter[1] = data;
                if(data == DONE)
                {
                    for(std::set<uint64>::const_iterator itr = HorsemanAdds.begin(); itr != HorsemanAdds.end(); ++itr)
                    {
                        Creature* add = instance->GetCreature(*itr);
                        if(add && add->isAlive())
                            add->Kill(add);
                    }
                    HorsemanAdds.clear();
                    HandleGameObject(PumpkinShrineGUID, false);
                }
                break;
            }
        }

        uint64 GetData64(uint32 type)
        {
            switch(type)
            {
                //case GAMEOBJECT_PUMPKIN_SHRINE:   return PumpkinShrineGUID;
                //case DATA_HORSEMAN:               return HorsemanGUID;
                //case DATA_HEAD:                   return HeadGUID;
                case DATA_MOGRAINE:             return MograineGUID;
                case DATA_WHITEMANE:            return WhitemaneGUID;
                case DATA_VORREL:               return VorrelGUID;
                case DATA_DOOR_WHITEMANE:       return DoorHighInquisitorGUID;
            }
            return 0;
        }

        uint32 GetData(uint32 type)
        {
            if(type == TYPE_MOGRAINE_AND_WHITE_EVENT)
                return m_auiEncounter[0];
            if(type == DATA_HORSEMAN_EVENT)
                return m_auiEncounter[1];
            return 0;
        }
    };
};

void AddSC_instance_scarlet_monastery()
{
    new instance_scarlet_monastery;
}

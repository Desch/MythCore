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
#include "forge_of_souls.h"

#define MAX_ENCOUNTER 2

class instance_forge_of_souls : public InstanceMapScript
{
    public:
        instance_forge_of_souls() : InstanceMapScript(FoSScriptName, 632) { }

        struct instance_forge_of_souls_InstanceScript : public InstanceScript
        {
            instance_forge_of_souls_InstanceScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(MAX_ENCOUNTER);
                bronjahm = 0;
                devourerOfSouls = 0;

                teamInInstance = 0;
            }

            void OnCreatureCreate(Creature* creature)
            {
                Map::PlayerList const &players = instance->GetPlayers();
                if(!players.isEmpty())
                    if(Player* player = players.begin()->getSource())
                        teamInInstance = player->GetTeam();

                switch(creature->GetEntry())
                {
                    case CREATURE_BRONJAHM:
                        bronjahm = creature->GetGUID();
                        break;
                    case CREATURE_DEVOURER:
                        devourerOfSouls = creature->GetGUID();
                        break;
                    case NPC_SYLVANAS_PART1:
                        if(teamInInstance == ALLIANCE)
                            creature->UpdateEntry(NPC_JAINA_PART1, ALLIANCE);
                        break;
                    case NPC_LORALEN:
                        if(teamInInstance == ALLIANCE)
                            creature->UpdateEntry(NPC_ELANDRA, ALLIANCE);
                        break;
                    case NPC_KALIRA:
                        if(teamInInstance == ALLIANCE)
                            creature->UpdateEntry(NPC_KORELN, ALLIANCE);
                        break;
                }
            }

            uint32 GetData(uint32 type)
            {
                switch(type)
                {
                    case DATA_TEAM_IN_INSTANCE:
                        return teamInInstance;
                    default:
                        break;
                }

                return 0;
            }

            uint64 GetData64(uint32 type)
            {
                switch(type)
                {
                    case DATA_BRONJAHM:
                        return bronjahm;
                    case DATA_DEVOURER:
                        return devourerOfSouls;
                    default:
                        break;
                }

                return 0;
            }

            std::string GetSaveData()
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "F S " << GetBossSaveData();

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

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2;

                if(dataHead1 == 'F' && dataHead2 == 'S')
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
            uint64 bronjahm;
            uint64 devourerOfSouls;

            uint32 teamInInstance;
        };

        InstanceScript* GetInstanceScript(InstanceMap *map) const
        {
            return new instance_forge_of_souls_InstanceScript(map);
        }
};

void AddSC_instance_forge_of_souls()
{
    new instance_forge_of_souls;
}

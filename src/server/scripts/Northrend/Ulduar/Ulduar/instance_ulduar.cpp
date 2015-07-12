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
#include "InstanceScript.h"
#include "ulduar.h"

static DoorData const doorData[] =
{
    { GO_LEVIATHAN_DOOR, BOSS_LEVIATHAN, DOOR_TYPE_ROOM, BOUNDARY_S    },
    { 0,                 0,              DOOR_TYPE_ROOM, BOUNDARY_NONE },
};

class instance_ulduar : public InstanceMapScript
{
    public:
        instance_ulduar() : InstanceMapScript("instance_ulduar", 603) { }

        struct instance_ulduar_InstanceMapScript : public InstanceScript
        {
            instance_ulduar_InstanceMapScript(InstanceMap* map) : InstanceScript(map) { }

            uint32 Encounter[MAX_ENCOUNTER];

            // Creatures
            uint64 LeviathanGUID;
            uint64 IgnisGUID;
            uint64 RazorscaleGUID;
            uint64 RazorscaleController;
            uint64 RazorHarpoonGUIDs[4];
            uint64 ExpeditionCommanderGUID;
            uint64 XT002GUID;
            uint64 XTToyPileGUIDs[4];
            uint64 AssemblyGUIDs[3];
            uint64 KologarnGUID;
            uint64 LeftArmGUID;
            uint64 RightArmGUID;
            uint64 AuriayaGUID;

            // Mimiron
            uint64 MimironGUID;
            uint64 LeviathanMKIIGUID;
            uint64 VX001GUID;
            uint64 AerialUnitGUID;
            uint64 MagneticCoreGUID;
            uint64 MimironElevatorGUID;
            std::list<uint64> MimironDoorGUIDList;

            // Thorim
            uint64 ThorimGUID;
            uint64 ThorimDoorGUID;
            uint64 RunicColossusGUID;
            uint64 RuneGiantGUID;
            uint64 RunicDoorGUID;
            uint64 StoneDoorGUID;
            uint64 ThorimChestGUID;

            uint64 HodirGUID;
            uint64 FreyaGUID;
            uint64 KeeperGUIDs[3];
            uint64 VezaxGUID;
            uint64 YoggSaronGUID;
            uint64 AlgalonGUID;
            uint64 LeviathanGateGUID;
            uint64 VezaxDoorGUID;

            // GameObjects
            uint64 KologarnChestGUID;
            uint64 KologarnBridgeGUID;
            uint64 KologarnDoorGUID;
            uint64 HodirRareCacheGUID;
            uint64 HodirChestGUID;
            uint64 FreyaChestGUID;
            uint64 HodirDoorGUID;
            uint64 HodirIceDoorGUID;
            uint64 ArchivumDoorGUID;

            // Miscellaneous
            uint32 TeamInInstance;
            uint32 HodirRareCacheData;
            uint8 elderCount;
            bool conSpeedAtory;

            std::set<uint64> mRubbleSpawns;

            void Initialize()
            {
                SetBossNumber(MAX_ENCOUNTER);
                LoadDoorData(doorData);
                IgnisGUID                        = 0;
                RazorscaleGUID                   = 0;
                RazorscaleController             = 0;
                ExpeditionCommanderGUID          = 0;
                XT002GUID                        = 0;
                KologarnGUID                     = 0;
                LeftArmGUID                      = 0;
                RightArmGUID                     = 0;
                AuriayaGUID                      = 0;

                // Mimiron
                MimironGUID                      = 0;
                LeviathanMKIIGUID                = 0;
                VX001GUID                        = 0;
                AerialUnitGUID                   = 0;
                MagneticCoreGUID                 = 0;
                MimironElevatorGUID              = 0;

                // Thorim
                ThorimGUID                       = 0;
                ThorimDoorGUID                   = 0;
                ThorimDoorGUID                   = 0;
                RunicDoorGUID                    = 0;
                StoneDoorGUID                    = 0;

                HodirGUID                        = 0;
                FreyaGUID                        = 0;
                VezaxGUID                        = 0;
                YoggSaronGUID                    = 0;
                AlgalonGUID                      = 0;
                KologarnChestGUID                = 0;
                KologarnBridgeGUID               = 0;
                KologarnChestGUID                = 0;
                ThorimChestGUID                  = 0;
                HodirRareCacheGUID               = 0;
                HodirChestGUID                   = 0;
                FreyaChestGUID                   = 0;
                LeviathanGateGUID                = 0;
                VezaxDoorGUID                    = 0;
                HodirDoorGUID                    = 0;
                HodirIceDoorGUID                 = 0;
                ArchivumDoorGUID                 = 0;
                TeamInInstance                   = 0;
                HodirRareCacheData               = 0;
                elderCount                       = 0;
                conSpeedAtory                    = false;

                memset(Encounter, 0, sizeof(Encounter));
                memset(XTToyPileGUIDs, 0, sizeof(XTToyPileGUIDs));
                memset(AssemblyGUIDs, 0, sizeof(AssemblyGUIDs));
                memset(RazorHarpoonGUIDs, 0, sizeof(RazorHarpoonGUIDs));
                memset(KeeperGUIDs, 0, sizeof(KeeperGUIDs));

                Encounter[DATA_COLOSSUS] = 0;
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

            void OnPlayerEnter(Player* player)
            {
                if(!TeamInInstance)
                    TeamInInstance = player->GetTeam();
            }

            void OnCreatureCreate(Creature* creature)
            {
                if(!TeamInInstance)
                {
                    Map::PlayerList const& Players = instance->GetPlayers();
                    if(!Players.isEmpty())
                        if(Player* player = Players.begin()->getSource())
                            TeamInInstance = player->GetTeam();
                }

                switch(creature->GetEntry())
                {
                    case NPC_LEVIATHAN:
                        LeviathanGUID = creature->GetGUID();
                        break;
                    case NPC_IGNIS:
                        IgnisGUID = creature->GetGUID();
                        break;
                    case NPC_RAZORSCALE:
                        RazorscaleGUID = creature->GetGUID();
                        break;
                    case NPC_RAZORSCALE_CONTROLLER:
                        RazorscaleController = creature->GetGUID();
                        break;
                    case NPC_EXPEDITION_COMMANDER:
                        ExpeditionCommanderGUID = creature->GetGUID();
                        break;
                    case NPC_XT002:
                        XT002GUID = creature->GetGUID();
                        break;
                    case NPC_XT_TOY_PILE:
                        for(uint8 i = 0; i < 4; ++i)
                            if(!XTToyPileGUIDs[i])
                                XTToyPileGUIDs[i] = creature->GetGUID();
                        break;

                    // Assembly of Iron
                    case NPC_STEELBREAKER:
                        AssemblyGUIDs[0] = creature->GetGUID();
                        break;
                    case NPC_MOLGEIM:
                        AssemblyGUIDs[1] = creature->GetGUID();
                        break;
                    case NPC_BRUNDIR:
                        AssemblyGUIDs[2] = creature->GetGUID();
                        break;

                    // Freya's Keeper
                    case NPC_IRONBRANCH:
                        KeeperGUIDs[0] = creature->GetGUID();
                        if(GetBossState(BOSS_FREYA) == DONE)
                            creature->DespawnOrUnsummon();
                        break;
                    case NPC_BRIGHTLEAF:
                        KeeperGUIDs[1] = creature->GetGUID();
                        if(GetBossState(BOSS_FREYA) == DONE)
                            creature->DespawnOrUnsummon();
                        break;
                    case NPC_STONEBARK:
                        KeeperGUIDs[2] = creature->GetGUID();
                        if(GetBossState(BOSS_FREYA) == DONE)
                            creature->DespawnOrUnsummon();
                         break;

                    // Kologarn
                    case NPC_KOLOGARN:
                        KologarnGUID = creature->GetGUID();
                        break;

                    case NPC_AURIAYA:
                        AuriayaGUID = creature->GetGUID();
                        break;

                    // Mimiron
                    case NPC_MIMIRON:
                        MimironGUID = creature->GetGUID();
                        break;
                    case NPC_LEVIATHAN_MKII:
                        LeviathanMKIIGUID = creature->GetGUID();
                        break;
                    case NPC_VX_001:
                        VX001GUID = creature->GetGUID();
                        break;
                    case NPC_AERIAL_COMMAND_UNIT:
                        AerialUnitGUID = creature->GetGUID();
                        break;
                    case NPC_MAGNETIC_CORE:
                        MagneticCoreGUID = creature->GetGUID();
                        break;

                    // Thorim
                    case NPC_THORIM:
                        ThorimGUID = creature->GetGUID();
                        break;
                    case NPC_RUNIC_COLOSSUS:
                        RunicColossusGUID = creature->GetGUID();
                        break;
                    case NPC_RUNE_GIANT:
                        RuneGiantGUID = creature->GetGUID();
                        break;

                    case NPC_HODIR:
                        HodirGUID = creature->GetGUID();
                        break;
                    case NPC_FREYA:
                        FreyaGUID = creature->GetGUID();
                        break;
                    case NPC_VEZAX:
                        VezaxGUID = creature->GetGUID();
                        break;
                    case NPC_YOGGSARON:
                        YoggSaronGUID = creature->GetGUID();
                        break;
                    case NPC_ALGALON:
                        AlgalonGUID = creature->GetGUID();
                        break;

                    // Hodir's Helper NPCs
                    case NPC_EIVI_NIGHTFEATHER:
                        if(TeamInInstance == HORDE)
                            creature->UpdateEntry(NPC_TOR_GREYCLOUD, HORDE);
                        break;
                    case NPC_ELLIE_NIGHTFEATHER:
                        if(TeamInInstance == HORDE)
                            creature->UpdateEntry(NPC_KAR_GREYCLOUD, HORDE);
                        break;
                    case NPC_ELEMENTALIST_MAHFUUN:
                        if(TeamInInstance == HORDE)
                            creature->UpdateEntry(NPC_SPIRITWALKER_TARA, HORDE);
                        break;
                    case NPC_ELEMENTALIST_AVUUN:
                        if(TeamInInstance == HORDE)
                            creature->UpdateEntry(NPC_SPIRITWALKER_YONA, HORDE);
                        break;
                    case NPC_MISSY_FLAMECUFFS:
                        if(TeamInInstance == HORDE)
                            creature->UpdateEntry(NPC_AMIRA_BLAZEWEAVER, HORDE);
                        break;
                    case NPC_SISSY_FLAMECUFFS:
                        if(TeamInInstance == HORDE)
                            creature->UpdateEntry(NPC_VEESHA_BLAZEWEAVER, HORDE);
                        break;
                    case NPC_FIELD_MEDIC_PENNY:
                        if(TeamInInstance == HORDE)
                            creature->UpdateEntry(NPC_BATTLE_PRIEST_ELIZA, HORDE);
                        break;
                    case NPC_FIELD_MEDIC_JESSI:
                        if(TeamInInstance == HORDE)
                            creature->UpdateEntry(NPC_BATTLE_PRIEST_GINA, HORDE);
                        break;
                }

             }

            void OnGameObjectCreate(GameObject* gameObject)
            {
                switch(gameObject->GetEntry())
                {
                    case GO_KOLOGARN_CHEST_HERO:
                    case GO_KOLOGARN_CHEST:
                        KologarnChestGUID = gameObject->GetGUID();
                        break;
                    case GO_KOLOGARN_BRIDGE:
                        KologarnBridgeGUID = gameObject->GetGUID();
                        if(GetBossState(BOSS_KOLOGARN) == DONE)
                            HandleGameObject(0, false, gameObject);
                        break;
                    case GO_KOLOGARN_DOOR:
                        KologarnDoorGUID = gameObject->GetGUID();
                        break;
                    case GO_HODIR_RARE_CACHE_OF_WINTER_HERO:
                    case GO_HODIR_RARE_CACHE_OF_WINTER:
                        HodirRareCacheGUID = gameObject->GetGUID();
                        break;
                    case GO_HODIR_CHEST_HERO:
                    case GO_HODIR_CHEST:
                        HodirChestGUID = gameObject->GetGUID();
                        break;
                    case GO_FREYA_CHEST_HERO:
                    case GO_FREYA_CHEST:
                        FreyaChestGUID = gameObject->GetGUID();
                        break;
                    case GO_LEVIATHAN_DOOR:
                        AddDoor(gameObject, true);
                        break;
                    case GO_LEVIATHAN_GATE:
                        LeviathanGateGUID = gameObject->GetGUID();
                        if(GetBossState(BOSS_LEVIATHAN) == DONE)
                            gameObject->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                        break;
                    case GO_VEZAX_DOOR:
                        VezaxDoorGUID = gameObject->GetGUID();
                        HandleGameObject(0, false, gameObject);
                        break;
                    case GO_RAZOR_HARPOON_1:
                        RazorHarpoonGUIDs[0] = gameObject->GetGUID();
                        break;
                    case GO_RAZOR_HARPOON_2:
                        RazorHarpoonGUIDs[1] = gameObject->GetGUID();
                        break;
                    case GO_RAZOR_HARPOON_3:
                        RazorHarpoonGUIDs[2] = gameObject->GetGUID();
                        break;
                    case GO_RAZOR_HARPOON_4:
                        RazorHarpoonGUIDs[3] = gameObject->GetGUID();
                        break;
                    case GO_MOLE_MACHINE:
                        if(GetBossState(BOSS_RAZORSCALE) == IN_PROGRESS)
                            gameObject->SetGoState(GO_STATE_ACTIVE);
                    case GO_HODIR_DOOR:
                        HodirDoorGUID = gameObject->GetGUID();
                        break;
                    case GO_HODIR_ICE_DOOR:
                        HodirIceDoorGUID = gameObject->GetGUID();
                        break;
                    case GO_ARCHIVUM_DOOR:
                        ArchivumDoorGUID = gameObject->GetGUID();
                        if(GetBossState(BOSS_ASSEMBLY_OF_IRON) == DONE)
                            HandleGameObject(0, false, gameObject);
                        break;

                    // Mimiron
                    case GO_MIMIRON_ELEVATOR:
                        gameObject->setActive(true);
                        MimironElevatorGUID = gameObject->GetGUID();
                        break;
                    case GO_MIMIRON_DOOR_1:
                    case GO_MIMIRON_DOOR_2:
                    case GO_MIMIRON_DOOR_3:
                        gameObject->setActive(true);
                        MimironDoorGUIDList.push_back(gameObject->GetGUID());
                        break;

                    // Thorim
                    case GO_THORIM_CHEST_HERO:
                    case GO_THORIM_CHEST:
                        ThorimChestGUID = gameObject->GetGUID();
                        break;
                    case GO_THORIM_ENCOUNTER_DOOR:
                        ThorimDoorGUID = gameObject->GetGUID();
                        break;
                    case GO_THORIM_STONE_DOOR:
                        StoneDoorGUID = gameObject->GetGUID();
                        break;
                    case GO_THORIM_RUNIC_DOOR:
                        RunicDoorGUID = gameObject->GetGUID();
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* gameObject)
            {
                switch(gameObject->GetEntry())
                {
                    case GO_LEVIATHAN_DOOR:
                        AddDoor(gameObject, false);
                        break;
                    default:
                        break;
                }
            }

            void OnCreatureDeath(Creature* creature)
            {
                switch(creature->GetEntry())
                {
                    case NPC_CORRUPTED_SERVITOR:
                    case NPC_MISGUIDED_NYMPH:
                    case NPC_GUARDIAN_LASHER:
                    case NPC_FOREST_SWARMER:
                    case NPC_MANGROVE_ENT:
                    case NPC_IRONROOT_LASHER:
                    case NPC_NATURES_BLADE:
                    case NPC_GUARDIAN_OF_LIFE:
                        if(!conSpeedAtory)
                        {
                            DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, CRITERIA_CON_SPEED_ATORY);
                            conSpeedAtory = true;
                        }
                        break;
                    default:
                        break;
                }
            }

            void ProcessEvent(WorldObject* /*gameObject*/, uint32 eventId)
            {
                // Flame Leviathan's Tower Event triggers
                Creature* FlameLeviathan = instance->GetCreature(LeviathanGUID);
                if(FlameLeviathan && FlameLeviathan->isAlive()) // No leviathan, no event triggering ;)
                    switch(eventId)
                    {
                        case EVENT_TOWER_OF_STORM_DESTROYED:
                            FlameLeviathan->AI()->DoAction(ACTION_TOWER_OF_STORM_DESTROYED);
                            break;
                        case EVENT_TOWER_OF_FROST_DESTROYED:
                            FlameLeviathan->AI()->DoAction(ACTION_TOWER_OF_FROST_DESTROYED);
                            break;
                        case EVENT_TOWER_OF_FLAMES_DESTROYED:
                            FlameLeviathan->AI()->DoAction(ACTION_TOWER_OF_FLAMES_DESTROYED);
                            break;
                        case EVENT_TOWER_OF_LIFE_DESTROYED:
                            FlameLeviathan->AI()->DoAction(ACTION_TOWER_OF_LIFE_DESTROYED);
                            break;
                    }
            }

            bool SetBossState(uint32 type, EncounterState state)
            {
                if(!InstanceScript::SetBossState(type, state))
                    return false;

                switch(type)
                {
                    case BOSS_LEVIATHAN:
                    case BOSS_IGNIS:
                    case BOSS_RAZORSCALE:
                    case BOSS_XT002:
                    case BOSS_AURIAYA:
                    case BOSS_MIMIRON:
                        for(std::list<uint64>::iterator i = MimironDoorGUIDList.begin(); i != MimironDoorGUIDList.end(); i++)
                        {
                            if(GameObject* obj = instance->GetGameObject(*i))
                                obj->SetGoState(state == IN_PROGRESS ? GO_STATE_READY : GO_STATE_ACTIVE );
                        }
                        break;
                    case BOSS_ASSEMBLY_OF_IRON:
                        if(state == DONE)
                            HandleGameObject(ArchivumDoorGUID, true);
                        break;
                    case BOSS_VEZAX:
                        if(state == DONE)
                            //HandleGameObject(VezaxDoorGUID, true);
                        break;
                    case BOSS_YOGGSARON:
                        break;
                    case BOSS_KOLOGARN:
                        if(state == DONE)
                        {
                            if(GameObject* gameObject = instance->GetGameObject(KologarnChestGUID))
                                gameObject->SetRespawnTime(gameObject->GetRespawnDelay());
                            HandleGameObject(KologarnBridgeGUID, false);
                        }
                        break;
                    case BOSS_HODIR:
                        if(state == DONE)
                        {
                            if(GameObject* HodirRareCache = instance->GetGameObject(HodirRareCacheGUID))
                            {
                                if(GetData(DATA_HODIR_RARE_CACHE))
                                {
                                    HodirRareCache->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);
                                    HodirRareCache->SetRespawnTime(HodirRareCache->GetRespawnDelay());
                                }
                            }

                            if(GameObject* pChest = instance->GetGameObject(HodirChestGUID))
                                pChest->SetRespawnTime(pChest->GetRespawnDelay());
                            HandleGameObject(HodirDoorGUID, true);
                            HandleGameObject(HodirIceDoorGUID, true);
                        }
                        break;
                    case BOSS_THORIM:
                        if(state == DONE)
                            if(GameObject* pChest = instance->GetGameObject(ThorimChestGUID))
                                pChest->SetRespawnTime(pChest->GetRespawnDelay());

                        if(GameObject* gameObjectDoor = instance->GetGameObject(ThorimDoorGUID))
                            gameObjectDoor->SetGoState(state == IN_PROGRESS ? GO_STATE_READY : GO_STATE_ACTIVE);
                        break;
                    case BOSS_FREYA:
                        if(state == DONE)
                            if(GameObject* pChest = instance->GetGameObject(FreyaChestGUID))
                                pChest->SetRespawnTime(pChest->GetRespawnDelay());
                        break;
                }

                return true;
            }

            void SetData(uint32 type, uint32 data)
            {
                switch(type)
                {
                    case DATA_COLOSSUS:
                        if(Encounter[DATA_COLOSSUS])
                        {
                            if(Creature* Leviathan = instance->GetCreature(LeviathanGUID))
                                Leviathan->AI()->DoAction(ACTION_MOVE_TO_CENTER_POSITION);
                            if(GameObject* gameObject = instance->GetGameObject(LeviathanGateGUID))
                                gameObject->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                            SaveToDB();
                        } else
                            Encounter[DATA_COLOSSUS] = 1;
                        break;
                    case DATA_HODIR_RARE_CACHE:
                        HodirRareCacheData = data;
                        if(!HodirRareCacheData)
                        {
                            if(Creature* Hodir = instance->GetCreature(HodirGUID))
                                if(GameObject* gameObject = instance->GetGameObject(HodirRareCacheGUID))
                                    Hodir->RemoveGameObject(gameObject, false);
                        }
                        break;

                    // Mimiron
                    case DATA_MIMIRON_ELEVATOR:
                        if(GameObject* go = instance->GetGameObject(MimironElevatorGUID))
                            go->SetGoState(GOState(data));
                        break;

                    // Thorim
                    case DATA_RUNIC_DOOR:
                        if(GameObject* go = instance->GetGameObject(RunicDoorGUID))
                            go->SetGoState(GOState(data));
                        break;
                    case DATA_STONE_DOOR:
                        if(GameObject* go = instance->GetGameObject(StoneDoorGUID))
                            go->SetGoState(GOState(data));
                        break;
                    default:
                        break;
                }
            }

            void SetData64(uint32 type, uint64 data)
            {
                switch(type)
                {
                    case DATA_LEFT_ARM:
                        LeftArmGUID = data;
                        break;
                    case DATA_RIGHT_ARM:
                        RightArmGUID = data;
                        break;
                }
            }

            uint64 GetData64(uint32 data)
            {
                switch(data)
                {
                    case BOSS_LEVIATHAN:
                        return LeviathanGUID;
                    case BOSS_IGNIS:
                        return IgnisGUID;
                    case BOSS_RAZORSCALE:
                        return RazorscaleGUID;
                    case DATA_RAZORSCALE_CONTROL:
                        return RazorscaleController;
                    case BOSS_XT002:
                        return XT002GUID;
                    case DATA_TOY_PILE_0:
                    case DATA_TOY_PILE_1:
                    case DATA_TOY_PILE_2:
                    case DATA_TOY_PILE_3:
                        return XTToyPileGUIDs[data - DATA_TOY_PILE_0];
                    case BOSS_KOLOGARN:
                        return KologarnGUID;
                    case DATA_LEFT_ARM:
                        return LeftArmGUID;
                    case DATA_RIGHT_ARM:
                        return RightArmGUID;
                    case BOSS_AURIAYA:
                        return AuriayaGUID;
                    case BOSS_MIMIRON:
                        return MimironGUID;
                    case BOSS_HODIR:
                        return HodirGUID;

                    // Thorim
                    case BOSS_THORIM:
                        return ThorimGUID;
                    case DATA_RUNIC_COLOSSUS:
                        return RunicColossusGUID;
                    case DATA_RUNE_GIANT:
                        return RuneGiantGUID;

                    case BOSS_FREYA:
                        return FreyaGUID;
                    case BOSS_VEZAX:
                        return VezaxGUID;
                    case BOSS_YOGGSARON:
                        return YoggSaronGUID;
                    case BOSS_ALGALON:
                        return AlgalonGUID;

                    // Razorscale expedition commander
                    case DATA_EXPEDITION_COMMANDER:
                        return ExpeditionCommanderGUID;
                    case GO_RAZOR_HARPOON_1:
                        return RazorHarpoonGUIDs[0];
                    case GO_RAZOR_HARPOON_2:
                        return RazorHarpoonGUIDs[1];
                    case GO_RAZOR_HARPOON_3:
                        return RazorHarpoonGUIDs[2];
                    case GO_RAZOR_HARPOON_4:
                        return RazorHarpoonGUIDs[3];

                    // Mimiron
                    case DATA_LEVIATHAN_MK_II:
                        return LeviathanMKIIGUID;
                    case DATA_VX_001:
                        return VX001GUID;
                    case DATA_AERIAL_UNIT:
                        return AerialUnitGUID;
                    case DATA_MAGNETIC_CORE:
                        return MagneticCoreGUID;

                    // Assembly of Iron
                    case BOSS_STEELBREAKER:
                        return AssemblyGUIDs[0];
                    case BOSS_MOLGEIM:
                        return AssemblyGUIDs[1];
                    case BOSS_BRUNDIR:
                        return AssemblyGUIDs[2];

                    // Freya's Keepers
                    case BOSS_BRIGHTLEAF:
                        return KeeperGUIDs[0];
                    case BOSS_IRONBRANCH:
                        return KeeperGUIDs[1];
                    case BOSS_STONEBARK:
                        return KeeperGUIDs[2];
                }

                return 0;
            }

            uint32 GetData(uint32 type)
            {
                switch(type)
                {
                    case DATA_COLOSSUS:
                        return Encounter[type];
                    case DATA_HODIR_RARE_CACHE:
                        return HodirRareCacheData;
                    default:
                        break;
                }

                return 0;
            }

            std::string GetSaveData()
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "U U " << GetBossSaveData() << GetData(DATA_COLOSSUS);

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* strIn)
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

                if(dataHead1 == 'U' && dataHead2 == 'U')
                {
                    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if(tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;

                        if(i == DATA_COLOSSUS)
                            SetData(i, tmpState);
                        else
                            SetBossState(i, EncounterState(tmpState));
                    }
                }

                OUT_LOAD_INST_DATA_COMPLETE;
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_ulduar_InstanceMapScript(map);
        }
};

void AddSC_instance_ulduar()
{
    new instance_ulduar;
}

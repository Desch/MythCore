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

#ifndef DEF_ULDUAR_H
#define DEF_ULDUAR_H

#include "ObjectMgr.h"
#define UlduarScriptName "instance_ulduar"

enum UlduarBosses
{
    MAX_ENCOUNTER            = 20,

    BOSS_LEVIATHAN           = 0,
    BOSS_IGNIS               = 1,
    BOSS_RAZORSCALE          = 2,
    BOSS_XT002               = 3,
    BOSS_ASSEMBLY_OF_IRON    = 4,
    BOSS_STEELBREAKER        = 5,
    BOSS_MOLGEIM             = 6,
    BOSS_BRUNDIR             = 7,
    BOSS_KOLOGARN            = 8,
    BOSS_AURIAYA             = 9,
    BOSS_MIMIRON             = 10,
    BOSS_HODIR               = 11,
    BOSS_THORIM              = 12,
    BOSS_FREYA               = 13,
    BOSS_BRIGHTLEAF          = 14,
    BOSS_IRONBRANCH          = 15,
    BOSS_STONEBARK           = 16,
    BOSS_VEZAX               = 17,
    BOSS_YOGGSARON           = 18,
    BOSS_ALGALON             = 19,
};

enum UlduarNPCs
{
    // General
    NPC_LEVIATHAN                = 33113,
    NPC_IGNIS                    = 33118,
    NPC_RAZORSCALE               = 33186,
    NPC_RAZORSCALE_CONTROLLER    = 33233,
    NPC_STEELFORGED_DEFFENDER    = 33236,
    NPC_EXPEDITION_COMMANDER     = 33210,
    NPC_XT002                    = 33293,
    NPC_XT_TOY_PILE              = 33337,
    NPC_STEELBREAKER             = 32867,
    NPC_MOLGEIM                  = 32927,
    NPC_BRUNDIR                  = 32857,
    NPC_KOLOGARN                 = 32930,
    NPC_FOCUSED_EYEBEAM          = 33632,
    NPC_FOCUSED_EYEBEAM_RIGHT    = 33802,
    NPC_LEFT_ARM                 = 32933,
    NPC_RIGHT_ARM                = 32934,
    NPC_RUBBLE                   = 33768,
    NPC_AURIAYA                  = 33515,
    NPC_MIMIRON                  = 33350,
    NPC_HODIR                    = 32845,
    NPC_FREYA                    = 32906,
    NPC_VEZAX                    = 33271,
    NPC_YOGGSARON                = 33288,
    NPC_ALGALON                  = 32871,

    // Mimiron
    NPC_LEVIATHAN_MKII           = 33432,
    NPC_VX_001                   = 33651,
    NPC_AERIAL_COMMAND_UNIT      = 33670,
    NPC_MAGNETIC_CORE            = 34068,

    // Thorim
    NPC_THORIM                   = 32865,
    NPC_RUNIC_COLOSSUS           = 32872,
    NPC_RUNE_GIANT               = 32873,

    // Freya's Keepers
    NPC_IRONBRANCH               = 32913,
    NPC_BRIGHTLEAF               = 32915,
    NPC_STONEBARK                = 32914,

    // Hodir's Helper NPCs
    NPC_TOR_GREYCLOUD            = 32941,
    NPC_KAR_GREYCLOUD            = 33333,
    NPC_EIVI_NIGHTFEATHER        = 33325,
    NPC_ELLIE_NIGHTFEATHER       = 32901,
    NPC_SPIRITWALKER_TARA        = 33332,
    NPC_SPIRITWALKER_YONA        = 32950,
    NPC_ELEMENTALIST_MAHFUUN     = 33328,
    NPC_ELEMENTALIST_AVUUN       = 32900,
    NPC_AMIRA_BLAZEWEAVER        = 33331,
    NPC_VEESHA_BLAZEWEAVER       = 32946,
    NPC_MISSY_FLAMECUFFS         = 32893,
    NPC_SISSY_FLAMECUFFS         = 33327,
    NPC_BATTLE_PRIEST_ELIZA      = 32948,
    NPC_BATTLE_PRIEST_GINA       = 33330,
    NPC_FIELD_MEDIC_PENNY        = 32897,
    NPC_FIELD_MEDIC_JESSI        = 33326,

    // Freya's trash NPCs
    NPC_CORRUPTED_SERVITOR       = 33354,
    NPC_MISGUIDED_NYMPH          = 33355,
    NPC_GUARDIAN_LASHER          = 33430,
    NPC_FOREST_SWARMER           = 33431,
    NPC_MANGROVE_ENT             = 33525,
    NPC_IRONROOT_LASHER          = 33526,
    NPC_NATURES_BLADE            = 33527,
    NPC_GUARDIAN_OF_LIFE         = 33528,
};

enum UlduarGameObjects
{
    GO_KOLOGARN_CHEST_HERO              = 195047,
    GO_KOLOGARN_CHEST                   = 195046,
    GO_KOLOGARN_BRIDGE                  = 194232,
    GO_KOLOGARN_DOOR                    = 194553,
    GO_HODIR_RARE_CACHE_OF_WINTER       = 194200,
    GO_HODIR_RARE_CACHE_OF_WINTER_HERO  = 194201,
    GO_HODIR_CHEST_HERO                 = 194308,
    GO_HODIR_CHEST                      = 194307,
    GO_FREYA_CHEST                      = 194324,
    GO_FREYA_CHEST_HERO                 = 194325,
    GO_FREYA_CHEST_HARD                 = 194327,
    GO_FREYA_CHEST_HERO_HARD            = 194331,
    GO_LEVIATHAN_DOOR                   = 194905,
    GO_LEVIATHAN_GATE                   = 194630,
    GO_VEZAX_DOOR                       = 194750,
    GO_MOLE_MACHINE                     = 194316,
    GO_RAZOR_HARPOON_1                  = 194542,
    GO_RAZOR_HARPOON_2                  = 194541,
    GO_RAZOR_HARPOON_3                  = 194543,
    GO_RAZOR_HARPOON_4                  = 194519,
    GO_RAZOR_BROKEN_HARPOON             = 194565,
    GO_HODIR_DOOR                       = 194634,
    GO_HODIR_ICE_DOOR                   = 194441,
    GO_ARCHIVUM_DOOR                    = 194556,

    // Mimiron
    GO_MIMIRON_TRAIN                    = 194675,
    GO_MIMIRON_ELEVATOR                 = 194749,
    GO_MIMIRON_DOOR_1                   = 194776,
    GO_MIMIRON_DOOR_2                   = 194774,
    GO_MIMIRON_DOOR_3                   = 194775,
    GO_BIG_RED_BUTTON                   = 194739,

    // Thorim
    GO_THORIM_CHEST_HERO                = 194315,
    GO_THORIM_CHEST                     = 194314,
    GO_THORIM_ENCOUNTER_DOOR            = 194559,
    GO_THORIM_STONE_DOOR                = 194558,
    GO_THORIM_RUNIC_DOOR                = 194557,

    GO_GIFT_OF_THE_OBSERVER_10          = 194821,
    GO_GIFT_OF_THE_OBSERVER_25          = 194822,
};

enum LeviathanData
{
    EVENT_TOWER_OF_STORM_DESTROYED      = 21031,
    EVENT_TOWER_OF_FROST_DESTROYED      = 21032,
    EVENT_TOWER_OF_FLAMES_DESTROYED     = 21033,
    EVENT_TOWER_OF_LIFE_DESTROYED       = 21030,
    ACTION_TOWER_OF_STORM_DESTROYED     = 1,
    ACTION_TOWER_OF_FROST_DESTROYED     = 2,
    ACTION_TOWER_OF_FLAMES_DESTROYED    = 3,
    ACTION_TOWER_OF_LIFE_DESTROYED      = 4,
    ACTION_MOVE_TO_CENTER_POSITION      = 10,
};

enum UlduarAchievementCriteriaIds
{
    CRITERIA_CON_SPEED_ATORY    = 21597,
    CRITERIA_DISARMED           = 21687,
};

enum UlduarData
{
    // Collosus (Leviathan)
    DATA_COLOSSUS                = 20,

    // Razorscale
    DATA_EXPEDITION_COMMANDER,
    DATA_RAZORSCALE_CONTROL,

    // XT-002
    DATA_TOY_PILE_0,
    DATA_TOY_PILE_1,
    DATA_TOY_PILE_2,
    DATA_TOY_PILE_3,

    // Kologarn
    DATA_LEFT_ARM,
    DATA_RIGHT_ARM,

    // Hodir
    DATA_HODIR_RARE_CACHE,

    // Mimiron
    DATA_LEVIATHAN_MK_II,
    DATA_MIMIRON_ELEVATOR,
    DATA_VX_001,
    DATA_AERIAL_UNIT,
    DATA_MAGNETIC_CORE,

    // Thorim
    DATA_RUNIC_COLOSSUS,
    DATA_RUNE_GIANT,
    DATA_RUNIC_DOOR,
    DATA_STONE_DOOR,
    DATA_HODIR_RARE_CHEST,
};

template<class AI>
CreatureAI* GetUlduarAI(Creature* pCreature)
{
    if(InstanceMap* pInstance = pCreature->GetMap()->ToInstanceMap())
        if(pInstance->GetInstanceScript())
            if(pInstance->GetScriptId() == GetScriptId(UlduarScriptName))
                return new AI(pCreature);
    return NULL;
}

class PlayerOrPetCheck
{
    public:
        bool operator() (Unit* unit)
        {
            if(unit->GetTypeId() != TYPEID_PLAYER)
                if(!unit->ToCreature()->isPet())
                    return true;

            return false;
        }
};

#endif

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

#ifndef DEF_BLACKROCK_SPIRE_H
#define DEF_BLACKROCK_SPIRE_H

enum Data
{
    DATA_OMOKK,
    DATA_SHADOW_HUNTER_VOSHGAJIN,
    DATA_WARMASTER_VOONE,
    DATA_MOTHER_SMOLDERWEB,
    DATA_UROK_DOOMHOWL,                       // not scripted
    DATA_QUARTERMASTER_ZIGRIS,
    DATA_GIZRUL_THE_SLAVENER,                 // not scripted
    DATA_HALYCON,
    DATA_OVERLORD_WYRMTHALAK,
    DATA_PYROGAURD_EMBERSEER,
    DATA_WARCHIEF_REND_BLACKHAND,
    DATA_GYTH,
    DATA_THE_BEAST,
    DATA_GENERAL_DRAKKISATH
};

enum Npc
{
    NPC_OMOKK                       = 9196,
    NPC_SHADOW_HUNTER_VOSHGAJIN     = 9236,
    NPC_WARMASTER_VOONE             = 9237,
    NPC_MOTHER_SMOLDERWEB           = 10596,
    NPC_UROK_DOOMHOWL               = 10584,
    NPC_QUARTERMASTER_ZIGRIS        = 9736,
    NPC_GIZRUL_THE_SLAVENER         = 10268,
    NPC_HALYCON                     = 10220,
    NPC_OVERLORD_WYRMTHALAK         = 9568,
    NPC_PYROGAURD_EMBERSEER         = 9816,
    NPC_WARCHIEF_REND_BLACKHAND     = 10429,
    NPC_GYTH                        = 10339,
    NPC_THE_BEAST                   = 10430,
    NPC_GENERAL_DRAKKISATH          = 10363,
};

enum AdditionalData
{
    SPELL_SUMMON_ROOKERY_WHELP      = 15745,
    MAX_ENCOUNTER                   = 14,
};

enum GameObjects
{
    GO_WHELP_SPAWNER                = 175622, //trap spawned by go id 175124
};

#endif


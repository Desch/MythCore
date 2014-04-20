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

#ifndef DEF_OCULUS_H
#define DEF_OCULUS_H

enum Data
{
    DATA_DRAKOS_EVENT,
    DATA_VAROS_EVENT,
    DATA_UROM_EVENT,
    DATA_EREGOS_EVENT,
    DATA_UROM_PLATAFORM
};

enum Data64
{
    DATA_DRAKOS,
    DATA_VAROS,
    DATA_UROM,
    DATA_EREGOS
};

enum Bosses
{
    NPC_DRAKOS                  = 27654,
    NPC_VAROS                   = 27447,
    NPC_UROM                    = 27655,
    NPC_EREGOS                  = 27656,

    NPC_AZURE_RING_GUARDIAN     = 28236,
    NPC_CENTRIFUGE_CONSTRUCT    = 27641,
};

enum GameObjects
{
    GO_DRAGON_CAGE_DOOR                           = 193995,
    GO_EREGOS_CACHE_5N                            = 191349,
    GO_EREGOS_CACHE_5H                            = 193603,
    GO_SPOTLIGHT                                  = 191351
};

enum SpellEvents
{
    EVENT_CALL_DRAGON = 12229
};

enum CreatureActions
{
    ACTION_CALL_DRAGON_EVENT = 1
};

enum OculusWorldStates
{
    WORLD_STATE_CENTRIFUGE_CONSTRUCT_SHOW   = 3524,
    WORLD_STATE_CENTRIFUGE_CONSTRUCT_AMOUNT = 3486
};

enum OculusSpells
{
    SPELL_CENTRIFUGE_SHIELD = 50053
};

#endif
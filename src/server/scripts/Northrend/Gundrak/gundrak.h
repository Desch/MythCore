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

#ifndef DEF_GUNDRAK_H
#define DEF_GUNDRAK_H

enum Data
{
    DATA_SLAD_RAN_EVENT,
    DATA_MOORABI_EVENT,
    DATA_DRAKKARI_COLOSSUS_EVENT,
    DATA_GAL_DARAH_EVENT,
    DATA_ECK_THE_FEROCIOUS_EVENT,
    DATA_ALIVE_RUIN_DWELLERS
};

enum Data64
{
    DATA_SLAD_RAN_ALTAR,
    DATA_MOORABI_ALTAR,
    DATA_DRAKKARI_COLOSSUS_ALTAR,
    DATA_SLAD_RAN_STATUE,
    DATA_MOORABI_STATUE,
    DATA_DRAKKARI_COLOSSUS_STATUE,
    DATA_DRAKKARI_COLOSSUS,
    DATA_RUIN_DWELLER_DIED,
    DATA_STATUE_ACTIVATE,
};

enum mainCreatures
{
    CREATURE_RUIN_DWELLER                         = 29920,
    CREATURE_SLAD_RAN                             = 29304,
    CREATURE_MOORABI                              = 29305,
    CREATURE_GALDARAH                             = 29306,
    CREATURE_DRAKKARICOLOSSUS                     = 29307,
    CREATURE_ECK                                  = 29932
};

#endif

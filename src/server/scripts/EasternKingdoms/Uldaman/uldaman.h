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

#ifndef DEF_ULDAMAN_H
#define DEF_ULDAMAN_H

enum eObjects
{
    GO_ARCHAEDAS_TEMPLE_DOOR            = 141869,
    GO_ALTAR_OF_THE_KEEPER_TEMPLE_DOOR  = 124367,
    GO_ANCIENT_VAULT_DOOR               = 124369,
    GO_IRONAYA_SEAL_DOOR                = 124372,
    GO_KEYSTONE                         = 124371,
};

enum eData
{
    DATA_ALTAR_DOORS                    = 1,
    DATA_ANCIENT_DOOR                   = 2,
    DATA_IRONAYA_DOOR                   = 3,
    DATA_STONE_KEEPERS                  = 4,
    DATA_MINIONS                        = 5,
    DATA_IRONAYA_SEAL                   = 6,
};

#define MAX_ENCOUNTER                   3
#endif
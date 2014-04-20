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

#ifndef __ADDONHANDLER_H
#define __ADDONHANDLER_H

#include "Common.h"
#include "Config.h"
#include <ace/Singleton.h>
#include "WorldPacket.h"

class AddonHandler
{
    /* Construction */
    friend class ACE_Singleton<AddonHandler, ACE_Null_Mutex>;
    AddonHandler();

    public:
        ~AddonHandler();
                                                            //build addon packet
        bool BuildAddonPacket(WorldPacket* Source, WorldPacket* Target);
};

#define sAddOnHandler ACE_Singleton<AddonHandler, ACE_Null_Mutex>::instance()
#endif


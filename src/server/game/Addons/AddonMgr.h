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

#ifndef _ADDONMGR_H
#define _ADDONMGR_H

#include "Common.h"
#include <ace/Singleton.h>

#include <string>

class WorldSession;

struct AddonInfo
{
    AddonInfo(const std::string& name, uint8 enabled, uint32 crc, uint8 state, bool crcOrPubKey)
    {
        Name = name;
        Enabled = enabled;
        CRC = crc;
        State = state;
        UsePublicKeyOrCRC = crcOrPubKey;
    }

    std::string Name;
    uint8 Enabled;
    uint32 CRC;
    uint8 State;
    bool UsePublicKeyOrCRC;
};

struct SavedAddon
{
    SavedAddon(const std::string& name, uint32 crc)
    {
        Name = name;
        CRC = crc;
    }

    std::string Name;
    uint32 CRC;
};

// List of client addons (for WorldSession).
typedef std::list<AddonInfo> AddonsList;

// List of saved addons (in DB).
typedef std::list<SavedAddon> SavedAddonsList;

#define STANDARD_ADDON_CRC 0x4c1c776d

class AddonMgr
{
    friend class ACE_Singleton<AddonMgr, ACE_Null_Mutex>;
    AddonMgr();
    ~AddonMgr();

public:
    void LoadFromDB();
    void SaveAddon(AddonInfo const& addon);

    SavedAddon const* GetAddonInfo(const std::string& name) const;

private:
    SavedAddonsList m_knownAddons;                           // Known addons.
};

#define sAddonMgr ACE_Singleton<AddonMgr, ACE_Null_Mutex>::instance()

#endif


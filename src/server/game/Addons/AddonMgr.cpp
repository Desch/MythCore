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

#include "DatabaseEnv.h"
#include "AddonMgr.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "Util.h"
#include "SHA1.h"

AddonMgr::AddonMgr() { }

AddonMgr::~AddonMgr() { }

void AddonMgr::LoadFromDB()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.Query("SELECT name, crc FROM addons");

    if(!result)
    {
        sLog->outString(">> Loaded 0 known addons. DB table `addons` is empty!");
        sLog->outString();
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        std::string name = fields[0].GetString();
        uint32 crc = fields[1].GetUInt32();

        SavedAddon addon(name, crc);
        m_knownAddons.push_back(addon);

        ++count;
    }
    while(result->NextRow());

    sLog->outString(">> Loaded %u known addons in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void AddonMgr::SaveAddon(AddonInfo const& addon)
{
    std::string name = addon.Name;
    CharacterDatabase.EscapeString(name);
    CharacterDatabase.PExecute("INSERT INTO addons (name, crc) VALUES ('%s', %u)", name.c_str(), addon.CRC);

    SavedAddon newAddon(addon.Name, addon.CRC);
    m_knownAddons.push_back(newAddon);
}

SavedAddon const* AddonMgr::GetAddonInfo(const std::string& name) const
{
    for(SavedAddonsList::const_iterator it = m_knownAddons.begin(); it != m_knownAddons.end(); ++it)
    {
        SavedAddon const& addon = (*it);
        if(addon.Name == name)
            return &addon;
    }

    return NULL;
}

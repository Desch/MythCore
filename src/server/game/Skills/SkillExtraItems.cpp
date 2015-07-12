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

#include "SkillExtraItems.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "Player.h"
#include <map>

// some type definitions
// no use putting them in the header file, they're only used in this .cpp

// struct to store information about extra item creation
// one entry for every spell that is able to create an extra item
struct SkillExtraItemEntry
{
    // the spell id of the specialization required to create extra items
    uint32 requiredSpecialization;
    // the chance to create one additional item
    float additionalCreateChance;
    // maximum number of extra items created per crafting
    uint8 additionalMaxNum;

    uint32 newItemId;

    SkillExtraItemEntry()
        : requiredSpecialization(0), additionalCreateChance(0.0f), additionalMaxNum(0) { }

    SkillExtraItemEntry(uint32 rS, float aCC, uint8 aMN)
        : requiredSpecialization(rS), additionalCreateChance(aCC), additionalMaxNum(aMN) { }
};

// map to store the extra item creation info, the key is the spellId of the creation spell, the mapped value is the assigned SkillExtraItemEntry
typedef std::map<uint32, SkillExtraItemEntry> SkillExtraItemMap;

SkillExtraItemMap SkillExtraItemStore;

// loads the extra item creation info from DB
void LoadSkillExtraItemTable()
{
    uint32 oldMSTime = getMSTime();

    SkillExtraItemStore.clear();                            // need for reload

    //                                                 0        1                       2                       3
    QueryResult result = WorldDatabase.Query("SELECT spellId, requiredSpecialization, additionalCreateChance, additionalMaxNum, newItemId FROM skill_extra_item_template");

    if(!result)
    {
        sLog->outErrorDb(">> Loaded 0 spell specialization definitions. DB table `skill_extra_item_template` is empty.");
        sLog->outString();
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 spellId = fields[0].GetUInt32();

        if(!sSpellStore.LookupEntry(spellId))
        {
            sLog->outError("Skill specialization %u has non-existent spell id in `skill_extra_item_template`!", spellId);
            continue;
        }

        uint32 requiredSpecialization = fields[1].GetUInt32();
        if(!sSpellStore.LookupEntry(requiredSpecialization))
        {
            sLog->outError("Skill specialization %u have not existed required specialization spell id %u in `skill_extra_item_template`!", spellId, requiredSpecialization);
            continue;
        }

        float additionalCreateChance = fields[2].GetFloat();
        if(additionalCreateChance <= 0.0f)
        {
            sLog->outError("Skill specialization %u has too low additional create chance in `skill_extra_item_template`!", spellId);
            continue;
        }

        uint8 additionalMaxNum = fields[3].GetUInt8();
        if(!additionalMaxNum)
        {
            sLog->outError("Skill specialization %u has 0 max number of extra items in `skill_extra_item_template`!", spellId);
            continue;
        }

        uint32 newItemId = fields[4].GetUInt32();
        if(newItemId < 0)
        {
            sLog->outError("Skill specialization %u has newItemId lower than 0 in `skill_extra_item_template`!", spellId);
            continue;
        }

        SkillExtraItemEntry& skillExtraItemEntry = SkillExtraItemStore[spellId];

        skillExtraItemEntry.requiredSpecialization = requiredSpecialization;
        skillExtraItemEntry.additionalCreateChance = additionalCreateChance;
        skillExtraItemEntry.additionalMaxNum       = additionalMaxNum;
        skillExtraItemEntry.newItemId              = newItemId;

        ++count;
    }
    while(result->NextRow());

    sLog->outString(">> Loaded %u spell specialization definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

bool canCreateExtraItems(Player* pPlayer, uint32 spellId, float &additionalChance, uint8 &additionalMax, uint32 &newItemId)
{
    // get the info for the specified spell
    SkillExtraItemMap::const_iterator ret = SkillExtraItemStore.find(spellId);
    if(ret == SkillExtraItemStore.end())
        return false;

    SkillExtraItemEntry const* specEntry = &ret->second;

    // if no entry, then no extra items can be created
    if(!specEntry)
        return false;

    // the player doesn't have the required specialization, return false
    if(!pPlayer->HasSpell(specEntry->requiredSpecialization))
        return false;

    // set the arguments to the appropriate values
    additionalChance = specEntry->additionalCreateChance;
    additionalMax = specEntry->additionalMaxNum;
    newItemId = specEntry->newItemId;

    // enable extra item creation
    return true;
}
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

#include "SpellMgr.h"
#include "ObjectMgr.h"
#include "SpellAuras.h"
#include "SpellAuraDefines.h"
#include "DBCStores.h"
#include "World.h"
#include "Chat.h"
#include "Spell.h"
#include "BattlegroundMgr.h"
#include "CreatureAI.h"
#include "MapManager.h"
#include "BattlegroundIC.h"
#include "OutdoorPvPMgr.h"
#include "OutdoorPvPWG.h"

bool IsAreaEffectTarget[TOTAL_SPELL_TARGETS];
SpellEffectTargetTypes EffectTargetType[TOTAL_SPELL_EFFECTS];
SpellSelectTargetTypes SpellTargetType[TOTAL_SPELL_TARGETS];

SpellMgr::SpellMgr()
{
    for(uint8 i = 0; i < TOTAL_SPELL_EFFECTS; ++i)
    {
        switch(i)
        {
            case SPELL_EFFECT_PERSISTENT_AREA_AURA: //27
            case SPELL_EFFECT_SUMMON:               //28
            case SPELL_EFFECT_TRIGGER_MISSILE:      //32
            case SPELL_EFFECT_TRANS_DOOR:           //50 summon object
            case SPELL_EFFECT_SUMMON_PET:           //56
            case SPELL_EFFECT_ADD_FARSIGHT:         //72
            case SPELL_EFFECT_SUMMON_OBJECT_WILD:   //76
            //case SPELL_EFFECT_SUMMON_CRITTER:       //97 not 303
            case SPELL_EFFECT_SUMMON_OBJECT_SLOT1:  //104
            case SPELL_EFFECT_SUMMON_OBJECT_SLOT2:  //105
            case SPELL_EFFECT_SUMMON_OBJECT_SLOT3:  //106
            case SPELL_EFFECT_SUMMON_OBJECT_SLOT4:  //107
            case SPELL_EFFECT_SUMMON_DEAD_PET:      //109
            case SPELL_EFFECT_TRIGGER_SPELL_2:      //151 ritual of summon
                EffectTargetType[i] = SPELL_REQUIRE_DEST;
                break;
            case SPELL_EFFECT_PARRY: // 0
            case SPELL_EFFECT_BLOCK: // 0
            case SPELL_EFFECT_SKILL: // always with dummy 3 as A
            //case SPELL_EFFECT_LEARN_SPELL: // 0 may be 5 pet
            case SPELL_EFFECT_TRADE_SKILL: // 0 or 1
            case SPELL_EFFECT_PROFICIENCY: // 0
                EffectTargetType[i] = SPELL_REQUIRE_NONE;
                break;
            case SPELL_EFFECT_ENCHANT_ITEM:
            case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
            case SPELL_EFFECT_DISENCHANT:
            //in 243 this is 0, in 309 it is 1
            //so both item target and unit target is pushed, and cause crash
            //case SPELL_EFFECT_FEED_PET:
            case SPELL_EFFECT_PROSPECTING:
            case SPELL_EFFECT_MILLING:
            case SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC:
                EffectTargetType[i] = SPELL_REQUIRE_ITEM;
                break;
            //caster must be pushed otherwise no sound
            case SPELL_EFFECT_APPLY_AREA_AURA_PARTY:
            case SPELL_EFFECT_APPLY_AREA_AURA_FRIEND:
            case SPELL_EFFECT_APPLY_AREA_AURA_ENEMY:
            case SPELL_EFFECT_APPLY_AREA_AURA_PET:
            case SPELL_EFFECT_APPLY_AREA_AURA_OWNER:
            case SPELL_EFFECT_APPLY_AREA_AURA_RAID:
            case SPELL_EFFECT_CHARGE:
            case SPELL_EFFECT_CHARGE_DEST:
            case SPELL_EFFECT_JUMP:
            case SPELL_EFFECT_JUMP_DEST:
            case SPELL_EFFECT_LEAP_BACK:
                EffectTargetType[i] = SPELL_REQUIRE_CASTER;
                break;
            //case SPELL_EFFECT_WMO_DAMAGE:
            //case SPELL_EFFECT_WMO_REPAIR:
            //case SPELL_EFFECT_WMO_CHANGE:
            //    EffectTargetType[i] = SPELL_REQUIRE_GOBJECT;
            //    break;
            default:
                EffectTargetType[i] = SPELL_REQUIRE_UNIT;
                break;
        }
    }

    for(uint8 i = 0; i < TOTAL_SPELL_TARGETS; ++i)
    {
        switch(i)
        {
            case TARGET_UNIT_CASTER:
            case TARGET_UNIT_CASTER_FISHING:
            case TARGET_UNIT_MASTER:
            case TARGET_UNIT_PET:
            case TARGET_UNIT_PARTY_CASTER:
            case TARGET_UNIT_RAID_CASTER:
            case TARGET_UNIT_VEHICLE:
            case TARGET_UNIT_PASSENGER_0:
            case TARGET_UNIT_PASSENGER_1:
            case TARGET_UNIT_PASSENGER_2:
            case TARGET_UNIT_PASSENGER_3:
            case TARGET_UNIT_PASSENGER_4:
            case TARGET_UNIT_PASSENGER_5:
            case TARGET_UNIT_PASSENGER_6:
            case TARGET_UNIT_PASSENGER_7:
            case TARGET_UNIT_SUMMONER:
                SpellTargetType[i] = TARGET_TYPE_UNIT_CASTER;
                break;
            case TARGET_UNIT_TARGET_MINIPET:
            case TARGET_UNIT_TARGET_ALLY:
            case TARGET_UNIT_TARGET_RAID:
            case TARGET_UNIT_TARGET_ANY:
            case TARGET_UNIT_TARGET_ENEMY:
            case TARGET_UNIT_TARGET_PARTY:
            case TARGET_UNIT_TARGET_PASSENGER:
            case TARGET_UNIT_TARGET_ALLY_PARTY:
            case TARGET_UNIT_TARGET_CLASS_RAID:
            case TARGET_UNIT_CHAINHEAL:
                SpellTargetType[i] = TARGET_TYPE_UNIT_TARGET;
                break;
            case TARGET_UNIT_NEARBY_ENEMY:
            case TARGET_UNIT_NEARBY_ALLY:
            case TARGET_UNIT_NEARBY_ENTRY:
            case TARGET_UNIT_NEARBY_PARTY:
            case TARGET_UNIT_NEARBY_RAID:
            case TARGET_GAMEOBJECT_NEARBY_ENTRY:
                SpellTargetType[i] = TARGET_TYPE_UNIT_NEARBY;
                break;
            case TARGET_UNIT_AREA_ENEMY_SRC:
            case TARGET_UNIT_AREA_ALLY_SRC:
            case TARGET_UNIT_AREA_ENTRY_SRC:
            case TARGET_UNIT_AREA_PARTY_SRC:
            case TARGET_GAMEOBJECT_AREA_SRC:
                SpellTargetType[i] = TARGET_TYPE_AREA_SRC;
                break;
            case TARGET_UNIT_AREA_ENEMY_DST:
            case TARGET_UNIT_AREA_ALLY_DST:
            case TARGET_UNIT_AREA_ENTRY_DST:
            case TARGET_UNIT_AREA_PARTY_DST:
            case TARGET_GAMEOBJECT_AREA_DST:
                SpellTargetType[i] = TARGET_TYPE_AREA_DST;
                break;
            case TARGET_UNIT_CONE_ENEMY:
            case TARGET_UNIT_CONE_ALLY:
            case TARGET_UNIT_CONE_ENTRY:
            case TARGET_UNIT_CONE_ENEMY_UNKNOWN:
            case TARGET_UNIT_AREA_PATH:
            case TARGET_GAMEOBJECT_AREA_PATH:
                SpellTargetType[i] = TARGET_TYPE_AREA_CONE;
                break;
            case TARGET_DST_CASTER:
            case TARGET_SRC_CASTER:
            case TARGET_MINION:
            case TARGET_DEST_CASTER_FRONT_LEAP:
            case TARGET_DEST_CASTER_FRONT:
            case TARGET_DEST_CASTER_BACK:
            case TARGET_DEST_CASTER_RIGHT:
            case TARGET_DEST_CASTER_LEFT:
            case TARGET_DEST_CASTER_FRONT_LEFT:
            case TARGET_DEST_CASTER_BACK_LEFT:
            case TARGET_DEST_CASTER_BACK_RIGHT:
            case TARGET_DEST_CASTER_FRONT_RIGHT:
            case TARGET_DEST_CASTER_RANDOM:
            case TARGET_DEST_CASTER_RADIUS:
                SpellTargetType[i] = TARGET_TYPE_DEST_CASTER;
                break;
            case TARGET_DST_TARGET_ENEMY:
            case TARGET_DEST_TARGET_ANY:
            case TARGET_DEST_TARGET_FRONT:
            case TARGET_DEST_TARGET_BACK:
            case TARGET_DEST_TARGET_RIGHT:
            case TARGET_DEST_TARGET_LEFT:
            case TARGET_DEST_TARGET_FRONT_LEFT:
            case TARGET_DEST_TARGET_BACK_LEFT:
            case TARGET_DEST_TARGET_BACK_RIGHT:
            case TARGET_DEST_TARGET_FRONT_RIGHT:
            case TARGET_DEST_TARGET_RANDOM:
            case TARGET_DEST_TARGET_RADIUS:
                SpellTargetType[i] = TARGET_TYPE_DEST_TARGET;
                break;
            case TARGET_DEST_DYNOBJ_ENEMY:
            case TARGET_DEST_DYNOBJ_ALLY:
            case TARGET_DEST_DYNOBJ_NONE:
            case TARGET_DEST_DEST:
            case TARGET_DEST_TRAJ:
            case TARGET_DEST_DEST_FRONT_LEFT:
            case TARGET_DEST_DEST_BACK_LEFT:
            case TARGET_DEST_DEST_BACK_RIGHT:
            case TARGET_DEST_DEST_FRONT_RIGHT:
            case TARGET_DEST_DEST_FRONT:
            case TARGET_DEST_DEST_BACK:
            case TARGET_DEST_DEST_RIGHT:
            case TARGET_DEST_DEST_LEFT:
            case TARGET_DEST_DEST_RANDOM:
            case TARGET_DEST_DEST_RANDOM_DIR_DIST:
                SpellTargetType[i] = TARGET_TYPE_DEST_DEST;
                break;
            case TARGET_DST_DB:
            case TARGET_DST_HOME:
            case TARGET_DST_NEARBY_ENTRY:
                SpellTargetType[i] = TARGET_TYPE_DEST_SPECIAL;
                break;
            case TARGET_UNIT_CHANNEL_TARGET:
            case TARGET_DEST_CHANNEL_TARGET:
            case TARGET_DEST_CHANNEL_CASTER:
                SpellTargetType[i] = TARGET_TYPE_CHANNEL;
                break;
            default:
                SpellTargetType[i] = TARGET_TYPE_DEFAULT;
        }
    }

    for(int32 i = 0; i < TOTAL_SPELL_TARGETS; ++i)
    {
        switch(i)
        {
            case TARGET_UNIT_AREA_ENEMY_DST:
            case TARGET_UNIT_AREA_ENEMY_SRC:
            case TARGET_UNIT_AREA_ALLY_DST:
            case TARGET_UNIT_AREA_ALLY_SRC:
            case TARGET_UNIT_AREA_ENTRY_DST:
            case TARGET_UNIT_AREA_ENTRY_SRC:
            case TARGET_UNIT_AREA_PARTY_DST:
            case TARGET_UNIT_AREA_PARTY_SRC:
            case TARGET_UNIT_TARGET_ALLY_PARTY:
            case TARGET_UNIT_PARTY_CASTER:
            case TARGET_UNIT_CONE_ENEMY:
            case TARGET_UNIT_CONE_ALLY:
            case TARGET_UNIT_CONE_ENEMY_UNKNOWN:
            case TARGET_UNIT_AREA_PATH:
            case TARGET_GAMEOBJECT_AREA_PATH:
            case TARGET_UNIT_RAID_CASTER:
                IsAreaEffectTarget[i] = true;
                break;
            default:
                IsAreaEffectTarget[i] = false;
                break;
        }
    }
}

SpellMgr::~SpellMgr() { }

bool SpellMgr::IsSrcTargetSpell(SpellEntry const *spellInfo) const
{
    for(uint8 i = 0; i< MAX_SPELL_EFFECTS; ++i)
    {
        if(SpellTargetType[spellInfo->EffectImplicitTargetA[i]] == TARGET_TYPE_AREA_SRC || SpellTargetType[spellInfo->EffectImplicitTargetB[i]] == TARGET_TYPE_AREA_SRC)
            return true;
    }
    return false;
}

int32 GetSpellDuration(SpellEntry const *spellInfo)
{
    if(!spellInfo)
        return 0;
    SpellDurationEntry const* du = sSpellDurationStore.LookupEntry(spellInfo->DurationIndex);
    if(!du)
        return 0;
    return (du->Duration[0] == -1) ? -1 : abs(du->Duration[0]);
}

int32 GetSpellMaxDuration(SpellEntry const *spellInfo)
{
    if(!spellInfo)
        return 0;
    SpellDurationEntry const* du = sSpellDurationStore.LookupEntry(spellInfo->DurationIndex);
    if(!du)
        return 0;
    return (du->Duration[2] == -1) ? -1 : abs(du->Duration[2]);
}

uint32 GetDispelChance(Unit* auraCaster, Unit* target, uint32 spellId, bool offensive, bool *result)
{
    // we assume that aura dispel chance is 100% on start
    // need formula for level difference based chance
    int32 resist_chance = 0;

    // Apply dispel mod from aura caster
    if(auraCaster)
        if(Player* modOwner = auraCaster->GetSpellModOwner())
            modOwner->ApplySpellMod(spellId, SPELLMOD_RESIST_DISPEL_CHANCE, resist_chance);

    // Dispel resistance from target SPELL_AURA_MOD_DISPEL_RESIST
    // Only affects offensive dispels
    if(offensive && target)
        resist_chance += target->GetTotalAuraModifier(SPELL_AURA_MOD_DISPEL_RESIST);

    // Try dispel
    if(result)
        *result = !roll_chance_i(resist_chance);

    resist_chance = resist_chance < 0 ? 0 : resist_chance;
    resist_chance = resist_chance > 100 ? 100 : resist_chance;
    return 100 - resist_chance;
}

uint32 GetSpellCastTime(SpellEntry const* spellInfo, Spell* spell)
{
    SpellCastTimesEntry const* spellCastTimeEntry = sSpellCastTimesStore.LookupEntry(spellInfo->CastingTimeIndex);

    // not all spells have cast time index and this is all is pasiive abilities
    if(!spellCastTimeEntry)
        return 0;

    int32 castTime = spellCastTimeEntry->CastTime;

    if(Unit* caster = (spell ? spell->GetCaster() : NULL))
        caster->ModSpellCastTime(spellInfo, castTime, spell);

    if(spellInfo->Attributes & SPELL_ATTR0_REQ_AMMO && (!spell || !(spell->IsAutoRepeat())))
        castTime += 500;

    return (castTime > 0) ? uint32(castTime) : 0;
}

bool IsPassiveSpell(uint32 spellId)
{
    SpellEntry const* spellInfo = sSpellStore.LookupEntry(spellId);
    if(!spellInfo)
        return false;
    return IsPassiveSpell(spellInfo);
}

bool IsPassiveSpell(SpellEntry const* spellInfo)
{
    if(spellInfo->Attributes & SPELL_ATTR0_PASSIVE)
        return true;
    return false;
}

bool IsAutocastableSpell(uint32 spellId)
{
    SpellEntry const* spellInfo = sSpellStore.LookupEntry(spellId);
    if(!spellInfo)
        return false;
    if(spellInfo->Attributes & SPELL_ATTR0_PASSIVE)
        return false;
    if(spellInfo->AttributesEx & SPELL_ATTR1_UNAUTOCASTABLE_BY_PET)
        return false;
    return true;
}

bool IsHigherHankOfSpell(uint32 spellId_1, uint32 spellId_2)
{
    return sSpellMgr->GetSpellRank(spellId_1) < sSpellMgr->GetSpellRank(spellId_2);
}

uint32 CalculatePowerCost(SpellEntry const* spellInfo, Unit const* caster, SpellSchoolMask schoolMask)
{
    // Spell drain all exist power on cast (Only paladin lay of Hands)
    if(spellInfo->AttributesEx & SPELL_ATTR1_DRAIN_ALL_POWER)
    {
        // If power type - health drain all
        if(spellInfo->powerType == POWER_HEALTH)
            return caster->GetHealth();
        // Else drain all power
        if(spellInfo->powerType < MAX_POWERS)
            return caster->GetPower(Powers(spellInfo->powerType));
        sLog->outError("CalculateManaCost: Unknown power type '%d' in spell %d", spellInfo->powerType, spellInfo->Id);
        return 0;
    }

    // Base powerCost
    int32 powerCost = spellInfo->manaCost;
    // PCT cost from total amount
    if(spellInfo->ManaCostPercentage)
    {
        switch(spellInfo->powerType)
        {
            // health as power used
            case POWER_HEALTH:
                powerCost += int32(CalculatePctU(caster->GetCreateHealth(), spellInfo->ManaCostPercentage));
                break;
            case POWER_MANA:
                powerCost += int32(CalculatePctU(caster->GetCreateMana(), spellInfo->ManaCostPercentage));
                break;
            case POWER_RAGE:
            case POWER_FOCUS:
            case POWER_ENERGY:
            case POWER_HAPPINESS:
                powerCost += int32(CalculatePctU(caster->GetMaxPower(Powers(spellInfo->powerType)), spellInfo->ManaCostPercentage));
                break;
            case POWER_RUNE:
            case POWER_RUNIC_POWER:
                sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "CalculateManaCost: Not implemented yet!");
                break;
            default:
                sLog->outError("CalculateManaCost: Unknown power type '%d' in spell %d", spellInfo->powerType, spellInfo->Id);
                return 0;
        }
    }
    SpellSchools school = GetFirstSchoolInMask(schoolMask);
    // Flat mod from caster auras by spell school
    powerCost += caster->GetInt32Value(UNIT_FIELD_POWER_COST_MODIFIER + school);
    // Shiv - costs 20 + weaponSpeed*10 energy (apply only to non-triggered spell with energy cost)
    if(spellInfo->AttributesEx4 & SPELL_ATTR4_SPELL_VS_EXTEND_COST)
        powerCost += caster->GetAttackTime(OFF_ATTACK) / 100;
    // Apply cost mod by spell
    if(Player* modOwner = caster->GetSpellModOwner())
        modOwner->ApplySpellMod(spellInfo->Id, SPELLMOD_COST, powerCost);

    if(spellInfo->Attributes & SPELL_ATTR0_LEVEL_DAMAGE_CALCULATION)
        powerCost = int32(powerCost / (1.117f * spellInfo->spellLevel / caster->getLevel() -0.1327f));

    // PCT mod from user auras by school
    powerCost = int32(powerCost * (1.0f + caster->GetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER + school)));
    if(powerCost < 0)
        powerCost = 0;
    return powerCost;
}

bool IsSpellRequiringFocusedTarget(SpellEntry const* spellInfo)
{
    for(uint8 i = 0 ; i < MAX_SPELL_EFFECTS; ++i)
    {
        if(SpellTargetType[spellInfo->EffectImplicitTargetA[i]] == TARGET_TYPE_UNIT_TARGET
            || SpellTargetType[spellInfo->EffectImplicitTargetB[i]] == TARGET_TYPE_UNIT_TARGET
            || SpellTargetType[spellInfo->EffectImplicitTargetA[i]] == TARGET_TYPE_CHANNEL
            || SpellTargetType[spellInfo->EffectImplicitTargetB[i]] == TARGET_TYPE_CHANNEL
            || SpellTargetType[spellInfo->EffectImplicitTargetA[i]] == TARGET_TYPE_DEST_TARGET
            || SpellTargetType[spellInfo->EffectImplicitTargetB[i]] == TARGET_TYPE_DEST_TARGET)
            return true;
    }
    return false;
}

Unit* GetTriggeredSpellCaster(SpellEntry const* spellInfo, Unit* caster, Unit* target)
{
    if(IsSpellRequiringFocusedTarget(spellInfo))
        return caster;
    return target;
}

AuraState GetSpellAuraState(SpellEntry const* spellInfo)
{
    // Seals
    if(IsSealSpell(spellInfo))
        return AURA_STATE_JUDGEMENT;

    // Conflagrate aura state on Immolate and Shadowflame
    if(spellInfo->SpellFamilyName == SPELLFAMILY_WARLOCK &&
        // Immolate
        ((spellInfo->SpellFamilyFlags[0] & 4) ||
        // Shadowflame
        (spellInfo->SpellFamilyFlags[2] & 2)))
        return AURA_STATE_CONFLAGRATE;

    // Faerie Fire (druid versions)
    if(spellInfo->SpellFamilyName == SPELLFAMILY_DRUID && spellInfo->SpellFamilyFlags[0] & 0x400)
        return AURA_STATE_FAERIE_FIRE;

    // Sting (hunter's pet ability)
    if(spellInfo->Category == 1133)
        return AURA_STATE_FAERIE_FIRE;

    // Victorious
    if(spellInfo->SpellFamilyName == SPELLFAMILY_WARRIOR &&  spellInfo->SpellFamilyFlags[1] & 0x00040000)
        return AURA_STATE_WARRIOR_VICTORY_RUSH;

    // Swiftmend state on Regrowth & Rejuvenation
    if(spellInfo->SpellFamilyName == SPELLFAMILY_DRUID && spellInfo->SpellFamilyFlags[0] & 0x50)
        return AURA_STATE_SWIFTMEND;

    // Deadly poison aura state
    if(spellInfo->SpellFamilyName == SPELLFAMILY_ROGUE && spellInfo->SpellFamilyFlags[0] & 0x10000)
        return AURA_STATE_DEADLY_POISON;

    // Enrage aura state
    if(spellInfo->Dispel == DISPEL_ENRAGE)
        return AURA_STATE_ENRAGE;

    // Bleeding aura state
    if(GetAllSpellMechanicMask(spellInfo) & 1<<MECHANIC_BLEED)
        return AURA_STATE_BLEEDING;

    if(GetSpellSchoolMask(spellInfo) & SPELL_SCHOOL_MASK_FROST)
        for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            if(spellInfo->EffectApplyAuraName[i] == SPELL_AURA_MOD_STUN
                || spellInfo->EffectApplyAuraName[i] == SPELL_AURA_MOD_ROOT)
                return AURA_STATE_FROZEN;

    switch(spellInfo->Id)
    {
        case 71465: // Divine Surge
            return AURA_STATE_UNKNOWN22;
        default:
            break;
    }

    return AURA_STATE_NONE;
}

SpellSpecific GetSpellSpecific(SpellEntry const* spellInfo)
{
    switch(spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
        {
            // Food / Drinks (mostly)
            if(spellInfo->AuraInterruptFlags & AURA_INTERRUPT_FLAG_NOT_SEATED)
            {
                bool food = false;
                bool drink = false;
                for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                {
                    switch(spellInfo->EffectApplyAuraName[i])
                    {
                        // Food
                        case SPELL_AURA_MOD_REGEN:
                        case SPELL_AURA_OBS_MOD_HEALTH:
                            food = true;
                            break;
                        // Drink
                        case SPELL_AURA_MOD_POWER_REGEN:
                        case SPELL_AURA_OBS_MOD_POWER:
                            drink = true;
                            break;
                        default:
                            break;
                    }
                }

                if(food && drink)
                    return SPELL_SPECIFIC_FOOD_AND_DRINK;
                else if(food)
                    return SPELL_SPECIFIC_FOOD;
                else if(drink)
                    return SPELL_SPECIFIC_DRINK;
            }
            // scrolls effects
            else
            {
                uint32 firstSpell = sSpellMgr->GetFirstSpellInChain(spellInfo->Id);
                switch(firstSpell)
                {
                    case 8118: // Strength
                    case 8099: // Stamina
                    case 8112: // Spirit
                    case 8096: // Intellect
                    case 8115: // Agility
                    case 8091: // Armor
                        return SPELL_SPECIFIC_SCROLL;
                    case 12880: // Enrage (Enrage)
                    case 57518: // Enrage (Wrecking Crew)
                        return SPELL_SPECIFIC_WARRIOR_ENRAGE;
                }
            }
            break;
        }
        case SPELLFAMILY_MAGE:
        {
            // family flags 18(Molten), 25(Frost/Ice), 28(Mage)
            if(spellInfo->SpellFamilyFlags[0] & 0x12040000)
                return SPELL_SPECIFIC_MAGE_ARMOR;

            // Arcane brillance and Arcane intelect (normal check fails because of flags difference)
            if(spellInfo->SpellFamilyFlags[0] & 0x400)
                return SPELL_SPECIFIC_MAGE_ARCANE_BRILLANCE;

            if((spellInfo->SpellFamilyFlags[0] & 0x1000000) && spellInfo->EffectApplyAuraName[0] == SPELL_AURA_MOD_CONFUSE)
                return SPELL_SPECIFIC_MAGE_POLYMORPH;

            break;
        }
        case SPELLFAMILY_WARRIOR:
        {
            if(spellInfo->Id == 12292) // Death Wish
                return SPELL_SPECIFIC_WARRIOR_ENRAGE;

            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            // only warlock curses have this
            if(spellInfo->Dispel == DISPEL_CURSE)
                return SPELL_SPECIFIC_CURSE;

            // Warlock (Demon Armor | Demon Skin | Fel Armor)
            if(spellInfo->SpellFamilyFlags[1] & 0x20000020 || spellInfo->SpellFamilyFlags[2] & 0x00000010)
                return SPELL_SPECIFIC_WARLOCK_ARMOR;

            //seed of corruption and corruption
            if(spellInfo->SpellFamilyFlags[1] & 0x10 || spellInfo->SpellFamilyFlags[0] & 0x2)
                return SPELL_SPECIFIC_WARLOCK_CORRUPTION;
            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            // Divine Spirit and Prayer of Spirit
            if(spellInfo->SpellFamilyFlags[0] & 0x20)
                return SPELL_SPECIFIC_PRIEST_DIVINE_SPIRIT;

            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            // only hunter stings have this
            if(spellInfo->Dispel == DISPEL_POISON)
                return SPELL_SPECIFIC_STING;

            // only hunter aspects have this (but not all aspects in hunter family)
            if(spellInfo->SpellFamilyFlags.HasFlag(0x00380000, 0x00440000, 0x00001010))
                return SPELL_SPECIFIC_ASPECT;

            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            if(IsSealSpell(spellInfo))
                return SPELL_SPECIFIC_SEAL;

            if(spellInfo->SpellFamilyFlags[0] & 0x00002190)
                return SPELL_SPECIFIC_HAND;

            // Judgement of Wisdom, Judgement of Light, Judgement of Justice
            if(spellInfo->Id == 20184 || spellInfo->Id == 20185 || spellInfo->Id == 20186)
                return SPELL_SPECIFIC_JUDGEMENT;

            // only paladin auras have this (for palaldin class family)
            if(spellInfo->SpellFamilyFlags[2] & 0x00000020)
                return SPELL_SPECIFIC_AURA;

            break;
        }
        case SPELLFAMILY_SHAMAN:
        {
            if(IsElementalShield(spellInfo))
                return SPELL_SPECIFIC_ELEMENTAL_SHIELD;

            break;
        }

        case SPELLFAMILY_DEATHKNIGHT:
            if(spellInfo->Id == 48266 || spellInfo->Id == 48263 || spellInfo->Id == 48265)
            //if(spellInfo->Category == 47)
                return SPELL_SPECIFIC_PRESENCE;
            break;
    }

    for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if(spellInfo->Effect[i] == SPELL_EFFECT_APPLY_AURA)
        {
            switch(spellInfo->EffectApplyAuraName[i])
            {
                case SPELL_AURA_MOD_CHARM:
                case SPELL_AURA_MOD_POSSESS_PET:
                case SPELL_AURA_MOD_POSSESS:
                case SPELL_AURA_AOE_CHARM:
                    return SPELL_SPECIFIC_CHARM;
                case SPELL_AURA_TRACK_CREATURES:
                case SPELL_AURA_TRACK_RESOURCES:
                case SPELL_AURA_TRACK_STEALTHED:
                    return SPELL_SPECIFIC_TRACKER;
                case SPELL_AURA_PHASE:
                    return SPELL_SPECIFIC_PHASE;
            }
        }
    }

    return SPELL_SPECIFIC_NORMAL;
}

// target not allow have more one spell specific from same caster
bool IsSingleFromSpellSpecificPerCaster(SpellSpecific spellSpec1, SpellSpecific spellSpec2)
{
    switch(spellSpec1)
    {
        case SPELL_SPECIFIC_SEAL:
        case SPELL_SPECIFIC_HAND:
        case SPELL_SPECIFIC_AURA:
        case SPELL_SPECIFIC_STING:
        case SPELL_SPECIFIC_CURSE:
        case SPELL_SPECIFIC_ASPECT:
        case SPELL_SPECIFIC_JUDGEMENT:
        case SPELL_SPECIFIC_WARLOCK_CORRUPTION:
            return spellSpec1 == spellSpec2;
        default:
            return false;
    }
}

bool IsSingleFromSpellSpecificPerTarget(SpellSpecific spellSpec1, SpellSpecific spellSpec2)
{
    switch(spellSpec1)
    {
        case SPELL_SPECIFIC_PHASE:
        case SPELL_SPECIFIC_TRACKER:
        case SPELL_SPECIFIC_WARLOCK_ARMOR:
        case SPELL_SPECIFIC_MAGE_ARMOR:
        case SPELL_SPECIFIC_ELEMENTAL_SHIELD:
        case SPELL_SPECIFIC_MAGE_POLYMORPH:
        case SPELL_SPECIFIC_PRESENCE:
        case SPELL_SPECIFIC_CHARM:
        case SPELL_SPECIFIC_SCROLL:
        case SPELL_SPECIFIC_WARRIOR_ENRAGE:
        case SPELL_SPECIFIC_MAGE_ARCANE_BRILLANCE:
        case SPELL_SPECIFIC_PRIEST_DIVINE_SPIRIT:
            return spellSpec1 == spellSpec2;
        case SPELL_SPECIFIC_FOOD:
            return spellSpec2 == SPELL_SPECIFIC_FOOD
                || spellSpec2 == SPELL_SPECIFIC_FOOD_AND_DRINK;
        case SPELL_SPECIFIC_DRINK:
            return spellSpec2 == SPELL_SPECIFIC_DRINK
                || spellSpec2 == SPELL_SPECIFIC_FOOD_AND_DRINK;
        case SPELL_SPECIFIC_FOOD_AND_DRINK:
            return spellSpec2 == SPELL_SPECIFIC_FOOD
                || spellSpec2 == SPELL_SPECIFIC_DRINK
                || spellSpec2 == SPELL_SPECIFIC_FOOD_AND_DRINK;
        default:
            return false;
    }
}

bool IsPositiveTarget(uint32 targetA, uint32 targetB)
{
    // non-positive targets
    switch(targetA)
    {
        case TARGET_UNIT_NEARBY_ENEMY:
        case TARGET_UNIT_TARGET_ENEMY:
        case TARGET_UNIT_AREA_ENEMY_SRC:
        case TARGET_UNIT_AREA_ENEMY_DST:
        case TARGET_UNIT_CONE_ENEMY:
        case TARGET_UNIT_AREA_PATH:
        case TARGET_DEST_DYNOBJ_ENEMY:
        case TARGET_DST_TARGET_ENEMY:
            return false;
        default:
            break;
    }
    if(targetB)
        return IsPositiveTarget(targetB, 0);
    return true;
}

bool SpellMgr::_isPositiveEffect(uint32 spellId, uint32 effIndex, bool deep) const
{
    SpellEntry const* spellproto = sSpellStore.LookupEntry(spellId);
    if(!spellproto)
        return false;

    // not found a single positive spell with this attribute
    if(spellproto->Attributes & SPELL_ATTR0_NEGATIVE_1)
        return false;

    switch(spellproto->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
            switch(spellId)
            {
                case 34700: // Allergic Reaction
                case 34709: // Shadow Sight
                case 61987: // Avenging Wrath Marker
                case 61988: // Divine Shield exclude aura
                case 62532: // Conservator's Grip
                case 50524: // Runic Power Feed
                    return false;
                case 61716: // Rabbit Costume
                case 61734: // Noblegarden Bunny
                case 30877: // Tag Murloc
                case 62344: // Fists of Stone
                    return true;
                default:
                    break;
            }
            break;
        case SPELLFAMILY_ROGUE:
            // Envenom
            if(spellproto->SpellFamilyFlags[1] & 0x8)
                return true;
            // Slice and Dice
            else if(spellproto->SpellFamilyFlags[0] & 0x40000)
                return true;
            break;
        case SPELLFAMILY_MAGE:
            // Amplify Magic, Dampen Magic
            if(spellproto->SpellFamilyFlags[0] == 0x00002000)
                return true;
            // Ignite
            if(spellproto->SpellIconID == 45)
                return true;
            break;
        case SPELLFAMILY_WARRIOR:
            // Shockwave
            if(spellId == 46968)
                return false;
            break;
        case SPELLFAMILY_PRIEST:
            switch(spellId)
            {
                case 64844: // Divine Hymn
                case 64904: // Hymn of Hope
                case 47585: // Dispersion
                    return true;
                case 527:   // Dispel rank 1
                case 988:   // Dispel rank 2
                    return false;
                default:
                    break;
            }
            break;
        case SPELLFAMILY_HUNTER:
            // Aspect of the Viper
            if(spellId == 34074)
                return true;
            break;
        case SPELLFAMILY_SHAMAN:
            if(spellId == 30708)
                return false;
            break;
        default:
            break;
    }

    switch(spellproto->Mechanic)
    {
        case MECHANIC_IMMUNE_SHIELD:
            return true;
        default:
            break;
    }

    // Special case: effects which determine positivity of whole spell
    for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if(spellproto->EffectApplyAuraName[i] == SPELL_AURA_MOD_STEALTH)
            return true;
    }

    switch(spellproto->Effect[effIndex])
    {
        case SPELL_EFFECT_DUMMY:
            // some explicitly required dummy effect sets
            switch(spellId)
            {
                case 28441:
                    return false; // AB Effect 000
                default:
                    break;
            }
            break;
        // always positive effects (check before target checks that provided non-positive result in some case for positive effects)
        case SPELL_EFFECT_HEAL:
        case SPELL_EFFECT_LEARN_SPELL:
        case SPELL_EFFECT_SKILL_STEP:
        case SPELL_EFFECT_HEAL_PCT:
        case SPELL_EFFECT_ENERGIZE_PCT:
            return true;

            // non-positive aura use
        case SPELL_EFFECT_APPLY_AURA:
        case SPELL_EFFECT_APPLY_AREA_AURA_FRIEND:
        {
            switch(spellproto->EffectApplyAuraName[effIndex])
            {
                case SPELL_AURA_MOD_DAMAGE_DONE:            // dependent from bas point sign (negative -> negative)
                case SPELL_AURA_MOD_STAT:
                case SPELL_AURA_MOD_SKILL:
                case SPELL_AURA_MOD_DODGE_PERCENT:
                case SPELL_AURA_MOD_HEALING_PCT:
                case SPELL_AURA_MOD_HEALING_DONE:
                case SPELL_AURA_MOD_DAMAGE_PERCENT_DONE:
                    if(SpellMgr::CalculateSpellEffectAmount(spellproto, effIndex) < 0)
                        return false;
                    break;
                case SPELL_AURA_MOD_DAMAGE_TAKEN:           // dependent from bas point sign (positive -> negative)
                    if(SpellMgr::CalculateSpellEffectAmount(spellproto, effIndex) > 0)
                        return false;
                    break;
                case SPELL_AURA_MOD_CRIT_PCT:
                case SPELL_AURA_MOD_SPELL_CRIT_CHANCE:
                    if(SpellMgr::CalculateSpellEffectAmount(spellproto, effIndex) > 0)
                        return true;                        // some expected positive spells have SPELL_ATTR1_NEGATIVE
                    break;
                case SPELL_AURA_ADD_TARGET_TRIGGER:
                    return true;
                case SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE:
                case SPELL_AURA_PERIODIC_TRIGGER_SPELL:
                    if(!deep)
                    {
                        uint32 spellTriggeredId = spellproto->EffectTriggerSpell[effIndex];
                        SpellEntry const* spellTriggeredProto = sSpellStore.LookupEntry(spellTriggeredId);

                        if(spellTriggeredProto)
                        {
                            // non-positive targets of main spell return early
                            for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                            {
                                if(!spellTriggeredProto->Effect[i])
                                    continue;
                                // if non-positive trigger cast targeted to positive target this main cast is non-positive
                                // this will place this spell auras as debuffs
                                if(IsPositiveTarget(spellTriggeredProto->EffectImplicitTargetA[effIndex], spellTriggeredProto->EffectImplicitTargetB[effIndex]) && !_isPositiveEffect(spellTriggeredId, i, true))
                                    return false;
                            }
                        }
                    }
                case SPELL_AURA_PROC_TRIGGER_SPELL:
                    // many positive auras have negative triggered spells at damage for example and this not make it negative (it can be canceled for example)
                    break;
                case SPELL_AURA_MOD_STUN:                   //have positive and negative spells, we can't sort its correctly at this moment.
                    if(effIndex == 0 && spellproto->Effect[1] == 0 && spellproto->Effect[2] == 0)
                        return false;                       // but all single stun aura spells is negative
                    break;
                case SPELL_AURA_MOD_PACIFY_SILENCE:
                    if(spellproto->Id == 24740)             // Wisp Costume
                        return true;
                    return false;
                case SPELL_AURA_MOD_ROOT:
                case SPELL_AURA_MOD_SILENCE:
                case SPELL_AURA_GHOST:
                case SPELL_AURA_PERIODIC_LEECH:
                case SPELL_AURA_MOD_STALKED:
                case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
                case SPELL_AURA_PREVENT_RESSURECTION:
                    return false;
                case SPELL_AURA_PERIODIC_DAMAGE:            // used in positive spells also.
                    // part of negative spell if casted at self (prevent cancel)
                    if(spellproto->EffectImplicitTargetA[effIndex] == TARGET_UNIT_CASTER)
                        return false;
                    break;
                case SPELL_AURA_MOD_DECREASE_SPEED:         // used in positive spells also
                    // part of positive spell if casted at self
                    if(spellproto->EffectImplicitTargetA[effIndex] != TARGET_UNIT_CASTER)
                        return false;
                    // but not this if this first effect (didn't find better check)
                    if(spellproto->Attributes & SPELL_ATTR0_NEGATIVE_1 && effIndex == 0)
                        return false;
                    break;
                case SPELL_AURA_MECHANIC_IMMUNITY:
                {
                    // non-positive immunities
                    switch(spellproto->EffectMiscValue[effIndex])
                    {
                        case MECHANIC_BANDAGE:
                        case MECHANIC_SHIELD:
                        case MECHANIC_MOUNT:
                        case MECHANIC_INVULNERABILITY:
                            return false;
                        default:
                            break;
                    }
                    break;
                }
                case SPELL_AURA_ADD_FLAT_MODIFIER:          // mods
                case SPELL_AURA_ADD_PCT_MODIFIER:
                {
                    // non-positive mods
                    switch(spellproto->EffectMiscValue[effIndex])
                    {
                        case SPELLMOD_COST:                 // dependent from bas point sign (negative -> positive)
                            if(SpellMgr::CalculateSpellEffectAmount(spellproto, effIndex) > 0)
                            {
                                if(!deep)
                                {
                                    bool negative = true;
                                    for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                                    {
                                        if(i != effIndex)
                                            if(_isPositiveEffect(spellId, i, true))
                                            {
                                                negative = false;
                                                break;
                                            }
                                    }
                                    if(negative)
                                        return false;
                                }
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }

    // non-positive targets
    if(!IsPositiveTarget(spellproto->EffectImplicitTargetA[effIndex], spellproto->EffectImplicitTargetB[effIndex]))
        return false;

    if(!deep && spellproto->EffectTriggerSpell[effIndex]
        && !spellproto->EffectApplyAuraName[effIndex]
        && IsPositiveTarget(spellproto->EffectImplicitTargetA[effIndex], spellproto->EffectImplicitTargetB[effIndex])
        && !_isPositiveSpell(spellproto->EffectTriggerSpell[effIndex], true))
        return false;

    // ok, positive
    return true;
}

bool IsPositiveSpell(uint32 spellId)
{
    if(!sSpellStore.LookupEntry(spellId)) // non-existing spells
        return false;
    return !(sSpellMgr->GetSpellCustomAttr(spellId) & SPELL_ATTR0_CU_NEGATIVE);
}

bool IsPositiveEffect(uint32 spellId, uint32 effIndex)
{
    if(!sSpellStore.LookupEntry(spellId))
        return false;
    switch(effIndex)
    {
        default:
        case 0:
            return !(sSpellMgr->GetSpellCustomAttr(spellId) & SPELL_ATTR0_CU_NEGATIVE_EFF0);
        case 1:
            return !(sSpellMgr->GetSpellCustomAttr(spellId) & SPELL_ATTR0_CU_NEGATIVE_EFF1);
        case 2:
            return !(sSpellMgr->GetSpellCustomAttr(spellId) & SPELL_ATTR0_CU_NEGATIVE_EFF2);
    }
}

bool SpellMgr::_isPositiveSpell(uint32 spellId, bool deep) const
{
    SpellEntry const* spellproto = sSpellStore.LookupEntry(spellId);
    if(!spellproto)
        return false;

    // spells with at least one negative effect are considered negative
    // some self-applied spells have negative effects but in self casting case negative check ignored.
    for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if(!_isPositiveEffect(spellId, i, deep))
            return false;
    return true;
}

bool IsSingleTargetSpell(SpellEntry const* spellInfo)
{
    // all other single target spells have if it has AttributesEx5
    if(spellInfo->AttributesEx5 & SPELL_ATTR5_SINGLE_TARGET_SPELL)
        return true;

    switch(GetSpellSpecific(spellInfo))
    {
        case SPELL_SPECIFIC_JUDGEMENT:
            return true;
        default:
            break;
    }

    return false;
}

bool IsSingleTargetSpells(SpellEntry const* spellInfo1, SpellEntry const* spellInfo2)
{
    // TODO - need better check
    // Equal icon and spellfamily
    if(spellInfo1->SpellFamilyName == spellInfo2->SpellFamilyName &&
        spellInfo1->SpellIconID == spellInfo2->SpellIconID)
        return true;

    // TODO - need found Judgements rule
    SpellSpecific spec1 = GetSpellSpecific(spellInfo1);
    // spell with single target specific types
    switch(spec1)
    {
        case SPELL_SPECIFIC_JUDGEMENT:
        case SPELL_SPECIFIC_MAGE_POLYMORPH:
            if(GetSpellSpecific(spellInfo2) == spec1)
                return true;
            break;
        default:
            break;
    }

    return false;
}

SpellCastResult GetErrorAtShapeshiftedCast(SpellEntry const* spellInfo, uint32 form)
{
    // talents that learn spells can have stance requirements that need ignore
    // (this requirement only for client-side stance show in talent description)
    if(GetTalentSpellCost(spellInfo->Id) > 0 &&
        (spellInfo->Effect[0] == SPELL_EFFECT_LEARN_SPELL || spellInfo->Effect[1] == SPELL_EFFECT_LEARN_SPELL || spellInfo->Effect[2] == SPELL_EFFECT_LEARN_SPELL))
        return SPELL_CAST_OK;

    uint32 stanceMask = (form ? 1 << (form - 1) : 0);

    if(stanceMask & spellInfo->StancesNot)                 // can explicitly not be casted in this stance
        return SPELL_FAILED_NOT_SHAPESHIFT;

    if(stanceMask & spellInfo->Stances)                    // can explicitly be casted in this stance
        return SPELL_CAST_OK;

    bool actAsShifted = false;
    SpellShapeshiftEntry const* shapeInfo = NULL;
    if(form > 0)
    {
        shapeInfo = sSpellShapeshiftStore.LookupEntry(form);
        if(!shapeInfo)
        {
            sLog->outError("GetErrorAtShapeshiftedCast: unknown shapeshift %u", form);
            return SPELL_CAST_OK;
        }
        actAsShifted = !(shapeInfo->flags1 & 1);            // shapeshift acts as normal form for spells
    }

    if(actAsShifted)
    {
        if(spellInfo->Attributes & SPELL_ATTR0_NOT_SHAPESHIFT) // not while shapeshifted
            return SPELL_FAILED_NOT_SHAPESHIFT;
        else if(spellInfo->Stances != 0)                   // needs other shapeshift
            return SPELL_FAILED_ONLY_SHAPESHIFT;
    }
    else
    {
        // needs shapeshift
        if(!(spellInfo->AttributesEx2 & SPELL_ATTR2_NOT_NEED_SHAPESHIFT) && spellInfo->Stances != 0)
            return SPELL_FAILED_ONLY_SHAPESHIFT;
    }

    // Check if stance disables cast of not-stance spells
    // Example: cannot cast any other spells in zombie or ghoul form
    // TODO: Find a way to disable use of these spells clientside
    if(shapeInfo && shapeInfo->flags1 & 0x400)
    {
        if(!(stanceMask & spellInfo->Stances))
            return SPELL_FAILED_ONLY_SHAPESHIFT;
    }

    return SPELL_CAST_OK;
}

SpellThreatEntry const* SpellMgr::GetSpellThreatEntry(uint32 spellID) const
{
    SpellThreatMap::const_iterator itr = mSpellThreatMap.find(spellID);
    if(itr != mSpellThreatMap.end())
        return &itr->second;
    else
    {
        uint32 firstSpell = GetFirstSpellInChain(spellID);
        SpellThreatMap::const_iterator itr = mSpellThreatMap.find(firstSpell);
        if(itr != mSpellThreatMap.end())
            return &itr->second;
    }
    return NULL;
}

void SpellMgr::LoadSpellTargetPositions()
{
    uint32 oldMSTime = getMSTime();

    mSpellTargetPositions.clear();                                // need for reload case

    //                                                0  1           2                  3                  4                  5
    QueryResult result = WorldDatabase.Query("SELECT id, target_map, target_position_x, target_position_y, target_position_z, target_orientation FROM spell_target_position");
    if(!result)
    {
        sLog->outString(">> Loaded 0 spell target coordinates. DB table `spell_target_position` is empty.");
        sLog->outString();
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 Spell_ID = fields[0].GetUInt32();

        SpellTargetPosition st;

        st.target_mapId       = fields[1].GetUInt32();
        st.target_X           = fields[2].GetFloat();
        st.target_Y           = fields[3].GetFloat();
        st.target_Z           = fields[4].GetFloat();
        st.target_Orientation = fields[5].GetFloat();

        MapEntry const* mapEntry = sMapStore.LookupEntry(st.target_mapId);
        if(!mapEntry)
        {
            sLog->outErrorDb("Spell (ID:%u) target map (ID: %u) does not exist in `Map.dbc`.", Spell_ID, st.target_mapId);
            continue;
        }

        if(st.target_X==0 && st.target_Y==0 && st.target_Z==0)
        {
            sLog->outErrorDb("Spell (ID:%u) target coordinates not provided.", Spell_ID);
            continue;
        }

        SpellEntry const* spellInfo = sSpellStore.LookupEntry(Spell_ID);
        if(!spellInfo)
        {
            sLog->outErrorDb("Spell (ID:%u) listed in `spell_target_position` does not exist.", Spell_ID);
            continue;
        }

        bool found = false;
        for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if(spellInfo->EffectImplicitTargetA[i] == TARGET_DST_DB || spellInfo->EffectImplicitTargetB[i] == TARGET_DST_DB)
            {
                // additional requirements
                if(spellInfo->Effect[i]==SPELL_EFFECT_BIND && spellInfo->EffectMiscValue[i])
                {
                    uint32 area_id = sMapMgr->GetAreaId(st.target_mapId, st.target_X, st.target_Y, st.target_Z);
                    if(area_id != uint32(spellInfo->EffectMiscValue[i]))
                    {
                        sLog->outErrorDb("Spell (Id: %u) listed in `spell_target_position` expected point to zone %u bit point to zone %u.", Spell_ID, spellInfo->EffectMiscValue[i], area_id);
                        break;
                    }
                }

                found = true;
                break;
            }
        }
        if(!found)
        {
            sLog->outErrorDb("Spell (Id: %u) listed in `spell_target_position` does not have target TARGET_DST_DB (17).", Spell_ID);
            continue;
        }

        mSpellTargetPositions[Spell_ID] = st;
    } while(result->NextRow());

    // Check all spells
    for(uint32 i = 1; i < sSpellStore.GetNumRows(); ++i)
    {
        SpellEntry const* spellInfo = sSpellStore.LookupEntry(i);
        if(!spellInfo)
            continue;

        bool found = false;
        for(int j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            switch(spellInfo->EffectImplicitTargetA[j])
            {
                case TARGET_DST_DB:
                    found = true;
                    break;
            }
            if(found)
                break;
            switch(spellInfo->EffectImplicitTargetB[j])
            {
                case TARGET_DST_DB:
                    found = true;
                    break;
            }
            if(found)
                break;
        }
    }

    sLog->outString(">> Loaded spell teleport coordinates in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

bool SpellMgr::IsAffectedByMod(SpellEntry const *spellInfo, SpellModifier *mod) const
{
    // false for spellInfo == NULL
    if(!spellInfo || !mod)
        return false;

    SpellEntry const* affect_spell = sSpellStore.LookupEntry(mod->spellId);
    // False if affect_spell == NULL or spellFamily not equal
    if(!affect_spell || affect_spell->SpellFamilyName != spellInfo->SpellFamilyName)
        return false;

    // true
    if(mod->mask  & spellInfo->SpellFamilyFlags)
        return true;

    return false;
}

void SpellMgr::LoadSpellProcEvents()
{
    uint32 oldMSTime = getMSTime();

    mSpellProcEventMap.clear();                             // need for reload case

    //                                                0      1           2                3                 4                 5                 6          7       8        9             10
    QueryResult result = WorldDatabase.Query("SELECT entry, SchoolMask, SpellFamilyName, SpellFamilyMask0, SpellFamilyMask1, SpellFamilyMask2, procFlags, procEx, ppmRate, CustomChance, Cooldown FROM spell_proc_event");
    if(!result)
    {
        sLog->outString(">> Loaded 0 spell proc event conditions");
        sLog->outString();
        return;
    }

    uint32 customProc = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        SpellEntry const* spell = sSpellStore.LookupEntry(entry);
        if(!spell)
        {
            sLog->outErrorDb("Spell %u listed in `spell_proc_event` does not exist", entry);
            continue;
        }

        SpellProcEventEntry spe;

        spe.schoolMask      = fields[1].GetUInt32();
        spe.spellFamilyName = fields[2].GetUInt32();
        spe.spellFamilyMask[0] = fields[3].GetUInt32();
        spe.spellFamilyMask[1] = fields[4].GetUInt32();
        spe.spellFamilyMask[2] = fields[5].GetUInt32();
        spe.procFlags       = fields[6].GetUInt32();
        spe.procEx          = fields[7].GetUInt32();
        spe.ppmRate         = fields[8].GetFloat();
        spe.customChance    = fields[9].GetFloat();
        spe.cooldown        = fields[10].GetUInt32();

        mSpellProcEventMap.insert(std::pair<uint32,SpellProcEventEntry>(entry, spe));

        if(spell->procFlags == 0)
        {
            if(spe.procFlags == 0)
            {
                sLog->outErrorDb("Spell %u listed in `spell_proc_event` probally not triggered spell", entry);
                continue;
            }
            customProc++;
        }
    } while(result->NextRow());

    if(customProc)
        sLog->outString(">> Loaded extra and %u custom spell proc event conditions in %u ms", customProc, GetMSTimeDiffToNow(oldMSTime));
    else
        sLog->outString(">> Loaded extra spell proc event conditions in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

bool SpellMgr::IsSpellProcEventCanTriggeredBy(SpellProcEventEntry const* spellProcEvent, uint32 EventProcFlag, SpellEntry const* procSpell, uint32 procFlags, uint32 procExtra, bool active)
{
    // No extra req need
    uint32 procEvent_procEx = PROC_EX_NONE;

    // check prockFlags for condition
    if((procFlags & EventProcFlag) == 0)
        return false;

    bool hasFamilyMask = false;

    /* Check Periodic Auras

    *Dots can trigger if spell has no PROC_FLAG_SUCCESSFUL_NEGATIVE_MAGIC_SPELL
        nor PROC_FLAG_TAKEN_POSITIVE_MAGIC_SPELL

    *Only Hots can trigger if spell has PROC_FLAG_TAKEN_POSITIVE_MAGIC_SPELL
    *Only dots can trigger if spell has both positivity flags or PROC_FLAG_SUCCESSFUL_NEGATIVE_MAGIC_SPELL
    *Aura has to have PROC_FLAG_TAKEN_POSITIVE_MAGIC_SPELL or spellfamily specified to trigger from Hot

    */

    if(procFlags & PROC_FLAG_DONE_PERIODIC)
    {
        if(EventProcFlag & PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG)
        {
            if(!(procExtra & PROC_EX_INTERNAL_DOT))
                return false;
        }
        else if(procExtra & PROC_EX_INTERNAL_HOT)
            procExtra |= PROC_EX_INTERNAL_REQ_FAMILY;
        else if(EventProcFlag & PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS)
            return false;
    }

    if(procFlags & PROC_FLAG_TAKEN_PERIODIC)
    {
        if(EventProcFlag & PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS)
        {
            if(!(procExtra & PROC_EX_INTERNAL_DOT))
                return false;
        }
        else if(procExtra & PROC_EX_INTERNAL_HOT)
            procExtra |= PROC_EX_INTERNAL_REQ_FAMILY;
        else if(EventProcFlag & PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_POS)
            return false;
    }
    // Trap casts are active by default
    if(procFlags & PROC_FLAG_DONE_TRAP_ACTIVATION)
        active = true;

    // Always trigger for this
    if(procFlags & (PROC_FLAG_KILLED | PROC_FLAG_KILL | PROC_FLAG_DEATH))
        return true;

    if(spellProcEvent)     // Exist event data
    {
        // Store extra req
        procEvent_procEx = spellProcEvent->procEx;

        // For melee triggers
        if(procSpell == NULL)
        {
            // Check (if set) for school (melee attack have Normal school)
            if(spellProcEvent->schoolMask && (spellProcEvent->schoolMask & SPELL_SCHOOL_MASK_NORMAL) == 0)
                return false;
        }
        else // For spells need check school/spell family/family mask
        {
            // Check (if set) for school
            if(spellProcEvent->schoolMask && (spellProcEvent->schoolMask & procSpell->SchoolMask) == 0)
                return false;

            // Check (if set) for spellFamilyName
            if(spellProcEvent->spellFamilyName && (spellProcEvent->spellFamilyName != procSpell->SpellFamilyName))
                return false;

            // spellFamilyName is Ok need check for spellFamilyMask if present
            if(spellProcEvent->spellFamilyMask)
            {
                if(!(spellProcEvent->spellFamilyMask & procSpell->SpellFamilyFlags))
                    return false;
                hasFamilyMask = true;
                // Some spells are not considered as active even with have spellfamilyflags
                if(!(procEvent_procEx & PROC_EX_ONLY_ACTIVE_SPELL))
                    active = true;
            }
        }
    }

    if(procExtra & (PROC_EX_INTERNAL_REQ_FAMILY))
    {
        if(!hasFamilyMask)
            return false;
    }

    // Check for extra req (if none) and hit/crit
    if(procEvent_procEx == PROC_EX_NONE)
    {
        // No extra req, so can trigger only for hit/crit - spell has to be active
        if((procExtra & (PROC_EX_NORMAL_HIT|PROC_EX_CRITICAL_HIT)) && active)
            return true;
    }
    else // Passive spells hits here only if resist/reflect/immune/evade
    {
        if(procExtra & AURA_SPELL_PROC_EX_MASK)
        {
            // if spell marked as procing only from not active spells
            if(active && procEvent_procEx & PROC_EX_NOT_ACTIVE_SPELL)
                return false;
            // if spell marked as procing only from active spells
            if(!active && procEvent_procEx & PROC_EX_ONLY_ACTIVE_SPELL)
                return false;
            // Exist req for PROC_EX_EX_TRIGGER_ALWAYS
            if(procEvent_procEx & PROC_EX_EX_TRIGGER_ALWAYS)
                return true;
            // PROC_EX_NOT_ACTIVE_SPELL and PROC_EX_ONLY_ACTIVE_SPELL flags handle: if passed checks before
            if((procExtra & (PROC_EX_NORMAL_HIT|PROC_EX_CRITICAL_HIT)) && ((procEvent_procEx & (AURA_SPELL_PROC_EX_MASK)) == 0))
                return true;
        }
        // Check Extra Requirement like (hit/crit/miss/resist/parry/dodge/block/immune/reflect/absorb and other)
        if(procEvent_procEx & procExtra)
            return true;
    }
    return false;
}

void SpellMgr::LoadSpellProcs()
{
    uint32 oldMSTime = getMSTime();

    mSpellProcMap.clear();                             // need for reload case

    //                                               0        1           2                3                 4                 5                 6         7              8               9        10              11             12      13        14
    QueryResult result = WorldDatabase.Query("SELECT spellId, schoolMask, spellFamilyName, spellFamilyMask0, spellFamilyMask1, spellFamilyMask2, typeMask, spellTypeMask, spellPhaseMask, hitMask, attributesMask, ratePerMinute, chance, cooldown, charges FROM spell_proc");
    if(!result)
    {
        sLog->outString(">> Loaded 0 spell proc conditions and data");
        sLog->outString();
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        int32 spellId = fields[0].GetInt32();

        bool allRanks = false;
        if(spellId <=0)
        {
            allRanks = true;
            spellId = -spellId;
        }

        SpellEntry const* spellEntry = sSpellStore.LookupEntry(spellId);
        if(!spellEntry)
        {
            sLog->outErrorDb("Spell %u listed in `spell_proc` does not exist", spellId);
            continue;
        }

        if(allRanks)
        {
            if(sSpellMgr->GetFirstSpellInChain(spellId) != uint32(spellId))
            {
                sLog->outErrorDb("Spell %u listed in `spell_proc` is not first rank of spell.", fields[0].GetInt32());
                continue;
            }
        }

        SpellProcEntry baseProcEntry;

        baseProcEntry.schoolMask      = fields[1].GetUInt32();
        baseProcEntry.spellFamilyName = fields[2].GetUInt32();
        baseProcEntry.spellFamilyMask[0] = fields[3].GetUInt32();
        baseProcEntry.spellFamilyMask[1] = fields[4].GetUInt32();
        baseProcEntry.spellFamilyMask[2] = fields[5].GetUInt32();
        baseProcEntry.typeMask        = fields[6].GetUInt32();
        baseProcEntry.spellTypeMask   = fields[7].GetUInt32();
        baseProcEntry.spellPhaseMask  = fields[8].GetUInt32();
        baseProcEntry.hitMask         = fields[9].GetUInt32();
        baseProcEntry.attributesMask  = fields[10].GetUInt32();
        baseProcEntry.ratePerMinute   = fields[11].GetFloat();
        baseProcEntry.chance          = fields[12].GetFloat();
        float cooldown                = fields[13].GetFloat();
        baseProcEntry.cooldown        = uint32(cooldown);
        baseProcEntry.charges         = fields[14].GetUInt32();

        while(true)
        {
            if(mSpellProcMap.find(spellId) != mSpellProcMap.end())
            {
                sLog->outErrorDb("Spell %u listed in `spell_proc` has duplicate entry in the table", spellId);
                break;
            }
            SpellProcEntry procEntry = SpellProcEntry(baseProcEntry);

            // take defaults from dbcs
            if(!procEntry.typeMask)
                procEntry.typeMask = spellEntry->procFlags;
            if(!procEntry.charges)
                procEntry.charges = spellEntry->procCharges;
            if(!procEntry.chance && !procEntry.ratePerMinute)
                procEntry.chance = float(spellEntry->procChance);

            // validate data
            if(procEntry.schoolMask & ~SPELL_SCHOOL_MASK_ALL)
                sLog->outErrorDb("`spell_proc` table entry for spellId %u has wrong `schoolMask` set: %u", spellId, procEntry.schoolMask);
            if(procEntry.spellFamilyName && (procEntry.spellFamilyName < 3 || procEntry.spellFamilyName > 17 || procEntry.spellFamilyName == 14 || procEntry.spellFamilyName == 16))
                sLog->outErrorDb("`spell_proc` table entry for spellId %u has wrong `spellFamilyName` set: %u", spellId, procEntry.spellFamilyName);
            if(procEntry.chance < 0)
            {
                sLog->outErrorDb("`spell_proc` table entry for spellId %u has negative value in `chance` field", spellId);
                procEntry.chance = 0;
            }
            if(procEntry.ratePerMinute < 0)
            {
                sLog->outErrorDb("`spell_proc` table entry for spellId %u has negative value in `ratePerMinute` field", spellId);
                procEntry.ratePerMinute = 0;
            }
            if(cooldown < 0)
            {
                sLog->outErrorDb("`spell_proc` table entry for spellId %u has negative value in `cooldown` field", spellId);
                procEntry.cooldown = 0;
            }
            if(procEntry.chance == 0 && procEntry.ratePerMinute == 0)
                sLog->outErrorDb("`spell_proc` table entry for spellId %u doesn't have `chance` and `ratePerMinute` values defined, proc will not be triggered", spellId);
            if(procEntry.charges > 99)
            {
                sLog->outErrorDb("`spell_proc` table entry for spellId %u has too big value in `charges` field", spellId);
                procEntry.charges = 99;
            }
            if(!procEntry.typeMask)
                sLog->outErrorDb("`spell_proc` table entry for spellId %u doesn't have `typeMask` value defined, proc will not be triggered", spellId);
            if(procEntry.spellTypeMask & ~PROC_SPELL_PHASE_MASK_ALL)
                sLog->outErrorDb("`spell_proc` table entry for spellId %u has wrong `spellTypeMask` set: %u", spellId, procEntry.spellTypeMask);
            if(procEntry.spellTypeMask && !(procEntry.typeMask & (SPELL_PROC_FLAG_MASK | PERIODIC_PROC_FLAG_MASK)))
                sLog->outErrorDb("`spell_proc` table entry for spellId %u has `spellTypeMask` value defined, but it won't be used for defined `typeMask` value", spellId);
            if(!procEntry.spellPhaseMask && procEntry.typeMask & REQ_SPELL_PHASE_PROC_FLAG_MASK)
                sLog->outErrorDb("`spell_proc` table entry for spellId %u doesn't have `spellPhaseMask` value defined, but it's required for defined `typeMask` value, proc will not be triggered", spellId);
            if(procEntry.spellPhaseMask & ~PROC_SPELL_PHASE_MASK_ALL)
                sLog->outErrorDb("`spell_proc` table entry for spellId %u has wrong `spellPhaseMask` set: %u", spellId, procEntry.spellPhaseMask);
            if(procEntry.spellPhaseMask && !(procEntry.typeMask & REQ_SPELL_PHASE_PROC_FLAG_MASK))
                sLog->outErrorDb("`spell_proc` table entry for spellId %u has `spellPhaseMask` value defined, but it won't be used for defined `typeMask` value", spellId);
            if(procEntry.hitMask & ~PROC_HIT_MASK_ALL)
                sLog->outErrorDb("`spell_proc` table entry for spellId %u has wrong `hitMask` set: %u", spellId, procEntry.hitMask);
            if(procEntry.hitMask && !(procEntry.typeMask & TAKEN_HIT_PROC_FLAG_MASK || (procEntry.typeMask & DONE_HIT_PROC_FLAG_MASK && (!procEntry.spellPhaseMask || procEntry.spellPhaseMask & (PROC_SPELL_PHASE_HIT | PROC_SPELL_PHASE_FINISH)))))
                sLog->outErrorDb("`spell_proc` table entry for spellId %u has `hitMask` value defined, but it won't be used for defined `typeMask` and `spellPhaseMask` values", spellId);

            mSpellProcMap[spellId] = procEntry;

            if(allRanks)
            {
                spellId = sSpellMgr->GetNextSpellInChain(spellId);
                spellEntry = sSpellStore.LookupEntry(spellId);
            }
            else
                break;
        }
    } while(result->NextRow());

    sLog->outString(">> Loaded spell proc conditions and data in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

bool SpellMgr::CanSpellTriggerProcOnEvent(SpellProcEntry const& procEntry, ProcEventInfo& eventInfo)
{
    // proc type doesn't match
    if(!(eventInfo.GetTypeMask() & procEntry.typeMask))
        return false;

    // check XP or honor target requirement
    if(procEntry.attributesMask & PROC_ATTR_REQ_EXP_OR_HONOR)
        if(Player* actor = eventInfo.GetActor()->ToPlayer())
            if(eventInfo.GetActionTarget() && !actor->isHonorOrXPTarget(eventInfo.GetActionTarget()))
                return false;

    // always trigger for these types
    if(eventInfo.GetTypeMask() & (PROC_FLAG_KILLED | PROC_FLAG_KILL | PROC_FLAG_DEATH))
        return true;

    // check school mask (if set) for other trigger types
    if(procEntry.schoolMask && !(eventInfo.GetSchoolMask() & procEntry.schoolMask))
        return false;

    // check spell family name/flags (if set) for spells
    if(eventInfo.GetTypeMask() & (PERIODIC_PROC_FLAG_MASK | SPELL_PROC_FLAG_MASK | PROC_FLAG_DONE_TRAP_ACTIVATION))
    {
        if(procEntry.spellFamilyName && (procEntry.spellFamilyName != eventInfo.GetSpellInfo()->SpellFamilyName))
            return false;

        if(procEntry.spellFamilyMask && !(procEntry.spellFamilyMask & eventInfo.GetSpellInfo()->SpellFamilyFlags))
            return false;
    }

    // check spell type mask (if set)
    if(eventInfo.GetTypeMask() & (SPELL_PROC_FLAG_MASK | PERIODIC_PROC_FLAG_MASK))
    {
        if(procEntry.spellTypeMask && !(eventInfo.GetSpellTypeMask() & procEntry.spellTypeMask))
            return false;
    }

    // check spell phase mask
    if(eventInfo.GetTypeMask() & REQ_SPELL_PHASE_PROC_FLAG_MASK)
    {
        if(!(eventInfo.GetSpellPhaseMask() & procEntry.spellPhaseMask))
            return false;
    }

    // check hit mask (on taken hit or on done hit, but not on spell cast phase)
    if((eventInfo.GetTypeMask() & TAKEN_HIT_PROC_FLAG_MASK) || ((eventInfo.GetTypeMask() & DONE_HIT_PROC_FLAG_MASK) && !(eventInfo.GetSpellPhaseMask() & PROC_SPELL_PHASE_CAST)))
    {
        uint32 hitMask = procEntry.hitMask;
        // get default values if hit mask not set
        if(!hitMask)
        {
            // for taken procs allow normal + critical hits by default
            if(eventInfo.GetTypeMask() & TAKEN_HIT_PROC_FLAG_MASK)
                hitMask |= PROC_HIT_NORMAL | PROC_HIT_CRITICAL;
            // for done procs allow normal + critical + absorbs by default
            else
                hitMask |= PROC_HIT_NORMAL | PROC_HIT_CRITICAL | PROC_HIT_ABSORB;
        }
        if(!(eventInfo.GetHitMask() & hitMask))
            return false;
    }

    return true;
}

void SpellMgr::LoadSpellBonusess()
{
    uint32 oldMSTime = getMSTime();

    mSpellBonusMap.clear();                             // need for reload case
    //                                                0      1             2          3         4
    QueryResult result = WorldDatabase.Query("SELECT entry, direct_bonus, dot_bonus, ap_bonus, ap_dot_bonus FROM spell_bonus_data");
    if(!result)
    {
        sLog->outString(">> Loaded 0 spell bonus data");
        sLog->outString();
        return;
    }

    do
    {
        Field* fields = result->Fetch();
        uint32 entry = fields[0].GetUInt32();

        SpellEntry const* spell = sSpellStore.LookupEntry(entry);
        if(!spell)
        {
            sLog->outErrorDb("Spell %u listed in `spell_bonus_data` does not exist", entry);
            continue;
        }

        SpellBonusEntry& sbe = mSpellBonusMap[entry];
        sbe.direct_damage = fields[1].GetFloat();
        sbe.dot_damage    = fields[2].GetFloat();
        sbe.ap_bonus      = fields[3].GetFloat();
        sbe.ap_dot_bonus   = fields[4].GetFloat();

    } while(result->NextRow());

    sLog->outString(">> Loaded extra spell bonus data in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void SpellMgr::LoadSpellGroups()
{
    uint32 oldMSTime = getMSTime();

    mSpellSpellGroup.clear();                                  // need for reload case
    mSpellGroupSpell.clear();

    //                                                       0   1
    QueryResult result = WorldDatabase.Query("SELECT id, spell_id FROM spell_group");
    if(!result)
    {
        sLog->outString();
        sLog->outString(">> Loaded 0 spell group definitions");
        return;
    }

    std::set<uint32> groups;

    do
    {
        Field* fields = result->Fetch();

        uint32 group_id = fields[0].GetUInt32();
        if(group_id <= SPELL_GROUP_DB_RANGE_MIN && group_id >= SPELL_GROUP_CORE_RANGE_MAX)
        {
            sLog->outErrorDb("SpellGroup id %u listed in `spell_groups` is in core range, but is not defined in core!", group_id);
            continue;
        }
        int32 spell_id = fields[1].GetInt32();

        groups.insert(std::set<uint32>::value_type(group_id));
        mSpellGroupSpell.insert(SpellGroupSpellMap::value_type((SpellGroup)group_id, spell_id));

    } while(result->NextRow());

    for(SpellGroupSpellMap::iterator itr = mSpellGroupSpell.begin(); itr!= mSpellGroupSpell.end() ;)
    {
        if(itr->second < 0)
        {
            if(groups.find(abs(itr->second)) == groups.end())
            {
                sLog->outErrorDb("SpellGroup id %u listed in `spell_groups` does not exist", abs(itr->second));
                mSpellGroupSpell.erase(itr++);
            }
            else
                ++itr;
        }
        else
        {
            SpellEntry const* spellInfo = sSpellStore.LookupEntry(itr->second);

            if(!spellInfo)
            {
                sLog->outErrorDb("Spell %u listed in `spell_group` does not exist", itr->second);
                mSpellGroupSpell.erase(itr++);
            }
            else if(GetSpellRank(itr->second) > 1)
            {
                sLog->outErrorDb("Spell %u listed in `spell_group` is not first rank of spell", itr->second);
                mSpellGroupSpell.erase(itr++);
            }
            else
                ++itr;
        }
    }

    for(std::set<uint32>::iterator groupItr = groups.begin() ; groupItr != groups.end() ; ++groupItr)
    {
        std::set<uint32> spells;
        GetSetOfSpellsInSpellGroup(SpellGroup(*groupItr), spells);

        for(std::set<uint32>::iterator spellItr = spells.begin() ; spellItr != spells.end() ; ++spellItr)
            mSpellSpellGroup.insert(SpellSpellGroupMap::value_type(*spellItr, SpellGroup(*groupItr)));
    }

    sLog->outString(">> Loaded spell group definitions in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void SpellMgr::LoadSpellGroupStackRules()
{
    uint32 oldMSTime = getMSTime();

    mSpellGroupStack.clear();                                  // need for reload case

    //                                                       0         1
    QueryResult result = WorldDatabase.Query("SELECT group_id, stack_rule FROM spell_group_stack_rules");
    if(!result)
    {
        sLog->outString(">> Loaded 0 spell group stack rules");
        sLog->outString();
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 group_id = fields[0].GetUInt32();
        uint8 stack_rule = fields[1].GetUInt32();
        if(stack_rule >= SPELL_GROUP_STACK_RULE_MAX)
        {
            sLog->outErrorDb("SpellGroupStackRule %u listed in `spell_group_stack_rules` does not exist", stack_rule);
            continue;
        }

        SpellGroupSpellMapBounds spellGroup = GetSpellGroupSpellMapBounds((SpellGroup)group_id);

        if(spellGroup.first == spellGroup.second)
        {
            sLog->outErrorDb("SpellGroup id %u listed in `spell_group_stack_rules` does not exist", group_id);
            continue;
        }

        mSpellGroupStack[(SpellGroup)group_id] = (SpellGroupStackRule)stack_rule;

    } while(result->NextRow());

    sLog->outString(">> Loaded spell group stack rules in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void SpellMgr::LoadSpellThreats()
{
    uint32 oldMSTime = getMSTime();

    mSpellThreatMap.clear();                                // need for reload case

    //                                                0      1        2       3
    QueryResult result = WorldDatabase.Query("SELECT entry, flatMod, pctMod, apPctMod FROM spell_threat");
    if(!result)
    {
        sLog->outString(">> Loaded 0 aggro generating spells");
        sLog->outString();
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        if(!sSpellStore.LookupEntry(entry))
        {
            sLog->outErrorDb("Spell %u listed in `spell_threat` does not exist", entry);
            continue;
        }

        SpellThreatEntry ste;
        ste.flatMod  = fields[1].GetInt16();
        ste.pctMod   = fields[2].GetFloat();
        ste.apPctMod = fields[3].GetFloat();

        mSpellThreatMap[entry] = ste;
    } while(result->NextRow());

    sLog->outString(">> Loaded 0 SpellThreatEntries in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

bool SpellMgr::IsRankSpellDueToSpell(SpellEntry const *spellInfo_1, uint32 spellId_2) const
{
    SpellEntry const* spellInfo_2 = sSpellStore.LookupEntry(spellId_2);
    if(!spellInfo_1 || !spellInfo_2) return false;
    if(spellInfo_1->Id == spellId_2) return false;

    return GetFirstSpellInChain(spellInfo_1->Id) == GetFirstSpellInChain(spellId_2);
}

bool SpellMgr::canStackSpellRanks(SpellEntry const *spellInfo)
{
    if(IsPassiveSpell(spellInfo->Id))                       // ranked passive spell
        return false;
    if(spellInfo->powerType != POWER_MANA && spellInfo->powerType != POWER_HEALTH)
        return false;
    if(IsProfessionOrRidingSpell(spellInfo->Id))
        return false;

    if(sSpellMgr->IsSkillBonusSpell(spellInfo->Id))
        return false;

    // All stance spells. if any better way, change it.
    for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        switch(spellInfo->SpellFamilyName)
        {
            case SPELLFAMILY_PALADIN:
                // Paladin aura Spell
                if(spellInfo->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA_RAID)
                    return false;
                break;
            case SPELLFAMILY_DRUID:
                // Druid form Spell
                if(spellInfo->Effect[i] == SPELL_EFFECT_APPLY_AURA &&
                    spellInfo->EffectApplyAuraName[i] == SPELL_AURA_MOD_SHAPESHIFT)
                    return false;
                break;
            case SPELLFAMILY_ROGUE:
                // Rogue Stealth
                if(spellInfo->Effect[i] == SPELL_EFFECT_APPLY_AURA &&
                    spellInfo->EffectApplyAuraName[i] == SPELL_AURA_MOD_SHAPESHIFT)
                    return false;
        }
    }
    return true;
}

bool SpellMgr::IsProfessionOrRidingSpell(uint32 spellId)
{
    SpellEntry const* spellInfo = sSpellStore.LookupEntry(spellId);
    if(!spellInfo)
        return false;

    for(uint8 i = 0 ; i < MAX_SPELL_EFFECTS ; ++i)
    {
        if(spellInfo->Effect[i] == SPELL_EFFECT_SKILL)
        {
            uint32 skill = spellInfo->EffectMiscValue[i];

            bool found = IsProfessionOrRidingSkill(skill);
            if(found)
                return true;
        }
    }
    return false;
}

bool SpellMgr::IsProfessionSpell(uint32 spellId)
{
    SpellEntry const* spellInfo = sSpellStore.LookupEntry(spellId);
    if(!spellInfo)
        return false;

    for(uint8 i = 0 ; i < MAX_SPELL_EFFECTS ; ++i)
    {
        if(spellInfo->Effect[i] == SPELL_EFFECT_SKILL)
        {
            uint32 skill = spellInfo->EffectMiscValue[i];

            bool found = IsProfessionSkill(skill);
            if(found)
                return true;
        }
    }
    return false;
}

bool SpellMgr::IsPrimaryProfessionSpell(uint32 spellId)
{
    SpellEntry const* spellInfo = sSpellStore.LookupEntry(spellId);
    if(!spellInfo)
        return false;

    for(uint8 i = 0 ; i < MAX_SPELL_EFFECTS ; ++i)
    {
        if(spellInfo->Effect[i] == SPELL_EFFECT_SKILL)
        {
            uint32 skill = spellInfo->EffectMiscValue[i];

            bool found = IsPrimaryProfessionSkill(skill);
            if(found)
                return true;
        }
    }
    return false;
}

bool SpellMgr::IsPrimaryProfessionFirstRankSpell(uint32 spellId) const
{
    return IsPrimaryProfessionSpell(spellId) && GetSpellRank(spellId) == 1;
}

bool SpellMgr::IsSkillBonusSpell(uint32 spellId) const
{
    SkillLineAbilityMapBounds bounds = GetSkillLineAbilityMapBounds(spellId);

    for(SkillLineAbilityMap::const_iterator _spell_idx = bounds.first; _spell_idx != bounds.second; ++_spell_idx)
    {
        SkillLineAbilityEntry const* pAbility = _spell_idx->second;
        if(!pAbility || pAbility->learnOnGetSkill != ABILITY_LEARNED_ON_GET_PROFESSION_SKILL)
            continue;

        if(pAbility->req_skill_value > 0)
            return true;
    }

    return false;
}

bool SpellMgr::IsSkillTypeSpell(uint32 spellId, SkillType type) const
{
    SkillLineAbilityMapBounds bounds = GetSkillLineAbilityMapBounds(spellId);

    for(SkillLineAbilityMap::const_iterator _spell_idx = bounds.first; _spell_idx != bounds.second; ++_spell_idx)
        if(_spell_idx->second->skillId == uint32(type))
            return true;

    return false;
}

// basepoints provided here have to be valid basepoints (use SpellMgr::CalculateSpellEffectBaseAmount)
int32 SpellMgr::CalculateSpellEffectAmount(SpellEntry const* spellEntry, uint8 effIndex, Unit const* caster, int32 const* effBasePoints, Unit const* /*target*/)
{
    float basePointsPerLevel = spellEntry->EffectRealPointsPerLevel[effIndex];
    int32 basePoints = effBasePoints ? *effBasePoints : spellEntry->EffectBasePoints[effIndex];
    int32 randomPoints = int32(spellEntry->EffectDieSides[effIndex]);

    // base amount modification based on spell lvl vs caster lvl
    if(caster)
    {
        int32 level = int32(caster->getLevel());
        if(level > int32(spellEntry->maxLevel) && spellEntry->maxLevel > 0)
            level = int32(spellEntry->maxLevel);
        else if(level < int32(spellEntry->baseLevel))
            level = int32(spellEntry->baseLevel);
        level -= int32(spellEntry->spellLevel);
        basePoints += int32(level * basePointsPerLevel);
    }

    // roll in a range <1;EffectDieSides> as of patch 3.3.3
    switch(randomPoints)
    {
        case 0: break;
        case 1: basePoints += 1; break;                     // range 1..1
        default:
            // range can have positive (1..rand) and negative (rand..1) values, so order its for irand
            int32 randvalue = (randomPoints >= 1)
                ? irand(1, randomPoints)
                : irand(randomPoints, 1);

            basePoints += randvalue;
            break;
    }

    float value = float(basePoints);

    // random damage
    if(caster)
    {
        // bonus amount from combo points
        if(caster->m_movedPlayer)
            if(uint8 comboPoints = caster->m_movedPlayer->GetComboPoints())
                if(float comboDamage = spellEntry->EffectPointsPerComboPoint[effIndex])
                    value += comboDamage * comboPoints;

        value = caster->ApplyEffectModifiers(spellEntry, effIndex, value);

        // amount multiplication based on caster's level
        if(!basePointsPerLevel && (spellEntry->Attributes & SPELL_ATTR0_LEVEL_DAMAGE_CALCULATION && spellEntry->spellLevel) &&
                spellEntry->Effect[effIndex] != SPELL_EFFECT_WEAPON_PERCENT_DAMAGE &&
                spellEntry->Effect[effIndex] != SPELL_EFFECT_KNOCK_BACK &&
                spellEntry->EffectApplyAuraName[effIndex] != SPELL_AURA_MOD_SPEED_ALWAYS &&
                spellEntry->EffectApplyAuraName[effIndex] != SPELL_AURA_MOD_SPEED_NOT_STACK &&
                spellEntry->EffectApplyAuraName[effIndex] != SPELL_AURA_MOD_INCREASE_SPEED &&
                spellEntry->EffectApplyAuraName[effIndex] != SPELL_AURA_MOD_DECREASE_SPEED)
                //there are many more: slow speed, -healing pct
            value *= 0.25f * exp(caster->getLevel() * (70 - spellEntry->spellLevel) / 1000.0f);
            //value = int32(value * (int32)getLevel() / (int32)(spellProto->spellLevel ? spellProto->spellLevel : 1));
    }

    return int32(value);
}

int32 SpellMgr::CalculateSpellEffectBaseAmount(int32 value, SpellEntry const* spellEntry, uint8 effIndex)
{
    if(spellEntry->EffectDieSides[effIndex] == 0)
        return value;
    else
        return value - 1;
}

float SpellMgr::CalculateSpellEffectValueMultiplier(SpellEntry const* spellEntry, uint8 effIndex, Unit* caster, Spell* spell)
{
    float multiplier = spellEntry->EffectValueMultiplier[effIndex];
    if(Player* modOwner = (caster ? caster->GetSpellModOwner() : NULL))
        modOwner->ApplySpellMod(spellEntry->Id, SPELLMOD_VALUE_MULTIPLIER, multiplier, spell);
    return multiplier;
}

float SpellMgr::CalculateSpellEffectDamageMultiplier(SpellEntry const* spellEntry, uint8 effIndex, Unit* caster, Spell* spell)
{
    float multiplier = spellEntry->EffectDamageMultiplier[effIndex];
    if(Player* modOwner = (caster ? caster->GetSpellModOwner() : NULL))
        modOwner->ApplySpellMod(spellEntry->Id, SPELLMOD_DAMAGE_MULTIPLIER, multiplier, spell);
    return multiplier;
}

SpellEntry const* SpellMgr::SelectAuraRankForPlayerLevel(SpellEntry const* spellInfo, uint32 playerLevel) const
{
    // ignore passive spells
    if(IsPassiveSpell(spellInfo->Id))
        return spellInfo;

    bool needRankSelection = false;
    for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if(IsPositiveEffect(spellInfo->Id, i) && (
            spellInfo->Effect[i] == SPELL_EFFECT_APPLY_AURA ||
            spellInfo->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA_PARTY ||
            spellInfo->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA_RAID
        ))
        {
            needRankSelection = true;
            break;
        }
    }

    // not required
    if(!needRankSelection)
        return spellInfo;

    for(uint32 nextSpellId = spellInfo->Id; nextSpellId != 0; nextSpellId = GetPrevSpellInChain(nextSpellId))
    {
        SpellEntry const* nextSpellInfo = sSpellStore.LookupEntry(nextSpellId);
        if(!nextSpellInfo)
            break;
        // if found appropriate level
        if(playerLevel + 10 >= nextSpellInfo->spellLevel)
            return nextSpellInfo;

        // one rank less then
    }

    // not found
    return NULL;
}

void SpellMgr::LoadSpellLearnSkills()
{
    uint32 oldMSTime = getMSTime();

    mSpellLearnSkills.clear();                              // need for reload case

    // search auto-learned skills and add its to map also for use in unlearn spells/talents
    uint32 dbc_count = 0;
    for(uint32 spell = 0; spell < sSpellStore.GetNumRows(); ++spell)
    {
        SpellEntry const* entry = sSpellStore.LookupEntry(spell);

        if(!entry)
            continue;

        for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if(entry->Effect[i] == SPELL_EFFECT_SKILL)
            {
                SpellLearnSkillNode dbc_node;
                dbc_node.skill = entry->EffectMiscValue[i];
                dbc_node.step  = SpellMgr::CalculateSpellEffectAmount(entry, i);
                if(dbc_node.skill != SKILL_RIDING)
                    dbc_node.value = 1;
                else
                    dbc_node.value = dbc_node.step * 75;
                dbc_node.maxvalue = dbc_node.step * 75;
                mSpellLearnSkills[spell] = dbc_node;
                ++dbc_count;
                break;
            }
        }
    }

    sLog->outString(">> Loaded %u Spell Learn Skills from DBC in %u ms", dbc_count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void SpellMgr::LoadSpellLearnSpells()
{
    uint32 oldMSTime = getMSTime();

    mSpellLearnSpells.clear();                              // need for reload case

    //                                                  0      1        2
    QueryResult result = WorldDatabase.Query("SELECT entry, SpellID, Active FROM spell_learn_spell");
    if(!result)
    {
        sLog->outString(">> Loaded 0 spell learn spells");
        sLog->outString();
        sLog->outErrorDb("`spell_learn_spell` table is empty!");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 spell_id = fields[0].GetUInt32();

        SpellLearnSpellNode node;
        node.spell      = fields[1].GetUInt32();
        node.active     = fields[2].GetBool();
        node.autoLearned= false;

        if(!sSpellStore.LookupEntry(spell_id))
        {
            sLog->outErrorDb("Spell %u listed in `spell_learn_spell` does not exist", spell_id);
            continue;
        }

        if(!sSpellStore.LookupEntry(node.spell))
        {
            sLog->outErrorDb("Spell %u listed in `spell_learn_spell` learning not existed spell %u", spell_id, node.spell);
            continue;
        }

        if(GetTalentSpellCost(node.spell))
        {
            sLog->outErrorDb("Spell %u listed in `spell_learn_spell` attempt learning talent spell %u, skipped", spell_id, node.spell);
            continue;
        }

        mSpellLearnSpells.insert(SpellLearnSpellMap::value_type(spell_id, node));

    } while(result->NextRow());

    // search auto-learned spells and add its to map also for use in unlearn spells/talents
    uint32 dbc_count = 0;
    for(uint32 spell = 0; spell < sSpellStore.GetNumRows(); ++spell)
    {
        SpellEntry const* entry = sSpellStore.LookupEntry(spell);

        if(!entry)
            continue;

        for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if(entry->Effect[i] == SPELL_EFFECT_LEARN_SPELL)
            {
                SpellLearnSpellNode dbc_node;
                dbc_node.spell = entry->EffectTriggerSpell[i];
                dbc_node.active = true;                     // all dbc based learned spells is active (show in spell book or hide by client itself)

                // ignore learning not existed spells (broken/outdated/or generic learnig spell 483
                if(!sSpellStore.LookupEntry(dbc_node.spell))
                    continue;

                // talent or passive spells or skill-step spells auto-casted and not need dependent learning,
                // pet teaching spells must not be dependent learning (casted)
                // other required explicit dependent learning
                dbc_node.autoLearned = entry->EffectImplicitTargetA[i] == TARGET_UNIT_PET || GetTalentSpellCost(spell) > 0 || IsPassiveSpell(spell) || IsSpellHaveEffect(entry, SPELL_EFFECT_SKILL_STEP);

                SpellLearnSpellMapBounds db_node_bounds = GetSpellLearnSpellMapBounds(spell);

                bool found = false;
                for(SpellLearnSpellMap::const_iterator itr = db_node_bounds.first; itr != db_node_bounds.second; ++itr)
                {
                    if(itr->second.spell == dbc_node.spell)
                    {
                        sLog->outErrorDb("Spell %u auto-learn spell %u in spell.dbc then the record in `spell_learn_spell` is redundant, please fix DB.",
                            spell, dbc_node.spell);
                        found = true;
                        break;
                    }
                }

                if(!found)                                  // add new spell-spell pair if not found
                {
                    mSpellLearnSpells.insert(SpellLearnSpellMap::value_type(spell, dbc_node));
                    ++dbc_count;
                }
            }
        }
    }

    sLog->outString(">> Loaded spell learn spells and from database and from DBC in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void SpellMgr::LoadSpellPetAuras()
{
    uint32 oldMSTime = getMSTime();

    mSpellPetAuraMap.clear();                                  // need for reload case

    //                                                  0       1       2    3
    QueryResult result = WorldDatabase.Query("SELECT spell, effectId, pet, aura FROM spell_pet_auras");
    if(!result)
    {
        sLog->outString(">> Loaded 0 spell pet auras. DB table `spell_pet_auras` is empty.");
        sLog->outString();
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 spell = fields[0].GetUInt32();
        uint8 eff = fields[1].GetUInt8();
        uint32 pet = fields[2].GetUInt32();
        uint32 aura = fields[3].GetUInt32();

        SpellPetAuraMap::iterator itr = mSpellPetAuraMap.find((spell<<8) + eff);
        if(itr != mSpellPetAuraMap.end())
            itr->second.AddAura(pet, aura);
        else
        {
            SpellEntry const* spellInfo = sSpellStore.LookupEntry(spell);
            if(!spellInfo)
            {
                sLog->outErrorDb("Spell %u listed in `spell_pet_auras` does not exist", spell);
                continue;
            }
            if(spellInfo->Effect[eff] != SPELL_EFFECT_DUMMY &&
               (spellInfo->Effect[eff] != SPELL_EFFECT_APPLY_AURA ||
                spellInfo->EffectApplyAuraName[eff] != SPELL_AURA_DUMMY))
            {
                sLog->outError("Spell %u listed in `spell_pet_auras` does not have dummy aura or dummy effect", spell);
                continue;
            }

            SpellEntry const* spellInfo2 = sSpellStore.LookupEntry(aura);
            if(!spellInfo2)
            {
                sLog->outErrorDb("Aura %u listed in `spell_pet_auras` does not exist", aura);
                continue;
            }

            PetAura pa(pet, aura, spellInfo->EffectImplicitTargetA[eff] == TARGET_UNIT_PET, SpellMgr::CalculateSpellEffectAmount(spellInfo, eff));
            mSpellPetAuraMap[(spell<<8) + eff] = pa;
        }

    } while(result->NextRow());

    sLog->outString(">> Loaded spell pet auras in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void SpellMgr::LoadPetLevelupSpellMap()
{
    uint32 oldMSTime = getMSTime();

    mPetLevelupSpellMap.clear();                                   // need for reload case

    uint32 family_count = 0;

    for(uint32 i = 0; i < sCreatureFamilyStore.GetNumRows(); ++i)
    {
        CreatureFamilyEntry const* creatureFamily = sCreatureFamilyStore.LookupEntry(i);
        if(!creatureFamily)                                     // not exist
            continue;

        for(uint8 j = 0; j < 2; ++j)
        {
            if(!creatureFamily->skillLine[j])
                continue;

            for(uint32 k = 0; k < sSkillLineAbilityStore.GetNumRows(); ++k)
            {
                SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(k);
                if(!skillLine)
                    continue;

                //if(skillLine->skillId != creatureFamily->skillLine[0] &&
                //    (!creatureFamily->skillLine[1] || skillLine->skillId != creatureFamily->skillLine[1]))
                //    continue;

                if(skillLine->skillId != creatureFamily->skillLine[j])
                    continue;

                if(skillLine->learnOnGetSkill != ABILITY_LEARNED_ON_GET_RACE_OR_CLASS_SKILL)
                    continue;

                SpellEntry const* spell = sSpellStore.LookupEntry(skillLine->spellId);
                if(!spell) // not exist or triggered or talent
                    continue;

                if(!spell->spellLevel)
                    continue;

                PetLevelupSpellSet& spellSet = mPetLevelupSpellMap[creatureFamily->ID];
                if(spellSet.empty())
                    ++family_count;

                spellSet.insert(PetLevelupSpellSet::value_type(spell->spellLevel, spell->Id));
            }
        }
    }

    sLog->outString(">> Loaded pet levelup and default spells for %u families in %u ms", family_count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

bool LoadPetDefaultSpells_helper(CreatureTemplate const* cInfo, PetDefaultSpellsEntry& petDefSpells)
{
    // skip empty list;
    bool have_spell = false;
    for(uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
    {
        if(petDefSpells.spellid[j])
        {
            have_spell = true;
            break;
        }
    }
    if(!have_spell)
        return false;

    // remove duplicates with levelupSpells if any
    if(PetLevelupSpellSet const* levelupSpells = cInfo->family ? sSpellMgr->GetPetLevelupSpellList(cInfo->family) : NULL)
    {
        for(uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
        {
            if(!petDefSpells.spellid[j])
                continue;

            for(PetLevelupSpellSet::const_iterator itr = levelupSpells->begin(); itr != levelupSpells->end(); ++itr)
            {
                if(itr->second == petDefSpells.spellid[j])
                {
                    petDefSpells.spellid[j] = 0;
                    break;
                }
            }
        }
    }

    // skip empty list;
    have_spell = false;
    for(uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
    {
        if(petDefSpells.spellid[j])
        {
            have_spell = true;
            break;
        }
    }

    return have_spell;
}

void SpellMgr::LoadPetDefaultSpells()
{
    uint32 oldMSTime = getMSTime();

    mPetDefaultSpellsMap.clear();

    uint32 countCreature = 0;
    uint32 countData = 0;

    CreatureTemplateContainer const* ctc = sObjectMgr->GetCreatureTemplates();
    for(CreatureTemplateContainer::const_iterator itr = ctc->begin(); itr != ctc->end(); ++itr)
    {
        if(!itr->second.PetSpellDataId)
            continue;

        // for creature with PetSpellDataId get default pet spells from dbc
        CreatureSpellDataEntry const* spellDataEntry = sCreatureSpellDataStore.LookupEntry(itr->second.PetSpellDataId);
        if(!spellDataEntry)
            continue;

        int32 petSpellsId = -int32(itr->second.PetSpellDataId);
        PetDefaultSpellsEntry petDefSpells;
        for(uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
            petDefSpells.spellid[j] = spellDataEntry->spellId[j];

        if(LoadPetDefaultSpells_helper(&itr->second, petDefSpells))
        {
            mPetDefaultSpellsMap[petSpellsId] = petDefSpells;
            ++countData;
        }
    }

    sLog->outString(">> Loaded addition spells for %u pet spell data entries in %u ms", countData, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();

    sLog->outString("Loading summonable creature templates...");
    oldMSTime = getMSTime();

    // different summon spells
    for(uint32 i = 0; i < sSpellStore.GetNumRows(); ++i)
    {
        SpellEntry const* spellEntry = sSpellStore.LookupEntry(i);
        if(!spellEntry)
            continue;

        for(uint8 k = 0; k < MAX_SPELL_EFFECTS; ++k)
        {
            if(spellEntry->Effect[k] == SPELL_EFFECT_SUMMON || spellEntry->Effect[k] == SPELL_EFFECT_SUMMON_PET)
            {
                uint32 creature_id = spellEntry->EffectMiscValue[k];
                CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(creature_id);
                if(!cInfo)
                    continue;

                // already loaded
                if(cInfo->PetSpellDataId)
                    continue;

                // for creature without PetSpellDataId get default pet spells from creature_template
                int32 petSpellsId = cInfo->Entry;
                if(mPetDefaultSpellsMap.find(cInfo->Entry) != mPetDefaultSpellsMap.end())
                    continue;

                PetDefaultSpellsEntry petDefSpells;
                for(uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
                    petDefSpells.spellid[j] = cInfo->spells[j];

                if(LoadPetDefaultSpells_helper(cInfo, petDefSpells))
                {
                    mPetDefaultSpellsMap[petSpellsId] = petDefSpells;
                    ++countCreature;
                }
            }
        }
    }

    sLog->outString(">> Loaded %u summonable creature templates in %u ms", countCreature, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

/// Some checks for spells, to prevent adding deprecated/broken spells for trainers, spell book, etc
bool SpellMgr::IsSpellValid(SpellEntry const *spellInfo, Player* pl, bool msg)
{
    // not exist
    if(!spellInfo)
        return false;

    bool need_check_reagents = false;

    // check effects
    for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        switch(spellInfo->Effect[i])
        {
            case 0:
                continue;

            // craft spell for crafting non-existed item (break client recipes list show)
            case SPELL_EFFECT_CREATE_ITEM:
            case SPELL_EFFECT_CREATE_ITEM_2:
            {
                if(spellInfo->EffectItemType[i] == 0)
                {
                    // skip auto-loot crafting spells, its not need explicit item info (but have special fake items sometime)
                    if(!IsLootCraftingSpell(spellInfo))
                    {
                        if(msg)
                        {
                            if(pl)
                                ChatHandler(pl).PSendSysMessage("Craft spell %u not have create item entry.", spellInfo->Id);
                            else
                                sLog->outErrorDb("Craft spell %u not have create item entry.", spellInfo->Id);
                        }
                        return false;
                    }

                }
                // also possible IsLootCraftingSpell case but fake item must exist anyway
                else if(!sObjectMgr->GetItemTemplate(spellInfo->EffectItemType[i]))
                {
                    if(msg)
                    {
                        if(pl)
                            ChatHandler(pl).PSendSysMessage("Craft spell %u create not-exist in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->EffectItemType[i]);
                        else
                            sLog->outErrorDb("Craft spell %u create not-exist in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->EffectItemType[i]);
                    }
                    return false;
                }

                need_check_reagents = true;
                break;
            }
            case SPELL_EFFECT_LEARN_SPELL:
            {
                SpellEntry const* spellInfo2 = sSpellStore.LookupEntry(spellInfo->EffectTriggerSpell[i]);
                if(!IsSpellValid(spellInfo2, pl, msg))
                {
                    if(msg)
                    {
                        if(pl)
                            ChatHandler(pl).PSendSysMessage("Spell %u learn to broken spell %u, and then...", spellInfo->Id, spellInfo->EffectTriggerSpell[i]);
                        else
                            sLog->outErrorDb("Spell %u learn to invalid spell %u, and then...", spellInfo->Id, spellInfo->EffectTriggerSpell[i]);
                    }
                    return false;
                }
                break;
            }
        }
    }

    if(need_check_reagents)
    {
        for(uint8 j = 0; j < MAX_SPELL_REAGENTS; ++j)
        {
            if(spellInfo->Reagent[j] > 0 && !sObjectMgr->GetItemTemplate(spellInfo->Reagent[j]))
            {
                if(msg)
                {
                    if(pl)
                        ChatHandler(pl).PSendSysMessage("Craft spell %u have not-exist reagent in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Reagent[j]);
                    else
                        sLog->outErrorDb("Craft spell %u have not-exist reagent in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Reagent[j]);
                }
                return false;
            }
        }
    }

    return true;
}

void SpellMgr::LoadSpellAreas()
{
    uint32 oldMSTime = getMSTime();

    mSpellAreaMap.clear();                                  // need for reload case
    mSpellAreaForQuestMap.clear();
    mSpellAreaForActiveQuestMap.clear();
    mSpellAreaForQuestEndMap.clear();
    mSpellAreaForAuraMap.clear();

    //                                                  0     1         2              3               4           5          6        7       8
    QueryResult result = WorldDatabase.Query("SELECT spell, area, quest_start, quest_start_active, quest_end, aura_spell, racemask, gender, autocast FROM spell_area");

    if(!result)
    {
        sLog->outString(">> Loaded 0 spell area requirements. DB table `spell_area` is empty.");
        sLog->outString();
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 spell = fields[0].GetUInt32();
        SpellArea spellArea;
        spellArea.spellId             = spell;
        spellArea.areaId              = fields[1].GetUInt32();
        spellArea.questStart          = fields[2].GetUInt32();
        spellArea.questStartCanActive = fields[3].GetBool();
        spellArea.questEnd            = fields[4].GetUInt32();
        spellArea.auraSpell           = fields[5].GetInt32();
        spellArea.raceMask            = fields[6].GetUInt32();
        spellArea.gender              = Gender(fields[7].GetUInt8());
        spellArea.autocast            = fields[8].GetBool();

        if(SpellEntry const* spellInfo = sSpellStore.LookupEntry(spell))
        {
            if(spellArea.autocast)
                const_cast<SpellEntry*>(spellInfo)->Attributes |= SPELL_ATTR0_CANT_CANCEL;
        }
        else
        {
            sLog->outErrorDb("Spell %u listed in `spell_area` does not exist", spell);
            continue;
        }

        {
            bool ok = true;
            SpellAreaMapBounds sa_bounds = GetSpellAreaMapBounds(spellArea.spellId);
            for(SpellAreaMap::const_iterator itr = sa_bounds.first; itr != sa_bounds.second; ++itr)
            {
                if(spellArea.spellId != itr->second.spellId)
                    continue;
                if(spellArea.areaId != itr->second.areaId)
                    continue;
                if(spellArea.questStart != itr->second.questStart)
                    continue;
                if(spellArea.auraSpell != itr->second.auraSpell)
                    continue;
                if((spellArea.raceMask & itr->second.raceMask) == 0)
                    continue;
                if(spellArea.gender != itr->second.gender)
                    continue;

                // duplicate by requirements
                ok =false;
                break;
            }

            if(!ok)
            {
                sLog->outErrorDb("Spell %u listed in `spell_area` already listed with similar requirements.", spell);
                continue;
            }
        }

        if(spellArea.areaId && !GetAreaEntryByAreaID(spellArea.areaId))
        {
            sLog->outErrorDb("Spell %u listed in `spell_area` have wrong area (%u) requirement", spell, spellArea.areaId);
            continue;
        }

        if(spellArea.questStart && !sObjectMgr->GetQuestTemplate(spellArea.questStart))
        {
            sLog->outErrorDb("Spell %u listed in `spell_area` have wrong start quest (%u) requirement", spell, spellArea.questStart);
            continue;
        }

        if(spellArea.questEnd)
        {
            if(!sObjectMgr->GetQuestTemplate(spellArea.questEnd))
            {
                sLog->outErrorDb("Spell %u listed in `spell_area` have wrong end quest (%u) requirement", spell, spellArea.questEnd);
                continue;
            }

            if(spellArea.questEnd == spellArea.questStart && !spellArea.questStartCanActive)
            {
                sLog->outErrorDb("Spell %u listed in `spell_area` have quest (%u) requirement for start and end in same time", spell, spellArea.questEnd);
                continue;
            }
        }

        if(spellArea.auraSpell)
        {
            SpellEntry const* spellInfo = sSpellStore.LookupEntry(abs(spellArea.auraSpell));
            if(!spellInfo)
            {
                sLog->outErrorDb("Spell %u listed in `spell_area` have wrong aura spell (%u) requirement", spell, abs(spellArea.auraSpell));
                continue;
            }

            if(uint32(abs(spellArea.auraSpell)) == spellArea.spellId)
            {
                sLog->outErrorDb("Spell %u listed in `spell_area` have aura spell (%u) requirement for itself", spell, abs(spellArea.auraSpell));
                continue;
            }

            // not allow autocast chains by auraSpell field (but allow use as alternative if not present)
            if(spellArea.autocast && spellArea.auraSpell > 0)
            {
                bool chain = false;
                SpellAreaForAuraMapBounds saBound = GetSpellAreaForAuraMapBounds(spellArea.spellId);
                for(SpellAreaForAuraMap::const_iterator itr = saBound.first; itr != saBound.second; ++itr)
                {
                    if(itr->second->autocast && itr->second->auraSpell > 0)
                    {
                        chain = true;
                        break;
                    }
                }

                if(chain)
                {
                    sLog->outErrorDb("Spell %u listed in `spell_area` have aura spell (%u) requirement that itself autocast from aura", spell, spellArea.auraSpell);
                    continue;
                }

                SpellAreaMapBounds saBound2 = GetSpellAreaMapBounds(spellArea.auraSpell);
                for(SpellAreaMap::const_iterator itr2 = saBound2.first; itr2 != saBound2.second; ++itr2)
                {
                    if(itr2->second.autocast && itr2->second.auraSpell > 0)
                    {
                        chain = true;
                        break;
                    }
                }

                if(chain)
                {
                    sLog->outErrorDb("Spell %u listed in `spell_area` have aura spell (%u) requirement that itself autocast from aura", spell, spellArea.auraSpell);
                    continue;
                }
            }
        }

        if(spellArea.raceMask && (spellArea.raceMask & RACEMASK_ALL_PLAYABLE) == 0)
        {
            sLog->outErrorDb("Spell %u listed in `spell_area` have wrong race mask (%u) requirement", spell, spellArea.raceMask);
            continue;
        }

        if(spellArea.gender != GENDER_NONE && spellArea.gender != GENDER_FEMALE && spellArea.gender != GENDER_MALE)
        {
            sLog->outErrorDb("Spell %u listed in `spell_area` have wrong gender (%u) requirement", spell, spellArea.gender);
            continue;
        }

        SpellArea const* sa = &mSpellAreaMap.insert(SpellAreaMap::value_type(spell, spellArea))->second;

        // for search by current zone/subzone at zone/subzone change
        if(spellArea.areaId)
            mSpellAreaForAreaMap.insert(SpellAreaForAreaMap::value_type(spellArea.areaId, sa));

        // for search at quest start/reward
        if(spellArea.questStart)
        {
            if(spellArea.questStartCanActive)
                mSpellAreaForActiveQuestMap.insert(SpellAreaForQuestMap::value_type(spellArea.questStart, sa));
            else
                mSpellAreaForQuestMap.insert(SpellAreaForQuestMap::value_type(spellArea.questStart, sa));
        }

        // for search at quest start/reward
        if(spellArea.questEnd)
            mSpellAreaForQuestEndMap.insert(SpellAreaForQuestMap::value_type(spellArea.questEnd, sa));

        // for search at aura apply
        if(spellArea.auraSpell)
            mSpellAreaForAuraMap.insert(SpellAreaForAuraMap::value_type(abs(spellArea.auraSpell), sa));

    } while(result->NextRow());

    sLog->outString(">> Loaded spell area requirements in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

SpellCastResult SpellMgr::GetSpellAllowedInLocationError(SpellEntry const *spellInfo, uint32 map_id, uint32 zone_id, uint32 area_id, Player const* player)
{
    // normal case
    if(spellInfo->AreaGroupId > 0)
    {
        bool found = false;
        AreaGroupEntry const* groupEntry = sAreaGroupStore.LookupEntry(spellInfo->AreaGroupId);
        while(groupEntry)
        {
            for(uint8 i = 0; i < MAX_GROUP_AREA_IDS; ++i)
                if(groupEntry->AreaId[i] == zone_id || groupEntry->AreaId[i] == area_id)
                    found = true;
            if(found || !groupEntry->nextGroup)
                break;
            // Try search in next group
            groupEntry = sAreaGroupStore.LookupEntry(groupEntry->nextGroup);
        }

        if(!found)
            return SPELL_FAILED_INCORRECT_AREA;
    }

    // continent limitation (virtual continent)
    if(spellInfo->AttributesEx4 & SPELL_ATTR4_CAST_ONLY_IN_OUTLAND)
    {
        uint32 v_map = GetVirtualMapForMapAndZone(map_id, zone_id);
        MapEntry const* mapEntry = sMapStore.LookupEntry(v_map);
        if(!mapEntry || mapEntry->addon < 1 || !mapEntry->IsContinent())
            return SPELL_FAILED_INCORRECT_AREA;
    }

    // raid instance limitation
    if(spellInfo->AttributesEx6 & SPELL_ATTR6_NOT_IN_RAID_INSTANCE)
    {
        MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
        if(!mapEntry || mapEntry->IsRaid())
            return SPELL_FAILED_NOT_IN_RAID_INSTANCE;
    }

    // DB base check (if non empty then must fit at least single for allow)
    SpellAreaMapBounds saBounds = sSpellMgr->GetSpellAreaMapBounds(spellInfo->Id);
    if(saBounds.first != saBounds.second)
    {
        for(SpellAreaMap::const_iterator itr = saBounds.first; itr != saBounds.second; ++itr)
        {
            if(itr->second.IsFitToRequirements(player, zone_id, area_id))
                return SPELL_CAST_OK;
        }
        return SPELL_FAILED_INCORRECT_AREA;
    }

    // bg spell checks
    switch(spellInfo->Id)
    {
        case 23333:                                         // Warsong Flag
        case 23335:                                         // Silverwing Flag
            return map_id == 489 && player && player->InBattleground() ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        case 34976:                                         // Netherstorm Flag
            return map_id == 566 && player && player->InBattleground() ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        case 2584:                                          // Waiting to Resurrect
        case 22011:                                         // Spirit Heal Channel
        case 22012:                                         // Spirit Heal
        case 24171:                                         // Resurrection Impact Visual
        case 42792:                                         // Recently Dropped Flag
        case 43681:                                         // Inactive
        case 44535:                                         // Spirit Heal (mana)
        {
            MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
            if(!mapEntry)
                return SPELL_FAILED_INCORRECT_AREA;

            return zone_id == 4197 || (mapEntry->IsBattleground() && player && player->InBattleground()) ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        }
        case 44521:                                         // Preparation
        {
            if(!player)
                return SPELL_FAILED_REQUIRES_AREA;

            MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
            if(!mapEntry)
                return SPELL_FAILED_INCORRECT_AREA;

            if(!mapEntry->IsBattleground())
                return SPELL_FAILED_REQUIRES_AREA;

            Battleground* pBG = player->GetBattleground();
            return pBG && pBG->GetStatus() == STATUS_WAIT_JOIN ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        }
        case 32724:                                         // Gold Team (Alliance)
        case 32725:                                         // Green Team (Alliance)
        case 35774:                                         // Gold Team (Horde)
        case 35775:                                         // Green Team (Horde)
        {
            MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
            if(!mapEntry)
                return SPELL_FAILED_INCORRECT_AREA;

            return mapEntry->IsBattleArena() && player && player->InBattleground() ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        }
        case 32727:                                         // Arena Preparation
        {
            if(!player)
                return SPELL_FAILED_REQUIRES_AREA;

            MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
            if(!mapEntry)
                return SPELL_FAILED_INCORRECT_AREA;

            if(!mapEntry->IsBattleArena())
                return SPELL_FAILED_REQUIRES_AREA;

            Battleground* pBG = player->GetBattleground();
            return pBG && pBG->GetStatus() == STATUS_WAIT_JOIN ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        }
    }

    // aura limitations
    for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        switch(spellInfo->EffectApplyAuraName[i])
        {
            case SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED:
            case SPELL_AURA_FLY:
            {
                if(player && !player->IsKnowHowFlyIn(map_id, zone_id))
                    return SPELL_FAILED_INCORRECT_AREA;
            }
        }
    }

    return SPELL_CAST_OK;
}

void SpellMgr::LoadSkillLineAbilityMap()
{
    uint32 oldMSTime = getMSTime();

    mSkillLineAbilityMap.clear();

    for(uint32 i = 0; i < sSkillLineAbilityStore.GetNumRows(); ++i)
    {
        SkillLineAbilityEntry const* SkillInfo = sSkillLineAbilityStore.LookupEntry(i);
        if(!SkillInfo)
            continue;

        mSkillLineAbilityMap.insert(SkillLineAbilityMap::value_type(SkillInfo->spellId, SkillInfo));
    }

    sLog->outString(">> Loaded SkillLineAbility MultiMap Data in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

DiminishingGroup GetDiminishingReturnsGroupForSpell(SpellEntry const* spellproto, bool triggered)
{
    if(IsPositiveSpell(spellproto->Id))
        return DIMINISHING_NONE;

    for(uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if(spellproto->EffectApplyAuraName[i] == SPELL_AURA_MOD_TAUNT)
            return DIMINISHING_TAUNT;
    }

    // Explicit Diminishing Groups
    switch(spellproto->SpellFamilyName)
    {
        // Event spells
        case SPELLFAMILY_UNK1:
            return DIMINISHING_NONE;
        case SPELLFAMILY_DEATHKNIGHT:
        {
            // Hungering Cold (no flags)
            if(spellproto->SpellIconID == 2797)
                return DIMINISHING_DISORIENT;
            // Mark of Blood
            else if((spellproto->SpellFamilyFlags[0] & 0x10000000) && spellproto->SpellIconID == 2285)
                return DIMINISHING_LIMITONLY;
            break;
        }
        case SPELLFAMILY_DRUID:
        {
            // Pounce
            if(spellproto->SpellFamilyFlags[0] & 0x20000)
                return DIMINISHING_OPENING_STUN;
            // Cyclone
            else if(spellproto->SpellFamilyFlags[1] & 0x20)
                return DIMINISHING_CYCLONE;
            // Entangling Roots
            // Nature's Grasp
            else if(spellproto->SpellFamilyFlags[0] & 0x00000200)
                return DIMINISHING_CONTROLLED_ROOT;
            // Faerie Fire
            else if(spellproto->SpellFamilyFlags[0] & 0x400)
                return DIMINISHING_LIMITONLY;
            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            // Hunter's mark
            if((spellproto->SpellFamilyFlags[0] & 0x400) && spellproto->SpellIconID == 538)
                return DIMINISHING_LIMITONLY;
            // Scatter Shot (own diminishing)
            else if((spellproto->SpellFamilyFlags[0] & 0x40000) && spellproto->SpellIconID == 132)
                return DIMINISHING_SCATTER_SHOT;
            // Entrapment (own diminishing)
            else if(spellproto->SpellVisual[0] == 7484 && spellproto->SpellIconID == 20)
                return DIMINISHING_ENTRAPMENT;
            // Wyvern Sting mechanic is MECHANIC_SLEEP but the diminishing is DIMINISHING_DISORIENT
            else if((spellproto->SpellFamilyFlags[1] & 0x1000) && spellproto->SpellIconID == 1721)
                return DIMINISHING_DISORIENT;
            // Freezing Arrow
            else if(spellproto->SpellFamilyFlags[0] & 0x8)
                return DIMINISHING_DISORIENT;
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            // Judgement of Justice - limit duration to 10s in PvP
            if(spellproto->SpellFamilyFlags[0] & 0x100000)
                return DIMINISHING_LIMITONLY;
            // Turn Evil
            else if((spellproto->SpellFamilyFlags[1] & 0x804000) && spellproto->SpellIconID == 309)
                return DIMINISHING_FEAR;
            break;
        }
        case SPELLFAMILY_SHAMAN:
        {
            // Storm, Earth and Fire - Earthgrab
            if(spellproto->SpellFamilyFlags[2] & 0x4000)
                return DIMINISHING_LIMITONLY;
            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            // Psychic Horror
            if(spellproto->SpellFamilyFlags[2] & 0x2000)
                return DIMINISHING_HORROR;
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            // Gouge
            if(spellproto->SpellFamilyFlags[0] & 0x8)
                return DIMINISHING_DISORIENT;
            // Blind
            else if(spellproto->SpellFamilyFlags[0] & 0x1000000)
                return DIMINISHING_FEAR;
            // Cheap Shot
            else if(spellproto->SpellFamilyFlags[0] & 0x400)
                return DIMINISHING_OPENING_STUN;
            // Crippling poison - Limit to 10 seconds in PvP (No SpellFamilyFlags)
            else if(spellproto->SpellIconID == 163)
                return DIMINISHING_LIMITONLY;
            break;
        }
        case SPELLFAMILY_MAGE:
        {
            // Frostbite
            if(spellproto->SpellFamilyFlags[1] & 0x80000000)
                return DIMINISHING_ROOT;
            // Shattered Barrier
            else if(spellproto->SpellVisual[0] == 12297)
                return DIMINISHING_ROOT;
            // Deep Freeze
            else if(spellproto->SpellIconID == 2939 && spellproto->SpellVisual[0] == 9963)
                return DIMINISHING_CONTROLLED_STUN;
            // Frost Nova / Freeze (Water Elemental)
            else if(spellproto->SpellIconID == 193)
                return DIMINISHING_CONTROLLED_ROOT;
            // Dragon's Breath
            else if(spellproto->SpellFamilyFlags[0] & 0x800000)
                return DIMINISHING_DRAGONS_BREATH;
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            // Death Coil
            if(spellproto->SpellFamilyFlags[0] & 0x80000)
                return DIMINISHING_HORROR;
            // Curses/etc
            else if((spellproto->SpellFamilyFlags[0] & 0x80000000) || (spellproto->SpellFamilyFlags[1] & 0x200))
                return DIMINISHING_LIMITONLY;
            // Seduction
            else if(spellproto->SpellFamilyFlags[1] & 0x10000000)
                return DIMINISHING_FEAR;
            break;
        }
        case SPELLFAMILY_WARRIOR:
        {
            // Hamstring - limit duration to 10s in PvP
            if(spellproto->SpellFamilyFlags[0] & 0x2)
                return DIMINISHING_LIMITONLY;
            // Improved Hamstring
            else if(spellproto->AttributesEx3 & 0x80000 && spellproto->SpellIconID == 23)
                return DIMINISHING_ROOT;
            // Charge Stun (own diminishing)
            else if(spellproto->SpellFamilyFlags[0] & 0x01000000)
                return DIMINISHING_CHARGE;
            break;
        }
        case SPELLFAMILY_GENERIC:
        {
            switch(spellproto->Id)
            {
                case 51750: // Screams of the Dead
                    return DIMINISHING_NONE;
                case 47481: // Gnaw
                    return DIMINISHING_CONTROLLED_STUN;
                default:
                    break;
            }
            // Pet charge effects (Infernal Awakening, Demon Charge)
            if(spellproto->SpellVisual[0] == 2816 && spellproto->SpellIconID == 15)
                return DIMINISHING_CONTROLLED_STUN;
        }
        default:
            break;
    }

    // Lastly - Set diminishing depending on mechanic
    uint32 mechanic = GetAllSpellMechanicMask(spellproto);
    if(mechanic & (1 << MECHANIC_CHARM))
        return DIMINISHING_MIND_CONTROL;
    if(mechanic & (1 << MECHANIC_SILENCE))
        return DIMINISHING_SILENCE;
    if(mechanic & (1 << MECHANIC_SLEEP))
        return DIMINISHING_SLEEP;
    if(mechanic & ((1 << MECHANIC_SAPPED) | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_SHACKLE)))
        return DIMINISHING_DISORIENT;
    // Mechanic Knockout, except Blast Wave
    if(mechanic & (1 << MECHANIC_KNOCKOUT) && spellproto->SpellIconID != 292)
        return DIMINISHING_DISORIENT;
    if(mechanic & (1 << MECHANIC_DISARM))
        return DIMINISHING_DISARM;
    if(mechanic & (1 << MECHANIC_FEAR))
        return DIMINISHING_FEAR;
    if(mechanic & (1 << MECHANIC_STUN))
        return triggered ? DIMINISHING_STUN : DIMINISHING_CONTROLLED_STUN;
    if(mechanic & (1 << MECHANIC_BANISH))
        return DIMINISHING_BANISH;
    if(mechanic & (1 << MECHANIC_ROOT))
        return triggered ? DIMINISHING_ROOT : DIMINISHING_CONTROLLED_ROOT;

    return DIMINISHING_NONE;
}

int32 GetDiminishingReturnsLimitDuration(DiminishingGroup pGroup, SpellEntry const* pSpellEntry)
{
    if(!IsDiminishingReturnsGroupDurationLimited(pGroup))
        return 0;

    // Explicit diminishing duration
    switch(pSpellEntry->SpellFamilyName)
    {
        case SPELLFAMILY_DRUID:
        {
            // Faerie Fire - limit to 40 seconds in PvP (3.1)
            if(pSpellEntry->SpellFamilyFlags[0] & 0x400)
                return 40 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            // Wyvern Sting
            if(pSpellEntry->SpellFamilyFlags[1] & 0x1000)
                return 6 * IN_MILLISECONDS;
            // Hunter's Mark
            if(pSpellEntry->SpellFamilyFlags[0] & 0x400)
                return 120 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            // Repentance - limit to 6 seconds in PvP
            if(pSpellEntry->SpellFamilyFlags[0] & 0x4)
                return 6 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            // Banish - limit to 6 seconds in PvP
            if(pSpellEntry->SpellFamilyFlags[1] & 0x8000000)
                return 6 * IN_MILLISECONDS;
            // Curse of Tongues - limit to 12 seconds in PvP
            else if(pSpellEntry->SpellFamilyFlags[2] & 0x800)
                return 12 * IN_MILLISECONDS;
            // Curse of Elements - limit to 120 seconds in PvP
            else if(pSpellEntry->SpellFamilyFlags[1] & 0x200)
               return 120 * IN_MILLISECONDS;
            break;
        }
        default:
            break;
    }

    return 10 * IN_MILLISECONDS;
}

bool IsDiminishingReturnsGroupDurationLimited(DiminishingGroup group)
{
    switch(group)
    {
        case DIMINISHING_CONTROLLED_STUN:
        case DIMINISHING_STUN:
        case DIMINISHING_ENTRAPMENT:
        case DIMINISHING_CONTROLLED_ROOT:
        case DIMINISHING_ROOT:
        case DIMINISHING_FEAR:
        case DIMINISHING_MIND_CONTROL:
        case DIMINISHING_DISORIENT:
        case DIMINISHING_CYCLONE:
        case DIMINISHING_BANISH:
        case DIMINISHING_LIMITONLY:
        case DIMINISHING_OPENING_STUN:
        case DIMINISHING_HORROR:
        case DIMINISHING_SLEEP:
            return true;
        default:
            return false;
    }
}

DiminishingLevels GetDiminishingReturnsMaxLevel(DiminishingGroup group)
{
    switch(group)
    {
        case DIMINISHING_TAUNT:
            return DIMINISHING_LEVEL_TAUNT_IMMUNE;
        default:
            return DIMINISHING_LEVEL_IMMUNE;
    }
}

DiminishingReturnsType GetDiminishingReturnsGroupType(DiminishingGroup group)
{
    switch(group)
    {
        case DIMINISHING_TAUNT:
        case DIMINISHING_CONTROLLED_STUN:
        case DIMINISHING_STUN:
        case DIMINISHING_OPENING_STUN:
        case DIMINISHING_CYCLONE:
        case DIMINISHING_CHARGE:
            return DRTYPE_ALL;
        case DIMINISHING_FEAR:
        case DIMINISHING_CONTROLLED_ROOT:
        case DIMINISHING_ROOT:
        case DIMINISHING_MIND_CONTROL:
        case DIMINISHING_DISORIENT:
        case DIMINISHING_ENTRAPMENT:
        case DIMINISHING_SILENCE:
        case DIMINISHING_DISARM:
        case DIMINISHING_BANISH:
        case DIMINISHING_SCATTER_SHOT:
        case DIMINISHING_HORROR:
        case DIMINISHING_SLEEP:
            return DRTYPE_PLAYER;
        default:
            break;
    }

    return DRTYPE_NONE;
}

bool IsPartOfSkillLine(uint32 skillId, uint32 spellId)
{
    SkillLineAbilityMapBounds skillBounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellId);
    for(SkillLineAbilityMap::const_iterator itr = skillBounds.first; itr != skillBounds.second; ++itr)
        if(itr->second->skillId == skillId)
            return true;

    return false;
}

bool SpellArea::IsFitToRequirements(Player const* pPlayer, uint32 newZone, uint32 newArea) const
{
    OutdoorPvPWG *pvpWG = (OutdoorPvPWG*)sOutdoorPvPMgr->GetOutdoorPvPToZoneId(4197);

    if(gender != GENDER_NONE)                   // not in expected gender
        if(!pPlayer || gender != pPlayer->getGender())
            return false;

    if(raceMask)                                // not in expected race
        if(!pPlayer || !(raceMask & pPlayer->getRaceMask()))
            return false;

    if(areaId)                                  // not in expected zone
        if(newZone != areaId && newArea != areaId)
            return false;

    if(questStart)                              // not in expected required quest state
        if(!pPlayer || ((!questStartCanActive || !pPlayer->IsActiveQuest(questStart)) && !pPlayer->GetQuestRewardStatus(questStart)))
            return false;

    if(questEnd)                                // not in expected forbidden quest state
        if(!pPlayer || pPlayer->GetQuestRewardStatus(questEnd))
            return false;

    if(auraSpell)                               // not have expected aura
        if(!pPlayer || (auraSpell > 0 && !pPlayer->HasAura(auraSpell)) || (auraSpell < 0 && pPlayer->HasAura(-auraSpell)))
            return false;

    // Extra conditions -- leaving the possibility add extra conditions...
    switch(spellId)
    {
        case 58600: // No fly Zone - Dalaran
        {
            if(!pPlayer)
                return false;
            AreaTableEntry const* pArea = GetAreaEntryByAreaID(pPlayer->GetAreaId());
            if(!(pArea && pArea->flags & AREA_FLAG_NO_FLY_ZONE))
                return false;
            if(!pPlayer->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) && !pPlayer->HasAuraType(SPELL_AURA_FLY))
                return false;
            break;
        }
        case 58730: // No fly Zone - Wintergrasp
            if(sWorld->getBoolConfig(CONFIG_OUTDOORPVP_WINTERGRASP_ENABLED))
            {
                if(!pPlayer || !pvpWG)
                    return false;

                if(!pvpWG->isWarTime() ||
                    !pPlayer->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED)
                 && !pPlayer->HasAuraType(SPELL_AURA_FLY)
                 || pPlayer->HasAura(45472)
                 || pPlayer->HasAura(44795))
                     return false;
            }
            break;
        case 58045: // Essence of Wintergrasp - Wintergrasp
        case 57940: // Essence of Wintergrasp - Northrend
            if(!pPlayer || !pvpWG)
                return false;
            if((uint8)pPlayer->GetTeamId() != sWorld->getWorldState(WORLDSTATE_WINTERGRASP_CONTROLING_FACTION))
                return false;
            break;/*
        case 55774: // Alliance Control Phase
            if(newZone == 4197 && sWorld->getWorldState(WORLDSTATE_WINTERGRASP_DEFENDERS) != TEAM_HORDE)
                return false;
            break;
        case 55773: // Horde Control Phase
            if(newZone == 4197 && sWorld->getWorldState(WORLDSTATE_WINTERGRASP_DEFENDERS) != TEAM_ALLIANCE)
                return false;
            break;*/
        case 68719: // Oil Refinery - Isle of Conquest.
        case 68720: // Quarry - Isle of Conquest.
        {
            if(pPlayer->GetBattlegroundTypeId() != BATTLEGROUND_IC || !pPlayer->GetBattleground())
                return false;

            uint8 nodeType  = spellId == 68719 ? NODE_TYPE_REFINERY : NODE_TYPE_QUARRY;
            uint8 nodeState = pPlayer->GetTeamId() == TEAM_ALLIANCE ? NODE_STATE_CONTROLLED_A : NODE_STATE_CONTROLLED_H;

            BattlegroundIC* pIC = static_cast<BattlegroundIC*>(pPlayer->GetBattleground());
            if(pIC->GetNodeState(nodeType) == nodeState)
                return true;

            return false;
        }
    }
    return true;
}

bool SpellMgr::CanAurasStack(Aura const *aura1, Aura const *aura2, bool sameCaster) const
{
    SpellEntry const* spellInfo_1 = aura1->GetSpellProto();
    SpellEntry const* spellInfo_2 = aura2->GetSpellProto();
    SpellSpecific spellSpec_1 = GetSpellSpecific(spellInfo_1);
    SpellSpecific spellSpec_2 = GetSpellSpecific(spellInfo_2);
    if(spellSpec_1 && spellSpec_2)
        if(IsSingleFromSpellSpecificPerTarget(spellSpec_1, spellSpec_2)
            || (sameCaster && IsSingleFromSpellSpecificPerCaster(spellSpec_1, spellSpec_2)))
            return false;

    SpellGroupStackRule stackRule = CheckSpellGroupStackRules(spellInfo_1->Id, spellInfo_2->Id);
    if(stackRule)
    {
        if(stackRule == SPELL_GROUP_STACK_RULE_EXCLUSIVE)
            return false;
        if(sameCaster && stackRule == SPELL_GROUP_STACK_RULE_EXCLUSIVE_FROM_SAME_CASTER)
            return false;
    }

    if(spellInfo_1->SpellFamilyName != spellInfo_2->SpellFamilyName)
        return true;

    if(!sameCaster)
    {
        if(spellInfo_1->AttributesEx3 & SPELL_ATTR3_STACK_FOR_DIFF_CASTERS)
            return true;

        // check same periodic auras
        for(uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            switch(spellInfo_1->EffectApplyAuraName[i])
            {
                // DOT or HOT from different casters will stack
                case SPELL_AURA_PERIODIC_DAMAGE:
                case SPELL_AURA_PERIODIC_DUMMY:
                case SPELL_AURA_PERIODIC_HEAL:
                case SPELL_AURA_PERIODIC_TRIGGER_SPELL:
                case SPELL_AURA_PERIODIC_ENERGIZE:
                case SPELL_AURA_PERIODIC_MANA_LEECH:
                case SPELL_AURA_PERIODIC_LEECH:
                case SPELL_AURA_POWER_BURN_MANA:
                case SPELL_AURA_OBS_MOD_POWER:
                case SPELL_AURA_OBS_MOD_HEALTH:
                case SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE:
                    // periodic auras which target areas are not allowed to stack this way (replenishment for example)
                    if(IsAreaOfEffectSpellEffect(spellInfo_1, i) || IsAreaOfEffectSpellEffect(spellInfo_2, i))
                        break;
                    return true;
                default:
                    break;
            }
        }
    }

    bool isVehicleAura1 = false;
    bool isVehicleAura2 = false;
    uint8 i = 0;

    while(i < MAX_SPELL_EFFECTS && !(isVehicleAura1 && isVehicleAura2))
    {
        if(spellInfo_1->EffectApplyAuraName[i] == SPELL_AURA_CONTROL_VEHICLE)
            isVehicleAura1 = true;
        if(spellInfo_2->EffectApplyAuraName[i] == SPELL_AURA_CONTROL_VEHICLE)
            isVehicleAura2 = true;

        ++i;
    }

    if(isVehicleAura1 && isVehicleAura2)
    {
        Vehicle* veh = NULL;
        if(aura1->GetOwner()->ToUnit())
            veh = aura1->GetOwner()->ToUnit()->GetVehicleKit();

        if(!veh)           // We should probably just let it stack. Vehicle system will prevent undefined behaviour later
            return true;

        if(!veh->GetAvailableSeatCount())
            return false;   // No empty seat available

        return true;        // Empty seat available (skip rest)
    }

    uint32 spellId_1 = GetLastSpellInChain(spellInfo_1->Id);
    uint32 spellId_2 = GetLastSpellInChain(spellInfo_2->Id);

    // same spell
    if(spellId_1 == spellId_2)
    {
        if(aura1->GetCastItemGUID() && aura2->GetCastItemGUID())
            if(aura1->GetCastItemGUID() != aura2->GetCastItemGUID() && (GetSpellCustomAttr(spellId_1) & SPELL_ATTR0_CU_ENCHANT_PROC))
                return true;

        switch(spellId_1)
        {
            case 30166: // Shadow Grasp
            case 30207: // Shadow Grasp
            case 30531: // Soul Transfer
            case 44413: // Hack for Incanter's Absorption
            case 55849: // Power Spark
            case 62505: // Harpoon Shot
                return true;
            default:
                return false; // same spell with same caster should not stack
        }
    }

    return true;
}

bool CanSpellDispelAura(SpellEntry const* dispelSpell, SpellEntry const* aura)
{
    // These auras (like ressurection sickness) can't be dispelled
    if(aura->Attributes & SPELL_ATTR0_NEGATIVE_1)
        return false;

    // These spells (like Mass Dispel) can dispell all auras
    if(dispelSpell->Attributes & SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY)
        return true;

    // These auras (like Divine Shield) can't be dispelled
    if(aura->Attributes & SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY)
        return false;

    // These auras (Cyclone for example) are not dispelable
    if(aura->AttributesEx & SPELL_ATTR1_UNAFFECTED_BY_SCHOOL_IMMUNE)
        return false;

    return true;
}

bool CanSpellPierceImmuneAura(SpellEntry const* pierceSpell, SpellEntry const* aura)
{
    // these spells pierce all avalible spells (Resurrection Sickness for example)
    if(pierceSpell->Attributes & SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY)
        return true;

    // these spells (Cyclone for example) can pierce all...
    if((pierceSpell->AttributesEx & SPELL_ATTR1_UNAFFECTED_BY_SCHOOL_IMMUNE)
        // ...but not these (Divine shield for example)
        && !(aura && (aura->Mechanic == MECHANIC_IMMUNE_SHIELD || aura->Mechanic == MECHANIC_INVULNERABILITY)))
        return true;

    return false;
}

void SpellMgr::LoadSpellEnchantProcData()
{
    uint32 oldMSTime = getMSTime();

    mSpellEnchantProcEventMap.clear();                             // need for reload case

    //                                                  0         1           2         3
    QueryResult result = WorldDatabase.Query("SELECT entry, customChance, PPMChance, procEx FROM spell_enchant_proc_data");
    if(!result)
    {
        sLog->outString(">> Loaded 0 spell enchant proc event conditions");
        sLog->outString();
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 enchantId = fields[0].GetUInt32();

        SpellItemEnchantmentEntry const* ench = sSpellItemEnchantmentStore.LookupEntry(enchantId);
        if(!ench)
        {
            sLog->outErrorDb("Enchancment %u listed in `spell_enchant_proc_data` does not exist", enchantId);
            continue;
        }

        SpellEnchantProcEntry spe;

        spe.customChance = fields[1].GetUInt32();
        spe.PPMChance = fields[2].GetFloat();
        spe.procEx = fields[3].GetUInt32();

        mSpellEnchantProcEventMap[enchantId] = spe;

    } while(result->NextRow());

    sLog->outString(">> Loaded enchant proc data definitions in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void SpellMgr::LoadSpellRequired()
{
    uint32 oldMSTime = getMSTime();

    mSpellsReqSpell.clear();                                   // need for reload case
    mSpellReq.clear();                                         // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT spell_id, req_spell from spell_required");

    if(!result)
    {
        sLog->outString(">> Loaded 0 spell required records");
        sLog->outString();
        sLog->outErrorDb("`spell_required` table is empty!");
        return;
    }
    uint32 rows = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 spell_id =  fields[0].GetUInt32();
        uint32 spell_req = fields[1].GetUInt32();
        // check if chain is made with valid first spell
        SpellEntry const* spell = sSpellStore.LookupEntry(spell_id);
        if(!spell)
        {
            sLog->outErrorDb("spell_id %u in `spell_required` table is not found in dbcs, skipped", spell_id);
            continue;
        }
        SpellEntry const* req_spell = sSpellStore.LookupEntry(spell_req);
        if(!req_spell)
        {
            sLog->outErrorDb("req_spell %u in `spell_required` table is not found in dbcs, skipped", spell_req);
            continue;
        }
        if(GetFirstSpellInChain(spell_id) == GetFirstSpellInChain(spell_req))
        {
            sLog->outErrorDb("req_spell %u and spell_id %u in `spell_required` table are ranks of the same spell, entry not needed, skipped", spell_req, spell_id);
            continue;
        }
        if(IsSpellRequiringSpell(spell_id, spell_req))
        {
            sLog->outErrorDb("duplicated entry of req_spell %u and spell_id %u in `spell_required`, skipped", spell_req, spell_id);
            continue;
        }

        mSpellReq.insert (std::pair<uint32, uint32>(spell_id, spell_req));
        mSpellsReqSpell.insert (std::pair<uint32, uint32>(spell_req, spell_id));
        ++rows;
    } while(result->NextRow());

    sLog->outString(">> Loaded %u spell required records in %u ms", rows, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void SpellMgr::LoadSpellRanks()
{
    uint32 oldMSTime = getMSTime();

    mSpellChains.clear();                                   // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT first_spell_id, spell_id, rank from spell_ranks ORDER BY first_spell_id, rank");

    if(!result)
    {
        sLog->outString(">> Loaded 0 spell rank records");
        sLog->outString();
        sLog->outErrorDb("`spell_ranks` table is empty!");
        return;
    }

    uint32 rows = 0;
    bool finished = false;

    do
    {   // spellid, rank
        std::list < std::pair < int32, int32 > > rankChain;
        int32 currentSpell = -1;
        int32 lastSpell = -1;

        // fill one chain
        while(currentSpell == lastSpell && !finished)
        {
            Field* fields = result->Fetch();

            currentSpell = fields[0].GetUInt32();
            if(lastSpell == -1)
                lastSpell = currentSpell;
            uint32 spell_id = fields[1].GetUInt32();
            uint32 rank = fields[2].GetUInt32();

            // don't drop the row if we're moving to the next rank
            if(currentSpell == lastSpell)
            {
                rankChain.push_back(std::make_pair(spell_id, rank));
                if(!result->NextRow())
                    finished = true;
            }
            else
                break;
        }
        // check if chain is made with valid first spell
        SpellEntry const* first = sSpellStore.LookupEntry(lastSpell);
        if(!first)
        {
            sLog->outErrorDb("Spell rank identifier(first_spell_id) %u listed in `spell_ranks` does not exist!", lastSpell);
            continue;
        }
        // check if chain is long enough
        if(rankChain.size() < 2)
        {
            sLog->outErrorDb("There is only 1 spell rank for identifier(first_spell_id) %u in `spell_ranks`, entry is not needed!", lastSpell);
            continue;
        }
        int32 curRank = 0;
        bool valid = true;
        // check spells in chain
        for(std::list<std::pair<int32, int32> >::iterator itr = rankChain.begin() ; itr!= rankChain.end(); ++itr)
        {
            SpellEntry const* spell = sSpellStore.LookupEntry(itr->first);
            if(!spell)
            {
                sLog->outErrorDb("Spell %u (rank %u) listed in `spell_ranks` for chain %u does not exist!", itr->first, itr->second, lastSpell);
                valid = false;
                break;
            }
            ++curRank;
            if(itr->second != curRank)
            {
                sLog->outErrorDb("Spell %u (rank %u) listed in `spell_ranks` for chain %u does not have proper rank value(should be %u)!", itr->first, itr->second, lastSpell, curRank);
                valid = false;
                break;
            }
        }
        if(!valid)
            continue;
        int32 prevRank = 0;
        // insert the chain
        std::list<std::pair<int32, int32> >::iterator itr = rankChain.begin();
        do
        {
            ++rows;
            int32 addedSpell = itr->first;
            mSpellChains[addedSpell].first = lastSpell;
            mSpellChains[addedSpell].last = rankChain.back().first;
            mSpellChains[addedSpell].rank = itr->second;
            mSpellChains[addedSpell].prev = prevRank;
            prevRank = addedSpell;
            ++itr;
            if(itr == rankChain.end())
            {
                mSpellChains[addedSpell].next = 0;
                break;
            }
            else
                mSpellChains[addedSpell].next = itr->first;
        }
        while(true);
    } while(!finished);

    sLog->outString(">> Loaded %u spell rank records in %u ms", rows, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

// set data in core for now
void SpellMgr::LoadSpellCustomAttr()
{
    uint32 oldMSTime = getMSTime();

    mSpellCustomAttr.resize(GetSpellStore()->GetNumRows(), 0);  // initialize with 0 values

    SpellEntry* spellInfo = NULL;
    for(uint32 i = 0; i < sSpellStore.GetNumRows(); ++i)
    {
        spellInfo = (SpellEntry*)sSpellStore.LookupEntry(i);
        if(!spellInfo)
            continue;

        for(uint8 j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            switch(spellInfo->Effect[j])
            {
                case SPELL_EFFECT_SCHOOL_DAMAGE:
                case SPELL_EFFECT_WEAPON_DAMAGE:
                case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
                case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
                case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
                case SPELL_EFFECT_HEAL:
                    mSpellCustomAttr[i] |= SPELL_ATTR0_CU_DIRECT_DAMAGE;
                    break;
                case SPELL_EFFECT_POWER_DRAIN:
                case SPELL_EFFECT_POWER_BURN:
                case SPELL_EFFECT_HEAL_MAX_HEALTH:
                case SPELL_EFFECT_HEALTH_LEECH:
                case SPELL_EFFECT_HEAL_PCT:
                case SPELL_EFFECT_ENERGIZE_PCT:
                case SPELL_EFFECT_ENERGIZE:
                case SPELL_EFFECT_HEAL_MECHANICAL:
                    mSpellCustomAttr[i] |= SPELL_ATTR0_CU_NO_INITIAL_THREAT;
                    break;
                case SPELL_EFFECT_CHARGE:
                case SPELL_EFFECT_CHARGE_DEST:
                case SPELL_EFFECT_JUMP:
                case SPELL_EFFECT_JUMP_DEST:
                case SPELL_EFFECT_LEAP_BACK:
                    if(!spellInfo->speed && !spellInfo->SpellFamilyName)
                        spellInfo->speed = SPEED_CHARGE;
                    mSpellCustomAttr[i] |= SPELL_ATTR0_CU_CHARGE;
                    break;
                case SPELL_EFFECT_PICKPOCKET:
                    mSpellCustomAttr[i] |= SPELL_ATTR0_CU_PICKPOCKET;
                    break;
                case SPELL_EFFECT_TRIGGER_SPELL:
                    if(IsPositionTarget(spellInfo->EffectImplicitTargetA[j]) ||
                        spellInfo->Targets & (TARGET_FLAG_SOURCE_LOCATION | TARGET_FLAG_DEST_LOCATION))
                        spellInfo->Effect[j] = SPELL_EFFECT_TRIGGER_MISSILE;
                    break;
                case SPELL_EFFECT_ENCHANT_ITEM:
                case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
                case SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC:
                case SPELL_EFFECT_ENCHANT_HELD_ITEM:
                {
                    // only enchanting profession enchantments procs can stack
                    if(IsPartOfSkillLine(SKILL_ENCHANTING, i))
                    {
                        uint32 enchantId = spellInfo->EffectMiscValue[j];
                        SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(enchantId);
                        for(uint8 s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s)
                        {
                            if(enchant->type[s] != ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL)
                                continue;

                            SpellEntry const* procInfo = sSpellStore.LookupEntry(enchant->spellid[s]);
                            if(!procInfo)
                                continue;

                            // if proced directly from enchantment, not via proc aura
                            // NOTE: Enchant Weapon - Blade Ward also has proc aura spell and is proced directly
                            // however its not expected to stack so this check is good
                            if(IsSpellHaveAura(procInfo, SPELL_AURA_PROC_TRIGGER_SPELL))
                                continue;

                            mSpellCustomAttr[enchant->spellid[s]] |= SPELL_ATTR0_CU_ENCHANT_PROC;
                        }
                    }
                    break;
                }
            }

            switch(SpellTargetType[spellInfo->EffectImplicitTargetA[j]])
            {
                case TARGET_TYPE_UNIT_TARGET:
                case TARGET_TYPE_DEST_TARGET:
                    spellInfo->Targets |= TARGET_FLAG_UNIT;
                    break;
                default:
                    break;
            }
        }

        for(uint8 j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            switch(spellInfo->EffectApplyAuraName[j])
            {
                case SPELL_AURA_MOD_POSSESS:
                case SPELL_AURA_MOD_CHARM:
                case SPELL_AURA_AOE_CHARM:
                    mSpellCustomAttr[i] |= SPELL_ATTR0_CU_AURA_CC;
                    break;
                case SPELL_AURA_MOD_FEAR:
                case SPELL_AURA_MOD_STUN:
                case SPELL_AURA_MOD_CONFUSE:
                    if(spellInfo->speed == 0 && spellInfo->Mechanic != MECHANIC_FREEZE)
                        spellInfo->speed = 46;
                    mSpellCustomAttr[i] |= SPELL_ATTR0_CU_AURA_CC;
                    break;
                case SPELL_AURA_PERIODIC_HEAL:
                case SPELL_AURA_PERIODIC_DAMAGE:
                case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
                case SPELL_AURA_PERIODIC_LEECH:
                case SPELL_AURA_PERIODIC_MANA_LEECH:
                case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
                case SPELL_AURA_PERIODIC_ENERGIZE:
                case SPELL_AURA_OBS_MOD_HEALTH:
                case SPELL_AURA_OBS_MOD_POWER:
                case SPELL_AURA_POWER_BURN_MANA:
                    mSpellCustomAttr[i] |= SPELL_ATTR0_CU_NO_INITIAL_THREAT;
                    break;
            }
        }

        if(!_isPositiveEffect(i, 0, false))
            mSpellCustomAttr[i] |= SPELL_ATTR0_CU_NEGATIVE_EFF0;

        if(!_isPositiveEffect(i, 1, false))
            mSpellCustomAttr[i] |= SPELL_ATTR0_CU_NEGATIVE_EFF1;

        if(!_isPositiveEffect(i, 2, false))
            mSpellCustomAttr[i] |= SPELL_ATTR0_CU_NEGATIVE_EFF2;

        if(spellInfo->SpellVisual[0] == 3879)
            mSpellCustomAttr[i] |= SPELL_ATTR0_CU_CONE_BACK;

        if(spellInfo->activeIconID == 2158)  // flight
            spellInfo->Attributes |= SPELL_ATTR0_PASSIVE;

        switch(i)
        {
            case 30283: // Shadow Fury - [Warlock]
            case 30413: // Shadow Fury - [Warlock]
            case 30414: // Shadow Fury - [Warlock]
            case 47846: // Shadow Fury - [Warlock]
            case 47847: // Shadow Fury - [Warlock]
            case 64044: // Psychic Horror - [Priest]
            case 46968: // Shockwave - [Warrior]
            case 33786: // Cyclone - [Druid]
            case 44572: // Deep Freeze - [Mage]
            case 36554: // Shadowstep - [Rogue]
                spellInfo->speed = 300;
                break;
            case 8178: // Grounding Totem - [Shaman]
                spellInfo->procFlags = 0;
                break;
            case 51632: // Blood Spatter (Rank 1)
                spellInfo->Effect[0] = 6;
                spellInfo->EffectBasePoints[0] = 15;
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                spellInfo->EffectImplicitTargetB[0] = 0;
                spellInfo->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->EffectMiscValue[0] = 22;
                break;
            case 51633: // Blood Spatter (Rank 2)
                spellInfo->Effect[0] = 6;
                spellInfo->EffectBasePoints[0] = 30;
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                spellInfo->EffectImplicitTargetB[0] = 0;
                spellInfo->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->EffectMiscValue[0] = 22;
                break;
            case 14171: // Serrated Blades (Rank 1)
                spellInfo->Effect[0] = 6;
                spellInfo->EffectBasePoints[0] = 10;
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                spellInfo->EffectImplicitTargetB[0] = 0;
                spellInfo->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->EffectMiscValue[0] = 22;
                break;
            case 14172: // Serrated Blades (Rank 2)
                spellInfo->Effect[0] = 6;
                spellInfo->EffectBasePoints[0] = 20;
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                spellInfo->EffectImplicitTargetB[0] = 0;
                spellInfo->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->EffectMiscValue[0] = 22;
                break;
            case 14173: // Serrated Blades (Rank 3)
                spellInfo->Effect[0] = 6;
                spellInfo->EffectBasePoints[0] = 30;
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                spellInfo->EffectImplicitTargetB[0] = 0;
                spellInfo->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->EffectMiscValue[0] = 22;
                break;
            case 14162: // Improved Eviscerate (Rank 1)
                spellInfo->Effect[0] = 6;
                spellInfo->EffectBasePoints[0] = 7;
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                spellInfo->EffectImplicitTargetB[0] = 0;
                spellInfo->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->EffectMiscValue[0] = 0;
                break;
            case 14163: // Improved Eviscerate (Rank 2)
                spellInfo->Effect[0] = 6;
                spellInfo->EffectBasePoints[0] = 14;
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                spellInfo->EffectImplicitTargetB[0] = 0;
                spellInfo->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->EffectMiscValue[0] = 0;
                break;
            case 14164: // Improved Eviscerate (Rank 3)
                spellInfo->Effect[0] = 6;
                spellInfo->EffectBasePoints[0] = 20;
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                spellInfo->EffectImplicitTargetB[0] = 0;
                spellInfo->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->EffectMiscValue[0] = 0;
                break;
            case 72502:
            case 72501:
                spellInfo->InterruptFlags |= SPELL_INTERRUPT_FLAG_INTERRUPT;
                break;
            case 55098:
                spellInfo->InterruptFlags = 13;
                spellInfo->CastingTimeIndex = 15;
                break;
            case 40244: case 40245: // Simon Game Visual
            case 40246: case 40247: // Simon Game Visual
            case 42835: // Spout, remove damage effect, only anim is needed
                spellInfo->Effect[0] = 0;
                break;
            case 40055: // Introspection
            case 40165: // Introspection
            case 40166: // Introspection
            case 40167: // Introspection
                spellInfo->Attributes |= SPELL_ATTR0_NEGATIVE_1;
                break;
            case 37770:
                spellInfo->Effect[0] = 2;
                break;
            case 30541:
                spellInfo->EffectImplicitTargetA[0] = 6;
                spellInfo->EffectImplicitTargetB[0] = 0;
                break;
            case 30657:
                spellInfo->EffectTriggerSpell[0] = 30571;
                break;
            case 63665: // Charge (Argent Tournament emote on riders)
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                spellInfo->EffectImplicitTargetB[0] = 0;
                break;
            case 31344:
                spellInfo->EffectRadiusIndex[0] = 12;
                break;
            case 50833:
                spellInfo->InterruptFlags = 0;
                break;
            case 51136:
                spellInfo->EffectImplicitTargetA[0] = 6;
                spellInfo->EffectImplicitTargetA[1] = 6;
                spellInfo->EffectImplicitTargetB[0] = 0;
                spellInfo->EffectImplicitTargetB[1] = 0;
                spellInfo->Effect[1] = 0;
                break;
            case 51001:
                spellInfo->EffectImplicitTargetA[0] = 6;
                spellInfo->EffectImplicitTargetA[1] = 6;
                spellInfo->EffectImplicitTargetB[0] = 0;
                spellInfo->EffectImplicitTargetB[1] = 0;
                break;
            case 31447:
            case 31298:
                spellInfo->EffectImplicitTargetA[0] = 1;
                spellInfo->EffectImplicitTargetB[0] = 0;
                break;
            case 44869: // Spectral Blast
                spellInfo->MaxAffectedTargets = 1;
                spellInfo->EffectImplicitTargetB[0] = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 42821:
            case 42818:
                spellInfo->rangeIndex = 6;
                break;
            case 43301:
                spellInfo->CastingTimeIndex = 5;
                break;
            case 43657:
                spellInfo->EffectBasePoints[1] = 49;
                break;
            case 1776: // Gouge
            case 1777:
            case 8629:
            case 11285:
            case 11286:
            case 12540:
            case 13579:
            case 24698:
            case 28456:
            case 29425:
            case 34940:
            case 36862:
            case 38764:
            case 38863:
            case 52743: // Head Smack
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_REQ_TARGET_FACING_CASTER;
                break;
            case 53257: // Cobra Strikes
                spellInfo->procCharges = 2;
                spellInfo->StackAmount = 0;
                break;
            case 53386: // Rune of Cinderglacier
                spellInfo->StackAmount = 0;
                spellInfo->procCharges = 2;
                spellInfo->procFlags &= PROC_FLAG_DONE_PERIODIC|PROC_FLAG_TAKEN_PERIODIC;
                break;
            case 53:    // Backstab
            case 2589:
            case 2590:
            case 2591:
            case 8721:
            case 11279:
            case 11280:
            case 11281:
            case 25300:
            case 26863:
            case 48656:
            case 48657:
            case 703: // Garrote
            case 8631:
            case 8632:
            case 8633:
            case 11289:
            case 11290:
            case 26839:
            case 26884:
            case 48675:
            case 48676:
            case 5221: // Shred
            case 6800:
            case 8992:
            case 9829:
            case 9830:
            case 27001:
            case 27002:
            case 48571:
            case 48572:
            case 8676: // Ambush
            case 8724:
            case 8725:
            case 11267:
            case 11268:
            case 11269:
            case 27441:
            case 48689:
            case 48690:
            case 48691:
            case 6785: // Ravage
            case 6787:
            case 9866:
            case 9867:
            case 27005:
            case 48578:
            case 48579:
            case 21987: // Lash of Pain
            case 23959: // Test Stab R50
            case 24825: // Test Backstab
            case 58563: // Assassinate Restless Lookout
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_REQ_CASTER_BEHIND_TARGET;
                break;
            case 36350: //They Must Burn Bomb Aura (self)
                spellInfo->EffectTriggerSpell[0] = 36325; // They Must Burn Bomb Drop (DND)
                break;
            case 32645: // Envenom (Rank 1)
            case 32684: // Envenom (Rank 2)
            case 57992: // Envenom (Rank 3)
            case 57993: // Envenom (Rank 4)
                spellInfo->Dispel = DISPEL_NONE;
                break;
            case 48278: // Sword Ritual (Utgarde - Slava)
                spellInfo->Targets = 1;
                spellInfo->StackAmount = 3;
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->EffectImplicitTargetB[0] = TARGET_UNIT_NEARBY_ENEMY;
                break;
            case 49838: // Stop Time
            case 52916: // Honor Among Thieves
            case 5171:  // Slice and Dice
            case 6774:  // Slice and Dice  Rank2
            case 49376: // Feral Charge (Cat)
            case 61138: // Feral Charge (Cat) effect
            case 50259: // Feral charge (Cat) effect 2
            case 61132: // Feral Charge (cat) effect 3
            case 52610: // Savage Roar
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_INITIAL_AGGRO;
                break;
            case 57994: // Wind Shear - improper data for EFFECT_1 in 3.3.5 DBC, but is correct in 4.x
                spellInfo->Effect[EFFECT_1] = SPELL_EFFECT_MODIFY_THREAT_PERCENT;
                spellInfo->EffectBasePoints[EFFECT_1] = -6; // -5%
                break;
            case 50526: // Wandering Plague
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_INITIAL_AGGRO;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 61407: // Energize Cores
            case 62136: // Energize Cores
            case 54069: // Energize Cores
            case 56251: // Energize Cores
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_AREA_ENTRY_SRC;
                break;
            case 50785: // Energize Cores
            case 59372: // Energize Cores
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_AREA_ENEMY_SRC;
                break;
            case 29444: // Magic Absorption
            case 29441:
                spellInfo->spellLevel = 0;
                break;
            case 3286:  // Bind
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->EffectImplicitTargetA[1] = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 45524: // Chains of Ice
                spellInfo->EffectImplicitTargetA[2] = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 8494: // Mana Shield (rank 2)
                // because of bug in dbc
                spellInfo->procChance = 0;
                break;
            case 32182: // Heroism
                spellInfo->excludeCasterAuraSpell = 57723; // Exhaustion
                break;
            case 59921: // Frost Fever
                spellInfo->AttributesEx4 |= SPELL_ATTR4_UNK3;
                break;
            case 2825:  // Bloodlust
                spellInfo->excludeCasterAuraSpell = 57724; // Sated
                break;
            case 44440: // Fiery Payback hack
            case 44441:
                spellInfo->CasterAuraStateNot = AURA_STATE_NONE;
                break;
            case 20335: // Heart of the Crusader
            case 20336:
            case 20337:
            case 63320: // Glyph of Life Tap
            // Entries were not updated after spell effect change, we have to do that manually :/
                spellInfo->AttributesEx3 |= SPELL_ATTR3_CAN_PROC_WITH_TRIGGERED;
                break;
            case 16007: // Draco-Incarcinatrix 900
                // was 46, but effect is aura effect
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_NEARBY_ENTRY;
                spellInfo->EffectImplicitTargetB[0] = TARGET_DST_NEARBY_ENTRY;
                break;
            case 26029: // Dark Glare
            case 37433: // Spout
            case 43140: // Flame Breath
            case 43215: // Flame Breath
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_NEGATIVE_EFF0;
                break;
            case 70461: // Coldflame Trap
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_CONE_LINE;
                break;
            case 24340: // Meteor
            case 26558: // Meteor
            case 28884: // Meteor
            case 36837: // Meteor
            case 38903: // Meteor
            case 41276: // Meteor
            case 57467: // Meteor
            case 26789: // Shard of the Fallen Star
            case 31436: // Malevolent Cleave
            case 35181: // Dive Bomb
            case 40810: // Saber Lash
            case 43267: // Saber Lash
            case 43268: // Saber Lash
            case 42384: // Brutal Swipe
            case 45150: // Meteor Slash
            case 64688: // Sonic Screech
            case 72373: // Shared Suffering
            case 71904: // Chaos Bane
            case 70492: // Ooze Eruption
            case 72505: // Ooze Eruption
            case 72624: // Ooze Eruption
            case 72625: // Ooze Eruption
                // ONLY SPELLS WITH SPELLFAMILY_GENERIC and EFFECT_SCHOOL_DAMAGE
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                break;
            case 59725: // Improved Spell Reflection - aoe aura
                // Target entry seems to be wrong for this spell :/
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_PARTY_CASTER;
                spellInfo->EffectRadiusIndex[0] = 10;
                break;
            case 27820: // Mana Detonation
            case 69782: // Ooze Flood
            case 69796: // Ooze Flood
            case 69798: // Ooze Flood
            case 69801: // Ooze Flood
            case 69538: // Ooze Combine
            case 69553: // Ooze Combine
            case 69610: // Ooze Combine
            case 71447: // Bloodbolt Splash
            case 71481: // Bloodbolt Splash
            case 71482: // Bloodbolt Splash
            case 71483: // Bloodbolt Splash
            case 71390: // Pact of the Darkfallen
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_EXCLUDE_SELF;
                break;
            case 64844: // Divine Hymn
            case 64904: // Hymn of Hope
                spellInfo->AttributesEx &= ~SPELL_ATTR1_CANT_BE_REFLECTED;
                break;
            case 44978: case 45001: case 45002: // Wild Magic
            case 45004: case 45006: case 45010: // Wild Magic
            case 31347: // Doom
            case 41635: // Prayer of Mending
            case 45027: // Revitalize
            case 45976: // Muru Portal Channel
            case 39365: // Thundering Storm
            case 41071: // Raise Dead (HACK)
            case 52124: // Sky Darkener Assault
            case 42442: // Vengeance Landing Cannonfire
            case 45863: // Cosmetic - Incinerate to Random Target
            case 25425: // Shoot
            case 45761: // Shoot
            case 42611: // Shoot
            case 71838: // Drain Life - Bryntroll Normal
            case 71839: // Drain Life - Bryntroll Heroic
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
                break;
            case 34471: // The Beast Within
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_CONFUSED | SPELL_ATTR5_USABLE_WHILE_FEARED | SPELL_ATTR5_USABLE_WHILE_STUNNED;
                break;
            case 62374: // Pursued
            case 61588: // Blazing Harpoon
            case 50988: // Glare of the Tribunal (nh)
            case 59870: // Glare of the Tribunal (hc)
            case 55927: // Sear Beam (nh)
            case 59509: // Sear Beam (hc)
            case 56397: // Arcane Barrage
            case 64599: // Arcane Barrage
            case 64607: // Arcane Barrage
            case 47731: // Critter
            case 52479: // Gift of the Harvester
                spellInfo->MaxAffectedTargets = 1;
                // a trap always has dst = src?
                spellInfo->EffectImplicitTargetA[0] = TARGET_DST_CASTER;
                spellInfo->EffectImplicitTargetA[1] = TARGET_DST_CASTER;
                break;
            case 41376: // Spite
            case 39992: // Needle Spine
            case 29576: // Multi-Shot
            case 40816: // Saber Lash
            case 37790: // Spread Shot
            case 46771: // Flame Sear
            case 45248: // Shadow Blades
            case 41303: // Soul Drain
            case 29213: // Curse of the Plaguebringer - Noth
            case 28542: // Life Drain - Sapphiron
            case 66588: // Flaming Spear
            case 54171: // Divine Storm (Main)
            case 54172: // Divine Storm (Heal)
                spellInfo->MaxAffectedTargets = 3;
                break;
            case 68645: // Rocket Pack!
                //spellInfo->EffectMiscValueB[0] = 70;
                spellInfo->Effect[0] = SPELL_EFFECT_KNOCK_BACK_DEST;
                spellInfo->EffectMiscValue[0] = -250;
                spellInfo->EffectBasePoints[0] = 150;
                break;
            case 38310: // Multi-Shot
            case 53385: // Divine Storm (Damage)
                spellInfo->MaxAffectedTargets = 4;
                break;
            case 42005: // Bloodboil
            case 38296: // Spitfire Totem
            case 37676: // Insidious Whisper
            case 46008: // Negative Energy
            case 45641: // Fire Bloom
            case 55665: // Life Drain - Sapphiron (H)
            case 28796: // Poison Bolt Volly - Faerlina
                spellInfo->MaxAffectedTargets = 5;
                break;
            case 40827: // Sinful Beam
            case 40859: // Sinister Beam
            case 40860: // Vile Beam
            case 40861: // Wicked Beam
            case 54835: // Curse of the Plaguebringer - Noth (H)
            case 54098: // Poison Bolt Volly - Faerlina (H)
                spellInfo->MaxAffectedTargets = 10;
                break;
            case 50312: // Unholy Frenzy
                spellInfo->MaxAffectedTargets = 15;
                break;
            case 38794: case 33711: //Murmur's Touch
                spellInfo->MaxAffectedTargets = 1;
                spellInfo->EffectTriggerSpell[0] = 33760;
                break;
           case 69839: // big ooze explode cast
                spellInfo->CastingTimeIndex = 15; // 4000 ms casting time
                break;
            case 17941: // Shadow Trance
            case 22008: // Netherwind Focus
            case 31834: // Light's Grace
            case 34754: // Clearcasting
            case 34936: // Backlash
            case 48108: // Hot Streak
            case 51124: // Killing Machine
            case 54741: // Firestarter
            case 57761: // Fireball!
            case 39805: // Lightning Overload
            case 57934: // Tricks of the trade
            case 64823: // Item - Druid T8 Balance 4P Bonus
            case 44401: // Missile Barrage
                spellInfo->procCharges = 1;
                break;
            case 35466: // Illusions
            case 37805:
            case 37806:
            case 37807:
            case 37808:
            case 37809:
            case 37810:
            case 37811:
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                spellInfo->AttributesEx4 |= SPELL_ATTR4_NOT_USABLE_IN_ARENA;
                break;
            case 63623: // Avoidance shadow fiend
            case 62137: // Avoidance summoned ghoul
            case 32233: // Avoidance warlock pet
            case 65220: // Avoidance hunter pet
                spellInfo->EffectApplyAuraName[0] = SPELL_AURA_MOD_CREATURE_AOE_DAMAGE_AVOIDANCE;
                break;
            case 53407: // Judgement of the Justice
            case 20271: // Judgement of the Light
            case 53408: // Judgement of the Wisdom
                spellInfo->Attributes |= SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK;
                break;
            case 67210: // Rogue T9 2P Bonus
                spellInfo->procCharges = 1;
                break;
            case 44544: // Fingers of Frost
                spellInfo->EffectSpellClassMask[0] = flag96(685904631, 1151048, 0);
                break;
            case 74396: // Fingers of Frost visual buff
                spellInfo->procCharges = 2;
                spellInfo->StackAmount = 0;
                break;
            case 28200: // Ascendance (Talisman of Ascendance trinket)
                spellInfo->procCharges = 6;
                break;
            case 47201: // Everlasting Affliction
            case 47202:
            case 47203:
            case 47204:
            case 47205:
                // add corruption to affected spells
                spellInfo->EffectSpellClassMask[1][0] |= 2;
                break;
            case 49305: // Teleport to Boss 1 DND
            case 64981: // Summon Random Vanquished Tentacle
                spellInfo->EffectImplicitTargetB[0] = TARGET_UNIT_CASTER;
                break;
            case 51852: // The Eye of Acherus (no spawn in phase 2 in db)
                spellInfo->EffectMiscValue[0] |= 1;
                break;
            case 51904: // Summon Ghouls On Scarlet Crusade (core does not know the triggered spell is summon spell)
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                break;
            case 29809: // Desecration Arm - 36 instead of 37 - typo? :/
                spellInfo->EffectRadiusIndex[0] = 37;
                break;
            case 67201: // Item - Priest T9 Healing 2P Bonus
                spellInfo->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->EffectMiscValue[0] = 0;
                break;
            case 67202: // Item - Priest T9 Healing 4P Bonus
                spellInfo->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->EffectMiscValue[0] = 0;
                spellInfo->EffectSpellClassMask[0] = flag96(0x00000000, 0x01000000, 0x00001000);
                break;
            case 18754: // Improved succubus - problems with apply if target is pet
            case 18755:
            case 18756:
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                break;
            // Master Shapeshifter: missing stance data for forms other than bear - bear version has correct data
            // To prevent aura staying on target after talent unlearned
            case 48420:
                spellInfo->Stances = 1 << (FORM_CAT - 1);
                break;
            case 48421:
                spellInfo->Stances = 1 << (FORM_MOONKIN - 1);
                break;
            case 48422:
                spellInfo->Stances = 1 << (FORM_TREE - 1);
                break;
            case 47569: // Improved Shadowform (Rank 1)
                // with this spell atrribute aura can be stacked several times
                spellInfo->Attributes &= ~SPELL_ATTR0_NOT_SHAPESHIFT;
                break;
            case 55689: // Glyph of Shadow (to prevent glyph aura loss)
                spellInfo->AttributesEx2 |= SPELL_ATTR2_NOT_NEED_SHAPESHIFT;
                break;
            case 30421: // Nether Portal - Perseverence
                spellInfo->EffectBasePoints[2] += 30000;
                break;
            case 49575: // death grip (leap effect)
                spellInfo->AttributesEx |= !SPELL_ATTR1_CANT_BE_REFLECTED;
                break;
            case 61607: // Mark of Blood
                spellInfo->AttributesEx |= SPELL_ATTR1_NO_THREAT;
                break;
            case 66665: // Burning Breath
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 62907: // Freya's Ward
            case 62947:
                spellInfo->DurationIndex = 0;
                break;
            case 14792:
                spellInfo->DurationIndex = 9; // 90 Seconds
                break;
            case 62713: // Ironbranch's Essence
            case 62968: // Brightleaf's Essence
                spellInfo->DurationIndex = 39;
                break;
            case 62661: // Searing Flames
            case 61915: // Lightning Whirl 10
            case 63483: // Lightning Whirl 25
                spellInfo->InterruptFlags = 47;
                break;
            case 16834: // Natural shapeshifter
            case 16835:
                spellInfo->DurationIndex = 21;
                break;
            case 72762: // Defile
                spellInfo->DurationIndex = 3;
                break;
            case 71614: // Ice Prison
                spellInfo->Targets = 6; //target chain damage
                break;
            case 69410: // Soul Reaper
                spellInfo->Targets = 1;
                break;
            case 74074: // Plague Siphon
                spellInfo->Targets = 18;
                break;
            case 69200: // Summon Raging Spirit
                spellInfo->DurationIndex = 28;
                spellInfo->Effect[0] = 6;
                break;
            case 73529: // Shadow Trap
                spellInfo->EffectRadiusIndex[1] = 13;
                break;
            case 72376: // Raise Dead
            case 72351: // Fury of Frostmourne
                spellInfo->EffectRadiusIndex[0] = 22;
                break;
            case 72350: // Fury of Frostmourne
                spellInfo->Effect[1] = SPELL_EFFECT_INSTAKILL;
                spellInfo->EffectRadiusIndex[0] = 22;
                spellInfo->EffectRadiusIndex[1] = 22;
                spellInfo->EffectImplicitTargetA[0] = TARGET_SRC_CASTER;
                spellInfo->EffectImplicitTargetB[0] = TARGET_UNIT_AREA_ENEMY_SRC;
                spellInfo->EffectAmplitude[0] = 50000;
                break;
            case 72429: // Mass Resurrection
                spellInfo->EffectRadiusIndex[0] = 4;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_REQUIRE_DEAD_TARGET;
                break;
            case 72754: // Defile Damage
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->EffectImplicitTargetB[1] = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 68981: // Remorseless Winter
            case 74270:
            case 74271:
            case 74272:
                spellInfo->Effect[2] = 0;
                break;
            case 74276: // In Frostmourne Room
                spellInfo->AttributesEx3 = SPELL_ATTR3_DEATH_PERSISTENT;
                break;
            case 73159: // Play Movie
                spellInfo->EffectImplicitTargetB[0] = TARGET_UNIT_AREA_ENEMY_SRC;
                spellInfo->EffectRadiusIndex[0] = 22;
                break;
            case 51735: // Ebon Plague
            case 51734:
            case 51726:
                spellInfo->SpellFamilyFlags[2] = 0x10;
                spellInfo->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
                break;
            case 41913: // Parasitic Shadowfiend Passive
                spellInfo->EffectApplyAuraName[0] = 4; // proc debuff, and summon infinite fiends
                break;
            case 27892: // To Anchor 1
            case 27928: // To Anchor 1
            case 27935: // To Anchor 1
            case 27915: // Anchor to Skulls
            case 27931: // Anchor to Skulls
            case 27937: // Anchor to Skulls
                spellInfo->rangeIndex = 13;
                break;
            case 49224: // Magic Suppression
            case 49610:
            case 49611:
                spellInfo->procCharges = 0;
                break;
            case 48743: // Death Pact
                spellInfo->AttributesEx &= ~SPELL_ATTR1_CANT_TARGET_SELF;
                break;
            // target allys instead of enemies, target A is src_caster, spells with effect like that have ally target
            // this is the only known exception, probably just wrong data
            case 29214: // Wrath of the Plaguebringer
            case 54836: // Wrath of the Plaguebringer
                spellInfo->EffectImplicitTargetB[0] = TARGET_UNIT_AREA_ALLY_SRC;
                spellInfo->EffectImplicitTargetB[1] = TARGET_UNIT_AREA_ALLY_SRC;
                break;
            case 31687: // Summon Water Elemental
                // 322-330 switch - effect changed to dummy, target entry not changed in client:(
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                break;
            case 12051: // Evocation - now we can interrupt this
                spellInfo->InterruptFlags |= SPELL_INTERRUPT_FLAG_INTERRUPT;
                break;
            case 61851: // Killing Spree - should remove snares from caster
                spellInfo->AttributesEx |= SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY;
                break;
            case 18500: // Wing Buffet
            case 33086: // Wild Bite
            case 49749: // Piercing Blow
            case 52890: // Penetrating Strike
            case 53454: // Impale
            case 59446: // Impale
            case 55276: // Puncture
            case 59826: // Puncture
            case 62383: // Shatter
            case 64777: // Machine Gun
            case 65239: // Machine Gun
            case 65919: // Impale
            case 67858: // Impale
            case 67859: // Impale
            case 67860: // Impale
            case 69293: // Wing Buffet
            case 74439: // Machine Gun
            case 62544: // Thrust (Argent Tournament)
            case 64588: // Thrust (Argent Tournament)
            case 66479: // Thrust (Argent Tournament)
            case 68505: // Thrust (Argent Tournament)
            case 62709: // Counterattack! (Argent Tournament)
            case 62626: // Break-Shield (Argent Tournament, Player)
            case 64590: // Break-Shield (Argent Tournament, Player)
            case 64342: // Break-Shield (Argent Tournament, NPC)
            case 64686: // Break-Shield (Argent Tournament, NPC)
            case 65147: // Break-Shield (Argent Tournament, NPC)
            case 68504: // Break-Shield (Argent Tournament, NPC)
            case 62874: // Charge (Argent Tournament, Player)
            case 68498: // Charge (Argent Tournament, Player)
            case 64591: // Charge (Argent Tournament, Player)
            case 63003: // Charge (Argent Tournament, NPC)
            case 63010: // Charge (Argent Tournament, NPC)
            case 68321: // Charge (Argent Tournament, NPC)
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case 20224: // Seals of the Pure (Rank 1)
            case 20225: // Seals of the Pure (Rank 2)
            case 20330: // Seals of the Pure (Rank 3)
            case 20331: // Seals of the Pure (Rank 4)
            case 20332: // Seals of the Pure (Rank 5)
                spellInfo->EffectSpellClassMask[EFFECT_0][1] = 0x20400800;
                break;
            case 66532:
            case 66963:
            case 66964:
            case 66965: // Fel Fireball
                 spellInfo->InterruptFlags = 0x0000000F;
                 break;
            case 63675: // Improved Devouring Plague
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 8145: // Tremor Totem (instant pulse)
            case 6474: // Earthbind Totem (instant pulse)
                spellInfo->AttributesEx5 |= SPELL_ATTR5_START_PERIODIC_AT_APPLY;
                break;
            case 53241: // Marked for Death (Rank 1)
            case 53243: // Marked for Death (Rank 2)
            case 53244: // Marked for Death (Rank 3)
            case 53245: // Marked for Death (Rank 4)
            case 53246: // Marked for Death (Rank 5)
                spellInfo->EffectSpellClassMask[0] = flag96(423937, 276955137, 2049);
                break;
            case 70728: // Exploit Weakness
            case 70840: // Devious Minds
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                spellInfo->EffectImplicitTargetB[0] = TARGET_UNIT_PET;
                break;
            case 70893: // Culling The Herd
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_CASTER;
                spellInfo->EffectImplicitTargetB[0] = TARGET_UNIT_MASTER;
                break;
            case 54800:
                // Sigil of the Frozen Conscience - change class mask to custom extended flags of Icy Touch
                // this is done because another spell also uses the same SpellFamilyFlags as Icy Touch
                // SpellFamilyFlags[0] & 0x00000040 in SPELLFAMILY_DEATHKNIGHT is currently unused (3.3.5a)
                // this needs research on modifier applying rules, does not seem to be in Attributes fields
                spellInfo->EffectSpellClassMask[0] = flag96(0x00000040, 0x00000000, 0x00000000);
                break;
            case 64949: // Idol of the Flourishing Life
                spellInfo->EffectSpellClassMask[EFFECT_0] = flag96(0x00000000, 0x02000000, 0x00000000);
                spellInfo->EffectApplyAuraName[EFFECT_0] = SPELL_AURA_ADD_FLAT_MODIFIER;
                break;
            case 34231: // Libram of the Lightbringer
            case 60792: // Libram of Tolerance
            case 64956: // Libram of the Resolute
                spellInfo->EffectSpellClassMask[EFFECT_0] = flag96(0x80000000, 0x00000000, 0x00000000);
                spellInfo->EffectApplyAuraName[EFFECT_0] = SPELL_AURA_ADD_FLAT_MODIFIER;
                break;
            case 28851: // Libram of Light
            case 28853: // Libram of Divinity
            case 32403: // Blessed Book of Nagrand
                spellInfo->EffectSpellClassMask[EFFECT_0] = flag96(0x40000000, 0x00000000, 0x00000000);
                spellInfo->EffectApplyAuraName[EFFECT_0] = SPELL_AURA_ADD_FLAT_MODIFIER;
                break;
            case 63163: // Apply Enchanted Bridle (Argent Tournament)
                spellInfo->EffectDieSides[0] = 0;
                break;
            case 19970: // Entangling Roots (Rank 6) -- Nature's Grasp Proc
            case 19971: // Entangling Roots (Rank 5) -- Nature's Grasp Proc
            case 19972: // Entangling Roots (Rank 4) -- Nature's Grasp Proc
            case 19973: // Entangling Roots (Rank 3) -- Nature's Grasp Proc
            case 19974: // Entangling Roots (Rank 2) -- Nature's Grasp Proc
            case 19975: // Entangling Roots (Rank 1) -- Nature's Grasp Proc
            case 27010: // Entangling Roots (Rank 7) -- Nature's Grasp Proc
            case 53313: // Entangling Roots (Rank 8) -- Nature's Grasp Proc
                spellInfo->CastingTimeIndex = 1;
                break;
            case 61719: // Easter Lay Noblegarden Egg Aura - Interrupt flags copied from aura which this aura is linked with
                spellInfo->AuraInterruptFlags = AURA_INTERRUPT_FLAG_HITBYSPELL | AURA_INTERRUPT_FLAG_TAKE_DAMAGE;
                break;
            case 63018: // Searing Light (XT-002)
            case 65121: // Searing Light (25m) (XT-002)
            case 63024: // Gravity Bomb (XT-002)
            case 64234: // Gravity Bomb (25m) (XT-002)
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 62834: // Boom (XT-002)
                // This hack is here because we suspect our implementation of spell effect execution on targets
                // is done in the wrong order. We suspect that EFFECT_0 needs to be applied on all targets,
                // then EFFECT_1, etc - instead of applying each effect on target1, then target2, etc.
                // The above situation causes the visual for this spell to be bugged, so we remove the instakill
                // effect and implement a script hack for that.
                spellInfo->Effect[EFFECT_1] = 0;
                break;
            case 62016: // Thorim - Charge Orb
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_IGNORE_LOS;
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 62039: // Hodir - Biting Cold - Remove on Move
                spellInfo->AuraInterruptFlags |= AURA_INTERRUPT_FLAG_MOVE;
                break;
            case 62775: // XT-002 - Tympanic Tantrum
            case 64443: // Algalon - Big Bang
            case 64584: // Algalon - Big Bang
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case 63025: // XT-002 Gravity Bomb
            case 64233: // XT-002 Gravity Bomb
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_EXCLUDE_SELF;
                break;
            case 65210: // Keeper Mimiron Destabilization Matrix - Ignore LoS (because Mimiron stands in a Tube and is out of LoS)
            case 62042: // Thorim - Stormhammer
            case 62521: // Freya - Attuned to Nature 25 Dose Reduction
            case 62524: // Freya - Attuned to Nature 2 Dose Reduction
            case 62525: // Freya - Attuned to Nature 10 Dose Reduction
            case 58875: // Shaman - Spirit Walk - Feral Spirit
            case 7720:  // Warlock: Ritual of Summoning
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_IGNORE_LOS;
                break;
            case 62488: // Ignis Activate Construct (only visually)
            case 65301: // Sara Psychosis
            case 63830: // Sara Malady of the Mind
            case 64465: // Yogg Saron Shadow Beacon
            case 63342: // Focused Eyebeam Summon Trigger (Kologarn)
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 63802: // Sara Brain Link
                spellInfo->MaxAffectedTargets = 2;
                break;
            case 62716: // Growth of Nature (Freya)
            case 65584: // Growth of Nature (Freya)
            case 64381: // Strength of the Pack (Auriaya)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                break;
            case 63293: // Mimiron - P3Wx2 Laser Barrage
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_CONE_LINE;
                break;
            case 64600: // Freya - Nature Bomb (GO Visual)
                spellInfo->DurationIndex = 38;
                break;
            case 62056: // Kologarn - some Stone Grip related Spells that have SPELL_ATTR1_IGNORE_IMMUNITY (NYI?)
            case 63985:
            case 64224:
            case 64225:
            case 64205: // Divine Sacrifice
            case 6940:  // Hand Of Sacrifice
            case 62287: // Tar Passive
                spellInfo->Attributes |= SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY;
                break;
            case 62711: // Ignis - Grab
                spellInfo->Attributes |= SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY;
                spellInfo->AttributesEx2 |= SPELL_ATTR2_UNK2;
                break;
            case 62470: // Deafening Thunder - Spell::DoSpellHitOnUnit sends EVADE if speed > 0
                spellInfo->speed = 0;
                break;
            case 64386: // Terrifying Screech (Auriaya)
            case 64389: // Sentinel Blast (Auriaya)
            case 64678: // Sentinel Blast (Auriaya)
                spellInfo->DurationIndex = 28; // 5 seconds, wrong DBC data?
                break;
            case 63278: // Mark of the Faceless (General Vezax)
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case 64422: // Sonic Screech (Auriaya)
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case 64321: // Potent Pheromones (Freya)
                // spell should dispel area aura, but doesn't have the attribute
                // may be db data bug, or blizz may keep reapplying area auras every update with checking immunity
                // that will be clear if we get more spells with problem like this
                spellInfo->AttributesEx |= SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY;
                break;
            case 66258: // Infernal Eruption (10N)
            case 67901: // Infernal Eruption (25N)
                // increase duration from 15 to 18 seconds because caster is already
                // unsummoned when spell missile hits the ground so nothing happen in result
                spellInfo->DurationIndex = 85;
                break;
            case 70781: // Light's Hammer Teleport
            case 70856: // Oratory of the Damned Teleport
            case 70857: // Rampart of Skulls Teleport
            case 70858: // Deathbringer's Rise Teleport
            case 70859: // Upper Spire Teleport
            case 70860: // Frozen Throne Teleport
            case 70861: // Sindragosa's Lair Teleport
                spellInfo->EffectImplicitTargetA[0] = TARGET_DST_DB;
                spellInfo->speed = 0;
                break;
            case 69055: // Saber Lash (Lord Marrowgar)
            case 70814: // Saber Lash (Lord Marrowgar)
                spellInfo->EffectRadiusIndex[0] = 8;    // 5yd
                spellInfo->AttributesEx |= SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK;
                break;
            case 69075: // Bone Storm (Lord Marrowgar)
            case 70834: // Bone Storm (Lord Marrowgar)
            case 70835: // Bone Storm (Lord Marrowgar)
            case 70836: // Bone Storm (Lord Marrowgar)
                spellInfo->EffectRadiusIndex[0] = 12;
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case 72864: // Death Plague (Rotting Frost Giant)
            case 72378: // Blood Nova (Deathbringer Saurfang)
            case 73058: // Blood Nova (Deathbringer Saurfang)
            case 71160: // Plague Stench (Stinky)
            case 71161: // Plague Stench (Stinky)
                spellInfo->EffectRadiusIndex[0] = 12;   // 100yd
                break;
            case 71123: // Decimate (Stinky & Precious)
                spellInfo->EffectRadiusIndex[0] = 28;   // 100yd
                break;
            case 72385: // Boiling Blood (Deathbringer Saurfang)
            case 72441: // Boiling Blood (Deathbringer Saurfang)
            case 72442: // Boiling Blood (Deathbringer Saurfang)
            case 72443: // Boiling Blood (Deathbringer Saurfang)
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->EffectImplicitTargetB[0] = 0;
                break;
            case 72723: // Resistant Skin (Deathbringer Saurfang adds)
                // this spell initially granted Shadow damage immunity, however it was removed but the data was left in client
                spellInfo->Effect[2] = 0;
                break;
            case 70460: // Coldflame Jets (Traps after Saurfang)
                spellInfo->DurationIndex = 1;   // 10 seconds
                break;
            case 71413: // Green Ooze Summon (Professor Putricide)
            case 71414: // Orange Ooze Summon (Professor Putricide)
                spellInfo->EffectImplicitTargetA[0] = TARGET_DEST_DEST;
                break;
            case 71159: // Awaken Plagued Zombies
                spellInfo->DurationIndex = 21;
                break;
            // THIS IS HERE BECAUSE COOLDOWN ON CREATURE PROCS IS NOT IMPLEMENTED
            case 71604: // Mutated Strength (Professor Putricide)
            case 72673: // Mutated Strength (Professor Putricide)
            case 72674: // Mutated Strength (Professor Putricide)
            case 72675: // Mutated Strength (Professor Putricide)
                spellInfo->Effect[1] = 0;
                break;
            case 70911: // Unbound Plague (Professor Putricide)
            case 72854: // Unbound Plague (Professor Putricide)
            case 72855: // Unbound Plague (Professor Putricide)
            case 72856: // Unbound Plague (Professor Putricide)
                spellInfo->EffectImplicitTargetB[0] = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 72454: // Mutated Plague (Professor Putricide)
            case 72464: // Mutated Plague (Professor Putricide)
            case 72506: // Mutated Plague (Professor Putricide)
            case 72507: // Mutated Plague (Professor Putricide)
                spellInfo->EffectRadiusIndex[0] = 28;   // 50000yd
                break;
            case 71518: // Unholy Infusion Quest Credit (Professor Putricide)
            case 72934: // Blood Infusion Quest Credit (Blood-Queen Lana'thel)
            case 72289: // Frost Infusion Quest Credit (Sindragosa)
                spellInfo->EffectRadiusIndex[0] = 28;   // another missing radius
                break;
            case 71708: // Empowered Flare (Blood Prince Council)
            case 72785: // Empowered Flare (Blood Prince Council)
            case 72786: // Empowered Flare (Blood Prince Council)
            case 72787: // Empowered Flare (Blood Prince Council)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 17364: // Stormstrike
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                break;
            case 71266: // Swarming Shadows
            case 72890: // Swarming Shadows
                spellInfo->AreaGroupId = 0; // originally, these require area 4522, which is... outside of Icecrown Citadel
                break;
            case 70602: // Corruption
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                break;
            case 70715: // Column of Frost (visual marker)
                spellInfo->DurationIndex = 32; // 6 seconds (missing)
                break;
            case 71085: // Mana Void (periodic aura)
                spellInfo->DurationIndex = 9; // 30 seconds (missing)
                break;
            case 70936: // Summon Suppressor
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_TARGET_ANY;
                spellInfo->EffectImplicitTargetB[0] = 0;
                break;
            case 72706: // Achievement Check (Valithria Dreamwalker)
            case 71357: // Order Whelp
                spellInfo->EffectRadiusIndex[0] = 22;   // 200yd
                break;
            case 70598: // Sindragosa's Fury
                spellInfo->EffectImplicitTargetA[0] = TARGET_DST_CASTER;
                break;
            case 69846: // Frost Bomb
                spellInfo->speed = 10;
                spellInfo->EffectImplicitTargetA[0] = TARGET_DEST_TARGET_ANY;
                spellInfo->EffectImplicitTargetB[0] = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effect[1] = 0;
                break;
            case 7922:  // charge stun
            case 20253: // intercept stun
                spellInfo->DmgClass = SPELL_DAMAGE_CLASS_MELEE;
                spellInfo->Attributes |= SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK;
                break;
            case 23126: // World Enlarger
                spellInfo->AuraInterruptFlags |= AURA_INTERRUPT_FLAG_SPELL_ATTACK;
                break;
            case 74768:
                spellInfo->Effect[0] = 0;
                spellInfo->EffectImplicitTargetA[0] = 0;
                spellInfo->EffectImplicitTargetB[0] = 0;
                break;
            case 74769: // SPELL_TWILIGHT_CUTTER
            case 77844:
            case 77845:
            case 77846:
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->EffectImplicitTargetB[0] = 0;
                break;
            case 74412: // Emergency Recall [Final]
            {
                for(int8 i = 0; i < 3; ++i)
                    spellInfo->EffectImplicitTargetB[i] = TARGET_UNIT_TARGET_ANY;
                break;
            }
            case 75545: case 75536: // Explosion (prevent error message in console)
            case 75553:             // Emergency Recall [Camera trigger]
                spellInfo->EffectImplicitTargetB[0] = TARGET_UNIT_TARGET_ANY;
                break;
            case 55368:             // Stefan's Horn
                spellInfo->EffectMiscValue[0] = 28518;
                break;
            case 56648: // Potent Fungus
                // should not be interrupted by taking damage
                spellInfo->AuraInterruptFlags &= ~AURA_INTERRUPT_FLAG_HITBYSPELL;
                spellInfo->AuraInterruptFlags &= ~AURA_INTERRUPT_FLAG_TAKE_DAMAGE;
                break;
            case 56651:
                spellInfo->EffectMiscValue[0] = 11;
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_IGNORE_LOS;
                break;
            case 72052: // Blood Prince Council - Kinetic Bomb 10N
            case 72800: // Blood Prince Council - Kinetic Bomb 25N
            case 72801: // Blood Prince Council - Kinetic Bomb 10H
            case 72802: // Blood Prince Council - Kinetic Bomb 25H
                // should ignore armor
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case 71615: // Professor Putricide's Tear Gas
            case 71617: // Professor Putricide's Tear Gas
            case 71618: // Professor Putricide's Tear Gas
                // hack to prevent boss casting spell on himself
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_EXCLUDE_SELF;
                break;
            case 70459: // Ooze Eruption Search Effect
                // This is here until targetAuraSpell and alike support SpellDifficulty.dbc
                spellInfo->targetAuraSpell = 0;
                break;
            case 70447: // Volatile Ooze Adhesive
            case 70672: // Gaseous Bloat
                spellInfo->EffectRadiusIndex[0] = 28;
                break;
            case 70701: // Expunged Gas
                // Damage should be shared
                spellInfo->EffectRadiusIndex[0] = 28;
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                break;
            case 72293: // Mark of the Fallen Champion
                // Something wrong and it applied as positive buff
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_NEGATIVE_EFF0;
                break;
            case 72255: // Mark of the Fallen Champion Trigger 10N
            case 72444: // Mark of the Fallen Champion Trigger 25N
            case 72445: // Mark of the Fallen Champion Trigger 10H
            case 72446: // Mark of the Fallen Champion Trigger 25H
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_DIRECT_DAMAGE;
                mSpellCustomAttr[i] |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                spellInfo->EffectImplicitTargetA[0] = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->EffectImplicitTargetB[0] = 0;
                break;
            case 60563: // Summon Dark Messenger
                spellInfo->EffectImplicitTargetA[0] = TARGET_DEST_CHANNEL_CASTER;
                spellInfo->Reagent[0] = 44434;
                spellInfo->ReagentCount[0] = 5;
            case 60578: // Summoning Stone Force Cast
                spellInfo->Reagent[0] = 44434;
                spellInfo->ReagentCount[0] = 5;
                break;
            case 53480: // Roar of Sacrifice
                spellInfo->Effect[1] = 6;
                spellInfo->EffectApplyAuraName[1] = 4;
                spellInfo->EffectImplicitTargetA[1] = 21;
                break;
            default:
                break;
        }

        switch(spellInfo->SpellFamilyName)
        {
            case SPELLFAMILY_WARRIOR:
                // Shout
                if(spellInfo->SpellFamilyFlags[0] & 0x20000 || spellInfo->SpellFamilyFlags[1] & 0x20)
                    mSpellCustomAttr[i] |= SPELL_ATTR0_CU_AURA_CC;
                break;
            case SPELLFAMILY_HUNTER:
                // Monstrous Bite
                if(spellInfo->SpellIconID == 599)
                    spellInfo->EffectImplicitTargetA[1] = TARGET_UNIT_CASTER;
                // Silencing Shot / Scatter Shot
                else if(spellInfo->SpellFamilyFlags[0] & 0x40000)
                    spellInfo->speed = 0;
                break;
            case SPELLFAMILY_DRUID:
                // Starfall Target Selection
                if(spellInfo->SpellFamilyFlags[2] & 0x100)
                    spellInfo->MaxAffectedTargets = 2;
                // Roar
                else if(spellInfo->SpellFamilyFlags[0] & 0x8)
                    mSpellCustomAttr[i] |= SPELL_ATTR0_CU_AURA_CC;
                // Rake
                else if(spellInfo->SpellFamilyFlags[0] & 0x1000 && spellInfo->SpellIconID == 494)
                    mSpellCustomAttr[i] |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case SPELLFAMILY_PALADIN:
                // Sanctified Retribution
                if(spellInfo->SpellFamilyFlags[2] & 0x20 && spellInfo->SpellIconID == 555) {
                    spellInfo->Effect[1] = 0;
                    spellInfo->Effect[2] = 0;
                // Seals of the Pure + Seal of Righteousness
                } else if(spellInfo->SpellIconID == 25 && spellInfo->Attributes & SPELL_ATTR0_PASSIVE)
                    spellInfo->EffectSpellClassMask[0][1] |= 0x20000000;
                break;
            case SPELLFAMILY_DEATHKNIGHT:
                // Icy Touch - extend FamilyFlags (unused value) for Sigil of the Frozen Conscience to use
                if(spellInfo->SpellIconID == 2721 && spellInfo->SpellFamilyFlags[0] & 0x2)
                    spellInfo->SpellFamilyFlags[0] |= 0x40;
                break;
            case SPELLFAMILY_PRIEST:
                switch(spellInfo->SpellIconID)
                {
                    case 2292:  // Twin Disciplines should affect at Prayer of Mending
                    case 46:    // Spiritual Healing should affect at Prayer of Mending
                        spellInfo->EffectSpellClassMask[0][1] |= 0x20;
                        break;
                    case 2845:  // Divine Providence should affect at Prayer of Mending
                        if(spellInfo->Id != 64844)
                            spellInfo->EffectSpellClassMask[0][1] |= 0x20;
                        break;
                    default:
                        break;
                }
                break;
        }
    }

    SummonPropertiesEntry* properties = const_cast<SummonPropertiesEntry*>(sSummonPropertiesStore.LookupEntry(121));
    properties->Type = SUMMON_TYPE_TOTEM;
    properties = const_cast<SummonPropertiesEntry*>(sSummonPropertiesStore.LookupEntry(647)); // 52893
    properties->Type = SUMMON_TYPE_TOTEM;

    CreatureAI::FillAISpellInfo();

    sLog->outString(">> Loaded custom spell attributes in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

// Fill custom data about enchancments
void SpellMgr::LoadEnchantCustomAttr()
{
    uint32 oldMSTime = getMSTime();

    uint32 size = sSpellItemEnchantmentStore.GetNumRows();
    mEnchantCustomAttr.resize(size);

    for(uint32 i = 0; i < size; ++i)
       mEnchantCustomAttr[i] = 0;

    for(uint32 i = 0; i < GetSpellStore()->GetNumRows(); ++i)
    {
        SpellEntry* spellInfo = (SpellEntry*)GetSpellStore()->LookupEntry(i);
        if(!spellInfo)
            continue;

        // TODO: find a better check
        if(!(spellInfo->AttributesEx2 & SPELL_ATTR2_UNK13) || !(spellInfo->Attributes & SPELL_ATTR0_NOT_SHAPESHIFT))
            continue;

        for(uint32 j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            if(spellInfo->Effect[j] == SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY)
            {
                uint32 enchId = spellInfo->EffectMiscValue[j];
                SpellItemEnchantmentEntry const* ench = sSpellItemEnchantmentStore.LookupEntry(enchId);
                if(!ench)
                    continue;
                mEnchantCustomAttr[enchId] = true;
                break;
            }
        }
    }

    sLog->outString(">> Loaded custom enchant attributes in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void SpellMgr::LoadSpellLinked()
{
    uint32 oldMSTime = getMSTime();

    mSpellLinkedMap.clear();    // need for reload case

    //                                                0              1             2
    QueryResult result = WorldDatabase.Query("SELECT spell_trigger, spell_effect, type FROM spell_linked_spell");
    if(!result)
    {
        sLog->outString(">> Loaded 0 linked spells. DB table `spell_linked_spell` is empty.");
        sLog->outString();
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        int32 trigger = fields[0].GetInt32();
        int32 effect =  fields[1].GetInt32();
        int32 type =    fields[2].GetInt32();

        SpellEntry const* spellInfo = sSpellStore.LookupEntry(abs(trigger));
        if(!spellInfo)
        {
            sLog->outErrorDb("Spell %u listed in `spell_linked_spell` does not exist", abs(trigger));
            continue;
        }
        spellInfo = sSpellStore.LookupEntry(abs(effect));
        if(!spellInfo)
        {
            sLog->outErrorDb("Spell %u listed in `spell_linked_spell` does not exist", abs(effect));
            continue;
        }

        if(trigger > 0)
        {
            switch(type)
            {
                case 0: mSpellCustomAttr[trigger] |= SPELL_ATTR0_CU_LINK_CAST; break;
                case 1: mSpellCustomAttr[trigger] |= SPELL_ATTR0_CU_LINK_HIT;  break;
                case 2: mSpellCustomAttr[trigger] |= SPELL_ATTR0_CU_LINK_AURA; break;
            }
        } else mSpellCustomAttr[-trigger] |= SPELL_ATTR0_CU_LINK_REMOVE;

        if(type) //we will find a better way when more types are needed
        {
            if(trigger > 0)
                trigger += SPELL_LINKED_MAX_SPELLS * type;
            else
                trigger -= SPELL_LINKED_MAX_SPELLS * type;
        }
        mSpellLinkedMap[trigger].push_back(effect);
    } while(result->NextRow());

    sLog->outString(">> Loaded linked spells in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

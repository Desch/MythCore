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

#include "UnitAI.h"
#include "Player.h"
#include "Creature.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "CreatureAIImpl.h"

void UnitAI::AttackStart(Unit* victim)
{
    if(victim && me->Attack(victim, true))
        me->GetMotionMaster()->MoveChase(victim);
}

void UnitAI::AttackStartCaster(Unit* victim, float dist)
{
    if(victim && me->Attack(victim, false))
        me->GetMotionMaster()->MoveChase(victim, dist);
}

void UnitAI::DoMeleeAttackIfReady()
{
    if(me->HasUnitState(UNIT_STAT_CASTING))
        return;

    //Make sure our attack is ready and we aren't currently casting before checking distance
    if(me->isAttackReady())
    {
        //If we are within range melee the target
        if(me->IsWithinMeleeRange(me->getVictim()))
        {
            me->AttackerStateUpdate(me->getVictim());
            me->resetAttackTimer();
        }
    }
    if(me->haveOffhandWeapon() && me->isAttackReady(OFF_ATTACK))
    {
        //If we are within range melee the target
        if(me->IsWithinMeleeRange(me->getVictim()))
        {
            me->AttackerStateUpdate(me->getVictim(), OFF_ATTACK);
            me->resetAttackTimer(OFF_ATTACK);
        }
    }
}

bool UnitAI::DoSpellAttackIfReady(uint32 spell)
{
    if(me->HasUnitState(UNIT_STAT_CASTING))
        return true;

    if(me->isAttackReady())
    {
        if(me->IsWithinCombatRange(me->getVictim(), GetSpellMaxRange(spell, false)))
        {
            me->CastSpell(me->getVictim(), spell, false);
            me->resetAttackTimer();
        }
        else
            return false;
    }
    return true;
}

Unit* UnitAI::SelectTarget(SelectAggroTarget targetType, uint32 position, float dist, bool playerOnly, int32 aura, SelectArc arctype, float arc )
{
    return SelectTarget(targetType, position, DefaultTargetSelector(me, dist, playerOnly, aura, arctype, arc));
}

void UnitAI::SelectTargetList(std::list<Unit*> &targetList, uint32 num, SelectAggroTarget targetType, float dist, bool playerOnly, int32 aura)
{
    SelectTargetList(targetList, DefaultTargetSelector(me, dist, playerOnly, aura), num, targetType);
}

float UnitAI::DoGetSpellMaxRange(uint32 spellId, bool positive)
{
    return GetSpellMaxRange(spellId, positive);
}

void UnitAI::DoAddAuraToAllHostilePlayers(uint32 spellid)
{
    if(me->isInCombat())
    {
        std::list<HostileReference*>& threatlist = me->getThreatManager().getThreatList();
        for(std::list<HostileReference*>::iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
        {
            if(Unit* pTemp = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                if(pTemp->GetTypeId() == TYPEID_PLAYER)
                    me->AddAura(spellid, pTemp);
        }
    } else
        return;
}

void UnitAI::DoCastToAllHostilePlayers(uint32 spellid, bool triggered)
{
    if(me->isInCombat())
    {
        std::list<HostileReference*>& threatlist = me->getThreatManager().getThreatList();
        for(std::list<HostileReference*>::iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
        {
            if(Unit* pTemp = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                if(pTemp->GetTypeId() == TYPEID_PLAYER)
                    me->CastSpell(pTemp, spellid, triggered);
        }
    } else
        return;
}

void UnitAI::DoCast(uint32 spellId)
{
    Unit* target = NULL;
    switch(AISpellInfo[spellId].target)
    {
        default:
        case AITARGET_SELF:     target = me; break;
        case AITARGET_VICTIM:   target = me->getVictim(); break;
        case AITARGET_ENEMY:
        {
            const SpellEntry * spellInfo = GetSpellStore()->LookupEntry(spellId);
            bool playerOnly = spellInfo->AttributesEx3 & SPELL_ATTR3_PLAYERS_ONLY;
            //float range = GetSpellMaxRange(spellInfo, false);
            target = SelectTarget(SELECT_TARGET_RANDOM, 0, GetSpellMaxRange(spellInfo, false), playerOnly);
            break;
        }
        case AITARGET_ALLY:     target = me; break;
        case AITARGET_BUFF:     target = me; break;
        case AITARGET_DEBUFF:
        {
            const SpellEntry * spellInfo = GetSpellStore()->LookupEntry(spellId);
            bool playerOnly = spellInfo->AttributesEx3 & SPELL_ATTR3_PLAYERS_ONLY;
            float range = GetSpellMaxRange(spellInfo, false);

            DefaultTargetSelector targetSelector(me, range, playerOnly, -(int32)spellId);
            if(!(spellInfo->Attributes & SPELL_ATTR0_BREAKABLE_BY_DAMAGE)
            && !(spellInfo->AuraInterruptFlags & AURA_INTERRUPT_FLAG_NOT_VICTIM)
            && targetSelector(me->getVictim()))
                target = me->getVictim();
            else
                target = SelectTarget(SELECT_TARGET_RANDOM, 0, targetSelector);
            break;
        }
    }

    if(target)
        me->CastSpell(target, spellId, false);
}

#define UPDATE_TARGET(a) {if(AIInfo->target<a) AIInfo->target=a;}

void UnitAI::FillAISpellInfo()
{
    AISpellInfo = new AISpellInfoType[GetSpellStore()->GetNumRows()];

    AISpellInfoType *AIInfo = AISpellInfo;
    const SpellEntry * spellInfo;

    for(uint32 i = 0; i < GetSpellStore()->GetNumRows(); ++i, ++AIInfo)
    {
        spellInfo = GetSpellStore()->LookupEntry(i);
        if(!spellInfo)
            continue;

        if(spellInfo->Attributes & SPELL_ATTR0_CASTABLE_WHILE_DEAD)
            AIInfo->condition = AICOND_DIE;
        else if(IsPassiveSpell(i) || GetSpellDuration(spellInfo) == -1)
            AIInfo->condition = AICOND_AGGRO;
        else
            AIInfo->condition = AICOND_COMBAT;

        if(AIInfo->cooldown < spellInfo->RecoveryTime)
            AIInfo->cooldown = spellInfo->RecoveryTime;

        if(!GetSpellMaxRange(spellInfo, false))
            UPDATE_TARGET(AITARGET_SELF)
        else
        {
            for(uint32 j = 0; j < MAX_SPELL_EFFECTS; ++j)
            {
                uint32 targetType = spellInfo->EffectImplicitTargetA[j];

                if(targetType == TARGET_UNIT_TARGET_ENEMY
                    || targetType == TARGET_DST_TARGET_ENEMY)
                    UPDATE_TARGET(AITARGET_VICTIM)
                else if(targetType == TARGET_UNIT_AREA_ENEMY_DST)
                    UPDATE_TARGET(AITARGET_ENEMY)

                if(spellInfo->Effect[j] == SPELL_EFFECT_APPLY_AURA)
                {
                    if(targetType == TARGET_UNIT_TARGET_ENEMY)
                        UPDATE_TARGET(AITARGET_DEBUFF)
                    else if(IsPositiveSpell(i))
                        UPDATE_TARGET(AITARGET_BUFF)
                }
            }
        }
        AIInfo->realCooldown = spellInfo->RecoveryTime + spellInfo->StartRecoveryTime;
        if(SpellRangeEntry const* pSpellRange = sSpellRangeStore.LookupEntry(spellInfo->rangeIndex))
            AIInfo->maxRange = pSpellRange->maxRangeHostile * 3 / 4;
    }
}

//Enable PlayerAI when charmed
void PlayerAI::OnCharmed(bool apply) { me->IsAIEnabled = apply; }

void SimpleCharmedAI::UpdateAI(const uint32 /*diff*/)
{
  Creature* charmer = me->GetCharmer()->ToCreature();

    //kill self if charm aura has infinite duration
    if(charmer->IsInEvadeMode())
    {
        Unit::AuraEffectList const& auras = me->GetAuraEffectsByType(SPELL_AURA_MOD_CHARM);
        for(Unit::AuraEffectList::const_iterator iter = auras.begin(); iter != auras.end(); ++iter)
            if((*iter)->GetCasterGUID() == charmer->GetGUID() && (*iter)->GetBase()->IsPermanent())
            {
                charmer->Kill(me);
                return;
            }
    }

    if(!charmer->isInCombat())
        me->GetMotionMaster()->MoveFollow(charmer, PET_FOLLOW_DIST, me->GetFollowAngle());

    Unit* target = me->getVictim();
    if(!target || !charmer->canAttack(target))
        AttackStart(charmer->SelectNearestTargetInAttackDistance());
}

bool NonTankTargetSelector::operator()(Unit const* target) const
{
    if(!target)
        return false;

    if(_playerOnly && target->GetTypeId() != TYPEID_PLAYER)
        return false;

    return target != _source->getVictim();
}

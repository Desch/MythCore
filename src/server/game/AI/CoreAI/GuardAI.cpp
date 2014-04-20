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

#include "GuardAI.h"
#include "Errors.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "World.h"
#include "CreatureAIImpl.h"

int GuardAI::Permissible(const Creature* creature)
{
    if(creature->isGuard())
        return PERMIT_BASE_SPECIAL;

    return PERMIT_BASE_NO;
}

GuardAI::GuardAI(Creature* pCreature) : ScriptedAI(pCreature), i_victimGuid(0), i_state(STATE_NORMAL), i_tracker(TIME_INTERVAL_LOOK) { }

bool GuardAI::CanSeeAlways(WorldObject const* pWO)
{
    if(!pWO || !pWO->isType(TYPEMASK_UNIT))
        return false;

    std::list<HostileReference *> t_list = me->getThreatManager().getThreatList();
    for(std::list<HostileReference *>::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
    {
        if(Unit* pUnit = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
            if(pUnit == pWO)
                return true;
    }

    return false;
}

void GuardAI::MoveInLineOfSight(Unit* unit)
{
    // Ignore Z for flying creatures
    if(!me->canFly() && me->GetDistanceZ(unit) > CREATURE_Z_ATTACK_RANGE)
        return;

    if(!me->getVictim() && me->canAttack(unit) &&
        (unit->IsHostileToPlayers() || me->IsHostileTo(unit) /*|| u->getVictim() && me->IsFriendlyTo(u->getVictim())*/) &&
        unit->isInAccessiblePlaceFor(me))
    {
        float attackRadius = me->GetAttackDistance(unit);
        if(me->IsWithinDistInMap(unit, attackRadius))
            AttackStart(unit);
    }
}

void GuardAI::EnterEvadeMode()
{
    if(!me->isAlive())
    {
        sLog->outDebug(LOG_FILTER_UNITS, "Creature stopped attacking because he is dead [guid=%u]", me->GetGUIDLow());
        me->GetMotionMaster()->MoveIdle();

        i_state = STATE_NORMAL;

        i_victimGuid = 0;
        me->CombatStop(true);
        me->DeleteThreatList();
        return;
    }

    me->RemoveAllAuras();
    me->DeleteThreatList();
    i_victimGuid = 0;
    me->CombatStop(true);
    i_state = STATE_NORMAL;

    // Remove ChaseMovementGenerator from MotionMaster stack list, and add HomeMovementGenerator instead
    if(me->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE)
        me->GetMotionMaster()->MoveTargetedHome();
}

void GuardAI::UpdateAI(const uint32 /*diff*/)
{
    // update i_victimGuid if me->getVictim() !=0 and changed
    if(!UpdateVictim())
        return;
    if(me->getVictim())
        i_victimGuid = me->getVictim()->GetGUID();
    else
    {
        i_victimGuid = 0;
        return;
    }

    if(me->isAttackReady())
    {
        if(me->IsWithinMeleeRange(me->getVictim()))
        {
            me->AttackerStateUpdate(me->getVictim());
            me->resetAttackTimer();
        }
    }
}

void GuardAI::JustDied(Unit* pKiller)
{
    if(Player* pPlayer = pKiller->GetCharmerOrOwnerPlayerOrPlayerItself())
        me->SendZoneUnderAttackMessage(pPlayer);
}

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

#ifndef TRINITY_PETAI_H
#define TRINITY_PETAI_H

#include "CreatureAI.h"
#include "Timer.h"

class Creature;
class Spell;

class PetAI : public CreatureAI
{
public:
    explicit PetAI(Creature* c);

    void EnterEvadeMode();
    void JustDied(Unit* /*pWho*/) { _stopAttack(); }

    void UpdateAI(const uint32);
    static int Permissible(const Creature* );

    void KilledUnit(Unit* /*pVictim*/);
    void AttackStart(Unit* target);
    void MovementInform(uint32 moveType, uint32 data);
    void OwnerDamagedBy(Unit* attacker);
    void OwnerAttacked(Unit* target);

private:
    bool _isVisible(Unit* ) const;
    bool _needToStop(void);
    void _stopAttack(void);

    void UpdateAllies();

    TimeTracker i_tracker;
    bool inCombat;
    std::set<uint64> m_AllySet;
    uint32 m_updateAlliesTimer;

    Unit* SelectNextTarget();
    void HandleReturnMovement();
    void DoAttack(Unit* target, bool chase);
    bool _CanAttack(Unit* target);
};

#endif


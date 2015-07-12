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

#ifndef TRINITY_GUARDAI_H
#define TRINITY_GUARDAI_H

#include "ScriptedCreature.h"
#include "Timer.h"

class Creature;

class GuardAI : public ScriptedAI
{
    enum GuardState
    {
        STATE_NORMAL = 1,
        STATE_LOOK_AT_VICTIM = 2
    };

    public:
        explicit GuardAI(Creature* c);

        void MoveInLineOfSight(Unit* );
        void EnterEvadeMode();
        void JustDied(Unit* );
        bool CanSeeAlways(WorldObject const* obj);

        void UpdateAI(const uint32);
        static int Permissible(const Creature* );

    private:
        uint64 i_victimGuid;
        GuardState i_state;
        TimeTracker i_tracker;
};

#endif


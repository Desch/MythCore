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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "blackrock_spire.h"

enum Spells
{
    SPELL_SNAPKICK                  = 15618,
    SPELL_CLEAVE                    = 15579,
    SPELL_UPPERCUT                  = 10966,
    SPELL_MORTALSTRIKE              = 16856,
    SPELL_PUMMEL                    = 15615,
    SPELL_THROWAXE                  = 16075,
};

enum Events
{
    EVENT_SNAP_KICK                 = 1,
    EVENT_CLEAVE                    = 2,
    EVENT_UPPERCUT                  = 3,
    EVENT_MORTAL_STRIKE             = 4,
    EVENT_PUMMEL                    = 5,
    EVENT_THROW_AXE                 = 6,
};

class boss_warmaster_voone : public CreatureScript
{
public:
    boss_warmaster_voone() : CreatureScript("boss_warmaster_voone") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_warmastervooneAI(pCreature);
    }

    struct boss_warmastervooneAI : public BossAI
    {
        boss_warmastervooneAI(Creature* pCreature) : BossAI(pCreature, DATA_WARMASTER_VOONE) { }

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_SNAP_KICK, 8*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_CLEAVE, 14*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_UPPERCUT, 20*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_MORTAL_STRIKE, 12*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_PUMMEL, 32*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_THROW_AXE, 1*IN_MILLISECONDS);
        }

        void JustDied(Unit* /*pWho*/)
        {
            _JustDied();
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            events.Update(diff);

            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_SNAP_KICK:
                        DoCast(me->getVictim(), SPELL_SNAPKICK);
                        events.ScheduleEvent(EVENT_SNAP_KICK, 6*IN_MILLISECONDS);
                        break;
                    case EVENT_CLEAVE:
                        DoCast(me->getVictim(), SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, 12*IN_MILLISECONDS);
                        break;
                    case EVENT_UPPERCUT:
                        DoCast(me->getVictim(), SPELL_UPPERCUT);
                        events.ScheduleEvent(EVENT_UPPERCUT, 14*IN_MILLISECONDS);
                        break;
                    case EVENT_MORTAL_STRIKE:
                        DoCast(me->getVictim(), SPELL_MORTALSTRIKE);
                        events.ScheduleEvent(EVENT_MORTAL_STRIKE, 10*IN_MILLISECONDS);
                        break;
                    case EVENT_PUMMEL:
                        DoCast(me->getVictim(), SPELL_PUMMEL);
                        events.ScheduleEvent(EVENT_MORTAL_STRIKE, 16*IN_MILLISECONDS);
                        break;
                    case EVENT_THROW_AXE:
                        DoCast(me->getVictim(), SPELL_THROWAXE);
                        events.ScheduleEvent(EVENT_THROW_AXE, 8*IN_MILLISECONDS);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_warmastervoone()
{
    new boss_warmaster_voone;
}

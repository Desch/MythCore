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
    SPELL_FLAMEBREAK                = 16785,
    SPELL_IMMOLATE                  = 20294,
    SPELL_TERRIFYINGROAR            = 14100,
};

enum Events
{
    EVENT_FLAME_BREAK              = 1,
    EVENT_IMMOLATE                 = 2,
    EVENT_TERRIFYING_ROAR          = 3,
};

class boss_the_beast : public CreatureScript
{
public:
    boss_the_beast() : CreatureScript("boss_the_beast") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_thebeastAI(pCreature);
    }

    struct boss_thebeastAI : public BossAI
    {
        boss_thebeastAI(Creature* pCreature) : BossAI(pCreature, DATA_THE_BEAST) { }

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_FLAME_BREAK, 12*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_IMMOLATE, 3*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_TERRIFYING_ROAR, 23*IN_MILLISECONDS);
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
                    case EVENT_FLAME_BREAK:
                        DoCast(me->getVictim(), SPELL_FLAMEBREAK);
                        events.ScheduleEvent(EVENT_FLAME_BREAK, 10*IN_MILLISECONDS);
                        break;
                    case EVENT_IMMOLATE:
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            DoCast(target, SPELL_IMMOLATE);
                        events.ScheduleEvent(EVENT_IMMOLATE, 8*IN_MILLISECONDS);
                        break;
                    case EVENT_TERRIFYING_ROAR:
                        DoCast(me->getVictim(), SPELL_TERRIFYINGROAR);
                        events.ScheduleEvent(EVENT_TERRIFYING_ROAR, 20*IN_MILLISECONDS);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_thebeast()
{
    new boss_the_beast;
}

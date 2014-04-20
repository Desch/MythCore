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
    SPELL_FIRENOVA                  = 23462,
    SPELL_CLEAVE                    = 20691,
    SPELL_CONFLIGURATION            = 16805,
    SPELL_THUNDERCLAP               = 15548, //Not sure if right ID. 23931 would be a harder possibility.
};

enum Events
{
    EVENT_FIRE_NOVA                = 1,
    EVENT_CLEAVE                   = 2,
    EVENT_CONFLIGURATION           = 3,
    EVENT_THUNDERCLAP              = 4,
};

class boss_drakkisath : public CreatureScript
{
public:
    boss_drakkisath() : CreatureScript("boss_drakkisath") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_drakkisathAI(pCreature);
    }

    struct boss_drakkisathAI : public BossAI
    {
        boss_drakkisathAI(Creature* pCreature) : BossAI(pCreature, DATA_GENERAL_DRAKKISATH) { }

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_FIRE_NOVA, 6*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_CLEAVE, 8*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_CONFLIGURATION, 15*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_THUNDERCLAP, 17*IN_MILLISECONDS);
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
                    case EVENT_FIRE_NOVA:
                        DoCast(me->getVictim(), SPELL_FIRENOVA);
                        events.ScheduleEvent(EVENT_FIRE_NOVA, 10*IN_MILLISECONDS);
                        break;
                    case EVENT_CLEAVE:
                        DoCast(me->getVictim(), SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, 8*IN_MILLISECONDS);
                        break;
                    case EVENT_CONFLIGURATION:
                        DoCast(me->getVictim(), SPELL_CONFLIGURATION);
                        events.ScheduleEvent(EVENT_CONFLIGURATION, 18*IN_MILLISECONDS);
                        break;
                    case EVENT_THUNDERCLAP:
                        DoCast(me->getVictim(), SPELL_THUNDERCLAP);
                        events.ScheduleEvent(EVENT_THUNDERCLAP, 20*IN_MILLISECONDS);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_drakkisath()
{
    new boss_drakkisath;
}

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
    SPELL_WARSTOMP                  = 24375,
    SPELL_CLEAVE                    = 15579,
    SPELL_STRIKE                    = 18368,
    SPELL_REND                      = 18106,
    SPELL_SUNDERARMOR               = 24317,
    SPELL_KNOCKAWAY                 = 20686,
    SPELL_SLOW                      = 22356,
};

enum Events
{
    EVENT_WARSTOMP                  = 1,
    EVENT_CLEAVE                    = 2,
    EVENT_STRIKE                    = 3,
    EVENT_REND                      = 4,
    EVENT_SUNDER_ARMOR              = 5,
    EVENT_KNOCK_AWAY                = 6,
    EVENT_SLOW                      = 7,
};

class boss_highlord_omokk : public CreatureScript
{
public:
    boss_highlord_omokk() : CreatureScript("boss_highlord_omokk") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_highlordomokkAI(pCreature);
    }

    struct boss_highlordomokkAI : public BossAI
    {
        boss_highlordomokkAI(Creature* pCreature) : BossAI(pCreature, DATA_OMOKK) { }

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_WARSTOMP, 15*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_CLEAVE, 6*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_STRIKE, 10*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_REND, 14*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_SUNDER_ARMOR, 2*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_KNOCK_AWAY, 18*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_SLOW, 24*IN_MILLISECONDS);
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
                    case EVENT_WARSTOMP:
                        DoCast(me->getVictim(), SPELL_WARSTOMP);
                        events.ScheduleEvent(EVENT_WARSTOMP, 14*IN_MILLISECONDS);
                        break;
                    case EVENT_CLEAVE:
                        DoCast(me->getVictim(), SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, 8*IN_MILLISECONDS);
                        break;
                    case EVENT_STRIKE:
                        DoCast(me->getVictim(), SPELL_STRIKE);
                        events.ScheduleEvent(EVENT_STRIKE, 10*IN_MILLISECONDS);
                        break;
                    case EVENT_REND:
                        DoCast(me->getVictim(), SPELL_REND);
                        events.ScheduleEvent(EVENT_REND, 18*IN_MILLISECONDS);
                        break;
                    case EVENT_SUNDER_ARMOR:
                        DoCast(me->getVictim(), SPELL_SUNDERARMOR);
                        events.ScheduleEvent(EVENT_SUNDER_ARMOR, 25*IN_MILLISECONDS);
                        break;
                    case EVENT_KNOCK_AWAY:
                        DoCast(me->getVictim(), SPELL_KNOCKAWAY);
                        events.ScheduleEvent(EVENT_KNOCK_AWAY, 12*IN_MILLISECONDS);
                        break;
                    case EVENT_SLOW:
                        DoCast(me->getVictim(), SPELL_SLOW);
                        events.ScheduleEvent(EVENT_SLOW, 18*IN_MILLISECONDS);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_highlordomokk()
{
    new boss_highlord_omokk;
}

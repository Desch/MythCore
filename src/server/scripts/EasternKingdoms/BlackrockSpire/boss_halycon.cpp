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
    SPELL_CROWDPUMMEL               = 10887,
    SPELL_MIGHTYBLOW                = 14099,
};

enum Events
{
    EVENT_CROWD_PUMMEL              = 1,
    EVENT_MIGHTY_BLOW               = 2,
};

const Position SummonLocation = { -169.839f, -324.961f, 64.401f, 3.124f };

class boss_halycon : public CreatureScript
{
public:
    boss_halycon() : CreatureScript("boss_halycon") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_halyconAI(pCreature);
    }

    struct boss_halyconAI : public BossAI
    {
        boss_halyconAI(Creature* pCreature) : BossAI(pCreature, DATA_HALYCON) { }

        bool Summoned;

        void Reset()
        {
            _Reset();
            Summoned = false;
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_CROWD_PUMMEL, 8*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_MIGHTY_BLOW, 14*IN_MILLISECONDS);
        }

        void JustDied(Unit* /*pWho*/)
        {
            _JustDied();
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            //Summon Gizrul
            if(!Summoned && HealthBelowPct(25))
            {
                me->SummonCreature(NPC_GIZRUL_THE_SLAVENER, SummonLocation, TEMPSUMMON_TIMED_DESPAWN, 300*IN_MILLISECONDS);
                Summoned = true;
            }

            events.Update(diff);

            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_CROWD_PUMMEL:
                        DoCast(me->getVictim(), SPELL_CROWDPUMMEL);
                        events.ScheduleEvent(EVENT_CROWD_PUMMEL, 14*IN_MILLISECONDS);
                        break;
                    case EVENT_MIGHTY_BLOW:
                        DoCast(me->getVictim(), SPELL_MIGHTYBLOW);
                        events.ScheduleEvent(EVENT_MIGHTY_BLOW, 10*IN_MILLISECONDS);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_halycon()
{
    new boss_halycon;
}

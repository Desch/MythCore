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
    SPELL_CRYSTALIZE                = 16104,
    SPELL_MOTHERSMILK               = 16468,
    SPELL_SUMMON_SPIRE_SPIDERLING   = 16103,
};

enum Events
{
    EVENT_CRYSTALIZE                = 1,
    EVENT_MOTHERS_MILK              = 2,
};

class boss_mother_smolderweb : public CreatureScript
{
public:
    boss_mother_smolderweb() : CreatureScript("boss_mother_smolderweb") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_mothersmolderwebAI(pCreature);
    }

    struct boss_mothersmolderwebAI : public BossAI
    {
        boss_mothersmolderwebAI(Creature* pCreature) : BossAI(pCreature, DATA_MOTHER_SMOLDERWEB) { }

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_CRYSTALIZE, 20*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_MOTHERS_MILK, 10*IN_MILLISECONDS);
        }

        void JustDied(Unit* /*pWho*/)
        {
            _JustDied();
        }

        void DamageTaken(Unit* /*done_by*/, uint32 &damage)
        {
            if(me->GetHealth() <= damage)
                DoCast(me, SPELL_SUMMON_SPIRE_SPIDERLING, true);
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
                    case EVENT_CRYSTALIZE:
                        DoCast(me, SPELL_CRYSTALIZE);
                        events.ScheduleEvent(EVENT_CRYSTALIZE, 15*IN_MILLISECONDS);
                        break;
                    case EVENT_MOTHERS_MILK:
                        DoCast(me, SPELL_MOTHERSMILK);
                        events.ScheduleEvent(EVENT_MOTHERS_MILK, urand(5*IN_MILLISECONDS, 12500));
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_mothersmolderweb()
{
    new boss_mother_smolderweb;
}

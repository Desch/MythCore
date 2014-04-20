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
    SPELL_CURSEOFBLOOD              = 24673,
    SPELL_HEX                       = 16708,
    SPELL_CLEAVE                    = 20691,
};

enum Events
{
    EVENT_CURSE_OF_BLOOD            = 1,
    EVENT_HEX                       = 2,
    EVENT_CLEAVE                    = 3,
};

class boss_shadow_hunter_voshgajin : public CreatureScript
{
public:
    boss_shadow_hunter_voshgajin() : CreatureScript("boss_shadow_hunter_voshgajin") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_shadowvoshAI(pCreature);
    }

    struct boss_shadowvoshAI : public BossAI
    {
        boss_shadowvoshAI(Creature* pCreature) : BossAI(pCreature, DATA_SHADOW_HUNTER_VOSHGAJIN) { }

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_CURSE_OF_BLOOD, 2*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_HEX, 8*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_CLEAVE, 14*IN_MILLISECONDS);
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
                    case EVENT_CURSE_OF_BLOOD:
                        DoCast(me->getVictim(), SPELL_CURSEOFBLOOD);
                        events.ScheduleEvent(EVENT_CURSE_OF_BLOOD, 45*IN_MILLISECONDS);
                        break;
                    case EVENT_HEX:
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            DoCast(target, SPELL_HEX);
                        events.ScheduleEvent(EVENT_HEX, 15*IN_MILLISECONDS);
                        break;
                    case EVENT_CLEAVE:
                        DoCast(me->getVictim(), SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, 7*IN_MILLISECONDS);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_shadowvosh()
{
    new boss_shadow_hunter_voshgajin;
}

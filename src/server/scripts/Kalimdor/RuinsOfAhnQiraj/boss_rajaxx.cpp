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

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ruins_of_ahnqiraj.h"

enum Yells
{
    // The time of our retribution is at hand! Let darkness reign in the hearts of our enemies! Sound: 8645 Emote: 35
    SAY_ANDOROV_INTRO         = -1509003,   // Before for the first wave
    SAY_ANDOROV_ATTACK        = -1509004,   // Beginning the event
    SAY_ANDOROV_WAVE_1        = -1509001,   // When the first wave comes text: Kill first, ask questions later... Incoming! emote: 45 sound: 8653
    SAY_WAVE3                 = -1509005,
    SAY_WAVE4                 = -1509006,
    SAY_WAVE5                 = -1509007,
    SAY_WAVE6                 = -1509008,
    SAY_WAVE7                 = -1509009,
    SAY_INTRO                 = -1509010,
    SAY_UNK1                  = -1509011,
    SAY_UNK2                  = -1509012,
    SAY_UNK3                  = -1509013,
    SAY_DEATH                 = -1509014,
    SAY_CHANGEAGGRO           = -1509015,
    SAY_KILLS_ANDOROV         = -1509016,
    SAY_COMPLETE_QUEST        = -1509017    // Yell when realm complete quest 8743 for world event
    // Warriors, Captains, continue the fight! Sound: 8640
};

enum Spells
{
    SPELL_DISARM            = 6713,
    SPELL_FRENZY            = 8269,
    SPELL_THUNDERCRASH      = 25599
};

enum Events
{
    EVENT_DISARM            = 1,        // 03:58:27, 03:58:49
    EVENT_THUNDERCRASH      = 2,        // 03:58:29, 03:58:50
    EVENT_CHANGE_AGGRO      = 3,
};

class boss_rajaxx : public CreatureScript
{
    public:
        boss_rajaxx() : CreatureScript("boss_rajaxx") { }

        struct boss_rajaxxAI : public BossAI
        {
            boss_rajaxxAI(Creature* creature) : BossAI(creature, BOSS_RAJAXX) { }

            void Reset()
            {
                _Reset();
                enraged = false;
                events.ScheduleEvent(EVENT_DISARM, 10000);
                events.ScheduleEvent(EVENT_THUNDERCRASH, 12000);
            }

            void JustDied(Unit* pKiller)
            {
                if(!pKiller)
                    return;
                _JustDied();
                if(Player* pPlayer = pKiller->ToPlayer())
                {
                    if(Creature* pCreature15471 = pPlayer->FindNearestCreature(15471, 100, true))
                        pCreature15471->SetFlag(UNIT_FIELD_FLAGS, UNIT_NPC_FLAG_VENDOR);
                }
            }

            void EnterCombat(Unit* /*pVictim*/)
            {
                _EnterCombat();
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
                        case EVENT_DISARM:
                            DoCastVictim(SPELL_DISARM);
                            events.ScheduleEvent(EVENT_DISARM, 22000);
                            break;
                        case EVENT_THUNDERCRASH:
                            DoCast(me, SPELL_THUNDERCRASH);
                            events.ScheduleEvent(EVENT_THUNDERCRASH, 21000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
            private:
                bool enraged;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_rajaxxAI(pCreature);
        }
};

void AddSC_boss_rajaxx()
{
    new boss_rajaxx;
}

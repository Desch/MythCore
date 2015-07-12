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

#include "ScriptPCH.h"
#include "vault_of_archavon.h"

#define EMOTE_BERSERK           -1590002

//Spells Archavon
#define SPELL_ROCK_SHARDS        58678
#define SPELL_CRUSHING_LEAP      RAID_MODE(58960, 60894)//Instant (10-80yr range) -- Leaps at an enemy, inflicting 8000 Physical damage, knocking all nearby enemies away, and creating a cloud of choking debris.
#define SPELL_STOMP              RAID_MODE(58663, 60880)
#define SPELL_IMPALE             RAID_MODE(58666, 60882) //Lifts an enemy off the ground with a spiked fist, inflicting 47125 to 52875 Physical damage and 9425 to 10575 additional damage each second for 8 sec.
#define SPELL_BERSERK            47008
//Spells Archavon Warders
#define SPELL_ROCK_SHOWER        RAID_MODE(60919, 60923)
#define SPELL_SHIELD_CRUSH       RAID_MODE(60897, 60899)
#define SPELL_WHIRL              RAID_MODE(60902, 60916)

//4 Warders spawned
#define ARCHAVON_WARDER          32353 //npc 32353

//Yell
#define SAY_LEAP "Archavon le gardien des pierres se précipite sur $N!" //$N should be the target

enum Events
{
    // Archavon
    EVENT_ROCK_SHARDS       = 1,    // 15s cd
    EVENT_CHOKING_CLOUD     = 2,    // 30s cd
    EVENT_STOMP             = 3,    // 45s cd
    EVENT_IMPALE            = 4,
    EVENT_BERSERK           = 5,    // 300s cd

    //mob
    EVENT_ROCK_SHOWER       = 6,    // set = 20s cd, unkown cd
    EVENT_SHIELD_CRUSH      = 7,    // set = 30s cd
    EVENT_WHIRL             = 8,    // set= 10s cd
};

class boss_archavon : public CreatureScript
{
    public:
        boss_archavon() : CreatureScript("boss_archavon") { }

        struct boss_archavonAI : public BossAI
        {
            boss_archavonAI(Creature* creature) : BossAI(creature, DATA_ARCHAVON) { }

            void EnterCombat(Unit* /*pWho*/)
            {
                events.ScheduleEvent(EVENT_ROCK_SHARDS, 15000);
                events.ScheduleEvent(EVENT_CHOKING_CLOUD, 30000);
                events.ScheduleEvent(EVENT_STOMP, 45000);
                events.ScheduleEvent(EVENT_BERSERK, 300000);

                _EnterCombat();
            }

            // Below UpdateAI may need review/debug.
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
                        case EVENT_ROCK_SHARDS:
                            if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_ROCK_SHARDS);
                            events.ScheduleEvent(EVENT_ROCK_SHARDS, 15000);
                            break;
                        case EVENT_CHOKING_CLOUD:
                            if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_CRUSHING_LEAP, true); //10y~80y, ignore range
                            events.ScheduleEvent(EVENT_CHOKING_CLOUD, 30000);
                            break;
                        case EVENT_STOMP:
                            DoCastVictim(SPELL_STOMP);
                            events.ScheduleEvent(EVENT_IMPALE, 3000);
                            events.ScheduleEvent(EVENT_STOMP, 45000);
                            break;
                        case EVENT_IMPALE:
                            DoCastVictim(SPELL_IMPALE);
                            break;
                        case EVENT_BERSERK:
                            DoCast(me, SPELL_BERSERK);
                            DoScriptText(EMOTE_BERSERK, me);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_archavonAI(pCreature);
        }
};

/*######
##  Mob Archavon Warder
######*/
class mob_archavon_warder : public CreatureScript
{
    public:
        mob_archavon_warder() : CreatureScript("mob_archavon_warder") { }

        struct mob_archavon_warderAI : public ScriptedAI //npc 32353
        {
            mob_archavon_warderAI(Creature* pCreature): ScriptedAI(pCreature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_ROCK_SHOWER, 2000);
                events.ScheduleEvent(EVENT_SHIELD_CRUSH, 20000);
                events.ScheduleEvent(EVENT_WHIRL, 7500);
            }

            void EnterCombat(Unit* /*pWho*/)
            {
                DoZoneInCombat();
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
                        case EVENT_ROCK_SHOWER:
                            if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_ROCK_SHOWER);
                            events.ScheduleEvent(EVENT_ROCK_SHARDS, 6000);
                            break;
                        case EVENT_SHIELD_CRUSH:
                            DoCastVictim(SPELL_SHIELD_CRUSH);
                            events.ScheduleEvent(EVENT_SHIELD_CRUSH, 20000);
                            break;
                        case EVENT_WHIRL:
                            DoCastVictim(SPELL_WHIRL);
                            events.ScheduleEvent(EVENT_WHIRL, 8000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new mob_archavon_warderAI(pCreature);
        }
};

void AddSC_boss_archavon()
{
    new boss_archavon;
    new mob_archavon_warder;
}

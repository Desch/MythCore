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

enum Spells
{
    // Toravon
    SPELL_FREEZING_GROUND   = 72090,    // don't know cd... using 20 secs.
    SPELL_FROZEN_ORB        = 72091,
    SPELL_WHITEOUT          = 72034,    // Every 38 sec. cast. (after SPELL_FROZEN_ORB)
    SPELL_FROZEN_MALLET     = 71993,

    // Frost Warder
    SPELL_FROST_BLAST       = 72123,    // don't know cd... using 20 secs.
    SPELL_FROZEN_MALLET_2   = 72122,

    // Frozen Orb
    SPELL_FROZEN_ORB_DMG    = 72081,    // priodic dmg aura
    SPELL_FROZEN_ORB_AURA   = 72067,    // make visible

    // Frozen Orb Stalker
    SPELL_FROZEN_ORB_SUMMON = 72093,    // summon orb
};

// Events boss
enum Events
{
    EVENT_FREEZING_GROUND   = 1,
    EVENT_FROZEN_ORB        = 2,
    EVENT_WHITEOUT          = 3,

    EVENT_FROST_BLAST       = 4,
};

// Mob Frozen Orb
#define MOB_FROZEN_ORB 38456    // 1 in 10 mode and 3 in 25 mode

class boss_toravon : public CreatureScript
{
    public:
        boss_toravon() : CreatureScript("boss_toravon") { }

        struct boss_toravonAI : public BossAI
        {
            boss_toravonAI(Creature* creature) : BossAI(creature, DATA_TORAVON) { }

            void EnterCombat(Unit* /*pWho*/)
            {
                DoCast(me, SPELL_FROZEN_MALLET);

                events.ScheduleEvent(EVENT_FROZEN_ORB, 11000);
                events.ScheduleEvent(EVENT_WHITEOUT, 13000);
                events.ScheduleEvent(EVENT_FREEZING_GROUND, 15000);

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
                        case EVENT_FROZEN_ORB:
                            me->CastCustomSpell(SPELL_FROZEN_ORB, SPELLVALUE_MAX_TARGETS, 1, me);
                            events.ScheduleEvent(EVENT_FROZEN_ORB, 38000);
                            break;
                        case EVENT_WHITEOUT:
                            DoCast(me, SPELL_WHITEOUT);
                            events.ScheduleEvent(EVENT_WHITEOUT, 38000);
                            break;
                        case EVENT_FREEZING_GROUND:
                            if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                                DoCast(target, SPELL_FREEZING_GROUND);
                            events.ScheduleEvent(EVENT_FREEZING_GROUND, 20000);
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
            return new boss_toravonAI(pCreature);
        }
};

/*######
##  Mob Frost Warder
######*/
class mob_frost_warder : public CreatureScript
{
    public:
        mob_frost_warder() : CreatureScript("mob_frost_warder") { }

        struct mob_frost_warderAI : public ScriptedAI
        {
            mob_frost_warderAI(Creature* pCreature) : ScriptedAI(pCreature) { }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*pWho*/)
            {
                DoZoneInCombat();

                DoCast(me, SPELL_FROZEN_MALLET_2);

                events.ScheduleEvent(EVENT_FROST_BLAST, 5000);
            }

            void UpdateAI(const uint32 diff)
            {
                if(!UpdateVictim())
                    return;

                events.Update(diff);

                if(me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                if(events.ExecuteEvent() == EVENT_FROST_BLAST)
                {
                    DoCastVictim(SPELL_FROST_BLAST);
                    events.ScheduleEvent(EVENT_FROST_BLAST, 20000);
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new mob_frost_warderAI(pCreature);
        }
};

/*######
##  Mob Frozen Orb
######*/
class mob_frozen_orb : public CreatureScript
{
public:
    mob_frozen_orb() : CreatureScript("mob_frozen_orb") { }

    struct mob_frozen_orbAI : public ScriptedAI
    {
        mob_frozen_orbAI(Creature* pCreature): ScriptedAI(pCreature) { }

        void Reset()
        {
            done = false;
            killTimer = 60000; // if after this time there is no victim -> destroy!
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoZoneInCombat();
        }

        void UpdateAI(const uint32 diff)
        {
            if(!done)
            {
                DoCast(me, SPELL_FROZEN_ORB_AURA, true);
                DoCast(me, SPELL_FROZEN_ORB_DMG, true);
                done = true;
            }

            if(killTimer <= diff)
            {
                if(!UpdateVictim())
                    me->DespawnOrUnsummon();
                killTimer = 10000;
            }
            else
                killTimer -= diff;
        }

    private:
        uint32 killTimer;
        bool done;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_frozen_orbAI(pCreature);
    }
};

/*######
##  Mob Frozen Orb Stalker
######*/
class mob_frozen_orb_stalker : public CreatureScript
{
    public:
        mob_frozen_orb_stalker() : CreatureScript("mob_frozen_orb_stalker") { }

        struct mob_frozen_orb_stalkerAI : public Scripted_NoMovementAI
        {
            mob_frozen_orb_stalkerAI(Creature* pCreature): Scripted_NoMovementAI(pCreature)
            {
                pCreature->SetVisible(false);
                pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_DISABLE_MOVE);
                pCreature->SetReactState(REACT_PASSIVE);

                pInstance = pCreature->GetInstanceScript();
                spawned = false;
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                if(spawned)
                    return;

                spawned = true;
                Unit* toravon = me->GetCreature(*me, pInstance ? pInstance->GetData64(DATA_TORAVON) : 0);
                if(!toravon)
                    return;

                uint8 num_orbs = RAID_MODE(1, 3);
                for(uint8 i = 0; i < num_orbs; ++i)
                {
                    Position pos;
                    me->GetNearPoint(toravon, pos.m_positionX, pos.m_positionY, pos.m_positionZ, 0.0f, 10.0f, 0.0f);
                    me->SetPosition(pos, true);
                    DoCast(me, SPELL_FROZEN_ORB_SUMMON);
                }
            }

        private:
            InstanceScript* pInstance;
            bool spawned;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new mob_frozen_orb_stalkerAI(pCreature);
        }
};

void AddSC_boss_toravon()
{
    new boss_toravon;
    new mob_frost_warder;
    new mob_frozen_orb;
    new mob_frozen_orb_stalker;
}

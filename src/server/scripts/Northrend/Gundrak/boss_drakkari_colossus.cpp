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
#include "gundrak.h"

enum Spells
{
    SPELL_EMERGE                                  = 54850,
    SPELL_ELEMENTAL_SPAWN_EFFECT                  = 54888,
    SPELL_MOJO_VOLLEY                             = 54849,
    SPELL_SURGE_VISUAL                            = 54827,
    SPELL_MERGE                                   = 54878,
    SPELL_MIGHTY_BLOW                             = 54719,
    SPELL_SURGE                                   = 54801,
    SPELL_FREEZE_ANIM                             = 16245,
    SPELL_MOJO_PUDDLE                             = 55627,
    SPELL_MOJO_WAVE                               = 55626,
};

enum ColossusEvents
{
    EVENT_MIGHTY_BLOW   = 1,
};

enum ColossusActions
{
    ACTION_SUMMON_ELEMENTAL     = 1,
    ACTION_FREEZE_COLOSSUS      = 2,
    ACTION_UNFREEZE_COLOSSUS    = 3,
};

enum ColossusPhases
{
    COLOSSUS_PHASE_NORMAL                   = 1,
    COLOSSUS_PHASE_FIRST_ELEMENTAL_SUMMON   = 2,
    COLOSSUS_PHASE_SECOND_ELEMENTAL_SUMMON  = 3
};

enum ColossusData
{
    DATA_COLOSSUS_PHASE = 1,
    DATA_INTRO_DONE     = 2
};

enum ElementalActions
{
    ACTION_RETURN_TO_COLOSSUS = 1
};

enum ElementalEvents
{
    EVENT_SURGE = 1
};

class boss_drakkari_colossus : public CreatureScript
{
    public:
        boss_drakkari_colossus() : CreatureScript("boss_drakkari_colossus") { }

        struct boss_drakkari_colossusAI : public BossAI
        {
            boss_drakkari_colossusAI(Creature* creature) : BossAI(creature, DATA_DRAKKARI_COLOSSUS_EVENT)
            {
                me->SetReactState(REACT_PASSIVE);
                introDone = false;
            }

            void InitializeAI()
            {
                if(!me->isDead())
                    Reset();
            }

            void Reset()
            {
                _Reset();

                if(GetData(DATA_INTRO_DONE))
                {
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_OOC_NOT_ATTACKABLE);
                    me->RemoveAura(SPELL_FREEZE_ANIM);
                }

                //events.Reset(); -> done in _Reset();
                events.ScheduleEvent(EVENT_MIGHTY_BLOW, urand(10000, 30000));

                phase = COLOSSUS_PHASE_NORMAL;

                // Note: This should not be called, but before use SetBossState function we should use BossAI
                //        in all the bosses of the instance
                instance->SetData(DATA_DRAKKARI_COLOSSUS_EVENT, NOT_STARTED);
            }

            void EnterCombat(Unit* /*pWho*/)
            {
                _EnterCombat();

                me->RemoveAura(SPELL_FREEZE_ANIM);

                // Note: This should not be called, but before use SetBossState function we should use BossAI
                //        in all the bosses of the instance
                instance->SetData(DATA_DRAKKARI_COLOSSUS_EVENT, IN_PROGRESS);
            }

            void JustDied(Unit* /*pKiller*/)
            {
                _JustDied();
                me->CombatStop(true);
                me->getHostileRefManager().deleteReferences();

                // Note: This should not be called, but before use SetBossState function we should use BossAI
                //        in all the bosses of the instance
                instance->SetData(DATA_DRAKKARI_COLOSSUS_EVENT, DONE);
            }

            void JustReachedHome()
            {
                // Note: This should not be called, but before use SetBossState function we should use BossAI
                //        in all the bosses of the instance
                instance->SetData(DATA_DRAKKARI_COLOSSUS_EVENT, FAIL);
            }

            void DoAction(const int32 action)
            {
                switch(action)
                {
                    case ACTION_SUMMON_ELEMENTAL:
                        DoCast(SPELL_EMERGE);
                        break;
                    case ACTION_FREEZE_COLOSSUS:
                        me->GetMotionMaster()->MoveIdle();
                        me->SetReactState(REACT_PASSIVE);
                        me->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_OOC_NOT_ATTACKABLE);
                        DoCast(me,SPELL_FREEZE_ANIM);
                        break;
                    case ACTION_UNFREEZE_COLOSSUS:
                        if(me->GetReactState() == REACT_AGGRESSIVE)
                            return;

                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_OOC_NOT_ATTACKABLE);
                        me->RemoveAura(SPELL_FREEZE_ANIM);
                        me->SetInCombatWithZone();

                        if(me->getVictim())
                            me->GetMotionMaster()->MoveChase(me->getVictim(), 0, 0);
                        break;
                }
            }

            void DamageTaken(Unit* /*pAttacker*/, uint32& damage)
            {
                if(me->HasFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_OOC_NOT_ATTACKABLE))
                    damage = 0;

                if(phase == COLOSSUS_PHASE_NORMAL ||
                    phase == COLOSSUS_PHASE_FIRST_ELEMENTAL_SUMMON)
                {
                    if(HealthBelowPct( phase == COLOSSUS_PHASE_NORMAL ? 50 : 5))
                    {
                        damage = 0;
                        phase = (phase == COLOSSUS_PHASE_NORMAL ? COLOSSUS_PHASE_FIRST_ELEMENTAL_SUMMON : COLOSSUS_PHASE_SECOND_ELEMENTAL_SUMMON);
                        DoAction(ACTION_FREEZE_COLOSSUS);
                        DoAction(ACTION_SUMMON_ELEMENTAL);

                    }
                }
            }

            uint32 GetData(uint32 data)
            {
               if(data == DATA_COLOSSUS_PHASE)
                   return phase;
               else if(data == DATA_INTRO_DONE)
                   return introDone;

               return 0;
            }

            void SetData(uint32 type, uint32 data)
            {
                if(type == DATA_INTRO_DONE)
                    introDone = data;
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
                        case EVENT_MIGHTY_BLOW:
                            DoCast(me->getVictim(),SPELL_MIGHTY_BLOW);
                            events.ScheduleEvent(EVENT_MIGHTY_BLOW, urand(5000, 15000));
                            break;
                    }
                }

                if(me->GetReactState() == REACT_AGGRESSIVE)
                    DoMeleeAttackIfReady();
            }

            void JustSummoned(Creature* summon)
            {
               summon->SetInCombatWithZone();

               if(phase == COLOSSUS_PHASE_SECOND_ELEMENTAL_SUMMON)
                   summon->SetHealth(summon->GetMaxHealth() / 2);
            }

        private:
            uint8 phase;
            bool introDone;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_drakkari_colossusAI(pCreature);
        }
};

class boss_drakkari_elemental : public CreatureScript
{
    public:
        boss_drakkari_elemental() : CreatureScript("boss_drakkari_elemental") { }

        struct boss_drakkari_elementalAI : public ScriptedAI
        {
            boss_drakkari_elementalAI(Creature* pCreature): ScriptedAI(pCreature)
            {
                DoCast(me,SPELL_ELEMENTAL_SPAWN_EFFECT);
                pInstance = pCreature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_SURGE,urand(5000, 15000));

                me->AddAura(SPELL_MOJO_VOLLEY,me);
            }

            void JustDied(Unit* killer)
            {
                if(killer == me)
                    return;

                me->RemoveAurasDueToSpell(SPELL_MOJO_VOLLEY);

                if(pInstance)
                {
                    if(Creature* colossus = Unit::GetCreature(*me, pInstance->GetData64(DATA_DRAKKARI_COLOSSUS)))
                        killer->Kill(colossus);
                }
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
                        case EVENT_SURGE:
                            DoCast(SPELL_SURGE_VISUAL);
                            if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                                DoCast(target,SPELL_SURGE);
                            events.ScheduleEvent(EVENT_SURGE,urand(5000, 15000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

           void DoAction(const int32 action)
            {
                switch(action)
                {
                    case ACTION_RETURN_TO_COLOSSUS:
                        DoCast(SPELL_SURGE_VISUAL);
                        if(pInstance)
                        {
                            if(Creature* colossus = Unit::GetCreature(*me, pInstance->GetData64(DATA_DRAKKARI_COLOSSUS)))
                                // what if the elemental is more than 80 yards from drakkari colossus ?
                                DoCast(colossus,SPELL_MERGE, true);
                        }
                        break;
                }
           }

            void DamageTaken(Unit* /*pAttacker*/, uint32& damage)
            {
                if(HealthBelowPct(50) && pInstance)
                {
                    if(Creature* colossus = Unit::GetCreature(*me, pInstance->GetData64(DATA_DRAKKARI_COLOSSUS)))
                    {
                        if(colossus->AI()->GetData(DATA_COLOSSUS_PHASE) ==  COLOSSUS_PHASE_FIRST_ELEMENTAL_SUMMON)
                        {
                            damage = 0;

                            // to prevent spell spaming
                            if(me->HasUnitState(UNIT_STAT_CHARGING))
                                return;
                            DoAction(ACTION_RETURN_TO_COLOSSUS);
                        }
                    }
                }
            }

            void EnterEvadeMode()
            {
                me->DespawnOrUnsummon();
            }

            void SpellHitTarget(Unit* target, SpellEntry const* spell)
            {
                if(spell->Id == SPELL_MERGE)
                {
                    if(Creature* colossus = target->ToCreature())
                    {
                        colossus->AI()->DoAction(ACTION_UNFREEZE_COLOSSUS);
                        me->DespawnOrUnsummon(1000);
                    }
                }
            }

        private:
            EventMap events;
            InstanceScript* pInstance;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_drakkari_elementalAI(pCreature);
        }
};

class npc_living_mojo : public CreatureScript
{
public:
    npc_living_mojo() : CreatureScript("npc_living_mojo") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_living_mojoAI(pCreature);
    }

    struct npc_living_mojoAI : public ScriptedAI
    {
        npc_living_mojoAI(Creature* pCreature): ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        void Reset()
        {
            mojoWaveTimer = 2*IN_MILLISECONDS;
            mojoPuddleTimer = 7*IN_MILLISECONDS;
        }

        void MoveMojos(Creature* boss)
        {
            std::list<Creature*> mojosList;
            boss->GetCreatureListWithEntryInGrid(mojosList,me->GetEntry(), 12.0f);
            if(!mojosList.empty())
            {
                for(std::list<Creature*>::const_iterator itr = mojosList.begin(); itr != mojosList.end(); ++itr)
                {
                    if(Creature* mojo = *itr)
                        mojo->GetMotionMaster()->MovePoint(1,boss->GetHomePosition().GetPositionX(),boss->GetHomePosition().GetPositionY(),boss->GetHomePosition().GetPositionZ());
                }
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if(type != POINT_MOTION_TYPE)
                return;

            if(id == 1)
            {
                if(Creature* colossus = Unit::GetCreature(*me, pInstance ? pInstance->GetData64(DATA_DRAKKARI_COLOSSUS) : 0))
                {
                    colossus->AI()->DoAction(ACTION_UNFREEZE_COLOSSUS);
                    if(!colossus->AI()->GetData(DATA_INTRO_DONE))
                        colossus->AI()->SetData(DATA_INTRO_DONE, true);
                    colossus->SetInCombatWithZone();
                    me->DespawnOrUnsummon();
                }
            }
        }

        void AttackStart(Unit* attacker)
        {
            if(me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                return;

            // we do this checks to see if the creature is one of the creatures that sorround the boss
            if(Creature* colossus = Unit::GetCreature(*me, pInstance ? pInstance->GetData64(DATA_DRAKKARI_COLOSSUS) : 0))
            {
                Position homePosition;
                me->GetHomePosition().GetPosition(&homePosition);

                Position colossusHomePosition;
                colossus->GetHomePosition().GetPosition(&colossusHomePosition);

                float distance = homePosition.GetExactDist(colossusHomePosition.GetPositionX(),colossusHomePosition.GetPositionY(),colossusHomePosition.GetPositionZ());

                if(distance < 12.0f)
                {
                    MoveMojos(colossus);
                    me->SetReactState(REACT_PASSIVE);
                }
                else
                    ScriptedAI::AttackStart(attacker);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if(!UpdateVictim())
                return;

            if(mojoWaveTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_MOJO_WAVE);
                mojoWaveTimer = 15*IN_MILLISECONDS;
            } else mojoWaveTimer -= diff;

            if(mojoPuddleTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_MOJO_PUDDLE);
                mojoPuddleTimer = 18*IN_MILLISECONDS;
            } else mojoPuddleTimer -= diff;

            DoMeleeAttackIfReady();
        }
    private:
        InstanceScript* pInstance;
        uint32 mojoWaveTimer;
        uint32 mojoPuddleTimer;
    };
};

void AddSC_boss_drakkari_colossus()
{
    new boss_drakkari_colossus;
    new boss_drakkari_elemental;
    new npc_living_mojo;
}

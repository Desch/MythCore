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
#include "violet_hold.h"

enum Spells
{
    SPELL_SHROUD_OF_DARKNESS                    = 54524,
    H_SPELL_SHROUD_OF_DARKNESS                  = 59745,
    SPELL_SUMMON_VOID_SENTRY                    = 54369,
    SPELL_VOID_SHIFT                            = 54361,
    H_SPELL_VOID_SHIFT                          = 59743,

    SPELL_ZURAMAT_ADD_2                         = 54342,
    H_SPELL_ZURAMAT_ADD_2                       = 59747
};

enum ZuramatCreatures
{
    CREATURE_VOID_SENTRY                        = 29364
};

enum Yells
{
    SAY_AGGRO                                   = -1608037,
    SAY_SLAY_1                                  = -1608038,
    SAY_SLAY_2                                  = -1608039,
    SAY_SLAY_3                                  = -1608040,
    SAY_DEATH                                   = -1608041,
    SAY_SPAWN                                   = -1608042,
    SAY_SHIELD                                  = -1608043,
    SAY_WHISPER                                 = -1608044
};

enum eActions
{
    ACTION_VOID_DEAD,
};

enum eAchievements
{
    ACHIEVEMENTS_THE_VOID_DANCE                 = 2153,
};

class boss_zuramat : public CreatureScript
{
public:
    boss_zuramat() : CreatureScript("boss_zuramat") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_zuramatAI(pCreature);
    }

    struct boss_zuramatAI : public ScriptedAI
    {
        boss_zuramatAI(Creature* pCreature) : ScriptedAI(pCreature), Summons(me)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        SummonList Summons;

        uint32 SpellVoidShiftTimer;
        uint32 SpellSummonVoidTimer;
        uint32 SpellShroudOfDarknessTimer;

        bool bVoidWalkerKilled;

        void Reset()
        {
            Summons.DespawnAll();
            if(pInstance)
            {
                if(pInstance->GetData(DATA_WAVE_COUNT) == 6)
                    pInstance->SetData(DATA_1ST_BOSS_EVENT, NOT_STARTED);
                else if(pInstance->GetData(DATA_WAVE_COUNT) == 12)
                    pInstance->SetData(DATA_2ND_BOSS_EVENT, NOT_STARTED);
            }

            SpellShroudOfDarknessTimer = 22000;
            SpellVoidShiftTimer = 15000;
            SpellSummonVoidTimer = 12000;

            bVoidWalkerKilled = false;
        }

        void DoAction(int32 const action)
        {
            switch(action)
            {
                case ACTION_VOID_DEAD:
                    bVoidWalkerKilled = true;
                    break;
                default:
                    break;
            }
        }

        void AttackStart(Unit* who)
        {
            if(me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE) || me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                return;

            if(me->Attack(who, true))
            {
                me->AddThreat(who, 0.0f);
                me->SetInCombatWith(who);
                who->SetInCombatWith(me);
                DoStartMovement(who);
            }
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);
            if(pInstance)
            {
                if(GameObject* pDoor = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_ZURAMAT_CELL)))
                    if(pDoor->GetGoState() == GO_STATE_READY)
                    {
                        EnterEvadeMode();
                        return;
                    }
                if(pInstance->GetData(DATA_WAVE_COUNT) == 6)
                    pInstance->SetData(DATA_1ST_BOSS_EVENT, IN_PROGRESS);
                else if(pInstance->GetData(DATA_WAVE_COUNT) == 12)
                    pInstance->SetData(DATA_2ND_BOSS_EVENT, IN_PROGRESS);
            }
        }

        void MoveInLineOfSight(Unit* /*pWho*/) { }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if(!UpdateVictim())
                return;

            if(SpellSummonVoidTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SUMMON_VOID_SENTRY, false);
                SpellSummonVoidTimer = 20000;
            } else SpellSummonVoidTimer -=diff;

            if(SpellVoidShiftTimer <= diff)
            {
                 if(Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(pUnit, SPELL_VOID_SHIFT);
                SpellVoidShiftTimer = 20000;
            } else SpellVoidShiftTimer -=diff;

            if(SpellShroudOfDarknessTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SHROUD_OF_DARKNESS);
                SpellShroudOfDarknessTimer = 20000;
            } else SpellShroudOfDarknessTimer -=diff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*pKiller*/)
        {
            DoScriptText(SAY_DEATH, me);

            if(pInstance)
            {
                if(pInstance->GetData(DATA_WAVE_COUNT) == 6)
                {
                    if(IsHeroic() && pInstance->GetData(DATA_1ST_BOSS_EVENT) == DONE)
                        me->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);

                    pInstance->SetData(DATA_1ST_BOSS_EVENT, DONE);
                    pInstance->SetData(DATA_WAVE_COUNT, 7);
                }
                else if(pInstance->GetData(DATA_WAVE_COUNT) == 12)
                {
                    if(IsHeroic() && pInstance->GetData(DATA_2ND_BOSS_EVENT) == DONE)
                        me->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);

                    pInstance->SetData(DATA_2ND_BOSS_EVENT, DONE);
                    pInstance->SetData(DATA_WAVE_COUNT, 13);
                }

                if(GetDifficulty() == DUNGEON_DIFFICULTY_HEROIC && !bVoidWalkerKilled)
                    pInstance->DoCompleteAchievement(ACHIEVEMENTS_THE_VOID_DANCE);
            }

            Summons.DespawnAll();
        }

        void KilledUnit(Unit* victim)
        {
            if(victim == me)
                return;

            DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2,SAY_SLAY_3), me);
        }

        void JustSummoned(Creature* summon)
        {
            Summons.Summon(summon);
        }
    };
};

class npc_void_sentry : public CreatureScript
{
public:
    npc_void_sentry() : CreatureScript("npc_void_sentry") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_void_sentryAI (pCreature);
    }

    struct npc_void_sentryAI : public ScriptedAI
    {
        npc_void_sentryAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
            me->setFaction(14);

            if(pInstance)
                if(Creature* Zuramat = Creature::GetCreature((*me),pInstance->GetData64(DATA_ZURAMAT)))
                    Zuramat->AI()->JustSummoned(me);
        }

        InstanceScript* pInstance;

        Unit* SelectPlayerTargetInRange(float range)
        {
            Player* target = NULL;
            Trinity::AnyPlayerInObjectRangeCheck u_check(me, range, true);
            Trinity::PlayerSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, target, u_check);
            me->VisitNearbyObject(range, searcher);
            return target;
        }

        void Reset()
        {
            if(Unit* target = SelectPlayerTargetInRange(100.0f))
                me->AI()->AttackStart(target);

            DoCastAOE(DUNGEON_MODE(SPELL_ZURAMAT_ADD_2,H_SPELL_ZURAMAT_ADD_2), true);
            me->SetPhaseMask(17, true);
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(Creature* Zuramat = Creature::GetCreature((*me),pInstance->GetData64(DATA_ZURAMAT)))
                Zuramat->AI()->DoAction(ACTION_VOID_DEAD);
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if(!UpdateVictim())
                return;
        }
    };
};

void AddSC_boss_zuramat()
{
    new boss_zuramat;
    new npc_void_sentry;
}

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
#include "magisters_terrace.h"

enum eEnums
{
    SAY_AGGRO                       = -1585007,
    SAY_ENERGY                      = -1585008,
    SAY_OVERLOAD                    = -1585009,
    SAY_KILL                        = -1585010,
    EMOTE_DISCHARGE_ENERGY          = -1585011,

    //is this text for real?
    //#define SAY_DEATH             "What...happen...ed."

    //Pure energy spell info
    SPELL_ENERGY_BOLT               = 46156,
    SPELL_ENERGY_FEEDBACK           = 44335,

    //Vexallus spell info
    SPELL_CHAIN_LIGHTNING           = 44318,
    SPELL_H_CHAIN_LIGHTNING         = 46380,                //heroic spell
    SPELL_OVERLOAD                  = 44353,
    SPELL_ARCANE_SHOCK              = 44319,
    SPELL_H_ARCANE_SHOCK            = 46381,                //heroic spell

    SPELL_SUMMON_PURE_ENERGY        = 44322,                //mod scale -10
    H_SPELL_SUMMON_PURE_ENERGY1     = 46154,                //mod scale -5
    H_SPELL_SUMMON_PURE_ENERGY2     = 46159,                //mod scale -5

    //Creatures
    NPC_PURE_ENERGY                 = 24745,

    INTERVAL_MODIFIER               = 15,
    INTERVAL_SWITCH                 = 6
};

class boss_vexallus : public CreatureScript
{
public:
    boss_vexallus() : CreatureScript("boss_vexallus") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_vexallusAI(pCreature);
    };

    struct boss_vexallusAI : public ScriptedAI
    {
        boss_vexallusAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;

        uint32 ChainLightningTimer;
        uint32 ArcaneShockTimer;
        uint32 OverloadTimer;
        uint32 IntervalHealthAmount;
        bool Enraged;

        void Reset()
        {
            ChainLightningTimer = 8000;
            ArcaneShockTimer = 5000;
            OverloadTimer = 1200;
            IntervalHealthAmount = 1;
            Enraged = false;

            if(pInstance)
                pInstance->SetData(DATA_VEXALLUS_EVENT, NOT_STARTED);
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(SAY_KILL, me);
        }

        void JustDied(Unit* /*pVictim*/)
        {
            if(pInstance)
                pInstance->SetData(DATA_VEXALLUS_EVENT, DONE);
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);

            if(pInstance)
                pInstance->SetData(DATA_VEXALLUS_EVENT, IN_PROGRESS);
        }

        void JustSummoned(Creature* summoned)
        {
            if(Unit* temp = SelectTarget(SELECT_TARGET_RANDOM, 0))
                summoned->GetMotionMaster()->MoveFollow(temp, 0, 0);

            //spells are SUMMON_TYPE_GUARDIAN, so using setOwner should be ok
            summoned->CastSpell(summoned, SPELL_ENERGY_BOLT, false, 0, 0, me->GetGUID());
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(!Enraged)
            {
                //used for check, when Vexallus cast adds 85%, 70%, 55%, 40%, 25%
                if(!HealthAbovePct(100 - INTERVAL_MODIFIER * IntervalHealthAmount))
                {
                    //increase amount, unless we're at 10%, then we switch and return
                    if(IntervalHealthAmount == INTERVAL_SWITCH)
                    {
                        Enraged = true;
                        return;
                    }
                    else
                        ++IntervalHealthAmount;

                    DoScriptText(SAY_ENERGY, me);
                    DoScriptText(EMOTE_DISCHARGE_ENERGY, me);

                    if(IsHeroic())
                    {
                        DoCast(me, H_SPELL_SUMMON_PURE_ENERGY1, false);
                        DoCast(me, H_SPELL_SUMMON_PURE_ENERGY2, false);
                    }
                    else
                        DoCast(me, SPELL_SUMMON_PURE_ENERGY, false);

                    //below are workaround summons, remove when summoning spells w/implicitTarget 73 implemented in the core
                    me->SummonCreature(NPC_PURE_ENERGY, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);

                    if(IsHeroic())
                        me->SummonCreature(NPC_PURE_ENERGY, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
                }

                if(ChainLightningTimer <= diff)
                {
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_CHAIN_LIGHTNING);

                    ChainLightningTimer = 8000;
                } else ChainLightningTimer -= diff;

                if(ArcaneShockTimer <= diff)
                {
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    if(target)
                        DoCast(target, SPELL_ARCANE_SHOCK);

                    ArcaneShockTimer = 8000;
                } else ArcaneShockTimer -= diff;
            }
            else
            {
                if(OverloadTimer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_OVERLOAD);

                    OverloadTimer = 2000;
                } else OverloadTimer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };
};

class mob_pure_energy : public CreatureScript
{
public:
    mob_pure_energy() : CreatureScript("mob_pure_energy") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_pure_energyAI(pCreature);
    };

    struct mob_pure_energyAI : public ScriptedAI
    {
        mob_pure_energyAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        void Reset()
        {
            me->DespawnOrUnsummon( );
        }

        void JustDied(Unit* slayer)
        {
            if(Unit* temp = me->GetOwner())
            {
                if(temp && temp->isAlive())
                    slayer->CastSpell(slayer, SPELL_ENERGY_FEEDBACK, true, 0, 0, temp->GetGUID());
            }
        }

        void EnterCombat(Unit* /*pWho*/) { }
        void MoveInLineOfSight(Unit* /*pWho*/) { }
        void AttackStart(Unit* /*pWho*/) { }
    };
};

void AddSC_boss_vexallus()
{
    new boss_vexallus;
    new mob_pure_energy;
}

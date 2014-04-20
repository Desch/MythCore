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
#include "culling_of_stratholme.h"

enum corruptorData
{
    SPELL_VOID_STIKE        = 60590,
    SPELL_CORRUPTING_BLIGHT = 60588,
    SPELL_CHANNEL_VISUAL    = 31387,

    SAY_CORRUPTOR_AGGRO     = -1595045,
    SAY_CORRUPTOR_DEAD      = -1595046,
    SAY_CORRUPTOR_DESPAWN   = -1595047,

    NPC_TIME_RIFT           = 28409,
};

class boss_infinite_corruptor : public CreatureScript
{
public:
    boss_infinite_corruptor() : CreatureScript("boss_infinite_corruptor") { }

    struct boss_infinite_corruptorAI : public ScriptedAI
    {
        boss_infinite_corruptorAI(Creature* pCreature): ScriptedAI(pCreature)
        {
            pInstance = me->GetInstanceScript();
            timeRift = NULL;
            Reset();
        }

        uint32 uiStrikeTimer;
        uint32 uiBlightTimer;
        Creature* timeRift;

        InstanceScript* pInstance;

        void JustReachedHome()
        {
            // Data for visual
            if(timeRift)
                timeRift->DisappearAndDie();
            timeRift = me->SummonCreature(NPC_TIME_RIFT, me->GetPositionX() - 10.0f, me->GetPositionY(), me->GetPositionZ());

            // Visual Channeling
            if(timeRift)
                me->CastSpell(timeRift, SPELL_CHANNEL_VISUAL, true);
        }

        void Reset()
        {
            JustReachedHome();

            // Starting Timers
            uiStrikeTimer = urand(1000, 3000);
            uiBlightTimer = urand(5000, 8000);
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            me->Yell(SAY_CORRUPTOR_AGGRO, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*pKiller*/)
        {
            me->Yell(SAY_CORRUPTOR_DEAD, LANG_UNIVERSAL, 0);
            if(pInstance)
                pInstance->SetData(DATA_INFINITE_EVENT, DONE);
        }

        void DoAction(const int32 id)
        {
            if(id == 0) // Called from InstanceScript
            {
                me->YellToZone(SAY_CORRUPTOR_DESPAWN, LANG_UNIVERSAL, 0);
                me->DespawnOrUnsummon(1000);
            }
        }
        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(uiStrikeTimer < diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                   DoCast(target, SPELL_VOID_STIKE);
                uiStrikeTimer = urand(3000, 8000);
            }
            else uiStrikeTimer -= diff;

            if(uiBlightTimer < diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                   DoCast(target, SPELL_CORRUPTING_BLIGHT);
                uiBlightTimer = urand(6000, 9000);
            }
            else uiBlightTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_infinite_corruptorAI(pCreature);
    }
};

void AddSC_boss_infinite_corruptor()
{
    new boss_infinite_corruptor;
}

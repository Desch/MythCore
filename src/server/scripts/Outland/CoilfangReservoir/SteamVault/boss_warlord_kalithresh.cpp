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
#include "steam_vault.h"

#define SAY_INTRO                   -1545016
#define SAY_REGEN                   -1545017
#define SAY_AGGRO1                  -1545018
#define SAY_AGGRO2                  -1545019
#define SAY_AGGRO3                  -1545020
#define SAY_SLAY1                   -1545021
#define SAY_SLAY2                   -1545022
#define SAY_DEATH                   -1545023

#define SPELL_SPELL_REFLECTION      31534
#define SPELL_IMPALE                39061
#define SPELL_WARLORDS_RAGE         37081
#define SPELL_WARLORDS_RAGE_NAGA    31543

#define SPELL_WARLORDS_RAGE_PROC    36453

class mob_naga_distiller : public CreatureScript
{
public:
    mob_naga_distiller() : CreatureScript("mob_naga_distiller") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_naga_distillerAI(pCreature);
    }

    struct mob_naga_distillerAI : public ScriptedAI
    {
        mob_naga_distillerAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            //hack, due to really weird spell behaviour :(
            if(pInstance)
            {
                if(pInstance->GetData(TYPE_DISTILLER) == IN_PROGRESS)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
            }
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void StartRageGen(Unit* /*pCaster*/)
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            DoCast(me, SPELL_WARLORDS_RAGE_NAGA, true);

            if(pInstance)
                pInstance->SetData(TYPE_DISTILLER, IN_PROGRESS);
        }

        void DamageTaken(Unit* /*done_by*/, uint32 &damage)
        {
            if(me->GetHealth() <= damage)
                if(pInstance)
                    pInstance->SetData(TYPE_DISTILLER, DONE);
        }
    };
};

class boss_warlord_kalithresh : public CreatureScript
{
public:
    boss_warlord_kalithresh() : CreatureScript("boss_warlord_kalithresh") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_warlord_kalithreshAI(pCreature);
    }

    struct boss_warlord_kalithreshAI : public ScriptedAI
    {
        boss_warlord_kalithreshAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;

        uint32 Reflection_Timer;
        uint32 Impale_Timer;
        uint32 Rage_Timer;
        bool CanRage;

        void Reset()
        {
            Reflection_Timer = 10000;
            Impale_Timer = 7000+rand()%7000;
            Rage_Timer = 45000;
            CanRage = false;

            if(pInstance)
                pInstance->SetData(TYPE_WARLORD_KALITHRESH, NOT_STARTED);
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(RAND(SAY_AGGRO1, SAY_AGGRO2, SAY_AGGRO3), me);

            if(pInstance)
                pInstance->SetData(TYPE_WARLORD_KALITHRESH, IN_PROGRESS);
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(RAND(SAY_SLAY1, SAY_SLAY2), me);
        }

        void SpellHit(Unit* /*pCaster*/, const SpellEntry* spell)
        {
            //hack :(
            if(spell->Id == SPELL_WARLORDS_RAGE_PROC)
                if(pInstance)
                    if(pInstance->GetData(TYPE_DISTILLER) == DONE)
                        me->RemoveAurasDueToSpell(SPELL_WARLORDS_RAGE_PROC);
        }

        void JustDied(Unit* /*pKiller*/)
        {
            DoScriptText(SAY_DEATH, me);

            if(pInstance)
                pInstance->SetData(TYPE_WARLORD_KALITHRESH, DONE);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(Rage_Timer <= diff)
            {
                if(Creature* distiller = me->FindNearestCreature(17954, 100.0f))
                {
                    DoScriptText(SAY_REGEN, me);
                    DoCast(me, SPELL_WARLORDS_RAGE);
                    CAST_AI(mob_naga_distiller::mob_naga_distillerAI, distiller->AI())->StartRageGen(me);
                }
                Rage_Timer = 3000+rand()%15000;
            } else Rage_Timer -= diff;

            //Reflection_Timer
            if(Reflection_Timer <= diff)
            {
                DoCast(me, SPELL_SPELL_REFLECTION);
                Reflection_Timer = 15000+rand()%10000;
            } else Reflection_Timer -= diff;

            //Impale_Timer
            if(Impale_Timer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_IMPALE);

                Impale_Timer = 7500+rand()%5000;
            } else Impale_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_warlord_kalithresh()
{
    new mob_naga_distiller;
    new boss_warlord_kalithresh;
}

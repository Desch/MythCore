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

#define SAY_AGGRO                   -1070000

#define SPELL_ARCINGSMASH           8374
#define SPELL_KNOCKAWAY             10101
#define SPELL_WSTOMP                11876

class boss_ironaya : public CreatureScript
{
public:
    boss_ironaya() : CreatureScript("boss_ironaya") { }

    struct boss_ironayaAI : public ScriptedAI
    {
        boss_ironayaAI(Creature* pCreature): ScriptedAI(pCreature) { }

        uint32 uiArcingTimer;
        bool bHasCastedWstomp;
        bool bHasCastedKnockaway;

        void Reset()
        {
            uiArcingTimer = 3000;
            bHasCastedKnockaway = false;
            bHasCastedWstomp = false;
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if(!UpdateVictim())
                return;

            //If we are <50% hp do knockaway ONCE
            if(!bHasCastedKnockaway && HealthBelowPct(50))
            {
                DoCast(me->getVictim(), SPELL_KNOCKAWAY, true);

                // current aggro target is knocked away pick new target
                Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0);

                if(!target || target == me->getVictim())
                    target = SelectTarget(SELECT_TARGET_TOPAGGRO, 1);

                if(target)
                    me->TauntApply(target);

                //Shouldn't cast this agian
                bHasCastedKnockaway = true;
            }

            if(uiArcingTimer <= diff)
            {
                DoCast(me, SPELL_ARCINGSMASH);
                uiArcingTimer = 13000;
            } else uiArcingTimer -= diff;

            if(!bHasCastedWstomp && HealthBelowPct(25))
            {
                DoCast(me, SPELL_WSTOMP);
                bHasCastedWstomp = true;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_ironayaAI(pCreature);
    }
};

void AddSC_boss_ironaya()
{
    new boss_ironaya;
}
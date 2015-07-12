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

//Razorgore Phase 2 Script

#define SAY_EGGS_BROKEN1        -1469022
#define SAY_EGGS_BROKEN2        -1469023
#define SAY_EGGS_BROKEN3        -1469024
#define SAY_DEATH               -1469025

#define SPELL_CLEAVE            22540
#define SPELL_WARSTOMP          24375
#define SPELL_FIREBALLVOLLEY    22425
#define SPELL_CONFLAGRATION     23023

class boss_razorgore : public CreatureScript
{
public:
    boss_razorgore() : CreatureScript("boss_razorgore") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_razorgoreAI(pCreature);
    }

    struct boss_razorgoreAI : public ScriptedAI
    {
        boss_razorgoreAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 Cleave_Timer;
        uint32 WarStomp_Timer;
        uint32 FireballVolley_Timer;
        uint32 Conflagration_Timer;

        void Reset()
        {
            Cleave_Timer = 15000;                               //These times are probably wrong
            WarStomp_Timer = 35000;
            FireballVolley_Timer = 7000;
            Conflagration_Timer = 12000;
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoZoneInCombat();
        }

        void JustDied(Unit* /*pKiller*/)
        {
            DoScriptText(SAY_DEATH, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(Cleave_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CLEAVE);
                Cleave_Timer = urand(7000, 10000);
            } else Cleave_Timer -= diff;

            if(WarStomp_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_WARSTOMP);
                WarStomp_Timer = urand(15000, 25000);
            } else WarStomp_Timer -= diff;

            if(FireballVolley_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FIREBALLVOLLEY);
                FireballVolley_Timer = urand(12000, 15000);
            } else FireballVolley_Timer -= diff;

            if(Conflagration_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CONFLAGRATION);
                //We will remove this threat reduction and add an aura check.
                Conflagration_Timer = 12000;
            } else Conflagration_Timer -= diff;

            // Aura Check. If the gamer is affected by confliguration we attack a random gamer.
            if(me->getVictim() && me->getVictim()->HasAura(SPELL_CONFLAGRATION))
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100, true))
                    me->TauntApply(target);

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_razorgore()
{
    new boss_razorgore;
}

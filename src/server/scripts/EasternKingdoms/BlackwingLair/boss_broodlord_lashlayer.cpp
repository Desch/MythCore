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

#define SAY_AGGRO               -1469000
#define SAY_LEASH               -1469001

#define SPELL_CLEAVE            26350
#define SPELL_BLASTWAVE         23331
#define SPELL_MORTALSTRIKE      24573
#define SPELL_KNOCKBACK         25778

class boss_broodlord : public CreatureScript
{
public:
    boss_broodlord() : CreatureScript("boss_broodlord") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_broodlordAI(pCreature);
    }

    struct boss_broodlordAI : public ScriptedAI
    {
        boss_broodlordAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 Cleave_Timer;
        uint32 BlastWave_Timer;
        uint32 MortalStrike_Timer;
        uint32 KnockBack_Timer;

        void Reset()
        {
            Cleave_Timer        = 8000;     //These times are probably wrong
            BlastWave_Timer     = 12000;
            MortalStrike_Timer  = 20000;
            KnockBack_Timer     = 30000;
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);
            DoZoneInCombat();
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(Cleave_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CLEAVE);
                Cleave_Timer = 7000;
            } else Cleave_Timer -= diff;

            if(BlastWave_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_BLASTWAVE);
                BlastWave_Timer = urand(8000, 16000);
            } else BlastWave_Timer -= diff;

            if(MortalStrike_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_MORTALSTRIKE);
                MortalStrike_Timer = urand(25000, 35000);
            } else MortalStrike_Timer -= diff;

            if(KnockBack_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_KNOCKBACK);
                //Drop 50% aggro
                if(DoGetThreat(me->getVictim()))
                    DoModifyThreatPercent(me->getVictim(), -50);

                KnockBack_Timer = urand(15000, 30000);
            } else KnockBack_Timer -= diff;

            if(EnterEvadeIfOutOfCombatArea(diff))
                DoScriptText(SAY_LEASH, me);

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_broodlord()
{
    new boss_broodlord;
}
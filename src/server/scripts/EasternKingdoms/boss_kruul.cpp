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

#define SPELL_SHADOWVOLLEY          21341
#define SPELL_CLEAVE                20677
#define SPELL_THUNDERCLAP           23931
#define SPELL_TWISTEDREFLECTION     21063
#define SPELL_VOIDBOLT              21066
#define SPELL_RAGE                  21340
#define SPELL_CAPTURESOUL           21054

class boss_kruul : public CreatureScript
{
public:
    boss_kruul() : CreatureScript("boss_kruul") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_kruulAI(pCreature);
    }

    struct boss_kruulAI : public ScriptedAI
    {
        boss_kruulAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 ShadowVolley_Timer;
        uint32 Cleave_Timer;
        uint32 ThunderClap_Timer;
        uint32 TwistedReflection_Timer;
        uint32 VoidBolt_Timer;
        uint32 Rage_Timer;
        uint32 Hound_Timer;

        void Reset()
        {
            ShadowVolley_Timer = 10000;
            Cleave_Timer = 14000;
            ThunderClap_Timer = 20000;
            TwistedReflection_Timer = 25000;
            VoidBolt_Timer = 30000;
            Rage_Timer = 60000;                                 //Cast rage after 1 minute
            Hound_Timer = 8000;
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void KilledUnit(Unit* /*pVictim*/)
        {
            // When a player, pet or totem gets killed, Lord Kazzak casts this spell to instantly regenerate 70, 000 health.
            DoCast(me, SPELL_CAPTURESOUL);
        }

        void SummonHounds(Unit* victim)
        {
            if(Creature* Hound = DoSpawnCreature(19207, float(irand(-9, 9)), float(irand(-9, 9)), 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 300000))
                Hound->AI()->AttackStart(victim);
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if(!UpdateVictim())
                return;

            if(ShadowVolley_Timer <= diff)
            {
                if(urand(0, 99) < 45)
                    DoCast(me->getVictim(), SPELL_SHADOWVOLLEY);

                ShadowVolley_Timer = 5000;
            } else ShadowVolley_Timer -= diff;

            if(Cleave_Timer <= diff)
            {
                if(urand(0, 1))
                    DoCast(me->getVictim(), SPELL_CLEAVE);

                Cleave_Timer = 10000;
            } else Cleave_Timer -= diff;

            if(ThunderClap_Timer <= diff)
            {
                if(urand(0, 9) < 2)
                    DoCast(me->getVictim(), SPELL_THUNDERCLAP);

                ThunderClap_Timer = 12000;
            } else ThunderClap_Timer -= diff;

            if(TwistedReflection_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_TWISTEDREFLECTION);
                TwistedReflection_Timer = 30000;
            } else TwistedReflection_Timer -= diff;

            if(VoidBolt_Timer <= diff)
            {
                if(urand(0, 9) < 4)
                    DoCast(me->getVictim(), SPELL_VOIDBOLT);

                VoidBolt_Timer = 18000;
            } else VoidBolt_Timer -= diff;

            if(Rage_Timer <= diff)
            {
                DoCast(me, SPELL_RAGE);
                Rage_Timer = 70000;
            } else Rage_Timer -= diff;

            if(Hound_Timer <= diff)
            {
                SummonHounds(me->getVictim());
                SummonHounds(me->getVictim());
                SummonHounds(me->getVictim());

                Hound_Timer = 45000;
            } else Hound_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_kruul()
{
    new boss_kruul;
}
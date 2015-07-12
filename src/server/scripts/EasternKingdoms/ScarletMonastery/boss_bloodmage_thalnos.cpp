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

enum eEnums
{
    SAY_AGGRO               = -1189016,
    SAY_HEALTH              = -1189017,
    SAY_KILL                = -1189018,

    SPELL_FLAMESHOCK        = 8053,
    SPELL_SHADOWBOLT        = 1106,
    SPELL_FLAMESPIKE        = 8814,
    SPELL_FIRENOVA          = 16079,
};

class boss_bloodmage_thalnos : public CreatureScript
{
public:
    boss_bloodmage_thalnos() : CreatureScript("boss_bloodmage_thalnos") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_bloodmage_thalnosAI(pCreature);
    }

    struct boss_bloodmage_thalnosAI : public ScriptedAI
    {
        boss_bloodmage_thalnosAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        bool HpYell;
        uint32 FlameShock_Timer;
        uint32 ShadowBolt_Timer;
        uint32 FlameSpike_Timer;
        uint32 FireNova_Timer;

        void Reset()
        {
            HpYell = false;
            FlameShock_Timer = 10000;
            ShadowBolt_Timer = 2000;
            FlameSpike_Timer = 8000;
            FireNova_Timer = 40000;
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(SAY_KILL, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            //If we are <35% hp
            if(!HpYell && !HealthAbovePct(35))
            {
                DoScriptText(SAY_HEALTH, me);
                HpYell = true;
            }

            //FlameShock_Timer
            if(FlameShock_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FLAMESHOCK);
                FlameShock_Timer = 10000 + rand()%5000;
            } else FlameShock_Timer -= diff;

            //FlameSpike_Timer
            if(FlameSpike_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FLAMESPIKE);
                FlameSpike_Timer = 30000;
            } else FlameSpike_Timer -= diff;

            //FireNova_Timer
            if(FireNova_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FIRENOVA);
                FireNova_Timer = 40000;
            } else FireNova_Timer -= diff;

            //ShadowBolt_Timer
            if(ShadowBolt_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SHADOWBOLT);
                ShadowBolt_Timer = 2000;
            } else ShadowBolt_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_bloodmage_thalnos()
{
    new boss_bloodmage_thalnos;
}

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
#include "zulgurub.h"

#define SPELL_LIGHTNINGCLOUD         25033
#define SPELL_LIGHTNINGWAVE          24819

class boss_wushoolay : public CreatureScript
{
    public:
        boss_wushoolay() : CreatureScript("boss_wushoolay") { }

        struct boss_wushoolayAI : public ScriptedAI
        {
            boss_wushoolayAI(Creature* pCreature) : ScriptedAI(pCreature) { }

            uint32 LightningCloud_Timer;
            uint32 LightningWave_Timer;

            void Reset()
            {
                LightningCloud_Timer = 5000 + rand()%5000;
                LightningWave_Timer = 8000 + rand()%8000;
            }

            void EnterCombat(Unit* /*pWho*/) { }

            void UpdateAI(const uint32 diff)
            {
                if(!UpdateVictim())
                    return;

                //LightningCloud_Timer
                if(LightningCloud_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_LIGHTNINGCLOUD);
                    LightningCloud_Timer = 15000 + rand()%5000;
                } else LightningCloud_Timer -= diff;

                //LightningWave_Timer
                if(LightningWave_Timer <= diff)
                {
                    Unit* target = NULL;
                    target = SelectTarget(SELECT_TARGET_RANDOM, 0);
                    if(target) DoCast(target, SPELL_LIGHTNINGWAVE);

                    LightningWave_Timer = 12000 + rand()%4000;
                } else LightningWave_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_wushoolayAI(pCreature);
        }
};

void AddSC_boss_wushoolay()
{
    new boss_wushoolay;
}


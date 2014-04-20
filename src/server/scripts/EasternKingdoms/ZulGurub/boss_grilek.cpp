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

#define SPELL_AVARTAR                24646                  //The Enrage Spell
#define SPELL_GROUNDTREMOR            6524

class boss_grilek : public CreatureScript
{
    public:
        boss_grilek() : CreatureScript("boss_grilek") { }

        struct boss_grilekAI : public ScriptedAI
        {
            boss_grilekAI(Creature* pCreature) : ScriptedAI(pCreature) { }

            uint32 Avartar_Timer;
            uint32 GroundTremor_Timer;

            void Reset()
            {
                Avartar_Timer = 15000 + rand()%10000;
                GroundTremor_Timer = 8000 + rand()%8000;
            }

            void EnterCombat(Unit* /*pWho*/) { }

            void UpdateAI(const uint32 diff)
            {
                //Return since we have no target
                if(!UpdateVictim())
                    return;

                //Avartar_Timer
                if(Avartar_Timer <= diff)
                {
                    DoCast(me, SPELL_AVARTAR);
                    Unit* target = NULL;

                    target = SelectTarget(SELECT_TARGET_RANDOM, 1);

                    if(DoGetThreat(me->getVictim()))
                        DoModifyThreatPercent(me->getVictim(), -50);
                    if(target)
                        AttackStart(target);

                    Avartar_Timer = 25000 + rand()%10000;
                } else Avartar_Timer -= diff;

                //GroundTremor_Timer
                if(GroundTremor_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_GROUNDTREMOR);
                    GroundTremor_Timer = 12000 + rand()%4000;
                } else GroundTremor_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_grilekAI(pCreature);
        }
};

void AddSC_boss_grilek()
{
    new boss_grilek;
}


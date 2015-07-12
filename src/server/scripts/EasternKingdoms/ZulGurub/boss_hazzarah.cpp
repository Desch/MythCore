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

#define SPELL_MANABURN         26046
#define SPELL_SLEEP            24664

class boss_hazzarah : public CreatureScript
{
    public:
        boss_hazzarah() : CreatureScript("boss_hazzarah") { }

        struct boss_hazzarahAI : public ScriptedAI
        {
            boss_hazzarahAI(Creature* pCreature) : ScriptedAI(pCreature) { }

            uint32 ManaBurn_Timer;
            uint32 Sleep_Timer;
            uint32 Illusions_Timer;

            void Reset()
            {
                ManaBurn_Timer = 4000 + rand()%6000;
                Sleep_Timer = 10000 + rand()%8000;
                Illusions_Timer = 10000 + rand()%8000;
            }

            void EnterCombat(Unit* /*pWho*/) { }

            void UpdateAI(const uint32 diff)
            {
                if(!UpdateVictim())
                    return;

                //ManaBurn_Timer
                if(ManaBurn_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_MANABURN);
                    ManaBurn_Timer = 8000 + rand()%8000;
                } else ManaBurn_Timer -= diff;

                //Sleep_Timer
                if(Sleep_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_SLEEP);
                    Sleep_Timer = 12000 + rand()%8000;
                } else Sleep_Timer -= diff;

                //Illusions_Timer
                if(Illusions_Timer <= diff)
                {
                    //We will summon 3 illusions that will spawn on a random gamer and attack this gamer
                    //We will just use one model for the beginning
                    Unit* target = NULL;
                    for(uint8 i = 0; i < 3; ++i)
                    {
                        target = SelectTarget(SELECT_TARGET_RANDOM, 0);
                        if(!target)
                            return;

                        Creature* Illusion = me->SummonCreature(15163, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                        if(Illusion)
                            Illusion->AI()->AttackStart(target);
                    }

                    Illusions_Timer = 15000 + rand()%10000;
                } else Illusions_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_hazzarahAI(pCreature);
        }
};

void AddSC_boss_hazzarah()
{
    new boss_hazzarah;
}


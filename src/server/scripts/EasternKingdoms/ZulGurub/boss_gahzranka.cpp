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

#define SPELL_FROSTBREATH            16099
#define SPELL_MASSIVEGEYSER          22421                  //Not working. Cause its a summon...
#define SPELL_SLAM                   24326

class boss_gahzranka : public CreatureScript
{
    public:
        boss_gahzranka() : CreatureScript("boss_gahzranka") { }

        struct boss_gahzrankaAI : public ScriptedAI
        {
            boss_gahzrankaAI(Creature* pCreature) : ScriptedAI(pCreature) { }
            uint32 Frostbreath_Timer;
            uint32 MassiveGeyser_Timer;
            uint32 Slam_Timer;

            void Reset()
            {
                Frostbreath_Timer = 8000;
                MassiveGeyser_Timer = 25000;
                Slam_Timer = 17000;
            }

            void EnterCombat(Unit* /*pWho*/) { }

            void UpdateAI(const uint32 diff)
            {
                //Return since we have no target
                if(!UpdateVictim())
                    return;

                //Frostbreath_Timer
                if(Frostbreath_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_FROSTBREATH);
                    Frostbreath_Timer = 7000 + rand()%4000;
                } else Frostbreath_Timer -= diff;

                //MassiveGeyser_Timer
                if(MassiveGeyser_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_MASSIVEGEYSER);
                    DoResetThreat();

                    MassiveGeyser_Timer = 22000 + rand()%10000;
                } else MassiveGeyser_Timer -= diff;

                //Slam_Timer
                if(Slam_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_SLAM);
                    Slam_Timer = 12000 + rand()%8000;
                } else Slam_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_gahzrankaAI(pCreature);
        }
};

void AddSC_boss_gahzranka()
{
    new boss_gahzranka;
}


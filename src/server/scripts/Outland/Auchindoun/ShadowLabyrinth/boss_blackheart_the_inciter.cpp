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
#include "shadow_labyrinth.h"

#define SPELL_INCITE_CHAOS    33676
#define SPELL_INCITE_CHAOS_B  33684                         //debuff applied to each member of party
#define SPELL_CHARGE          33709
#define SPELL_WAR_STOMP       33707

#define SAY_INTRO1          -1555008 //not used
#define SAY_INTRO2          -1555009 //not used
#define SAY_INTRO3          -1555010 //not used
#define SAY_AGGRO1          -1555011
#define SAY_AGGRO2          -1555012
#define SAY_AGGRO3          -1555013
#define SAY_SLAY1           -1555014
#define SAY_SLAY2           -1555015
#define SAY_HELP            -1555016 //not used
#define SAY_DEATH           -1555017

//below, not used
#define SAY2_INTRO1         -1555018
#define SAY2_INTRO2         -1555019
#define SAY2_INTRO3         -1555020
#define SAY2_AGGRO1         -1555021
#define SAY2_AGGRO2         -1555022
#define SAY2_AGGRO3         -1555023
#define SAY2_SLAY1          -1555024
#define SAY2_SLAY2          -1555025
#define SAY2_HELP           -1555026
#define SAY2_DEATH          -1555027

class boss_blackheart_the_inciter : public CreatureScript
{
public:
    boss_blackheart_the_inciter() : CreatureScript("boss_blackheart_the_inciter") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_blackheart_the_inciterAI(pCreature);
    }

    struct boss_blackheart_the_inciterAI : public ScriptedAI
    {
        boss_blackheart_the_inciterAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;

        bool InciteChaos;
        uint32 InciteChaos_Timer;
        uint32 InciteChaosWait_Timer;
        uint32 Charge_Timer;
        uint32 Knockback_Timer;

        void Reset()
        {
            InciteChaos = false;
            InciteChaos_Timer = 20000;
            InciteChaosWait_Timer = 15000;
            Charge_Timer = 5000;
            Knockback_Timer = 15000;

            if(pInstance)
                pInstance->SetData(DATA_BLACKHEARTTHEINCITEREVENT, NOT_STARTED);
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(RAND(SAY_SLAY1, SAY_SLAY2), me);
        }

        void JustDied(Unit* /*pVictim*/)
        {
            DoScriptText(SAY_DEATH, me);

            if(pInstance)
                pInstance->SetData(DATA_BLACKHEARTTHEINCITEREVENT, DONE);
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(RAND(SAY_AGGRO1, SAY_AGGRO2, SAY_AGGRO3), me);

            if(pInstance)
                pInstance->SetData(DATA_BLACKHEARTTHEINCITEREVENT, IN_PROGRESS);
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if(!UpdateVictim())
                return;

            if(InciteChaos)
            {
                if(InciteChaosWait_Timer <= diff)
                {
                    InciteChaos = false;
                    InciteChaosWait_Timer = 15000;
                } else InciteChaosWait_Timer -= diff;

                return;
            }

            if(InciteChaos_Timer <= diff)
            {
                DoCast(me, SPELL_INCITE_CHAOS);

                std::list<HostileReference *> t_list = me->getThreatManager().getThreatList();
                for(std::list<HostileReference *>::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                {
                    Unit* target = Unit::GetUnit(*me, (*itr)->getUnitGuid());
                    if(target && target->GetTypeId() == TYPEID_PLAYER)
                        target->CastSpell(target, SPELL_INCITE_CHAOS_B, true);
                }

                DoResetThreat();
                InciteChaos = true;
                InciteChaos_Timer = 40000;
                return;
            } else InciteChaos_Timer -= diff;

            //Charge_Timer
            if(Charge_Timer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_CHARGE);
                Charge_Timer = 15000 + rand()%10000;
            } else Charge_Timer -= diff;

            //Knockback_Timer
            if(Knockback_Timer <= diff)
            {
                DoCast(me, SPELL_WAR_STOMP);
                Knockback_Timer = 18000 + rand()%6000;
            } else Knockback_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_blackheart_the_inciter()
{
    new boss_blackheart_the_inciter;
}

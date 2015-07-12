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
#include "scarlet_monastery.h"

enum eEnums
{
    SAY_AGGRO               = -1189011,
    SAY_HEALTH1             = -1189012,
    SAY_HEALTH2             = -1189013,
    SAY_KILL                = -1189014,
    SAY_TRIGGER_VORREL      = -1189015,

    SPELL_SHADOWWORDPAIN    = 2767,
};

class boss_interrogator_vishas : public CreatureScript
{
public:
    boss_interrogator_vishas() : CreatureScript("boss_interrogator_vishas") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_interrogator_vishasAI(pCreature);
    }

    struct boss_interrogator_vishasAI : public ScriptedAI
    {
        boss_interrogator_vishasAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = me->GetInstanceScript();
        }

        InstanceScript* pInstance;

        bool Yell30;
        bool Yell60;
        uint32 ShadowWordPain_Timer;

        void Reset()
        {
            ShadowWordPain_Timer = 5000;
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(SAY_KILL, me);
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(!pInstance)
                return;

            //Any other actions to do with vorrel? setStandState?
            if(Unit* vorrel = Unit::GetUnit(*me, pInstance->GetData64(DATA_VORREL)))
                DoScriptText(SAY_TRIGGER_VORREL, vorrel);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            //If we are low on hp Do sayings
            if(!Yell60 && !HealthAbovePct(60))
            {
                DoScriptText(SAY_HEALTH1, me);
                Yell60 = true;
            }

            if(!Yell30 && !HealthAbovePct(30))
            {
                DoScriptText(SAY_HEALTH2, me);
                Yell30 = true;
            }

            //ShadowWordPain_Timer
            if(ShadowWordPain_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SHADOWWORDPAIN);
                ShadowWordPain_Timer = 5000 + rand()%10000;
            } else ShadowWordPain_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_interrogator_vishas()
{
    new boss_interrogator_vishas;
}

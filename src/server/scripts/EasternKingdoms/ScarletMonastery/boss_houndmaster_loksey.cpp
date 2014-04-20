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
    SAY_AGGRO                       = -1189021,
    SPELL_SUMMONSCARLETHOUND        = 17164,
    SPELL_BLOODLUST                 = 6742
};

class boss_houndmaster_loksey : public CreatureScript
{
public:
    boss_houndmaster_loksey() : CreatureScript("boss_houndmaster_loksey") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_houndmaster_lokseyAI(pCreature);
    }

    struct boss_houndmaster_lokseyAI : public ScriptedAI
    {
        boss_houndmaster_lokseyAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 BloodLust_Timer;

        void Reset()
        {
            BloodLust_Timer = 20000;
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(BloodLust_Timer <= diff)
            {
                DoCast(me, SPELL_BLOODLUST);
                BloodLust_Timer = 20000;
            } else BloodLust_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_houndmaster_loksey()
{
    new boss_houndmaster_loksey;
}

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

#define SPELL_LICHSLAP                  28873
#define SPELL_FROSTBOLTVOLLEY           8398
#define SPELL_MINDFLAY                  17313
#define SPELL_FROSTNOVA                 15531

class boss_scorn : public CreatureScript
{
public:
    boss_scorn() : CreatureScript("boss_scorn") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_scornAI(pCreature);
    }

    struct boss_scornAI : public ScriptedAI
    {
        boss_scornAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 LichSlap_Timer;
        uint32 FrostboltVolley_Timer;
        uint32 MindFlay_Timer;
        uint32 FrostNova_Timer;

        void Reset()
        {
            LichSlap_Timer = 45000;
            FrostboltVolley_Timer = 30000;
            MindFlay_Timer = 30000;
            FrostNova_Timer = 30000;
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            //LichSlap_Timer
            if(LichSlap_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_LICHSLAP);
                LichSlap_Timer = 45000;
            } else LichSlap_Timer -= diff;

            //FrostboltVolley_Timer
            if(FrostboltVolley_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FROSTBOLTVOLLEY);
                FrostboltVolley_Timer = 20000;
            } else FrostboltVolley_Timer -= diff;

            //MindFlay_Timer
            if(MindFlay_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_MINDFLAY);
                MindFlay_Timer = 20000;
            } else MindFlay_Timer -= diff;

            //FrostNova_Timer
            if(FrostNova_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FROSTNOVA);
                FrostNova_Timer = 15000;
            } else FrostNova_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_scorn()
{
    new boss_scorn;
}

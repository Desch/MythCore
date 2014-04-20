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

#define SPELL_SHADOWFLAME       22539
#define SPELL_WINGBUFFET        23339
#define SPELL_FLAMEBUFFET       23341

class boss_firemaw : public CreatureScript
{
public:
    boss_firemaw() : CreatureScript("boss_firemaw") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_firemawAI(pCreature);
    }

    struct boss_firemawAI : public ScriptedAI
    {
        boss_firemawAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 ShadowFlame_Timer;
        uint32 WingBuffet_Timer;
        uint32 FlameBuffet_Timer;

        void Reset()
        {
            ShadowFlame_Timer   = 30000;    //These times are probably wrong
            WingBuffet_Timer    = 24000;
            FlameBuffet_Timer   = 5000;
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoZoneInCombat();
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(ShadowFlame_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SHADOWFLAME);
                ShadowFlame_Timer = urand(15000, 18000);
            } else ShadowFlame_Timer -= diff;

            if(WingBuffet_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_WINGBUFFET);
                if(DoGetThreat(me->getVictim()))
                    DoModifyThreatPercent(me->getVictim(), -75);

                WingBuffet_Timer = 25000;
            } else WingBuffet_Timer -= diff;

            if(FlameBuffet_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FLAMEBUFFET);
                FlameBuffet_Timer = 5000;
            } else FlameBuffet_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_firemaw()
{
    new boss_firemaw;
}

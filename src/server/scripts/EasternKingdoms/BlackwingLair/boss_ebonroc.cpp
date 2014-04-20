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

#define SPELL_SHADOWFLAME           22539
#define SPELL_WINGBUFFET            18500
#define SPELL_SHADOWOFEBONROC       23340
#define SPELL_HEAL                  41386                   //Thea Heal spell of his Shadow

class boss_ebonroc : public CreatureScript
{
public:
    boss_ebonroc() : CreatureScript("boss_ebonroc") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_ebonrocAI(pCreature);
    }

    struct boss_ebonrocAI : public ScriptedAI
    {
        boss_ebonrocAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 ShadowFlame_Timer;
        uint32 WingBuffet_Timer;
        uint32 ShadowOfEbonroc_Timer;
        uint32 Heal_Timer;

        void Reset()
        {
            ShadowFlame_Timer       = 15000;    //These times are probably wrong
            WingBuffet_Timer        = 30000;
            ShadowOfEbonroc_Timer   = 45000;
            Heal_Timer              = 1000;
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
                ShadowFlame_Timer = urand(12000, 15000);
            } else ShadowFlame_Timer -= diff;

            if(WingBuffet_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_WINGBUFFET);
                WingBuffet_Timer = 25000;
            } else WingBuffet_Timer -= diff;

            if(ShadowOfEbonroc_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SHADOWOFEBONROC);
                ShadowOfEbonroc_Timer = urand(25000, 350000);
            } else ShadowOfEbonroc_Timer -= diff;

            if(me->getVictim()->HasAura(SPELL_SHADOWOFEBONROC))
            {
                if(Heal_Timer <= diff)
                {
                    DoCast(me, SPELL_HEAL);
                    Heal_Timer = urand(1000, 3000);
                } else Heal_Timer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_ebonroc()
{
    new boss_ebonroc;
}
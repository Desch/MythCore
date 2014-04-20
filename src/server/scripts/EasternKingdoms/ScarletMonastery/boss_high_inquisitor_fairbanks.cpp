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

enum eSpells
{
    SPELL_CURSEOFBLOOD              = 8282,
    SPELL_DISPELMAGIC               = 15090,
    SPELL_FEAR                      = 12096,
    SPELL_HEAL                      = 12039,
    SPELL_POWERWORDSHIELD           = 11647,
    SPELL_SLEEP                     = 8399
};

class boss_high_inquisitor_fairbanks : public CreatureScript
{
public:
    boss_high_inquisitor_fairbanks() : CreatureScript("boss_high_inquisitor_fairbanks") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_high_inquisitor_fairbanksAI(pCreature);
    }

    struct boss_high_inquisitor_fairbanksAI : public ScriptedAI
    {
        boss_high_inquisitor_fairbanksAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 CurseOfBlood_Timer;
        uint32 DispelMagic_Timer;
        uint32 Fear_Timer;
        uint32 Heal_Timer;
        uint32 Sleep_Timer;
        uint32 Dispel_Timer;
        bool PowerWordShield;

        void Reset()
        {
            CurseOfBlood_Timer = 10000;
            DispelMagic_Timer = 30000;
            Fear_Timer = 40000;
            Heal_Timer = 30000;
            Sleep_Timer = 30000;
            Dispel_Timer = 20000;
            PowerWordShield = false;
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            //If we are <25% hp cast Heal
            if(!HealthAbovePct(25) && !me->IsNonMeleeSpellCasted(false) && Heal_Timer <= diff)
            {
                DoCast(me, SPELL_HEAL);
                Heal_Timer = 30000;
            } else Heal_Timer -= diff;

            //Fear_Timer
            if(Fear_Timer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                    DoCast(target, SPELL_FEAR);

                Fear_Timer = 40000;
            } else Fear_Timer -= diff;

            //Sleep_Timer
            if(Sleep_Timer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0))
                    DoCast(target, SPELL_SLEEP);

                Sleep_Timer = 30000;
            } else Sleep_Timer -= diff;

            //PowerWordShield_Timer
            if(!PowerWordShield && !HealthAbovePct(25))
            {
                DoCast(me, SPELL_POWERWORDSHIELD);
                PowerWordShield = true;
            }

            //Dispel_Timer
            if(Dispel_Timer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_DISPELMAGIC);

                DispelMagic_Timer = 30000;
            } else DispelMagic_Timer -= diff;

            //CurseOfBlood_Timer
            if(CurseOfBlood_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CURSEOFBLOOD);
                CurseOfBlood_Timer = 25000;
            } else CurseOfBlood_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_high_inquisitor_fairbanks()
{
    new boss_high_inquisitor_fairbanks;
}

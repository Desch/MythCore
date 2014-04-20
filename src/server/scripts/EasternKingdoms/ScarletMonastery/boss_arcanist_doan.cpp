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
    SAY_AGGRO                   = -1189019,
    SAY_SPECIALAE               = -1189020,

    SPELL_POLYMORPH             = 13323,
    SPELL_AOESILENCE            = 8988,
    SPELL_ARCANEEXPLOSION       = 9433,
    SPELL_FIREAOE               = 9435,
    SPELL_ARCANEBUBBLE          = 9438,
};

class boss_arcanist_doan : public CreatureScript
{
public:
    boss_arcanist_doan() : CreatureScript("boss_arcanist_doan") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_arcanist_doanAI(pCreature);
    }

    struct boss_arcanist_doanAI : public ScriptedAI
    {
        boss_arcanist_doanAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 Polymorph_Timer;
        uint32 AoESilence_Timer;
        uint32 ArcaneExplosion_Timer;
        bool bCanDetonate;
        bool bShielded;

        void Reset()
        {
            Polymorph_Timer = 20000;
            AoESilence_Timer = 15000;
            ArcaneExplosion_Timer = 3000;
            bCanDetonate = false;
            bShielded = false;
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(bShielded && bCanDetonate)
            {
                DoCast(me, SPELL_FIREAOE);
                bCanDetonate = false;
            }

            if(me->HasAura(SPELL_ARCANEBUBBLE))
                return;

            //If we are <50% hp cast Arcane Bubble
            if(!bShielded && !HealthAbovePct(50))
            {
                //wait if we already casting
                if(me->IsNonMeleeSpellCasted(false))
                    return;

                DoScriptText(SAY_SPECIALAE, me);
                DoCast(me, SPELL_ARCANEBUBBLE);

                bCanDetonate = true;
                bShielded = true;
            }

            if(Polymorph_Timer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                    DoCast(target, SPELL_POLYMORPH);

                Polymorph_Timer = 20000;
            } else Polymorph_Timer -= diff;

            //AoESilence_Timer
            if(AoESilence_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_AOESILENCE);
                AoESilence_Timer = 15000 + rand()%5000;
            } else AoESilence_Timer -= diff;

            //ArcaneExplosion_Timer
            if(ArcaneExplosion_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_ARCANEEXPLOSION);
                ArcaneExplosion_Timer = 8000;
            } else ArcaneExplosion_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_arcanist_doan()
{
    new boss_arcanist_doan;
}

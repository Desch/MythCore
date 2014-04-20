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

class mob_yenniku : public CreatureScript
{
public:
    mob_yenniku() : CreatureScript("mob_yenniku") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_yennikuAI(pCreature);
    }

    struct mob_yennikuAI : public ScriptedAI
    {
        mob_yennikuAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            bReset = false;
        }

        uint32 Reset_Timer;
        bool bReset;

        void Reset()
        {
            Reset_Timer = 0;
            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
        }

        void SpellHit(Unit* caster, const SpellEntry* spell)
        {
            if(caster->GetTypeId() == TYPEID_PLAYER)
            {
                                                                //Yenniku's Release
                if(!bReset && CAST_PLR(caster)->GetQuestStatus(592) == QUEST_STATUS_INCOMPLETE && spell->Id == 3607)
                {
                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STUN);
                    me->CombatStop();                   //stop combat
                    me->DeleteThreatList();             //unsure of this
                    me->setFaction(83);                 //horde generic

                    bReset = true;
                    Reset_Timer = 60000;
                }
            }
            return;
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void UpdateAI(const uint32 diff)
        {
            if(bReset)
            {
                if(Reset_Timer <= diff)
                {
                    EnterEvadeMode();
                    bReset = false;
                    me->setFaction(28);                     //troll, bloodscalp
                    return;
                } else Reset_Timer -= diff;

                if(me->isInCombat() && me->getVictim())
                {
                    if(me->getVictim()->GetTypeId() == TYPEID_PLAYER)
                    {
                        Unit* victim = me->getVictim();
                        if(CAST_PLR(victim)->GetTeam() == HORDE)
                        {
                            me->CombatStop();
                            me->DeleteThreatList();
                        }
                    }
                }
             }

            //Return since we have no target
            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_stranglethorn_vale()
{
    new mob_yenniku;
}
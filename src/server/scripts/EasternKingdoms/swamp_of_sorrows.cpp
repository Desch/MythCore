/*
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
#include "ScriptedEscortAI.h"

enum Galen
{
    QUEST_GALENS_ESCAPE     = 1393,

    GO_GALENS_CAGE          = 37118,

    SAY_PERIODIC            = -1000500,
    SAY_QUEST_ACCEPTED      = -1000501,
    SAY_ATTACKED_1          = -1000502,
    SAY_ATTACKED_2          = -1000503,
    SAY_QUEST_COMPLETE      = -1000504,
    EMOTE_WHISPER           = -1000505,
    EMOTE_DISAPPEAR         = -1000506
};

class npc_galen_goodward : public CreatureScript
{
public:
    npc_galen_goodward() : CreatureScript("npc_galen_goodward") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const *quest)
    {
        if(quest->GetQuestId() == QUEST_GALENS_ESCAPE)
        {
            CAST_AI(npc_galen_goodward::npc_galen_goodwardAI, creature->AI())->Start(false, false, player->GetGUID());
            creature->setFaction(FACTION_ESCORT_N_NEUTRAL_ACTIVE);
            DoScriptText(SAY_QUEST_ACCEPTED, creature);
        }
        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_galen_goodwardAI(pCreature);
    }

    struct npc_galen_goodwardAI : public npc_escortAI
    {
        npc_galen_goodwardAI(Creature* pCreature) : npc_escortAI(pCreature)
        {
            m_uiGalensCageGUID = 0;
            Reset();
        }

        uint64 m_uiGalensCageGUID;
        uint32 m_uiPeriodicSay;

        void Reset()
        {
            m_uiPeriodicSay = 6000;
        }

        void EnterCombat(Unit* who)
        {
            if(HasEscortState(STATE_ESCORT_ESCORTING))
                DoScriptText(RAND(SAY_ATTACKED_1, SAY_ATTACKED_2), me, who);
        }

        void WaypointStart(uint32 uiPointId)
        {
            switch(uiPointId)
            {
                case 0:
                    {
                        GameObject* pCage = NULL;
                        if(m_uiGalensCageGUID)
                            pCage = me->GetMap()->GetGameObject(m_uiGalensCageGUID);
                        else
                            pCage = GetClosestGameObjectWithEntry(me, GO_GALENS_CAGE, INTERACTION_DISTANCE);
                        if(pCage)
                        {
                            pCage->UseDoorOrButton();
                            m_uiGalensCageGUID = pCage->GetGUID();
                        }
                        break;
                    }
                case 21:
                    DoScriptText(EMOTE_DISAPPEAR, me);
                    break;
            }
        }

        void WaypointReached(uint32 uiPointId)
        {
            switch(uiPointId)
            {
                case 0:
                    if(GameObject* pCage = me->GetMap()->GetGameObject(m_uiGalensCageGUID))
                        pCage->ResetDoorOrButton();
                    break;
                case 20:
                    if(Player* player = GetPlayerForEscort())
                    {
                        me->SetFacingToObject(player);
                        DoScriptText(SAY_QUEST_COMPLETE, me, player);
                        DoScriptText(EMOTE_WHISPER, me, player);
                        player->GroupEventHappens(QUEST_GALENS_ESCAPE, me);
                    }
                    SetRun(true);
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);

            if(HasEscortState(STATE_ESCORT_NONE))
                return;

            if(m_uiPeriodicSay < diff)
            {
                if(!HasEscortState(STATE_ESCORT_ESCORTING))
                    DoScriptText(SAY_PERIODIC, me);
                m_uiPeriodicSay = 15000;
            }
            else
                m_uiPeriodicSay -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_swamp_of_sorrows()
{
    new npc_galen_goodward;
}
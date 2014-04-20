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
#include "ScriptedFollowerAI.h"

enum eMist
{
    SAY_AT_HOME             = -1000323,
    EMOTE_AT_HOME           = -1000324,
    QUEST_MIST              = 938,
    NPC_ARYNIA              = 3519,
    FACTION_DARNASSUS       = 79
};

class npc_mist : public CreatureScript
{
public:
    npc_mist() : CreatureScript("npc_mist") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* pQuest)
    {
        if(pQuest->GetQuestId() == QUEST_MIST)
        {
            if(npc_mistAI* pMistAI = CAST_AI(npc_mist::npc_mistAI, creature->AI()))
                pMistAI->StartFollow(player, FACTION_DARNASSUS, pQuest);
        }

        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mistAI(pCreature);
    }

    struct npc_mistAI : public FollowerAI
    {
        npc_mistAI(Creature* pCreature) : FollowerAI(pCreature) { }

        void Reset() { }

        void MoveInLineOfSight(Unit* who)
        {
            FollowerAI::MoveInLineOfSight(who);

            if(!me->getVictim() && !HasFollowState(STATE_FOLLOW_COMPLETE) && who->GetEntry() == NPC_ARYNIA)
            {
                if(me->IsWithinDistInMap(who, 10.0f))
                {
                    DoScriptText(SAY_AT_HOME, who);
                    DoComplete();
                }
            }
        }

        void DoComplete()
        {
            DoScriptText(EMOTE_AT_HOME, me);

            if(Player* player = GetLeaderForFollower())
            {
                if(player->GetQuestStatus(QUEST_MIST) == QUEST_STATUS_INCOMPLETE)
                    player->GroupEventHappens(QUEST_MIST, me);
            }

            SetFollowComplete();
        }
    };
};

void AddSC_teldrassil()
{
    new npc_mist;
}

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

enum eCorporalKeeshan
{
    QUEST_MISSING_IN_ACTION = 219,

    SAY_CORPORAL_1  = -1000464,
    SAY_CORPORAL_2  = -1000465,
    SAY_CORPORAL_3  = -1000466,
    SAY_CORPORAL_4  = -1000467,
    SAY_CORPORAL_5  = -1000468,

    SPELL_MOCKING_BLOW  = 21008,
    SPELL_SHIELD_BASH   = 11972,
};

class npc_corporal_keeshan : public CreatureScript
{
public:
    npc_corporal_keeshan() : CreatureScript("npc_corporal_keeshan") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const *pQuest)
    {
        if(pQuest->GetQuestId() == QUEST_MISSING_IN_ACTION)
        {
            CAST_AI(npc_corporal_keeshan::npc_corporal_keeshanAI, creature->AI())->Start(true, false, player->GetGUID(), pQuest);
            DoScriptText(SAY_CORPORAL_1, creature);
        }

        return false;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_corporal_keeshanAI(pCreature);
    }

    struct npc_corporal_keeshanAI : public npc_escortAI
    {
        npc_corporal_keeshanAI(Creature* pCreature) : npc_escortAI(pCreature) { }

        uint32 uiPhase;
        uint32 uiTimer;
        uint32 uiMockingBlowTimer;
        uint32 uiShieldBashTimer;

        void Reset()
        {
            uiTimer = 0;
            uiPhase = 0;
            uiMockingBlowTimer = 5000;
            uiShieldBashTimer  = 8000;
        }

        void WaypointReached(uint32 uiI)
        {
            Player* player = GetPlayerForEscort();

            if(!player)
                return;

            if(uiI >= 65 && me->GetUnitMovementFlags() == MOVEMENTFLAG_WALKING)
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);

            switch(uiI)
            {
                case 39:
                    SetEscortPaused(true);
                    uiTimer = 2000;
                    uiPhase = 1;
                    break;
                case 65:
                    me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                    break;
                case 115:
                    player->AreaExploredOrEventHappens(QUEST_MISSING_IN_ACTION);
                    uiTimer = 2000;
                    uiPhase = 4;
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(HasEscortState(STATE_ESCORT_NONE))
                return;

            npc_escortAI::UpdateAI(diff);

            if(uiPhase)
            {
                if(uiTimer <= diff)
                {
                    switch(uiPhase)
                    {
                        case 1:
                            me->SetStandState(UNIT_STAND_STATE_SIT);
                            uiTimer = 1000;
                            uiPhase = 2;
                            break;
                        case 2:
                            DoScriptText(SAY_CORPORAL_2, me);
                            uiTimer = 15000;
                            uiPhase = 3;
                            break;
                        case 3:
                            DoScriptText(SAY_CORPORAL_3, me);
                            me->SetStandState(UNIT_STAND_STATE_STAND);
                            SetEscortPaused(false);
                            uiTimer = 0;
                            uiPhase = 0;
                            break;
                        case 4:
                            DoScriptText(SAY_CORPORAL_4, me);
                            uiTimer = 2500;
                            uiPhase = 5;
                        case 5:
                            DoScriptText(SAY_CORPORAL_5, me);
                            uiTimer = 0;
                            uiPhase = 0;
                    }
                } else uiTimer -= diff;
            }

            if(!UpdateVictim())
                return;

            if(uiMockingBlowTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_MOCKING_BLOW);
                uiMockingBlowTimer = 5000;
            } else uiMockingBlowTimer -= diff;

            if(uiShieldBashTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_MOCKING_BLOW);
                uiShieldBashTimer = 8000;
            } else uiShieldBashTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_redridge_mountains()
{
    new npc_corporal_keeshan;
}
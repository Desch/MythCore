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

//possible creatures to be spawned
const uint32 possibleSpawns[32] = {17322, 17661, 17496, 17522, 17340, 17352, 17333, 17524, 17654, 17348, 17339, 17345, 17359, 17353, 17336, 17550, 17330, 17701, 17321, 17680, 17325, 17320, 17683, 17342, 17715, 17334, 17341, 17338, 17337, 17346, 17344, 17327};

class mob_webbed_creature : public CreatureScript
{
public:
    mob_webbed_creature() : CreatureScript("mob_webbed_creature") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_webbed_creatureAI(pCreature);
    }

    struct mob_webbed_creatureAI : public ScriptedAI
    {
        mob_webbed_creatureAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        void Reset() { }

        void EnterCombat(Unit* /*pWho*/) { }

        void JustDied(Unit* Killer)
        {
            uint32 spawnCreatureID = 0;

            switch(urand(0, 2))
            {
                case 0:
                    spawnCreatureID = 17681;
                    if(Killer->GetTypeId() == TYPEID_PLAYER)
                        CAST_PLR(Killer)->KilledMonsterCredit(spawnCreatureID, 0);
                    break;
                case 1:
                case 2:
                    spawnCreatureID = possibleSpawns[urand(0, 30)];
                    break;
            }

            if(spawnCreatureID)
                me->SummonCreature(spawnCreatureID, 0.0f, 0.0f, 0.0f, me->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
        }
    };
};

#define C_SUNHAWK_TRIGGER 17974

#define GOSSIP_HELLO_CSA     "[PH] "
#define GOSSIP_SELECT_CSA1   "[PH] "
#define GOSSIP_SELECT_CSA2   "[PH] "
#define GOSSIP_SELECT_CSA3   "[PH] "
#define GOSSIP_SELECT_CSA4   "[PH] "
#define GOSSIP_SELECT_CSA5   "[PH] "

class npc_captured_sunhawk_agent : public CreatureScript
{
public:
    npc_captured_sunhawk_agent() : CreatureScript("npc_captured_sunhawk_agent") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT_CSA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                player->SEND_GOSSIP_MENU(9137, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT_CSA2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
                player->SEND_GOSSIP_MENU(9138, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT_CSA3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
                player->SEND_GOSSIP_MENU(9139, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT_CSA4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
                player->SEND_GOSSIP_MENU(9140, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+5:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT_CSA5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
                player->SEND_GOSSIP_MENU(9141, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+6:
                player->CLOSE_GOSSIP_MENU();
                player->TalkedToCreature(C_SUNHAWK_TRIGGER, creature->GetGUID());
                break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(player->HasAura(31609) && player->GetQuestStatus(9756) == QUEST_STATUS_INCOMPLETE)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_CSA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(9136, creature->GetGUID());
        }
        else
            player->SEND_GOSSIP_MENU(9134, creature->GetGUID());

        return true;
    }
};

enum eStillpine
{
    QUEST_SAVING_PRINCESS_STILLPINE               = 9667,
    NPC_PRINCESS_STILLPINE                        = 17682,
    GO_PRINCESS_STILLPINES_CAGE                   = 181928,
    SPELL_OPENING_PRINCESS_STILLPINE_CREDIT       = 31003,
    SAY_DIRECTION                                 = -1800074
};

class go_princess_stillpines_cage : public GameObjectScript
{
public:
    go_princess_stillpines_cage() : GameObjectScript("go_princess_stillpines_cage") { }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        if(Creature* stillpine = go->FindNearestCreature(NPC_PRINCESS_STILLPINE, 25, true))
        {
            go->SetGoState(GO_STATE_ACTIVE);
            stillpine->GetMotionMaster()->MovePoint(1, go->GetPositionX(), go->GetPositionY()-15, go->GetPositionZ());
            player->CastedCreatureOrGO(NPC_PRINCESS_STILLPINE, 0, SPELL_OPENING_PRINCESS_STILLPINE_CREDIT);
        }
        return true;
    }
};

class npc_princess_stillpine : public CreatureScript
{
public:
    npc_princess_stillpine() : CreatureScript("npc_princess_stillpine") { }

    struct npc_princess_stillpineAI : public ScriptedAI
    {
        npc_princess_stillpineAI(Creature* pCreature): ScriptedAI(pCreature) { }

        void MovementInform(uint32 type, uint32 id)
        {
            if(type == POINT_MOTION_TYPE && id == 1)
            {
                DoScriptText(SAY_DIRECTION, me);
                me->ForcedDespawn();
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_princess_stillpineAI(pCreature);
    }
};

void AddSC_bloodmyst_isle()
{
    new mob_webbed_creature;
    new npc_captured_sunhawk_agent;
    new npc_princess_stillpine;
    new go_princess_stillpines_cage;
}

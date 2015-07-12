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
#include "oculus.h"

#define GOSSIP_ITEM_DRAKES         "Alors, où allons-nous maintenant ?"
#define GOSSIP_ITEM_BELGARISTRASZ1 "Je veux voler sur les ailes d'un vol Rouge"
#define GOSSIP_ITEM_BELGARISTRASZ2 "Quelles sont les aptitudes que possèdent les Dragons de ruby ? (Tank)"
#define GOSSIP_ITEM_VERDISA1       "Je veux voler sur les ailes d'un vol vert"
#define GOSSIP_ITEM_VERDISA2       "Quelles sont les aptitudes que possèdent les Dragons d'émeraude ? (Healer)"
#define GOSSIP_ITEM_ETERNOS1       "Je veux voler sur les ailes d'un vol de bronze"
#define GOSSIP_ITEM_ETERNOS2       "Quelles sont les aptitudes que possèdent les Dragons de bronze ? (DPS)"

#define HAS_ESSENCE(a) ((a)->HasItemCount(ITEM_EMERALD_ESSENCE, 1) || (a)->HasItemCount(ITEM_AMBER_ESSENCE, 1) || (a)->HasItemCount(ITEM_RUBY_ESSENCE, 1))

enum Drakes
{
    GOSSIP_TEXTID_DRAKES                          = 13267,
    GOSSIP_TEXTID_BELGARISTRASZ1                  = 12916,
    GOSSIP_TEXTID_BELGARISTRASZ2                  = 13466,
    GOSSIP_TEXTID_BELGARISTRASZ3                  = 13254,
    GOSSIP_TEXTID_VERDISA1                        = 1,
    GOSSIP_TEXTID_VERDISA2                        = 1,
    GOSSIP_TEXTID_VERDISA3                        = 1,
    GOSSIP_TEXTID_ETERNOS1                        = 1,
    GOSSIP_TEXTID_ETERNOS2                        = 1,
    GOSSIP_TEXTID_ETERNOS3                        = 13256,

    ITEM_EMERALD_ESSENCE                          = 37815,
    ITEM_AMBER_ESSENCE                            = 37859,
    ITEM_RUBY_ESSENCE                             = 37860,

    NPC_VERDISA                                   = 27657,
    NPC_BELGARISTRASZ                             = 27658,
    NPC_ETERNOS                                   = 27659
};

class npc_oculus_drake : public CreatureScript
{
public:
    npc_oculus_drake() : CreatureScript("npc_oculus_drake") { }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch(pCreature->GetEntry())
        {
        case NPC_VERDISA:
            switch(uiAction)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                if(!HAS_ESSENCE(pPlayer))
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA1, pCreature->GetGUID());
                }
                else
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA2, pCreature->GetGUID());
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
            {
                ItemPosCountVec dest;
                uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_EMERALD_ESSENCE, 1);
                if(msg == EQUIP_ERR_OK)
                    pPlayer->StoreNewItem(dest, ITEM_EMERALD_ESSENCE, true);
                pPlayer->CLOSE_GOSSIP_MENU();
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3:
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA3, pCreature->GetGUID());
                break;
            }
            break;
        case NPC_BELGARISTRASZ:
            switch(uiAction)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                if(!HAS_ESSENCE(pPlayer))
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ1, pCreature->GetGUID());
                }
                else
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ2, pCreature->GetGUID());
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
            {
                ItemPosCountVec dest;
                uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_RUBY_ESSENCE, 1);
                if(msg == EQUIP_ERR_OK)
                    pPlayer->StoreNewItem(dest, ITEM_RUBY_ESSENCE, true);
                pPlayer->CLOSE_GOSSIP_MENU();
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3:
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ3, pCreature->GetGUID());
                break;
            }
            break;
        case NPC_ETERNOS:
            switch(uiAction)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                if(!HAS_ESSENCE(pPlayer))
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS1, pCreature->GetGUID());
                }
                else
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS2, pCreature->GetGUID());
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
            {
                ItemPosCountVec dest;
                uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_AMBER_ESSENCE, 1);
                if(msg == EQUIP_ERR_OK)
                    pPlayer->StoreNewItem(dest, ITEM_AMBER_ESSENCE, true);
                pPlayer->CLOSE_GOSSIP_MENU();
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3:
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS3, pCreature->GetGUID());
                break;
            }
            break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if(InstanceScript* instance = creature->GetInstanceScript())
        {
            if(instance->GetBossState(DATA_DRAKOS_EVENT) == DONE)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DRAKES, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DRAKES, creature->GetGUID());
            }
        }

        return true;
    }
};

void AddSC_oculus()
{
    new npc_oculus_drake;
}

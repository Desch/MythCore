/*
Custom patches
*/

#include "ScriptPCH.h"

#define MSG_GOSSIP_TEXT_MOUNT           "Призвать маунта"
#define MSG_HI_LEVEL                    "У вас слишком высокий уровень"
#define MSG_MOUTED                      "Вы уже на маунте"
#define MOUNT_SPELL_1                   58997
#define MOUNT_SPELL_2                   58999


class npc_mount : public CreatureScript
{
    public:
    npc_mount() : CreatureScript("npc_mount") { }
    
    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MSG_GOSSIP_TEXT_MOUNT, GOSSIP_SENDER_MAIN, 1);
        pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
	    if(uiSender != GOSSIP_SENDER_MAIN)
            return false;

        pPlayer->PlayerTalkClass->ClearMenus();

	    switch (uiAction)
	    {
		    case 1:
                if(pPlayer->getLevel()>60)
                {
                    pCreature->MonsterWhisper(MSG_HI_LEVEL, pPlayer->GetGUID());
                    pPlayer->CLOSE_GOSSIP_MENU();
			        break;
                }
                if(pPlayer->IsMounted())
                {
                    pCreature->MonsterWhisper(MSG_MOUTED, pPlayer->GetGUID());
                    pPlayer->CLOSE_GOSSIP_MENU();
			        break;
                }
                if(pPlayer->getLevel()>30)
                {
                    pPlayer->AddAura(MOUNT_SPELL_2, pPlayer);
                    pPlayer->CLOSE_GOSSIP_MENU();
                    break;
                }
                if(pPlayer->getLevel()<30)
                {
                    pPlayer->AddAura(MOUNT_SPELL_1, pPlayer);
                    pPlayer->CLOSE_GOSSIP_MENU();
                    break;
                }
		    default: 
			    pPlayer->CLOSE_GOSSIP_MENU();
	    }
        return true;
    }
};

void AddSC_npc_mount()
{
    new npc_mount;
}
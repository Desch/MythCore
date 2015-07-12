#include "ScriptPCH.h"



/*******************************************************
 * npc_welcome
 *******************************************************/

class npc_welcome : public CreatureScript
{
public:
	npc_welcome() : CreatureScript("npc_welcome") { }

	bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
	{
		if (uiSender == GOSSIP_SENDER_MAIN) SendDefaultMenu_npc_welcome(pPlayer, pCreature, uiAction);
		return true;
	}
 
	bool OnGossipHello(Player* pPlayer, Creature* pCreature)
	{
    		pPlayer->ADD_GOSSIP_ITEM( 1, " -  Lettre de Bienvenue" , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    		pPlayer->ADD_GOSSIP_ITEM( 0, " -  Channel général"     , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    		pPlayer->ADD_GOSSIP_ITEM( 5, " -  Capacité Serveur"    , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    		pPlayer->ADD_GOSSIP_ITEM( 3, " -  Règles"              , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    		pPlayer->ADD_GOSSIP_ITEM( 8, " -  Instance"            , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    		pPlayer->SEND_GOSSIP_MENU(100010, pCreature->GetGUID());

    		return true;
	}

	void SendDefaultMenu_npc_welcome(Player* pPlayer, Creature* pCreature, uint32 uiAction)
	{
    		switch (uiAction)
    		{
        		case GOSSIP_ACTION_INFO_DEF + 1:                    //Lettre de Bienvenue
            			pPlayer->SEND_GOSSIP_MENU(100005, pCreature->GetGUID());
            			break;
        		case GOSSIP_ACTION_INFO_DEF + 2:                    //Channel général
            			pPlayer->SEND_GOSSIP_MENU(100006, pCreature->GetGUID());
            			break;
        		case GOSSIP_ACTION_INFO_DEF + 3:                    //Capacité Serveur
            			pPlayer->SEND_GOSSIP_MENU(100007, pCreature->GetGUID());
           			break;
        		case GOSSIP_ACTION_INFO_DEF + 4:                    //Règles
            			pPlayer->SEND_GOSSIP_MENU(100008, pCreature->GetGUID());
            			break;
        		case GOSSIP_ACTION_INFO_DEF + 5:                    //Instance
            			pPlayer->SEND_GOSSIP_MENU(100009, pCreature->GetGUID());
            			break;

			pPlayer->CLOSE_GOSSIP_MENU();
    		}
	}

};


void AddSC_npc_welcome()
{
    new npc_welcome();
}

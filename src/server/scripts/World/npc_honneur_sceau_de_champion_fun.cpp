/* ScriptData
SDName: npc_honneur_sceau_de_champion_fun
SD%Complete: 100%
SDComment: Change des points d'honneur en Sceau de champion
SDCategory: NPC
EndScriptData */

#include "ScriptPCH.h"
#include "Log.h"
#include "Player.h"


#define GOSSIP_100GOULET_TO_HONNEUR    "Changer 100 marques du goulet de chanteguere contre 5000 points d'honneur"
#define GOSSIP_50GOULET_TO_HONNEUR     "Changer 50 marques du goulet de chanteguere contre 2500 points d'honneur"
#define GOSSIP_100ARATHI_TO_HONNEUR    "Changer 100 marques d'arathi contre 15000 points d'honneur"
#define GOSSIP_50ARATHI_TO_HONNEUR     "Changer 50 marques d'arathi contre 7500 points d'honneur"
#define GOSSIP_100ALTERAC_TO_HONNEUR   "Changer 100 marques d'alterac contre 15000 points d'honneur"
#define GOSSIP_50ALTERAC_TO_HONNEUR    "Changer 50 marques d'alterac contre 7500 points d'honneur"
#define GOSSIP_100CYCLONE_TO_HONNEUR   "Changer 100 marques de l'oeil du cyclone contre 15000 points d'honneur"
#define GOSSIP_50CYCLONE_TO_HONNEUR    "Changer 50 marques de l'oeil du cyclone contre 7500 points d'honneur"
#define GOSSIP_100JOUG_TO_HONNEUR      "Changer 100 marques du joug contre 15000 points d'honneur"
#define GOSSIP_50JOUG_TO_HONNEUR       "Changer 50 marques du joug contre 7500 points d'honneur"

class npc_honneur_sceau_de_champion_fun : public CreatureScript
{
public:
    npc_honneur_sceau_de_champion_fun() : CreatureScript("npc_honneur_sceau_de_champion_fun") { }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
		pPlayer->PlayerTalkClass->ClearMenus();
    	if (uiSender == GOSSIP_SENDER_MAIN)
    	   SendDefaultMenu_npc_honneur_sceau_de_champion_fun(pPlayer, pCreature, uiAction);

    return true;
    }


    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {

        pPlayer->ADD_GOSSIP_ITEM( 7, "Echange d'honneur contre des Sceau de champion ->"                       , GOSSIP_SENDER_MAIN, 6000);
        pPlayer->ADD_GOSSIP_ITEM( 7, "Echange d'arène contre des Sceau de champion ->"                         , GOSSIP_SENDER_MAIN, 7000);

        pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
        return true;
    }

	
void SendDefaultMenu_npc_honneur_sceau_de_champion_fun(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{

if (pPlayer->isInCombat())
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterWhisper("Vous êtes en combat.", pPlayer->GetGUID());
	return;
}

switch(uiAction)
{

case 1000:
	pPlayer->ADD_GOSSIP_ITEM(5, GOSSIP_50GOULET_TO_HONNEUR, GOSSIP_SENDER_MAIN, 1001);
	pPlayer->ADD_GOSSIP_ITEM(5, GOSSIP_100GOULET_TO_HONNEUR, GOSSIP_SENDER_MAIN, 1002);
	pPlayer->ADD_GOSSIP_ITEM(7, "Menu ->", GOSSIP_SENDER_MAIN, 9000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 2000:
	pPlayer->ADD_GOSSIP_ITEM(5, GOSSIP_50ARATHI_TO_HONNEUR, GOSSIP_SENDER_MAIN, 2001);
	pPlayer->ADD_GOSSIP_ITEM(5, GOSSIP_100ARATHI_TO_HONNEUR, GOSSIP_SENDER_MAIN, 2002);
	pPlayer->ADD_GOSSIP_ITEM(7, "Menu ->", GOSSIP_SENDER_MAIN, 9000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 3000:
	pPlayer->ADD_GOSSIP_ITEM(5, GOSSIP_50ALTERAC_TO_HONNEUR, GOSSIP_SENDER_MAIN, 3001);
	pPlayer->ADD_GOSSIP_ITEM(5, GOSSIP_100ALTERAC_TO_HONNEUR, GOSSIP_SENDER_MAIN, 3002);
	pPlayer->ADD_GOSSIP_ITEM(7, "Menu ->", GOSSIP_SENDER_MAIN, 9000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 4000:
	pPlayer->ADD_GOSSIP_ITEM(5, GOSSIP_50CYCLONE_TO_HONNEUR, GOSSIP_SENDER_MAIN, 4001);
	pPlayer->ADD_GOSSIP_ITEM(5, GOSSIP_100CYCLONE_TO_HONNEUR, GOSSIP_SENDER_MAIN, 4002);
	pPlayer->ADD_GOSSIP_ITEM(7, "Menu ->", GOSSIP_SENDER_MAIN, 9000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 5000:
	pPlayer->ADD_GOSSIP_ITEM(5, GOSSIP_50JOUG_TO_HONNEUR, GOSSIP_SENDER_MAIN, 5001);
	pPlayer->ADD_GOSSIP_ITEM(5, GOSSIP_100JOUG_TO_HONNEUR, GOSSIP_SENDER_MAIN, 5002);
	pPlayer->ADD_GOSSIP_ITEM(7, "Menu ->", GOSSIP_SENDER_MAIN, 9000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 6000:
	pPlayer->ADD_GOSSIP_ITEM(5, "Changer 1700 points d'honneurs contre 1 Sceau de champion",   GOSSIP_SENDER_MAIN, 6001);
	pPlayer->ADD_GOSSIP_ITEM(5, "Changer 17000 points d'honneurs contre 10 Sceau de champion", GOSSIP_SENDER_MAIN, 6002);
	pPlayer->ADD_GOSSIP_ITEM(5, "Changer 50000 points d'honneurs contre 30 Sceau de champion", GOSSIP_SENDER_MAIN, 6003);
	pPlayer->ADD_GOSSIP_ITEM(7, "Menu ->", GOSSIP_SENDER_MAIN, 9000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;    

case 7000:
	pPlayer->ADD_GOSSIP_ITEM(5, "Changer 1000 points d'arènes contre 10 Sceau de champion", GOSSIP_SENDER_MAIN, 7001);
	pPlayer->ADD_GOSSIP_ITEM(5, "Changer 2000 points d'arènes contre 20 Sceau de champion", GOSSIP_SENDER_MAIN, 7002);
	pPlayer->ADD_GOSSIP_ITEM(5, "Changer 3000 points d'arènes contre 30 Sceau de champion", GOSSIP_SENDER_MAIN, 7003);
	pPlayer->ADD_GOSSIP_ITEM(7, "Menu ->", GOSSIP_SENDER_MAIN, 9000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 9000:
    pPlayer->ADD_GOSSIP_ITEM( 7, "Echange d'honneur contre des Sceau de champion ->"                                   , GOSSIP_SENDER_MAIN, 6000);
    pPlayer->ADD_GOSSIP_ITEM( 7, "Echange d'arène contre des Sceau de champion ->"                                     , GOSSIP_SENDER_MAIN, 7000);

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 1001:
	if (pPlayer->HasItemCount(20558, 50))
	{
		pPlayer->DestroyItemCount(20558, 50, true);
		pPlayer->ModifyHonorPoints(2500);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de marque, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1002:
	if (pPlayer->HasItemCount(20558, 100))
	{
		pPlayer->DestroyItemCount(20558, 100, true);
		pPlayer->ModifyHonorPoints(5000);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de marque, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 2001:
	if (pPlayer->HasItemCount(20559, 50))
	{
		pPlayer->DestroyItemCount(20559, 50, true);
		pPlayer->ModifyHonorPoints(7500);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de marque, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 2002:
	if (pPlayer->HasItemCount(20559, 100))
	{
		pPlayer->DestroyItemCount(20559, 100, true);
		pPlayer->ModifyHonorPoints(15000);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de marque, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 3001:
	if (pPlayer->HasItemCount(20560, 50))
	{
		pPlayer->DestroyItemCount(20560, 50, true);
		pPlayer->ModifyHonorPoints(7500);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de marque, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 3002:
	if (pPlayer->HasItemCount(20560, 100))
	{
		pPlayer->DestroyItemCount(20560, 100, true);
		pPlayer->ModifyHonorPoints(15000);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de marque, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 4001:
	if (pPlayer->HasItemCount(29024, 50))
	{
		pPlayer->DestroyItemCount(29024, 50, true);
		pPlayer->ModifyHonorPoints(7500);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de marque, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 4002:
	if (pPlayer->HasItemCount(29024, 100))
	{
		pPlayer->DestroyItemCount(29024, 100, true);
		pPlayer->ModifyHonorPoints(15000);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de marque, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 5001:
	if (pPlayer->HasItemCount(43589, 50))
	{
		pPlayer->DestroyItemCount(43589, 50, true);
		pPlayer->ModifyHonorPoints(7500);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de marque, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 5002:
	if (pPlayer->HasItemCount(43589, 100))
	{
		pPlayer->DestroyItemCount(43589, 100, true);
		pPlayer->ModifyHonorPoints(15000);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de marque, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 6001:
	if (pPlayer->GetHonorPoints() >= 1700)
	{
              pCreature->addItem(pPlayer, 44990, 1, true, true, false);
		pPlayer->ModifyHonorPoints(-1700);
              sLog->outDetail("Compte '%u'; GUID '%u' ?chet?1' chap contre de l'honneur",pPlayer->GetSession()->GetAccountId(), pPlayer->GetGUID());
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez d'honneur, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 6002:
	if (pPlayer->GetHonorPoints() >= 17000)
	{
              pCreature->addItem(pPlayer, 44990, 10, true, true, false);
		pPlayer->ModifyHonorPoints(-17000);
              sLog->outDetail("Compte '%u'; GUID '%u' ?chet?10' chap contre de l'honneur",pPlayer->GetSession()->GetAccountId(), pPlayer->GetGUID());
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez d'honneur, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 6003:
	if (pPlayer->GetHonorPoints() >= 50000)
	{
              pCreature->addItem(pPlayer, 44990, 30, true, true, false);
		pPlayer->ModifyHonorPoints(-50000);
              sLog->outString("Compte '%u'; GUID '%u' ?chet?30' chap contre de l'honneur",pPlayer->GetSession()->GetAccountId(), pPlayer->GetGUID());
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez d'honneur, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 7001:
	if (pPlayer->GetArenaPoints() >= 1000)
	{
              pCreature->addItem(pPlayer, 44990, 10, true, true, false);
		pPlayer->ModifyArenaPoints(-1000);
              sLog->outString("Compte '%u'; GUID '%u' ?chet?10' chap contre des points d'arenes",pPlayer->GetSession()->GetAccountId(), pPlayer->GetGUID());
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de point d'arene, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 7002:
	if (pPlayer->GetArenaPoints() >= 2000)
	{
              pCreature->addItem(pPlayer, 44990, 20, true, true, false);
		pPlayer->ModifyArenaPoints(-2000);
              sLog->outString("Compte '%u'; GUID '%u' ?chet?20' chap contre des points d'arenes",pPlayer->GetSession()->GetAccountId(), pPlayer->GetGUID());
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de point d'arene, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 7003:
	if (pPlayer->GetArenaPoints() >= 3000)
	{
              pCreature->addItem(pPlayer, 44990, 30, true, true, false);
		pPlayer->ModifyArenaPoints(-3000);
              sLog->outDetail("Compte '%u'; GUID '%u' ?chet?30' chap contre des points d'arenes",pPlayer->GetSession()->GetAccountId(), pPlayer->GetGUID());
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de point d'arene, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 10001:
    if (pPlayer->HasItemCount(24245, 1))
	{
		pPlayer->DestroyItemCount(24245, 1, true);
		pCreature->addItem(pPlayer, 44990, 1, true, true, false);
              uint32 count = 1;
		//pPlayer->LogChap(count);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de Sceau de champion, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 10002:
    if (pPlayer->HasItemCount(24245, 10))
	{
		pPlayer->DestroyItemCount(24245, 10, true);
		pCreature->addItem(pPlayer, 44990, 10, true, true, false);
              uint32 count = 10;
		//pPlayer->LogChap(count);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de Sceau de champion, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 10003:
    if (pPlayer->HasItemCount(24245, 20))
	{
		pPlayer->DestroyItemCount(24245, 20, true);
		pCreature->addItem(pPlayer, 44990, 20, true, true, false);
              uint32 count = 20;
		//pPlayer->LogChap(count);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de Sceau de champion, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 10004:
    if (pPlayer->HasItemCount(24245, 30))
	{
		pPlayer->DestroyItemCount(24245, 30, true);
		pCreature->addItem(pPlayer, 44990, 30, true, true, false);
              uint32 count = 30;
		//pPlayer->LogChap(count);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de Sceau de champion, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 10005:
    if (pPlayer->HasItemCount(24245, 50))
	{
		pPlayer->DestroyItemCount(24245, 50, true);
		pCreature->addItem(pPlayer, 44990, 50, true, true, false);
              uint32 count = 50;
		//pPlayer->LogChap(count);
	}
	else 
		pCreature->MonsterWhisper("Vous n'avez pas assez de Sceau de champion, $C.", pPlayer->GetGUID());

	pPlayer->CLOSE_GOSSIP_MENU();
break;

}
}

};


void AddSC_npc_honneur_sceau_de_champion_fun()
{
    new npc_honneur_sceau_de_champion_fun();
}

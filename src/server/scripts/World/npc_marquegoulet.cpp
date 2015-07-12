#include "ScriptPCH.h"

#define EM_GOULET 20558


#define ERROR_GOULET "Vous n'avez pas assez de marques de Goulet"


class npc_marquegoulet : public CreatureScript
{
public:
	npc_marquegoulet() : CreatureScript("npc_marquegoulet") { }

	bool OnGossipHello(Player * pPlayer, Creature * _Creature)
	{
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger vos marques de goulet contre de l'honneur" , GOSSIP_SENDER_MAIN, 2);
		pPlayer->ADD_GOSSIP_ITEM(0, "Quitter", GOSSIP_SENDER_MAIN, 40);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
		return true;
	}

	void AddRewardToPlr(uint32 idToAdd, uint32 idToDel, uint32 cnt, Player *plr, Creature *crea, const char* msg)
	{
		crea->MonsterWhisper(msg, plr->GetGUID());
		plr->DestroyItemCount(idToDel, cnt, true);
		plr->ModifyHonorPoints(idToAdd);
	}

	void ShowErrorDueToTrans(Player *plr, Creature *crea, const char* error_val)
	{
		crea->MonsterWhisper(error_val, plr->GetGUID());
		plr->CLOSE_GOSSIP_MENU();
	}

	void SendDefaultMenu(Player *pPlayer, Creature *_Creature, uint32 action )
	{
		switch(action)
		{
		case 2: 
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 1 > marque de Goulet contre < 50 > points d'honneur" , GOSSIP_SENDER_MAIN, 6);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 2 > marques de Goulet contre < 100 > points d'honneur" , GOSSIP_SENDER_MAIN, 7);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 3 > marques de Goulet contre < 150 > points d'honneur" , GOSSIP_SENDER_MAIN, 8);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 5 > marques de Goulet contre < 250 > points d'honneur" , GOSSIP_SENDER_MAIN, 9);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 10 > marques de Goulet contre < 500 > points d'honneur" , GOSSIP_SENDER_MAIN, 10);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 20 > marques de Goulet contre < 1000 > points d'honneur" , GOSSIP_SENDER_MAIN, 11);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 50 > marques de Goulet contre < 2500 > points d'honneur" , GOSSIP_SENDER_MAIN, 12);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 100 > marques de Goulet contre < 5000 > points d'honneur" , GOSSIP_SENDER_MAIN, 13);
				pPlayer->ADD_GOSSIP_ITEM(0, "Quitter", GOSSIP_SENDER_MAIN, 40);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
			break;

		case 6: // 1 goul vs honnor
			if(!pPlayer->HasItemCount(EM_GOULET,1)) ShowErrorDueToTrans(pPlayer, _Creature, ERROR_GOULET);
			else AddRewardToPlr(50, EM_GOULET, 1, pPlayer, _Creature, "Vous avez recu 50 points d'honneur.");
			break;

		case 7: // 
			if(!pPlayer->HasItemCount(EM_GOULET,2)) ShowErrorDueToTrans(pPlayer, _Creature, ERROR_GOULET);
			else AddRewardToPlr(100, EM_GOULET, 2, pPlayer, _Creature, "Vous avez recu 100 points d'honneur.");
			break;

		case 8: 
			if(!pPlayer->HasItemCount(EM_GOULET,3)) ShowErrorDueToTrans(pPlayer, _Creature, ERROR_GOULET);
			else AddRewardToPlr(150, EM_GOULET, 3, pPlayer, _Creature, "Vous avez recu 150 points d'honneur.");
			break;

		case 9: 
			if(!pPlayer->HasItemCount(EM_GOULET,5)) ShowErrorDueToTrans(pPlayer, _Creature, ERROR_GOULET);
			else AddRewardToPlr(250, EM_GOULET, 5, pPlayer, _Creature, "Vous avez recu 250 points d'honneur.");
			break;

		case 10: 
			if(!pPlayer->HasItemCount(EM_GOULET,10)) ShowErrorDueToTrans(pPlayer, _Creature, ERROR_GOULET);
			else AddRewardToPlr(500, EM_GOULET, 10, pPlayer, _Creature, "Vous avez recu 500 points d'honneur.");
			break;

		case 11: 
			if(!pPlayer->HasItemCount(EM_GOULET,20)) ShowErrorDueToTrans(pPlayer, _Creature, ERROR_GOULET);
			else AddRewardToPlr(1000, EM_GOULET, 20, pPlayer, _Creature, "Vous avez recu 1000 points d'honneur.");
			break;

		case 12:
			if(!pPlayer->HasItemCount(EM_GOULET,50)) ShowErrorDueToTrans(pPlayer, _Creature, ERROR_GOULET);
			else AddRewardToPlr(2500, EM_GOULET, 50, pPlayer, _Creature, "Vous avez recu 2500 points d'honneur.");
			break;

		case 13: 
			if(!pPlayer->HasItemCount(EM_GOULET,100)) ShowErrorDueToTrans(pPlayer, _Creature, ERROR_GOULET);
			else AddRewardToPlr(5000, EM_GOULET, 100, pPlayer, _Creature, "Vous avez recu 5000 points d'honneur.");
			break;

		case 40:
			pPlayer->CLOSE_GOSSIP_MENU();
			break;
		}
	}

	bool OnGossipSelect(Player *pPlayer, Creature *_Creature, uint32 sender, uint32 action )
	{
		pPlayer->PlayerTalkClass->ClearMenus();

		// Main menu
		if (sender == GOSSIP_SENDER_MAIN) SendDefaultMenu(pPlayer, _Creature, action   );
		if(action > 5 && action != 40) pPlayer->CLOSE_GOSSIP_MENU();

		return true;
	}
};

void AddSC_npc_marquegoulet()
{
	new npc_marquegoulet();
}


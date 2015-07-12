#include "ScriptPCH.h"

#define em_conquette	45624
#define em_vaillance 40753
#define em_heroisme 40752
#define em_triomphe 47241
#define em_givre 49426

#define error_conquette "У вас недостаточно Эмблем Завоевания"
#define error_vaillance "У вас недостаточно Эмблем Доблести"
#define error_givre "У вас недостаточно Эмблем Льда"
#define error_triomphe "У вас недостаточно Эмблем Триумфа"


class npc_rapidchange : public CreatureScript
{
public:
	npc_rapidchange() : CreatureScript("npc_rapidchange") { }

	bool OnGossipHello(Player * pPlayer, Creature * _Creature)
	{
		pPlayer->ADD_GOSSIP_ITEM(6, "Обмен Эмблем Триумфа на Эмблемы Завоевания" , GOSSIP_SENDER_MAIN, 2);
		pPlayer->ADD_GOSSIP_ITEM(6, "Обмен Эмблем Завоевания на Эмблемы Доблести" , GOSSIP_SENDER_MAIN, 3);
		pPlayer->ADD_GOSSIP_ITEM(6, "Обмен Эмблем Доблести на Эмблемы Героизма" , GOSSIP_SENDER_MAIN, 4);
		pPlayer->ADD_GOSSIP_ITEM(6, "Обмен Эмблем Льда на Эмблемы Триумфа" , GOSSIP_SENDER_MAIN, 5);
		pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 40);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
		return true;
	}

	void AddRewardToPlr(uint32 idToAdd, uint32 idToDel, uint32 cnt, Player *plr, Creature *crea, const char* msg)
	{
		plr->DestroyItemCount(idToDel, cnt, true);
		plr->AddItem(idToAdd,cnt);
		
		crea->MonsterWhisper(msg, plr->GetGUID());
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
		case 2: // embleme conq vs triomphe
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 1 > Эмблему Триумфа на < 1 > Эмблему Завоевания" , GOSSIP_SENDER_MAIN, 6);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 2 > Эмблемы Триумфа на < 2 > Эмблемы Завоевания" , GOSSIP_SENDER_MAIN, 7);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 3 > Эмблемы Триумфа на < 3 > Эмблемы Завоевания" , GOSSIP_SENDER_MAIN, 8);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 5 > Эмблем Триумфа на < 5 > Эмблем Завоевания" , GOSSIP_SENDER_MAIN, 9);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 10 > Эмблем Триумфа на < 10 > Эмблем Завоевания" , GOSSIP_SENDER_MAIN, 10);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 20 > Эмблем Триумфа на < 20 > Эмблем Завоевания" , GOSSIP_SENDER_MAIN, 11);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 50 > Эмблем Триумфа на < 50 > Эмблем Завоевания" , GOSSIP_SENDER_MAIN, 12);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 100 > Эмблем Триумфа на < 100 > Эмблем Завоевания" , GOSSIP_SENDER_MAIN, 13);
				pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 40);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
			}
			break;
		case 3: // vaillance vs conq
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 1 > Эмблему Завоевания на < 1 > Эмблему Доблести" , GOSSIP_SENDER_MAIN, 14);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 2 > Эмблемы Завоевания на < 2 > Эмблемы Доблести" , GOSSIP_SENDER_MAIN, 15);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 3 > Эмблемы Завоевания на < 3 > Эмблемы Доблести" , GOSSIP_SENDER_MAIN, 16);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 5 > Эмблем Завоевания на < 5 > Эмблем Доблести" , GOSSIP_SENDER_MAIN, 17);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 10 > Эмблем Завоевания на < 10 > Эмблем Доблести" , GOSSIP_SENDER_MAIN, 18);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 20 > Эмблем Завоевания на < 20 > Эмблем Доблести" , GOSSIP_SENDER_MAIN, 19);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 50 > Эмблем Завоевания на < 50 > Эмблем Доблести" , GOSSIP_SENDER_MAIN, 20);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 100 > Эмблем Завоевания на < 100 > Эмблем Доблести" , GOSSIP_SENDER_MAIN, 21);
				pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 40);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());			
			}
			break;
		case 4: // hero vs vaillance
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 1 > Эмблему Доблести на < 1 > Эмблему Героизма" , GOSSIP_SENDER_MAIN, 22);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 2 > Эмблемы Доблести на < 2 > Эмблемы Героизма" , GOSSIP_SENDER_MAIN, 23);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 3 > Эмблемы Доблести на < 3 > Эмблемы Героизма" , GOSSIP_SENDER_MAIN, 24);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 5 > Эмблем Доблести на < 5 > Эмблем Героизма" , GOSSIP_SENDER_MAIN, 25);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 10 > Эмблем Доблести на < 10 > Эмблем Героизма" , GOSSIP_SENDER_MAIN, 26);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 20 > Эмблем Доблести на < 20 > Эмблем Героизма" , GOSSIP_SENDER_MAIN, 27);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 50 > Эмблем Доблести на < 50 > Эмблем Героизма" , GOSSIP_SENDER_MAIN, 28);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 100 > Эмблем Доблести на < 100 > Эмблем Героизма" , GOSSIP_SENDER_MAIN, 29);
				pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 40);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
			}
			break;
		case 5: // triomphe vs givre
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 1 > Эмблему Льда на < 1 > Эмблему Триумфа" , GOSSIP_SENDER_MAIN, 30);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 2 > Эмблемы Льда на < 2 > Эмблемы Триумфа" , GOSSIP_SENDER_MAIN, 31);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 3 > Эмблемы Льда на < 3 > Эмблемы Триумфа" , GOSSIP_SENDER_MAIN, 32);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 5 > Эмблем Льда на < 5 > Эмблем Триумфа" , GOSSIP_SENDER_MAIN, 33);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 10 > Эмблем Льда на < 10 > Эмблем Триумфа" , GOSSIP_SENDER_MAIN, 34);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 20 > Эмблем Льда на < 20 > Эмблем Триумфа" , GOSSIP_SENDER_MAIN, 35);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 50 > Эмблем Льда на < 50 > Эмблем Триумфа" , GOSSIP_SENDER_MAIN, 36);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 100 > Эмблем Льда на < 100 > Эмблем Триумфа" , GOSSIP_SENDER_MAIN, 37);
				pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 40);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());	
			}
			break;
		case 6: // 1 tri vs conq
			{
				if(!pPlayer->HasItemCount(em_triomphe,1))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 1, pPlayer, _Creature, "Вы приобрели 1 Эмблему Триумфа");

				//(uint32 idToAdd, uint32 idToDel, uint32 cnt, Player *plr, Creature *crea, const char* msg)
			}
			break;
		case 7: // 
			{
				if(!pPlayer->HasItemCount(em_triomphe,2))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 2, pPlayer, _Creature, "Вы приобрели 2 Эмблемы Завоевания");
			}
			break;
		case 8: 
			{
				if(!pPlayer->HasItemCount(em_triomphe,3))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 3, pPlayer, _Creature, "Вы приобрели 3 Эмблемы Завоевания");
			}
			break;
		case 9: 
			{
				if(!pPlayer->HasItemCount(em_triomphe,5))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 5, pPlayer, _Creature, "Вы приобрели 5 Эмблем Завоевания");
			}
			break;
		case 10: 
			{
				if(!pPlayer->HasItemCount(em_triomphe,10))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 10, pPlayer, _Creature, "Вы приобрели 10 Эмблем Завоевания");
				
			}
			break;
		case 11: 
			{
				if(!pPlayer->HasItemCount(em_triomphe,20))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 20, pPlayer, _Creature, "Вы приобрели 20 Эмблем Завоевания");

			}
			break;
		case 12:
			{
				if(!pPlayer->HasItemCount(em_triomphe,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 50, pPlayer, _Creature, "Вы приобрели 50 Эмблем Завоевания");
			}
			break;
		case 13: 
			{
				if(!pPlayer->HasItemCount(em_triomphe,100))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 100, pPlayer, _Creature, "Вы приобрели 100 Эмблем Завоевания");
			}
			break;
		case 14: // add vaillance vs conquette
			{
				if(!pPlayer->HasItemCount(em_conquette,1))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 1, pPlayer, _Creature, "Вы приобрели 1 embleme de vaillance");
			}
			break;
		case 15: 
			{
				if(!pPlayer->HasItemCount(em_conquette,2))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 2, pPlayer, _Creature, "Вы приобрели 2 Эмблемы Доблести");
			}
			break;
		case 16: 
			{
				if(!pPlayer->HasItemCount(em_conquette,3))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 3, pPlayer, _Creature, "Вы приобрели 3 Эмблемы Доблести");

			}
			break;
		case 17: 
			{
				if(!pPlayer->HasItemCount(em_conquette,5))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 5, pPlayer, _Creature, "Вы приобрели 5 Эмблем Доблести");
			}
			break;
		case 18: 
			{
				if(!pPlayer->HasItemCount(em_conquette,10))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 10, pPlayer, _Creature, "Вы приобрели 10 Эмблем Доблести");

			}
			break;
		case 19: 
			{
				if(!pPlayer->HasItemCount(em_conquette,20))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 20, pPlayer, _Creature, "Вы приобрели 20 Эмблем Доблести");

			}
			break;
		case 20: 
			{
				if(!pPlayer->HasItemCount(em_conquette,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 50, pPlayer, _Creature, "Вы приобрели 50 Эмблем Доблести");

			}
			break;
		case 21: 
			{
				if(!pPlayer->HasItemCount(em_conquette,100))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 100, pPlayer, _Creature, "Вы приобрели 100 Эмблем Доблести");

			}
			break;
		case 22: // add heroisme vs vaillance
			{
				if(!pPlayer->HasItemCount(em_vaillance,1))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 1, pPlayer, _Creature, "Вы приобрели 1 embleme d heroisme");

			}
			break;
		case 23: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,2))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 2, pPlayer, _Creature, "Вы приобрели 2 Эмблемы Героизма");

			}
			break;
		case 24: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,3))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 3, pPlayer, _Creature, "Вы приобрели 3 Эмблемы Героизма");

			}
			break;
		case 25: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,5))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 5, pPlayer, _Creature, "Вы приобрели 5 Эмблем Героизма");

			}
			break;
		case 26: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,10))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 10, pPlayer, _Creature, "Вы приобрели 10 Эмблем Героизма");

			}
			break;
		case 27: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,20))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 20, pPlayer, _Creature, "Вы приобрели 20 Эмблем Героизма");

			}
			break;
		case 28: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 50, pPlayer, _Creature, "Вы приобрели 50 Эмблем Героизма");

			}
			break;
		case 29: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,100))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 100, pPlayer, _Creature, "Вы приобрели 100 Эмблем Героизма");

			}
			break;
		case 30: // add triomphe на givre
			{
				if(!pPlayer->HasItemCount(em_givre,1))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 1, pPlayer, _Creature, "Вы приобрели 1 embleme de triomphe");

			}
			break;
		case 31:
			{
				if(!pPlayer->HasItemCount(em_givre,2))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 2, pPlayer, _Creature, "Вы приобрели 2 Эмблемы Триумфа");

			}
			break;
		case 32: 
			{
				if(!pPlayer->HasItemCount(em_givre,3))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 3, pPlayer, _Creature, "Вы приобрели 3 Эмблемы Триумфа");

			}
			break;
		case 33: 
			{
				if(!pPlayer->HasItemCount(em_givre,5))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 5, pPlayer, _Creature, "Вы приобрели 5 Эмблем Триумфа");

			}
			break;
		case 34: 
			{
				if(!pPlayer->HasItemCount(em_givre,10))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 10, pPlayer, _Creature, "Вы приобрели 10 Эмблем Триумфа");

			}
			break;
		case 35: 
			{
				if(!pPlayer->HasItemCount(em_givre,20))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 20, pPlayer, _Creature, "Вы приобрели 20 Эмблем Триумфа");

			}
			break;
		case 36: 
			{
				if(!pPlayer->HasItemCount(em_givre,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 50, pPlayer, _Creature, "Вы приобрели 50 Эмблем Триумфа");

			}
			break;
		case 37: 
			{
				if(!pPlayer->HasItemCount(em_givre,100))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 100, pPlayer, _Creature, "Вы приобрели 100 Эмблем Триумфа");

			}
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
		if (sender == GOSSIP_SENDER_MAIN)
			SendDefaultMenu(pPlayer, _Creature, action   );

		if(action > 5 && action != 40)
			pPlayer->CLOSE_GOSSIP_MENU();

		return true;

	}
};

void AddSC_npc_rapidchange()
{
	new npc_rapidchange();
}


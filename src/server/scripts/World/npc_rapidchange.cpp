#include "ScriptPCH.h"

#define em_conquette	45624
#define em_vaillance 40753
#define em_heroisme 40752
#define em_triomphe 47241
#define em_givre 49426

#define error_conquette "Vous n'avez pas assez d'emblèmes de Conquête"
#define error_vaillance "Vous n'avez pas assez d'emblèmes de Vaillance"
#define error_givre "Vous n'avez pas assez d'emblèmes de Givre"
#define error_triomphe "Vous n'avez pas assez d'emblèmes de Triomphe"


class npc_rapidchange : public CreatureScript
{
public:
	npc_rapidchange() : CreatureScript("npc_rapidchange") { }

	bool OnGossipHello(Player * pPlayer, Creature * _Creature)
	{
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger vos emblèmes de Triomphe contre des emblèmes de Conquête" , GOSSIP_SENDER_MAIN, 2);
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger vos emblèmes de Conquête contre des emblèmes de Vaillance" , GOSSIP_SENDER_MAIN, 3);
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger vos emblèmes de Vaillance contre des emblèmes d'Héroisme" , GOSSIP_SENDER_MAIN, 4);
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger vos emblèmes de Givre contre des emblèmes de Triomphe" , GOSSIP_SENDER_MAIN, 5);
		pPlayer->ADD_GOSSIP_ITEM(6, "Quitter", GOSSIP_SENDER_MAIN, 40);
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
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 1 > emblèmes de Triomphe contre < 1 > emblèmes de Conquête" , GOSSIP_SENDER_MAIN, 6);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 2 > emblèmes de Triomphe contre < 2 > emblèmes de Conquête" , GOSSIP_SENDER_MAIN, 7);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 3 > emblèmes de Triomphe contre < 3 > emblèmes de Conquête" , GOSSIP_SENDER_MAIN, 8);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 5 > emblèmes de Triomphe contre < 5 > emblèmes de Conquête" , GOSSIP_SENDER_MAIN, 9);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 10 > emblèmes de Triomphe contre < 10 > emblèmes de Conquête" , GOSSIP_SENDER_MAIN, 10);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 20 > emblèmes de Triomphe contre < 20 > emblèmes de Conquête" , GOSSIP_SENDER_MAIN, 11);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 50 > emblèmes de Triomphe contre < 50 > emblèmes de Conquête" , GOSSIP_SENDER_MAIN, 12);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 100 > emblèmes de Triomphe contre < 100 > emblèmes de Conquête" , GOSSIP_SENDER_MAIN, 13);
				pPlayer->ADD_GOSSIP_ITEM(6, "Quitter", GOSSIP_SENDER_MAIN, 40);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
			}
			break;
		case 3: // vaillance vs conq
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 1 > emblèmes de Conquête contre < 1 > emblèmes de Vaillance" , GOSSIP_SENDER_MAIN, 14);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 2 > emblèmes de Conquête contre < 2 > emblèmes de Vaillance" , GOSSIP_SENDER_MAIN, 15);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 3 > emblèmes de Conquête contre < 3 > emblèmes de Vaillance" , GOSSIP_SENDER_MAIN, 16);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 5 > emblèmes de Conquête contre < 5 > emblèmes de vaillance" , GOSSIP_SENDER_MAIN, 17);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 10 > emblèmes de Conquête contre < 10 > emblèmes de Vaillance" , GOSSIP_SENDER_MAIN, 18);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 20 > emblèmes de Conquête contre < 20 > emblèmes de Vaillance" , GOSSIP_SENDER_MAIN, 19);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 50 > emblèmes de Conquête contre < 50 > emblèmes de Vaillance" , GOSSIP_SENDER_MAIN, 20);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 100 > emblèmes de Conquête contre < 100 > emblèmes de Vaillance" , GOSSIP_SENDER_MAIN, 21);
				pPlayer->ADD_GOSSIP_ITEM(6, "Quitter", GOSSIP_SENDER_MAIN, 40);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());			
			}
			break;
		case 4: // hero vs vaillance
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 1 > emblèmes de Vaillance contre < 1 > emblèmes d'Héroisme" , GOSSIP_SENDER_MAIN, 22);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 2 > emblèmes de Vaillance contre < 2 > emblèmes d'Héroisme" , GOSSIP_SENDER_MAIN, 23);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 3 > emblèmes de Vaillance contre < 3 > emblèmes d'Héroisme" , GOSSIP_SENDER_MAIN, 24);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 5 > emblèmes de Vaillance contre < 5 > emblèmes d'Héroisme" , GOSSIP_SENDER_MAIN, 25);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 10 > emblèmes de Vaillance contre < 10 > emblèmes d'Héroisme" , GOSSIP_SENDER_MAIN, 26);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 20 > emblèmes de Vaillance contre < 20 > emblèmes d'Héroisme" , GOSSIP_SENDER_MAIN, 27);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 50 > emblèmes de Vaillance contre < 50 > emblèmes d'Héroisme" , GOSSIP_SENDER_MAIN, 28);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 100 > emblèmes de Vaillance contre < 100 > emblèmes d'Héroisme" , GOSSIP_SENDER_MAIN, 29);
				pPlayer->ADD_GOSSIP_ITEM(6, "Quitter", GOSSIP_SENDER_MAIN, 40);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
			}
			break;
		case 5: // triomphe vs givre
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 1 > emblèmes de Givre contre < 1 > emblèmes de Triomphe" , GOSSIP_SENDER_MAIN, 30);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 2 > emblèmes de Givre contre < 2 > emblèmes de Triomphe" , GOSSIP_SENDER_MAIN, 31);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 3 > emblèmes de Givre contre < 3 > emblèmes de Triomphe" , GOSSIP_SENDER_MAIN, 32);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 5 > emblèmes de Givre contre < 5 > emblèmes de Triomphe" , GOSSIP_SENDER_MAIN, 33);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 10 > emblèmes de Givre contre < 10 > emblèmes de Triomphe" , GOSSIP_SENDER_MAIN, 34);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 20 > emblèmes de Givre contre < 20 > emblèmes de Triomphe" , GOSSIP_SENDER_MAIN, 35);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 50 > emblèmes de Givre contre < 50 > emblèmes de Triomphe" , GOSSIP_SENDER_MAIN, 36);
				pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 100 > emblèmes de Givre contre < 100 > emblèmes de Triomphe" , GOSSIP_SENDER_MAIN, 37);
				pPlayer->ADD_GOSSIP_ITEM(6, "Quitter", GOSSIP_SENDER_MAIN, 40);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());	
			}
			break;
		case 6: // 1 tri vs conq
			{
				if(!pPlayer->HasItemCount(em_triomphe,1))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 1, pPlayer, _Creature, "Vous venez d'acquérir 1 emblème de Conquête");

				//(uint32 idToAdd, uint32 idToDel, uint32 cnt, Player *plr, Creature *crea, const char* msg)
			}
			break;
		case 7: // 
			{
				if(!pPlayer->HasItemCount(em_triomphe,2))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 2, pPlayer, _Creature, "Vous venez d'acquérir 2 emblèmes de Conquête");
			}
			break;
		case 8: 
			{
				if(!pPlayer->HasItemCount(em_triomphe,3))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 3, pPlayer, _Creature, "Vous venez d'acquérir 3 emblèmes de Conquête");
			}
			break;
		case 9: 
			{
				if(!pPlayer->HasItemCount(em_triomphe,5))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 5, pPlayer, _Creature, "Vous venez d'acquérir 5 emblèmes de Conquête");
			}
			break;
		case 10: 
			{
				if(!pPlayer->HasItemCount(em_triomphe,10))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 10, pPlayer, _Creature, "Vous venez d'acquerir 10 emblèmes de Conquête");
				
			}
			break;
		case 11: 
			{
				if(!pPlayer->HasItemCount(em_triomphe,20))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 20, pPlayer, _Creature, "Vous venez d'acquérir 20 emblèmes de Conquête");

			}
			break;
		case 12:
			{
				if(!pPlayer->HasItemCount(em_triomphe,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 50, pPlayer, _Creature, "Vous venez d'acquérir 50 emblèmes de Conquête");
			}
			break;
		case 13: 
			{
				if(!pPlayer->HasItemCount(em_triomphe,100))
					ShowErrorDueToTrans(pPlayer, _Creature, error_triomphe);
				else
					AddRewardToPlr(em_conquette, em_triomphe, 100, pPlayer, _Creature, "Vous venez d'acquérir 100 emblèmes de Conquête");
			}
			break;
		case 14: // add vaillance vs conquette
			{
				if(!pPlayer->HasItemCount(em_conquette,1))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 1, pPlayer, _Creature, "Vous venez d'acquerir 1 emblème de Vaillance");
			}
			break;
		case 15: 
			{
				if(!pPlayer->HasItemCount(em_conquette,2))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 2, pPlayer, _Creature, "Vous venez d'acquérir 2 emblèmes de Vaillance");
			}
			break;
		case 16: 
			{
				if(!pPlayer->HasItemCount(em_conquette,3))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 3, pPlayer, _Creature, "Vous venez d'acquérir 3 emblèmes de Vaillance");

			}
			break;
		case 17: 
			{
				if(!pPlayer->HasItemCount(em_conquette,5))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 5, pPlayer, _Creature, "Vous venez d'acquérir 5 emblèmes de Vaillance");
			}
			break;
		case 18: 
			{
				if(!pPlayer->HasItemCount(em_conquette,10))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 10, pPlayer, _Creature, "Vous venez d'acquérir 10 emblèmes de Vaillance");

			}
			break;
		case 19: 
			{
				if(!pPlayer->HasItemCount(em_conquette,20))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 20, pPlayer, _Creature, "Vous venez d'acquérir 20 emblèmes de Vaillance");

			}
			break;
		case 20: 
			{
				if(!pPlayer->HasItemCount(em_conquette,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 50, pPlayer, _Creature, "Vous venez d'acquérir 50 emblèmes de Vaillance");

			}
			break;
		case 21: 
			{
				if(!pPlayer->HasItemCount(em_conquette,100))
					ShowErrorDueToTrans(pPlayer, _Creature, error_conquette);
				else
					AddRewardToPlr(em_vaillance, em_conquette, 100, pPlayer, _Creature, "Vous venez d'acquérir 100 emblèmes de Vaillance");

			}
			break;
		case 22: // add heroisme vs vaillance
			{
				if(!pPlayer->HasItemCount(em_vaillance,1))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 1, pPlayer, _Creature, "Vous venez d'acquérir 1 emblème d'Héroisme");

			}
			break;
		case 23: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,2))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 2, pPlayer, _Creature, "Vous venez d'acquérir 2 emblèmes d'Héroisme");

			}
			break;
		case 24: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,3))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 3, pPlayer, _Creature, "Vous venez d'acquérir 3 emblèmes d'Héroisme");

			}
			break;
		case 25: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,5))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 5, pPlayer, _Creature, "Vous venez d'acquérir 5 emblèmes d'Héroisme");

			}
			break;
		case 26: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,10))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 10, pPlayer, _Creature, "Vous venez d'acquérir 10 emblèmes d'Héroisme");

			}
			break;
		case 27: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,20))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 20, pPlayer, _Creature, "Vous venez d'acquérir 20 emblèmes d'Héroisme");

			}
			break;
		case 28: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 50, pPlayer, _Creature, "Vous venez d'acquérir 50 emblèmes d'Héroisme");

			}
			break;
		case 29: 
			{
				if(!pPlayer->HasItemCount(em_vaillance,100))
					ShowErrorDueToTrans(pPlayer, _Creature, error_vaillance);
				else
					AddRewardToPlr(em_heroisme, em_vaillance, 100, pPlayer, _Creature, "Vous venez d'acquérir 100 emblèmes d'Héroisme");

			}
			break;
		case 30: // add triomphe contre givre
			{
				if(!pPlayer->HasItemCount(em_givre,1))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 1, pPlayer, _Creature, "Vous venez d'acquérir 1 emblème de Triomphe");

			}
			break;
		case 31:
			{
				if(!pPlayer->HasItemCount(em_givre,2))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 2, pPlayer, _Creature, "Vous venez d'acquérir 2 emblèmes de Triomphe");

			}
			break;
		case 32: 
			{
				if(!pPlayer->HasItemCount(em_givre,3))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 3, pPlayer, _Creature, "Vous venez d'acquérir 3 emblèmes de Triomphe");

			}
			break;
		case 33: 
			{
				if(!pPlayer->HasItemCount(em_givre,5))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 5, pPlayer, _Creature, "Vous venez d'acquérir 5 emblèmes de Triomphe");

			}
			break;
		case 34: 
			{
				if(!pPlayer->HasItemCount(em_givre,10))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 10, pPlayer, _Creature, "Vous venez d'acquérir 10 emblèmes de Triomphe");

			}
			break;
		case 35: 
			{
				if(!pPlayer->HasItemCount(em_givre,20))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 20, pPlayer, _Creature, "Vous venez d'acquérir 20 emblèmes de Triomphe");

			}
			break;
		case 36: 
			{
				if(!pPlayer->HasItemCount(em_givre,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 50, pPlayer, _Creature, "Vous venez d'acquérir 50 emblèmes de Triomphe");

			}
			break;
		case 37: 
			{
				if(!pPlayer->HasItemCount(em_givre,100))
					ShowErrorDueToTrans(pPlayer, _Creature, error_givre);
				else
					AddRewardToPlr(em_triomphe, em_givre, 100, pPlayer, _Creature, "Vous venez d'acquérir 100 emblèmes de Triomphe");

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


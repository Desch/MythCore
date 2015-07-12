#include "ScriptPCH.h"

/*******************************************************
 * /********* Add Custom ADDR ***************/
 /*******************************************************/
 
#define ALLONULL "Vous n'avez pas assez de point de votes."
#define ALLOTRUE "Vous avez reçu %u sceaux de champion".
#define TRANS_FAIL  "Echec de la transaction, veuillez contacter un administrateur pour régler votre problème"

#define sc_champ	44990

class npc_shop : public CreatureScript
{
public:
	npc_shop() : CreatureScript("npc_shop") { }

	bool OnGossipHello(Player* pPlayer, Creature* pCreature)
	{
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 10 > votes contre < 1 > Sceau de Champion" , GOSSIP_SENDER_MAIN, 2);
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 20 > votes contre < 2 > Sceaux de Champion" , GOSSIP_SENDER_MAIN, 3);
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 50 > votes contre < 5 > Sceaux de Champion" , GOSSIP_SENDER_MAIN, 4);
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 100 > votes contre < 10 > Sceaux de Champion" , GOSSIP_SENDER_MAIN, 5);
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 200 > votes contre < 20 > Sceaux de Champion" , GOSSIP_SENDER_MAIN, 6);
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 300 > votes contre < 30 > Sceaux de Champion" , GOSSIP_SENDER_MAIN, 7);
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 400 > votes contre < 40 > Sceaux de Champion" , GOSSIP_SENDER_MAIN, 8);
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 500 > votes contre < 50 > Sceaux de Champion" , GOSSIP_SENDER_MAIN, 9);
		pPlayer->ADD_GOSSIP_ITEM(6, "Echanger < 1500 > votes contre < 150 > Sceaux de Champion" , GOSSIP_SENDER_MAIN, 10);
		pPlayer->ADD_GOSSIP_ITEM(6, "Quitter", GOSSIP_SENDER_MAIN, 11);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
		return true;
	}

	void AddRewardToPlr(uint32 cnt, Player* pPlayer, Creature* pCreature, uint32 pointbefore)
	{
		char msg[255];
		pPlayer->AddItem(sc_champ,cnt);
		uint32 pointafter = pointbefore -(cnt*10);
		LoginDatabase.PExecute("UPDATE `vote_points` SET `vote_points` = %u WHERE `user_id` = %u", pointafter, pPlayer->GetSession()->GetAccountId());
		sprintf(msg,"Vous avez reçu %u sceaux de champion", cnt);
		pCreature->MonsterWhisper(msg, pPlayer->GetGUID());
		pPlayer->SaveToDB();
	}

	void ShowErrorDueToTrans(Player* pPlayer, Creature* pCreature, const char* error_val)
	{
		pCreature->MonsterWhisper(error_val, pPlayer->GetGUID());
		pPlayer->CLOSE_GOSSIP_MENU();
	}

	void SendDefaultMenu(Player* pPlayer, Creature *pCreature, uint32 action )
	{
		QueryResult result;
		
		uint32 count = 0;

		result = LoginDatabase.PQuery("SELECT `vote_points` FROM `vote_points` WHERE `user_id` = %u", pPlayer->GetSession()->GetAccountId());
	
		if (result)
		{
			Field *fields = result->Fetch();
			count = fields[0].GetUInt32();
			if (count < 10)
			{
				ShowErrorDueToTrans(pPlayer, pCreature, ALLONULL);
				return;
			}
		}
		else
		{
			ShowErrorDueToTrans(pPlayer, pCreature, TRANS_FAIL);
			return;
		}
		
		switch(action)
		{
		
			case 2: // 10
				{
					if(count < 10) ShowErrorDueToTrans(pPlayer, pCreature, ALLONULL);
					else AddRewardToPlr(1, pPlayer, pCreature,count);
				}
				break;

			case 3: // 20
				{
					if(count < 20) ShowErrorDueToTrans(pPlayer, pCreature, ALLONULL);
					else AddRewardToPlr(2, pPlayer, pCreature,count);
				}
				break;

			case 4: // 50
				{
					if(count < 50) ShowErrorDueToTrans(pPlayer, pCreature, ALLONULL);
					else AddRewardToPlr(5, pPlayer, pCreature,count);
				}
				break;

			case 5: // 100
				{
					if(count < 100) ShowErrorDueToTrans(pPlayer, pCreature, ALLONULL);
					else AddRewardToPlr(10, pPlayer, pCreature,count);
				}
				break;

			case 6: // 200
				{
					if(count < 200) ShowErrorDueToTrans(pPlayer, pCreature, ALLONULL);
					else AddRewardToPlr(20, pPlayer, pCreature,count);
				}
				break;

			case 7: // 300
				{
					if(count < 300) ShowErrorDueToTrans(pPlayer, pCreature, ALLONULL);
					else AddRewardToPlr(30, pPlayer, pCreature,count);
				}
				break;

			case 8: // 400
				{
					if(count < 400) ShowErrorDueToTrans(pPlayer, pCreature, ALLONULL);
					else AddRewardToPlr(40, pPlayer, pCreature,count);
				}
				break;

			case 9: // 500
				{
					if(count < 500) ShowErrorDueToTrans(pPlayer, pCreature, ALLONULL);
					else AddRewardToPlr(50, pPlayer, pCreature,count);
				}
				break;

			case 10: // 1500
				{
					if(count < 1500) ShowErrorDueToTrans(pPlayer, pCreature, ALLONULL);
					else AddRewardToPlr(150, pPlayer, pCreature,count);
				}
				break;

			case 11: // Quit :)
				{
					pPlayer->CLOSE_GOSSIP_MENU();
				}
				break;
		}
	}

	bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action )
	{
		if (sender == GOSSIP_SENDER_MAIN)
		{
			SendDefaultMenu(pPlayer, pCreature, action );
			pPlayer->CLOSE_GOSSIP_MENU();
		}
		return true;
	}
};

void AddSC_npc_shop()
{
	new npc_shop();
}
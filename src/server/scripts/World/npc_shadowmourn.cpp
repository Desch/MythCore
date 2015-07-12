#include "ScriptPCH.h"

#define TRANS_FAIL  "Echec de la transaction, veuillez contacter un administrateur pour régler votre problème"

class npc_shadowmourn : public CreatureScript
{
public:
	npc_shadowmourn() : CreatureScript("npc_shadowmourn") { }

	bool OnGossipHello(Player * pPlayer, Creature * pCreature)
	{
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Acquérir Deuillelombre en échange de 900 points de vote et 50 000 points d'honneur en ayant une côte d'arene de 1400", GOSSIP_SENDER_MAIN, 1);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Quitter", GOSSIP_SENDER_MAIN, 2);
		pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
		return true;
	}
	void AddRewardToPlr(uint32 cnt, Player* pPlayer, Creature* pCreature, uint32 pointbefore)
	{
		pPlayer->AddItem(49623,1);
		pPlayer->ModifyHonorPoints(-50000);
		LoginDatabase.PExecute("UPDATE `vote_points` SET `vote_points` = %u WHERE `user_id` = %u", (pointbefore - cnt), pPlayer->GetSession()->GetAccountId());
		pCreature->MonsterWhisper("Félicitation, vous recevez Deuillelombre !", pPlayer->GetGUID());
		pPlayer->SaveToDB();
	}
	
	void ShowErrorDueToTrans(Player* pPlayer, Creature* pCreature, const char* error_val)
	{
		pCreature->MonsterWhisper(error_val, pPlayer->GetGUID());
		pPlayer->CLOSE_GOSSIP_MENU();
	}
	
	void SendDefaultMenu(Player *pPlayer, Creature *pCreature, uint32 action )
	{
		uint32 count = 0;

		QueryResult result = LoginDatabase.PQuery("SELECT `vote_points` FROM `vote_points` WHERE `user_id` = %u", pPlayer->GetSession()->GetAccountId());
		
		if (result)
		{
			Field *fields = result->Fetch();
			count = fields[0].GetUInt32();
		}
		else
		{
			ShowErrorDueToTrans(pPlayer, pCreature, TRANS_FAIL);
			return;
		}
		
		if(action==1)
		{
			if((pPlayer->GetMaxPersonalArenaRatingRequirement(0) >= 1400) && (pPlayer->GetHonorPoints() >= 50000) && (count >= 900)) AddRewardToPlr(900, pPlayer, pCreature,count);
			else pCreature->MonsterWhisper("Vous ne remplissez pas les conditions nécéssaires pour acquérir cette arme.",pPlayer->GetGUID());
			pPlayer->CLOSE_GOSSIP_MENU();
		}
		else if (action == 2) pPlayer->CLOSE_GOSSIP_MENU();
	}			
	
	bool OnGossipSelect(Player *pPlayer, Creature *pCreature, uint32 sender, uint32 action )
	{
		// Main menu
		if (sender == GOSSIP_SENDER_MAIN) SendDefaultMenu(pPlayer, pCreature, action   );
		return true;
	}
};

void AddSC_npc_shadowmourn()
{
    new npc_shadowmourn();
}

#include "ScriptPCH.h"

#define em_monets 37711

#define error_em "У вас недостаточно Эмблем"
#define error_gold "У вас недостаточно Золота"
#define error_ap "У вас недостаточно Арена Поинтов"

class npc_emblem_vendor : public CreatureScript
{
public:
	npc_emblem_vendor() : CreatureScript("npc_emblem_vendor") { }

	bool OnGossipHello(Player * pPlayer, Creature * _Creature)
	{
		pPlayer->ADD_GOSSIP_ITEM(6, "Обменять золото" , GOSSIP_SENDER_MAIN, 2);
		pPlayer->ADD_GOSSIP_ITEM(6, "Обменять эмблемы льда" , GOSSIP_SENDER_MAIN, 3);
		pPlayer->ADD_GOSSIP_ITEM(6, "Обменять эмблемы триумфа" , GOSSIP_SENDER_MAIN, 4);
        pPlayer->ADD_GOSSIP_ITEM(6, "Обменять Арена Поинты" , GOSSIP_SENDER_MAIN, 5);
		pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 40);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
		return true;
	}

	void AddRewardToPlrled(uint32 idToAdd, uint32 idToDel, uint32 cnt, Player *plr, Creature *crea, const char* msg)
	{
		plr->DestroyItemCount(idToDel, cnt, true);
		plr->AddItem(idToAdd,cnt);
		
		crea->MonsterWhisper(msg, plr->GetGUID());
	}
    
    void AddRewardToPlrtri(uint32 idToAdd, uint32 idToDel, uint32 cnt, uint32 cnt2, Player *plr, Creature *crea, const char* msg)
	{
		plr->DestroyItemCount(idToDel, cnt, true);
		plr->AddItem(idToAdd, cnt2);
		
		crea->MonsterWhisper(msg, plr->GetGUID());
	}
    
    void AddRewardToPlrgold(uint32 idToAdd, uint32 cnt, uint32 goldToDel, Player *plr, Creature *crea, const char* msg)
	{
		plr->ModifyMoney(-(int32)goldToDel);
		plr->AddItem(idToAdd, cnt);
		
		crea->MonsterWhisper(msg, plr->GetGUID());
	}

    void AddRewardToPlrArena(uint32 idToAdd, uint32 cnt, uint32 ArenaToDel, Player *plr, Creature *crea, const char* msg)
	{
        plr->ModifyArenaPoints(-(int32)ArenaToDel);
		plr->AddItem(idToAdd, cnt);
		
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
		case 2: // Золото
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 1к > Золота на < 2 > Монеты" , GOSSIP_SENDER_MAIN, 6);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 2к > Золота на < 4 > Монеты" , GOSSIP_SENDER_MAIN, 7);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 3к > Золота на < 6 > Монет" , GOSSIP_SENDER_MAIN, 8);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 5к > Золота на < 10 > Монет" , GOSSIP_SENDER_MAIN, 9);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 10к > Золота на < 20 > Монет" , GOSSIP_SENDER_MAIN, 10);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 20к > Золота на < 40 > Монет" , GOSSIP_SENDER_MAIN, 11);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 30к > Золота на < 60 > Монет" , GOSSIP_SENDER_MAIN, 12);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 50к > Золота на < 100 > Монет" , GOSSIP_SENDER_MAIN, 13);
				pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 40);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
			}
			break;
		case 3: // Эмблемы льда
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 1 > Эмблему Льда на < 1 > Монету" , GOSSIP_SENDER_MAIN, 14);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 2 > Эмблемы Льда на < 2 > Монеты" , GOSSIP_SENDER_MAIN, 15);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 3 > Эмблемы Льда на < 3 > Монеты" , GOSSIP_SENDER_MAIN, 16);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 5 > Эмблем Льда на < 5 > Монет" , GOSSIP_SENDER_MAIN, 17);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 10 > Эмблем Льда на < 10 > Монет" , GOSSIP_SENDER_MAIN, 18);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 20 > Эмблем Льда на < 20 > Монет" , GOSSIP_SENDER_MAIN, 19);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 50 > Эмблем Льда на < 50 > Монет" , GOSSIP_SENDER_MAIN, 20);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 100 > Эмблем Льда на < 100 > Монет" , GOSSIP_SENDER_MAIN, 21);
				pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 40);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());			
			}
			break;
		case 4: // Эмблемы Триумфа
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 2 > Эмблему Триумфа на < 1 > Монету" , GOSSIP_SENDER_MAIN, 22);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 4 > Эмблему Триумфа на < 2 > Монеты" , GOSSIP_SENDER_MAIN, 23);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 10 > Эмблему Триумфа на < 5 > Монет" , GOSSIP_SENDER_MAIN, 24);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 20 > Эмблему Триумфа на < 10 > Монет" , GOSSIP_SENDER_MAIN, 25);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 30 > Эмблему Триумфа на < 15 > Монет" , GOSSIP_SENDER_MAIN, 26);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 40 > Эмблему Триумфа на < 20 > Монет" , GOSSIP_SENDER_MAIN, 27);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 50 > Эмблему Триумфа на < 25 > Монет" , GOSSIP_SENDER_MAIN, 28);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 60 > Эмблему Триумфа на < 30 > Монет" , GOSSIP_SENDER_MAIN, 29);
				pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 40);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
			}
			break;
		case 5: // Арена
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 15 > АП на < 1 > Монету" , GOSSIP_SENDER_MAIN, 30);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 75 > АП на < 5 > Монет" , GOSSIP_SENDER_MAIN, 31);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 150 > АП на < 10 > Монет" , GOSSIP_SENDER_MAIN, 32);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 375 > АП на < 25 > Монет" , GOSSIP_SENDER_MAIN, 33);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 750 > АП на < 50 > Монет" , GOSSIP_SENDER_MAIN, 34);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 1125 > АП на < 75 > Монет" , GOSSIP_SENDER_MAIN, 35);
				//pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 50 > Эмблем Льда на < 50 > Эмблем Триумфа" , GOSSIP_SENDER_MAIN, 36);
				//pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 100 > Эмблем Льда на < 100 > Эмблем Триумфа" , GOSSIP_SENDER_MAIN, 37);
				pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 40);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());	
			}
			break;
		case 6: // 1к Золото
			{
				if(!pPlayer->HasEnoughMoney(10000000))
					ShowErrorDueToTrans(pPlayer, _Creature, error_gold);
				else
					AddRewardToPlrgold(em_monets, 2, 10000000, pPlayer, _Creature, "Вы приобрели 2 Монеты");
			}
			break;
		case 7: // 2к Золото
			{
				if(!pPlayer->HasEnoughMoney(20000000))
					ShowErrorDueToTrans(pPlayer, _Creature, error_gold);
				else
					AddRewardToPlrgold(em_monets, 4, 20000000, pPlayer, _Creature, "Вы приобрели 4 Монеты");
			}
			break;
		case 8: // 3к Золото
			{
				if(!pPlayer->HasEnoughMoney(30000000))
					ShowErrorDueToTrans(pPlayer, _Creature, error_gold);
				else
					AddRewardToPlrgold(em_monets, 6, 30000000, pPlayer, _Creature, "Вы приобрели 6 Монет");
			}
			break;
		case 9: // 5к Золото
			{
				if(!pPlayer->HasEnoughMoney(50000000))
					ShowErrorDueToTrans(pPlayer, _Creature, error_gold);
				else
					AddRewardToPlrgold(em_monets, 10, 50000000, pPlayer, _Creature, "Вы приобрели 10 Монет");
			}
			break;
		case 10: // 10к Золото
			{
				if(!pPlayer->HasEnoughMoney(100000000))
					ShowErrorDueToTrans(pPlayer, _Creature, error_gold);
				else
					AddRewardToPlrgold(em_monets, 20, 100000000, pPlayer, _Creature, "Вы приобрели 20 Монет");
				
			}
			break;
		case 11: // 20к Золото
			{
				if(!pPlayer->HasEnoughMoney(200000000))
					ShowErrorDueToTrans(pPlayer, _Creature, error_gold);
				else
					AddRewardToPlrgold(em_monets, 40, 200000000, pPlayer, _Creature, "Вы приобрели 40 Монет");

			}
			break;
		case 12:// 30к Золото
			{
				if(!pPlayer->HasEnoughMoney(300000000))
					ShowErrorDueToTrans(pPlayer, _Creature, error_gold);
				else
					AddRewardToPlrgold(em_monets, 60, 600000000, pPlayer, _Creature, "Вы приобрели 60 Монет");
			}
			break;
		case 13: // 50к Золото
			{
				if(!pPlayer->HasEnoughMoney(500000000))
					ShowErrorDueToTrans(pPlayer, _Creature, error_gold);
				else
					AddRewardToPlrgold(em_monets, 100, 1000000000, pPlayer, _Creature, "Вы приобрели 100 Монет");
			}
			break;
		case 14: // 1 Эмблему Льда
			{
				if(!pPlayer->HasItemCount(49426,1))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrled(em_monets, 49426, 1, pPlayer, _Creature, "Вы приобрели 1 Монету");
			}
			break;
		case 15: // 2 Эмблему Льда
			{
				if(!pPlayer->HasItemCount(49426,2))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrled(em_monets, 49426, 2, pPlayer, _Creature, "Вы приобрели 2 Монеты");
			}
			break;
		case 16: // 3 Эмблему Льда
			{
				if(!pPlayer->HasItemCount(49426,3))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrled(em_monets, 49426, 3, pPlayer, _Creature, "Вы приобрели 3 Монеты");

			}
			break;
		case 17: // 5 Эмблему Льда
			{
				if(!pPlayer->HasItemCount(49426,5))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrled(em_monets, 49426, 5, pPlayer, _Creature, "Вы приобрели 5 Монет");
			}
			break;
		case 18: // 10 Эмблему Льда
			{
				if(!pPlayer->HasItemCount(49426,10))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrled(em_monets, 49426, 10, pPlayer, _Creature, "Вы приобрели 10 Монет");

			}
			break;
		case 19: // 20 Эмблему Льда
			{
				if(!pPlayer->HasItemCount(49426,20))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrled(em_monets, 49426, 20, pPlayer, _Creature, "Вы приобрели 20 Монет");

			}
			break;
		case 20: // 50 Эмблему Льда
			{
				if(!pPlayer->HasItemCount(49426,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrled(em_monets, 49426, 50, pPlayer, _Creature, "Вы приобрели 50 Монет");

			}
			break;
		case 21: // 100 Эмблему Льда
			{
				if(!pPlayer->HasItemCount(49426,100))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrled(em_monets, 49426, 100, pPlayer, _Creature, "Вы приобрели 100 Монет");

			}
			break;
		case 22: // 2 Эмблем Триумфа
			{
				if(!pPlayer->HasItemCount(47241,2))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrtri(em_monets, 47241, 2, 1, pPlayer, _Creature, "Вы приобрели 1 Монету");

			}
			break;
		case 23: // 4 Эмблем Триумфа
			{
				if(!pPlayer->HasItemCount(47241,4))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrtri(em_monets, 47241, 4, 2, pPlayer, _Creature, "Вы приобрели 2 Монеты");

			}
			break;
		case 24: // 10 Эмблем Триумфа
			{
				if(!pPlayer->HasItemCount(47241,10))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrtri(em_monets, 47241, 10, 5, pPlayer, _Creature, "Вы приобрели 5 Монет");

			}
			break;
		case 25: // 20 Эмблем Триумфа
			{
				if(!pPlayer->HasItemCount(47241,20))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrtri(em_monets, 47241, 20, 10, pPlayer, _Creature, "Вы приобрели 10 Монет");

			}
			break;
		case 26: // 30 Эмблем Триумфа
			{
				if(!pPlayer->HasItemCount(47241,30))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrtri(em_monets, 47241, 30, 15, pPlayer, _Creature, "Вы приобрели 15 Монет");

			}
			break;
		case 27: // 40 Эмблем Триумфа
			{
				if(!pPlayer->HasItemCount(47241,40))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrtri(em_monets, 47241, 40, 20, pPlayer, _Creature, "Вы приобрели 20 Монет");

			}
			break;
		case 28: // 50 Эмблем Триумфа
			{
				if(!pPlayer->HasItemCount(47241,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrtri(em_monets, 47241, 50, 25, pPlayer, _Creature, "Вы приобрели 25 Монет");

			}
			break;
		case 29: // 60 Эмблем Триумфа
			{
				if(!pPlayer->HasItemCount(47241,60))
					ShowErrorDueToTrans(pPlayer, _Creature, error_em);
				else
					AddRewardToPlrtri(em_monets, 47241, 60, 30, pPlayer, _Creature, "Вы приобрели 30 Монет");

			}
			break;
        case 30: // 15 АП
            {
                if(pPlayer->GetArenaPoints() >= 15)
                    AddRewardToPlrArena(em_monets, 1, 15, pPlayer, _Creature, "Вы приобрели 1 Монету");
                else
                    ShowErrorDueToTrans(pPlayer, _Creature, error_ap);
            }
            break;
        case 31: // 75 АП
            {
                if(pPlayer->GetArenaPoints() >= 75)
                    AddRewardToPlrArena(em_monets, 5, 75, pPlayer, _Creature, "Вы приобрели 5 Монет");
                else
                    ShowErrorDueToTrans(pPlayer, _Creature, error_ap);
            }
            break;
        case 32: // 150 АП
            {
                if(pPlayer->GetArenaPoints() >= 150)
                    AddRewardToPlrArena(em_monets, 10, 150, pPlayer, _Creature, "Вы приобрели 10 Монет");
                else
                    ShowErrorDueToTrans(pPlayer, _Creature, error_ap);
            }
            break;
        case 33: // 375 АП
            {
                if(pPlayer->GetArenaPoints() >= 375)
                    AddRewardToPlrArena(em_monets, 25, 375, pPlayer, _Creature, "Вы приобрели 25 Монет");
                else
                    ShowErrorDueToTrans(pPlayer, _Creature, error_ap);
            }
            break;
        case 34: // 750 АП
            {
                if(pPlayer->GetArenaPoints() >= 750)
                    AddRewardToPlrArena(em_monets, 50, 750, pPlayer, _Creature, "Вы приобрели 50 Монет");
                else
                    ShowErrorDueToTrans(pPlayer, _Creature, error_ap);
            }
            break;
        case 35: // 1125 АП
            {
                if(pPlayer->GetArenaPoints() >= 1125)
                    AddRewardToPlrArena(em_monets, 75, 1125, pPlayer, _Creature, "Вы приобрели 75 Монет");
                else
                    ShowErrorDueToTrans(pPlayer, _Creature, error_ap);
            }
            break;
		case 40:
			pPlayer->CLOSE_GOSSIP_MENU();
			break;
        default:
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
void AddSC_npc_emblem_vendor()
{
    new npc_emblem_vendor();
}
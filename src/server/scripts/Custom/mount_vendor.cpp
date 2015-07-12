#include "ScriptPCH.h"

#define em_monets 37711

#define error_monets "У вас недостаточно Монет"

class npc_mounts_vendor : public CreatureScript
{
public:
	npc_mounts_vendor() : CreatureScript("npc_mounts_vendor") { }

	bool OnGossipHello(Player * pPlayer, Creature * _Creature)
	{
		pPlayer->ADD_GOSSIP_ITEM(6, "Маунты" , GOSSIP_SENDER_MAIN, 2);
		pPlayer->ADD_GOSSIP_ITEM(6, "Питомцы" , GOSSIP_SENDER_MAIN, 3);
		pPlayer->ADD_GOSSIP_ITEM(6, "Эликсиры и другие пряности" , GOSSIP_SENDER_MAIN, 4);
		pPlayer->ADD_GOSSIP_ITEM(6, "Накидки" , GOSSIP_SENDER_MAIN, 5);
		pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 60);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
		return true;
	}

	void AddRewardToPlr(uint32 idToAdd, uint32 idToDel, uint32 cnt, Player *plr, Creature *crea, const char* msg)
	{
        const char* error_msg="У вас нет свободного места";
        ItemPosCountVec dest;
        uint8 canstore = plr->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, idToAdd, 1);
        if(canstore == EQUIP_ERR_OK)
        {
            plr->DestroyItemCount(idToDel, cnt, true);
            plr->AddItem(idToAdd,1);
            
            crea->MonsterWhisper(msg, plr->GetGUID());
        }
        else
            crea->MonsterWhisper(error_msg, plr->GetGUID());
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
		case 2: // Маунты
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 30 > на Верховая черепаха" , GOSSIP_SENDER_MAIN, 6);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 50 > на Стремительная жевра" , GOSSIP_SENDER_MAIN, 7);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 100 > на Аманийский боевой медведь" , GOSSIP_SENDER_MAIN, 8);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 100 > на ИКС-ключительная ракета Пустоты X-51" , GOSSIP_SENDER_MAIN, 9);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 150 > на Белый шерстистый носорог" , GOSSIP_SENDER_MAIN, 10);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 250 > на Поводья черного протодракона" , GOSSIP_SENDER_MAIN, 11);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 250 > на Поводья чумного протодракона" , GOSSIP_SENDER_MAIN, 12);
                pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 300 > на Большой медведь Blizzard" , GOSSIP_SENDER_MAIN, 42);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 400 > на Прогулочная ракета X-53" , GOSSIP_SENDER_MAIN, 13);
                pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 500 > на Поводья резвого призрачного тигра" , GOSSIP_SENDER_MAIN, 38);
				pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 60);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
			}
			break;
		case 3: // Питомцы
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 10 > на Призрачный тигренок" , GOSSIP_SENDER_MAIN, 14);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 10 > на Турбоцыпленок" , GOSSIP_SENDER_MAIN, 15);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 10 > на Маяк продавца душ" , GOSSIP_SENDER_MAIN, 16);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 15 > на Банановый оберег" , GOSSIP_SENDER_MAIN, 17);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 25 > на Детеныш ветрокрыла" , GOSSIP_SENDER_MAIN, 18);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 25 > на Птенец грифона" , GOSSIP_SENDER_MAIN, 19);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 50 > на Клыкаррский воздушный змей" , GOSSIP_SENDER_MAIN, 20);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 50 > на Воздушный змеедрак" , GOSSIP_SENDER_MAIN, 21);
                pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 50 > на Талисманчик" , GOSSIP_SENDER_MAIN, 39);
                pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 45 > на Синее яйцо мурлока" , GOSSIP_SENDER_MAIN, 40);
                pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 45 > на Розовое яйцо мурлока" , GOSSIP_SENDER_MAIN, 41);
				pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 60);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());			
			}
			break;
		case 4: // Эликсиры
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 1 > на Путь Кенария" , GOSSIP_SENDER_MAIN, 22);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 1 > на Путь Иллидана" , GOSSIP_SENDER_MAIN, 23);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 2 > на Старомодное лакомство для питомцев от Папаши Хюммеля" , GOSSIP_SENDER_MAIN, 24);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 2 > на П.Е.Т.А.Р.Д.А. для вечеринки" , GOSSIP_SENDER_MAIN, 25);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 3 > на Тигр-качалка" , GOSSIP_SENDER_MAIN, 26);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 20 > на Превосходная лиловая рубашка" , GOSSIP_SENDER_MAIN, 27);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 25 > на Резной огрский идол" , GOSSIP_SENDER_MAIN, 28);
                //pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 30 > на Резной огрский идол" , GOSSIP_SENDER_MAIN, 29);
				pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 60);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
			}
			break;
		case 5: // Разное
			{
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 15 > на Гербовая накидка гениальности" , GOSSIP_SENDER_MAIN, 30);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 15 > на Гербовая накидка Льда" , GOSSIP_SENDER_MAIN, 31);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 15 > на Гербовая накидка Гнева" , GOSSIP_SENDER_MAIN, 32);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 15 > на Гербовая накидка Природы" , GOSSIP_SENDER_MAIN, 33);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 15 > на Гербовая накидка Тайной магии" , GOSSIP_SENDER_MAIN, 34);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 15 > на Гербовая накидка Защитника" , GOSSIP_SENDER_MAIN, 35);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 15 > на Гербовая накидка Бездны" , GOSSIP_SENDER_MAIN, 36);
				pPlayer->ADD_GOSSIP_ITEM(6, "Обменять < 15 > на Гербовая накидка Пламени" , GOSSIP_SENDER_MAIN, 37);
				pPlayer->ADD_GOSSIP_ITEM(6, "Выход", GOSSIP_SENDER_MAIN, 60);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());	
			}
			break;
		case 6: // 1 Маунт
			{
				if(!pPlayer->HasItemCount(em_monets,30))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(23720, em_monets, 30, pPlayer, _Creature, "Вы приобрели Верховая черепаха");

				//(uint32 idToAdd, uint32 idToDel, uint32 cnt, Player *plr, Creature *crea, const char* msg)
			}
			break;
		case 7: // 2 Маунт
			{
				if(!pPlayer->HasItemCount(em_monets,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(37719, em_monets, 50, pPlayer, _Creature, "Вы приобрели Стремительная жевра");
			}
			break;
		case 8: // 3 Маунт
			{
				if(!pPlayer->HasItemCount(em_monets,100))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(33809, em_monets, 100, pPlayer, _Creature, "Вы приобрели Аманийский боевой медведь");
			}
			break;
		case 9: // 4 Маунт
			{
				if(!pPlayer->HasItemCount(em_monets,100))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(49286, em_monets, 100, pPlayer, _Creature, "Вы приобрели ИКС-ключительная ракета Пустоты X-51");
			}
			break;
		case 10: // 5 Маунт
			{
				if(!pPlayer->HasItemCount(em_monets,150))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(54068, em_monets, 150, pPlayer, _Creature, "Вы приобрели Белый шерстистый носорог");
				
			}
			break;
		case 11: // 6 Маунт
			{
				if(!pPlayer->HasItemCount(em_monets,250))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(44164, em_monets, 250, pPlayer, _Creature, "Вы приобрели Поводья черного протодракона");

			}
			break;
		case 12:// 7 Маунт
			{
				if(!pPlayer->HasItemCount(em_monets,250))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(44175, em_monets, 250, pPlayer, _Creature, "Вы приобрели Поводья чумного протодракона");
			}
			break;
		case 13: // 8 Маунт
			{
				if(!pPlayer->HasItemCount(em_monets,400))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(54860, em_monets, 400, pPlayer, _Creature, "Вы приобрели Прогулочная ракета X-53");
			}
			break;
		case 14: // 1 Питомец
			{
				if(!pPlayer->HasItemCount(em_monets,10))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(49343, em_monets, 10, pPlayer, _Creature, "Вы приобрели Призрачный тигренок");
			}
			break;
		case 15: // 2 Питомец
			{
				if(!pPlayer->HasItemCount(em_monets,10))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(34492, em_monets, 10, pPlayer, _Creature, "Вы приобрели Турбоцыпленок");
			}
			break;
		case 16: // 3 Питомец
			{
				if(!pPlayer->HasItemCount(em_monets,10))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(38050, em_monets, 10, pPlayer, _Creature, "Вы приобрели Маяк продавца душ");

			}
			break;
		case 17: // 4 Питомец
			{
				if(!pPlayer->HasItemCount(em_monets,15))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(32588, em_monets, 15, pPlayer, _Creature, "Вы приобрели Банановый оберег");
			}
			break;
		case 18: // 5 Питомец
			{
				if(!pPlayer->HasItemCount(em_monets,25))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(49663, em_monets, 25, pPlayer, _Creature, "Вы приобрели Детеныш ветрокрыла");

			}
			break;
		case 19: // 6 Питомец
			{
				if(!pPlayer->HasItemCount(em_monets,25))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(49662, em_monets, 25, pPlayer, _Creature, "Вы приобрели Птенец грифона");

			}
			break;
		case 20: // 7 Питомец
			{
				if(!pPlayer->HasItemCount(em_monets,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(49287, em_monets, 50, pPlayer, _Creature, "Вы приобрели Клыкаррский воздушный змей");

			}
			break;
		case 21: // 8 Питомец
			{
				if(!pPlayer->HasItemCount(em_monets,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(34493, em_monets, 50, pPlayer, _Creature, "Вы приобрели Воздушный змеедрак");

			}
			break;
		case 22: // 1 Эликсир
			{
				if(!pPlayer->HasItemCount(em_monets,1))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(46779, em_monets, 1, pPlayer, _Creature, "Вы приобрели Путь Кенария");

			}
			break;
		case 23: // 2 Эликсир
			{
				if(!pPlayer->HasItemCount(em_monets,1))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(38233, em_monets, 1, pPlayer, _Creature, "Вы приобрели Путь Иллидана");

			}
			break;
		case 24: // 3 Эликсир
			{
				if(!pPlayer->HasItemCount(em_monets,2))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(35223, em_monets, 2, pPlayer, _Creature, "Вы приобрели Старомодное лакомство для питомцев");

			}
			break;
		case 25: // 4 Эликсир
			{
				if(!pPlayer->HasItemCount(em_monets,2))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(38577, em_monets, 2, pPlayer, _Creature, "Вы приобрели П.Е.Т.А.Р.Д.А. для вечеринки");

			}
			break;
		case 26: // 5 Эликсир
			{
				if(!pPlayer->HasItemCount(em_monets,3))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(45047, em_monets, 3, pPlayer, _Creature, "Вы приобрели Тигр-качалка");

			}
			break;
		case 27: // 6 Эликсир
			{
				if(!pPlayer->HasItemCount(em_monets,20))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(45037, em_monets, 20, pPlayer, _Creature, "Вы приобрели Превосходная лиловая рубашка");

			}
			break;
		case 28: // 7 Эликсир
			{
				if(!pPlayer->HasItemCount(em_monets,25))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(49704, em_monets, 25, pPlayer, _Creature, "Вы приобрели Резной огрский идол");

			}
			break;
		/*case 29: // 8 Эликсир
			{
				if(!pPlayer->HasItemCount(em_monets,100))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(id, em_monets, 100, pPlayer, _Creature, "Вы приобрели 100 Эмблем Героизма");

			}
			break;*/
		case 30: // 1 разное
			{
				if(!pPlayer->HasItemCount(em_monets,15))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(38312, em_monets, 15, pPlayer, _Creature, "Вы приобрели Гербовая накидка гениальности");

			}
			break;
		case 31: // 2 разное
			{
				if(!pPlayer->HasItemCount(em_monets,15))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(23709, em_monets, 15, pPlayer, _Creature, "Вы приобрели Гербовая накидка Льда");

			}
			break;
		case 32: // 3 разное
			{
				if(!pPlayer->HasItemCount(em_monets,15))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(38313, em_monets, 15, pPlayer, _Creature, "Вы приобрели Гербовая накидка Гнева");

			}
			break;
		case 33: // 4 разное
			{
				if(!pPlayer->HasItemCount(em_monets,15))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(38309, em_monets, 15, pPlayer, _Creature, "Вы приобрели Гербовая накидка Природы");

			}
			break;
		case 34: // 5 разное
			{
				if(!pPlayer->HasItemCount(em_monets,15))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(38310, em_monets, 15, pPlayer, _Creature, "Вы приобрели Гербовая накидка Тайной магии");

			}
			break;
		case 35: // 6 разное
			{
				if(!pPlayer->HasItemCount(em_monets,15))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(38314, em_monets, 15, pPlayer, _Creature, "Вы приобрели Гербовая накидка Защитника");

			}
			break;
		case 36: // 7 разное
			{
				if(!pPlayer->HasItemCount(em_monets,15))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(38311, em_monets, 15, pPlayer, _Creature, "Вы приобрели Гербовая накидка Бездны");

			}
			break;
		case 37: // 8 разное
			{
				if(!pPlayer->HasItemCount(em_monets,15))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(23705, em_monets, 15, pPlayer, _Creature, "Вы приобрели Гербовая накидка Пламени");

			}
			break;
        case 38: // 9 маунт
			{
				if(!pPlayer->HasItemCount(em_monets,500))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(49284, em_monets, 500, pPlayer, _Creature, "Вы приобрели Поводья резвого призрачного тигра");

			}
			break;
        case 39: // 9 питомец
			{
				if(!pPlayer->HasItemCount(em_monets,50))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(49693, em_monets, 50, pPlayer, _Creature, "Вы приобрели Талисманчик");

			}
			break;
        case 40: // 10 питомец
			{
				if(!pPlayer->HasItemCount(em_monets,45))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(20371, em_monets, 45, pPlayer, _Creature, "Вы приобрели Синее яйцо мурлока");

			}
			break;
        case 41: // 11 питомец
			{
				if(!pPlayer->HasItemCount(em_monets,45))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(22114, em_monets, 45, pPlayer, _Creature, "Вы приобрели Розовое яйцо мурлока");

			}
			break;
        case 42: // 10 маунт
			{
				if(!pPlayer->HasItemCount(em_monets,300))
					ShowErrorDueToTrans(pPlayer, _Creature, error_monets);
				else
					AddRewardToPlr(43599, em_monets, 300, pPlayer, _Creature, "Вы приобрели Большой медведь Blizzard");

			}
			break;
		case 60:
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

		if(action > 5 && action != 60)
			pPlayer->CLOSE_GOSSIP_MENU();

		return true;

	}
};

void AddSC_npc_mounts_vendor()
{
	new npc_mounts_vendor();
}
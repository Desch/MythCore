/*
Custom patches
*/

#include "ScriptPCH.h"
#include "ScriptMgr.h"

#define MSG_GOSSIP_TEXT_GETTING_STARTED	"Приветствуем вас на сервере Battleage!"

#define MSG_ERR_HONOR "У вас не достаточно хонора для совершения покупки!"
#define MSG_ERR_INCOMBAT "Вы находитесь в бою. Чтобы использовать данного Npc выйдите из него."

#define MSG_GOSSIP_TEXT_BUFF_POWER_WORD "[Бафнуть] Слово силы: Стойкость"
#define MSG_GOSSIP_TEXT_BUFF_BLESSING_OF_KINGS "[Бафнуть] Благословение Королей"
#define MSG_GOSSIP_TEXT_BUFF_MARK_OF_THE_WILD "[Бафнуть] Знак дикой природы"
#define MSG_GOSSIP_TEXT_BUFF_ARCANE_BRILLIANCE "[Бафнуть] Чародейская гениальность Даларана"
#define MSG_GOSSIP_TEXT_BUFF_BLESSING_OF_MIGHT "[Бафнуть] Благословение могущества"
#define MSG_GOSSIP_TEXT_BUFF_BLESSING_OF_WISDOM "[Бафнуть] Благословение мудрости"
#define MSG_GOSSIP_TEXT_BUFF_THORNS "[Бафнуть] Шипы"
#define MSG_GOSSIP_TEXT_BUFF_DIVINE_SPIRIT "[Бафнуть] Божественный дух"
#define MSG_GOSSIP_TEXT_BUFF_SHADOW_PROTECTION "[Бафнуть] Защита от темной магии"
#define MSG_GOSSIP_TEXT_BUFF_STAMINA "[Бафнуть] Выносливость"

#define MSG_GOSSIP_TEXT_SUPPER_BUFF_BLESSING_ADALS "[Бафнуть] Благословение А'далла."
#define MSG_GOSSIP_TEXT_SUPPER_BUFF_BLESSING_PINCHI "[Бафнуть] Благословение мистера Пинчи."

#define MSG_GOSSIP_TEXT_BUFF_MENU "[Меню Баффов] ->"
#define MSG_GOSSIP_TEXT_SUPER_MENU "[Меню особых бафов] ->"
#define MSG_GOSSIP_TEXT_MAIN_MENU "<- [Вернутся в Главное меню]"
#define MSG_GOSSIP_TEXT_MAIN_EXIT "[Выход]"

#define MSG_HI_LEVEL "У вас слишком высокий уровень"

#define CONST_HONOR_1 1000000
#define CONST_HONOR_2 100000


class npc_buffer : public CreatureScript
{
    public: 
    npc_buffer() : CreatureScript("npc_buffer") { }


	bool OnGossipHello(Player *player, Creature *creature)
	{ 
	    player->PlayerTalkClass->ClearMenus();
        //creature->MonsterWhisper(MSG_GOSSIP_TEXT_GETTING_STARTED, player->GetGUID());
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_BUFF_MENU, GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_SUPER_MENU, GOSSIP_SENDER_MAIN, 13);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_MAIN_EXIT, GOSSIP_SENDER_MAIN, 22);
        player->SEND_GOSSIP_MENU (DEFAULT_GOSSIP_MESSAGE,creature->GetGUID()); 
        return true;
	}
             
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 action )
{
		if(!player->getAttackers().empty())
        {
            creature->MonsterWhisper(MSG_ERR_INCOMBAT, player->GetGUID());
            player->CLOSE_GOSSIP_MENU();
            return false;
        }
        if(player->getLevel()>79)
        {
            creature->MonsterWhisper(MSG_HI_LEVEL, player->GetGUID());
            player->CLOSE_GOSSIP_MENU();
            return false;
        }
    switch (action)
	{
	case 2:
        player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_POWER_WORD, GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_BLESSING_OF_KINGS, GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_MARK_OF_THE_WILD, GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_ARCANE_BRILLIANCE, GOSSIP_SENDER_MAIN, 6);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_BLESSING_OF_MIGHT, GOSSIP_SENDER_MAIN, 7);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_BLESSING_OF_WISDOM, GOSSIP_SENDER_MAIN, 8);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_THORNS, GOSSIP_SENDER_MAIN, 9);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_DIVINE_SPIRIT, GOSSIP_SENDER_MAIN, 10);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_SHADOW_PROTECTION, GOSSIP_SENDER_MAIN, 11);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_BUFF_STAMINA, GOSSIP_SENDER_MAIN, 12);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_MAIN_MENU, GOSSIP_SENDER_MAIN, 21);
	    player->SEND_GOSSIP_MENU (DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
		break;
	case 3:
		// Наложение стойкости (Исправлено)
	    player->CastSpell(player, 69377, true);
		player->CLOSE_GOSSIP_MENU();
	    break;
    case 4:
		 // Наложение Благословение Королей (Исправлено)
		player->CastSpell(player, 56525, true);
		player->CLOSE_GOSSIP_MENU();
		break;
	case 5:
		 // Наложение Знак дикой природы
		player->CastSpell(player, 1126, true);
		player->CLOSE_GOSSIP_MENU();
		break;
	case 6:
		 // Наложение Чародейской гениальности Даларана (Исправлено)
		player->CastSpell(player, 61316, true);
		player->CLOSE_GOSSIP_MENU();
		break;
	case 7:
         // Благословение могущества (Исправлено)
		player->CastSpell(player, 48932, true);
		player->CLOSE_GOSSIP_MENU();
		break;
	case 8:
		 // Наложение Благословение мудрости (Исправлено)
		player->CastSpell(player, 56521, true);
		player->CLOSE_GOSSIP_MENU();
		break;
	case 9:
		 // Наложение Шипов (Исправлено)
		player->CastSpell(player, 66068, true);
		player->CLOSE_GOSSIP_MENU();
		break;
	case 10:
		 // Наложение Божественный духа (Исправлено)
		player->CastSpell(player, 48074, true);
		player->CLOSE_GOSSIP_MENU();
		break;
	case 11:
		 // Наложение Защиты от темной магии (Исправлено)
		player->CastSpell(player, 48170, true);
	    player->CLOSE_GOSSIP_MENU();
		break;
	case 12:
        // Выносливость (Исправлено)
		player->CastSpell(player, 48102, true);
		player->CLOSE_GOSSIP_MENU();
		break;
    case 13:
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, MSG_GOSSIP_TEXT_SUPPER_BUFF_BLESSING_ADALS, GOSSIP_SENDER_MAIN, 14);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, MSG_GOSSIP_TEXT_SUPPER_BUFF_BLESSING_PINCHI, GOSSIP_SENDER_MAIN, 15);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_MAIN_MENU, GOSSIP_SENDER_MAIN, 21);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID()); 
        break;
	case 14:
            // Наложение Благословение А'дала
            player->CastSpell(player, 35076, true);
            player->CLOSE_GOSSIP_MENU();
        break;
	case 15:
            // Наложение Благословения мистера Пинчи
            player->CastSpell(player, 33053, true);
            player->CLOSE_GOSSIP_MENU();
		break;
	case 21:
		OnGossipHello(player, creature);
		break;
    case 22:
		player->CLOSE_GOSSIP_MENU();
		break;
	}
    return true;
}
};

void AddSC_npc_buffer()
{
    new npc_buffer;
}
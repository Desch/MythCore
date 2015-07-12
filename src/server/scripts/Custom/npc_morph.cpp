/*
Custom patches
*/

#include "ScriptPCH.h"
#include "ScriptMgr.h"

#define MSG_GOSSIP_TEXT_GETTING_STARTED	"Приветствуем вас на сервере Battleage!"

#define MSG_ERR_INCOMBAT "Вы находитесь в бою. Чтобы использовать данного Npc выйдите из него."

#define MSG_GOSSIP_TEXT_MORTH_GNOME_MALE "[Превратить] Гном, мужчина."
#define MSG_GOSSIP_TEXT_MORTH_GNOME_FEMALE "[Превратить] Гном, женщина."
#define MSG_GOSSIP_TEXT_MORTH_HUMAN_FEMALE "[Превратить] Человек, женщина."
#define MSG_GOSSIP_TEXT_MORTH_HUMAN_MALE "[Превратить] Человек, мужчина."
#define MSG_GOSSIP_TEXT_MORTH_BLOOD_ELF_MALE "[Превратить] Эльф Крови, мужчина."
#define MSG_GOSSIP_TEXT_MORTH_BLOOD_ELF_FEMALE "[Превратить] Эльф Крови, женщина."
#define MSG_GOSSIP_TEXT_MORTH_TAUREN_MALE "[Превратить] Таурен, мужчина."
#define MSG_GOSSIP_TEXT_MORTH_TAUREN_FEMALE "[Превратить] Таурен, женщина."

class npc_morph : public CreatureScript
{
    public: 
    npc_morph() : CreatureScript("npc_morph") { }

	bool OnGossipHello(Player *player, Creature *creature)
		{ 
			creature->MonsterWhisper(MSG_GOSSIP_TEXT_GETTING_STARTED, player->GetGUID());
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_GNOME_MALE, GOSSIP_SENDER_MAIN, 2);
		    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_GNOME_FEMALE, GOSSIP_SENDER_MAIN, 3);
		    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_HUMAN_MALE, GOSSIP_SENDER_MAIN, 4);
		    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_HUMAN_FEMALE, GOSSIP_SENDER_MAIN, 5);
		    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_BLOOD_ELF_MALE, GOSSIP_SENDER_MAIN, 6);
		    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_BLOOD_ELF_FEMALE, GOSSIP_SENDER_MAIN, 7);
		    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_TAUREN_MALE, GOSSIP_SENDER_MAIN, 8);
		    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_TAUREN_FEMALE, GOSSIP_SENDER_MAIN, 9);
		    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
			return true;
	}
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 action )
    {
		  if (!player->getAttackers().empty())
        {
		creature->MonsterWhisper(MSG_ERR_INCOMBAT, player->GetGUID());
		player->CLOSE_GOSSIP_MENU();
        return false;
        }
    switch (action)
	{
	case 2:
		player->CastSpell(player, 37808, true);
        player->CLOSE_GOSSIP_MENU();
		break;
	case 3:
		player->CastSpell(player, 37809, true);
        player->CLOSE_GOSSIP_MENU();
		break;
	case 4:
		player->CastSpell(player, 35466, true);
        player->CLOSE_GOSSIP_MENU();
		break;
	case 5:
		player->CastSpell(player, 37805, true);
        player->CLOSE_GOSSIP_MENU();
		break;
	case 6:
		player->CastSpell(player, 37807, true);
        player->CLOSE_GOSSIP_MENU();
        break;
	case 7:
		player->CastSpell(player, 37806, true);
        player->CLOSE_GOSSIP_MENU();
        break;
	case 8:
		player->CastSpell(player, 37810, true);
        player->CLOSE_GOSSIP_MENU();
		break;
	case 9:
		player->CastSpell(player, 37811, true);
        player->CLOSE_GOSSIP_MENU();
		break;
	}
	return true;
    }
};
void AddSC_npc_morph()
{
    new npc_morph;
}
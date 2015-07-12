#include "ScriptPCH.h"

class morpher : public CreatureScript
{
public:
	morpher() : CreatureScript("morpher") {}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM( 5, "Je voudrai changer ma race en Murloc."		, GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM( 5, "Je voudrai changer ma race en Banshee."		, GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM( 5, "Je voudrai changer ma race en Troll de la Mort."		, GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM( 5, "Je voudrai changer ma race en Worgen."		, GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM( 5, "Je voudrai changer ma race en Goblin."		, GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM( 5, "Je voudrai changer ma race en Chevalier dechu."		, GOSSIP_SENDER_MAIN, 6);
		player->ADD_GOSSIP_ITEM( 5, "Je voudrai revenir a ma race d'origine."		, GOSSIP_SENDER_MAIN, 7);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
		return true;
	}
	void MorphChange(Player* player, uint32 modelid)
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT * FROM character_morph WHERE guid = %u", player->GetGUID());
		if (result)
		{
			CharacterDatabase.PExecute("UPDATE character_morph SET morph = %u WHERE guid = %u",modelid,player->GetGUID());
		}
		else
		{
			CharacterDatabase.PExecute("INSERT INTO character_morph VALUES (%u,%u)",player->GetGUID(),modelid);
		}
		player->SetDisplayId(modelid);
	}
	void SendDefaultMenu(Player *player, Creature *creature, uint32 action)
	{
		switch (action)
		{
		case 1:
			if (player->GetItemCount(44990) >= 25)
			{
				player->DestroyItemCount(44990, 25, true);
				MorphChange(player,652);
				creature->MonsterWhisper("Votre race a bien ete modifie",player->GetGUID());
			}
			else
				creature->MonsterWhisper("Il vous faut 25 sceaux de champion pour changer de race.",player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
			break;
		case 2:
			if (player->GetItemCount(44990) >= 25)
			{
				player->DestroyItemCount(44990, 25, true);
				MorphChange(player,915);
				creature->MonsterWhisper("Votre race a bien ete modifie",player->GetGUID());
			}
			else
				creature->MonsterWhisper("Il vous faut 25 sceaux de champion pour changer de race.",player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
			break;
		case 3:
			if (player->GetItemCount(44990) >= 25)
			{
				player->DestroyItemCount(44990, 25, true);
				MorphChange(player,26923);
				creature->MonsterWhisper("Votre race a bien ete modifie",player->GetGUID());
			}
			else
				creature->MonsterWhisper("Il vous faut 25 sceaux de champion pour changer de race.",player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
			break;
		case 4:
			if (player->GetItemCount(44990) >= 25)
			{
				player->DestroyItemCount(44990, 25, true);
				MorphChange(player,657);
				creature->MonsterWhisper("Votre race a bien ete modifie",player->GetGUID());
			}
			else
				creature->MonsterWhisper("Il vous faut 25 sceaux de champion pour changer de race.",player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
			break;
		case 5:
			if (player->GetItemCount(44990) >= 25)
			{
				player->DestroyItemCount(44990, 25, true);
				MorphChange(player,7191);
				creature->MonsterWhisper("Votre race a bien ete modifie",player->GetGUID());
			}
			else
				creature->MonsterWhisper("Il vous faut 25 sceaux de champion pour changer de race.",player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
			break;
		case 6:
			if (player->GetItemCount(44990) >= 25)
			{
				player->DestroyItemCount(44990, 25, true);
				MorphChange(player,27153);
				creature->MonsterWhisper("Votre race a bien ete modifie",player->GetGUID());
			}
			else
				creature->MonsterWhisper("Il vous faut 25 sceaux de champion pour changer de race.",player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
			break;
		case 7:
			//player->DeMorphReset();
			player->CLOSE_GOSSIP_MENU();
			break;
		}
	}
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 action )
	{
		// Main menu
		if (sender == GOSSIP_SENDER_MAIN)
			SendDefaultMenu(player, creature, action);

		return true;

	}
};

class morphscript : public PlayerScript
{
public:
    morphscript() : PlayerScript("morphscript") { }

    Field* fields;

    void OnPlayerLogin(Player* pPlayer)
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT morph FROM character_morph WHERE guid=%u", pPlayer->GetGUID());
        fields = result->Fetch();
		if (result->Fetch())
			pPlayer->SetDisplayId(fields[0].GetUInt32());
    }
};

void AddSC_Reset_Morph()
{
	new morpher;
	new morphscript();
}

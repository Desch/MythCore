#include "ScriptPCH.h"
#include "Config.h"
#include "UnicodeConvert.h"

extern LoginDatabaseWorkerPool LoginDatabase;
extern CharacterDatabaseWorkerPool CharacterDatabase;

extern char FactionHordeToAllianceChar[255];
extern char FactionAllianceToHordeChar[255];
extern char CustomizeChar[255];
extern char RaceChar[255] ;
class npc_character_change : public CreatureScript
{
public:
	npc_character_change() : CreatureScript("npc_character_change") { }

	bool OnGossipHello(Player * pPlayer, Creature * _Creature)
	{
		if(pPlayer->getFaction() == ALLIANCE)
			pPlayer->ADD_GOSSIP_ITEM(6, FactionAllianceToHordeChar , GOSSIP_SENDER_MAIN, 2);
		else
			pPlayer->ADD_GOSSIP_ITEM(6, FactionHordeToAllianceChar , GOSSIP_SENDER_MAIN, 2);
		pPlayer->ADD_GOSSIP_ITEM(6, CustomizeChar , GOSSIP_SENDER_MAIN, 3);
		pPlayer->ADD_GOSSIP_ITEM(6, RaceChar , GOSSIP_SENDER_MAIN, 4);
		pPlayer->ADD_GOSSIP_ITEM(6, "Quitter", GOSSIP_SENDER_MAIN, 11);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player *pPlayer, Creature *_Creature, uint32 sender, uint32 action )
	{
		// Main menu
		char msg[255];
		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch(action)
			{
			case 2:// changement de faction
			{
				uint32 count  = pPlayer->getFaction() == ALLIANCE ? sWorld->getIntConfig(CONFIG_CHANGE_FACTION_ITEM_COUNT_ALLIANCE_TO_HORDE):sWorld->getIntConfig(CONFIG_CHANGE_FACTION_ITEM_COUNT_HORDE_TO_ALLIANCE);
				
				if (pPlayer->HasItemCount(sWorld->getIntConfig(CONFIG_CHANGE_FACTION_ITEM), count ))
				{
					ChatHandler chathandler(pPlayer);
					if(chathandler.HandleCharacterChangeFactionCommandAddr(pPlayer->GetName()))
					{
						pPlayer->DestroyItemCount(sWorld->getIntConfig(CONFIG_CHANGE_FACTION_ITEM), count , true);
						char mess[] = "L'option de changement de faction à été effectué avec succès, veuillez vous déconnecter et vous reconnecter puis procéder à vos modification";
						ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
						_Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
					}

				}
				else
				{
					_Creature->MonsterTextEmote("Nombre d'item requis insuffisant!", pPlayer->GetGUID());
				}
				break;
			}
			case 3:// customization
			{
				if (pPlayer->HasItemCount(sWorld->getIntConfig(CONFIG_CHANGE_CUSTOMIZE_ITEM), sWorld->getIntConfig(CONFIG_CHANGE_CUSTOMIZE_ITEM_COUNT)))
				{
					ChatHandler chathandler(pPlayer);
					if(chathandler.HandleCharacterCustomizeCommandAddr(pPlayer->GetName()))
					{
						pPlayer->DestroyItemCount(sWorld->getIntConfig(CONFIG_CHANGE_CUSTOMIZE_ITEM), sWorld->getIntConfig(CONFIG_CHANGE_CUSTOMIZE_ITEM_COUNT), true);
						char mess[] = "La demande de customization à été effectué avec succès, veuillez vous déconnecter et vous reconnecter puis procéder à vos modification";
						ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
						_Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
					}
				}
				else
				{
					_Creature->MonsterTextEmote("Nombre d'item requis insuffisant!", pPlayer->GetGUID());
				}
				break;
			}
			case 4:// changement de race
			{
				if (pPlayer->HasItemCount(sWorld->getIntConfig(CONFIG_CHANGE_RACE_ITEM), sWorld->getIntConfig(CONFIG_CHANGE_RACE_ITEM_COUNT)))
				{
					ChatHandler chathandler(pPlayer);
					if(chathandler.HandleCharacterChangeRaceCommandAddr(pPlayer->GetName()))
					{
						pPlayer->DestroyItemCount(sWorld->getIntConfig(CONFIG_CHANGE_RACE_ITEM), sWorld->getIntConfig(CONFIG_CHANGE_RACE_ITEM_COUNT), true);
						char mess[] = "La demande de changement de race à été effectué avec succès, veuillez vous déconnecter et vous reconnecter puis procéder à vos modification";
						ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
						_Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
					}
				}
				else
				{
					_Creature->MonsterTextEmote("Nombre d'item requis insuffisant!", pPlayer->GetGUID());
				}
				break;
			}

			}
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		return true;
	}
};


void AddSC_Change_Character()
{
	new npc_character_change();
}
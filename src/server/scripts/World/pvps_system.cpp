#include "ScriptPCH.h"
#include "Config.h"
#include "Group.h"
extern CharacterDatabaseWorkerPool CharacterDatabase;

///////////////////////////////////////////////////////////////////////
///////////////////////      CONFIG      /////////////////////////////
/////////////////////////////////////////////////////////////////////

float Version = 2.50f; //Don't touch this.
bool PvPSystemEnabled = false; //Set to false if you want to disable the PvP System
bool OnlyInBattlegrounds = true; //Set to true if you want to enable this only in battlegrounds.
bool LooseTokenOnPvPDeath = false; //Set to true if you want the victim to loose tokens when the victim dies.
int32 AmountOfItemsYouWantTheVictimToLoose = 1; //Amount of items you want the victim to loose when victim dies.
bool AddTokenOnPvPKill = true; //Set to false if you don't want the killer to be rewarded.
int32 ItemReward = 20558; //The ItemID of the reward.
int32 AmountOfRewardsOnKillStreak[5] = { 1, 3, 5, 7, 10 }; //With how many items you want to reward the killer when he slays the victim.
int32 HowManyTimesYouWantTheKillerToGetAwardedForKillingTheSameVictim = 50; //Name speaks for It self.
const int32 KillerStreak1 = 10; 
const int32 KillerStreak2 = 30; 
const int32 KillerStreak3 = 100; 
const int32 KillerStreak4 = 200; 
const int32 KillerStreak5 = 300; 
int32 KillStreaks[5] = { KillerStreak1, KillerStreak2, KillerStreak3, KillerStreak4, KillerStreak5 };
int32 amounthonor = 124;
///////////////////////////////////////////////////////////////////
///////////////////////      END      ////////////////////////////
/////////////////////////////////////////////////////////////////

struct SystemInfo
{
    uint32 KillStreak;
    uint32 LastGUIDKill;
    uint8 KillCount;
};

static std::map<uint32, SystemInfo> KillingStreak;

class System_OnPvPKill : public PlayerScript
{
    public:
        System_OnPvPKill() : PlayerScript("System_OnPvPKill") {}

        
    void OnPVPKill(Player *pKiller, Player *pVictim)
    {
        if(PvPSystemEnabled == false)
        {
                 return;
        }
        else if(PvPSystemEnabled == true)
        {
                uint32 kGUID; 
                uint32 vGUID;
                char msg[500];
                kGUID = pKiller->GetGUID();
                vGUID = pVictim->GetGUID();                 
                
                if(kGUID == vGUID)
                {
                  return;
                }
                        
              if(KillingStreak[kGUID].LastGUIDKill == vGUID)
                {
                KillingStreak[kGUID].KillCount++;
                KillingStreak[vGUID].KillCount = 1;
                pKiller->AddItem(ItemReward, 1);
               
               if(LooseTokenOnPvPDeath == true)
                    pVictim->DestroyItemCount(ItemReward, AmountOfItemsYouWantTheVictimToLoose, true, false);
                }
                if(KillingStreak[kGUID].LastGUIDKill != vGUID)
                {
                    KillingStreak[kGUID].KillCount = 1;
                    KillingStreak[vGUID].KillCount = 1;
                }

                if(KillingStreak[kGUID].KillCount == HowManyTimesYouWantTheKillerToGetAwardedForKillingTheSameVictim)
                {
                    return;
                }

                if(OnlyInBattlegrounds == true)
                {
                    if(!pKiller->GetMap()->IsBattleground())
                        return;
                }

                    KillingStreak[kGUID].KillStreak++;
                    KillingStreak[vGUID].KillStreak = 0;
                    KillingStreak[kGUID].LastGUIDKill = vGUID;
                    KillingStreak[vGUID].LastGUIDKill = 0;
                if(AddTokenOnPvPKill == true)
                    pKiller->AddItem(ItemReward, 1);
                if(LooseTokenOnPvPDeath == true)
                    pVictim->DestroyItemCount(ItemReward, AmountOfItemsYouWantTheVictimToLoose, true, false);
                        
                switch(KillingStreak[kGUID].KillStreak)
                {
                case KillerStreak1:
                    sprintf(msg, " %s a tué %s et est à %u kill d'affilés! Un nouveau guerrier est née ! ", pKiller->GetName(), pVictim->GetName(), KillStreaks[1]);
                    sWorld->SendWorldText(LANG_SYSTEMMESSAGE, msg);
                    pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[1]);
                    break;

                case KillerStreak2:
                    sprintf(msg, " %s a tué %s et est à %u kill d'affilés! Il aime le goût du Sang ! ", pKiller->GetName(), pVictim->GetName(), KillStreaks[2]);
                    sWorld->SendWorldText(LANG_SYSTEMMESSAGE, msg);
                    pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[2]);
                    break;

                case KillerStreak3:
                    sprintf(msg, " %s a tué %s et est à %u kill d'affilés! Il est prit de Folie Meurtrière ! ", pKiller->GetName(), pVictim->GetName(), KillStreaks[3]);
                    sWorld->SendWorldText(LANG_SYSTEMMESSAGE, msg);
                    pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[3]);
                    break;

                case KillerStreak4:
                    sprintf(msg, " %s a tué %s et est à %u kill d'affilés! Il est prit de Frénésie Meurtrière ! ", pKiller->GetName(), pVictim->GetName(), KillStreaks[4]);
                    sWorld->SendWorldText(LANG_SYSTEMMESSAGE, msg);
                    pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[4]);
                    break;

                case KillerStreak5:
                    sprintf(msg, " %s a tué %s et est à %u kill d'affilés et gagne le titre ASSASSIN! ", pKiller->GetName(), pVictim->GetName(), KillStreaks[5]);
                    CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(95);
                    sWorld->SendWorldText(LANG_SYSTEMMESSAGE, msg);
                    pKiller->SetTitle(titleInfo);
		    pKiller->AddItem(19902,1);
                    KillingStreak[kGUID].KillStreak = 0;
		break;
            }
        }
    }
};


class flagffa : public CreatureScript
{
public:
	flagffa() : CreatureScript("flagffa") {}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		MapRefManager const &playerList = player->GetMap()->GetPlayers();
		bool hasAura = false;

		if (!playerList.isEmpty())
		{
			for (MapRefManager::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
			{
				if (Player *iplayer = i->getSource())
				{
					if (iplayer->GetAreaId() == 4163 && iplayer->HasAura(45450))
						hasAura = true;
				}
			}

			if(!hasAura)
			{
				SpellEntry const *spellInfo = sSpellStore.LookupEntry(58549);
				Group* grp = player->GetGroup();
				if (grp)
					for (GroupReference *itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
					{
						Player *playergrp = itr->getSource();
						if (playergrp)
						{
							playergrp->RemoveAurasDueToSpell(58549);
							Aura::TryCreate(spellInfo, MAX_EFFECT_MASK,playergrp,playergrp);
							Aura::TryCreate(spellInfo, MAX_EFFECT_MASK,playergrp,playergrp);
							Aura::TryCreate(spellInfo, MAX_EFFECT_MASK,playergrp,playergrp);
							Aura::TryCreate(spellInfo, MAX_EFFECT_MASK,playergrp,playergrp);
							Aura::TryCreate(spellInfo, MAX_EFFECT_MASK,playergrp,playergrp);
							playergrp->CastSpell(player,45450,true);
						}
						creature->MonsterYell("Un nouveau groupe d'Assassin est née !",LANG_UNIVERSAL,player->GetGUID());
					}
				else
				{
					player->RemoveAurasDueToSpell(58549);
					Aura::TryCreate(spellInfo, MAX_EFFECT_MASK,player,player);
					Aura::TryCreate(spellInfo, MAX_EFFECT_MASK,player,player);
					Aura::TryCreate(spellInfo, MAX_EFFECT_MASK,player,player);
					Aura::TryCreate(spellInfo, MAX_EFFECT_MASK,player,player);
					Aura::TryCreate(spellInfo, MAX_EFFECT_MASK,player,player);
					player->CastSpell(player,45450,true);
					creature->MonsterYell("Un nouvel Assassin est née !",LANG_UNIVERSAL,player->GetGUID());
				}
			}
		}
		return true;
	}
};

/*
class logoutffa : public PlayerScript
{
public:
	logoutffa() : PlayerScript("logoutffa") {}

	void OnLogout(Player* player)
	{
		player->RemoveAurasDueToSpell(45450);
		player->RemoveAurasDueToSpell(28747);
		player->RemoveAurasDueToSpell(58549);
	}
};

class loginoutffa : public PlayerScript
{
public:
	loginoutffa() : PlayerScript("loginoutffa") {}

	void OnLogin(Player* player)
	{
		if (player->HasAura(45450))
		{
			player->RemoveAurasDueToSpell(45450);
			player->RemoveAurasDueToSpell(58549);
		}
		Field* fields;
		QueryResult result = CharacterDatabase.PQuery("SELECT morph FROM character_morph WHERE guid=%u", player->GetGUID());
		if(result && sConfig->GetBoolDefault("Morph.Perm", true))
		{
			fields = result->Fetch();
			if(fields[0].GetUInt32() != 0)
				player->SetDisplayId(fields[0].GetUInt32());
		}
	}
};
*/


class pnjobsarena : public CreatureScript
{
public:
	pnjobsarena() : CreatureScript("pnjobsarena") {}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Je voudrais observer une arène (Entrez le nom du Joueur).", GOSSIP_SENDER_MAIN, 1, "", 0, true);
		player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
		return true;
	}
	bool OnGossipSelectCode(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction, const char* code)
	{
		pPlayer->PlayerTalkClass->ClearMenus();
		if (uiSender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				{
					QueryResult targetresult = CharacterDatabase.PQuery("SELECT `guid` FROM `characters` WHERE `name` ='%s'", code);
					if(targetresult)
					{
						uint64 target_guid;
						Field *fields = targetresult->Fetch();
						target_guid = fields[0].GetUInt32();
						Player * target = sObjectMgr->GetPlayer(target_guid);
						if (pPlayer->GetGUID() != target_guid)
						{
							if (target)
							{
								if (target->InArena() && !target->isGameMaster())
								{
									if (!target->HasAura(44521))
									{
										// only allow if gm mode is on
										if (!pPlayer->isGameMaster())
										{
											sLog->outError("Erreur ne peux pas etre tp normalement");
										}
										// if both players are in different bgs
										else if (pPlayer->GetBattlegroundId() && pPlayer->GetBattlegroundId() != target->GetBattlegroundId())
											pPlayer->LeaveBattleground(false); // Note: should be changed so _player gets no Deserter debuff

										// all's well, set bg id
										// when porting out from the bg, it will be reset to 0
										pPlayer->SetBattlegroundId(target->GetBattlegroundId(), target->GetBattlegroundTypeId());
										// remember current position as entry point for return at bg end teleportation
										if (!pPlayer->GetMap()->IsBattlegroundOrArena())
											pPlayer->SetBattlegroundEntryPoint();
										float x,y,z;
										target->GetContactPoint(pPlayer,x,y,z);
										pPlayer->TeleportTo(target->GetMapId(), x, y, z, pPlayer->GetAngle(target), TELE_TO_GM_MODE);
										pPlayer->SetPhaseMask(target->GetPhaseMask(), true);
										pPlayer->SetVisible(false);
										pPlayer->CastSpell(pPlayer,1852,true);
										pPlayer->CastSpell(pPlayer,28747,true);
										pPlayer->setFaction(35);
										if (Pet* pet = pPlayer->GetPet())
										{
											pet->setFaction(35);
											pet->getHostileRefManager().setOnlineOfflineState(false);
										}
										pPlayer->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
										pPlayer->ResetContestedPvP();

										pPlayer->getHostileRefManager().setOnlineOfflineState(false);
										pPlayer->CombatStopWithPets();
										pPlayer->CLOSE_GOSSIP_MENU();
									}
									else
										pCreature->MonsterWhisper("L'arène de votre cible n'a pas encore commencée",pPlayer->GetGUID());
								}
								else
									pCreature->MonsterWhisper("Votre cible n'est pas en arène.",pPlayer->GetGUID());
							}
							else
								pCreature->MonsterWhisper("Votre cible n'est pas connectée.",pPlayer->GetGUID());
						}
						else
							pCreature->MonsterWhisper("Vous ne pouvez pas vous cibler vous même.",pPlayer->GetGUID());
					}
					else
						pCreature->MonsterWhisper("Vous n'avez rien rempli ou votre cible n'éxiste pas.",pPlayer->GetGUID());
					pPlayer->CLOSE_GOSSIP_MENU();
					return true;
				}
			}
		}
		return false;
	}
};

class titrearene : public CreatureScript
{
public:
	titrearene() : CreatureScript("titrearene") { }

	bool OnGossipHello(Player * pPlayer, Creature * _Creature)
	{
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Titre Gladiateur [1250 de côte requis]", GOSSIP_SENDER_MAIN, 1);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Titre Gladiateur Impitoyable [1500 de côte requis]", GOSSIP_SENDER_MAIN, 2);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Titre Gladiateur Vengeur [1700 de côte requis]", GOSSIP_SENDER_MAIN, 3);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Titre Gladiateur Furieux + Monture + Tabard [1800 de côte requis]", GOSSIP_SENDER_MAIN, 4);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Titre Gladiateur Brutal + Monture [1950 de côte requis]", GOSSIP_SENDER_MAIN, 5);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Titre Gladiateur Implacable + Monture + Tabard [2050 de côte requis]", GOSSIP_SENDER_MAIN, 6);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Titre Gladiateur Courroucé + Monture + Tabard [2350 de côte requis]", GOSSIP_SENDER_MAIN, 7);
		pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(_Creature), _Creature->GetGUID());
		return true;
	}

	void SendDefaultMenu(Player *pPlayer, Creature *_Creature, uint32 action )
	{
		switch(action)
		{
			case 1:
			{
				uint32 rate = pPlayer->GetMaxPersonalArenaRatingRequirement(0);
				if (rate >= 1250)
				{
					CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(45);
					pPlayer->SetTitle(titleInfo);
					_Creature->MonsterWhisper("Félicitation, vous recevez votre nouveau titre !",pPlayer->GetGUID());
				}
				else
					_Creature->MonsterWhisper("Vous n'avez pas 1250 de côte d'arène",pPlayer->GetGUID());
				pPlayer->CLOSE_GOSSIP_MENU();
			}
			break;
			case 2:
			{
				uint32 rate = pPlayer->GetMaxPersonalArenaRatingRequirement(0);
				if (rate >= 1500)
				{
					CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(62);
					pPlayer->SetTitle(titleInfo);
					_Creature->MonsterWhisper("Félicitation, vous recevez votre nouveau titre !",pPlayer->GetGUID());
				}
				else
					_Creature->MonsterWhisper("Vous n'avez pas 1500 de côte d'arène",pPlayer->GetGUID());
				pPlayer->CLOSE_GOSSIP_MENU();
			}
			break;
			case 3:
			{
				uint32 rate = pPlayer->GetMaxPersonalArenaRatingRequirement(0);
				if (rate >= 1700)
				{
					CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(71);
					pPlayer->SetTitle(titleInfo);
					_Creature->MonsterWhisper("Félicitation, vous recevez votre nouveau titre !",pPlayer->GetGUID());
				}
				else
					_Creature->MonsterWhisper("Vous n'avez pas 1700 de côte d'arène",pPlayer->GetGUID());
				pPlayer->CLOSE_GOSSIP_MENU();
			}
			break;
			case 4:
			{
				uint32 rate = pPlayer->GetMaxPersonalArenaRatingRequirement(0);
				if (rate >= 1800)
				{
					CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(167);
					pPlayer->SetTitle(titleInfo);
					pPlayer->AddItem(45983,1);
					pPlayer->AddItem(46171,1);
					_Creature->MonsterWhisper("Félicitation, vous recevez votre nouveau titre ainsi que la monture et le tabard furieux !",pPlayer->GetGUID());
				}
				else
					_Creature->MonsterWhisper("Vous n'avez pas 1800 de côte d'arène",pPlayer->GetGUID());
				pPlayer->CLOSE_GOSSIP_MENU();
			}
			break;
			case 5:
			{
				uint32 rate = pPlayer->GetMaxPersonalArenaRatingRequirement(0);
				if (rate >= 1950)
				{
					CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(80);
					pPlayer->SetTitle(titleInfo);
					pPlayer->AddItem(54811,1);
					_Creature->MonsterWhisper("Félicitation, vous recevez votre nouveau titre ainsi que le Palefroi Célèste !",pPlayer->GetGUID());
				}
				else
					_Creature->MonsterWhisper("Vous n'avez pas 1950 de côte d'arène",pPlayer->GetGUID());
				pPlayer->CLOSE_GOSSIP_MENU();
			}
			break;
			case 6:
			{
				uint32 rate = pPlayer->GetMaxPersonalArenaRatingRequirement(0);
				if (rate >= 2050)
				{
					CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(169);
					pPlayer->SetTitle(titleInfo);
					pPlayer->AddItem(49086,1);
					pPlayer->AddItem(47840,1);
					_Creature->MonsterWhisper("Félicitation, vous recevez votre nouveau titre ainsi que la monture et le tabard Implacable !",pPlayer->GetGUID());
				}
				else
					_Creature->MonsterWhisper("Vous n'avez pas 2050 de côte d'arène",pPlayer->GetGUID());
				pPlayer->CLOSE_GOSSIP_MENU();
			}
			break;
			case 7:
			{
				uint32 rate = pPlayer->GetMaxPersonalArenaRatingRequirement(0);
				if (rate >= 2350)
				{
					CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(177);
					pPlayer->SetTitle(titleInfo);
					pPlayer->AddItem(51534,1);
					pPlayer->AddItem(50435,1);
					_Creature->MonsterWhisper("Félicitation, vous recevez votre nouveau titre ainsi que la monture et le tabard Couroucé !",pPlayer->GetGUID());
				}
				else
					_Creature->MonsterWhisper("Vous n'avez pas 2350 de côte d'arène",pPlayer->GetGUID());
				pPlayer->CLOSE_GOSSIP_MENU();
			}
			break;
		}		
	}			
	
	bool OnGossipSelect(Player *pPlayer, Creature *_Creature, uint32 sender, uint32 action )
	{
		// Main menu
		if (sender == GOSSIP_SENDER_MAIN)
			SendDefaultMenu(pPlayer, _Creature, action   );

		return true;

	}
};

void AddSC_Reset_Pvp()
{
	new System_OnPvPKill;
	new flagffa;
	//new logoutffa;
	//new loginoutffa;
	new pnjobsarena;
	new titrearene();
}

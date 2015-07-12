/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: professionnpc
SD%Complete: 100
SDComment: Profession Master
SDCategory: NPC
EndScriptData */

#include "ScriptPCH.h"
#include "Configuration/Config.h"


class professionnpc : public CreatureScript
{
public:
    professionnpc() : CreatureScript("professionnpc") { }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
		pPlayer->PlayerTalkClass->ClearMenus();
    	// Main menu
    	if (uiSender == GOSSIP_SENDER_MAIN)
    		SendDefaultMenu_ProfessionNPC(pPlayer, pCreature, uiAction);

    return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
    	

    	if(sConfig->GetBoolDefault("ProfessionNPC.OnlyGMs", false)) // If ProfessionNPC.OnlyGMs is enabled in trinitycore.conf
    		if (pPlayer->GetSession()->GetSecurity() == SEC_PLAYER)
    		{
    			pCreature->MonsterWhisper("Désolé je ne peux vendre que au Maitre du jeu.", pPlayer->GetGUID());
    			return true;
    		}

    	bool EnableProfessions = sConfig->GetBoolDefault("ProfessionNPC.EnableProfessions", true);
    	bool EnableSecondarySkills = sConfig->GetBoolDefault("ProfessionNPC.EnableSecondarySkills", true);

    	// Main Menu for Alliance
        if (pPlayer->GetTeam() == ALLIANCE)
    	{
    	// Check config if "Professions" is enabled or not
    	if(EnableProfessions)
    		pPlayer->ADD_GOSSIP_ITEM( 7, "Proféssions ->"        , GOSSIP_SENDER_MAIN, 1000);
    	// Check config if "Secondary Skills" is enabled or not
    	if(EnableSecondarySkills)
    		pPlayer->ADD_GOSSIP_ITEM( 7, "Métiers secondaires ->"   , GOSSIP_SENDER_MAIN, 2000);
    	}
    	else // Main Menu for Horde
    	{
    	// Check config if "Professions" is enabled or not
    	if(EnableProfessions)
    		pPlayer->ADD_GOSSIP_ITEM( 7, "Proféssions ->"        , GOSSIP_SENDER_MAIN, 1000);
    	// Check config if "Secondary Skills" is enabled or not
    	if(EnableSecondarySkills)
    		pPlayer->ADD_GOSSIP_ITEM( 7, "Métier secondaire ->"   , GOSSIP_SENDER_MAIN, 2000);
    	}

        pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());

    return true;
    }

void SendDefaultMenu_ProfessionNPC(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{

// Not allow in combat
if (pPlayer->isInCombat())
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterSay("Vous êtes en combat !", LANG_UNIVERSAL, NULL);
	return;
}


	bool EnableProfessions = sConfig->GetBoolDefault("ProfessionNPC.EnableProfessions", true);
	bool EnableSecondarySkills = sConfig->GetBoolDefault("ProfessionNPC.EnableSecondarySkills", true);

//Mony Check
if (pPlayer->GetMoney() < (sConfig->GetFloatDefault("SkillGoldCost",0)))
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterWhisper("Vous n'avez pas assez d'argent.", pPlayer->GetGUID());
	return;
}

switch(uiAction)
{

case 1000: //Profession
            pPlayer->ADD_GOSSIP_ITEM( 5, "Alchimie."             , GOSSIP_SENDER_MAIN, 1001);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Forge."                , GOSSIP_SENDER_MAIN, 1002);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Enchanteur."           , GOSSIP_SENDER_MAIN, 1003);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Ingénieure."           , GOSSIP_SENDER_MAIN, 1004);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Herboriste."           , GOSSIP_SENDER_MAIN, 1005);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Calligraphie."         , GOSSIP_SENDER_MAIN, 1006);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Joaillerie."           , GOSSIP_SENDER_MAIN, 1007);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Travail du cuir."      , GOSSIP_SENDER_MAIN, 1008);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Minage."               , GOSSIP_SENDER_MAIN, 1009);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Dépeçage."             , GOSSIP_SENDER_MAIN, 1010);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Tailleur."             , GOSSIP_SENDER_MAIN, 1011);
            pPlayer->ADD_GOSSIP_ITEM( 7, "<- Menu Principal"     , GOSSIP_SENDER_MAIN, 3000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 2000: //Secondary Skills
            pPlayer->ADD_GOSSIP_ITEM( 5, "Cuisine."              , GOSSIP_SENDER_MAIN, 2001);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Secouriste."           , GOSSIP_SENDER_MAIN, 2002);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Pêche."                , GOSSIP_SENDER_MAIN, 2003);
          //pPlayer->ADD_GOSSIP_ITEM( 5, "Riding."               , GOSSIP_SENDER_MAIN, 2004);
            pPlayer->ADD_GOSSIP_ITEM( 7, "<- Menu principal"     , GOSSIP_SENDER_MAIN, 3000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 3000: //Back To Main Menu
	// Main Menu for Alliance
    if (pPlayer->GetTeam() == ALLIANCE)
	{
	// Check config if "Professions" is enabled or not
	if(EnableProfessions)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Proféssions ->"        , GOSSIP_SENDER_MAIN, 1000);
	// Check config if "Secondary Skills" is enabled or not
	if(EnableSecondarySkills)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Métiers secondaires ->"   , GOSSIP_SENDER_MAIN, 2000);
	}
	else // Main Menu for Horde
	{
	// Check config if "Professions" is enabled or not
	if(EnableProfessions)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Proféssions ->"        , GOSSIP_SENDER_MAIN, 1000);
	// Check config if "Secondary Skills" is enabled or not
	if(EnableSecondarySkills)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Métier secondaires ->"   , GOSSIP_SENDER_MAIN, 2000);
	}

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

//////////////////////////////////////////////////Professions///////////////////////////////////////////////////////////////

case 1001: // Alchemy
	if(!pPlayer->UpdateSkill(171,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(171,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1002: // Blacksmithing
	if(!pPlayer->UpdateSkill(164,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(164,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1003: // Enchanting
	if(!pPlayer->UpdateSkill(333,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(333,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1004: // Engineering
	if(!pPlayer->UpdateSkill(202,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(202,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1005: // Herbalism
	if(!pPlayer->UpdateSkill(182,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(182,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1006: // Inscription
	if(!pPlayer->UpdateSkill(773,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(773,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1007: // Jewelcrafting
	if(!pPlayer->UpdateSkill(755,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(755,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1008: // Leatherworking
	if(!pPlayer->UpdateSkill(165,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(165,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1009: // Mining
	if(!pPlayer->UpdateSkill(186,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(186,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1010: // Skinning
	if(!pPlayer->UpdateSkill(393,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(393,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1011: // Tailoring
	if(!pPlayer->UpdateSkill(197,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(197,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

//////////////////////////////////////////////////Secondary Skills///////////////////////////////////////////////////////////////

case 2001: // Cooking
	if(!pPlayer->UpdateSkill(185,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(185,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 2002: // First Aid
	if(!pPlayer->UpdateSkill(129,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(129,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 2003: // Fishing
	if(!pPlayer->UpdateSkill(356,0))
      {
		pCreature->MonsterWhisper("Vous n'avez pas appris le métier ou alors vous avez déja la valeur maximal pour ce métier $C.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(356,(sConfig->GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

//case 2004: // Riding
//	if(!pPlayer->UpdateSkill(,0))
//    {
//		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
//		OnGossipHello(pPlayer, pCreature);
//		return;
//	}
//
//	pPlayer->UpdateSkill(,(sConfig->GetFloatDefault("SkillPoints",0)));
//	pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("SkillGoldCost",0)));
//	pPlayer->CLOSE_GOSSIP_MENU();
//break;

 pPlayer->CLOSE_GOSSIP_MENU();

} // end of switch
} //end of function


};




void AddSC_professionnpc()
{
    new professionnpc();
}

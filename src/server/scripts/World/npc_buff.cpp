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
SDName: buffnpc
SD%Complete: 100
SDComment: Buff Master
SDCategory: NPC
EndScriptData */

#include "ScriptPCH.h"
#include "Configuration/Config.h"


class buffnpc : public CreatureScript
{
public:
    buffnpc() : CreatureScript("buffnpc") { }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
		pPlayer->PlayerTalkClass->ClearMenus();
    	// Main menu
    	if (uiSender == GOSSIP_SENDER_MAIN)
    		SendDefaultMenu_BuffNPC(pPlayer, pCreature, uiAction);

    return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
    	

    	if(sConfig->GetBoolDefault("BuffNPC.OnlyGMs", false)) // If BuffNPC.OnlyGMs is enabled in trinitycore.conf
    		if (pPlayer->GetSession()->GetSecurity() == SEC_PLAYER)
    		{
    			pCreature->MonsterWhisper("Désolé, je ne peux que buff les maitres du jeu.", pPlayer->GetGUID());
    			return true;
    		}

    	bool EnableSmallBuff = sConfig->GetBoolDefault("BuffNPC.EnableSmallBuff", true);
    	bool EnableGreatBuff = sConfig->GetBoolDefault("BuffNPC.EnableGreatBuff", true);
    	bool EnableGMBuff = sConfig->GetBoolDefault("BuffNPC.EnableGMBuff", true);
    	bool EnablePlayerTools = sConfig->GetBoolDefault("BuffNPC.EnablePlayerTools", true);
    	bool EnableResSickness = sConfig->GetBoolDefault("BuffNPC.EnableRemoveResSickness", true);
    	bool EnableGivemeGold = sConfig->GetBoolDefault("BuffNPC.EnableGivemeGold", false);

    	// Main Menu for Alliance
        if (pPlayer->GetTeam() == ALLIANCE)
    	{
    	// Check config if "Small  Buff " is enabled or not
    	if(EnableSmallBuff)
    		pPlayer->ADD_GOSSIP_ITEM( 7, "Buff inférieure ->"         , GOSSIP_SENDER_MAIN, 1000);
    	// Check config if "Great Buff" is enabled or not
    	if(EnableGreatBuff)
    		pPlayer->ADD_GOSSIP_ITEM( 7, "Buff supérieure ->"         , GOSSIP_SENDER_MAIN, 2000);
    	// Check config if "GM Buff" is enabled or not
    	if(EnableGMBuff)
    		pPlayer->ADD_GOSSIP_ITEM( 7, "Buff MJ ->"                 , GOSSIP_SENDER_MAIN, 3000);
    	// Check config if "Player Tools" is enabled or not
    	if(EnablePlayerTools)
    		pPlayer->ADD_GOSSIP_ITEM( 7, "items spéciaux ->"          , GOSSIP_SENDER_MAIN, 4000);
    	}
    	else // Main Menu for Horde
    	{
    	// Check config if "Small  Buff " is enabled or not
    	if(EnableSmallBuff)
    		pPlayer->ADD_GOSSIP_ITEM( 7, "Buff supérieure ->"         , GOSSIP_SENDER_MAIN, 1000);
    	// Check config if "Great Buff" is enabled or not
    	if(EnableGreatBuff)
    		pPlayer->ADD_GOSSIP_ITEM( 7, "Buff inférieure ->"         , GOSSIP_SENDER_MAIN, 2000);
    	// Check config if "GM Buff" is enabled or not
    	if(EnableGMBuff)
    		pPlayer->ADD_GOSSIP_ITEM( 7, "Buff MJ ->"                 , GOSSIP_SENDER_MAIN, 3000);
    	// Check config if "Player Tools" is enabled or not
    	if(EnablePlayerTools)
    		pPlayer->ADD_GOSSIP_ITEM( 7, "Items spéciaux ->"          , GOSSIP_SENDER_MAIN, 4000);
    	}

    	// Check config file if "Remove res sickness" option is enabled or not
    	if(EnableResSickness)
    		pPlayer->ADD_GOSSIP_ITEM( 10, "Suprimmer le mal de REZ"   , GOSSIP_SENDER_MAIN, 5000);

        pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());

    return true;
    }

void SendDefaultMenu_BuffNPC(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{



// Not allow in combat
if (pPlayer->isInCombat())
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterSay("Vous êtes en combat !", LANG_UNIVERSAL, NULL);
	return;
}


	bool EnableSmallBuff = sConfig->GetBoolDefault("BuffNPC.EnableSmallBuff", true);
	bool EnableGreatBuff = sConfig->GetBoolDefault("BuffNPC.EnableGreatBuff", true);
	bool EnableGMBuff = sConfig->GetBoolDefault("BuffNPC.EnableGMBuff", true);
	bool EnablePlayerTools = sConfig->GetBoolDefault("BuffNPC.EnablePlayerTools", true);
	bool EnableResSickness = sConfig->GetBoolDefault("BuffNPC.EnableRemoveResSickness", true);
	bool EnableGivemeGold = sConfig->GetBoolDefault("BuffNPC.EnableGivemeGold", false);

//Mony Check
if (pPlayer->GetMoney() < (sConfig->GetFloatDefault("BuffGoldCost",0)))
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterWhisper("Vous n'avez pas l'argent nécéssaire.", pPlayer->GetGUID());
	return;
}

switch(uiAction)
{

case 1000: //Small  Buff
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Marque du fauve"                , GOSSIP_SENDER_MAIN, 1001);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Epines"                         , GOSSIP_SENDER_MAIN, 1005);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Amplification de la magie"      , GOSSIP_SENDER_MAIN, 1010);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Intélligence des arcanes"       , GOSSIP_SENDER_MAIN, 1015);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Intélligence de Dalaran"        , GOSSIP_SENDER_MAIN, 1020);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Atténuation de la magie"        , GOSSIP_SENDER_MAIN, 1025);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Bénédiction des rois"           , GOSSIP_SENDER_MAIN, 1030);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Bénédiction de puissance"       , GOSSIP_SENDER_MAIN, 1035);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Bénédiction de sagesse"         , GOSSIP_SENDER_MAIN, 1040);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi L'esprit divin"                 , GOSSIP_SENDER_MAIN, 1045);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi mot de pouvoir: Robustesse"     , GOSSIP_SENDER_MAIN, 1050);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Protection contre l'ombre"      , GOSSIP_SENDER_MAIN, 1055);
		pPlayer->ADD_GOSSIP_ITEM( 7, "<- Menu principal"                       , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 2000: //Great Buff
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Don du fauve"                            , GOSSIP_SENDER_MAIN, 2001);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Illumination des arcanes"                , GOSSIP_SENDER_MAIN, 2005);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Illumination de Dalaran"                 , GOSSIP_SENDER_MAIN, 2010);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Bénédiction des rois supérieure"         , GOSSIP_SENDER_MAIN, 2015);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Bénédiction de puissance supérieure"     , GOSSIP_SENDER_MAIN, 2020);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Bénédiction de sanctuaire supérieure"    , GOSSIP_SENDER_MAIN, 2025);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Bénédiction de sagesse supérieure"       , GOSSIP_SENDER_MAIN, 2030);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Prière de robustesse"                    , GOSSIP_SENDER_MAIN, 2035);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Prière de protection contre l'ombre"     , GOSSIP_SENDER_MAIN, 2040);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff-moi Prière d'esprit"                         , GOSSIP_SENDER_MAIN, 2045);
		pPlayer->ADD_GOSSIP_ITEM( 7, "<- Menu principal"                                , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 3000: //GM  Buff
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Agamaggan's Agility"             , GOSSIP_SENDER_MAIN, 3001);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Agamaggan's Strength"            , GOSSIP_SENDER_MAIN, 3005);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Armor Magic"                     , GOSSIP_SENDER_MAIN, 3010);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Armor Penetration"               , GOSSIP_SENDER_MAIN, 3015);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Increased Stamina"               , GOSSIP_SENDER_MAIN, 3020);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Razorhide"                       , GOSSIP_SENDER_MAIN, 3025);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Rising Spirit"                   , GOSSIP_SENDER_MAIN, 3030);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Spirit of the Wind"              , GOSSIP_SENDER_MAIN, 3035);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Buff me Wisdom of Agamaggan"             , GOSSIP_SENDER_MAIN, 3040);
		pPlayer->ADD_GOSSIP_ITEM( 7, "<- Menu principal"                            , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 4000: //Player Tools
	// Check config if "Give me Gold" is enabled or not
	if(EnableGivemeGold)
		pPlayer->ADD_GOSSIP_ITEM( 5, "Donnez moi de l'or"                      , GOSSIP_SENDER_MAIN, 4001);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Donnez moi des fraguement d'âme"         , GOSSIP_SENDER_MAIN, 4005);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Heal-moi s'il te plait"                  , GOSSIP_SENDER_MAIN, 4010);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Heal les membres de mon raid"            , GOSSIP_SENDER_MAIN, 4015);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Invoquer moi des rafraichissement"       , GOSSIP_SENDER_MAIN, 4020);
		pPlayer->ADD_GOSSIP_ITEM( 5, "Invoquer moi une gêmme de mana"          , GOSSIP_SENDER_MAIN, 4025);
		pPlayer->ADD_GOSSIP_ITEM( 7, "<- Menu principal"                       , GOSSIP_SENDER_MAIN, 5005);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 5005: //Back To Main Menu
	// Main Menu for Alliance
    if (pPlayer->GetTeam() == ALLIANCE)
	{
	// Check config if "Small  Buff " is enabled or not
	if(EnableSmallBuff)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Buff inférieure ->"         , GOSSIP_SENDER_MAIN, 1000);
	// Check config if "Great Buff" is enabled or not
	if(EnableGreatBuff)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Buff supérieure ->"         , GOSSIP_SENDER_MAIN, 2000);
	// Check config if "GM Buff" is enabled or not
	if(EnableGMBuff)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Buff MJ ->"                 , GOSSIP_SENDER_MAIN, 3000);
	// Check config if "Player Tools" is enabled or not
	if(EnablePlayerTools)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Items spéciaux ->"          , GOSSIP_SENDER_MAIN, 4000);
	}
	else // Main Menu for Horde
	{
	// Check config if "Small  Buff " is enabled or not
	if(EnableSmallBuff)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Buff inférieure ->"         , GOSSIP_SENDER_MAIN, 1000);
	// Check config if "Great Buff" is enabled or not
	if(EnableGreatBuff)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Buff supérieure ->"         , GOSSIP_SENDER_MAIN, 2000);
	// Check config if "GM Buff" is enabled or not
	if(EnableGMBuff)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Buff MJ ->"                 , GOSSIP_SENDER_MAIN, 3000);
	// Check config if "Player Tools" is enabled or not
	if(EnablePlayerTools)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Items spéciaux ->"          , GOSSIP_SENDER_MAIN, 4000);
	}

	// Check if Ress Sickness option is enabled in scriptdev2.conf
	if(EnableResSickness)
		pPlayer->ADD_GOSSIP_ITEM( 10, "Suprimmer le mal de REZ"   , GOSSIP_SENDER_MAIN, 5000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

//////////////////////////////////////////////////Small Buff///////////////////////////////////////////////////////////////

case 1001: // Buff me Mark of the Wild
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48469,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 1005: // Buff me Thorns
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,53307,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 1010: // Buff me Amplify Magic
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,43017,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 1015: // Buff me Arcane Intellect
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,42995,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 1020: // Buff me Dalaran Intellect
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,61024,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 1025: // Buff me Dampen Magic
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,43015,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 1030: // Buff me Blessing of Kings
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,20217,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 1035: // Buff me Blessing of Might
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48932,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 1040: // Buff me Blessing of Wisdom
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48936,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 1045: // Buff me Divine Spirit
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48073,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 1050: // Buff me Power Word: Fortitude
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48161,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 1055: // Buff me Shadow Protection
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48169,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

//////////////////////////////////////////////////Great Buff///////////////////////////////////////////////////////////////

case 2001: // Buff me Gift of the Wild
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,48470,true);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 2005: // Buff me Arcane Brilliance
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,43002,true);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 2010: // Buff me Dalaran Brilliance
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,61316,true);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 2015: // Buff me Greater Blessing of Kings
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,25898,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 2020: // Buff me Greater Blessing of Might
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48934,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 2025: // Buff me Greater Blessing of Sanctuary
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,25899,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 2030: // Buff me Greater Blessing of Wisdom
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,48938,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 2035: // Buff me Prayer of Fortitude
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,48162,true);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 2040: // Buff me Prayer of Shadow Protection
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,48170,true);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 2045: // Buff me Prayer of Spirit
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,48074,true);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

//////////////////////////////////////////////////GM Buff///////////////////////////////////////////////////////////////

case 3001: // Buff me Agamaggan's Agility
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,17013,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 3005: // Buff me Agamaggan's Strength
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,16612,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 3010: // Buff me Armor Magic
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,58453,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 3015: // Buff me Armor Penetration
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,34106,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 3020: // Buff me Increased Stamina
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,25661,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 3025: // Buff me Razorhide
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,16610,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 3030: // Buff me Rising Spirit
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,10767,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 3035: // Buff me Spirit of the Wind
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,16618,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 3040: // Buff me Wisdom of Agamaggan
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,7764,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

//////////////////////////////////////////////////Player Tools///////////////////////////////////////////////////////////////

case 4001://Give me Gold
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,46642,false); // 5000 gold
break;

case 4005://Give me Soul Shards
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,24827,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 4010: // Heal me please
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(pPlayer,38588,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 4015: // Heal me and party members Please
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,53251,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 4020: // Conjure Refreshment
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,42956,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 4025: // Conjure Mana Gem
    pPlayer->CLOSE_GOSSIP_MENU();
    pPlayer->CastSpell(pPlayer,42985,false);
    pPlayer->ModifyMoney(-(sConfig->GetFloatDefault("BuffGoldCost",0)));
break;

case 5000://Remove Res Sickness
	if(!pPlayer->HasAura(15007,0))
	{
		pCreature->MonsterWhisper("Vous n'avez pas le Mal de Résurrection.", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pCreature->CastSpell(pPlayer,38588,false); // Healing effect
	pPlayer->RemoveAurasDueToSpell(15007);
	pPlayer->CLOSE_GOSSIP_MENU();
break;

 pPlayer->CLOSE_GOSSIP_MENU();

} // end of switch
} //end of function

};






void AddSC_buffnpc()
{
    new buffnpc();
}



#include "ScriptPCH.h"

enum Items
{
    /* WINTERGRASP LEVELUP ITEMS */

    DAGGER_ONE_HAND_ATTACK_POWER = 42944,
    AXE_TWO_HAND_ATTACK_POWER = 42943,
    MACE_ONE_HAND_SPELL_POWER = 42948,
    STAFF_TWO_HAND_SPELL_POWER = 42947,
    TRINKET_SPELL_POWER = 42992,
    SHOULDER_MAIL_ATTACK_POWER = 42950,
    CHEST_MAIL_ATTACK_POWER = 48677,
    BOW_RANGED_ATTACK_POWER = 42946,
    SHOULDER_MAIL_SPELL_POWER = 42951,
    CHEST_MAIL_SPELL_POWER = 48683,
    CHEST_PLATE_ATTACK_POWER = 48685,
    SHOULDER_PLATE_ATTACK_POWER = 42949,
    CHEST_LEATHER_SPELL_POWER = 48687,
    SHOULDER_LEATHER_SPELL_POWER = 42984,
    MACE_TWO_HAND_ATTACK_POWER = 48718,
    SHOULDER_LEATHER_ATTACK_POWER = 42952,
    CHEST_LEATHER_ATTACK_POWER = 48689,
    TRINKET_ATTACK_POWER = 42991,
    SHOULDER_CLOTH_SPELL_POWER = 42985,
    CHEST_CLOTH_SPELL_POWER = 48691,
    SWORD_ONE_HAND_ATTACK_POWER = 42945,
    MACE_ONEHAND_ATTACK_POWER = 48716,

    /* WINTERGRASP LEVELUP ITEMS */

    /* SAVAGE SETS */

    // DEATHKNIGHT
    /*ITEM_DEATHKNIGHT_1 = 40779,
    ITEM_DEATHKNIGHT_2 = 40799,
    ITEM_DEATHKNIGHT_3 = 40817,
    ITEM_DEATHKNIGHT_4 = 40837,
    ITEM_DEATHKNIGHT_5 = 40857,

    // DRUID
    ITEM_DRUID_1 = 41658,
    ITEM_DRUID_2 = 41664,
    ITEM_DRUID_3 = 41675,
    ITEM_DRUID_4 = 41712,
    ITEM_DRUID_5 = 41770,

    // HUNTER
    ITEM_HUNTER_1 = 41084,
    ITEM_HUNTER_2 = 41140,
    ITEM_HUNTER_3 = 41154,
    ITEM_HUNTER_4 = 41202,
    ITEM_HUNTER_5 = 41214,

    // MAGE
    ITEM_MAGE_1 = 41943,
    ITEM_MAGE_2 = 41949,
    ITEM_MAGE_3 = 41956,
    ITEM_MAGE_4 = 41962,
    ITEM_MAGE_5 = 41968,

    // PALADIN
    ITEM_PALADIN_1 = 40780,
    ITEM_PALADIN_2 = 40798,
    ITEM_PALADIN_3 = 40818,
    ITEM_PALADIN_4 = 40838,
    ITEM_PALADIN_5 = 40858,

    // PRIEST
    ITEM_PRIEST_1 = 41912,
    ITEM_PRIEST_2 = 41918,
    ITEM_PRIEST_3 = 41924,
    ITEM_PRIEST_4 = 41930,
    ITEM_PRIEST_5 = 41937,

    // ROGUE
    ITEM_ROGUE_1 = 41643,
    ITEM_ROGUE_2 = 41644,
    ITEM_ROGUE_3 = 41645,
    ITEM_ROGUE_4 = 41646,
    ITEM_ROGUE_5 = 41647,

    // SHAMAN
    ITEM_SHAMAN_1 = 41078,
    ITEM_SHAMAN_2 = 41134,
    ITEM_SHAMAN_3 = 41148,
    ITEM_SHAMAN_4 = 41160,
    ITEM_SHAMAN_5 = 41208,

    // WARLOCK
    ITEM_WARLOCK_1 = 41990,
    ITEM_WARLOCK_2 = 41996,
    ITEM_WARLOCK_3 = 42002,
    ITEM_WARLOCK_4 = 42008,
    ITEM_WARLOCK_5 = 42014,
	
    // WARRIOR
    ITEM_WARRIOR_1 = 40778,
    ITEM_WARRIOR_2 = 40797,
    ITEM_WARRIOR_3 = 40816,
    ITEM_WARRIOR_4 = 40836,
    ITEM_WARRIOR_5 = 40856,*/

    /* SAVAGE SETS */

    /* DEADLY SETS */

    ITEM_DEATHKNIGHT_DAMAGE_MEELE_1 = 40791,
    ITEM_DEATHKNIGHT_DAMAGE_MEELE_2 = 40811,
    ITEM_DEATHKNIGHT_DAMAGE_MEELE_3 = 40830,
    ITEM_DEATHKNIGHT_DAMAGE_MEELE_4 = 40851,
    ITEM_DEATHKNIGHT_DAMAGE_MEELE_5 = 40871,

    // DRUID
    ITEM_DRUID_DAMAGE_MEELE_1 = 41662,
    ITEM_DRUID_DAMAGE_MEELE_2 = 41668,
    ITEM_DRUID_DAMAGE_MEELE_3 = 41679,
    ITEM_DRUID_DAMAGE_MEELE_4 = 41716,
    ITEM_DRUID_DAMAGE_MEELE_5 = 41774,

    ITEM_DRUID_DAMAGE_SPELL_1 = 41282,
    ITEM_DRUID_DAMAGE_SPELL_2 = 41294,
    ITEM_DRUID_DAMAGE_SPELL_3 = 41305,
    ITEM_DRUID_DAMAGE_SPELL_4 = 41317,
    ITEM_DRUID_DAMAGE_SPELL_5 = 41328,

    ITEM_DRUID_HEAL_SPELL_1 = 41276,
    ITEM_DRUID_HEAL_SPELL_2 = 41288,
    ITEM_DRUID_HEAL_SPELL_3 = 41299,
    ITEM_DRUID_HEAL_SPELL_4 = 41311,
    ITEM_DRUID_HEAL_SPELL_5 = 41322,

    // HUNTER
    ITEM_HUNTER_DAMAGE_RANGED_1 = 41088,
    ITEM_HUNTER_DAMAGE_RANGED_2 = 41144,
    ITEM_HUNTER_DAMAGE_RANGED_3 = 41158,
    ITEM_HUNTER_DAMAGE_RANGED_4 = 41206,
    ITEM_HUNTER_DAMAGE_RANGED_5 = 41218,

    // MAGE
    ITEM_MAGE_DAMAGE_SPELL_1 = 41947,
    ITEM_MAGE_DAMAGE_SPELL_2 = 41954,
    ITEM_MAGE_DAMAGE_SPELL_3 = 41960,
    ITEM_MAGE_DAMAGE_SPELL_4 = 41966,
    ITEM_MAGE_DAMAGE_SPELL_5 = 41972,

    // PALADIN
    ITEM_PALADIN_DAMAGE_MEELE_1 = 40792,
    ITEM_PALADIN_DAMAGE_MEELE_2 = 40812,
    ITEM_PALADIN_DAMAGE_MEELE_3 = 40831,
    ITEM_PALADIN_DAMAGE_MEELE_4 = 40852,
    ITEM_PALADIN_DAMAGE_MEELE_5 = 40872,

    ITEM_PALADIN_HEAL_SPELL_1 = 40910,
    ITEM_PALADIN_HEAL_SPELL_2 = 40928,
    ITEM_PALADIN_HEAL_SPELL_3 = 40934,
    ITEM_PALADIN_HEAL_SPELL_4 = 40940,
    ITEM_PALADIN_HEAL_SPELL_5 = 40964,

    // PRIEST
    ITEM_PRIEST_DAMAGE_SPELL_1 = 41916,
    ITEM_PRIEST_DAMAGE_SPELL_2 = 41922,
    ITEM_PRIEST_DAMAGE_SPELL_3 = 41928,
    ITEM_PRIEST_DAMAGE_SPELL_4 = 41935,
    ITEM_PRIEST_DAMAGE_SPELL_5 = 41941,

    ITEM_PRIEST_HEAL_SPELL_1 = 41855,
    ITEM_PRIEST_HEAL_SPELL_2 = 41860,
    ITEM_PRIEST_HEAL_SPELL_3 = 41865,
    ITEM_PRIEST_HEAL_SPELL_4 = 41870,
    ITEM_PRIEST_HEAL_SPELL_5 = 41875,

    // ROGUE
    ITEM_ROGUE_DAMAGE_MEELE_1 = 41651,
    ITEM_ROGUE_DAMAGE_MEELE_2 = 41656,
    ITEM_ROGUE_DAMAGE_MEELE_3 = 41673,
    ITEM_ROGUE_DAMAGE_MEELE_4 = 41684,
    ITEM_ROGUE_DAMAGE_MEELE_5 = 41768,

    // SHAMAN
    ITEM_SHAMAN_DAMAGE_MEELE_1 = 41082,
    ITEM_SHAMAN_DAMAGE_MEELE_2 = 41138,
    ITEM_SHAMAN_DAMAGE_MEELE_3 = 41152,
    ITEM_SHAMAN_DAMAGE_MEELE_4 = 41200,
    ITEM_SHAMAN_DAMAGE_MEELE_5 = 41212,

    ITEM_SHAMAN_DAMAGE_SPELL_1 = 40995,
    ITEM_SHAMAN_DAMAGE_SPELL_2 = 41008,
    ITEM_SHAMAN_DAMAGE_SPELL_3 = 41020,
    ITEM_SHAMAN_DAMAGE_SPELL_4 = 41034,
    ITEM_SHAMAN_DAMAGE_SPELL_5 = 41045,

    ITEM_SHAMAN_HEAL_SPELL_1 = 40994,
    ITEM_SHAMAN_HEAL_SPELL_2 = 41002,
    ITEM_SHAMAN_HEAL_SPELL_3 = 41014,
    ITEM_SHAMAN_HEAL_SPELL_4 = 41028,
    ITEM_SHAMAN_HEAL_SPELL_5 = 41039,

    // WARLOCK
    ITEM_WARLOCK_DAMAGE_SPELL_1 = 41994,
    ITEM_WARLOCK_DAMAGE_SPELL_2 = 41999,
    ITEM_WARLOCK_DAMAGE_SPELL_3 = 42006,
    ITEM_WARLOCK_DAMAGE_SPELL_4 = 42012,
    ITEM_WARLOCK_DAMAGE_SPELL_5 = 42018,
	
    // WARRIOR
    ITEM_WARRIOR_DAMAGE_MEELE_1 = 40790,
    ITEM_WARRIOR_DAMAGE_MEELE_2 = 40810,
    ITEM_WARRIOR_DAMAGE_MEELE_3 = 40829,
    ITEM_WARRIOR_DAMAGE_MEELE_4 = 40850,
    ITEM_WARRIOR_DAMAGE_MEELE_5 = 40870,

    /* DEADLY SETS */

    /* DEADLY WEAPONS */

    ITEM_RANGED_LONGBOW = 42492,
    ITEM_RANGED_GUN = 42487,
    ITEM_RANGED_CROSSBOW = 42498,
    ITEM_RANGED_THROWN = 42483,
    ITEM_RANGED_WAND_HEAL = 42515,
    ITEM_RANGED_WAND_DAMAGE = 42521,
    ITEM_MEELE_DAGGER_MAIN = 42244,
    ITEM_MEELE_DAGGER_OFF = 42257,
    ITEM_SPELL_MACE_MAIN = 423540,
    ITEM_SPELL_SHIELD_DAMAGE = 42566,
    ITEM_SPELL_SHIELD_HEAL = 42572,
    ITEM_MEELE_STAFF = 42392,
    ITEM_SPELL_STAFF_HEAL = 42386,
    ITEM_SPELL_STAFF_DAMAGE = 42366,
    ITEM_MEELE_AXE_TWO_HAND = 42319,
    ITEM_MEELE_MACE_TWO_HAND = 42324,
    ITEM_MEELE_SWORD_TWO_HAND = 42334,
    ITEM_MEELE_AXE_MAIN = 42210,
    ITEM_MEELE_AXE_OFF = 42234,
    ITEM_SPELL_DAGGER_MAIN = 42348,

    /* DEADLY WEAPONS */

    /* ADITIONAL */

    BAG_16_SLOTS = 4500

    /* ADITIONAL */
};

enum Gossip
{
    DEATH_KNIGHT_DAMAGE_MEELE = 1000,
    DRUID_DAMAGE_MEELE = 1001,
    DRUID_DAMAGE_SPELL = 1002,
    DRUID_HEAL_SPELL = 1003,
    HUNTER_DAMAGE_RANGED = 1004,
    MAGE_DAMAGE_SPELL = 1005,
    PALADIN_DAMAGE_MEELE = 1006,
    PALADIN_HEAL_SPELL = 1007,
    PRIEST_DAMAGE_SPELL = 1008,
    PRIEST_HEAL_SPELL = 1009,
    ROGUE_DAMAGE_MEELE = 1010,
    SHAMAN_DAMAGE_MEELE = 1011,
    SHAMAN_DAMAGE_SPELL = 1012,
    SHAMAN_HEAL_SPELL = 1013,
    WARLOCK_DAMAGE_SPELL = 1014,
    WARRIOR_DAMAGE_MEELE = 1015
};
class custom_npc : public CreatureScript
{
public:
    custom_npc() : CreatureScript("custom_npc") { }
	
	bool Check_Transfer(uint32 Guid)
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT npc_transfer FROM character_transfer WHERE guid = %u", Guid);
		
		if (result)
		{
/*			Field *fields = result->Fetch();
			uint32 Value = fields[0].GetUInt32();
			
			if ( !Value ) return true;
*/
			return false;
		}
		else
		{
			return true;
		}
	}
  

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if(pPlayer->getLevel() < 20)
            return false;

		if( Check_Transfer(pPlayer->GetGUID()) )
		{
			if(pPlayer->isAlive())
			{
				switch(pPlayer->getClass())
				{
					case CLASS_DEATH_KNIGHT:
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff CAC pour DK + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, DEATH_KNIGHT_DAMAGE_MEELE);
						break;
					case CLASS_DRUID:
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff CAC pour Druide + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, DRUID_DAMAGE_MEELE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff DPS pour Druide + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, DRUID_DAMAGE_SPELL);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff HEAL pour Druide + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, DRUID_HEAL_SPELL);
						break;
					case CLASS_HUNTER:
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff DPS pour Chasseur + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, HUNTER_DAMAGE_RANGED);
						break;
					case CLASS_MAGE:
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff DPS pour Mage + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, MAGE_DAMAGE_SPELL);
						break;
					case CLASS_PALADIN:
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff CAC pour Paladin + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, PALADIN_DAMAGE_MEELE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff HEAL pour Paladin + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, PALADIN_HEAL_SPELL);
						break;
					case CLASS_PRIEST:
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff DPS pour Prêtre + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, PRIEST_DAMAGE_SPELL);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff HEAL pour Prêtre + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, PRIEST_HEAL_SPELL);
						break;
					case CLASS_ROGUE:
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff CAC pour Voleur + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, ROGUE_DAMAGE_MEELE);
						break;
					case CLASS_SHAMAN:
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff CAC pour Chaman + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, SHAMAN_DAMAGE_MEELE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff DPS pour Chaman + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, SHAMAN_DAMAGE_SPELL);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff HEAL pour Chaman + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, SHAMAN_HEAL_SPELL);
						break;
					case CLASS_WARLOCK:
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff DPS pour Démoniste + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, WARLOCK_DAMAGE_SPELL);
						break;
					case CLASS_WARRIOR:
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "(Je veux le Stuff CAC pour Guerrier + 9000 po) Vous serez téléporté à la Zone de Shop!", GOSSIP_SENDER_MAIN, WARRIOR_DAMAGE_MEELE);
						break;
				}
			}
		}
		else
		{
			pCreature->MonsterWhisper("$N , Vous ne pouvez le faire qu'une seule fois, dommage ^__^", pPlayer->GetGUID());
		}

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();

        switch(uiAction)
        {
            // DEATHKNIGHT DAMAGE MEELE
            case DEATH_KNIGHT_DAMAGE_MEELE:
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_DEATHKNIGHT_DAMAGE_MEELE_1, 1);
                    pPlayer->AddItem(ITEM_DEATHKNIGHT_DAMAGE_MEELE_2, 1);
                    pPlayer->AddItem(ITEM_DEATHKNIGHT_DAMAGE_MEELE_3, 1);
                    pPlayer->AddItem(ITEM_DEATHKNIGHT_DAMAGE_MEELE_4, 1);
                    pPlayer->AddItem(ITEM_DEATHKNIGHT_DAMAGE_MEELE_5, 1);
                    pPlayer->AddItem(ITEM_MEELE_SWORD_TWO_HAND, 1);
                }
                break;

            // DRUID DAMAGE MEELE		
            case DRUID_DAMAGE_MEELE:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_DRUID_DAMAGE_MEELE_1, 1);
                    pPlayer->AddItem(ITEM_DRUID_DAMAGE_MEELE_2, 1);
                    pPlayer->AddItem(ITEM_DRUID_DAMAGE_MEELE_3, 1);
                    pPlayer->AddItem(ITEM_DRUID_DAMAGE_MEELE_4, 1);
                    pPlayer->AddItem(ITEM_DRUID_DAMAGE_MEELE_5, 1);
                    pPlayer->AddItem(ITEM_MEELE_STAFF, 1);
                }
                else
                {
                    pPlayer->AddItem(MACE_TWO_HAND_ATTACK_POWER, 1);
                    pPlayer->AddItem(SHOULDER_LEATHER_ATTACK_POWER, 1);
                    pPlayer->AddItem(CHEST_LEATHER_ATTACK_POWER, 1);
                }
                pPlayer->AddItem(TRINKET_ATTACK_POWER, 1);
                break;

            // DRUID DAMAGE SPELL
            case DRUID_DAMAGE_SPELL:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_DRUID_DAMAGE_SPELL_1, 1);
                    pPlayer->AddItem(ITEM_DRUID_DAMAGE_SPELL_2, 1);
                    pPlayer->AddItem(ITEM_DRUID_DAMAGE_SPELL_3, 1);
                    pPlayer->AddItem(ITEM_DRUID_DAMAGE_SPELL_4, 1);
                    pPlayer->AddItem(ITEM_DRUID_DAMAGE_SPELL_5, 1);
                    pPlayer->AddItem(ITEM_SPELL_STAFF_DAMAGE, 1);
                }
                else
                {
                    pPlayer->AddItem(STAFF_TWO_HAND_SPELL_POWER, 1);
                    pPlayer->AddItem(SHOULDER_LEATHER_SPELL_POWER, 1);
                    pPlayer->AddItem(CHEST_LEATHER_SPELL_POWER, 1);
                }
                pPlayer->AddItem(TRINKET_SPELL_POWER, 1);
                break;

            // DRUID HEAL SPELL	
            case DRUID_HEAL_SPELL:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_DRUID_HEAL_SPELL_1, 1);
                    pPlayer->AddItem(ITEM_DRUID_HEAL_SPELL_2, 1);
                    pPlayer->AddItem(ITEM_DRUID_HEAL_SPELL_3, 1);
                    pPlayer->AddItem(ITEM_DRUID_HEAL_SPELL_4, 1);
                    pPlayer->AddItem(ITEM_DRUID_HEAL_SPELL_5, 1);
                    pPlayer->AddItem(ITEM_SPELL_STAFF_HEAL, 1);
                }
                else
                {
                    pPlayer->AddItem(STAFF_TWO_HAND_SPELL_POWER, 1);
                    pPlayer->AddItem(SHOULDER_LEATHER_SPELL_POWER, 1);
                    pPlayer->AddItem(CHEST_LEATHER_SPELL_POWER, 1);
                }
                pPlayer->AddItem(TRINKET_SPELL_POWER, 1);
                break;

            // HUNTER DAMAGE RANGED
            case HUNTER_DAMAGE_RANGED:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() <= 39 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(AXE_TWO_HAND_ATTACK_POWER, 1);
                    pPlayer->AddItem(BOW_RANGED_ATTACK_POWER, 1);
                    pPlayer->AddItem(SHOULDER_LEATHER_ATTACK_POWER, 1);
                    pPlayer->AddItem(CHEST_LEATHER_ATTACK_POWER, 1);
                }
                if(pPlayer->getLevel() >= 40 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(AXE_TWO_HAND_ATTACK_POWER, 1);
                    pPlayer->AddItem(BOW_RANGED_ATTACK_POWER, 1);
                    pPlayer->AddItem(SHOULDER_MAIL_ATTACK_POWER, 1);
                    pPlayer->AddItem(CHEST_MAIL_ATTACK_POWER, 1);


                }
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_HUNTER_DAMAGE_RANGED_1, 1);
                    pPlayer->AddItem(ITEM_HUNTER_DAMAGE_RANGED_2, 1);
                    pPlayer->AddItem(ITEM_HUNTER_DAMAGE_RANGED_3, 1);
                    pPlayer->AddItem(ITEM_HUNTER_DAMAGE_RANGED_4, 1);
                    pPlayer->AddItem(ITEM_HUNTER_DAMAGE_RANGED_5, 1);
                    pPlayer->AddItem(ITEM_MEELE_AXE_TWO_HAND, 1);
                    if(pPlayer->getRace() == RACE_DWARF || pPlayer->getRace() == RACE_TAUREN)
                        pPlayer->AddItem(ITEM_RANGED_GUN, 1);
                    else if(pPlayer->getRace() == RACE_DRAENEI)
                        pPlayer->AddItem(ITEM_RANGED_CROSSBOW, 1);
                    else
                        pPlayer->AddItem(ITEM_RANGED_LONGBOW, 1);
                }
                pPlayer->AddItem(TRINKET_ATTACK_POWER, 1);				
                break;

            // MAGE DAMAGE SPELL
            case MAGE_DAMAGE_SPELL:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_MAGE_DAMAGE_SPELL_1, 1);
                    pPlayer->AddItem(ITEM_MAGE_DAMAGE_SPELL_2, 1);
                    pPlayer->AddItem(ITEM_MAGE_DAMAGE_SPELL_3, 1);
                    pPlayer->AddItem(ITEM_MAGE_DAMAGE_SPELL_4, 1);
                    pPlayer->AddItem(ITEM_MAGE_DAMAGE_SPELL_5, 1);
                    pPlayer->AddItem(ITEM_SPELL_STAFF_DAMAGE, 1);
                    pPlayer->AddItem(ITEM_RANGED_WAND_DAMAGE, 1);
                }
                else
                {
                    pPlayer->AddItem(STAFF_TWO_HAND_SPELL_POWER, 1);
                    pPlayer->AddItem(SHOULDER_CLOTH_SPELL_POWER, 1);
                    pPlayer->AddItem(CHEST_CLOTH_SPELL_POWER, 1);
                }
                pPlayer->AddItem(TRINKET_SPELL_POWER, 1);
                break;

            // PALADIN DAMAGE MEELE
            case PALADIN_DAMAGE_MEELE:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() <= 39 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(SHOULDER_MAIL_ATTACK_POWER, 1);
                    pPlayer->AddItem(CHEST_MAIL_ATTACK_POWER, 1);
                    pPlayer->AddItem(MACE_TWO_HAND_ATTACK_POWER, 1);
                }
                if(pPlayer->getLevel() >= 40 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(SHOULDER_PLATE_ATTACK_POWER, 1);
                    pPlayer->AddItem(CHEST_PLATE_ATTACK_POWER, 1);
                    pPlayer->AddItem(MACE_TWO_HAND_ATTACK_POWER, 1);
                }
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_PALADIN_DAMAGE_MEELE_1, 1);
                    pPlayer->AddItem(ITEM_PALADIN_DAMAGE_MEELE_2, 1);
                    pPlayer->AddItem(ITEM_PALADIN_DAMAGE_MEELE_3, 1);
                    pPlayer->AddItem(ITEM_PALADIN_DAMAGE_MEELE_4, 1);
                    pPlayer->AddItem(ITEM_PALADIN_DAMAGE_MEELE_5, 1);
                    if(pPlayer->getRace() == RACE_HUMAN || pPlayer->getRace() == RACE_DWARF)
                        pPlayer->AddItem(ITEM_MEELE_MACE_TWO_HAND, 1);
                    else 
                        pPlayer->AddItem(ITEM_MEELE_SWORD_TWO_HAND, 1);
                }
                pPlayer->AddItem(TRINKET_ATTACK_POWER, 1);
                break;

            // PALADIN HEAL SPELL
            case PALADIN_HEAL_SPELL:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() <= 39 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(SHOULDER_MAIL_SPELL_POWER, 1);
                    pPlayer->AddItem(CHEST_MAIL_SPELL_POWER, 1);
                    pPlayer->AddItem(MACE_ONE_HAND_SPELL_POWER, 1);
                }
                if(pPlayer->getLevel() >= 40 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(SHOULDER_MAIL_SPELL_POWER, 1);
                    pPlayer->AddItem(CHEST_MAIL_SPELL_POWER, 1);
                    pPlayer->AddItem(MACE_ONE_HAND_SPELL_POWER, 1);
                }
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_PALADIN_HEAL_SPELL_1, 1);
                    pPlayer->AddItem(ITEM_PALADIN_HEAL_SPELL_2, 1);
                    pPlayer->AddItem(ITEM_PALADIN_HEAL_SPELL_3, 1);
                    pPlayer->AddItem(ITEM_PALADIN_HEAL_SPELL_4, 1);
                    pPlayer->AddItem(ITEM_PALADIN_HEAL_SPELL_5, 1);
                    pPlayer->AddItem(ITEM_SPELL_MACE_MAIN, 1);
                    pPlayer->AddItem(ITEM_SPELL_SHIELD_HEAL, 1);
                }
                pPlayer->AddItem(TRINKET_SPELL_POWER, 1);
                break;

            // PRIEST DAMAGE SPELL
            case PRIEST_DAMAGE_SPELL:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_PRIEST_DAMAGE_SPELL_1, 1);
                    pPlayer->AddItem(ITEM_PRIEST_DAMAGE_SPELL_2, 1);
                    pPlayer->AddItem(ITEM_PRIEST_DAMAGE_SPELL_3, 1);
                    pPlayer->AddItem(ITEM_PRIEST_DAMAGE_SPELL_4, 1);
                    pPlayer->AddItem(ITEM_PRIEST_DAMAGE_SPELL_5, 1);
                    pPlayer->AddItem(ITEM_SPELL_STAFF_DAMAGE, 1);
                    pPlayer->AddItem(ITEM_RANGED_WAND_DAMAGE, 1);
                }
                else
                {
                    pPlayer->AddItem(SHOULDER_CLOTH_SPELL_POWER, 1);
                    pPlayer->AddItem(CHEST_CLOTH_SPELL_POWER, 1);
                    pPlayer->AddItem(STAFF_TWO_HAND_SPELL_POWER, 1);
                }
                pPlayer->AddItem(TRINKET_SPELL_POWER, 1);
                break;

            // PRIEST HEAL SPELL
            case PRIEST_HEAL_SPELL:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_PRIEST_HEAL_SPELL_1, 1);
                    pPlayer->AddItem(ITEM_PRIEST_HEAL_SPELL_2, 1);
                    pPlayer->AddItem(ITEM_PRIEST_HEAL_SPELL_3, 1);
                    pPlayer->AddItem(ITEM_PRIEST_HEAL_SPELL_4, 1);
                    pPlayer->AddItem(ITEM_PRIEST_HEAL_SPELL_5, 1);
                    pPlayer->AddItem(ITEM_SPELL_STAFF_HEAL, 1);
                    pPlayer->AddItem(ITEM_RANGED_WAND_HEAL, 1);
                }
                else
                {
                    pPlayer->AddItem(SHOULDER_CLOTH_SPELL_POWER, 1);
                    pPlayer->AddItem(CHEST_CLOTH_SPELL_POWER, 1);
                    pPlayer->AddItem(STAFF_TWO_HAND_SPELL_POWER, 1);
                }
                pPlayer->AddItem(TRINKET_SPELL_POWER, 1);
                break;

            // ROGUE DAMAGE MEELE
            case ROGUE_DAMAGE_MEELE:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_ROGUE_DAMAGE_MEELE_1, 1);
                    pPlayer->AddItem(ITEM_ROGUE_DAMAGE_MEELE_2, 1);
                    pPlayer->AddItem(ITEM_ROGUE_DAMAGE_MEELE_3, 1);
                    pPlayer->AddItem(ITEM_ROGUE_DAMAGE_MEELE_4, 1);
                    pPlayer->AddItem(ITEM_ROGUE_DAMAGE_MEELE_5, 1);
                    pPlayer->AddItem(ITEM_MEELE_DAGGER_MAIN, 1);
                    pPlayer->AddItem(ITEM_MEELE_DAGGER_OFF, 1);
                    pPlayer->AddItem(ITEM_RANGED_THROWN, 1);
                }
                else
                {
                    pPlayer->AddItem(SHOULDER_LEATHER_ATTACK_POWER, 1);
                    pPlayer->AddItem(CHEST_LEATHER_ATTACK_POWER, 1);
                    pPlayer->AddItem(DAGGER_ONE_HAND_ATTACK_POWER, 2);

                }
                pPlayer->AddItem(TRINKET_ATTACK_POWER, 1);
                break;



            // SHAMAN DAMAGE MEELE
            case SHAMAN_DAMAGE_MEELE:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() <= 39 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(AXE_TWO_HAND_ATTACK_POWER, 1);
                    pPlayer->AddItem(SHOULDER_LEATHER_ATTACK_POWER, 1);
                    pPlayer->AddItem(CHEST_LEATHER_ATTACK_POWER, 1);
                }
                if(pPlayer->getLevel() >= 40 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(AXE_TWO_HAND_ATTACK_POWER, 1);
                    pPlayer->AddItem(SHOULDER_MAIL_ATTACK_POWER, 1);
                    pPlayer->AddItem(CHEST_MAIL_ATTACK_POWER, 1);
                }
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_SHAMAN_DAMAGE_MEELE_1, 1);
                    pPlayer->AddItem(ITEM_SHAMAN_DAMAGE_MEELE_2, 1);
                    pPlayer->AddItem(ITEM_SHAMAN_DAMAGE_MEELE_3, 1);
                    pPlayer->AddItem(ITEM_SHAMAN_DAMAGE_MEELE_4, 1);
                    pPlayer->AddItem(ITEM_SHAMAN_DAMAGE_MEELE_5, 1);
                    pPlayer->AddItem(ITEM_MEELE_AXE_MAIN, 1);
                    pPlayer->AddItem(ITEM_MEELE_AXE_OFF, 1);
                }
                pPlayer->AddItem(TRINKET_ATTACK_POWER, 1);
                break;

            // SHAMAN DAMAGE SPELL
            case SHAMAN_DAMAGE_SPELL:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() <= 39 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(MACE_ONE_HAND_SPELL_POWER, 1);
                    pPlayer->AddItem(SHOULDER_LEATHER_SPELL_POWER, 1);
                    pPlayer->AddItem(CHEST_LEATHER_SPELL_POWER, 1);
                }
                if(pPlayer->getLevel() >= 40 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(MACE_ONE_HAND_SPELL_POWER, 1);
                    pPlayer->AddItem(SHOULDER_MAIL_SPELL_POWER, 1);
                    pPlayer->AddItem(CHEST_MAIL_SPELL_POWER, 1);
                }
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_SHAMAN_DAMAGE_SPELL_1, 1);
                    pPlayer->AddItem(ITEM_SHAMAN_DAMAGE_SPELL_2, 1);
                    pPlayer->AddItem(ITEM_SHAMAN_DAMAGE_SPELL_3, 1);
                    pPlayer->AddItem(ITEM_SHAMAN_DAMAGE_SPELL_4, 1);
                    pPlayer->AddItem(ITEM_SHAMAN_DAMAGE_SPELL_5, 1);
                    pPlayer->AddItem(ITEM_SPELL_DAGGER_MAIN, 1);
                    pPlayer->AddItem(ITEM_SPELL_SHIELD_DAMAGE, 1);
                }
                pPlayer->AddItem(TRINKET_SPELL_POWER, 1);
                break;

            // SHAMAN HEAL SPELL
            case SHAMAN_HEAL_SPELL:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() <= 39 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(MACE_ONE_HAND_SPELL_POWER, 1);
                    pPlayer->AddItem(SHOULDER_LEATHER_SPELL_POWER, 1);
                    pPlayer->AddItem(CHEST_LEATHER_SPELL_POWER, 1);
                }
                if(pPlayer->getLevel() >= 40 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(MACE_ONE_HAND_SPELL_POWER, 1);
                    pPlayer->AddItem(SHOULDER_MAIL_SPELL_POWER, 1);
                    pPlayer->AddItem(CHEST_MAIL_SPELL_POWER, 1);
                }
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_SHAMAN_HEAL_SPELL_1, 1);
                    pPlayer->AddItem(ITEM_SHAMAN_HEAL_SPELL_2, 1);
                    pPlayer->AddItem(ITEM_SHAMAN_HEAL_SPELL_3, 1);
                    pPlayer->AddItem(ITEM_SHAMAN_HEAL_SPELL_4, 1);
                    pPlayer->AddItem(ITEM_SHAMAN_HEAL_SPELL_5, 1);
                    pPlayer->AddItem(ITEM_SPELL_MACE_MAIN, 1);
                    pPlayer->AddItem(ITEM_SPELL_SHIELD_HEAL, 1);
                }
                pPlayer->AddItem(TRINKET_SPELL_POWER, 1);
                break;

            // WARLOCK DAMAGE SPELL
            case WARLOCK_DAMAGE_SPELL:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_WARLOCK_DAMAGE_SPELL_1, 1);
                    pPlayer->AddItem(ITEM_WARLOCK_DAMAGE_SPELL_2, 1);
                    pPlayer->AddItem(ITEM_WARLOCK_DAMAGE_SPELL_3, 1);
                    pPlayer->AddItem(ITEM_WARLOCK_DAMAGE_SPELL_4, 1);
                    pPlayer->AddItem(ITEM_WARLOCK_DAMAGE_SPELL_5, 1);
                    pPlayer->AddItem(ITEM_SPELL_STAFF_DAMAGE, 1);
                    pPlayer->AddItem(ITEM_RANGED_WAND_DAMAGE, 1);
                }
                else
                {
                    pPlayer->AddItem(SHOULDER_CLOTH_SPELL_POWER, 1);
                    pPlayer->AddItem(CHEST_CLOTH_SPELL_POWER, 1);
                    pPlayer->AddItem(STAFF_TWO_HAND_SPELL_POWER, 1);
                }
                pPlayer->AddItem(TRINKET_SPELL_POWER, 1);
                break;
    
            // WARRIOR DAMAGE MEELE
            case WARRIOR_DAMAGE_MEELE:
                pPlayer->AddItem(BAG_16_SLOTS, 2);
                if(pPlayer->getLevel() <= 39 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(MACE_TWO_HAND_ATTACK_POWER, 1);
                    pPlayer->AddItem(SHOULDER_MAIL_ATTACK_POWER, 1);
                    pPlayer->AddItem(CHEST_MAIL_ATTACK_POWER, 1);
                }
                if(pPlayer->getLevel() >= 40 && pPlayer->getLevel() != 80)
                {
                    pPlayer->AddItem(MACE_TWO_HAND_ATTACK_POWER, 1);
                    pPlayer->AddItem(SHOULDER_PLATE_ATTACK_POWER, 1);
                    pPlayer->AddItem(CHEST_PLATE_ATTACK_POWER, 1);
                }
                if(pPlayer->getLevel() == 80)
                {
                    pPlayer->AddItem(ITEM_WARRIOR_DAMAGE_MEELE_1, 1);
                    pPlayer->AddItem(ITEM_WARRIOR_DAMAGE_MEELE_2, 1);
                    pPlayer->AddItem(ITEM_WARRIOR_DAMAGE_MEELE_3, 1);
                    pPlayer->AddItem(ITEM_WARRIOR_DAMAGE_MEELE_4, 1);
                    pPlayer->AddItem(ITEM_WARRIOR_DAMAGE_MEELE_5, 1);
                    pPlayer->AddItem(ITEM_RANGED_THROWN, 1);
                    if(pPlayer->getRace() == RACE_ORC || pPlayer->getRace() == RACE_DWARF)
                        pPlayer->AddItem(ITEM_MEELE_AXE_TWO_HAND, 1);
                    else if(pPlayer->getRace() == RACE_TAUREN)
                        pPlayer->AddItem(ITEM_MEELE_MACE_TWO_HAND, 1);
                    else
                        pPlayer->AddItem(ITEM_MEELE_SWORD_TWO_HAND, 1);
                }
                pPlayer->AddItem(TRINKET_ATTACK_POWER, 1);
                break;

            default:
                pCreature->MonsterSay("Nieco sa posralo", LANG_UNIVERSAL, 0);
                break;
        }
		
		CharacterDatabase.PExecute("INSERT INTO character_transfer (guid, npc_transfer) VALUES ('%u', '1')", pPlayer->GetGUID());
		//CharacterDatabase.PExecute("UPDATE character_transfer SET npc_transfer=0 WHERE guid = %u",pPlayer->GetGUID());
        TeleportPlayerAndGiveMoney(pPlayer);		
		
        return true;
    }

    /* PORT */
    void TeleportPlayerAndGiveMoney(Player * pPlayer)
    {
        if (pPlayer->isAlive() && pPlayer->getLevel() >= 1 && pPlayer->getLevel() <= 10)
        {
            if(pPlayer->GetTeamId() == TEAM_ALLIANCE)
            {
                pPlayer->ModifyMoney(50000);
                pPlayer->TeleportTo(0, -8830.18f, 623.96f, 93.89f, 3.8f, 0);
                pPlayer->CLOSE_GOSSIP_MENU();
            }
            else
            {
                pPlayer->ModifyMoney(50000);
                pPlayer->TeleportTo(1, 1629.36f, -4373.39f, 31.26f, 3.5f, 0);
                pPlayer->CLOSE_GOSSIP_MENU();
            }
        }

        if (pPlayer->isAlive() && pPlayer->getLevel() >= 11 && pPlayer->getLevel() <= 20)
        {
            if(pPlayer->GetTeamId() == TEAM_ALLIANCE)
            {
                pPlayer->ModifyMoney(250000);
                pPlayer->TeleportTo(0, -8830.18f, 623.96f, 93.89f, 3.8f, 0);
                pPlayer->CLOSE_GOSSIP_MENU();
            }
            else
            {
                pPlayer->ModifyMoney(250000);
                pPlayer->TeleportTo(1, 1629.36f, -4373.39f, 31.26f, 3.5f, 0);
                pPlayer->CLOSE_GOSSIP_MENU();
            }
        }

        if (pPlayer->isAlive() && pPlayer->getLevel() >= 21 && pPlayer->getLevel() <= 30)
        {
            if(pPlayer->GetTeamId() == TEAM_ALLIANCE)
            {
                pPlayer->ModifyMoney(1000000);
                pPlayer->TeleportTo(0, -8830.18f, 623.96f, 93.89f, 3.8f, 0);
                pPlayer->CLOSE_GOSSIP_MENU();
            }
            else
            {
                pPlayer->ModifyMoney(1000000);
                pPlayer->TeleportTo(1, 1629.36f, -4373.39f, 31.26f, 3.5f, 0);
                pPlayer->CLOSE_GOSSIP_MENU();
            }
        }

        if (pPlayer->isAlive() && pPlayer->getLevel() >= 31 && pPlayer->getLevel() <= 40)
        {
            if(pPlayer->GetTeamId() == TEAM_ALLIANCE)
            {
                pPlayer->ModifyMoney(2500000);
                pPlayer->TeleportTo(0, -8830.18f, 623.96f, 93.89f, 3.8f, 0);
                pPlayer->CLOSE_GOSSIP_MENU();
            }
            else
            {
                pPlayer->ModifyMoney(2500000);
                pPlayer->TeleportTo(1, 1629.36f, -4373.39f, 31.26f, 3.5f, 0);
                pPlayer->CLOSE_GOSSIP_MENU();
            }
        }

        if (pPlayer->isAlive() && pPlayer->getLevel() >= 41 && pPlayer->getLevel() <= 50)
        {
            if(pPlayer->GetTeamId() == TEAM_ALLIANCE)
            {
                pPlayer->ModifyMoney(5000000);
                pPlayer->TeleportTo(0, -8830.18f, 623.96f, 93.89f, 3.8f, 0);
                pPlayer->CLOSE_GOSSIP_MENU();
            }
            else
            {
                pPlayer->ModifyMoney(5000000);
                pPlayer->TeleportTo(1, 1629.36f, -4373.39f, 31.26f, 3.5f, 0);
                pPlayer->CLOSE_GOSSIP_MENU();
            }
        }

        if (pPlayer->isAlive() && pPlayer->getLevel() >= 51 && pPlayer->getLevel() <= 60)
        {
            if(pPlayer->GetTeamId() == TEAM_ALLIANCE)
            {
                pPlayer->ModifyMoney(10000000);
                pPlayer->TeleportTo(0, -8830.18f, 623.96f, 93.89f, 3.8f, 0);
                pPlayer->CLOSE_GOSSIP_MENU();
            }
            else
            {
                pPlayer->ModifyMoney(10000000);
                pPlayer->TeleportTo(1, 1629.36f, -4373.39f, 31.26f, 3.5f, 0);
                pPlayer->CLOSE_GOSSIP_MENU();
            }
        }

        if (pPlayer->isAlive() && pPlayer->getLevel() >= 61 && pPlayer->getLevel() <= 70)
        {
            pPlayer->ModifyMoney(40000000);
            pPlayer->TeleportTo(530, -1815.10f, 5333.19f, -12.43f, 1.24f, 0);
            pPlayer->CLOSE_GOSSIP_MENU();
        }

        if (pPlayer->isAlive() && pPlayer->getLevel() >= 71 && pPlayer->getLevel() <= 80)
        {
            pPlayer->ModifyMoney(90000000);
            pPlayer->TeleportTo(571, 5778.92f, 607.65f, 565.30f, 4.04f, 0);
            pPlayer->CLOSE_GOSSIP_MENU();
        }
    }
};

void AddSC_custom_npc()
{
    new custom_npc();
}

/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#include "ScriptPCH.h"

enum Spells
{
    SPELL_TRESPASSER_A = 54028,
    SPELL_TRESPASSER_H = 54029
};

enum NPCs // All outdoor guards are within 35.0f of these NPCs
{
    NPC_APPLEBOUGH_A = 29547,
    NPC_SWEETBERRY_H = 29715,
};

class npc_mageguard_dalaran : public CreatureScript
{
public:
    npc_mageguard_dalaran() : CreatureScript("npc_mageguard_dalaran") { }

    struct npc_mageguard_dalaranAI : public Scripted_NoMovementAI
    {
        npc_mageguard_dalaranAI(Creature* pCreature): Scripted_NoMovementAI(pCreature)
        {
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pCreature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_NORMAL, true);
            pCreature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
        }

        void Reset(){ }

        void EnterCombat(Unit* /*pWho*/){ }

        void AttackStart(Unit* /*pWho*/){ }

        void MoveInLineOfSight(Unit* who)
        {
            if(!who || !who->IsInWorld() || who->GetZoneId() != 4395)
                return;

            if(!me->IsWithinDist(who, 65.0f, false))
                return;

            Player* pPlayer = who->GetCharmerOrOwnerPlayerOrPlayerItself();

            if(!pPlayer || pPlayer->isGameMaster() || pPlayer->IsBeingTeleported() || pPlayer->HasAura(70971) || pPlayer->HasAura(70972) || pPlayer->HasAura(70973) || pPlayer->HasAura(70974))
                return;

            switch(me->GetEntry())
            {
                case 29254:
                    if(pPlayer->GetTeam() == HORDE)              // Horde unit found in Alliance area
                    {
                        if(GetClosestCreatureWithEntry(me, NPC_APPLEBOUGH_A, 32.0f))
                        {
                            if(me->isInBackInMap(who, 12.0f))   // In my line of sight, "outdoors", and behind me
                                DoCast(who, SPELL_TRESPASSER_A); // Teleport the Horde unit out
                        }
                        else                                      // In my line of sight, and "indoors"
                            DoCast(who, SPELL_TRESPASSER_A);     // Teleport the Horde unit out
                    }
                    break;
                case 29255:
                    if(pPlayer->GetTeam() == ALLIANCE)           // Alliance unit found in Horde area
                    {
                        if(GetClosestCreatureWithEntry(me, NPC_SWEETBERRY_H, 32.0f))
                        {
                            if(me->isInBackInMap(who, 12.0f))   // In my line of sight, "outdoors", and behind me
                                DoCast(who, SPELL_TRESPASSER_H); // Teleport the Alliance unit out
                        }
                        else                                      // In my line of sight, and "indoors"
                            DoCast(who, SPELL_TRESPASSER_H);     // Teleport the Alliance unit out
                    }
                    break;
            }
            me->SetOrientation(me->GetHomePosition().GetOrientation());
            return;
        }

        void UpdateAI(const uint32 /*diff*/){ }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mageguard_dalaranAI(pCreature);
    }
};

enum eHiraSnowdawn
{
    SPELL_COLD_WEATHER_FLYING                   = 54197
};

#define GOSSIP_TEXT_TRAIN_HIRA "Je souhaite un entrainement pour monter à cheval."

class npc_hira_snowdawn : public CreatureScript
{
public:
    npc_hira_snowdawn() : CreatureScript("npc_hira_snowdawn") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(!creature->isVendor() || !creature->isTrainer())
            return false;

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_TRAIN_HIRA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRAIN);

        if(player->getLevel() >= 80 && player->HasSpell(SPELL_COLD_WEATHER_FLYING))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        if(uiAction == GOSSIP_ACTION_TRAIN)
            player->GetSession()->SendTrainerList(creature->GetGUID());

        if(uiAction == GOSSIP_ACTION_TRADE)
            player->GetSession()->SendListInventory(creature->GetGUID());

        return true;
    }
};

enum eShits
{
    QUEST_A_MEETING_WITH_THE_MAGISTER   = 20439,
    QUEST_AN_AUDIENCE_WITH_THE_ARCANIST = 24451,

    ITEM_QUEST                          = 49698,
};

enum eArcanistTybalin
{
    SPELL_SUNREAVER_DISGUISE_F        = 70973,
    SPELL_SUNREAVER_DISGUISE_M        = 70974
};

class npc_arcanist_tybalin : public CreatureScript
{
public:
    npc_arcanist_tybalin() : CreatureScript("npc_arcanist_tybalin") { }

    bool OnGossipHello(Player* pPlayer, Creature* /*pCreature*/)
    {
        if(pPlayer->IsActiveQuest( QUEST_AN_AUDIENCE_WITH_THE_ARCANIST ) && pPlayer->GetQuestStatus( QUEST_AN_AUDIENCE_WITH_THE_ARCANIST ) != QUEST_STATUS_COMPLETE )
        {
            ItemPosCountVec dest;
            uint32 count = 1, no_space_count = 0;
            uint8 msg = pPlayer->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, ITEM_QUEST, count, &no_space_count );

            if(msg == EQUIP_ERR_ITEM_NOT_FOUND ) {
                sLog->outErrorDb( "[src/server/scripts/Northrend/dalaran.cpp] Item (Entry %u) not exist in `item_template`.", ITEM_QUEST );
                return false;
            }

            if(msg != EQUIP_ERR_OK ) // convert to possible store amount
                count -= no_space_count;

            if(count != 0 && !dest.empty( ) ) // can add some
                if(Item* item = pPlayer->StoreNewItem( dest, ITEM_QUEST, true, 0 ) )
                    pPlayer->SendNewItem( item, count, true, false );
        }

        if(pPlayer->GetQuestStatus( QUEST_A_MEETING_WITH_THE_MAGISTER ) == QUEST_STATUS_COMPLETE )
        {
            if(pPlayer->HasAura( SPELL_SUNREAVER_DISGUISE_F ) )
                pPlayer->RemoveAurasDueToSpell( SPELL_SUNREAVER_DISGUISE_F );

            if(pPlayer->HasAura( SPELL_SUNREAVER_DISGUISE_M ) )
                pPlayer->RemoveAurasDueToSpell( SPELL_SUNREAVER_DISGUISE_M );
        }

        return false;
    }

    bool OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const *pQuest)
    {
        if(pQuest->GetQuestId( ) == QUEST_A_MEETING_WITH_THE_MAGISTER )
        {
            pCreature->CastSpell( pPlayer, ( pPlayer->getGender( ) == GENDER_MALE ) ? SPELL_SUNREAVER_DISGUISE_M : SPELL_SUNREAVER_DISGUISE_F, true );

            return true;
        }
        return false;
    }
};

enum eMagisterHathorel
{
    SPELL_SILVER_COVENANT_DISGUISE_F    = 70971,
    SPELL_SILVER_COVENANT_DISGUISE_M    = 70972
};

class npc_magister_hathorel : public CreatureScript
{
public:
    npc_magister_hathorel() : CreatureScript("npc_magister_hathorel") { }

    bool OnGossipHello(Player* pPlayer, Creature* /*pCreature*/)
    {
        if(pPlayer->IsActiveQuest( QUEST_A_MEETING_WITH_THE_MAGISTER ) && pPlayer->GetQuestStatus( QUEST_A_MEETING_WITH_THE_MAGISTER ) != QUEST_STATUS_COMPLETE )
        {
            ItemPosCountVec dest;
            uint32 count = 1, no_space_count = 0;
            uint8 msg = pPlayer->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, ITEM_QUEST, count, &no_space_count );

            if(msg == EQUIP_ERR_ITEM_NOT_FOUND ) {
                sLog->outErrorDb( "[src/server/scripts/Northrend/dalaran.cpp] Item (Entry %u) not exist in `item_template`.", ITEM_QUEST );
                return false;
            }

            if(msg != EQUIP_ERR_OK ) // convert to possible store amount
                count -= no_space_count;

            if(count != 0 && !dest.empty( ) ) // can add some
                if(Item* item = pPlayer->StoreNewItem( dest, ITEM_QUEST, true, 0 ) )
                    pPlayer->SendNewItem( item, count, true, false );
        }

        if(pPlayer->GetQuestStatus( QUEST_AN_AUDIENCE_WITH_THE_ARCANIST ) == QUEST_STATUS_COMPLETE )
        {
            if(pPlayer->HasAura( SPELL_SILVER_COVENANT_DISGUISE_F ) )
                 pPlayer->RemoveAurasDueToSpell( SPELL_SILVER_COVENANT_DISGUISE_F );

            if(pPlayer->HasAura( SPELL_SILVER_COVENANT_DISGUISE_M ) )
                 pPlayer->RemoveAurasDueToSpell( SPELL_SILVER_COVENANT_DISGUISE_M );
        }

        return false;
    }

    bool OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const *pQuest)
    {
        if(pQuest->GetQuestId( ) == QUEST_AN_AUDIENCE_WITH_THE_ARCANIST )
        {
            pCreature->CastSpell( pPlayer, ( pPlayer->getGender( ) == GENDER_MALE ) ? SPELL_SILVER_COVENANT_DISGUISE_M : SPELL_SILVER_COVENANT_DISGUISE_F, true );

            return true;
        }
        return false;
    }
};

/*######
## go_memorial_plaque
######*/

enum GOSSIPS
{
    EPIC_FAIL,
    GOSSIP_TEXT_SEE_FALL_OF_THE_LICH_KING = 12001,
};

#define GOSSIP_MESSAGE_MEMORIAL                    15921

class go_memorial_plaque : public GameObjectScript
{
public: 
    go_memorial_plaque() : GameObjectScript("go_memorial_plaque") { }
    
    bool OnGossipHello(Player *player, GameObject *go)
    {
        WorldSession* session = player->GetSession();
        
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, session->GetTrinityString(GOSSIP_TEXT_SEE_FALL_OF_THE_LICH_KING), GOSSIP_SENDER_MAIN, EPIC_FAIL);
        player->SEND_GOSSIP_MENU(GOSSIP_MESSAGE_MEMORIAL, go->GetGUID());
        
        return true;
    }
    
    bool OnGossipSelect(Player *player, GameObject *go, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        switch(uiAction)
        {
            case EPIC_FAIL:
                player->SendMovieStart(16);
                player->CLOSE_GOSSIP_MENU(); 
                break;
        }
        
        return true;
    }
};

void AddSC_dalaran()
{
    new npc_mageguard_dalaran;
    new npc_hira_snowdawn;
    new npc_arcanist_tybalin;
    new npc_magister_hathorel;
    new go_memorial_plaque;
}

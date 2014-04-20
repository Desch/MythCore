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
#include "Spell.h"

class item_rune_activation_device : public ItemScript
{
public:
    item_rune_activation_device() : ItemScript("item_rune_activation_device") { }

    bool OnUse(Player* pPlayer, Item* /*pItem*/, SpellCastTargets const& /*pTargets*/)
    {
        if(pPlayer->GetQuestStatus(10240) == QUEST_STATUS_INCOMPLETE)
        {
            Creature* pCreature;
            pCreature = pPlayer->FindNearestCreature(19866, 5.0f);
            if(pCreature)
            {
                pPlayer->KilledMonsterCredit(19866, 0);
                return true;
            }

            pCreature = pPlayer->FindNearestCreature(19867, 5.0f);
            if(pCreature)
            {
                pPlayer->KilledMonsterCredit(19867, 0);
                return true;
            }

            pCreature = pPlayer->FindNearestCreature(19868, 5.0f);
            if(pCreature)
            {
                pPlayer->KilledMonsterCredit(19868, 0);
                return true;
            }
        }
        return true;
    }
};

enum eSurveyingMarkers
{
    QUEST_SURVEYING_THE_RUINS = 10335,

    NPC_ONE                   = 20473,
    NPC_TWO                   = 20475,
    NPC_THREE                 = 20476
};

class item_surveying_markers : public ItemScript
{
public:
    item_surveying_markers() : ItemScript("item_surveying_markers") { }

    bool OnUse(Player* pPlayer, Item* /*pItem*/, SpellCastTargets const& /*targets*/)
    {
        if(pPlayer->GetQuestStatus(QUEST_SURVEYING_THE_RUINS) == QUEST_STATUS_INCOMPLETE)
        {
            Creature* pCreature;
            pCreature = pPlayer->FindNearestCreature(NPC_ONE, 10);
            if(pCreature)
            {
                pPlayer->KilledMonsterCredit(NPC_ONE, 0);
                return true;
            }

            pCreature = pPlayer->FindNearestCreature(NPC_TWO, 10);
            if(pCreature)
            {
                pPlayer->KilledMonsterCredit(NPC_TWO, 0);
                return true;
            }

            pCreature = pPlayer->FindNearestCreature(NPC_THREE, 10);
            if(pCreature)
            {
                pPlayer->KilledMonsterCredit(NPC_THREE, 0);
                return true;
            }
        }
        return true;
    }
};

enum eOnlyForFlight
{
    SPELL_ARCANE_CHARGES    = 45072
};

class item_only_for_flight : public ItemScript
{
public:
    item_only_for_flight() : ItemScript("item_only_for_flight") { }

    bool OnUse(Player* player, Item* pItem, SpellCastTargets const& /*targets*/)
    {
        uint32 itemId = pItem->GetEntry();
        bool disabled = false;

        //for special scripts
        switch(itemId)
        {
            case 24538:
                if(player->GetAreaId() != 3628)
                    disabled = true;
                break;
            case 34489:
                if(player->GetZoneId() != 4080)
                    disabled = true;
                break;
            case 34475:
                if(const SpellEntry* pSpellInfo = GetSpellStore()->LookupEntry(SPELL_ARCANE_CHARGES))
                    Spell::SendCastResult(player, pSpellInfo, 1, SPELL_FAILED_NOT_ON_GROUND);
                break;
        }

        // allow use in flight only
        if(player->isInFlight() && !disabled)
            return false;

        // error
        player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, pItem, NULL);
        return true;
    }
};

class item_draenei_fishing_net : public ItemScript
{
public:
    item_draenei_fishing_net() : ItemScript("item_draenei_fishing_net") { }

    //This is just a hack and should be removed from here.
    //Creature/Item are in fact created before spell are sucessfully casted, without any checks at all to ensure proper/expected behavior.
    bool OnUse(Player* player, Item* /*pItem*/, SpellCastTargets const& /*targets*/)
    {
        if(player->GetQuestStatus(9452) == QUEST_STATUS_INCOMPLETE)
        {
            if(urand(0, 99) < 35)
            {
                Creature* Murloc = player->SummonCreature(17102, player->GetPositionX(), player->GetPositionY()+20, player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                if(Murloc)
                    Murloc->AI()->AttackStart(player);
            }
            else
            {
                ItemPosCountVec dest;
                uint32 itemId = 23614;
                InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, 1);
                if(msg == EQUIP_ERR_OK)
                {
                    if(Item* item = player->StoreNewItem(dest, itemId, true))
                        player->SendNewItem(item, 1, false, true);
                }
                else
                    player->SendEquipError(msg, NULL, NULL, itemId);
            }
        }
        return false;
    }
};

class item_nether_wraith_beacon : public ItemScript
{
public:
    item_nether_wraith_beacon() : ItemScript("item_nether_wraith_beacon") { }

    bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/)
    {
        if(player->GetQuestStatus(10832) == QUEST_STATUS_INCOMPLETE)
        {
            if(Creature* nether = player->SummonCreature(22408, player->GetPositionX(), player->GetPositionY()+20, player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 180000))
                nether->AI()->AttackStart(player);
            if(Creature* nether = player->SummonCreature(22408, player->GetPositionX(), player->GetPositionY()-20, player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 180000))
                nether->AI()->AttackStart(player);
        }
        return false;
    }
};

class item_gor_dreks_ointment : public ItemScript
{
public:
    item_gor_dreks_ointment() : ItemScript("item_gor_dreks_ointment") { }

    bool OnUse(Player* player, Item* pItem, SpellCastTargets const& targets)
    {
        if(targets.GetUnitTarget() && targets.GetUnitTarget()->GetTypeId() == TYPEID_UNIT &&
            targets.GetUnitTarget()->GetEntry() == 20748 && !targets.GetUnitTarget()->HasAura(32578))
            return false;

        player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, pItem, NULL);
        return true;
    }
};

class item_incendiary_explosives : public ItemScript
{
public:
    item_incendiary_explosives() : ItemScript("item_incendiary_explosives") { }

    bool OnUse(Player* player, Item* pItem, SpellCastTargets const & /*targets*/)
    {
        if(player->FindNearestCreature(26248, 15) || player->FindNearestCreature(26249, 15))
            return false;
        else
        {
            player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, pItem, NULL);
            return true;
        }
    }
};

class item_mysterious_egg : public ItemScript
{
public:
    item_mysterious_egg() : ItemScript("item_mysterious_egg") { }
    bool OnExpire(Player* player, ItemTemplate const* /*pItemProto*/)
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 39883, 1); // Cracked Egg
        if(msg == EQUIP_ERR_OK)
            player->StoreNewItem(dest, 39883, true, Item::GenerateItemRandomPropertyId(39883));

        return true;
    }
};

class item_disgusting_jar : public ItemScript
{
public:
    item_disgusting_jar() : ItemScript("item_disgusting_jar") { }

    bool OnExpire(Player* player, ItemTemplate const* /*pItemProto*/)
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 44718, 1); // Ripe Disgusting Jar
        if(msg == EQUIP_ERR_OK)
            player->StoreNewItem(dest, 44718, true, Item::GenerateItemRandomPropertyId(44718));

        return true;
    }
};

enum ePileFakeFur
{
    GO_CARIBOU_TRAP_1                                      = 187982,
    GO_CARIBOU_TRAP_2                                      = 187995,
    GO_CARIBOU_TRAP_3                                      = 187996,
    GO_CARIBOU_TRAP_4                                      = 187997,
    GO_CARIBOU_TRAP_5                                      = 187998,
    GO_CARIBOU_TRAP_6                                      = 187999,
    GO_CARIBOU_TRAP_7                                      = 188000,
    GO_CARIBOU_TRAP_8                                      = 188001,
    GO_CARIBOU_TRAP_9                                      = 188002,
    GO_CARIBOU_TRAP_10                                     = 188003,
    GO_CARIBOU_TRAP_11                                     = 188004,
    GO_CARIBOU_TRAP_12                                     = 188005,
    GO_CARIBOU_TRAP_13                                     = 188006,
    GO_CARIBOU_TRAP_14                                     = 188007,
    GO_CARIBOU_TRAP_15                                     = 188008,
    GO_HIGH_QUALITY_FUR                                    = 187983,
    NPC_NESINGWARY_TRAPPER                                 = 25835
};

#define CaribouTrapsNum 15
const uint32 CaribouTraps[CaribouTrapsNum] =
{
    GO_CARIBOU_TRAP_1, GO_CARIBOU_TRAP_2, GO_CARIBOU_TRAP_3, GO_CARIBOU_TRAP_4, GO_CARIBOU_TRAP_5,
    GO_CARIBOU_TRAP_6, GO_CARIBOU_TRAP_7, GO_CARIBOU_TRAP_8, GO_CARIBOU_TRAP_9, GO_CARIBOU_TRAP_10,
    GO_CARIBOU_TRAP_11, GO_CARIBOU_TRAP_12, GO_CARIBOU_TRAP_13, GO_CARIBOU_TRAP_14, GO_CARIBOU_TRAP_15,
};

class item_pile_fake_furs : public ItemScript
{
public:
    item_pile_fake_furs() : ItemScript("item_pile_fake_furs") { }

    bool OnUse(Player* player, Item* /*pItem*/, SpellCastTargets const & /*targets*/)
    {
        GameObject* pGo = NULL;
        for(uint8 i = 0; i < CaribouTrapsNum; ++i)
        {
            pGo = player->FindNearestGameObject(CaribouTraps[i], 5.0f);
            if(pGo)
                break;
        }

        if(!pGo)
            return false;

        if(pGo->FindNearestCreature(NPC_NESINGWARY_TRAPPER, 10.0f, true) || pGo->FindNearestCreature(NPC_NESINGWARY_TRAPPER, 10.0f, false) || pGo->FindNearestGameObject(GO_HIGH_QUALITY_FUR, 2.0f))
            return true;

        float x, y, z;
        pGo->GetClosePoint(x, y, z, pGo->GetObjectSize() / 3, 7.0f);
        pGo->SummonGameObject(GO_HIGH_QUALITY_FUR, pGo->GetPositionX(), pGo->GetPositionY(), pGo->GetPositionZ(), 0, 0, 0, 0, 0, 1000);
        if(TempSummon* summon = player->SummonCreature(NPC_NESINGWARY_TRAPPER, x, y, z, pGo->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 1000))
        {
            summon->SetVisible(false);
            summon->SetReactState(REACT_PASSIVE);
            summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        }
        return false;
    }
};

enum ePetrovClusterBombs
{
    SPELL_PETROV_BOMB           = 42406,
    AREA_ID_SHATTERED_STRAITS   = 4064,
    ZONE_ID_HOWLING             = 495
};

class item_petrov_cluster_bombs : public ItemScript
{
public:
    item_petrov_cluster_bombs() : ItemScript("item_petrov_cluster_bombs") { }

    bool OnUse(Player* player, Item* pItem, const SpellCastTargets & /*pTargets*/)
    {
        if(player->GetZoneId() != ZONE_ID_HOWLING)
            return false;

        if(!player->GetTransport() || player->GetAreaId() != AREA_ID_SHATTERED_STRAITS)
        {
            player->SendEquipError(EQUIP_ERR_NONE, pItem, NULL);

            if(const SpellEntry* pSpellInfo = GetSpellStore()->LookupEntry(SPELL_PETROV_BOMB))
                Spell::SendCastResult(player, pSpellInfo, 1, SPELL_FAILED_NOT_HERE);

            return true;
        }

        return false;
    }
};

enum eHelpThemselves
{
    QUEST_CANNOT_HELP_THEMSELVES    =  11876,
    NPC_TRAPPED_MAMMOTH_CALF        =  25850,
    GO_MAMMOTH_TRAP_1               = 188022,
    GO_MAMMOTH_TRAP_2               = 188024,
    GO_MAMMOTH_TRAP_3               = 188025,
    GO_MAMMOTH_TRAP_4               = 188026,
    GO_MAMMOTH_TRAP_5               = 188027,
    GO_MAMMOTH_TRAP_6               = 188028,
    GO_MAMMOTH_TRAP_7               = 188029,
    GO_MAMMOTH_TRAP_8               = 188030,
    GO_MAMMOTH_TRAP_9               = 188031,
    GO_MAMMOTH_TRAP_10              = 188032,
    GO_MAMMOTH_TRAP_11              = 188033,
    GO_MAMMOTH_TRAP_12              = 188034,
    GO_MAMMOTH_TRAP_13              = 188035,
    GO_MAMMOTH_TRAP_14              = 188036,
    GO_MAMMOTH_TRAP_15              = 188037,
    GO_MAMMOTH_TRAP_16              = 188038,
    GO_MAMMOTH_TRAP_17              = 188039,
    GO_MAMMOTH_TRAP_18              = 188040,
    GO_MAMMOTH_TRAP_19              = 188041,
    GO_MAMMOTH_TRAP_20              = 188042,
    GO_MAMMOTH_TRAP_21              = 188043,
    GO_MAMMOTH_TRAP_22              = 188044,
};

#define MammothTrapsNum 22
const uint32 MammothTraps[MammothTrapsNum] =
{
    GO_MAMMOTH_TRAP_1, GO_MAMMOTH_TRAP_2, GO_MAMMOTH_TRAP_3, GO_MAMMOTH_TRAP_4, GO_MAMMOTH_TRAP_5,
    GO_MAMMOTH_TRAP_6, GO_MAMMOTH_TRAP_7, GO_MAMMOTH_TRAP_8, GO_MAMMOTH_TRAP_9, GO_MAMMOTH_TRAP_10,
    GO_MAMMOTH_TRAP_11, GO_MAMMOTH_TRAP_12, GO_MAMMOTH_TRAP_13, GO_MAMMOTH_TRAP_14, GO_MAMMOTH_TRAP_15,
    GO_MAMMOTH_TRAP_16, GO_MAMMOTH_TRAP_17, GO_MAMMOTH_TRAP_18, GO_MAMMOTH_TRAP_19, GO_MAMMOTH_TRAP_20,
    GO_MAMMOTH_TRAP_21, GO_MAMMOTH_TRAP_22
};

class item_dehta_trap_smasher : public ItemScript
{
public:
    item_dehta_trap_smasher() : ItemScript("item_dehta_trap_smasher") { }

    bool OnUse(Player* player, Item* /*pItem*/, const SpellCastTargets & /*pTargets*/)
    {
        if(player->GetQuestStatus(QUEST_CANNOT_HELP_THEMSELVES) != QUEST_STATUS_INCOMPLETE)
            return false;

        Creature* pMammoth;
        pMammoth = player->FindNearestCreature(NPC_TRAPPED_MAMMOTH_CALF, 5.0f);
        if(!pMammoth)
            return false;

        GameObject* pTrap;
        for(uint8 i = 0; i < MammothTrapsNum; ++i)
        {
            pTrap = player->FindNearestGameObject(MammothTraps[i], 11.0f);
            if(pTrap)
            {
                pMammoth->AI()->DoAction(1);
                pTrap->SetGoState(GO_STATE_READY);
                player->KilledMonsterCredit(NPC_TRAPPED_MAMMOTH_CALF, 0);
                return true;
            }
        }
        return false;
    }
};

enum TheEmissary
{
    QUEST_THE_EMISSARY      =   11626,
    NPC_LEVIROTH            =   26452
};

class item_trident_of_nazjan : public ItemScript
{
public:
    item_trident_of_nazjan() : ItemScript("item_Trident_of_Nazjan") { }

    bool OnUse(Player* player, Item* pItem, const SpellCastTargets & /*pTargets*/)
    {
        if(player->GetQuestStatus(QUEST_THE_EMISSARY) == QUEST_STATUS_INCOMPLETE)
        {
            if(Creature* pLeviroth = player->FindNearestCreature(NPC_LEVIROTH, 10.0f)) // spell range
            {
                pLeviroth->AI()->AttackStart(player);
                return false;
            } else
                player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, pItem, NULL);
        } else
            player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, pItem, NULL);
        return true;
    }
};

enum eCapturedFrog
{
    QUEST_THE_PERFECT_SPIES      = 25444,
    NPC_VANIRAS_SENTRY_TOTEM     = 40187
};

class item_captured_frog : public ItemScript
{
public:
    item_captured_frog() : ItemScript("item_captured_frog") { }

    bool OnUse(Player* player, Item* pItem, SpellCastTargets const& /*targets*/)
    {
        if(player->GetQuestStatus(QUEST_THE_PERFECT_SPIES) == QUEST_STATUS_INCOMPLETE)
        {
            if(player->FindNearestCreature(NPC_VANIRAS_SENTRY_TOTEM, 10.0f))
                return false;
            else
                player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, pItem, NULL);
        }
        else
            player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, pItem, NULL);
        return true;
    }
};

enum eBrewfestSampler
{
    QUEST_CHUG_AND_CHUCK_ALLIANCE = 12022,
    QUEST_CHUG_AND_CHUCK_HORDE    = 12191,

    NPC_STOUT                     = 24108
};

class item_complimentary_brewfest_sampler : public ItemScript
{
public:
    item_complimentary_brewfest_sampler() : ItemScript("item_complimentary_brewfest_sampler") { }

    bool OnUse(Player* pPlayer, Item* /*pItem*/, SpellCastTargets const& /*targets*/)
    {
        if(pPlayer->GetQuestStatus(QUEST_CHUG_AND_CHUCK_ALLIANCE) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(QUEST_CHUG_AND_CHUCK_HORDE) == QUEST_STATUS_INCOMPLETE)
        {
            Creature* pCreature = pPlayer->FindNearestCreature(NPC_STOUT, 25);
            if(pCreature)
            {
                pPlayer->DestroyItemCount(33096, -1, true, false);
                pPlayer->KilledMonsterCredit(NPC_STOUT, 0);
                return true;
            }
        }
        return true;
    }
};

class item_tricky_treat : public ItemScript
{
public:
    item_tricky_treat() : ItemScript("item_tricky_treat") { }

    bool OnUse(Player* pPlayer, Item* /*pItem*/, SpellCastTargets const& /*targets*/)
    {
        pPlayer->CastSpell(pPlayer, 42965, true);
        pPlayer->DestroyItemCount(33226, 1, true, false);
        if(roll_chance_i(15))
            pPlayer->CastSpell(pPlayer, 42966, false);
        return true;
    }
};

class item_titanium_seal_of_dalaran : public ItemScript
{
public:
    item_titanium_seal_of_dalaran() : ItemScript("item_titanium_seal_of_dalaran") { }

    bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
    {
        if(pPlayer)
        {
            pPlayer->CastSpell(pPlayer, 60458, true);
            pPlayer->TextEmote("throws into the air its Titanium print Dalaran");
            switch(irand(0, 1))
            {
                case 1:
                    pPlayer->TextEmote("eagle");
                    break;
                case 0:
                    pPlayer->TextEmote("tail");
                    break;
            }
        }
        return false;
    }
};

void AddSC_item_scripts()
{
    new item_surveying_markers;
    new item_rune_activation_device;
    new item_only_for_flight;
    new item_draenei_fishing_net;
    new item_nether_wraith_beacon;
    new item_gor_dreks_ointment;
    new item_incendiary_explosives;
    new item_mysterious_egg;
    new item_disgusting_jar;
    new item_pile_fake_furs;
    new item_petrov_cluster_bombs;
    new item_dehta_trap_smasher;
    new item_trident_of_nazjan;
    new item_captured_frog;
    new item_complimentary_brewfest_sampler;
    new item_tricky_treat;
    new item_titanium_seal_of_dalaran;
}

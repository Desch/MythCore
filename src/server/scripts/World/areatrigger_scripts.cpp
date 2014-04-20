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

enum eDrakeHunt
{
    SPELL_DRAKE_HATCHLING_SUBDUED = 46691,
    QUEST_DRAKE_HUNT              = 11919,
    NPC_DRAKE                     = 26127
};

class AreaTrigger_at_drake_hunt : public AreaTriggerScript
{
    public:
        AreaTrigger_at_drake_hunt() : AreaTriggerScript("at_drake_hunt") { }

        bool OnTrigger(Player* pPlayer, AreaTriggerEntry const* /*pTrigger*/)
        {
            Creature* pCreature = pPlayer->FindNearestCreature(NPC_DRAKE, 50);
            if(pPlayer->GetQuestStatus(QUEST_DRAKE_HUNT) == QUEST_STATUS_INCOMPLETE && pPlayer->HasAura(SPELL_DRAKE_HATCHLING_SUBDUED) && pCreature)
            {
                pPlayer->KilledMonsterCredit(26175, 0);
                pCreature->DespawnOrUnsummon();
            }

            return false;
        }
};

class AreaTrigger_at_quest_the_end_of_the_line : public AreaTriggerScript
{
    public:
        AreaTrigger_at_quest_the_end_of_the_line() : AreaTriggerScript("at_quest_the_end_of_the_line") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*pTrigger*/)
        {
            if(player->GetQuestStatus(12107) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(12110) == QUEST_STATUS_INCOMPLETE)
                player->KilledMonsterCredit(26889, 0);

            return false;
        }
};

enum eBlendingIn
{
    QUEST_BLENDING_IN           = 11633,

    SPELL_SHROUD_OF_THE_SCOURGE = 45614,

    AT_DECAY                    = 4857,
    AT_BLOOD                    = 4858,
    AT_PAIN                     = 4860,

    NPC_DECAY                   = 25471,
    NPC_BLOOD                   = 25472,
    NPC_PAIN                    = 25473
};

class AreaTrigger_at_blending_in : public AreaTriggerScript
{
    public:
        AreaTrigger_at_blending_in() : AreaTriggerScript("at_blending_in") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger)
        {
            if(player->GetQuestStatus(QUEST_BLENDING_IN) == QUEST_STATUS_INCOMPLETE && player->HasAura(SPELL_SHROUD_OF_THE_SCOURGE))
            {
                switch(trigger->id)
                {
                    case AT_DECAY:
                        player->KilledMonsterCredit(NPC_DECAY, 0);
                        break;
                    case AT_BLOOD:
                        player->KilledMonsterCredit(NPC_BLOOD, 0);
                        break;
                    case AT_PAIN:
                        player->KilledMonsterCredit(NPC_PAIN, 0);
                        break;
                    default:
                        break;
                }
            }
            return false;
        }
};

enum eAldurtharGate
{
    TRIGGER_SOUTH                               = 5284,

    TRIGGER_CENTRAL                             = 5285,
    TRIGGER_NORTH                               = 5286,
    TRIGGER_NORTHWEST                           = 5287,

    NPC_SOUTH_GATE                              = 32195,
    NPC_CENTRAL_GATE                            = 32196,
    NPC_NORTH_GATE                              = 32197,
    NPC_NORTHWEST_GATE                          = 32199
};

class AreaTrigger_at_aldurthar_gate : public AreaTriggerScript
{
    public:
        AreaTrigger_at_aldurthar_gate() : AreaTriggerScript("at_aldurthar_gate") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger)
        {
            switch(trigger->id)
            {
                case TRIGGER_SOUTH:     player->KilledMonsterCredit(NPC_SOUTH_GATE, 0);     break;
                case TRIGGER_CENTRAL:   player->KilledMonsterCredit(NPC_CENTRAL_GATE, 0);   break;
                case TRIGGER_NORTH:     player->KilledMonsterCredit(NPC_NORTH_GATE, 0);     break;
                case TRIGGER_NORTHWEST: player->KilledMonsterCredit(NPC_NORTHWEST_GATE, 0); break;
            }
            return true;
        }
};

enum eCoilfangGOs
{
    GO_COILFANG_WATERFALL   = 184212
};

class AreaTrigger_at_coilfang_waterfall : public AreaTriggerScript
{
    public:
        AreaTrigger_at_coilfang_waterfall() : AreaTriggerScript("at_coilfang_waterfall") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            if(GameObject* pGo = GetClosestGameObjectWithEntry(player, GO_COILFANG_WATERFALL, 35.0f))
            {
                if(pGo->getLootState() == GO_READY)
                    pGo->UseDoorOrButton();
            }

            return false;
        }
};

enum eLegionTeleporter
{
    SPELL_TELE_A_TO         = 37387,
    QUEST_GAINING_ACCESS_A  = 10589,

    SPELL_TELE_H_TO         = 37389,
    QUEST_GAINING_ACCESS_H  = 10604
};

class AreaTrigger_at_legion_teleporter : public AreaTriggerScript
{
    public:
        AreaTrigger_at_legion_teleporter() : AreaTriggerScript("at_legion_teleporter") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            if(player->isAlive() && !player->isInCombat())
            {
                if(player->GetTeam() == ALLIANCE && player->GetQuestRewardStatus(QUEST_GAINING_ACCESS_A))
                {
                    player->CastSpell(player, SPELL_TELE_A_TO, false);
                    return true;
                }

                if(player->GetTeam() == HORDE && player->GetQuestRewardStatus(QUEST_GAINING_ACCESS_H))
                {
                    player->CastSpell(player, SPELL_TELE_H_TO, false);
                    return true;
                }

                return false;
            }
            return false;
        }
};

enum eRavenholdt
{
    QUEST_MANOR_RAVENHOLDT  = 6681,
    NPC_RAVENHOLDT          = 13936
};

class AreaTrigger_at_ravenholdt : public AreaTriggerScript
{
    public:
        AreaTrigger_at_ravenholdt() : AreaTriggerScript("at_ravenholdt") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            if(player->GetQuestStatus(QUEST_MANOR_RAVENHOLDT) == QUEST_STATUS_INCOMPLETE)
                player->KilledMonsterCredit(NPC_RAVENHOLDT, 0);

            return false;
        }
};

enum eWarsongFarms
{
    QUEST_THE_WARSONG_FARMS                     = 11686,

    NPC_CREDIT_SLAUGHTERHOUSE                   = 25672,
    NPC_CREDIT_GRAINERY                         = 25669,
    NPC_CREDIT_TORP_FARM                        = 25671,

    AT_SLAUGHTERHOUSE                           = 4873,
    AT_GRAINERY                                 = 4871,
    AT_TORP_FARM                                = 4872
};

class AreaTrigger_at_warsong_farms : public AreaTriggerScript
{
    public:
        AreaTrigger_at_warsong_farms() : AreaTriggerScript("at_warsong_farms") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger)
        {
            if(!player->isDead() && player->GetQuestStatus(QUEST_THE_WARSONG_FARMS) == QUEST_STATUS_INCOMPLETE)
            {
                switch(trigger->id)
                {
                    case AT_SLAUGHTERHOUSE: player->KilledMonsterCredit(NPC_CREDIT_SLAUGHTERHOUSE, 0); break;
                    case AT_GRAINERY:       player->KilledMonsterCredit(NPC_CREDIT_GRAINERY, 0);       break;
                    case AT_TORP_FARM:      player->KilledMonsterCredit(NPC_CREDIT_TORP_FARM, 0);      break;
                }
            }
            return true;
        }
};

enum eStormwrightShelf
{
    QUEST_STRENGTH_OF_THE_TEMPEST               = 12741,

    SPELL_CREATE_TRUE_POWER_OF_THE_TEMPEST      = 53067
};

class AreaTrigger_at_stormwright_shelf : public AreaTriggerScript
{
    public:
        AreaTrigger_at_stormwright_shelf() : AreaTriggerScript("at_stormwright_shelf") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            if(!player->isDead() && player->GetQuestStatus(QUEST_STRENGTH_OF_THE_TEMPEST) == QUEST_STATUS_INCOMPLETE)
                player->CastSpell(player, SPELL_CREATE_TRUE_POWER_OF_THE_TEMPEST, false);

            return true;
        }
};

enum eScentLarkorwi
{
    QUEST_SCENT_OF_LARKORWI                     = 4291,
    NPC_LARKORWI_MATE                           = 9683
};

class AreaTrigger_at_scent_larkorwi : public AreaTriggerScript
{
    public:
        AreaTrigger_at_scent_larkorwi() : AreaTriggerScript("at_scent_larkorwi") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            if(!player->isDead() && player->GetQuestStatus(QUEST_SCENT_OF_LARKORWI) == QUEST_STATUS_INCOMPLETE)
            {
                if(!player->FindNearestCreature(NPC_LARKORWI_MATE, 15))
                    player->SummonCreature(NPC_LARKORWI_MATE, player->GetPositionX()+5, player->GetPositionY(), player->GetPositionZ(), 3.3f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 100000);
            }

            return false;
        }
};

enum eAtLastRites
{
    QUEST_LAST_RITES                          = 12019,
    QUEST_BREAKING_THROUGH                    = 11898,
};

class AreaTrigger_at_last_rites : public AreaTriggerScript
{
    public:
        AreaTrigger_at_last_rites() : AreaTriggerScript("at_last_rites") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger)
        {
            if(!(player->GetQuestStatus(QUEST_LAST_RITES) == QUEST_STATUS_INCOMPLETE ||
                player->GetQuestStatus(QUEST_LAST_RITES) == QUEST_STATUS_COMPLETE ||
                player->GetQuestStatus(QUEST_BREAKING_THROUGH) == QUEST_STATUS_INCOMPLETE ||
                player->GetQuestStatus(QUEST_BREAKING_THROUGH) == QUEST_STATUS_COMPLETE))
                return false;

            WorldLocation pPosition;

            switch(trigger->id)
            {
                case 5332:
                case 5338:
                    pPosition = WorldLocation(571, 3733.68f, 3563.25f, 290.812f, 3.665192f);
                    break;
                case 5334:
                    pPosition = WorldLocation(571, 3802.38f, 3585.95f, 49.5765f, 0.0f);
                    break;
                case 5340:
                    pPosition = WorldLocation(571, 3687.91f, 3577.28f, 473.342f, 0.0f);
                    break;
                default:
                    return false;
            }

            player->TeleportTo(pPosition);

            return false;
        }
};

enum eWaygate
{
    SPELL_SHOLAZAR_TO_UNGORO_TELEPORT           = 52056,
    SPELL_UNGORO_TO_SHOLAZAR_TELEPORT           = 52057,

    AT_SHOLAZAR                                 = 5046,
    AT_UNGORO                                   = 5047,

    QUEST_THE_MAKERS_OVERLOOK                   = 12613,
    QUEST_THE_MAKERS_PERCH                      = 12559,
};

class AreaTrigger_at_sholazar_waygate : public AreaTriggerScript
{
    public:
        AreaTrigger_at_sholazar_waygate() : AreaTriggerScript("at_sholazar_waygate") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger)
        {
            if(player->GetQuestStatus(QUEST_THE_MAKERS_OVERLOOK) == QUEST_STATUS_REWARDED && !player->isDead() &&
                player->GetQuestStatus(QUEST_THE_MAKERS_PERCH)    == QUEST_STATUS_REWARDED)
            {
                switch(trigger->id)
                {
                    case AT_SHOLAZAR: player->CastSpell(player, SPELL_SHOLAZAR_TO_UNGORO_TELEPORT, false); break;
                    case AT_UNGORO:   player->CastSpell(player, SPELL_UNGORO_TO_SHOLAZAR_TELEPORT, false); break;
                }
            }

            return false;
        }
};

class AreaTrigger_at_quest_the_echo_of_ymiron : public AreaTriggerScript
{
    public:
        AreaTrigger_at_quest_the_echo_of_ymiron() : AreaTriggerScript("at_quest_the_echo_of_ymiron") { }

        bool OnTrigger(Player* pPlayer, AreaTriggerEntry const* /*pTrigger*/)
        {
            if(pPlayer->GetQuestStatus(11343) != QUEST_STATUS_INCOMPLETE)
                return false;

            if(Creature* pMale = pPlayer->FindNearestCreature(24314, 10.0f)) {
                if(Creature* pFemale = pPlayer->FindNearestCreature(24315, 10.0f)) {
                    pMale->MonsterSay("So then we too are cursed?", LANG_UNIVERSAL, 0);
                    pMale->MonsterSay("The gods have forsaken us! We must dispose of it before Ymiron is notified!", LANG_UNIVERSAL, 0);
                    pFemale->MonsterSay("NO! You cannot! I beg of you! It is our child!", LANG_UNIVERSAL, 0);
                    pMale->MonsterSay("Then what are we to do, wife? The others cannot find out. Should they learn of this aberration, we will all be executed.", LANG_UNIVERSAL, 0);
                    pFemale->MonsterSay("I... I will hide it. I will hide it until I find it a home, far away from here...", LANG_UNIVERSAL, 0);
                    pPlayer->CompleteQuest(11343);

                    if(pPlayer->HasAura(42786))
                        pPlayer->RemoveAura(42786);
                }
            }

            return false;
        }
};

enum ChildrensWeek
{
    QUEST_DOWN_AT_THE_DOCKS         = 910,
    QUEST_GATEWAY_TO_THE_FRONTIER   = 911,
    QUEST_LORDAERON_THRONE_ROOM     = 1800,
    QUEST_BOUGHT_OF_ETERNALS        = 1479,
    QUEST_SPOOKY_LIGHTHOUSE         = 1687,
    QUEST_STONEWROUGHT_DAM          = 1558,
    QUEST_DARK_PORTAL_H             = 10951,
    QUEST_DARK_PORTAL_A             = 10952,

    AT_DOWN_AT_THE_DOCKS            = 3551,
    AT_GATEWAY_TO_THE_FRONTIER      = 3549,
    AT_LORDAERON_THRONE_ROOM        = 3547,
    AT_BOUGHT_OF_ETERNALS           = 3546,
    AT_SPOOKY_LIGHTHOUSE            = 3552,
    AT_STONEWROUGHT_DAM             = 3548,
    AT_DARK_PORTAL                  = 4356,

    AURA_ORPHAN_OUT                 = 58818,
};

class AreaTrigger_at_bring_your_orphan_to : public AreaTriggerScript
{
public:
    AreaTrigger_at_bring_your_orphan_to() : AreaTriggerScript("at_bring_your_orphan_to") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* pTrigger)
    {
        uint32 questId = 0;

        if(player->isDead() || !player->HasAura(AURA_ORPHAN_OUT))
            return false;

        switch(pTrigger->id)
        {
            case AT_DOWN_AT_THE_DOCKS:      questId = QUEST_DOWN_AT_THE_DOCKS;      break;
            case AT_GATEWAY_TO_THE_FRONTIER:questId = QUEST_GATEWAY_TO_THE_FRONTIER;break;
            case AT_LORDAERON_THRONE_ROOM:  questId = QUEST_LORDAERON_THRONE_ROOM;  break;
            case AT_BOUGHT_OF_ETERNALS:     questId = QUEST_BOUGHT_OF_ETERNALS;     break;
            case AT_SPOOKY_LIGHTHOUSE:      questId = QUEST_SPOOKY_LIGHTHOUSE;      break;
            case AT_STONEWROUGHT_DAM:       questId = QUEST_STONEWROUGHT_DAM;       break;
            case AT_DARK_PORTAL:
                questId = player->GetTeam() == ALLIANCE ? QUEST_DARK_PORTAL_A : QUEST_DARK_PORTAL_H;
                break;
        }

        if(questId && player->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
            player->AreaExploredOrEventHappens(questId);

        return true;
    }
};

enum ToLegionHold
{
    SPELL_CRATE_DISGUISE = 37097,
    NPC_RAZUUN           = 21502,
    NPC_JOVAAN           = 21633
};

class AreaTrigger_at_legion_hold_smvalley : public AreaTriggerScript
{
public:
   AreaTrigger_at_legion_hold_smvalley() : AreaTriggerScript("at_legion_hold_smvalley") { }

   bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
   {
        if(player->HasAura(SPELL_CRATE_DISGUISE))
        {
            player->SummonCreature(NPC_RAZUUN, -3302.599854f, 2928.284424f, 170.921692f, 2.609816f, TEMPSUMMON_TIMED_DESPAWN, 40000);
            player->SummonCreature(NPC_JOVAAN, -3306.278076f, 2932.812500f, 170.923660f, 5.535417f, TEMPSUMMON_TIMED_DESPAWN, 40000);
            player->KilledMonsterCredit(21502, 0);
        }
        return true;
    }
};

/*######
## AreaTrigger_at_lady_of_tigers 5843 (game event Zalazane's Fall)
######*/
class AreaTrigger_at_lady_of_tigers : public AreaTriggerScript
{
public:
    AreaTrigger_at_lady_of_tigers() : AreaTriggerScript("at_lady_of_tigers") {}

    enum
    {
        EVENTID_ZALAZANES_FALL      = 46,
        QUEST_LADY_OF_DA_TIGERS     = 25470,
        SPELL_SUMMON_TIGER_MATRIARCH= 75187,
        NPC_TIGER_MATRIARCH         = 40312,
        NPC_ZEN_TABRA               = 40196,
        TEXTID_TS_TIGER_IS_HERE     = 10075,
        // Vehicle Id 736
    };

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*at*/)
    {
        if(!sGameEventMgr->IsActiveEvent(EVENTID_ZALAZANES_FALL)
            || player->GetQuestStatus(QUEST_LADY_OF_DA_TIGERS) != QUEST_STATUS_INCOMPLETE
            || !player->GetVehicle())
            return false;

        if(!player->isInCombat() && !player->FindNearestCreature(NPC_TIGER_MATRIARCH, 20.0f) && !player->FindNearestCreature(NPC_ZEN_TABRA, 20.0f))
        {
            player->CastSpell(player, SPELL_SUMMON_TIGER_MATRIARCH, true);
            player->GetSession()->SendNotification(TEXTID_TS_TIGER_IS_HERE);
        }
        return true;
    }
};

void AddSC_areatrigger_scripts()
{
    new AreaTrigger_at_bring_your_orphan_to;
    new AreaTrigger_at_quest_the_end_of_the_line;
    new AreaTrigger_at_drake_hunt;
    new AreaTrigger_at_blending_in;
    new AreaTrigger_at_aldurthar_gate;
    new AreaTrigger_at_coilfang_waterfall;
    new AreaTrigger_at_legion_teleporter;
    new AreaTrigger_at_ravenholdt;
    new AreaTrigger_at_warsong_farms;
    new AreaTrigger_at_stormwright_shelf;
    new AreaTrigger_at_scent_larkorwi;
    new AreaTrigger_at_last_rites;
    new AreaTrigger_at_sholazar_waygate;
    new AreaTrigger_at_quest_the_echo_of_ymiron;
    new AreaTrigger_at_legion_hold_smvalley;
    new AreaTrigger_at_lady_of_tigers;
}

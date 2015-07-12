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
#include "ScriptedEscortAI.h"

enum eData
{
    NPC_BJORN_HALGURDSSON       = 24238,
    SPELL_VRYKUL_INSULT         = 43315,
    QUEST_DROP_IT_ROCK_IT       = 11429,
    NPC_WINTERSKORN_DEFENDER    = 24015
};

class npc_banner_q11429 : public CreatureScript
{
public:
    npc_banner_q11429() : CreatureScript("npc_banner_q11429") { }

    struct npc_banner_q11429AI : public ScriptedAI
    {
        npc_banner_q11429AI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 uiWaveTimer;
        uint8 killCounter;

        void Reset()
        {
            killCounter = 0;
            uiWaveTimer = 2000;
            me->SetReactState(REACT_PASSIVE);
            me->GetMotionMaster()->MoveIdle();
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(Player* player = me->GetOwner()->ToPlayer())
                player->FailQuest(QUEST_DROP_IT_ROCK_IT);
        }

        void UpdateAI(const uint32 diff)
        {
            if(uiWaveTimer < diff)
            {
                if(Creature* pVrykul = me->SummonCreature(NPC_WINTERSKORN_DEFENDER, (1476.85f + rand()%20), (-5327.56f + rand()%20), (194.8f  + rand()%2), 0.0f, TEMPSUMMON_CORPSE_DESPAWN))
                {
                    pVrykul->AI()->AttackStart(me);
                    pVrykul->GetMotionMaster()->Clear();
                    pVrykul->GetMotionMaster()->MoveChase(me);
                }
                uiWaveTimer = urand(8000, 16000);
            }
            else
                uiWaveTimer -= diff;
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            if(summon->GetEntry() == NPC_WINTERSKORN_DEFENDER)
                killCounter++;

            if(killCounter >= 3)
            {
                if(Player* player = me->GetOwner()->ToPlayer())
                    player->GroupEventHappens(QUEST_DROP_IT_ROCK_IT, me);

                me->DespawnOrUnsummon(2000);
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_banner_q11429AI(pCreature);
    }
};

class item_vrykul_insult : public ItemScript
{
public:
    item_vrykul_insult() : ItemScript("item_vrykul_insult") { }

    bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*pTargets*/)
    {
        if(Unit* pTarget = pPlayer->GetSelectedUnit()) {
            if(pTarget && pTarget->GetTypeId() == TYPEID_UNIT) {
                if(pTarget->ToCreature()->GetEntry() == NPC_BJORN_HALGURDSSON)
                    return false;
            }
        }

        if(const SpellEntry* pSpellInfo = GetSpellStore()->LookupEntry(SPELL_VRYKUL_INSULT))
            Spell::SendCastResult(pPlayer, pSpellInfo, 1, SPELL_FAILED_BAD_TARGETS);

        pPlayer->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, pItem, NULL);
        return true;
    }
};

enum RivenWidowCocon {
    SPELL_NO_QUEST           = 43288,
    SPELL_QUEST              = 43287,

    QUEST_RIVENWOOD_CAPTIVES = 11296
};

class npc_riven_widow_cocoon : public CreatureScript
{
public:
    npc_riven_widow_cocoon() : CreatureScript("npc_riven_widow_cocoon") { }

    struct npc_riven_widow_cocoonAI : public Scripted_NoMovementAI
    {
        npc_riven_widow_cocoonAI(Creature* pCreature): Scripted_NoMovementAI(pCreature) { }

        void JustDied(Unit* who)
        {
            if(!who)
                return;

            Player* pKiller = who->ToPlayer();
            if(!pKiller || !pKiller->IsInWorld())
                return;

            if(who->GetTypeId() == TYPEID_PLAYER)
            {
                if(who->ToPlayer()->GetQuestStatus(QUEST_RIVENWOOD_CAPTIVES) == QUEST_STATUS_INCOMPLETE)
                {
                    if(urand(0, 3) == 0)
                        me->CastSpell(who, SPELL_QUEST, true);
                    else
                        me->CastSpell(who, SPELL_NO_QUEST, true);
                } else me->CastSpell(who, SPELL_NO_QUEST, true);
            } else me->CastSpell(who, SPELL_NO_QUEST, true);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_riven_widow_cocoonAI(pCreature);
    }
};

enum Entries
{
    NPC_APOTHECARY_HANES         = 23784,
    FACTION_ESCORTEE_A           = 774,
    FACTION_ESCORTEE_H           = 775,
    NPC_HANES_FIRE_TRIGGER       = 23968,
    QUEST_TRAIL_OF_FIRE          = 11241,
    SPELL_COSMETIC_LOW_POLY_FIRE = 56274
};

class npc_apothecary_hanes : public CreatureScript
{
public:
    npc_apothecary_hanes() : CreatureScript("npc_apothecary_hanes") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if(quest->GetQuestId() == QUEST_TRAIL_OF_FIRE)
        {
            switch(player->GetTeam())
            {
                case ALLIANCE:
                    creature->setFaction(FACTION_ESCORTEE_A);
                    break;
                case HORDE:
                    creature->setFaction(FACTION_ESCORTEE_H);
                    break;
            }
            CAST_AI(npc_escortAI, (creature->AI()))->Start(true, false, player->GetGUID());
        }
        return true;
    }

    struct npc_Apothecary_HanesAI : public npc_escortAI
    {
        npc_Apothecary_HanesAI(Creature* pCreature) : npc_escortAI(pCreature){ }
        uint32 PotTimer;

        void Reset ()
        {
            SetDespawnAtFar(false);
            PotTimer = 10000; //10 sec cooldown on potion
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(Player* player = GetPlayerForEscort())
                player->FailQuest(QUEST_TRAIL_OF_FIRE);
        }

        void UpdateEscortAI(const uint32 diff)
        {
            if(HealthBelowPct(75))
            {
                if(PotTimer <= diff)
                {
                    DoCast(me, 17534, true);
                    PotTimer = 10000;
                } else PotTimer -= diff;
            }
            if(GetAttack() && UpdateVictim())
                DoMeleeAttackIfReady();
        }

        void WaypointReached(uint32 i)
        {
            Player* player = GetPlayerForEscort();
            if(!player)
                return;
            switch(i)
            {
                case 1:
                    me->SetReactState(REACT_AGGRESSIVE);
                    SetRun(true);
                    break;
                case 23:
                    if(player)
                        player->GroupEventHappens(QUEST_TRAIL_OF_FIRE, me);
                    me->DespawnOrUnsummon();
                    break;
                case 5:
                    if(Unit* Trigger = me->FindNearestCreature(NPC_HANES_FIRE_TRIGGER, 10.0f))
                        Trigger->CastSpell(Trigger, SPELL_COSMETIC_LOW_POLY_FIRE, false);
                    SetRun(false);
                    break;
                case 6:
                    if(Unit* Trigger = me->FindNearestCreature(NPC_HANES_FIRE_TRIGGER, 10.0f))
                        Trigger->CastSpell(Trigger, SPELL_COSMETIC_LOW_POLY_FIRE, false);
                    SetRun(true);
                    break;
                case 8:
                    if(Unit* Trigger = me->FindNearestCreature(NPC_HANES_FIRE_TRIGGER, 10.0f))
                        Trigger->CastSpell(Trigger, SPELL_COSMETIC_LOW_POLY_FIRE, false);
                    SetRun(false);
                    break;
                case 9:
                    if(Unit* Trigger = me->FindNearestCreature(NPC_HANES_FIRE_TRIGGER, 10.0f))
                        Trigger->CastSpell(Trigger, SPELL_COSMETIC_LOW_POLY_FIRE, false);
                    break;
                case 10:
                    SetRun(true);
                    break;
                case 13:
                    SetRun(false);
                    break;
                case 14:
                    if(Unit* Trigger = me->FindNearestCreature(NPC_HANES_FIRE_TRIGGER, 10.0f))
                        Trigger->CastSpell(Trigger, SPELL_COSMETIC_LOW_POLY_FIRE, false);
                    SetRun(true);
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_Apothecary_HanesAI(pCreature);
    }
};

enum ePlaguehound
{
    QUEST_SNIFF_OUT_ENEMY        = 11253
};

class npc_plaguehound_tracker : public CreatureScript
{
public:
    npc_plaguehound_tracker() : CreatureScript("npc_plaguehound_tracker") { }

    struct npc_plaguehound_trackerAI : public npc_escortAI
    {
        npc_plaguehound_trackerAI(Creature* pCreature) : npc_escortAI(pCreature) { }

        void Reset()
        {
            uint64 summonerGUID = 0;

            if(me->isSummon())
                if(Unit* summoner = me->ToTempSummon()->GetSummoner())
                    if(summoner->GetTypeId() == TYPEID_PLAYER)
                        summonerGUID = summoner->GetGUID();

            if(!summonerGUID)
                return;

            me->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
            Start(false, false, summonerGUID);
        }

        void WaypointReached(uint32 waypointId)
        {
            if(waypointId != 26)
                return;

            me->DespawnOrUnsummon();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_plaguehound_trackerAI(pCreature);
    }
};

#define GOSSIP_RAZAEL_REPORT "Le Grand exécuteur Anselm veut un rapport sur la situation."
#define GOSSIP_LYANA_REPORT "Le Grand exécuteur Anselm demandes votre rapport."

enum eRazael
{
    QUEST_REPORTS_FROM_THE_FIELD = 11221,
    NPC_RAZAEL = 23998,
    NPC_LYANA = 23778,
    GOSSIP_TEXTID_RAZAEL1 = 11562,
    GOSSIP_TEXTID_RAZAEL2 = 11564,
    GOSSIP_TEXTID_LYANA1 = 11586,
    GOSSIP_TEXTID_LYANA2 = 11588
};

class npc_razael_and_lyana : public CreatureScript
{
public:
    npc_razael_and_lyana() : CreatureScript("npc_razael_and_lyana") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if(player->GetQuestStatus(QUEST_REPORTS_FROM_THE_FIELD) == QUEST_STATUS_INCOMPLETE)
            switch(creature->GetEntry())
            {
                case NPC_RAZAEL:
                    if(!player->GetReqKillOrCastCurrentCount(QUEST_REPORTS_FROM_THE_FIELD, NPC_RAZAEL))
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_RAZAEL_REPORT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                        player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_RAZAEL1, creature->GetGUID());
                        return true;
                    }
                    break;
                case NPC_LYANA:
                    if(!player->GetReqKillOrCastCurrentCount(QUEST_REPORTS_FROM_THE_FIELD, NPC_LYANA))
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LYANA_REPORT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                        player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_LYANA1, creature->GetGUID());
                        return true;
                    }
                    break;
            }

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_RAZAEL2, creature->GetGUID());
                player->TalkedToCreature(NPC_RAZAEL, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_LYANA2, creature->GetGUID());
                player->TalkedToCreature(NPC_LYANA, creature->GetGUID());
                break;
        }
        return true;
    }
};

#define GOSSIP_ITEM_MG_I  "Walt m'a envoyé pour prendre quelques lingots de fer noir."
#define GOSSIP_ITEM_MG_II "Yarp."

enum eMcGoyver
{
    QUEST_WE_CAN_REBUILD_IT             = 11483,

    SPELL_CREATURE_DARK_IRON_INGOTS     = 44512,
    SPELL_TAXI_EXPLORERS_LEAGUE         = 44280,

    GOSSIP_TEXTID_MCGOYVER              = 12193
};

class npc_mcgoyver : public CreatureScript
{
public:
    npc_mcgoyver() : CreatureScript("npc_mcgoyver") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(player->GetQuestStatus(QUEST_WE_CAN_REBUILD_IT) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_MG_I, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_MG_II, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_MCGOYVER, creature->GetGUID());
                player->CastSpell(player, SPELL_CREATURE_DARK_IRON_INGOTS, true);
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->CastSpell(player, SPELL_TAXI_EXPLORERS_LEAGUE, true);
                player->CLOSE_GOSSIP_MENU();
                break;
        }
        return true;
    }
};

enum eDaegarnn
{
    QUEST_DEFEAT_AT_RING            = 11300,

    NPC_FIRJUS                      = 24213,
    NPC_JLARBORN                    = 24215,
    NPC_YOROS                       = 24214,
    NPC_OLUF                        = 23931,

    NPC_PRISONER_1                  = 24253,  // looks the same but has different abilities
    NPC_PRISONER_2                  = 24254,
    NPC_PRISONER_3                  = 24255,
};

static float afSummon[] = {838.81f, -4678.06f, -94.182f};
static float afCenter[] = {801.88f, -4721.87f, -96.143f};

class npc_daegarn : public CreatureScript
{
public:
    npc_daegarn() : CreatureScript("npc_daegarn") { }

    bool OnQuestAccept(Player* player, Creature* creature, const Quest* pQuest)
    {
        if(pQuest->GetQuestId() == QUEST_DEFEAT_AT_RING)
        {
            if(npc_daegarnAI* pDaegarnAI = CAST_AI(npc_daegarn::npc_daegarnAI, creature->AI()))
                pDaegarnAI->StartEvent(player->GetGUID());
        }

        return true;
    }

    // TODO: make prisoners help (unclear if summoned or using npc's from surrounding cages (summon inside small cages?))
    struct npc_daegarnAI : public ScriptedAI
    {
        npc_daegarnAI(Creature* pCreature): ScriptedAI(pCreature) { }

        bool bEventInProgress;
        uint64 uiPlayerGUID;

        void Reset()
        {
            bEventInProgress = false;
            uiPlayerGUID = 0;
        }

        void StartEvent(uint64 uiGUID)
        {
            if(bEventInProgress)
                return;

            uiPlayerGUID = uiGUID;

            SummonGladiator(NPC_FIRJUS);
        }

        void JustSummoned(Creature* summon)
        {
            if(Player* player = me->GetPlayer(*me, uiPlayerGUID))
            {
                if(player->isAlive())
                {
                    summon->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                    summon->GetMotionMaster()->MovePoint(0, afCenter[0], afCenter[1], afCenter[2]);
                    summon->AI()->AttackStart(player);
                    return;
                }
            }

            Reset();
        }

        void SummonGladiator(uint32 uiEntry)
        {
            me->SummonCreature(uiEntry, afSummon[0], afSummon[1], afSummon[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30*IN_MILLISECONDS);
        }

        void SummonedCreatureDies(Creature* summoned, Unit* /*pKiller*/)
        {
            uint32 uiEntry = 0;

            // will eventually reset the event if something goes wrong
            switch(summoned->GetEntry())
            {
                case NPC_FIRJUS:    uiEntry = NPC_JLARBORN; break;
                case NPC_JLARBORN:  uiEntry = NPC_YOROS;    break;
                case NPC_YOROS:     uiEntry = NPC_OLUF;     break;
                case NPC_OLUF:      Reset();                return;
            }

            SummonGladiator(uiEntry);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_daegarnAI(pCreature);
    }
};

enum AttractedReefBullData
{
    NPC_FEMALE_REEF_COW        = 24797,
    SPELL_ANUNIAQS_NET         = 21014,
    SPELL_TASTY_REEF_FISH      = 44454,
    SPELL_LOVE_COSMETIC        = 52148,
    ITEM_TASTY_REEF_FISH       = 34127,
    QUEST_THE_WAY_TO_HIS_HEART = 11472
};

class npc_attracted_reef_bull : public CreatureScript
{
public:
    npc_attracted_reef_bull() : CreatureScript("npc_attracted_reef_bull") { }

    struct npc_attracted_reef_bullAI : public ScriptedAI
    {
        npc_attracted_reef_bullAI(Creature* pCreature): ScriptedAI(pCreature) { }

        uint64 playerGUID;
        uint8 point;

        void Reset()
        {
            playerGUID = 0;
            point = 0;
        }

        void UpdateAI(const uint32 diff) { }

        void SpellHit(Unit* caster, const SpellEntry* spell)
        {
            if(!caster->ToPlayer())
                return;

            if(spell->Id == SPELL_TASTY_REEF_FISH)
            {
                if(playerGUID == 0)
                    playerGUID = caster->GetGUID();

                me->GetMotionMaster()->MovePoint(point, caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ());
                ++point;
            }

            if(Creature* female = me->FindNearestCreature(NPC_FEMALE_REEF_COW, 5.0f, true))
            {
                if(Player* player = me->GetPlayer(*me, playerGUID))
                {
                    DoCast(me, SPELL_LOVE_COSMETIC);
                    female->AI()->DoCast(female, SPELL_LOVE_COSMETIC);
                    player->GroupEventHappens(QUEST_THE_WAY_TO_HIS_HEART, me);
                    me->DespawnOrUnsummon(5000);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_attracted_reef_bullAI(pCreature);
    }
};

class spell_anuniaqs_net : public SpellScriptLoader
{
public:
    spell_anuniaqs_net() : SpellScriptLoader("spell_anuniaqs_net") { }

    class spell_anuniaqs_net_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_anuniaqs_net_SpellScript);

        bool Validate(SpellEntry const* /*spellInfo*/)
        {
            if(!sSpellStore.LookupEntry(SPELL_ANUNIAQS_NET))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if(Unit* caster = GetCaster())
                if(caster->ToPlayer())
                    caster->ToPlayer()->AddItem(ITEM_TASTY_REEF_FISH, urand(1, 5));
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_anuniaqs_net_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_anuniaqs_net_SpellScript();
    }
};

void AddSC_howling_fjord()
{
    new npc_banner_q11429;
    new npc_riven_widow_cocoon;
    new npc_apothecary_hanes;
    new npc_plaguehound_tracker;
    new npc_razael_and_lyana;
    new npc_mcgoyver;
    new npc_daegarn;
    new item_vrykul_insult;
    new npc_attracted_reef_bull;
    new spell_anuniaqs_net;
}

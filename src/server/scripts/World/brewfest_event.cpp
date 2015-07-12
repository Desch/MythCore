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
#include "ScriptedEscortAI.h"
#include "LFGMgr.h"
#include "SpellAuraEffects.h"

enum eBrewfestBarkQuests
{
    BARK_FOR_THE_THUNDERBREWS       = 11294,
    BARK_FOR_TCHALIS_VOODOO_BREWERY = 11408,
    BARK_FOR_THE_BARLEYBREWS        = 11293,
    BARK_FOR_DROHNS_DISTILLERY      = 11407,
    SPELL_BREWFEST_RAM              = 43883,
    SPELL_RAM_FATIGUE               = 43052,
    SPELL_SPEED_RAM_GALLOP          = 42994,
    SPELL_SPEED_RAM_CANTER          = 42993,
    SPELL_SPEED_RAM_TROT            = 42992,
    SPELL_SPEED_RAM_NORMAL          = 43310,
    SPELL_SPEED_RAM_EXHAUSED        = 43332,
    NPC_SPEED_BUNNY_GREEN           = 24263,
    NPC_SPEED_BUNNY_YELLOW          = 24264,
    NPC_SPEED_BUNNY_RED             = 24265,
    NPC_BARKER_BUNNY_1              = 24202,
    NPC_BARKER_BUNNY_2              = 24203,
    NPC_BARKER_BUNNY_3              = 24204,
    NPC_BARKER_BUNNY_4              = 24205
};

class npc_brewfest_trigger : public CreatureScript
{
public:
    npc_brewfest_trigger() : CreatureScript("npc_brewfest_trigger") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_brewfest_triggerAI(pCreature);
    }

    struct npc_brewfest_triggerAI : public ScriptedAI
    {
        npc_brewfest_triggerAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        void MoveInLineOfSight(Unit* who)
        {
            if(!who)
                return;

            if(who->GetTypeId() == TYPEID_PLAYER)
            {
                if(!(CAST_PLR(who)->GetAura(SPELL_BREWFEST_RAM)))
                    return;

                if(CAST_PLR(who)->GetQuestStatus(BARK_FOR_THE_THUNDERBREWS) == QUEST_STATUS_INCOMPLETE||
                    CAST_PLR(who)->GetQuestStatus(BARK_FOR_TCHALIS_VOODOO_BREWERY) == QUEST_STATUS_INCOMPLETE||
                    CAST_PLR(who)->GetQuestStatus(BARK_FOR_THE_BARLEYBREWS) == QUEST_STATUS_INCOMPLETE||
                    CAST_PLR(who)->GetQuestStatus(BARK_FOR_DROHNS_DISTILLERY) == QUEST_STATUS_INCOMPLETE)
                {
                    uint32 creditMarkerId = me->GetEntry();
                    if((creditMarkerId >= 24202) && (creditMarkerId <= 24205))
                    {
                        // 24202: Brewfest Barker Bunny 1, 24203: Brewfest Barker Bunny 2, 24204: Brewfest Barker Bunny 3, 24205: Brewfest Barker Bunny 4
                        if(!CAST_PLR(who)->GetReqKillOrCastCurrentCount(BARK_FOR_THE_BARLEYBREWS, creditMarkerId)||
                            !CAST_PLR(who)->GetReqKillOrCastCurrentCount(BARK_FOR_THE_THUNDERBREWS, creditMarkerId)||
                            !CAST_PLR(who)->GetReqKillOrCastCurrentCount(BARK_FOR_DROHNS_DISTILLERY, creditMarkerId)||
                            !CAST_PLR(who)->GetReqKillOrCastCurrentCount(BARK_FOR_TCHALIS_VOODOO_BREWERY, creditMarkerId))
                            CAST_PLR(who)->KilledMonsterCredit(creditMarkerId, me->GetGUID());
                        // Caso para quest 11293 que no se completa teniendo todas las marcas
                        if(CAST_PLR(who)->GetReqKillOrCastCurrentCount(BARK_FOR_THE_BARLEYBREWS, NPC_BARKER_BUNNY_1)&&
                            CAST_PLR(who)->GetReqKillOrCastCurrentCount(BARK_FOR_THE_BARLEYBREWS, NPC_BARKER_BUNNY_2)&&
                            CAST_PLR(who)->GetReqKillOrCastCurrentCount(BARK_FOR_THE_BARLEYBREWS, NPC_BARKER_BUNNY_3)&&
                            CAST_PLR(who)->GetReqKillOrCastCurrentCount(BARK_FOR_THE_BARLEYBREWS, NPC_BARKER_BUNNY_4))
                            CAST_PLR(who)->CompleteQuest(BARK_FOR_THE_BARLEYBREWS);
                    }
                }
            }
        }
    };
};

class item_brewfest_ram_reins : public ItemScript
{
public:
    item_brewfest_ram_reins() : ItemScript("item_brewfest_ram_reins") { }

    bool OnUse(Player* pPlayer, Item* pItem, const SpellCastTargets & /*pTargets*/)
    {
        if(pPlayer->HasAura(SPELL_BREWFEST_RAM) && !pPlayer->HasAura(SPELL_SPEED_RAM_EXHAUSED))
        {
            if(pPlayer->HasAura(SPELL_SPEED_RAM_NORMAL))
                pPlayer->CastSpell(pPlayer,SPELL_SPEED_RAM_TROT,false);
            else if(pPlayer->HasAura(SPELL_SPEED_RAM_TROT))
            {
                if(pPlayer->GetAura(SPELL_SPEED_RAM_TROT)->GetDuration() < 7000)
                    pPlayer->GetAura(SPELL_SPEED_RAM_TROT)->SetDuration(8000);
                else
                  pPlayer->CastSpell(pPlayer,SPELL_SPEED_RAM_CANTER,false);
            } else if(pPlayer->HasAura(SPELL_SPEED_RAM_CANTER)) {
                if(pPlayer->GetAura(SPELL_SPEED_RAM_CANTER)->GetDuration() < 7000)
                    pPlayer->GetAura(SPELL_SPEED_RAM_CANTER)->SetDuration(8000);
                else
                  pPlayer->CastSpell(pPlayer,SPELL_SPEED_RAM_GALLOP,false);
            } else if(pPlayer->HasAura(SPELL_SPEED_RAM_GALLOP))
                pPlayer->GetAura(SPELL_SPEED_RAM_GALLOP)->SetDuration(8000);
        }
        else
            pPlayer->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW,pItem, NULL);

        return true;
    }
};

class spell_brewfest_speed : public SpellScriptLoader
{
public:
    spell_brewfest_speed() : SpellScriptLoader("spell_brewfest_speed") { }

    class spell_brewfest_speed_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_brewfest_speed_AuraScript);

        bool Validate(SpellEntry const* /*spellInfo*/)
        {
            if(!sSpellStore.LookupEntry(SPELL_RAM_FATIGUE))
                return false;
            if(!sSpellStore.LookupEntry(SPELL_BREWFEST_RAM))
                return false;
            if(!sSpellStore.LookupEntry(SPELL_SPEED_RAM_GALLOP))
                return false;
            if(!sSpellStore.LookupEntry(SPELL_SPEED_RAM_CANTER))
                return false;
            if(!sSpellStore.LookupEntry(SPELL_SPEED_RAM_TROT))
                return false;
            if(!sSpellStore.LookupEntry(SPELL_SPEED_RAM_NORMAL))
                return false;
            if(!sSpellStore.LookupEntry(SPELL_SPEED_RAM_GALLOP))
                return false;
            if(!sSpellStore.LookupEntry(SPELL_SPEED_RAM_EXHAUSED))
                return false;
            return true;
        }

        void HandlePeriodicTick(AuraEffect const* aurEff)
        {
            if(GetId() == SPELL_SPEED_RAM_EXHAUSED)
                return;

            Player* pCaster = GetCaster()->ToPlayer();
            if(!pCaster)
                return;
            int i;
            switch(GetId())
            {
                case SPELL_SPEED_RAM_GALLOP:
                    for(i = 0; i < 5; i++)
                        pCaster->AddAura(SPELL_RAM_FATIGUE, pCaster);
                    break;
                case SPELL_SPEED_RAM_CANTER:
                    pCaster->AddAura(SPELL_RAM_FATIGUE, pCaster);
                    break;
                case SPELL_SPEED_RAM_TROT:
                    if(pCaster->HasAura(SPELL_RAM_FATIGUE)) {
                        if(pCaster->GetAura(SPELL_RAM_FATIGUE)->GetStackAmount() <= 2)
                            pCaster->RemoveAura(SPELL_RAM_FATIGUE);
                        else
                            pCaster->GetAura(SPELL_RAM_FATIGUE)->ModStackAmount(-2);
                    }
                    break;
            }

            switch(aurEff->GetId())
            {
                case SPELL_SPEED_RAM_TROT:
                    if(aurEff->GetTickNumber() == 4)
                        pCaster->KilledMonsterCredit(NPC_SPEED_BUNNY_GREEN, 0);
                    break;
                case SPELL_SPEED_RAM_CANTER:
                    if(aurEff->GetTickNumber() == 8)
                        pCaster->KilledMonsterCredit(NPC_SPEED_BUNNY_YELLOW, 0);
                    break;
                case SPELL_SPEED_RAM_GALLOP:
                    if(aurEff->GetTickNumber() == 8)
                        pCaster->KilledMonsterCredit(NPC_SPEED_BUNNY_RED, 0);
                    break;
            }
            if(pCaster->HasAura(SPELL_RAM_FATIGUE))
                if(pCaster->GetAura(SPELL_RAM_FATIGUE)->GetStackAmount() >= 100)
                    pCaster->CastSpell(pCaster,SPELL_SPEED_RAM_EXHAUSED, false);

        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* pCaster = GetCaster()->ToPlayer();
            if(!pCaster)
                return;
            if(!pCaster->HasAura(SPELL_BREWFEST_RAM))
                return;

            if(GetId() == SPELL_SPEED_RAM_EXHAUSED)
            {
                if(pCaster->HasAura(SPELL_RAM_FATIGUE))
                    pCaster->GetAura(SPELL_RAM_FATIGUE)->ModStackAmount(-15);
            } else if(!pCaster->HasAura(SPELL_RAM_FATIGUE) || pCaster->GetAura(SPELL_RAM_FATIGUE)->GetStackAmount() < 100) {
                switch(GetId())
                {
                    case SPELL_SPEED_RAM_GALLOP:
                        if(!pCaster->HasAura(SPELL_SPEED_RAM_EXHAUSED))
                            pCaster->CastSpell(pCaster,SPELL_SPEED_RAM_CANTER, false);
                        break;
                    case SPELL_SPEED_RAM_CANTER:
                        if(!pCaster->HasAura(SPELL_SPEED_RAM_GALLOP))
                            pCaster->CastSpell(pCaster,SPELL_SPEED_RAM_TROT, false);
                        break;
                    case SPELL_SPEED_RAM_TROT:
                        if(!pCaster->HasAura(SPELL_SPEED_RAM_CANTER))
                            pCaster->CastSpell(pCaster,SPELL_SPEED_RAM_NORMAL, false);
                        break;
                }
            }
        }

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* pCaster = GetCaster()->ToPlayer();

            if(!pCaster)
                return;

            switch(GetId())
            {
                case SPELL_SPEED_RAM_GALLOP:
                    pCaster->GetAura(SPELL_SPEED_RAM_GALLOP)->SetDuration(8000);
                    break;
                case SPELL_SPEED_RAM_CANTER:
                    pCaster->GetAura(SPELL_SPEED_RAM_CANTER)->SetDuration(8000);
                    break;
                case SPELL_SPEED_RAM_TROT:
                    pCaster->GetAura(SPELL_SPEED_RAM_TROT)->SetDuration(8000);
                    break;
             }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_brewfest_speed_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_brewfest_speed_AuraScript::HandlePeriodicTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            OnEffectRemove += AuraEffectRemoveFn(spell_brewfest_speed_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_USE_NORMAL_MOVEMENT_SPEED, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_brewfest_speed_AuraScript();
    }
};

class spell_brewfest_speed_normal : public SpellScriptLoader
{
public:
    spell_brewfest_speed_normal() : SpellScriptLoader("spell_brewfest_speed_normal") { }

    class spell_brewfest_speed_normal_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_brewfest_speed_normal_AuraScript);

        bool Validate(SpellEntry const* /*spellInfo*/)
        {
            if(!sSpellStore.LookupEntry(SPELL_SPEED_RAM_NORMAL))
                return false;
            return true;
        }

        void HandlePeriodicTick(AuraEffect const* /*aurEff*/)
        {
            if(GetId() == SPELL_SPEED_RAM_EXHAUSED)
                return;

            Player* pCaster = GetCaster()->ToPlayer();
            if(!pCaster)
                return;

            if(GetId() == SPELL_SPEED_RAM_NORMAL)
                if(pCaster->HasAura(SPELL_RAM_FATIGUE))
                {
                    if(pCaster->GetAura(SPELL_RAM_FATIGUE)->GetStackAmount() <= 4)
                        pCaster->RemoveAura(SPELL_RAM_FATIGUE);
                    else
                        pCaster->GetAura(SPELL_RAM_FATIGUE)->ModStackAmount(-4);
                }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_brewfest_speed_normal_AuraScript::HandlePeriodicTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_brewfest_speed_normal_AuraScript();
    }
};

class npc_brewfest_apple_trigger : public CreatureScript
{
public:
    npc_brewfest_apple_trigger() : CreatureScript("npc_brewfest_apple_trigger") { }

    struct npc_brewfest_apple_triggerAI : public ScriptedAI
    {
        npc_brewfest_apple_triggerAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        void MoveInLineOfSight(Unit* who)
        {
            Player* player = who->ToPlayer();
            if(!player)
                return;
            if(player->HasAura(SPELL_RAM_FATIGUE) && me->GetDistance(player->GetPositionX(),player->GetPositionY(),player->GetPositionZ()) <= 7.5f)
                player->RemoveAura(SPELL_RAM_FATIGUE);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_brewfest_apple_triggerAI(pCreature);
    }
};

enum BrewfestKegThrower
{
    SPELL_THROW_KEG      = 43660,
    ITEM_BREWFEST_KEG    = 33797
};

class npc_brewfest_keg_thrower : public CreatureScript
{
public:
    npc_brewfest_keg_thrower() : CreatureScript("npc_brewfest_keg_thrower") { }

    struct npc_brewfest_keg_throwerAI : public ScriptedAI
    {
        npc_brewfest_keg_throwerAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        void MoveInLineOfSight(Unit* who)
        {
            Player* player = who->ToPlayer();
            if(!player)
                return;
            if((player->HasAura(SPELL_BREWFEST_RAM))
                && me->GetDistance(player->GetPositionX(),player->GetPositionY(),player->GetPositionZ()) <= 25.0f
                && !player->HasItemCount(ITEM_BREWFEST_KEG, 1))
            {
                me->CastSpell(player,SPELL_THROW_KEG,false);
                me->CastSpell(player, 42414,false);
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_brewfest_keg_throwerAI(pCreature);
    }
};

enum BrewfestKegReceiver
{
    SPELL_CREATE_TICKETS            = 44501,
    QUEST_THERE_AND_BACK_AGAIN_A    = 11122,
    QUEST_THERE_AND_BACK_AGAIN_H    = 11412,
    NPC_BREWFEST_DELIVERY_BUNNY     = 24337
};

class npc_brewfest_keg_receiver : public CreatureScript
{
public:
    npc_brewfest_keg_receiver() : CreatureScript("npc_brewfest_keg_receiver") { }

    struct npc_brewfest_keg_receiverAI : public ScriptedAI
    {
        npc_brewfest_keg_receiverAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        void MoveInLineOfSight(Unit* who)
        {
            Player* player = who->ToPlayer();
            if(!player)
                return;

            if((player->HasAura(SPELL_BREWFEST_RAM))
                && me->GetDistance(player->GetPositionX(),player->GetPositionY(),player->GetPositionZ()) <= 5.0f
                && player->HasItemCount(ITEM_BREWFEST_KEG, 1))
            {
                player->CastSpell(me,SPELL_THROW_KEG, true);
                player->DestroyItemCount(ITEM_BREWFEST_KEG, 1, true);
                if(player->HasAura(SPELL_BREWFEST_RAM))
                    player->GetAura(SPELL_BREWFEST_RAM)->SetDuration(player->GetAura(SPELL_BREWFEST_RAM)->GetDuration() + 30000);
                if(player->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_A)
                    || player->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_H))
                {
                    player->CastSpell(player,SPELL_CREATE_TICKETS, true);
                }
                else
                {
                    player->KilledMonsterCredit(NPC_BREWFEST_DELIVERY_BUNNY, 0);
                    if(player->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_A) == QUEST_STATUS_INCOMPLETE)
                        player->AreaExploredOrEventHappens(QUEST_THERE_AND_BACK_AGAIN_A);
                    if(player->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_H) == QUEST_STATUS_INCOMPLETE)
                        player->AreaExploredOrEventHappens(QUEST_THERE_AND_BACK_AGAIN_H);
                    if(player->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_A) == QUEST_STATUS_COMPLETE
                        || player->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_H) == QUEST_STATUS_COMPLETE)
                        player->RemoveAura(SPELL_BREWFEST_RAM);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_brewfest_keg_receiverAI(pCreature);
    }
};

#define GOSSIP_ITEM_RAM             "Do you have additional work?"
#define GOSSIP_ITEM_RAM_REINS       "Give me another Ram Racing Reins"
#define SPELL_BREWFEST_SUMMON_RAM   43883

class npc_brewfest_ram_master : public CreatureScript
{
public:
    npc_brewfest_ram_master() : CreatureScript("npc_brewfest_ram_master") { }

    bool OnGossipHello(Player* player, Creature* pCreature)
    {
        if(pCreature->isQuestGiver())
            player->PrepareQuestMenu(pCreature->GetGUID());

            if(player->HasSpellCooldown(SPELL_BREWFEST_SUMMON_RAM)
                && !player->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_A)
                && !player->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_H)
                && (player->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_A) == QUEST_STATUS_INCOMPLETE
                || player->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_H) == QUEST_STATUS_INCOMPLETE))
                player->RemoveSpellCooldown(SPELL_BREWFEST_SUMMON_RAM);

            if(!player->HasAura(SPELL_BREWFEST_RAM) && ((player->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_A) == QUEST_STATUS_INCOMPLETE
            || player->GetQuestStatus(QUEST_THERE_AND_BACK_AGAIN_H) == QUEST_STATUS_INCOMPLETE
            || (!player->HasSpellCooldown(SPELL_BREWFEST_SUMMON_RAM) &&
                (player->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_A)
                || player->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_H))))))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RAM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

            if((player->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_A)
                || player->GetQuestRewardStatus(QUEST_THERE_AND_BACK_AGAIN_H))
                && !player->HasItemCount(33306, 1, true))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RAM_REINS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

        player->SEND_GOSSIP_MENU(384, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*pCreature*/, uint32 /*uiSender*/, uint32 uiAction)
    {
        if(uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            if(player->HasItemCount(ITEM_BREWFEST_KEG, 1))
                player->DestroyItemCount(ITEM_BREWFEST_KEG, 1, true);
            player->CastSpell(player, SPELL_BREWFEST_SUMMON_RAM, true);
            player->AddSpellCooldown(SPELL_BREWFEST_SUMMON_RAM, 0, time(NULL) + 18*60*60);
        }

        if(uiAction == GOSSIP_ACTION_INFO_DEF+2)
            player->CastSpell(player, 44371, false);
        return true;
    }
};

enum CorenDirebrew
{
    SPELL_DISARM                = 47310,
    SPELL_DISARM_PRECAST        = 47407,
    SPELL_MOLE_MACHINE_EMERGE   = 50313,
    NPC_ILSA_DIREBREW           = 26764,
    NPC_URSULA_DIREBREW         = 26822,
    NPC_DIREBREW_MINION         = 26776,
    EQUIP_ID_TANKARD            = 48663,
    FACTION_HOSTILE_CORE        = 736
};

#define GOSSIP_TEXT_INSULT "Insult Coren Direbrew's brew."

static Position _pos[]=
{
    {890.87f, -133.95f, -48.0f, 1.53f},
    {892.47f, -133.26f, -48.0f, 2.16f},
    {893.54f, -131.81f, -48.0f, 2.75f}
};

class npc_coren_direbrew : public CreatureScript
{
public:
    npc_coren_direbrew() : CreatureScript("npc_coren_direbrew") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT_INSULT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(15858, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if(action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            creature->setFaction(FACTION_HOSTILE_CORE);
            creature->AI()->AttackStart(player);
            creature->AI()->DoZoneInCombat();
            player->CLOSE_GOSSIP_MENU();
        }

        return true;
    }

    struct npc_coren_direbrewAI : public ScriptedAI
    {
        npc_coren_direbrewAI(Creature* pCreature) : ScriptedAI(pCreature), _summons(me) { }

        void Reset()
        {
            me->RestoreFaction();
            me->SetCorpseDelay(90); // 1.5 minutes

            _addTimer = 20000;
            _disarmTimer = urand(10, 15) *IN_MILLISECONDS;

            _spawnedIlsa = false;
            _spawnedUrsula = false;
            _summons.DespawnAll();

            for(uint8 i = 0; i < 3; ++i)
                if(Creature* creature = me->SummonCreature(NPC_DIREBREW_MINION, _pos[i], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000))
                    _add[i] = creature->GetGUID();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            SetEquipmentSlots(false, EQUIP_ID_TANKARD, EQUIP_ID_TANKARD, EQUIP_NO_CHANGE);

            for(uint8 i = 0; i < 3; ++i)
            {
                if(_add[i])
                {
                    Creature* creature = ObjectAccessor::GetCreature((*me), _add[i]);
                    if(creature && creature->isAlive())
                    {
                        creature->setFaction(FACTION_HOSTILE_CORE);
                        creature->SetInCombatWithZone();
                    }

                    _add[i] = 0;
                }
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if(!UpdateVictim())
                return;

            // disarm
            if(_disarmTimer <= diff)
            {
                DoCast(SPELL_DISARM_PRECAST);
                DoCastVictim(SPELL_DISARM, false);
                _disarmTimer = urand(20, 25) *IN_MILLISECONDS;
            } else _disarmTimer -= diff;

            // spawn non-elite adds
            if(_addTimer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                {
                    float posX, posY, posZ;
                    target->GetPosition(posX, posY, posZ);
                    target->CastSpell(target, SPELL_MOLE_MACHINE_EMERGE, true, 0, 0, me->GetGUID());
                    me->SummonCreature(NPC_DIREBREW_MINION, posX, posY, posZ, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                    _addTimer = 15000;

                    if(_spawnedIlsa)
                        _addTimer -= 3000;
                    if(_spawnedUrsula)
                        _addTimer -= 3000;
                }
            } else _addTimer -= diff;

            if(!_spawnedIlsa && HealthBelowPct(66))
            {
                DoSpawnCreature(NPC_ILSA_DIREBREW, 0, 0, 0, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                _spawnedIlsa = true;
            }

            if(!_spawnedUrsula && HealthBelowPct(33))
            {
                DoSpawnCreature(NPC_URSULA_DIREBREW, 0, 0, 0, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                _spawnedUrsula = true;
            }

            DoMeleeAttackIfReady();
        }

        void JustSummoned(Creature* summon)
        {
            if(me->getFaction() == FACTION_HOSTILE_CORE)
            {
                summon->setFaction(FACTION_HOSTILE_CORE);

                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    summon->AI()->AttackStart(target);
            }

            _summons.Summon(summon);
        }

        void JustDied(Unit* /*pKiller*/)
        {
            _summons.DespawnAll();

            // TODO: unhack
            Map* map = me->GetMap();
            if(map && map->IsDungeon())
            {
                Map::PlayerList const& players = map->GetPlayers();
                if(!players.isEmpty())
                    for(Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                        if(Player* player = i->getSource())
                            if(player->GetDistance(me) < 100.0f)
                                sLFGMgr->RewardDungeonDoneFor(287, player);
            }
        }

    private:
        SummonList _summons;
        uint64 _add[3];
        uint32 _addTimer;
        uint32 _disarmTimer;
        bool _spawnedIlsa;
        bool _spawnedUrsula;
    };

    CreatureAI* GetAI(Creature* c) const
    {
        return new npc_coren_direbrewAI(c);
    }
};

enum Brewmaiden
{
    SPELL_SEND_FIRST_MUG = 47333,
    SPELL_SEND_SECOND_MUG = 47339,
    //SPELL_CREATE_BREW = 47345,
    SPELL_HAS_BREW_BUFF = 47376,
    //SPELL_HAS_BREW = 47331,
    //SPELL_DARK_BREWMAIDENS_STUN = 47340,
    SPELL_CONSUME_BREW = 47377,
    SPELL_BARRELED = 47442,
    SPELL_CHUCK_MUG = 50276
};

class npc_brewmaiden : public CreatureScript
{
public:
    npc_brewmaiden() : CreatureScript("npc_brewmaiden") { }

    struct npc_brewmaidenAI : public ScriptedAI
    {
        npc_brewmaidenAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        void Reset()
        {
            _brewTimer = 2000;
            _barrelTimer = 5000;
            _chuckMugTimer = 10000;
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            me->SetInCombatWithZone();
        }

        void AttackStart(Unit* who)
        {
            if(!who)
                return;

            if(me->Attack(who, true))
            {
                me->AddThreat(who, 1.0f);
                me->SetInCombatWith(who);
                who->SetInCombatWith(me);

                if(me->GetEntry() == NPC_URSULA_DIREBREW)
                    me->GetMotionMaster()->MoveFollow(who, 10.0f, 0.0f);
                else
                    me->GetMotionMaster()->MoveChase(who);
            }
        }

        void SpellHitTarget(Unit* target, SpellEntry const* spell)
        {
            // TODO: move to DB
            if(spell->Id == SPELL_SEND_FIRST_MUG)
                target->CastSpell(target, SPELL_HAS_BREW_BUFF, true);

            if(spell->Id == SPELL_SEND_SECOND_MUG)
                target->CastSpell(target, SPELL_CONSUME_BREW, true);
        }

        void UpdateAI(uint32 const diff)
        {
            if(!UpdateVictim())
                return;

            if(_brewTimer <= diff)
            {
                if(!me->IsNonMeleeSpellCasted(false))
                {
                    Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true);

                    if(target && me->GetDistance(target) > 5.0f)
                    {
                        DoCast(target, SPELL_SEND_FIRST_MUG);
                        _brewTimer = 12000;
                    }
                }
            } else _brewTimer -= diff;

            if(_chuckMugTimer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_CHUCK_MUG);

                _chuckMugTimer = 15000;
            } else _chuckMugTimer -= diff;

            if(me->GetEntry() == NPC_URSULA_DIREBREW)
            {
                if(_barrelTimer <= diff)
                {
                    if(!me->IsNonMeleeSpellCasted(false))
                    {
                        DoCastVictim(SPELL_BARRELED);
                        _barrelTimer = urand(15, 18) *IN_MILLISECONDS;
                    }
                } else _barrelTimer -= diff;
            } else DoMeleeAttackIfReady();
        }

    private:
        uint32 _brewTimer;
        uint32 _barrelTimer;
        uint32 _chuckMugTimer;
    };

    CreatureAI* GetAI(Creature* c) const
    {
        return new npc_brewmaidenAI(c);
    }
};

enum MoleMachineConsole
{
    SPELL_TELEPORT = 49466 // bad id?
};

#define GOSSIP_ITEM_MOLE_CONSOLE "[PH] Please teleport me."

class go_mole_machine_console : public GameObjectScript
{
public:
    go_mole_machine_console() : GameObjectScript("go_mole_machine_console") { }

    bool OnGossipHello (Player* player, GameObject* go)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_MOLE_CONSOLE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(12709, go->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, GameObject* /*pGo*/, uint32 /*sender*/, uint32 action)
    {
        if(action == GOSSIP_ACTION_INFO_DEF + 1)
            player->CastSpell(player, SPELL_TELEPORT, true);

         return true;
    }
};

enum Brewfest
{
    NPC_TAPPER_SWINDLEKEG       = 24711,
    NPC_IPFELKOFER_IRONKEG      = 24710,
    AT_BREWFEST_DUROTAR         = 4829,
    AT_BREWFEST_DUN_MOROGH      = 4820,
    SAY_WELCOME                 = 1,
    AREATRIGGER_TALK_COOLDOWN   = 5, // in seconds
};

class AreaTrigger_at_brewfest : public AreaTriggerScript
{
public:
    AreaTrigger_at_brewfest() : AreaTriggerScript("at_brewfest")
    {
        // Initialize for cooldown
        _triggerTimes[AT_BREWFEST_DUROTAR] = _triggerTimes[AT_BREWFEST_DUN_MOROGH] = 0;
    }

    bool OnTrigger(Player* player, AreaTriggerEntry const* trigger)
    {
        uint32 triggerId = trigger->id;
        // Second trigger happened too early after first, skip for now
        if(sWorld->GetGameTime() - _triggerTimes[triggerId] < AREATRIGGER_TALK_COOLDOWN)
            return false;

        switch(triggerId)
        {
            case AT_BREWFEST_DUROTAR:
                if(Creature* tapper = player->FindNearestCreature(NPC_TAPPER_SWINDLEKEG, 20.0f))
                    tapper->AI()->Talk(SAY_WELCOME, player->GetGUID());
                break;
            case AT_BREWFEST_DUN_MOROGH:
                if(Creature* ipfelkofer = player->FindNearestCreature(NPC_IPFELKOFER_IRONKEG, 20.0f))
                    ipfelkofer->AI()->Talk(SAY_WELCOME, player->GetGUID());
                break;
            default:
                break;
        }

        _triggerTimes[triggerId] = sWorld->GetGameTime();
        return false;
    }

private:
    std::map<uint32, time_t> _triggerTimes;
};

void AddSC_brewfest_event()
{
    new spell_brewfest_speed;
    new spell_brewfest_speed_normal;
    new item_brewfest_ram_reins;
    new npc_brewfest_trigger;
    new npc_brewfest_apple_trigger;
    new npc_brewfest_keg_thrower;
    new npc_brewfest_keg_receiver;
    new npc_brewfest_ram_master;
    new npc_coren_direbrew;
    new npc_brewmaiden;
    new go_mole_machine_console;
    new AreaTrigger_at_brewfest;
}
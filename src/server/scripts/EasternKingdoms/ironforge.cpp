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

#define GOSSIP_ITEM_ROYAL   "I am ready to listen"
#define GOSSIP_ITEM_ROYAL_1 "That is tragic. How did this happen?"
#define GOSSIP_ITEM_ROYAL_2 "Interesting, continue please."
#define GOSSIP_ITEM_ROYAL_3 "Unbelievable! How dare they??"
#define GOSSIP_ITEM_ROYAL_4 "Of course I will help!"

class npc_royal_historian_archesonus : public CreatureScript
{
public:
    npc_royal_historian_archesonus() : CreatureScript("npc_royal_historian_archesonus") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(2236, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                player->SEND_GOSSIP_MENU(2237, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                player->SEND_GOSSIP_MENU(2238, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                player->SEND_GOSSIP_MENU(2239, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                player->CLOSE_GOSSIP_MENU();
                player->AreaExploredOrEventHappens(3702);
                break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if(player->GetQuestStatus(3702) == QUEST_STATUS_INCOMPLETE)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            player->SEND_GOSSIP_MENU(2235, creature->GetGUID());
        }
        else
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }
};

class npc_captain_tread_sparknozzle : public CreatureScript
{
public:
    npc_captain_tread_sparknozzle() : CreatureScript("npc_captain_tread_sparknozzle") { }

    struct npc_captain_tread_sparknozzleAI : public ScriptedAI
    {
        npc_captain_tread_sparknozzleAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        void MoveInLineOfSight(Unit* who)
        {
            ScriptedAI::MoveInLineOfSight(who);

            if(who->GetTypeId() == TYPEID_PLAYER)
            {
                if(((Player*)who)->GetQuestStatus(25229) == QUEST_STATUS_INCOMPLETE)
                {
                    std::list<Creature*> GnomeList;
                    me->GetCreatureListWithEntryInGrid(GnomeList, 39623, 7.5f);
                    if(!GnomeList.empty())
                    {
                        for(std::list<Creature*>::const_iterator itr = GnomeList.begin(); itr != GnomeList.end(); ++itr)
                        {
                            if(Creature* pCreature = *itr)
                            {
                                pCreature->ForcedDespawn();
                                ((Player*)who)->KilledMonsterCredit(39466, 0);
                            }
                        }
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_captain_tread_sparknozzleAI(pCreature);
    }
};

uint32 const gnomemodel[] =
{ 3040, 7852, 15453, 6975, 4999, 10216, 7041, 7028, 3040, 6975, 2184, 7038, 7039 };

class npc_gnome_volunteer : public CreatureScript   //TODO: join with npc_troll_volunteer
{
public:
    npc_gnome_volunteer() : CreatureScript("npc_gnome_volunteer") { }

    struct npc_gnome_volunteerAI : public ScriptedAI
    {
        enum
        {
            SPELL_VOLUNTEER_AURA_OG = 74034,
            SPELL_MOUNTING_CHECK    = 75420,
            SPELL_PETACT_AURA       = 74071,
            SPELL_AOE_TURNIN_OG     = 73955,
            SPELL_TURNIN            = 73953,
            SPELL_QUEST_CREDIT_OG   = 73960,
            NPC_SPARKNOZZLE         = 39675,
            SAY_VOLUNTEER_START     = 0,
            SAY_VOLUNTEER_END       = 1,
            POINT_SPARKNOZZLE       = 2,
        };

        npc_gnome_volunteerAI(Creature* creature) : ScriptedAI(creature)
        {
            _mountModel = RAND(14377, 14374, 14376, 9474, 9476, 6569, 10661, 10666, 9473);
            creature->SetDisplayId(gnomemodel[urand(0, 12)]);
        }

        void Reset()
        {
            _complete = false;
            me->AddAura(SPELL_VOLUNTEER_AURA_OG, me);
            me->AddAura(SPELL_MOUNTING_CHECK, me);
            DoCast(me, SPELL_PETACT_AURA);
            me->SetReactState(REACT_PASSIVE);
            Talk(SAY_VOLUNTEER_START);
        }

        void IsSummonedBy(Unit* owner)
        {
            if(Player* player = owner->ToPlayer())
            {
                me->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE + M_PI/urand(1, 6));
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
            }
        }

        void DoAction(const int32 type)
        {
            if(type == SPELL_MOUNTING_CHECK && _mountModel)
            {
                me->Mount(_mountModel);
                if(Player* player = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                    me->SetSpeed(MOVE_RUN, player->GetTotalAuraModifier(SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED)/100.0f+1.0f);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if(type == POINT_MOTION_TYPE && id == POINT_SPARKNOZZLE)
                me->DespawnOrUnsummon();
        }

        void SpellHit(Unit* caster, SpellEntry const* spell)
        {
            if(spell->Id == SPELL_AOE_TURNIN_OG && caster->GetEntry() == NPC_SPARKNOZZLE && !_complete)
            {
                _complete = true;    // Preventing from giving credit twice
                me->RemoveAurasDueToSpell(SPELL_MOUNTING_CHECK);
                me->Unmount();
                DoCast(me, SPELL_TURNIN);
                DoCast(me, SPELL_QUEST_CREDIT_OG);
                Talk(SAY_VOLUNTEER_END);
                me->GetMotionMaster()->MovePoint(POINT_SPARKNOZZLE, caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ());
            }
        }

    private:
        uint32 _mountModel;
        bool _complete;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_gnome_volunteerAI(creature);
    }
};

class spell_motivate_a_tron : public SpellScriptLoader  //TODO join with spell_voljin_war_drums
{
public:
    spell_motivate_a_tron() : SpellScriptLoader("spell_motivate_a_tron") {}

    class spell_motivate_a_tron_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_motivate_a_tron_SpellScript);

        enum
        {
            SPELL_MOTIVATE_1    = 73943,
            SPELL_MOTIVATE_2    = 74080,
            NPC_CITIZEN_1       = 39253,
            NPC_CITIZEN_2       = 39623,
        };

        bool Validate(SpellEntry const * /*spellEntry*/)
        {
            if(!sSpellStore.LookupEntry(SPELL_MOTIVATE_1))
                return false;
            if(!sSpellStore.LookupEntry(SPELL_MOTIVATE_2))
                return false;
           return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if(Unit* target = GetHitUnit())
            {
                uint32 motivate = 0;
                if(target->GetEntry() == NPC_CITIZEN_1)
                    motivate = SPELL_MOTIVATE_1;
                else if(target->GetEntry() == NPC_CITIZEN_2)
                    motivate = SPELL_MOTIVATE_2;
                if(motivate)
                {
                    caster->CastSpell(target, motivate, true, NULL, NULL, caster->GetGUID());
                    target->ToCreature()->DespawnOrUnsummon();
                }
            }
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_motivate_a_tron_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_motivate_a_tron_SpellScript();
    }
};

static const uint8 delaySparknozzle[20] = { 3,3,2,3,5, 2,4,2,5, 2,3,4,5, 2,3,2,5, 2,3, };

class npc_sergeant_steamcrank : public CreatureScript
{
public:
    npc_sergeant_steamcrank() : CreatureScript("npc_sergeant_steamcrank") {}

    enum
    {
        QUEST_BASIC_ORDERS      = 25199,
        SPELL_TEST_SALUTE       = 73835,
        SPELL_SALUTE_VALID      = 73770,
        SPELL_SALUTE_QC         = 73771,
        SPELL_TEST_ROAR         = 73836,
        SPELL_ROAR_VALID        = 73831,
        SPELL_ROAR_QC           = 73832,
        SPELL_TEST_CHEER        = 73725,
        SPELL_CHEER_VALID       = 73834,
        SPELL_CHEER_QC          = 73833,
        SPELL_TEST_DANCE        = 73837,
        SPELL_DANCE_VALID       = 73829,
        SPELL_DANCE_QC          = 73830,
        SPELL_STOP_DANCE        = 73886,
    };

    struct npc_sergeant_steamcrankAI : public ScriptedAI
    {
        npc_sergeant_steamcrankAI(Creature *c) : ScriptedAI(c) {}

        void Reset()
        {
            timer = 0;
            stage = 0;
            active = false;
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void MoveInLineOfSight(Unit* who)
        {
            if(active || who->GetTypeId() != TYPEID_PLAYER)
                return;
            if(who->ToPlayer()->GetQuestStatus(QUEST_BASIC_ORDERS) != QUEST_STATUS_INCOMPLETE)
                return;
            if(!me->IsInRange(who, 0.0f, 15.0f, false))
                return;
            stage = 0;
            active = true;
            timer = 0;
        }

        void ReceiveEmote(Player* player, uint32 emoteId)
        {
            switch(emoteId)
            {
            case TEXT_EMOTE_SALUTE:
                DoCast(player, SPELL_SALUTE_QC);
                break;
            case TEXT_EMOTE_ROAR:
                DoCast(player, SPELL_ROAR_QC);
                break;
            case TEXT_EMOTE_CHEER:
                DoCast(player, SPELL_CHEER_QC);
                break;
            case TEXT_EMOTE_DANCE:
                DoCast(player, SPELL_DANCE_QC);
                break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!active)
                return;

            if(timer < diff)
            {
                Talk(stage);
                timer = delaySparknozzle[stage]*IN_MILLISECONDS;
                switch(++stage)
                {
                case 5:
                    DoCastAOE(SPELL_TEST_SALUTE);
                    DoCast(me, SPELL_SALUTE_VALID);
                    break;
                case 9:
                    DoCastAOE(SPELL_TEST_ROAR);
                    DoCast(me, SPELL_ROAR_VALID);
                    break;
                case 13:
                    DoCastAOE(SPELL_TEST_CHEER);
                    DoCast(me, SPELL_CHEER_VALID);
                    break;
                case 17:
                    DoCastAOE(SPELL_TEST_DANCE);
                    DoCast(me, SPELL_DANCE_VALID);
                    break;
                case 19:
                    DoCastAOE(SPELL_STOP_DANCE);
                    active = false;
                    stage = 0;
                    break;
                default:
                    break;
                }
            }
            else timer -= diff;
        }

    private:
        uint32 timer;
        uint8 stage;
        bool active;
    };

    CreatureAI* GetAI(Creature *c) const
    {
        return new npc_sergeant_steamcrankAI(c);
    }
};

class spell_test_gnomeregan_trainee : public SpellScriptLoader
{
public:
    spell_test_gnomeregan_trainee(const char* name, uint32 emote) : SpellScriptLoader(name), _emote(emote) {}

    class spell_test_gnomeregan_trainee_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_test_gnomeregan_trainee_AuraScript);

    public:
        spell_test_gnomeregan_trainee_AuraScript(uint32 emote) : AuraScript(), _emote(emote) {}

        void HandleDummyTick(AuraEffect const* /*aurEff*/)
        {
            //sLog->outBasic("Handle dummy tick for emote %u, caster %s, target %s!", _emote, GetCaster()->GetName(), GetTarget()->GetName());
            GetTarget()->SetUInt64Value(UNIT_FIELD_TARGET, GetCaster()->GetGUID());
            GetTarget()->HandleEmoteCommand(_emote);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_test_gnomeregan_trainee_AuraScript::HandleDummyTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }

    private:
        uint32 _emote;
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_test_gnomeregan_trainee_AuraScript(_emote);
    }
private:
    uint32 _emote;
};

enum ePerformingSpeech
{
    NPC_OZZIE_TOGGLEVOLT        = 1268,
    NPC_MILLI_FEATHERWHISTLE    = 7955,
    NPC_TOG_RUSTSPROCKET        = 6119,
    SPELL_KC_OZZIE  = 74154,
    SPELL_KC_MILLI  = 74155,
    SPELL_KC_TOG    = 74156,
    TEXT_OZZIE_1    = 0,
    TEXT_OZZIE_2    = 1,
    TEXT_OZZIE_3    = 2,
    TEXT_MILLI_1    = 3,
    TEXT_MILLI_2    = 4,
    TEXT_TOG_1      = 5,
    TEXT_TOG_2      = 6,
    TEXT_TOG_3      = 7,
};

class spell_og_perform_speech : public SpellScriptLoader
{
public:
    spell_og_perform_speech() : SpellScriptLoader("spell_og_perform_speech") {}

    class spell_og_perform_speech_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_og_perform_speech_SpellScript);

        SpellCastResult CheckTarget()
        {
            Unit* target = GetTargetUnit();
            if(!target)
                return SPELL_FAILED_BAD_TARGETS;
            if(target->GetEntry() == NPC_OZZIE_TOGGLEVOLT || target->GetEntry() == NPC_MILLI_FEATHERWHISTLE || target->GetEntry() == NPC_TOG_RUSTSPROCKET)
                return SPELL_CAST_OK;
            return SPELL_FAILED_BAD_TARGETS;
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_og_perform_speech_SpellScript::CheckTarget);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_og_perform_speech_SpellScript();
    }
};

class npc_mekkatorque_vision : public CreatureScript
{
public:
    npc_mekkatorque_vision() : CreatureScript("npc_mekkatorque_vision") {}

    struct npc_mekkatorque_visionAI : public Scripted_NoMovementAI
    {
        npc_mekkatorque_visionAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) {}

        void Reset()
        {
            events.Reset();
            targetGUID = 0;
        }

        void IsSummonedBy(Unit* pWho)
        {
            if(pWho->GetTypeId() != TYPEID_PLAYER)
                return;
            Creature* target = NULL;
            if(target = me->FindNearestCreature(NPC_OZZIE_TOGGLEVOLT, 13.0f))
            {
                Talk(TEXT_OZZIE_1);
                events.ScheduleEvent(TEXT_OZZIE_2, 5*IN_MILLISECONDS);
                events.ScheduleEvent(TEXT_OZZIE_3, 7*IN_MILLISECONDS);
                events.ScheduleEvent(NPC_OZZIE_TOGGLEVOLT, 9100);
            }
            else if(target = me->FindNearestCreature(NPC_MILLI_FEATHERWHISTLE, 13.0f))
            {
                Talk(TEXT_MILLI_1);
                events.ScheduleEvent(TEXT_MILLI_2, 3*IN_MILLISECONDS);
                events.ScheduleEvent(NPC_MILLI_FEATHERWHISTLE, 5100);
            }
            else if(target = me->FindNearestCreature(NPC_TOG_RUSTSPROCKET, 13.0f))
            {
                Talk(TEXT_TOG_1);
                events.ScheduleEvent(TEXT_TOG_2, 3*IN_MILLISECONDS);
                events.ScheduleEvent(TEXT_TOG_3, 5*IN_MILLISECONDS);
                events.ScheduleEvent(NPC_TOG_RUSTSPROCKET, 8100);
            }
            if(target)
                targetGUID = target->GetGUID();
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);
            if(uint32 eventId = events.ExecuteEvent())
            {
                if(eventId < 100)
                    Talk(eventId);
                else
                {
                    //DoCast(me, eventId, true);
                    if(Player *pl = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                        pl->KilledMonsterCredit(eventId, 0);
                    if(Creature *target = ObjectAccessor::GetCreature(*me, targetGUID))
                        sCreatureTextMgr->SendChat(target, 0);
                    me->DespawnOrUnsummon(500);
                }
            }
        }

    private:
        EventMap events;
        uint64 targetGUID;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mekkatorque_visionAI(pCreature);
    }
};

void AddSC_ironforge()
{
    new npc_captain_tread_sparknozzle;
    new npc_royal_historian_archesonus;
    new npc_sergeant_steamcrank;
    new npc_gnome_volunteer;
    new spell_motivate_a_tron;
    new spell_test_gnomeregan_trainee("spell_test_salute", EMOTE_ONESHOT_SALUTE);
    new spell_test_gnomeregan_trainee("spell_test_roar", EMOTE_ONESHOT_ROAR);
    new spell_test_gnomeregan_trainee("spell_test_cheer", EMOTE_ONESHOT_CHEER);
    new spell_test_gnomeregan_trainee("spell_test_dance", EMOTE_STATE_DANCE);
    new spell_test_gnomeregan_trainee("spell_test_stop_dance", EMOTE_ONESHOT_NONE);
    new spell_og_perform_speech;
    new npc_mekkatorque_vision;
}
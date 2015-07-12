/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Copyright (C) 2012 SymphonyArt <http://symphonyart.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#include "ScriptPCH.h"
#include "slave_pens.h"

enum ahune_spells
{
    /* Ahune */
    SPELL_AHUNES_SHIELD         = 45954,
    SPELL_AHUNE_SLIPPERY_FLOOR  = 46314,
    SPELL_KNOCKED_UP            = 46014,
    SPELL_KNOCKDOWN_STUN        = 68848, //definitely wrong but effect seems OK
    SPELL_YOU_SLIPPED           = 45946,
    SPELL_COLD_SLAP             = 46198,
    SPELL_AHUNE_RESURFACES      = 46402,
    SPELL_SUMMON_VISUAL         = 45937,
    SPELL_SUMMON_VISUAL_H       = 45938,
    SPELL_BONFIRE               = 45930,
    SPELL_SUMMON_HAILSTONE      = 46176,
    SPELL_SUMMON_COLDWAVE       = 46143,
    SPELL_SUMMON_FROSTWIND      = 46382,
    SPELL_SUMMON_ICE_SPEAR_BUNNY= 46359,
    SPELL_SUMMON_LOOT_MISSILE   = 45941,
    SPELL_SUMMON_LOOT           = 45939,
    SPELL_SUMMON_LOOT_MISSILE_H = 46623,
    SPELL_SUMMON_LOOT_H         = 46622,
    SPELL_SUMMON_LOOT_BOTTLES   = 46891,
    SPELL_SUMMON_LOOT_BOTTLES_H = 46892,
    //Ice Spear Bunny
    SPELL_SUMMON_ICE_SPEAR_OBJ  = 46369,
    SPELL_ICE_SPEAR_KNOCKBACK   = 46360,

    /* Ahunite Hailstone */
    SPELL_CHILLING_AURA     = 46542,
    SPELL_PULVERIZE         = 2676,

    /* Ahunite Frostwind */
    SPELL_WIND_BUFFET       = 46568,
    SPELL_LIGHTNING_SHIELD  = 12550,

    /* Ahunite Coldwave */
    SPELL_BITTER_BLAST      = 46406,
};
enum ahune_events
{
    EVENT_COLD          = 1,
    EVENT_ICE_SPIKES,
    EVENT_SUMMON_HAILSTONE,
    EVENT_SUMMON_FROSTWIND,
    EVENT_SUMMON_COLDWAVE,
    EVENT_PHASE_ONE,
    EVENT_PHASE_TWO,
    EVENT_ANNOUNCE_EMERGE,
    EVENT_COOLDOWN,
    // trash events
    EVENT_PULVERIZE,
    EVENT_WIND_BUFFET,
    EVENT_BITTER_BLAST,
};
enum ahune_textes
{
    TEXTID_EMERGE   = -1999977,
    TEXTID_SUBMERGE = -1999978,
    TEXTID_EVADE    = -1999979,
    TEXTID_AGGRO    = -1999980,
};
enum ahune_phases
{
    PHASE_1     = 1,
    PHASE_2,
};
enum ahune_miscdata
{
    SOUNDID_SUMMON  = 97999,
    GO_ICE_SPEAR_OBJ= 188077,
    GO_ICE_CHEST    = 187892,
    GO_ICE_CHEST_H  = 188124,
};

class boss_ahune : public CreatureScript
{
public:
    boss_ahune() : CreatureScript("boss_ahune") { }

    struct boss_ahuneAI : public BossAI
    {
        boss_ahuneAI(Creature* creature) : BossAI(creature, DATA_AHUNE)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        uint64 FrozenCoreGUID;
        uint8 phase,round;

        void Reset()
        {
            me->setFaction(14);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
            me->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
            me->SetReactState(REACT_AGGRESSIVE);

            FrozenCoreGUID = 0;
            phase = 0;
            round = 1;
            BossAI::Reset();
        }

        void JustDied(Unit* /*pWho*/)
        {
            DoCast(GetDifficulty() == DUNGEON_DIFFICULTY_NORMAL ? SPELL_SUMMON_LOOT_MISSILE : SPELL_SUMMON_LOOT_MISSILE_H);
            //BossAI::JustDied(who);
        }

        void JustSummoned(Creature* pSummon)
        {
            switch(pSummon->GetEntry())
            {
                case NPC_AHUNITE_HAILSTONE:
                case NPC_AHUNITE_FROSTWIND:
                case NPC_AHUNITE_COLDWAVE:
                    break;
                case NPC_AHUNE_FROZEN_CORE:
                    FrozenCoreGUID = pSummon->GetGUID();
                    break;
                case NPC_AHUNE_SUMMONLOC_BUNNY:
                    pSummon->DespawnOrUnsummon(400);
                    //no break intended
                default:
                    return;
            }
            summons.Summon(pSummon);
            pSummon->SetReactState(REACT_AGGRESSIVE);
        }

        void EnterCombat(Unit* who)
        {
            DoScriptText(TEXTID_AGGRO, me);
            DoSummon(NPC_AHUNE_FROZEN_CORE, *me, 86400000);
            EnterPhaseOne();
            BossAI::EnterCombat(who);

            //DoCastAOE(GetDifficulty() == DUNGEON_DIFFICULTY_NORMAL ? SPELL_SUMMON_VISUAL : SPELL_SUMMON_VISUAL_H, true);
            DoCast(me, SPELL_AHUNES_SHIELD, true);
        }

        void EnterPhaseTwo()
        {
            round++;
            //sLog->outBasic("TSCR Ahune: phase 2 with health %f", me->GetHealthPct());
            phase = PHASE_2;

            DoCast(me, SPELL_BONFIRE, true);
            DoScriptText(TEXTID_SUBMERGE, me);

            DoStopAttack();
            me->SetReactState(REACT_PASSIVE);
            me->InterruptNonMeleeSpells(false);
            me->setFaction(35);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);
            me->HandleEmoteCommand(EMOTE_ONESHOT_SUBMERGE);

            if(Unit* FrozenCore = ObjectAccessor::GetUnit(*me, FrozenCoreGUID))
            {
                FrozenCore->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                FrozenCore->SetVisible(true);
            } else if(instance->GetBossState(DATA_AHUNE) == IN_PROGRESS) {
                me->Kill(me, false);
                return;
            }
            events.Reset();
            events.ScheduleEvent(EVENT_ANNOUNCE_EMERGE, 25000);
            events.ScheduleEvent(EVENT_PHASE_ONE, 30000);
        }

        void EnterPhaseOne()
        {
            phase = PHASE_1;
            DoCastAOE(SPELL_AHUNE_RESURFACES);
            me->SetReactState(REACT_AGGRESSIVE);
            me->setFaction(14);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
            me->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);

            if(Unit* FrozenCore = ObjectAccessor::GetUnit(*me, FrozenCoreGUID))
            {
                FrozenCore->SetVisible(false);
                FrozenCore->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                FrozenCore->RemoveAllAuras();
            }
            events.Reset();
            events.ScheduleEvent(EVENT_SUMMON_HAILSTONE, 2000);
            events.ScheduleEvent(EVENT_SUMMON_FROSTWIND, 3000);
            events.ScheduleEvent(EVENT_SUMMON_COLDWAVE, 4000);
            events.ScheduleEvent(EVENT_ICE_SPIKES, 7500);
            events.ScheduleEvent(EVENT_COLD, 10000);
            events.ScheduleEvent(EVENT_PHASE_TWO, 90000);
        }

        void UpdateAI(uint32 const diff)
        {
            if(!UpdateVictim())
                return;

            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_COLD:
                        DoCastVictim(SPELL_COLD_SLAP);
                        events.ScheduleEvent(EVENT_COLD, 10000);
                        break;
                    case EVENT_SUMMON_HAILSTONE:
                        SpawnSummonLocBunnies(round);
                        DoCastAOE(SPELL_SUMMON_HAILSTONE);
                        events.ScheduleEvent(EVENT_SUMMON_HAILSTONE, urand(17500, 20000));
                        break;
                    case EVENT_SUMMON_COLDWAVE:
                        SpawnSummonLocBunnies(round);
                        DoCastAOE(SPELL_SUMMON_COLDWAVE);
                        events.ScheduleEvent(EVENT_SUMMON_FROSTWIND, urand(12500, 15000));
                        break;
                    case EVENT_SUMMON_FROSTWIND:
                        SpawnSummonLocBunnies(2*round);
                        DoCastAOE(SPELL_SUMMON_FROSTWIND);
                        events.ScheduleEvent(EVENT_SUMMON_COLDWAVE, urand(10000, 12000));
                        break;
                    case EVENT_PHASE_TWO:
                        EnterPhaseTwo();
                        return;
                    case EVENT_PHASE_ONE:
                        EnterPhaseOne();
                        break;
                    case EVENT_ANNOUNCE_EMERGE:
                        DoScriptText(TEXTID_EMERGE, me);
                        break;
                    case EVENT_ICE_SPIKES:
                        if(Unit* victim = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                            DoCast(victim, SPELL_SUMMON_ICE_SPEAR_BUNNY);
                        events.ScheduleEvent(EVENT_ICE_SPIKES, urand(6000, 7500));
                        break;
                }
            }
            if(phase == PHASE_1)
                DoMeleeAttackIfReady();
        }

        void EnterEvadeMode()
        {
            DoScriptText(TEXTID_EVADE, me);
            BossAI::EnterEvadeMode();
        }

        void SpawnSummonLocBunnies(uint8 num)
        {
            std::list<Unit*> targetList;
            SelectTargetList(targetList, num, SELECT_TARGET_RANDOM, 0.0f, true);
            for(std::list<Unit*>::const_iterator it = targetList.begin(); it != targetList.end(); ++it)
                DoSummon(NPC_AHUNE_SUMMONLOC_BUNNY, *it);
            //TODO: if num>5
            //if(!targetList.empty())
            //    DoPlaySoundToSet(me, SOUNDID_SUMMON);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ahuneAI(creature);
    }
};

class npc_ahunite_hailstone : public CreatureScript
{
public:
    npc_ahunite_hailstone() : CreatureScript("npc_ahunite_hailstone") { }

    struct npc_ahunite_hailstoneAI : public ScriptedAI
    {
        npc_ahunite_hailstoneAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void Reset()
        {
            events.Reset();

            if(Unit* target = GetPlayerAtMinimumRange(0.0f))
                AttackStartNoMove(target);

            //me->AddAura(SPELL_CHILLING_AURA, me);
            DoCast(me, SPELL_CHILLING_AURA, true);
            events.ScheduleEvent(EVENT_COLD, 3000);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_PULVERIZE:
                        DoCastAOE(SPELL_PULVERIZE);
                        events.ScheduleEvent(EVENT_COLD, urand(5000, 6000));
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ahunite_hailstoneAI(creature);
    }
};

class npc_ahunite_frostwind : public CreatureScript
{
public:
    npc_ahunite_frostwind() : CreatureScript("npc_ahunite_frostwind") { }

    struct npc_ahunite_frostwindAI : public ScriptedAI
    {
        npc_ahunite_frostwindAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void Reset()
        {
            events.Reset();

            if(Unit* target = GetPlayerAtMinimumRange(0.0f))
                AttackStartNoMove(target);

            //me->AddAura(SPELL_LIGHTING_SHIELD, me);
            DoCast(me, SPELL_LIGHTNING_SHIELD, true);
            events.ScheduleEvent(EVENT_WIND_BUFFET, 3000);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                case EVENT_WIND_BUFFET:
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                        DoCast(target, SPELL_WIND_BUFFET, true);
                    events.ScheduleEvent(EVENT_WIND_BUFFET, urand(5000, 6000));
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ahunite_frostwindAI(creature);
    }
};

class npc_ahunite_coldwave : public CreatureScript
{
public:
    npc_ahunite_coldwave() : CreatureScript("npc_ahunite_coldwave") {}

    struct npc_ahunite_coldwaveAI : public ScriptedAI
    {
        npc_ahunite_coldwaveAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void Reset()
        {
            events.Reset();

            if(Unit* target = GetPlayerAtMinimumRange(0.0f))
                AttackStartNoMove(target);

            events.ScheduleEvent(EVENT_BITTER_BLAST, 3000);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_BITTER_BLAST:
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                            DoCast(target, SPELL_BITTER_BLAST);

                        events.ScheduleEvent(EVENT_BITTER_BLAST, urand(5000, 6000));
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ahunite_coldwaveAI(creature);
    }
};

class boss_ahune_frostcore : public CreatureScript
{
public:
    boss_ahune_frostcore() : CreatureScript("boss_ahune_frostcore") { }

    struct boss_ahune_frostcoreAI : public ScriptedAI
    {
        boss_ahune_frostcoreAI(Creature* creature) : ScriptedAI(creature)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetVisible(false);
        }

        void DamageTaken(Unit* who, uint32 &dmg)    //copy the damage to Ahune
        {
            if(Unit* ahune = me->ToTempSummon()->GetSummoner())
                me->DealDamage(ahune, dmg);
        }

        void JustDied(Unit* /*pVictim*/)
        {
            if(Unit* ahune = me->ToTempSummon()->GetSummoner())
            {
                ahune->SetVisible(true);
                me->Kill(ahune, false);
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_ahune_frostcoreAI(pCreature);
    }
};

class npc_ahune_ice_spear_bunny : public CreatureScript
{
public:
    npc_ahune_ice_spear_bunny() : CreatureScript("npc_ahune_ice_spear_bunny") {}

    struct npc_ahune_ice_spear_bunnyAI : public Scripted_NoMovementAI
    {
        npc_ahune_ice_spear_bunnyAI(Creature *c) : Scripted_NoMovementAI(c) {}

        uint32 timer;

        void Reset()
        {
            DoCast(SPELL_SUMMON_ICE_SPEAR_OBJ);
            timer = 3000;
        }

        void JustDied(Unit* /*who*/)
        {
            if(GameObject *ice = me->FindNearestGameObject(GO_ICE_SPEAR_OBJ, 1.0f))
                ice->Use(me);
        }

        void UpdateAI(uint32 const diff)
        {
            if(timer < diff)
            {
                if(GameObject *ice = me->FindNearestGameObject(GO_ICE_SPEAR_OBJ, 1.0f))
                {
                    //ice->SetSpawnedByDefault(false);
                    ice->Use(me);
                }
                DoCastAOE(SPELL_ICE_SPEAR_KNOCKBACK, true);
                me->DespawnOrUnsummon(3000);
                timer = 10000;
            } else timer -= diff;
        }
    };

    CreatureAI* GetAI(Creature *c) const
    {
        return new npc_ahune_ice_spear_bunnyAI(c);
    }
};

//class go_ice_stone : public GameObjectScript    //BAD! TODO: THREAD SAFETY!!!!! tis bad for 2 or more instance copies simultaneously!
//{
//public:
//    go_ice_stone() : GameObjectScript("go_ice_stone") { }
//
//    bool OnGossipSelect(Player* player, GameObject* gameObject, uint32 sender, uint32 action)
//    {
//        player->PlayerTalkClass->ClearMenus();
//        player->CLOSE_GOSSIP_MENU();
//
//        switch(action)
//        {
//        case PHASE_1:
//            if(Creature* Ahune = gameObject->FindNearestCreature(NPC_AHUNE, 500.0f, true))
//            {
//                Ahune->SetVisible(true);
//                Ahune->SetReactState(REACT_AGGRESSIVE);
//            }
//
//            //gameObject->MonsterTextEmote(GOSSIP_ITEM, player->GetGUID(), true);
//            break;
//        }
//        return true;
//    }
//
//    bool OnGossipHello(Player* player, GameObject* gameObject)
//    {
//        if(player->getLevel() < 80)
//            return false;
//
//        if(Creature* Ahune = gameObject->FindNearestCreature(NPC_AHUNE, 500.0f, true))
//            if(Ahune->isAlive() && Ahune->GetReactState() == REACT_PASSIVE)
//                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "BIER, BIER, BIER, BIER, BIER, BIER, BIER, BIER, BIER, BIER, BIER, BIER!", GOSSIP_SENDER_MAIN, PHASE_1);
//
//        player->SEND_GOSSIP_MENU(gameObject->GetGOInfo()->GetGossipMenuId(), gameObject->GetGUID());
//        return true;
//    }
//};

class spell_slippery_floor_periodic : public SpellScriptLoader  //46320
{
public:
    spell_slippery_floor_periodic() : SpellScriptLoader("spell_slippery_floor_periodic") {}

    class spell_slippery_floor_periodic_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_slippery_floor_periodic_SpellScript);

        void HandleScript(SpellEffIndex effIndex)
        {
            if(GetHitUnit()->isMoving() && !GetHitUnit()->HasUnitMovementFlag(MOVEMENTFLAG_WALKING))
            {
                GetCaster()->CastSpell(GetHitUnit(), SPELL_YOU_SLIPPED, true);
                GetHitUnit()->CastSpell(GetHitUnit(), SPELL_KNOCKDOWN_STUN, true);
            }
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_slippery_floor_periodic_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_slippery_floor_periodic_SpellScript();
    }
};

class spell_ice_spear_knockback : public SpellScriptLoader //46360
{
public:
    spell_ice_spear_knockback() : SpellScriptLoader("spell_ice_spear_knockback") {}

    class spell_ice_spear_knockback_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ice_spear_knockback_SpellScript);

        void AddKnockingUp()
        {
            GetCaster()->CastSpell(GetHitUnit(), SPELL_KNOCKED_UP, true);   //dunno why it does not work correctly from spell_linked_spell
        }

        void Register()
        {
            AfterHit += SpellHitFn(spell_ice_spear_knockback_SpellScript::AddKnockingUp);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ice_spear_knockback_SpellScript();
    }
};

void AddSC_boss_ahune_midsummer()
{
    new boss_ahune();
    new npc_ahunite_hailstone();
    new npc_ahunite_frostwind();
    new npc_ahunite_coldwave();
    new boss_ahune_frostcore();
    new npc_ahune_ice_spear_bunny();
    //new go_ice_stone();
    new spell_slippery_floor_periodic();
    new spell_ice_spear_knockback();
}

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
#include "pit_of_saron.h"
#include "Vehicle.h"

enum Spells
{
    SPELL_MIGHTY_KICK                           = 69021,
    SPELL_SHADOW_BOLT                           = 69028,
    SPELL_TOXIC_WASTE                           = 69024,
    SPELL_EXPLOSIVE_BARRAGE_KRICK               = 69012,
    SPELL_EXPLOSIVE_BARRAGE_ICK                 = 69263,
    SPELL_POISON_NOVA                           = 68989,
    SPELL_PURSUIT                               = 68987,

    SPELL_EXPLOSIVE_BARRAGE_SUMMON              = 69015,
    SPELL_EXPLODING_ORB                         = 69017,
    SPELL_AUTO_GROW                             = 69020,
    SPELL_HASTY_GROW                            = 44851,
    SPELL_EXPLOSIVE_BARRAGE_DAMAGE              = 69019,

    SPELL_STRANGULATING                         = 69413,
    SPELL_SUICIDE                               = 7,
    SPELL_KRICK_KILL_CREDIT                     = 71308,
    SPELL_NECROMANTIC_POWER                     = 69753,
};

enum Yells
{
    SAY_KRICK_AGGRO                             = -1658010,
    SAY_KRICK_SLAY_1                            = -1658011,
    SAY_KRICK_SLAY_2                            = -1658012,
    SAY_KRICK_BARRAGE_1                         = -1658013,
    SAY_KRICK_BARRAGE_2                         = -1658014,
    SAY_KRICK_POISON_NOVA                       = -1658015,
    SAY_KRICK_CHASE_1                           = -1658016,
    SAY_KRICK_CHASE_2                           = -1658017,
    SAY_KRICK_CHASE_3                           = -1658018,

    SAY_ICK_POISON_NOVA                         = -1658020,
    SAY_ICK_CHASE_1                             = -1658021,

    SAY_KRICK_OUTRO_1                           = -1658030,
    SAY_JAYNA_OUTRO_2                           = -1658031,
    SAY_SYLVANAS_OUTRO_2                        = -1658032,
    SAY_KRICK_OUTRO_3                           = -1658033,
    SAY_JAYNA_OUTRO_4                           = -1658034,
    SAY_SYLVANAS_OUTRO_4                        = -1658035,
    SAY_KRICK_OUTRO_5                           = -1658036,
    SAY_TYRANNUS_OUTRO_7                        = -1658037,
    SAY_KRICK_OUTRO_8                           = -1658038,
    SAY_TYRANNUS_OUTRO_9                        = -1658039,
    SAY_JAYNA_OUTRO_10                          = -1658040,
    SAY_SYLVANAS_OUTRO_10                       = -1658041,
};

enum Events
{
    EVENT_MIGHTY_KICK           = 1,
    EVENT_SHADOW_BOLT           = 2,
    EVENT_TOXIC_WASTE           = 3,
    EVENT_SPECIAL               = 4,
    EVENT_PURSUIT               = 5,
    EVENT_POISON_NOVA           = 6,
    EVENT_EXPLOSIVE_BARRAGE     = 7,

    EVENT_OUTRO_1               = 8,
    EVENT_OUTRO_2               = 9,
    EVENT_OUTRO_3               = 10,
    EVENT_OUTRO_4               = 11,
    EVENT_OUTRO_5               = 12,
    EVENT_OUTRO_6               = 13,
    EVENT_OUTRO_7               = 14,
    EVENT_OUTRO_8               = 15,
    EVENT_OUTRO_9               = 16,
    EVENT_OUTRO_10              = 17,
    EVENT_OUTRO_11              = 18,
    EVENT_OUTRO_12              = 19,
    EVENT_OUTRO_13              = 20,
    EVENT_OUTRO_END             = 21,
};

enum KrickPhase
{
    PHASE_COMBAT    = 1,
    PHASE_OUTRO     = 2,
};

enum Actions
{
    ACTION_OUTRO                               = 1,
};

enum Points
{
    POINT_KRICK_INTRO       = 364770,
    POINT_KRICK_DEATH       = 364771,
};

static const Position outroPos[8] =
{
    {828.9342f, 118.6247f, 509.5190f, 0.0000000f},
    {841.0100f, 196.2450f, 573.9640f, 0.2046099f},
    {777.2274f, 119.5521f, 510.0363f, 6.0562930f},
    {823.3984f, 114.4907f, 509.4899f, 0.0000000f},
    {835.5887f, 139.4345f, 530.9526f, 0.0000000f},
    {828.9342f, 118.6247f, 514.5190f, 0.0000000f},
    {828.9342f, 118.6247f, 509.4958f, 0.0000000f},
    {914.4820f, 143.1602f, 633.3624f, 0.0000000f},
};

class boss_ick : public CreatureScript
{
public:
    boss_ick() : CreatureScript("boss_ick") { }

    struct boss_ickAI : public BossAI
    {
        boss_ickAI(Creature* creature) : BossAI(creature, DATA_ICK), _vehicle(creature->GetVehicleKit())
        {
            ASSERT(_vehicle);
        }

        void InitializeAI()
        {
            if(!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != GetScriptId(PoSScriptName))
                me->IsAIEnabled = false;
            else if(!me->isDead())
                Reset();
        }

        void Reset()
        {
            events.Reset();
                instance->SetBossState(DATA_ICK, NOT_STARTED);
        }

        Creature* GetKrick()
        {
                return ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_KRICK));
        }

        void EnterCombat(Unit* /*pWho*/)
        {
                if(Creature* krick = GetKrick())
                    DoScriptText(SAY_KRICK_AGGRO, krick);

                events.ScheduleEvent(EVENT_MIGHTY_KICK, 20000);
                events.ScheduleEvent(EVENT_TOXIC_WASTE, 5000);
                events.ScheduleEvent(EVENT_SHADOW_BOLT, 10000);
                events.ScheduleEvent(EVENT_SPECIAL, urand(30000, 35000));

                instance->SetBossState(DATA_ICK, IN_PROGRESS);
        }

        void EnterEvadeMode()
        {
            me->GetMotionMaster()->Clear();
            ScriptedAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(Creature* krick = GetKrick())
            {
                _vehicle->RemoveAllPassengers();
                if(krick->AI())
                    krick->AI()->DoAction(ACTION_OUTRO);
            }

            instance->SetBossState(DATA_ICK, DONE);
        }

        void SetTempThreat(float threat)
        {
            _tempThreat = threat;
        }

        void _ResetThreat(Unit* target)
        {
            DoModifyThreatPercent(target, -100);
            me->AddThreat(target, _tempThreat);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!me->isInCombat())
                return;

            if(!me->getVictim() && me->getThreatManager().isThreatListEmpty())
            {
                EnterEvadeMode();
                return;
            }

            events.Update(diff);

            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                case EVENT_TOXIC_WASTE:
                            if(Creature* krick = GetKrick())
                                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                    krick->CastSpell(target, SPELL_TOXIC_WASTE, false);
                            events.ScheduleEvent(EVENT_TOXIC_WASTE, urand(7000, 10000));
                            break;
                case EVENT_SHADOW_BOLT:
                            if(Creature* krick = GetKrick())
                                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                                    krick->CastSpell(target, SPELL_SHADOW_BOLT, false);
                    events.ScheduleEvent(EVENT_SHADOW_BOLT, 15000);
                    return;
                        case EVENT_MIGHTY_KICK:
                            DoCastVictim(SPELL_MIGHTY_KICK);
                            events.ScheduleEvent(EVENT_MIGHTY_KICK, 25000);
                            return;
                        case EVENT_SPECIAL:
                            events.ScheduleEvent(RAND(EVENT_EXPLOSIVE_BARRAGE, EVENT_POISON_NOVA, EVENT_PURSUIT), 1000);
                            events.ScheduleEvent(EVENT_SPECIAL, urand(23000, 28000));
                            break;
                case EVENT_EXPLOSIVE_BARRAGE:
                            if(Creature* krick = GetKrick())
                    {
                                DoScriptText(SAY_KRICK_BARRAGE_1, krick);
                                DoScriptText(SAY_KRICK_BARRAGE_2, krick);
                                krick->CastSpell(krick, SPELL_EXPLOSIVE_BARRAGE_KRICK, true);
                                DoCast(me, SPELL_EXPLOSIVE_BARRAGE_ICK);
                            }
                            events.DelayEvents(20000);
                    break;
                        case EVENT_POISON_NOVA:
                            if(Creature* krick = GetKrick())
                                DoScriptText(SAY_KRICK_POISON_NOVA, krick);

                            DoScriptText(SAY_ICK_POISON_NOVA, me);
                            DoCast(me, SPELL_POISON_NOVA);
                            break;
                        case EVENT_PURSUIT:
                            if(Creature* krick = GetKrick())
                                DoScriptText(RAND(SAY_KRICK_CHASE_1, SAY_KRICK_CHASE_2, SAY_KRICK_CHASE_3), krick);
                            DoCast(me, SPELL_PURSUIT);
                            break;
                        default:
                            break;
                    }
            }

            DoMeleeAttackIfReady();
        }

    private:
        Vehicle* _vehicle;
        float _tempThreat;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_ickAI(pCreature);
    }
};

class boss_krick : public CreatureScript
{
public:
boss_krick() : CreatureScript("boss_krick") { }

    struct boss_krickAI : public ScriptedAI
    {
        boss_krickAI(Creature* pCreature): ScriptedAI(pCreature), _instanceScript(pCreature->GetInstanceScript()), _summons(pCreature) { }

        void InitializeAI()
        {
            if(!_instanceScript || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != GetScriptId(PoSScriptName))
                me->IsAIEnabled = false;
            else if(!me->isDead())
                Reset();
        }

        void Reset()
        {
            _events.Reset();
            _phase = PHASE_COMBAT;
            _outroNpcGUID = 0;
            _tyrannusGUID = 0;

            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        Creature* GetIck()
        {
                return ObjectAccessor::GetCreature(*me, _instanceScript->GetData64(DATA_ICK));
        }

        void KilledUnit(Unit* victim)
        {
            if(victim == me)
                return;

            DoScriptText(RAND(SAY_KRICK_SLAY_1,SAY_KRICK_SLAY_2), me);
        }

        void JustSummoned(Creature* summon)
        {
            _summons.Summon(summon);
            if(summon->GetEntry() == NPC_EXPLODING_ORB)
            {
                summon->CastSpell(summon, SPELL_EXPLODING_ORB, true);
                summon->CastSpell(summon, SPELL_AUTO_GROW, true);
            }
        }

            void DoAction(const int32 actionId)
        {
            if(actionId == ACTION_OUTRO)
            {
                Creature* tyrannusPtr = ObjectAccessor::GetCreature(*me, _instanceScript->GetData64(DATA_TYRANNUS_EVENT));
                if(tyrannusPtr)
                    tyrannusPtr->NearTeleportTo(outroPos[1].GetPositionX(), outroPos[1].GetPositionY(), outroPos[1].GetPositionZ(), outroPos[1].GetOrientation());
                else
                    tyrannusPtr = me->SummonCreature(NPC_TYRANNUS_EVENTS, outroPos[1], TEMPSUMMON_MANUAL_DESPAWN);
                tyrannusPtr->SetFlying(true);
                me->GetMotionMaster()->MovePoint(POINT_KRICK_INTRO, outroPos[0].GetPositionX(), outroPos[0].GetPositionY(), outroPos[0].GetPositionZ());
                tyrannusPtr->SetFacingToObject(me);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if(type != POINT_MOTION_TYPE || id != POINT_KRICK_INTRO)
                return;

            DoScriptText(SAY_KRICK_OUTRO_1, me);
            _phase = PHASE_OUTRO;
            _events.Reset();
            _events.ScheduleEvent(EVENT_OUTRO_1, 1000);
        }

        void UpdateAI(const uint32 diff)
        {
            if(_phase != PHASE_OUTRO)
                return;

            _events.Update(diff);

            while(uint32 eventId = _events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_OUTRO_1:
                    {
                        if(Creature* temp = me->GetCreature(*me, _instanceScript->GetData64(DATA_JAINA_SYLVANAS_1)))
                            temp->DespawnOrUnsummon();

                        Creature* jainaOrSylvanas = NULL;
                        if(_instanceScript->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                            jainaOrSylvanas = me->SummonCreature(NPC_JAINA_PART1, outroPos[2], TEMPSUMMON_MANUAL_DESPAWN);
                        else
                            jainaOrSylvanas = me->SummonCreature(NPC_SYLVANAS_PART1, outroPos[2], TEMPSUMMON_MANUAL_DESPAWN);

                        if(jainaOrSylvanas)
                        {
                            jainaOrSylvanas->GetMotionMaster()->MovePoint(0, outroPos[3]);
                            _outroNpcGUID = jainaOrSylvanas->GetGUID();
                        }
                        _events.ScheduleEvent(EVENT_OUTRO_2, 6000);
                        break;
                    }
                    case EVENT_OUTRO_2:
                        if(Creature* jainaOrSylvanas = ObjectAccessor::GetCreature(*me, _outroNpcGUID))
                        {
                            jainaOrSylvanas->SetFacingToObject(me);
                            me->SetFacingToObject(jainaOrSylvanas);
                            if(_instanceScript->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                                DoScriptText(SAY_JAYNA_OUTRO_2, jainaOrSylvanas);
                            else
                                DoScriptText(SAY_SYLVANAS_OUTRO_2, jainaOrSylvanas);
                        }
                        _events.ScheduleEvent(EVENT_OUTRO_3, 5000);
                    break;
                    case EVENT_OUTRO_3:
                        DoScriptText(SAY_KRICK_OUTRO_3, me);
                        _events.ScheduleEvent(EVENT_OUTRO_4, 18000);
                        break;
                    case EVENT_OUTRO_4:
                        if(Creature* jainaOrSylvanas = ObjectAccessor::GetCreature(*me, _outroNpcGUID))
                        {
                            if(_instanceScript->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                                DoScriptText(SAY_JAYNA_OUTRO_4, jainaOrSylvanas);
                            else
                                DoScriptText(SAY_SYLVANAS_OUTRO_4, jainaOrSylvanas);
                        }
                        _events.ScheduleEvent(EVENT_OUTRO_5, 5000);
                        break;
                    case EVENT_OUTRO_5:
                        DoScriptText(SAY_KRICK_OUTRO_5, me);
                        _events.ScheduleEvent(EVENT_OUTRO_6, 1000);
                    break;
                    case EVENT_OUTRO_6:
                        if(Creature* tyrannus = me->GetCreature(*me, _instanceScript->GetData64(DATA_TYRANNUS_EVENT)))
                        {
                            tyrannus->SetSpeed(MOVE_FLIGHT, 3.5f, true);
                            tyrannus->GetMotionMaster()->MovePoint(1, outroPos[4]);
                            _tyrannusGUID = tyrannus->GetGUID();
                        }
                        _events.ScheduleEvent(EVENT_OUTRO_7, 5000);
                    break;
                    case EVENT_OUTRO_7:
                        if(Creature* tyrannus = ObjectAccessor::GetCreature(*me, _tyrannusGUID))
                            DoScriptText(SAY_TYRANNUS_OUTRO_7, tyrannus);
                        _events.ScheduleEvent(EVENT_OUTRO_8, 5000);
                    break;
                    case EVENT_OUTRO_8:
                        me->AddUnitMovementFlag(MOVEMENTFLAG_FLYING);
                        me->GetMotionMaster()->MovePoint(0, outroPos[5]);
                        DoCast(me, SPELL_STRANGULATING);
                        _events.ScheduleEvent(EVENT_OUTRO_9, 2000);
                        break;
                    case EVENT_OUTRO_9:
                        DoScriptText(SAY_KRICK_OUTRO_8, me);
                        if(Creature* tyrannus = ObjectAccessor::GetCreature(*me, _tyrannusGUID))
                            tyrannus->CastSpell(me, SPELL_NECROMANTIC_POWER, true);
                        _events.ScheduleEvent(EVENT_OUTRO_10, 1000);
                        break;
                    case EVENT_OUTRO_10:
                        me->RemoveUnitMovementFlag(MOVEMENTFLAG_FLYING);
                        me->AddUnitMovementFlag(MOVEMENTFLAG_FALLING);
                        me->GetMotionMaster()->MovePoint(0, outroPos[6]);
                        _events.ScheduleEvent(EVENT_OUTRO_11, 2000);
                        break;
                    case EVENT_OUTRO_11:
                        DoCast(me, SPELL_KRICK_KILL_CREDIT);
                        me->SetStandState(UNIT_STAND_STATE_DEAD);
                        me->SetHealth(0);
                        _events.ScheduleEvent(EVENT_OUTRO_12, 3000);
                        break;
                    case EVENT_OUTRO_12:
                        if(Creature* tyrannus = ObjectAccessor::GetCreature(*me, _tyrannusGUID))
                            DoScriptText(SAY_TYRANNUS_OUTRO_9, tyrannus);
                        _events.ScheduleEvent(EVENT_OUTRO_13, 2000);
                        break;
                    case EVENT_OUTRO_13:
                        if(Creature* jainaOrSylvanas = ObjectAccessor::GetCreature(*me, _outroNpcGUID))
                        {
                            if(_instanceScript->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                                DoScriptText(SAY_JAYNA_OUTRO_10, jainaOrSylvanas);
                            else
                                DoScriptText(SAY_SYLVANAS_OUTRO_10, jainaOrSylvanas);
                        }
                        _events.ScheduleEvent(EVENT_OUTRO_END, 3000);
                        if(Creature* tyrannus = ObjectAccessor::GetCreature(*me, _tyrannusGUID))
                            tyrannus->GetMotionMaster()->MovePoint(0, outroPos[7]);
                        break;
                    case EVENT_OUTRO_END:
                        if(Creature* tyrannus = ObjectAccessor::GetCreature(*me, _tyrannusGUID))
                            tyrannus->DespawnOrUnsummon();

                        me->DisappearAndDie();
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        InstanceScript* _instanceScript;
        SummonList _summons;
        EventMap _events;

        KrickPhase _phase;
        uint64 _outroNpcGUID;
        uint64 _tyrannusGUID;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_krickAI(pCreature);
    }
};

class spell_krick_explosive_barrage : public SpellScriptLoader
{
public:
    spell_krick_explosive_barrage() : SpellScriptLoader("spell_krick_explosive_barrage") { }

    class spell_krick_explosive_barrage_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_krick_explosive_barrage_AuraScript);

        void HandlePeriodicTick(AuraEffect const* /*aurEff*/)
        {
            PreventDefaultAction();
            if(Unit* caster = GetCaster())
                if(caster->GetTypeId() == TYPEID_UNIT)
                {
                    Map::PlayerList const &players = caster->GetMap()->GetPlayers();
                    for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        if(Player* player = itr->getSource())
                            if(player->IsWithinDist(caster, 60.0f))
                                caster->CastSpell(player, SPELL_EXPLOSIVE_BARRAGE_SUMMON, true);
                }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_krick_explosive_barrage_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_krick_explosive_barrage_AuraScript();
    }
};

class spell_ick_explosive_barrage : public SpellScriptLoader
{
public:
    spell_ick_explosive_barrage() : SpellScriptLoader("spell_ick_explosive_barrage") { }

    class spell_ick_explosive_barrage_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ick_explosive_barrage_AuraScript);

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if(Unit* caster = GetCaster())
                if(caster->GetTypeId() == TYPEID_UNIT)
                    caster->GetMotionMaster()->MoveIdle();
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if(Unit* caster = GetCaster())
                if(caster->GetTypeId() == TYPEID_UNIT)
                {
                    caster->GetMotionMaster()->Clear();
                    caster->GetMotionMaster()->MoveChase(caster->getVictim());
                }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_ick_explosive_barrage_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_ick_explosive_barrage_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_ick_explosive_barrage_AuraScript();
    }
};

class spell_exploding_orb_hasty_grow : public SpellScriptLoader
{
public:
    spell_exploding_orb_hasty_grow() : SpellScriptLoader("spell_exploding_orb_hasty_grow") { }

    class spell_exploding_orb_hasty_grow_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_exploding_orb_hasty_grow_AuraScript);

        void OnStackChange(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if(GetStackAmount() == 15)
            {
                Unit* target = GetTarget();
                PreventDefaultAction();
                target->CastSpell(target, SPELL_EXPLOSIVE_BARRAGE_DAMAGE, false);
                target->RemoveAurasDueToSpell(SPELL_HASTY_GROW);
                target->RemoveAurasDueToSpell(SPELL_AUTO_GROW);
                target->RemoveAurasDueToSpell(SPELL_EXPLODING_ORB);
                if(Creature* creature = target->ToCreature())
                    creature->DespawnOrUnsummon();
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_exploding_orb_hasty_grow_AuraScript::OnStackChange, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAPPLY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_exploding_orb_hasty_grow_AuraScript();
    }
};

class spell_krick_pursuit : public SpellScriptLoader
{
public:
    spell_krick_pursuit() : SpellScriptLoader("spell_krick_pursuit") { }

    class spell_krick_pursuit_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_krick_pursuit_SpellScript);

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if(GetCaster()->GetTypeId() != TYPEID_UNIT)
                return;

            Unit* caster = GetCaster();
            CreatureAI* ickAI = caster->ToCreature()->AI();
            if(Unit* target = ickAI->SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, false))
            {
                DoScriptText(SAY_ICK_CHASE_1,target);
                caster->AddAura(GetSpellInfo()->Id, target);
                CAST_AI(boss_ick::boss_ickAI, ickAI)->SetTempThreat(caster->getThreatManager().getThreat(target));
                caster->AddThreat(target, float(GetEffectValue()));
                target->AddThreat(caster, float(GetEffectValue()));
            }
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_krick_pursuit_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    class spell_krick_pursuit_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_krick_pursuit_AuraScript);

        void HandleExtraEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if(Unit* caster = GetCaster())
                if(Creature* creCaster = caster->ToCreature())
                    CAST_AI(boss_ick::boss_ickAI, creCaster->AI())->_ResetThreat(GetTarget());
        }

        void Register()
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_krick_pursuit_AuraScript::HandleExtraEffect, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_krick_pursuit_SpellScript();
    }

    AuraScript* GetAuraScript() const
    {
        return new spell_krick_pursuit_AuraScript();
    }
};

class spell_krick_pursuit_confusion : public SpellScriptLoader
{
public:
    spell_krick_pursuit_confusion() : SpellScriptLoader("spell_krick_pursuit_confusion") { }

    class spell_krick_pursuit_confusion_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_krick_pursuit_confusion_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            GetTarget()->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
            GetTarget()->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_krick_pursuit_confusion_AuraScript::OnApply, EFFECT_2, SPELL_AURA_LINKED, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_krick_pursuit_confusion_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_LINKED, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_krick_pursuit_confusion_AuraScript();
    }
};

void AddSC_boss_ick()
{
    new boss_ick;
    new boss_krick;
    new spell_krick_explosive_barrage;
    new spell_ick_explosive_barrage;
    new spell_exploding_orb_hasty_grow;
    new spell_krick_pursuit;
    new spell_krick_pursuit_confusion;
}
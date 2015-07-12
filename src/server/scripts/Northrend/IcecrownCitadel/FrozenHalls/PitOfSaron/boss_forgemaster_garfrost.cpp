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

enum Yells
{
    SAY_AGGRO                                   = -1658001,
    SAY_SLAY_1                                  = -1658002,
    SAY_SLAY_2                                  = -1658003,
    SAY_DEATH                                   = -1658004,
    SAY_PHASE2                                  = -1658005,
    SAY_PHASE3                                  = -1658006,

    SAY_TYRANNUS_DEATH                          = -1659007,
};

enum Spells
{
    SPELL_PERMAFROST                            = 70326,
    SPELL_THROW_SARONITE                        = 68788,
    SPELL_THUNDERING_STOMP                      = 68771,
    SPELL_CHILLING_WAVE                         = 68778,
    SPELL_DEEP_FREEZE                           = 70381,
    SPELL_FORGE_MACE                            = 68785,
    SPELL_FORGE_BLADE                           = 68774,
};

#define SPELL_PERMAFROST_HELPER RAID_MODE<uint32>(68786, 70336)
#define SPELL_FORGE_BLADE_HELPER RAID_MODE<uint32>(68774, 70334)

enum Events
{
    EVENT_THROW_SARONITE    = 1,
    EVENT_CHILLING_WAVE     = 2,
    EVENT_DEEP_FREEZE       = 3,
    EVENT_JUMP              = 4,
    EVENT_FORGING           = 5,
    EVENT_RESUME_ATTACK     = 6,
};

enum Phases
{
    PHASE_ONE           = 1,
    PHASE_TWO           = 2,
    PHASE_THREE         = 3,

    PHASE_ONE_MASK      = 1 << PHASE_ONE,
    PHASE_TWO_MASK      = 1 << PHASE_TWO,
    PHASE_THREE_MASK    = 1 << PHASE_THREE,
};

enum MiscData
{
    EQUIP_ID_SWORD                              = 49345,
    EQUIP_ID_MACE                               = 49344,
    ACHIEV_DOESNT_GO_TO_ELEVEN                  = 4524,
    POINT_FORGE                                 = 0,
};

Position const northForgePos = {722.5643f, -234.1615f, 527.182f, 2.16421f};
Position const southForgePos = {639.257f, -210.1198f, 529.015f, 0.523599f};

class boss_garfrost : public CreatureScript
{
public:
    boss_garfrost() : CreatureScript("boss_garfrost") { }

    struct boss_garfrostAI : public BossAI
    {
        boss_garfrostAI(Creature* pCreature) : BossAI(pCreature, DATA_GARFROST) { }

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
                events.SetPhase(PHASE_ONE);
                SetEquipmentSlots(true);
                _permafrostStack = 0;

                instance->SetBossState(DATA_GARFROST, NOT_STARTED);
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);
            DoCast(me, SPELL_PERMAFROST);
                events.ScheduleEvent(EVENT_THROW_SARONITE, 7000);

                instance->SetBossState(DATA_GARFROST, IN_PROGRESS);
        }

        void KilledUnit(Unit* victim)
        {
                if(victim->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2), me);
        }

        void JustDied(Unit* /*pKiller*/)
        {
            DoScriptText(SAY_DEATH, me);
            if(Creature* tyrannus = me->GetCreature(*me, instance->GetData64(DATA_TYRANNUS)))
                DoScriptText(SAY_TYRANNUS_DEATH, tyrannus);

            instance->SetBossState(DATA_GARFROST, DONE);
        }

        void DamageTaken(Unit* /*pAttacker*/, uint32& /*uiDamage*/)
        {
            if(events.GetPhaseMask() & PHASE_ONE_MASK && !HealthAbovePct(66))
            {
                events.SetPhase(PHASE_TWO);
                events.DelayEvents(8000);
                DoCast(me, SPELL_THUNDERING_STOMP);
                events.ScheduleEvent(EVENT_JUMP, 1500);
                return;
            }

            if(events.GetPhaseMask() & PHASE_TWO_MASK && !HealthAbovePct(33))
            {
                events.SetPhase(PHASE_THREE);
                events.DelayEvents(8000);
                DoCast(me, SPELL_THUNDERING_STOMP);
                events.ScheduleEvent(EVENT_JUMP, 1500);
                return;
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if(type != POINT_MOTION_TYPE || id != POINT_FORGE)
                return;

            if(events.GetPhaseMask() & PHASE_TWO_MASK)
                DoCast(me, SPELL_FORGE_BLADE);
            if(events.GetPhaseMask() & PHASE_THREE_MASK)
                {
                    me->RemoveAurasDueToSpell(SPELL_FORGE_BLADE_HELPER);
                DoCast(me, SPELL_FORGE_MACE);
                }
            events.ScheduleEvent(EVENT_RESUME_ATTACK, 5000);
        }

        void SpellHitTarget(Unit* target, const SpellEntry* spell)
        {
            if(spell->Id == SPELL_PERMAFROST_HELPER)
            {
                if(Aura *aura = target->GetAura(SPELL_PERMAFROST_HELPER))
                    _permafrostStack = std::max<uint32>(_permafrostStack, aura->GetStackAmount());
            }
            else if(spell->Id == SPELL_FORGE_BLADE)
                SetEquipmentSlots(false, EQUIP_ID_SWORD);
            else if(spell->Id == SPELL_FORGE_MACE)
                SetEquipmentSlots(false, EQUIP_ID_MACE);
        }

        uint32 GetData(uint32 /*type*/)
        {
            return _permafrostStack;
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            events.Update(diff);

            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_THROW_SARONITE:
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(target, SPELL_THROW_SARONITE);
                        events.ScheduleEvent(EVENT_THROW_SARONITE, urand(12500, 20000));
                        break;
                    case EVENT_CHILLING_WAVE:
                        DoCast(me, SPELL_CHILLING_WAVE);
                        events.ScheduleEvent(EVENT_CHILLING_WAVE, 40000, 0, PHASE_TWO);
                        break;
                    case EVENT_DEEP_FREEZE:
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(target, SPELL_DEEP_FREEZE);
                        events.ScheduleEvent(EVENT_DEEP_FREEZE, 35000, 0, PHASE_THREE);
                        break;
                    case EVENT_JUMP:
                        me->AttackStop();
                        if(events.GetPhaseMask() & PHASE_TWO_MASK)
                            me->GetMotionMaster()->MoveJump(northForgePos.GetPositionX(), northForgePos.GetPositionY(), northForgePos.GetPositionZ(), 25.0f, 15.0f);
                        else if(events.GetPhaseMask() & PHASE_THREE_MASK)
                            me->GetMotionMaster()->MoveJump(southForgePos.GetPositionX(), southForgePos.GetPositionY(), southForgePos.GetPositionZ(), 25.0f, 15.0f);
                        break;
                    case EVENT_RESUME_ATTACK:
                        if(events.GetPhaseMask() & PHASE_TWO_MASK)
                            events.ScheduleEvent(EVENT_CHILLING_WAVE, 5000, 0, PHASE_TWO);
                        else if(events.GetPhaseMask() & PHASE_THREE_MASK)
                            events.ScheduleEvent(EVENT_DEEP_FREEZE, 10000, 0, PHASE_THREE);
                        AttackStart(me->getVictim());
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        uint32 _permafrostStack;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_garfrostAI(pCreature);
    }
};

class spell_garfrost_permafrost : public SpellScriptLoader
{
public:
    spell_garfrost_permafrost() : SpellScriptLoader("spell_garfrost_permafrost") { }

    class spell_garfrost_permafrost_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_garfrost_permafrost_SpellScript);

        bool Load()
        {
            prevented = false;
            return true;
        }

        void PreventHitByLoS()
        {
            if(Unit* target = GetHitUnit())
            {
                Unit* caster = GetCaster();

                std::list<GameObject*> blockList;
                caster->GetGameObjectListWithEntryInGrid(blockList, GO_SARONITE_ROCK, 100.0f);
                if(!blockList.empty())
                {
                    for(std::list<GameObject*>::const_iterator itr = blockList.begin(); itr != blockList.end(); ++itr)
                    {
                        if((*itr)->isVisibleForInState(target))
                        {
                            if((*itr)->IsInBetween(caster, target, 4.0f))
                            {
                                prevented = true;
                                target->ApplySpellImmune(GetSpellInfo()->Id, IMMUNITY_ID, GetSpellInfo()->Id, true);
                                PreventHitDefaultEffect(EFFECT_0);
                                PreventHitDefaultEffect(EFFECT_1);
                                PreventHitDefaultEffect(EFFECT_2);
                                PreventHitDamage();
                                break;
                            }
                        }
                    }
                }
            }
        }

        void RestoreImmunity()
        {
            if(Unit* target = GetHitUnit())
            {
                target->ApplySpellImmune(GetSpellInfo()->Id, IMMUNITY_ID, GetSpellInfo()->Id, false);
                if(prevented)
                    PreventHitAura();
            }
        }

        void Register()
        {
            BeforeHit += SpellHitFn(spell_garfrost_permafrost_SpellScript::PreventHitByLoS);
            AfterHit += SpellHitFn(spell_garfrost_permafrost_SpellScript::RestoreImmunity);
        }

        bool prevented;
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_garfrost_permafrost_SpellScript();
    }
};

class achievement_doesnt_go_to_eleven : public AchievementCriteriaScript
{
public:
    achievement_doesnt_go_to_eleven() : AchievementCriteriaScript("achievement_doesnt_go_to_eleven") { }

    bool OnCheck(Player* /*pSource*/, Unit* target)
    {
        if(target)
            if(Creature* garfrost = target->ToCreature())
                if(garfrost->AI()->GetData(ACHIEV_DOESNT_GO_TO_ELEVEN) <= 10)
                    return true;

        return false;
    }
};

void AddSC_boss_garfrost()
{
    new boss_garfrost;
    new spell_garfrost_permafrost;
    new achievement_doesnt_go_to_eleven;
}
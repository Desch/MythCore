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
#include "SpellAuraEffects.h"

enum PaladinSpells
{
    PALADIN_SPELL_DIVINE_PLEA                   = 54428,
    PALADIN_SPELL_BLESSING_OF_SANCTUARY_BUFF    = 67480,
    PALADIN_SPELL_BLESSING_OF_SANCTUARY_HELPER  = 20912,
    PALADIN_SPELL_HOLY_SHOCK_R1                 = 20473,
    PALADIN_SPELL_HOLY_SHOCK_R1_DAMAGE          = 25912,
    PALADIN_SPELL_HOLY_SHOCK_R1_HEALING         = 25914,
    PALADIN_SPELL_SACRED_SHIELD_EFFECT          = 58597,

    SPELL_BLESSING_OF_LOWER_CITY_DRUID          = 37878,
    SPELL_BLESSING_OF_LOWER_CITY_PALADIN        = 37879,
    SPELL_BLESSING_OF_LOWER_CITY_PRIEST         = 37880,
    SPELL_BLESSING_OF_LOWER_CITY_SHAMAN         = 37881,

    SPELL_RIGHTEOUS_DEFENCE                     = 31789,
    SPELL_RIGHTEOUS_DEFENCE_EFFECT_1            = 31790
};

// 31850 - Ardent Defender
class spell_pal_ardent_defender : public SpellScriptLoader
{
public:
    spell_pal_ardent_defender() : SpellScriptLoader("spell_pal_ardent_defender") { }

    class spell_pal_ardent_defender_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_ardent_defender_AuraScript);

        uint32 absorbPct, healPct;

        enum Spell
        {
            PAL_SPELL_ARDENT_DEFENDER_HEAL = 66235,
        };

        bool Load()
        {
            healPct = SpellMgr::CalculateSpellEffectAmount(GetSpellProto(), EFFECT_1);
            absorbPct = SpellMgr::CalculateSpellEffectAmount(GetSpellProto(), EFFECT_0);
            return GetUnitOwner()->ToPlayer();
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
        {
            // Set absorbtion amount to unlimited
            amount = -1;
        }

        void Absorb(AuraEffect* aurEff, DamageInfo & dmgInfo, uint32 & absorbAmount)
        {
            Unit* pVictim = GetTarget();
            int32 remainingHealth = pVictim->GetHealth() - dmgInfo.GetDamage();
            uint32 allowedHealth = pVictim->CountPctFromMaxHealth(35);
            // If damage kills us
            if(remainingHealth <= 0 && !pVictim->ToPlayer()->HasSpellCooldown(PAL_SPELL_ARDENT_DEFENDER_HEAL))
            {
                // Cast healing spell, completely avoid damage
                absorbAmount = dmgInfo.GetDamage();

                uint32 defenseSkillValue = pVictim->GetDefenseSkillValue();
                // Max heal when defense skill denies critical hits from raid bosses
                // Formula: max defense at level + 140 (raiting from gear)
                uint32 reqDefForMaxHeal  = pVictim->getLevel() * 5 + 140;
                float pctFromDefense = (defenseSkillValue >= reqDefForMaxHeal)
                    ? 1.0f : float(defenseSkillValue) / float(reqDefForMaxHeal);

                int32 healAmount = int32(pVictim->CountPctFromMaxHealth(uint32(healPct * pctFromDefense)));
                pVictim->CastCustomSpell(pVictim, PAL_SPELL_ARDENT_DEFENDER_HEAL, &healAmount, NULL, NULL, true, NULL, aurEff);
                pVictim->ToPlayer()->AddSpellCooldown(PAL_SPELL_ARDENT_DEFENDER_HEAL, 0, time(NULL) + 120);
            }
            else if(remainingHealth < int32(allowedHealth))
            {
                // Reduce damage that brings us under 35% (or full damage if we are already under 35%) by x%
                uint32 damageToReduce = (pVictim->GetHealth() < allowedHealth) ? dmgInfo.GetDamage() : allowedHealth - remainingHealth;
                absorbAmount = CalculatePctN(damageToReduce, absorbPct);
            }
        }

        void Register()
        {
             DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_ardent_defender_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
             OnEffectAbsorb += AuraEffectAbsorbFn(spell_pal_ardent_defender_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_pal_ardent_defender_AuraScript();
    }
};

class spell_pal_blessing_of_faith : public SpellScriptLoader
{
public:
    spell_pal_blessing_of_faith() : SpellScriptLoader("spell_pal_blessing_of_faith") { }

    class spell_pal_blessing_of_faith_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_blessing_of_faith_SpellScript)
        bool Validate(SpellEntry const* /*pSpellEntry*/)
        {
            if(!sSpellStore.LookupEntry(SPELL_BLESSING_OF_LOWER_CITY_DRUID))
                return false;
            if(!sSpellStore.LookupEntry(SPELL_BLESSING_OF_LOWER_CITY_PALADIN))
                return false;
            if(!sSpellStore.LookupEntry(SPELL_BLESSING_OF_LOWER_CITY_PRIEST))
                return false;
            if(!sSpellStore.LookupEntry(SPELL_BLESSING_OF_LOWER_CITY_SHAMAN))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if(Unit* unitTarget = GetHitUnit())
            {
                uint32 spell_id = 0;
                switch(unitTarget->getClass())
                {
                    case CLASS_DRUID:   spell_id = SPELL_BLESSING_OF_LOWER_CITY_DRUID; break;
                    case CLASS_PALADIN: spell_id = SPELL_BLESSING_OF_LOWER_CITY_PALADIN; break;
                    case CLASS_PRIEST:  spell_id = SPELL_BLESSING_OF_LOWER_CITY_PRIEST; break;
                    case CLASS_SHAMAN:  spell_id = SPELL_BLESSING_OF_LOWER_CITY_SHAMAN; break;
                    default: return;                    // ignore for non-healing classes
                }

                GetCaster()->CastSpell(GetCaster(), spell_id, true);
            }
        }

        void Register()
        {
            // add dummy effect spell handler to Blessing of Faith
            OnEffect += SpellEffectFn(spell_pal_blessing_of_faith_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pal_blessing_of_faith_SpellScript();
    }
};

// 20911 Blessing of Sanctuary
// 25899 Greater Blessing of Sanctuary
class spell_pal_blessing_of_sanctuary : public SpellScriptLoader
{
public:
    spell_pal_blessing_of_sanctuary() : SpellScriptLoader("spell_pal_blessing_of_sanctuary") { }

    class spell_pal_blessing_of_sanctuary_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_blessing_of_sanctuary_AuraScript)
        bool Validate(SpellEntry const* /*pSpellEntry*/)
        {
            if(!sSpellStore.LookupEntry(PALADIN_SPELL_BLESSING_OF_SANCTUARY_BUFF))
                return false;
            return true;
        }

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if(Unit* pTarget = GetTarget())
            {
                if(Unit* pCaster = GetCaster())
                {
                    pCaster->CastSpell(pTarget, PALADIN_SPELL_BLESSING_OF_SANCTUARY_BUFF, true);
                    pCaster->CastSpell(pTarget, PALADIN_SPELL_BLESSING_OF_SANCTUARY_HELPER, true);
                }
            }
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* pTarget = GetTarget();
            pTarget->RemoveAura(PALADIN_SPELL_BLESSING_OF_SANCTUARY_BUFF, GetCasterGUID());
            pTarget->RemoveAura(PALADIN_SPELL_BLESSING_OF_SANCTUARY_HELPER);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_pal_blessing_of_sanctuary_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_pal_blessing_of_sanctuary_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_pal_blessing_of_sanctuary_AuraScript();
    }
};

// 63521 Guarded by The Light
class spell_pal_guarded_by_the_light : public SpellScriptLoader
{
public:
    spell_pal_guarded_by_the_light() : SpellScriptLoader("spell_pal_guarded_by_the_light") { }

    class spell_pal_guarded_by_the_light_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_guarded_by_the_light_SpellScript)
        bool Validate(SpellEntry const* /*pSpellEntry*/)
        {
            if(!sSpellStore.LookupEntry(PALADIN_SPELL_DIVINE_PLEA))
                return false;
            return true;
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            // Divine Plea
            if(Aura* aura = GetCaster()->GetAura(PALADIN_SPELL_DIVINE_PLEA))
                aura->RefreshDuration();
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_pal_guarded_by_the_light_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pal_guarded_by_the_light_SpellScript();
    }
};

class spell_pal_holy_shock : public SpellScriptLoader
{
public:
    spell_pal_holy_shock() : SpellScriptLoader("spell_pal_holy_shock") { }

    class spell_pal_holy_shock_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_holy_shock_SpellScript)
        bool Validate(SpellEntry const *spellEntry)
        {
            if(!sSpellStore.LookupEntry(PALADIN_SPELL_HOLY_SHOCK_R1))
                return false;

            // can't use other spell than holy shock due to spell_ranks dependency
            if(sSpellMgr->GetFirstSpellInChain(PALADIN_SPELL_HOLY_SHOCK_R1) != sSpellMgr->GetFirstSpellInChain(spellEntry->Id))
                return false;

            uint8 rank = sSpellMgr->GetSpellRank(spellEntry->Id);
            if(!sSpellMgr->GetSpellWithRank(PALADIN_SPELL_HOLY_SHOCK_R1_DAMAGE, rank, true))
                return false;
            if(!sSpellMgr->GetSpellWithRank(PALADIN_SPELL_HOLY_SHOCK_R1_HEALING, rank, true))
                return false;

            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if(Unit* pTarget = GetHitUnit())
            {
                if(Unit* pCaster = GetCaster())
                {
                    uint8 rank = sSpellMgr->GetSpellRank(GetSpellInfo()->Id);

                    if(pCaster->IsFriendlyTo(pTarget))
                    {
                        Unit* spellTarget = pTarget;
                        if(pTarget->GetTypeId() == TYPEID_PLAYER && pTarget->ToPlayer()->duel != NULL)
                        spellTarget = pCaster;

                        pCaster->CastSpell(spellTarget, sSpellMgr->GetSpellWithRank(PALADIN_SPELL_HOLY_SHOCK_R1_HEALING, rank), true, 0);
                    }
                    else
                        pCaster->CastSpell(pTarget, sSpellMgr->GetSpellWithRank(PALADIN_SPELL_HOLY_SHOCK_R1_DAMAGE, rank), true, 0);
                }
            }
        }

        SpellCastResult CheckCast()
        {
            Player* caster = GetCaster()->ToPlayer();
            if(GetTargetUnit())
                if(Unit* target = GetTargetUnit())
                {
                    if(!caster->IsFriendlyTo(target))
                    {
                        if(!caster->IsHostileTo(target))
                            return SPELL_FAILED_BAD_TARGETS;

                        if(!caster->isInFront(target, 40))
                            return SPELL_FAILED_UNIT_NOT_INFRONT;
                    }
                }
                else
                    return SPELL_FAILED_BAD_TARGETS;
            return SPELL_CAST_OK;
        }
        
        void Register()
        {
            // add dummy effect spell handler to Holy Shock
            OnEffect += SpellEffectFn(spell_pal_holy_shock_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnCheckCast += SpellCheckCastFn(spell_pal_holy_shock_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pal_holy_shock_SpellScript();
    }
};

class spell_pal_judgement_of_command : public SpellScriptLoader
{
public:
    spell_pal_judgement_of_command() : SpellScriptLoader("spell_pal_judgement_of_command") { }

    class spell_pal_judgement_of_command_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_judgement_of_command_SpellScript)
        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if(Unit* unitTarget = GetHitUnit())
                if(SpellEntry const* spell_proto = sSpellStore.LookupEntry(GetEffectValue()))
                    GetCaster()->CastSpell(unitTarget, spell_proto, true, NULL);
        }

        void Register()
        {
            // add dummy effect spell handler to Judgement of Command
            OnEffect += SpellEffectFn(spell_pal_judgement_of_command_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pal_judgement_of_command_SpellScript();
    }
};

class spell_pal_sacred_shield : public SpellScriptLoader
{
public:
    spell_pal_sacred_shield() : SpellScriptLoader("spell_pal_sacred_shield") { }

    class spell_pal_sacred_shield_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_sacred_shield_AuraScript)
        bool Validate(SpellEntry const* /*pSpellEntry*/)
        {
            if(!sSpellStore.LookupEntry(PALADIN_SPELL_SACRED_SHIELD_EFFECT))
                return false;
            return true;
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if(Unit* pCaster = GetCaster())
            {
                if(pCaster->ToPlayer())
                    pCaster->ToPlayer()->AddSpellCooldown(PALADIN_SPELL_SACRED_SHIELD_EFFECT, 0, time(NULL) + 6);
                else if(pCaster->ToCreature())
                    pCaster->ToCreature()->_AddCreatureSpellCooldown(PALADIN_SPELL_SACRED_SHIELD_EFFECT, time(NULL) + 6);
            }
        }

        void Register()
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_pal_sacred_shield_AuraScript::HandleEffectRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_pal_sacred_shield_AuraScript();
    }
};

class spell_pal_righteous_defense : public SpellScriptLoader
{
public:
    spell_pal_righteous_defense() : SpellScriptLoader("spell_pal_righteous_defense") { }

    class spell_pal_righteous_defense_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_righteous_defense_SpellScript)
        bool Validate(SpellEntry const* /*pSpellEntry*/)
        {
            if(!sSpellStore.LookupEntry(SPELL_RIGHTEOUS_DEFENCE))
                return false;
            return true;
        }

        void HandleSpellEffectTriggerSpell(SpellEffIndex /*effIndex*/)
        {
            if(Unit* pCaster = GetCaster())
            {
                if(Unit* pTarget = GetHitUnit())
                    pCaster->CastSpell(pTarget, SPELL_RIGHTEOUS_DEFENCE_EFFECT_1, true);
            }
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_pal_righteous_defense_SpellScript::HandleSpellEffectTriggerSpell, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pal_righteous_defense_SpellScript();
    }
};

void AddSC_paladin_spell_scripts()
{
    new spell_pal_sacred_shield;
    new spell_pal_ardent_defender;
    new spell_pal_blessing_of_faith;
    new spell_pal_blessing_of_sanctuary;
    new spell_pal_guarded_by_the_light;
    new spell_pal_holy_shock;
    new spell_pal_judgement_of_command;
    new spell_pal_righteous_defense;
}
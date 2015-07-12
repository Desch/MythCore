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

enum DeathKnightSpells
{
    DK_SPELL_RUNIC_POWER_ENERGIZE               = 49088,
    DK_SPELL_ANTI_MAGIC_SHELL_TALENT            = 51052,
    DK_SPELL_CORPSE_EXPLOSION_TRIGGERED         = 43999,
    DK_SPELL_GHOUL_EXPLODE                      = 47496,
    DISPLAY_GHOUL_CORPSE                        = 25537,
    DK_SPELL_SCOURGE_STRIKE_TRIGGERED           = 70890,
    DK_SPELL_BLOOD_BOIL_TRIGGERED               = 65658,
    DK_SPELL_WILL_OF_THE_NECROPOLIS_TALENT_R1   = 49189,
    DK_SPELL_WILL_OF_THE_NECROPOLIS_AURA_R1     = 52284,
    DK_SPELL_BLOOD_PRESENCE                     = 48266,
    DK_SPELL_IMPROVED_BLOOD_PRESENCE_TRIGGERED  = 63611,
    DK_SPELL_UNHOLY_PRESENCE                    = 48265,
    DK_SPELL_IMPROVED_UNHOLY_PRESENCE_TRIGGERED = 63622,
    DK_SPELL_RAISE_ALLY_TRIGGERED               = 46619,
    DK_SPELL_UNHOLY_BLIGHT                      = 50536
};

class spell_dk_raise_ally : public SpellScriptLoader
{
public:
    spell_dk_raise_ally() : SpellScriptLoader("spell_dk_raise_ally") { }

    class spell_dk_raise_ally_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_raise_ally_SpellScript);

        bool Validate(SpellEntry const* /*pSpell*/)
        {
            if(!sSpellStore.LookupEntry(DK_SPELL_RAISE_ALLY_TRIGGERED))
                return false;
            return true;
        }

        SpellCastResult CheckTarget()
        {
            Unit* target = GetTargetUnit();
            if(!target || target->GetTypeId() != TYPEID_PLAYER || target->isAlive())
                return SPELL_FAILED_BAD_TARGETS;
            else if(GetCaster()->ToPlayer()->InArena())
                return SPELL_FAILED_NOT_IN_ARENA;

            return SPELL_CAST_OK;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if(Unit* target = GetHitUnit())
                target->CastSpell(target, DK_SPELL_RAISE_ALLY_TRIGGERED, true);
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_dk_raise_ally_SpellScript::CheckTarget);
            OnEffect += SpellEffectFn(spell_dk_raise_ally_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dk_raise_ally_SpellScript();
    }
};

// 50462 - Anti-Magic Shell (on raid member)
class spell_dk_anti_magic_shell_raid : public SpellScriptLoader
{
public:
    spell_dk_anti_magic_shell_raid() : SpellScriptLoader("spell_dk_anti_magic_shell_raid") { }

    class spell_dk_anti_magic_shell_raid_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_anti_magic_shell_raid_AuraScript);

        uint32 absorbPct;

        bool Load()
        {
            absorbPct = SpellMgr::CalculateSpellEffectAmount(GetSpellProto(), EFFECT_0, GetCaster());
            return true;
        }
         void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
        {
            // TODO: this should absorb limited amount of damage, but no info on calculation formula
            amount = -1;
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
        {
             absorbAmount = CalculatePctN(dmgInfo.GetDamage(), absorbPct);
        }

        void Register()
        {
             DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_raid_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
             OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_raid_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_anti_magic_shell_raid_AuraScript();
    }
};

// 48707 - Anti-Magic Shell (on self)
class spell_dk_anti_magic_shell_self : public SpellScriptLoader
{
public:
    spell_dk_anti_magic_shell_self() : SpellScriptLoader("spell_dk_anti_magic_shell_self") { }

    class spell_dk_anti_magic_shell_self_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_anti_magic_shell_self_AuraScript);
         uint32 absorbPct, hpPct;
        bool Load()
        {
            absorbPct = SpellMgr::CalculateSpellEffectAmount(GetSpellProto(), EFFECT_0, GetCaster());
            if(GetCaster()->HasSpell(49224))
                absorbPct += 8;
            if(GetCaster()->HasSpell(49610))
                absorbPct += 16;
            if(GetCaster()->HasSpell(49611))
                absorbPct += 25;
            hpPct = SpellMgr::CalculateSpellEffectAmount(GetSpellProto(), EFFECT_1, GetCaster());
            return true;
        }

        bool Validate(SpellEntry const* /*pSpellEntry*/)
        {
            return sSpellStore.LookupEntry(DK_SPELL_RUNIC_POWER_ENERGIZE);
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
        {
            // Set absorbtion amount to unlimited
            amount = -1;
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
        {
            absorbAmount = std::min(CalculatePctN(dmgInfo.GetDamage(), absorbPct), GetTarget()->CountPctFromMaxHealth(hpPct));
        }

        void Trigger(AuraEffect* aurEff, DamageInfo & /*dmgInfo*/, uint32 & absorbAmount)
        {
            Unit* target = GetTarget();
            // damage absorbed by Anti-Magic Shell energizes the DK with additional runic power.
            // This, if I'm not mistaken, shows that we get back ~20% of the absorbed damage as runic power.
            int32 bp = absorbAmount * 2 / 10;
            target->CastCustomSpell(target, DK_SPELL_RUNIC_POWER_ENERGIZE, &bp, NULL, NULL, true, NULL, aurEff);
        }

        void Register()
        {
             DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_self_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
             OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Absorb, EFFECT_0);
             AfterEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Trigger, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_anti_magic_shell_self_AuraScript();
    }
};

// 50461 - Anti-Magic Zone
class spell_dk_anti_magic_zone : public SpellScriptLoader
{
public:
    spell_dk_anti_magic_zone() : SpellScriptLoader("spell_dk_anti_magic_zone") { }

    class spell_dk_anti_magic_zone_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_anti_magic_zone_AuraScript);

        uint32 absorbPct;

        bool Load()
        {
            absorbPct = SpellMgr::CalculateSpellEffectAmount(GetSpellProto(), EFFECT_0, GetCaster());
            return true;
        }

        bool Validate(SpellEntry const* /*pSpellEntry*/)
        {
            return sSpellStore.LookupEntry(DK_SPELL_ANTI_MAGIC_SHELL_TALENT);
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
        {
            SpellEntry const* talentSpell = sSpellStore.LookupEntry(DK_SPELL_ANTI_MAGIC_SHELL_TALENT);
            amount = SpellMgr::CalculateSpellEffectAmount(talentSpell, EFFECT_0, GetCaster());
            // assume caster is a player here
            if(Unit* pCaster = GetCaster())
                if(Player* pPlayer = pCaster->ToPlayer())
                    amount += int32(2 * pPlayer->GetTotalAttackPowerValue(BASE_ATTACK));
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
        {
             absorbAmount = CalculatePctN(dmgInfo.GetDamage(), absorbPct);
        }

        void Register()
        {
             DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_zone_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
             OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_zone_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_anti_magic_zone_AuraScript();
    }
};

// 49158 Corpse Explosion (51325, 51326, 51327, 51328)
class spell_dk_corpse_explosion : public SpellScriptLoader
{
public:
    spell_dk_corpse_explosion() : SpellScriptLoader("spell_dk_corpse_explosion") { }

    class spell_dk_corpse_explosion_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_corpse_explosion_SpellScript);

        bool Validate(SpellEntry const* /*pSpellEntry*/)
        {
            if(!sSpellStore.LookupEntry(DK_SPELL_CORPSE_EXPLOSION_TRIGGERED))
                return false;
            if(!sSpellStore.LookupEntry(DK_SPELL_GHOUL_EXPLODE))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if(Unit* unitTarget = GetHitUnit())
            {
                int32 bp = 0;
                if(unitTarget->isAlive())  // Living ghoul as a target
                {
                    bp = int32(unitTarget->CountPctFromMaxHealth(25));
                    unitTarget->CastCustomSpell(unitTarget, DK_SPELL_GHOUL_EXPLODE, &bp, NULL, NULL, false);
                }
                else                        // Some corpse
                {
                    bp = GetEffectValue();
                    GetCaster()->CastCustomSpell(unitTarget, SpellMgr::CalculateSpellEffectAmount(GetSpellInfo(), 1), &bp, NULL, NULL, true);
                    // Corpse Explosion (Suicide)
                    unitTarget->CastSpell(unitTarget, DK_SPELL_CORPSE_EXPLOSION_TRIGGERED, true);
                    // Set corpse look
                    unitTarget->SetDisplayId(DISPLAY_GHOUL_CORPSE + urand(0, 3));
                }
            }
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_dk_corpse_explosion_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dk_corpse_explosion_SpellScript();
    }
};

// 47496 - Explode, Ghoul spell for Corpse Explosion
class spell_dk_ghoul_explode : public SpellScriptLoader
{
public:
    spell_dk_ghoul_explode() : SpellScriptLoader("spell_dk_ghoul_explode") { }

    class spell_dk_ghoul_explode_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_ghoul_explode_SpellScript);

        void Suicide(SpellEffIndex /*effIndex*/)
        {
            if(Unit* unitTarget = GetHitUnit())
            {
                // Corpse Explosion (Suicide)
                unitTarget->CastSpell(unitTarget, DK_SPELL_CORPSE_EXPLOSION_TRIGGERED, true);
                // Set corpse look
                unitTarget->SetDisplayId(DISPLAY_GHOUL_CORPSE + urand(0, 3));
            }
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_dk_ghoul_explode_SpellScript::Suicide, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dk_ghoul_explode_SpellScript();
    }
};

class spell_dk_death_gate : public SpellScriptLoader
{
public:
    spell_dk_death_gate() : SpellScriptLoader("spell_dk_death_gate") { }

    class spell_dk_death_gate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_death_gate_SpellScript);

        SpellCastResult CheckClass()
        {
            if(GetCaster()->getClass() != CLASS_DEATH_KNIGHT)
            {
                SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_DEATH_KNIGHT);
                return SPELL_FAILED_CUSTOM_ERROR;
            }

            return SPELL_CAST_OK;
        }

        void HandleScript(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if(!GetHitUnit())
                return;
            GetHitUnit()->CastSpell(GetHitUnit(), GetEffectValue(), false);
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_dk_death_gate_SpellScript::CheckClass);
            OnEffect += SpellEffectFn(spell_dk_death_gate_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dk_death_gate_SpellScript();
    }
};

class spell_dk_death_pact : public SpellScriptLoader
{
public:
    spell_dk_death_pact() : SpellScriptLoader("spell_dk_death_pact") { }

    class spell_dk_death_pact_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_death_pact_SpellScript);

        void FilterTargets(std::list<Unit*>& unitList)
        {
            Unit* unit_to_add = NULL;
            for(std::list<Unit*>::iterator itr = unitList.begin() ; itr != unitList.end(); ++itr)
            {
                if((*itr)->GetTypeId() == TYPEID_UNIT
                    && (*itr)->GetOwnerGUID() == GetCaster()->GetGUID()
                    && (*itr)->ToCreature()->GetCreatureInfo()->type == CREATURE_TYPE_UNDEAD)
                {
                    unit_to_add = (*itr);
                    break;
                }
            }

            unitList.clear();
            if(unit_to_add)
                unitList.push_back(unit_to_add);
            else
            {
                // Pet not found - remove cooldown
                if(Player* modOwner = GetCaster()->GetSpellModOwner())
                    modOwner->RemoveSpellCooldown(GetSpellInfo()->Id, true);
                FinishCast(SPELL_FAILED_NO_PET);
            }
        }

        void Register()
        {
            OnUnitTargetSelect += SpellUnitTargetFn(spell_dk_death_pact_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_AREA_ALLY_DST);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dk_death_pact_SpellScript();
    }
};

// 55090 Scourge Strike (55265, 55270, 55271)
class spell_dk_scourge_strike : public SpellScriptLoader
{
public:
    spell_dk_scourge_strike() : SpellScriptLoader("spell_dk_scourge_strike") { }

    class spell_dk_scourge_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_scourge_strike_SpellScript);

        bool Validate(SpellEntry const* /*pSpellEntry*/)
        {
            if(!sSpellStore.LookupEntry(DK_SPELL_SCOURGE_STRIKE_TRIGGERED))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if(Unit* unitTarget = GetHitUnit())
            {
                int32 bp = CalculatePctN(GetHitDamage(), GetEffectValue() * unitTarget->GetDiseasesByCaster(caster->GetGUID()));
                caster->CastCustomSpell(unitTarget, DK_SPELL_SCOURGE_STRIKE_TRIGGERED, &bp, NULL, NULL, true);
            }
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_dk_scourge_strike_SpellScript::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dk_scourge_strike_SpellScript();
    }
};

// 49145 - Spell Deflection
class spell_dk_spell_deflection : public SpellScriptLoader
{
public:
    spell_dk_spell_deflection() : SpellScriptLoader("spell_dk_spell_deflection") { }

    class spell_dk_spell_deflection_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_spell_deflection_AuraScript);

        uint32 absorbPct;

        bool Load()
        {
            absorbPct = SpellMgr::CalculateSpellEffectAmount(GetSpellProto(), EFFECT_0, GetCaster());
            return true;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
        {
            // Set absorbtion amount to unlimited
            amount = -1;
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
        {
            // You have a chance equal to your Parry chance
            if((dmgInfo.GetDamageType() == SPELL_DIRECT_DAMAGE) && roll_chance_f(GetTarget()->GetUnitParryChance()))
                absorbAmount = CalculatePctN(dmgInfo.GetDamage(), absorbPct);
        }

        void Register()
        {
             DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_spell_deflection_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
             OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_spell_deflection_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_spell_deflection_AuraScript();
    }
};

// 48721 Blood Boil
class spell_dk_blood_boil : public SpellScriptLoader
{
public:
    spell_dk_blood_boil() : SpellScriptLoader("spell_dk_blood_boil") { }

    class spell_dk_blood_boil_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_blood_boil_SpellScript);

        bool Validate(SpellEntry const* /*pSpellEntry*/)
        {
            if(!sSpellStore.LookupEntry(DK_SPELL_BLOOD_BOIL_TRIGGERED))
                return false;
            return true;
        }

        bool Load()
        {
            _executed = false;
            return GetCaster()->GetTypeId() == TYPEID_PLAYER && GetCaster()->getClass() == CLASS_DEATH_KNIGHT;
        }

        void HandleAfterHit()
        {
            if(_executed || !GetHitUnit())
                return;

            _executed = true;
            GetCaster()->CastSpell(GetCaster(), DK_SPELL_BLOOD_BOIL_TRIGGERED, true);
        }

        void Register()
        {
            AfterHit += SpellHitFn(spell_dk_blood_boil_SpellScript::HandleAfterHit);
        }

        bool _executed;
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dk_blood_boil_SpellScript();
    }
};

// 52284 - Will of the Necropolis
class spell_dk_will_of_the_necropolis : public SpellScriptLoader
{
public:
    spell_dk_will_of_the_necropolis() : SpellScriptLoader("spell_dk_will_of_the_necropolis") { }

    class spell_dk_will_of_the_necropolis_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_will_of_the_necropolis_AuraScript);

        bool Validate(SpellEntry const *spellEntry)
        {
            // can't use other spell than will of the necropolis due to spell_ranks dependency
            if(sSpellMgr->GetFirstSpellInChain(DK_SPELL_WILL_OF_THE_NECROPOLIS_AURA_R1) != sSpellMgr->GetFirstSpellInChain(spellEntry->Id))
                return false;

            uint8 rank = sSpellMgr->GetSpellRank(spellEntry->Id);
            if(!sSpellMgr->GetSpellWithRank(DK_SPELL_WILL_OF_THE_NECROPOLIS_TALENT_R1, rank, true))
                return false;

            return true;
        }

        uint32 absorbPct;

        bool Load()
        {
            absorbPct = SpellMgr::CalculateSpellEffectAmount(GetSpellProto(), EFFECT_0, GetCaster());
            return true;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
        {
            // Set absorbtion amount to unlimited
            amount = -1;
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
        {
            // min pct of hp is stored in effect 0 of talent spell
            uint32 rank = sSpellMgr->GetSpellRank(GetSpellProto()->Id);
            SpellEntry const* talentProto = sSpellStore.LookupEntry(sSpellMgr->GetSpellWithRank(DK_SPELL_WILL_OF_THE_NECROPOLIS_TALENT_R1, rank));

            int32 remainingHp = int32(GetTarget()->GetHealth() - dmgInfo.GetDamage());
            int32 minHp = int32(GetTarget()->CountPctFromMaxHealth(SpellMgr::CalculateSpellEffectAmount(talentProto, EFFECT_0, GetCaster())));

            // Damage that would take you below [effect0] health or taken while you are at [effect0]
            if(remainingHp < minHp)
                absorbAmount = CalculatePctN(dmgInfo.GetDamage(), absorbPct);
        }

        void Register()
        {
             DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_will_of_the_necropolis_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
             OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_will_of_the_necropolis_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_will_of_the_necropolis_AuraScript();
    }
};

// 50365, 50371 Improved Blood Presence
class spell_dk_improved_blood_presence : public SpellScriptLoader
{
public:
    spell_dk_improved_blood_presence() : SpellScriptLoader("spell_dk_improved_blood_presence") { }

    class spell_dk_improved_blood_presence_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_improved_blood_presence_AuraScript)
        bool Validate(SpellEntry const* /*pSpellEntry*/)
        {
            if(!sSpellStore.LookupEntry(DK_SPELL_BLOOD_PRESENCE))
                return false;
            if(!sSpellStore.LookupEntry(DK_SPELL_IMPROVED_BLOOD_PRESENCE_TRIGGERED))
                return false;
            return true;
        }

        void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if(!target->HasAura(DK_SPELL_BLOOD_PRESENCE) && !target->HasAura(DK_SPELL_IMPROVED_BLOOD_PRESENCE_TRIGGERED))
            {
                int32 basePoints1 = aurEff->GetAmount();
                target->CastCustomSpell(target, 63611, NULL, &basePoints1, NULL, true, 0, aurEff);
            }
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if(!target->HasAura(DK_SPELL_BLOOD_PRESENCE))
                target->RemoveAura(DK_SPELL_IMPROVED_BLOOD_PRESENCE_TRIGGERED);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dk_improved_blood_presence_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dk_improved_blood_presence_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_improved_blood_presence_AuraScript();
    }
};

// 50391, 50392 Improved Unholy Presence
class spell_dk_improved_unholy_presence : public SpellScriptLoader
{
public:
    spell_dk_improved_unholy_presence() : SpellScriptLoader("spell_dk_improved_unholy_presence") { }

    class spell_dk_improved_unholy_presence_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_improved_unholy_presence_AuraScript)
        bool Validate(SpellEntry const* /*pSpellEntry*/)
        {
            if(!sSpellStore.LookupEntry(DK_SPELL_UNHOLY_PRESENCE))
                return false;
            if(!sSpellStore.LookupEntry(DK_SPELL_IMPROVED_UNHOLY_PRESENCE_TRIGGERED))
                return false;
            return true;
        }

        void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if(target->HasAura(DK_SPELL_UNHOLY_PRESENCE) && !target->HasAura(DK_SPELL_IMPROVED_UNHOLY_PRESENCE_TRIGGERED))
            {
                // Not listed as any effect, only base points set in dbc
                int32 basePoints0 = SpellMgr::CalculateSpellEffectAmount(aurEff->GetSpellProto(), 1);
                target->CastCustomSpell(target, DK_SPELL_IMPROVED_UNHOLY_PRESENCE_TRIGGERED, &basePoints0, &basePoints0, &basePoints0, true, 0, aurEff);
            }
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if(target->HasAura(DK_SPELL_UNHOLY_PRESENCE))
                target->RemoveAura(DK_SPELL_IMPROVED_UNHOLY_PRESENCE_TRIGGERED);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dk_improved_unholy_presence_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dk_improved_unholy_presence_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_improved_unholy_presence_AuraScript();
    }
};
class spell_dk_unholy_blight : public SpellScriptLoader
{
public:
    spell_dk_unholy_blight() : SpellScriptLoader("spell_dk_unholy_blight") { }

    class spell_dk_unholy_blight_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_unholy_blight_AuraScript);

        //Keeps track of how much damage the spell has to do.
        int32 prev_damage;

        bool Validate(SpellEntry const* /*entry*/)
        {
            if(!sSpellStore.LookupEntry(DK_SPELL_UNHOLY_BLIGHT))
                return false;
            return true;
        }

        bool Load()
        {
            prev_damage = 0;
            return true;
        }

        void CalculateAmount(AuraEffect const* aurEff, int32 & amount, bool & /*canBeRecalculated*/)
        {
            amount += prev_damage;
            prev_damage = amount;
            amount = amount / aurEff->GetTotalTicks();
        }

        void OnPeriodic(AuraEffect const* aurEff)
        {
            prev_damage -= prev_damage / aurEff->GetTotalTicks();
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_unholy_blight_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_unholy_blight_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_unholy_blight_AuraScript();
    }
};

enum DeathCoil
{
    SPELL_DEATH_COIL_DAMAGE     = 47632,
    SPELL_DEATH_COIL_HEAL       = 47633,
    SPELL_SIGIL_VENGEFUL_HEART  = 64962
};

class spell_dk_death_coil : public SpellScriptLoader
{
public:
    spell_dk_death_coil() : SpellScriptLoader("spell_dk_death_coil") { }

    class spell_dk_death_coil_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_death_coil_SpellScript);

        bool Validate(SpellEntry const* /*spell*/)
        {
            if(!sSpellStore.LookupEntry(SPELL_DEATH_COIL_DAMAGE) || !sSpellStore.LookupEntry(SPELL_DEATH_COIL_HEAL))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            int32 damage = GetEffectValue();
            Unit* caster = GetCaster();
            if(Unit* target = GetHitUnit())
            {
                if(caster->IsFriendlyTo(target))
                {
                    int32 bp = int32(damage * 1.5f);
                    caster->CastCustomSpell(target, SPELL_DEATH_COIL_HEAL, &bp, NULL, NULL, true);
                }
                else
                {
                    if(AuraEffect const* auraEffect = caster->GetAuraEffect(SPELL_SIGIL_VENGEFUL_HEART, EFFECT_1))
                        damage += auraEffect->GetBaseAmount();
                    caster->CastCustomSpell(target, SPELL_DEATH_COIL_DAMAGE, &damage, NULL, NULL, true);
                }
            }
        }

        SpellCastResult CheckCast()
        {
        Unit* caster = GetCaster();
        if(GetTargetUnit())
            if(Unit* target = GetTargetUnit())
            {
                if(!caster->IsFriendlyTo(target) && !caster->isInFront(target, 40))
                    return SPELL_FAILED_UNIT_NOT_INFRONT;

                if(target->IsFriendlyTo(caster) && target->GetCreatureType() != CREATURE_TYPE_UNDEAD)
                    return SPELL_FAILED_BAD_TARGETS;
            }
            else
                return SPELL_FAILED_BAD_TARGETS;

            return SPELL_CAST_OK;
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_dk_death_coil_SpellScript::CheckCast);
            OnEffect += SpellEffectFn(spell_dk_death_coil_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dk_death_coil_SpellScript();
    }
};

void AddSC_deathknight_spell_scripts()
{
    new spell_dk_raise_ally;
    new spell_dk_anti_magic_shell_raid;
    new spell_dk_anti_magic_shell_self;
    new spell_dk_anti_magic_zone;
    new spell_dk_corpse_explosion;
    new spell_dk_ghoul_explode;
    new spell_dk_death_gate;
    new spell_dk_death_pact;
    new spell_dk_scourge_strike;
    new spell_dk_spell_deflection;
    new spell_dk_blood_boil;
    new spell_dk_will_of_the_necropolis;
    new spell_dk_improved_blood_presence;
    new spell_dk_improved_unholy_presence;
    new spell_dk_unholy_blight;
    new spell_dk_death_coil;
}
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
#include "nexus.h"

enum Spells
{
    SPELL_ICE_NOVA            = 47772,
    H_SPELL_ICE_NOVA          = 56935,
    SPELL_FIREBOMB            = 47773,
    H_SPELL_FIREBOMB          = 56934,
    SPELL_GRAVITY_WELL        = 47756,
    SPELL_TELESTRA_BACK       = 47714,
    SPELL_FIRE_MAGUS_VISUAL   = 47705,
    SPELL_FROST_MAGUS_VISUAL  = 47706,
    SPELL_ARCANE_MAGUS_VISUAL = 47704,
    SPELL_CRITTER             = 47731,
    SPELL_TIMESTOP            = 47736
};

enum Creatures
{
    MOB_FIRE_MAGUS            = 26928,
    MOB_FROST_MAGUS           = 26930,
    MOB_ARCANE_MAGUS          = 26929
};

enum Yells
{
    SAY_AGGRO                 = -1576000,
    SAY_KILL                  = -1576001,
    SAY_DEATH                 = -1576002,
    SAY_MERGE                 = -1576003,
    SAY_SPLIT_1               = -1576004,
    SAY_SPLIT_2               = -1576005
};

enum Achievements
{
    ACHIEV_SPLIT_PERSONALITY  = 2150,
    ACHIEV_TIMER              = 5*IN_MILLISECONDS
};

const Position  CenterOfRoom = {504.80f, 89.07f, -16.12f, 6.27f};

class boss_magus_telestra : public CreatureScript
{
public:
    boss_magus_telestra() : CreatureScript("boss_magus_telestra") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_magus_telestraAI(pCreature);
    }

    struct boss_magus_telestraAI : public ScriptedAI
    {
        boss_magus_telestraAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        uint64 uiFireMagusGUID;
        uint64 uiFrostMagusGUID;
        uint64 uiArcaneMagusGUID;
        bool bFireMagusDead;
        bool bFrostMagusDead;
        bool bArcaneMagusDead;
        bool bIsWaitingToAppear;
        bool bIsAchievementTimerRunning;
        uint32 uiIsWaitingToAppearTimer;
        uint32 uiIceNovaTimer;
        uint32 uiFireBombTimer;
        uint32 uiGravityWellTimer;
        uint32 uiCooldown;
        uint32 uiAchievementTimer;
        uint8 Phase;
        uint8 uiAchievementProgress;

        void Reset()
        {
            Phase = 0;
            uiIceNovaTimer =  7*IN_MILLISECONDS;
            uiFireBombTimer =  0;
            uiGravityWellTimer = 15*IN_MILLISECONDS;
            uiCooldown = 0;
            uiFireMagusGUID = 0;
            uiFrostMagusGUID = 0;
            uiArcaneMagusGUID = 0;
            uiAchievementProgress = 0;
            uiAchievementTimer = 0;
            bIsAchievementTimerRunning = false;
            bIsWaitingToAppear = false;
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetVisible(true);

            if(pInstance)
                pInstance->SetData(DATA_MAGUS_TELESTRA_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);

            if(pInstance)
                pInstance->SetData(DATA_MAGUS_TELESTRA_EVENT, IN_PROGRESS);
        }

        void JustDied(Unit* /*pKiller*/)
        {
            DoScriptText(SAY_DEATH, me);

            if(pInstance)
            {
                if(IsHeroic() && uiAchievementProgress == 2)
                    pInstance->DoCompleteAchievement(ACHIEV_SPLIT_PERSONALITY);
                pInstance->SetData(DATA_MAGUS_TELESTRA_EVENT, DONE);
            }
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(SAY_KILL, me);
        }

        uint64 SplitPersonality(uint32 entry)
        {
            if(Creature* Summoned = me->SummonCreature(entry, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1*IN_MILLISECONDS))
            {
                switch(entry)
                {
                    case MOB_FIRE_MAGUS:
                    {
                        Summoned->CastSpell(Summoned, SPELL_FIRE_MAGUS_VISUAL, false);
                        break;
                    }
                    case MOB_FROST_MAGUS:
                    {
                        Summoned->CastSpell(Summoned, SPELL_FROST_MAGUS_VISUAL, false);
                        break;
                    }
                    case MOB_ARCANE_MAGUS:
                    {
                        Summoned->CastSpell(Summoned, SPELL_ARCANE_MAGUS_VISUAL, false);
                        break;
                    }
                }
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    Summoned->AI()->AttackStart(target);
                return Summoned->GetGUID();
            }
            return 0;
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            if(summon->isAlive())
                return;

            if(summon->GetGUID() == uiFireMagusGUID)
            {
                bFireMagusDead = true;
                bIsAchievementTimerRunning = true;
            }
            else if(summon->GetGUID() == uiFrostMagusGUID)
            {
                bFrostMagusDead = true;
                bIsAchievementTimerRunning = true;
            }
            else if(summon->GetGUID() == uiArcaneMagusGUID)
            {
                bArcaneMagusDead = true;
                bIsAchievementTimerRunning = true;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(bIsWaitingToAppear)
            {
                me->StopMoving();
                me->AttackStop();
                if(uiIsWaitingToAppearTimer <= diff)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    bIsWaitingToAppear = false;
                } else uiIsWaitingToAppearTimer -= diff;
                return;
            }

            if((Phase == 1) ||(Phase == 3))
            {
                if(bIsAchievementTimerRunning)
                    uiAchievementTimer += diff;
                if(bFireMagusDead && bFrostMagusDead && bArcaneMagusDead)
                {
                    if(uiAchievementTimer <= ACHIEV_TIMER)
                        uiAchievementProgress +=1;
                    me->GetMotionMaster()->Clear();
                    me->GetMap()->CreatureRelocation(me, CenterOfRoom.GetPositionX(), CenterOfRoom.GetPositionY(), CenterOfRoom.GetPositionZ(), CenterOfRoom.GetOrientation());
                    DoCast(me, SPELL_TELESTRA_BACK);
                    me->SetVisible(true);
                    Phase++;
                    uiFireMagusGUID = 0;
                    uiFrostMagusGUID = 0;
                    uiArcaneMagusGUID = 0;
                    bIsWaitingToAppear = true;
                    uiIsWaitingToAppearTimer = 4*IN_MILLISECONDS;
                    DoScriptText(SAY_MERGE, me);
                    bIsAchievementTimerRunning = false;
                    uiAchievementTimer = 0;
                } else return;
            }

            if((Phase == 0) && HealthBelowPct(50))
            {
                Phase = 1;
                me->CastStop();
                me->RemoveAllAuras();
                me->SetVisible(false);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                uiFireMagusGUID = SplitPersonality(MOB_FIRE_MAGUS);
                uiFrostMagusGUID = SplitPersonality(MOB_FROST_MAGUS);
                uiArcaneMagusGUID = SplitPersonality(MOB_ARCANE_MAGUS);
                bFireMagusDead = false;
                bFrostMagusDead = false;
                bArcaneMagusDead = false;
                DoScriptText(RAND(SAY_SPLIT_1,SAY_SPLIT_2), me);
                return;
            }

        if(IsHeroic() && (Phase == 2) && HealthBelowPct(15))
            {
                Phase = 3;
                me->CastStop();
                me->RemoveAllAuras();
                me->SetVisible(false);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                uiFireMagusGUID = SplitPersonality(MOB_FIRE_MAGUS);
                uiFrostMagusGUID = SplitPersonality(MOB_FROST_MAGUS);
                uiArcaneMagusGUID = SplitPersonality(MOB_ARCANE_MAGUS);
                bFireMagusDead = false;
                bFrostMagusDead = false;
                bArcaneMagusDead = false;
                DoScriptText(RAND(SAY_SPLIT_1,SAY_SPLIT_2), me);
                return;
            }

            if(uiCooldown)
            {
                if(uiCooldown <= diff)
                    uiCooldown = 0;
                else
                {
                    uiCooldown -= diff;
                    return;
                }
            }

            if(uiIceNovaTimer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    DoCast(target, SPELL_ICE_NOVA, false);
                    uiCooldown = 1500;
                }
                uiIceNovaTimer = 15*IN_MILLISECONDS;
            } else uiIceNovaTimer -= diff;

            if(uiGravityWellTimer <= diff)
            {
                if(Unit* target = me->getVictim())
                {
                    DoCast(target, SPELL_GRAVITY_WELL);
                    uiCooldown = 6*IN_MILLISECONDS;
                }
                uiGravityWellTimer = 15*IN_MILLISECONDS;
            } else uiGravityWellTimer -= diff;

            if(uiFireBombTimer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    DoCast(target, SPELL_FIREBOMB, false);
                    uiCooldown = 2*IN_MILLISECONDS;
                }
                uiFireBombTimer = 2*IN_MILLISECONDS;
            } else uiFireBombTimer -=diff;

            DoMeleeAttackIfReady();
        }
    };
};

class boss_magus_telestra_arcane : public CreatureScript
{
public:
    boss_magus_telestra_arcane() : CreatureScript("boss_magus_telestra_arcane") { }

    struct boss_magus_telestra_arcaneAI : public ScriptedAI
    {
        boss_magus_telestra_arcaneAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        uint32 uiCritterTimer;
        uint32 uiTimeStopTimer;

        void Reset()
        {
            uiCritterTimer = urand(3000, 6000);
            uiTimeStopTimer = urand(7000, 10000);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(uiCritterTimer<=diff)
            {
                DoCast(SPELL_CRITTER);
                    uiCritterTimer = urand(5000, 8000);
            } else uiCritterTimer-=diff;

            if(uiTimeStopTimer<=diff)
            {
                DoCastAOE(SPELL_TIMESTOP);
                uiTimeStopTimer = urand(15000, 18000);
            } else uiTimeStopTimer-=diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_magus_telestra_arcaneAI(pCreature);
    }
};

class spell_nexus_critter_targeting : public SpellScriptLoader
{
    public:
        spell_nexus_critter_targeting() : SpellScriptLoader("spell_nexus_critter_targeting") { }

        class spell_nexus_critter_targeting_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_nexus_critter_targeting_SpellScript);

            void FilterTargetsInitial(std::list<Unit*>& unitList)
            {
                sharedUnitList = unitList;
            }

            void FilterTargetsSubsequent(std::list<Unit*>& unitList)
            {
                unitList = sharedUnitList;
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_nexus_critter_targeting_SpellScript::FilterTargetsInitial, EFFECT_0, TARGET_UNIT_AREA_ENEMY_SRC);
                OnUnitTargetSelect += SpellUnitTargetFn(spell_nexus_critter_targeting_SpellScript::FilterTargetsSubsequent, EFFECT_1, TARGET_UNIT_AREA_ENEMY_SRC);
                OnUnitTargetSelect += SpellUnitTargetFn(spell_nexus_critter_targeting_SpellScript::FilterTargetsSubsequent, EFFECT_2, TARGET_UNIT_AREA_ENEMY_SRC);
            }

            std::list<Unit*> sharedUnitList;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_nexus_critter_targeting_SpellScript();
        }
};

void AddSC_boss_magus_telestra()
{
    new boss_magus_telestra;
    new boss_magus_telestra_arcane;
    new spell_nexus_critter_targeting;
}
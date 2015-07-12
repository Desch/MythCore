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
#include "violet_hold.h"

enum Spells
{
    SPELL_ARCANE_VACUUM                         = 58694,
    SPELL_BLIZZARD                              = 58693,
    H_SPELL_BLIZZARD                            = 59369,
    SPELL_MANA_DESTRUCTION                      = 59374,
    SPELL_TAIL_SWEEP                            = 58690,
    H_SPELL_TAIL_SWEEP                          = 59283,
    SPELL_UNCONTROLLABLE_ENERGY                 = 58688,
    H_SPELL_UNCONTROLLABLE_ENERGY               = 59281,
    SPELL_TRANSFORM                             = 58668
};

enum Yells
{
    SAY_AGGRO                                   = -1608000,
    SAY_SLAY_1                                  = -1608001,
    SAY_SLAY_2                                  = -1608002,
    SAY_SLAY_3                                  = -1608003,
    SAY_DEATH                                   = -1608004,
    SAY_SPAWN                                   = -1608005,
    SAY_DISRUPTION                              = -1608006,
    SAY_BREATH_ATTACK                           = -1608007,
    SAY_SPECIAL_ATTACK_1                        = -1608008,
    SAY_SPECIAL_ATTACK_2                        = -1608009
};

class boss_cyanigosa : public CreatureScript
{
public:
    boss_cyanigosa() : CreatureScript("boss_cyanigosa") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_cyanigosaAI(pCreature);
    }

    struct boss_cyanigosaAI : public ScriptedAI
    {
        boss_cyanigosaAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        uint32 uiArcaneVacuumTimer;
        uint32 uiBlizzardTimer;
        uint32 uiManaDestructionTimer;
        uint32 uiTailSweepTimer;
        uint32 uiUncontrollableEnergyTimer;

        InstanceScript* pInstance;

        void Reset()
        {
            uiArcaneVacuumTimer = 10000;
            uiBlizzardTimer = 15000;
            uiManaDestructionTimer = 30000;
            uiTailSweepTimer = 5000;
            uiUncontrollableEnergyTimer = 25000;
            if(pInstance)
                pInstance->SetData(DATA_CYANIGOSA_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);

            if(pInstance)
                pInstance->SetData(DATA_CYANIGOSA_EVENT, IN_PROGRESS);
        }

        void SpellHitTarget (Unit* target,const SpellEntry* spell)
        {
            if(spell->Id == SPELL_ARCANE_VACUUM)
            {
                if(target->ToPlayer())
                    target->ToPlayer()->TeleportTo(me->GetMapId(),me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(), 0);
            }
        }

        void MoveInLineOfSight(Unit* /*pWho*/) { }

        void UpdateAI(const uint32 diff)
        {
            if(pInstance && pInstance->GetData(DATA_REMOVE_NPC) == 1)
            {
                me->DespawnOrUnsummon();
                pInstance->SetData(DATA_REMOVE_NPC, 0);
            }

            //Return since we have no target
            if(!UpdateVictim())
                return;

            if(uiArcaneVacuumTimer <= diff)
            {
                DoCast(SPELL_ARCANE_VACUUM);
                uiArcaneVacuumTimer = 30000;
            } else uiArcaneVacuumTimer -= diff;

            if(uiBlizzardTimer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_BLIZZARD);
                uiBlizzardTimer = 15000;
            } else uiBlizzardTimer -= diff;

            if(uiTailSweepTimer <= diff)
            {
                DoCast(DUNGEON_MODE(SPELL_TAIL_SWEEP,H_SPELL_TAIL_SWEEP));
                uiTailSweepTimer = 5000;
            } else uiTailSweepTimer -= diff;

            if(uiUncontrollableEnergyTimer <= diff)
            {
                DoCastVictim(SPELL_UNCONTROLLABLE_ENERGY);
                uiUncontrollableEnergyTimer = 25000;
            } else uiUncontrollableEnergyTimer -= diff;

            if(IsHeroic())
            {
                if(uiManaDestructionTimer <= diff)
                {
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        DoCast(target, SPELL_MANA_DESTRUCTION);
                    uiManaDestructionTimer = 30000;
                } else uiManaDestructionTimer -= diff;
            }

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*pKiller*/)
        {
            DoScriptText(SAY_DEATH, me);

            if(pInstance)
                pInstance->SetData(DATA_CYANIGOSA_EVENT, DONE);
        }

        void KilledUnit(Unit* victim)
        {
            if(victim == me)
                return;
            DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2,SAY_SLAY_3), me);
        }
    };
};

class achievement_defenseless : public AchievementCriteriaScript
{
public:
    achievement_defenseless() : AchievementCriteriaScript("achievement_defenseless") { }

    bool OnCheck(Player* /*pPlayer*/, Unit* pTarget)
    {
        if(!pTarget)
            return false;

        InstanceScript* pInstanceScript = pTarget->GetInstanceScript();
        if(!pInstanceScript)
            return false;

        if(!pInstanceScript->GetData(DATA_DEFENSELESS))
            return false;

        return true;
    }
};

void AddSC_boss_cyanigosa()
{
    new boss_cyanigosa;
    new achievement_defenseless;
}

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
  SPELL_CAUTERIZING_FLAMES                      = 59466, //Only in heroic
  SPELL_FIREBOLT                                = 54235,
  H_SPELL_FIREBOLT                              = 59468,
  SPELL_FLAME_BREATH                            = 54282,
  H_SPELL_FLAME_BREATH                          = 59469,
  SPELL_LAVA_BURN                               = 54249,
  H_SPELL_LAVA_BURN                             = 59594
};

class boss_lavanthor : public CreatureScript
{
public:
    boss_lavanthor() : CreatureScript("boss_lavanthor") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_lavanthorAI(pCreature);
    }

    struct boss_lavanthorAI : public ScriptedAI
    {
        boss_lavanthorAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        uint32 uiFireboltTimer;
        uint32 uiFlameBreathTimer;
        uint32 uiLavaBurnTimer;
        uint32 uiCauterizingFlamesTimer;

        InstanceScript* pInstance;

        void Reset()
        {
            uiFireboltTimer = 1000;
            uiFlameBreathTimer = 5000;
            uiLavaBurnTimer = 10000;
            uiCauterizingFlamesTimer = 3000;
            if(pInstance)
            {
                if(pInstance->GetData(DATA_WAVE_COUNT) == 6)
                    pInstance->SetData(DATA_1ST_BOSS_EVENT, NOT_STARTED);
                else if(pInstance->GetData(DATA_WAVE_COUNT) == 12)
                    pInstance->SetData(DATA_2ND_BOSS_EVENT, NOT_STARTED);
            }
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            if(pInstance)
            {
            if(GameObject* pDoor = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_LAVANTHOR_CELL)))
                    if(pDoor->GetGoState() == GO_STATE_READY)
                    {
                        EnterEvadeMode();
                        return;
                    }
                if(pInstance->GetData(DATA_WAVE_COUNT) == 6)
                    pInstance->SetData(DATA_1ST_BOSS_EVENT, IN_PROGRESS);
                else if(pInstance->GetData(DATA_WAVE_COUNT) == 12)
                    pInstance->SetData(DATA_2ND_BOSS_EVENT, IN_PROGRESS);
            }
        }

        void AttackStart(Unit* who)
        {
            if(me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE) || me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                return;

            if(me->Attack(who, true))
            {
                me->AddThreat(who, 0.0f);
                me->SetInCombatWith(who);
                who->SetInCombatWith(me);
                DoStartMovement(who);
            }
        }

        void MoveInLineOfSight(Unit* /*pWho*/) { }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if(!UpdateVictim())
                return;

            if(uiFireboltTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FIREBOLT);
                uiFireboltTimer = urand(5000, 13000);
            } else uiFireboltTimer -= diff;

            if(uiFlameBreathTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FLAME_BREATH);
                uiFlameBreathTimer = urand(10000, 15000);
            } else uiFlameBreathTimer -= diff;

            if(uiLavaBurnTimer <= diff)
            {
                DoCast(me->getVictim(), DUNGEON_MODE(SPELL_LAVA_BURN, H_SPELL_LAVA_BURN));
                uiLavaBurnTimer = urand(15000, 23000);
            }

            if(IsHeroic())
            {
                if(uiCauterizingFlamesTimer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_CAUTERIZING_FLAMES);
                    uiCauterizingFlamesTimer = urand(10000, 16000);
                } else uiCauterizingFlamesTimer -= diff;
            }

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(pInstance)
            {
                if(pInstance->GetData(DATA_WAVE_COUNT) == 6)
                {
                    if(IsHeroic() && pInstance->GetData(DATA_1ST_BOSS_EVENT) == DONE)
                        me->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);

                    pInstance->SetData(DATA_1ST_BOSS_EVENT, DONE);
                    pInstance->SetData(DATA_WAVE_COUNT, 7);
                }
                else if(pInstance->GetData(DATA_WAVE_COUNT) == 12)
                {
                    if(IsHeroic() && pInstance->GetData(DATA_2ND_BOSS_EVENT) == DONE)
                        me->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);

                    pInstance->SetData(DATA_2ND_BOSS_EVENT, DONE);
                    pInstance->SetData(DATA_WAVE_COUNT, 13);
                }
            }
        }
    };
};

void AddSC_boss_lavanthor()
{
    new boss_lavanthor;
}

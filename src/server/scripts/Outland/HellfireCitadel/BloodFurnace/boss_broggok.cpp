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
#include "blood_furnace.h"

enum eEnums
{
    SAY_AGGRO               = -1542008,

    SPELL_SLIME_SPRAY       = 30913,
    SPELL_POISON_CLOUD      = 30916,
    SPELL_POISON_BOLT       = 30917,

    SPELL_POISON            = 30914
};

class boss_broggok : public CreatureScript
{
    public:
        boss_broggok() : CreatureScript("boss_broggok") { }

        struct boss_broggokAI : public ScriptedAI
        {
            boss_broggokAI(Creature* pCreature): ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
            }

            InstanceScript* pInstance;

            uint32 AcidSpray_Timer;
            uint32 PoisonSpawn_Timer;
            uint32 PoisonBolt_Timer;

            void Reset()
            {
                AcidSpray_Timer = 10000;
                PoisonSpawn_Timer = 5000;
                PoisonBolt_Timer = 7000;
                if(pInstance)
                {
                    pInstance->SetData(TYPE_BROGGOK_EVENT, NOT_STARTED);
                    pInstance->HandleGameObject(pInstance->GetData64(DATA_DOOR4), true);
                }
            }

            void EnterCombat(Unit* /*pWho*/)
            {
                DoScriptText(SAY_AGGRO, me);
                if(pInstance)
                {
                    pInstance->SetData(TYPE_BROGGOK_EVENT, IN_PROGRESS);
                    pInstance->HandleGameObject(pInstance->GetData64(DATA_DOOR4), false);
                }
            }

            void JustSummoned(Creature* summoned)
            {
                summoned->setFaction(16);
                summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                summoned->CastSpell(summoned, SPELL_POISON, false, 0, 0, me->GetGUID());
            }

            void UpdateAI(const uint32 diff)
            {
                if(!UpdateVictim())
                    return;

                if(AcidSpray_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_SLIME_SPRAY);
                    AcidSpray_Timer = 4000+rand()%8000;
                }
                else
                    AcidSpray_Timer -=diff;

                if(PoisonBolt_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_POISON_BOLT);
                    PoisonBolt_Timer = 4000+rand()%8000;
                }
                else
                    PoisonBolt_Timer -=diff;

                if(PoisonSpawn_Timer <= diff)
                {
                    DoCast(me, SPELL_POISON_CLOUD);
                    PoisonSpawn_Timer = 20000;
                }
                else
                    PoisonSpawn_Timer -=diff;

                DoMeleeAttackIfReady();
            }

            void JustDied(Unit* /*pWho*/)
            {
                if(pInstance)
                {
                    pInstance->HandleGameObject(pInstance->GetData64(DATA_DOOR4), true);
                    pInstance->HandleGameObject(pInstance->GetData64(DATA_DOOR5), true);
                    pInstance->SetData(TYPE_BROGGOK_EVENT, DONE);
                }
            }

        };

        CreatureAI* GetAI(Creature* Creature) const
        {
            return new boss_broggokAI (Creature);
        }
};

void AddSC_boss_broggok()
{
    new boss_broggok;
}

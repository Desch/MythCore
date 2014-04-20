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
#include "culling_of_stratholme.h"

enum Spells
{
    SPELL_CURSE_OF_TWISTED_FLESH                = 58845,
    SPELL_EXPLODE_GHOUL                         = 52480,
    H_SPELL_EXPLODE_GHOUL                       = 58825,
    SPELL_SHADOW_BOLT                           = 57725,
    H_SPELL_SHADOW_BOLT                         = 58828,
    SPELL_STEAL_FLESH                           = 52708,
    SPELL_SUMMON_GHOULS                         = 52451
};

enum Yells
{
    SAY_AGGRO                                   = -1595032,
    SAY_SPAWN                                   = -1595033,
    SAY_SLAY_1                                  = -1595034,
    SAY_SLAY_2                                  = -1595035,
    SAY_SLAY_3                                  = -1595036,
    SAY_DEATH                                   = -1595037,
    SAY_EXPLODE_GHOUL_1                         = -1595038,
    SAY_EXPLODE_GHOUL_2                         = -1595039,
    SAY_STEAL_FLESH_1                           = -1595040,
    SAY_STEAL_FLESH_2                           = -1595041,
    SAY_STEAL_FLESH_3                           = -1595042,
    SAY_SUMMON_GHOULS_1                         = -1595043,
    SAY_SUMMON_GHOULS_2                         = -1595044
};

class boss_salramm : public CreatureScript
{
public:
    boss_salramm() : CreatureScript("boss_salramm") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_salrammAI(pCreature);
    }

    struct boss_salrammAI : public ScriptedAI
    {
        boss_salrammAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
            if(pInstance)
                DoScriptText(SAY_SPAWN, me);
        }

        uint32 uiCurseFleshTimer;
        uint32 uiExplodeGhoulTimer;
        uint32 uiShadowBoltTimer;
        uint32 uiStealFleshTimer;
        uint32 uiSummonGhoulsTimer;
        uint32 uiRespawnZombiesTimer;
        bool bTransformed;

        InstanceScript* pInstance;

        void Reset()
        {
             uiCurseFleshTimer = 30000;  //30s DBM
             uiExplodeGhoulTimer = urand(25000, 28000); //approx 6 sec after summon ghouls
             uiShadowBoltTimer = urand(8000, 12000); // approx 10s
             uiStealFleshTimer = 12345;
             uiSummonGhoulsTimer = urand(19000, 24000); //on a video approx 24s after aggro
             uiRespawnZombiesTimer = 200000;
             bTransformed = false;

             if(pInstance)
                 pInstance->SetData(DATA_SALRAMM_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);

            if(pInstance)
                 pInstance->SetData(DATA_SALRAMM_EVENT, IN_PROGRESS);
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if(!UpdateVictim())
                return;

            //Curse of twisted flesh timer
            if(uiCurseFleshTimer <= diff) {
                DoCast(me->getVictim(), SPELL_CURSE_OF_TWISTED_FLESH);
                uiCurseFleshTimer = 37000;
            } else uiCurseFleshTimer -= diff;

            //Shadow bolt timer
            if(uiShadowBoltTimer <= diff) {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_SHADOW_BOLT);
                uiShadowBoltTimer = urand(8000, 12000);
            } else uiShadowBoltTimer -= diff;

            //Steal Flesh timer
            if(uiStealFleshTimer <= diff) {
                DoScriptText(RAND(SAY_STEAL_FLESH_1, SAY_STEAL_FLESH_2, SAY_STEAL_FLESH_3), me);
                if(Unit* random_pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(random_pTarget, SPELL_STEAL_FLESH);
                uiStealFleshTimer = 10000;
            } else uiStealFleshTimer -= diff;

            //Summon ghouls timer
            if(uiSummonGhoulsTimer <= diff) {
                DoScriptText(RAND(SAY_SUMMON_GHOULS_1, SAY_SUMMON_GHOULS_2), me);
                if(Unit* random_pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(random_pTarget, SPELL_SUMMON_GHOULS);
                uiSummonGhoulsTimer = 10000;
            } else uiSummonGhoulsTimer -= diff;

            // Respawn risen zombies at town
            if(!bTransformed) {
                if(uiRespawnZombiesTimer <= diff) {
                    pInstance->SetData(DATA_TRANSFORM_CITIZENS, SPECIAL);
                    bTransformed = true;
                } else uiRespawnZombiesTimer -= diff;
            }

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*pKiller*/)
        {
            DoScriptText(SAY_DEATH, me);

            if(pInstance)
                pInstance->SetData(DATA_SALRAMM_EVENT, DONE);
        }

        void KilledUnit(Unit* victim)
        {
            if(victim == me)
                return;

            DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2, SAY_SLAY_3), me);
        }
    };
};

void AddSC_boss_salramm()
{
    new boss_salramm;
}

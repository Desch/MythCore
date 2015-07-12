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
#include "GroupReference.h"
#include "Group.h"
#include "trial_of_the_crusader.h"

enum Yells
{
    //Gormok
    SAY_SNOBOLLED        = -1649000,
    //Acidmaw & Dreadscale
    SAY_SUBMERGE         = -1649010,
    SAY_EMERGE           = -1649011,
    SAY_BERSERK          = -1649012,
    //Icehowl
    SAY_TRAMPLE_STARE    = -1649020,
    SAY_TRAMPLE_FAIL     = -1649021,
    SAY_TRAMPLE_START    = -1649022,
};

enum Equipment
{
    EQUIP_MAIN           = 50760,
    EQUIP_OFFHAND        = 48040,
    EQUIP_RANGED         = 47267,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum Model
{
    MODEL_ACIDMAW_STATIONARY     = 29815,
    MODEL_ACIDMAW_MOBILE         = 29816,
    MODEL_DREADSCALE_STATIONARY  = 26935,
    MODEL_DREADSCALE_MOBILE      = 24564,
};

enum Summons
{
    NPC_SNOBOLD_VASSAL   = 34800,
    NPC_SLIME_POOL       = 35176,
};

enum BossSpells
{
    //Gormok
    SPELL_IMPALE_10_N            = 66331,
    SPELL_IMPALE_25_N            = 67477,
    SPELL_IMPALE_10_H            = 67478,
    SPELL_IMPALE_25_H            = 67479,
    SPELL_STAGGERING_STOMP_10_N  = 66330,
    SPELL_STAGGERING_STOMP_25_N  = 67647,
    SPELL_STAGGERING_STOMP_10_H  = 67648,
    SPELL_STAGGERING_STOMP_25_H  = 67649,
    SPELL_RISING_ANGER      = 66636,
    //Snobold
    SPELL_SNOBOLLED         = 66406,
    SPELL_BATTER            = 66408,
    SPELL_FIRE_BOMB         = 66313,
    SPELL_FIRE_BOMB_1       = 66317,
    SPELL_FIRE_BOMB_DOT     = 66318,
    SPELL_HEAD_CRACK        = 66407,

    //Acidmaw & Dreadscale
    SPELL_ACID_SPIT_10_N         = 66880,
    SPELL_ACID_SPIT_25_N         = 67606,
    SPELL_ACID_SPIT_10_H         = 67607,
    SPELL_ACID_SPIT_25_H         = 67608,
    SPELL_PARALYTIC_SPRAY_10_N   = 66901,
    SPELL_PARALYTIC_SPRAY_25_N   = 67615,
    SPELL_PARALYTIC_SPRAY_10_H   = 67616,
    SPELL_PARALYTIC_SPRAY_25_H   = 67617,
    SPELL_ACID_SPEW              = 66819,
    SPELL_PARALYTIC_BITE_10_N    = 66824,
    SPELL_PARALYTIC_BITE_25_N    = 67612,
    SPELL_PARALYTIC_BITE_10_H    = 67613,
    SPELL_PARALYTIC_BITE_25_H    = 67614,
    SPELL_SWEEP_0_10_N           = 66794,
    SPELL_SWEEP_0_25_N           = 67644,
    SPELL_SWEEP_0_10_H           = 67645,
    SPELL_SWEEP_0_25_H           = 67646,
    SUMMON_SLIME_POOL_10_N       = 66883,
    SUMMON_SLIME_POOL_25_N       = 67641,
    SUMMON_SLIME_POOL_10_H       = 67642,
    SUMMON_SLIME_POOL_25_H       = 67643,
    SPELL_FIRE_SPIT_10_N         = 66796,
    SPELL_FIRE_SPIT_25_N         = 67632,
    SPELL_FIRE_SPIT_10_H         = 67633,
    SPELL_FIRE_SPIT_25_H         = 67634,
    SPELL_MOLTEN_SPEW            = 66821,
    SPELL_BURNING_BITE_10_N      = 66879,
    SPELL_BURNING_BITE_25_N      = 67624,
    SPELL_BURNING_BITE_10_H      = 67625,
    SPELL_BURNING_BITE_25_H      = 67626,
    SPELL_BURNING_SPRAY_10_N     = 66902,
    SPELL_BURNING_SPRAY_25_N     = 67627,
    SPELL_BURNING_SPRAY_10_H     = 67628,
    SPELL_BURNING_SPRAY_25_H     = 67629,
    SPELL_SWEEP_1_10_N           = 66794,
    SPELL_SWEEP_1_25_N           = 67644,
    SPELL_SWEEP_1_10_H           = 67645,
    SPELL_SWEEP_1_25_H           = 67646,
    SPELL_EMERGE_0               = 66947,
    SPELL_SUBMERGE_0             = 53421,
    SPELL_ENRAGE                 = 68335,
    SPELL_SLIME_POOL_EFFECT      = 66882, //In 60s it diameter grows from 10y to 40y (r=r+0.25 per second)

    //Icehowl
    SPELL_JUMP_BACK              = 66733,
    SPELL_FEROCIOUS_BUTT_10_N    = 66770,
    SPELL_FEROCIOUS_BUTT_25_N    = 67654,
    SPELL_FEROCIOUS_BUTT_10_H    = 67655,
    SPELL_FEROCIOUS_BUTT_25_H    = 67656,
    SPELL_MASSIVE_CRASH_10_N     = 66683,
    SPELL_MASSIVE_CRASH_25_N     = 67660,
    SPELL_MASSIVE_CRASH_10_H     = 67661,
    SPELL_MASSIVE_CRASH_25_H     = 67662,
    SPELL_WHIRL_10_N             = 67345,
    SPELL_WHIRL_25_N             = 67663,
    SPELL_WHIRL_10_H             = 67664,
    SPELL_WHIRL_25_H             = 67665,
    SPELL_ARCTIC_BREATH_10_N     = 66689,
    SPELL_ARCTIC_BREATH_25_N     = 67650,
    SPELL_ARCTIC_BREATH_10_H     = 67651,
    SPELL_ARCTIC_BREATH_25_H     = 67652,
    SPELL_TRAMPLE                = 66734,
    SPELL_FROTHING_RAGE          = 66759,
    SPELL_STAGGERED_DAZE         = 66758,
};

class boss_gormok : public CreatureScript
{
public:
    boss_gormok() : CreatureScript("boss_gormok") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_gormokAI(pCreature);
    }

    struct boss_gormokAI : public ScriptedAI
    {
        boss_gormokAI(Creature* pCreature) : ScriptedAI(pCreature), Summons(me)
        {
            pInstance = pCreature->GetInstanceScript();
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        InstanceScript* pInstance;

        uint32 m_uiImpaleTimer;
        uint32 m_uiStaggeringStompTimer;
        SummonList Summons;
        uint32 m_uiSummonTimer;
        uint32 m_uiSummonCount;
        uint32 m_uiNextBossTimer;
        bool m_uiNextBossCheck;

        void Reset()
        {
            m_uiImpaleTimer = urand(8*IN_MILLISECONDS, 10*IN_MILLISECONDS);
            m_uiStaggeringStompTimer = 15*IN_MILLISECONDS;
            m_uiSummonTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);;

            if(GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL || GetDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC)
                m_uiSummonCount = 5;
            else
                m_uiSummonCount = 4;

            m_uiNextBossTimer = 180*IN_MILLISECONDS;
            m_uiNextBossCheck = false;

            Summons.DespawnAll();
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(pInstance && !m_uiNextBossCheck)
                pInstance->SetData(TYPE_NORTHREND_BEASTS, GORMOK_DONE);

            Summons.DespawnAll(); // this was missing to despawn the snobold on gormok death, okay
        }

        void JustReachedHome()
        {
            if(pInstance)
                pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);
            me->DespawnOrUnsummon();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            me->SetInCombatWithZone();
            pInstance->SetData(TYPE_NORTHREND_BEASTS, GORMOK_IN_PROGRESS);
        }

        void JustSummoned(Creature* summon)
        {
            if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
            {
                if(summon->GetEntry() == NPC_SNOBOLD_VASSAL)
                {
                    summon->GetMotionMaster()->MoveJump(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 10.0f, 20.0f);
                    DoCast(me, SPELL_RISING_ANGER);
                    --m_uiSummonCount;
                }
                summon->AI()->AttackStart(target);
            }
            Summons.Summon(summon);
        }

        void SummonedCreatureDespawn(Creature* pSummon)
        {
            if(pSummon->GetEntry() == NPC_SNOBOLD_VASSAL)
                if(pSummon->isAlive())
                    ++m_uiSummonCount;
            Summons.Despawn(pSummon);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(IsHeroic() && !m_uiNextBossCheck)
            {
                if(m_uiNextBossTimer <= diff)
                {
                    m_uiNextBossTimer = 0;
                    m_uiNextBossCheck = true;
                    pInstance->SetData(TYPE_NORTHREND_BEASTS, GORMOK_DONE);
                } else m_uiNextBossTimer -= diff;
            }

            if(m_uiImpaleTimer <= diff)
            {
                DoCastVictim(RAID_MODE(SPELL_IMPALE_10_N, SPELL_IMPALE_25_N, SPELL_IMPALE_10_H, SPELL_IMPALE_25_H));
                m_uiImpaleTimer = urand(8*IN_MILLISECONDS, 10*IN_MILLISECONDS);
            } else m_uiImpaleTimer -= diff;

            if(m_uiStaggeringStompTimer <= diff)
            {
                DoCastVictim(RAID_MODE(SPELL_STAGGERING_STOMP_10_N, SPELL_STAGGERING_STOMP_25_N, SPELL_STAGGERING_STOMP_10_H, SPELL_STAGGERING_STOMP_25_H));
                m_uiStaggeringStompTimer = urand(20*IN_MILLISECONDS, 25*IN_MILLISECONDS);
            } else m_uiStaggeringStompTimer -= diff;

            if(m_uiSummonTimer <= diff)
            {
                if(m_uiSummonCount > 0)
                {
                    me->SummonCreature(NPC_SNOBOLD_VASSAL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN);
                    DoScriptText(SAY_SNOBOLLED, me);
                }
                m_uiSummonTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
            } else m_uiSummonTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

class mob_snobold_vassal : public CreatureScript
{
public:
    mob_snobold_vassal() : CreatureScript("mob_snobold_vassal") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_snobold_vassalAI(pCreature);
    }

    struct mob_snobold_vassalAI : public ScriptedAI
    {
        mob_snobold_vassalAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
            if(pInstance)
                pInstance->SetData(DATA_SNOBOLD_COUNT, INCREASE);
        }

        InstanceScript* pInstance;
        uint32 m_uiFireBombTimer;
        uint32 m_uiBatterTimer;
        uint32 m_uiHeadCrackTimer;
        uint64 m_uiBossGUID;
        uint64 m_uiTargetGUID;
        bool   m_bTargetDied;

        void Reset()
        {
            m_uiFireBombTimer   = 15000;
            m_uiBatterTimer     = 5000;
            m_uiHeadCrackTimer  = 25000;

            m_uiTargetGUID      = 0;
            m_bTargetDied       = false;
            if(pInstance)
                m_uiBossGUID = pInstance->GetData64(NPC_GORMOK);
            //Workaround for Snobold
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        }
        //gogog
        //void EnterEvadeMode()
        //{
        //    ScriptedAI::EnterEvadeMode();
        //}

        void EnterCombat(Unit* pWho)
        {
            m_uiTargetGUID = pWho->GetGUID();
            me->TauntApply(pWho);
            DoCast(pWho, SPELL_SNOBOLLED);
        }

        //void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
        //{
        //    if(pDoneBy->GetGUID() == m_uiTargetGUID)
        //        uiDamage = 0;
        //}

        void MovementInform(uint32 uiType, uint32 uiId)
        {
            if(uiType != POINT_MOTION_TYPE)
                return;

            switch(uiId)
            {
                case 0: // JUMP!? Fuck! THAT'S BEEZARR! Would someone PLEASE make MotionMaster->Move* work better?
                    if(m_bTargetDied)
                        me->DespawnOrUnsummon();
                    break;
            }
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(Unit* pTarget = Unit::GetPlayer(*me, m_uiTargetGUID))
                if(pTarget->isAlive())
                    pTarget->RemoveAurasDueToSpell(SPELL_SNOBOLLED);
            if(pInstance)
                pInstance->SetData(DATA_SNOBOLD_COUNT, DECREASE);
        }

        void UpdateAI(const uint32 diff)
        {
            if(m_bTargetDied || !UpdateVictim())
                return;

            if(Unit* pTarget = Unit::GetPlayer(*me, m_uiTargetGUID))
            {
                if(!pTarget->isAlive())//wait, until boss not killed this vassals are "uber/mega not dieing" ? yeah , they also should be mounted on players ... :| i don't know if it works here or not, not works =/ let's make it despawn first and see later..since now they are buggy as :|
                {
                    if(pInstance)
                    {
                        Unit* gormok = ObjectAccessor::GetCreature(*me, pInstance->GetData64(NPC_GORMOK));
                        if(gormok && gormok->isAlive())
                        {
                            SetCombatMovement(false);
                            m_bTargetDied   = true;
                            me->GetMotionMaster()->MoveJump(gormok->GetPositionX(), gormok->GetPositionY(), gormok->GetPositionZ(), 15.0f, 15.0f);
                        } else if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0)) {
                            m_uiTargetGUID = target->GetGUID();
                            me->GetMotionMaster()->MoveJump(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 15.0f, 15.0f);
                        }
                    }
                }
            }

            if(m_uiFireBombTimer < diff)
            {
                if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_FIRE_BOMB);
                m_uiFireBombTimer = 20000;
            } else m_uiFireBombTimer -= diff;

            if(m_uiBatterTimer < diff)
            {
                if(Unit* pTarget = Unit::GetPlayer(*me, m_uiTargetGUID))
                    DoCast(pTarget, SPELL_BATTER);
                m_uiBatterTimer = 10000;
            } else m_uiBatterTimer -= diff;

            if(m_uiHeadCrackTimer < diff)
            {
                if(Unit* pTarget = Unit::GetPlayer(*me, m_uiTargetGUID))
                    DoCast(pTarget, SPELL_HEAD_CRACK);
                m_uiHeadCrackTimer = 35000;
            } else m_uiHeadCrackTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

struct boss_jormungarAI : public ScriptedAI
{
    boss_jormungarAI(Creature* pCreature): ScriptedAI(pCreature)
    {
        pInstance = pCreature->GetInstanceScript();
        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
    }

    void Reset()
    {
        enraged = false;
        biteTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
        spewTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
        slimePoolTimer = 15*IN_MILLISECONDS;
        spitTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
        sprayTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
        sweepTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
        m_uiNextBossTimer = 180*IN_MILLISECONDS;
        m_uiNextBossCheck = false;
    }

    void JustDied(Unit* /*pKiller*/)
    {
        if(pInstance)
        {
            if(Creature* otherWorm = Unit::GetCreature(*me, pInstance->GetData64(otherWormEntry)))
            {
                if(!otherWorm->isAlive())
                {
                    if(!m_uiNextBossCheck)
                        pInstance->SetData(TYPE_NORTHREND_BEASTS, SNAKES_DONE);

                    me->DespawnOrUnsummon();
                    otherWorm->DespawnOrUnsummon();
                }
                else
                    pInstance->SetData(TYPE_NORTHREND_BEASTS, SNAKES_SPECIAL);
            }
        }
    }

    void JustReachedHome()
    {
        if(pInstance && pInstance->GetData(TYPE_NORTHREND_BEASTS) != FAIL)
            pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);

        me->DespawnOrUnsummon();
    }

    void KilledUnit(Unit* pWho)
    {
        if(pWho->GetTypeId() == TYPEID_PLAYER)
        {
            if(pInstance)
                pInstance->SetData(DATA_TRIBUTE_TO_IMMORTALITY_ELEGIBLE, 0);
        }
    }

    void EnterCombat(Unit* /*pWho*/)
    {
        me->SetInCombatWithZone();
        if(pInstance)
            pInstance->SetData(TYPE_NORTHREND_BEASTS, SNAKES_IN_PROGRESS);
    }

    void UpdateAI(const uint32 diff)
    {
        if(!UpdateVictim())
            return;

        if(IsHeroic() && !m_uiNextBossCheck)
        {
            if(m_uiNextBossTimer <= diff)
            {
                if(!m_uiNextBossCheck)
                    pInstance->SetData(TYPE_NORTHREND_BEASTS, SNAKES_DONE);
                m_uiNextBossCheck = true;
            } else m_uiNextBossTimer -= diff;
        }

        if(pInstance && pInstance->GetData(TYPE_NORTHREND_BEASTS) == SNAKES_SPECIAL && !enraged)
        {
            DoScriptText(SAY_EMERGE, me);
            me->RemoveAurasDueToSpell(SPELL_SUBMERGE_0);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            DoCast(SPELL_ENRAGE);
            enraged = true;
            DoScriptText(SAY_BERSERK, me);
            switch(stage)
            {
                case 0:
                    break;
                case 4:
                    stage = 5;
                    submergeTimer = 5*IN_MILLISECONDS;
                    break;
                default:
                    stage = 7;
            }
        }

        switch(stage)
        {
            case 0: // Mobile
                if(biteTimer <= diff)
                {
                    DoCastVictim(biteSpell);
                    biteTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
                } else biteTimer -= diff;

                if(spewTimer <= diff)
                {
                    DoCastAOE(spewSpell);
                    spewTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
                } else spewTimer -= diff;

                if(slimePoolTimer <= diff)
                {
                    /* Spell summon has only 30s duration */
                    DoCast(me, RAID_MODE(SUMMON_SLIME_POOL_10_N, SUMMON_SLIME_POOL_25_N, SUMMON_SLIME_POOL_10_H, SUMMON_SLIME_POOL_25_H));
                    slimePoolTimer = 30*IN_MILLISECONDS;
                } else slimePoolTimer -= diff;

                if(submergeTimer <= diff && !enraged)
                {
                    stage = 1;
                    submergeTimer = 5*IN_MILLISECONDS;
                } else submergeTimer -= diff;

                DoMeleeAttackIfReady();
                break;
            case 1: // Submerge
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                DoCast(me, SPELL_SUBMERGE_0);
                DoScriptText(SAY_SUBMERGE, me);
                me->GetMotionMaster()->MovePoint(0, ToCCommonLoc[1].GetPositionX()+urand(0, 80)-40, ToCCommonLoc[1].GetPositionY()+urand(0, 80)-40, ToCCommonLoc[1].GetPositionZ());
                stage = 2;
            case 2: // Wait til emerge
                if(submergeTimer <= diff)
                {
                    stage = 3;
                    submergeTimer = 50*IN_MILLISECONDS;
                } else submergeTimer -= diff;
                break;
            case 3: // Emerge
                me->SetDisplayId(modelStationary);
                DoScriptText(SAY_EMERGE, me);
                me->RemoveAurasDueToSpell(SPELL_SUBMERGE_0);
                DoCast(me, SPELL_EMERGE_0);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                SetCombatMovement(false);
                me->GetMotionMaster()->MoveIdle();
                stage = 4;
                break;
            case 4: // Stationary
                if(sprayTimer <= diff)
                {
                    if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(pTarget, spraySpell);
                    sprayTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
                } else sprayTimer -= diff;

                if(sweepTimer <= diff)
                {
                    DoCastAOE(RAID_MODE(SPELL_SWEEP_0_10_N, SPELL_SWEEP_0_25_N, SPELL_SWEEP_0_10_H, SPELL_SWEEP_0_25_H));
                    sweepTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
                } else sweepTimer -= diff;

                if(submergeTimer <= diff)
                {
                    stage = 5;
                    submergeTimer = 10*IN_MILLISECONDS;
                } else submergeTimer -= diff;

                DoSpellAttackIfReady(spitSpell);
                break;
            case 5: // Submerge
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                DoCast(me, SPELL_SUBMERGE_0);
                DoScriptText(SAY_SUBMERGE, me);
                me->GetMotionMaster()->MovePoint(0, ToCCommonLoc[1].GetPositionX()+urand(0, 80)-40, ToCCommonLoc[1].GetPositionY()+urand(0, 80)-40, ToCCommonLoc[1].GetPositionZ());
                stage = 6;
            case 6: // Wait til emerge
                if(submergeTimer <= diff)
                {
                    stage = 7;
                    submergeTimer = 45*IN_MILLISECONDS;
                } else submergeTimer -= diff;
                break;
            case 7: // Emerge
                me->SetDisplayId(modelMobile);
                DoScriptText(SAY_EMERGE, me);
                me->RemoveAurasDueToSpell(SPELL_SUBMERGE_0);
                DoCast(me, SPELL_EMERGE_0);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                SetCombatMovement(true);
                me->GetMotionMaster()->MoveChase(me->getVictim());
                stage = 0;
                break;
        }
    }

    InstanceScript* pInstance;

    uint32 otherWormEntry;
    uint32 modelStationary;
    uint32 modelMobile;
    uint32 biteSpell;
    uint32 spewSpell;
    uint32 spitSpell;
    uint32 spraySpell;
    uint32 biteTimer;
    uint32 spewTimer;
    uint32 slimePoolTimer;
    uint32 spitTimer;
    uint32 sprayTimer;
    uint32 sweepTimer;
    uint32 submergeTimer;
    uint8  stage;
    bool   enraged;

    uint32 m_uiNextBossTimer;
    bool m_uiNextBossCheck;
};

class boss_acidmaw : public CreatureScript
{
    public:
    boss_acidmaw() : CreatureScript("boss_acidmaw") { }

    struct boss_acidmawAI : public boss_jormungarAI
    {
        boss_acidmawAI(Creature* pCreature) : boss_jormungarAI(pCreature) { }

        void Reset()
        {
            boss_jormungarAI::Reset();
            biteSpell       = RAID_MODE(SPELL_PARALYTIC_BITE_10_N, SPELL_PARALYTIC_BITE_25_N, SPELL_PARALYTIC_BITE_10_H, SPELL_PARALYTIC_BITE_25_H);
            spewSpell       = SPELL_ACID_SPEW;
            spitSpell       = RAID_MODE(SPELL_ACID_SPIT_10_N, SPELL_ACID_SPIT_25_N, SPELL_ACID_SPIT_10_H, SPELL_ACID_SPIT_25_H);
            spraySpell      = RAID_MODE(SPELL_PARALYTIC_SPRAY_10_N, SPELL_PARALYTIC_SPRAY_25_N, SPELL_PARALYTIC_SPRAY_10_H, SPELL_PARALYTIC_SPRAY_25_H);
            modelStationary = MODEL_ACIDMAW_STATIONARY;
            modelMobile     = MODEL_ACIDMAW_MOBILE;
            otherWormEntry  = NPC_DREADSCALE;

            submergeTimer   = 500;
            DoCast(me, SPELL_SUBMERGE_0);
            stage           = 2;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_acidmawAI(pCreature);
    }
};

class boss_dreadscale : public CreatureScript
{
public:
    boss_dreadscale() : CreatureScript("boss_dreadscale") { }

    struct boss_dreadscaleAI : public boss_jormungarAI
    {
        boss_dreadscaleAI(Creature* pCreature) : boss_jormungarAI(pCreature) { }

        void Reset()
        {
            boss_jormungarAI::Reset();
            biteSpell       = RAID_MODE(SPELL_BURNING_BITE_10_N, SPELL_BURNING_BITE_25_N, SPELL_BURNING_BITE_10_H, SPELL_BURNING_BITE_25_H);
            spewSpell       = SPELL_MOLTEN_SPEW;
            spitSpell       = RAID_MODE(SPELL_FIRE_SPIT_10_N, SPELL_FIRE_SPIT_25_N, SPELL_FIRE_SPIT_10_H, SPELL_FIRE_SPIT_25_H);
            spraySpell      = RAID_MODE(SPELL_BURNING_SPRAY_10_N, SPELL_BURNING_SPRAY_25_N, SPELL_BURNING_SPRAY_10_H, SPELL_BURNING_SPRAY_25_H);
            modelStationary = MODEL_DREADSCALE_STATIONARY;
            modelMobile     = MODEL_DREADSCALE_MOBILE;
            otherWormEntry  = NPC_ACIDMAW;

            submergeTimer   = 45 * IN_MILLISECONDS;
            stage           = 0;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_dreadscaleAI(pCreature);
    }
};

class mob_slime_pool : public CreatureScript
{
public:
    mob_slime_pool() : CreatureScript("mob_slime_pool") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_slime_poolAI(pCreature);
    }

    struct mob_slime_poolAI : public ScriptedAI
    {
        mob_slime_poolAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        bool casted;
        void Reset()
        {
            casted = false;
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if(!casted)
            {
                casted = true;
                DoCast(me, SPELL_SLIME_POOL_EFFECT);
            }
        }
    };
};

enum eIcehowlMoveEvents
{
    POINT_CENTER    = 1,
    //POINT_TRAMPLE,
};

class boss_icehowl : public CreatureScript
{
public:
    boss_icehowl() : CreatureScript("boss_icehowl") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_icehowlAI(pCreature);
    }

    struct boss_icehowlAI : public ScriptedAI
    {
        boss_icehowlAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        InstanceScript* pInstance;

        uint32 m_uiFerociousButtTimer;
        uint32 m_uiArticBreathTimer;
        uint32 m_uiWhirlTimer;
        uint32 m_uiMassiveCrashTimer;
        uint32 m_uiTrampleTimer;
        Position tramplePos;
        uint64 m_uiTrampleTargetGUID;
        bool   m_bTrampleHit;
        uint8  m_uiStage;

        void Reset()
        {
            m_uiFerociousButtTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
            m_uiArticBreathTimer = urand(25*IN_MILLISECONDS, 40*IN_MILLISECONDS);
            m_uiWhirlTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
            m_uiMassiveCrashTimer = 30*IN_MILLISECONDS;
            m_uiTrampleTimer = IN_MILLISECONDS;
            m_bTrampleHit = false;
            m_uiTrampleTargetGUID = 0;
            m_uiStage = 0;
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(pInstance)
                pInstance->SetData(TYPE_NORTHREND_BEASTS, ICEHOWL_DONE);
        }

        void MovementInform(uint32 uiType, uint32 uiId)
        {
            if(uiType == POINT_MOTION_TYPE && uiId == POINT_CENTER)
                m_uiStage = 1;
        }

        void JustReachedHome()
        {
            if(pInstance)
                pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);
            me->DespawnOrUnsummon();
        }

        void KilledUnit(Unit* pWho)
        {
            if(pWho->GetTypeId() == TYPEID_PLAYER)
            {
                if(pInstance)
                    pInstance->SetData(DATA_TRIBUTE_TO_IMMORTALITY_ELEGIBLE, 0);

                // Custom hack fix for Upper Back Pain (25 player) Achievement http://www.wowhead.com/achievement=3813/upper-back-pain-25-player
                if(pWho->ToPlayer()->GetGroup() && GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL)
                {
                    Group* group = pWho->ToPlayer()->GetGroup();
                    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                    {
                        Player* pl = itr->getSource();

                        if (!pl || !pl->GetSession())
                            continue;

                        uint32 achiev = 3813;

                        if(AchievementEntry const* pAchievment = GetAchievementStore()->LookupEntry(achiev))
                            pl->CompletedAchievement(pAchievment);            

                    }
                }
            }
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            if(pInstance)
                pInstance->SetData(TYPE_NORTHREND_BEASTS, ICEHOWL_IN_PROGRESS);
            me->SetInCombatWithZone();
        }

        void SpellHitTarget(Unit* target, const SpellEntry* spell)  //TODO: spellscript 66734 instead
        {
            if(spell->Id == SPELL_TRAMPLE && target->GetTypeId() == TYPEID_PLAYER)
            {
                if(!m_bTrampleHit)
                {
                    DoCast(me, SPELL_FROTHING_RAGE, true);
                    m_bTrampleHit = true;
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim() || me->HasUnitState(UNIT_STAT_CASTING))
                return;

            if(Unit* pVictim = me->getVictim()) //WTF is this?
            {
                if(pVictim->GetMap() != me->GetMap())
                {
                    if(Unit* pNewTarget = SelectTarget(SELECT_TARGET_NEAREST, 0, 180.0f, true))
                    {
                        me->getThreatManager().tauntApply(pNewTarget);
                        pVictim->CombatStop(true);
                    }
                }
            }

            switch(m_uiStage)
            {
                case 0:
                    if(m_uiFerociousButtTimer <= diff)
                    {
                        DoCastVictim(RAID_MODE(SPELL_FEROCIOUS_BUTT_10_N, SPELL_FEROCIOUS_BUTT_25_N, SPELL_FEROCIOUS_BUTT_10_H, SPELL_FEROCIOUS_BUTT_25_H));
                        m_uiFerociousButtTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
                    } else m_uiFerociousButtTimer -= diff;

                    if(m_uiArticBreathTimer <= diff)
                    {
                        if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(pTarget, RAID_MODE(SPELL_ARCTIC_BREATH_10_N, SPELL_ARCTIC_BREATH_25_N, SPELL_ARCTIC_BREATH_10_H, SPELL_ARCTIC_BREATH_25_H));
                        m_uiArticBreathTimer = urand(25*IN_MILLISECONDS, 40*IN_MILLISECONDS);
                    } else m_uiArticBreathTimer -= diff;

                    if(m_uiWhirlTimer <= diff)
                    {
                        DoCastAOE(RAID_MODE(SPELL_WHIRL_10_N, SPELL_WHIRL_25_N, SPELL_WHIRL_10_H, SPELL_WHIRL_25_H));
                        m_uiWhirlTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
                    } else m_uiWhirlTimer -= diff;

                    if(m_uiMassiveCrashTimer <= diff)
                    {
                        me->SetReactState(REACT_PASSIVE);
                        me->GetMotionMaster()->MovementExpired();
                        me->GetMotionMaster()->MovePoint(POINT_CENTER, ToCCommonLoc[1]);
                        m_uiStage = 7; //Invalid (Do nothing more than move)
                        m_uiMassiveCrashTimer = 30*IN_MILLISECONDS;
                        return;
                    } else m_uiMassiveCrashTimer -= diff;

                    DoMeleeAttackIfReady();
                    break;
                case 1:
                    DoCastAOE(RAID_MODE(SPELL_MASSIVE_CRASH_10_N, SPELL_MASSIVE_CRASH_25_N, SPELL_MASSIVE_CRASH_10_H, SPELL_MASSIVE_CRASH_25_H));
                    me->StopMoving();
                    me->AttackStop();
                    m_uiStage = 2;
                    break;
                case 2:
                    if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                    {
                        me->StopMoving();
                        me->AttackStop();
                        m_uiTrampleTargetGUID = pTarget->GetGUID();
                        me->SetTarget(m_uiTrampleTargetGUID);
                        SetGazeOn(pTarget);
                        DoScriptText(SAY_TRAMPLE_STARE, me, pTarget);
                        tramplePos.Relocate(me);
                        m_bTrampleHit = false;
                        me->GetMotionMaster()->MovementExpired();
                        m_uiTrampleTimer = 4*IN_MILLISECONDS;
                        m_uiStage = 3;
                    } else m_uiStage = 0;
                    break;
                case 3:
                    if(m_uiTrampleTimer <= diff)
                    {
                        if(Unit* pTarget = ObjectAccessor::GetPlayer(*me, m_uiTrampleTargetGUID))
                            tramplePos.Relocate(pTarget);
                        else if(!tramplePos.IsPositionValid()) {
                            m_uiStage = 6;
                            return;
                        }
                        tramplePos.m_positionZ = me->GetMap()->GetHeight(tramplePos.GetPositionX(), tramplePos.GetPositionY(), tramplePos.GetPositionZ()+2.0f);
                        DoCastVictim(SPELL_JUMP_BACK);
                        m_uiStage = 4;
                    } else m_uiTrampleTimer -= diff;
                    break;
                case 4:
                    if(me->HasUnitMovementFlag(MOVEMENTFLAG_JUMPING | MOVEMENTFLAG_MASK_MOVING))
                        return;

                    if(Unit* pTarget = ObjectAccessor::GetPlayer(*me, m_uiTrampleTargetGUID))
                        SetGazeOn(pTarget);
                    DoScriptText(SAY_TRAMPLE_START, me);
                    me->AddUnitState(UNIT_STAT_IGNORE_PATHFINDING); //generate straight path to prevent buggering
                    me->GetMotionMaster()->MoveCharge(tramplePos.GetPositionX(), tramplePos.GetPositionY(), tramplePos.GetPositionZ(), 42.0f);
                    me->ClearUnitState(UNIT_STAT_IGNORE_PATHFINDING);
                    m_uiTrampleTimer = IN_MILLISECONDS;
                    m_uiStage = 5;
                    break;
                case 5:
                    if(me->GetExactDistSq(&tramplePos) < 9.0f)
                    {
                        m_uiStage = 6;
                        me->GetMotionMaster()->MovementExpired();
                        return;
                    }
                    if(m_uiTrampleTimer <= diff)    //TODO: trample should be casted when it lands from each jump, not during in air
                    {
                        DoCastAOE(SPELL_TRAMPLE);
                        m_uiTrampleTimer = 700;     //here was 1000 used. Probably it should be in a correspondence with diff times
                    } else m_uiTrampleTimer -= diff;
                    break;
                case 6:
                    if(!m_bTrampleHit)
                    {
                        DoScriptText(SAY_TRAMPLE_FAIL, me);
                        DoCast(me, SPELL_STAGGERED_DAZE);
                        m_uiMassiveCrashTimer += 10*IN_MILLISECONDS;
                    }
                    me->SetReactState(REACT_AGGRESSIVE);
                    DoStartMovement(me->getVictim());
                    tramplePos.m_positionX = tramplePos.m_positionY = tramplePos.m_positionZ = 0.0f;
                    m_uiTrampleTargetGUID = 0;
                    m_uiStage = 0;
                    break;
            }
        }
    };
};

void AddSC_boss_northrend_beasts()
{
    new boss_gormok;
    new mob_snobold_vassal;
    new boss_acidmaw;
    new boss_dreadscale;
    new mob_slime_pool;
    new boss_icehowl;
}

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

#define SAY_MIDNIGHT_KILL           -1532000
#define SAY_APPEAR1                 -1532001
#define SAY_APPEAR2                 -1532002
#define SAY_APPEAR3                 -1532003
#define SAY_MOUNT                   -1532004
#define SAY_KILL1                   -1532005
#define SAY_KILL2                   -1532006
#define SAY_DISARMED                -1532007
#define SAY_DEATH                   -1532008
#define SAY_RANDOM1                 -1532009
#define SAY_RANDOM2                 -1532010

#define SPELL_SHADOWCLEAVE          29832
#define SPELL_INTANGIBLE_PRESENCE   29833
#define SPELL_BERSERKER_CHARGE      26561                   //Only when mounted

#define MOUNTED_DISPLAYID           16040

//Attumen (TODO: Use the summoning spell instead of Creature id. It works, but is not convenient for us)
#define SUMMON_ATTUMEN 15550

class boss_attumen : public CreatureScript
{
public:
    boss_attumen() : CreatureScript("boss_attumen") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_attumenAI(pCreature);
    }

    struct boss_attumenAI : public ScriptedAI
    {
        boss_attumenAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            Phase = 1;

            CleaveTimer = urand(10000, 15000);
            CurseTimer = 30000;
            RandomYellTimer = urand(30000, 60000);              //Occasionally yell
            ChargeTimer = 20000;
            ResetTimer = 0;
        }

        uint64 Midnight;
        uint8 Phase;
        uint32 CleaveTimer;
        uint32 CurseTimer;
        uint32 RandomYellTimer;
        uint32 ChargeTimer;                                     //only when mounted
        uint32 ResetTimer;

        void Reset()
        {
            ResetTimer = 2000;
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(RAND(SAY_KILL1, SAY_KILL2), me);
        }

        void JustDied(Unit* /*pVictim*/)
        {
            DoScriptText(SAY_DEATH, me);
            if(Unit* pMidnight = Unit::GetUnit(*me, Midnight))
                pMidnight->Kill(pMidnight);
        }

        void UpdateAI(const uint32 diff);

        void SpellHit(Unit* /*source*/, const SpellEntry* spell)
        {
            if(spell->Mechanic == MECHANIC_DISARM)
                DoScriptText(SAY_DISARMED, me);
        }
    };
};

class boss_midnight : public CreatureScript
{
public:
    boss_midnight() : CreatureScript("boss_midnight") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_midnightAI(pCreature);
    }

    struct boss_midnightAI : public ScriptedAI
    {
        boss_midnightAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint64 Attumen;
        uint8 Phase;
        uint32 Mount_Timer;

        void Reset()
        {
            Phase = 1;
            Attumen = 0;
            Mount_Timer = 0;

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetVisible(true);
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void KilledUnit(Unit* /*pVictim*/)
        {
            if(Phase == 2)
            {
                if(Unit* pUnit = Unit::GetUnit(*me, Attumen))
                DoScriptText(SAY_MIDNIGHT_KILL, pUnit);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(Phase == 1 && HealthBelowPct(95))
            {
                Phase = 2;
                if(Creature* pAttumen = me->SummonCreature(SUMMON_ATTUMEN, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 30000))
                {
                    Attumen = pAttumen->GetGUID();
                    pAttumen->AI()->AttackStart(me->getVictim());
                    SetMidnight(pAttumen, me->GetGUID());
                    DoScriptText(RAND(SAY_APPEAR1, SAY_APPEAR2, SAY_APPEAR3), pAttumen);
                }
            }
            else if(Phase == 2 && HealthBelowPct(25))
            {
                if(Unit* pAttumen = Unit::GetUnit(*me, Attumen))
                    Mount(pAttumen);
            }
            else if(Phase == 3)
            {
                if(Mount_Timer)
                {
                    if(Mount_Timer <= diff)
                    {
                        Mount_Timer = 0;
                        me->SetVisible(false);
                        me->GetMotionMaster()->MoveIdle();
                        if(Unit* pAttumen = Unit::GetUnit(*me, Attumen))
                        {
                            pAttumen->SetDisplayId(MOUNTED_DISPLAYID);
                            pAttumen->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            if(pAttumen->getVictim())
                            {
                                pAttumen->GetMotionMaster()->MoveChase(pAttumen->getVictim());
                                pAttumen->SetTarget(pAttumen->getVictim()->GetGUID());
                            }
                            pAttumen->SetFloatValue(OBJECT_FIELD_SCALE_X, 1);
                        }
                    } else Mount_Timer -= diff;
                }
            }

            if(Phase != 3)
                DoMeleeAttackIfReady();
        }

        void Mount(Unit* pAttumen)
        {
            DoScriptText(SAY_MOUNT, pAttumen);
            Phase = 3;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pAttumen->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            float angle = me->GetAngle(pAttumen);
            float distance = me->GetDistance2d(pAttumen);
            float newX = me->GetPositionX() + cos(angle)*(distance/2) ;
            float newY = me->GetPositionY() + sin(angle)*(distance/2) ;
            float newZ = 50;
            //me->Relocate(newX, newY, newZ, angle);
            //me->SendMonsterMove(newX, newY, newZ, 0, true, 1000);
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MovePoint(0, newX, newY, newZ);
            distance += 10;
            newX = me->GetPositionX() + cos(angle)*(distance/2) ;
            newY = me->GetPositionY() + sin(angle)*(distance/2) ;
            pAttumen->GetMotionMaster()->Clear();
            pAttumen->GetMotionMaster()->MovePoint(0, newX, newY, newZ);
            //pAttumen->Relocate(newX, newY, newZ, -angle);
            //pAttumen->SendMonsterMove(newX, newY, newZ, 0, true, 1000);
            Mount_Timer = 1000;
        }

        void SetMidnight(Creature* pAttumen, uint64 value)
        {
            CAST_AI(boss_attumen::boss_attumenAI, pAttumen->AI())->Midnight = value;
        }
    };
};

void boss_attumen::boss_attumenAI::UpdateAI(const uint32 diff)
{
    if(ResetTimer)
    {
        if(ResetTimer <= diff)
        {
            ResetTimer = 0;
            Unit* pMidnight = Unit::GetUnit(*me, Midnight);
            if(pMidnight)
            {
                pMidnight->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pMidnight->SetVisible(true);
            }
            Midnight = 0;
            me->SetVisible(false);
            me->Kill(me);
        }
    } else ResetTimer -= diff;

    //Return since we have no target
    if(!UpdateVictim())
        return;

    if(me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE))
        return;

    if(CleaveTimer <= diff)
    {
        DoCast(me->getVictim(), SPELL_SHADOWCLEAVE);
        CleaveTimer = urand(10000, 15000);
    } else CleaveTimer -= diff;

    if(CurseTimer <= diff)
    {
        DoCast(me->getVictim(), SPELL_INTANGIBLE_PRESENCE);
        CurseTimer = 30000;
    } else CurseTimer -= diff;

    if(RandomYellTimer <= diff)
    {
        DoScriptText(RAND(SAY_RANDOM1, SAY_RANDOM2), me);
        RandomYellTimer = urand(30000, 60000);
    } else RandomYellTimer -= diff;

    if(me->GetUInt32Value(UNIT_FIELD_DISPLAYID) == MOUNTED_DISPLAYID)
    {
        if(ChargeTimer <= diff)
        {
            Unit* target = NULL;
            std::list<HostileReference *> t_list = me->getThreatManager().getThreatList();
            std::vector<Unit* > target_list;
            for(std::list<HostileReference *>::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                target = Unit::GetUnit(*me, (*itr)->getUnitGuid());
                if(target && !target->IsWithinDist(me, ATTACK_DISTANCE, false))
                    target_list.push_back(target);
                target = NULL;
            }
            if(!target_list.empty())
                target = *(target_list.begin()+rand()%target_list.size());

            DoCast(target, SPELL_BERSERKER_CHARGE);
            ChargeTimer = 20000;
        } else ChargeTimer -= diff;
    }
    else
    {
        if(HealthBelowPct(25))
        {
            Creature* pMidnight = Unit::GetCreature(*me, Midnight);
            if(pMidnight && pMidnight->GetTypeId() == TYPEID_UNIT)
            {
                CAST_AI(boss_midnight::boss_midnightAI, (pMidnight->AI()))->Mount(me);
                me->SetHealth(pMidnight->GetHealth());
                DoResetThreat();
            }
        }
    }

    DoMeleeAttackIfReady();
}

void AddSC_boss_attumen()
{
    new boss_attumen;
    new boss_midnight;
}

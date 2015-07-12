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
#include "halls_of_reflection.h"

enum
{
    SAY_FALRIC_AGGRO            = -1668050,
    SAY_FALRIC_DEATH            = -1668053,
    SAY_FALRIC_SLAY01           = -1668051,
    SAY_FALRIC_SLAY02           = -1668052,
    SAY_FALRIC_SP01             = -1668054,
    SAY_FALRIC_SP02             = -1668055,

    SPELL_HOPELESSNESS          = 72395,
    SPELL_IMPENDING_DESPAIR     = 72426,
    SPELL_DEFILING_HORROR_N     = 72435,
    SPELL_DEFILING_HORROR_H     = 72452,
    SPELL_QUIVERING_STRIKE_N    = 72422,
    SPELL_QUIVERING_STRIKE_H    = 72453,

    SPELL_BERSERK               = 47008
};

class boss_falric : public CreatureScript
{
public:
    boss_falric() : CreatureScript("boss_falric") { }

    struct boss_falricAI : public ScriptedAI
    {
        boss_falricAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
            HeroMode = pCreature->GetMap()->IsRegularDifficulty();
            Reset();
        }

        InstanceScript* pInstance;
        bool HeroMode;
        bool m_bIsCall;
        uint32 m_uiBerserkTimer;
        uint32 m_uiGrowlTimer;
        uint32 m_uiHorrorTimer;
        uint32 m_uiStrikeTimer;
        uint32 m_uiSummonTimer;
        uint32 m_uiLocNo;
        uint64 m_uiSummonGUID[16];
        uint32 m_uiCheckSummon;
        uint8 SummonCount;
        uint32 NEXTSUMMON;

        void Reset()
        {
            m_uiBerserkTimer    = 180000;
            SummonCount         = 0;
            m_bIsCall           = false;
            m_uiGrowlTimer      = 20000;
            m_uiHorrorTimer     = urand(14000, 20000);
            m_uiStrikeTimer     = 2000;
            m_uiSummonTimer     = 11000;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetVisible(false);
        }

        void EnterCombat(Unit* /*pVictim*/)
        {
            DoScriptText(SAY_FALRIC_AGGRO, me);
            DoCast(me, SPELL_HOPELESSNESS);
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            switch(urand(0, 1))
            {
                case 0: DoScriptText(SAY_FALRIC_SLAY01, me); break;
                case 1: DoScriptText(SAY_FALRIC_SLAY02, me); break;
            }
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(!pInstance) return;
                pInstance->SetData(TYPE_MARWYN, SPECIAL);

            DoScriptText(SAY_FALRIC_DEATH, me);
        }

        void AttackStart(Unit* who)
        {
            if(!pInstance) return;

            if(pInstance->GetData(TYPE_FALRIC) != IN_PROGRESS)
                return;

            ScriptedAI::AttackStart(who);
        }

        void Summon()
        {
            m_uiLocNo = 0;

            for(int i = 0; i < 14; i++)
            {
                switch(urand(0, 3))
                {
                    case 0:
                        switch(urand(1, 3)) {
                            case 1: NEXTSUMMON = NPC_DARK_1; break;
                            case 2: NEXTSUMMON = NPC_DARK_3; break;
                            case 3: NEXTSUMMON = NPC_DARK_6; break;
                        }
                        break;
                    case 1:
                        switch(urand(1, 3)) {
                            case 1: NEXTSUMMON = NPC_DARK_2; break;
                            case 2: NEXTSUMMON = NPC_DARK_3; break;
                            case 3: NEXTSUMMON = NPC_DARK_4; break;
                        }
                        break;
                    case 2:
                        switch(urand(1, 3)) {
                            case 1: NEXTSUMMON = NPC_DARK_2; break;
                            case 2: NEXTSUMMON = NPC_DARK_5; break;
                            case 3: NEXTSUMMON = NPC_DARK_6; break;
                        }
                        break;
                    case 3:
                        switch(urand(1, 3)) {
                            case 1: NEXTSUMMON = NPC_DARK_1; break;
                            case 2: NEXTSUMMON = NPC_DARK_5; break;
                            case 3: NEXTSUMMON = NPC_DARK_4; break;
                        }
                        break;
                }

                m_uiCheckSummon = 0;

                Creature* pCreature = me->SummonCreature(NEXTSUMMON, SpawnLoc[m_uiLocNo].x, SpawnLoc[m_uiLocNo].y, SpawnLoc[m_uiLocNo].z, SpawnLoc[m_uiLocNo].o, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                if(pCreature) {
                    m_uiSummonGUID[i] = pCreature->GetGUID();
                    pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pCreature->setFaction(974);
                    pCreature->SetReactState(REACT_PASSIVE);
                }
                m_uiLocNo++;
            }
       }

       void CallFallSoldier()
       {
            for(int i = 0; i < 4; i++)
            {
                if(Creature* Summon = pInstance->instance->GetCreature(m_uiSummonGUID[m_uiCheckSummon]))
                {
                    Summon->setFaction(14);
                    Summon->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    Summon->SetReactState(REACT_AGGRESSIVE);
                    Summon->SetInCombatWithZone();
                }
                m_uiCheckSummon++;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!pInstance) return;

            if(pInstance->GetData(TYPE_FALRIC) == SPECIAL)
            {
                if(!m_bIsCall)
                {
                    m_bIsCall = true;
                    Summon();
                }

                if(m_uiSummonTimer < diff)
                {
                    ++SummonCount;
                    if(SummonCount > 4)
                    {
                        pInstance->SetData(TYPE_FALRIC, IN_PROGRESS);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->SetInCombatWithZone();
                    } else CallFallSoldier();
                    m_uiSummonTimer = 60000;
                } else m_uiSummonTimer -= diff;
            }

            if(!UpdateVictim())
                return;

            if(m_uiStrikeTimer < diff)
            {
                DoCast(me->getVictim(), HeroMode ? SPELL_QUIVERING_STRIKE_N : SPELL_QUIVERING_STRIKE_H);
                m_uiStrikeTimer = (urand(7000, 14000));
            } else m_uiStrikeTimer -= diff;

            if(m_uiHorrorTimer < diff)
            {
                DoScriptText(SAY_FALRIC_SP01, me);
                if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                   DoCast(pTarget, SPELL_IMPENDING_DESPAIR);
                m_uiHorrorTimer = (urand(15000, 25000));
            } else m_uiHorrorTimer -= diff;

            if(m_uiGrowlTimer < diff)
            {
                DoScriptText(SAY_FALRIC_SP02, me);
                DoCast(me->getVictim(), HeroMode ? SPELL_DEFILING_HORROR_N : SPELL_DEFILING_HORROR_H);
                m_uiGrowlTimer = (urand(25000, 30000));
            } else m_uiGrowlTimer -= diff;

            if(m_uiBerserkTimer < diff)
            {
                DoCast(me, SPELL_BERSERK);
                m_uiBerserkTimer = 180000;
            } else  m_uiBerserkTimer -= diff;

            DoMeleeAttackIfReady();

            return;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_falricAI(pCreature);
    }
};

void AddSC_boss_falric()
{
    new boss_falric;
}
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
    SAY_MARWYN_INTRO                        = -1594506,
    SAY_MARWYN_AGGRO                        = -1668060,
    SAY_MARWYN_DEATH                        = -1668063,
    SAY_MARWYN_SLAY01                       = -1668061,
    SAY_MARWYN_SLAY02                       = -1668062,
    SAY_MARWYN_SP01                         = -1668064,
    SAY_MARWYN_SP02                         = -1668065,

    SPELL_OBLITERATE_N                      = 72360,
    SPELL_OBLITERATE_H                      = 72434,
    SPELL_SHARED_SUFFERING_N                = 72368,
    SPELL_SHARED_SUFFERING_H                = 72369,
    SPELL_WELL_OF_CORRUPTION                = 72362,
    SPELL_CORRUPTED_FLESH_N                 = 72363,
    SPELL_CORRUPTED_FLESH_H                 = 72436,

    SPELL_BERSERK                           = 47008,
};

class boss_marwyn : public CreatureScript
{
public:
    boss_marwyn() : CreatureScript("boss_marwyn") { }

    struct boss_marwynAI : public ScriptedAI
    {
        boss_marwynAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
            HeroMode = pCreature->GetMap()->IsRegularDifficulty();
            Reset();
        }

       InstanceScript* pInstance;
       bool HeroMode;
       bool m_bIsCall;

       uint32 m_uiBerserkTimer;
       uint32 m_uiSharedSufferingTimer;
       uint32 m_uiWellTimer;
       uint32 m_uiTouchTimer;
       uint32 m_uiFleshTimer;
       uint32 m_uiObliterateTimer;
       uint32 m_uiSummonTimer;

       uint32 m_uiLocNo;
       uint64 m_uiSummonGUID[16];
       uint32 m_uiCheckSummon;

       uint8 SummonCount;

       uint32 NEXTSUMMON;

        void Reset()
        {
            m_uiBerserkTimer = 180000;
            m_uiSharedSufferingTimer = 4000;
            m_uiWellTimer = 12000;
            m_uiTouchTimer = 8000;
            m_uiFleshTimer = 21000;
            m_uiObliterateTimer = 5000;
            SummonCount = 0;
            m_bIsCall = false;
            m_uiSummonTimer = 15000;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetVisible(false);
        }

        void Summon()
        {
            m_uiLocNo = 14;

            for(int i = 0; i < 14; i++)
            {
                switch(urand(0, 3))
                {
                    case 0:
                        switch(urand(1, 3))
                        {
                            case 1: NEXTSUMMON = NPC_DARK_1; break;
                            case 2: NEXTSUMMON = NPC_DARK_3; break;
                            case 3: NEXTSUMMON = NPC_DARK_6; break;
                        }
                        break;
                    case 1:
                        switch(urand(1, 3))
                        {
                            case 1: NEXTSUMMON = NPC_DARK_2; break;
                            case 2: NEXTSUMMON = NPC_DARK_3; break;
                            case 3: NEXTSUMMON = NPC_DARK_4; break;
                        }
                        break;
                    case 2:
                        switch(urand(1, 3))
                        {
                            case 1: NEXTSUMMON = NPC_DARK_2; break;
                            case 2: NEXTSUMMON = NPC_DARK_5; break;
                            case 3: NEXTSUMMON = NPC_DARK_6; break;
                        }
                        break;
                    case 3:
                        switch(urand(1, 3))
                        {
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
                    pCreature->SetReactState(REACT_PASSIVE);
                    pCreature->setFaction(974);
                }
                m_uiLocNo++;
            }
        }

        void CallFallSoldier()
        {
            for(int i = 0; i < 4; i++)
            {
                Creature* pCreature = pInstance->instance->GetCreature(m_uiSummonGUID[m_uiCheckSummon]);
                if(pCreature) {
                    pCreature->setFaction(14);
                    pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pCreature->SetReactState(REACT_AGGRESSIVE);
                    pCreature->SetInCombatWithZone();
                }

                m_uiCheckSummon++;
            }
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(pInstance) {
                pInstance->SetData(TYPE_MARWYN, DONE);
                pInstance->SetData(TYPE_PHASE, 3);
                DoScriptText(SAY_MARWYN_DEATH, me);
            }
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            switch(urand(0, 1))
            {
                case 0: DoScriptText(SAY_MARWYN_SLAY01, me); break;
                case 1: DoScriptText(SAY_MARWYN_SLAY02, me); break;
            }
        }

        void EnterCombat(Unit* /*pVictim*/)
        {
            if(pInstance)
                DoScriptText(SAY_MARWYN_AGGRO, me);
        }

        void AttackStart(Unit* pWho)
        {
            if(!pInstance || pInstance->GetData(TYPE_MARWYN) != IN_PROGRESS)
                return;

            ScriptedAI::AttackStart(pWho);
        }

       void UpdateAI(const uint32 diff)
        {
            if(!pInstance) return;

            if(pInstance->GetData(TYPE_FALRIC) == SPECIAL)
            {
                if(!m_bIsCall) {
                    m_bIsCall = true;
                    Summon();
                }
            }

            if(pInstance->GetData(TYPE_MARWYN) == SPECIAL)
            {
                if(m_uiSummonTimer < diff)
                {
                    ++SummonCount;
                    if(SummonCount == 1)
                        DoScriptText(SAY_MARWYN_INTRO, me);

                    if(SummonCount > 4) {
                        pInstance->SetData(TYPE_MARWYN, IN_PROGRESS);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        me->SetInCombatWithZone();
                    } else CallFallSoldier();
                    m_uiSummonTimer = 60000;
                } else m_uiSummonTimer -= diff;
            }

            if(!UpdateVictim())
                return;

            if(m_uiObliterateTimer < diff) {
                DoCast(me->getVictim(), HeroMode ? SPELL_OBLITERATE_N : SPELL_OBLITERATE_H);
                m_uiObliterateTimer = urand(8000, 12000);
            } else m_uiObliterateTimer -= diff;

            if(m_uiWellTimer < diff) {
                DoScriptText(SAY_MARWYN_SP02, me);
                if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                   DoCast(pTarget, SPELL_WELL_OF_CORRUPTION);
                m_uiWellTimer= urand(25000, 30000);
            } else m_uiWellTimer -= diff;

            if(m_uiSharedSufferingTimer < diff) {
                if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                   DoCast(pTarget, HeroMode ? SPELL_SHARED_SUFFERING_N : SPELL_SHARED_SUFFERING_H);
                m_uiSharedSufferingTimer = urand(15000, 20000);
            } else m_uiSharedSufferingTimer -= diff;

            if(m_uiFleshTimer < diff) {
                DoScriptText(SAY_MARWYN_SP01, me);
                if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                    DoCast(pTarget, HeroMode ? SPELL_CORRUPTED_FLESH_N : SPELL_CORRUPTED_FLESH_H);
                m_uiFleshTimer = urand(10000, 16000);
            } else m_uiFleshTimer -= diff;

            if(m_uiBerserkTimer < diff) {
                DoCast(me, SPELL_BERSERK);
                m_uiBerserkTimer = 180000;
            } else  m_uiBerserkTimer -= diff;

            DoMeleeAttackIfReady();

            return;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_marwynAI(pCreature);
    }
};

void AddSC_boss_marwyn()
{
    new boss_marwyn;
}
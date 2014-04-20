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
#include "scarlet_monastery.h"

enum eEnums
{
    //Mograine says
    SAY_MO_AGGRO                 = -1189005,
    SAY_MO_KILL                  = -1189006,
    SAY_MO_RESSURECTED           = -1189007,

    //Whitemane says
    SAY_WH_INTRO                 = -1189008,
    SAY_WH_KILL                  = -1189009,
    SAY_WH_RESSURECT             = -1189010,

    //Mograine Spells
    SPELL_CRUSADERSTRIKE         = 14518,
    SPELL_HAMMEROFJUSTICE        = 5589,
    SPELL_LAYONHANDS             = 9257,
    SPELL_RETRIBUTIONAURA        = 8990,

    //Whitemanes Spells
    SPELL_DEEPSLEEP              = 9256,
    SPELL_SCARLETRESURRECTION    = 9232,
    SPELL_DOMINATEMIND           = 14515,
    SPELL_HOLYSMITE              = 9481,
    SPELL_HEAL                   = 12039,
    SPELL_POWERWORDSHIELD        = 22187
};

class boss_scarlet_commander_mograine : public CreatureScript
{
public:
    boss_scarlet_commander_mograine() : CreatureScript("boss_scarlet_commander_mograine") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_scarlet_commander_mograineAI(pCreature);
    }

    struct boss_scarlet_commander_mograineAI : public ScriptedAI
    {
        boss_scarlet_commander_mograineAI(Creature* pCreature): ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;

        uint32 m_uiCrusaderStrike_Timer;
        uint32 m_uiHammerOfJustice_Timer;

        bool m_bHasDied;
        bool m_bHeal;
        bool m_bFakeDeath;

        void Reset()
        {
            m_uiCrusaderStrike_Timer = 10000;
            m_uiHammerOfJustice_Timer = 10000;

            //Incase wipe during phase that mograine fake death
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetStandState(UNIT_STAND_STATE_STAND);

            if(pInstance)
                if(me->isAlive())
                    pInstance->SetData(TYPE_MOGRAINE_AND_WHITE_EVENT, NOT_STARTED);

            m_bHasDied = false;
            m_bHeal = false;
            m_bFakeDeath = false;
        }

        void JustReachedHome()
        {
            if(pInstance)
            {
                if(pInstance->GetData(TYPE_MOGRAINE_AND_WHITE_EVENT != NOT_STARTED))
                    pInstance->SetData(TYPE_MOGRAINE_AND_WHITE_EVENT, FAIL);
            }
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_MO_AGGRO, me);
            DoCast(me, SPELL_RETRIBUTIONAURA);

            me->CallForHelp(VISIBLE_RANGE);
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(SAY_MO_KILL, me);
        }

        void DamageTaken(Unit* /*pDoneBy*/, uint32 &uiDamage)
        {
            if(uiDamage < me->GetHealth() || m_bHasDied || m_bFakeDeath)
                return;

            if(!pInstance)
                return;

            //On first death, fake death and open door, as well as initiate whitemane if exist
            if(Unit* Whitemane = Unit::GetUnit((*me), pInstance->GetData64(DATA_WHITEMANE)))
            {
                pInstance->SetData(TYPE_MOGRAINE_AND_WHITE_EVENT, IN_PROGRESS);

                Whitemane->GetMotionMaster()->MovePoint(1, 1163.113370f, 1398.856812f, 32.527786f);

                me->GetMotionMaster()->MovementExpired();
                me->GetMotionMaster()->MoveIdle();

                me->SetHealth(0);

                if(me->IsNonMeleeSpellCasted(false))
                    me->InterruptNonMeleeSpells(false);

                me->ClearComboPointHolders();
                me->RemoveAllAuras();
                me->ClearAllReactives();

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetStandState(UNIT_STAND_STATE_DEAD);

                m_bHasDied = true;
                m_bFakeDeath = true;

                uiDamage = 0;
            }
        }

        void SpellHit(Unit* /*pWho*/, const SpellEntry* pSpell)
        {
            //When hit with ressurection say text
            if(pSpell->Id == SPELL_SCARLETRESURRECTION)
            {
                DoScriptText(SAY_MO_RESSURECTED, me);
                m_bFakeDeath = false;

                if(pInstance)
                    pInstance->SetData(TYPE_MOGRAINE_AND_WHITE_EVENT, SPECIAL);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(m_bHasDied && !m_bHeal && pInstance && pInstance->GetData(TYPE_MOGRAINE_AND_WHITE_EVENT) == SPECIAL)
            {
                //On ressurection, stop fake death and heal whitemane and resume fight
                if(Unit* Whitemane = Unit::GetUnit((*me), pInstance->GetData64(DATA_WHITEMANE)))
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->SetStandState(UNIT_STAND_STATE_STAND);
                    DoCast(Whitemane, SPELL_LAYONHANDS);

                    m_uiCrusaderStrike_Timer = 10000;
                    m_uiHammerOfJustice_Timer = 10000;

                    if(me->getVictim())
                        me->GetMotionMaster()->MoveChase(me->getVictim());

                    m_bHeal = true;
                }
            }

            //This if-check to make sure mograine does not attack while fake death
            if(m_bFakeDeath)
                return;

            //m_uiCrusaderStrike_Timer
            if(m_uiCrusaderStrike_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CRUSADERSTRIKE);
                m_uiCrusaderStrike_Timer = 10000;
            } else m_uiCrusaderStrike_Timer -= diff;

            //m_uiHammerOfJustice_Timer
            if(m_uiHammerOfJustice_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_HAMMEROFJUSTICE);
                m_uiHammerOfJustice_Timer = 60000;
            } else m_uiHammerOfJustice_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

class boss_high_inquisitor_whitemane : public CreatureScript
{
public:
    boss_high_inquisitor_whitemane() : CreatureScript("boss_high_inquisitor_whitemane") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_high_inquisitor_whitemaneAI(pCreature);
    }

    struct boss_high_inquisitor_whitemaneAI : public ScriptedAI
    {
        boss_high_inquisitor_whitemaneAI(Creature* pCreature): ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;

        uint32 m_uiHeal_Timer;
        uint32 m_uiPowerWordShield_Timer;
        uint32 m_uiHolySmite_Timer;
        uint32 m_uiWait_Timer;

        bool m_bCanResurrectCheck;
        bool m_bCanResurrect;

        void Reset()
        {
            m_uiWait_Timer = 7000;
            m_uiHeal_Timer = 10000;
            m_uiPowerWordShield_Timer = 15000;
            m_uiHolySmite_Timer = 6000;

            m_bCanResurrectCheck = false;
            m_bCanResurrect = false;

            if(pInstance)
                if(me->isAlive())
                    pInstance->SetData(TYPE_MOGRAINE_AND_WHITE_EVENT, NOT_STARTED);
        }

        void AttackStart(Unit* who)
        {
            if(pInstance && pInstance->GetData(TYPE_MOGRAINE_AND_WHITE_EVENT) == NOT_STARTED)
                return;

            ScriptedAI::AttackStart(who);
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_WH_INTRO, me);
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(SAY_WH_KILL, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(m_bCanResurrect)
            {
                //When casting resuruction make sure to delay so on rez when reinstate battle deepsleep runs out
                if(pInstance && m_uiWait_Timer <= diff)
                {
                    if(Unit* Mograine = Unit::GetUnit((*me), pInstance->GetData64(DATA_MOGRAINE)))
                    {
                        DoCast(Mograine, SPELL_SCARLETRESURRECTION);
                        DoScriptText(SAY_WH_RESSURECT, me);
                        m_bCanResurrect = false;
                    }
                }
                else m_uiWait_Timer -= diff;
            }

            //Cast Deep sleep when health is less than 50%
            if(!m_bCanResurrectCheck && !HealthAbovePct(50))
            {
                if(me->IsNonMeleeSpellCasted(false))
                    me->InterruptNonMeleeSpells(false);

                DoCast(me->getVictim(), SPELL_DEEPSLEEP);
                m_bCanResurrectCheck = true;
                m_bCanResurrect = true;
                return;
            }

            //while in "resurrect-mode", don't do anything
            if(m_bCanResurrect)
                return;

            //If we are <75% hp cast healing spells at self or Mograine
            if(m_uiHeal_Timer <= diff)
            {
                Creature* target = NULL;

                if(!HealthAbovePct(75))
                    target = me;

                if(pInstance)
                {
                    if(Creature* pMograine = Unit::GetCreature((*me), pInstance->GetData64(DATA_MOGRAINE)))
                    {
                        // checking m_bCanResurrectCheck prevents her healing Mograine while he is "faking death"
                        if(m_bCanResurrectCheck && pMograine->isAlive() && !pMograine->HealthAbovePct(75))
                            target = pMograine;
                    }
                }

                if(target)
                    DoCast(target, SPELL_HEAL);

                m_uiHeal_Timer = 13000;
            } else m_uiHeal_Timer -= diff;

            //m_uiPowerWordShield_Timer
            if(m_uiPowerWordShield_Timer <= diff)
            {
                DoCast(me, SPELL_POWERWORDSHIELD);
                m_uiPowerWordShield_Timer = 15000;
            } else m_uiPowerWordShield_Timer -= diff;

            //m_uiHolySmite_Timer
            if(m_uiHolySmite_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_HOLYSMITE);
                m_uiHolySmite_Timer = 6000;
            } else m_uiHolySmite_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_mograine_and_whitemane()
{
    new boss_scarlet_commander_mograine;
    new boss_high_inquisitor_whitemane;
}

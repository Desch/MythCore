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
#include "gruuls_lair.h"

enum eEnums
{
    SAY_AGGRO                   = -1565010,
    SAY_SLAM1                   = -1565011,
    SAY_SLAM2                   = -1565012,
    SAY_SHATTER1                = -1565013,
    SAY_SHATTER2                = -1565014,
    SAY_SLAY1                   = -1565015,
    SAY_SLAY2                   = -1565016,
    SAY_SLAY3                   = -1565017,
    SAY_DEATH                   = -1565018,

    EMOTE_GROW                  = -1565019,

    SPELL_GROWTH                = 36300,
    SPELL_CAVE_IN               = 36240,
    SPELL_GROUND_SLAM           = 33525,                    //AoE Ground Slam applying Ground Slam to everyone with a script effect (most likely the knock back, we can code it to a set knockback)
    SPELL_REVERBERATION         = 36297,
    SPELL_SHATTER               = 33654,

    SPELL_SHATTER_EFFECT        = 33671,
    SPELL_HURTFUL_STRIKE        = 33813,
    SPELL_STONED                = 33652,                    //Spell is self cast by target

    SPELL_MAGNETIC_PULL         = 28337,
    SPELL_KNOCK_BACK            = 24199,                    //Knockback spell until correct implementation is made
};

class boss_gruul : public CreatureScript
{
public:
    boss_gruul() : CreatureScript("boss_gruul") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_gruulAI(pCreature);
    }

    struct boss_gruulAI : public ScriptedAI
    {
        boss_gruulAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;

        uint32 m_uiGrowth_Timer;
        uint32 m_uiCaveIn_Timer;
        uint32 m_uiCaveIn_StaticTimer;
        uint32 m_uiGroundSlamTimer;
        uint32 m_uiHurtfulStrike_Timer;
        uint32 m_uiReverberation_Timer;

        bool m_bPerformingGroundSlam;

        void Reset()
        {
            m_uiGrowth_Timer= 30000;
            m_uiCaveIn_Timer= 27000;
            m_uiCaveIn_StaticTimer = 30000;
            m_uiGroundSlamTimer= 35000;
            m_bPerformingGroundSlam= false;
            m_uiHurtfulStrike_Timer= 8000;
            m_uiReverberation_Timer= 60000+45000;

            if(pInstance)
                pInstance->SetData(DATA_GRUULEVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);

            if(pInstance)
                pInstance->SetData(DATA_GRUULEVENT, IN_PROGRESS);
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(RAND(SAY_SLAY1, SAY_SLAY2, SAY_SLAY3), me);
        }

        void JustDied(Unit* /*pKiller*/)
        {
            DoScriptText(SAY_DEATH, me);

            if(pInstance)
            {
                pInstance->SetData(DATA_GRUULEVENT, DONE);
                pInstance->HandleGameObject(pInstance->GetData64(DATA_GRUULDOOR), true);         // Open the encounter door
            }
        }

        void SpellHitTarget(Unit* target, const SpellEntry* pSpell)
        {
            //This to emulate effect1 (77) of SPELL_GROUND_SLAM, knock back to any direction
            //It's initially wrong, since this will cause fall damage, which is by comments, not intended.
            if(pSpell->Id == SPELL_GROUND_SLAM)
            {
                if(target->GetTypeId() == TYPEID_PLAYER)
                {
                    switch(urand(0, 1))
                    {
                        case 0: target->CastSpell(target, SPELL_MAGNETIC_PULL, true, NULL, NULL, me->GetGUID()); break;
                        case 1: target->CastSpell(target, SPELL_KNOCK_BACK, true, NULL, NULL, me->GetGUID()); break;
                    }
                }
            }

            //this part should be in the core
            if(pSpell->Id == SPELL_SHATTER)
            {
                //this spell must have custom handling in the core, dealing damage based on distance
                target->CastSpell(target, SPELL_SHATTER_EFFECT, true);

                if(target->HasAura(SPELL_STONED))
                    target->RemoveAurasDueToSpell(SPELL_STONED);

                //clear this, if we are still performing
                if(m_bPerformingGroundSlam)
                {
                    m_bPerformingGroundSlam = false;

                    //and correct movement, if not already
                    if(me->GetMotionMaster()->GetCurrentMovementGeneratorType() != CHASE_MOTION_TYPE)
                    {
                        if(me->getVictim())
                            me->GetMotionMaster()->MoveChase(me->getVictim());
                    }
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if(!UpdateVictim())
                return;

            // Growth
            // Gruul can cast this spell up to 30 times
            if(m_uiGrowth_Timer <= diff)
            {
                DoScriptText(EMOTE_GROW, me);
                DoCast(me, SPELL_GROWTH);
                m_uiGrowth_Timer = 30000;
            }
            else
                m_uiGrowth_Timer -= diff;

            if(m_bPerformingGroundSlam)
            {
                if(m_uiGroundSlamTimer <= diff)
                {
                    m_uiGroundSlamTimer =120000;
                    m_uiHurtfulStrike_Timer= 8000;

                    if(m_uiReverberation_Timer < 10000)     //Give a little time to the players to undo the damage from shatter
                        m_uiReverberation_Timer += 10000;

                    DoCast(me, SPELL_SHATTER);
                }
                else
                    m_uiGroundSlamTimer -= diff;
            }
            else
            {
                // Hurtful Strike
                if(m_uiHurtfulStrike_Timer <= diff)
                {
                    Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 1);

                    if(target && me->IsWithinMeleeRange(me->getVictim()))
                        DoCast(target, SPELL_HURTFUL_STRIKE);
                    else
                        DoCast(me->getVictim(), SPELL_HURTFUL_STRIKE);

                    m_uiHurtfulStrike_Timer= 8000;
                }
                else
                    m_uiHurtfulStrike_Timer -= diff;

                // Reverberation
                if(m_uiReverberation_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_REVERBERATION, true);
                    m_uiReverberation_Timer = 15000 + rand()%10000;
                }
                else
                    m_uiReverberation_Timer -= diff;

                // Cave In
                if(m_uiCaveIn_Timer <= diff)
                {
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_CAVE_IN);

                    if(m_uiCaveIn_StaticTimer >= 4000)
                        m_uiCaveIn_StaticTimer -= 2000;

                        m_uiCaveIn_Timer = m_uiCaveIn_StaticTimer;
                }
                else
                    m_uiCaveIn_Timer -= diff;

                // Ground Slam, Gronn Lord's Grasp, Stoned, Shatter
                if(m_uiGroundSlamTimer <= diff)
                {
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveIdle();

                    m_bPerformingGroundSlam= true;
                    m_uiGroundSlamTimer = 10000;

                    DoCast(me, SPELL_GROUND_SLAM);
                }
                else
                    m_uiGroundSlamTimer -= diff;

                DoMeleeAttackIfReady();
            }
        }
    };
};

void AddSC_boss_gruul()
{
    new boss_gruul;
}

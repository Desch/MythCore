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
#include "gundrak.h"

enum eSpells
{
    // troll form
    SPELL_DETERMINED_STAB                         = 55104,
    SPELL_GROUND_TREMOR                           = 55142,
    SPELL_NUMBING_SHOUT                           = 55106,
    SPELL_MOJO_FRENZY                             = 55163,
    SPELL_MOJO_FRENZY_HASTE                       = 55096,
    SPELL_TRANSFORMATION                          = 55098, //Periodic, The caster transforms into a powerful mammoth, increasing Physical damage done by 25% and granting immunity to Stun effects.
    // mammoth
    SPELL_DETERMINED_GORE                         = 55102,
    H_SPELL_DETERMINED_GORE                       = 59444,
    SPELL_QUAKE                                   = 55101,
    SPELL_NUMBING_ROAR                            = 55100
};

enum eAchievements
{
    ACHIEV_LESS_RABI                              = 2040
};

enum eSays
{
    SAY_AGGRO                                     = -1604011,
    SAY_QUAKE                                     = -1604012,
    SAY_TRANSFORM                                 = -1604013,
    SAY_SLAY_1                                    = -1604014,
    SAY_SLAY_2                                    = -1604015,
    SAY_SLAY_3                                    = -1604016,
    SAY_DEATH                                     = -1604017,
    EMOTE_TRANSFORM                               = -1604018,
    EMOTE_TRANSFORMED                             = -1604029,
};

class boss_moorabi : public CreatureScript
{
public:
    boss_moorabi() : CreatureScript("boss_moorabi") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_moorabiAI(pCreature);
    }

    struct boss_moorabiAI : public ScriptedAI
    {
        boss_moorabiAI(Creature* pCreature): ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;

        bool bPhase;

        uint32 uiNumblingShoutTimer;
        uint32 uiGroundTremorTimer;
        uint32 uiDeterminedStabTimer;
        uint32 uiTransformationTimer;

        void Reset()
        {
            uiGroundTremorTimer = 18*IN_MILLISECONDS;
            uiNumblingShoutTimer =  10*IN_MILLISECONDS;
            uiDeterminedStabTimer = 20*IN_MILLISECONDS;
            uiTransformationTimer = 12*IN_MILLISECONDS;

            bPhase = false;

            if(pInstance)
                pInstance->SetData(DATA_MOORABI_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);
            DoCast(me, SPELL_MOJO_FRENZY, true);

            if(pInstance)
                pInstance->SetData(DATA_MOORABI_EVENT, IN_PROGRESS);
        }

        void AdjustCastSpeed() { }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
             if(!UpdateVictim())
                 return;

            if(!bPhase && me->HasAura(SPELL_TRANSFORMATION))
            {
                bPhase = true;
                me->RemoveAura(SPELL_MOJO_FRENZY);
                DoScriptText(EMOTE_TRANSFORMED, me);
            }

            if(uiGroundTremorTimer <= diff)
            {
                DoScriptText(SAY_QUAKE, me);
                if(bPhase)
                    DoCast(me->getVictim(), SPELL_QUAKE);
                else
                    DoCast(me->getVictim(), SPELL_GROUND_TREMOR);
                uiGroundTremorTimer = urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS);
            } else uiGroundTremorTimer -= diff;

            if(uiNumblingShoutTimer <= diff)
            {
                if(bPhase)
                    DoCast(me->getVictim(), SPELL_NUMBING_ROAR, true);
                else
                    DoCast(me->getVictim(), SPELL_NUMBING_SHOUT, true);
                uiNumblingShoutTimer = 10*IN_MILLISECONDS;
            } else uiNumblingShoutTimer -=diff;

            if(uiDeterminedStabTimer <= diff)
            {
                if(bPhase)
                    DoCast(me->getVictim(), DUNGEON_MODE(SPELL_DETERMINED_GORE, H_SPELL_DETERMINED_GORE), true);
                else
                    DoCast(me->getVictim(), SPELL_DETERMINED_STAB, true);
                uiDeterminedStabTimer = 7*IN_MILLISECONDS;
            } else uiDeterminedStabTimer -=diff;

            if(!bPhase && uiTransformationTimer <= diff)
            {
                DoScriptText(EMOTE_TRANSFORM, me);
                DoScriptText(SAY_TRANSFORM, me);
                AdjustCastSpeed(); //FIXME
                DoCast(me, SPELL_TRANSFORMATION, false);
                uiTransformationTimer = 10*IN_MILLISECONDS;
            } else uiTransformationTimer -= diff;

            DoMeleeAttackIfReady();
         }

         void JustDied(Unit* /*pKiller*/)
         {
            DoScriptText(SAY_DEATH, me);

            if(pInstance)
            {
                pInstance->SetData(DATA_MOORABI_EVENT, DONE);

                if(IsHeroic() && !bPhase)
                    pInstance->DoCompleteAchievement(ACHIEV_LESS_RABI);
            }
        }

        void KilledUnit(Unit* victim)
        {
            if(victim == me)
                return;

            DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2,SAY_SLAY_3), me);
        }
    };
};

#define DATA_LESS_RABI 1

class achievement_less_rabi : public AchievementCriteriaScript
{
    public:
        achievement_less_rabi() : AchievementCriteriaScript("achievement_less_rabi") { }

        bool OnCheck(Player* /*pPlayer*/, Unit* target)
        {
            if(!target)
                return false;

            if(Creature* Moorabi = target->ToCreature())
                if(Moorabi->AI()->GetData(DATA_LESS_RABI))
                    return true;

            return false;
        }
};

void AddSC_boss_moorabi()
{
    new boss_moorabi;
    new achievement_less_rabi;
}

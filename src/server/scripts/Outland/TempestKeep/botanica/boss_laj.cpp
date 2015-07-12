/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Copyright (C) 2012 SymphonyArt <http://symphonyart.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#include "ScriptPCH.h"

enum eSpells
{
    SPELL_ALLERGIC_REACTION    = 34697,
    SPELL_TELEPORT_SELF        = 34673,

    SPELL_SUMMON_LASHER_1      = 34681,
    SPELL_SUMMON_FLAYER_1      = 34682,
    SPELL_SUMMON_LASHER_2      = 34684,
    SPELL_SUMMON_FLAYER_2      = 34685,
    SPELL_SUMMON_LASHER_3      = 34686,
    SPELL_SUMMON_FLAYER_4      = 34687,
    SPELL_SUMMON_LASHER_4      = 34688,
    SPELL_SUMMON_FLAYER_3      = 34690,
};

enum eOthers
{
    EMOTE_SUMMON               = -1553006,
    MODEL_DEFAULT              = 13109,
    MODEL_ARCANE               = 14213,
    MODEL_FIRE                 = 13110,
    MODEL_FROST                = 14112,
    MODEL_NATURE               = 14214,
};

class boss_laj : public CreatureScript
{
    public:
        boss_laj() : CreatureScript("boss_laj") { }

        struct boss_lajAI : public ScriptedAI
        {
            boss_lajAI(Creature* pCreature): ScriptedAI(pCreature) { }

            bool CanSummon;
            uint32 Teleport_Timer;
            uint32 Summon_Timer;
            uint32 Transform_Timer;
            uint32 Allergic_Timer;

            void Reset()
            {
                me->SetDisplayId(MODEL_DEFAULT);
                me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_SHADOW, true);
                me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_ARCANE, false);
                me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, false);
                me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_NATURE, false);

                CanSummon = false;
                Teleport_Timer = 20000;
                Summon_Timer = 2500;
                Transform_Timer = 30000;
                Allergic_Timer = 5000;
            }

            void DoTransform()
            {
                switch(rand()%5)
                {
                    case 0:
                        me->SetDisplayId(MODEL_DEFAULT);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_SHADOW, true);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_ARCANE, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_NATURE, false);
                        break;
                    case 1:
                        me->SetDisplayId(MODEL_ARCANE);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_SHADOW, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_ARCANE, true);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_NATURE, false);
                        break;
                    case 2:
                        me->SetDisplayId(MODEL_FIRE);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_SHADOW, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_ARCANE, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, true);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_NATURE, false);
                        break;
                    case 3:
                        me->SetDisplayId(MODEL_FROST);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_SHADOW, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_ARCANE, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_NATURE, false);
                        break;
                    case 4:
                        me->SetDisplayId(MODEL_NATURE);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_SHADOW, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_ARCANE, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                        me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_NATURE, true);
                        break;
                }
            }

            void DoSummons()
            {
                switch(rand()%4)
                {
                    case 0:
                        DoCast(me, SPELL_SUMMON_LASHER_1, true);
                        DoCast(me, SPELL_SUMMON_FLAYER_1, true);
                        break;
                    case 1:
                        DoCast(me, SPELL_SUMMON_LASHER_2, true);
                        DoCast(me, SPELL_SUMMON_FLAYER_2, true);
                        break;
                    case 2:
                        DoCast(me, SPELL_SUMMON_LASHER_3, true);
                        DoCast(me, SPELL_SUMMON_FLAYER_3, true);
                        break;
                    case 3:
                        DoCast(me, SPELL_SUMMON_LASHER_4, true);
                        DoCast(me, SPELL_SUMMON_FLAYER_4, true);
                        break;
                }
                CanSummon = false;
            }

            void EnterCombat(Unit* /*pWho*/) { }

            void JustSummoned(Creature* summon)
            {
                if(summon && me->getVictim())
                    summon->AI()->AttackStart(SelectTarget(SELECT_TARGET_RANDOM, 0));
            }

            void UpdateAI(const uint32 diff)
            {
                if(!UpdateVictim())
                    return;

                if(CanSummon)
                {
                    if(Summon_Timer <= diff)
                    {
                        DoScriptText(EMOTE_SUMMON, me);
                        DoSummons();
                        Summon_Timer = 2500;
                    }
                    else
                        Summon_Timer -= diff;
                }

                if(Allergic_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_ALLERGIC_REACTION);
                    Allergic_Timer = 25000+rand()%15000;
                }
                else
                    Allergic_Timer -= diff;

                if(Teleport_Timer <= diff)
                {
                    DoCast(me, SPELL_TELEPORT_SELF);
                    Teleport_Timer = 30000+rand()%10000;
                    CanSummon = true;
                }
                else
                    Teleport_Timer -= diff;

                if(Transform_Timer <= diff)
                {
                    DoTransform();
                    Transform_Timer = 25000+rand()%15000;
                }
                else
                    Transform_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* Creature) const
        {
            return new boss_lajAI (Creature);
        }
};

void AddSC_boss_laj()
{
    new boss_laj;
}


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

#define SAY_AGGRO                       -1532057
#define SAY_SUMMON1                     -1532058
#define SAY_SUMMON2                     -1532059
#define SAY_EVOCATE                     -1532060
#define SAY_ENRAGE                      -1532061
#define SAY_KILL1                       -1532062
#define SAY_KILL2                       -1532063
#define SAY_DEATH                       -1532064

//Flare spell info
#define SPELL_ASTRAL_FLARE_PASSIVE      30234               //Visual effect + Flare damage

//Curator spell info
#define SPELL_HATEFUL_BOLT              30383
#define SPELL_EVOCATION                 30254
#define SPELL_ENRAGE                    30403               //Arcane Infusion: Transforms Curator and adds damage.
#define SPELL_BERSERK                   26662

class boss_curator : public CreatureScript
{
public:
    boss_curator() : CreatureScript("boss_curator") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_curatorAI(pCreature);
    }

    struct boss_curatorAI : public ScriptedAI
    {
        boss_curatorAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 AddTimer;
        uint32 HatefulBoltTimer;
        uint32 BerserkTimer;

        bool Enraged;
        bool Evocating;

        void Reset()
        {
            AddTimer = 10000;
            HatefulBoltTimer = 15000;                           //This time may be wrong
            BerserkTimer = 720000;                              //12 minutes
            Enraged = false;
            Evocating = false;

            me->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ARCANE, true);
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(RAND(SAY_KILL1, SAY_KILL2), me);
        }

        void JustDied(Unit* /*pVictim*/)
        {
            DoScriptText(SAY_DEATH, me);
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            //always decrease BerserkTimer
            if(BerserkTimer <= diff)
            {
                //if evocate, then break evocate
                if(Evocating)
                {
                    if(me->HasAura(SPELL_EVOCATION))
                        me->RemoveAurasDueToSpell(SPELL_EVOCATION);

                    Evocating = false;
                }

                //may not be correct SAY (generic hard enrage)
                DoScriptText(SAY_ENRAGE, me);

                me->InterruptNonMeleeSpells(true);
                DoCast(me, SPELL_BERSERK);

                //don't know if he's supposed to do summon/evocate after hard enrage (probably not)
                Enraged = true;
            } else BerserkTimer -= diff;

            if(Evocating)
            {
                //not supposed to do anything while evocate
                if(me->HasAura(SPELL_EVOCATION))
                    return;
                else
                    Evocating = false;
            }

            if(!Enraged)
            {
                if(AddTimer <= diff)
                {
                    //Summon Astral Flare
                    Creature* AstralFlare = DoSpawnCreature(17096, float(rand()%37), float(rand()%37), 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                    Unit* target = NULL;
                    target = SelectTarget(SELECT_TARGET_RANDOM, 0);

                    if(AstralFlare && target)
                    {
                        AstralFlare->CastSpell(AstralFlare, SPELL_ASTRAL_FLARE_PASSIVE, false);
                        AstralFlare->AI()->AttackStart(target);
                    }

                    //Reduce Mana by 10% of max health
                    if(int32 mana = me->GetMaxPower(POWER_MANA))
                    {
                        mana /= 10;
                        me->ModifyPower(POWER_MANA, -mana);

                        //if this get's us below 10%, then we evocate (the 10th should be summoned now)
                        if(me->GetPower(POWER_MANA)*100 / me->GetMaxPower(POWER_MANA) < 10)
                        {
                            DoScriptText(SAY_EVOCATE, me);
                            me->InterruptNonMeleeSpells(false);
                            DoCast(me, SPELL_EVOCATION);
                            Evocating = true;
                            //no AddTimer cooldown, this will make first flare appear instantly after evocate end, like expected
                            return;
                        }
                        else
                        {
                            if(urand(0, 1) == 0)
                            {
                                DoScriptText(RAND(SAY_SUMMON1, SAY_SUMMON2), me);
                            }
                        }
                    }

                    AddTimer = 10000;
                } else AddTimer -= diff;

                if(!HealthAbovePct(15))
                {
                    Enraged = true;
                    DoCast(me, SPELL_ENRAGE);
                    DoScriptText(SAY_ENRAGE, me);
                }
            }

            if(HatefulBoltTimer <= diff)
            {
                if(Enraged)
                    HatefulBoltTimer = 7000;
                else
                    HatefulBoltTimer = 15000;

                if(Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 1))
                    DoCast(target, SPELL_HATEFUL_BOLT);

            } else HatefulBoltTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_curator()
{
    new boss_curator;
}

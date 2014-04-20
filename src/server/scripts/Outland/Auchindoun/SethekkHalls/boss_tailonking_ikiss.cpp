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
#include "sethekk_halls.h"

#define SAY_INTRO                   -1556007

#define SAY_AGGRO_1                 -1556008
#define SAY_AGGRO_2                 -1556009
#define SAY_AGGRO_3                 -1556010

#define SAY_SLAY_1                  -1556011
#define SAY_SLAY_2                  -1556012
#define SAY_DEATH                   -1556013
#define EMOTE_ARCANE_EXP            -1556015

#define SPELL_BLINK                 38194
#define SPELL_BLINK_TELEPORT        38203
#define SPELL_MANA_SHIELD           38151
#define SPELL_ARCANE_BUBBLE         9438
#define H_SPELL_SLOW                35032

#define SPELL_POLYMORPH             38245
#define H_SPELL_POLYMORPH           43309

#define SPELL_ARCANE_VOLLEY         35059
#define H_SPELL_ARCANE_VOLLEY       40424

#define SPELL_ARCANE_EXPLOSION      38197
#define H_SPELL_ARCANE_EXPLOSION    40425

class boss_talon_king_ikiss : public CreatureScript
{
public:
    boss_talon_king_ikiss() : CreatureScript("boss_talon_king_ikiss") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_talon_king_ikissAI(pCreature);
    }

    struct boss_talon_king_ikissAI : public ScriptedAI
    {
        boss_talon_king_ikissAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;

        uint32 ArcaneVolley_Timer;
        uint32 Sheep_Timer;
        uint32 Blink_Timer;
        uint32 Slow_Timer;

        bool ManaShield;
        bool Blink;
        bool Intro;

        void Reset()
        {
            ArcaneVolley_Timer = 5000;
            Sheep_Timer = 8000;
            Blink_Timer = 35000;
            Slow_Timer = 15000+rand()%15000;
            Blink = false;
            Intro = false;
            ManaShield = false;
        }

        void MoveInLineOfSight(Unit* who)
        {
            if(!me->getVictim() && who->isTargetableForAttack() && (me->IsHostileTo(who)) && who->isInAccessiblePlaceFor(me))
            {
                if(!Intro && me->IsWithinDistInMap(who, 100))
                {
                    Intro = true;
                    DoScriptText(SAY_INTRO, me);
                }

                if(!me->canFly() && me->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                    return;

                float attackRadius = me->GetAttackDistance(who);
                if(me->IsWithinDistInMap(who, attackRadius) && me->IsWithinLOSInMap(who))
                {
                    //who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                    AttackStart(who);
                }
            }
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(RAND(SAY_AGGRO_1, SAY_AGGRO_2, SAY_AGGRO_3), me);
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(!pInstance)
                return;

            DoScriptText(SAY_DEATH, me);
            pInstance->SetData(DATA_IKISSDOOREVENT, DONE);

            if(IsHolidayActive(HOLIDAY_PILGRIMS_BOUNTY))
            {
                Map::PlayerList const &PlayerList = pInstance->instance->GetPlayers();

                if(!PlayerList.isEmpty())
                {
                    for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    {
                        if(Player* pPlayer = i->getSource())
                        {
                            if(pPlayer->HasAura(66303) && pPlayer->HasAura(66304))
                            {
                                if(AchievementEntry const* pAchievement = GetAchievementStore()->LookupEntry(3582))
                                    pPlayer->CompletedAchievement(pAchievement);
                            }
                        }
                    }
                }
            }
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(Blink)
            {
                DoCast(me, SPELL_ARCANE_EXPLOSION);
                DoCast(me, SPELL_ARCANE_BUBBLE, true);
                Blink = false;
            }

            if(ArcaneVolley_Timer <= diff)
            {
                DoCast(me, SPELL_ARCANE_VOLLEY);
                ArcaneVolley_Timer = 7000+rand()%5000;
            } else ArcaneVolley_Timer -= diff;

            if(Sheep_Timer <= diff)
            {
                Unit* target;

                //second top aggro target in normal, random target in heroic correct?
                if(IsHeroic())
                    target = SelectTarget(SELECT_TARGET_RANDOM, 0);
                else
                    target = SelectTarget(SELECT_TARGET_TOPAGGRO, 1);

                if(target)
                    DoCast(target, SPELL_POLYMORPH);
                Sheep_Timer = 15000+rand()%2500;
            } else Sheep_Timer -= diff;

            //may not be correct time to cast
            if(!ManaShield && HealthBelowPct(20))
            {
                DoCast(me, SPELL_MANA_SHIELD);
                ManaShield = true;
            }

            if(IsHeroic())
            {
                if(Slow_Timer <= diff)
                {
                    DoCast(me, H_SPELL_SLOW);
                    Slow_Timer = 15000+rand()%25000;
                } else Slow_Timer -= diff;
            }

            if(Blink_Timer <= diff)
            {
                DoScriptText(EMOTE_ARCANE_EXP, me);

                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    if(me->IsNonMeleeSpellCasted(false))
                        me->InterruptNonMeleeSpells(false);

                    //Spell doesn't work, but we use for visual effect at least
                    DoCast(target, SPELL_BLINK);

                    float X = target->GetPositionX();
                    float Y = target->GetPositionY();
                    float Z = target->GetPositionZ();

                    DoTeleportTo(X, Y, Z);

                    DoCast(target, SPELL_BLINK_TELEPORT);
                    Blink = true;
                }
                Blink_Timer = 35000+rand()%5000;
            } else Blink_Timer -= diff;

            if(!Blink)
                DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_talon_king_ikiss()
{
    new boss_talon_king_ikiss;
}

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

class at_twilight_grove : public AreaTriggerScript
{
public:
    at_twilight_grove() : AreaTriggerScript("at_twilight_grove") { }

    bool OnTrigger(Player* player, const AreaTriggerEntry * /*at*/)
    {
        if(player->HasQuestForItem(21149))
        {
            if(Unit* TCorrupter = player->SummonCreature(15625, -10328.16f, -489.57f, 49.95f, 0, TEMPSUMMON_MANUAL_DESPAWN, 60000))
            {
                TCorrupter->setFaction(14);
                TCorrupter->SetMaxHealth(832750);
            }
            if(Unit* CorrupterSpeaker = player->SummonCreature(1, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ()-1, 0, TEMPSUMMON_TIMED_DESPAWN, 15000))
            {
                CorrupterSpeaker->SetName("Twilight Corrupter");
                CorrupterSpeaker->SetVisible(true);
                CorrupterSpeaker->MonsterYell("Come, $N. See what the Nightmare brings...", 0, player->GetGUID());
            }
        }
        return false;
    };
};

#define SPELL_SOUL_CORRUPTION 25805
#define SPELL_CREATURE_OF_NIGHTMARE 25806
#define SPELL_LEVEL_UP 24312

class boss_twilight_corrupter : public CreatureScript
{
public:
    boss_twilight_corrupter() : CreatureScript("boss_twilight_corrupter") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_twilight_corrupterAI(pCreature);
    }

    struct boss_twilight_corrupterAI : public ScriptedAI
    {
        boss_twilight_corrupterAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 SoulCorruption_Timer;
        uint32 CreatureOfNightmare_Timer;
        uint8 KillCount;

        void Reset()
        {
            SoulCorruption_Timer = 15000;
            CreatureOfNightmare_Timer = 30000;
            KillCount = 0;
        }
        void EnterCombat(Unit* /*pWho*/)
        {
            me->MonsterYell("The Nightmare cannot be stopped!", 0, me->GetGUID());
        }

        void KilledUnit(Unit* victim)
        {
            if(victim->GetTypeId() == TYPEID_PLAYER)
            {
                ++KillCount;
                me->MonsterTextEmote("Twilight Corrupter squeezes the last bit of life out of $N and swallows their soul.", victim->GetGUID(), true);

                if(KillCount == 3)
                {
                    DoCast(me, SPELL_LEVEL_UP, true);
                    KillCount = 0;
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;
            if(SoulCorruption_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SOUL_CORRUPTION);
                SoulCorruption_Timer = rand()%4000+15000; //gotta confirm Timers
            } else SoulCorruption_Timer-=diff;

            if(CreatureOfNightmare_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CREATURE_OF_NIGHTMARE);
                CreatureOfNightmare_Timer = 45000; //gotta confirm Timers
            } else CreatureOfNightmare_Timer-=diff;
            DoMeleeAttackIfReady();
        };
    };
};

void AddSC_duskwood()
{
    new boss_twilight_corrupter;
    new at_twilight_grove;
}
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

enum Sylvanas
{
    QUEST_JOURNEY_TO_UNDERCITY  = 9180,
    SAY_LAMENT_END              = -1000196,
    EMOTE_LAMENT_END            = -1000197,

    SOUND_CREDIT                = 10896,
    ENTRY_HIGHBORNE_LAMENTER    = 21628,
    ENTRY_HIGHBORNE_BUNNY       = 21641,

    SPELL_HIGHBORNE_AURA        = 37090,
    SPELL_SYLVANAS_CAST         = 36568,
    SPELL_RIBBON_OF_SOULS       = 34432,                   //the real one to use might be 37099
};

float HighborneLoc[4][3]=
{
    {1285.41f, 312.47f, 0.51f},
    {1286.96f, 310.40f, 1.00f},
    {1289.66f, 309.66f, 1.52f},
    {1292.51f, 310.50f, 1.99f},
};

#define HIGHBORNE_LOC_Y             -61.00f
#define HIGHBORNE_LOC_Y_NEW         -55.50f

class npc_lady_sylvanas_windrunner : public CreatureScript
{
public:
    npc_lady_sylvanas_windrunner() : CreatureScript("npc_lady_sylvanas_windrunner") { }

    bool OnQuestReward(Player* /*pPlayer*/, Creature* creature, const Quest *_Quest, uint32 /*slot*/)
    {
        if(_Quest->GetQuestId() == QUEST_JOURNEY_TO_UNDERCITY)
        {
            CAST_AI(npc_lady_sylvanas_windrunner::npc_lady_sylvanas_windrunnerAI, creature->AI())->LamentEvent = true;
            CAST_AI(npc_lady_sylvanas_windrunner::npc_lady_sylvanas_windrunnerAI, creature->AI())->DoPlaySoundToSet(creature, SOUND_CREDIT);
            creature->CastSpell(creature, SPELL_SYLVANAS_CAST, false);

            for(uint8 i = 0; i < 4; ++i)
                creature->SummonCreature(ENTRY_HIGHBORNE_LAMENTER, HighborneLoc[i][0], HighborneLoc[i][1], HIGHBORNE_LOC_Y, HighborneLoc[i][2], TEMPSUMMON_TIMED_DESPAWN, 160000);
        }

        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_lady_sylvanas_windrunnerAI(pCreature);
    }

    struct npc_lady_sylvanas_windrunnerAI : public ScriptedAI
    {
        npc_lady_sylvanas_windrunnerAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 LamentEvent_Timer;
        bool LamentEvent;
        uint64 targetGUID;

        void Reset()
        {
            LamentEvent_Timer = 5000;
            LamentEvent = false;
            targetGUID = 0;
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void JustSummoned(Creature* summoned)
        {
            if(summoned->GetEntry() == ENTRY_HIGHBORNE_BUNNY)
            {
                if(Unit* target = Unit::GetUnit(*summoned, targetGUID))
                {
                    target->MonsterMoveWithSpeed(target->GetPositionX(), target->GetPositionY(), me->GetPositionZ()+15.0f, 0);
                    target->GetMap()->CreatureRelocation(me, target->GetPositionX(), target->GetPositionY(), me->GetPositionZ()+15.0f, 0.0f);
                    summoned->CastSpell(target, SPELL_RIBBON_OF_SOULS, false);
                }

                summoned->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
                targetGUID = summoned->GetGUID();
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(LamentEvent)
            {
                if(LamentEvent_Timer <= diff)
                {
                    DoSummon(ENTRY_HIGHBORNE_BUNNY, me, 10.0f, 3000, TEMPSUMMON_TIMED_DESPAWN);

                    LamentEvent_Timer = 2000;
                    if(!me->HasAura(SPELL_SYLVANAS_CAST))
                    {
                        DoScriptText(SAY_LAMENT_END, me);
                        DoScriptText(EMOTE_LAMENT_END, me);
                        LamentEvent = false;
                    }
                } else LamentEvent_Timer -= diff;
            }

            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

class npc_highborne_lamenter : public CreatureScript
{
public:
    npc_highborne_lamenter() : CreatureScript("npc_highborne_lamenter") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_highborne_lamenterAI(pCreature);
    }

    struct npc_highborne_lamenterAI : public ScriptedAI
    {
        npc_highborne_lamenterAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 EventMove_Timer;
        uint32 EventCast_Timer;
        bool EventMove;
        bool EventCast;

        void Reset()
        {
            EventMove_Timer = 10000;
            EventCast_Timer = 17500;
            EventMove = true;
            EventCast = true;
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void UpdateAI(const uint32 diff)
        {
            if(EventMove)
            {
                if(EventMove_Timer <= diff)
                {
                    me->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
                    me->MonsterMoveWithSpeed(me->GetPositionX(), me->GetPositionY(), HIGHBORNE_LOC_Y_NEW, me->GetDistance(me->GetPositionX(), me->GetPositionY(), HIGHBORNE_LOC_Y_NEW) / (5000 * 0.001f));
                    me->GetMap()->CreatureRelocation(me, me->GetPositionX(), me->GetPositionY(), HIGHBORNE_LOC_Y_NEW, me->GetOrientation());
                    EventMove = false;
                } else EventMove_Timer -= diff;
            }
            if(EventCast)
            {
                if(EventCast_Timer <= diff)
                {
                    DoCast(me, SPELL_HIGHBORNE_AURA);
                    EventCast = false;
                } else EventCast_Timer -= diff;
            }
        }
    };
};

#define SPELL_MARK_OF_SHAME 6767

#define GOSSIP_HPF1 "Gul'dan"
#define GOSSIP_HPF2 "Kel'Thuzad"
#define GOSSIP_HPF3 "Ner'zhul"

class npc_parqual_fintallas : public CreatureScript
{
public:
    npc_parqual_fintallas() : CreatureScript("npc_parqual_fintallas") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        if(uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            player->CLOSE_GOSSIP_MENU();
            creature->CastSpell(player, SPELL_MARK_OF_SHAME, false);
        }
        if(uiAction == GOSSIP_ACTION_INFO_DEF+2)
        {
            player->CLOSE_GOSSIP_MENU();
            player->AreaExploredOrEventHappens(6628);
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if(player->GetQuestStatus(6628) == QUEST_STATUS_INCOMPLETE && !player->HasAura(SPELL_MARK_OF_SHAME))
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HPF1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HPF2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HPF3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->SEND_GOSSIP_MENU(5822, creature->GetGUID());
        }
        else
            player->SEND_GOSSIP_MENU(5821, creature->GetGUID());

        return true;
    }
};

void AddSC_undercity()
{
    new npc_lady_sylvanas_windrunner;
    new npc_highborne_lamenter;
    new npc_parqual_fintallas;
}
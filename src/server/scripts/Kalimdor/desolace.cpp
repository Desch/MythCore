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
#include "ScriptedEscortAI.h"

enum eDyingKodo
{
    // signed for 9999
    SAY_SMEED_HOME_1                = -1000348,
    SAY_SMEED_HOME_2                = -1000349,
    SAY_SMEED_HOME_3                = -1000350,

    QUEST_KODO                      = 5561,

    NPC_SMEED                       = 11596,
    NPC_AGED_KODO                   = 4700,
    NPC_DYING_KODO                  = 4701,
    NPC_ANCIENT_KODO                = 4702,
    NPC_TAMED_KODO                  = 11627,

    SPELL_KODO_KOMBO_ITEM           = 18153,
    SPELL_KODO_KOMBO_PLAYER_BUFF    = 18172,                //spells here have unclear function, but using them at least for visual parts and checks
    SPELL_KODO_KOMBO_DESPAWN_BUFF   = 18377,
    SPELL_KODO_KOMBO_GOSSIP         = 18362
};

class npc_aged_dying_ancient_kodo : public CreatureScript
{
public:
    npc_aged_dying_ancient_kodo() : CreatureScript("npc_aged_dying_ancient_kodo") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(player->HasAura(SPELL_KODO_KOMBO_PLAYER_BUFF) && creature->HasAura(SPELL_KODO_KOMBO_DESPAWN_BUFF))
        {
            //the expected quest objective
            player->TalkedToCreature(creature->GetEntry(), creature->GetGUID());

            player->RemoveAurasDueToSpell(SPELL_KODO_KOMBO_PLAYER_BUFF);

            creature->GetMotionMaster()->MoveIdle();
            creature->RemoveAllAuras();
            creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
            creature->CastSpell(creature, SPELL_KODO_KOMBO_GOSSIP, true);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;

        }
        else return false;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_aged_dying_ancient_kodoAI(pCreature);
    }

    struct npc_aged_dying_ancient_kodoAI : public ScriptedAI
    {
        npc_aged_dying_ancient_kodoAI(Creature* pCreature): ScriptedAI(pCreature) { Reset(); }

        uint32 m_uiDespawnTimer;
        uint32 m_uiTimeoutTimer;
        bool m_uiDespawn;
        bool m_uiTimeout;

        void Reset()
        {
            m_uiDespawnTimer = 60000;
            m_uiTimeoutTimer = 300000;
            m_uiDespawn = false;
            m_uiTimeout = false;
        }

        void MoveInLineOfSight(Unit* who)
        {
            if(who->GetEntry() == NPC_SMEED)
            {
                if(me->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
                    return;

                if(me->IsWithinDistInMap(who, 10.0f))
                {
                    DoScriptText(RAND(SAY_SMEED_HOME_1, SAY_SMEED_HOME_2, SAY_SMEED_HOME_3), who);
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }
        }

        void SpellHit(Unit* pCaster, SpellEntry const* pSpell)
        {
            if(pSpell->Id == SPELL_KODO_KOMBO_GOSSIP)
            {
                m_uiDespawn = true;
                m_uiTimeout = false;
            }
            else if(pSpell->Id == SPELL_KODO_KOMBO_ITEM)
            {
                if(!pCaster->HasAura(SPELL_KODO_KOMBO_PLAYER_BUFF) && !me->HasAura(SPELL_KODO_KOMBO_DESPAWN_BUFF))
                {
                    pCaster->CastSpell(pCaster, SPELL_KODO_KOMBO_PLAYER_BUFF, true);

                    me->UpdateEntry(NPC_TAMED_KODO);
                    me->CastSpell(me, SPELL_KODO_KOMBO_DESPAWN_BUFF, false);
                    m_uiTimeout = true;
                    me->CombatStop(true);
                    me->DeleteThreatList();
                    me->GetMotionMaster()->MoveFollow(pCaster, PET_FOLLOW_DIST, me->GetFollowAngle());
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(m_uiDespawn == true && m_uiDespawnTimer <= diff)
            {
                Reset();
                me->ForcedDespawn(0);
                return;
            } else if(m_uiDespawn == true) m_uiDespawnTimer -= diff;

            if(m_uiTimeout == true && m_uiTimeoutTimer <= diff)
            {
                Reset();
                me->ForcedDespawn(0);
                return;
            }
            else if(m_uiTimeout == true) m_uiTimeoutTimer -= diff;

            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

class go_iruxos : public GameObjectScript
{
public:
    go_iruxos() : GameObjectScript("go_iruxos") { }

    bool OnGossipHello(Player* player, GameObject* /*pGo*/)
    {
            if(player->GetQuestStatus(5381) == QUEST_STATUS_INCOMPLETE)
                player->SummonCreature(11876, player->GetInnPosX(), player->GetInnPosY(), player->GetInnPosZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);

            return true;
    }
};

#define QUEST_RETURN_TO_VAHLARRIEL     1440

class npc_dalinda : public CreatureScript
{
public:
    npc_dalinda() : CreatureScript("npc_dalinda") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if(quest->GetQuestId() == QUEST_RETURN_TO_VAHLARRIEL)
       {
            if(npc_escortAI* pEscortAI = CAST_AI(npc_dalinda::npc_dalindaAI, creature->AI()))
            {
                pEscortAI->Start(true, false, player->GetGUID());
                creature->setFaction(113);
            }
        }
        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_dalindaAI(pCreature);
    }

    struct npc_dalindaAI : public npc_escortAI
    {
        npc_dalindaAI(Creature* pCreature) : npc_escortAI(pCreature) { }

        void WaypointReached(uint32 i)
        {
            Player* player = GetPlayerForEscort();
            switch(i)
            {
                case 1:
                    me->IsStandState();
                    break;
                case 15:
                    if(player)
                    player->GroupEventHappens(QUEST_RETURN_TO_VAHLARRIEL, me);
                    break;
            }
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void Reset() { }

        void JustDied(Unit* /*pKiller*/)
        {
            Player* player = GetPlayerForEscort();
            if(player)
                player->FailQuest(QUEST_RETURN_TO_VAHLARRIEL);
            return;
        }

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);
            if(!UpdateVictim())
                return;
            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_desolace()
{
    new npc_aged_dying_ancient_kodo;
    new go_iruxos;
    new npc_dalinda;
}

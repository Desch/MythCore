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

enum eCalvin
{
    SAY_COMPLETE        = -1000431,
    SPELL_DRINK         = 2639,                             // possibly not correct spell (but iconId is correct)
    QUEST_590           = 590,
    FACTION_HOSTILE     = 168
};

class npc_calvin_montague : public CreatureScript
{
public:
    npc_calvin_montague() : CreatureScript("npc_calvin_montague") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* pQuest)
    {
        if(pQuest->GetQuestId() == QUEST_590)
        {
            creature->setFaction(FACTION_HOSTILE);
            creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            CAST_AI(npc_calvin_montague::npc_calvin_montagueAI, creature->AI())->AttackStart(player);
        }
        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_calvin_montagueAI(pCreature);
    }

    struct npc_calvin_montagueAI : public ScriptedAI
    {
        npc_calvin_montagueAI(Creature* pCreature): ScriptedAI(pCreature) { }

        uint32 m_uiPhase;
        uint32 m_uiPhaseTimer;
        uint64 m_uiPlayerGUID;

        void Reset()
        {
            m_uiPhase = 0;
            m_uiPhaseTimer = 5000;
            m_uiPlayerGUID = 0;

            me->RestoreFaction();

            if(!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE))
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void AttackedBy(Unit* pAttacker)
        {
            if(me->getVictim() || me->IsFriendlyTo(pAttacker))
                return;

            AttackStart(pAttacker);
        }

        void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
        {
            if(uiDamage > me->GetHealth() || me->HealthBelowPctDamaged(15, uiDamage))
            {
                uiDamage = 0;

                me->RestoreFaction();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                me->CombatStop(true);

                m_uiPhase = 1;

                if(pDoneBy->GetTypeId() == TYPEID_PLAYER)
                    m_uiPlayerGUID = pDoneBy->GetGUID();
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(m_uiPhase)
            {
                if(m_uiPhaseTimer <= diff)
                    m_uiPhaseTimer = 7500;
                else
                {
                    m_uiPhaseTimer -= diff;
                    return;
                }

                switch(m_uiPhase)
                {
                    case 1:
                        DoScriptText(SAY_COMPLETE, me);
                        ++m_uiPhase;
                        break;
                    case 2:
                        if(Player* player = Unit::GetPlayer(*me, m_uiPlayerGUID))
                            player->AreaExploredOrEventHappens(QUEST_590);

                        DoCast(me, SPELL_DRINK, true);
                        ++m_uiPhase;
                        break;
                    case 3:
                        EnterEvadeMode();
                        break;
                }

                return;
            }

            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

enum eMausoleum
{
    QUEST_ULAG      = 1819,
    NPC_ULAG        = 6390,
    GO_TRIGGER      = 104593,
    GO_DOOR         = 176594
};

class go_mausoleum_door : public GameObjectScript
{
public:
    go_mausoleum_door() : GameObjectScript("go_mausoleum_door") { }

    bool OnGossipHello(Player* player, GameObject* /*pGo*/)
    {
        if(player->GetQuestStatus(QUEST_ULAG) != QUEST_STATUS_INCOMPLETE)
            return false;

        if(GameObject* pTrigger = player->FindNearestGameObject(GO_TRIGGER, 30.0f))
        {
            pTrigger->SetGoState(GO_STATE_READY);
            player->SummonCreature(NPC_ULAG, 2390.26f, 336.47f, 40.01f, 2.26f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
            return false;
        }

        return false;
    }
};

class go_mausoleum_trigger : public GameObjectScript
{
public:
    go_mausoleum_trigger() : GameObjectScript("go_mausoleum_trigger") { }

    bool OnGossipHello(Player* player, GameObject* pGo)
    {
        if(player->GetQuestStatus(QUEST_ULAG) != QUEST_STATUS_INCOMPLETE)
            return false;

        if(GameObject* pDoor = player->FindNearestGameObject(GO_DOOR, 30.0f))
        {
            pGo->SetGoState(GO_STATE_ACTIVE);
            pDoor->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
            return true;
        }

        return false;
    }
};

void AddSC_tirisfal_glades()
{
    new npc_calvin_montague;
    new go_mausoleum_door;
    new go_mausoleum_trigger;
}
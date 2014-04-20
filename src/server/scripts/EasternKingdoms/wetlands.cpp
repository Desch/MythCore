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

enum eTapokeSlim
{
    QUEST_MISSING_DIPLO_PT11    = 1249,
    FACTION_ENEMY               = 168,
    SPELL_STEALTH               = 1785,
    SPELL_CALL_FRIENDS          = 16457,
    NPC_SLIMS_FRIEND            = 4971,
    NPC_TAPOKE_SLIM_JAHN        = 4962
};

class npc_tapoke_slim_jahn : public CreatureScript
{
public:
    npc_tapoke_slim_jahn() : CreatureScript("npc_tapoke_slim_jahn") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_tapoke_slim_jahnAI(pCreature);
    }

    struct npc_tapoke_slim_jahnAI : public npc_escortAI
    {
        npc_tapoke_slim_jahnAI(Creature* pCreature) : npc_escortAI(pCreature) { }

        bool m_bFriendSummoned;

        void Reset()
        {
            if(!HasEscortState(STATE_ESCORT_ESCORTING))
                m_bFriendSummoned = false;
        }

        void WaypointReached(uint32 uiPointId)
        {
            switch(uiPointId)
            {
                case 2:
                    if(me->HasStealthAura())
                        me->RemoveAurasByType(SPELL_AURA_MOD_STEALTH);

                    SetRun();
                    me->setFaction(FACTION_ENEMY);
                    break;
            }
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            Player* player = GetPlayerForEscort();

            if(HasEscortState(STATE_ESCORT_ESCORTING) && !m_bFriendSummoned && player)
            {
                for(uint8 i = 0; i < 3; ++i)
                    DoCast(me, SPELL_CALL_FRIENDS, true);

                m_bFriendSummoned = true;
            }
        }

        void JustSummoned(Creature* summoned)
        {
            if(Player* player = GetPlayerForEscort())
                summoned->AI()->AttackStart(player);
        }

        void AttackedBy(Unit* pAttacker)
        {
            if(me->getVictim())
                return;

            if(me->IsFriendlyTo(pAttacker))
                return;

            AttackStart(pAttacker);
        }

        void DamageTaken(Unit* /*pDoneBy*/, uint32& uiDamage)
        {
            if(HealthBelowPct(20))
            {
                if(Player* player = GetPlayerForEscort())
                {
                    if(player->GetTypeId() == TYPEID_PLAYER)
                        CAST_PLR(player)->GroupEventHappens(QUEST_MISSING_DIPLO_PT11, me);

                    uiDamage = 0;

                    me->RestoreFaction();
                    me->RemoveAllAuras();
                    me->DeleteThreatList();
                    me->CombatStop(true);

                    SetRun(false);
                }
            }
        }
    };
};

class npc_mikhail : public CreatureScript
{
public:
    npc_mikhail() : CreatureScript("npc_mikhail") { }

    bool OnQuestAccept(Player* player, Creature* creature, const Quest* pQuest)
    {
        if(pQuest->GetQuestId() == QUEST_MISSING_DIPLO_PT11)
        {
            Creature* pSlim = creature->FindNearestCreature(NPC_TAPOKE_SLIM_JAHN, 25.0f);

            if(!pSlim)
                return false;

            if(!pSlim->HasStealthAura())
                pSlim->CastSpell(pSlim, SPELL_STEALTH, true);

            if(npc_tapoke_slim_jahn::npc_tapoke_slim_jahnAI* pEscortAI = CAST_AI(npc_tapoke_slim_jahn::npc_tapoke_slim_jahnAI, pSlim->AI()))
                pEscortAI->Start(false, false, player->GetGUID(), pQuest);
        }
        return false;
    }
};

void AddSC_wetlands()
{
    new npc_tapoke_slim_jahn;
    new npc_mikhail;
}
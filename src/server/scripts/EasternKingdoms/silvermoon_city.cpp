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

enum eStillbladeData
{
    SAY_HEAL                    = -1000193,

    QUEST_REDEEMING_THE_DEAD    = 9685,
    SPELL_SHIMMERING_VESSEL     = 31225,
    SPELL_REVIVE_SELF           = 32343,
};

class npc_blood_knight_stillblade : public CreatureScript
{
public:
    npc_blood_knight_stillblade() : CreatureScript("npc_blood_knight_stillblade") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_blood_knight_stillbladeAI(pCreature);
    }

    struct npc_blood_knight_stillbladeAI : public ScriptedAI
    {
        npc_blood_knight_stillbladeAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 lifeTimer;
        bool spellHit;

        void Reset()
        {
            lifeTimer = 120000;
            me->SetStandState(UNIT_STAND_STATE_DEAD);
            me->SetUInt32Value(UNIT_FIELD_BYTES_1, 7);   // lay down
            spellHit = false;
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void MoveInLineOfSight(Unit* /*pWho*/) { }

        void UpdateAI(const uint32 diff)
        {
            if(me->IsStandState())
            {
                if(lifeTimer <= diff)
                    me->AI()->EnterEvadeMode();
                else
                    lifeTimer -= diff;
            }
        }

        void SpellHit(Unit* pHitter, const SpellEntry* pSpell)
        {
            if((pSpell->Id == SPELL_SHIMMERING_VESSEL) && !spellHit &&
                (pHitter->GetTypeId() == TYPEID_PLAYER) && (CAST_PLR(pHitter)->IsActiveQuest(QUEST_REDEEMING_THE_DEAD)))
            {
                CAST_PLR(pHitter)->AreaExploredOrEventHappens(QUEST_REDEEMING_THE_DEAD);
                DoCast(me, SPELL_REVIVE_SELF);
                me->SetStandState(UNIT_STAND_STATE_STAND);
                me->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
                DoScriptText(SAY_HEAL, me);
                spellHit = true;
            }
        }
    };
};

void AddSC_silvermoon_city()
{
    new npc_blood_knight_stillblade;
}
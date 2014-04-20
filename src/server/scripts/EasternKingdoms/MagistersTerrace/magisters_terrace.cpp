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

enum eEnums
{
    SPELL_TRANSFORM_TO_KAEL     = 44670,
    SPELL_ORB_KILL_CREDIT       = 46307,
    NPC_KAEL                    = 24848,                    //human form entry
    POINT_ID_LAND               = 1
};

const float afKaelLandPoint[] = {225.045f, -276.236f, -5.434f};

#define GOSSIP_ITEM_KAEL_1      "Who are you?"
#define GOSSIP_ITEM_KAEL_2      "What can we do to assist you?"
#define GOSSIP_ITEM_KAEL_3      "What brings you to the Sunwell?"
#define GOSSIP_ITEM_KAEL_4      "You're not alone here?"
#define GOSSIP_ITEM_KAEL_5      "What would Kil'jaeden want with a mortal woman?"

// This is friendly keal that appear after used Orb.
// If we assume DB handle summon, summon appear somewhere outside the platform where Orb is
class npc_kalecgos : public CreatureScript
{
public:
    npc_kalecgos() : CreatureScript("npc_kalecgos") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KAEL_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(12500, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KAEL_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                player->SEND_GOSSIP_MENU(12502, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KAEL_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                player->SEND_GOSSIP_MENU(12606, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KAEL_5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                player->SEND_GOSSIP_MENU(12607, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                player->SEND_GOSSIP_MENU(12608, creature->GetGUID());
                break;
        }

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_KAEL_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        player->SEND_GOSSIP_MENU(12498, creature->GetGUID());

        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_kalecgosAI(pCreature);
    }

    struct npc_kalecgosAI : public ScriptedAI
    {
        npc_kalecgosAI(Creature* pCreature): ScriptedAI(pCreature) { }

        uint32 m_uiTransformTimer;

        void Reset()
        {
            m_uiTransformTimer = 0;

            // we must assume he appear as dragon somewhere outside the platform of orb, and then move directly to here
            if(me->GetEntry() != NPC_KAEL)
                me->GetMotionMaster()->MovePoint(POINT_ID_LAND, afKaelLandPoint[0], afKaelLandPoint[1], afKaelLandPoint[2]);
        }

        void MovementInform(uint32 uiType, uint32 uiPointId)
        {
            if(uiType != POINT_MOTION_TYPE)
                return;

            if(uiPointId == POINT_ID_LAND)
                m_uiTransformTimer = MINUTE*IN_MILLISECONDS;
        }

        // some targeting issues with the spell, so use this workaround as temporary solution
        void DoWorkaroundForQuestCredit()
        {
            Map* pMap = me->GetMap();

            if(!pMap || pMap->IsHeroic())
                return;

            Map::PlayerList const &lList = pMap->GetPlayers();

            if(lList.isEmpty())
                return;

            SpellEntry const* pSpell = GetSpellStore()->LookupEntry(SPELL_ORB_KILL_CREDIT);

            for(Map::PlayerList::const_iterator i = lList.begin(); i != lList.end(); ++i)
            {
                if(Player* player = i->getSource())
                {
                    if(pSpell && pSpell->EffectMiscValue[0])
                        player->KilledMonsterCredit(pSpell->EffectMiscValue[0], 0);
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(m_uiTransformTimer)
            {
                if(m_uiTransformTimer <= diff)
                {
                    DoCast(me, SPELL_ORB_KILL_CREDIT, false);
                    DoWorkaroundForQuestCredit();

                    // Transform and update entry, now ready for quest/read gossip
                    DoCast(me, SPELL_TRANSFORM_TO_KAEL, false);
                    me->UpdateEntry(NPC_KAEL);

                    m_uiTransformTimer = 0;
                } else m_uiTransformTimer -= diff;
            }
        }
    };
};

void AddSC_magisters_terrace()
{
    new npc_kalecgos;
}

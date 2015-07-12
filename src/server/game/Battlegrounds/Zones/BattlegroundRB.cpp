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

#include "Player.h"
#include "Battleground.h"
#include "BattlegroundRB.h"
#include "Language.h"

BattlegroundRB::BattlegroundRB()
{
    //TODO FIX ME!
    m_StartMessageIds[BG_STARTING_EVENT_FIRST]  = 0;
    m_StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_WS_START_ONE_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_WS_START_HALF_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_WS_HAS_BEGUN;
}

BattlegroundRB::~BattlegroundRB() { }

void BattlegroundRB::Update(uint32 diff)
{
    Battleground::Update(diff);
}

void BattlegroundRB::StartingEventCloseDoors() { }

void BattlegroundRB::StartingEventOpenDoors() { }

void BattlegroundRB::AddPlayer(Player* plr)
{
    Battleground::AddPlayer(plr);
    //create score and add it to map, default values are set in constructor
    BattlegroundRBScore* sc = new BattlegroundRBScore;

    m_PlayerScores[plr->GetGUID()] = sc;
}

void BattlegroundRB::RemovePlayer(Player* /*pPlayer*/, uint64 /*guid*/, uint32 /*team*/) { }

void BattlegroundRB::HandleAreaTrigger(Player* /*pSource*/, uint32 /*Trigger*/)
{
    // this is wrong way to implement these things. On official it done by gameobject spell cast.
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;
}

void BattlegroundRB::UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor)
{
    std::map<uint64, BattlegroundScore*>::iterator itr = m_PlayerScores.find(Source->GetGUID());

    if(itr == m_PlayerScores.end())                         // player not found...
        return;

    Battleground::UpdatePlayerScore(Source, type, value, doAddHonor);
}

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

#include "Battleground.h"
#include "BattlegroundRL.h"
#include "Language.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "WorldPacket.h"

BattlegroundRL::BattlegroundRL()
{
    BGArrayForObjects.resize(BG_RL_OBJECT_MAX);

    m_StartDelayTimes[BG_STARTING_EVENT_FIRST]  = BG_START_DELAY_1M;
    m_StartDelayTimes[BG_STARTING_EVENT_SECOND] = BG_START_DELAY_30S;
    m_StartDelayTimes[BG_STARTING_EVENT_THIRD]  = BG_START_DELAY_15S;
    m_StartDelayTimes[BG_STARTING_EVENT_FOURTH] = BG_START_DELAY_NONE;
    //we must set messageIds
    m_StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_ARENA_ONE_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_ARENA_THIRTY_SECONDS;
    m_StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_ARENA_FIFTEEN_SECONDS;
    m_StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_ARENA_HAS_BEGUN;
}

BattlegroundRL::~BattlegroundRL() { }

void BattlegroundRL::Update(uint32 diff)
{
    Battleground::Update(diff);

    if(GetStatus() == STATUS_IN_PROGRESS)
    {
        if(GetStartTime() >= 47*MINUTE*IN_MILLISECONDS)    // after 47 minutes without one team losing, the arena closes with no winner and no rating change
        {
            UpdateArenaWorldState();
            CheckArenaAfterTimerConditions();
        }

        if(m_uiTeleport < diff)
        {
            for(BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            {
                Player*  plr = sObjectMgr->GetPlayer(itr->first);
                if(plr && plr->GetPositionZ() < 30)
                    plr->TeleportTo(plr->GetMapId(), plr->GetPositionX(), plr->GetPositionY(), 40, plr->GetOrientation(), false);
                if(plr && plr->GetPositionZ() < 30)
                    plr->TeleportTo(plr->GetMapId(), plr->GetPositionX(), plr->GetPositionY(), 40, plr->GetOrientation(), false);
            }
            m_uiTeleport = 1000;
        } else m_uiTeleport -= diff;
    }
}

void BattlegroundRL::StartingEventCloseDoors()
{
    for(uint32 i = BG_RL_OBJECT_DOOR_1; i <= BG_RL_OBJECT_DOOR_2; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
}

void BattlegroundRL::StartingEventOpenDoors()
{
    for(uint32 i = BG_RL_OBJECT_DOOR_1; i <= BG_RL_OBJECT_DOOR_2; ++i)
        DoorOpen(i);

    for(uint32 i = BG_RL_OBJECT_BUFF_1; i <= BG_RL_OBJECT_BUFF_2; ++i)
        SpawnBGObject(i, 90);
}

void BattlegroundRL::AddPlayer(Player* plr)
{
    Battleground::AddPlayer(plr);
    //create score and add it to map, default values are set in constructor
    BattlegroundRLScore* sc = new BattlegroundRLScore;

    m_PlayerScores[plr->GetGUID()] = sc;

    UpdateArenaWorldState();
}

void BattlegroundRL::RemovePlayer(Player* /*pPlayer*/, uint64 /*guid*/, uint32 /*team*/)
{
    if(GetStatus() == STATUS_WAIT_LEAVE)
        return;

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void BattlegroundRL::HandleKillPlayer(Player* player, Player* killer)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    if(!killer)
    {
        sLog->outError("Killer player not found");
        return;
    }

    Battleground::HandleKillPlayer(player, killer);

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

bool BattlegroundRL::HandlePlayerUnderMap(Player* player)
{
    player->TeleportTo(GetMapId(), 1285.810547f, 1667.896851f, 39.957642f, player->GetOrientation(), false);
    return true;
}

void BattlegroundRL::HandleAreaTrigger(Player* /*pSource*/, uint32 Trigger)
{
    // this is wrong way to implement these things. On official it done by gameobject spell cast.
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    switch(Trigger)
    {
        case 4696:                                          // buff trigger?
        case 4697:                                          // buff trigger?
            break;
        default:
            sLog->outError("WARNING: Unhandled AreaTrigger in Battleground: %u", Trigger);
            break;
    }
}

void BattlegroundRL::FillInitialWorldStates(WorldPacket &data)
{
    data << uint32(0xbba) << uint32(1);           // 9
    UpdateArenaWorldState();
}

void BattlegroundRL::Reset()
{
    //call parent's reset
    Battleground::Reset();
    m_uiTeleport = 22000;
}

bool BattlegroundRL::SetupBattleground()
{
    // gates
    if(!AddObject(BG_RL_OBJECT_DOOR_1, BG_RL_OBJECT_TYPE_DOOR_1, 1293.561f, 1601.938f, 31.60557f, -1.457349f, 0, 0, -0.6658813f, 0.7460576f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RL_OBJECT_DOOR_2, BG_RL_OBJECT_TYPE_DOOR_2, 1278.648f, 1730.557f, 31.60557f, 1.684245f, 0, 0, 0.7460582f, 0.6658807f, RESPAWN_IMMEDIATELY)
    // buffs
        || !AddObject(BG_RL_OBJECT_BUFF_1, BG_RL_OBJECT_TYPE_BUFF_1, 1328.719971f, 1632.719971f, 36.730400f, -1.448624f, 0, 0, 0.6626201f, -0.7489557f, 120)
        || !AddObject(BG_RL_OBJECT_BUFF_2, BG_RL_OBJECT_TYPE_BUFF_2, 1243.300049f, 1699.170044f, 34.872601f, -0.06981307f, 0, 0, 0.03489945f, -0.9993908f, 120))
    {
        sLog->outErrorDb("BatteGroundRL: Failed to spawn some object!");
        return false;
    }

    return true;
}
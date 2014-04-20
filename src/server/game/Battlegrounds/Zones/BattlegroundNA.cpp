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
#include "BattlegroundNA.h"
#include "Language.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "WorldPacket.h"

BattlegroundNA::BattlegroundNA()
{
    BGArrayForObjects.resize(BG_NA_OBJECT_MAX);

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

BattlegroundNA::~BattlegroundNA() { }

void BattlegroundNA::Update(uint32 diff)
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
                Player* pPlayer = sObjectMgr->GetPlayer(itr->first);
                if(pPlayer && pPlayer->GetPositionZ() < 10)
                    pPlayer->TeleportTo(pPlayer->GetMapId(), 4056.513f, 2921.693f, 13.932f, pPlayer->GetOrientation(), false);
            }
            m_uiTeleport = 1000;
        } else m_uiTeleport -= diff;
    }
}

void BattlegroundNA::StartingEventCloseDoors()
{
    for(uint32 i = BG_NA_OBJECT_DOOR_1; i <= BG_NA_OBJECT_DOOR_4; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
}

void BattlegroundNA::StartingEventOpenDoors()
{
    for(uint32 i = BG_NA_OBJECT_DOOR_1; i <= BG_NA_OBJECT_DOOR_2; ++i)
        DoorOpen(i);

    for(uint32 i = BG_NA_OBJECT_BUFF_1; i <= BG_NA_OBJECT_BUFF_2; ++i)
        SpawnBGObject(i, 90);
}

void BattlegroundNA::AddPlayer(Player* plr)
{
    Battleground::AddPlayer(plr);
    //create score and add it to map, default values are set in constructor
    BattlegroundNAScore* sc = new BattlegroundNAScore;

    m_PlayerScores[plr->GetGUID()] = sc;

    UpdateArenaWorldState();
}

void BattlegroundNA::RemovePlayer(Player* /*pPlayer*/, uint64 /*guid*/, uint32 /*team*/)
{
    if(GetStatus() == STATUS_WAIT_LEAVE)
        return;

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void BattlegroundNA::HandleKillPlayer(Player* player, Player* killer)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    if(!killer)
    {
        sLog->outError("BattlegroundNA: Killer player not found");
        return;
    }

    Battleground::HandleKillPlayer(player, killer);

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

bool BattlegroundNA::HandlePlayerUnderMap(Player* player)
{
    player->TeleportTo(GetMapId(), 4055.504395f, 2919.660645f, 13.611241f, player->GetOrientation(), false);
    return true;
}

void BattlegroundNA::HandleAreaTrigger(Player* Source, uint32 Trigger)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    switch(Trigger)
    {
        case 4536:                                          // buff trigger?
        case 4537:                                          // buff trigger?
            break;
        default:
            sLog->outError("WARNING: Unhandled AreaTrigger in Battleground: %u", Trigger);
            Source->GetSession()->SendAreaTriggerMessage("Warning: Unhandled AreaTrigger in Battleground: %u", Trigger);
            break;
    }
}

void BattlegroundNA::FillInitialWorldStates(WorldPacket &data)
{
    data << uint32(0xa11) << uint32(1);           // 9
    UpdateArenaWorldState();
}

void BattlegroundNA::Reset()
{
    //call parent's class reset
    Battleground::Reset();
    m_uiTeleport = 22000;
}

bool BattlegroundNA::SetupBattleground()
{
    // gates
    if(!AddObject(BG_NA_OBJECT_DOOR_1, BG_NA_OBJECT_TYPE_DOOR_1, 4031.854f, 2966.833f, 12.6462f, -2.648788f, 0, 0, 0.9697962f, -0.2439165f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_NA_OBJECT_DOOR_2, BG_NA_OBJECT_TYPE_DOOR_2, 4081.179f, 2874.97f, 12.39171f, 0.4928045f, 0, 0, 0.2439165f, 0.9697962f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_NA_OBJECT_DOOR_3, BG_NA_OBJECT_TYPE_DOOR_3, 4023.709f, 2981.777f, 10.70117f, -2.648788f, 0, 0, 0.9697962f, -0.2439165f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_NA_OBJECT_DOOR_4, BG_NA_OBJECT_TYPE_DOOR_4, 4090.064f, 2858.438f, 10.23631f, 0.4928045f, 0, 0, 0.2439165f, 0.9697962f, RESPAWN_IMMEDIATELY)
    // buffs
        || !AddObject(BG_NA_OBJECT_BUFF_1, BG_NA_OBJECT_TYPE_BUFF_1, 4009.189941f, 2895.250000f, 13.052700f, -1.448624f, 0, 0, 0.6626201f, -0.7489557f, 120)
        || !AddObject(BG_NA_OBJECT_BUFF_2, BG_NA_OBJECT_TYPE_BUFF_2, 4103.330078f, 2946.350098f, 13.051300f, -0.06981307f, 0, 0, 0.03489945f, -0.9993908f, 120))
    {
        sLog->outErrorDb("BatteGroundNA: Failed to spawn some object!");
        return false;
    }

    return true;
}

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
#include "BattlegroundDS.h"
#include "Language.h"
#include "Player.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"

BattlegroundDS::BattlegroundDS()
{
    BGArrayForObjects.resize(BG_DS_OBJECT_MAX);

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

BattlegroundDS::~BattlegroundDS() { }

void BattlegroundDS::Update(uint32 diff)
{
    Battleground::Update(diff);

    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    if(GetStatus() == STATUS_IN_PROGRESS)
    {
        if(GetStartTime() >= 47*MINUTE*IN_MILLISECONDS)    // after 47 minutes without one team losing, the arena closes with no winner and no rating change
        {
            UpdateArenaWorldState();
            CheckArenaAfterTimerConditions();
        }

        if(GetStartTime() >= 75*IN_MILLISECONDS)
        {
            for(BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end();itr++)
            {
                if(Player* pPlayer = sObjectMgr->GetPlayer(itr->first))
                {
                    if(pPlayer->isAlive())
                    { 
                        if(pPlayer->GetPositionX() < 1260 && pPlayer->GetPositionY() >755 && pPlayer->GetPositionY() < 775 && pPlayer->GetPositionZ() > 13)
                        {
                            KnockBackPlayer(pPlayer, 6.15f, 50.00f, 5.00f);
                            pPlayer->RemoveAurasDueToSpell(48018);
                        } 
                        else if(pPlayer->GetPositionX() > 1330 && pPlayer->GetPositionY() >805 && pPlayer->GetPositionY() < 825 && pPlayer->GetPositionZ() > 13)
                        {
                            KnockBackPlayer(pPlayer, 3.10f, 50.00f, 5.00f);
                            pPlayer->RemoveAurasDueToSpell(48018);
                        } 
                    }
                }
            }
        }
    }

    if(getPipeKnockBackCount() < BG_DS_PIPE_KNOCKBACK_TOTAL_COUNT)
    {
        if(getPipeKnockBackTimer() < diff)
        {
            for(uint32 i = BG_DS_NPC_PIPE_KNOCKBACK_1; i <= BG_DS_NPC_PIPE_KNOCKBACK_2; ++i)
                if(Creature* waterSpout = GetBgMap()->GetCreature(BGArrayForCreatures[i]))
                    waterSpout->CastSpell(waterSpout, BG_DS_SPELL_FLUSH, true);

            setPipeKnockBackCount(getPipeKnockBackCount() + 1);
            setPipeKnockBackTimer(BG_DS_PIPE_KNOCKBACK_DELAY);
        }
        else
            setPipeKnockBackTimer(getPipeKnockBackTimer() - diff);
    }

    if(getWaterFallTimer() < diff)
    {
        switch(getWaterFallStatus())
        {
        case BG_DS_WATERFALL_STATUS_OFF: // Add the water
            DoorClose(BG_DS_OBJECT_WATER_2);
            setWaterFallTimer(BG_DS_WATERFALL_WARNING_DURATION);
            setWaterFallStatus(BG_DS_WATERFALL_STATUS_WARNING);
            break;
        case BG_DS_WATERFALL_STATUS_WARNING: // Active collision and start knockback timer
            if(GameObject* gob = GetBgMap()->GetGameObject(BGArrayForObjects[BG_DS_OBJECT_WATER_1]))
                gob->SetGoState(GO_STATE_READY);

            setWaterFallTimer(BG_DS_WATERFALL_DURATION);
            setWaterFallStatus(BG_DS_WATERFALL_STATUS_ON);
            setWaterFallKnockbackTimer(BG_DS_WATERFALL_KNOCKBACK_TIMER);
            break;
        case BG_DS_WATERFALL_STATUS_ON: // Remove collision and water
            // turn off collision
            if(GameObject* gob = GetBgMap()->GetGameObject(BGArrayForObjects[BG_DS_OBJECT_WATER_1]))
                gob->SetGoState(GO_STATE_ACTIVE);

            DoorOpen(BG_DS_OBJECT_WATER_2);
            setWaterFallTimer(urand(BG_DS_WATERFALL_TIMER_MIN, BG_DS_WATERFALL_TIMER_MAX));
            setWaterFallStatus(BG_DS_WATERFALL_STATUS_OFF);
            break;
        }
    }
    else
        setWaterFallTimer(getWaterFallTimer() - diff);
}

void BattlegroundDS::StartingEventCloseDoors()
{
    for(uint32 i = BG_DS_OBJECT_DOOR_1; i <= BG_DS_OBJECT_DOOR_2; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
}

void BattlegroundDS::StartingEventOpenDoors()
{
    for(uint32 i = BG_DS_OBJECT_DOOR_1; i <= BG_DS_OBJECT_DOOR_2; ++i)
        DoorOpen(i);

    for(uint32 i = BG_DS_OBJECT_BUFF_1; i <= BG_DS_OBJECT_BUFF_2; ++i)
        SpawnBGObject(i, 60);

    SpawnBGObject(BG_DS_OBJECT_WATER_1, RESPAWN_ONE_DAY);
    SpawnBGObject(BG_DS_OBJECT_WATER_2, RESPAWN_IMMEDIATELY);
    DoorOpen(BG_DS_OBJECT_WATER_2);

    setWaterFallTimer(urand(BG_DS_WATERFALL_TIMER_MIN, BG_DS_WATERFALL_TIMER_MAX));
    setWaterFallStatus(BG_DS_WATERFALL_STATUS_OFF);

    setPipeKnockBackTimer(BG_DS_PIPE_KNOCKBACK_FIRST_DELAY);
    setPipeKnockBackCount(0);
}

void BattlegroundDS::AddPlayer(Player* plr)
{
    Battleground::AddPlayer(plr);
    //create score and add it to map, default values are set in constructor
    BattlegroundDSScore* sc = new BattlegroundDSScore;

    m_PlayerScores[plr->GetGUID()] = sc;

    UpdateArenaWorldState();
}

void BattlegroundDS::RemovePlayer(Player* /*pPlayer*/, uint64 /*guid*/, uint32 /*team*/)
{
    if(GetStatus() == STATUS_WAIT_LEAVE)
        return;

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void BattlegroundDS::HandleKillPlayer(Player* pVictim, Player* pKiller)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    if(!pKiller)
    {
        sLog->outError("BattlegroundDS: Killer player not found");
        return;
    }

    Battleground::HandleKillPlayer(pVictim, pKiller);

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void BattlegroundDS::HandleAreaTrigger(Player* Source, uint32 uiTrigger)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    switch(uiTrigger)
    {
        case 5347:
        case 5348:
            // Remove effects of Demonic Circle Summon
            if(Source->HasAura(48018))
                Source->RemoveAurasDueToSpell(48018);
            // Someone has get back into the pipes and the knockback has already been performed,
            // so we reset the knockback count for kicking the player again into the arena.
            if(getPipeKnockBackCount() >= BG_DS_PIPE_KNOCKBACK_TOTAL_COUNT)
                setPipeKnockBackCount(0);
            break;
        default:
            sLog->outError("WARNING: Unhandled AreaTrigger in Battleground: %u", uiTrigger);
            Source->GetSession()->SendAreaTriggerMessage("Warning: Unhandled AreaTrigger in Battleground: %u", uiTrigger);
            break;
    }
}

bool BattlegroundDS::HandlePlayerUnderMap(Player* player)
{
    player->TeleportTo(GetMapId(), 1299.046f, 784.825f, 9.338f, 2.422f, false);
    return true;
}

void BattlegroundDS::FillInitialWorldStates(WorldPacket &data)
{
    data << uint32(3610) << uint32(1);                                              // 9 show
    UpdateArenaWorldState();
}

void BattlegroundDS::Reset()
{
    //call parent's class reset
    Battleground::Reset();
}

bool BattlegroundDS::SetupBattleground()
{
    BGArrayForObjects.resize(BG_DS_OBJECT_MAX);
    BGArrayForCreatures.resize(BG_DS_NPC_MAX);
    // DS: Gates
    if(!AddObject(BG_DS_OBJECT_DOOR_1, BG_DS_OBJECT_TYPE_DOOR_1, 1350.95f, 817.2f, 20.8096f, 3.15f, 0, 0, 0.99627f, 0.0862864f, RESPAWN_IMMEDIATELY)
    || !AddObject(BG_DS_OBJECT_DOOR_2, BG_DS_OBJECT_TYPE_DOOR_2, 1232.65f, 764.913f, 20.0729f, 6.3f, 0, 0, 0.0310211f, -0.999519f, RESPAWN_IMMEDIATELY)
    // DS: Waterr
    || !AddObject(BG_DS_OBJECT_WATER_1, BG_DS_OBJECT_TYPE_WATER_1, 1291.56f, 790.837f, 7.1f, 3.14238f, 0, 0, 0.694215f, -0.719768f, 120)
    || !AddObject(BG_DS_OBJECT_WATER_2, BG_DS_OBJECT_TYPE_WATER_2, 1291.56f, 790.837f, 7.1f, 3.14238f, 0, 0, 0.694215f, -0.719768f, 120)
    // DS: Buffs
    || !AddObject(BG_DS_OBJECT_BUFF_1, BG_DS_OBJECT_TYPE_BUFF_1, 1291.7f, 813.424f, 7.11472f, 4.64562f, 0, 0, 0.730314f, -0.683111f, 120)
    || !AddObject(BG_DS_OBJECT_BUFF_2, BG_DS_OBJECT_TYPE_BUFF_2, 1291.7f, 768.911f, 7.11472f, 1.55194f, 0, 0, 0.700409f, 0.713742f, 120)
    // DS: Knockback NPC
    || !AddCreature(BG_DS_NPC_TYPE_WATER_SPOUT, BG_DS_NPC_WATERFALL_KNOCKBACK, 0, 1292.587f, 790.2205f, 7.19796f, 3.054326f, RESPAWN_IMMEDIATELY)
    || !AddCreature(BG_DS_NPC_TYPE_WATER_SPOUT, BG_DS_NPC_PIPE_KNOCKBACK_1, 0, 1369.977f, 817.2882f, 16.08718f, 3.106686f, RESPAWN_IMMEDIATELY)
    || !AddCreature(BG_DS_NPC_TYPE_WATER_SPOUT, BG_DS_NPC_PIPE_KNOCKBACK_2, 0, 1212.833f, 765.3871f, 16.09484f, 0.0f, RESPAWN_IMMEDIATELY))
    {
        sLog->outErrorDb("BatteGroundDS: Failed to spawn some object!");
        return false;
    }

    return true;
}

void BattlegroundDS::KnockBackPlayer(Unit* pPlayer, float angle, float horizontalSpeed, float verticalSpeed)
{
    if(pPlayer->GetTypeId() == TYPEID_PLAYER)
    {
        WorldPacket data(SMSG_MOVE_KNOCK_BACK, 8+4+4+4+4+2);
        data.append(pPlayer->GetPackGUID());
        data << uint32(0);
        data << float(cos(angle));
        data << float(sin(angle));
        data << float(horizontalSpeed);
        data << float(-verticalSpeed);
        ((Player*)pPlayer)->GetSession()->SendPacket(&data);
    }
    else
        sLog->outError("The target of KnockBackPlayer must be a player !");
}

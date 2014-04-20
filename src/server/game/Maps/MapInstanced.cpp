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

#include "MapInstanced.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "Battleground.h"
#include "VMapFactory.h"
#include "InstanceSaveMgr.h"
#include "World.h"
#include "Group.h"

MapInstanced::MapInstanced(uint32 id, time_t expiry) : Map(id, expiry, 0, DUNGEON_DIFFICULTY_NORMAL)
{
    // initialize instanced maps list
    m_InstancedMaps.clear();
    // fill with zero
    memset(&GridMapReference, 0, MAX_NUMBER_OF_GRIDS*MAX_NUMBER_OF_GRIDS*sizeof(uint16));
}

void MapInstanced::InitVisibilityDistance()
{
    if(m_InstancedMaps.empty())
        return;
    //initialize visibility distances for all instance copies
    for(InstancedMaps::iterator i = m_InstancedMaps.begin(); i != m_InstancedMaps.end(); ++i)
    {
        (*i).second->InitVisibilityDistance();
    }
}

void MapInstanced::Update(const uint32& t)
{
    // take care of loaded GridMaps (when unused, unload it!)
    Map::Update(t);

    // update the instanced maps
    InstancedMaps::iterator i = m_InstancedMaps.begin();

    while(i != m_InstancedMaps.end())
    {
        if(i->second->CanUnload(t))
            DestroyInstance(i);
        else
        {
            // update only here, because it may schedule some bad things before delete
            if(sMapMgr->GetMapUpdater()->activated())
                sMapMgr->GetMapUpdater()->schedule_update(*i->second, t);
            else
                i->second->Update(t);
            ++i;
        }
    }
}

void MapInstanced::DelayedUpdate(const uint32 diff)
{
    for(InstancedMaps::iterator i = m_InstancedMaps.begin(); i != m_InstancedMaps.end(); ++i)
        i->second->DelayedUpdate(diff);

    Map::DelayedUpdate(diff); // this may be removed
}

void MapInstanced::UnloadAll()
{
    // Unload instanced maps
    for(InstancedMaps::iterator i = m_InstancedMaps.begin(); i != m_InstancedMaps.end(); ++i)
        i->second->UnloadAll();

    // Delete the maps only after everything is unloaded to prevent crashes
    for(InstancedMaps::iterator i = m_InstancedMaps.begin(); i != m_InstancedMaps.end(); ++i)
        delete i->second;

    m_InstancedMaps.clear();

    // Unload own grids (just dummy(placeholder) grids, neccesary to unload GridMaps!)
    Map::UnloadAll();
}

/*
- return the right instance for the object, based on its InstanceId
- create the instance if it's not created already
- the player is not actually added to the instance (only in InstanceMap::Add)
*/
Map* MapInstanced::CreateInstanceForPlayer(const uint32 mapId, Player* pPlayer)
{
    if(GetId() != mapId || !pPlayer)
        return NULL;

    Map* pMap = NULL;
    uint32 NewInstanceId = 0;                       // instanceId of the resulting pMap

    if(IsBattlegroundOrArena())
    {
        // instantiate or find existing bg pMap for pPlayer
        // the instance id is set in battlegroundid
        NewInstanceId = pPlayer->GetBattlegroundId();
        if(!NewInstanceId) return NULL;
        pMap = sMapMgr->FindMap(mapId, NewInstanceId);
        if(!pMap)
        {
            if(Battleground* bg = pPlayer->GetBattleground())
                pMap = CreateBattleground(NewInstanceId, bg);
            else
            {
                pPlayer->TeleportToBGEntryPoint();
                return NULL;
            }
        }
    }
    else
    {
        InstancePlayerBind* pBind = pPlayer->GetBoundInstance(GetId(), pPlayer->GetDifficulty(IsRaid()));
        InstanceSave *pSave = pBind ? pBind->save : NULL;

        // the pPlayer's permanent pPlayer bind is taken into consideration first
        // then the pPlayer's group bind and finally the solo bind.
        if(!pBind || !pBind->perm)
        {
            InstanceGroupBind *groupBind = NULL;
            Group* group = pPlayer->GetGroup();
            // use the pPlayer's difficulty setting (it may not be the same as the group's)
            if(group)
            {
                groupBind = group->GetBoundInstance(this);
                if(groupBind)
                    pSave = groupBind->save;
            }
        }
        if(pSave)
        {
            // solo/perm/group
            NewInstanceId = pSave->GetInstanceId();
            pMap = FindInstanceMap(NewInstanceId);
            // it is possible that the save exists but the pMap doesn't
            if(!pMap)
                pMap = CreateInstance(NewInstanceId, pSave, pSave->GetDifficulty());
        }
        else
        {
            // if no instanceId via group members or instance saves is found
            // the instance will be created for the first time
            NewInstanceId = sMapMgr->GenerateInstanceId();

            Difficulty diff = pPlayer->GetGroup() ? pPlayer->GetGroup()->GetDifficulty(IsRaid()) : pPlayer->GetDifficulty(IsRaid());
            pMap = FindInstanceMap(NewInstanceId);
            if(!pMap)
               pMap = CreateInstance(NewInstanceId, NULL, diff);
        }
    }

    return pMap;
}

InstanceMap* MapInstanced::CreateInstance(uint32 InstanceId, InstanceSave* pSave, Difficulty difficulty)
{
    // load/create a map
    ACE_GUARD_RETURN(ACE_Thread_Mutex, Guard, Lock, NULL);

    // make sure we have a valid map id
    const MapEntry* entry = sMapStore.LookupEntry(GetId());
    if(!entry)
    {
        sLog->outError("CreateInstance: no entry for map %d", GetId());
        ASSERT(false);
    }
    const InstanceTemplate* iTemplate = sObjectMgr->GetInstanceTemplate(GetId());
    if(!iTemplate)
    {
        sLog->outError("CreateInstance: no instance template for map %d", GetId());
        ASSERT(false);
    }

    // some instances only have one difficulty
    GetDownscaledMapDifficultyData(GetId(), difficulty);

    sLog->outDebug(LOG_FILTER_MAPS, "MapInstanced::CreateInstance: %s map instance %d for %d created with difficulty %s", pSave?"":"new ", InstanceId, GetId(), difficulty?"heroic":"normal");

    InstanceMap* pMap = new InstanceMap(GetId(), GetGridExpiry(), InstanceId, difficulty, this);
    ASSERT(pMap->IsDungeon());

    bool load_data = pSave != NULL;
    pMap->CreateInstanceData(load_data);

    m_InstancedMaps[InstanceId] = pMap;
    return pMap;
}

BattlegroundMap* MapInstanced::CreateBattleground(uint32 InstanceId, Battleground* pBg)
{
    if(!pBg)
        return NULL;

    // load/create map
    ACE_GUARD_RETURN(ACE_Thread_Mutex, Guard, Lock, NULL);

    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(pBg->GetMapId(), pBg->GetMinLevel());

    uint8 spawnMode;

    if(bracketEntry)
        spawnMode = bracketEntry->difficulty;
    else
        spawnMode = REGULAR_DIFFICULTY;

    BattlegroundMap* pMap = new BattlegroundMap(GetId(), GetGridExpiry(), InstanceId, this, spawnMode);
    ASSERT(pMap->IsBattlegroundOrArena());
    pMap->SetBG(pBg);
    pBg->SetBgMap(pMap);

    m_InstancedMaps[InstanceId] = pMap;
    return pMap;
}

// increments the iterator after erase
bool MapInstanced::DestroyInstance(InstancedMaps::iterator &itr)
{
    itr->second->RemoveAllPlayers();
    if(itr->second->HavePlayers())
    {
        ++itr;
        return false;
    }

    itr->second->UnloadAll();
    // should only unload VMaps if this is the last instance and grid unloading is enabled
    if(m_InstancedMaps.size() <= 1 && sWorld->getBoolConfig(CONFIG_GRID_UNLOAD))
    {
        VMAP::VMapFactory::createOrGetVMapManager()->unloadMap(itr->second->GetId());
        // in that case, unload grids of the base map, too
        // so in the next map creation, (EnsureGridCreated actually) VMaps will be reloaded
        Map::UnloadAll();
    }

    // Free up the instance id and allow it to be reused for bgs and arenas (other instances are handled in the InstanceSaveMgr)
    if(itr->second->IsBattlegroundOrArena())
        sMapMgr->FreeInstanceId(itr->second->GetInstanceId());

    // erase map
    delete itr->second;
    m_InstancedMaps.erase(itr++);

    return true;
}

bool MapInstanced::CanEnter(Player* /*pPlayer*/)
{
    return true;
}

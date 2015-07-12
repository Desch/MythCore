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

#ifndef TRINITY_MAPMANAGER_H
#define TRINITY_MAPMANAGER_H

#include "Define.h"
#include <ace/Singleton.h>
#include <ace/Thread_Mutex.h>
#include "Common.h"
#include "Map.h"
#include "GridStates.h"
#include "MapUpdater.h"

class Transport;
struct TransportCreatureProto;

class MapManager
{
    friend class ACE_Singleton<MapManager, ACE_Thread_Mutex>;
    typedef UNORDERED_MAP<uint32, Map*> MapMapType;
    typedef std::vector<bool> InstanceIds;

    public:
        Map* CreateMap(uint32, const WorldObject* obj, uint32 instanceId);
        Map const* CreateBaseMap(uint32 id) const { return const_cast<MapManager*>(this)->_createBaseMap(id); }
        Map* FindMap(uint32 mapid, uint32 instanceId = 0) const;

        uint16 GetAreaFlag(uint32 mapid, float x, float y, float z) const
        {
            Map const* m = CreateBaseMap(mapid);
            return m->GetAreaFlag(x, y, z);
        }
        uint32 GetAreaId(uint32 mapid, float x, float y, float z) const
        {
            return Map::GetAreaIdByAreaFlag(GetAreaFlag(mapid, x, y, z), mapid);
        }
        uint32 GetZoneId(uint32 mapid, float x, float y, float z) const
        {
            return Map::GetZoneIdByAreaFlag(GetAreaFlag(mapid, x, y, z), mapid);
        }
        void GetZoneAndAreaId(uint32& zoneid, uint32& areaid, uint32 mapid, float x, float y, float z)
        {
            Map::GetZoneAndAreaIdByAreaFlag(zoneid, areaid, GetAreaFlag(mapid, x, y, z), mapid);
        }

        void Initialize(void);
        void Update(uint32);

        void SetGridCleanUpDelay(uint32 t)
        {
            if(t < MIN_GRID_DELAY)
                i_gridCleanUpDelay = MIN_GRID_DELAY;
            else
                i_gridCleanUpDelay = t;
        }

        void SetMapUpdateInterval(uint32 t)
        {
            if(t > MIN_MAP_UPDATE_DELAY)
                t = MIN_MAP_UPDATE_DELAY;

            i_timer.SetInterval(t);
            i_timer.Reset();
        }

        void UnloadAll();

        static bool ExistMapAndVMap(uint32 mapid, float x, float y);
        static bool IsValidMAP(uint32 mapid, bool startUp);

        static bool IsValidMapCoord(uint32 mapid, float x, float y)
        {
            return IsValidMAP(mapid, false) && Trinity::IsValidMapCoord(x, y);
        }

        static bool IsValidMapCoord(uint32 mapid, float x, float y, float z)
        {
            return IsValidMAP(mapid, false) && Trinity::IsValidMapCoord(x, y, z);
        }

        static bool IsValidMapCoord(uint32 mapid, float x, float y, float z, float o)
        {
            return IsValidMAP(mapid, false) && Trinity::IsValidMapCoord(x, y, z, o);
        }

        static bool IsValidMapCoord(WorldLocation const& loc)
        {
            return IsValidMapCoord(loc.GetMapId(), loc.GetPositionX(), loc.GetPositionY(), loc.GetPositionZ(), loc.GetOrientation());
        }

        // modulos a radian orientation to the range of 0..2PI
        static float NormalizeOrientation(float o)
        {
            // fmod only supports positive numbers. Thus we have
            // to emulate negative numbers
            if(o < 0)
            {
                float mod = o *-1;
                mod = fmod(mod, 2.0f * static_cast<float>(M_PI));
                mod = -mod + 2.0f * static_cast<float>(M_PI);
                return mod;
            }
            return fmod(o, 2.0f * static_cast<float>(M_PI));
        }

        void DoDelayedMovesAndRemoves();

        void LoadTransports();
        void LoadTransportNPCs();

         //Load transport to instance
        Transport* LoadTransportInMap(Map* instance, uint32 goEntry, uint32 period);

        void UnLoadTransportFromMap(Transport* t);
        void LoadTransportForPlayers(Player* player);
        void UnLoadTransportForPlayers(Player* player);

        typedef std::set<Transport *> TransportSet;
        TransportSet m_Transports;

        typedef std::map<uint32, TransportSet> TransportMap;
        TransportMap m_TransportsByMap;
        TransportMap m_TransportsByInstanceIdMap;

        bool CanPlayerEnter(uint32 mapid, Player* player, bool loginCheck = false);
        void InitializeVisibilityDistanceInfo();

        /* statistics */
        uint32 GetNumInstances();
        uint32 GetNumPlayersInInstances();

        // Instance ID management
        void InitInstanceIds();
        uint32 GenerateInstanceId();
        void RegisterInstanceId(uint32 instanceId);
        void FreeInstanceId(uint32 instanceId);

        uint32 GetNextInstanceId() { return _nextInstanceId; };
        void SetNextInstanceId(uint32 nextInstanceId) { _nextInstanceId = nextInstanceId; };

        MapUpdater * GetMapUpdater() { return &m_updater; }

    private:
        // debugging code, should be deleted some day
        void checkAndCorrectGridStatesArray();              // just for debugging to find some memory overwrites
        GridState* i_GridStates[MAX_GRID_STATE];            // shadow entries to the global array in Map.cpp
        int i_GridStateErrorCount;
        MapManager();
        ~MapManager();

        MapManager(const MapManager &);
        MapManager& operator=(const MapManager &);

        Map* _createBaseMap(uint32 id);
        Map* _findMap(uint32 id) const
        {
            MapMapType::const_iterator iter = i_maps.find(id);
            return (iter == i_maps.end() ? NULL : iter->second);
        }

        ACE_Thread_Mutex Lock;
        uint32 i_gridCleanUpDelay;
        MapMapType i_maps;
        IntervalTimer i_timer;

        InstanceIds _instanceIds;
        uint32 _nextInstanceId;
        MapUpdater m_updater;
};

#define sMapMgr ACE_Singleton<MapManager, ACE_Thread_Mutex>::instance()
#endif

/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Copyright (C) 2012 SymphonyArt <http://symphonyart.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#ifndef _MAPTREE_H
#define _MAPTREE_H

#include "Define.h"
#include "Dynamic/UnorderedMap.h"
#include "BoundingIntervalHierarchy.h"

namespace VMAP
{
    class ModelInstance;
    class GroupModel;
    class VMapManager2;

    struct LocationInfo
    {
        LocationInfo(): hitInstance(0), hitModel(0), ground_Z(-G3D::inf()) { };
        const ModelInstance* hitInstance;
        const GroupModel* hitModel;
        float ground_Z;
    };

    class StaticMapTree
    {
        typedef UNORDERED_MAP<uint32, bool> loadedTileMap;
        typedef UNORDERED_MAP<uint32, uint32> loadedSpawnMap;
    private:
        uint32 iMapID;
        bool iIsTiled;
        BIH iTree;
        ModelInstance* iTreeValues; // the tree entries
        uint32 iNTreeValues;

        // Store all the map tile idents that are loaded for that map
        // some maps are not splitted into tiles and we have to make sure, not removing the map before all tiles are removed
        // empty tiles have no tile file, hence map with bool instead of just a set (consistency check)
        loadedTileMap iLoadedTiles;
        // stores <tree_index, reference_count> to invalidate tree values, unload map, and to be able to report errors
        loadedSpawnMap iLoadedSpawns;
        std::string iBasePath;

    private:
        bool getIntersectionTime(const G3D::Ray& pRay, float &pMaxDist, bool pStopAtFirstHit) const;
        //bool containsLoadedMapTile(unsigned int pTileIdent) const { return(iLoadedMapTiles.containsKey(pTileIdent)); }
    public:
        static std::string getTileFileName(uint32 mapID, uint32 tileX, uint32 tileY);
        static uint32 packTileID(uint32 tileX, uint32 tileY) { return tileX<<16 | tileY; }
        static void unpackTileID(uint32 ID, uint32 &tileX, uint32 &tileY) { tileX = ID>>16; tileY = ID&0xFF; }
        static bool CanLoadMap(const std::string &basePath, uint32 mapID, uint32 tileX, uint32 tileY);

        StaticMapTree(uint32 mapID, const std::string &basePath);
        ~StaticMapTree();

        bool isInLineOfSight(const G3D::Vector3& pos1, const G3D::Vector3& pos2) const;
        bool getObjectHitPos(const G3D::Vector3& pos1, const G3D::Vector3& pos2, G3D::Vector3& pResultHitPos, float pModifyDist) const;
        float getHeight(const G3D::Vector3& pPos, float maxSearchDist) const;
        bool getAreaInfo(G3D::Vector3 &pos, uint32 &flags, int32 &adtId, int32 &rootId, int32 &groupId) const;
        bool GetLocationInfo(const Vector3 &pos, LocationInfo &info) const;

        bool InitMap(const std::string &fname, VMapManager2* vm);
        void UnloadMap(VMapManager2* vm);
        bool LoadMapTile(uint32 tileX, uint32 tileY, VMapManager2* vm);
        void UnloadMapTile(uint32 tileX, uint32 tileY, VMapManager2* vm);
        bool isTiled() const { return iIsTiled; }
        uint32 numLoadedTiles() const { return iLoadedTiles.size(); }
#ifdef MMAP_GENERATOR
    public:
        void getModelInstances(ModelInstance* &models, uint32 &count);
#endif
    };

    struct AreaInfo
    {
        AreaInfo(): result(false), ground_Z(-G3D::inf()) { };
        bool result;
        float ground_Z;
        uint32 flags;
        int32 adtId;
        int32 rootId;
        int32 groupId;
    };
}                                                           // VMAP

#endif // _MAPTREE_H

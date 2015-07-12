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

#include "ObjectAccessor.h"
#include "ScriptMgr.h"
#include "Chat.h"

class gps_commandscript : public CommandScript
{
public:
    gps_commandscript() : CommandScript("gps_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand commandTable[] =
        {
            { "gps",      SEC_ADMINISTRATOR,  false, &HandleGPSCommand,                  "", NULL },
            { "wpgps",    SEC_ADMINISTRATOR,  false, &HandleWPGPSCommand,                "", NULL },
            { NULL,       0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleGPSCommand(ChatHandler* handler, const char *args)
    {
        WorldObject* obj = NULL;
        if(*args)
        {
            uint64 guid = handler->extractGuidFromLink((char*)args);
            if(guid)
                obj = (WorldObject*)ObjectAccessor::GetObjectByTypeMask(*handler->GetSession()->GetPlayer(), guid, TYPEMASK_UNIT|TYPEMASK_GAMEOBJECT);

            if(!obj)
            {
                handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                handler->SetSentErrorMessage(true);
                return false;
            }
        }
        else
        {
            obj = handler->getSelectedUnit();

            if(!obj)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }
        }
        CellPair cell_val = Trinity::ComputeCellPair(obj->GetPositionX(), obj->GetPositionY());
        Cell cell(cell_val);

        uint32 zone_id, area_id;
        obj->GetZoneAndAreaId(zone_id, area_id);

        MapEntry const* mapEntry = sMapStore.LookupEntry(obj->GetMapId());
        AreaTableEntry const* zoneEntry = GetAreaEntryByAreaID(zone_id);
        AreaTableEntry const* areaEntry = GetAreaEntryByAreaID(area_id);

        float zone_x = obj->GetPositionX();
        float zone_y = obj->GetPositionY();

        Map2ZoneCoordinates(zone_x, zone_y, zone_id);

        Map const *map = obj->GetMap();
        float ground_z = map->GetHeight(obj->GetPhaseMask(), obj->GetPositionX(), obj->GetPositionY(), MAX_HEIGHT);
        float floor_z = map->GetHeight(obj->GetPhaseMask(), obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ());

        GridPair p = Trinity::ComputeGridPair(obj->GetPositionX(), obj->GetPositionY());

        // 63? WHY?
        int gx = 63 - p.x_coord;
        int gy = 63 - p.y_coord;

        uint32 have_map = Map::ExistMap(obj->GetMapId(), gx, gy) ? 1 : 0;
        uint32 have_vmap = Map::ExistVMap(obj->GetMapId(), gx, gy) ? 1 : 0;

        if(have_vmap)
        {
            if(map->IsOutdoors(obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ()))
                handler->PSendSysMessage("You are outdoors");
            else
                handler->PSendSysMessage("You are indoors");
        }
        else handler->PSendSysMessage("no VMAP available for area info");

        handler->PSendSysMessage(LANG_MAP_POSITION,
            obj->GetMapId(), (mapEntry ? mapEntry->name[handler->GetSessionDbcLocale()] : "<unknown>"),
            zone_id, (zoneEntry ? zoneEntry->area_name[handler->GetSessionDbcLocale()] : "<unknown>"),
            area_id, (areaEntry ? areaEntry->area_name[handler->GetSessionDbcLocale()] : "<unknown>"),
            obj->GetPhaseMask(),
            obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), obj->GetOrientation(),
            cell.GridX(), cell.GridY(), cell.CellX(), cell.CellY(), obj->GetInstanceId(),
            zone_x, zone_y, ground_z, floor_z, have_map, have_vmap);

        LiquidData liquid_status;
        ZLiquidStatus res = map->getLiquidStatus(obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), MAP_ALL_LIQUIDS, &liquid_status);
        if(res)
        {
            handler->PSendSysMessage(LANG_LIQUID_STATUS, liquid_status.level, liquid_status.depth_level, liquid_status.type_flags, res);
        }
        return true;
    }

    static bool HandleWPGPSCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();

        sLog->outSQLDev("(@PATH,XX,%.3f,%.3f,%.5f, 0, 0, 0, 100, 0),", player->GetPositionX(), player->GetPositionY(), player->GetPositionZ());

        handler->PSendSysMessage("Waypoint SQL written to SQL Developer log");
        return true;
    }
};

void AddSC_gps_commandscript()
{
    new gps_commandscript;
}

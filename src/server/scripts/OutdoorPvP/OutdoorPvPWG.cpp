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

#include "OutdoorPvPWG.h"
#include "SpellAuras.h"
#include "Vehicle.h"
#include "ObjectMgr.h"
#include "World.h"
#include "Group.h"
#include "Chat.h"
#include "MapManager.h"
#include "ScriptedCreature.h"
#include "ScriptPCH.h"

Creature* FortressSpirit;
uint32 entry;
uint32 guide_entry;
uint32 guide_entry_fortress_horde;
uint32 guide_entry_fortress_alliance;

char const *fmtstring(char const *format, ...)
{
    va_list     argptr;
    #define MAX_FMT_STRING    32000
    static char temp_buffer[MAX_FMT_STRING];
    static char string[MAX_FMT_STRING];
    static int  index = 0;
    char        *buf;
    int         len;

    va_start(argptr, format);
    vsnprintf(temp_buffer,MAX_FMT_STRING, format, argptr);
    va_end(argptr);

    len = strlen(temp_buffer);

    if(len >= MAX_FMT_STRING)
        return "ERROR";

    if(len + index >= MAX_FMT_STRING-1)
        index = 0;

    buf = &string[index];
    memcpy(buf, temp_buffer, len+1);

    index += len + 1;

    return buf;
}

OutdoorPvPWG::OutdoorPvPWG()
{
    m_TypeId = OUTDOOR_PVP_WG;
    m_LastResurrectTime = 0;
}

void _LoadTeamPair(TeamPairMap &pairMap, const TeamPair* pPair)
{
    while((*pPair)[0]) {
        pairMap[(*pPair)[0]] = (*pPair)[1];
        pairMap[(*pPair)[1]] = (*pPair)[0];
        ++pPair;
    }
}

void _RespawnCreatureIfNeeded(Creature* pCreature, uint32 entry)
{
    if(!pCreature)
        return;
    pCreature->UpdateEntry(entry);
    if(!pCreature->isAlive())
        pCreature->Respawn(true);
    pCreature->SetVisible(true);
}

bool OutdoorPvPWG::SetupOutdoorPvP()
{
    if(!sWorld->getBoolConfig(CONFIG_OUTDOORPVP_WINTERGRASP_ENABLED)) {
        sWorld->setWorldState(WORLDSTATE_WINTERGRASP_CONTROLING_FACTION, TEAM_NEUTRAL);
        return false;
    }

    m_wartime  = sWorld->getWorldState(WORLDSTATE_WINTERGRASP_WARTIME) ? true : false;
    m_timer    = uint32(sWorld->getWorldState(WORLDSTATE_WINTERGRASP_TIMER));
    m_defender = TeamId(sWorld->getWorldState(WORLDSTATE_WINTERGRASP_DEFENDERS) ? true : false);
    m_WSSaveTimer = sWorld->getIntConfig(CONFIG_OUTDOORPVP_WINTERGRASP_SAVESTATE_PERIOD);

    if(m_timer == 0)
       m_timer = sWorld->getIntConfig(CONFIG_OUTDOORPVP_WINTERGRASP_START_TIME) * MINUTE * IN_MILLISECONDS;

    sWorld->setWorldState(WORLDSTATE_WINTERGRASP_CONTROLING_FACTION, getDefenderTeam());
    sWorld->setWorldState(WORLDSTATE_WINTERGRASP_DEFENDERS, getDefenderTeam());
    m_changeDefender = false;
    m_workshopCount[TEAM_ALLIANCE] = 0;
    m_workshopCount[TEAM_HORDE] = 0;
    m_tenacityStack = 0;
    m_gate = NULL;

    std::list<uint32> engGuids;
    std::list<uint32> spiritGuids;

    QueryResult result = WorldDatabase.PQuery("SELECT `guid`, `id` FROM `creature` WHERE `map` = 571 \
        AND `id` IN (30499, 30400, 31842, 31841, 31101, 31051, 31102, 31052, 31107, 31109, 31151, 31153, 31106, 31108, 31053, 31054, 31091, 31036)");
    if(!result)
        sLog->outError("Cannot find siege workshop master or spirit guides in creature!");
    else {
        do {
            Position posHorde, posAlli;
            Field* fields = result->Fetch();
            switch(fields[1].GetUInt32())
            {
                case CRE_ENG_A:
                case CRE_ENG_H:
                    engGuids.push_back(fields[0].GetUInt32());
                    break;
                case CRE_SPI_A:
                case CRE_SPI_H:
                    spiritGuids.push_back(fields[0].GetUInt32());
                    break;
                case 31051:
                    posHorde.Relocate(5081.7f, 2173.73f, 365.878f, 0.855211f);
                    posAlli.Relocate(5298.39f, 2890.56f, 409.191f, 5.602510f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                case 31101:
                    posHorde.Relocate(5296.56f, 2789.87f, 409.275f, 0.733038f);
                    posAlli.Relocate(5016.57f, 3677.53f, 362.982f, 5.7525f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                case 31052:
                    posHorde.Relocate(5100.07f, 2168.89f, 365.779f, 1.97222f);
                    posAlli.Relocate(5298.43f, 2738.76f, 409.316f, 3.97174f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                case 31102:
                    posHorde.Relocate(5298.43f, 2738.76f, 409.316f, 3.97174f);
                    posAlli.Relocate(5030.44f, 3659.82f, 363.194f, 1.83336f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                case 31109:
                    posHorde.Relocate(5080.4f, 2199.0f, 359.489f, 2.96706f);
                    posAlli.Relocate(5234.97f, 2883.4f, 409.275f, 4.29351f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                case 31107:
                    posHorde.Relocate(5234.97f, 2883.4f, 409.275f, 4.29351f);
                    posAlli.Relocate(5008.64f, 3659.91f, 361.07f, 4.0796f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                case 31153:
                    posHorde.Relocate(5088.49f, 2188.18f, 365.647f, 5.25344f);
                    posAlli.Relocate(5366.13f, 2833.4f, 409.323f, 3.14159f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                case 31151:
                    posHorde.Relocate(5366.13f, 2833.4f, 409.323f, 3.14159f);
                    posAlli.Relocate(5032.33f, 3680.7f, 363.018f, 3.43167f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                case 31108:
                    posHorde.Relocate(5095.67f, 2193.28f, 365.924f, 4.93928f);
                    posAlli.Relocate(5295.56f, 2926.67f, 409.275f, 0.872665f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                case 31106:
                    posHorde.Relocate(5295.56f, 2926.67f, 409.275f, 0.872665f);
                    posAlli.Relocate(5032.66f, 3674.28f, 363.053f, 2.9447f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                case 31054:
                    posHorde.Relocate(5088.61f, 2167.66f, 365.689f, 0.680678f);
                    posAlli.Relocate(5371.4f, 3026.51f, 409.206f, 3.25003f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                case 31053:
                    posHorde.Relocate(5371.4f, 3026.51f, 409.206f, 3.25003f);
                    posAlli.Relocate(5032.44f, 3668.66f, 363.11f, 2.87402f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                case 31036:
                    posHorde.Relocate(5078.28f, 2183.7f, 365.029f, 1.46608f);
                    posAlli.Relocate(5359.13f, 2837.99f, 409.364f, 4.69893f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                case 31091:
                    posHorde.Relocate(5359.13f, 2837.99f, 409.364f, 4.69893f);
                    posAlli.Relocate(5022.43f, 3659.91f, 361.61f, 1.35426f);
                    LoadQuestGiverMap(fields[0].GetUInt32(), posHorde, posAlli);
                    break;
                default:
                    break;
            }
        } while(result->NextRow());
    }

    AreaPOIList areaPOIs;
    float minX = 9999, minY = 9999, maxX = -9999, maxY = -9999;
    for(uint32 i = 0; i < sAreaPOIStore.GetNumRows(); ++i)
    {
        const AreaPOIEntry * poiInfo = sAreaPOIStore.LookupEntry(i);
        if(poiInfo && poiInfo->zoneId == ZONE_WINTERGRASP)
        {
            areaPOIs.push_back(poiInfo);
            if(minX > poiInfo->x) minX = poiInfo->x;
            if(minY > poiInfo->y) minY = poiInfo->y;
            if(maxX < poiInfo->x) maxX = poiInfo->x;
            if(maxY < poiInfo->y) maxY = poiInfo->y;
        }
    }
    minX -= 20; minY -= 20; maxX += 20; maxY += 20;

    result = WorldDatabase.PQuery("SELECT `guid` FROM `gameobject`,`gameobject_template`"
        " WHERE `gameobject`.`map` = 571"
        " AND `gameobject`.`position_x` > %f AND `gameobject`.`position_y` > %f"
        " AND `gameobject`.`position_x` < %f AND `gameobject`.`position_y` < %f"
        " AND `gameobject_template`.`type` = 33"
        " AND `gameobject`.`id` = `gameobject_template`.`entry`",
        minX, minY, maxX, maxY);
    if(!result)
        return false;
    else {
        do {
            Field* fields = result->Fetch();

            uint32 guid = fields[0].GetUInt32();
            GameObjectData const * goData = sObjectMgr->GetGOData(guid);
            if(!goData)
                continue;

            float x = goData->posX, y = goData->posY;
            float minDist = 100;
            AreaPOIList::iterator poi = areaPOIs.end();
            for(AreaPOIList::iterator itr = areaPOIs.begin(); itr != areaPOIs.end(); ++itr) {
                if(!(*itr)->icon[1])
                    continue;

                float dist = (abs((*itr)->x - x) + abs((*itr)->y - y));
                if(minDist > dist) {
                    minDist = dist;
                    poi = itr;
                }
            }

            if(poi == areaPOIs.end())
                continue;

            TeamId teamId = x > POS_X_CENTER - 300 ? getDefenderTeam() : getAttackerTeam();
            m_buildingStates[guid] = new BuildingState((*poi)->worldState, teamId, getDefenderTeam() != TEAM_ALLIANCE);
            if((*poi)->id == 2246)
                m_gate = m_buildingStates[guid];
            areaPOIs.erase(poi);

            uint32 capturePointEntry = 0;

            switch(goData->id)
            {
                case 192028: // NW
                case 192030: // W
                case 192032: // SW
                    capturePointEntry = 190475;
                    break;
                case 192029: // NE
                case 192031: // E
                case 192033: // SE
                    capturePointEntry = 190487;
                    break;
            }

            if(capturePointEntry) {
                uint32 engGuid = 0;
                uint32 spiritGuid = 0;

                float minDist = 100;
                for(std::list<uint32>::iterator itr = engGuids.begin(); itr != engGuids.end(); ++itr) {
                    const CreatureData *creData = sObjectMgr->GetCreatureData(*itr);
                    if(!creData)
                        continue;

                    float dist = (abs(creData->posX - x) + abs(creData->posY - y));
                    if(minDist > dist) {
                        minDist = dist;
                        engGuid = *itr;
                    }
                }

                if(!engGuid) {
                    sLog->outError("Cannot find nearby siege workshop master!");
                    continue;
                } else engGuids.remove(engGuid);

                minDist = 255;
                for(std::list<uint32>::iterator itr = spiritGuids.begin(); itr != spiritGuids.end(); ++itr) {
                    const CreatureData *creData = sObjectMgr->GetCreatureData(*itr);
                    if(!creData)
                        continue;

                    float dist = (abs(creData->posX - x) + abs(creData->posY - y));
                    if(minDist > dist)
                    {
                        minDist = dist;
                        spiritGuid = *itr;
                    }
                }

                OPvPCapturePointWG* pWO = new OPvPCapturePointWG(this, m_buildingStates[guid]);
                if(goData->posX < POS_X_CENTER && !pWO->SetCapturePointData(capturePointEntry, goData->mapid, goData->posX + 40 * cos(goData->orientation + M_PI / 2), goData->posY + 40 * sin(goData->orientation + M_PI / 2), goData->posZ)) {
                    delete pWO;
                    sLog->outError("Cannot add capture point!");
                    continue;
                }

                if(goData->id==192028 || goData->id==192029)
                    pWO->m_capturePointGUID = goData->id;

                const CreatureData *creData = sObjectMgr->GetCreatureData(engGuid);
                if(!creData)
                    continue;

                pWO->m_engEntry = const_cast<uint32*>(&creData->id);
                const_cast<CreatureData*>(creData)->displayid = 0;
                pWO->m_engGuid = engGuid;

                if(spiritGuid)
                {
                    spiritGuids.remove(spiritGuid);

                    const CreatureData *spiritData = sObjectMgr->GetCreatureData(spiritGuid);
                    if(!spiritData)
                        continue;

                    pWO->m_spiEntry = const_cast<uint32*>(&spiritData->id);
                    const_cast<CreatureData*>(spiritData)->displayid = 0;
                    pWO->m_spiGuid = spiritGuid;
                } else pWO->m_spiGuid = 0;
                pWO->m_workshopGuid = guid;
                AddCapturePoint(pWO);
                m_buildingStates[guid]->type = BUILDING_WORKSHOP;
                pWO->SetTeamByBuildingState();
            }
        } while(result->NextRow());
    }

    engGuids.clear();
    spiritGuids.clear();

    if(!m_gate) {
        sLog->outError("Cannot find wintergrasp fortress gate!");
        return false;
    }

    GraveYardMap::const_iterator graveLow  = sObjectMgr->mGraveYardMap.lower_bound(ZONE_WINTERGRASP);
    GraveYardMap::const_iterator graveUp   = sObjectMgr->mGraveYardMap.upper_bound(ZONE_WINTERGRASP);
    for(AreaPOIList::iterator itr = areaPOIs.begin(); itr != areaPOIs.end();)
    {
        if((*itr)->icon[1] == 8) {
            const WorldSafeLocsEntry *loc = sObjectMgr->GetClosestGraveYard((*itr)->x, (*itr)->y, (*itr)->z, (*itr)->mapId, 0);
            if(!loc) {
                ++itr;
                continue;
            }

            GraveYardMap::const_iterator graveItr;
            for(graveItr = graveLow; graveItr != graveUp; ++graveItr)
                if(graveItr->second.safeLocId == loc->ID)
                    break;
            if(graveItr == graveUp)
            {
                GraveYardData graveData;
                graveData.safeLocId = loc->ID;
                graveData.team = 0;
                graveItr = sObjectMgr->mGraveYardMap.insert(std::make_pair(ZONE_WINTERGRASP, graveData));
            }

            for(BuildingStateMap::iterator stateItr = m_buildingStates.begin(); stateItr != m_buildingStates.end(); ++stateItr) {
                if(stateItr->second->worldState == (*itr)->worldState)
                {
                    stateItr->second->graveTeam = const_cast<uint32*>(&graveItr->second.team);
                    break;
                }
            }
            areaPOIs.erase(itr++);
        } else ++itr;
    }

    sObjectMgr->AddGOData(192829, 571, 5440.0f, 2840.8f, 420.43f + 10.0f, 0);

    _LoadTeamPair(m_goDisplayPair, OutdoorPvPWGGODisplayPair);
    _LoadTeamPair(m_creEntryPair, OutdoorPvPWGCreEntryPair);

    sWorld->SendWintergraspState();
    m_towerDamagedCount[TEAM_ALLIANCE]      = 0;
    m_towerDestroyedCount[TEAM_ALLIANCE]    = 0;
    m_towerDamagedCount[TEAM_HORDE]         = 0;
    m_towerDestroyedCount[TEAM_HORDE]       = 0;

    RegisterZone(ZONE_WINTERGRASP);

    if(m_wartime)
    {
        uint32 m_WSTimer = m_timer;
        StartBattle();
        m_timer = m_WSTimer;
    } else RemoveOfflinePlayerWGAuras();

    return true;
}

void OutdoorPvPWG::ProcessEvent(WorldObject* pWO, uint32 eventId)
{
    GameObject* obj = pWO->ToGameObject();
    if(!obj)
        return;

    if(obj->GetEntry() == 192829)
    {   // Titan Relic
        if(obj->GetGOInfo()->goober.eventId == eventId && isWarTime() && m_gate &&  m_gate->damageState == DAMAGE_DESTROYED)
        {
            m_changeDefender = true;
            m_timer = 0;
        }
    } else if(obj->GetGoType() == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING) {
        BuildingStateMap::const_iterator itr = m_buildingStates.find(obj->GetDBTableGUIDLow());
        if(itr == m_buildingStates.end())
            return;
        std::string msgStr;
        switch(eventId)
        {
            case 19672: case 19675: msgStr = "Flamewatch";      break;  // Flamewatch Tower
            case 18553: case 19677: msgStr = "Shadowsight";     break;  // Shadowsight Tower
            case 19673: case 19676: msgStr = "Winter's Edge";   break;  // Winter's Edge Tower
            case 19776: case 19778: msgStr = "Sunken Ring";     break;  // E Workshop damaged
            case 19777: case 19779: msgStr = "Broken Temple";   break;  // W Workshop damaged
            case 19782: case 19786: msgStr = "north-western";   break;  // NW Workshop damaged
            case 19783: case 19787: msgStr = "north-eastern";   break;  // NE Workshop damaged
            case 19784: case 19788: msgStr = "Westpark";        break;  // SW Workshop damaged
            case 19785: case 19789: msgStr = "Eastpark";        break;  // SE Workshop damaged
            case 19657: case 19661: msgStr = "north-western";   break;  // NW Wintergrasp Keep Tower damaged
            case 19658: case 19663: msgStr = "north-eastern";   break;  // NE Wintergrasp Keep Tower damaged
            case 19659: case 19662: msgStr = "south-western";   break;  // SW Wintergrasp Keep Tower damaged
            case 19660: case 19664: msgStr = "south-eastern";   break;  // SE Wintergrasp Keep Tower damaged
            default:
                msgStr = "";
        }

        BuildingState* state = itr->second;
        if(eventId == obj->GetGOInfo()->building.damagedEvent) {
            state->damageState = DAMAGE_DAMAGED;
            switch(state->type)
            {
                case BUILDING_WORKSHOP:
                    msgStr = fmtstring(sObjectMgr->GetTrinityStringForDBCLocale(LANG_BG_WG_WORKSHOP_DAMAGED), msgStr.c_str(), sObjectMgr->GetTrinityStringForDBCLocale(getDefenderTeam() == TEAM_ALLIANCE ? LANG_BG_AB_ALLY : LANG_BG_AB_HORDE));
                    sWorld->SendZoneText(ZONE_WINTERGRASP, msgStr.c_str());
                    break;
                case BUILDING_WALL:
                    sWorld->SendZoneText(ZONE_WINTERGRASP, sObjectMgr->GetTrinityStringForDBCLocale(LANG_BG_WG_FORTRESS_UNDER_ATTACK));
                    for(PlayerSet::iterator itr = m_players[getDefenderTeam()].begin(); itr != m_players[getDefenderTeam()].end(); ++itr) {
                        TeamIDsound = getDefenderTeam() == TEAM_ALLIANCE ? OutdoorPvP_WG_SOUND_KEEP_ASSAULTED_HORDE : OutdoorPvP_WG_SOUND_KEEP_ASSAULTED_ALLIANCE;                        
                        (*itr)->PlayDirectSound(TeamIDsound);
                    }
                    for(PlayerSet::iterator itr = m_players[getAttackerTeam()].begin(); itr != m_players[getAttackerTeam()].end(); ++itr) {
                        TeamIDsound = getDefenderTeam() == TEAM_ALLIANCE ? OutdoorPvP_WG_SOUND_KEEP_ASSAULTED_HORDE : OutdoorPvP_WG_SOUND_KEEP_ASSAULTED_ALLIANCE;                        
                        (*itr)->PlayDirectSound(TeamIDsound);
                    }
                    break;
                case BUILDING_TOWER:
                    ++m_towerDamagedCount[state->GetTeam()];
                    msgStr = fmtstring(sObjectMgr->GetTrinityStringForDBCLocale(LANG_BG_WG_TOWER_DAMAGED), msgStr.c_str());
                    sWorld->SendZoneText(ZONE_WINTERGRASP, msgStr.c_str());
                    for(PlayerSet::iterator itr = m_players[getDefenderTeam()].begin(); itr != m_players[getDefenderTeam()].end(); ++itr) {
                        TeamIDsound = getDefenderTeam() == TEAM_ALLIANCE ? OutdoorPvP_WG_SOUND_KEEP_CAPTURED_HORDE : OutdoorPvP_WG_SOUND_KEEP_CAPTURED_ALLIANCE;                        
                        (*itr)->PlayDirectSound(TeamIDsound);
                    }
                    for(PlayerSet::iterator itr = m_players[getAttackerTeam()].begin(); itr != m_players[getAttackerTeam()].end(); ++itr) {
                        TeamIDsound = getDefenderTeam() == TEAM_ALLIANCE ? OutdoorPvP_WG_SOUND_KEEP_CAPTURED_HORDE : OutdoorPvP_WG_SOUND_KEEP_CAPTURED_ALLIANCE;                        
                        (*itr)->PlayDirectSound(TeamIDsound);
                    }
                    break;
            }
        } else if(eventId == obj->GetGOInfo()->building.destroyedEvent) {
            state->damageState = DAMAGE_DESTROYED;

            switch(state->type)
            {
                case BUILDING_WORKSHOP:
                    ModifyWorkshopCount(state->GetTeam(), false);
                    msgStr = fmtstring(sObjectMgr->GetTrinityStringForDBCLocale(LANG_BG_WG_WORKSHOP_DESTROYED), msgStr.c_str(), sObjectMgr->GetTrinityStringForDBCLocale(getDefenderTeam() == TEAM_ALLIANCE ? LANG_BG_AB_ALLY : LANG_BG_AB_HORDE));
                    sWorld->SendZoneText(ZONE_WINTERGRASP, msgStr.c_str());
                    break;
                case BUILDING_WALL:
                    sWorld->SendZoneText(ZONE_WINTERGRASP, sObjectMgr->GetTrinityStringForDBCLocale(LANG_BG_WG_FORTRESS_UNDER_ATTACK));
                    for(PlayerSet::iterator itr = m_players[getDefenderTeam()].begin(); itr != m_players[getDefenderTeam()].end(); ++itr) {
                        TeamIDsound = getDefenderTeam() == TEAM_ALLIANCE ? OutdoorPvP_WG_SOUND_KEEP_CAPTURED_HORDE : OutdoorPvP_WG_SOUND_KEEP_CAPTURED_ALLIANCE;
                        (*itr)->PlayDirectSound(TeamIDsound);
                    }
                    for(PlayerSet::iterator itr = m_players[getAttackerTeam()].begin(); itr != m_players[getAttackerTeam()].end(); ++itr) {
                        TeamIDsound = getDefenderTeam() == TEAM_ALLIANCE ? OutdoorPvP_WG_SOUND_KEEP_CAPTURED_HORDE : OutdoorPvP_WG_SOUND_KEEP_CAPTURED_ALLIANCE;
                        (*itr)->PlayDirectSound(TeamIDsound);
                    }
                    break;
                case BUILDING_TOWER:
                {
                    --m_towerDamagedCount[state->GetTeam()];
                    ++m_towerDestroyedCount[state->GetTeam()];
                    if(state->GetTeam() == getAttackerTeam())
                    {
                        TeamCastSpell(getAttackerTeam(), -SPELL_TOWER_CONTROL);
                        TeamCastSpell(getDefenderTeam(), -SPELL_TOWER_CONTROL);
                        uint32 attStack = 3 - m_towerDestroyedCount[getAttackerTeam()];
                        if(m_towerDestroyedCount[getAttackerTeam()])
                        {
                            for(PlayerSet::iterator itr = m_players[getAttackerTeam()].begin(); itr != m_players[getAttackerTeam()].end(); ++itr)
                            {
                                (*itr)->SetAuraStack(SPELL_TOWER_CONTROL, (*itr), m_towerDestroyedCount[getAttackerTeam()]);
                                (*itr)->Achievement1727Check();
                                (*itr)->CheckQuest13538();
                            }
                        }
                        if(attStack != 0)
                        {
                            for(PlayerSet::iterator itr = m_players[getAttackerTeam()].begin(); itr != m_players[getAttackerTeam()].end(); ++itr)
                                (*itr)->SetAuraStack(SPELL_TOWER_CONTROL, (*itr), attStack);
                        } else if(m_timer < 600000)
                            m_timer = 0;
                        else
                            m_timer = m_timer - 600000;
                    }
                    msgStr = fmtstring(sObjectMgr->GetTrinityStringForDBCLocale(LANG_BG_WG_TOWER_DESTROYED), msgStr.c_str());
                    sWorld->SendZoneText(ZONE_WINTERGRASP, msgStr.c_str());
                    for(PlayerSet::iterator itr = m_players[getDefenderTeam()].begin(); itr != m_players[getDefenderTeam()].end(); ++itr)
                    {
                        TeamIDsound = getDefenderTeam() == TEAM_ALLIANCE ? OutdoorPvP_WG_HORDE_CAPTAIN : OutdoorPvP_WG_ALLIANCE_CAPTAIN;
                        (*itr)->PlayDirectSound(TeamIDsound);

                        if(obj->GetEntry() == 190356 || obj->GetEntry() == 190357 || obj->GetEntry() == 190358)
                            (*itr)->RewardPlayerAndGroupAtEvent(TOWER_PVP_DESTROYED, obj);
                    }
                    for(PlayerSet::iterator itr = m_players[getAttackerTeam()].begin(); itr != m_players[getAttackerTeam()].end(); ++itr)
                    {
                        TeamIDsound = getDefenderTeam() == TEAM_ALLIANCE ? OutdoorPvP_WG_HORDE_CAPTAIN : OutdoorPvP_WG_ALLIANCE_CAPTAIN;
                        (*itr)->PlayDirectSound(TeamIDsound);
                    }
                }
                break;
            }
            BroadcastStateChange(state);
        }
    }
}

void OutdoorPvPWG::RemoveOfflinePlayerWGAuras()
{
    CharacterDatabase.PExecute("DELETE FROM `character_aura` WHERE `spell` IN (%u, %u, %u, %u, %u)",
         SPELL_RECRUIT, SPELL_CORPORAL, SPELL_LIEUTENANT, SPELL_TENACITY, SPELL_TOWER_CONTROL);
}

void OutdoorPvPWG::ModifyWorkshopCount(TeamId team, bool ADD)
{
    if(team == TEAM_NEUTRAL)
        return;

    if(ADD)
        ++m_workshopCount[team];
    else if(m_workshopCount[team])
        --m_workshopCount[team];

    SendUpdateWorldState(MaxVehNumWorldState[team], m_workshopCount[team] * MAX_VEHICLE_PER_WORKSHOP);
}

uint32 OutdoorPvPWG::GetCreatureEntry(uint32 /*guidlow*/, const CreatureData* pCreatureData)
{
    if(getDefenderTeam() == TEAM_ALLIANCE)
    {
        TeamPairMap::const_iterator itr = m_creEntryPair.find(pCreatureData->id);
        if(itr != m_creEntryPair.end())
        {
            const_cast<CreatureData*>(pCreatureData)->displayid = 0;
            return itr->second;
        }
    }
    return pCreatureData->id;
}

OutdoorPvPWGCreType OutdoorPvPWG::GetCreatureType(uint32 entry) const
{
    switch(entry)
    {
        case 27881: // Catapult
        case 28094: // Demolisher
        case 28312: // Alliance Siege Engine
        case 32627: // Horde Siege Engine
        case 28319: // Siege turret
        case 32629: // Siege turret
            return CREATURE_SIEGE_VEHICLE;
        case 28366: // Wintergrasp Tower cannon
            return CREATURE_TURRET;
        case CRE_ENG_A: // Alliance Engineer
        case CRE_ENG_H: // Horde Engineer
            return CREATURE_ENGINEER;
        case 30739: case 30740: // Champions
        case 32307: case 32308: // Guards
            return CREATURE_GUARD;
        case CRE_SPI_A: // Dwarven Spirit Guide
        case CRE_SPI_H: // Taunka Spirit Guide
            return CREATURE_SPIRIT_GUIDE;
        case 6491: // Spirit Healers
            return CREATURE_SPIRIT_HEALER;
        case 31101: case 31051: // Hoodoo Master & Sorceress
        case 31102: case 31052: // Vieron Blazefeather & Bowyer
        case 31107: case 31109: // Lieutenant & Senior Demolitionist
        case 31151: case 31153: // Tactical Officer
        case 31106: case 31108: // Siegesmith & Siege Master
        case 31053: case 31054: // Primalist & Anchorite
        case 31091: case 31036: // Commander
            return CREATURE_QUESTGIVER;
        case 32615: case 32626: // Warbringer && Brigadier General
        case 32296: case 32294: // Quartermaster
        case 39173: case 39172: // Ros'slai && Marshal Magruder
        case 30870: case 30869: // Flight Masters
            return CREATURE_SPECIAL;
        default:
            return CREATURE_OTHER;
    }
}

void OutdoorPvPWG::OnCreatureCreate(Creature* pCreature)
{
    uint32 entry = pCreature->GetEntry();
    switch(GetCreatureType(entry))
    {
        case CREATURE_SIEGE_VEHICLE:
        {
            if(!pCreature->isSummon())
                return;

            TeamId team;
            if(pCreature->getFaction() == WintergraspFaction[TEAM_ALLIANCE])
                team = TEAM_ALLIANCE;
            else if(pCreature->getFaction() == WintergraspFaction[TEAM_HORDE])
                team = TEAM_HORDE;
            else return;

            if(uint32 engLowguid = GUID_LOPART(((TempSummon*)pCreature)->GetSummonerGUID()))
            {
                if(OPvPCapturePointWG* pPWG = GetWorkshopByEngGuid(engLowguid))
                {
                    if(CanBuildVehicle(pPWG))
                        m_vehicles[team].insert(pCreature);
                    else {
                        pCreature->setDeathState(DEAD);
                        pCreature->SetRespawnTime(DAY);
                        return;
                    }
                }

                if(m_tenacityStack > 0 && team == TEAM_ALLIANCE)
                    pCreature->SetAuraStack(SPELL_TENACITY_VEHICLE, pCreature, m_tenacityStack);
                else if(m_tenacityStack < 0 && team == TEAM_HORDE)
                    pCreature->SetAuraStack(SPELL_TENACITY_VEHICLE, pCreature, -m_tenacityStack);
            }
            SendUpdateWorldState(VehNumWorldState[team], m_vehicles[team].size());
            break;
        }
        case CREATURE_QUESTGIVER:
            m_questgivers[pCreature->GetDBTableGUIDLow()] = pCreature;
            pCreature->SetReactState(REACT_PASSIVE);
            break;
        case CREATURE_ENGINEER:
            for(OutdoorPvP::OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
            {
                if(OPvPCapturePointWG* pPWG = dynamic_cast<OPvPCapturePointWG*>(itr->second))
                {
                    if(pPWG->m_engGuid == pCreature->GetDBTableGUIDLow())
                    {
                        pPWG->m_engineer = pCreature;
                        break;
                    }
                }
            }
            break;
        case CREATURE_SPIRIT_GUIDE:
        {
            for(OutdoorPvP::OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
            {
                if(OPvPCapturePointWG* pPWG = dynamic_cast<OPvPCapturePointWG*>(itr->second))
                {
                    if(pPWG->m_spiGuid == pCreature->GetDBTableGUIDLow())
                    {
                        pPWG->m_spiritguide = pCreature;
                        break;
                    }
                }
            }
            pCreature->CastSpell(pCreature, SPELL_SPIRITUAL_IMMUNITY, true);
        }
        case CREATURE_SPIRIT_HEALER:
        case CREATURE_TURRET:
        case CREATURE_OTHER:
            UpdateCreatureInfo(pCreature);
        default:
            m_creatures.insert(pCreature);
            break;
    }
}

void OutdoorPvPWG::OnCreatureRemove(Creature* pCreature)
{
    uint32 entry = pCreature->GetEntry();
    switch(GetCreatureType(entry))
    {
        case CREATURE_SIEGE_VEHICLE:
        {
            if(!pCreature->isSummon())
                return;

            TeamId team;

            if(m_vehicles[TEAM_ALLIANCE].erase(pCreature))
                team = TEAM_ALLIANCE;
            else if(m_vehicles[TEAM_HORDE].erase(pCreature))
                team = TEAM_HORDE;
            else
                return;

            SendUpdateWorldState(VehNumWorldState[team], m_vehicles[team].size());
            break;
        }
        case CREATURE_QUESTGIVER:
            m_questgivers.erase(pCreature->GetDBTableGUIDLow());
            break;
        case CREATURE_ENGINEER:
            for(OutdoorPvP::OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
            {
                if(OPvPCapturePointWG* pPWG = dynamic_cast<OPvPCapturePointWG*>(itr->second))
                {
                    if(pPWG->m_engGuid == pCreature->GetDBTableGUIDLow())
                    {
                        pPWG->m_engineer = NULL;
                        break;
                    }
                }
            }
            m_creatures.erase(pCreature);
            break;
        case CREATURE_SPIRIT_GUIDE:
            for(OutdoorPvP::OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
            {
                if(OPvPCapturePointWG* pPWG = dynamic_cast<OPvPCapturePointWG*>(itr->second))
                {
                    if(pPWG->m_spiGuid == pCreature->GetDBTableGUIDLow())
                    {
                        pPWG->m_spiritguide = NULL;
                        break;
                    }
                }
            }
        default:
            m_creatures.erase(pCreature);
            break;
    }
}

void OutdoorPvPWG::OnGameObjectCreate(GameObject* pGo)
{
    OutdoorPvP::OnGameObjectCreate(pGo);

    if(UpdateGameObjectInfo(pGo))
        m_gobjects.insert(pGo);

    else if(pGo->GetGoType() == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING)
    {
        BuildingStateMap::const_iterator itr = m_buildingStates.find(pGo->GetDBTableGUIDLow());
        if(itr != m_buildingStates.end())
        {
            itr->second->building = pGo;
            if(pGo->GetGOInfo()->displayId == 7878 || pGo->GetGOInfo()->displayId == 7900)
                itr->second->type = BUILDING_TOWER;
            if(itr->second->damageState == DAMAGE_INTACT && !itr->second->health) {
                itr->second->health = pGo->GetGOValue()->Building.Health;
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_DESTROYED);
            } else {
                pGo->GetGOValue()->Building.Health = itr->second->health;
                if(itr->second->damageState == DAMAGE_DAMAGED)
                    pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_DAMAGED);
                else if(itr->second->damageState == DAMAGE_DESTROYED)
                    pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_DESTROYED);
            }
        }
    }
}

void OutdoorPvPWG::OnGameObjectRemove(GameObject* pGo)
{
    OutdoorPvP::OnGameObjectRemove(pGo);

    if(UpdateGameObjectInfo(pGo))
        m_gobjects.erase(pGo);

    else if(pGo->GetGoType() == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING) {
        BuildingStateMap::const_iterator itr = m_buildingStates.find(pGo->GetDBTableGUIDLow());
        if(itr != m_buildingStates.end())
            itr->second->building = NULL;
    }
}

void OutdoorPvPWG::UpdateAllWorldObject()
{
    for(GameObjectSet::iterator itr = m_gobjects.begin(); itr != m_gobjects.end(); ++itr)
        UpdateGameObjectInfo(*itr);
    for(CreatureSet::iterator itr = m_creatures.begin(); itr != m_creatures.end(); ++itr)
        UpdateCreatureInfo(*itr);
    for(QuestGiverMap::iterator itr = m_questgivers.begin(); itr != m_questgivers.end(); ++itr)
        UpdateQuestGiverPosition((*itr).first, (*itr).second);

    RebuildAllBuildings();

    for(OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if(OPvPCapturePointWG *workshop = dynamic_cast<OPvPCapturePointWG*>(itr->second))
            workshop->SetTeamByBuildingState();
}

void OutdoorPvPWG::RebuildAllBuildings()
{
    for(BuildingStateMap::const_iterator itr = m_buildingStates.begin(); itr != m_buildingStates.end(); ++itr)
    {
        if(itr->second->building && itr->second->building->GetGoType() == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING) {
            UpdateGameObjectInfo(itr->second->building);
            itr->second->building->SetDestructibleState(GO_DESTRUCTIBLE_REBUILDING, NULL, true);
            itr->second->health = itr->second->building->GetGOValue()->Building.Health;
            itr->second->damageState = DAMAGE_INTACT;
        } else itr->second->health = 0;
        itr->second->SetTeam(getDefenderTeam() == TEAM_ALLIANCE ? OTHER_TEAM(itr->second->defaultTeam) : itr->second->defaultTeam);
    }
    m_towerDamagedCount[TEAM_ALLIANCE]      = 0;
    m_towerDestroyedCount[TEAM_ALLIANCE]    = 0;
    m_towerDamagedCount[TEAM_HORDE]         = 0;
    m_towerDestroyedCount[TEAM_HORDE]       = 0;
}

void OutdoorPvPWG::SendInitWorldStatesTo(Player* pPlayer) const
{
    WorldPacket data(SMSG_INIT_WORLD_STATES, (4+4+4+2+(m_buildingStates.size()*8)));
    data << uint32(571);
    data << uint32(ZONE_WINTERGRASP);
    data << uint32(0);
    data << uint16(4+2+4+m_buildingStates.size());

    data << uint32(3803) << uint32(getDefenderTeam() == TEAM_ALLIANCE ? 1 : 0);
    data << uint32(3802) << uint32(getDefenderTeam() != TEAM_ALLIANCE ? 1 : 0);
    data << uint32(3801) << uint32(isWarTime() ? 0 : 1);
    data << uint32(3710) << uint32(isWarTime() ? 1 : 0);

    for(uint32 i = 0; i < 2; ++i)
        data << ClockWorldState[i] << m_clock[i];

    data << uint32(3490) << uint32(m_vehicles[TEAM_HORDE].size());
    data << uint32(3491) << m_workshopCount[TEAM_HORDE] * MAX_VEHICLE_PER_WORKSHOP;
    data << uint32(3680) << uint32(m_vehicles[TEAM_ALLIANCE].size());
    data << uint32(3681) << m_workshopCount[TEAM_ALLIANCE] * MAX_VEHICLE_PER_WORKSHOP;

    for(BuildingStateMap::const_iterator itr = m_buildingStates.begin(); itr != m_buildingStates.end(); ++itr)
        itr->second->FillData(data);

    if(pPlayer)
        pPlayer->GetSession()->SendPacket(&data);
    else
        BroadcastPacket(data);
}

void OutdoorPvPWG::BroadcastStateChange(BuildingState* pState) const
{
    if(m_sendUpdate) {
        for(uint32 team = 0; team < 2; ++team) {
            for(PlayerSet::const_iterator p_itr = m_players[team].begin(); p_itr != m_players[team].end(); ++p_itr)
                pState->SendUpdate(*p_itr);
        }
    }
}

bool OutdoorPvPWG::UpdateCreatureInfo(Creature* pCreature)
{
    if(!pCreature)
        return false;

    uint32 entry = pCreature->GetEntry();
    switch(GetCreatureType(entry))
    {
        case CREATURE_TURRET:
            if(isWarTime()) {
                if(!pCreature->isAlive())
                    pCreature->Respawn(true);
                pCreature->setFaction(WintergraspFaction[getDefenderTeam()]);
            } else {
                pCreature->GetVehicleKit()->RemoveAllPassengers();
                pCreature->DisappearAndDie();
            }
            return false;
        case CREATURE_OTHER:
            if(isWarTime()) {
                pCreature->SetVisible(false);
                pCreature->setFaction(35);
            } else {
                pCreature->RestoreFaction();
                pCreature->SetVisible(true);
            }
            return false;
        case CREATURE_SPIRIT_GUIDE:
            pCreature->SetVisible(isWarTime() ? true : false);
            return false;
        case CREATURE_SPIRIT_HEALER:
            pCreature->SetVisible(isWarTime() ? false : true);
            return false;
        case CREATURE_SIEGE_VEHICLE:
            if(!isWarTime()) {
                if(pCreature->IsVehicle() && pCreature->GetVehicleKit())
                    pCreature->GetVehicleKit()->RemoveAllPassengers();
                pCreature->DisappearAndDie();
            }
            return false;
        case CREATURE_QUESTGIVER:
        case CREATURE_SPECIAL:
        case CREATURE_GUARD:
            _RespawnCreatureIfNeeded(pCreature, entry);
            pCreature->AI()->EnterEvadeMode();
            return false;
        default:
            return false;
    }
}

bool OutdoorPvPWG::UpdateQuestGiverPosition(uint32 guid, Creature* pCreature)
{
    assert(guid);
    Position pos = m_qgPosMap[std::pair<uint32, bool>(guid, getDefenderTeam() == TEAM_HORDE)];

    if(pCreature && pCreature->IsInWorld())
    {
        if(pCreature->GetPositionX() == pos.GetPositionX() &&
            pCreature->GetPositionY() == pos.GetPositionY() &&
            pCreature->GetPositionZ() == pos.GetPositionZ())
            return false;

        if(!pCreature->isAlive())
            pCreature->Respawn(true);

        if(pCreature->isAlive() && pCreature->isInCombat()) {
            pCreature->CombatStop(true);
            pCreature->getHostileRefManager().deleteReferences();
        }

        pCreature->SetHomePosition(pos);

        if(pCreature->GetEntry() != 30400 || pCreature->GetEntry() != 30499)
           pCreature->SetReactState(REACT_AGGRESSIVE);

        pCreature->DestroyForNearbyPlayers();

        if(!pCreature->GetMap()->IsLoaded(pos.GetPositionX(), pos.GetPositionY()))
            pCreature->GetMap()->LoadGrid(pos.GetPositionX(), pos.GetPositionY());

        pCreature->GetMap()->CreatureRelocation(pCreature, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation());
    } else sObjectMgr->MoveCreData(guid, 571, pos);
    return true;
}

bool OutdoorPvPWG::UpdateGameObjectInfo(GameObject* pGo) const
{
    uint32 attFaction;
    uint32 defFaction;

    if(isWarTime())
    {
        attFaction = WintergraspFaction[getAttackerTeam()];
        defFaction = WintergraspFaction[getDefenderTeam()];
    } else {
        attFaction = 35;
        defFaction = 35;
    }

    if(!pGo || !pGo->GetGOInfo())
       return true;

    switch(pGo->GetGOInfo()->displayId)
    {
        case 8244: // Defender's Portal - Vehicle Teleporter
            pGo->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[getDefenderTeam()]);
            return true;
        case 7967: // Titan relic
            pGo->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[getAttackerTeam()]);
            return true;
        case 8165: // Wintergrasp Keep Door
        case 7877: // Wintergrasp Fortress Wall
        case 7878: // Wintergrasp Keep Tower
        case 7906: // Wintergrasp Fortress Gate
        case 7909: // Wintergrasp Wall
            pGo->SetUInt32Value(GAMEOBJECT_FACTION, defFaction);
            return false;
        case 8256: //Alliance Banner
        case 5651: //Alliance Banner
            if(getDefenderTeam() == TEAM_ALLIANCE)
            {
                if(pGo->GetAreaId() == 4575 || pGo->GetAreaId() == 4539 || pGo->GetAreaId() == 4538)
                    pGo->SetPhaseMask(1, true);
                else pGo->SetPhaseMask(2, true);
            } else {
                if(pGo->GetAreaId() == 4575 || pGo->GetAreaId() == 4539 || pGo->GetAreaId() == 4538)
                    pGo->SetPhaseMask(2, true);
                else pGo->SetPhaseMask(1, true);
            }
            return true;
        case 8257: //Horde Banner
        case 5652: //Horde Banner
            if(getDefenderTeam() == TEAM_ALLIANCE)
            {
                if(pGo->GetAreaId() == 4575 || pGo->GetAreaId() == 4539 || pGo->GetAreaId() == 4538)
                    pGo->SetPhaseMask(2, true);
                else pGo->SetPhaseMask(1, true);
            } else {
                if(pGo->GetAreaId() == 4575 || pGo->GetAreaId() == 4539 || pGo->GetAreaId() == 4538)
                    pGo->SetPhaseMask(1, true);
                else pGo->SetPhaseMask(2, true);
            }
            return true;
        case 7900: // Flamewatch Tower - Shadowsight Tower - Winter's Edge Tower
            pGo->SetUInt32Value(GAMEOBJECT_FACTION, attFaction);
            return false;
        case 8208: // Goblin Workshop
            if(OPvPCapturePointWG* pPWG = GetWorkshopByGOGuid(pGo->GetGUID()))
                pGo->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[pPWG->m_buildingState->GetTeam()]);
            return false;
    }

    TeamPairMap::const_iterator itr = m_goDisplayPair.find(pGo->GetGOInfo()->displayId);
    if(itr != m_goDisplayPair.end())
    {
        pGo->SetUInt32Value(GAMEOBJECT_DISPLAYID, getDefenderTeam() == TEAM_ALLIANCE ? itr->second : itr->first);
        return true;
    }
    return false;
}

void OutdoorPvPWG::HandlePlayerEnterZone(Player* pPlayer, uint32 zone)
{
    if(!sWorld->getBoolConfig(CONFIG_OUTDOORPVP_WINTERGRASP_ENABLED))
        return;

    if(isWarTime())
    {
        if(pPlayer->getLevel() < 75)
        {
            pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_DALARAN, true);
            return;
        }

        if(!pPlayer->HasAura(SPELL_RECRUIT)
        && !pPlayer->HasAura(SPELL_CORPORAL)
        && !pPlayer->HasAura(SPELL_LIEUTENANT))
            pPlayer->CastSpell(pPlayer, SPELL_RECRUIT, true);

        if(pPlayer->GetTeamId() == getAttackerTeam())
        {
            if(m_towerDestroyedCount[getAttackerTeam()] < 3)
                pPlayer->SetAuraStack(SPELL_TOWER_CONTROL, pPlayer, 3 - m_towerDestroyedCount[getAttackerTeam()]);
        } else if(m_towerDestroyedCount[getAttackerTeam()])
            pPlayer->SetAuraStack(SPELL_TOWER_CONTROL, pPlayer, m_towerDestroyedCount[getAttackerTeam()]);
    }

    SendInitWorldStatesTo(pPlayer);
    OutdoorPvP::HandlePlayerEnterZone(pPlayer, zone);
    UpdateTenacityStack();
}

void OutdoorPvPWG::CheckWGAuraJOIN(Player* pPlayer)
{
    pPlayer->AddAura(55773, pPlayer);
    pPlayer->AddAura(55774, pPlayer);
}

void OutdoorPvPWG::CheckWGAuraLEFT(Player* pPlayer)
{
    pPlayer->RemoveAura(55773);
    pPlayer->RemoveAura(55774);
}

void OutdoorPvPWG::HandlePlayerResurrects(Player* pPlayer, uint32 zone)
{
    if(!sWorld->getBoolConfig(CONFIG_OUTDOORPVP_WINTERGRASP_ENABLED))
        return;

    CheckWGAuraJOIN(pPlayer);

    if(isWarTime())
    {
        if(pPlayer->getLevel() > 74)
        {
            if(pPlayer->GetTeamId() == TEAM_ALLIANCE && m_tenacityStack > 0 || pPlayer->GetTeamId() == TEAM_HORDE && m_tenacityStack < 0)
            {
                if(pPlayer->HasAura(SPELL_TENACITY))
                    pPlayer->RemoveAurasDueToSpell(SPELL_TENACITY);

                int32 newStack = m_tenacityStack < 0 ? -m_tenacityStack : m_tenacityStack;
                if(newStack > 20)
                    newStack = 20;
                pPlayer->SetAuraStack(SPELL_TENACITY, pPlayer, newStack);
            }

            if(pPlayer->GetTeamId() == getAttackerTeam())
            {
                if(m_towerDestroyedCount[getAttackerTeam()] < 3)
                    pPlayer->SetAuraStack(SPELL_TOWER_CONTROL, pPlayer, 3 - m_towerDestroyedCount[getAttackerTeam()]);
            } else if(m_towerDestroyedCount[getAttackerTeam()])
                pPlayer->SetAuraStack(SPELL_TOWER_CONTROL, pPlayer, m_towerDestroyedCount[getAttackerTeam()]);
        }
    }
    OutdoorPvP::HandlePlayerResurrects(pPlayer, zone);
}

void OutdoorPvPWG::HandlePlayerLeaveZone(Player* pPlayer, uint32 zone)
{
    if(!sWorld->getBoolConfig(CONFIG_OUTDOORPVP_WINTERGRASP_ENABLED))
        return;

    CheckWGAuraLEFT(pPlayer);

    if(!pPlayer->GetSession()->PlayerLogout())
    {
        if(pPlayer->GetVehicle())
            pPlayer->GetVehicle()->Dismiss();
        pPlayer->RemoveAurasDueToSpell(SPELL_RECRUIT);
        pPlayer->RemoveAurasDueToSpell(SPELL_CORPORAL);
        pPlayer->RemoveAurasDueToSpell(SPELL_LIEUTENANT);
        pPlayer->RemoveAurasDueToSpell(SPELL_TOWER_CONTROL);
        pPlayer->RemoveAurasDueToSpell(SPELL_SPIRITUAL_IMMUNITY);
    }
    pPlayer->RemoveAurasDueToSpell(SPELL_TENACITY);
    OutdoorPvP::HandlePlayerLeaveZone(pPlayer, zone);
    UpdateTenacityStack();
}

void OutdoorPvPWG::PromotePlayer(Player* pKiller) const
{
    Aura* pAura;
    pAura = pKiller->GetAura(SPELL_RECRUIT);
    if(pAura) {
        if(pAura->GetStackAmount() >= 5) {
            pKiller->RemoveAura(SPELL_RECRUIT);
            pKiller->CastSpell(pKiller, SPELL_CORPORAL, true);
            ChatHandler(pKiller).PSendSysMessage(LANG_BG_WG_RANK1);
        } else pKiller->CastSpell(pKiller, SPELL_RECRUIT, true);
    } else {
        pAura = pKiller->GetAura(SPELL_CORPORAL);
        if(pAura) {
            if(pAura->GetStackAmount() >= 5) {
                pKiller->RemoveAura(SPELL_CORPORAL);
                pKiller->CastSpell(pKiller, SPELL_LIEUTENANT, true);
                ChatHandler(pKiller).PSendSysMessage(LANG_BG_WG_RANK2);
            } else pKiller->CastSpell(pKiller, SPELL_CORPORAL, true);
        }
    }
}

void OutdoorPvPWG::HandleKill(Player* pKiller, Unit* pVictim)
{
    if(!sWorld->getBoolConfig(CONFIG_OUTDOORPVP_WINTERGRASP_ENABLED) || !isWarTime())
        return;

    bool ok = false;
    if(pVictim->GetTypeId() == TYPEID_PLAYER)
    {
        if(pVictim->getLevel() >= 74)
            ok = true;
        pKiller->RewardPlayerAndGroupAtEvent(CRE_PVP_KILL, pVictim);
    } else {
        switch(GetCreatureType(pVictim->GetEntry()))
        {
            case CREATURE_SIEGE_VEHICLE:
                pKiller->RewardPlayerAndGroupAtEvent(CRE_PVP_KILL_V, pVictim);
                ok = true;
                break;
            case CREATURE_QUESTGIVER:
            case CREATURE_GUARD:
            case CREATURE_SPECIAL:
                pKiller->RewardPlayerAndGroupAtEvent(CRE_PVP_KILL, pVictim);
                ok = true;
                break;
            case CREATURE_TURRET:
                ok = true;
                break;
            default:
                break;
        }
    }

    Group* pGroup = pKiller->GetGroup();
    if(ok)
    {
        if(pGroup) {
            for(GroupReference* itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next()) {
                if(itr->getSource()->IsAtGroupRewardDistance(pKiller) && itr->getSource()->getLevel() > 74)
                    PromotePlayer(itr->getSource());
            }
        } else if(pKiller->getLevel() > 74)
            PromotePlayer(pKiller);
    }
}

void OutdoorPvPWG::UpdateTenacityStack()
{
    if(!isWarTime())
        return;

    TeamId team         = TEAM_NEUTRAL;
    uint32 allianceNum  = 0;
    uint32 hordeNum     = 0;
    int32 newStack      = 0;

    for(PlayerSet::iterator itr = m_players[TEAM_ALLIANCE].begin(); itr != m_players[TEAM_ALLIANCE].end(); ++itr)
        if((*itr)->getLevel() > 74)
            ++allianceNum;

    for(PlayerSet::iterator itr = m_players[TEAM_HORDE].begin(); itr != m_players[TEAM_HORDE].end(); ++itr)
        if((*itr)->getLevel() > 74)
            ++hordeNum;

    if(allianceNum && hordeNum)
    {
        if(allianceNum < hordeNum)
            newStack = int32((float(hordeNum) / float(allianceNum) - 1)*4);
        else if(allianceNum > hordeNum)
            newStack = int32((1 - float(allianceNum) / float(hordeNum))*4);
    }

    if(newStack == m_tenacityStack)
        return;

    if(m_tenacityStack > 0 && newStack <= 0)
        team = TEAM_ALLIANCE;
    else if(m_tenacityStack < 0 && newStack >= 0)
        team = TEAM_HORDE;

    m_tenacityStack = newStack;

    if(team != TEAM_NEUTRAL) {
        for(PlayerSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
            if((*itr)->getLevel() > 74)
                (*itr)->RemoveAurasDueToSpell(SPELL_TENACITY);

        for(CreatureSet::const_iterator itr = m_vehicles[team].begin(); itr != m_vehicles[team].end(); ++itr)
            (*itr)->RemoveAurasDueToSpell(SPELL_TENACITY_VEHICLE);
    }

    if(newStack)
    {
        team = newStack > 0 ? TEAM_ALLIANCE : TEAM_HORDE;
        if(newStack < 0)
            newStack = -newStack;
        if(newStack > 20)
            newStack = 20;

        for(PlayerSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
            if((*itr)->getLevel() > 74)
                (*itr)->SetAuraStack(SPELL_TENACITY, (*itr), newStack);

        for(CreatureSet::const_iterator itr = m_vehicles[team].begin(); itr != m_vehicles[team].end(); ++itr)
            (*itr)->SetAuraStack(SPELL_TENACITY_VEHICLE, (*itr), newStack);
    }
}

void OutdoorPvPWG::UpdateClockDigit(uint32 &timer, uint32 digit, uint32 mod)
{
    uint32 value = timer%mod;
    if(m_clock[digit] != value)
    {
        m_clock[digit] = value;
        SendUpdateWorldState(ClockWorldState[digit], uint32(timer + time(NULL)));
        sWorld->SetWintergrapsTimer(uint32(timer + time(NULL)), digit);
    }
}

void OutdoorPvPWG::UpdateClock()
{
    uint32 timer = m_timer / 1000;
    if(!isWarTime())
        UpdateClockDigit(timer, 1, 10);
    else
        UpdateClockDigit(timer, 0, 10);

    if((m_timer > 1800000) && (m_timer < 1801000) && (m_wartime == false))
        sWorld->SendWorldText(LANG_BG_WG_WORLD_ANNOUNCE_30);
    else if((m_timer > 600000) && (m_timer < 601000) && (m_wartime == false))
        sWorld->SendWorldText(LANG_BG_WG_WORLD_ANNOUNCE_10);
    else if((m_timer > 300000) && (m_timer < 301000) && (m_wartime == false))
        sWorld->SendWorldText(LANG_BG_WG_WORLD_ANNOUNCE_5);
    else if((m_timer > 120000) && (m_timer < 121000) && (m_wartime == false))
        sWorld->SendWorldText(LANG_BG_WG_WORLD_ANNOUNCE_2);
    else if((m_timer > 60000) && (m_timer < 61000) && (m_wartime == false))
        sWorld->SendWorldText(LANG_BG_WG_WORLD_ANNOUNCE_1);
}

bool OutdoorPvPWG::Update(uint32 diff)
{
    if(!sWorld->getBoolConfig(CONFIG_OUTDOORPVP_WINTERGRASP_ENABLED))
        return false;

    if(m_timer > (int32)diff)
    {
        m_timer -= (int32)diff;

        if(isWarTime())
        {
            OutdoorPvP::Update(diff);

            m_LastResurrectTime += diff;
            if(m_LastResurrectTime >= RESURRECTION_INTERVAL) {
                if(GetReviveQueueSize()) {
                    for(std::map<uint64, std::vector<uint64> >::iterator itr = m_ReviveQueue.begin(); itr != m_ReviveQueue.end(); ++itr) {
                        Creature* sh = NULL;
                        for(std::vector<uint64>::const_iterator itr2 = (itr->second).begin(); itr2 != (itr->second).end(); ++itr2)
                        {
                            Player* pPlayer = sObjectMgr->GetPlayer(*itr2);
                            if(!pPlayer)
                                continue;

                            if(!sh && pPlayer->IsInWorld()) {
                                sh = pPlayer->GetMap()->GetCreature(itr->first);
                                if(sh)
                                    sh->CastSpell(sh, SPELL_SPIRIT_HEAL, true);
                            }

                            if(sh && pPlayer->GetDistance2d(sh) <= 18.0f) {
                                pPlayer->CastSpell(pPlayer, SPELL_RESURRECTION_VISUAL, true);
                                m_ResurrectQueue.push_back(*itr2);
                            }
                        }
                        (itr->second).clear();
                    }

                    m_ReviveQueue.clear();
                    m_LastResurrectTime = 0;
                } else m_LastResurrectTime = 0;
            } else if(m_LastResurrectTime > 500) {
                for(std::vector<uint64>::const_iterator itr = m_ResurrectQueue.begin(); itr != m_ResurrectQueue.end(); ++itr) {
                    Player* pPlayer = sObjectMgr->GetPlayer(*itr);
                    if(!pPlayer)
                        continue;

                    pPlayer->ResurrectPlayer(1.0f);
                    pPlayer->CastSpell(pPlayer, 6962, true);
                    pPlayer->CastSpell(pPlayer, SPELL_SPIRIT_HEAL_MANA, true);
                    sObjectAccessor->ConvertCorpseForPlayer(*itr);
                }
                m_ResurrectQueue.clear();
            }
        }
        UpdateClock();
    } else {
        m_sendUpdate = false;
        int32 entry = LANG_BG_WG_DEFENDED;

        if(m_changeDefender) {
            m_changeDefender = false;
            m_defender = getAttackerTeam();
            entry = LANG_BG_WG_CAPTURED;
            if(m_changeAlly == true || m_changeHorde == true) {
                RebuildAllBuildings();
                m_changeAlly = false;
                m_changeHorde = false;
            }
        }

        if(isWarTime())
        {
            if(m_timer != 1)
                sWorld->SendZoneText(ZONE_WINTERGRASP, fmtstring(sObjectMgr->GetTrinityStringForDBCLocale(entry), sObjectMgr->GetTrinityStringForDBCLocale(getDefenderTeam() == TEAM_ALLIANCE ? LANG_BG_AB_ALLY : LANG_BG_AB_HORDE)));
            EndBattle();
        } else {
            if(m_timer != 1)
                sWorld->SendZoneText(ZONE_WINTERGRASP, sObjectMgr->GetTrinityStringForDBCLocale(LANG_BG_WG_BATTLE_STARTS));
            StartBattle();
        }

        UpdateAllWorldObject();
        UpdateClock();
        SendInitWorldStatesTo();
        m_sendUpdate = true;
    }

    if(m_WSSaveTimer < diff) {
        sWorld->setWorldState(WORLDSTATE_WINTERGRASP_WARTIME, m_wartime);
        sWorld->setWorldState(WORLDSTATE_WINTERGRASP_TIMER, m_timer);
        sWorld->setWorldState(WORLDSTATE_WINTERGRASP_DEFENDERS, m_defender);
        m_WSSaveTimer = sWorld->getIntConfig(CONFIG_OUTDOORPVP_WINTERGRASP_SAVESTATE_PERIOD);
    } else m_WSSaveTimer -= diff;

   return false;
}

void OutdoorPvPWG::forceStartBattle()
{
    m_wartime = false;

    if(m_timer != 1) {
        m_timer = 1;
        sWorld->SendZoneText(ZONE_WINTERGRASP, sObjectMgr->GetTrinityStringForDBCLocale(LANG_BG_WG_BATTLE_FORCE_START));
    }
    sWorld->SendWintergraspState();
}

void OutdoorPvPWG::forceStopBattle()
{
    if(!isWarTime())
        m_wartime = true;
    if(m_timer != 1) {
        m_timer = 1;
        sWorld->SendZoneText(ZONE_WINTERGRASP, sObjectMgr->GetTrinityStringForDBCLocale(LANG_BG_WG_BATTLE_FORCE_STOP));
    }
    sWorld->SendWintergraspState();
}

void OutdoorPvPWG::forceChangeTeam()
{
    m_changeDefender    = true;
    m_timer             = 1;
    m_changeAlly        = true;
    m_changeHorde       = true;

    sWorld->SendZoneText(ZONE_WINTERGRASP, fmtstring(sObjectMgr->GetTrinityStringForDBCLocale(LANG_BG_WG_SWITCH_FACTION), sObjectMgr->GetTrinityStringForDBCLocale(getAttackerTeam() == TEAM_ALLIANCE ? LANG_BG_AB_ALLY : LANG_BG_AB_HORDE)));
    if(isWarTime())
        forceStartBattle();
    else
        forceStopBattle();
}

void OutdoorPvPWG::StartBattle()
{
    uint32 CountDef = 0;
    uint32 CountAtk = 0;
    m_wartime = true;
    m_timer = sWorld->getIntConfig(CONFIG_OUTDOORPVP_WINTERGRASP_BATTLE_TIME) * MINUTE * IN_MILLISECONDS;

    //Remove Essence of Wintergrasp to all players
    sWorld->setWorldState(WORLDSTATE_WINTERGRASP_CONTROLING_FACTION, TEAM_NEUTRAL);
    sWorld->UpdateAreaDependentAuras();

    for(PlayerSet::iterator itr = m_players[getAttackerTeam()].begin(); itr != m_players[getAttackerTeam()].end(); ++itr) {
        (*itr)->RemoveAurasDueToSpell(SPELL_RECRUIT);
        (*itr)->RemoveAurasDueToSpell(SPELL_CORPORAL);
        (*itr)->RemoveAurasDueToSpell(SPELL_LIEUTENANT);
        (*itr)->RemoveAurasDueToSpell(SPELL_TOWER_CONTROL);
        (*itr)->RemoveAurasDueToSpell(SPELL_SPIRITUAL_IMMUNITY);
        if((*itr)->getLevel() < 75)
            (*itr)->CastSpell((*itr), SPELL_TELEPORT_DALARAN, true);
        else {
            CountAtk++;
            (*itr)->RemoveAurasByType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED);
            (*itr)->RemoveAurasByType(SPELL_AURA_FLY);
            (*itr)->CastSpell((*itr), 45472, true);
            (*itr)->PlayDirectSound(OutdoorPvP_WG_SOUND_START_BATTLE);
            (*itr)->SetAuraStack(SPELL_TOWER_CONTROL, (*itr), 3);
            (*itr)->CastSpell(*itr, SPELL_RECRUIT, true);
        }
    }

    // Remove All Wintergrasp auras. Add Recruit rank
    for(PlayerSet::iterator itr = m_players[getDefenderTeam()].begin(); itr != m_players[getDefenderTeam()].end(); ++itr) {
        (*itr)->RemoveAurasDueToSpell(SPELL_RECRUIT);
        (*itr)->RemoveAurasDueToSpell(SPELL_CORPORAL);
        (*itr)->RemoveAurasDueToSpell(SPELL_LIEUTENANT);
        (*itr)->RemoveAurasDueToSpell(SPELL_TOWER_CONTROL);
        (*itr)->RemoveAurasDueToSpell(SPELL_SPIRITUAL_IMMUNITY);
        if((*itr)->getLevel() < 75)
            (*itr)->CastSpell((*itr), SPELL_TELEPORT_DALARAN, true);
        else {
            CountDef++;
            (*itr)->RemoveAurasByType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED);
            (*itr)->RemoveAurasByType(SPELL_AURA_FLY);
            (*itr)->CastSpell((*itr), 45472, true); //prevent die if fall
            (*itr)->PlayDirectSound(OutdoorPvP_WG_SOUND_START_BATTLE); // START Battle
            (*itr)->CastSpell(*itr, SPELL_RECRUIT, true);
        }
    }

    UpdateTenacityStack();

    sWorld->SendWintergraspState();
}

void OutdoorPvPWG::EndBattle()
{
    sWorld->setWorldState(WORLDSTATE_WINTERGRASP_CONTROLING_FACTION, getDefenderTeam());
    sWorld->UpdateAreaDependentAuras();

    for(PlayerSet::iterator itr = m_players[getDefenderTeam()].begin(); itr != m_players[getDefenderTeam()].end(); ++itr) {
        TeamIDsound = getDefenderTeam() == TEAM_ALLIANCE ? OutdoorPvP_WG_SOUND_ALLIANCE_WINS : OutdoorPvP_WG_SOUND_HORDE_WINS;
        (*itr)->PlayDirectSound(TeamIDsound);
    }

    for(PlayerSet::iterator itr = m_players[getAttackerTeam()].begin(); itr != m_players[getAttackerTeam()].end(); ++itr)
        (*itr)->PlayDirectSound(OutdoorPvP_WG_SOUND_NEAR_VICTORY);

    for(int team = 0; team < 2; ++team)
    {
        while(!m_vehicles[team].empty()) {
            Creature* veh = *m_vehicles[team].begin();
            m_vehicles[team].erase(m_vehicles[team].begin());
            veh->setDeathState(JUST_DIED);
        }

        if(m_players[team].empty())
            continue;

        for(PlayerSet::iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr) {
            if((*itr)->isDead()) {
                (*itr)->ResurrectPlayer(1.0f);
                sObjectAccessor->ConvertCorpseForPlayer((*itr)->GetGUID());
            }
            (*itr)->RemoveAurasDueToSpell(SPELL_TENACITY);
            (*itr)->CombatStop(true);
            (*itr)->getHostileRefManager().deleteReferences();
        }

        if(m_timer == 1) {
            for(PlayerSet::iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr) {
                (*itr)->RemoveAurasDueToSpell(SPELL_RECRUIT);
                (*itr)->RemoveAurasDueToSpell(SPELL_CORPORAL);
                (*itr)->RemoveAurasDueToSpell(SPELL_LIEUTENANT);
                (*itr)->RemoveAurasDueToSpell(SPELL_TOWER_CONTROL);
                (*itr)->RemoveAurasDueToSpell(SPELL_SPIRITUAL_IMMUNITY);
            }
            continue;
        }

        uint32 intactNum = 0;
        uint32 damagedNum = 0;

        for(OutdoorPvP::OPvPCapturePointMap::const_iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr) {
            if(OPvPCapturePointWG *workshop = dynamic_cast<OPvPCapturePointWG*>(itr->second)) {
                if(workshop->m_buildingState->GetTeam() == team) {
                    if(workshop->m_buildingState->damageState == DAMAGE_DAMAGED)
                        ++damagedNum;
                    else if(workshop->m_buildingState->damageState == DAMAGE_INTACT)
                        ++intactNum;
                }
            }
        }
        uint32 spellRewardId = team == getDefenderTeam() ? SPELL_VICTORY_REWARD : SPELL_DEFEAT_REWARD;
        uint32 baseHonor = 0;
        uint32 marks = 0;
        uint32 playersWithRankNum = 0;
        uint32 honor = 0;

        if(sWorld->getBoolConfig(CONFIG_OUTDOORPVP_WINTERGRASP_CUSTOM_HONOR)) {
            for(PlayerSet::iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr) {
                if((*itr)->getLevel() > 74 && ((*itr)->HasAura(SPELL_LIEUTENANT) || (*itr)->HasAura(SPELL_CORPORAL)))
                    ++playersWithRankNum;
            }
            baseHonor = team == getDefenderTeam() ? sWorld->getIntConfig(CONFIG_OUTDOORPVP_WINTERGRASP_WIN_BATTLE) : sWorld->getIntConfig(CONFIG_OUTDOORPVP_WINTERGRASP_LOSE_BATTLE);
            baseHonor += (sWorld->getIntConfig(CONFIG_OUTDOORPVP_WINTERGRASP_DAMAGED_TOWER) * m_towerDamagedCount[OTHER_TEAM(team)]);
            baseHonor += (sWorld->getIntConfig(CONFIG_OUTDOORPVP_WINTERGRASP_DESTROYED_TOWER) * m_towerDestroyedCount[OTHER_TEAM(team)]);
            baseHonor += (sWorld->getIntConfig(CONFIG_OUTDOORPVP_WINTERGRASP_INTACT_BUILDING) * intactNum);
            baseHonor += (sWorld->getIntConfig(CONFIG_OUTDOORPVP_WINTERGRASP_DAMAGED_BUILDING) * damagedNum);
            if(playersWithRankNum)
                baseHonor /= playersWithRankNum;
        }

        for(PlayerSet::iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr) {
            if((*itr)->getLevel() < 75)
                continue;

            if(sWorld->getBoolConfig(CONFIG_OUTDOORPVP_WINTERGRASP_CUSTOM_HONOR))
            {
                if(team == getDefenderTeam())
                {
                    if((*itr)->HasAura(SPELL_LIEUTENANT)) {
                        marks = 3;
                        honor = baseHonor;
                    } else if((*itr)->HasAura(SPELL_CORPORAL)) {
                        marks = 2;
                        honor = baseHonor;
                    } else {
                        marks = 1;
                        honor = 0;
                    }
                } else if((*itr)->HasAura(SPELL_LIEUTENANT)) {
                    marks = 1;
                    honor = baseHonor;
                } else if((*itr)->HasAura(SPELL_CORPORAL)) {
                    marks = 1;
                    honor = baseHonor;
                } else {
                    marks = 0;
                    honor = 0;
                }

                (*itr)->RewardHonor(NULL, 1, honor);
                RewardMarkOfHonor(*itr, marks);
                (*itr)->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, spellRewardId);
            } else if((*itr)->HasAura(SPELL_LIEUTENANT) || (*itr)->HasAura(SPELL_CORPORAL)) {
                (*itr)->CastSpell(*itr, spellRewardId, true);
                for(uint32 i = 0; i < intactNum; ++i)
                    (*itr)->CastSpell(*itr, SPELL_INTACT_BUILDING, true);
                for(uint32 i = 0; i < damagedNum; ++i)
                    (*itr)->CastSpell(*itr, SPELL_DAMAGED_BUILDING, true);
                for(uint32 i = 0; i < m_towerDamagedCount[OTHER_TEAM(team)]; ++i)
                    (*itr)->CastSpell(*itr, SPELL_DAMAGED_TOWER, true);
                for(uint32 i = 0; i < m_towerDestroyedCount[OTHER_TEAM(team)]; ++i)
                    (*itr)->CastSpell(*itr, SPELL_DESTROYED_TOWER, true);
            }
            if(team == getDefenderTeam()) {
                if((*itr)->HasAura(SPELL_LIEUTENANT) || (*itr)->HasAura(SPELL_CORPORAL)) {
                    (*itr)->AreaExploredOrEventHappens(A_VICTORY_IN_WG);
                    (*itr)->AreaExploredOrEventHappens(H_VICTORY_IN_WG);
                }
            }
            (*itr)->RemoveAurasDueToSpell(SPELL_RECRUIT);
            (*itr)->RemoveAurasDueToSpell(SPELL_CORPORAL);
            (*itr)->RemoveAurasDueToSpell(SPELL_LIEUTENANT);
            (*itr)->RemoveAurasDueToSpell(SPELL_TOWER_CONTROL);
            (*itr)->RemoveAurasDueToSpell(SPELL_SPIRITUAL_IMMUNITY);
        }
    }

    m_wartime = false;
    m_timer = sWorld->getIntConfig(CONFIG_OUTDOORPVP_WINTERGRASP_INTERVAL) * MINUTE * IN_MILLISECONDS;
    RemoveOfflinePlayerWGAuras();

    sWorld->SendWintergraspState();

    for(GameObjectSet::iterator itr = m_gobjects.begin(); itr != m_gobjects.end(); ++itr)
       UpdateGameObjectInfo(*itr);
}

bool OutdoorPvPWG::CanBuildVehicle(OPvPCapturePointWG* pPWG) const
{
    TeamId team = pPWG->m_buildingState->GetTeam();
    if(team == TEAM_NEUTRAL)
        return false;

    return isWarTime()
        && pPWG->m_buildingState->damageState != DAMAGE_DESTROYED
        && m_vehicles[team].size() < m_workshopCount[team] * MAX_VEHICLE_PER_WORKSHOP;
}

uint32 OutdoorPvPWG::GetData(uint32 id)
{
    if(OPvPCapturePointWG* pPWG = GetWorkshopByEngGuid(id))
        return CanBuildVehicle(pPWG) ? 1 : 0;
    return 0;
}

void OutdoorPvPWG::RewardMarkOfHonor(Player* pPlayer, uint32 count)
{
    if(pPlayer->HasAura(SPELL_AURA_PLAYER_INACTIVE))
        return;
    if(count == 0)
        return;

    ItemPosCountVec dest;
    uint32 no_space_count = 0;
    uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, WG_MARK_OF_HONOR, count, &no_space_count);

    if(msg == EQUIP_ERR_ITEM_NOT_FOUND) {
        sLog->outErrorDb("Wintergrasp reward item (Entry %u) not exist in `item_template`.", WG_MARK_OF_HONOR);
        return;
    }

    if(msg != EQUIP_ERR_OK)
        count -= no_space_count;

    if(count != 0 && !dest.empty())
        if(Item* pItem = pPlayer->StoreNewItem(dest, WG_MARK_OF_HONOR, true, 0))
            pPlayer->SendNewItem(pItem, count, true, false);
}

void OutdoorPvPWG::LoadQuestGiverMap(uint32 guid, Position posHorde, Position posAlli)
{
    m_qgPosMap[std::pair<uint32, bool>(guid, true)] = posHorde,
    m_qgPosMap[std::pair<uint32, bool>(guid, false)] = posAlli,
    m_questgivers[guid] = NULL;
    if(getDefenderTeam() == TEAM_ALLIANCE)
        sObjectMgr->MoveCreData(guid, 571, posAlli);
    if(getDefenderTeam() == TEAM_HORDE)
        sObjectMgr->MoveCreData(guid, 571, posHorde);
}

OPvPCapturePointWG* OutdoorPvPWG::GetWorkshop(uint32 lowguid) const
{
    if(OPvPCapturePoint* pCP = GetCapturePoint(lowguid))
        return dynamic_cast<OPvPCapturePointWG*>(pCP);
    return NULL;
}

OPvPCapturePointWG* OutdoorPvPWG::GetWorkshopByEngGuid(uint32 lowguid) const
{
    for(OutdoorPvP::OPvPCapturePointMap::const_iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
    {
        if(OPvPCapturePointWG* pPWG = dynamic_cast<OPvPCapturePointWG*>(itr->second))
        {
            if(pPWG->m_engGuid == lowguid)
                return pPWG;
        }
    }

    return NULL;
}

OPvPCapturePointWG* OutdoorPvPWG::GetWorkshopByGOGuid(uint64 lowguid) const
{
    for(OutdoorPvP::OPvPCapturePointMap::const_iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
    {
        if(OPvPCapturePointWG* pPWG = dynamic_cast<OPvPCapturePointWG*>(itr->second))
        {
            if(pPWG->m_workshopGuid == lowguid)
                return pPWG;
        }
    }

    return NULL;
}

void OutdoorPvPWG::SendAreaSpiritHealerQueryOpcode(Player* pPlayer, const uint64& guid)
{
    WorldPacket data(SMSG_AREA_SPIRIT_HEALER_TIME, 12);
    uint32 time_ = 30000 - GetLastResurrectTime();
    if(time_ == uint32(-1))
        time_ = 0;
    data << guid << time_;
    pPlayer->GetSession()->SendPacket(&data);
}

void OutdoorPvPWG::AddPlayerToResurrectQueue(uint64 npc_guid, uint64 player_guid)
{
    m_ReviveQueue[npc_guid].push_back(player_guid);

    Player* pPlayer = sObjectMgr->GetPlayer(player_guid);
    if(!pPlayer)
        return;

    pPlayer->CastSpell(pPlayer, SPELL_WAITING_FOR_RESURRECT, true);
}

void OutdoorPvPWG::RemovePlayerFromResurrectQueue(uint64 player_guid)
{
    for(std::map<uint64, std::vector<uint64> >::iterator itr = m_ReviveQueue.begin(); itr != m_ReviveQueue.end(); ++itr)
    {
        for(std::vector<uint64>::iterator itr2 =(itr->second).begin(); itr2 != (itr->second).end(); ++itr2)
        {
            if(*itr2 == player_guid)
            {
                (itr->second).erase(itr2);

                Player* plr = sObjectMgr->GetPlayer(player_guid);
                if(!plr)
                    return;

                plr->RemoveAurasDueToSpell(SPELL_WAITING_FOR_RESURRECT);

                return;
            }
        }
    }
}

void OutdoorPvPWG::RelocateAllianceDeadPlayers(Creature* pCreature)
{
    if(!pCreature || GetCreatureType(pCreature->GetEntry()) != CREATURE_SPIRIT_GUIDE)
        return;

    std::vector<uint64> ghost_list = m_ReviveQueue[pCreature->GetGUID()];
    if(!ghost_list.empty())
    {
        WorldSafeLocsEntry const *ClosestGrave = NULL;
        for(std::vector<uint64>::const_iterator itr = ghost_list.begin(); itr != ghost_list.end(); ++itr) {
            Player* pPlayer = sObjectMgr->GetPlayer(*itr);
            if(!pPlayer)
                continue;

            if(pPlayer->getFaction() == ALLIANCE) {
                if(ClosestGrave)
                    pPlayer->TeleportTo(pPlayer->GetMapId(), ClosestGrave->x, ClosestGrave->y, ClosestGrave->z, pPlayer->GetOrientation());
                else
                    ClosestGrave = sObjectMgr->GetClosestGraveYard(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetMapId(), pPlayer->GetTeam());
            }
        }
    }
}

void OutdoorPvPWG::RelocateHordeDeadPlayers(Creature* pCreature)
{
    if(!pCreature || GetCreatureType(pCreature->GetEntry()) != CREATURE_SPIRIT_GUIDE)
        return;

    std::vector<uint64> ghost_list = m_ReviveQueue[pCreature->GetGUID()];
    if(!ghost_list.empty())
    {
        WorldSafeLocsEntry const *ClosestGrave = NULL;
        for(std::vector<uint64>::const_iterator itr = ghost_list.begin(); itr != ghost_list.end(); ++itr) {
            Player* pPlayer = sObjectMgr->GetPlayer(*itr);
            if(!pPlayer)
                continue;
            if(pPlayer->getFaction() == HORDE) {
                if(ClosestGrave)
                    pPlayer->TeleportTo(pPlayer->GetMapId(), ClosestGrave->x, ClosestGrave->y, ClosestGrave->z, pPlayer->GetOrientation());
                else
                    ClosestGrave = sObjectMgr->GetClosestGraveYard(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetMapId(), pPlayer->GetTeam());
            }
        }
    }
}

OPvPCapturePointWG::OPvPCapturePointWG(OutdoorPvPWG *opvp, BuildingState *state) : OPvPCapturePoint(opvp), m_spiGuid(0), m_spiritguide(NULL), m_spiritguide_horde(NULL), m_spiritguide_alliance(NULL), m_engGuid(0), m_engineer(NULL), m_buildingState(state), m_wintergrasp(opvp) { }

void OPvPCapturePointWG::SetTeamByBuildingState()
{
    if(m_buildingState->GetTeam() == TEAM_ALLIANCE) {
        m_value = m_maxValue;
        m_State = OBJECTIVESTATE_ALLIANCE;
    } else if(m_buildingState->GetTeam() == TEAM_HORDE) {
        m_value = -m_maxValue;
        m_State = OBJECTIVESTATE_HORDE;
    } else {
        m_value = 0;
        m_State = OBJECTIVESTATE_NEUTRAL;
    }

    if(m_team != m_buildingState->GetTeam()) {
        TeamId oldTeam = m_team;
        m_team = m_buildingState->GetTeam();
        ChangeTeam(oldTeam);
    }

    SendChangePhase();
}

void OPvPCapturePointWG::ChangeTeam(TeamId oldTeam)
{
    entry                           = 0;
    guide_entry                     = 0;
    guide_entry_fortress_horde      = 0;
    guide_entry_fortress_alliance   = 0;

    if(oldTeam != TEAM_NEUTRAL)
        m_wintergrasp->ModifyWorkshopCount(oldTeam, false);

    if(m_team != TEAM_NEUTRAL)
    {
        entry                           = m_team == TEAM_ALLIANCE ? CRE_ENG_A : CRE_ENG_H;
        guide_entry                     = m_team == TEAM_ALLIANCE ? CRE_SPI_A : CRE_SPI_H;
        guide_entry_fortress_horde      = m_team == TEAM_HORDE ? CRE_SPI_H : CRE_SPI_H;
        guide_entry_fortress_alliance   = m_team == TEAM_ALLIANCE ? CRE_SPI_A : CRE_SPI_A;
        m_wintergrasp->ModifyWorkshopCount(m_team, true);
    }

    if(m_capturePoint)
        GameObject::SetGoArtKit(CapturePointArtKit[m_team], m_capturePoint, m_capturePointGUID);

    m_buildingState->SetTeam(m_team);

    m_wintergrasp->BroadcastStateChange(m_buildingState);

    if(m_buildingState->building)
        m_buildingState->building->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[m_team]);

    if(entry) {
        if(m_engGuid) {
            *m_engEntry = entry;
            _RespawnCreatureIfNeeded(m_engineer, entry);
        }

        if(m_spiGuid) {
            if(m_wintergrasp->getAttackerTeam() == TEAM_ALLIANCE)
            {
                *m_spiEntry = guide_entry;
                _RespawnCreatureIfNeeded(m_spiritguide_horde, guide_entry_fortress_horde);
                m_wintergrasp->RelocateHordeDeadPlayers(m_spiritguide_horde);
                _RespawnCreatureIfNeeded(m_spiritguide, guide_entry);
                m_wintergrasp->RelocateAllianceDeadPlayers(m_spiritguide);
            } else {
                *m_spiEntry = guide_entry;
                _RespawnCreatureIfNeeded(m_spiritguide_alliance, guide_entry_fortress_alliance);
                m_wintergrasp->RelocateAllianceDeadPlayers(m_spiritguide_alliance);
                _RespawnCreatureIfNeeded(m_spiritguide, guide_entry);
                m_wintergrasp->RelocateHordeDeadPlayers(m_spiritguide);
            }
        }
    } else if(m_engineer)
        m_engineer->SetVisible(false);
}

class OutdoorPvP_wintergrasp : public OutdoorPvPScript
{
public:
    OutdoorPvP_wintergrasp() : OutdoorPvPScript("outdoorpvp_wg") { }

    OutdoorPvP* GetOutdoorPvP() const
    {
        return new OutdoorPvPWG();
    }
};

void AddSC_outdoorpvp_wg()
{
    new OutdoorPvP_wintergrasp;
}
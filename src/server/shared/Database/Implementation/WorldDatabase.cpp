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

#include "WorldDatabase.h"

void WorldDatabaseConnection::DoPrepareStatements()
{
    if(!m_reconnecting)
        m_stmts.resize(MAX_WORLDDATABASE_STATEMENTS);

    PREPARE_STATEMENT(WORLD_LOAD_QUEST_POOLS, "SELECT entry, pool_entry FROM pool_quest", CONNECTION_SYNCH)
    PREPARE_STATEMENT(WORLD_DEL_CRELINKED_RESPAWN, "DELETE FROM linked_respawn WHERE `guid` = ?", CONNECTION_ASYNC)
    PREPARE_STATEMENT(WORLD_REP_CRELINKED_RESPAWN, "REPLACE INTO linked_respawn (guid, linkedGuid) VALUES (?, ?)", CONNECTION_ASYNC)
    PREPARE_STATEMENT(WORLD_LOAD_CRETEXT, "SELECT entry, groupid, id, text, type, language, probability, emote, duration, sound FROM creature_text", CONNECTION_SYNCH)
    PREPARE_STATEMENT(WORLD_LOAD_SMART_SCRIPTS,  "SELECT entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o FROM smart_scripts ORDER BY entryorguid, source_type, id, link", CONNECTION_SYNCH)
    PREPARE_STATEMENT(WORLD_LOAD_SMARTAI_WP,  "SELECT entry, pointid, position_x, position_y, position_z FROM waypoints ORDER BY entry, pointid", CONNECTION_SYNCH)
    PREPARE_STATEMENT(WORLD_DEL_GAMEOBJECT, "DELETE FROM gameobject WHERE guid = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(WORLD_INS_GAMEOBJECT, "INSERT INTO gameobject (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
    PREPARE_STATEMENT(WORLD_DEL_EVENT_GAMEOBJECT, "DELETE FROM game_event_gameobject WHERE guid = ?", CONNECTION_ASYNC);
}

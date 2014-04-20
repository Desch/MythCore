-- -------------------------------------------------------------------------
-- --------------- Myth Project 'Characters' Database Update ---------------
-- -------------------------------------------------------------------------
-- 'old' Myth Anticheat Tables
DROP TABLE IF EXISTS
`players_reports_status`,   -- DEPRICATED TABLE
`daily_players_reports`,    -- DEPRICATED TABLE
`anticheat_config`,         -- DEPRICATED TABLE
`cheat_temp_reports`,       -- DEPRICATED TABLE
`cheat_reports`,            -- DEPRICATED TABLE
`cheat_first_report`,       -- DEPRICATED TABLE
`anticheat_log`;            -- DEPRICATED TABLE

-- Myth OutdoorPvP: Wintergrasp
DELETE FROM `worldstates` WHERE `entry` IN (31001, 31002, 31003, 31004);
INSERT INTO `worldstates` VALUES
(31001, 0,'WG War Status'),
(31002, 8993750,'WG Clock'),
(31003, 1,'WG Fortress Defender'),
(31004, 1,'WG TEMP Battle Control Faction');

-- Truncate Tables to avoid crashes. Because Mech. && Spells Changed.
TRUNCATE `character_aura`;              -- CLEAN WRONG DATA
TRUNCATE `pet_aura`;                    -- CLEAN WRONG DATA
TRUNCATE `pet_spell_cooldown`;          -- CLEAN WRONG DATA
TRUNCATE `character_spell_cooldown`;    -- CLEAN WRONG DATA
TRUNCATE `lag_reports`;                 -- CLEAN WRONG DATA
TRUNCATE `groups`;                      -- CLEAN WRONG DATA
TRUNCATE `group_member`;                -- CLEAN WRONG DATA
TRUNCATE `group_instance`;              -- CLEAN WRONG DATA
TRUNCATE `gm_subsurveys`;               -- CLEAN WRONG DATA
TRUNCATE `gm_surveys`;                  -- CLEAN WRONG DATA
TRUNCATE `gm_tickets`;                  -- CLEAN WRONG DATA
TRUNCATE `gameobject_respawn`;          -- CLEAN WRONG DATA
TRUNCATE `creature_respawn`;            -- CLEAN WRONG DATA
TRUNCATE `instance_reset`;              -- CLEAN WRONG DATA
TRUNCATE `bugreport`;                   -- CLEAN WRONG DATA
TRUNCATE `instance`;                    -- CLEAN WRONG DATA
-- FIX and Prevent Crash into "Invite System"
UPDATE `characters` SET `online` = 0;
-- FIX and Prevent Crash into Guild System
ALTER TABLE `guild`
DROP PRIMARY KEY,
ADD PRIMARY KEY (`name`,`guildid`);
-- FIX and Prevent Crash into Char System
ALTER TABLE `characters`
DROP PRIMARY KEY,
ADD PRIMARY KEY (`guid`);
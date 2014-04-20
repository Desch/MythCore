-- -------------------------------------------------------------------------
-- --------------- Myth Project 'Characters' Database Update ---------------
-- -------------------------------------------------------------------------
ALTER TABLE `character_glyphs` CHANGE `glyph1` `glyph1` smallint(5) unsigned DEFAULT 0;
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

DROP TABLE IF EXISTS `calendar_events`;
CREATE TABLE IF NOT EXISTS `calendar_events` (
  `id` bigint(20) unsigned NOT NULL DEFAULT 0,
  `creator` int(10) unsigned NOT NULL DEFAULT 0,
  `title` varchar(255) NOT NULL DEFAULT '',
  `description` varchar(255) NOT NULL DEFAULT '',
  `type` tinyint(1) unsigned NOT NULL DEFAULT '4',
  `dungeon` int(10) NOT NULL DEFAULT '-1',
  `eventtime` int(10) unsigned NOT NULL DEFAULT 0,
  `flags` int(10) unsigned NOT NULL DEFAULT 0,
  `time2` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `calendar_invites`;
CREATE TABLE IF NOT EXISTS `calendar_invites` (
  `id` bigint(20) unsigned NOT NULL DEFAULT 0,
  `event` bigint(20) unsigned NOT NULL DEFAULT 0,
  `invitee` int(10) unsigned NOT NULL DEFAULT 0,
  `sender` int(10) unsigned NOT NULL DEFAULT 0,
  `status` tinyint(1) unsigned NOT NULL DEFAULT 0,
  `statustime` int(10) unsigned NOT NULL DEFAULT 0,
  `rank` tinyint(1) unsigned NOT NULL DEFAULT 0,
  `text` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `character_equipmentsets`;
CREATE TABLE `character_equipmentsets` (
  `guid` int(10) NOT NULL DEFAULT 0,
  `setguid` bigint(20) NOT NULL AUTO_INCREMENT,
  `setindex` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `name` varchar(31) NOT NULL,
  `iconname` varchar(100) NOT NULL,
  `item0` int(10) unsigned NOT NULL DEFAULT 0,
  `item1` int(10) unsigned NOT NULL DEFAULT 0,
  `item2` int(10) unsigned NOT NULL DEFAULT 0,
  `item3` int(10) unsigned NOT NULL DEFAULT 0,
  `item4` int(10) unsigned NOT NULL DEFAULT 0,
  `item5` int(10) unsigned NOT NULL DEFAULT 0,
  `item6` int(10) unsigned NOT NULL DEFAULT 0,
  `item7` int(10) unsigned NOT NULL DEFAULT 0,
  `item8` int(10) unsigned NOT NULL DEFAULT 0,
  `item9` int(10) unsigned NOT NULL DEFAULT 0,
  `item10` int(10) unsigned NOT NULL DEFAULT 0,
  `item11` int(10) unsigned NOT NULL DEFAULT 0,
  `item12` int(10) unsigned NOT NULL DEFAULT 0,
  `item13` int(10) unsigned NOT NULL DEFAULT 0,
  `item14` int(10) unsigned NOT NULL DEFAULT 0,
  `item15` int(10) unsigned NOT NULL DEFAULT 0,
  `item16` int(10) unsigned NOT NULL DEFAULT 0,
  `item17` int(10) unsigned NOT NULL DEFAULT 0,
  `item18` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`setguid`),
  UNIQUE KEY `idx_set` (`guid`,`setguid`,`setindex`),
  KEY `Idx_setindex` (`setindex`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
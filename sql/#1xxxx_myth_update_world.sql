﻿/* Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Copyright (C) 2012 SymphonyArt <http://symphonyart.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only. */
-- -------------------------------------------------------------------------}
-- ----------------- Myth Project "World" Database Update ------------------
-- -------------------------------------------------------------------------{
DROP TABLE IF EXISTS `version`;
CREATE TABLE `version` (
  `db_version` VARCHAR(15) DEFAULT NULL,
  `script_version` VARCHAR(15) DEFAULT NULL,
  `cache_id` INT(4) DEFAULT 0
) ENGINE = MYISAM DEFAULT CHARSET = utf8 ROW_FORMAT = FIXED;
INSERT INTO `version` VALUES
 ('MythDB v12.54', 'MythAI v12.54', 1254); -- BASED ON YTDB 604
-- -------------------------------------------------------------------------}
-- -------------------- Myth Project "Achievements" Update -----------------
-- -------------------------------------------------------------------------{
-- Achievement: Bros. Before Ho Ho Ho's, http://wotlk.openwow.com/achievement=1685
UPDATE `creature_template` SET `unit_flags` = 8 WHERE `entry` = 5661;
-- Achievement: The Frozen Throne (25 player), http://wotlk.openwow.com/achievement=4597
DELETE FROM `achievement_criteria_data` WHERE `criteria_id` = 12909;
INSERT INTO `achievement_criteria_data` VALUES 
 (12909, 12, 1, 0,''),
 (12909, 1, 36597, 0,'');
-- Achievement: Higher Learning, http://wotlk.openwow.com/achievement=1956
SET @Gossip = 21255; -- Set by avengerweb (gossip_menu.entry - need 2)
SET @Guid = 151794; -- Set by avengerweb (gameobject.guid - need 35)
SET @Pool = 14134; -- Set by avengerweb (pool_template.entry - need 8)
SET @GoDespawnTime = 10800; -- Three hours
-- Spawns
DELETE FROM `gameobject` WHERE `id` IN (192708, 192706, 192871, 192905, 192710, 192886, 192869, 192880, 192895, 192713, 192889, 192890, 192894, 192884, 192866, 192891, 192872, 192881, 192709, 192883, 192651, 192888, 192711, 192653, 192887, 192652, 192865, 192874, 192868, 192870, 192885, 192867, 192882, 192707, 192896);
INSERT INTO `gameobject`(`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
-- Introduction
 (@Guid+0, 192708, 571, 1, 1, 5814.522, 582.1118, 652.7509, 0.244345576, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Schools of Arcane Magic - Introduction
 (@Guid+1, 192706, 571, 1, 1, 5814.522, 582.1118, 652.7509, 0.244345576, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Worst Mage in Dalaran: A Children's Book
 (@Guid+2, 192871, 571, 1, 1, 5814.522, 582.1118, 652.7509, 0.244345576, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Archmage Antonidas - Part II
 (@Guid+3, 192905, 571, 1, 1, 5814.522, 582.1118, 652.7509, 0.244345576, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Old Wizard's Almanac
-- Conjuration
 (@Guid+4, 192710, 571, 1, 1, 5812.64, 797.223, 662.57, -2.86234, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Schools of Arcane Magic - Conjuration
 (@Guid+5, 192886, 571, 1, 1, 5812.64, 797.223, 662.57, -2.86234, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- In Time, the Arcane Corrupts All!
 (@Guid+6, 192869, 571, 1, 1, 5812.64, 797.223, 662.57, -2.86234, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- LAVA BLAST - by Pyroco
 (@Guid+7, 192880, 571, 1, 1, 5812.64, 797.223, 662.57, -2.86234, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- Journal of Archmage Antonidas
 (@Guid+8, 192895, 571, 1, 1, 5812.64, 797.223, 662.57, -2.86234, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- Kirin Tor Monthly (March Issue)
-- Enchantment
 (@Guid+9, 192713, 571, 1, 1, 5808.22, 691.076, 658.715, 2.179470, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Schools of Arcane Magic - Enchantment
 (@Guid+10, 192889, 571, 1, 1, 5808.22, 691.076, 658.715, 2.179470, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- Kirin Tor Monthly (November Issue)
 (@Guid+11, 192890, 571, 1, 1, 5808.22, 691.076, 658.715, 2.179470, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- STAY OUT!!!
 (@Guid+12, 192894, 571, 1, 1, 5808.22, 691.076, 658.715, 2.179470, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- Remedial Magic 101: Tips from the Pros
 (@Guid+13, 192884, 571, 1, 1, 5808.22, 691.076, 658.715, 2.179470, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- Portals are NOT Garbage Bins!
-- Necromancy
 (@Guid+14, 192866, 571, 1, 1, 5850.57, 664.754, 659.569, -1.93732, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Schools of Arcane Magic - Necromancy
 (@Guid+15, 192891, 571, 1, 1, 5850.57, 664.754, 659.569, -1.93732, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- To Do Today
 (@Guid+16, 192872, 571, 1, 1, 5850.57, 664.754, 659.569, -1.93732, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Archmage Antonidas - Part III
 (@Guid+17, 192881, 571, 1, 1, 5850.57, 664.754, 659.569, -1.93732, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- Thinking with Portals - A Memorandum on Proper Portal Usage
-- Abjuration
 (@Guid+18, 192709, 571, 1, 1, 5763.5, 618.08, 650.092, 3.49199, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Schools of Arcane Magic - Abjuration
 (@Guid+19, 192883, 571, 1, 1, 5763.5, 618.08, 650.092, 3.49199, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- Blinking Rules & Regulations
 (@Guid+20, 192651, 571, 1, 1, 5763.5, 618.08, 650.092, 3.49199, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- Damaged Apprentice Journal
 (@Guid+21, 192888, 571, 1, 1, 5763.5, 618.08, 650.092, 3.49199, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- Bundle of Romantic Correspondences
-- Divination
 (@Guid+22, 192711, 571, 1, 1, 5778.18, 832.302, 680.268, 0.0283899, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Schools of Arcane Magic - Divination
 (@Guid+23, 192653, 571, 1, 1, 5778.18, 832.302, 680.268, 0.0283899, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- On the Virtues of Magic
 (@Guid+24, 192887, 571, 1, 1, 5778.18, 832.302, 680.268, 0.0283899, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- Losses of the Third War
 (@Guid+25, 192652, 571, 1, 1, 5778.18, 832.302, 680.268, 0.0283899, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- Kirin Tor Monthly (May Issue)
-- Illusion
 (@Guid+26, 192865, 571, 1, 1, 5776.88, 517.79, 653.911, 0.401425, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Schools of Arcane Magic - Illusion
 (@Guid+27, 192874, 571, 1, 1, 5776.88, 517.79, 653.911, 0.401425, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Fluffy Bunny
 (@Guid+28, 192868, 571, 1, 1, 5776.88, 517.79, 653.911, 0.401425, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Dangers of Magic Abuse
 (@Guid+29, 192870, 571, 1, 1, 5776.88, 517.79, 653.911, 0.401425, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Archmage Antonidas - Part I
 (@Guid+30, 192885, 571, 1, 1, 5776.88, 517.79, 653.911, 0.401425, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Fate of Apprentice Argoly
-- Transmutation
 (@Guid+31, 192867, 571, 1, 1, 5844.61328, 664.541, 649.3733, -0.3316107, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- The Schools of Arcane Magic - Transmutation
 (@Guid+32, 192882, 571, 1, 1, 5844.61328, 664.541, 649.3733, -0.3316107, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- Polymorphic Rules & Regulations
 (@Guid+33, 192707, 571, 1, 1, 5844.61328, 664.541, 649.3733, -0.3316107, 0, 0, 0, 0, @GoDespawnTime, 0, 1), -- Weathered Diary - Dates from the Third War
 (@Guid+34, 192896, 571, 1, 1, 5844.61328, 664.541, 649.3733, -0.3316107, 0, 0, 0, 0, @GoDespawnTime, 0, 1); -- Excerpts from the Journal of Archmage Vargoth
-- Pooling
DELETE FROM `pool_template` WHERE `entry` BETWEEN @Pool+0 AND @Pool+7;
INSERT INTO `pool_template`(`entry`,`max_limit`,`description`) VALUES
 (@Pool+0, 1, "Dalaran Books: Violet Gate"),
 (@Pool+1, 1, "Dalaran Books: Violet Citadel Lower"),
 (@Pool+2, 1, "Dalaran Books: Threads of Fate"),
 (@Pool+3, 1, "Dalaran Books: Ledgerdemain Lounge Upstairs"),
 (@Pool+4, 1, "Dalaran Books: Visitor’s Center"),
 (@Pool+5, 1, "Dalaran Books: Violet Citadel Upstairs"),
 (@Pool+6, 1, "Dalaran Books: Violet Hold"),
 (@Pool+7, 1, "Dalaran Books: Ledgerdemain Lounge Lower");
DELETE FROM `pool_gameobject` WHERE `pool_entry` BETWEEN @Pool+0 AND @Pool+7;
INSERT INTO `pool_gameobject`(`guid`,`pool_entry`,`chance`,`description`) VALUES
-- Introduction
 (@Guid+0, @Pool+0, 0, "The Schools of Arcane Magic - Introduction"),
 (@Guid+1, @Pool+0, 0, "The Worst Mage in Dalaran: A Children''s Book"),
 (@Guid+2, @Pool+0, 0, "The Archmage Antonidas - Part II"),
 (@Guid+3, @Pool+0, 0, "The Old Wizard''s Almanac"),
-- Conjuration
 (@Guid+4, @Pool+1, 0, "The Schools of Arcane Magic - Conjuration"),
 (@Guid+5, @Pool+1, 0, "In Time, the Arcane Corrupts All!"),
 (@Guid+6, @Pool+1, 0, "LAVA BLAST - by Pyroco"),
 (@Guid+7, @Pool+1, 0, "Journal of Archmage Antonidas"),
 (@Guid+8, @Pool+1, 0, "Kirin Tor Monthly (March Issue)"),
-- Enchantment
 (@Guid+9, @Pool+2, 0, "The Schools of Arcane Magic - Enchantment"),
 (@Guid+10, @Pool+2, 0, "Kirin Tor Monthly (November Issue)"),
 (@Guid+11, @Pool+2, 0, "STAY OUT!!!"),
 (@Guid+12, @Pool+2, 0, "Remedial Magic 101: Tips from the Pros"),
 (@Guid+13, @Pool+2, 0, "Portals are NOT Garbage Bins!"),
-- Necromancy
 (@Guid+14, @Pool+3, 0, "The Schools of Arcane Magic - Necromancy"),
 (@Guid+15, @Pool+3, 0, "To Do Today"),
 (@Guid+16, @Pool+3, 0, "The Archmage Antonidas - Part III"),
 (@Guid+17, @Pool+3, 0, "Thinking with Portals - A Memorandum on Proper Portal Usage"),
-- Abjuration
 (@Guid+18, @Pool+4, 0, "The Schools of Arcane Magic - Abjuration"),
 (@Guid+19, @Pool+4, 0, "Blinking Rules & Regulations"),
 (@Guid+20, @Pool+4, 0, "Damaged Apprentice Journal"),
 (@Guid+21, @Pool+4, 0, "Bundle of Romantic Correspondences"),
-- Divination
 (@Guid+22, @Pool+5, 0, "The Schools of Arcane Magic - Divination"),
 (@Guid+23, @Pool+5, 0, "On the Virtues of Magic"),
 (@Guid+24, @Pool+5, 0, "Losses of the Third War"),
 (@Guid+25, @Pool+5, 0, "Kirin Tor Monthly (May Issue)"),
-- Illusion
 (@Guid+26, @Pool+6, 0, "The Schools of Arcane Magic - Illusion"),
 (@Guid+27, @Pool+6, 0, "The Fluffy Bunny"),
 (@Guid+28, @Pool+6, 0, "The Dangers of Magic Abuse"),
 (@Guid+29, @Pool+6, 0, "The Archmage Antonidas - Part I"),
 (@Guid+30, @Pool+6, 0, "The Fate of Apprentice Argoly"),
-- Transmutation
 (@Guid+31, @Pool+7, 0, "The Schools of Arcane Magic - Transmutation"),
 (@Guid+32, @Pool+7, 0, "Polymorphic Rules & Regulations"),
 (@Guid+33, @Pool+7, 0, "Weathered Diary - Dates from the Third War"),
 (@Guid+34, @Pool+7, 0, "Excerpts from the Journal of Archmage Vargoth");

UPDATE `creature_template` SET `gossip_menu_id` = @Gossip+0,`AIName` = 'SmartAI' WHERE `entry` = 19481; -- Archmage Vargoth
UPDATE `creature_model_info` SET `bounding_radius` = 0.306,`combat_reach` = 1,`gender` = 0 WHERE `modelid` = 19284; -- Archmage Vargoth
UPDATE `creature` SET `npcflag` = 1 WHERE `guid` = 117023; -- Archmage Vargoth
-- Restore NPC Flags.
UPDATE `creature` SET `npcflag` = 0 WHERE `guid` = 98174;
DELETE FROM `smart_scripts` WHERE `entryorguid` = -98174;
DELETE FROM `creature_template_addon` WHERE `entry` = 19481;
INSERT INTO `creature_template_addon`(`entry`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
 (19481, 0, 0, 1, 0, NULL); -- Archmage Vargoth
-- Gossip for Archmage Vargoth
DELETE FROM `gossip_menu` WHERE `text_id` IN (14192, 14194);
INSERT INTO `gossip_menu`(`entry`,`text_id`) VALUES
 (@Gossip+0, 14192),
 (@Gossip+1, 14194);
DELETE FROM `gossip_menu_option` WHERE `menu_id` IN (@Gossip+0, @Gossip+1) AND `id` = 0;
INSERT INTO `gossip_menu_option`(`menu_id`,`id`,`option_icon`,`option_text`,`option_id`,`npc_option_npcflag`,`action_menu_id`,`action_poi_id`,`action_script_id`,`box_coded`,`box_money`,`box_text`) VALUES
 (@Gossip+0, 0, 0, "I have a book that might interest you. Would you like to take a look?", 1, 1, @Gossip+1, 0, 0, 0, 0,NULL),
 (@Gossip+1, 0, 0, "Thank you! I''ll be sure to notify you if I find anything else.", 1, 1, 0, 0, 0, 0, 0,NULL); -- SAI script
DELETE FROM `creature_template_addon` WHERE `entry` = 32643;
INSERT INTO `creature_template_addon`(`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
 (32643, 0, 0, 0, 3, 0, 61478); -- companion pet aura (arcane explosions)
DELETE FROM `smart_scripts` WHERE `entryorguid` = -117023 AND `source_type` = 0;
INSERT INTO `smart_scripts`(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES 
 (-117023, 0, 0, 0, 62, 0, 100, 0, @Gossip+1, 0, 0, 0, 11, 61457, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Archmage Vargoth (Dalaran) - On gossip select cast Kirin Tor Familiar on invoker");
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 14 AND `SourceGroup` = @Gossip+0 AND `SourceEntry` = 14192;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = @Gossip+0 AND `SourceEntry` = 0;
INSERT INTO `conditions`(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
 (14, @Gossip+0, 14192, 0, 4, 4395, 0, 0, 0, "", "Archmage Vargoth - Gossip: zone restriction to Dalaran"),
 (15, @Gossip+0, 0, 0, 4, 4395, 0, 0, 0, "", "Archmage Vargoth - Gossip option: zone restriction to Dalaran"),
 (15, @Gossip+0, 0, 0, 26, 44738, 1, 0, 0, "", "Archmage Vargoth - Gossip option: player should not have item 44738"),
 (15, @Gossip+0, 0, 0, 17, 1956, 0, 0, 0, "", "Archmage Vargoth - Gossip option: player should have achiev 1956");
-- FIXME: add a gossip option condition: player should not have spell 61472
-- The Captain's Booty, http://ru.wowhead.com/?achievement=3457
UPDATE `gossip_scripts` SET `datalong2`='1' WHERE `id`='8988';
DELETE FROM `conditions` WHERE `SourceGroup`=9647;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(14, 9647, 13059, 0, 11, 50517, 0, 0, 0, '', 'Pirates Day: DeMeza gossip'),
(14, 9647, 13065, 0, 1, 50517, 0, 0, 0, '', 'Pirates Day: DeMeza gossip'),
(15, 9647, 0, 0, 11, 50517, 0, 0, 0, '', 'Pirates Day: DeMeza gossip');
-- -------------------------------------------------------------------------}
-- ---------------------- Myth Project "Quests" Update ---------------------
-- -------------------------------------------------------------------------{
-- Quest: Forced Hand, http://old.wowhead.com/quest=12576
-- NPC: Frenzyheart Spearbearer, http://old.wowhead.com/npc=28080
-- NPC: Frenzyheart Scavenger, http://old.wowhead.com/npc=28081
UPDATE `creature_template` SET `faction_A` = 14,`faction_H` = 14,`unit_flags` = 0 WHERE `entry` IN (28080, 28081);
-- Quest: Lupus Pupus, http://wotlk.openwow.com/quest=11728
-- NPC: Oil-stained Wolf, http://old.wowhead.com/npc=25791
DELETE FROM `smart_scripts` WHERE `entryorguid` = 25791;
INSERT INTO `smart_scripts`(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
 ('25791','0','0','0','11','0','100','0','0','0','0','0','11','45948','2','0','0','0','0','1','0','0','0','0','0','0','0','Oil-stained Wolf - On Respawn - Cast Oil Coat'),
 ('25791','0','1','2','8','0','100','0','53326','0','0','0','2','35','0','0','0','0','0','1','0','0','0','0','0','0','0','Oil-stained Wolf becomes friendly'),
 ('25791','0','2','3','61','0','100','0','0','0','0','0','29','0','0','0','0','0','0','7','0','0','0','0','0','0','0','Oil-stained Wolf moves forward'),
 ('25791','0','3','4','61','0','100','0','0','0','0','0','67','4','4000','5000','0','0','100','7','0','0','0','0','0','0','0','Oil-stained Wolf Delay'),
 ('25791','0','4','5','61','0','100','0','0','0','0','0','5','53','0','0','0','0','0','1','0','0','0','0','0','0','0','Oil-stained Wolf - Do pooping emote (53)'),
 ('25791','0','5','6','61','0','100','0','0','0','0','0','50','187981','30','0','0','0','0','1','0','0','0','0','0','0','0','Oil-stained Wolf poops'),
 ('25791','0','6','0','61','0','100','0','0','0','0','0','41','2000','0','0','0','0','0','1','0','0','0','0','0','0','0','Oil-stained Wolf despawns on poop.');
UPDATE `creature_template` SET `ScriptName`='SmartAI' WHERE  `entry` = 25791;
-- Quest: Mastery Of Melee, http://old.wowhead.com/quest=13828, http://old.wowhead.com/quest=13829
SET @ENTRY := 33973;
SET @SOURCETYPE := 0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=@SOURCETYPE AND `id` = 5;
UPDATE creature_template SET AIName="SmartAI" WHERE entry=@ENTRY;
INSERT INTO `smart_scripts`(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES 
(@ENTRY,@SOURCETYPE, 5,0,62,0,100,0,10398,0,0,0,33,33973,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,"Credit - http://old.wowhead.com/quest=13829");
-- Quests: Mastery Of The Shield-Breaker, http://old.wowhead.com/quest=13835, http://old.wowhead.com/quest=13838
SET @ENTRY := 33974;
SET @SOURCETYPE := 0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=@SOURCETYPE AND `id` = 5;
UPDATE creature_template SET AIName="SmartAI" WHERE entry=@ENTRY;
INSERT INTO `smart_scripts`(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES 
(@ENTRY,@SOURCETYPE,5,0,62,0,100,0,10402,0,0,0,33,33974,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,"Credit - http://old.wowhead.com/quest=13829");
-- Quests: Mastery Of The Charge, http://old.wowhead.com/quest=13837, http://old.wowhead.com/quest=13839
SET @ENTRY := 33972;
SET @SOURCETYPE := 0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=@SOURCETYPE AND `id` = 5;
UPDATE creature_template SET AIName="SmartAI" WHERE entry=@ENTRY;
INSERT INTO `smart_scripts`(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES 
(@ENTRY,@SOURCETYPE,5,0,62,0,100,0,10400,0,0,0,33,33972,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,"Credit - http://old.wowhead.com/quest=13829");
-- Quest: Battling the Elements, http://wotlk.openwow.com/quest=12967
-- Quest: A Change of Scenery, http://wotlk.openwow.com/quest=12921
-- Quest: A Spark of Hope, http://wotlk.openwow.com/quest=12956
-- Quest: Forging an Alliance, http://wotlk.openwow.com/quest=12924
-- Quest: You Can't Miss Him, http://wotlk.openwow.com/quest=12966
-- Quest: Mounting Hodir's Helm, http://wotlk.openwow.com/quest=12987
-- Quest: Mending Fences, http://wotlk.openwow.com/quest=12915
DELETE FROM `spell_area` WHERE `area` IN (4438, 4439, 4440, 4495);
INSERT INTO `spell_area`(`spell`,`area`,`quest_start`,`quest_start_active`,`quest_end`,`aura_spell`,`racemask`,`gender`,`autocast`) VALUES
 (55012, 4438, 12921, 1, 12956, 0, 0, 2, 1),
 (55857, 4438, 0, 0, 12956, 0, 0, 2, 1),
 (55857, 4439, 0, 0, 12956, 0, 0, 2, 1),
 (55857, 4495, 0, 0, 12956, 0, 0, 2, 1),
 (55858, 4438, 12915, 0, 0, 0, 0, 2, 1),
 (55858, 4438, 12956, 0, 0, 0, 0, 2, 1),
 (55858, 4439, 12956, 0, 0, 0, 0, 2, 1),
 (55858, 4440, 0, 0, 0, 0, 0, 2, 1),
 (55858, 4495, 12956, 0, 12924, 0, 0, 2, 1),
 (55952, 4495, 12924, 0, 0, 0, 0, 2, 1),
 (55952, 4495, 12924, 1, 0, 0, 0, 2, 1),
 (56771, 4438, 12967, 0, 0, 0, 0, 2, 1),
 (56780, 4439, 12956, 0, 12987, 0, 0, 2, 1),
 (61209, 4495, 12966, 1, 12924, 0, 0, 2, 1),
 (72914, 4438, 12921, 1, 12956, 0, 0, 2, 1);
-- Quest: Monitoring the Rift: Cleftcliff Anomaly, http://wotlk.openwow.com/quest=11576
-- NPC: Borean - Westrift Cleftcliff Anomaly, http://wotlk.openwow.com/npc=25310
UPDATE `creature_template` SET `AIName` = '' WHERE `entry` = 25310;
UPDATE `quest_template` SET `ReqSpellCast1` = 45414 WHERE `entry` = 11576;
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 25310;
-- Quest: A Friendly Chat... (Alliance), http://wotlk.openwow.com/quest=24657
-- Quest: A Friendly Chat... (Horde), http://wotlk.openwow.com/quest=24576
UPDATE `quest_template` SET `Method` = 2 WHERE `entry` IN (24576, 24657);
-- Item: Lovely Charm Collector's Kit, http://wotlk.openwow.com/item=49661
UPDATE `gossip_scripts` SET `datalong2` = 1 WHERE `id` = 50029;
-- Quest: Bringing Down Heb'Jin, http://wotlk.openwow.com/quest=12662
-- Quest: The Thunderspike, http://wotlk.openwow.com/quest=10526
-- GO: The Thunderspike, http://wotlk.openwow.com/object=184729
-- GO: Heb'Jin's Drum, http://wotlk.openwow.com/object=190695
UPDATE `gameobject_template` SET `data6` = 60 WHERE `entry` IN (184729, 190695);
-- Quest: Nightbane, http://wotlk.openwow.com/quest=9644
-- NPC: Nightbane, http://wotlk.openwow.com/npc=17225
UPDATE `creature_template` SET `InhabitType` = 5 WHERE `entry` = 17225;
-- Quest: The Refiner's Fire, http://wotlk.openwow.com/quest=12922
-- Quest: Mending Fences, http://wotlk.openwow.com/quest=12915
UPDATE `quest_template` SET `PrevQuestId` = 12915 WHERE `entry` = 12922;
-- Quest: Forging an Alliance, http://wotlk.openwow.com/quest=12924
UPDATE `quest_template` SET `PrevQuestId` = 12956 WHERE `entry` = 12924;
-- Quest: A Spark of Hope, http://wotlk.openwow.com/quest=12956
UPDATE `quest_template` SET `NextQuestId` = 12924 WHERE `entry` = 12956;
-- NPC: King Jokkum, http://wotlk.openwow.com/npc=30105
UPDATE `creature` SET `phaseMask` = 4 WHERE `guid` = 98431;
-- NPC: Njormeld, http://wotlk.openwow.com/npc=30099
UPDATE `creature` SET `phaseMask` = 8 WHERE `id` = 30099;
-- NPC: Snorri, http://wotlk.openwow.com/npc=30123
UPDATE `creature` SET `phaseMask` = 8 WHERE `id` = 30123;
-- NPC: Njormeld, http://wotlk.openwow.com/npc=30127
UPDATE `creature` SET `phaseMask` = 4 WHERE `id` = 30127;
-- Questline: "Green Hills of Stranglethorn"
UPDATE creature_loot_template SET ChanceOrQuestChance = 10 WHERE item = 2725;
-- Quest: Across Transborea, http://wotlk.openwow.com/quest=11930
UPDATE `creature_template` SET `mindmg` = 10,`maxdmg` = 20,`dmg_multiplier` = 1,`minrangedmg` = 10,`maxrangedmg` = 20 WHERE `entry` = 26167;
-- Quest: A Short Fuse(Alliance), http://wotlk.openwow.com/quest=13389
-- Quest: A Short Fuse(Horde), http://wotlk.openwow.com/quest=13263
-- Quest: Blow it Up!, http://wotlk.openwow.com/quest=13262
-- Quest: Set it Off!, http://wotlk.openwow.com/quest=13388
DELETE FROM `gameobject` WHERE `guid` = 106178;
INSERT INTO `gameobject`(`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
 (106178, 193400, 571, 1, 8, 5813.9, 2023.67, 503.278, 0.81804, 0, 0, 0.39771, 0.917511, 300, 0, 1);
-- Quest: The Path to the Citadel (Alliance), http://wotlk.openwow.com/quest=24498
-- Quest: The Path to the Citadel (Horde), http://wotlk.openwow.com/quest=24507
UPDATE `quest_template` SET `Method` = 2 WHERE `entry` IN (24498, 24507);
-- Quest: The Purification of Quel'Delar, http://wotlk.openwow.com/quest=24564
UPDATE `quest_template` SET `SkillOrClassMask` = -431 WHERE `entry` = 24564;
-- Quest:Treats for Greatfather Winter (Alliance), http://wotlk.openwow.com/quest=7025 
UPDATE `quest_template` SET `PrevQuestId` = 7022 WHERE `entry` = 7025;
-- Quest: Treats for Great-father Winter (Horde), http://wotlk.openwow.com/quest=6962
UPDATE `quest_template` SET `RequiredRaces` = 690,`PrevQuestId` = 7021 WHERE `entry` = 6962;
-- Quest: Great-father Winter is Here! Winter http://wotlk.openwow.com/quest=6961
UPDATE `quest_template` SET `ExclusiveGroup` = 7021 WHERE `entry` = 6961;
-- Quest: HELP!, http://wotlk.openwow.com/quest=9923
UPDATE `creature` SET `id` = 18445 WHERE `guid` = 54944;
-- Quest: Veil Skith: Darkstone of Terokk, http://wotlk.openwow.com/quest=10839
UPDATE item_template SET ScriptName = 'rod_of_purification' WHERE entry = 31610;
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 22288;
DELETE FROM `smart_scripts` WHERE `source_type` IN (0, 9) AND `entryorguid` = 22288;
-- Quest: Get Kraken!, http://wotlk.openwow.com/quest=14108
UPDATE `creature_template` SET `AIName` = 'SmartAI',`ScriptName` = '' WHERE `entry` = 34852;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 34852;
INSERT INTO `smart_scripts`(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
 (34852, 0, 0, 1, 8, 0, 100, 0, 66390, 0, 3000, 3000, 33, 34852, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,'Slain Tualiq Villager - On Read Last Rites Hit - Give Kill Credit'), 
 (34852, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Slain Tualiq Villager - On Read Last Rites Hit - Despawn after 3 secs');
DELETE FROM `spell_script_names` WHERE `spell_id` = 66588;
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
 (66588,'spell_flaming_spear_targeting');
-- Quest: Garments of the Moon, http://wotlk.openwow.com/quest=5621
-- NPC: Sentinel Shaya, http://wotlk.openwow.com/npc=12429
UPDATE creature_template SET `faction_A` = 79, `faction_H` = 79 WHERE `entry` = 12429;
-- Quest: Cold Hearted, http://wotlk.openwow.com/quest=12856
-- Brunnhildar Prisoners
UPDATE `creature_template` SET `faction_A` = 2107,`faction_H` = 2107 WHERE `entry` = 29639;
UPDATE `creature_template` SET `flags_extra` = 2 WHERE `entry` = 26355;
-- Quest: Trust is Earned, http://wotlk.openwow.com/spell=11460
UPDATE `creature_template` SET `npcflag` = `npcflag`|1 WHERE `entry` IN (24750, 24752);
DELETE FROM `vehicle_template_accessory` WHERE `entry` = 24750;
INSERT INTO `vehicle_template_accessory`(`entry`,`accessory_entry`,`seat_id`,`minion`,`description`) VALUES
 (24750, 24751, 0, 1,'Excelsior rides Hidalgo the Master Falconer');
DELETE FROM `creature` WHERE `guid` = 88103;
DELETE FROM `gossip_menu` WHERE `entry` IN (8998, 9007) AND `text_id` IN (12153, 12168);
INSERT INTO `gossip_menu`(`entry`,`text_id`) VALUES
 (8998, 12153),(9007, 12168);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 14 AND `SourceGroup` IN (8998, 9007);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 9007;
INSERT INTO `conditions`(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`) VALUES
 (14, 8998, 12185, 8, 11460, 0), -- Must have Quest 11460 rewarded
 (14, 9007, 12168, 9, 11460, 0), -- Must have quest 11460 taken not complete
 (15, 9007, 0, 2, 34102, 1), -- Must have at least 1 count of item 34102
 (15, 9007, 0, 9, 11460, 0); -- Must have quest 11460 taken not complete
DELETE FROM `gossip_menu_option` WHERE `menu_id` IN (9007) AND `id` IN (0);
INSERT INTO `gossip_menu_option`(`menu_id`,`id`,`option_icon`,`option_text`,`option_id`,`npc_option_npcflag`,`action_menu_id`,`action_poi_id`,`action_script_id`,`box_coded`,`box_money`,`box_text`) VALUES
 (9007, 0, 0,'Feed the grub to the Rock Falcon.', 1, 1, 0, 0, 0, 0, 0,'');
SET @ENTRY := 24752; -- Rock Falcon SAI
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = @ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = @ENTRY;
INSERT INTO `smart_scripts`(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
 (@ENTRY, 0, 0, 1, 62, 0, 100, 0, 9007, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,'On gossip option Close Gossip'), -- Player needs to cast this on self "not working"
 (@ENTRY, 0, 1, 0, 61, 0, 100, 0, 100, 100, 100, 100, 86, 44363, 0, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,'Player selfcast spell');
 -- Quest: http://ru.wowhead.com/quest=12998
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = -192181 WHERE `entry` = 12998;
UPDATE `gameobject_template` SET `flags` = 4 WHERE `entry` = 192181;
-- [3.3.5]12976 can't find NPC Njormeld - Make NPC Visible to players
DELETE FROM creature_template_addon WHERE entry=30127;
UPDATE creature SET phaseMask=2,npcflag=2 WHERE id=30127;
UPDATE creature_template SET npcflag=2 WHERE entry=30127;
-- Issue #1901: Added Quest to NPC 32408
REPLACE INTO creature_questrelation (`id`,`quest`) VALUES
(32408,13394), (32408,13395), (32408,13396), (29799,13397);
REPLACE INTO creature_involvedrelation (`id`,`quest`) VALUES
(32408,13394), (32408,13395), (29799,13396), (29799,13397);
-- Make NPC Visible
DELETE FROM creature where guid=121098;
insert into `creature` 
(`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) values
('121098','32408','571','1','1','0','0','7268.25','1178.71','317.067','1.84812','300','0','0','12600','0','0','0','0','0');
DELETE FROM creature_template_addon WHERE entry=32408;
-- Correction for NPC 32408
UPDATE creature SET npcflag=2,phaseMask=1 WHERE id=32408;





-- -------------------------------------------------------------------------}
-- ----------------------- Myth Project "NPC" Update -----------------------
-- -------------------------------------------------------------------------{
-- NPC: Rimefang, http://www.wowhead.com/npc=37533
-- NPC: Spinestalker, http://www.wowhead.com/npc=37534
UPDATE `creature_template` SET `mechanic_immune_mask` = `mechanic_immune_mask`|0x00400000 WHERE `entry` IN (37533, 37534);
-- NPC: Lieutenant General Andorov, http://wotlk.openwow.com/npc=15471
UPDATE `creature_template` SET `npcflag`= 129 WHERE `entry` = 15471;
DELETE FROM `npc_vendor` WHERE `entry` = 15471;
INSERT INTO `npc_vendor` VALUES 
 (15471, 0, 13444, 4, 43200, 0),
 (15471, 0, 13446, 5, 43200, 0),
 (15471, 0, 14530, 14, 9000, 0),
 (15471, 0, 22219, 1, 43200, 0),
 (15471, 0, 22221, 1, 43200, 0);
-- NPC: Lillehoff, http://wotlk.openwow.com/npc=32540
-- NPC: Cielstrasza, http://wotlk.openwow.com/npc=32533
-- NPC: Veteran Crusader Aliocha Segard, http://wotlk.openwow.com/npc=30431
-- NPC: Archmage Alvareaux, http://wotlk.openwow.com/npc=32287
-- Item: Arcanum of Blissful Mending, http://old.wowhead.com/item=50370
-- Item: Arcanum of Burning Mysteries, http://old.wowhead.com/item=50368
-- Item: Arcanum of the Stalwart Protector, http://old.wowhead.com/item=50369
-- Item: Greater Inscription of the Axe, http://old.wowhead.com/item=50335
-- Item: Greater Inscription of the Crag, http://old.wowhead.com/item=50336
-- Item: Greater Inscription of the Pinnacle, http://old.wowhead.com/item=50337
-- Item: Greater Inscription of the Storm, http://old.wowhead.com/item=50338
DELETE FROM `npc_vendor` WHERE `entry` IN (32540, 32533, 30431, 32287) AND `item` IN (50369, 50368, 50370, 50335, 50336, 50337, 50338);
INSERT INTO `npc_vendor`(`entry`,`slot`,`item`,`maxcount`,`incrtime`,`ExtendedCost`) VALUES
 (32540, 0, 50335, 0, 0, 0),
 (32540, 0, 50336, 0, 0, 0),
 (32540, 0, 50337, 0, 0, 0),
 (32540, 0, 50338, 0, 0, 0),
 (32533, 0, 50370, 0, 0, 0),
 (30431, 0, 50369, 0, 0, 0),
 (32287, 0, 50368, 0, 0, 0);

/*
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceEntry` IN (44159, 50368, 44150, 50369, 44133, 44134, 44135, 44136, 44152, 50370, 50335, 50336, 50337, 50338);
INSERT INTO `conditions`(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
 (23, 32287, 44159, 0, 0, 5, 0, 1090, 192, 0, 0, 0, 0, "", "Arcanum of Burning Mysteries - when Kirin Tor revered/exalted"),
 (23, 32287, 50368, 0, 0, 5, 0, 1090, 63, 0, 0, 0, 0, "", "Arcanum of Burning Mysteries - when Kirin Tor not revered/exalted"),

 (23, 32540, 44133, 0, 0, 5, 0, 1119, 128, 0, 0, 0, 0, "", "Greater Inscription of the Axe - when The Sons of Hodir exalted"),
 (23, 32540, 50335, 0, 0, 5, 0, 1119, 127, 0, 0, 0, 0, "", "Greater Inscription of the Axe - when The Sons of Hodir not exalted"),

 (23, 32540, 44134, 0, 0, 5, 0, 1119, 128, 0, 0, 0, 0, "", "Greater Inscription of the Crag - when The Sons of Hodir exalted"),
 (23, 32540, 50336, 0, 0, 5, 0, 1119, 127, 0, 0, 0, 0, "", "Greater Inscription of the Crag - when The Sons of Hodir not exalted"),

 (23, 32540, 44136, 0, 0, 5, 0, 1119, 128, 0, 0, 0, 0, "", "Greater Inscription of the Pinnacle - when The Sons of Hodir exalted"),
 (23, 32540, 50337, 0, 0, 5, 0, 1119, 127, 0, 0, 0, 0, "", "Greater Inscription of the Pinnacle - when The Sons of Hodir not exalted"),

 (23, 32540, 44135, 0, 0, 5, 0, 1119, 128, 0, 0, 0, 0, "", "Greater Inscription of the Storm - when The Sons of Hodir exalted"),
 (23, 32540, 50338, 0, 0, 5, 0, 1119, 127, 0, 0, 0, 0, "", "Greater Inscription of the Storm - when The Sons of Hodir not exalted"),

 (23, 32533, 44152, 0, 0, 5, 0, 1091, 192, 0, 0, 0, 0, "", "Arcanum of Blissful Mending - when The Wyrmrest Accord revered/exalted"),
 (23, 32533, 50370, 0, 0, 5, 0, 1091, 63, 0, 0, 0, 0, "", "Arcanum of Blissful Mending - when The Wyrmrest Accord not revered/exalted"),

 (23, 30431, 44150, 0, 0, 5, 0, 1106, 192, 0, 0, 0, 0, "", "Arcanum of the Stalwart Protector - when Argent Crusade revered/exalted"),
 (23, 30431, 50369, 0, 0, 5, 0, 1106, 63, 0, 0, 0, 0, "", "Arcanum of the Stalwart Protector - when Argent Crusade not revered/exalted");
 */
-- NPC: Sister Svalna (10 Normal), http://wotlk.openwow.com/npc=37126
-- NPC: Sister Svalna (25 Normal), http://wotlk.openwow.com/npc=38258
DELETE FROM `creature_loot_template` WHERE `entry` IN (37126, 38258) AND `item` = 49426;
INSERT INTO `creature_loot_template`(`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES 
 (37126, 49426, 100, 1, 0, 1, 1),
 (38258, 49426, 100, 1, 0, 1, 1);
-- NPC: Possessed Iskalder, http://wotlk.openwow.com/npc=30924
-- NPC: Horde Defender, http://wotlk.openwow.com/npc=9457
-- NPC: Sundered Shard, http://wotlk.openwow.com/npc=20498
-- NPC: Flamebringer, http://wotlk.openwow.com/npc=27292
-- NPC: Gor Grimgut, http://wotlk.openwow.com/npc=21319
UPDATE `creature_template` SET `flags_extra` = `flags_extra`|64 WHERE `entry` IN (30924, 9457, 20498, 27292, 21319);
-- NPC: Magnus Frostwake, http://wotlk.openwow.com/npc=11278
DELETE FROM `creature_template_addon` WHERE `entry` = 11278;
-- NPC: Tu'u'gwar, http://wotlk.openwow.com/npc=26510 
UPDATE `creature_template` SET `InhabitType` = 7 WHERE `entry` = 26510;
-- NPC: Coren Direbrew, http://wotlk.openwow.com/npc=23872 
DELETE FROM `game_event_creature` WHERE `guid` = 180120;
INSERT INTO `game_event_creature`(`eventEntry`,`guid`) VALUES (26, 180120);
-- NPC: Charles Worth, http://wotlk.openwow.com/npc=28699
SET @NPC := 28699;
SET @GOSSIP := 10118;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = @NPC;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = @NPC;
INSERT INTO `smart_scripts`(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
 (@NPC, 0, 0, 2, 62, 0, 100, 0,@GOSSIP, 3, 0, 0, 85, 59817, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,'Charles Worth - On Gossip Select - Cast Spell Teach Wispcloak'),
 (@NPC, 0, 1, 2, 62, 0, 100, 0,@GOSSIP, 5, 0, 0, 85, 59841, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,'Charles Worth - On Gossip Select - Cast Spell Teach Deathchill Cloak'),
 (@NPC, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,'Charles Worth - On Gossip Select - Close Gossip');
DELETE FROM `gossip_menu` WHERE `entry` IN (14076, 10136, 10138, 10118) AND `text_id` IN (14072, 14074, 14076);
INSERT INTO `gossip_menu`(`entry`,`text_id`) VALUES (10136, 14072),(10138, 14074),(10118, 14076);
DELETE FROM `gossip_menu_option` WHERE `menu_id` = @GOSSIP AND `id` IN (2, 3, 4, 5);
INSERT INTO `gossip_menu_option`(`menu_id`,`id`,`option_icon`,`option_text`,`option_id`,`npc_option_npcflag`,`action_menu_id`,`action_poi_id`,`box_coded`,`box_money`,`box_text`) VALUES
 (@GOSSIP, 2, 0,'Tell me about the Wispcloak Recipe.', 1, 1, 10136, 0, 0, 0,''),
 (@GOSSIP, 3, 0,'I am ready to learn the Wispcloak Recipe.', 1, 1, 0, 0, 0, 0,''),
 (@GOSSIP, 4, 0,'Tell me about the Deathchill Cloak Recipe.', 1, 1, 10138, 0, 0, 0,''),
 (@GOSSIP, 5, 0,'I am ready to learn the Deathchill Cloak Recipe.', 1, 1, 0, 0, 0, 0,'');
DELETE FROM `conditions` WHERE `SourceGroup` = 10118;
INSERT INTO `conditions`(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
 (14, 10118, 14076, 0, 7, 197, 420, 0, 0,'','Only show gossip text 14076 if player has Tailoring with skill level 420'),
 (15, 10118, 2, 0, 7, 197, 420, 0, 0,'','Only show gossip option if player has Tailoring with skill level 420'),
 (15, 10118, 2, 0, 25, 56016, 0, 0, 0,'','Only show gossip option if player has not learned spell 56016'),
 (15, 10118, 5, 0, 7, 197, 420, 0, 0,'','Only show gossip option if player has Tailoring with skill level 420 Alliance'),
 (15, 10118, 5, 0, 17, 41, 0, 0, 0,'','Only show gossip option if player has achievement Loremaster of Northrend Alliance'),
 (15, 10118, 5, 1, 7, 197, 420, 0, 0,'','Only show gossip option if player has Tailoring with skill level 420 Horde'),
 (15, 10118, 5, 1, 17, 1360, 0, 0, 0,'','Only show gossip option if player has achievement Loremaster of Northrend Horde'),
 (15, 10118, 5, 0, 25, 56016, 0, 0, 0,'','Only show gossip option if player has not learned spell 56016'),
 (15, 10118, 5, 1, 25, 56016, 0, 0, 0,'','Only show gossip option if player has not learned spell 56016'),
 (15, 10118, 4, 0, 7, 197, 420, 0, 0,'','Only show gossip option if player has Tailoring with skill level 420'),
 (15, 10118, 4, 0, 25, 56017, 0, 0, 0,'','Only show gossip option if player has not learned spell 56017'),
 (15, 10118, 3, 0, 7, 197, 420, 0, 0,'','Only show gossip option if player has Tailoring with skill level 420'),
 (15, 10118, 3, 0, 17, 1288, 0, 0, 0,'','Only show gossip option if player has achievement Northrend Dungeonmaster'),
 (15, 10118, 3, 0, 25, 56017, 0, 0, 0,'','Only show gossip option if player has not learned spell 56017');
-- NPC: Minion of Omen, http://wotlk.openwow.com/npc=15466
DELETE FROM `creature_ai_scripts` WHERE `id` = 2421506;
INSERT INTO `creature_ai_scripts`(`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
 (2421506, 15466, 8, 0, 100, 1, 26636, -1, 5000, 5000, 12, 15467, 6, 0, 41, 1000, 0, 0, 0, 0, 0, 0,'');
UPDATE `creature_template` SET `AIName` = 'EventAI' WHERE `entry` = 15466;
-- NPC: Rhapsody Shindigger, http://wotlk.openwow.com/npc=5634
UPDATE `creature_template` SET `gossip_menu_id` = 0 WHERE `entry` = 5634;
-- NPC: Time-Lost Proto Drake, http://wotlk.openwow.com/npc=32491
UPDATE `creature_template` SET `InhabitType` = 4 WHERE `entry` = 32491;
-- NPC: Demolisher Engineer Blastwrench, http://wotlk.openwow.com/npc=33669
UPDATE `creature_template` SET `modelid1` = 28781, `modelid2` = 0 WHERE `entry` = 33669;
-- NPC: Watch Commander Zalaphil, http://wotlk.openwow.com/npc=5809
UPDATE `creature` SET `position_z` = 24.3416 WHERE `guid` = 49456;
-- NPC: Salanar the Horseman, http://wotlk.openwow.com/npc=28653
UPDATE `gossip_scripts` SET `datalong2` = 0 WHERE `id` = 9739 AND `delay` = 0 AND `command` = 15 AND `datalong` = 52693 AND `datalong2` = 3;
-- NPC: Iskalder, http://wotlk.openwow.com/npc=30884
UPDATE `creature_template` SET `faction_A` = 1885,`faction_H` = 1885 WHERE `entry` = 30884;
-- NPC: Watch Commander Zalaphil, http://wotlk.openwow.com/npc=5809
UPDATE `creature` SET `position_z` = 24.3416 WHERE `guid` = 49456;
-- NPC: Brother Keltan, http://wotlk.openwow.com/npc=31261
UPDATE `creature_template` SET `unit_flags` = 0,`type_flags` = 0 WHERE `entry` = 31261;
UPDATE `creature_transport` SET `guid` = 77, `transport_entry` = 192241, `npc_entry` = 31261,
 `TransOffsetX` = -5.19514, `TransOffsetY` = -7.07149, `TransOffsetZ` = 33.9339,
 `TransOffsetO` = 4.65055 WHERE `npc_entry` = 31261;
-- NPC: Zedd, http://wotlk.openwow.com/npc=24145
UPDATE `creature_template` SET  `faction_A` = 1892 , `faction_H` = 1892 WHERE `entry` = 24145;
-- VEHICLE: Wintergrasp Tower Cannon, http://wotlk.openwow.com/npc=28366
UPDATE `creature_template` SET `InhabitType` = 4 WHERE `entry` = 28366;
-- VEHICLE: Antipersonnel Cannon, http://wotlk.openwow.com/npc=32795
UPDATE `creature_template` SET `mechanic_immune_mask` = 920633343 WHERE `entry` = 32795;
-- NPC: Headless Horseman, http://www.wowhead.com/npc=23682
DELETE FROM `creature_loot_template` WHERE `entry`=23682 AND `item` IN (33154,33292,37012,49126,49128,37011);
DELETE FROM `item_loot_template` WHERE `entry`=54516;
INSERT INTO `item_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(54516,33154, 25,1,0, 1, 1),
(54516,33292, 33,1,0, 1, 1),
(54516,37012,  3,1,0, 1, 1),
(54516,49126,  5,1,0, 1, 1),
(54516,49128,  8,1,0, 1, 1),
(54516,37011, 25,1,0, 1, 1),
(54516,40753,100,1,0, 2, 2),
(54516,34068, 33,1,0, 3, 6),
(54516,33226,100,1,0,20,20);

-- Quest: They Grow Up So Fast, http://ru.wowhead.com/quest=13906
 SET @pool1_id := 350; -- Orcs, Bloodelves
 SET @pool2_id := 351; -- Undead, Tauren, Troll
 DELETE FROM `pool_template` WHERE `entry` IN (@pool1_id, @pool2_id);
 INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES
 (@pool1_id, 1, 'Toxic Tolerance Daily-Quests'),
 (@pool2_id, 1, 'Toxic Tolerance Daily-Quests');
 DELETE FROM `pool_quest` WHERE `entry` IN (13889, 13915, 13903, 13904, 13905, 13917, 13916, 13914) AND `pool_entry` IN (@pool1_id, @pool2_id);
 INSERT INTO `pool_quest` (`entry`, `pool_entry`, `description`) VALUES
 (13889, @pool1_id, 'Hungry, Hungry Hatchling'),
 (13903, @pool1_id, 'Gorishi Grub'),
 (13904, @pool1_id, 'Poached, Scrambled, Or Raw?'),
 (13905, @pool1_id, 'Searing Roc Feathers'),
 (13915, @pool2_id, 'Hungry, Hungry Hatchling'),
 (13917, @pool2_id, 'Gorishi Grub'),
 (13916, @pool2_id, 'Poached, Scrambled, Or Raw?'),
 (13914, @pool2_id, 'Searing Roc Feathers');
 
 -- Quest: Toxic Tolerance, http://www.wowhead.com/quest=13850
SET @ENTRY=6508;
DELETE FROM `creature_ai_scripts` WHERE `creature_id`=@ENTRY;
UPDATE `creature_template` SET `AIName`='' WHERE `entry`=@ENTRY;
DELETE FROM `creature_template_addon` WHERE (`entry`=@ENTRY);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (@ENTRY, 0, 0, 0, 0, 0, 14108);
-- -------------------------------------------------------------------------}
-- ----------------------- Myth Project "GO" Update ------------------------
-- -------------------------------------------------------------------------{
-- GO: Haal'eshi Altar, http://wotlk.openwow.com/object=181606
UPDATE `gossip_menu_option` SET `action_script_id` = 0 WHERE `menu_id` = 10219;
DELETE FROM `gossip_scripts` WHERE `id` = 10219;
UPDATE `gameobject_template` SET `type` = 10,`data3` = 30000,`data19` = 10219 WHERE `entry` = 181606;
-- GO: Kaliri Nest, http://wotlk.openwow.com/object=181582
UPDATE `gameobject_template` SET `data3` = 30000 WHERE `entry` = 181582;
-- GO: Dedication of Honor, http://wotlk.openwow.com/object=202443
UPDATE `gameobject_template` SET `ScriptName` = 'go_memorial_plaque' WHERE `entry` = 202443;
DELETE FROM `trinity_string` WHERE `entry` = 12001;
INSERT INTO `trinity_string`(`entry`,`content_default`,`content_loc1`,`content_loc2`,`content_loc3`,`content_loc4`,`content_loc5`,`content_loc6`,`content_loc7`,`content_loc8`) VALUES
 (12001,'See the fall of the Lich King.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
-- GO: 133234, http://wotlk.openwow.com/object=133234
UPDATE `gameobject_template` SET `data0` = 0,`data1` = 0,`data3` = 0,`data5` = 0,`data6` = 0 WHERE `entry` = 130511;
UPDATE `gameobject_template` SET `data2` = 30000 WHERE `entry` = 133234;
-- -------------------------------------------------------------------------}
-- --------------------- Myth Project "Spells" Update ----------------------
-- -------------------------------------------------------------------------{
-- Rogue: Shadowstep, http://wotlk.openwow.com/spell=36554
DELETE FROM `spell_script_names` WHERE `spell_id` = -36554;
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES (-36554, 'spell_rog_shadow_dance');
-- UNKNOWN SPELL: Horde Control Phase Shift, http://wotlk.openwow.com/spell=55773
-- UNKNOWN SPELL: Alliance Control Phase Shift, http://wotlk.openwow.com/spell=55774
-- UNKNOWN SPELL: Activate Secondary Spec, http://wotlk.openwow.com/spell=63644
-- UNKNOWN SPELL: Activate Primary Spec, http://wotlk.openwow.com/spell=63645
-- Warrior: Rampage, http://wotlk.openwow.com/spell=29801
-- Hunter: Master's Call, http://wotlk.openwow.com/spell=54216
-- Rogue: Master of Subtlety, http://wotlk.openwow.com/spell=31223
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (-55773, -55774, 54216, 63644, 63645) AND `spell_effect` IN (-56651, -67830, -63167, -58427, -29801, -31665);
INSERT INTO `spell_linked_spell`(`spell_trigger`,`spell_effect`,`type`,`comment`) VALUES
 (54216, -56651, 1, 'Removes Masters Call stun immunity'),
 (-55774, -67830, 0,'Remove Ride Vehicle'),
 (-55773, -67830, 0,'Remove Ride Vehicle'),
 (63644, -63167, 0,'Decimation remove when change talents'),
 (63644, -58427, 0,'Overkill remove when change talents'),
 (63644, -29801, 0,'Rampage remove when change talents'),
 (63644, -31665, 0,'Master of Subtlety remove when change talents'),
 (63645, -63167, 0,'Decimation remove when change talents'),
 (63645, -58427, 0,'Overkill remove when change talents'),
 (63645, -29801, 0,'Rampage remove when change talents'),
 (63645, -31665, 0,'Master of Subtlety remove when change talents');
-- Warrior: Taste for Blood (Rank 1), http://wotlk.openwow.com/spell=56636
-- Warrior: Taste for Blood (Rank 2), http://wotlk.openwow.com/spell=56637
-- Warrior: Taste for Blood (Rank 3), http://wotlk.openwow.com/spell=56638
DELETE FROM `spell_proc_event` WHERE `entry` IN (56636, 56637, 56638);
INSERT INTO `spell_proc_event` VALUES
 (56636, 0, 4, 0x00000020, 0, 0, 0, 0x00040000, 0, 0, 6), -- Taste for Blood (Rank 1)
 (56637, 0, 4, 0x00000020, 0, 0, 0, 0x00040000, 0, 0, 6), -- Taste for Blood (Rank 2)
 (56638, 0, 4, 0x00000020, 0, 0, 0, 0x00040000, 0, 0, 6); -- Taste for Blood (Rank 3)
-- Warrior: Revenge, http://wotlk.openwow.com/spell=6572
UPDATE `spell_bonus_data` SET `ap_bonus` = 0.310 WHERE `entry` = 6572;
-- Paladin: Righteous Defense, http://wotlk.openwow.com/spell=31789
DELETE FROM `spell_script_names` WHERE `spell_id` = -31789;
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
 (-31789,'spell_pal_righteous_defense');
-- Hunter: Furious Howl, http://wotlk.openwow.com/spell=24604
DELETE FROM `spell_script_names` WHERE `spell_id` IN (-24604, 53434);
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
 (-24604,'spell_hun_target_only_pet_and_owner'),
 (53434,'spell_hun_target_only_pet_and_owner');
-- DK: Unholy Blight, http://wotlk.openwow.com/spell=49194
DELETE FROM `spell_script_names` WHERE `spell_id` = 50536;
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES (50536,'spell_dk_unholy_blight');
-- DK: Death Coil, http://wotlk.openwow.com/spell=47541
DELETE FROM `spell_bonus_data` WHERE `entry` IN (6489, 6789);
INSERT INTO `spell_bonus_data`(`entry`,`direct_bonus`,`dot_bonus`,`ap_bonus`,`ap_dot_bonus`,`comments`) VALUES
 (6789, 0.2143, 0, 0, 0,'Spell Power Coeff for Death Coil');
DELETE FROM `spell_script_names` WHERE `spell_id` IN (-47541, 52375, 59134, -62900);
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES 
 (-47541,'spell_dk_death_coil'),
 (52375,'spell_dk_death_coil'),
 (59134,'spell_dk_death_coil'),
 (-62900,'spell_dk_death_coil');
-- DK: Death Strike, http://wotlk.openwow.com/spell=71489
UPDATE `spell_proc_event` SET `SpellFamilyMask0` = 0x10 WHERE `entry` = 60132;
-- DK: Summon Gargoyle, http://wotlk.openwow.com/spell=50514
DELETE FROM `spell_bonus_data` WHERE `entry` = 51963;
INSERT INTO `spell_bonus_data`(`entry`,`direct_bonus`,`comments`) VALUES
 (51963, 1, "Death Knight - Gargoyle Strike");
-- DK: Dancing Rune Weapon, http://wotlk.openwow.com/spell=49028
UPDATE `creature_template` SET `unit_flags` = 33685504 WHERE `entry` = 27893;
DELETE FROM `spell_proc_event` WHERE `entry` = 49028; 
INSERT INTO `spell_proc_event` (entry, procFlags) VALUES ("49028", "69652");
-- Hunter: Aspect of the Beast, http://wotlk.openwow.com/spell=13161
DELETE FROM `spell_script_names` WHERE `spell_id` = 13161;
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
 (13161, 'spell_hun_aspect_of_the_beast');
-- Shaman: Fire Elemental Totem, http://wotlk.openwow.com/spell=2894
-- Shaman: Fire Elemental Totem, http://wotlk.openwow.com/spell=2062
UPDATE `creature_template` SET `ScriptName`='npc_fire_elemental' WHERE `entry`=15438;
UPDATE `creature_template` SET `ScriptName`='npc_earth_elemental' WHERE `entry`=15352;
DELETE FROM `spell_bonus_data` WHERE `entry` IN (13376,57984);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`,`ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(13376,0.032,-1,-1,-1,'Greater Fire Elemental - Fire Shield'),
(57984,0.4289,-1,-1,-1,'Greater Fire Elemental - Fire Blast');
-- -------------------------------------------------------------------------}
-- ---------------------- Myth Project "Items" Update ----------------------
-- -------------------------------------------------------------------------{
-- Item: Titanium Lockbox, http://old.wowhead.com/item=43624
-- Item: Book of Glyph Mastery, http://old.wowhead.com/item=45912
DELETE FROM `item_loot_template` WHERE `entry` = 43624 AND `item` = 45912;
INSERT INTO `item_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES 
 (43624, 45912, 14, 1, 0, 1, 1);
-- Item: Tiny Abomination in a Jar (Normal), http://old.wowhead.com/item=50351
-- Item: Tiny Abomination in a Jar (Heroic), http://old.wowhead.com/item=50706
DELETE FROM `spell_proc_event` WHERE `entry` IN (71406, 71545);
INSERT INTO `spell_proc_event`(`entry`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`procFlags`,`procEx`,`ppmRate`,`CustomChance`,`Cooldown`) VALUES
 (71406, 0, 0, 0, 0, 0, 0, 0, 0, 45, 0),
 (71545, 0, 0, 0, 0, 0, 0, 0, 0, 45, 0);
-- Rogue: Glyph of Backstab, http://www.wowwiki.com/Glyph_of_Backstab
DELETE FROM `spell_dbc` WHERE `id` = 63975;
INSERT INTO `spell_dbc`(`Id`,`Dispel`,`Mechanic`,`Attributes`,`AttributesEx`,`AttributesEx2`,`AttributesEx3`,`AttributesEx4`,`AttributesEx5`,`Stances`,`StancesNot`,`Targets`,`CastingTimeIndex`,`AuraInterruptFlags`,`ProcFlags`,`ProcChance`,`ProcCharges`,`MaxLevel`,`BaseLevel`,`SpellLevel`,`DurationIndex`,`RangeIndex`,`StackAmount`,`EquippedItemClass`,`EquippedItemSubClassMask`,`EquippedItemInventoryTypeMask`,`Effect1`,`Effect2`,`Effect3`,`EffectDieSides1`,`EffectDieSides2`,`EffectDieSides3`,`EffectRealPointsPerLevel1`,`EffectRealPointsPerLevel2`,`EffectRealPointsPerLevel3`,`EffectBasePoints1`,`EffectBasePoints2`,`EffectBasePoints3`,`EffectMechanic1`,`EffectMechanic2`,`EffectMechanic3`,`EffectImplicitTargetA1`,`EffectImplicitTargetA2`,`EffectImplicitTargetA3`,`EffectImplicitTargetB1`,`EffectImplicitTargetB2`,`EffectImplicitTargetB3`,`EffectRadiusIndex1`,`EffectRadiusIndex2`,`EffectRadiusIndex3`,`EffectApplyAuraName1`,`EffectApplyAuraName2`,`EffectApplyAuraName3`,`EffectAmplitude1`,`EffectAmplitude2`,`EffectAmplitude3`,`EffectMultipleValue1`,`EffectMultipleValue2`,`EffectMultipleValue3`,`EffectMiscValue1`,`EffectMiscValue2`,`EffectMiscValue3`,`EffectMiscValueB1`,`EffectMiscValueB2`,`EffectMiscValueB3`,`EffectTriggerSpell1`,`EffectTriggerSpell2`,`EffectTriggerSpell3`,`EffectSpellClassMaskA1`,`EffectSpellClassMaskA2`,`EffectSpellClassMaskA3`,`EffectSpellClassMaskB1`,`EffectSpellClassMaskB2`,`EffectSpellClassMaskB3`,`EffectSpellClassMaskC1`,`EffectSpellClassMaskC2`,`EffectSpellClassMaskC3`,`MaxTargetLevel`,`SpellFamilyName`,`SpellFamilyFlags1`,`SpellFamilyFlags2`,`SpellFamilyFlags3`,`MaxAffectedTargets`,`DmgClass`,`PreventionType`,`DmgMultiplier1`,`DmgMultiplier2`,`DmgMultiplier3`,`AreaGroupId`,`SchoolMask`,`Comment`) VALUES
 (63975, 0, 0, 384, 0, 0, 262144, 128, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, -1, 0, 0, 77, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, "Glyph of Blackstab - 3.2.2");
-- Item: Book of Glyph Mastery, http://wotlk.openwow.com/item=45912
UPDATE `skill_discovery_template` SET `reqSkillValue` = 0 WHERE `spellId` IN (64309, 64268, 64295) AND `reqSpell` = 64323;
-- Item: Arcanum of Torment, http://wotlk.openwow.com/item=44149
UPDATE `npc_vendor` SET `item` = 44149 WHERE `entry` = 32538 AND `item` = 50367;
-- Item: Trauma, http://wotlk.openwow.com/item=50028
-- Item: Trauma (H), http://wotlk.openwow.com/item=50685
DELETE FROM `spell_proc_event` WHERE `entry` IN (71865, 71868);
INSERT INTO `spell_proc_event`(`entry`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`procFlags`,`procEx`,`ppmRate`,`CustomChance`,`Cooldown`) VALUES
-- (71865, 0, 0, 4294967295, 4294967295, 4294967295, 0, 131072, 0, 6, 0),
-- (71868, 0, 0, 4294967295, 4294967295, 4294967295, 0, 131072, 0, 6, 0);
 (71865, 0, 0, 0, 0, 0, 278528, 24, 0, 1, 0),
 (71868, 0, 0, 0, 0, 0, 278528, 24, 0, 1, 0);
-- Item: Extract of Necromantic Power, http://wotlk.openwow.com/item=40373
UPDATE `spell_proc_event` SET `Cooldown` = 15 WHERE `entry` = 60487;
-- Item: Phylactery of the Nameless Lich, http://wotlk.openwow.com/item=50360
UPDATE `spell_proc_event` SET `cooldown` = 100 WHERE `entry` IN (71606, 71637);
-- Item: Aegis of Preservation, http://wotlk.openwow.com/item=19345
DELETE FROM `spell_bonus_data` WHERE `entry` =  23781; 
INSERT INTO `spell_bonus_data`(`entry`,`comments`) VALUES ( 23781, 'Aegis Heal - no scaling'); 
-- -------------------------------------------------------------------------}
-- ------------------ Myth Project "Game Mech." Update ---------------------
-- -------------------------------------------------------------------------{
-- WARDEN - JUMP HACK
UPDATE `warden_data_result` SET `result` = '75440FB75E',`length` = 5 WHERE `id` = 785;
-- NPC: Paladin Trainter (Alliance)
UPDATE `npc_trainer` SET `spell` = 23214 WHERE `spell` = 23215;
DELETE FROM `spell_required` WHERE `spell_id` = 23214 AND `req_spell` = 33391;
INSERT INTO `spell_required`(`spell_id`,`req_spell`) VALUES (23214, 33391);
-- NPC: Paladin Trainter (Horde)
UPDATE `npc_trainer` SET `spell` = 34767 WHERE `spell` = 34766;
DELETE FROM `spell_required` WHERE `spell_id` = 34767 AND `req_spell` = 33391;
INSERT INTO `spell_required`(`spell_id`,`req_spell`) VALUES (34767, 33391);
-- Quest: Chamber of Secrets, http://wotlk.openwow.com/quest=12616
DELETE FROM `disables` WHERE `sourceType` = 1 AND `entry` = 12616;
INSERT INTO `disables` VALUES
 (1, 12616, 0, 0, 0, "Old Quest");
-- Dual Spec learning
DELETE FROM `gossip_scripts` WHERE `id` = 50099;
UPDATE `gossip_menu_option` SET `box_money` = 0 WHERE `option_id` = 1 AND `box_money` = 10000000;
UPDATE `gossip_menu_option` SET option_id = 18, box_money = 10000000, `action_script_id` = 0  WHERE  menu_id = 10371;

DELETE FROM `gossip_scripts` WHERE `id` = 24752;
UPDATE `spell_area` SET `racemask` = 690 WHERE `spell` IN (35481, 35480) AND `area` = 4100;

DROP TABLE IF EXISTS `player_factionchange_quests`;
CREATE TABLE `player_factionchange_quests` (
  `alliance_id` INT(8) NOT NULL,
  `commentA` VARCHAR(255) DEFAULT NULL,
  `horde_id` INT(8) NOT NULL,
  `commentH` VARCHAR(255) DEFAULT NULL,
  PRIMARY KEY (`alliance_id`,`horde_id`)
) DEFAULT CHARSET=UTF8;

INSERT INTO `player_factionchange_quests` VALUES
 (12742, "A Special Surprise(A)", 12739, "A Special Surprise(H)"),
 (12743, "A Special Surprise(A)", 12747, "A Special Surprise(H)"),
 (12744, "A Special Surprise(A)", 12748, "A Special Surprise(H)"),
 (12745, "A Special Surprise(A)", 12749, "A Special Surprise(H)"),
 (12746, "A Special Surprise(A)", 12750, "A Special Surprise(H)"),
 (13188, "Where Kings Walk", 13189, "Warchief''s Blessing"),
 (13718, "The Valiant's Charge", 13697, "The Valiant's Charge"),
 (13714, "The Valiant's Charge", 13719, "The Valiant's Charge"),
 (13716, "The Valiant's Charge", 13722, "The Valiant's Charge"),
 (13717, "The Valiant's Charge", 13721, "The Valiant's Charge"),
 (13715, "The Valiant's Charge", 13720, "The Valiant's Charge"),
 (13667, "The Argent Tournament", 13668, "The Argent Tournament"),
 (13734, "A Champion Rises", 13740, "A Champion Rises"),
 (13735, "A Champion Rises", 13739, "A Champion Rises"),
 (13733, "A Champion Rises", 13738, "A Champion Rises"),
 (13702, "A Champion Rises", 13736, "A Champion Rises"),
 (13732, "A Champion Rises", 13737, "A Champion Rises"),
 (13828, "Mastery Of Melee", 13829, "Mastery Of Melee"),
 (13672, "Up To The Challenge", 13678, "Up To The Challenge"),
 (13679, "The Aspirant's Challenge", 13680, "The Aspirant's Challenge"),
 (13835, "Mastery Of The Shield-Breaker", 13838, "Mastery Of The Shield-Breaker"),
 (13837, "Mastery Of The Charge", 13839, "Mastery Of The Charge"),
 (13724, "The Valiant's Challenge", 13731, "The Valiant's Challenge"),
 (13723, "The Valiant's Challenge", 13728, "The Valiant's Challenge"),
 (13713, "The Valiant's Challenge", 13727, "The Valiant's Challenge"),
 (13699, "The Valiant's Challenge", 13726, "The Valiant's Challenge"),
 (13725, "The Valiant's Challenge", 13729, "The Valiant's Challenge"),
 (13690, "Valiant Of The Exodar", 13696, "Valiant Of Silvermoon"),
 (13705, "Valiant Of The Exodar", 13711, "Valiant Of Silvermoon"),
 (13593, "A Valiant Of Stormwind", 13707, "A Valiant Of Orgrimmar"),
 (13684, "A Valiant Of Stormwind", 13691, "A Valiant Of Orgrimmar"),
 (13685, "Valiant Of Ironforge", 13693, "Valiant Of Sen'jin"),
 (13703, "Valiant Of Ironforge", 13708, "Valiant Of Sen'jin"),
 (13689, "A Valiant Of Darnassus", 13695, "Valiant Of Undercity"),
 (13706, "A Valiant Of Darnassus", 13710, "Valiant Of Undercity"),
 (13688, "Valiant Of Gnomeregan", 13694, "Valiant Of Thunder Bluff"),
 (13704, "Valiant Of Gnomeregan", 13709, "Valiant Of Thunder Bluff"),
 (13633, "The Black Knight of Westfall?", 13634, "The Black Knight of Silverpine?"); 

INSERT IGNORE INTO `player_factionchange_items`(`race_A`,`alliance_id`,`commentA`,`race_H`,`horde_id`,`commentH`) VALUES
 (0, 44503, "Schematic: Mekgineers Chopper", 0, 44502, "Schematic: Mechano-Hog"),
 (0, 47003, "Dawnbreaker Greaves", 0, 47430, "Dawnbreaker Sabatons"),
 (0, 48076, "Velen's Mantle of Conquest", 0, 48101, "Zabra's Mantle of Conquest"),
 (0, 48075, "Velen's Raiments of Conquest", 0, 48100, "Zabra's Raiments of Conquest"),
 (0, 48074, "Velen's Pants of Conquest", 0, 48099, "Zabra's Pants of Conquest"),
 (0, 48072, "Velen's Handwraps of Conquest", 0, 48097, "Zabra's Handwraps of Conquest"),
 (0, 48073, "Velen's Circlet of Conquest", 0, 48098, "Zabra's Circlet of Conquest"),
 (0, 47711, "Girdle of the Nether Champion", 0, 47870, "Belt of the Nether Championt"),
 (0, 44235, "Reins of the Traveler's Tundra Mammoth", 0, 44234, "Reins of the Traveler's Tundra Mammoth"),
 (0, 54651, "Gnomeregan Pride", 0, 54653, "Darkspear Pride"),
 (0, 15198, "Knight's Colors", 0, 15199, "Stone Guard's Herald"),
 (0, 47937, "Girdle of the Nether Champion", 0, 48009, "Belt of the Nether Champion");

INSERT IGNORE INTO `player_factionchange_achievement`(`alliance_id`,`horde_id`) VALUES
 (58, 593),     (434, 445), (435, 444),     (436, 447),
 (437, 448),    (438, 469), (439, 451),     (442, 454),
 (470, 468),    (473, 446), (1280, 1279),   (1466, 926),
 (2419, 2497),  (3356, 3357);

INSERT IGNORE INTO `player_factionchange_spells`(`alliance_id`,`horde_id`) VALUES
 (60867, 60866),    (61425, 61447), (67064, 67144), (67065, 67147),
 (67066, 67146),    (67079, 67145), (67080, 67136), (67082, 67138),
 (67083, 67143),    (67084, 67140), (67085, 67141), (67086, 67142),
 (67087, 67139),    (67091, 67130), (67092, 67131), (67093, 67132),
 (67095, 67134),    (67096, 67135);

DELETE FROM `warden_data_result` WHERE `id` IN (791, 788, 789, 790);
INSERT INTO `warden_data_result` VALUES
 (791, 243,'','', 7728616, 1, 75, 1,'Simplefly', 1),
 (788, 243,'','', 9993834, 1, 74, 1,'Simplefly', 1),
 (789, 243,'','', 9993835, 1, 52, 1,'Simplefly', 1),
 (790, 243,'','', 7277234, 8,'85F60F86F2000000', 1,'Simplefly - speedhack', 1);
-- Game Event: Lunar Festival
UPDATE `game_event` SET `start_time` = '2013-01-20 00:01:00' WHERE `eventEntry` = 7;

DELETE FROM `creature_addon` WHERE `guid` IN (
 77152, 104533, 104532, 104531, 104530, 104539, 104538,
 104537, 104535, 104534, 102621, 102622, 102623, 102624);
-- DB/Spawns: Increase some mining pools to more reasonable values
UPDATE `pool_template` SET `max_limit` = 45 WHERE `entry` = 2002;
UPDATE `pool_template` SET `max_limit` = 40 WHERE `entry` = 2007;
UPDATE `pool_template` SET `max_limit` = 25 WHERE `entry` = 2004;
UPDATE `pool_template` SET `max_limit` = 20 WHERE `entry` IN (2009, 2018, 2005);
UPDATE `pool_template` SET `max_limit` = 15 WHERE `entry` = 2008;
UPDATE `pool_template` SET `max_limit` = 10 WHERE `entry` IN (2001, 2012, 2014);
UPDATE `pool_template` SET `max_limit` = 3 WHERE `entry` = 2016;
-- -------------------------------------------------------------------------}
-- ---------------------------------- Ulduar -------------------------------
-- -------------------------------------------------------------------------{
-- NPC: Ancient Rune Giant, http://old.wowhead.com/npc=32873
UPDATE `creature` SET `spawnMask` = 3 WHERE `guid` = 131858;
-- -------------------------------------------------------------------------}
-- -------------------------- Trial of the Crusader ------------------------
-- -------------------------------------------------------------------------{
-- NPC: Icehowl, http://wotlk.openwow.com/npc=35447
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = -67660 AND `spell_effect` = 68667;
INSERT INTO `spell_linked_spell`(`spell_trigger`,`spell_effect`,`type`,`comment`) VALUES
 (-67660, 68667, 0, "Icehowl - Surge of Adrenaline");
-- -------------------------------------------------------------------------}
-- -------------------------- the Obsidian Sanctum -------------------------
-- -------------------------------------------------------------------------{
-- NPC: Shadron, http://wotlk.openwow.com/npc=30451
-- NPC: Shadron (H), http://wotlk.openwow.com/npc=31520
-- NPC: Vesperon, http://wotlk.openwow.com/npc=30449
-- NPC: Vesperon (H), http://wotlk.openwow.com/npc=31535
-- NPC: Tenebron, http://wotlk.openwow.com/npc=30452
-- NPC: Tenebron (H), http://wotlk.openwow.com/npc=31534
UPDATE `creature_template` SET `InhabitType` = 4 WHERE `entry` IN (30451, 31520, 30449, 31535, 30452, 31534);
-- NPC: Sartharion, http://wotlk.openwow.com/npc=28860
-- NPC: Twilight Whelp, http://wotlk.openwow.com/npc=30890
-- NPC: Sartharion Twilight Whelp, http://wotlk.openwow.com/npc=31214
UPDATE `creature_template` SET `mindmg` = 500,`maxdmg` = 1000 WHERE `entry` IN (30890, 31214);
UPDATE `creature_template` SET `mindmg` = 1000,`maxdmg` = 2000 WHERE `entry` IN (31540, 31548);
-- -------------------------------------------------------------------------}
-- -------------------------- Culling Of Stratholme ------------------------
-- -------------------------------------------------------------------------{
-- COS: Final loot
UPDATE `gameobject_template` SET `flags` = 4 WHERE `entry` IN (190663, 193597);
UPDATE `gameobject` SET `phaseMask` = 100, `spawntimesecs` = 43200 WHERE `guid` IN (30032, 53514);
-- -------------------------------------------------------------------------}
-- ---------------------------- Icecrown Citadel ---------------------------
-- -------------------------------------------------------------------------{
UPDATE `creature` SET `orientation` = 3.01985 WHERE `guid` = 101906;
UPDATE `creature` SET `orientation` = 3.06148 WHERE `guid` = 101936;
UPDATE `creature` SET `orientation` = 4.72181 WHERE `guid` = 101937;
UPDATE `creature` SET `orientation` = 3.23505 WHERE `guid` = 101996;
UPDATE `creature` SET `orientation` = 4.66762 WHERE `guid` = 115555;
UPDATE `creature` SET `orientation` = 3.18323 WHERE `guid` = 115742;
UPDATE `creature_template` SET `Health_mod` = 5 WHERE `entry` = 38233;
UPDATE `creature_template` SET `Health_mod` = 9 WHERE `entry` = 38460;
-- NPC: Prince Valanar, http://wotlk.openwow.com/npc=37970
-- NPC: Prince Keleseth, http://wotlk.openwow.com/npc=37972
-- NPC: Prince Taldaram, http://wotlk.openwow.com/npc=37973
UPDATE `creature` SET `spawntimesecs` = 604800 WHERE `guid` = 104364;
-- NPC: Rotface, http://wotlk.openwow.com/npc=36627
DELETE FROM `creature_text` WHERE `entry` IN (36628, 36627) AND `groupid` = 9;
INSERT INTO `creature_text`(`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
 (36627, 9, 0,'|TInterface\\Icons\\ability_creature_disease_02.blp:16|tYou have |cFF00FF00Mutated Infection!|r', 42, 0, 0, 0, 0, 0,'Rotface - EMOTE_MUTATED_INFECTION');
-- NPC: Lich King, http://wotlk.openwow.com/npc=36597
UPDATE `spell_target_position` SET `target_position_z` = 1041.00 WHERE `id` = 70860;
-- Achievements: Portal Jockey, http://wotlk.openwow.com/achievement=4579 , 4619
UPDATE `achievement_criteria_data` SET `value1` = 2 WHERE `criteria_id` = 12979 AND `type` = 12;
UPDATE `achievement_criteria_data` SET `value1` = 1 WHERE `criteria_id` = 12971 AND `type` = 12;
-- NPC: Mutated Abomination, http://www.wowhead.com/npc=37672 (ICC25H)
UPDATE `creature_template` SET `spell3` = 70542 WHERE `entry` = 38790;
-- -------------------------------------------------------------------------}
-- ------------------------------ Ruby Sanctum -----------------------------
-- -------------------------------------------------------------------------{
-- NPC: Baltharus the Warborn, http://wotlk.openwow.com/npc=39751
DELETE FROM `script_texts` WHERE `entry` = -1999981;
INSERT INTO `script_texts`(`npc_entry`,`entry`,`content_default`,`content_loc1`,`content_loc2`,`content_loc3`,`content_loc4`,`content_loc5`,`content_loc6`,`content_loc7`,`content_loc8`,`sound`,`type`,`language`,`emote`,`comment`) VALUES 
(39751, -1999981,'Twice the pain and half the fun', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 6, 0, 0,'SAY_BALTHARUS_YELL');
-- NPC: Halion, http://wotlk.openwow.com/npc=39863
DELETE FROM `creature_template` WHERE `entry` IN (39863, 39864, 39944, 39945);
INSERT INTO `creature_template`(`entry`,`difficulty_entry_1`,`difficulty_entry_2`,`difficulty_entry_3`,`KillCredit1`,`KillCredit2`,`modelid1`,`modelid2`,`modelid3`,`modelid4`,`name`,`subname`,`IconName`,`gossip_menu_id`,`minlevel`,`maxlevel`,`exp`,`faction_A`,`faction_H`,`npcflag`,`speed_walk`,`speed_run`,`scale`,`rank`,`mindmg`,`maxdmg`,`dmgschool`,`attackpower`,`dmg_multiplier`,`baseattacktime`,`rangeattacktime`,`unit_class`,`unit_flags`,`dynamicflags`,`family`,`trainer_type`,`trainer_spell`,`trainer_class`,`trainer_race`,`minrangedmg`,`maxrangedmg`,`rangedattackpower`,`type`,`type_flags`,`lootid`,`pickpocketloot`,`skinloot`,`resistance1`,`resistance2`,`resistance3`,`resistance4`,`resistance5`,`resistance6`,`spell1`,`spell2`,`spell3`,`spell4`,`spell5`,`spell6`,`spell7`,`spell8`,`PetSpellDataId`,`VehicleId`,`mingold`,`maxgold`,`AIName`,`MovementType`,`InhabitType`,`Health_mod`,`Mana_mod`,`Armor_mod`,`RacialLeader`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`movementId`,`RegenHealth`,`equipment_id`,`mechanic_immune_mask`,`flags_extra`,`ScriptName`,`WDBVerified`) VALUES
 (39863, 39864, 39944, 39945, 0, 0, 31952, 0, 0, 0, "Halion", "The Twilight Destroyer", '', 0, 83, 83, 2, 14, 14, 0, 1, 1.42857, 1, 3, 468, 702, 0, 175, 74.2, 2000, 2000, 1, 768, 0, 0, 0, 0, 0, 0, 374, 562, 140, 2, 108, 39863, 0, 39863, 0, 0, 0, 0, 0, 0, 26662, 74524, 74562, 74531, 0, 0, 0, 0, 0, 0, 0, 0, "", 0, 3, 800, 1, 1, 0, 0, 0, 0, 0, 0, 0, 150, 1, 0, 803160063, 1, "boss_halion_real", 12340),
 (39864, 0, 0, 0, 0, 0, 31952, 0, 0, 0, "Halion (1)", "The Twilight Destroyer", '', 0, 83, 83, 2, 14, 14, 0, 1, 1.42857, 1, 3, 468, 702, 0, 175, 112.2, 2000, 2000, 1, 768, 0, 0, 0, 0, 0, 0, 374, 562, 140, 2, 108, 39864, 0, 39863, 0, 0, 0, 0, 0, 0, 26662, 74524, 74562, 74531, 0, 0, 0, 0, 0, 0, 0, 0, "", 0, 3, 2900, 1, 1, 0, 0, 0, 0, 0, 0, 0, 150, 1, 0, 803160063, 1, "", 12340),
 (39944, 0, 0, 0, 0, 0, 31952, 0, 0, 0, "Halion (2)", "The Twilight Destroyer", '', 0, 83, 83, 2, 14, 14, 0, 1, 1.42857, 1, 3, 468, 702, 0, 175, 112.2, 2000, 2000, 1, 768, 0, 0, 0, 0, 0, 0, 374, 562, 140, 2, 108, 39944, 0, 39863, 0, 0, 0, 0, 0, 0, 26662, 74524, 74562, 74531, 0, 0, 0, 0, 0, 0, 0, 0, "", 0, 3, 1100, 1, 1, 0, 0, 0, 0, 0, 0, 0, 150, 1, 0, 803160063, 1, "", 12340),
 (39945, 0, 0, 0, 0, 0, 31952, 0, 0, 0, "Halion (3)", "The Twilight Destroyer", '', 0, 83, 83, 2, 14, 14, 0, 1, 1.42857, 1, 3, 468, 702, 0, 175, 168.2, 2000, 2000, 1, 768, 0, 0, 0, 0, 0, 0, 374, 562, 140, 2, 108, 39945, 0, 39863, 0, 0, 0, 0, 0, 0, 26662, 74524, 74562, 74531, 0, 0, 0, 0, 0, 0, 0, 0, "", 0, 3, 4200, 1, 1, 0, 0, 0, 0, 0, 0, 0, 150, 1, 0, 803160063, 1, "", 12340);
DELETE FROM `creature_loot_template` WHERE `entry` IN (39863, 39864, 39944, 39945);
INSERT INTO `creature_loot_template`(`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
 (39863, 38557, 0, 1, 1, 8, 12),    (39863, 49426, 100, 1, 0, 3, 3),
 (39863, 53103, 0, 1, 2, 1, 1),     (39863, 53110, 0, 1, 2, 1, 1),
 (39863, 53111, 0, 1, 2, 1, 1),     (39863, 53112, 0, 1, 1, 1, 1),
 (39863, 53113, 0, 1, 1, 1, 1),     (39863, 53114, 0, 1, 1, 1, 1),
 (39863, 53115, 0, 1, 2, 1, 1),     (39863, 53116, 0, 1, 1, 1, 1),
 (39863, 53117, 0, 1, 1, 1, 1),     (39863, 53118, 0, 1, 1, 1, 1),
 (39863, 53119, 0, 1, 2, 1, 1),     (39863, 53121, 0, 1, 2, 1, 1),
 (39864, 49426, 100, 1, 0, 3, 3),   (39864, 53125, 0, 1, 2, 1, 1),
 (39864, 53126, 0, 1, 2, 1, 1),     (39864, 53127, 0, 1, 3, 1, 1),
 (39864, 53129, 0, 1, 3, 1, 1),     (39864, 53132, 0, 1, 2, 1, 1),
 (39864, 53133, 0, 1, 1, 1, 1),     (39864, 53134, 0, 1, 3, 1, 1),
 (39864, 53486, 0, 1, 1, 1, 1),     (39864, 53487, 0, 1, 3, 1, 1),
 (39864, 53488, 0, 1, 3, 1, 1),     (39864, 53489, 0, 1, 2, 1, 1),
 (39864, 53490, 0, 1, 3, 1, 1),     (39864, 54569, 0, 1, 1, 1, 1),
 (39864, 54571, 0, 1, 2, 1, 1),     (39864, 54572, 0, 1, 1, 1, 1),
 (39864, 54573, 0, 1, 1, 1, 1),     (39944, 33568, 0, 1, 1, 11, 12),
 (39944, 38557, 0, 1, 1, 8, 12),    (39944, 49426, 100, 1, 0, 3, 3),
 (39944, 54556, 0, 1, 1, 1, 1),     (39944, 54557, 0, 1, 2, 1, 1),
 (39944, 54558, 0, 1, 2, 1, 1),     (39944, 54559, 0, 1, 1, 1, 1),
 (39944, 54560, 0, 1, 2, 1, 1),     (39944, 54561, 0, 1, 2, 1, 1),
 (39944, 54562, 0, 1, 1, 1, 1),     (39944, 54563, 0, 1, 2, 1, 1),
 (39944, 54564, 0, 1, 2, 1, 1),     (39944, 54565, 0, 1, 1, 1, 1),
 (39944, 54566, 0, 1, 1, 1, 1),     (39944, 54567, 0, 1, 1, 1, 1),
 (39945, 49426, 100, 1, 0, 3, 3),   (39945, 54576, 0, 1, 2, 1, 1),
 (39945, 54577, 0, 1, 1, 1, 1),     (39945, 54578, 0, 1, 1, 1, 1),
 (39945, 54579, 0, 1, 1, 1, 1),     (39945, 54580, 0, 1, 3, 1, 1),
 (39945, 54581, 0, 1, 3, 1, 1),     (39945, 54582, 0, 1, 3, 1, 1),
 (39945, 54583, 0, 1, 1, 1, 1),     (39945, 54584, 0, 1, 3, 1, 1),
 (39945, 54585, 0, 1, 3, 1, 1),     (39945, 54586, 0, 1, 1, 1, 1),
 (39945, 54587, 0, 1, 1, 1, 1),     (39945, 54588, 0, 1, 2, 1, 1),
 (39945, 54589, 0, 1, 2, 1, 1),     (39945, 54590, 0, 1, 2, 1, 1),
 (39945, 54591, 0, 1, 2, 1, 1);
-- NPC: Valithria Dreamwalker, http://wotlk.openwow.com/npc=36789
-- UNKNOWN SPELL: Summon Suppresser, http://wotlk.openwow.com/spell=70936
-- UNKNOWN SPELL: Twisted Nightmares, http://wotlk.openwow.com/spell=71941
DELETE FROM `spell_script_names` WHERE `spell_id` IN (70936, 71941);
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
 (70936, 'spell_dreamwalker_summon_suppresser_effect'),
 (71941, 'spell_dreamwalker_twisted_nightmares');
-- -------------------------------------------------------------------------}
-- --------------- Myth Project "World" Database Cleaner -------------------
-- -------------------------------------------------------------------------{
 DROP TABLE IF EXISTS
 `db_version_ytdb`,         -- DEPRICATED TABLE
 `npc_gossip`,              -- DEPRICATED TABLE
 `game_event_npc_gossip`,   -- DEPRICATED TABLE
 `lfg_dungeon_encounters`,  -- DEPRICATED TABLE
 `gameobject_respawn`,      -- DEPRICATED TABLE
 `creature_respawn`,        -- DEPRICATED TABLE
 `reserved_name`,           -- DEPRICATED TABLE
 `custom_texts`,            -- DEPRICATED TABLE
 `id`,                      -- DEPRICATED TABLE
 `locales_item_set_name`,   -- DEPRICATED TABLE
 `calendar_invites`,        -- DEPRICATED TABLE
 `calendar_events`;         -- DEPRICATED TABLE
 DELETE FROM `db_script_string` WHERE entry = 2000001239;
/* -- -------------------------------------------------------------------------
SET @MINUS := xxx; SET @MAXG := xxx;
-- CREATURES
UPDATE `creature` SET `guid` = `guid` - @MINUS WHERE `guid` > @MAXG;
UPDATE `creature_addon` SET `guid` = `guid` - @MINUS WHERE `guid` > @MAXG;
UPDATE `creature_formations` SET `leaderGUID` = `leaderGUID` - @MINUS WHERE `leaderGUID` > @MAXG;
UPDATE `creature_formations` SET `memberGUID` = `memberGUID` - @MINUS WHERE `memberGUID` > @MAXG;
-- GAMEOBJECTS
UPDATE `gameobject` SET `guid` = `guid` - @MINUS WHERE `guid` > @MAXG;
UPDATE `pool_gameobject` SET `guid` = `guid` - @MINUS WHERE `guid` > @MAXG; */
-- -------------------------------------------------------------------------
-- CREATURES
 DELETE FROM `creature` WHERE `id` NOT IN (SELECT `entry` FROM `creature_template`);                            -- Dinamic // "creature" table
 DELETE FROM `creature_template_addon` WHERE `entry` NOT IN (SELECT `entry` FROM `creature_template`);          -- Dinamic // "creature_template_addon" table
 DELETE FROM `creature_formations` WHERE `leaderGUID` NOT IN (SELECT `guid` FROM `creature`)
OR `memberGUID` NOT IN (SELECT `guid` FROM `creature`);                                                         -- Dinamic // "creature_formations" table
 DELETE FROM `creature_onkill_reputation` WHERE `creature_id` NOT IN (SELECT `entry` FROM `creature_template`); -- Dinamic // "creature_onkill_reputation" table
 DELETE FROM `linked_respawn` WHERE `linkedguid` NOT IN (SELECT `guid` FROM `creature`);                        -- Dinamic // "linked_respawn" table
 DELETE FROM `creature_addon` WHERE `guid` NOT IN (SELECT `guid` FROM `creature`);                              -- Dinamic // "creature_addon" table
 DELETE FROM `battlemaster_entry` WHERE `entry` NOT IN (SELECT `entry` FROM `creature_template`);               -- Dinamic // "battlemaster_entry" table
-- -------------------------------------------------------------------------
-- GAMEOBJECTS
 DELETE FROM `gameobject` WHERE `id` NOT IN (SELECT `entry` FROM `gameobject_template`);                        -- Dinamic // "gameobject" table
 DELETE FROM `gameobject_scripts` WHERE `id` NOT IN (SELECT `guid` FROM `gameobject`);                          -- Dinamic // "gameobject_scripts" table
 DELETE FROM `gameobject_involvedrelation` WHERE `id` NOT IN (SELECT `entry` FROM `gameobject_template`);       -- Dinamic // "gameobject_involvedrelation" table
 DELETE FROM `gameobject_loot_template` WHERE `entry` NOT IN (SELECT `entry` FROM `gameobject_template`);       -- Dinamic // "gameobject_loot_template" table
 DELETE FROM `gameobject_questrelation` WHERE `id` NOT IN (SELECT `entry` FROM `gameobject_template`);          -- Dinamic // "gameobject_questrelation" table
-- -------------------------------------------------------------------------
-- EVENT AI
 DELETE FROM `creature_ai_scripts` WHERE `creature_id` IN (
SELECT `entry` FROM `creature_template` WHERE `AIName` NOT IN ("EventAI"));                                     -- Dinamic // "creature_ai_scripts" table
-- -------------------------------------------------------------------------
-- POOLS
 DELETE FROM `pool_creature` WHERE `guid` NOT IN (SELECT `guid` FROM `creature`);                               -- Dinamic // "creature_pool" table
 DELETE FROM `pool_gameobject` WHERE `guid` NOT IN (SELECT `guid` FROM `gameobject`);                           -- Dinamic // "gameobject_pool" table
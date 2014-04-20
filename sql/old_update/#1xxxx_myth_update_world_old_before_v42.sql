/* Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
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
 TRUNCATE `version`;
INSERT INTO `version` VALUES (NULL, NULL,'MythDB v12.42','MythAI v12.42', 604);
ALTER TABLE `conditions` CHANGE `SourceId` `SourceId` INT(10) DEFAULT 0;
-- -------------------------------------------------------------------------}
-- ------------------------------- Pit of Saron ----------------------------
-- -------------------------------------------------------------------------{
-- NPC: Ick, http://www.wowhead.com/npc=36476
UPDATE `creature_template` SET `RegenHealth` = 1 WHERE `entry` IN (36476, 37627);
-- -------------------------------------------------------------------------}
-- ----------------------------- Icecrown Citadel --------------------------
-- -------------------------------------------------------------------------{
-- NPC: Sister Svalna, http://www.wowhead.com/npc=37126
UPDATE `creature_template` SET `unit_flags` = 0 WHERE `entry` IN (37126, 38258, 3712602, 3712603);
-- NPC: Deathbringer Saurfang, http://www.wowhead.com/npc=37813
UPDATE `creature_template` SET `unit_flags` = 33588032,`type_flags` = `type_flags`|0x00000004 WHERE `entry` IN (37813, 38402, 38582, 38583);
-- NPC: Volatile Ooze, http://www.wowhead.com/npc=37697
UPDATE `creature_template` SET `mechanic_immune_mask` = `mechanic_immune_mask` &~ 1024 WHERE `entry` = 37697;
-- Game Mech.: Valithria Dreamwalker, event GameObjects 
UPDATE `gameobject_template` SET `flags` = 4 WHERE `entry` = 201919;
-- NPC: Val'kyr Shadowguard, http://old.wowhead.com/npc=36609
UPDATE `creature_template` SET `InhabitType`=`InhabitType`|4 WHERE `entry` IN (36609, 39122, 39121, 39120);
-- NPC: World Trigger (at the LK platform)
DELETE FROM `creature` WHERE `id` = 22515 AND `map` = 631 AND `guid` BETWEEN 86814 AND 86864;
INSERT INTO `creature` VALUES
 (86814, 22515, 631, 15, 1, 16925, 0, 494.024, -2172.17, 1040.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86815, 22515, 631, 15, 1, 16925, 0, 500.323, -2188.93, 1045.88, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86816, 22515, 631, 15, 1, 16925, 0, 477.042, -2164.2, 1040.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86817, 22515, 631, 15, 1, 16925, 0, 530.625, -2166.15, 1040.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86818, 22515, 631, 15, 1, 16925, 0, 470.72, -2183.84, 1045.88, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86819, 22515, 631, 15, 1, 16925, 0, 512.805, -2172.62, 1040.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86820, 22515, 631, 15, 1, 16925, 0, 530.698, -2182.86, 1045.88, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86821, 22515, 631, 15, 1, 16925, 0, 542.559, -2151.41, 1040.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86822, 22515, 631, 15, 1, 16925, 0, 567.837, -2144.41, 1045.88, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86837, 22515, 631, 15, 1, 16925, 0, 567.837, -2144.41, 1045.88, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86838, 22515, 631, 15, 1, 16925, 0, 554.196, -2166.77, 1045.88, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86839, 22515, 631, 15, 1, 16925, 0, 503.969, -2075.85, 1040.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86855, 22515, 631, 15, 1, 16925, 0, 466.25, -2071.3, 1045.88, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86856, 22515, 631, 15, 1, 16925, 0, 486.092, -2061.76, 1045.88, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86857, 22515, 631, 15, 1, 16925, 0, 482.924, -2078.25, 1040.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86858, 22515, 631, 15, 1, 16925, 0, 521.602, -2079.36, 1040.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86859, 22515, 631, 15, 1, 16925, 0, 515.104, -2058.46, 1045.88, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86860, 22515, 631, 15, 1, 16925, 0, 548.729, -2110.41, 1040.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86861, 22515, 631, 15, 1, 16925, 0, 568.533, -2106.96, 1045.88, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86862, 22515, 631, 15, 1, 16925, 0, 542.776, -2071.84, 1045.88, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86863, 22515, 631, 15, 1, 16925, 0, 539.628, -2089.51, 1040.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
 (86864, 22515, 631, 15, 1, 16925, 0, 550.62, -2132.89, 1040.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0);
-- -------------------------------------------------------------------------}
-- --------------------------------- Ulduar --------------------------------
-- -------------------------------------------------------------------------{
DELETE FROM `spell_script_names` WHERE `spell_id`=64414;
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
(64414,'spell_load_into_catapult');
DELETE FROM `spell_script_names` WHERE `spell_id`=62705;
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
(62705,'spell_auto_repair');
DELETE FROM `spell_script_names` WHERE `spell_id` IN (62374,62475);
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES 
(62374,'spell_pursue'),
(62475,'spell_systems_shutdown');

DELETE FROM spell_script_names WHERE spell_id = 62692;
INSERT INTO spell_script_names VALUE (62692,'spell_general_vezax_aura_of_despair_aura');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` BETWEEN 10082 AND 10087 AND `type` IN (11,12);
INSERT INTO `achievement_criteria_data`(`criteria_id`,`type`,`value1`,`value2`,`ScriptName`)
VALUES
(10082, 11, 0, 0,'achievement_i_choose_you'),
(10083, 11, 0, 0,'achievement_i_choose_you'),
(10084, 11, 0, 0,'achievement_i_choose_you'),
(10085, 11, 0, 0,'achievement_i_choose_you'),
(10086, 11, 0, 0,'achievement_i_choose_you'),
(10087, 11, 0, 0,'achievement_i_choose_you'),
(10082,12,0,0,''),
(10083,12,0,0,''),
(10084,12,0,0,''),
(10085,12,1,0,''),
(10086,12,1,0,''),
(10087,12,1,0,'');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10088,10418,10419,10089,10420,10421) AND `type` IN (11,12);
INSERT INTO `achievement_criteria_data`(`criteria_id`,`type`,`value1`,`value2`,`ScriptName`)
VALUES
(10088, 11, 0, 0,'achievement_but_i_am_on_your_side'),
(10418, 11, 0, 0,'achievement_but_i_am_on_your_side'),
(10419, 11, 0, 0,'achievement_but_i_am_on_your_side'),
(10088,12,0,0,''),
(10418,12,0,0,''),
(10419,12,0,0,''),
(10089, 11, 0, 0,'achievement_but_i_am_on_your_side'),
(10420, 11, 0, 0,'achievement_but_i_am_on_your_side'),
(10421, 11, 0, 0,'achievement_but_i_am_on_your_side'),
(10089,12,1,0,''),
(10420,12,1,0,''),
(10421,12,1,0,'');



UPDATE creature_template SET ScriptName="mob_living_constellation" WHERE entry=33052;
UPDATE creature_template SET ScriptName="mob_cosmic_smash_target" WHERE entry=33104;
UPDATE creature_template SET ScriptName="mob_black_hole" WHERE entry=32953;
UPDATE `creature_template` SET `ScriptName`='npc_vereth_the_cunning' WHERE `entry`=30944;
UPDATE `gameobject_template` SET `flags` = 6553632, ScriptName= 'go_celestial_acces' WHERE entry IN (194628, 194752);
UPDATE `creature_template` SET `faction_a`=14,`faction_h`=14,`movementId`=48,`ScriptName`='npc_argent_champion' WHERE `entry` = 33707;
UPDATE `creature_template` SET `ScriptName`='npc_vendor_tournament_fraction_champion' WHERE `entry` IN (33553, 33554, 33556, 33555, 33557, 33307, 33310, 33653, 33650, 33657);

DELETE FROM `spell_script_names` WHERE `spell_id` IN(64412,62293,62295,62311,64596,64443,64584,62168,62301,64598,64487,65312);
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
( '64412','spell_algalon_phase_punch'),
( '62295','spell_algalon_summon_asteroid_stalkers'),
( '62293','spell_algalon_summon_asteroid_stalkers'),
( '62311','spell_algalon_cosmic_smash_damage'),
( '64596','spell_algalon_cosmic_smash_damage'),
( '64443','spell_algalon_big_bang'),
( '64584','spell_algalon_big_bang'),
( '62168','spell_algalon_black hole'),
( '62301','spell_algalon_cosmic_smash_initial'),
( '64598','spell_algalon_cosmic_smash_initial'),
( '64487','spell_algalon_ascend_to_the_heavens');

UPDATE `gameobject_template` SET `ScriptName` = 'go_celestial_console' WHERE `entry` = 194628;
UPDATE `gameobject_template` SET `flags` = 34 WHERE `entry` = 194628;
DELETE FROM `spell_script_names` WHERE `spell_id` IN (61911,63495);
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
(61911, 'spell_steelbreaker_static_disruption'),
(63495, 'spell_steelbreaker_static_disruption');

DELETE FROM `achievement_criteria_data` WHERE `ScriptName`='achievement_cant_do_that_while_stunned';
INSERT INTO `achievement_criteria_data`(`criteria_id`,`type`,`value1`,`value2`,`ScriptName`) VALUES
(10090, 11, 0, 0,'achievement_cant_do_that_while_stunned'),
(10422, 11, 0, 0,'achievement_cant_do_that_while_stunned'),
(10423, 11, 0, 0,'achievement_cant_do_that_while_stunned'),
(10091, 11, 0, 0,'achievement_cant_do_that_while_stunned'),
(10424, 11, 0, 0,'achievement_cant_do_that_while_stunned'),
(10425, 11, 0, 0,'achievement_cant_do_that_while_stunned');
DELETE FROM `achievement_criteria_data` WHERE `criteria_id` = 10678;
INSERT INTO `achievement_criteria_data`(`criteria_id`,`type`,`value1`,`value2`,`ScriptName`) VALUES
 (10678, 11, 0, 0,'achievement_herald_of_the_titans');


-- Argent Tournament Trainers part
SET @SPELL_ON_ARGENT_MOUNT         := 63034;

SET @NPC_JAERAN_LOCKWOOD           := 33973;
SET @QUEST_MASTERY_OF_MELEE_A      := 13828;
SET @QUEST_MASTERY_OF_MELEE_H      := 13829;

SET @GOSSIP_MENU_JERAN_MOUNTED     := 10398;
SET @GOSSIP_MENU_JERAN_EXPLANATION := 10397; -- From Aokromes Sniffs

SET @GOSSIP_TEXT_JERAN_MOUNTED     := 14431;
SET @GOSSIP_TEXT_JERAN_EXPLANATION := 14434;
SET @SPELL_CREDIT_JERAN            := 64113;

SET @NPC_RUGAN_STEELBELLY          := 33972;
SET @QUEST_MASTERY_OF_CHARGE_A     := 13837;
SET @QUEST_MASTERY_OF_CHARGE_H     := 13839;

SET @GOSSIP_MENU_RUGAN_MOUNTED     := 10400;
SET @GOSSIP_MENU_RUGAN_EXPLANATION := 10399; -- From Aokromes Sniffs

SET @GOSSIP_TEXT_RUGAN_MOUNTED     := 14436;
SET @GOSSIP_TEXT_RUGAN_EXPLANATION := 14437;
SET @SPELL_CREDIT_RUGAN            := 64114;

SET @NPC_VALIS_WINDCHASER          := 33974;
SET @QUEST_MASTERY_OF_SH_BREAKER_A := 13835;
SET @QUEST_MASTERY_OF_SH_BREAKER_H := 13838;

SET @GOSSIP_MENU_VALIS_MOUNTED     := 10402;
SET @GOSSIP_MENU_VALIS_EXPLANATION := 10401; -- From Aokromes Sniffs

SET @GOSSIP_TEXT_VALIS_MOUNTED     := 14438;
SET @GOSSIP_TEXT_VALIS_EXPLANATION := 14439;
SET @SPELL_CREDIT_VALIS            := 64115;

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (@NPC_JAERAN_LOCKWOOD,@NPC_RUGAN_STEELBELLY,@NPC_VALIS_WINDCHASER);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_JAERAN_LOCKWOOD,@NPC_RUGAN_STEELBELLY,@NPC_VALIS_WINDCHASER) AND `source_type`=0;
INSERT INTO `smart_scripts`(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES 
(@NPC_JAERAN_LOCKWOOD,0,0,0,64,0,100,0,0,0,0,0,98,@GOSSIP_MENU_JERAN_MOUNTED,@GOSSIP_TEXT_JERAN_MOUNTED,0,0,0,0,7,0,0,0,0,0,0,0,'Jeran Lockwood - Send different gossip when mounted (Requires conditions)'),
(@NPC_JAERAN_LOCKWOOD,0,1,3,62,0,100,0,@GOSSIP_MENU_JERAN_MOUNTED,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Jeran Lockwood - Send text when option clicked'),
(@NPC_JAERAN_LOCKWOOD,0,2,3,62,0,100,0,@GOSSIP_MENU_JERAN_EXPLANATION,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Jeran Lockwood - Send text when option clicked'),
(@NPC_JAERAN_LOCKWOOD,0,3,4,61,0,100,0,0,0,0,0,11,@SPELL_CREDIT_JERAN,0,0,0,0,0,7,0,0,0,0,0,0,0,'Jeran Lockwood - Give Credit'),
(@NPC_JAERAN_LOCKWOOD,0,4,0,61,0,100,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Jeran Lockwood - Close Gossip'),

(@NPC_RUGAN_STEELBELLY,0,0,0,64,0,100,0,0,0,0,0,98,@GOSSIP_MENU_RUGAN_MOUNTED,@GOSSIP_TEXT_RUGAN_MOUNTED,0,0,0,0,7,0,0,0,0,0,0,0,'Rugan Steelbelly - Send different gossip when mounted (Requires conditions)'),
(@NPC_RUGAN_STEELBELLY,0,1,3,62,0,100,0,@GOSSIP_MENU_RUGAN_MOUNTED,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Rugan Steelbelly - Send text when option clicked'),
(@NPC_RUGAN_STEELBELLY,0,2,3,62,0,100,0,@GOSSIP_MENU_RUGAN_EXPLANATION,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Rugan Steelbelly - Send text when option clicked'),
(@NPC_RUGAN_STEELBELLY,0,3,4,61,0,100,0,0,0,0,0,11,@SPELL_CREDIT_RUGAN,0,0,0,0,0,7,0,0,0,0,0,0,0,'Rugan Steelbelly - Give Credit'),
(@NPC_RUGAN_STEELBELLY,0,4,0,61,0,100,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Rugan Steelbelly - Close Gossip'),

(@NPC_VALIS_WINDCHASER,0,0,0,64,0,100,0,0,0,0,0,98,@GOSSIP_MENU_VALIS_MOUNTED,@GOSSIP_TEXT_VALIS_MOUNTED,0,0,0,0,7,0,0,0,0,0,0,0,'Valis Windchaser - Send different gossip when mounted (Requires conditions)'),
(@NPC_VALIS_WINDCHASER,0,1,3,62,0,100,0,@GOSSIP_MENU_VALIS_MOUNTED,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Valis Windchaser - Send text when option clicked'),
(@NPC_VALIS_WINDCHASER,0,2,3,62,0,100,0,@GOSSIP_TEXT_VALIS_EXPLANATION,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Valis Windchaser - Send text when option clicked'),
(@NPC_VALIS_WINDCHASER,0,3,4,61,0,100,0,0,0,0,0,11,@SPELL_CREDIT_VALIS,0,0,0,0,0,7,0,0,0,0,0,0,0,'Valis Windchaser - Give Credit'),
(@NPC_VALIS_WINDCHASER,0,4,0,61,0,100,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Valis Windchaser - Close Gossip');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceEntry` IN (@NPC_JAERAN_LOCKWOOD,@NPC_RUGAN_STEELBELLY,@NPC_VALIS_WINDCHASER);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup` IN (@GOSSIP_MENU_JERAN_MOUNTED,@GOSSIP_MENU_RUGAN_MOUNTED,@GOSSIP_MENU_VALIS_MOUNTED);
INSERT INTO `conditions`(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,1,@NPC_JAERAN_LOCKWOOD,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'','SAI - Jeran Lockwood show different menu if player mounted'),
(15,@GOSSIP_MENU_JERAN_MOUNTED,0,0,0,9,@QUEST_MASTERY_OF_MELEE_A,0,0,0,'',"Jeran Lockwood - Show gossip if player has quest"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,0,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Jeran Lockwood - Show gossip if player has aura"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,0,0,1,9,@QUEST_MASTERY_OF_MELEE_H,0,0,0,'',"Jeran Lockwood - Show gossip if player has quest"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,0,0,1,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Jeran Lockwood - Show gossip if player has aura"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,1,0,0,9,@QUEST_MASTERY_OF_MELEE_A,0,0,0,'',"Jeran Lockwood - Show gossip if player has quest"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,1,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Jeran Lockwood - Show gossip if player has aura"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,1,0,1,9,@QUEST_MASTERY_OF_MELEE_H,0,0,0,'',"Jeran Lockwood - Show gossip if player has quest"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,1,0,1,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Jeran Lockwood - Show gossip if player has aura"),

(22,1,@NPC_RUGAN_STEELBELLY,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'','SAI - Rugan Steelbelly show different menu if player mounted'),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,0,0,0,9,@QUEST_MASTERY_OF_CHARGE_A,0,0,0,'',"Rugan Steelbelly - Show gossip if player has quest"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,0,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Rugan Steelbelly - Show gossip if player has aura"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,0,0,1,9,@QUEST_MASTERY_OF_CHARGE_H,0,0,0,'',"Rugan Steelbelly - Show gossip if player has quest"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,0,0,1,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Rugan Steelbelly - Show gossip if player has aura"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,1,0,0,9,@QUEST_MASTERY_OF_CHARGE_A,0,0,0,'',"Rugan Steelbelly - Show gossip if player has quest"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,1,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Rugan Steelbelly - Show gossip if player has aura"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,1,0,1,9,@QUEST_MASTERY_OF_CHARGE_H,0,0,0,'',"Rugan Steelbelly - Show gossip if player has quest"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,1,0,1,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Rugan Steelbelly - Show gossip if player has aura"),

(22,1,@NPC_VALIS_WINDCHASER,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'','SAI - Valis Windchaser show different menu if player mounted'),
(15,@GOSSIP_MENU_VALIS_MOUNTED,0,0,0,9,@QUEST_MASTERY_OF_SH_BREAKER_A,0,0,0,'',"Valis Windchaser - Show gossip if player has quest"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,0,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Valis Windchaser - Show gossip if player has aura"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,0,0,1,9,@QUEST_MASTERY_OF_SH_BREAKER_H,0,0,0,'',"Valis Windchaser - Show gossip if player has quest"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,0,0,1,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Valis Windchaser - Show gossip if player has aura"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,1,0,0,9,@QUEST_MASTERY_OF_SH_BREAKER_A,0,0,0,'',"Valis Windchaser - Show gossip if player has quest"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,1,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Valis Windchaser - Show gossip if player has aura"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,1,0,1,9,@QUEST_MASTERY_OF_SH_BREAKER_H,0,0,0,'',"Valis Windchaser - Show gossip if player has quest"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,1,0,1,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Valis Windchaser - Show gossip if player has aura");

DELETE FROM `gossip_menu` WHERE `entry` IN (@GOSSIP_MENU_JERAN_EXPLANATION,@GOSSIP_MENU_RUGAN_EXPLANATION,@GOSSIP_MENU_VALIS_EXPLANATION);
INSERT INTO `gossip_menu`(`entry`,`text_id`) VALUES
(@GOSSIP_MENU_JERAN_EXPLANATION,@GOSSIP_TEXT_JERAN_EXPLANATION),
(@GOSSIP_MENU_RUGAN_EXPLANATION,@GOSSIP_TEXT_RUGAN_EXPLANATION),
(@GOSSIP_MENU_VALIS_EXPLANATION,@GOSSIP_TEXT_VALIS_EXPLANATION);

DELETE FROM `gossip_menu_option` WHERE `menu_id` IN (@GOSSIP_MENU_JERAN_MOUNTED,@GOSSIP_MENU_JERAN_EXPLANATION,@GOSSIP_MENU_RUGAN_MOUNTED,@GOSSIP_MENU_RUGAN_EXPLANATION,@GOSSIP_MENU_VALIS_MOUNTED,@GOSSIP_MENU_VALIS_EXPLANATION);
INSERT INTO `gossip_menu_option`(`menu_id`,`id`,`option_icon`,`option_text`,`option_id`,`npc_option_npcflag`,`action_menu_id`,`action_poi_id`,`box_coded`,`box_money`,`box_text`) VALUES
(@GOSSIP_MENU_JERAN_MOUNTED,0,0,'Show me how to train with a Melee Target.',1,1,0,0,0,0,''),
(@GOSSIP_MENU_JERAN_MOUNTED,1,0,'Tell me more about Defend and Thrust!',1,1,@GOSSIP_MENU_JERAN_EXPLANATION,0,0,0,''),
(@GOSSIP_MENU_JERAN_EXPLANATION,0,0,'Show me how to train with a Melee Target.',1,1,0,0,0,0,''),

(@GOSSIP_MENU_RUGAN_MOUNTED,0,0,'Show me how to train with a Charge Target.',1,1,0,0,0,0,''),
(@GOSSIP_MENU_RUGAN_MOUNTED,1,0,'Tell me more about the Charge!',1,1,@GOSSIP_MENU_RUGAN_EXPLANATION,0,0,0,''),
(@GOSSIP_MENU_RUGAN_EXPLANATION,0,0,'Show me how to train with a Charge Target.',1,1,0,0,0,0,''),

(@GOSSIP_MENU_VALIS_MOUNTED,0,0,'Show me how to train with a Ranged Target.',1,1,0,0,0,0,''),
(@GOSSIP_MENU_VALIS_MOUNTED,1,0,'Tell me more about the Shield-Breaker!',1,1,@GOSSIP_MENU_VALIS_EXPLANATION,0,0,0,''),
(@GOSSIP_MENU_VALIS_EXPLANATION,0,0,'Show me how to train with a Ranged Target.',1,1,0,0,0,0,'');

DELETE FROM `creature_text` WHERE `entry` IN (@NPC_JAERAN_LOCKWOOD,@NPC_RUGAN_STEELBELLY,@NPC_VALIS_WINDCHASER);
INSERT INTO `creature_text`(`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@NPC_JAERAN_LOCKWOOD,0,0,'Put up defend$B|TInterface\\Icons\\ability_warrior_shieldmastery.blp:32|t$BThen use Thrust on a Melee Target$B|TInterface\\Icons\\inv_sword_65.blp:32|t',42,0,0,0,0,0,'Argent Tournament - Melee Tutorial'),
(@NPC_RUGAN_STEELBELLY,0,0,'Use Shield-Breaker on a Charge Target$B|TInterface\\Icons\\ability_warrior_shieldbreak.blp:32|t$BFollow up with Charge while the target is vulnerable$B|TInterface\\Icons\\ability_mount_charger.blp:32|t',42,0,0,0,0,0,'Argent Tournament - Charge Tutorial'),
(@NPC_VALIS_WINDCHASER,0,0,'Use Shield-Breaker on a Ranged Target$B|TInterface\\Icons\\ability_warrior_shieldbreak.blp:32|t$BThen use Shield-Breaker while the target is defenseless$B|TInterface\\Icons\\ability_warrior_shieldbreak.blp:32|t',42,0,0,0,0,0,'Argent Tournament - Ranged Tutorial');

-- Training Dummies Part
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=62709;
INSERT INTO `conditions`(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,0,62709,0,0,18,1,33845,0,0, '','Counterattack! (Argent Tournament) - Target near aspirant mounts'),
(13,0,62709,0,0,18,1,33323,0,0, '','Counterattack! (Argent Tournament) - Target near aspirant mounts');

DELETE FROM `spell_script_names` WHERE `spell_id`=62709;
INSERT INTO `spell_script_names` VALUES (62709, 'spell_gen_tournament_counterattack');
-- Delorah texts
DELETE FROM `creature_text` WHERE `entry` = 33701;
INSERT INTO `creature_text`(`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(33701, 0, 0, 'I heard a story or two of a Lore Keeper in Uldaman that fit your description. Do you serve a similar purpose?', 12, 0, 0, 0, 0, 0, 'Dellorah SAY_EVENT_1'),
(33701, 1, 0, 'Frontal defense systems? Is there something I should let Brann know before he has anyone attempt to enter the complex?', 12, 0, 0, 0, 0, 0, 'Dellorah SAY_EVENT_2'),
(33701, 2, 0, 'Can you detail the nature of these defense systems?', 12, 0, 0, 0, 0, 0, 'Dellorah SAY_EVENT_3'),
(33701, 3, 0, 'Got it. At least we don\'t have to deal with those orbital emplacements.', 12, 0, 0, 0, 0, 0, 'Dellorah SAY_EVENT_4'),
(33701, 4, 0, 'Rhydian, make sure you let Brann and Archmage Pentarus know about those defenses immediately.', 12, 0, 0, 0, 0, 0, 'Dellorah SAY_EVENT_5'),
(33701, 5, 0, 'And you mentioned an imprisoned entity? What is the nature of this entity and what is its status?', 12, 0, 0, 0, 0, 0, 'Dellorah SAY_EVENT_7'),
(33701, 7, 0, 'Yogg-Saron is here? It sounds like we really will have our hands full then.', 12, 0, 0, 0, 0, 0, 'Dellorah SAY_EVENT_8'),
(33701, 8, 0, 'What... What did you just do, $n?! Brann! Braaaaannn! ', 14, 0, 0, 0, 0, 0, 'Dellorah SAY_BRANN'),
(33701, 9, 0, 'Brann! $n just activated the orbital defense system! If we dont get out here soon, we re going to be toast!', 14, 0, 0, 0, 0, 0, 'Dellorah SAY_BRANN2');

-- Norgannon texts
DELETE FROM `creature_text` WHERE `entry` = 33686;
INSERT INTO `creature_text`(`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(33686, 0, 0, ' I was constructed to serve as a repository for essential information regarding this complex. My primary functions include communicating the status of the frontal defense systems and assessing the status of the entity that this complex was built to imprison.', 12, 0, 0, 0, 0, 0, 'Norgannon SAY_EVENT_1'),
(33686, 1, 0, 'Access to the interior of the complex is currently restricted. Primary defensive emplacements are active. Secondary systems are currently non-active.', 12, 0, 0, 0, 0, 0, 'Norgannon SAY_EVENT_2'),
(33686, 2, 0, 'Compromise of complex detected, security override enabled - query permitted.', 12, 0, 0, 0, 0, 0, 'Norgannon SAY_EVENT_3'),
(33686, 3, 0, 'Primary defensive emplacements consist of iron constructs and Storm Beacons, which will generate additional constructs as necessary. Secondary systems consist of orbital defense emplacements.', 12, 0, 0, 0, 0, 0, 'Norgannon SAY_EVENT_4'),
(33686, 7, 0, 'Entity designate: Yogg-Saron. Security has been compromised. Prison operational status unknown. Unable to contact Watchers for notification purposes.', 12, 0, 0, 0, 0, 0, 'Norgannon SAY_EVENT_8'),
(33686, 10, 0, 'Deactivate', 12, 0, 0, 0, 0, 0, 'Norgannon SAY_DEACTIVATE');

-- Rhydian emotes
DELETE FROM `creature_text` WHERE `entry` = 33696;
INSERT INTO `creature_text`(`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(33696, 6, 0, '%s nods.', 16, 0, 0, 0, 0, 0, 'Rhydian SAY_EVENT_6'),
(33696, 11, 0, '%s whispers something to Brann', 16, 0, 0, 0, 0, 0, 'Rhydian SAY_WHISPER');

-- Gountlet
UPDATE `creature_template` SET `ScriptName` = 'npc_gauntlet_generator' WHERE `entry` = 34159;
UPDATE `creature_template` SET `ScriptName`= 'npc_ulduar_lorekeeper' WHERE `entry`= 33686;
UPDATE `creature_template` SET `ScriptName` = 'npc_ulduar_warmage' WHERE entry = 33662;
UPDATE `creature_template` SET `ScriptName` = 'npc_bronzebeard_radio' WHERE entry = 34054;
UPDATE `creature_template` SET `ScriptName` = 'npc_ulduar_engineer' WHERE entry = 33626;

-- Spawns 10/25 NORMAL
DELETE FROM `creature` WHERE `id` IN (27306, 34054) AND `map` = 603;
INSERT INTO `creature` VALUES 
(190010, '27306', '603', '3', '1', '0', '0', '-665.945', '-5.68514', '518.75', '0.538015', '300', '0', '0', '42', '0', '0', '0', '0', '0'),
(190003, '27306', '603', '3', '1', '0', '0', '-642', '-101.168', '518.75', '6.16147', '300', '0', '0', '42', '0', '0', '0', '0', '0'),
(190004, '34054', '603', '3', '1', '0', '0', '-79.2613', '112.818', '433.792', '1.6148', '300', '0', '0', '75600', '0', '0', '0', '0', '0'),
(190005, '34054', '603', '3', '1', '0', '0', '-225.065', '-274.866', '369.466', '1.59124', '300', '0', '0', '75600', '0', '0', '0', '0', '0'),
(190006, '34054', '603', '3', '1', '0', '0', '83.5056', '-371', '410.819', '1.6423', '300', '0', '0', '75600', '0', '0', '0', '0', '0'),
(190007, '34054', '603', '3', '1', '0', '0', '317.382', '318.532', '409.802', '3.01675', '300', '0', '0', '75600', '0', '0', '0', '0', '0'),
(190008, '34054', '603', '3', '1', '0', '0', '138.118', '56.1308', '410.969', '5.81588', '300', '0', '0', '75600', '0', '0', '0', '0', '0'),
(190009, '34054', '603', '3', '1', '0', '0', '130.321', '-127.606', '410.968', '5.25824', '300', '0', '0', '75600', '0', '0', '0', '0', '0');

UPDATE creature_template SET scriptname = 'boss_sara' WHERE entry = 33134;
UPDATE script_texts SET npc_entry = 33134 WHERE npc_entry = 33288 AND entry IN (-1603330,-1603331,-1603332,-1603333);
UPDATE script_texts SET content_default = "Help me! Please get them off me!" WHERE npc_entry = 33134 AND entry = -1603310;
UPDATE script_texts SET content_default = "What do you want from me? Leave me alone!" WHERE npc_entry = 33134 AND entry = -1603311;
UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry` = 33134;
UPDATE creature_template SET scriptname = 'npc_ominous_cloud' WHERE entry = 33292;
UPDATE creature_template SET scriptname = 'npc_guardian_of_yogg_saron' WHERE entry = 33136;
UPDATE creature_template SET scriptname = 'npc_yogg_saron_tentacle' WHERE entry IN (33966,33985,33983);
UPDATE creature_template SET scriptname = 'npc_descend_into_madness' WHERE entry = 34072;
UPDATE creature_template SET scriptname = 'npc_brain_of_yogg_saron' WHERE entry = 33890;
UPDATE creature_template SET scriptname = 'boss_yogg_saron' WHERE entry = 33288;
UPDATE creature_template SET scriptname = 'npc_influence_tentacle' WHERE entry IN (33716,33720,33719,33717,33433,33567);
UPDATE creature_template SET scriptname = 'npc_immortal_guardian' WHERE entry = 33988;
UPDATE creature_template SET scriptname = 'npc_support_keeper' WHERE entry IN (33410,33411,33412,33413);
UPDATE creature_template SET scriptname = 'npc_sanity_well' WHERE entry = 33991;
UPDATE creature_template SET modelid1 = 11686, modelid2 = 11686 WHERE entry = 33991;
UPDATE creature_template SET scriptname = 'npc_death_orb' WHERE entry = 33882;
UPDATE creature_template SET modelid1 = 16946, modelid2 = 16946 WHERE entry = 33882;
UPDATE creature_template SET scriptname = 'npc_death_ray' WHERE entry = 33881;
UPDATE creature_template SET modelid1 = 17612, modelid2 = 17612 WHERE entry = 33881;
UPDATE creature_template SET minlevel = 80, maxlevel = 80, scriptname = 'npc_laughting_skull' WHERE entry = 33990;
UPDATE creature_template SET modelid1 = 15880, modelid2 = 15880 WHERE entry = 33990;
UPDATE creature_template SET scriptname = 'npc_keeper_help' WHERE entry IN(33241,33244,33242,33213);
UPDATE `creature_template` SET `minlevel`=80, `maxlevel`=80 WHERE `entry` = 33943;

UPDATE gameobject_template SET scriptname = 'go_flee_to_surface' WHERE entry = 194625;
UPDATE item_template SET scriptname = 'item_unbound_fragments_of_valanyr' WHERE entry = 45896;

UPDATE creature_template SET RegenHealth = 0 WHERE entry IN (33134,34332,33890,33954);

-- Secound Damage Effekt of ShadowNova only on other Guardians or Sara
DELETE FROM conditions WHERE SourceEntry = 65209;
INSERT INTO conditions
(SourceTypeOrReferenceId,SourceGroup,SourceEntry,ElseGroup,
 ConditionTypeOrReference,ConditionValue1,ConditionValue2,ConditionValue3,
 ErrorTextId,ScriptName,COMMENT)
VALUES
(13,0,65209,0,18,1,33136,0,0,'','Effekt only for Guardian of YoggSaron'),
(13,0,65209,0,18,1,33134,0,0,'','Effekt only for Sara');

DELETE FROM gameobject WHERE id = 194625;
INSERT INTO gameobject
(guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state)
VALUES
(603001, 194625, 603, 3, 1, 2001.40, -59.66, 245.07, 0, 0, 0, 0, 0, 60, 100, 1),
(603002, 194625, 603, 3, 1, 1941.61, -25.88, 244.98, 0, 0, 0, 0, 0, 60, 100, 1),
(603003, 194625, 603, 3, 1, 2001.18,  9.409, 245.24, 0, 0, 0, 0, 0, 60, 100, 1);

-- Auren shouldnt hit other friendly NPCs
DELETE FROM spell_script_names WHERE spell_id IN (62670,62671,62702,62650);
INSERT INTO spell_script_names (spell_id,Scriptname)
VALUES
(62670,'spell_keeper_support_aura_targeting'),
(62671,'spell_keeper_support_aura_targeting'),
(62702,'spell_keeper_support_aura_targeting'),
(62650,'spell_keeper_support_aura_targeting');

-- Script for Target Faces Caster
DELETE FROM spell_script_names WHERE spell_id IN (64164,64168);
INSERT INTO spell_script_names (spell_id,Scriptname)
VALUES
(64164,'spell_lunatic_gaze_targeting'),
(64168,'spell_lunatic_gaze_targeting');

-- Trigger Effekt on Near Player with Brain Link
DELETE FROM spell_script_names WHERE spell_id IN (63802);
INSERT INTO spell_script_names (spell_id,Scriptname)
VALUES
(63802,'spell_brain_link_periodic_dummy');

-- Needed for Titanic Storm
-- Script for Target have Weakened Aura
DELETE FROM spell_script_names WHERE spell_id IN (64172);
INSERT INTO spell_script_names (spell_id,Scriptname)
VALUES
(64172,'spell_titanic_storm_targeting');

-- Condition because NPCs need this else no hit
DELETE FROM conditions WHERE SourceEntry IN (64172,64465);
INSERT INTO conditions
(SourceTypeOrReferenceId,SourceGroup,SourceEntry,ElseGroup,
 ConditionTypeOrReference,ConditionValue1,ConditionValue2,ConditionValue3,
 ErrorTextId,ScriptName,COMMENT)
VALUES
(13,0,64172,0,18,1,33988,0,0,'','Effekt only for Immortal Guardians'),
(13,0,64465,0,18,1,33988,0,0,'','Effekt only for Immortal Guardians');

-- Hodir Secound Aura Script
DELETE FROM spell_script_names WHERE spell_id IN (64174);
INSERT INTO spell_script_names (spell_id,Scriptname)
VALUES
(64174,'spell_hodir_protective_gaze');

-- Insane Death trigger on Remove
DELETE FROM spell_script_names WHERE spell_id IN (63120);
INSERT INTO spell_script_names (spell_id,Scriptname)
VALUES
(63120,'spell_insane_death_effekt');

-- Deathray Effekt on Death Orb
DELETE FROM conditions WHERE SourceEntry IN (63882,63886);
INSERT INTO conditions
(SourceTypeOrReferenceId,SourceGroup,SourceEntry,ElseGroup,
 ConditionTypeOrReference,ConditionValue1,ConditionValue2,ConditionValue3,
 ErrorTextId,ScriptName,COMMENT)
VALUES
(13,0,63882,0,18,1,33882,0,0,'','Effekt on Death Orb'),
(13,0,63886,0,18,1,33882,0,0,'','Effekt on Death Orb');

-- Correct Summon Position of Tentacle
DELETE FROM spell_script_names WHERE spell_id IN (64139,64143,64133);
INSERT INTO spell_script_names (spell_id,Scriptname)
VALUES
(64139,'spell_summon_tentacle_position'),
(64143,'spell_summon_tentacle_position'),
(64133,'spell_summon_tentacle_position');

-- Heal Trigger for Empowering Shadows
DELETE FROM spell_script_names WHERE spell_id IN (64466);
INSERT INTO spell_script_names (spell_id,Scriptname)
VALUES
(64466,'spell_empowering_shadows');

-- Create Val'anyr on Yogg-Saron
DELETE FROM conditions WHERE SourceEntry IN (64184);
INSERT INTO conditions
(SourceTypeOrReferenceId,SourceGroup,SourceEntry,ElseGroup,
 ConditionTypeOrReference,ConditionValue1,ConditionValue2,ConditionValue3,
 ErrorTextId,ScriptName,COMMENT)
VALUES
(13,0,64184,0,18,1,33288,0,0,'','Effekt on YoggSaron');

-- Missing Says Vision
DELETE FROM script_texts WHERE entry BETWEEN -1603360 AND -1603342;
INSERT INTO script_texts (`npc_entry`,`entry`,`content_default`,`sound`,`type`,`language`,`emote`,`comment`)
VALUES
(33436,-1603342,'Bad news sire.',15538,0,0,0,'Garona KingLlaneVision_Say1'),
(33436,-1603343,'The clans are united under Blackhand in this assault. They will stand together until Stormwind has fallen.',15539,0,0,0,'Garona KingLlaneVision_Say2'),
(33436,-1603344,'Gul\'dan is bringing up his warlocks by nightfall. Until then, the Blackrock clan will be trying to take the Eastern Wall.',15540,0,0,0,'Garona KingLlaneVision_Say3'),
(33288,-1603345,'A thousand deaths... or one murder.',15762,0,0,0,'YoggSaron KingLlaneVision_Say1'),
(33288,-1603346,'',15763,0,0,0,'YoggSaron KingLlianeVision_Say2'),
(33437,-1603347,'We will hold until the reinforcements come. As long as men with stout hearts are manning the walls and throne Stormwind will hold.',15585,0,0,0,'KingLlane KingLlaneVision_Say'),
(33436,-1603348,'The orc leaders agree with your assessment.',15541,0,0,0,'Garona KingLlaneVision_Say4'),
(33288,-1603349,'Your petty quarrels only make me stronger!',15764,0,0,0,'YoggSaron KingLlaneVision_Say3'),

(33441,-1603350,'Your resilience is admirable.',15598,0,0,0,'TheLichKing LichKingVision_Say1'),
(33442,-1603351,'Arrrrrrgh!',15470,1,0,0,'ImmolatedChampion LichKingVision_Say1'),
(33442,-1603352,'I\'m not afraid of you!',15471,0,0,0,'ImmolatedChampion LichKingVision_Say2'),
(33441,-1603353,'I will break you as I broke him.',15599,0,0,0,'TheLichKing LichKingVision_Say2'),
(33288,-1603354,'Yrr n\'lyeth... shuul anagg!',15766,0,0,0,'YoggSaron LichKingVision_Say1'),
(33288,-1603355,'He will learn... no king rules forever, only death is eternal!',15767,0,0,0,'YoggSaron LichKingVision_Say2'),

(33523,-1603356,'It is done... All have been given that which must be given. I now seal the Dragon Soul forever...',15631,0,0,0,'Neltharion DragonSoulVision_Say1'),
(33495,-1603357,'That terrible glow... should that be?',15702,0,0,0,'Ysera DragonSoulVision_Say'),
(33523,-1603358,'For it to be as it must, yes.',15632,0,0,0,'Neltharion DragonSoulVision_Say2'),
(33535,-1603359,'It is a weapon like no other. It must be like no other.',15610,0,0,0,'Malygos DragonSoulVision_Say'),
(33288,-1603360,'His brood learned their lesson before too long, you shall soon learn yours!',15765,0,0,0,'YoggSaron DragonSoulVision_Say1');
-- -------------------------------------------------------------------------}
-- -------------------------- Upper Blackrock Spire ------------------------
-- -------------------------------------------------------------------------{
-- NPC: Pyroguard Emberseer, http://old.wowhead.com/npc=9816
UPDATE `gameobject_template` SET `data3` = 0 WHERE `entry` = 175706;
DELETE FROM `creature` WHERE `id` = 9816;
-- -------------------------------------------------------------------------}
-- ------------------------------ The Slave Pens ---------------------------
-- -------------------------------------------------------------------------{
UPDATE `instance_template` SET `script` = 'instance_slave_pens' WHERE `map` = 547;
UPDATE `creature_template` SET `unit_flags` = `unit_flags`|300,`ScriptName` = 'npc_ahune_ice_spear_bunny' WHERE `entry` = 25985;
DELETE FROM `spell_script_names` WHERE `spell_id` IN (46320, 46360);
INSERT INTO `spell_script_names` VALUES
 (46320,'spell_slippery_floor_periodic'),
 (46360,'spell_ice_spear_knockback');
DELETE FROM `creature_template_addon` WHERE `entry` = 25952;
INSERT INTO `creature_template_addon` VALUES
 (25952, 0, 0, 0, 0, 0, 46314);
UPDATE `gameobject_template` SET `type` = 10,`flags` = `flags`|1,`data5` = 1 WHERE `entry` = 188077;
UPDATE `gameobject_template` SET `faction` = 0,`data0` = 43 WHERE `entry` = 187892;
UPDATE `gameobject_template` SET `faction` = 0,`data0` = 43,`data1` = 188124,`data10` = 1,`data11` = 1,`data12` = 1,`data13` = 1,`data15` = 1 WHERE `entry` = 188124;
UPDATE `creature_template` SET `scale` = 0.7,`AIName` = '',`ScriptName` = 'boss_ahune' WHERE `entry` = 25740;
UPDATE `creature_template` SET `AIName` = '',`ScriptName` = 'npc_ahunite_hailstone' WHERE `entry` = 25755;
UPDATE `creature_template` SET `AIName` = '',`ScriptName` = 'npc_ahunite_frostwind' WHERE `entry` = 25757;
UPDATE `creature_template` SET `AIName` = '',`ScriptName` = 'npc_ahunite_coldwave' WHERE `entry` = 25756;
UPDATE `creature_template` SET `AIName` = '',`ScriptName` = 'boss_ahune_frostcore' WHERE `entry` = 25865;
DELETE FROM `quest_end_scripts` WHERE `id` = 11691 AND `delay` > 5;
UPDATE `quest_end_scripts` SET `x` = -95.810844,`y` = -229.153961,`z` = -1.26299,`o` = 1.526232,`datalong2` = 86400000 WHERE `id` = 11691;
DELETE FROM `creature` WHERE `guid` IN (71580, 71578);
INSERT INTO `creature` VALUES
 (71580, 25746, 547, 3, 1, 0, 0, -93.3087, -248.553, -1.15367, 4.91023, 300, 0, 0, 1, 0, 0, 0, 0, 0);
UPDATE `creature` SET `position_z` = -1.25893 WHERE `guid` = 59726;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` IN (46588, 46143, 46176, 46382, 46623, 46320);
INSERT INTO `conditions` SET `SourceTypeOrReferenceId` = 13,`SourceEntry` = 46588,`ConditionTypeOrReference` = 18,`ConditionValue1` = 1,`Comment` = 'IceSpear - target player';
INSERT INTO `conditions` SET `SourceTypeOrReferenceId` = 13,`SourceEntry` = 46320,`ConditionTypeOrReference` = 18,`ConditionValue1` = 1,`Comment` = 'SlipperyFloor - target player';
INSERT INTO `conditions` SET `SourceTypeOrReferenceId` = 13,`SourceEntry` = 46143,`ConditionTypeOrReference` = 18,`ConditionValue1` = 1,`ConditionValue2` = 25745,`Comment` = 'Ahune summon - missile target';
INSERT INTO `conditions` SET `SourceTypeOrReferenceId` = 13,`SourceEntry` = 46176,`ConditionTypeOrReference` = 18,`ConditionValue1` = 1,`ConditionValue2` = 25745,`Comment` = 'Ahune summon - missile target';
INSERT INTO `conditions` SET `SourceTypeOrReferenceId` = 13,`SourceEntry` = 46382,`ConditionTypeOrReference` = 18,`ConditionValue1` = 1,`ConditionValue2` = 25745,`Comment` = 'Ahune summon - missile target';
INSERT INTO `conditions` SET `SourceTypeOrReferenceId` = 13,`SourceEntry` = 46623,`ConditionTypeOrReference` = 18,`ConditionValue1` = 1,`ConditionValue2` = 25746,`Comment` = 'Ahune summon HM loot - target';
UPDATE `creature_template` SET `faction_A` = 14,`faction_H` = 14 WHERE `entry` = 25865;
DELETE FROM `spell_scripts` WHERE `id` IN (45941, 46623);
INSERT INTO `spell_scripts` SET `id` = 45941,`command` = 15,`datalong` = 45939,`datalong2` = 2;
INSERT INTO `spell_scripts` SET `id` = 46623,`command` = 15,`datalong` = 46622,`datalong2` = 2;
DELETE FROM `gameobject_loot_template` WHERE `entry` = 188124;
INSERT INTO `gameobject_loot_template` VALUES
 (188124, 34955, 2, 1, 0, 1, 1),
 (188124, 35498, 7, 1, 0, 1, 1),
 (188124, 35557, 30, 1, 0, 2, 2),
 (188124, 35720, 5, 1, 0, 1, 6),
 (188124, 35723, 45, 1, 0, 1, 1),
 (188124, 54801, 0, 1, 1, 1, 1),
 (188124, 54802, 0, 1, 1, 1, 1),
 (188124, 54803, 0, 1, 1, 1, 1),
 (188124, 54804, 0, 1, 1, 1, 1),
 (188124, 54805, 0, 1, 1, 1, 1),
 (188124, 54806, 0, 1, 1, 1, 1),
 (188124, 49426, 100, 1, 0, 1, 1);
DELETE FROM `script_texts` WHERE `npc_entry` = 25740;
INSERT INTO `script_texts` SET `npc_entry` = 25740,`entry` = -1999977,`type` = 3,`content_default` = '%s will emerge soon!',`comment` = 'Ahune SAY1: emerge';
INSERT INTO `script_texts` SET `npc_entry` = 25740,`entry` = -1999978,`type` = 3,`content_default` = '%s submerged!',`comment` = 'Ahune SAY2: submerge';
INSERT INTO `script_texts` SET `npc_entry` = 25740,`entry` = -1999979,`type` = 3,`content_default` = '%s leaves the battle!',`comment` = 'Ahune SAY3: evade';
INSERT INTO `script_texts` SET `npc_entry` = 25740,`entry` = -1999980,`type` = 3,`content_default` = '%s enters the battle!',`comment` = 'Ahune SAY4: aggro';
-- -------------------------------------------------------------------------}
-- -------------------- Myth Project "Achievements" Update -----------------
-- -------------------------------------------------------------------------{
-- Achievement: The Incredible Hulk, http://old.wowhead.com/achievement=2043 
DELETE FROM `achievement_criteria_data` WHERE `criteria_id` = 7322 AND `type` = 18;
-- Achievement: Burning Hot Pole Dance, http://old.wowhead.com/achievement=271
DELETE FROM `spell_dbc` WHERE `Id` IN (29710, 58934);
INSERT INTO `spell_dbc`(`Id`,`Dispel`,`Mechanic`,`Attributes`,`AttributesEx`,`AttributesEx2`,`AttributesEx3`,`AttributesEx4`,`AttributesEx5`,`Stances`,`StancesNot`,`Targets`,`CastingTimeIndex`,`AuraInterruptFlags`,`ProcFlags`,`ProcChance`,`ProcCharges`,`MaxLevel`,`BaseLevel`,`SpellLevel`,`DurationIndex`,`RangeIndex`,`StackAmount`,`EquippedItemClass`,`EquippedItemSubClassMask`,`EquippedItemInventoryTypeMask`,`Effect1`,`Effect2`,`Effect3`,`EffectDieSides1`,`EffectDieSides2`,`EffectDieSides3`,`EffectRealPointsPerLevel1`,`EffectRealPointsPerLevel2`,`EffectRealPointsPerLevel3`,`EffectBasePoints1`,`EffectBasePoints2`,`EffectBasePoints3`,`EffectMechanic1`,`EffectMechanic2`,`EffectMechanic3`,`EffectImplicitTargetA1`,`EffectImplicitTargetA2`,`EffectImplicitTargetA3`,`EffectImplicitTargetB1`,`EffectImplicitTargetB2`,`EffectImplicitTargetB3`,`EffectRadiusIndex1`,`EffectRadiusIndex2`,`EffectRadiusIndex3`,`EffectApplyAuraName1`,`EffectApplyAuraName2`,`EffectApplyAuraName3`,`EffectAmplitude1`,`EffectAmplitude2`,`EffectAmplitude3`,`EffectMultipleValue1`,`EffectMultipleValue2`,`EffectMultipleValue3`,`EffectMiscValue1`,`EffectMiscValue2`,`EffectMiscValue3`,`EffectMiscValueB1`,`EffectMiscValueB2`,`EffectMiscValueB3`,`EffectTriggerSpell1`,`EffectTriggerSpell2`,`EffectTriggerSpell3`,`EffectSpellClassMaskA1`,`EffectSpellClassMaskA2`,`EffectSpellClassMaskA3`,`EffectSpellClassMaskB1`,`EffectSpellClassMaskB2`,`EffectSpellClassMaskB3`,`EffectSpellClassMaskC1`,`EffectSpellClassMaskC2`,`EffectSpellClassMaskC3`,`MaxTargetLevel`,`SpellFamilyName`,`SpellFamilyFlags1`,`SpellFamilyFlags2`,`SpellFamilyFlags3`,`MaxAffectedTargets`,`DmgClass`,`PreventionType`,`DmgMultiplier1`,`DmgMultiplier2`,`DmgMultiplier3`,`AreaGroupId`,`SchoolMask`,`Comment`) VALUES
 (29710, 0, 0, 256, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 101, 0, 0, 0, 0, 0, 1, 0, -1, 0, 0, 140, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29531, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1,'Ribbon Pole - Force Cast Ribbon Pole Channel'), 
 (58934, 0, 0, 536870912, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 101, 0, 0, 0, 0, 0, 1, 0, -1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'Burning Hot Pole Dance credit marker');
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (29531, 45390);
INSERT INTO `spell_linked_spell`(`spell_trigger`,`spell_effect`,`type`,`comment`) VALUES
 (29531, 45390, 0,'Ribbon Pole - Dancer Check Aura'), 
 (45390, 45406, 2,'Ribbon Pole - Periodic Visual');
DELETE FROM `spell_script_names` WHERE `spell_id` = 45390;
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
 (45390,'spell_gen_ribbon_pole_dancer_check');
-- -------------------------------------------------------------------------}
-- ----------------------- Myth Project "Quests" Update --------------------
-- -------------------------------------------------------------------------{
-- Quest: A Distraction for Akama, http://old.wowhead.com/quest=10985
DELETE FROM `creature` WHERE `guid` IN(77388,77152);
UPDATE `creature_template` SET `faction_A` = 1866, `faction_H` = 1866 WHERE `entry` IN (23191, 21701);
UPDATE `creature_template` SET scriptname = 'npc_xiri' WHERE entry = 18528;
UPDATE `creature_template` SET `modelid1` = 21349, `modelid2` = 21349, `minlevel` = 73, `maxlevel` = 73, `mindmg` = 2000, `maxdmg` = 3000, `attackpower` = 16125, `baseattacktime` = 2000, `faction_A` = 1866, `faction_H` = 1866, `equipment_id` = 2182 , scriptname = 'npc_preeven_maiev' WHERE `entry` = 22989;
UPDATE `creature_template` SET `attackpower` = 6540, `speed_run` = 1.1 WHERE `entry` = 23191;
UPDATE `creature_template` SET `minlevel` = 72, `maxlevel` = 72, `mindmg` = 3003, `maxdmg` = 6139, `attackpower` = 31997, `baseattacktime` = 1400, `rangeattacktime` = 1900, `rangedattackpower` = 100, `rank` = 3, `faction_A` = 14, `faction_H` = 14, `type_flags` = 0, `speed_run` = 1.48, `mechanic_immune_mask` = 617299803 WHERE `entry` = 23152;
-- Quest: Curing the Sick, http://old.wowhead.com/quest=6124
-- Quest: Curing the Sick, http://old.wowhead.com/quest=6129
DELETE FROM `spell_script_names` WHERE `spell_id`='19512';
-- Quest: And Now, the Moment of Truth, http://www.wowhead.com/quest=10201
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_grek' WHERE `entry` = 19606;
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 19606;
-- Quest: Mission: The Abyssal Shelf, http://www.wowhead.com/quest=10162
UPDATE `quest_template` SET `StartScript` = 10162 WHERE `entry`  = 10162;
DELETE FROM `quest_start_scripts` WHERE `id` = 10162;
INSERT INTO `quest_start_scripts`(`id`,`delay`,`command`,`datalong`,`datalong2`,`dataint`,`x`,`y`,`z`,`o`) VALUES
(10162, 0, 7, 10162, 75, '0', 0, 0, 0, 0);
-- Quest: Jormuttar is Soo Fat..., http://old.wowhead.com/quest=13011
UPDATE `quest_template` SET `Method` = 2 WHERE `entry` IN (13011,
-- Quest: Preparations for War, http://old.wowhead.com/quest=13418
 13418,
-- Quest: It's All Fun and Games, http://old.wowhead.com/quest=12887
 12887,
-- Quest: Mission: The Abyssal Shelf, http://old.wowhead.com/quest=10163
 10163,
-- Quest: They Took Our Men!, http://old.wowhead.com/quest=12843
 12843,
-- Quest: Elven Legends, http://old.wowhead.com/quest=7481
 7481,
-- Quest: Leave No One Behind, http://old.wowhead.com/quest=12512
 12512);
-- Quest: Mounting Hodir's Helm, http://old.wowhead.com/quest=12987
SET @ENTRY := 30210;
SET @SPELL_READ_PRONOUNCEMENT := 56278;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = @ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts`(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,8,0,100,0,@SPELL_READ_PRONOUNCEMENT,0,0,0,33,@ENTRY,0,0,0,0,0,1,0,0,0,0,0,0,0,"Hodir's Helm KC Bunny - On Spellhit - Quest Credit"),
(@ENTRY,0,1,2,61,0,100,0,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Hodir's Helm KC Bunny - On Spellhit - Say Line 0"),
(@ENTRY,0,2,0,61,0,100,0,0,0,0,0,50,192080,30000,0,0,0,0,8,0,0,0,7390.143066,-2725.382080,874.256104,2.973291,"Hodir's Helm KC Bunny - On Spellhit - Summon Hodir's Helm");
DELETE FROM `creature_text` WHERE `entry`=@ENTRY;
INSERT INTO `creature_text`(`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@ENTRY,0,0,"Sons of Hodir! I humbly present to you... The Helm of Hodir!",42,0,100,0,0,0,"Hodir's Helm KC Bunny");
UPDATE `gameobject_template` SET `flags`=`flags`|36 WHERE `entry`=192080;
-- DELETE FROM `conditions` WHERE `SourceEntry`=@SPELL_READ_PRONOUNCEMENT AND `ConditionValue2`=@ENTRY;
-- INSERT INTO `conditions`(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`,`Comment`) VALUES
-- (13,0,@SPELL_READ_PRONOUNCEMENT,18,1,@ENTRY,"Read Pronouncement targets Hodir's Helm KC Bunny");
SET @GUID := 190002;
DELETE FROM `creature` WHERE `id` = @ENTRY;
INSERT INTO `creature`(`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(@GUID,@ENTRY,571,1,1,0,0,7390.14,-2725.38,874.256,2.90103,300,0,0,42,0,0,0,0,0);
UPDATE `gameobject_template` SET `ScriptName` = 'go_org_portal' WHERE `entry` = 193052;
UPDATE `gameobject_template` SET `ScriptName` = 'go_sw_portal' WHERE `entry` = 193053;
UPDATE `gameobject` SET `phaseMask` = 384 WHERE `id` IN (193052, 193053);
-- Quest: Gothik the Harvester, http://old.wowhead.com/quest=12697
DELETE FROM `creature_questrelation` WHERE quest = 12697;
INSERT INTO `creature_questrelation`(`id`,`quest`) VALUES
(28377,12697);
-- Quest: Scarlet Armies Approach..., http://old.wowhead.com/quest=12757
UPDATE `quest_template` SET SrcSpell = 0 WHERE entry = 12757;
UPDATE `gameobject_template` SET flags = 0 WHERE entry = 191155;
DELETE FROM `spell_scripts` WHERE id = 53099;
INSERT INTO `spell_scripts`(`id`,`command`,`datalong`) VALUES
(53099,15,53098);
DELETE FROM `spell_target_position` WHERE id = 53098;
INSERT INTO `spell_target_position`(`id`,`target_map`,`target_position_x`,`target_position_y`,`target_position_z`,`target_orientation`) VALUES
(53098,609,2400.03,-5630.87,377,0.57491);
DELETE FROM `creature_template_addon` WHERE `entry` = 28511;
 
DELETE FROM `fishing_loot_template` WHERE `entry` = 4395 AND `item` = 43630;
INSERT INTO `fishing_loot_template`(`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES 
(4395, 43630, 0.2, 1, 1, 1, 1);
-- DK Questline
UPDATE `creature_template` SET `ScriptName` = 'npc_eye_of_acherus' WHERE `entry` = 28511;
UPDATE `creature_template` SET `ScriptName` = 'npc_acherus_teleporter' WHERE `entry` IN (29580, 29581, 29588, 29589); -- Teleport
-- Quest: If He Cannot Be Turned (Alliance), http://wotlk.openwow.com/?quest=12896
-- Quest: If He Cannot Be Turned (Horde), http://wotlk.openwow.com/?quest=12897
UPDATE `quest_template` SET `PrevQuestId` = 12893,`NextQuestId` = 13117 WHERE `entry` IN (12896, 12897);
-- Quest: Fresh Remounts, http://wotlk.openwow.com/?quest=12214
-- NPC: Onslaught Warhorse, http://wotlk.openwow.com/?npc=27213
UPDATE `creature_template` SET `faction_A` = 14,`faction_H` = 14 WHERE `entry` = 27213;
UPDATE `quest_template` SET `ReqItemId1` = 37202,`ReqItemCount1` = 1 WHERE `entry` = 12214;
-- Quest: Sniffing Out the Perpetrator, http://wotlk.openwow.com/?quest=12855
-- NPC: Tracker Thulin, http://wotlk.openwow.com/?npc=29695
UPDATE `creature_loot_template` SET `ChanceOrQuestChance` = 78,`groupid` = 1 WHERE `entry` = 29695 AND `item` = 40971;
-- Quest: Mission: The Abyssal Shelf, http://wotlk.openwow.com/?quest=10162
UPDATE `quest_template` SET
-- `ReqItemId1` = 28132,`ReqItemId2` = 28132,`ReqItemId3` = 28132,
-- `ReqItemId4` = 28132,`ReqItemId4` = 28132,`ReqItemId6` = 28132,
-- `ReqItemCount1` = 1,`ReqItemCount1` = 2,`ReqItemCount3` = 1,
-- `ReqItemCount4` = 1,`ReqItemCount1` = 5,`ReqItemCount6` = 1,
 `Method` = 2 WHERE `entry` = 10162;
-- Quest: Strengthen the Ancients (Alliance), http://old.wowhead.com/quest=12092
-- Quest: Strengthen the Ancients (Horde), http://old.wowhead.com/quest=12096
-- NPC: Woodlands Walker, http://old.wowhead.com/npc=26421
UPDATE `creature_template` SET `IconName` = '',`npcflag` = 1 WHERE `entry` = 26421;
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 26421;
DELETE FROM `creature_ai_scripts` WHERE `id` = 264210 AND `creature_id` = 26421;
INSERT INTO `creature_ai_scripts`(`id`,`creature_id`,`event_type`,`event_flags`,`action1_type`,`action1_param1`) VALUES
 (264210, 26421, 11, 1, 2, 35);
-- Quest: Garments of the Light, http://old.wowhead.com/quest=5624
-- NPC: Guard Roberts, http://old.wowhead.com/npc=12423
UPDATE `creature_template` SET `unit_flags` = 4104 WHERE `entry` = 12423;
-- Quest: An Innocent Disguise, http://old.wowhead.com/quest=11891
-- NPC: Ice Caller Briatha, http://old.wowhead.com/npc=25949
-- NPC: Heretic Emissary, http://old.wowhead.com/npc=25951
SET @Briatha := 25949;
SET @Emissary := 25951;
SET @QuestEventCredit := 46375;
UPDATE `creature_template` SET `AIName` =  'SmartAI' WHERE `entry` = @Briatha;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = @Briatha;
INSERT INTO `smart_scripts`(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@Briatha, 0, 0, 0, 60, 0, 100, 0, 0, 60000, 60000, 80001, 1, 0, 4000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Briatha - On event update /60 secs/ - Say text 0 /to trigger spy event dialogue/'), 
(@Briatha, 0, 1, 0, 52, 0, 100, 0, 0, @Briatha, 0, 0, 1, 0, 4000, 0, 0, 0, 0, 9, @Emissary, 0, 15, 0, 0, 0, 0,'Emissary - On creature text end /Briatha 0/ - Say text 0'), 
(@Briatha, 0, 2, 0, 52, 0, 100, 0, 0, @Emissary, 0, 0, 1, 1, 4000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Briatha - On creature text end /Emissary 0/ - Say text 1'), 
(@Briatha, 0, 3, 0, 52, 0, 100, 0, 1, @Briatha, 0, 0, 1, 1, 4000, 0, 0, 0, 0, 9, @Emissary, 0, 15, 0, 0, 0, 0,'Emissary - On creature text end /Briatha 1/ - Say text 1'), 
(@Briatha, 0, 4, 0, 52, 0, 100, 0, 1, @Emissary, 0, 0, 1, 2, 3500, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Briatha - On creature text end /Emisary 1/ - Say text 2'), 
(@Briatha, 0, 5, 0, 52, 0, 100, 0, 2, @Briatha, 0, 0, 11, @QuestEventCredit, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Briatha - On creature text end /Briatha 2/ - Cast spell quest credit on self');
DELETE FROM `creature_text` WHERE `entry` = @Briatha;
INSERT INTO `creature_text`(`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@Briatha, 0, 0,'These stones should be the last of them. Our coordination with Neptulon''s forces will be impeccable.', 12, 0, 100, 1, 1000, 0,'Briatha - Spy event'), 
(@Briatha, 1, 0,'And your own preparations? Will the Frost Lord have a path to the portal?', 12, 0, 100, 1, 1000, 0,'Briatha - Spy event'), 
(@Briatha, 2, 0,'The ritual in coilfang will bring Ahune through once he is fully prepared, and the resulting clash between Firelord and Frostlord will rend the foundations of this world. Our ultimate goals are in reach at last...', 12, 0, 100, 1, 3500, 0,'Briatha - Spy event');
DELETE FROM `creature_text` WHERE `entry` = @Emissary;
INSERT INTO `creature_text`(`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@Emissary, 0, 0,'Yess. The Tidehunter will be pleased at this development. The Firelord''s hold will weaken.', 12, 0, 100, 1, 1000, 0,'Heretic Emissary - Spy event'), 
(@Emissary, 1, 0,'Skar''this has informed us well. We have worked our way into the slave pens and await your cryomancerss.', 12, 0, 100, 1, 1000, 0,'Heretic Emissary - Spy event');
-- Quest: The Honored Dead, http://old.wowhead.com/quest=11593
UPDATE `creature_template` SET `unit_flags` = `unit_flags`&0xFFFFFFFD WHERE `entry` IN (25342, 25343);
UPDATE `creature_template` SET `unit_flags` = `unit_flags`|0x100 WHERE `entry` IN (25342, 25343);
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 25343;
UPDATE `creature_ai_scripts` SET `action2_type` = 41 WHERE `id` = 20491;
DELETE FROM `creature_ai_scripts` WHERE `id` = 21024;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 45474 AND `ConditionValue1` = 2;
-- Quest: Plan B, http://old.wowhead.com/quest=11658 (a minor cleanup)
UPDATE `gossip_scripts` SET `datalong` = 0 WHERE `id` = 9155 AND `command` = 18;
DELETE FROM `gossip_scripts` WHERE `id` = 9155 AND `command` = 33;
INSERT INTO `gossip_scripts` SET `id` = 9155, `command` = 33;
-- Quest: Blood Oath of the Horde, http://www.wowhead.com/quest=11983
-- see also unused spell 47010
UPDATE `creature_template` SET `gossip_menu_id` = 26184 WHERE `entry` = 26179;
DELETE FROM `gossip_menu` WHERE `entry` BETWEEN 26184 AND 26187;
INSERT INTO `gossip_menu` VALUES (26184, 12611),(26185, 12618),(26186, 12619),(26187, 12620);
DELETE FROM `gossip_menu_option` WHERE `menu_id` BETWEEN 26184 AND 26187;
INSERT INTO `gossip_menu_option` SET `menu_id` = 26184,`option_id` = 1,`npc_option_npcflag` = 1,`option_text` = 'Worry no more, taunka. The Horde will save and protect you and your people, but first you must swear allegiance to the Warchief by taking the blood oath of the Horde.',`action_menu_id` = 26185;
INSERT INTO `gossip_menu_option` SET `menu_id` = 26185,`option_id` = 1,`npc_option_npcflag` = 1,`option_text` = 'Yes, taunka. Retribution is a given right of all members of the Horde.',`action_menu_id` = 26186;
INSERT INTO `gossip_menu_option` SET `menu_id` = 26186,`option_id` = 1,`npc_option_npcflag` = 1,`option_text` = 'Then repeat after me: "Lok\'tar ogar! Victory or death - it is these words that bind me to the Horde. For they are the most sacred and fundamental of truths to any warrior of the Horde.\r\nI give my flesh and blood freely to the Warchief. I am the instrument of my Warchief\'s desire. I am a weapon of my Warchief\'s command.\r\nFrom this moment until the end of days I live and die - For the Horde!"',`action_menu_id` = 26187;
INSERT INTO `gossip_menu_option` SET `menu_id` = 26187,`option_id` = 1,`npc_option_npcflag` = 1,`option_text` = 'For the Horde!\r\nArm yourself from the crates that surround us and report to Agmar\'s Hammer, east of here. Your first trial as a member of the Horde is to survive the journey.\r\nLok\'tar ogar!',`action_script_id` = 26184;
DELETE FROM `gossip_scripts` WHERE `id` = 26184;
INSERT INTO `gossip_scripts` SET `id` = 26184,`delay` = 0,`command` = 33;
INSERT INTO `gossip_scripts` SET `id` = 26184,`delay` = 0,`command` = 5,`datalong` = 82,`datalong2` = 1;
INSERT INTO `gossip_scripts` SET `id` = 26184,`delay` = 0,`command` = 8,`datalong` = 26179;
INSERT INTO `gossip_scripts` SET `id` = 26184,`delay` = 300,`command` = 4,`datalong` = 82,`datalong2` = 1;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 26184;
INSERT INTO `conditions` SET `SourceTypeOrReferenceId` = 15,`SourceGroup` = 26184,`SourceEntry` = 0,`ConditionTypeOrReference` = 9,`ConditionValue1` = 11983,`Comment` = 'q:11983 talk with n:26179';
-- Quest: Subject to Interpretation, http://old.wowhead.com/quest=11991
DELETE FROM `conditions` WHERE `SourceEntry` = 47110 AND `ConditionValue1` = 0;
DELETE FROM `creature_ai_scripts` WHERE `id` = 25815;
INSERT INTO `creature_ai_scripts` VALUES (25815, 26498, 8, 0, 100, 0, 47110, '-1', 0, 0, 11, 47118, 6, 17, 0, 0, 0, 0, 0, 0, 0, 0, 'Drakuru Bunny1: Envision forcecast');
UPDATE `creature_template` SET `AIName` = 'EventAI' WHERE `entry` = 26498;
-- Quest: An Apexis Relic, http://old.wowhead.com/quest=11058
-- Quest: Guardian of the Monument, http://old.wowhead.com/quest=11059
-- Quest: The Relic's Emanation, http://old.wowhead.com/quest=11080
UPDATE `quest_template` SET `Method` = 2, `ReqCreatureOrGOId1` = 0, `ReqCreatureOrGOCount1` = 0 WHERE `entry` IN(11058, 11059, 11080);
UPDATE `gameobject_template` SET `data3` = 8704 WHERE `entry` = 185944;
DELETE FROM `gossip_menu_option` WHERE `menu_id` = 8704;
INSERT INTO `gossip_menu_option` VALUES (8704, 0, 0, 'Insert 35 Apexis Shards, and begin!', 1, 1, 0, 0, 0, 0, 0, NULL);
SET @NPC_SIMON_BUNNY           := 22923;
SET @GO_APEXIS_RELIC           := 185890;
SET @GO_APEXIS_MONUMENT        := 185944;
SET @SPELL_SIMON_BLUE_VISUAL   := 40244;
SET @SPELL_SIMON_GREEN_VISUAL  := 40245;
SET @SPELL_SIMON_RED_VISUAL    := 40246;
SET @SPELL_SIMON_YELLOW_VISUAL := 40247;
SET @ITEM_APEXIS_SHARD         := 32569;
SET @QUEST_THE_CRYSTALS        := 11025;
SET @QUEST_GUARDIAN_MONUMENT   := 11059;
SET @GOSSIP_MENU_ID_NORMAL     := 8703;
SET @GOSSIP_MENU_ID_LARGE      := 8704;
SET @ID_SPELL_GROUP            := 1115;
UPDATE `creature_template` SET `ScriptName` = 'npc_simon_bunny',`flags_extra` = 130 WHERE `entry` = @NPC_SIMON_BUNNY;
UPDATE `gameobject_template` SET `ScriptName` = 'go_apexis_relic' WHERE `entry` IN (@GO_APEXIS_MONUMENT,@GO_APEXIS_RELIC);
UPDATE `gameobject_template` SET `flags` = 16,`ScriptName` = 'go_simon_cluster' WHERE `displayId` IN (7364, 7365, 7366, 7367, 7369, 7371, 7373, 7375);
DELETE FROM `spell_group` WHERE `spell_id` IN (40055, 40165, 40166, 40167, 40623, 40625, 40626);
INSERT INTO `spell_group`(`id`,`spell_id`) VALUES
(@ID_SPELL_GROUP+0, 40055),
(@ID_SPELL_GROUP+0, 40165),
(@ID_SPELL_GROUP+0, 40166),
(@ID_SPELL_GROUP+0, 40167),
(@ID_SPELL_GROUP+1, 40623),
(@ID_SPELL_GROUP+1, 40625),
(@ID_SPELL_GROUP+1, 40626);
DELETE FROM `spell_group_stack_rules` WHERE `group_id` IN (@ID_SPELL_GROUP,@ID_SPELL_GROUP+1);
INSERT INTO `spell_group_stack_rules`(`group_id`,`stack_rule`) VALUES
(@ID_SPELL_GROUP+0, 1),
(@ID_SPELL_GROUP+1, 1);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` IN (@SPELL_SIMON_BLUE_VISUAL,@SPELL_SIMON_GREEN_VISUAL,@SPELL_SIMON_RED_VISUAL,@SPELL_SIMON_YELLOW_VISUAL);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` IN (@GOSSIP_MENU_ID_NORMAL,@GOSSIP_MENU_ID_LARGE);
INSERT INTO `conditions`(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(15,@GOSSIP_MENU_ID_NORMAL, 0, 0, 8,@QUEST_THE_CRYSTALS , 0, 0, 0,'','Apexis Relic Gossip Condition - Must have completed quest The Crystals'),
(15,@GOSSIP_MENU_ID_NORMAL, 0, 0, 2,@ITEM_APEXIS_SHARD, 1, 0, 0,'','Apexis Relic Gossip Condition - Must have an Apexis Shard'),
(15,@GOSSIP_MENU_ID_LARGE, 0, 0, 9,@QUEST_GUARDIAN_MONUMENT , 0, 0, 0,'','Apexis Monument Gossip Condition - Must be on quest Guardian of the Monument'),
(15,@GOSSIP_MENU_ID_LARGE, 0, 0, 2,@ITEM_APEXIS_SHARD, 35, 0, 0,'','Apexis Monument Gossip Condition - Must have 35 Apexis Shard'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185604, 0, 0,'','Simon Game - Blue visual target Large Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185609, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185617, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185625, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185637, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185640, 0, 0,'','Simon Game - Blue visual target Large Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185647, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185649, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185652, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185659, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185660, 0, 0,'','Simon Game - Blue visual target Large Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185665, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185669, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185673, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185770, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185772, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185776, 0, 0,'','Simon Game - Blue visual target Large Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185780, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185784, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185791, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185795, 0, 0,'','Simon Game - Blue visual target Large Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185796, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185807, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185808, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185812, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185816, 0, 0,'','Simon Game - Blue visual target Large Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185820, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185824, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185828, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185832, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185836, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185840, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185844, 0, 0,'','Simon Game - Blue visual target Large Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185848, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_BLUE_VISUAL, 0, 18, 0, 185853, 0, 0,'','Simon Game - Blue visual target Blue Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185605, 0, 0,'','Simon Game - Green visual target Large Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185611, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185619, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185627, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185639, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185641, 0, 0,'','Simon Game - Green visual target Large Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185646, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185651, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185654, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185658, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185661, 0, 0,'','Simon Game - Green visual target Large Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185667, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185671, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185675, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185768, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185774, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185777, 0, 0,'','Simon Game - Green visual target Large Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185781, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185786, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185789, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185793, 0, 0,'','Simon Game - Green visual target Large Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185798, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185805, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185810, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185814, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185818, 0, 0,'','Simon Game - Green visual target Large Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185822, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185826, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185830, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185834, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185838, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185842, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185846, 0, 0,'','Simon Game - Green visual target Large Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185850, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_GREEN_VISUAL, 0, 18, 0, 185855, 0, 0,'','Simon Game - Green visual target Green Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185606, 0, 0,'','Simon Game - Red visual target Large Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185613, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185621, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185626, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185638, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185642, 0, 0,'','Simon Game - Red visual target Large Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185645, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185650, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185655, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185657, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185662, 0, 0,'','Simon Game - Red visual target Large Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185666, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185670, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185674, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185771, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185773, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185778, 0, 0,'','Simon Game - Red visual target Large Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185782, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185785, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185788, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185794, 0, 0,'','Simon Game - Red visual target Large Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185797, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185804, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185809, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185815, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185817, 0, 0,'','Simon Game - Red visual target Large Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185823, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185827, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185831, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185835, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185839, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185843, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185845, 0, 0,'','Simon Game - Red visual target Large Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185851, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_RED_VISUAL, 0, 18, 0, 185854, 0, 0,'','Simon Game - Red visual target Red Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185607, 0, 0,'','Simon Game - Yellow visual target Large Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185615, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185623, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185624, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185636, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185643, 0, 0,'','Simon Game - Yellow visual target Large Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185644, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185648, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185653, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185656, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185663, 0, 0,'','Simon Game - Yellow visual target Large Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185664, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185668, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185672, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185769, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185775, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185779, 0, 0,'','Simon Game - Yellow visual target Large Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185783, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185787, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185790, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185792, 0, 0,'','Simon Game - Yellow visual target Large Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185803, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185806, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185811, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185813, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185819, 0, 0,'','Simon Game - Yellow visual target Large Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185821, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185825, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185829, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185833, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185837, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185841, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185847, 0, 0,'','Simon Game - Yellow visual target Large Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185849, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster'),
(13, 0,@SPELL_SIMON_YELLOW_VISUAL, 0, 18, 0, 185852, 0, 0,'','Simon Game - Yellow visual target Yellow Cluster');
-- Quest: A Curse Upon Both of Your Clans!, http://old.wowhead.com/quest=10544
SET @GUID := 106556;
DELETE FROM `gameobject` WHERE `guid` IN(@GUID, @GUID+1);
INSERT INTO `gameobject`(`guid`,`id`,`map`,`position_x`,`position_y`,`position_z`,`spawntimesecs`,`state`) VALUES (@GUID, 300104, 530, 1537.82, 6317.24, 3.20611, -300, 1), (@GUID+1, 300104, 530, 1586.59, 6286.58, 3.81469, -300, 1);
DELETE FROM `spell_script_names` WHERE `spell_id` = 32580;
INSERT INTO `spell_script_names` VALUES (32580, 'spell_wicked_strong_fetish');
-- Quest: Cold Hearted, http://old.wowhead.com/quest=12856
SET @NPC_CAPTIVE_DRAKE        := 29708; -- Captive Proto-Drake
SET @NPC_FREED_DRAKE          := 29709; -- Freed Proto-Drake
SET @NPC_PRISONER             := 29639; -- Brunnhildar Prisoner
SET @NPC_LIBERATED            := 29734; -- Liberated Brunnhildar
SET @SPELL_SUM_FREE_DRAKE     := 55028; -- Summon Freed Proto-Drake
SET @SPELL_ICE_SHARD          := 55046; -- Ice Shard (Rank 3)
SET @SPELL_FREE_PRISONER      := 55048; -- Free Brunnhildar Prisoner
SET @AURA_IMPRISONMENT        := 54894; -- Icy Imprisonment
UPDATE `creature_template` SET `faction_A`=35,`faction_H`=35 WHERE `entry`=@NPC_CAPTIVE_DRAKE;
UPDATE `creature_template` SET `speed_walk`=1,`speed_run`=1.14286,`spell1`=@SPELL_ICE_SHARD,`unit_flags`=0x1000008,`InhabitType`=4,`ScriptName`='npc_freed_protodrake' WHERE `entry`=@NPC_FREED_DRAKE;
UPDATE `creature_template` SET `unit_flags`=33554432,`AIName`='SmartAI' WHERE `entry`=@NPC_LIBERATED;
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=@NPC_CAPTIVE_DRAKE;
INSERT INTO `npc_spellclick_spells`(`npc_entry`,`spell_id`,`quest_start`,`cast_flags`,`user_type`) VALUES
(@NPC_CAPTIVE_DRAKE,@SPELL_SUM_FREE_DRAKE, 0, 1, 0);
DELETE FROM `creature_template_addon` WHERE `entry`=@NPC_FREED_DRAKE;
INSERT INTO `creature_template_addon`(`entry`,`mount`,`bytes1`,`bytes2`,`auras`) VALUES
(@NPC_FREED_DRAKE, 0, 0x3000000, 0x1,'55034 61183');
DELETE FROM `creature_text` WHERE `entry`=@NPC_FREED_DRAKE;
INSERT INTO `creature_text`(`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@NPC_FREED_DRAKE, 0, 0,'Return to Brunnhildar Village!', 42, 0, 100, 0, 0, 0,'Freed Proto-Drake'),
(@NPC_FREED_DRAKE, 0, 1,'The proto-drake smells freedom and flies homeward!', 41, 0, 100, 0, 0, 0,'Freed Proto-Drake');
DELETE FROM `spell_scripts` WHERE `id`=@SPELL_FREE_PRISONER;
INSERT INTO `spell_scripts`(`id`,`effIndex`,`delay`,`command`,`datalong`,`datalong2`,`dataint`,`x`,`y`,`z`,`o`) VALUES
(@SPELL_FREE_PRISONER, 0, 0, 14,@AURA_IMPRISONMENT, 0, 0, 0, 0, 0, 0);
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@NPC_LIBERATED;
INSERT INTO `smart_scripts`(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@NPC_LIBERATED, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 41, 30000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Liberated Brunnhildar - On Spawn - Despawn in 30 seconds');
-- Quest: The Fallen Exarch, http://old.wowhead.com/quest=10915
DELETE FROM `event_scripts` WHERE `id` = 14444;
INSERT INTO `event_scripts` VALUES (14444, 1, 10, 22452, 120000, 0, -3361.267, 5154.073, -9.001, 0.0);
-- Quest: Catch the Wild Wolpertinger!, http://old.wowhead.com/quest=11431
DELETE FROM `creature_questrelation` WHERE `id` = 24657 AND `quest` = 11431;
INSERT INTO `creature_questrelation` VALUES (24657, 11431);
-- Quest: Massacre At Light's Point, http://www.wowhead.com/quest=12701 - moving the cart
UPDATE `gameobject_template` SET `ScriptName`='' WHERE `entry`=190767;
DELETE FROM `creature_text` WHERE `entry`=28841;
INSERT INTO `creature_text` SET `entry`=28841,`groupid`=0,`id`=0,`type`=12,`text`='Where\'d this come from? I better get this down to the ships before the foreman sees it!',`comment`='Scarlet Miner';
INSERT INTO `creature_text` SET `entry`=28841,`groupid`=1,`id`=0,`type`=12,`text`='Now I can have a rest!',`comment`='Scarlet Miner';
DELETE FROM `spell_target_position` WHERE `id`=52464;
INSERT INTO `spell_target_position` VALUES (52464, 609, 2383.869629,-5900.312500, 107.996086, 5.077);
-- -------------------------------------------------------------------------}
-- ---------------- From "< Personal Branch > Kyrgyzstan" ------------------
-- -------------------------------------------------------------------------{
-- Fix http://ru.wowhead.com/quest=12998
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = -192181 WHERE `entry` = 12998;
UPDATE `gameobject_template` SET `flags` = 4 WHERE `entry` = 192181;

-- FIx quest http://www.wowhead.com/quest=12029
DELETE FROM `creature_ai_scripts` WHERE (`creature_id`=26570);-- Famished Scourge Troll SAI
DELETE FROM spell_script_names WHERE spell_id = 47214;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(47214, 'spell_q12029_burninate');

-- Dregmar RUnebrand (Quest Reclusive Runemaster - 12150)
UPDATE `creature_template` SET `ScriptName` = 'npc_reclusive_runemaster', `AIName` = '' WHERE `entry` = '27003';
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = '27003';
DELETE FROM `script_texts` WHERE `entry` IN ('-1011020', '-1011021', '-1011022');
INSERT INTO `script_texts` (`npc_entry`, `entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `sound`, `type`, `language`, `emote`, `comment`)
VALUES
('27003', '-1011020', 'I know why you''ve come - one of those foolish Magnataur on the plains meddled and managed to get the dragons involved. Do you enjoy serving them like dogs?', '', '', '', '', '', '', '', '', '0', '1', '0', '0', 'Dregmar Runebrand - Aggro1'),
('27003', '-1011021', 'You seek their leader... little things, you wage war against the clans of Grom''thar the Thunderbringer himself. Don''t be so eager to rush to your deaths.', '', '', '', '', '', '', '', '', '0', '1', '0', '0', 'Dregmar Runebrand - Aggro2'),
('27003', '-1011022', 'Hah! So be it. Blow the horn of a magnataur leader at the ring of torches south of the Blue Dragonshrine. Make peace with your gods... Grom''thar will come.', '', '', '', '', '', '', '', '', '0', '1', '0', '0', 'Dregmar Runebrand - 20%');

-- Fix quest http://ru.wowhead.com/quest=11078
UPDATE `gameobject_template` SET `data3` = 0, `ScriptName` = 'go_dragonegg' WHERE `entry` IN (185932, 185936, 185937, 185938);
DELETE FROM `gossip_scripts` WHERE `id` IN (8685, 8689, 8690, 8691);
DELETE FROM `gossip_menu_option` WHERE `menu_id` IN (8685, 8689, 8690, 8691);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`='15' AND `SourceGroup` IN (8685, 8689, 8690, 8691);

-- Fix NPCs: 24214, 24213, 24215, 23931 & Quest http://ru.wowhead.com/quest=11300
DELETE FROM `creature` WHERE `id` IN (24214, 24213, 24215, 23931);
-- Val'kyrs flying addon
-- Daegarn say OOC
UPDATE creature_template SET AIName='EventAI' WHERE entry=24151;
DELETE FROM creature_ai_scripts WHERE creature_id=24151;
INSERT INTO creature_ai_scripts VALUES
(2415101,24151,1,0,70,1,38000,38000,48000,68000,1,-241511,-241512,0,0,0,0,0,0,0,0,0,'Daegarn say OOC'),
(2415102,24151,1,0,55,1,78000,78000,78000,78000,1,-241513,0,0,0,0,0,0,0,0,0,0,'Daegarn say OOC');
 -- Texts
DELETE FROM creature_ai_texts WHERE entry IN (-241511,-241512,-241513,-242131,-242151,-242141,-239311,-239312);
INSERT INTO creature_ai_texts (entry,content_default,TYPE,COMMENT)  VALUES
(-241511,'What did you do with the people of this village!?',0,'Common Daegarn text'),
(-241512,'What do you want with me?',0,'Common Daegarn text'),
(-241513,'Release me, cowards! I\'ll show you what a dwarf is made of!',0,'Common Daegarn text'),
(-242131,'Firjus fears nothing! Release the combatants!',1,'Firjus the Soul Crusher yell'),
(-242151,'Firjus was unworthy! Test your battle prowess against a true soldier of the Lich King!',1,'Jlarborn the Strategist yell'),
(-242141,'Good... More flesh for the harvest...',1,'Yorus the Flesh Harvester yell'),
(-239311,'The ring will overflow with the blood of the interlopers! Oluf has come!',1,'Oluf the Violent yell'),
(-239312,'The ancient cipher falls on the ground.',3,'Oluf the Violent notify');
 -- Add spawn place
DELETE FROM creature_ai_summons WHERE id=11300;
INSERT INTO creature_ai_summons VALUES
(11300,846.877,-4688.59,-94.1835,3.47087,300000,'24213-24215-24214');
-- Add spell Head Crush (43348) and Cleave (15284)
DELETE FROM creature_ai_scripts WHERE id IN (2421301,2421302,2421303,2421304);
INSERT INTO creature_ai_scripts VALUES
(2421301,24213,4,0,100,0,0,0,0,0,1,-242131,0,0,0,0,0,0,0,0,0,0,'Firjus the Soul Crusher - Yell on Aggro'),
(2421302,24213,0,0,100,1,3000,5000,5000,7000,11,15284,1,0,0,0,0,0,0,0,0,0,'Firjus the Soul Crusher - Cast Cleave'),
(2421303,24213,0,0,75,1,5000,8000,8000,14000,11,43348,1,1,0,0,0,0,0,0,0,0,'Firjus the Soul Crusher - Cast Head Crush');
 -- ACID summon after dead
DELETE FROM creature_ai_scripts WHERE id=2421305;
UPDATE creature_template SET AIName='EventAI' WHERE entry=24213;
-- Add spell Arcing Smash (8374), Shield Bash (38233), Shield Block (12169), Shield Charge (15749)
DELETE FROM creature_ai_scripts WHERE id IN (2421501,2421502,2421503,2421504,2421505,2421506);
INSERT INTO creature_ai_scripts VALUES
(2421501,24215,4,0,100,0,0,0,0,0,1,-242151,0,0,0,0,0,0,0,0,0,0,'Firjus the Soul Crusher - Yell on Aggro'),
(2421502,24215,9,0,100,1,8,25,20000,25000,11,15749,1,0,0,0,0,0,0,0,0,0,'Firjus the Soul Crusher - Cast Shield Charge'),
(2421503,24215,0,0,75,1,2000,2000,6000,6000,11,8374,1,0,0,0,0,0,0,0,0,0,'Firjus the Soul Crusher - Cast Arcing Smash'),
(2421504,24215,13,0,75,1,1000,6000,0,0,11,38233,1,1,0,0,0,0,0,0,0,0,'Firjus the Soul Crusher - Cast Shield Bash'),
(2421505,24215,0,0,100,1,7000,11000,11000,18000,11,15284,0,0,0,0,0,0,0,0,0,0,'Firjus the Soul Crusher - Cast Shield Block');
 -- ACID summon after dead
UPDATE creature_template SET AIName='EventAI' WHERE entry=24215;
-- Add spell Cleave (15284) and Whirlwind (41056)
DELETE FROM creature_ai_scripts WHERE id IN(2421401,2421402,2421403,2421404);
INSERT INTO creature_ai_scripts VALUES
(2421401,24214,4,0,100,0,0,0,0,0,1,-242141,0,0,0,0,0,0,0,0,0,0,'Yorus the Flesh Harvester - Yell on Aggro'),
(2421402,24214,0,0,100,1,3000,5000,5000,7000,11,15284,1,0,0,0,0,0,0,0,0,0,'Yorus the Flesh Harvester - Cast Cleave'),
(2421403,24214,0,0,100,1,15000,15000,30000,30000,11,41056,0,1,0,0,0,0,0,0,0,0,'Yorus the Flesh Harvester - Cast Whirlwind');
-- ACID summon after dead
UPDATE creature_template SET AIName='EventAI' WHERE entry=24214;
-- Add spell Cleave (15284), Demoralizing Shout (13730), Whirlwind (41056), Net (6533), Throw Dragonflayer Harpoon (42870)
DELETE FROM creature_ai_scripts WHERE id IN(2393101,2393102,2393103,2393104,2393105,2393106,2393107);
INSERT INTO `creature_ai_scripts` VALUES
(2393101,23931,4,0,100,0,0,0,0,0,1,-239311,0,0,0,0,0,0,0,0,0,0,'Oluf the Violent - Yell on Aggro'),
(2393102,23931,0,0,100,1,3000,5000,5000,7000,11,15284,1,0,0,0,0,0,0,0,0,0,'Oluf the Violent - Cast Cleave'),
(2393103,23931,0,0,100,1,4000,4000,17200,25200,11,13730,0,1,0,0,0,0,0,0,0,0,'Oluf the Violent - Cast Demoralizing Shout'),
(2393104,23931,0,0,100,1,15000,15000,30000,30000,11,41056,0,1,0,0,0,0,0,0,0,0,'Oluf the Violent - Cast Whirlwind'),
(2393105,23931,9,0,100,1,10,20,1000,1000,11,6533,1,0,0,0,0,0,0,0,0,0,'Oluf the Violent - Cast Net'),
(2393106,23931,9,0,100,1,15,30,2000,2000,11,42870,1,1,0,0,0,0,0,0,0,0,'Oluf the Violent - Cast Throw Dragonflayer Harpoon'),
-- Drop Ancient Cipher (43326)
(2393107,23931,6,0,100,0,0,0,0,0,11,43326,0,3,1,-239312,0,0,0,0,0,0,'Oluf the Violent - Spawn Ancient Cipher and Notify on Death');
UPDATE creature_template SET AIName='EventAI' WHERE entry=23931;

-- Fix Toravon Loot
DELETE FROM `creature_loot_template` WHERE (`entry`=38462);
INSERT INTO `creature_loot_template` VALUES 
(38462, 51161, 0, 1, 1, 1, 1), (38462, 51211, 0, 1, 2, 1, 1), (38462, 51327, 0, 1, 4, 1, 1), (38462, 51328, 0, 1, 4, 1, 1), (38462, 51329, 0, 1, 4, 1, 1), (38462, 51330, 0, 1, 4, 1, 1), (38462, 51331, 0, 1, 4, 1, 1), (38462, 51332, 0, 1, 4, 1, 1), (38462, 51333, 0, 1, 4, 1, 1), (38462, 51334, 0, 1, 4, 1, 1), (38462, 51335, 0, 1, 4, 1, 1), (38462, 51336, 0, 1, 4, 1, 1), (38462, 51340, 0, 1, 4, 1, 1), (38462, 51341, 0, 1, 4, 1, 1), (38462, 51342, 0, 1, 4, 1, 1), (38462, 51343, 0, 1, 4, 1, 1), (38462, 51344, 0, 1, 4, 1, 1), (38462, 51345, 0, 1, 4, 1, 1), (38462, 51346, 0, 1, 4, 1, 1), (38462, 51347, 0, 1, 4, 1, 1), (38462, 51348, 0, 1, 4, 1, 1), (38462, 51349, 0, 1, 4, 1, 1), (38462, 51350, 0, 1, 4, 1, 1), (38462, 51351, 0, 1, 4, 1, 1), (38462, 51352, 0, 1, 4, 1, 1), (38462, 51353, 0, 1, 4, 1, 1), (38462, 51354, 0, 1, 4, 1, 1), (38462, 51355, 0, 1, 4, 1, 1), (38462, 51356, 0, 1, 4, 1, 1), (38462, 51357, 0, 1, 4, 1, 1), (38462, 51358, 0, 1, 4, 1, 1), (38462, 51359, 0, 1, 4, 1, 1), (38462, 51360, 0, 1, 4, 1, 1), (38462, 51361, 0, 1, 4, 1, 1), (38462, 51362, 0, 1, 4, 1, 1), (38462, 51363, 0, 1, 4, 1, 1), (38462, 51364, 0, 1, 4, 1, 1), (38462, 51365, 0, 1, 4, 1, 1), (38462, 51366, 0, 1, 4, 1, 1), (38462, 51367, 0, 1, 4, 1, 1), (38462, 51368, 0, 1, 4, 1, 1), (38462, 51369, 0, 1, 4, 1, 1), (38462, 51370, 0, 1, 4, 1, 1), (38462, 51371, 0, 1, 4, 1, 1), (38462, 51372, 0, 1, 4, 1, 1), (38462, 51373, 0, 1, 4, 1, 1), (38462, 51374, 0, 1, 4, 1, 1), (38462, 51375, 0, 1, 4, 1, 1), (38462, 51376, 0, 1, 4, 1, 1), (38462, 51542, 0, 1, 3, 1, 1), (38462, 51420, 1.66, 1, 3, 1, 1), (38462, 51422, 1.66, 1, 3, 1, 1), (38462, 51426, 1.66, 1, 3, 1, 1), (38462, 51428, 1.66, 1, 3, 1, 1), (38462, 51434, 1.66, 1, 3, 1, 1), (38462, 51436, 1.66, 1, 3, 1, 1), (38462, 51498, 1.66, 1, 3, 1, 1), (38462, 51500, 1.66, 1, 3, 1, 1), (38462, 51504, 1.66, 1, 3, 1, 1), (38462, 51506, 1.66, 1, 3, 1, 1), (38462, 51510, 1.66, 1, 3, 1, 1), (38462, 51512, 1.66, 1, 3, 1, 1), (38462, 43959, 2, 1, 0, 1, 1), (38462, 44083, 2, 1, 0, 1, 1), (38462, 51469, 2.5, 1, 3, 1, 1), (38462, 51471, 2.5, 1, 3, 1, 1), (38462, 51475, 2.5, 1, 3, 1, 1), (38462, 51477, 2.5, 1, 3, 1, 1), (38462, 51483, 2.5, 1, 3, 1, 1), (38462, 51485, 2.5, 1, 3, 1, 1), (38462, 51488, 2.5, 1, 3, 1, 1), (38462, 51490, 2.5, 1, 3, 1, 1), (38462, 51136, 3.33, 1, 1, 1, 1), (38462, 51138, 3.33, 1, 1, 1, 1), (38462, 51142, 3.33, 1, 1, 1, 1), (38462, 51144, 3.33, 1, 1, 1, 1), (38462, 51146, 3.33, 1, 1, 1, 1), (38462, 51148, 3.33, 1, 1, 1, 1), (38462, 51163, 3.33, 1, 1, 1, 1), (38462, 51168, 3.33, 1, 1, 1, 1), (38462, 51169, 3.33, 1, 1, 1, 1), (38462, 51171, 3.33, 1, 1, 1, 1), (38462, 51172, 3.33, 1, 1, 1, 1), (38462, 51191, 3.33, 1, 2, 1, 1), (38462, 51193, 3.33, 1, 2, 1, 1), (38462, 51196, 3.33, 1, 2, 1, 1), (38462, 51198, 3.33, 1, 2, 1, 1), (38462, 51201, 3.33, 1, 2, 1, 1), (38462, 51203, 3.33, 1, 2, 1, 1), (38462, 51126, 5, 1, 1, 1, 1), (38462, 51128, 5, 1, 1, 1, 1), (38462, 51131, 5, 1, 1, 1, 1), (38462, 51132, 5, 1, 1, 1, 1), (38462, 51177, 5, 1, 2, 1, 1), (38462, 51179, 5, 1, 2, 1, 1), (38462, 51181, 5, 1, 2, 1, 1), (38462, 51183, 5, 1, 2, 1, 1), (38462, 51213, 5, 1, 2, 1, 1), (38462, 51216, 5, 1, 2, 1, 1), (38462, 51217, 5, 1, 2, 1, 1), (38462, 51414, 5, 1, 3, 1, 1), (38462, 51416, 5, 1, 3, 1, 1), (38462, 51459, 5, 1, 3, 1, 1), (38462, 51461, 5, 1, 3, 1, 1), (38462, 51464, 5, 1, 3, 1, 1), (38462, 51466, 5, 1, 3, 1, 1), (38462, 51493, 5, 1, 3, 1, 1), (38462, 51495, 5, 1, 3, 1, 1), (38462, 51537, 5, 1, 3, 1, 1), (38462, 51539, 5, 1, 3, 1, 1), (38462, 51544, 5, 1, 3, 1, 1), (38462, 51152, 10, 1, 1, 1, 1), (38462, 51154, 10, 1, 1, 1, 1), (38462, 51157, 10, 1, 1, 1, 1), (38462, 51159, 10, 1, 1, 1, 1), (38462, 51186, 10, 1, 2, 1, 1), (38462, 51188, 10, 1, 2, 1, 1), (38462, 51207, 10, 1, 2, 1, 1), (38462, 51209, 10, 1, 2, 1, 1), (38462, 49426, 100, 1, 0, 2, 2);

-- Fix ahievement http://ru.wowhead.com/?achievement=3457
DELETE FROM `conditions` WHERE `SourceGroup`=9647;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(14, 9647, 13059, 0, 11, 50517, 0, 0, 0, '', 'Pirates Day: DeMeza gossip'),
(14, 9647, 13065, 0, 1, 50517, 0, 0, 0, '', 'Pirates Day: DeMeza gossip'),
(15, 9647, 0, 0, 11, 50517, 0, 0, 0, '', 'Pirates Day: DeMeza gossip');

-- Deleted bugged creature text(creatures are unneded...for what they texts there???)(now creature spam this text)
DELETE FROM `creature_addon` WHERE `guid`IN (77152,104533,104532,104531,104530,104539,104538,104537,104535,104534);
DELETE FROM `creature_addon` WHERE `guid` IN (102621, 102622, 102623, 102624);

-- Fix http://old.wowhead.com/item=40373 (cooldown must be 15 insteed 45seconds)
UPDATE `spell_proc_event` SET `Cooldown`='15' WHERE `entry` = 60487;

-- Fix item http://ru.wowhead.com/item=44430
UPDATE `item_template` SET `ScriptName` = 'item_titanium_seal_of_dalaran' WHERE `entry` = 44430;

-- Fix pet 15068;
UPDATE `creature_template` SET `spell1` = 0 WHERE `entry` = 15068;

/*
-- Hallow's End
UPDATE `quest_template` SET `SpecialFlags`=1 WHERE `entry`=25482;
DELETE FROM `creature_questrelation` WHERE `quest` = 25482;
DELETE FROM `gameobject_questrelation` WHERE `quest` = 25482;
UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 25482;
INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (9934, 25482);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 9934;
INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (9935, 25482);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 9935;
DELETE FROM `creature_involvedrelation` WHERE `quest` = 25482;
DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 25482;
INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (9934, 25482);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=9934;
INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (9935, 25482);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=9935;
DELETE FROM `quest_template` WHERE `entry` = 25482;
INSERT INTO `quest_template` (`entry`, `Method`, `ZoneOrSort`, `SkillOrClassMask`, `MinLevel`, `MaxLevel`, `QuestLevel`, `Type`, `RequiredRaces`, `RequiredSkillValue`, `RepObjectiveFaction`, `RepObjectiveValue`, `RepObjectiveFaction2`, `RepObjectiveValue2`, `RequiredMinRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepFaction`, `RequiredMaxRepValue`, `SuggestedPlayers`, `LimitTime`, `QuestFlags`, `SpecialFlags`, `CharTitleId`, `PlayersSlain`, `BonusTalents`, `RewardArenaPoints`, `PrevQuestId`, `NextQuestId`, `ExclusiveGroup`, `NextQuestInChain`, `RewXPId`, `SrcItemId`, `SrcItemCount`, `SrcSpell`, `Title`, `Details`, `Objectives`, `OfferRewardText`, `RequestItemsText`, `EndText`, `CompletedText`, `ObjectiveText1`, `ObjectiveText2`, `ObjectiveText3`, `ObjectiveText4`, `ReqItemId1`, `ReqItemId2`, `ReqItemId3`, `ReqItemId4`, `ReqItemId5`, `ReqItemId6`, `ReqItemCount1`, `ReqItemCount2`, `ReqItemCount3`, `ReqItemCount4`, `ReqItemCount5`, `ReqItemCount6`, `ReqSourceId1`, `ReqSourceId2`, `ReqSourceId3`, `ReqSourceId4`, `ReqSourceCount1`, `ReqSourceCount2`, `ReqSourceCount3`, `ReqSourceCount4`, `ReqCreatureOrGOId1`, `ReqCreatureOrGOId2`, `ReqCreatureOrGOId3`, `ReqCreatureOrGOId4`, `ReqCreatureOrGOCount1`, `ReqCreatureOrGOCount2`, `ReqCreatureOrGOCount3`, `ReqCreatureOrGOCount4`, `ReqSpellCast1`, `ReqSpellCast2`, `ReqSpellCast3`, `ReqSpellCast4`, `RewChoiceItemId1`, `RewChoiceItemId2`, `RewChoiceItemId3`, `RewChoiceItemId4`, `RewChoiceItemId5`, `RewChoiceItemId6`, `RewChoiceItemCount1`, `RewChoiceItemCount2`, `RewChoiceItemCount3`, `RewChoiceItemCount4`, `RewChoiceItemCount5`, `RewChoiceItemCount6`, `RewItemId1`, `RewItemId2`, `RewItemId3`, `RewItemId4`, `RewItemCount1`, `RewItemCount2`, `RewItemCount3`, `RewItemCount4`, `RewRepFaction1`, `RewRepFaction2`, `RewRepFaction3`, `RewRepFaction4`, `RewRepFaction5`, `RewRepValueId1`, `RewRepValueId2`, `RewRepValueId3`, `RewRepValueId4`, `RewRepValueId5`, `RewRepValue1`, `RewRepValue2`, `RewRepValue3`, `RewRepValue4`, `RewRepValue5`, `RewHonorAddition`, `RewHonorMultiplier`, `unk0`, `RewOrReqMoney`, `RewMoneyMaxLevel`, `RewSpell`, `RewSpellCast`, `RewMailTemplateId`, `RewMailDelaySecs`, `PointMapId`, `PointX`, `PointY`, `PointOpt`, `DetailsEmote1`, `DetailsEmote2`, `DetailsEmote3`, `DetailsEmote4`, `DetailsEmoteDelay1`, `DetailsEmoteDelay2`, `DetailsEmoteDelay3`, `DetailsEmoteDelay4`, `IncompleteEmote`, `CompleteEmote`, `OfferRewardEmote1`, `OfferRewardEmote2`, `OfferRewardEmote3`, `OfferRewardEmote4`, `OfferRewardEmoteDelay1`, `OfferRewardEmoteDelay2`, `OfferRewardEmoteDelay3`, `OfferRewardEmoteDelay4`, `StartScript`, `CompleteScript`, `WDBVerified`) VALUES (25482, 2, 0, 0, 75, 80, 80, 0, 2047, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4232, 1, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 'World Event Dungeon - Headless Horseman', '', '', '', '', '', '', '', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23682, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 54516, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 148000, 198600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12340);
*/
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
DELETE FROM `spell_script_names` WHERE `spell_id` = 19512;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('19512','spell_q6124_6129_apply_salve');
-- Fix http://www.wowhead.com/quest=13141
-- SQL Changes Needed
UPDATE creature_template SET scriptname = 'npc_blessed_banner' WHERE entry = 30891;
UPDATE creature_template SET faction_A = 2068, faction_H = 2068 WHERE entry IN (30986,30984,30987,30989);
UPDATE creature_template SET faction_A = 2131, faction_H = 2131 WHERE entry IN (31003,30919,30900);
UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry IN (30891);
DELETE FROM event_scripts WHERE id = 20082;
INSERT INTO event_scripts VALUES
(20082,0,10,30891,600000,0,6418.31,423.00,511.2,5.945);
DELETE FROM `creature` WHERE `id` IN (30986, 30987, 30989, 30900, 30984);
-- -------------------------------------------------------------------------}
-- ---------------------- Myth Project "NPCs" Update -----------------------
-- -------------------------------------------------------------------------{
-- NPC: Wintergarde Gryphon, http://old.wowhead.com/npc=27258
UPDATE `creature_template` SET `InhabitType` = 7 WHERE `entry` = 27258;
-- Dual Specialization Trainers:
UPDATE `gossip_menu_option` SET `box_money` = 10000000 WHERE `option_text` LIKE "%Dual Talent%";
-- NPC: Drakkari Guardian, http://old.wowhead.com/npc=26620
UPDATE `creature_template` SET `unit_flags` = 0,`dynamicflags` = 8,`type_flags` = 128 WHERE `entry` = 26620;
-- DK start location - triggers visibility
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=29521;
-- -------------------------------------------------------------------------}
-- ----------------------- Myth Project "GO" Update ------------------------
-- -------------------------------------------------------------------------{
-- Dark Runed Chest http://old.wowhead.com/object=190663 (193597) locked
UPDATE `gameobject_template` SET `flags` = `flags`|4 WHERE `entry` IN (190663, 193597);
-- -------------------------------------------------------------------------}
-- --------------------- Myth Project "Events" Update ----------------------
-- -------------------------------------------------------------------------{
-- * Zalazane Fall (world event), no last quest
-- misc
UPDATE `creature_template` SET `InhabitType`=3 WHERE `entry`=40416;
-- Quest: Frogs Away! (q:25446)
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=17 AND `SourceEntry`=74977;
INSERT INTO `conditions` SET `SourceTypeOrReferenceId`=17,`SourceEntry`=74977,`ConditionTypeOrReference`=29,`ConditionValue1`=40218,`ConditionValue2`=100,`ErrorTextId`=12000,`Comment`='Cast Frog Away! only near valid target';
UPDATE `creature_template_addon` SET `auras`='74971 74982' WHERE `entry`=40218;
-- Quest: Lady of da Tigers (q:25470)
UPDATE `quest_start_scripts` SET `datalong`=75147,`datalong2`=2 WHERE `id`=25470 AND `datalong`=75186;
UPDATE `quest_start_scripts` SET `datalong2`=2 WHERE `id`=25470 AND `datalong`=60957;
DELETE FROM `quest_start_scripts` WHERE `id`=25470 AND `command` IN (10, 0);
UPDATE `creature_template` SET `faction_A`=14,`faction_H`=14,`AIName`='',ScriptName='npc_tiger_matriarch' WHERE `entry`=40312;
DELETE FROM `areatrigger_scripts` WHERE `entry`=5843;
INSERT INTO `areatrigger_scripts` VALUES (5843, 'at_lady_of_tigers');

DELETE FROM `trinity_string` WHERE `entry`=10075;
INSERT INTO `trinity_string` SET `entry`=10075,content_default='The tiger matriarch appears! Prove yourself in combat!';
DELETE FROM `creature_template_addon` WHERE `entry`=40312;

UPDATE `creature_template` SET `unit_flags`=8 WHERE `entry`=40305;

SET @N_Zentabra = 40329; -- Zen'tabra
SET @S_ZentrabaSmoke = 36747; -- Spawn Smoke (Druid)
SET @N_TigerCredit = 40301; -- Tiger Matriarch Credit
SET @S_ZentrabaTransform = 74931; -- [DND] Tiger Transform

DELETE FROM `creature_text` WHERE `entry`=@N_Zentabra;
INSERT INTO `creature_text`(`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@N_Zentabra, 0, 0, 'Dat be enough for now!', 12, 0, 100, 25, 0, 0, 'Zen''tabra'),
(@N_Zentabra, 1, 0, 'Don''t be lookin'' so surprised. Your shaman friend has sharp eyes, or should I say, sharp frogs.', 12, 0, 100, 1, 0, 0, 'Zen''tabra'),
(@N_Zentabra, 2, 0, 'My kind, da druids, we been layin'' low for some time now. We been waitin'' to see when an'' if Vol''jin might return.', 12, 0, 100, 11, 0, 0, 'Zen''tabra'),
(@N_Zentabra, 3, 0, 'Now dat it looks like de Darkspear be returnin'' to these isles, maybe de time has come for us to reveal ourselves.', 12, 0, 100, 1, 0, 0, 'Zen''tabra'),
(@N_Zentabra, 4, 0, 'Go back to Vol''jin an'' tell him dis: Zen''tabra stands ready to help him in de coming battle.', 12, 0, 100, 1, 0, 0, 'Zen''tabra');
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@N_Zentabra, @N_Zentabra*100);
INSERT INTO `smart_scripts`(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`)VALUES
(@N_Zentabra, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 80,@N_Zentabra*100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Call actionlist on spawn'),
(@N_Zentabra*100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Set passive react'),
(@N_Zentabra*100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Set orientation to player'),
(@N_Zentabra*100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 11,@S_ZentrabaSmoke, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast spawn smoke effect on self'),
(@N_Zentabra*100, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Say text 0'),
(@N_Zentabra*100, 9, 4, 0, 0, 0, 100, 0, 1500, 1500, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Say text 1'),
(@N_Zentabra*100, 9, 5, 0, 0, 0, 100, 0, 5500, 5500, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Say text 2'),
(@N_Zentabra*100, 9, 6, 0, 0, 0, 100, 0, 5500, 5500, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Say text 3'),
(@N_Zentabra*100, 9, 7, 0, 0, 0, 100, 0, 5500, 5500, 0, 0, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Say text 4'),
(@N_Zentabra*100, 9, 8, 0, 0, 0, 100, 0, 0, 0, 0, 0, 33,@N_TigerCredit, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Award kill credit to player'),
(@N_Zentabra*100, 9, 9, 0, 0, 0, 100, 0, 5500, 5500, 0, 0, 11,@S_ZentrabaTransform, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast tiger transform on self'),
(@N_Zentabra*100, 9, 10, 0, 0, 0, 100, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Set passive react'),
(@N_Zentabra*100, 9, 11, 0, 0, 0, 100, 0, 100, 100, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Set run on self'),
(@N_Zentabra*100, 9, 12, 0, 0, 0, 100, 0, 0, 0, 0, 0, 46, 10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Move self forward 10 yards'),
(@N_Zentabra*100, 9, 13, 0, 0, 0, 100, 0, 600, 600, 0, 0, 41, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Despawn self');
UPDATE `creature_template` SET `unit_flags`=0x8300,`ScriptName`='',`AIName`='SmartAI',`flags_extra`=2 WHERE `entry`=40329;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=75197;
INSERT INTO `conditions` SET `SourceTypeOrReferenceId`=13,`SourceEntry`=75197,`ConditionTypeOrReference`=18,`ConditionValue1`=1,`ConditionValue2`=0,`Comment`='Zen\'tabra QC - target player';

-- Quest: Dance Of De Spirits (q:25480)
SET @N_DancePart = 40363; -- Dance Participant
SET @N_RDrummer = 40373; -- Ritual Drummer
SET @N_DAncestor = 40388; -- Darkspear Ancestor
SET @N_Voice = 40374; -- Voice of the Spirits
SET @N_Doctor = 40352; -- Witch Doctor Hez'tok
SET @Gossip = 50428;

UPDATE `creature_template` SET `faction_A`=126,`faction_H`=126,`exp`=2,`minlevel`=80,`maxlevel`=80,`baseattacktime`=2000,`unit_flags`=`unit_flags`|33536,`unit_class`=2,`speed_run`=1.1428571428571,`gossip_menu_id`=@Gossip,`npcflag`=`npcflag`|1,`AIName`='SmartAI' WHERE `entry`=@N_Doctor; -- Witch Doctor Hez'tok
UPDATE `creature_template` SET `faction_A`=126,`faction_H`=126,`exp`=2,`minlevel`=80,`maxlevel`=80,`baseattacktime`=2000,`unit_flags`=`unit_flags`|33536,`unit_class`=2,`speed_run`=1.1428571428571 WHERE `entry`=@N_Voice; -- Voice of the Spirits
UPDATE `creature_template` SET `faction_A`=126,`faction_H`=126,`exp`=2,`minlevel`=70,`maxlevel`=80,`baseattacktime`=2000,`unit_flags`=`unit_flags`|32768,`speed_run`=1.1428571428571 WHERE `entry`=@N_DAncestor; -- Darkspear Ancestor
UPDATE `creature_template` SET `faction_A`=126,`faction_H`=126,`exp`=2,`minlevel`=70,`maxlevel`=80,`baseattacktime`=2000,`equipment_id`=36,`unit_class`=2,`speed_run`=1.1428571428571,`AIName`='SmartAI' WHERE `entry`=@N_RDrummer; -- Ritual Drummer
UPDATE `creature_template` SET `baseattacktime`=2000,`unit_flags`=`unit_flags`|33555200,`speed_walk`=1.6,`speed_run`=2,`InhabitType`=5,`MovementType`=1,`AIName`='SmartAI' WHERE `entry`=@N_DancePart; -- Dance Participant

DELETE FROM `gossip_menu` WHERE `entry` IN (@Gossip);
INSERT INTO `gossip_menu`(`entry`,`text_id`) VALUES (@Gossip, 15846); -- Witch Doctor Hez'tok
DELETE FROM `gossip_menu_option` WHERE `menu_id` IN (@Gossip);
INSERT INTO `gossip_menu_option`(`menu_id`,`id`,`option_icon`,`option_text`,`option_id`,`npc_option_npcflag`,`action_menu_id`,`action_poi_id`,`action_script_id`,`box_coded`,`box_money`,`box_text`) VALUES
(@Gossip, 0, 0, 'Let us consult the omens.' , 1, 1, 0, 0, 0, 0, 0,NULL); -- Witch Doctor Hez'tok (quest Dance of De Spirits)

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@N_Doctor,@N_Doctor*100,@N_RDrummer,@N_DancePart);
INSERT INTO `smart_scripts` VALUES
(@N_Doctor, 0, 0, 0, 62, 0, 0, 0,@Gossip+0, 0, 0, 0, 80,@N_Doctor*100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - On gossip select start script'),
(@N_Doctor*100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Remove gossip flag'),
(@N_Doctor*100, 9, 1, 0, 0, 0, 100, 0, 1500, 1500, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Say text 0'),
(@N_Doctor*100, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 12,@N_RDrummer, 1, 50000, 0, 0, 0, 8, 0, 0, 0,-812.137,-4986.7, 17.3759, 5.89921, 'Witch Doctor Hez''tok - Summon Ritual Drummer (1)'),
(@N_Doctor*100, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_RDrummer, 1, 50000, 0, 0, 0, 8, 0, 0, 0,-798.187,-4985.52, 17.7904, 4.41568, 'Witch Doctor Hez''tok - Summon Ritual Drummer (2)'),
(@N_Doctor*100, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_RDrummer, 1, 50000, 0, 0, 0, 8, 0, 0, 0,-799.888,-4975.01, 17.9325, 0.942478, 'Witch Doctor Hez''tok - Summon Ritual Drummer (3)'),
(@N_Doctor*100, 9, 5, 0, 0, 0, 100, 0, 3500, 3500, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0,-806.2,-4989.5, 17.5177, 0, 'Witch Doctor Hez''tok - Move to pos'),
(@N_Doctor*100, 9, 6, 0, 0, 0, 100, 0, 6500, 6500, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Say text 1'),
(@N_Doctor*100, 9, 7, 0, 0, 0, 100, 0, 5500, 5500, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Say text 2'),
(@N_Doctor*100, 9, 8, 0, 0, 0, 100, 0, 3500, 3500, 0, 0, 5, 25, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Do emote'),
(@N_Doctor*100, 9, 9, 0, 0, 0, 100, 0, 3500, 3500, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Say text 3'),
(@N_Doctor*100, 9, 10, 0, 0, 0, 100, 0, 2500, 2500, 0, 0, 5, 6, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Do emote'),
(@N_Doctor*100, 9, 11, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 11, 56745, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Cast Drink Alcohol self'),
(@N_Doctor*100, 9, 12, 0, 0, 0, 100, 0, 600, 600, 0, 0, 11, 29389, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Cast Firebreathing self'),
(@N_Doctor*100, 9, 13, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 90, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Set bytes1 kneel state'),
(@N_Doctor*100, 9, 14, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 12,@N_DancePart, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-805.8477,-5003.044, 20.18328, 5.89921, 'Witch Doctor Hez''tok - Summon Dance Participant (1)'),
(@N_Doctor*100, 9, 15, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DancePart, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-801.0605,-4998.501, 18.59358, 4.41568, 'Witch Doctor Hez''tok - Summon Dance Participant (2)'),
(@N_Doctor*100, 9, 16, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DancePart, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-808.2397,-4985.208, 19.54311, 0.942478, 'Witch Doctor Hez''tok - Summon Dance Participant (3)'),
(@N_Doctor*100, 9, 17, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DancePart, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-799.2618,-4994.353, 19.61933, 5.89921, 'Witch Doctor Hez''tok - Summon Dance Participant (4)'),
(@N_Doctor*100, 9, 18, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DancePart, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-797.8184,-4986.597, 21.60157, 4.41568, 'Witch Doctor Hez''tok - Summon Dance Participant (5)'),
(@N_Doctor*100, 9, 19, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DancePart, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-805.1284,-4987.553, 18.79003, 0.942478, 'Witch Doctor Hez''tok - Summon Dance Participant (6)'),
(@N_Doctor*100, 9, 20, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DancePart, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-814.1109,-5001.676, 19.44409, 5.89921, 'Witch Doctor Hez''tok - Summon Dance Participant (7)'),
(@N_Doctor*100, 9, 21, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DancePart, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-795.7561,-4993.671, 21.80729, 5.89921, 'Witch Doctor Hez''tok - Summon Dance Participant (8)'),
(@N_Doctor*100, 9, 22, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 12,@N_DAncestor, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-790.217041,-4999.733, 17.171814, 2.84488654, 'Witch Doctor Hez''tok - Summon Darkspear Ancestor (1)'),
(@N_Doctor*100, 9, 23, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DAncestor, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-792.0052,-4994.14258, 17.4839725, 2.70526028, 'Witch Doctor Hez''tok - Summon Darkspear Ancestor (2)'),
(@N_Doctor*100, 9, 24, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DAncestor, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-795.2049,-5003.078, 17.716095, 2.46091413, 'Witch Doctor Hez''tok - Summon Darkspear Ancestor (3)'),
(@N_Doctor*100, 9, 25, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DAncestor, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-799.2274,-5005.68066, 16.6322536, 2.11184835, 'Witch Doctor Hez''tok - Summon Darkspear Ancestor (4)'),
(@N_Doctor*100, 9, 26, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DAncestor, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-799.2557,-4990.76172, 17.6279469, 2.70527148, 'Witch Doctor Hez''tok - Summon Darkspear Ancestor (5)'),
(@N_Doctor*100, 9, 27, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DAncestor, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-801.422058,-4998.04346, 17.0008545, 2.46088934, 'Witch Doctor Hez''tok - Summon Darkspear Ancestor (6)'),
(@N_Doctor*100, 9, 28, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DAncestor, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-805.4358,-5002.88525, 16.544487, 1.727876, 'Witch Doctor Hez''tok - Summon Darkspear Ancestor (7)'),
(@N_Doctor*100, 9, 29, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DAncestor, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-807.135,-4997.469, 17.0008545, 1.15000379, 'Witch Doctor Hez''tok - Summon Darkspear Ancestor (8)'),
(@N_Doctor*100, 9, 30, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DAncestor, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-810.3698,-4993.825, 17.1258545, 0.808653831, 'Witch Doctor Hez''tok - Summon Darkspear Ancestor (9)'),
(@N_Doctor*100, 9, 31, 0, 0, 0, 100, 0, 0, 0, 0, 0, 12,@N_DAncestor, 1, 40000, 0, 0, 0, 8, 0, 0, 0,-810.3889,-5004.778, 16.12407, 1.15191734, 'Witch Doctor Hez''tok - Summon Darkspear Ancestor (10)'),
(@N_Doctor*100, 9, 32, 0, 0, 0, 100, 0, 7000, 7000, 0, 0, 11, 70663, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Cast Shadow Nova self'),
(@N_Doctor*100, 9, 33, 0, 0, 0, 100, 0, 0, 0, 0, 0, 11, 31309, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Cast Spirit Particles self'),
(@N_Doctor*100, 9, 34, 0, 0, 0, 100, 0, 0, 0, 0, 0, 91, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Set bytes1 none'),
(@N_Doctor*100, 9, 35, 0, 0, 0, 100, 0, 100, 100, 0, 0, 3,@N_Voice, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Change entry to Voice of the Spirits'),
(@N_Doctor*100, 9, 36, 0, 0, 0, 100, 0, 100, 100, 0, 0, 3, 0, 31819, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Change model to Voice of the Spirits'),
(@N_Doctor*100, 9, 37, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Say text 4'),
(@N_Doctor*100, 9, 38, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Say text 5'),
(@N_Doctor*100, 9, 39, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 5, 25, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Do emote'),
(@N_Doctor*100, 9, 40, 0, 0, 0, 100, 0, 3500, 3500, 0, 0, 1, 6, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Say text 6'),
(@N_Doctor*100, 9, 41, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Say text 7'),
(@N_Doctor*100, 9, 42, 0, 0, 0, 100, 0, 4500, 4500, 0, 0, 5, 274, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Do emote'),
(@N_Doctor*100, 9, 43, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 1, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Say text 8'),
(@N_Doctor*100, 9, 44, 0, 0, 0, 100, 0, 4500, 4500, 0, 0, 5, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Do emote'),
(@N_Doctor*100, 9, 45, 0, 0, 0, 100, 0, 5500, 5500, 0, 0, 11, 70663, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Cast Shadow Nova self'),
(@N_Doctor*100, 9, 46, 0, 0, 0, 100, 0, 100, 100, 0, 0, 28, 31309, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Remove aura Spirit Particles'),
(@N_Doctor*100, 9, 47, 0, 0, 0, 100, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Change entry and model to Witch Doctor Hez''tok'),
(@N_Doctor*100, 9, 48, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 1, 9, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Say text 9'),
(@N_Doctor*100, 9, 49, 0, 0, 0, 100, 0, 2500, 2500, 0, 0, 5, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Do emote'),
(@N_Doctor*100, 9, 50, 0, 0, 0, 100, 0, 0, 0, 0, 0, 85, 75319, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Cast Omen Event Credit invoker'), -- working
-- (@N_Doctor*100, 9, 50, 0, 0, 0, 100, 0, 0, 0, 0, 0, 11, 75319, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Cast Omen Event Credit self'), -- how it should but doesn't work
(@N_Doctor*100, 9, 51, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0,-805.0104,-4975.75, 17.75085, 0, 'Witch Doctor Hez''tok - Move to pos'),
(@N_Doctor*100, 9, 52, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 4.625123, 'Witch Doctor Hez''tok - Change orientation'),
(@N_Doctor*100, 9, 53, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 82, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witch Doctor Hez''tok - Enable gossip flag'),
(@N_DancePart, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 89, 10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dance Participant - On reset set random movement'),
(@N_DancePart, 0, 1, 0, 25, 0, 100, 0, 0, 0, 0, 0, 3, 0, 22769, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dance Participant - On reset set model'),
(@N_RDrummer, 0, 0, 0, 1, 0, 100, 0, 0, 0, 1000, 2000, 5, 38, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ritual Drummer - Play emote attack 2h every 1 or 2 secs'),
(@N_RDrummer, 0, 1, 0, 1, 0, 100, 0, 0, 0, 5000, 5000, 11, 75313, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ritual Drummer - Cast Bang Ritual Gong every 5 seconds'),
(@N_RDrummer, 0, 2, 0, 1, 0, 100, 1, 4000, 4000, 0, 0, 4, 7294, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ritual Drummer - Play TrollDrumLoop after 4 seconds of spawn - not repeteable'),
(@N_RDrummer, 0, 3, 0, 1, 0, 100, 0, 0, 0, 30000, 30000, 4, 7294, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ritual Drummer - Play Drumms sound every 30 secs');

DELETE FROM `creature_text` WHERE `entry` IN (@N_Doctor);
INSERT INTO `creature_text`(`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@N_Doctor, 0, 0, 'Darkspears, we consult de spirits! Drummers, take your places!' , 12, 0, 100, 25, 0, 0, 'Witch Doctor Hez''tok' ),
(@N_Doctor, 1, 0, 'Spirits, we be gathered here to ask for your guidance.' , 12, 0, 100, 5, 0, 0, 'Witch Doctor Hez''tok' ),
(@N_Doctor, 2, 0, 'Our leader, Vol''jin, son of Sen''jin, issued de call to all Darkspears: reclaim de Echo Isles for our tribe.' , 12, 0, 100, 1, 0, 0, 'Witch Doctor Hez''tok'),
(@N_Doctor, 3, 0, 'Spirits! I offer me own body to you! Speak through me! Is de time right for mighty Vol''jin''s undertaking?' , 12, 0, 100, 5, 0, 0, 'Witch Doctor Hez''tok'),
(@N_Doctor, 4, 0, 'De ancestors hear ya, witch doctor!' , 12, 0, 100, 1, 0, 0, 'Witch Doctor Hez''tok'),
(@N_Doctor, 5, 0, 'Know dat your plans please us, Darkspears. De son of Sen''jin walks de right path.' , 12, 0, 100, 1, 0, 0, 'Witch Doctor Hez''tok'),
(@N_Doctor, 6, 0, 'De task in front of ya will not be easy, but ya have our blessin''.' , 12, 0, 100, 1, 0, 0, 'Witch Doctor Hez''tok'),
(@N_Doctor, 7, 0, 'Ya gave up your home an'' ya gave up de loas of your ancestors when ya left de Echo Isles. Dey will not be pleased dat you been ignorin'' dem.' , 12, 0, 100, 1, 0, 0, 'Witch Doctor Hez''tok'),
(@N_Doctor, 8, 0, 'Ya must make amends wit'' Bwonsamdi, de guardian of de dead, if ya hope to defeat Zalazane. It be de only way. Tell de son of Sen''jin dese things.' , 12, 0, 100, 1, 0, 0, 'Witch Doctor Hez''tok'),
(@N_Doctor, 9, 0, 'De spirits have blessed us wit'' an answer! De Echo Isles will be ours again!' , 12, 0, 100, 5, 0, 0, 'Witch Doctor Hez''tok');

-- Quest: Trollin' For Volunteers (q:25461)
UPDATE `quest_template` SET `Method`=2 WHERE `entry`=25229;
DELETE FROM `gossip_menu_option` WHERE `menu_id`=11345 AND `id`=2;
INSERT INTO `gossip_menu_option` SET `menu_id`=11345,`id`=2,`option_icon`=0,`option_text`='I need to get to Razor Hill to reqruit on behalf of Vol''jin. May i borrow a bat?',`option_id`=1,`npc_option_npcflag`=1,`action_script_id`=25461;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=11345 AND `SourceEntry`=2;
INSERT INTO `conditions` SET `SourceTypeOrReferenceId`=15,`SourceGroup`=11345,`SourceEntry`=2,`ConditionTypeOrReference`=9,`ConditionValue1`=25461,`Comment`='Bat ride to Razor Hill - for q:25461';
DELETE FROM `gossip_scripts` WHERE `id`=25461;
INSERT INTO `gossip_scripts` VALUES (25461, 0, 15, 75421, 3, 0, 0, 0, 0, 0);

SET @N_Citizien1 = 40256; -- Troll Citizien (1)
SET @N_Citizien2 = 40257; -- Troll Citizien (2)
SET @N_Volunteer1 = 40260; -- Troll Volunteer
SET @N_Volunteer2 = 40264; -- Troll Volunteer
SET @S_VolunterSummon1 = 75088; -- Motivate
SET @S_VolunterSummon2 = 75086; -- Motivate

UPDATE `creature_template` SET `faction_A`=126,`faction_H`=126,`baseattacktime`=2000,`unit_flags`=`unit_flags`|33280,`speed_run`=1.1428571428571,`AIName`='' WHERE `entry`=@N_Citizien1; -- Troll Citizien (1)
UPDATE `creature_template` SET `faction_A`=126,`faction_H`=126,`baseattacktime`=2000,`unit_flags`=`unit_flags`|33280,`speed_run`=1.1428571428571,`AIName`='' WHERE `entry`=@N_Citizien2; -- Troll Citizien (2)
UPDATE `creature_template` SET `flags_extra`=0,`InhabitType`=3,`baseattacktime`=2000,`unit_flags`=`unit_flags`|0x00008308,ScriptName='npc_troll_volunteer' WHERE `entry` IN (@N_Volunteer1,@N_Volunteer2); -- Troll Volunteer

DELETE FROM `creature_text` WHERE `entry` IN (@N_Volunteer1,@N_Volunteer2);
INSERT INTO `creature_text`(`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@N_Volunteer1, 0, 0, 'Sign me up!', 12, 0, 100, 66, 0, 0, 'Troll Volunteer JustSpawned'),
(@N_Volunteer1, 0, 1, 'Anythin'' for Vol''jin!', 12, 0, 100, 0, 0, 0, 'Troll Volunteer JustSpawned'),
(@N_Volunteer1, 0, 2, 'I''d be glad to help.', 12, 0, 100, 273, 0, 0, 'Troll Volunteer JustSpawned'),
(@N_Volunteer1, 0, 3, 'Just point me at de enemy!', 12, 0, 100, 66, 0, 0, 'Troll Volunteer JustSpawned'),
(@N_Volunteer2, 0, 0, 'Sign me up!', 12, 0, 100, 66, 0, 0, 'Troll Volunteer JustSpawned'),
(@N_Volunteer2, 0, 1, 'Anythin'' for Vol''jin!', 12, 0, 100, 0, 0, 0, 'Troll Volunteer JustSpawned'),
(@N_Volunteer2, 0, 2, 'I''d be glad to help.', 12, 0, 100, 273, 0, 0, 'Troll Volunteer JustSpawned'),
(@N_Volunteer2, 0, 3, 'Just point me at de enemy!', 12, 0, 100, 66, 0, 0, 'Troll Volunteer JustSpawned'),
(@N_Volunteer1, 1, 0, 'Reportin'' for duty.', 12, 0, 100, 66, 0, 0, 'Troll Volunteer Quest Turn in'),
(@N_Volunteer1, 1, 1, 'Ready to take de fight to Zalazane.', 12, 0, 100, 1, 0, 0, 'Troll Volunteer Quest Turn in'),
(@N_Volunteer1, 1, 2, 'Ready to fight beside Vol''jin!', 12, 0, 100, 66, 0, 0, 'Troll Volunteer Quest Turn in'),
(@N_Volunteer1, 1, 3, 'New troll here!', 12, 0, 100, 0, 0, 0, 'Troll Volunteer Quest Turn in'),
(@N_Volunteer1, 1, 4, 'When does de trainin'' start?', 12, 0, 100, 6, 0, 0, 'Troll Volunteer Quest Turn in'),
(@N_Volunteer2, 1, 0, 'Reportin'' for duty.', 12, 0, 100, 66, 0, 0, 'Troll Volunteer Quest Turn in'),
(@N_Volunteer2, 1, 1, 'Ready to take de fight to Zalazane.', 12, 0, 100, 1, 0, 0, 'Troll Volunteer Quest Turn in'),
(@N_Volunteer2, 1, 2, 'Ready to fight beside Vol''jin!', 12, 0, 100, 66, 0, 0, 'Troll Volunteer Quest Turn in'),
(@N_Volunteer2, 1, 3, 'New troll here!', 12, 0, 100, 0, 0, 0, 'Troll Volunteer Quest Turn in'),
(@N_Volunteer2, 1, 4, 'When does de trainin'' start?', 12, 0, 100, 6, 0, 0, 'Troll Volunteer Quest Turn in');

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@N_Citizien1,@N_Citizien2) AND `source_type`=0;
DELETE FROM `spell_script_names` WHERE `spell_id` IN (75420, 75102);
INSERT INTO `spell_script_names` VALUES (75420,'spell_mount_check');
INSERT INTO `spell_script_names` VALUES (75102,'spell_voljin_war_drums');

-- Quest: Preparin' For Battle (q:25495)
UPDATE `gossip_menu_option` SET `option_icon`=0 WHERE `menu_id`=11345 AND `id`=1;
UPDATE `conditions` SET `ConditionTypeOrReference`=28,`Comment`='Bat ride: Echo Isles for (completed) q:25495' WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=11345 AND `SourceEntry`=1;
UPDATE `gossip_scripts` SET `datalong2`=3 WHERE `id`=40204;

-- * Operation Gnomeregan event (2 start quests only)
-- Quest: A Few Good Gnomes (q:25229)
UPDATE `creature_template` SET `InhabitType`=3,`ScriptName`='npc_gnome_volunteer',`flags_extra`=0 WHERE `entry` IN (39466, 39624);
DELETE FROM `spell_script_names` WHERE `spell_id`=74035;
INSERT INTO `spell_script_names` VALUES (74035,'spell_motivate_a_tron');

DELETE FROM `creature_text` WHERE `entry` IN (39466, 39624);
INSERT INTO `creature_text` VALUES
(39466, 0, 0, 'Anything for King Mekkatorque!', 12, 0, 100, 66, 0, 0, 'Gnome Volunteer JustSpawned'),
(39466, 0, 1, 'Can I bring my wrench?', 12, 0, 100, 0, 0, 0, 'Gnome Volunteer JustSpawned'),
(39466, 0, 2, 'Is this going to hurt?', 12, 0, 100, 273, 0, 0, 'Gnome Volunteer JustSpawned'),
(39466, 0, 3, 'I\'d love to help!', 12, 0, 100, 66, 0, 0, 'Gnome Volunteer JustSpawned'),
(39624, 0, 0, 'Sign me up!', 12, 0, 100, 66, 0, 0, 'Gnome Volunteer JustSpawned'),
(39624, 0, 1, 'Wow! We\'re taking back Gnomeregan? I\'m in!', 12, 0, 100, 0, 0, 0, 'Gnome Volunteer JustSpawned'),
(39624, 0, 2, 'My wrench of vengeance awaits!', 12, 0, 100, 273, 0, 0, 'Gnome Volunteer JustSpawned'),
(39624, 0, 3, 'I want to drive a Spider Tank!', 12, 0, 100, 66, 0, 0, 'Gnome Volunteer JustSpawned');
INSERT INTO `creature_text` VALUES
(39466, 1, 0, 'This is going to be fascinating!', 12, 0, 100, 66, 0, 0, 'Gnome Volunteer Delivered'),
(39466, 1, 1, 'Do you think they will let me keep my uniform when I\'m done?', 12, 0, 100, 0, 0, 0, 'Gnome Volunteer Delivered'),
(39466, 1, 2, 'How soon until the victory party starts? I\'m ready to dance!', 12, 0, 100, 273, 0, 0, 'Gnome Volunteer Delivered'),
(39466, 1, 3, 'My wrench of vengeance awaits!', 12, 0, 100, 66, 0, 0, 'Gnome Volunteer Delivered'),
(39624, 1, 0, 'How soon until the victory party starts? I\'m ready to dance!', 12, 0, 100, 66, 0, 0, 'Gnome Volunteer Delivered'),
(39624, 1, 1, 'Do you think they will let me keep my uniform when I\'m done?', 12, 0, 100, 0, 0, 0, 'Gnome Volunteer Delivered'),
(39624, 1, 2, 'My wrench of vengance awaits!', 12, 0, 100, 273, 0, 0, 'Gnome Volunteer Delivered'),
(39624, 1, 3, 'I want to drive a Spider Tank!', 12, 0, 100, 66, 0, 0, 'Gnome Volunteer Delivered');

-- Quest: Basic Orders (q:25199)
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry` IN (73835, 73836, 73725, 73837, 73886) AND `ConditionTypeOrReference`=18;
INSERT INTO `conditions` SET `SourceTypeOrReferenceId`=13,`SourceEntry`=73835,`ConditionTypeOrReference`=18,`ConditionValue1`=1,`ConditionValue2`=39349,`Comment`='Gnomeregan Trainee perform Salute';
INSERT INTO `conditions` SET `SourceTypeOrReferenceId`=13,`SourceEntry`=73836,`ConditionTypeOrReference`=18,`ConditionValue1`=1,`ConditionValue2`=39349,`Comment`='Gnomeregan Trainee perform Roar';
INSERT INTO `conditions` SET `SourceTypeOrReferenceId`=13,`SourceEntry`=73725,`ConditionTypeOrReference`=18,`ConditionValue1`=1,`ConditionValue2`=39349,`Comment`='Gnomeregan Trainee perform Cheer';
INSERT INTO `conditions` SET `SourceTypeOrReferenceId`=13,`SourceEntry`=73837,`ConditionTypeOrReference`=18,`ConditionValue1`=1,`ConditionValue2`=39349,`Comment`='Gnomeregan Trainee perform Dance';
INSERT INTO `conditions` SET `SourceTypeOrReferenceId`=13,`SourceEntry`=73886,`ConditionTypeOrReference`=18,`ConditionValue1`=1,`ConditionValue2`=39349,`Comment`='Gnomeregan Trainee stop Dance';

DELETE FROM `spell_script_names` WHERE `spell_id` IN (73835, 73836, 73725, 73837, 73886);
INSERT INTO `spell_script_names` VALUES (73835,'spell_test_salute'),(73836,'spell_test_roar'),(73725,'spell_test_cheer'),(73837,'spell_test_dance'),(73886,'spell_test_stop_dance');

UPDATE `creature_template` SET `ScriptName`='npc_sergeant_steamcrank' WHERE `entry`=39368;
UPDATE `creature` SET `orientation`=1.427 WHERE `guid`=88173;

DELETE FROM `creature_text` WHERE `entry`=39368;
INSERT INTO `creature_text`(`entry`,`groupid`,`id`,`text`,`type`,`probability`,`comment`) VALUES
(39368, 0, 0,'All right, you twisted mess of broken gears, let\'s get to work!', 12, 100.,'q25199'),
(39368, 1, 0,'I am going to teach you all what it takes to be a proper soldier!', 12, 100.,'q25199'),
(39368, 2, 0,'The first thing you need to learn is proper discipline.', 12, 100.,'q25199'),
(39368, 3, 0,'Show me that discipline with a proper salute when I say so!', 12, 100.,'q25199'),
(39368, 4, 0,'Okay, recruits, salute!', 14, 100.,'q25199'),
(39368, 5, 0,'Nice job!', 12, 100.,'q25199'),
(39368, 6, 0,'When you go into combat, it is important that you know how to intimidate the enemy with a deafening battle roar!', 12, 100.,'q25199'),
(39368, 7, 0,'When I give the signal, let loose your greatest roar!', 12, 100.,'q25199'),
(39368, 8, 0,'Let me hear that battle roar!', 14, 100.,'q25199'),
(39368, 9, 0,'Wow, nice and scary!', 12, 100.,'q25199'),
(39368, 10, 0,'Remember though, a key factor in winning any battle is positive reinforcement.', 12, 100.,'q25199'),
(39368, 11, 0,'Reward your fellow soldiers in battle with a resounding cheer. Now, cheer at me when I tell you to!', 12, 100.,'q25199'),
(39368, 12, 0,'Everyone, cheer!', 14, 100.,'q25199'),
(39368, 13, 0,'Fantastic!', 12, 100.,'q25199'),
(39368, 14, 0,'However, the most important part of battle is how you celebrate a good hard earned victory!', 12, 100.,'q25199'),
(39368, 15, 0,'Show me your best victory dance when I give the signal!', 12, 100.,'q25199'),
(39368, 16, 0,'Now, dance!', 14, 100.,'q25199'),
(39368, 17, 0,'Superb!', 12, 100.,'q25199'),
(39368, 18, 0,'You might be the best set of recruits I\'ve ever seen! Let\'s go through all of that again!', 12, 100.,'q25199');
DELETE FROM `spell_script_names` WHERE `spell_id` = 74222;
INSERT INTO `spell_script_names` VALUES
 (74222,'spell_og_perform_speech');
UPDATE `creature_template` SET `ScriptName` = 'npc_mekkatorque_vision' WHERE `entry` = 39712;
UPDATE `gameobject_template` SET `ScriptName` = 'go_musty_coffin' WHERE `entry` = 190949;
UPDATE `creature_involvedrelation` SET `id` = 3446 WHERE `id`= 4794 AND `quest` = 1270;
 DELETE FROM `spell_script_names` WHERE `spell_id` IN (70924,73015);
 INSERT INTO `spell_script_names` VALUES
  (70924, 'spell_blood_queen_uncontrollable_frenzy'),
  (73015, 'spell_blood_queen_uncontrollable_frenzy');
-- -------------------------------------------------------------------------}
-- --------------------- Myth Project "Spells" Update ----------------------
-- -------------------------------------------------------------------------{
-- Spell Bonus data
-- POISONS
DELETE FROM `spell_bonus_data` WHERE `entry` IN ( 13219,
 8679, 2823, 18799, 10577, 43730, 28714, 67712, 67758, 54841, 7711, 7712, 15438,
 55755, 35792, 13897, 6296, 44525, 27102, 27104, 46579, 46629, 31535, 60306, 18276,
 13442, 18205, 13440, 18083, 55747, 17330, 18086, 18833, 5707, 23267, 18816, 23266, 
 20886, 18138, 13439, 16415, 29638, 17154, 18796, 18211, 26789, 29502, 24251, 24254);
INSERT INTO `spell_bonus_data` VALUES
(13219, 0, 0, 0, 0, 'Wound Poison'),
(8679, 0, 0, 0, 0, 'Instant Poison'),
(2823, 0, 0, 0, 0, 'Deadly Poison'),
-- ENCHANTS
(31535, 0, 0, 0, 0, 'Bloodboil Poison'),
(35792, 0, 0, 0, 0, 'Battlemaster Enchant'),
(13897, 0, 0, 0, 0, 'Fiery Weapon Enchant'),
(6296, 0, 0, 0, 0, 'Fiery Blaze Enchant'),
(44525, 0, 0, 0, 0, 'Icebreaker Enchant'),
(27102, 0, 0, 0, 0, 'Unholy Weapon Enchant'),
(27104, 0, 0, 0, 0, 'Lifestealing Enchant'),
(46579, 0, 0, 0, 0, 'Deathfrost Enchant'),
(46629, 0, 0, 0, 0, 'Deathfrost Enchant trigger'),
-- ITEMS 
(18799, 0, 0, 0, 0, 'Freezing Band'),
(10577, 0, 0, 0, 0, 'Gauntlets of the Sea'),
(43730, 0, 0, 0, 0, 'Stormchops'),
(28714, 0, 0, 0, 0, 'Flamecap'),
(67712, 0, 0, 0, 0, 'Reign of the Dead & Unliving normal'),
(67758, 0, 0, 0, 0, 'Reign of the Dead & Unliving heroic'),
(54841, 0, 0, 0, 0, 'Thunder Capacitor'),
(55755, 0, 0, 0, 0, 'Brunnhildar weapons'),
(7712, 0, 0, 0, 0, 'Blazefury Medallion & Fiery Retributor'),
-- Other items
(60306, 0, 0, 0, 0, 'Vestige of Haldor'),
(18276, 0, 0, 0, 0, 'Frost Blast'),
(13442, 0, 0, 0, 0, 'Firebolt'),
(18205, 0, 0, 0, 0, 'Shadow Bolt'),
(13440, 0, 0, 0, 0, 'Shadow Bolt'),
(18083, 0, 0, 0, 0, 'Galgann\'s Firehammer'),
(55747, 0, 0, 0, 0, 'Horn of Argent Fury'),
(17330, 0, 0, 0, 0, 'Smolderweb\'s Eye'),
(18086, 0, 0, 0, 0, 'Teebu\'s Blazing Longsword'),
(18833, 0, 0, 0, 0, 'Alcor\'s Sunrazor'),
(5707, 0, 0, 0, 0, 'Lifestone'),
(23267, 0, 0, 0, 0, 'Perdition\'s Blade'),
(18816, 0, 0, 0, 0, 'Skullflame Shield'),
(23266, 0, 0, 0, 0, 'Essence of the Pure Flame'),
(15438, 0, 0, 0, 0, 'Naglering and Drillborer Disk'),
(20886, 0, 0, 0, 0, 'Razor Gauntlets'),
(18138, 0, 0, 0, 0, 'Shadowblade'),
(13439, 0, 0, 0, 0, 'Hammer of the Northern Wind'),
(16415, 0, 0, 0, 0, 'Taran Icebreaker'),
(29638, 0, 0, 0, 0, 'Bow of Searing Arrows'),
(17154, 0, 0, 0, 0, 'The Green Tower'),
(18796, 0, 0, 0, 0, 'Fiery War Axe'),
(18211, 0, 0, 0, 0, 'Nightblade'),
(26789, 0, 0, 0, 0, 'Shard of the Fallen Star'),
(29502, 0, 0, 0, 0, 'Hurricane'),
(24251, 0, 0, 0, 0, 'Zulian Slice'),
(24254, 0, 0, 0, 0, 'Sceptre of Smiting');

-- Death Knight
DELETE FROM `spell_bonus_data` WHERE `entry` IN (
 58621, 47632, 47633, 50444, 70890, 50526, 48982, 59754, 49184, 33745, 9007, 56641, 42243, 3674, 19306, 1495, 63468, 136,
 133, 44614, 44457, 44461, 633, 54158, 20467, 20187, 31804, 53733, 7001, 32645, 2098, 703, 1776, 1943, 8680, 26688, 13218,
 6262, 23468, 23469, 6263, 23470, 23471, 5720, 23472, 23473, 5723, 23474, 23475, 11732, 23476, 23477, 27235, 27236, 27237,
 47874, 47873, 47872, 47875, 47876, 47877, 71838, 71839, 20006, 43125, 31024, 16614, 24131, 48503, 49760, 50454,
 53353, 53366, 53652, 53653, 53654, 58879, 60307, 60443, 60483, 64891, 67890, 69729, 69730, 69733, 69734, 28715, 43731,
 43733, 38616, 7712, 7714, 10577, 17484, 18798, 21992, 27655, 28788, 38395, 55756, 40972, 60526, 67760, 67714,
 13907, 64442, 44578, 44525, 28005, 46579, 20004, 20007, 13897, 64569, 6297);
INSERT INTO `spell_bonus_data`(`entry`,`direct_bonus`,`dot_bonus`,`ap_bonus`,`ap_dot_bonus`,`comments`) VALUES
 (58621, 0, 0, 0.08, 0,'Death Knight - Glyph of Chains of Ice'), -- Shouldn't scale with SP.
 (47632, 0, 0, 0.15, 0,'Death Knight - Death Coil'), -- Shouldn't scale with SP.
 (47633, 0, 0, 0.15, 0,'Death Knight - Death Coil (Heal)'), -- Shouldn't scale with SP.
 (50444, 0, 0, 0.105, 0,'Death Knight - Corpse Explosion (Triggered)'), -- Shouldn't scale with SP.
 (70890, 0, 0, 0, 0,'Death Knight - Scourge Strike (Shadow)'), -- Shouldn't scale with SP.
 (50526, 0, 0, 0, 0,'Death Knight - Wandering Plague (Triggered)'), -- Shouldn't scale with SP.
 (48982, 0, 0, 0, 0,'Death Knight - Rune Tap'), -- Shouldn't scale with SP.
 (59754, 0, 0, 0, 0,'Death Knight - Glyph of Rune Tap (Triggered)'), -- Shouldn't scale with SP.
 (49184, 0, 0, 0.2, 0,'Death Knight - Howling Blast'), -- Shouldn't scale with SP.
 -- Druid
 (33745, 0, 0, 0.01, 0.01,'Druid - Lacerate ($AP*0.05/number of ticks)'), -- Direct damage should scale with AP, based on DrDamage addon(3.3.5a).
 (9007, 0, 0, 0, 0.03,'Druid - Pounce Bleed (Triggered)'), -- Shouldn't scale with SP. Increase AP coefficient, based on DrDamage addon(3.3.5a) and Wowwiki.com.
 -- Hunter
 (56641, 0, 0, 0.1, 0,'Hunter - Steady Shot'), -- Shouldn't scale with SP.
 (42243, 0, 0, 0.0837, 0,'Hunter - Volley (Triggered)'), -- Shouldn't scale with SP.
 (3674, 0, 0, 0, 0.023,'Hunter - Black Arrow ($RAP*0.1 / number of ticks)'), -- Shouldn't scale with SP. Increase AP coefficient, based on DrDamage addon(3.3.5a).
 (19306, 0, 0, 0.2, 0,'Hunter - Counterattack'), -- Shouldn't scale with SP.
 (1495, 0, 0, 0.2, 0,'Hunter - Mongoose Bite'), -- Shouldn't scale with SP.
 (63468, 0, 0, 0, 0,'Hunter - Piercing Shots'), -- Shouldn't scale with SP.
 (136, 0, 0, 0, 0,'Hunter - Mend Pet'), -- Shouldn't scale with SP.
 -- Mage
 (133, 1, 0, 0, 0,'Mage - Fireball'), -- DoT componenet shouldn't scale.
 (44614, 0.8571, 0, 0, 0,'Mage - Frostfire Bolt'), -- DoT componenet shouldn't scale.
 (44457, 0, 0.2, 0, 0,'Mage - Living Bomb ($SP*0.8 / number of ticks)'), -- Direct damage bonus correction.
 (44461, 0.4286, 0, 0, 0,'Mage - Living Bomb (Triggered)'), -- Direct damage bonus correction.
 -- Paladin
 (633, 0, 0, 0, 0,'Paladin - Lay on Hands'), -- Shouldn't scale with SP.
 (54158, 0.27, 0, 0.175, 0,'Paladin - Judgement (Seal of Light, Seal of Wisdom, Seal of Justice)'), -- Increase coefficient, based on Wowwiki.com.
 (20467, 0.14, 0, 0.115, 0,'Paladin - Judgement of Command'), -- Decrease coefficient, based on Wowwiki.com.
 (20187, 0.355, 0, 0.225, 0,'Paladin - Judgement of Righteousness'), -- Increase coefficient, based on Wowwiki.com.
 (31804, 0.24, 0, 0.15, 0,'Paladin - Judgement of Vengeance'), -- Increase coefficient, based on Wowwiki.com.
 (53733, 0.24, 0, 0.15, 0,'Paladin - Judgement of Corruption'), -- Increase coefficient, based on Wowwiki.com.
 -- Priest
 (7001, 0, 0.626, 0, 0,'Priest - Lightwell Renew ($SP*1.878 / number of ticks)'), -- Increase coefficient, based on DrDamage addon(3.3.5a).
 -- Rogue
 (32645, 0, 0, 0, 0,'Rogue - Envenom'), -- Shouldn't scale with SP.
 (2098, 0, 0, 0, 0,'Rogue - Eviscerate'), -- Shouldn't scale with SP.
 (703, 0, 0, 0, 0.07,'Rogue - Garrote'), -- Shouldn't scale with SP.
 (1776, 0, 0, 0.21, 0,'Rogue - Gouge'), -- Shouldn't scale with SP.
 (1943, 0, 0, 0, 0,'Rogue - Rupture'), -- Shouldn't scale with SP.
 (8680, 0, 0, 0.1, 0,'Rogue - Instant Poison'), -- Shouldn't scale with SP.
 (26688, 0, 0, 0, 0,'Rogue - Anesthetic Poison'), -- Shouldn't scale with SP nor AP.
 (13218, 0, 0, 0.04, 0,'Rogue - Wound Poison'), -- Shouldn't scale with SP.
 -- Warlock Healthstones Shouldn't scale with SP.
 (6262, 0, 0, 0, 0,'Warlock - HS - Minor Healthstone (with 0/2 Improved Healthstone)'),
 (23468, 0, 0, 0, 0,'Warlock - HS - Minor Healthstone (with 1/2 Improved Healthstone)'),
 (23469, 0, 0, 0, 0,'Warlock - HS - Minor Healthstone (with 2/2 Improved Healthstone)'),
 (6263, 0, 0, 0, 0,'Warlock - HS - Lesser Healthstone (with 0/2 Improved Healthstone)'),
 (23470, 0, 0, 0, 0,'Warlock - HS - Lesser Healthstone (with 1/2 Improved Healthstone)'),
 (23471, 0, 0, 0, 0,'Warlock - HS - Lesser Healthstone (with 2/2 Improved Healthstone)'),
 (5720, 0, 0, 0, 0,'Warlock - HS - Healthstone (with 0/2 Improved Healthstone)'),
 (23472, 0, 0, 0, 0,'Warlock - HS - Healthstone (with 1/2 Improved Healthstone)'),
 (23473, 0, 0, 0, 0,'Warlock - HS - Healthstone (with 2/2 Improved Healthstone)'),
 (5723, 0, 0, 0, 0,'Warlock - HS - Greater Healthstone (with 0/2 Improved Healthstone)'),
 (23474, 0, 0, 0, 0,'Warlock - HS - Greater Healthstone (with 1/2 Improved Healthstone)'),
 (23475, 0, 0, 0, 0,'Warlock - HS - Greater Healthstone (with 2/2 Improved Healthstone)'),
 (11732, 0, 0, 0, 0,'Warlock - HS - Major Healthstone (with 0/2 Improved Healthstone)'),
 (23476, 0, 0, 0, 0,'Warlock - HS - Major Healthstone (with 1/2 Improved Healthstone)'),
 (23477, 0, 0, 0, 0,'Warlock - HS - Major Healthstone (with 2/2 Improved Healthstone)'),
 (27235, 0, 0, 0, 0,'Warlock - HS - Master Healthstone (with 0/2 Improved Healthstone)'),
 (27236, 0, 0, 0, 0,'Warlock - HS - Master Healthstone (with 1/2 Improved Healthstone)'),
 (27237, 0, 0, 0, 0,'Warlock - HS - Master Healthstone (with 2/2 Improved Healthstone)'),
 (47874, 0, 0, 0, 0,'Warlock - HS - Demonic Healthstone (with 0/2 Improved Healthstone)'),
 (47873, 0, 0, 0, 0,'Warlock - HS - Demonic Healthstone (with 1/2 Improved Healthstone)'),
 (47872, 0, 0, 0, 0,'Warlock - HS - Demonic Healthstone (with 2/2 Improved Healthstone)'),
 (47875, 0, 0, 0, 0,'Warlock - HS - Fel Healthstone (with 0/2 Improved Healthstone)'),
 (47876, 0, 0, 0, 0,'Warlock - HS - Fel Healthstone (with 1/2 Improved Healthstone)'),
 (47877, 0, 0, 0, 0,'Warlock - HS - Fel Healthstone (with 2/2 Improved Healthstone)'),
 -- Enchants
 (13907, 0, 0, -1, -1,'Demonslaying - Smite Demon'),
 (44578, 0, 0, -1, -1,'Lifeward - Lifeward'),
 (44525, 0, 0, -1, -1,'Icebreaker - Icebreaker'),
 (28005, 0, 0, -1, -1,'Battlemaster - Battlemaster'),
 (46579, 0, 0, -1, -1,'Deathfrost - Deathfrost'),
 (20004, 0, 0, -1, -1,'Lifestealing - Life Steal'),
 (20007, 0, 0, -1, -1,'Crusader - Holy Strength'),
 (64569, 0, 0, -1, -1,'Blood Draining - Blood Reserve'),
 (64442, 0, 0, -1, -1,'Blade Warding - Blade Warding'),
 (6297, 0, 0, -1, -1,'Fiery Blaze'),
 (13897, 0, 0, -1, -1,'Fiery Weapon - Fiery Weapon'),
 (7712, 0, 0, -1, -1,'Blazefury Medallion & Fiery Retributor (Fire Strike)'),
 (7714, 0, 0, -1, -1,'Fiery Plate Gauntlets (Fire Strike)'),
 (10577, 0, 0, -1, -1,'Gauntlets of the Sea (Heal)'),
 (17484, 0, 0, -1, -1,'Skullforge Reaver - Skullforge Brand'),
 (18798, 0, 0, -1, -1,'Freezing Band (Freeze)'),
 (21992, 0, 0, -1, -1,'Thunderfury'),
 (27655, 0, 0, -1, -1,'Heart of Wyrmthalak (Flame Lash)'),
 (28788, 0, 0, -1, -1,'Paladin T3 (8)'),
 (38395, 0, 0, -1, -1,'Warlock T5 (2)'),
 (55756, 0, 0, -1, -1,'Brunnhildar weapons (Chilling Blow)'),
 (40972, 0, 0, -1, -1,'Crystal Spire of Karabor - heal effect'),
 (60526, 0, 0, -1, -1,'Living Ice Crystals - heal effect'),
 (67760, 0, 0, -1, -1,'Pillar of Flame (Heroic)'),
 (67714, 0, 0, -1, -1,'Pillar of Flame (Normal)'),
 (28715, 0, 0, -1, -1,'Consumable - Flamecap (Flamecap Fire)'),
 (43731, 0, 0, -1, -1,'Consumable - Stormchops (Lightning Zap)'),
 (43733, 0, 0, -1, -1,'Consumable - Stormchops (Lightning Zap)'),
 (38616, 0, 0, -1, -1,'Poison - Bloodboil Poison'),
 (71838, 0, 0, 0, 0,'Drain Life proc - no scaling'),
 (71839, 0, 0, 0, 0,'Drain Life proc - no scaling'),
 (20006, 0, 0, 0, 0,'Unholy Curse proc - no scaling'),
 (43125, 0, 0, 0, 0,'Lifesteal proc - no scaling'),
 (31024, 0, 0, 0, 0,'Living Ruby Pendant proc'),
 (16614, 0, 0, 0, 0,'Storm Gauntlets proc'),
 (24131, 0, 0, 0, 0,'Hunter - Wyvern Sting'),
 (48503, 0, 0, 0, 0,'Druid - Living Seed'),
 (49760, 0, 0, 0, 0,'Rocket-Propelled Goblin Grenade'),
 (50454, 0, 0, 0, 0,'Bloodworms - Health Leech Heal'),
 (53353, 0, 0, 0, 0,'Hunter - Chimera Shot - Serpent'),
 (53366, 0, 0, 0, 0,'Hunter - Chimera Shot - Wyvern'),
 (53652, 0, 0, 0, 0,'Paladin - Beacon of Light heal'),
 (53653, 0, 0, 0, 0,'Paladin - Beacon of Light heal'),
 (53654, 0, 0, 0, 0,'Paladin - Beacon of Light heal'),
 (58879, 0, 0, 0, 0,'Shaman - Spirit Hunt (Feral Spirit)'),
 (60307, 0, 0, 0, 0,'Vestige of Haldor'),
 (60443, 0, 0, 0, 0,'Bandit\'s Insignia'),
 (60483, 0, 0, 0, 0,'Pendulum of Telluric Currents'),
 (64891, 0, 0, 0, 0,'Paladin T8 Holy 2P Bonus'),
 (67890, 0, 0, 0, 0,'Cobalt Frag Bomb'),
 (69729, 0, 0, 0, 0,'Searing Flames - Normal'),
 (69730, 0, 0, 0, 0,'Searing Flames - Heroic'),
 (69733, 0, 0, 0, 0,'Cauterizing Heal - Normal'),
 (69734, 0, 0, 0, 0,'Cauterizing Heal - Heroic');
DELETE FROM `spell_bonus_data` WHERE `entry` IN (16615, 44524);

DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (48707, 33786);
INSERT INTO `spell_linked_spell`(`spell_trigger`,`spell_effect`,`type`,`comment`) VALUES
 ('33786','-33786','2','Cyclone - Cyclone'),
 ('33786','-16857','2','Cyclone - Faerie Fire (Feral)'),
 ('33786','-770','2','Clyclone - Faerie fire'),
 ('48707','-33786','2','Anti-Magic Shell - Cyclone'),
 ('48707','-51514','2','Anti-Magic Shell - Hex'),
 ('48707','-3409','2','Anti-Magic Shell - Crippling Poison'),
 ('48707','-57970','2','Anti-Magic Shell - Deadly Posion IX'),
 ('48707','-57975','2','Anti-Magic Shell - Wound Poison VII'),
 ('48707','-55741','2','Anti-Magic Shell - Desecration R-1'),
 ('48707','-68766','2','Anti-Magic Shell - Desecration R-2'),
 ('48707','-13810','2','Anti-Magic Shell - Frost Trap');
-- Wrathful Gladiator's Dreadplate Gauntlets (i:51414) bonus: http://old.wowhead.com/spell=62459
UPDATE `spell_proc_event` SET `SchoolMask` = 16 WHERE `entry` = 62459;
-- Wrathful Gladiator's Dreadplate 4P bonus: http://old.wowhead.com/spell=61257
DELETE FROM `spell_proc_event` WHERE `entry` = 61257;
INSERT INTO `spell_proc_event` VALUES (61257, 127, 0, 0, 0, 0, 2237088, 3, 0, 0, 0);
-- -------------------------------------------------------------------------}
-- --------------------- Myth Project "Items" Update -----------------------
-- -------------------------------------------------------------------------{
-- Item: Black Bruise, http://old.wowhead.com/item=50035
-- Item: Black Bruise (Heroic), http://old.wowhead.com/item=50692
-- UNKNOWN SPELL: Necrotic Touch, http://old.wowhead.com/spell=71875
-- UNKNOWN SPELL: Necrotic Touch, http://old.wowhead.com/spell=71877
DELETE FROM `spell_bonus_data` WHERE `entry` IN (71875, 71877);
INSERT INTO `spell_bonus_data` VALUES
 (71875, -1, 0, -1, -1,''),
 (71877, -1, 0, -1, -1,'');
-- Item: Rocket Boots Xtreme, http://old.wowhead.com/item=23824
-- Item: Rocket Boots Xtreme Lite, http://old.wowhead.com/item=35581
-- UNKNOWN SPELL: Rocket Boots Engaged, http://old.wowhead.com/spell=51582
DELETE FROM `spell_script_names` WHERE `spell_id` IN (51582);
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES 
 (51582,'spell_item_rocket_boots');
-- -------------------------------------------------------------------------}
-- --------------- Myth Project "World" Database Cleaner -------------------
-- -------------------------------------------------------------------------{
-- NPC: Jeeves, http://old.wowhead.com/npc=35642
DELETE FROM `npc_vendor` WHERE `entry` = 35642 AND `slot` IN (15, 16, 17, 18, 19, 20);
INSERT INTO `npc_vendor`(`entry`,`slot`,`item`,`maxcount`,`incrtime`,`ExtendedCost`)
 VALUES
(35642, 15, 43235, 0, 0, 0),
(35642, 16, 5237, 0, 0, 0),
(35642, 17, 43231, 0, 0, 0),
(35642, 18, 43233, 0, 0, 0),
(35642, 19, 3775, 0, 0, 0),
(35642, 20, 43237, 0, 0, 0);
-- LOOT: GO: Cobalt Deposit, http://old.wowhead.com/object=189978
-- Item: Crystallized Earth, http://old.wowhead.com/item=37701
-- Item: Crystallized Water, http://old.wowhead.com/item=37705
UPDATE `gameobject_loot_template` SET `ChanceOrQuestChance` = 25 WHERE `entry` = 189978 AND `item` IN (37701, 37705);
-- LOOT: GO: Rich Cobalt Deposit, http://old.wowhead.com/object=189979
UPDATE `gameobject_loot_template` SET `ChanceOrQuestChance` = 40 WHERE `entry` = 189979 AND `item` IN (37701, 37705);
-- LOOT: GO: Frozen Herb, http://old.wowhead.com/object=190173
-- Item: Tiger Lily, http://old.wowhead.com/item=36904
-- Item: Talandra's Rose, http://old.wowhead.com/item=36907
UPDATE `gameobject_loot_template` SET `ChanceOrQuestChance` = 20 WHERE `entry` = 190173 AND `item` IN (36904, 36907);
UPDATE `trinity_string` 
 SET `content_loc1` = '',`content_loc2` = '',`content_loc3` = '',`content_loc4` = '',
     `content_loc5` = '',`content_loc6` = '',`content_loc7` = '',`content_loc8` = ''
 WHERE `entry` = 468;
-- TRIGGERS: LTEC TRIGGERS
UPDATE `creature_template` SET `flags_extra` = 128 WHERE `entry` IN (23845, 23850, 23854, 23855);
-- GO: Zul'Drak, normalize GO guids
DELETE FROM `gameobject` WHERE `id` IN(194625, 190446) AND `map` IN(603, 571) AND `guid` > 100000;
INSERT INTO `gameobject`(`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
 (106562,'194625','603','3','1','2001.18','9.409','245.24','0','0','0','0','0','60','100','1'),
 (106561,'194625','603','3','1','1941.61','-25.88','244.98','0','0','0','0','0','60','100','1'),
 (106560,'194625','603','3','1','2001.4','-59.66','245.07','0','0','0','0','0','60','100','1'),
 (106559,'190446','571','1','1','5974.2','-2341.7','293.839','3.07139','0','0','0.999384','0.0350946','300','0','1'),
 (106558,'190446','571','1','1','5973.59','-2351.48','293.839','3.07139','0','0','0.999384','0.0350946','300','0','1');
-- Creatures: normalize Creature guids
DELETE FROM `creature` WHERE `id` IN (20787, 23931, 4983);
INSERT INTO `creature`(`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
 (180551, 20787, 530, 1, 1, 0, 0, 3916.31, 2511.86, 105.199, 3.85911, 300, 0, 0, 6761, 0, 0, 0, 0, 0),
 (180550, 23931, 571, 1, 1, 0, 519, 775.951,-4724.76,-96.1438, 5.8571, 300, 0, 0, 15968, 0, 0, 0, 0, 0),
 (180549, 4983, 1, 1, 1, 0, 35,-3317.36,-3125.08, 32.8987, 2.92662, 300, 0, 0, 1342, 0, 0, 0, 0, 0);
 -- -------------------------------------------------------------------------
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
 `locales_item_set_name`;   -- DEPRICATED TABLE
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
-- -------------------------------------------------------------------------}
-- ------------------ Myth Project "Anti-Cheat" Update ---------------------
-- -------------------------------------------------------------------------{
DELETE FROM `warden_data_result` WHERE `id` = 437;
-- -------------------------------------------------------------------------}
-- ------------------ Myth Project Misc Update -----------------------------
-- -------------------------------------------------------------------------{
UPDATE `command` SET `security` = 3 WHERE `security` > 3;
UPDATE `command` SET `security` = 4 WHERE `name` IN ('character deleted delete','character deleted old','character erase',
'server togglequerylog','account set gmlevel','account set password','account set','account delete','account create');

UPDATE `trinity_string` SET `content_loc8`='Игрок выбрал DB GUID: %u. current GUID: %u. Faction: %u. npcFlags: %u. Entry: %u. DisplayID: %u (Native: %u).' WHERE `entry`='539';
DELETE FROM `command` WHERE `name` LIKE "pdump%";
DELETE FROM `command` WHERE `name` IN ('server set difftime','server set loglevel','server set logfilelevel','server exit');
INSERT INTO `command` VALUES
 ('server set difftime', 4, 'Syntax: .server set difftime #diff\r\n\r\nSet server difftime record interval to #diff ms.'),
 ('server set loglevel', 4, 'Syntax: .server set loglevel #lvl\r\n\r\nSet server common log level to #lvl (0-3).'),
 ('server set logfilelevel', 4, 'Syntax: .server set logfilelevel #lvl\r\n\r\nSet server specific logs level to #lvl (0-3).'),
 ('server exit', 4, 'Syntax: .server exit\r\n\r\nStop the server immediately.'),
 ('pdump load', 3,'Syntax: .pdump load $filename $account [$newname] [$newguid]\r\nLoad character dump from dump file into character list of $account with saved or $newname, with saved (or first free) or $newguid guid.'),
 ('pdump write', 3,'Syntax: .pdump write $filename $playerNameOrGUID\r\nWrite character dump with name/guid $playerNameOrGUID to file $filename.'),
 ('pdump', 3,'Syntax: .pdump $subcommand\nType .pdump to see the list of possible subcommands or .help pdump $subcommand to see info on subcommands');
-- -------------------------------------------------------------------------}
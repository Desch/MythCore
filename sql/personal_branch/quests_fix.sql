SET NAMES utf8;
-- http://ru.wowhead.com/quest=10985 A Distraction for Akama
DELETE FROM `creature` WHERE `guid` IN(77388,77152);
UPDATE `creature_template` SET `faction_A` = 1866, `faction_H` = 1866 WHERE `entry` IN (23191, 21701);
UPDATE `creature_template` SET scriptname = 'npc_xiri' WHERE entry = 18528;
UPDATE `creature_template` SET `modelid1` = 21349  , `modelid2` = 21349  , `minlevel` = 73  , `maxlevel` = 73, `mindmg` = 2000  , `maxdmg` = 3000  , `attackpower` = 16125  , `baseattacktime` = 2000  , `faction_A` = 1866  , `faction_H` = 1866, `equipment_id` = 2182 , scriptname = 'npc_preeven_maiev' WHERE `entry` = 22989;
UPDATE `creature_template` SET `attackpower` = 6540  , `speed_run` = 1.1 WHERE `entry` = 23191;
UPDATE `creature_template` SET `minlevel` = 72  , `maxlevel` = 72, `mindmg` = 3003  , `maxdmg` = 6139  , `attackpower` = 31997  , `baseattacktime` = 1400  , `rangeattacktime` = 1900  , `rangedattackpower` = 100  , `rank` = 3  , `faction_A` = 14  , `faction_H` = 14  , `type_flags` = 0, `speed_run` = 1.48  , `mechanic_immune_mask` = 617299803 WHERE `entry` = 23152;

-- http://ru.wowhead.com/quest=6124 & http://ru.wowhead.com/quest=6129 (Alliance doen't work...why???) 
DELETE FROM `spell_script_names` WHERE `spell_id`='19512';

-- Fix Grek for http://www.wowhead.com/quest=10201 (And Now, the Moment of Truth)
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_grek' WHERE `entry` = 19606;
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 19606;

-- Fix http://ru.wowhead.com/quest=10162
-- Quest: Mission: The Abyssal Shelf
UPDATE `quest_template` SET `StartScript`='10162' WHERE `entry` IN (10162);
-- StartScript fix
DELETE FROM `quest_start_scripts` WHERE `id`=10162;
INSERT INTO `quest_start_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(10162, 0, 7, 10162, 75, '0', 0, 0, 0, 0);

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

-- Fix http://old.wowhead.com/quest=5624
UPDATE `creature_template` SET `faction_A` = 11, `faction_H` = 11 WHERE `entry` = 12423;

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

-- fix quest Disclosure (Quest 12710)
UPDATE `gameobject_template` SET `ScriptName`='go_musty_coffin' WHERE `entry`='190949';

-- Hack http://www.wowhead.com/quest=13086 //Some f*cking bullshit is going with phases in this quest. Let's make it autocomplete
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 0, `ReqCreatureOrGOId2` = 0, `ReqCreatureOrGOCount1` = 0, `ReqCreatureOrGOCount2` = 0 WHERE `entry` = 13086;

-- Cold Heart
UPDATE `locales_creature` SET `subname_loc8`='' WHERE `entry`='29734';

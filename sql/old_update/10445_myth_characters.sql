ALTER TABLE `character_queststatus`
    ADD COLUMN `itemcount5` SMALLINT(5) DEFAULT '0' NOT NULL AFTER `itemcount4`;
ALTER TABLE `character_queststatus`
    ADD COLUMN `itemcount6` SMALLINT(5) DEFAULT '0' NOT NULL AFTER `itemcount5`;

/* -- 7567_trinity_1xxxx_characters.sql
ALTER TABLE `character_pet`
DROP COLUMN `resettalents_cost`,
DROP COLUMN `resettalents_time`;

ALTER TABLE `character_pet`
ADD KEY `idx_slot` (`slot`);
-- Uncomment this query if the ALTER query gives you primary key violation errors
-- SET @item_guid := (SELECT `item_guid` FROM `mail_items` GROUP BY `item_guid` HAVING COUNT(`item_guid`) > 1);
-- DELETE FROM `mail_items` WHERE `item_guid` IN (@item_guid);

ALTER TABLE `mail_items`
DROP PRIMARY KEY,
ADD PRIMARY KEY(`item_guid`),
ADD KEY `idx_mail_id` (`mail_id`);*/
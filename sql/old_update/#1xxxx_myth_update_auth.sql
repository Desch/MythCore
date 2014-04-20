-- -------------------------------------------------------------------------
-- ------------------ Myth Project "Login" Database Update -----------------
-- -------------------------------------------------------------------------
-- Myth Anticheat (Warden)
ALTER TABLE `account`
    ADD COLUMN `os` VARCHAR(4) DEFAULT '' NOT NULL AFTER `locale`;
-- Old Myth AC Table, NOT NEED NOW!
DROP TABLE IF EXISTS `warden_data_result`;
-- Possible CrashFIX
UPDATE `account` SET `online` = 0;
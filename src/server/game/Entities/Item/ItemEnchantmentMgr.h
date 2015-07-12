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

#ifndef _ITEM_ENCHANTMENT_MGR_H
#define _ITEM_ENCHANTMENT_MGR_H

#include "Common.h"

void LoadRandomEnchantmentsTable();
uint32 GetItemEnchantMod(int32 entry);
uint32 GenerateEnchSuffixFactor(uint32 item_id);
#endif

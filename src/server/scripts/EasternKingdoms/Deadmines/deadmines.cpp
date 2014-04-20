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

#include "ScriptPCH.h"
#include "deadmines.h"
#include "Spell.h"

/*#####
# item_Defias_Gunpowder
#####*/

class item_defias_gunpowder : public ItemScript
{
public:
    item_defias_gunpowder() : ItemScript("item_defias_gunpowder") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& targets)
    {
        InstanceScript* pInstance = player->GetInstanceScript();

        if(!pInstance)
        {
            player->GetSession()->SendNotification("Instance script not initialized");
            return true;
        }

        if(pInstance->GetData(EVENT_STATE)!= CANNON_NOT_USED)
            return false;
        if(targets.GetGOTarget() && targets.GetGOTarget()->GetEntry() == GO_DEFIAS_CANNON)
            pInstance->SetData(EVENT_STATE, CANNON_GUNPOWDER_USED);

        player->DestroyItemCount(item->GetEntry(), 1, true);
        return true;
    }
};

void AddSC_deadmines()
{
    new item_defias_gunpowder;
}

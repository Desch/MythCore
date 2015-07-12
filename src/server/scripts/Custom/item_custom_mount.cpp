/* ScriptData
SDName: item_custom_mount
SD%Complete: 100
SDComment: Item that gives an temporary mount
SDCategory: Items
EndScriptData */

#include "ScriptPCH.h"
#define MOUNT_SPELL_1               58997
#define MOUNT_SPELL_2               58999

class ItemUse_item_custom_mount : public ItemScript
{
    public:
    ItemUse_item_custom_mount() : ItemScript("item_custom_mount") { }

	bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*pTargets*/)
  	{
        if(pPlayer->getLevel()>60)
        {
            pPlayer->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, pItem, NULL);
            return false;
        }

    	if((pPlayer->isInCombat()) || (pPlayer->isInFlight()) || (pPlayer->isDead()) || (pPlayer->IsMounted()))
    	{
            pPlayer->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, pItem, NULL);
            return false;
    	}

        if(pPlayer->getLevel()>30)
        {
        pPlayer->AddAura(MOUNT_SPELL_2, pPlayer);
        pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true, false);
        return true;
        }

        if(pPlayer->getLevel()<30)
        {
        pPlayer->AddAura(MOUNT_SPELL_1, pPlayer);
        pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true, false);
        return true;
        }
	}
};

void AddSC_ItemUse_item_custom_mount()
{
    new ItemUse_item_custom_mount();
}
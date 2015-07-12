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

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Chat.h"

class quest_commandscript : public CommandScript
{
public:
    quest_commandscript() : CommandScript("quest_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand questCommandTable[] =
        {
            { "add",            SEC_ADMINISTRATOR,  false, &HandleQuestAdd,                    "", NULL },
            { "complete",       SEC_ADMINISTRATOR,  false, &HandleQuestComplete,               "", NULL },
            { "remove",         SEC_ADMINISTRATOR,  false, &HandleQuestRemove,                 "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "quest",          SEC_ADMINISTRATOR,  false, NULL,                  "", questCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleQuestAdd(ChatHandler* handler, const char* args)
    {
        Player* player = handler->getSelectedPlayer();
        if(!player)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // .addquest #entry'
        // number or [name] Shift-click form |color|Hquest:quest_id:quest_level|h[name]|h|r
        char* cId = handler->extractKeyFromLink((char*)args, "Hquest");
        if(!cId)
            return false;

        uint32 entry = atol(cId);

        Quest const* pQuest = sObjectMgr->GetQuestTemplate(entry);

        if(!pQuest)
        {
            handler->PSendSysMessage(LANG_COMMAND_QUEST_NOTFOUND, entry);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check item starting quest (it can work incorrectly if added without item in inventory)
        ItemTemplateContainer const* itc = sObjectMgr->GetItemTemplateStore();
        ItemTemplateContainer::const_iterator result = find_if(itc->begin(), itc->end(), Finder<uint32, ItemTemplate>(entry, &ItemTemplate::StartQuest));

        if(result != itc->end())
        {
            handler->PSendSysMessage(LANG_COMMAND_QUEST_STARTFROMITEM, entry, result->second.ItemId);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // ok, normal (creature/GO starting) quest
        if(player->CanAddQuest(pQuest, true))
        {
            player->AddQuest(pQuest, NULL);

            if(player->CanCompleteQuest(entry))
                player->CompleteQuest(entry);
        }

        return true;
    }

    static bool HandleQuestRemove(ChatHandler* handler, const char* args)
    {
        Player* player = handler->getSelectedPlayer();
        if(!player)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // .removequest #entry'
        // number or [name] Shift-click form |color|Hquest:quest_id:quest_level|h[name]|h|r
        char* cId = handler->extractKeyFromLink((char*)args, "Hquest");
        if(!cId)
            return false;

        uint32 entry = atol(cId);

        Quest const* pQuest = sObjectMgr->GetQuestTemplate(entry);

        if(!pQuest)
        {
            handler->PSendSysMessage(LANG_COMMAND_QUEST_NOTFOUND, entry);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // remove all quest entries for 'entry' from quest log
        for(uint8 slot = 0; slot < MAX_QUEST_LOG_SIZE; ++slot)
        {
            uint32 quest = player->GetQuestSlotQuestId(slot);
            if(quest == entry)
            {
                player->SetQuestSlot(slot, 0);

                // we ignore unequippable quest items in this case, its' still be equipped
                player->TakeQuestSourceItem(quest, false);
            }
        }

        player->RemoveActiveQuest(entry);
        player->RemoveRewardedQuest(entry);

        handler->SendSysMessage(LANG_COMMAND_QUEST_REMOVED);
        return true;
    }

    static bool HandleQuestComplete(ChatHandler* handler, const char* args)
    {
        Player* player = handler->getSelectedPlayer();
        if(!player)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // .quest complete #entry
        // number or [name] Shift-click form |color|Hquest:quest_id:quest_level|h[name]|h|r
        char* cId = handler->extractKeyFromLink((char*)args, "Hquest");
        if(!cId)
            return false;

        uint32 entry = atol(cId);

        Quest const* pQuest = sObjectMgr->GetQuestTemplate(entry);

        // If player doesn't have the quest
        if(!pQuest || player->GetQuestStatus(entry) == QUEST_STATUS_NONE)
        {
            handler->PSendSysMessage(LANG_COMMAND_QUEST_NOTFOUND, entry);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // Add quest items for quests that require items
        for(uint8 x = 0; x < QUEST_ITEM_OBJECTIVES_COUNT; ++x)
        {
            uint32 id = pQuest->ReqItemId[x];
            uint32 count = pQuest->ReqItemCount[x];
            if(!id || !count)
                continue;

            uint32 curItemCount = player->GetItemCount(id, true);

            ItemPosCountVec dest;
            uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, id, count-curItemCount);
            if(msg == EQUIP_ERR_OK)
            {
                Item* item = player->StoreNewItem(dest, id, true);
                player->SendNewItem(item, count-curItemCount, true, false);
            }
        }

        // All creature/GO slain/casted (not required, but otherwise it will display "Creature slain 0/10")
        for(uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        {
            int32 creature = pQuest->ReqCreatureOrGOId[i];
            uint32 creaturecount = pQuest->ReqCreatureOrGOCount[i];

            if(uint32 spell_id = pQuest->ReqSpell[i])
            {
                for(uint16 z = 0; z < creaturecount; ++z)
                    player->CastedCreatureOrGO(creature, 0, spell_id);
            }
            else if(creature > 0)
            {
                if(CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(creature))
                    for(uint16 z = 0; z < creaturecount; ++z)
                        player->KilledMonster(cInfo, 0);
            }
            else if(creature < 0)
            {
                for(uint16 z = 0; z < creaturecount; ++z)
                    player->CastedCreatureOrGO(creature, 0, 0);
            }
        }

        // If the quest requires reputation to complete
        if(uint32 repFaction = pQuest->GetRepObjectiveFaction())
        {
            uint32 repValue = pQuest->GetRepObjectiveValue();
            uint32 curRep = player->GetReputationMgr().GetReputation(repFaction);
            if(curRep < repValue)
                if(FactionEntry const *factionEntry = sFactionStore.LookupEntry(repFaction))
                    player->GetReputationMgr().SetReputation(factionEntry, repValue);
        }

        // If the quest requires a SECOND reputation to complete
        if(uint32 repFaction = pQuest->GetRepObjectiveFaction2())
        {
            uint32 repValue2 = pQuest->GetRepObjectiveValue2();
            uint32 curRep = player->GetReputationMgr().GetReputation(repFaction);
            if(curRep < repValue2)
                if(FactionEntry const *factionEntry = sFactionStore.LookupEntry(repFaction))
                    player->GetReputationMgr().SetReputation(factionEntry, repValue2);
        }

        // If the quest requires money
        int32 ReqOrRewMoney = pQuest->GetRewOrReqMoney();
        if(ReqOrRewMoney < 0)
            player->ModifyMoney(-ReqOrRewMoney);

        player->CompleteQuest(entry);
        return true;
    }
};

void AddSC_quest_commandscript()
{
    new quest_commandscript;
}

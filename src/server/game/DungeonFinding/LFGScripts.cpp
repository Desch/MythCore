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

#include "Common.h"
#include "SharedDefines.h"
#include "Player.h"
#include "Group.h"
#include "ScriptPCH.h"
#include "LFGScripts.h"
#include "LFGMgr.h"

LFGScripts::LFGScripts(): GroupScript("LFGScripts"), PlayerScript("LFGScripts") { }

void LFGScripts::OnAddMember(Group* group, uint64 guid)
{
    uint64 gguid = group->GetGUID();
    if(!gguid)
        return;

    sLog->outDebug(LOG_FILTER_LFG, "LFGScripts::OnAddMember [" UI64FMTD "]: added [" UI64FMTD "]", gguid, guid);
    LfgUpdateData updateData = LfgUpdateData(LFG_UPDATETYPE_CLEAR_LOCK_LIST);
    for(GroupReference *itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        if(Player* plrg = itr->getSource())
        {
            plrg->GetSession()->SendLfgUpdatePlayer(updateData);
            plrg->GetSession()->SendLfgUpdateParty(updateData);
        }
    }

    // TODO - if group is queued and new player is added convert to rolecheck without notify the current players queued
    if(sLFGMgr->GetState(gguid) == LFG_STATE_QUEUED)
        sLFGMgr->Leave(NULL, group);

    if(sLFGMgr->GetState(guid) == LFG_STATE_QUEUED)
        if(Player* plr = sObjectMgr->GetPlayer(guid))
            sLFGMgr->Leave(plr);
}

void LFGScripts::OnRemoveMember(Group* group, uint64 guid, RemoveMethod& method, uint64 kicker, const char* reason)
{
    uint64 gguid = group->GetGUID();
    if(!gguid || method == GROUP_REMOVEMETHOD_DEFAULT)
        return;

    sLog->outDebug(LOG_FILTER_LFG, "LFGScripts::OnRemoveMember [" UI64FMTD "]: remove [" UI64FMTD "] Method: %d Kicker: [" UI64FMTD "] Reason: %s", gguid, guid, method, kicker, (reason ? reason : ""));
    if(sLFGMgr->GetState(gguid) == LFG_STATE_QUEUED)
    {
        // TODO - Do not remove, just remove the one leaving and rejoin queue with all other data
        sLFGMgr->Leave(NULL, group);
    }

    if(!group->isLFGGroup())
        return;

    if(method == GROUP_REMOVEMETHOD_KICK)                 // Player have been kicked
    {
        // TODO - Update internal kick cooldown of kicker
        std::string str_reason = "";
        if(reason)
            str_reason = std::string(reason);
        sLFGMgr->InitBoot(group, kicker, guid, str_reason);
        return;
    }

    sLFGMgr->ClearState(guid);
    if(Player* plr = sObjectMgr->GetPlayer(guid))
    {
        // Add deserter flag
        if(method == GROUP_REMOVEMETHOD_LEAVE && plr->GetMap()->IsDungeon() && sLFGMgr->GetState(gguid) != LFG_STATE_FINISHED_DUNGEON)
            plr->CastSpell(plr, LFG_SPELL_DUNGEON_DESERTER, true);

        LfgUpdateData updateData = LfgUpdateData(LFG_UPDATETYPE_LEADER);
        plr->GetSession()->SendLfgUpdateParty(updateData);
        if(plr->GetMap()->IsDungeon())                    // Teleport player out the dungeon
            sLFGMgr->TeleportPlayer(plr, true);
    }

    if(sLFGMgr->GetState(gguid) != LFG_STATE_FINISHED_DUNGEON)// Need more players to finish the dungeon
        sLFGMgr->OfferContinue(group);
}

void LFGScripts::OnDisband(Group* group)
{
    uint64 gguid = group->GetGUID();
    sLog->outDebug(LOG_FILTER_LFG, "LFGScripts::OnDisband [" UI64FMTD "]", gguid);

    sLFGMgr->RemoveGroupData(gguid);
}

void LFGScripts::OnChangeLeader(Group* group, uint64 newLeaderGuid, uint64 oldLeaderGuid)
{
    uint64 gguid = group->GetGUID();
    if(!gguid)
        return;

    sLog->outDebug(LOG_FILTER_LFG, "LFGScripts::OnChangeLeader [" UI64FMTD "]: old [" UI64FMTD "] new [" UI64FMTD "]", gguid, newLeaderGuid, oldLeaderGuid);
    Player* plr = sObjectMgr->GetPlayer(newLeaderGuid);

    LfgUpdateData updateData = LfgUpdateData(LFG_UPDATETYPE_LEADER);
    if(plr)
        plr->GetSession()->SendLfgUpdateParty(updateData);

    plr = sObjectMgr->GetPlayer(oldLeaderGuid);
    if(plr)
    {
        updateData.updateType = LFG_UPDATETYPE_GROUP_DISBAND;
        plr->GetSession()->SendLfgUpdateParty(updateData);
    }
}

void LFGScripts::OnInviteMember(Group* group, uint64 guid)
{
    uint64 gguid = group->GetGUID();
    if(!gguid)
        return;

    sLog->outDebug(LOG_FILTER_LFG, "LFGScripts::OnInviteMember [" UI64FMTD "]: invite [" UI64FMTD "] leader [" UI64FMTD "]", gguid, guid, group->GetLeaderGUID());
    sLFGMgr->Leave(NULL, group);
}

void LFGScripts::OnLevelChanged(Player* player, uint8 /*oldLevel*/)
{
    sLFGMgr->InitializeLockedDungeons(player);
}

void LFGScripts::OnLogout(Player* player)
{
    sLFGMgr->Leave(player);
    LfgUpdateData updateData = LfgUpdateData(LFG_UPDATETYPE_REMOVED_FROM_QUEUE);
    player->GetSession()->SendLfgUpdateParty(updateData);
    player->GetSession()->SendLfgUpdatePlayer(updateData);
    player->GetSession()->SendLfgUpdateSearch(false);
    uint64 guid = player->GetGUID();
    // TODO - Do not remove, add timer before deleting
    sLFGMgr->RemovePlayerData(guid);
}

void LFGScripts::OnLogin(Player* player)
{
    sLFGMgr->InitializeLockedDungeons(player);
    // TODO - Restore LfgPlayerData and send proper status to player if it was in a group
}

void LFGScripts::OnBindToInstance(Player* player, Difficulty difficulty, uint32 mapId, bool /*permanent*/)
{
    MapEntry const* mapEntry = sMapStore.LookupEntry(mapId);
    if(mapEntry->IsDungeon() && difficulty > DUNGEON_DIFFICULTY_NORMAL)
        sLFGMgr->InitializeLockedDungeons(player);
}

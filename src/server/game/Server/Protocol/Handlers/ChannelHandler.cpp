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

#include "ObjectMgr.h"                                      // for normalizePlayerName
#include "ChannelMgr.h"

void WorldSession::HandleJoinChannel(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());

    uint32 channel_id;
    uint8 unknown1, unknown2;
    std::string channelname, pass;

    recvPacket >> channel_id;

    if(channel_id)
    {
        ChatChannelsEntry const* channel = sChatChannelsStore.LookupEntry(channel_id);
        if(!channel)
            return;

        AreaTableEntry const* current_zone = GetAreaEntryByAreaID(_player->GetZoneId());
        if(!current_zone)
            return;

        if(!_player->CanJoinConstantChannelInZone(channel, current_zone))
            return;
    }

    recvPacket >> unknown1 >> unknown2;
    recvPacket >> channelname;

    if(channelname.empty())
        return;

    recvPacket >> pass;
    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
    {
        cMgr->team = _player->GetTeam();
        if(Channel *chn = cMgr->GetJoinChannel(channelname, channel_id))
            chn->Join(_player->GetGUID(), pass.c_str());
    }
}

void WorldSession::HandleLeaveChannel(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();

    uint32 unk;
    std::string channelname;
    recvPacket >> unk;                                      // channel id?
    recvPacket >> channelname;

    if(channelname.empty())
        return;

    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
    {
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->Leave(_player->GetGUID(), true);
        cMgr->LeftChannel(channelname);
    }
}

void WorldSession::HandleChannelList(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();
    std::string channelname;
    recvPacket >> channelname;

    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->List(_player);
}

void WorldSession::HandleChannelPassword(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();
    std::string channelname, pass;
    recvPacket >> channelname;

    recvPacket >> pass;

    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->Password(_player->GetGUID(), pass.c_str());
}

void WorldSession::HandleChannelSetOwner(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();
    std::string channelname, newp;
    recvPacket >> channelname;

    recvPacket >> newp;

    if(!normalizePlayerName(newp))
        return;

    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->SetOwner(_player->GetGUID(), newp.c_str());
}

void WorldSession::HandleChannelOwner(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();
    std::string channelname;
    recvPacket >> channelname;
    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->SendWhoOwner(_player->GetGUID());
}

void WorldSession::HandleChannelModerator(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();
    std::string channelname, otp;
    recvPacket >> channelname;

    recvPacket >> otp;

    if(!normalizePlayerName(otp))
        return;

    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->SetModerator(_player->GetGUID(), otp.c_str());
}

void WorldSession::HandleChannelUnmoderator(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();
    std::string channelname, otp;
    recvPacket >> channelname;

    recvPacket >> otp;

    if(!normalizePlayerName(otp))
        return;

    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->UnsetModerator(_player->GetGUID(), otp.c_str());
}

void WorldSession::HandleChannelMute(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();
    std::string channelname, otp;
    recvPacket >> channelname;

    recvPacket >> otp;

    if(!normalizePlayerName(otp))
        return;

    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->SetMute(_player->GetGUID(), otp.c_str());
}

void WorldSession::HandleChannelUnmute(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();

    std::string channelname, otp;
    recvPacket >> channelname;

    recvPacket >> otp;

    if(!normalizePlayerName(otp))
        return;

    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->UnsetMute(_player->GetGUID(), otp.c_str());
}

void WorldSession::HandleChannelInvite(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();
    std::string channelname, otp;
    recvPacket >> channelname;

    recvPacket >> otp;

    if(!normalizePlayerName(otp))
        return;

    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->Invite(_player->GetGUID(), otp.c_str());
}

void WorldSession::HandleChannelKick(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();
    std::string channelname, otp;
    recvPacket >> channelname;

    recvPacket >> otp;
    if(!normalizePlayerName(otp))
        return;

    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->Kick(_player->GetGUID(), otp.c_str());
}

void WorldSession::HandleChannelBan(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();
    std::string channelname, otp;
    recvPacket >> channelname;

    recvPacket >> otp;

    if(!normalizePlayerName(otp))
        return;

    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->Ban(_player->GetGUID(), otp.c_str());
}

void WorldSession::HandleChannelUnban(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();

    std::string channelname, otp;
    recvPacket >> channelname;

    recvPacket >> otp;

    if(!normalizePlayerName(otp))
        return;

    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->UnBan(_player->GetGUID(), otp.c_str());
}

void WorldSession::HandleChannelAnnouncements(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();
    std::string channelname;
    recvPacket >> channelname;
    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->Announce(_player->GetGUID());
}

void WorldSession::HandleChannelDisplayListQuery(WorldPacket &recvPacket)
{
    // this should be OK because the 2 function _were_ the same
    HandleChannelList(recvPacket);
}

void WorldSession::HandleGetChannelMemberCount(WorldPacket &recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();
    std::string channelname;
    recvPacket >> channelname;
    if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
    {
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
        {
            WorldPacket data(SMSG_CHANNEL_MEMBER_COUNT, chn->GetName().size()+1+1+4);
            data << chn->GetName();
            data << uint8(chn->GetFlags());
            data << uint32(chn->GetNumPlayers());
            SendPacket(&data);
        }
    }
}

void WorldSession::HandleSetChannelWatch(WorldPacket &recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    //recvPacket.hexlike();
    std::string channelname;
    recvPacket >> channelname;
    /*if(ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if(Channel *chn = cMgr->GetChannel(channelname, _player))
            chn->JoinNotify(_player->GetGUID());*/
}


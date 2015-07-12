/*
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * Copyright (C) 2008-2010 Trinity <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Common.h"
#include "ChatLexicsCutter.h"
#include "ChatLog.h"
#include "Chat.h"
#include "Group.h"
#include "Guild.h"
#include "Channel.h"
#include "ObjectMgr.h"
#include "SpellAuras.h"
#include "Config.h"

ChatLogInfo::ChatLogInfo(ChatLogType type, bool chat, bool lexics, uint32 flushLength) : 
    _file(NULL), _screenFlag(false), _cutFlag(false), _flushLength(flushLength), _type(type), _writtenLength(0)
{
    std::string strType = ChatLog::GetChatNameByType(type);
    if (chat)
    {
        _name = sConfig->GetStringDefault(std::string("ChatLog." + strType + ".File").c_str(), "");
        _screenFlag = sConfig->GetBoolDefault(std::string("ChatLog." + strType + ".Screen").c_str(), false);
    }

    if (lexics)
        _cutFlag = sConfig->GetBoolDefault(std::string("ChatLog.Lexics." + strType + ".Cut").c_str(), true);
}

void ChatLogInfo::OpenFile(bool dateSplit, const std::string& date, bool utfHeader)
{
    if (!_name.empty() && !_file)
    {
        std::string tmp(_name);
        if (dateSplit)
        {
            // Replace $d with date value if applicable
            int dpos = tmp.find("$d");
            if (dpos != tmp.npos)
                tmp.replace(dpos, 2, date.c_str(), date.size());
        }
        _file = fopen(tmp.c_str(), "a+b");
        if (_file)
        {
            if (utfHeader && ftell(_file) == 0)
                fputs("\xEF\xBB\xBF", _file);

            std::string s("[SYSTEM] " + ChatLog::GetChatDescByType(_type) + " Log Initialized\n");
            WriteFile(s);
        }
    }
}

void ChatLogInfo::WriteFile(const std::string& msg)
{
    ACE_Guard<ACE_Thread_Mutex> guard(_lock);
    if (_file)
    {
        ChatLog::OutTimestamp(_file);
        _writtenLength += fprintf(_file, "%s\n", msg.c_str());
        if (_writtenLength >= _flushLength)
        {
            fflush(_file);
            _writtenLength = 0;
        }
    }
}

std::string ChatLog::GetChatNameByType(ChatLogType type)
{
    switch (type)
    {
        case CHAT_LOG_CHAT: return "Chat";
        case CHAT_LOG_PARTY: return "Party";
        case CHAT_LOG_GUILD: return "Guild";
        case CHAT_LOG_WHISPER: return "Whisper";
        case CHAT_LOG_CHANNEL: return "Channel";
        case CHAT_LOG_RAID: return "Raid";
        case CHAT_LOG_BATTLEGROUND: return "BattleGround";
        case CHAT_LOG_INNORMATIVE: return "Lexics.Innormative";
        default: return "Unknown";
    }
}

std::string ChatLog::GetChatDescByType(ChatLogType type)
{
    switch (type)
    {
        case CHAT_LOG_CHAT: return "Chat";
        case CHAT_LOG_PARTY: return "Party Chat";
        case CHAT_LOG_GUILD: return "Guild Chat";
        case CHAT_LOG_WHISPER: return "Whisper";
        case CHAT_LOG_CHANNEL: return "Channels";
        case CHAT_LOG_RAID: return "Raid Chat";
        case CHAT_LOG_BATTLEGROUND: return "Battleground Chat";
        case CHAT_LOG_INNORMATIVE: return "Lexics Innormative";
        default: return "Unknown";
    }
}

void ChatLog::OutTimestamp(FILE* file)
{
    time_t t = time(NULL);
    tm* aTm = localtime(&t);
    fprintf(file, "%-4d-%02d-%02d %02d:%02d:%02d ", aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
}

ChatLog::ChatLog() : PlayerScript("LexicsChatLog"), _lexics(NULL), _innormativeLog(NULL)
{
    _Initialize();
}

ChatLog::~ChatLog()
{
    // Close all files (avoiding double-close)
    _CloseAllFiles();

    if (_lexics)
    {
        delete _lexics;
        _lexics = NULL;
    }
    for (uint32 i = CHAT_LOG_CHAT; i < CHAT_LOG_COUNT; ++i)
        delete _logs[i];
}

void ChatLog::_Initialize()
{
    // Load config settings
    _enable = sConfig->GetBoolDefault("ChatLog.Enable", true);
    _dateSplit = sConfig->GetBoolDefault("ChatLog.DateSplit", true);
    _utfHeader = sConfig->GetBoolDefault("ChatLog.UTFHeader", true);
    _ignoreUnprintable = sConfig->GetBoolDefault("ChatLog.Ignore.Unprintable", true);
    _flushLength = sConfig->GetIntDefault("ChatLog.FlushAfterNumberOfBytes", 1024); // 1 kB

    _lexicsEnable = sConfig->GetBoolDefault("ChatLog.Lexics.Enable", true);
    if (_lexicsEnable)
    {
        std::string analogsFileName = sConfig->GetStringDefault("ChatLog.Lexics.AnalogsFile", "");
        std::string innormativeWordsFileName = sConfig->GetStringDefault("ChatLog.Lexics.WordsFile", "");

        _innormativeLog = new ChatLogInfo(CHAT_LOG_INNORMATIVE, true, false, 0);
        if (analogsFileName.empty() || innormativeWordsFileName.empty())
            _lexicsEnable = false;
        else
        {
            // Initialize lexics cutter parameters
            _lexicsInnormativeCut = sConfig->GetBoolDefault("ChatLog.Lexics.Cut.Enable", true);
            _lexicsCutReplacement = sConfig->GetStringDefault("ChatLog.Lexics.Cut.Replacement", "&!@^%!^&*!!!");
            _lexicsAction = LexicsActions(sConfig->GetIntDefault("ChatLog.Lexics.Action", LEXICS_ACTION_LOG));
            _lexicsActionDuration = sConfig->GetIntDefault("ChatLog.Lexics.Action.Duration", 0);

            // Initialize lexics cutter object
            _lexics = new LexicsCutter(analogsFileName, innormativeWordsFileName,
                sConfig->GetBoolDefault("ChatLog.Lexics.Ignore.Spaces", true),
                sConfig->GetBoolDefault("ChatLog.Lexics.Ignore.Repeats", true));

            // Read additional parameters
            _lexicsIgnoreGm = sConfig->GetBoolDefault("ChatLog.Lexics.Ignore.GM", true);
        }
    }

    for (uint32 i = CHAT_LOG_CHAT; i < CHAT_LOG_COUNT; ++i)
        _logs[i] = new ChatLogInfo(ChatLogType(i), _enable, _lexicsEnable, _flushLength);

    _OpenAllFiles();
}

void ChatLog::_OpenAllFiles()
{
    ACE_Guard<ACE_Thread_Mutex> guard(_lock);
    std::string date;
    if (_dateSplit)
    {
        time_t t = time(NULL);
        tm* aTm = localtime(&t);
        char szDate[12];
        sprintf(szDate, "%-4d-%02d-%02d", aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday);
        date = szDate;

        _lastDay = aTm->tm_mday;
    }

    if (_enable)
    {
        for (uint32 i = CHAT_LOG_CHAT; i <= CHAT_LOG_COUNT - 1; ++i)
        {
            for (uint32 j = i - 1; j >= CHAT_LOG_CHAT; --j)
                if (_logs[i]->SetFileIfSame(_logs[j]))
                    break;
            _logs[i]->OpenFile(_dateSplit, date, _utfHeader);
        }
    }

    // Initialize innormative log
    if (_lexicsEnable && _innormativeLog)
        _innormativeLog->OpenFile(_dateSplit, date, _utfHeader);
}

void ChatLog::_CloseAllFiles()
{
    ACE_Guard<ACE_Thread_Mutex> guard(_lock);
    for (uint32 i = CHAT_LOG_CHAT; i <= CHAT_LOG_COUNT - 1; ++i)
    {
        if (_logs[i]->GetFile())
        {
            for (uint32 j = i + 1; j <= CHAT_LOG_COUNT - 1; ++j)
                _logs[j]->CloseFileIfSame(_logs[i]);
            _logs[i]->CloseFile();
        }
    }

    if (_innormativeLog)
        _innormativeLog->CloseFile();
}

void ChatLog::_CheckDateSwitch()
{
    if (_dateSplit)
    {
        time_t t = time(NULL);
        tm* aTm = localtime(&t);
        if (_lastDay != aTm->tm_mday)
        {
            // Open new files for new date
            _CloseAllFiles();
            _OpenAllFiles();
        }
    }
}

bool ChatLog::_ChatCommon(ChatLogType type, Player* player, std::string& msg)
{
    // Check message for innormative lexics and punish if necessary.
    if (_lexicsEnable && _lexics && _logs[type]->IsCut() && _lexics->CheckLexics(msg)) 
        _Punish(player, msg);

    if (!_enable)
        return false;

    if (_ignoreUnprintable)
    {
        // If should ignore unprintables, verify string by UTF8 here
        unsigned int pos = 0;
        std::string ch;
        while (LexicsCutter::ReadUTF8(msg, ch, pos))
            if (ch.size() == 1)
                if (ch[0] < ' ')
                    return false; // Unprintable detected
    }
    return true;
}

void ChatLog::_Punish(Player* player, std::string& msg)
{
    std::string logStr;

    _AppendPlayerName(player, logStr);
    _WriteLog(_innormativeLog, logStr, msg, msg);

    // Check if should ignore GM
    if (_lexicsIgnoreGm && (player->GetSession()->GetSecurity() > SEC_PLAYER))
        return;

    // Cut innormative lexics
    if (_lexicsInnormativeCut)
        msg = _lexicsCutReplacement;

    if (!player || !player->GetSession())
        return;

    // special action
    switch (_lexicsAction)
    {
        case LEXICS_ACTION_SHEEP:       _ApplySpell(player, 118);   break;
        case LEXICS_ACTION_STUN:        _ApplySpell(player, 13005); break;
        case LEXICS_ACTION_STUCK:       _ApplySpell(player, 23312); break;
        case LEXICS_ACTION_SICKNESS:    _ApplySpell(player, 15007); break;
        case LEXICS_ACTION_SHEAR:       _ApplySpell(player, 41032); break;
        case LEXICS_ACTION_DIE:
            player->DealDamage(player, player->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            break;
        case LEXICS_ACTION_DRAIN:
            player->DealDamage(player, player->GetHealth() - 5, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            break;
        case LEXICS_ACTION_SILENCE:
            player->GetSession()->m_muteTime = time(NULL) + int64(_lexicsActionDuration / 1000);
            break;
        default:
            // No action except logging
            break;
    }
}

inline void ChatLog::_ApplySpell(Player *player, uint32 spellId)
{
    if (Aura *a = player->AddAura(spellId, player))
        a->SetDuration(_lexicsActionDuration);
}

inline void ChatLog::_WriteLog(ChatLogInfo* log, std::string& logStr, const std::string& msg, const std::string& origMsg)
{
    if (!log)
        return;

    if (log->IsScreen())
        printf("%s %s", logStr.c_str(), msg.c_str());

    _CheckDateSwitch();
    logStr.append(" ").append(origMsg);
    log->WriteFile(logStr);
}

inline void ChatLog::_AppendPlayerName(Player* player, std::string& s)
{
    s.append("[").append(player ? player->GetName() : "???").append("]");
}

inline void ChatLog::_AppendGroupMembers(Group* group, std::string& s)
{
    if (!group)
        s.append(" {unknown group}:");
    else
    {
        char sz[32];
        sprintf(sz, UI64FMTD, group->GetGUID());
        s.append(" {").append(sz).append("} [");
        const uint64& leaderGuid = group->GetLeaderGUID();
        if (Player* leader = sObjectMgr->GetPlayer(leaderGuid))
            s.append(leader->GetName());

        Group::MemberSlotList members = group->GetMemberSlots();
        for (Group::member_citerator itr = members.begin(); itr != members.end(); ++itr)
        {
            if (itr->guid == leaderGuid)
                continue;

            if (Player* member = sObjectMgr->GetPlayer(itr->guid))
                s.append(",").append(member->GetName());
        }
        s.append("]:");
    }
}

void ChatLog::OnChat(Player* player, uint32 type, uint32 /*lang*/, std::string& msg)
{
    std::string origMsg(msg);
    if (!_ChatCommon(CHAT_LOG_CHAT, player, msg))
        return;

    std::string logStr;
    switch (type)
    {
        case CHAT_MSG_SAY: logStr.append("{SAY}"); break;
        case CHAT_MSG_EMOTE: logStr.append("{EMOTE}"); break;
        case CHAT_MSG_YELL: logStr.append("{YELL}"); break;
    }
    _AppendPlayerName(player, logStr);
    _WriteLog(_logs[CHAT_LOG_CHAT], logStr, msg, origMsg);
}

void ChatLog::OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Player* receiver)
{
    std::string origMsg(msg);
    if (!_ChatCommon(CHAT_LOG_WHISPER, player, msg))
        return;

    std::string logStr;
    _AppendPlayerName(player, logStr);
    logStr.append("->");
    _AppendPlayerName(receiver, logStr);

    _WriteLog(_logs[CHAT_LOG_WHISPER], logStr, msg, origMsg);
}

void ChatLog::OnChat(Player* player, uint32 type, uint32 /*lang*/, std::string& msg, Group* group)
{
    std::string origMsg(msg);
    std::string logStr;
    _AppendPlayerName(player, logStr);

    switch (type)
    {
    case CHAT_MSG_PARTY:
    case CHAT_MSG_PARTY_LEADER:
        if (_ChatCommon(CHAT_LOG_PARTY, player, msg))
        {
            switch (type)
            {
                case CHAT_MSG_PARTY:        logStr.append("->PARTY");         break;
                case CHAT_MSG_PARTY_LEADER: logStr.append("->PARTY_LEADER");  break;
            }
            _AppendGroupMembers(group, logStr);
            _WriteLog(_logs[CHAT_LOG_PARTY], logStr, msg, origMsg);
        }
        break;
    case CHAT_MSG_RAID_LEADER:
    case CHAT_MSG_RAID_WARNING:
    case CHAT_MSG_RAID:
        if (_ChatCommon(CHAT_LOG_RAID, player, msg))
        {
            switch (type)
            {
                case CHAT_MSG_RAID_LEADER:  logStr.append("->RAID_LEADER");
                case CHAT_MSG_RAID_WARNING: logStr.append("->RAID_WARN");
                case CHAT_MSG_RAID:         logStr.append("->RAID");
            }
            _AppendGroupMembers(group, logStr);
            _WriteLog(_logs[CHAT_LOG_RAID], logStr, msg, origMsg);
        }
        break;
    case CHAT_MSG_BATTLEGROUND:
    case CHAT_MSG_BATTLEGROUND_LEADER:
        if (_ChatCommon(CHAT_LOG_BATTLEGROUND, player, msg))
        {
            switch (type)
            {
                case CHAT_MSG_BATTLEGROUND:         logStr.append("->BG");         break;
                case CHAT_MSG_BATTLEGROUND_LEADER:  logStr.append("->BG_LEADER");  break;
            }
            _AppendGroupMembers(group, logStr);
            _WriteLog(_logs[CHAT_LOG_BATTLEGROUND], logStr, msg, origMsg);
        }
        break;
    }
}

void ChatLog::OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Guild* guild)
{
    std::string origMsg(msg);
    if (!_ChatCommon(CHAT_LOG_GUILD, player, msg))
        return;

    std::string logStr;
    _AppendPlayerName(player, logStr);
    switch (type)
    {
        case CHAT_MSG_GUILD:    logStr.append("->GUILD");      break;
        case CHAT_MSG_OFFICER:  logStr.append("->GUILD_OFF");  break;
    }
    logStr.append(" {").append(guild ? guild->GetName() : "unknowng guild").append("}:");

    _WriteLog(_logs[CHAT_LOG_GUILD], logStr, msg, origMsg);
}

void ChatLog::OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Channel* channel)
{
    std::string origMsg(msg);
    if (!_ChatCommon(CHAT_LOG_CHANNEL, player, msg))
        return;

    std::string logStr;
    _AppendPlayerName(player, logStr);
    logStr.append(" {").append(channel ? channel->GetName() : "Unknown channel").append("}");

    _WriteLog(_logs[CHAT_LOG_CHANNEL], logStr, msg, origMsg);
}

void AddSC_lexics_chat_log()
{
    new ChatLog();
}

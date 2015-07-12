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

#ifndef TRINITYCORE_CHATLOG_H
#define TRINITYCORE_CHATLOG_H

#include "ScriptPCH.h"
#include "ChatLexicsCutter.h"

enum ChatLogType
{
    CHAT_LOG_NONE,
    CHAT_LOG_CHAT,
    CHAT_LOG_PARTY,
    CHAT_LOG_GUILD,
    CHAT_LOG_WHISPER,
    CHAT_LOG_CHANNEL,
    CHAT_LOG_RAID,
    CHAT_LOG_BATTLEGROUND,

    CHAT_LOG_COUNT,
    CHAT_LOG_INNORMATIVE
};

enum LexicsActions
{
    LEXICS_ACTION_LOG,
    LEXICS_ACTION_SHEEP,
    LEXICS_ACTION_STUN,
    LEXICS_ACTION_DIE,
    LEXICS_ACTION_DRAIN,
    LEXICS_ACTION_SILENCE,
    LEXICS_ACTION_STUCK,
    LEXICS_ACTION_SICKNESS,
    LEXICS_ACTION_SHEAR,
};

class ChatLogInfo
{
private:
    FILE *_file;
    std::string _name;
    bool _screenFlag;
    bool _cutFlag;
    uint32 _flushLength;
    uint32 _writtenLength;
    ChatLogType _type;
    ACE_Thread_Mutex _lock;

public:
    ChatLogInfo(ChatLogType type, bool chat, bool lexics, uint32 flushLength);

    void OpenFile(bool dateSplit, const std::string& date, bool utfHeader);
    void CloseFile()
    {
        ACE_Guard<ACE_Thread_Mutex> guard(_lock);
        if (_file)
        {
            fclose(_file);
            _file = NULL;
        }
    }
    void WriteFile(const std::string& msg);

    FILE* GetFile() const { return _file; }
    std::string GetName() const { return _name; }
    bool IsCut() const { return _cutFlag; }
    bool IsScreen() const { return _screenFlag; }

    bool SetFileIfSame(ChatLogInfo* log)
    {
        if (_name == log->GetName())
        {
            _file = log->GetFile();
            return true;
        }
        return false;
    }
    void CloseFileIfSame(ChatLogInfo* log)
    {
        if (_file == log->GetFile())
            _file = NULL;
    }
};

class ChatLog : public PlayerScript
{
public:
    static std::string GetChatNameByType(ChatLogType type);
    static std::string GetChatDescByType(ChatLogType type);
    static void OutTimestamp(FILE *file);

    ChatLog();
    ~ChatLog();

    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg);
    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Player* receiver);
    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Group* group);
    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Guild* guild);
    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg, Channel* channel);

private:
    bool _ChatCommon(ChatLogType type, Player* player, std::string& msg);
    void _Punish(Player* player, std::string& msg);
    void _ApplySpell(Player* player, uint32 spellId);

    void _Initialize();
    void _OpenAllFiles();
    void _CloseAllFiles();
    void _CheckDateSwitch();
    void _AppendPlayerName(Player* player, std::string& s);
    void _AppendGroupMembers(Group* group, std::string& s);
    void _WriteLog(ChatLogInfo* log, std::string& logStr, const std::string& msg, const std::string& origMsg);

    // Chats
    bool _enable;
    bool _dateSplit;
    bool _utfHeader;
    bool _ignoreUnprintable;

    uint32 _flushLength;
    int32 _lastDay;

    ChatLogInfo* _logs[CHAT_LOG_COUNT];

    // Lexics
    LexicsCutter* _lexics;

    bool _lexicsEnable;
    bool _lexicsInnormativeCut;
    bool _lexicsIgnoreGm;

    std::string _lexicsCutReplacement;
    LexicsActions _lexicsAction;
    uint32 _lexicsActionDuration;

    ChatLogInfo* _innormativeLog;
    ACE_Thread_Mutex _lock;
};

#define sChatLog (*ACE_Singleton<ChatLog, ACE_Thread_Mutex>::instance())
#endif

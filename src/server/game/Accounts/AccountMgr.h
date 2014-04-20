/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Copyright (C) 2012 SymphonyArt <http://symphonyart.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#ifndef _ACCMGR_H
#define _ACCMGR_H

#include <string>

#include "Common.h"

enum AccountOpResult
{
    AOR_OK,
    AOR_NAME_TOO_LONG,
    AOR_PASS_TOO_LONG,
    AOR_NAME_ALREDY_EXIST,
    AOR_NAME_NOT_EXIST,
    AOR_DB_INTERNAL_ERROR
};

#define MAX_ACCOUNT_STR 16

class AccountMgr
{
public:
    AccountMgr();
    ~AccountMgr();

    AccountOpResult CreateAccount(std::string username, std::string password);
    AccountOpResult DeleteAccount(uint32 accid);
    AccountOpResult ChangeUsername(uint32 accid, std::string new_uname, std::string new_passwd);
    AccountOpResult ChangePassword(uint32 accid, std::string new_passwd);
    bool CheckPassword(uint32 accid, std::string passwd);

    uint32 GetId(std::string username);
    uint32 GetSecurity(uint32 acc_id);
    uint32 GetSecurity(uint64 acc_id, int32 realm_id);
    bool GetName(uint32 acc_id, std::string &name);
    uint32 GetCharactersCount(uint32 acc_id);
    std::string CalculateShaPassHash(std::string& name, std::string& password);

    static bool normalizeString(std::string& utf8str);
};

#define sAccountMgr ACE_Singleton<AccountMgr, ACE_Null_Mutex>::instance()
#endif

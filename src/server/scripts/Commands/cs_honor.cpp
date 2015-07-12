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

class honor_commandscript : public CommandScript
{
public:
    honor_commandscript() : CommandScript("honor_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand honorAddCommandTable[] =
        {
            { "kill",           SEC_GAMEMASTER,     false, &HandleHonorAddKillCommand,         "", NULL },
            { "",               SEC_GAMEMASTER,     false, &HandleHonorAddCommand,             "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        static ChatCommand honorCommandTable[] =
        {
            { "add",            SEC_GAMEMASTER,     false, NULL,               "", honorAddCommandTable },
            { "update",         SEC_GAMEMASTER,     false, &HandleHonorUpdateCommand,          "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "honor",          SEC_GAMEMASTER,     false, NULL,                  "", honorCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }
    static bool HandleHonorAddCommand(ChatHandler* handler, const char* args)
    {
        if(!*args)
            return false;

        Player* target = handler->getSelectedPlayer();
        if(!target)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if(handler->HasLowerSecurity(target, 0))
            return false;

        uint32 amount = (uint32)atoi(args);
        target->RewardHonor(NULL, 1, amount);
        return true;
    }
    static bool HandleHonorAddKillCommand(ChatHandler* handler, const char* /*args*/)
    {
        Unit* target = handler->getSelectedUnit();
        if(!target)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if(target->GetTypeId() == TYPEID_PLAYER && handler->HasLowerSecurity((Player*)target, 0))
            return false;

        handler->GetSession()->GetPlayer()->RewardHonor(target, 1);
        return true;
    }
    static bool HandleHonorUpdateCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* target = handler->getSelectedPlayer();
        if(!target)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if(handler->HasLowerSecurity(target, 0))
            return false;

        target->UpdateHonorFields();
        return true;
    }
};

void AddSC_honor_commandscript()
{
    new honor_commandscript;
}

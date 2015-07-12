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
#include "SpellMgr.h"
#include "Chat.h"

class learn_commandscript : public CommandScript
{
public:
    learn_commandscript() : CommandScript("learn_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand learnAllCommandTable[] =
        {
            { "crafts",         SEC_GAMEMASTER,     false, &HandleLearnAllCraftsCommand,      "", NULL },
            { "default",        SEC_MODERATOR,      false, &HandleLearnAllDefaultCommand,     "", NULL },
            { "lang",           SEC_MODERATOR,      false, &HandleLearnAllLangCommand,        "", NULL },
            { "recipes",        SEC_GAMEMASTER,     false, &HandleLearnAllRecipesCommand,     "", NULL },
            { NULL,             0,                  false, NULL,                              "", NULL }
        };

        static ChatCommand learnCommandTable[] =
        {
            { "all",            SEC_ADMINISTRATOR,  false, NULL,              "", learnAllCommandTable },
            { "",               SEC_ADMINISTRATOR,  false, &HandleLearnCommand,               "", NULL },
            { NULL,             0,                  false, NULL,                              "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "learn",          SEC_MODERATOR,      false, NULL,                  "", learnCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleLearnCommand(ChatHandler* handler, const char* args)
    {
        Player* targetPlayer = handler->getSelectedPlayer();

        if(!targetPlayer)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
        uint32 spell = handler->extractSpellIdFromLink((char*)args);
        if(!spell || !sSpellStore.LookupEntry(spell))
            return false;

        char const* allStr = strtok(NULL, " ");
        bool allRanks = allStr ? (strncmp(allStr, "all", strlen(allStr)) == 0) : false;

        SpellEntry const* spellInfo = sSpellStore.LookupEntry(spell);
        if(!spellInfo || !SpellMgr::IsSpellValid(spellInfo, handler->GetSession()->GetPlayer()))
        {
            handler->PSendSysMessage(LANG_COMMAND_SPELL_BROKEN, spell);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if(!allRanks && targetPlayer->HasSpell(spell))
        {
            if(targetPlayer == handler->GetSession()->GetPlayer())
                handler->SendSysMessage(LANG_YOU_KNOWN_SPELL);
            else
                handler->PSendSysMessage(LANG_TARGET_KNOWN_SPELL, handler->GetNameLink(targetPlayer).c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        if(allRanks)
            targetPlayer->learnSpellHighRank(spell);
        else
            targetPlayer->learnSpell(spell, false);

        uint32 first_spell = sSpellMgr->GetFirstSpellInChain(spell);
        if(GetTalentSpellCost(first_spell))
            targetPlayer->SendTalentsInfoData(false);

        return true;
    }

    static bool HandleLearnAllLangCommand(ChatHandler* handler, const char* /*args*/)
    {
        // skipping UNIVERSAL language (0)
        for(uint8 i = 1; i < LANGUAGES_COUNT; ++i)
            handler->GetSession()->GetPlayer()->learnSpell(lang_description[i].spell_id, false);

        handler->SendSysMessage(LANG_COMMAND_LEARN_ALL_LANG);
        return true;
    }

    static bool HandleLearnAllDefaultCommand(ChatHandler* handler, const char* args)
    {
        Player* target;
        if(!handler->extractPlayerTarget((char*)args, &target))
            return false;

        target->learnDefaultSpells();
        target->learnQuestRewardedSpells();

        handler->PSendSysMessage(LANG_COMMAND_LEARN_ALL_DEFAULT_AND_QUEST, handler->GetNameLink(target).c_str());
        return true;
    }

    static bool HandleLearnAllCraftsCommand(ChatHandler* handler, const char* /*args*/)
    {
        for(uint32 i = 0; i < sSkillLineStore.GetNumRows(); ++i)
        {
            SkillLineEntry const *skillInfo = sSkillLineStore.LookupEntry(i);
            if(!skillInfo)
                continue;

            if((skillInfo->categoryId == SKILL_CATEGORY_PROFESSION || skillInfo->categoryId == SKILL_CATEGORY_SECONDARY) &&
                skillInfo->canLink)                             // only prof. with recipes have
            {
                HandleLearnSkillRecipesHelper(handler->GetSession()->GetPlayer(), skillInfo->id);
            }
        }

        handler->SendSysMessage(LANG_COMMAND_LEARN_ALL_CRAFT);
        return true;
    }

    static bool HandleLearnAllRecipesCommand(ChatHandler* handler, const char* args)
    {
        //  Learns all recipes of specified profession and sets skill to max
        //  Example: .learn all_recipes enchanting

        Player* target = handler->getSelectedPlayer();
        if(!target)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            return false;
        }

        if(!*args)
            return false;

        std::wstring wnamepart;

        if(!Utf8toWStr(args, wnamepart))
            return false;

        // converting string that we try to find to lower case
        wstrToLower(wnamepart);

        std::string name;

        SkillLineEntry const *targetSkillInfo = NULL;
        for(uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
        {
            SkillLineEntry const *skillInfo = sSkillLineStore.LookupEntry(i);
            if(!skillInfo)
                continue;

            if((skillInfo->categoryId != SKILL_CATEGORY_PROFESSION &&
                skillInfo->categoryId != SKILL_CATEGORY_SECONDARY) ||
                !skillInfo->canLink)                            // only prof with recipes have set
                continue;

            int loc = handler->GetSessionDbcLocale();
            name = skillInfo->name[loc];
            if(name.empty())
                continue;

            if(!Utf8FitTo(name, wnamepart))
            {
                loc = 0;
                for(; loc < TOTAL_LOCALES; ++loc)
                {
                    if(loc == handler->GetSessionDbcLocale())
                        continue;

                    name = skillInfo->name[loc];
                    if(name.empty())
                        continue;

                    if(Utf8FitTo(name, wnamepart))
                        break;
                }
            }

            if(loc < TOTAL_LOCALES)
            {
                targetSkillInfo = skillInfo;
                break;
            }
        }

        if(!targetSkillInfo)
            return false;

        HandleLearnSkillRecipesHelper(target, targetSkillInfo->id);

        uint16 maxLevel = target->GetPureMaxSkillValue(targetSkillInfo->id);
        target->SetSkill(targetSkillInfo->id, target->GetSkillStep(targetSkillInfo->id), maxLevel, maxLevel);
        handler->PSendSysMessage(LANG_COMMAND_LEARN_ALL_RECIPES, name.c_str());
        return true;
    }

    static void HandleLearnSkillRecipesHelper(Player* player, uint32 skill_id)
    {
        uint32 classmask = player->getClassMask();

        for(uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
        {
            SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
            if(!skillLine)
                continue;

            // wrong skill
            if(skillLine->skillId != skill_id)
                continue;

            // not high rank
            if(skillLine->forward_spellid)
                continue;

            // skip racial skills
            if(skillLine->racemask != 0)
                continue;

            // skip wrong class skills
            if(skillLine->classmask && (skillLine->classmask & classmask) == 0)
                continue;

            SpellEntry const* spellInfo = sSpellStore.LookupEntry(skillLine->spellId);
            if(!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
                continue;

            player->learnSpell(skillLine->spellId, false);
        }
    }
};

void AddSC_learn_commandscript()
{
    new learn_commandscript;
}

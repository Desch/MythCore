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

#include "Config.h"
#include <ace/Configuration_Import_Export.h>

static bool GetValueHelper(ACE_Configuration_Heap *mConf, const char *name, ACE_TString &result)
{
    if(!mConf)
        return false;

    ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, (sConfig->mMtx), false);

    ACE_TString section_name;
    ACE_Configuration_Section_Key section_key;
    ACE_Configuration_Section_Key root_key = mConf->root_section();

    int i = 0;
    while(mConf->enumerate_sections(root_key, i, section_name) == 0)
    {
        mConf->open_section(root_key, section_name.c_str(), 0, section_key);
        if(mConf->get_string_value(section_key, name, result) == 0)
            return true;
        ++i;
    }

    return false;
}

Config::Config() : mConf(NULL) { }

Config::~Config()
{
    delete mConf;
}

bool Config::SetSource(const char *file)
{
    mFilename = file;
    return Reload();
}

bool Config::Reload()
{
    delete mConf;
    mConf = new ACE_Configuration_Heap;
    if(mConf->open() == 0)
    {
        ACE_Ini_ImpExp config_importer(*mConf);
        if(config_importer.import_config(mFilename.c_str()) == 0)
            return true;
    }
    delete mConf;
    mConf = NULL;
    return false;
}

std::string Config::GetStringDefault(const char * name, std::string def)
{
    ACE_TString val;
    return GetValueHelper(mConf, name, val) ? val.c_str() : def;
}

bool Config::GetBoolDefault(const char * name, const bool def)
{
    ACE_TString val;
    if(!GetValueHelper(mConf, name, val))
        return def;
    const char* str = val.c_str();

    if(strcmp(str, "true") == 0 || strcmp(str, "TRUE") == 0 ||
        strcmp(str, "yes") == 0 || strcmp(str, "YES") == 0 ||
        strcmp(str, "1") == 0)
        return true;
    else
        return false;
}

int32 Config::GetIntDefault(const char * name, const int32 def)
{
    ACE_TString val;
    return GetValueHelper(mConf, name, val) ? atoi(val.c_str()) : def;
}

float Config::GetFloatDefault(const char * name, const float def)
{
    ACE_TString val;
    return GetValueHelper(mConf, name, val) ? (float)atof(val.c_str()) : def;
}
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

#include "AuthCodes.h"

namespace AuthHelper
{
    bool IsPreBCAcceptedClientBuild(int build)
    {
        int accepted_versions[] = PRE_BC_ACCEPTED_CLIENT_BUILD;

        for(int i = 0; accepted_versions[i]; ++i)
            if(build == accepted_versions[i])
                return true;

        return false;
    }

    bool IsPostBCAcceptedClientBuild(int build)
    {
        int accepted_versions[] = POST_BC_ACCEPTED_CLIENT_BUILD;

        for(int i = 0; accepted_versions[i]; ++i)
            if(build == accepted_versions[i])
                return true;

        return false;
    }

    bool IsAcceptedClientBuild(int build)
    {
        return (IsPostBCAcceptedClientBuild(build) || IsPreBCAcceptedClientBuild(build));
    }
};


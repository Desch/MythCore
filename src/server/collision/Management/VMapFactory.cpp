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

#include <sys/types.h>
#include "VMapFactory.h"
#include "VMapManager2.h"
#include "G3D/Table.h"

using namespace G3D;

namespace VMAP
{
    void chompAndTrim(std::string& str)
    {
        while(str.length() >0)
        {
            char lc = str[str.length()-1];
            if(lc == '\r' || lc == '\n' || lc == ' ' || lc == '"' || lc == '\'')
            {
                str = str.substr(0, str.length()-1);
            }
            else
            {
                break;
            }
        }
        while(str.length() >0)
        {
            char lc = str[0];
            if(lc == ' ' || lc == '"' || lc == '\'')
            {
                str = str.substr(1, str.length()-1);
            }
            else
            {
                break;
            }
        }
    }

    IVMapManager* gVMapManager = 0;

    //===============================================
    // result false, if no more id are found

    bool getNextId(const std::string& pString, unsigned int& pStartPos, unsigned int& pId)
    {
        bool result = false;
        unsigned int i;
        for(i = pStartPos; i < pString.size(); ++i)
        {
            if(pString[i] == ',')
            {
                break;
            }
        }
        if(i>pStartPos)
        {
            std::string idString = pString.substr(pStartPos, i-pStartPos);
            pStartPos = i+1;
            chompAndTrim(idString);
            pId = atoi(idString.c_str());
            result = true;
        }
        return(result);
    }

    //===============================================
    // just return the instance
    IVMapManager* VMapFactory::createOrGetVMapManager()
    {
        if(gVMapManager == 0)
            gVMapManager= new VMapManager2();                // should be taken from config ... Please change if you like :-)
        return gVMapManager;
    }

    //===============================================
    // delete all internal data structures
    void VMapFactory::clear()
    {
        delete gVMapManager;
        gVMapManager = NULL;
    }
}

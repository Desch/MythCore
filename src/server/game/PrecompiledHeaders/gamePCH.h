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

#include "WorldSocket.h"        // must be first to make ACE happy with ACE includes in it

#include "Common.h"

#include "MapManager.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "ObjectDefines.h"
#include "Opcodes.h"
#include "SharedDefines.h"
#include "ObjectMgr.h"

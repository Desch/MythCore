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

#ifndef _WARDEN_MAC_H
#define _WARDEN_MAC_H

#include "ARC4.h"
#include <map>
#include "Cryptography/BigNumber.h"
#include "ByteBuffer.h"
#include "WardenBase.h"

class WorldSession;
class WardenBase;

class WardenMac : WardenBase
{
public:
    WardenMac();
    ~WardenMac();

    void Init(WorldSession* pClient, BigNumber* K);
    ClientWardenModule* GetModuleForClient();
    void InitializeModule();
    void RequestHash();
    void HandleHashResult(ByteBuffer &buff);
    void RequestData();
    void HandleData(ByteBuffer &buff);
};

#endif

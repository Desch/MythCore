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

#ifndef _AUTHCRYPT_H
#define _AUTHCRYPT_H

#include <Common.h>
#include "Cryptography/ARC4.h"

class BigNumber;

class AuthCrypt
{
public:
    AuthCrypt();
    ~AuthCrypt();

    void Init(BigNumber* K);
    void DecryptRecv(uint8 *, size_t);
    void EncryptSend(uint8 *, size_t);

    bool IsInitialized() const { return _initialized; }

private:
    ARC4 _clientDecrypt;
    ARC4 _serverEncrypt;
    bool _initialized;
};

#endif
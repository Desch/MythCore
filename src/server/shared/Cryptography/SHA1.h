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

#ifndef _AUTH_SHA1_H
#define _AUTH_SHA1_H

#include "Common.h"
#include <openssl/sha.h>
#include <openssl/crypto.h>

class BigNumber;

class SHA1Hash
{
public:
    SHA1Hash();
    ~SHA1Hash();

    void UpdateFinalizeBigNumbers(BigNumber* bn0, ...);
    void UpdateBigNumbers(BigNumber* bn0, ...);

    void UpdateData(const uint8 *dta, int len);
    void UpdateData(const std::string &str);

    void Initialize();
    void Finalize();

    uint8 *GetDigest(void) { return mDigest; };
    int GetLength(void) const { return SHA_DIGEST_LENGTH; };

private:
    SHA_CTX mC;
    uint8 mDigest[SHA_DIGEST_LENGTH];
};
#endif
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

#ifndef _AUTH_HMAC_H
#define _AUTH_HMAC_H

#include "Common.h"
#include <openssl/hmac.h>
#include <openssl/sha.h>

class BigNumber;

#define SEED_KEY_SIZE 16

class HmacHash
{
public:
    HmacHash(uint32 len, uint8 *seed);
    ~HmacHash();
    void UpdateBigNumber(BigNumber* bn);
    void UpdateData(const uint8 *data, int length);
    void UpdateData(const std::string &str);
    void Finalize();
    uint8 *ComputeHash(BigNumber* bn);
    uint8 *GetDigest() { return (uint8*)m_digest; }
    int GetLength() const { return SHA_DIGEST_LENGTH; }
private:
    HMAC_CTX m_ctx;
    uint8 m_digest[SHA_DIGEST_LENGTH];
};
#endif
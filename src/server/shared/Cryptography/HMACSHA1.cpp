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

#include "HMACSHA1.h"
#include "BigNumber.h"

HmacHash::HmacHash(uint32 len, uint8 *seed)
{
    HMAC_CTX_init(&m_ctx);
    HMAC_Init_ex(&m_ctx, seed, len, EVP_sha1(), NULL);
}

HmacHash::~HmacHash()
{
    HMAC_CTX_cleanup(&m_ctx);
}

void HmacHash::UpdateBigNumber(BigNumber* bn)
{
    UpdateData(bn->AsByteArray(), bn->GetNumBytes());
}

void HmacHash::UpdateData(const uint8 *data, int length)
{
    HMAC_Update(&m_ctx, data, length);
}

void HmacHash::UpdateData(const std::string &str)
{
    UpdateData((uint8 const*)str.c_str(), str.length());
}

void HmacHash::Finalize()
{
    uint32 length = 0;
    HMAC_Final(&m_ctx, (uint8*)m_digest, &length);
    ASSERT(length == SHA_DIGEST_LENGTH)
}

uint8 *HmacHash::ComputeHash(BigNumber* bn)
{
    HMAC_Update(&m_ctx, bn->AsByteArray(), bn->GetNumBytes());
    Finalize();
    return (uint8*)m_digest;
}

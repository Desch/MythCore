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

#ifndef _AUTH_BIGNUMBER_H
#define _AUTH_BIGNUMBER_H

#include "Common.h"

struct bignum_st;

class BigNumber
{
public:
    BigNumber();
    BigNumber(const BigNumber &bn);
    BigNumber(uint32);
    ~BigNumber();

    void SetDword(uint32);
    void SetQword(uint64);
    void SetBinary(const uint8 *bytes, int len);
    void SetHexStr(const char *str);

    void SetRand(int numbits);

    BigNumber operator=(const BigNumber &bn);

    BigNumber operator+=(const BigNumber &bn);
    BigNumber operator+(const BigNumber &bn)
    {
        BigNumber t(*this);
        return t += bn;
    }
    BigNumber operator-=(const BigNumber &bn);
    BigNumber operator-(const BigNumber &bn)
    {
        BigNumber t(*this);
        return t -= bn;
    }
    BigNumber operator*=(const BigNumber &bn);
    BigNumber operator*(const BigNumber &bn)
    {
        BigNumber t(*this);
        return t *= bn;
    }
    BigNumber operator/=(const BigNumber &bn);
    BigNumber operator/(const BigNumber &bn)
    {
        BigNumber t(*this);
        return t /= bn;
    }
    BigNumber operator%=(const BigNumber &bn);
    BigNumber operator%(const BigNumber &bn)
    {
        BigNumber t(*this);
        return t %= bn;
    }

    bool isZero() const;

    BigNumber ModExp(const BigNumber &bn1, const BigNumber &bn2);
    BigNumber Exp(const BigNumber &);

    int GetNumBytes(void);

    struct bignum_st *BN() { return _bn; }

    uint32 AsDword();
    uint8* AsByteArray(int minSize = 0, bool reverse = true);

    const char *AsHexStr();
    const char *AsDecStr();

private:
    struct bignum_st *_bn;
    uint8 *_array;
};
#endif
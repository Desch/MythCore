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

#ifndef _WARDEN_DATA_STORAGE_H
#define _WARDEN_DATA_STORAGE_H

#include <map>
#include "BigNumber.h"

struct WardenData
{
    uint8 Type;
    BigNumber i;
    uint32 Address;                                         // PROC_CHECK, MEM_CHECK, PAGE_CHECK
    uint8 Length;                                           // PROC_CHECK, MEM_CHECK, PAGE_CHECK
    std::string str;                                        // LUA, MPQ, DRIVER
    uint8 banDays;
    uint32 DBid;
};

struct WardenDataResult
{
    BigNumber res;                                          // MEM_CHECK
};

class CWardenDataStorage
{
public:
    CWardenDataStorage();
    ~CWardenDataStorage();
    uint32 InternalDataID;
    std::vector<uint32> MemCheckIds;

private:
    std::map<uint32, WardenData*> _data_map;
    std::map<uint32, WardenDataResult*> _result_map;

public:
    inline uint32 GenerateInternalDataID() { return InternalDataID++; }
    WardenData *GetWardenDataById(uint32 Id);
    WardenDataResult *GetWardenResultById(uint32 Id);
    void Init();
protected:
    void LoadWardenDataResult();
};

extern CWardenDataStorage WardenDataStorage;
#endif

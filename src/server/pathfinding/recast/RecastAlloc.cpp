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

#include <stdlib.h>
#include <string.h>
#include "RecastAlloc.h"

static void *rcAllocDefault(int size, rcAllocHint)
{
    return malloc(size);
}

static void rcFreeDefault(void *ptr)
{
    free(ptr);
}

static rcAllocFunc* sRecastAllocFunc = rcAllocDefault;
static rcFreeFunc* sRecastFreeFunc = rcFreeDefault;

void rcAllocSetCustom(rcAllocFunc *allocFunc, rcFreeFunc *freeFunc)
{
    sRecastAllocFunc = allocFunc ? allocFunc : rcAllocDefault;
    sRecastFreeFunc = freeFunc ? freeFunc : rcFreeDefault;
}

void* rcAlloc(int size, rcAllocHint hint)
{
    return sRecastAllocFunc(size, hint);
}

void rcFree(void* ptr)
{
    if(ptr)
        sRecastFreeFunc(ptr);
}

void rcIntArray::resize(int n)
{
    if(n > m_cap)
    {
        if(!m_cap) m_cap = n;
        while (m_cap < n) m_cap *= 2;
        int* newData = (int*)rcAlloc(m_cap*sizeof(int), RC_ALLOC_TEMP);
        if(m_size && newData) memcpy(newData, m_data, m_size*sizeof(int));
        rcFree(m_data);
        m_data = newData;
    }
    m_size = n;
}


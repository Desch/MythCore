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
#include "DetourAlloc.h"

static void *dtAllocDefault(int size, dtAllocHint)
{
    return malloc(size);
}

static void dtFreeDefault(void *ptr)
{
    free(ptr);
}

static dtAllocFunc* sAllocFunc = dtAllocDefault;
static dtFreeFunc* sFreeFunc = dtFreeDefault;

void dtAllocSetCustom(dtAllocFunc *allocFunc, dtFreeFunc *freeFunc)
{
    sAllocFunc = allocFunc ? allocFunc : dtAllocDefault;
    sFreeFunc = freeFunc ? freeFunc : dtFreeDefault;
}

void* dtAlloc(int size, dtAllocHint hint)
{
    return sAllocFunc(size, hint);
}

void dtFree(void* ptr)
{
    if(ptr)
        sFreeFunc(ptr);
}

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

#ifndef DETOURALLOCATOR_H
#define DETOURALLOCATOR_H

enum dtAllocHint
{
    DT_ALLOC_PERM,        // Memory persist after a function call.
    DT_ALLOC_TEMP        // Memory used temporarily within a function.
};

typedef void* (dtAllocFunc)(int size, dtAllocHint hint);
typedef void (dtFreeFunc)(void* ptr);

void dtAllocSetCustom(dtAllocFunc *allocFunc, dtFreeFunc *freeFunc);

void* dtAlloc(int size, dtAllocHint hint);
void dtFree(void* ptr);

#endif

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

#ifndef RECASTALLOC_H
#define RECASTALLOC_H

enum rcAllocHint
{
    RC_ALLOC_PERM,        // Memory persist after a function call.
    RC_ALLOC_TEMP        // Memory used temporarily within a function.
};

typedef void* (rcAllocFunc)(int size, rcAllocHint hint);
typedef void (rcFreeFunc)(void* ptr);

void rcAllocSetCustom(rcAllocFunc *allocFunc, rcFreeFunc *freeFunc);

void* rcAlloc(int size, rcAllocHint hint);
void rcFree(void* ptr);

// Simple dynamic array ints.
class rcIntArray
{
    int* m_data;
    int m_size, m_cap;
    inline rcIntArray(const rcIntArray&);
    inline rcIntArray& operator=(const rcIntArray&);
public:
    inline rcIntArray() : m_data(0), m_size(0), m_cap(0) { }
    inline rcIntArray(int n) : m_data(0), m_size(0), m_cap(0) { resize(n); }
    inline ~rcIntArray() { rcFree(m_data); }
    void resize(int n);
    inline void push(int item) { resize(m_size+1); m_data[m_size-1] = item; }
    inline int pop() { if(m_size > 0) m_size--; return m_data[m_size]; }
    inline const int& operator[](int i) const { return m_data[i]; }
    inline int& operator[](int i) { return m_data[i]; }
    inline int size() const { return m_size; }
};

// Simple internal helper class to delete array in scope
template<class T> class rcScopedDelete
{
    T* ptr;
    inline T* operator=(T* p);
public:
    inline rcScopedDelete() : ptr(0) { }
    inline rcScopedDelete(T* p) : ptr(p) { }
    inline ~rcScopedDelete() { rcFree(ptr); }
    inline operator T*() { return ptr; }
};

#endif

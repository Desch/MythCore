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

#ifndef TRINITY_GRIDSTATES_H
#define TRINITY_GRIDSTATES_H

#include "Map.h"
#include "Object.h"

class GridState
{
    public:
#ifdef TRINITY_DEBUG
#define MAGIC_TESTVAL 0xFBE823BA
        GridState() { i_Magic = MAGIC_TESTVAL; }
        bool checkMagic()
        {
            if(i_Magic != MAGIC_TESTVAL)
            {
                sLog->outError("!!! GridState: Magic value gone !!!");
                return false;
            }
            return true;
        }
        void setMagic() { i_Magic = MAGIC_TESTVAL; }
        unsigned int i_Magic;
#endif
        virtual void Update(Map &, NGridType&, GridInfo &, const uint32 &x, const uint32 &y, const uint32 &t_diff) const = 0;
};

class InvalidState : public GridState
{
    public:
        void Update(Map &, NGridType &, GridInfo &, const uint32 &x, const uint32 &y, const uint32 &t_diff) const;
};

class ActiveState : public GridState
{
    public:
        void Update(Map &, NGridType &, GridInfo &, const uint32 &x, const uint32 &y, const uint32 &t_diff) const;
};

class IdleState : public GridState
{
    public:
        void Update(Map &, NGridType &, GridInfo &, const uint32 &x, const uint32 &y, const uint32 &t_diff) const;
};

class RemovalState : public GridState
{
    public:
        void Update(Map &, NGridType &, GridInfo &, const uint32 &x, const uint32 &y, const uint32 &t_diff) const;
};

#endif


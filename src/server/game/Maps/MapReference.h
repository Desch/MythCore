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

#ifndef _MAPREFERENCE_H
#define _MAPREFERENCE_H

#include "Reference.h"
#include "Map.h"

class MapReference : public Reference<Map, Player>
{
    protected:
        void targetObjectBuildLink()
        {
            // called from link()
            getTarget()->m_mapRefManager.insertFirst(this);
            getTarget()->m_mapRefManager.incSize();
        }
        void targetObjectDestroyLink()
        {
            // called from unlink()
            if(isValid()) getTarget()->m_mapRefManager.decSize();
        }
        void sourceObjectDestroyLink()
        {
            // called from invalidate()
            getTarget()->m_mapRefManager.decSize();
        }
    public:
        MapReference() : Reference<Map, Player>() { }
        ~MapReference() { unlink(); }
        MapReference *next() { return (MapReference*)Reference<Map, Player>::next(); }
        MapReference const *next() const { return (MapReference const*)Reference<Map, Player>::next(); }
        MapReference *nockeck_prev() { return (MapReference*)Reference<Map, Player>::nocheck_prev(); }
        MapReference const *nocheck_prev() const { return (MapReference const*)Reference<Map, Player>::nocheck_prev(); }
};

#endif
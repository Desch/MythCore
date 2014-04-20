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

#ifndef _GRIDREFERENCE_H
#define _GRIDREFERENCE_H

#include "LinkedReference/Reference.h"

template<class OBJECT>
class GridRefManager;

template<class OBJECT>
class GridReference : public Reference<GridRefManager<OBJECT>, OBJECT>
{
    protected:
        void targetObjectBuildLink()
        {
            // called from link()
            this->getTarget()->insertFirst(this);
            this->getTarget()->incSize();
        }
        void targetObjectDestroyLink()
        {
            // called from unlink()
            if(this->isValid()) this->getTarget()->decSize();
        }
        void sourceObjectDestroyLink()
        {
            // called from invalidate()
            this->getTarget()->decSize();
        }
    public:
        GridReference() : Reference<GridRefManager<OBJECT>, OBJECT>() { }
        ~GridReference() { this->unlink(); }
        GridReference *next() { return (GridReference*)Reference<GridRefManager<OBJECT>, OBJECT>::next(); }
};

#endif


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

#ifndef _REFMANAGER_H
#define _REFMANAGER_H

#include "Dynamic/LinkedList.h"
#include "Dynamic/LinkedReference/Reference.h"

template <class TO, class FROM> class RefManager : public LinkedListHead
{
public:
    typedef LinkedListHead::Iterator< Reference<TO, FROM> > iterator;
    RefManager() { }
    virtual ~RefManager() { clearReferences(); }

    Reference<TO, FROM>* getFirst() { return ((Reference<TO, FROM>*) LinkedListHead::getFirst()); }
    Reference<TO, FROM> const* getFirst() const { return ((Reference<TO, FROM> const*) LinkedListHead::getFirst()); }
    Reference<TO, FROM>* getLast() { return ((Reference<TO, FROM>*) LinkedListHead::getLast()); }
    Reference<TO, FROM> const* getLast() const { return ((Reference<TO, FROM> const*) LinkedListHead::getLast()); }

    iterator begin() { return iterator(getFirst()); }
    iterator end() { return iterator(NULL); }
    iterator rbegin() { return iterator(getLast()); }
    iterator rend() { return iterator(NULL); }

    void clearReferences()
    {
        LinkedListElement* ref;
        while((ref = getFirst()) != NULL)
        {
            ((Reference<TO, FROM>*) ref)->invalidate();
            ref->delink();  // the delink might be already done by invalidate(), but doing it here again does not hurt and insures an empty list
        }
    }
};

#endif
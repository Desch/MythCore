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

#ifndef TYPECONTAINER_FUNCTIONS_H
#define TYPECONTAINER_FUNCTIONS_H

/*
 * Here you'll find a list of helper functions to make
 * the TypeContainer usefull.  Without it, its hard
 * to access or mutate the container.
 */

#include "Define.h"
#include "Dynamic/TypeList.h"
#include <map>

namespace Trinity
{
    /* ContainerMapList Helpers */
    // count functions
    template<class SPECIFIC_TYPE> size_t Count(const ContainerMapList<SPECIFIC_TYPE> &elements, SPECIFIC_TYPE* /*pFake*/)
    {
        return elements._element.getSize();
    };

    template<class SPECIFIC_TYPE> size_t Count(const ContainerMapList<TypeNull> &/*elements*/, SPECIFIC_TYPE* /*pFake*/)
    {
        return 0;
    }

    template<class SPECIFIC_TYPE, class T> size_t Count(const ContainerMapList<T> &/*elements*/, SPECIFIC_TYPE* /*pFake*/)
    {
        return 0;
    }

    template<class SPECIFIC_TYPE, class T> size_t Count(const ContainerMapList<TypeList<SPECIFIC_TYPE, T> >&elements, SPECIFIC_TYPE* pFake)
    {
        return Count(elements._elements, pFake);
    }

    template<class SPECIFIC_TYPE, class H, class T> size_t Count(const ContainerMapList<TypeList<H, T> >&elements, SPECIFIC_TYPE* pFake)
    {
        return Count(elements._TailElements, pFake);
    }

    // non-const insert functions
    template<class SPECIFIC_TYPE> SPECIFIC_TYPE* Insert(ContainerMapList<SPECIFIC_TYPE> &elements, SPECIFIC_TYPE* pObj)
    {
        pObj->GetGridRef().link(&elements._element, pObj);
        return pObj;
    };

    template<class SPECIFIC_TYPE> SPECIFIC_TYPE* Insert(ContainerMapList<TypeNull> &/*elements*/, SPECIFIC_TYPE* /*pObj*/)
    {
        return NULL;
    }

    // this is a missed
    template<class SPECIFIC_TYPE, class T> SPECIFIC_TYPE* Insert(ContainerMapList<T> &/*elements*/, SPECIFIC_TYPE* /*pObj*/)
    {
        return NULL;                                        // a missed
    }

    // Recursion
    template<class SPECIFIC_TYPE, class H, class T> SPECIFIC_TYPE* Insert(ContainerMapList<TypeList<H, T> >&elements, SPECIFIC_TYPE* pObj)
    {
        SPECIFIC_TYPE* t= Insert(elements._elements, pObj);
        return (t != NULL ? t : Insert(elements._TailElements, pObj));
    }

    // non-const remove method
    template<class SPECIFIC_TYPE> SPECIFIC_TYPE* Remove(ContainerMapList<SPECIFIC_TYPE> & /*elements*/, SPECIFIC_TYPE* pObj)
    {
        if(pObj) {
            pObj->GetGridRef().unlink();
            return pObj;
        } else return NULL;
    }

    template<class SPECIFIC_TYPE> SPECIFIC_TYPE* Remove(ContainerMapList<TypeNull> &/*elements*/, SPECIFIC_TYPE* /*pObj*/)
    {
        return NULL;
    }

    // this is a missed
    template<class SPECIFIC_TYPE, class T> SPECIFIC_TYPE* Remove(ContainerMapList<T> &/*elements*/, SPECIFIC_TYPE* /*pObj*/)
    {
        return NULL;                                        // a missed
    }

    template<class SPECIFIC_TYPE, class T, class H> SPECIFIC_TYPE* Remove(ContainerMapList<TypeList<H, T> > &elements, SPECIFIC_TYPE* pObj)
    {
        // The head element is bad
        SPECIFIC_TYPE* t = Remove(elements._elements, pObj);
        return ( t != NULL ? t : Remove(elements._TailElements, pObj) );
    }
}
#endif


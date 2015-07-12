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

#ifndef DETOURASSERT_H
#define DETOURASSERT_H

// Note: This header file's only purpose is to include define assert.
// Feel free to change the file and include your own implementation instead.

#ifdef NDEBUG
// From http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/
#    define dtAssert(x) do { (void)sizeof(x); } while(__LINE__==-1,false)
#else
#    include <assert.h>
#    define dtAssert assert
#endif

#endif // DETOURASSERT_H

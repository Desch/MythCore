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

#ifndef TRINITYCORE_ERRORS_H
#define TRINITYCORE_ERRORS_H

#include "Common.h"
#include "Log.h"
#include <ace/Stack_Trace.h>

#define WPAssert( assertion ) { if(!(assertion)) { ACE_Stack_Trace st; sLog->outError( "\n%s:%i in %s ASSERTION FAILED:\n  %s\n%s\n", __FILE__, __LINE__, __FUNCTION__,  #assertion, st.c_str()); assert( #assertion &&0 ); ((void(*)())NULL)();} }
#define WPError( assertion, errmsg ) if(!(assertion)) { sLog->outError( "%\n%s:%i in %s ERROR:\n  %s\n", __FILE__, __LINE__, __FUNCTION__, (char *)errmsg ); assert( false ); }
#define WPWarning( assertion, errmsg ) if(!(assertion)) { sLog->outError( "\n%s:%i in %s WARNING:\n  %s\n", __FILE__, __LINE__, __FUNCTION__, (char *)errmsg ); }
#define WPFatal( assertion, errmsg ) if(!(assertion)) { sLog->outError( "\n%s:%i in %s FATAL ERROR:\n  %s\n", __FILE__, __LINE__, __FUNCTION__, (char *)errmsg ); assert( #assertion &&0 ); abort(); }

#define ASSERT WPAssert
#endif

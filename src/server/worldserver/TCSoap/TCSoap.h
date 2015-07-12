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

#ifndef _TCSOAP_H
#define _TCSOAP_H

#include "Common.h"
#include "World.h"
#include "AccountMgr.h"
#include "Log.h"

#include "soapH.h"
#include "soapStub.h"
#include "stdsoap2.h"

#include <ace/Semaphore.h>
#include <ace/Task.h>

class TCSoapRunnable: public ACE_Based::Runnable
{
public:
    TCSoapRunnable() { }
    void run();
    void setListenArguments(std::string host, uint16 port)
    {
        m_host = host;
        m_port = port;
    }
private:
    void process_message(ACE_Message_Block* mb);

    std::string m_host;
    uint16 m_port;
};

class SOAPCommand
{
public:
    SOAPCommand(): pendingCommands(0, USYNC_THREAD, "pendingCommands") { }

    ~SOAPCommand() { }

    void appendToPrintBuffer(const char* msg)
    {
        m_printBuffer += msg;
    }

    ACE_Semaphore pendingCommands;

    void setCommandSuccess(bool val)
    {
        m_success = val;
    }

    bool hasCommandSucceeded() const
    {
        return m_success;
    }

    static void print(void* callbackArg, const char* msg)
    {
        ((SOAPCommand*)callbackArg)->appendToPrintBuffer(msg);
    }

    static void commandFinished(void* callbackArg, bool success);

    bool m_success;
    std::string m_printBuffer;
};
#endif
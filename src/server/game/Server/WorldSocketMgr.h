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

#ifndef __WORLDSOCKETMGR_H
#define __WORLDSOCKETMGR_H

#include <ace/Basic_Types.h>
#include <ace/Singleton.h>
#include <ace/Thread_Mutex.h>

class WorldSocket;
class ReactorRunnable;
class ACE_Event_Handler;

/// Manages all sockets connected to peers and network threads
class WorldSocketMgr
{
public:
    friend class WorldSocket;
    friend class ACE_Singleton<WorldSocketMgr, ACE_Thread_Mutex>;

    /// Start network, listen at address:port .
    int StartNetwork(ACE_UINT16 port, const char* address);

    /// Stops all network threads, It will wait for all running threads .
    void StopNetwork();

    /// Wait untill all network threads have "joined" .
    void Wait();

private:
    int OnSocketOpen(WorldSocket* sock);

    int StartReactiveIO(ACE_UINT16 port, const char* address);

private:
    WorldSocketMgr();
    virtual ~WorldSocketMgr();

    ReactorRunnable* m_NetThreads;
    size_t m_NetThreadsCount;

    int m_SockOutKBuff;
    int m_SockOutUBuff;
    bool m_UseNoDelay;

    class WorldSocketAcceptor* m_Acceptor;
};

#define sWorldSocketMgr ACE_Singleton<WorldSocketMgr, ACE_Thread_Mutex>::instance()

#endif
/// @}

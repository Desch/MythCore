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

#ifndef __REALMSOCKET_H__
#define __REALMSOCKET_H__

#include <ace/Synch_Traits.h>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Message_Block.h>
#include <ace/Basic_Types.h>

class RealmSocket : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
private:
    typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> Base;

public:
    class Session
    {
    public:
        Session(void);
        virtual ~Session(void);

        virtual void OnRead(void) = 0;
        virtual void OnAccept(void) = 0;
        virtual void OnClose(void) = 0;
    };

    RealmSocket(void);
    virtual ~RealmSocket(void);

    size_t recv_len(void) const;
    bool recv_soft(char *buf, size_t len);
    bool recv(char *buf, size_t len);
    void recv_skip(size_t len);

    bool send(const char *buf, size_t len);

    const std::string& get_remote_address(void) const;

    virtual int open(void *);

    virtual int close(int);

    virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);
    virtual int handle_output(ACE_HANDLE = ACE_INVALID_HANDLE);

    virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

    void set_session(Session* session);

private:
    ssize_t noblk_send(ACE_Message_Block &message_block);

    ACE_Message_Block input_buffer_;
    Session *session_;
    std::string remote_address_;
};

#endif /* __REALMSOCKET_H__ */

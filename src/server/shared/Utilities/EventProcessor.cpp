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

#include "EventProcessor.h"

EventProcessor::EventProcessor()
{
    m_time = 0;
    m_aborting = false;
}

EventProcessor::~EventProcessor()
{
    KillAllEvents(true);
}

void EventProcessor::Update(uint32 p_time)
{
    // update time
    m_time += p_time;

    // main event loop
    EventList::iterator i;
    while(((i = m_events.begin()) != m_events.end()) && i->first <= m_time)
    {
        // get and remove event from queue
        BasicEvent* pBEvent = i->second;
        m_events.erase(i);
        if(!pBEvent)
            continue;

        if(!pBEvent->to_Abort) {
            if(pBEvent->Execute(m_time, p_time))
                delete pBEvent;
        } else {
            pBEvent->Abort(m_time);
            delete pBEvent;
        }
    }
}

void EventProcessor::KillAllEvents(bool force)
{
    // prevent event insertions
    m_aborting = true;

    // first, abort all existing events
    for(EventList::iterator i = m_events.begin(); i != m_events.end();)
    {
        EventList::iterator i_old = i;
        ++i;

        i_old->second->to_Abort = true;
        i_old->second->Abort(m_time);
        if(force || i_old->second->IsDeletable())
        {
            delete i_old->second;

            if(!force)                                      // need per-element cleanup
                m_events.erase (i_old);
        }
    }

    // fast clear event list (in force case)
    if(force)
        m_events.clear();
}

void EventProcessor::AddEvent(BasicEvent* Event, uint64 e_time, bool set_addtime)
{
    if(set_addtime) Event->m_addTime = m_time;
    Event->m_execTime = e_time;
    m_events.insert(std::pair<uint64, BasicEvent*>(e_time, Event));
}

uint64 EventProcessor::CalculateTime(uint64 t_offset) const
{
    return(m_time + t_offset);
}
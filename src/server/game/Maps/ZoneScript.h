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

#ifndef ZONE_SCRIPT_H_
#define ZONE_SCRIPT_H_

#include "Common.h"
#include "Creature.h"

class GameObject;

class ZoneScript
{
    public:
        ZoneScript() { }
        virtual ~ZoneScript() { }

        virtual uint32 GetCreatureEntry(uint32 /*guidlow*/, CreatureData const* data) { return data->id; }
        virtual uint32 GetGameObjectEntry(uint32 /*guidlow*/, uint32 entry) { return entry; }

        virtual void OnCreatureCreate(Creature* /*pCreature*/) { }
        virtual void OnCreatureRemove(Creature* /*pCreature*/) { }
        virtual void OnGameObjectCreate(GameObject* /*pGo*/) { }
        virtual void OnGameObjectRemove(GameObject* /*pGo*/) { }

        virtual void OnCreatureDeath(Creature* /*pCreature*/) { }

        //All-purpose data storage 64 bit
        virtual uint64 GetData64(uint32 /*DataId*/) { return 0; }
        virtual void SetData64(uint32 /*DataId*/, uint64 /*Value*/) { }

        //All-purpose data storage 32 bit
        virtual uint32 GetData(uint32 /*DataId*/) { return 0; }
        virtual void SetData(uint32 /*DataId*/, uint32 /*Value*/) { }

        virtual void ProcessEvent(WorldObject* /*obj*/, uint32 /*eventId*/) { }
};

#endif

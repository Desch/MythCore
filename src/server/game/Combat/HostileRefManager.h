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

#ifndef _HOSTILEREFMANAGER
#define _HOSTILEREFMANAGER

#include "Common.h"
#include "RefManager.h"

class Unit;
class ThreatManager;
class HostileReference;
struct SpellEntry;

//=================================================

class HostileRefManager : public RefManager<Unit, ThreatManager>
{
    private:
        Unit* iOwner;
    public:
        explicit HostileRefManager(Unit* owner) { iOwner = owner; }
        ~HostileRefManager();

        Unit* getOwner() { return iOwner; }

        // send threat to all my hateres for the victim
        // The victim is hated than by them as well
        // use for buffs and healing threat functionality
        void threatAssist(Unit* victim, float baseThreat, SpellEntry const* threatSpell = NULL);

        void addTempThreat(float threat, bool apply);

        void addThreatPercent(int32 percent);

        // The references are not needed anymore
        // tell the source to remove them from the list and free the mem
        void deleteReferences();

        // Remove specific faction references
        void deleteReferencesForFaction(uint32 faction);

        HostileReference* getFirst() { return ((HostileReference*) RefManager<Unit, ThreatManager>::getFirst()); }

        void updateThreatTables();

        void setOnlineOfflineState(bool isOnline);

        // set state for one reference, defined by Unit
        void setOnlineOfflineState(Unit* creature, bool isOnline);

        // delete one reference, defined by Unit
        void deleteReference(Unit* creature);

        void UpdateVisibility();
};

//=================================================
#endif


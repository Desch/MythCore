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

#ifndef _LFGGROUPDATA_H
#define _LFGGROUPDATA_H

#include "LFG.h"

enum LfgGroupEnum
{
    LFG_GROUP_MAX_KICKS                           = 3,
    LFG_GROUP_KICK_VOTES_NEEDED                   = 3
};

/**
    Stores all lfg data needed about a group.
*/
class LfgGroupData
{
    public:
        LfgGroupData();
        ~LfgGroupData();

        // General
        void SetState(LfgState state);
        void RestoreState();
        // Dungeon
        void SetDungeon(uint32 dungeon);
        // VoteKick
        void SetVotesNeeded(uint8 votes);
        void DecreaseKicksLeft();

        // General
        LfgState GetState() const;
        // Dungeon
        uint32 GetDungeon(bool asId = true) const;
        // VoteKick
        uint8 GetVotesNeeded() const;
        uint8 GetKicksLeft() const;

    private:
        // General
        LfgState m_State;                                  ///< State if group in LFG
        LfgState m_OldState;                               ///< Old State
        // Dungeon
        uint32 m_Dungeon;                                  ///< Dungeon entry
        // Vote Kick
        uint8 m_VotesNeeded;                               ///< Votes need to kick success
        uint8 m_KicksLeft;                                 ///< Number of kicks left
};

#endif

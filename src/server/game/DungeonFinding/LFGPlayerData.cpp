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

#include "LFGPlayerData.h"

LfgPlayerData::LfgPlayerData():
m_State(LFG_STATE_NONE), m_OldState(LFG_STATE_NONE), m_Roles(0), m_Comment("")
{ }

LfgPlayerData::~LfgPlayerData() { }

void LfgPlayerData::SetState(LfgState state)
{
    switch(state)
    {
        case LFG_STATE_NONE:
        case LFG_STATE_DUNGEON:
        case LFG_STATE_FINISHED_DUNGEON:
            m_OldState = state;
                    // No break on purpose
        default:
            m_State = state;
    }
}

void LfgPlayerData::ClearState()
{
    m_SelectedDungeons.clear();
    m_Roles = 0;
    m_State = m_OldState;
}

void LfgPlayerData::SetLockedDungeons(const LfgLockMap& lockStatus)
{
    m_LockedDungeons = lockStatus;
}

void LfgPlayerData::SetRoles(uint8 roles)
{
    m_Roles = roles;
}

void LfgPlayerData::SetComment(const std::string& comment)
{
    m_Comment = comment;
}

void LfgPlayerData::SetSelectedDungeons(const LfgDungeonSet& dungeons)
{
    m_SelectedDungeons = dungeons;
}

void LfgPlayerData::ClearSelectedDungeons()
{
    m_SelectedDungeons.clear();
}

LfgState LfgPlayerData::GetState() const
{
    return m_State;
}

const LfgLockMap & LfgPlayerData::GetLockedDungeons() const
{
    return m_LockedDungeons;
}

uint8 LfgPlayerData::GetRoles() const
{
    return m_Roles;
}

const std::string& LfgPlayerData::GetComment() const
{
    return m_Comment;
}

const LfgDungeonSet& LfgPlayerData::GetSelectedDungeons() const
{
    return m_SelectedDungeons;
}

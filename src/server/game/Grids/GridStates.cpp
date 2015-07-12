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

#include "GridStates.h"
#include "GridNotifiers.h"
#include "Grid.h"
#include "Log.h"

void InvalidState::Update(Map &, NGridType &, GridInfo &, const uint32 &/*x*/, const uint32 &/*y*/, const uint32 &) const { }

void ActiveState::Update(Map &m, NGridType &grid, GridInfo & info, const uint32 &x, const uint32 &y, const uint32 &t_diff) const
{
    // Only check grid activity every (grid_expiry/10) ms, because it's really useless to do it every cycle
    info.UpdateTimeTracker(t_diff);
    if(info.getTimeTracker().Passed())
    {
        if(grid.ActiveObjectsInGrid() == 0 && !m.ActiveObjectsNearGrid(x, y))
        {
            ObjectGridStoper stoper(grid);
            stoper.StopN();
            grid.SetGridState(GRID_STATE_IDLE);
            sLog->outDebug(LOG_FILTER_MAPS, "Grid[%u, %u] on map %u moved to IDLE state", x, y, m.GetId());
        }
        else
        {
            m.ResetGridExpiry(grid, 0.1f);
        }
    }
}

void
IdleState::Update(Map &m, NGridType &grid, GridInfo &, const uint32 &x, const uint32 &y, const uint32 &) const
{
    m.ResetGridExpiry(grid);
    grid.SetGridState(GRID_STATE_REMOVAL);
    sLog->outDebug(LOG_FILTER_MAPS, "Grid[%u, %u] on map %u moved to REMOVAL state", x, y, m.GetId());
}

void
RemovalState::Update(Map &m, NGridType &grid, GridInfo &info, const uint32 &x, const uint32 &y, const uint32 &t_diff) const
{
    if(!info.getUnloadLock())
    {
        info.UpdateTimeTracker(t_diff);
        if(info.getTimeTracker().Passed())
        {
            if(!m.UnloadGrid(x, y, false))
            {
                sLog->outDebug(LOG_FILTER_MAPS, "Grid[%u, %u] for map %u is not unloaded due to players or active objects nearby", x, y, m.GetId());
                m.ResetGridExpiry(grid);
            }
        }
    }
}


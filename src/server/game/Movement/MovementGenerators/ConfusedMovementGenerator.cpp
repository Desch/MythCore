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

#include "Creature.h"
#include "MapManager.h"
#include "ConfusedMovementGenerator.h"
#include "VMapFactory.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "PathFinder.h"

#ifdef MAP_BASED_RAND_GEN
#define rand_norm() unit.rand_norm()
#define urand(a, b) unit.urand(a, b)
#endif

template<class T>
void ConfusedMovementGenerator<T>::Initialize(T &unit)
{
    const float wander_distance = 4;
    float x, y, z;
    x = unit.GetPositionX();
    y = unit.GetPositionY();
    z = unit.GetPositionZ();

    Map const* map = unit.GetBaseMap();

    i_nextMove = 1;

    bool is_water_ok, is_land_ok;
    _InitSpecific(unit, is_water_ok, is_land_ok);

    for(uint8 idx = 0; idx < MAX_CONF_WAYPOINTS + 1; ++idx)
    {
        float wanderX = x + wander_distance*(float)rand_norm() - wander_distance/2;
        float wanderY = y + wander_distance*(float)rand_norm() - wander_distance/2;
        Trinity::NormalizeMapCoord(wanderX);
        Trinity::NormalizeMapCoord(wanderY);

        if(unit.IsWithinLOS(wanderX, wanderY, z))
        {
            bool is_water = map->IsInWater(wanderX, wanderY, z);

            if((is_water && !is_water_ok) || (!is_water && !is_land_ok))
            {
                //! Cannot use coordinates outside our InhabitType. Use the current or previous position.
                wanderX = idx > 0 ? i_waypoints[idx-1][0] : x;
                wanderY = idx > 0 ? i_waypoints[idx-1][1] : y;
            }
        }
        else
        {
            //! Trying to access path outside line of sight. Skip this by using the current or previous position.
            wanderX = idx > 0 ? i_waypoints[idx-1][0] : x;
            wanderY = idx > 0 ? i_waypoints[idx-1][1] : y;
        }

        unit.UpdateAllowedPositionZ(wanderX, wanderY, z);

        //! Positions are fine - apply them to this waypoint
        i_waypoints[idx][0] = wanderX;
        i_waypoints[idx][1] = wanderY;
        i_waypoints[idx][2] = z;
    }

    unit.StopMoving();
    unit.SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED);
    unit.AddUnitState(UNIT_STAT_CONFUSED | UNIT_STAT_CONFUSED_MOVE);
}

template<>
void ConfusedMovementGenerator<Creature>::_InitSpecific(Creature &creature, bool &is_water_ok, bool &is_land_ok)
{
    is_water_ok = creature.canSwim();
    is_land_ok  = creature.canWalk();
}

template<>
void ConfusedMovementGenerator<Player>::_InitSpecific(Player &, bool &is_water_ok, bool &is_land_ok)
{
    is_water_ok = true;
    is_land_ok  = true;
}

template<class T>
void ConfusedMovementGenerator<T>::Reset(T &unit)
{
    i_nextMove = 1;
    i_nextMoveTime.Reset(0);
    unit.StopMoving();
    unit.AddUnitState(UNIT_STAT_CONFUSED | UNIT_STAT_CONFUSED_MOVE);
}

template<class T>
bool ConfusedMovementGenerator<T>::Update(T &unit, const uint32 &diff)
{
    if(unit.HasUnitState(UNIT_STAT_ROOT | UNIT_STAT_STUNNED | UNIT_STAT_DISTRACTED))
        return true;

    if(i_nextMoveTime.Passed())
    {
        // currently moving, update location
        unit.AddUnitState(UNIT_STAT_CONFUSED_MOVE);

        if(unit.movespline->Finalized())
        {
            i_nextMove = urand(1, MAX_CONF_WAYPOINTS);
            i_nextMoveTime.Reset(urand(500, 1200)); // Guessed
        }
    }
    else
    {
        // waiting for next move
        i_nextMoveTime.Update(diff);
        if(i_nextMoveTime.Passed())
        {
            // start moving
            unit.AddUnitState(UNIT_STAT_CONFUSED_MOVE);

            ASSERT(i_nextMove <= MAX_CONF_WAYPOINTS);
            float x = i_waypoints[i_nextMove][0];
            float y = i_waypoints[i_nextMove][1];
            float z = i_waypoints[i_nextMove][2];

            PathFinder path(&unit);
            path.setPathLengthLimit(30.0f);
            path.calculate(x, y, z);
            if(path.getPathType() & PATHFIND_NOPATH)
            {
                i_nextMoveTime.Reset(urand(800, 1000));
                return true;
            }

            Movement::MoveSplineInit init(unit);
            init.MovebyPath(path.getPath());
            init.SetWalk(true);
            init.Launch();
        }
    }

    return true;
}

template<>
void ConfusedMovementGenerator<Player>::Finalize(Player &unit)
{
    unit.RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED);
    unit.ClearUnitState(UNIT_STAT_CONFUSED | UNIT_STAT_CONFUSED_MOVE);
}

template<>
void ConfusedMovementGenerator<Creature>::Finalize(Creature &unit)
{
    unit.RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED);
    unit.ClearUnitState(UNIT_STAT_CONFUSED | UNIT_STAT_CONFUSED_MOVE);
    if(unit.getVictim())
        unit.SetTarget(unit.getVictim()->GetGUID());
}

template void ConfusedMovementGenerator<Player>::Initialize(Player &player);
template void ConfusedMovementGenerator<Creature>::Initialize(Creature &creature);
template void ConfusedMovementGenerator<Player>::Reset(Player &player);
template void ConfusedMovementGenerator<Creature>::Reset(Creature &creature);
template bool ConfusedMovementGenerator<Player>::Update(Player &player, const uint32 &diff);
template bool ConfusedMovementGenerator<Creature>::Update(Creature &creature, const uint32 &diff);
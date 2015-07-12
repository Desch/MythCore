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
#include "RandomMovementGenerator.h"
#include "ObjectAccessor.h"
#include "Map.h"
#include "Util.h"
#include "CreatureGroups.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"

#define RUNNING_CHANCE_RANDOMMV 20                                  //will be "1 / RUNNING_CHANCE_RANDOMMV"

#ifdef MAP_BASED_RAND_GEN
#define rand_norm() creature.rand_norm()
#endif

template<>
void RandomMovementGenerator<Creature>::_setRandomLocation(Creature &creature)
{
    float respX, respY, respZ, respO, destX, destY, destZ, travelDistZ;
    creature.GetHomePosition(respX, respY, respZ, respO);
    Map const* map = creature.GetBaseMap();

    // For 2D/3D system selection
    //bool is_land_ok  = creature.CanWalk();                // not used?
    //bool is_water_ok = creature.CanSwim();                // not used?
    bool is_air_ok = creature.canFly();

    const float angle = float(rand_norm()) * static_cast<float>(M_PI*2);
    const float range = float(rand_norm()) * wander_distance;
    const float distanceX = range * cos(angle);
    const float distanceY = range * sin(angle);

    destX = respX + distanceX;
    destY = respY + distanceY;

    // prevent invalid coordinates generation
    Trinity::NormalizeMapCoord(destX);
    Trinity::NormalizeMapCoord(destY);

    travelDistZ = distanceX*distanceX + distanceY*distanceY;

    if(is_air_ok)                                          // 3D system above ground and above water (flying mode)
    {
        // Limit height change
        const float distanceZ = float(rand_norm()) * sqrtf(travelDistZ)/2.0f;
        destZ = respZ + distanceZ;
        float levelZ = map->GetWaterOrGroundLevel(destX, destY, destZ-2.0f);

        // Problem here, we must fly above the ground and water, not under. Let's try on next tick
        if(levelZ >= destZ)
            return;
    }
    //else if(is_water_ok)                                 // 3D system under water and above ground (swimming mode)
    else                                                    // 2D only
    {
        // 10.0 is the max that vmap high can check (MAX_CAN_FALL_DISTANCE)
        travelDistZ = travelDistZ >= 100.0f ? 10.0f : sqrtf(travelDistZ);

        // The fastest way to get an accurate result 90% of the time.
        // Better result can be obtained like 99% accuracy with a ray light, but the cost is too high and the code is too long.
        destZ = map->GetHeight(creature.GetPhaseMask(), destX, destY, respZ+travelDistZ-2.0f, false);

        if(fabs(destZ - respZ) > travelDistZ)              // Map check
        {
            // Vmap Horizontal or above
            destZ = map->GetHeight(creature.GetPhaseMask(), destX, destY, respZ - 2.0f, true);

            if(fabs(destZ - respZ) > travelDistZ)
            {
                // Vmap Higher
                destZ = map->GetHeight(creature.GetPhaseMask(), destX, destY, respZ+travelDistZ-2.0f, true);

                // let's forget this bad coords where a z cannot be found and retry at the next tick
                if(fabs(destZ - respZ) > travelDistZ)
                    return;
            }
        }
    }

    if(is_air_ok)
        i_nextMoveTime.Reset(0);
    else
        i_nextMoveTime.Reset(urand(500, 10000));

    creature.AddUnitState(UNIT_STAT_ROAMING_MOVE);

    Movement::MoveSplineInit init(creature);
    init.MoveTo(destX, destY, destZ, true);
    init.SetWalk(true);
    init.Launch();

    //Call for creature group update
    if(creature.GetFormation() && creature.GetFormation()->getLeader() == &creature)
        creature.GetFormation()->LeaderMoveTo(destX, destY, destZ);
}

template<>
void RandomMovementGenerator<Creature>::Initialize(Creature &creature)
{
    if(!creature.isAlive())
        return;

    if(!wander_distance)
        wander_distance = creature.GetRespawnRadius();

    creature.AddUnitState(UNIT_STAT_ROAMING|UNIT_STAT_ROAMING_MOVE);
    _setRandomLocation(creature);
}

template<>
void RandomMovementGenerator<Creature>::Reset(Creature &creature)
{
    Initialize(creature);
}

template<>
void RandomMovementGenerator<Creature>::Finalize(Creature &creature)
{
    creature.ClearUnitState(UNIT_STAT_ROAMING|UNIT_STAT_ROAMING_MOVE);
    creature.SetWalk(false);
}

template<>
bool RandomMovementGenerator<Creature>::Update(Creature &creature, const uint32 &diff)
{
    if(creature.HasUnitState(UNIT_STAT_ROOT | UNIT_STAT_STUNNED | UNIT_STAT_DISTRACTED))
    {
        i_nextMoveTime.Reset(0);  // Expire the timer
        creature.ClearUnitState(UNIT_STAT_ROAMING_MOVE);
        return true;
    }

    if(creature.movespline->Finalized())
    {
        i_nextMoveTime.Update(diff);
        if(i_nextMoveTime.Passed())
            _setRandomLocation(creature);
    }
    return true;
}

template<>
bool RandomMovementGenerator<Creature>::GetResetPosition(Creature &creature, float& x, float& y, float& z)
{
    float radius;
    creature.GetRespawnCoord(x, y, z, NULL, &radius);

    // use current if in range
    if(creature.IsWithinDist2d(x,y,radius))
        creature.GetPosition(x,y,z);

    return true;
}

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

#ifndef TRINITY_FLEEINGMOVEMENTGENERATOR_H
#define TRINITY_FLEEINGMOVEMENTGENERATOR_H

#include "MovementGenerator.h"

/*
* Fleeing movement generator used in case of spells like fear,
* where we need to make random movements in range between M to N yards away from caster.
*/

template<class T>
class FleeingMovementGenerator
: public MovementGeneratorMedium< T, FleeingMovementGenerator<T> >
{
    public:
        FleeingMovementGenerator(uint64 fright) : i_frightGUID(fright), i_nextCheckTime(0) { }

        void Initialize(T &);
        void Finalize(T &);
        void Reset(T &);
        bool Update(T &, const uint32 &);

        MovementGeneratorType GetMovementGeneratorType() { return FLEEING_MOTION_TYPE; }

    private:
        void _setTargetLocation(T &owner);
        bool _getPoint(T &owner, float &x, float &y, float &z);
        bool _setMoveData(T &owner);
        void _Init(T &);

        bool is_water_ok   :1;
        bool is_land_ok    :1;
        bool i_only_forward:1;

        float i_caster_x;
        float i_caster_y;
        float i_caster_z;
        float i_last_distance_from_caster;
        float i_to_distance_from_caster;
        float i_cur_angle;
        uint64 i_frightGUID;
        TimeTracker i_nextCheckTime;
};

class TimedFleeingMovementGenerator
: public FleeingMovementGenerator<Creature>
{
    public:
        TimedFleeingMovementGenerator(uint64 fright, uint32 time) :
            FleeingMovementGenerator<Creature>(fright),
            i_totalFleeTime(time) { }

        MovementGeneratorType GetMovementGeneratorType() { return TIMED_FLEEING_MOTION_TYPE; }
        bool Update(Unit &, const uint32 &);
        void Finalize(Unit &);

    private:
        TimeTracker i_totalFleeTime;
};

#endif


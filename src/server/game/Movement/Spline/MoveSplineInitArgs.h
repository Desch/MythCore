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

#ifndef TRINITYSERVER_MOVESPLINEINIT_ARGS_H
#define TRINITYSERVER_MOVESPLINEINIT_ARGS_H

#include "MoveSplineFlag.h"
#include <G3D/Vector3.h>

namespace Movement
{
    typedef std::vector<Vector3> PointsArray;

    union FacingInfo
    {
        struct{
            float x,y,z;
        }f;
        uint64  target;
        float   angle;

        FacingInfo(float o) : angle(o) { }
        FacingInfo(uint64 t) : target(t) { }
        FacingInfo() { }
    };

    struct MoveSplineInitArgs
    {
        MoveSplineInitArgs(size_t path_capacity = 16) : path_Idx_offset(0),
            velocity(0.f), parabolic_amplitude(0.f), time_perc(0.f), splineId(0), initialOrientation(0.f), HasVelocity(false)
        {
            path.reserve(path_capacity);
        }

        PointsArray path;
        FacingInfo facing;
        MoveSplineFlag flags;
        int32 path_Idx_offset;
        float velocity;
        float parabolic_amplitude;
        float time_perc;
        uint32 splineId;
        float initialOrientation;
        bool HasVelocity;

        /** Returns true to show that the arguments were configured correctly and MoveSpline initialization will succeed. */
        bool Validate() const;
    private:
        bool _checkPathBounds() const;
    };
}

#endif // TRINITYSERVER_MOVESPLINEINIT_ARGS_H

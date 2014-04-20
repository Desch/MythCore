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

#ifndef TRINITYSERVER_MOVESPLINEINIT_H
#define TRINITYSERVER_MOVESPLINEINIT_H

#include "MoveSplineInitArgs.h"
#include "PathFinder.h"

class Unit;

namespace Movement
{
    enum AnimType
    {
        ToGround    = 0, // 460 = ToGround, index of AnimationData.dbc
        FlyToFly    = 1, // 461 = FlyToFly?
        ToFly       = 2, // 458 = ToFly
        FlyToGround = 3, // 463 = FlyToGround
    };

    /*  Initializes and launches spline movement
     */
    class MoveSplineInit
    {
    public:
        explicit MoveSplineInit(Unit& m);

        /*  Final pass of initialization that launches spline movement.
         * @return duration - estimated travel time
         */
        int32 Launch();

        /* Adds movement by parabolic trajectory
         * @param amplitude  - the maximum height of parabola, value could be negative and positive
         * @param start_time - delay between movement starting time and beginning to move by parabolic trajectory
         * can't be combined with final animation
         */
        void SetParabolic(float amplitude, float start_time);
        /* Plays animation after movement done
         * can't be combined with parabolic movement
         */
        void SetAnimation(AnimType anim);

        /* Adds final facing animation
         * sets unit's facing to specified point/angle after all path done
         * you can have only one final facing: previous will be overriden
         */
        void SetFacing(float angle);
        void SetFacing(Vector3 const& point);
        void SetFacing(const Unit* target);

        /* Initializes movement by path
         * @param path - array of points, shouldn't be empty
         * @param pointId - Id of fisrt point of the path. Example: when third path point will be done it will notify that pointId + 3 done
         */
        void MovebyPath(const PointsArray& path, int32 pointId = 0);

        /* Initializes simple A to B mition, A is current unit's position, B is destination
         */
        void MoveTo(const Vector3& destination, bool generatePath = false, bool forceDestination = false);
        void MoveTo(float x, float y, float z, bool generatePath = false, bool forceDestination = false);

        /* Sets Id of fisrt point of the path. When N-th path point will be done ILisener will notify that pointId + N done
         * Needed for waypoint movement where path splitten into parts
         */
        void SetFirstPointId(int32 pointId) { args.path_Idx_offset = pointId; }

        /* Enables CatmullRom spline interpolation mode(makes path smooth)
         * if not enabled linear spline mode will be choosen. Disabled by default
         */
        void SetSmooth();
        /* Enables CatmullRom spline interpolation mode, enables flying animation. Disabled by default
         */
        void SetFly();
        /* Enables walk mode. Disabled by default
         */
        void SetWalk(bool enable);
        /* Makes movement cyclic. Disabled by default
         */
        void SetCyclic();
        /* Enables falling mode. Disabled by default
         */
        void SetFall();
        /* Inverses unit model orientation. Disabled by default
         */
        void SetOrientationInversed();
        /* Fixes unit's model rotation. Disabled by default
         */
        void SetOrientationFixed(bool enable);

        /* Sets the velocity (in case you want to have custom movement velocity)
         * if no set, speed will be selected based on unit's speeds and current movement mode
         * Has no effect if falling mode enabled
         * velocity shouldn't be negative
         */
        void SetVelocity(float velocity);

        PointsArray& Path() { return args.path; }

    protected:
        MoveSplineInitArgs args;
        Unit&  unit;
    };

    inline void MoveSplineInit::SetFly() { args.flags.EnableFlying();}
    inline void MoveSplineInit::SetWalk(bool enable) { args.flags.walkmode = enable;}
    inline void MoveSplineInit::SetSmooth() { args.flags.EnableCatmullRom();}
    inline void MoveSplineInit::SetCyclic() { args.flags.cyclic = true;}
    inline void MoveSplineInit::SetFall() { args.flags.EnableFalling();}
    inline void MoveSplineInit::SetVelocity(float vel){ args.velocity = vel; args.HasVelocity = true; }
    inline void MoveSplineInit::SetOrientationInversed() { args.flags.orientationInversed = true;}
    inline void MoveSplineInit::SetOrientationFixed(bool enable) { args.flags.orientationFixed = enable;}

    inline void MoveSplineInit::MovebyPath(const PointsArray& controls, int32 path_offset)
    {
        args.path_Idx_offset = path_offset;
        args.path.assign(controls.begin(),controls.end());
    }

    inline void MoveSplineInit::MoveTo(float x, float y, float z, bool generatePath, bool forceDestination)
    {
        Vector3 v(x,y,z);
        MoveTo(v, generatePath, forceDestination);
    }

    inline void MoveSplineInit::MoveTo(const Vector3& dest, bool generatePath, bool forceDestination)
    {
        if(generatePath)
        {
            PathFinder path(&unit);
            path.calculate(dest.x, dest.y, dest.z, forceDestination);
            MovebyPath(path.getPath());
        } else {
            args.path_Idx_offset = 0;
            args.path.resize(2);
            args.path[1] = dest;
        }
    }

    inline void MoveSplineInit::SetParabolic(float amplitude, float time_shift)
    {
        args.time_perc = time_shift;
        args.parabolic_amplitude = amplitude;
        args.flags.EnableParabolic();
    }

    inline void MoveSplineInit::SetAnimation(AnimType anim)
    {
        args.time_perc = 0.f;
        args.flags.EnableAnimation((uint8)anim);
    }

    inline void MoveSplineInit::SetFacing(Vector3 const& spot)
    {
        args.facing.f.x = spot.x;
        args.facing.f.y = spot.y;
        args.facing.f.z = spot.z;
        args.flags.EnableFacingPoint();
    }
}
#endif // TRINITYSERVER_MOVESPLINEINIT_H

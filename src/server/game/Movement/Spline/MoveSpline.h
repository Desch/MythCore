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

#ifndef TRINITYSERVER_MOVEPLINE_H
#define TRINITYSERVER_MOVEPLINE_H

#include "Spline.h"
#include "MoveSplineInitArgs.h"

namespace Movement
{
    struct Location : public Vector3
    {
        Location() : orientation(0) { }
        Location(float x, float y, float z, float o) : Vector3(x,y,z), orientation(o) { }
        Location(const Vector3& v) : Vector3(v), orientation(0) { }
        Location(const Vector3& v, float o) : Vector3(v), orientation(o) { }

        float orientation;
    };

    // MoveSpline represents smooth catmullrom or linear curve and point that moves belong it
    // curve can be cyclic - in this case movement will be cyclic
    // point can have vertical acceleration motion componemt(used in fall, parabolic movement)
    class MoveSpline
    {
    public:
        typedef Spline<int32> MySpline;
        enum UpdateResult{
            Result_None         = 0x01,
            Result_Arrived      = 0x02,
            Result_NextCycle    = 0x04,
            Result_NextSegment  = 0x08,
        };
        friend class PacketBuilder;
    protected:
        MySpline        spline;

        FacingInfo      facing;

        uint32          m_Id;

        MoveSplineFlag  splineflags;

        int32           time_passed;
        // currently duration mods are unused, but its _currently_
        //float           duration_mod;
        //float           duration_mod_next;
        float           vertical_acceleration;
        float           initialOrientation;
        int32           effect_start_time;
        int32           point_Idx;
        int32           point_Idx_offset;

        void init_spline(const MoveSplineInitArgs& args);
    protected:
        const MySpline::ControlArray& getPath() const { return spline.getPoints();}
        void computeParabolicElevation(float& el) const;
        void computeFallElevation(float& el) const;

        UpdateResult _updateState(int32& ms_time_diff);
        int32 next_timestamp() const { return spline.length(point_Idx+1);}
        int32 segment_time_elapsed() const { return next_timestamp()-time_passed;}
        int32 timeElapsed() const { return Duration() - time_passed;}
        int32 timePassed() const { return time_passed;}

    public:
        const MySpline& _Spline() const { return spline;}
        int32 _currentSplineIdx() const { return point_Idx;}
        void _Finalize();
        void _Interrupt() { splineflags.done = true;}
        void Initialize(const MoveSplineInitArgs&);
        bool Initialized() const { return !spline.empty();}

        explicit MoveSpline();

        template<class UpdateHandler>
        void updateState(int32 difftime, UpdateHandler& handler)
        {
            ASSERT(Initialized());
            do
                handler(_updateState(difftime));
            while(difftime > 0);
        }

        void updateState(int32 difftime)
        {
            ASSERT(Initialized());
            do _updateState(difftime);
            while(difftime > 0);
        }

        Location ComputePosition() const;

        uint32 GetId() const { return m_Id;}
        bool Finalized() const { return splineflags.done; }
        bool isCyclic() const { return splineflags.cyclic;}
        const Vector3 FinalDestination() const { return Initialized() ? spline.getPoint(spline.last()) : Vector3();}
        const Vector3 CurrentDestination() const { return Initialized() ? spline.getPoint(point_Idx+1) : Vector3();}
        int32 currentPathIdx() const;

        int32 Duration() const { return spline.length();}
        std::string ToString() const;
    };
}
#endif // TRINITYSERVER_MOVEPLINE_H

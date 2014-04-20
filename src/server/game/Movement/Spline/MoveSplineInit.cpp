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

#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "MovementPacketBuilder.h"
#include "Unit.h"

namespace Movement
{
    UnitMoveType SelectSpeedType(uint32 moveFlags)
    {
        /*! Not sure about MOVEMENTFLAG_CAN_FLY here - do creatures that can fly
            but are on ground right now also have it? If yes, this needs a more
            dynamic check, such as is flying now
        */
        if(moveFlags & (MOVEMENTFLAG_FLYING | MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_LEVITATING))
        {
            if(moveFlags & MOVEMENTFLAG_BACKWARD /*&& speed_obj.flight >= speed_obj.flight_back*/)
                return MOVE_FLIGHT_BACK;
            else
                return MOVE_FLIGHT;
        }
        else if(moveFlags & MOVEMENTFLAG_SWIMMING)
        {
            if(moveFlags & MOVEMENTFLAG_BACKWARD /*&& speed_obj.swim >= speed_obj.swim_back*/)
                return MOVE_SWIM_BACK;
            else
                return MOVE_SWIM;
        }
        else if(moveFlags & MOVEMENTFLAG_WALKING)
        {
            //if(speed_obj.run > speed_obj.walk)
            return MOVE_WALK;
        }
        else if(moveFlags & MOVEMENTFLAG_BACKWARD /*&& speed_obj.run >= speed_obj.run_back*/)
            return MOVE_RUN_BACK;

        return MOVE_RUN;
    }

    int32 MoveSplineInit::Launch()
    {
        MoveSpline& move_spline = *unit.movespline;

        Location real_position(unit.GetPositionX(),unit.GetPositionY(),unit.GetPositionZMinusOffset(),unit.GetOrientation());
        // there is a big chane that current position is unknown if current state is not finalized, need compute it
        // this also allows calculate spline position and update map position in much greater intervals
        if(!move_spline.Finalized())
            real_position = move_spline.ComputePosition();

        if(args.path.empty())
        {
            // should i do the things that user should do?
            MoveTo(real_position);
        }

        // corrent first vertex
        args.path[0] = real_position;
        args.initialOrientation = real_position.orientation;

        uint32 moveFlags = unit.m_movementInfo.GetMovementFlags();
        if(args.flags.walkmode)
            moveFlags |= MOVEMENTFLAG_WALKING;
        else
            moveFlags &= ~MOVEMENTFLAG_WALKING;

        moveFlags |= (MOVEMENTFLAG_SPLINE_ENABLED|MOVEMENTFLAG_FORWARD);

        if(!args.HasVelocity)
            args.velocity = unit.GetSpeed(SelectSpeedType(moveFlags));

        if(!args.Validate())
            return 0;

        if(moveFlags & MOVEMENTFLAG_ROOT)
            moveFlags &= ~MOVEMENTFLAG_MASK_MOVING;

        unit.m_movementInfo.SetMovementFlags((MovementFlags)moveFlags);
        move_spline.Initialize(args);

        WorldPacket data(SMSG_MONSTER_MOVE, 64);
        data.append(unit.GetPackGUID());
        PacketBuilder::WriteMonsterMove(move_spline, data);
        unit.SendMessageToSet(&data,true);
        return move_spline.Duration();
    }

    MoveSplineInit::MoveSplineInit(Unit& m) : unit(m)
    {
        // mix existing state into new
        args.flags.walkmode = unit.m_movementInfo.HasMovementFlag(MOVEMENTFLAG_WALKING);
        args.flags.flying = unit.m_movementInfo.HasMovementFlag((MovementFlags)(MOVEMENTFLAG_CAN_FLY|MOVEMENTFLAG_LEVITATING));
    }

    void MoveSplineInit::SetFacing(const Unit* target)
    {
        args.flags.EnableFacingTarget();
        //args.facing.target = target->GetObjectGuid().GetRawValue();
        args.facing.target = target->GetUInt64Value(OBJECT_FIELD_GUID);
    }

    void MoveSplineInit::SetFacing(float angle)
    {
        args.facing.angle = G3D::wrap(angle, 0.f, (float)G3D::twoPi());
        args.flags.EnableFacingAngle();
    }
}

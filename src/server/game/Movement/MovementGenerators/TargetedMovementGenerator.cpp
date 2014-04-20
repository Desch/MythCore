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

#include "ByteBuffer.h"
#include "TargetedMovementGenerator.h"
#include "Errors.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "World.h"

#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "PathFinder.h"

#include <cmath>

template<class T, typename D>
void TargetedMovementGeneratorMedium<T, D>::_setTargetLocation(T &owner)
{
    if(!i_target.isValid() || !i_target->IsInWorld())
        return;

    if(owner.HasUnitState(UNIT_STAT_NOT_MOVE))
        return;

    float x, y, z = 0.0f;
    //! Following block of code deleted by MrSmite in issue 4891
    //! Code kept for learning and diagnostical purposes
    if(!i_offset)
    {
        if(i_target->IsWithinMeleeRange(&owner))
            return;

        // to nearest random contact position
        i_target->GetRandomContactPoint(&owner, x, y, z, 0, MELEE_RANGE - 0.5f);
    }
    else
    {
        if(i_target->IsWithinDistInMap(&owner, i_offset + 1.0f))
            return;
        // to at i_offset distance from target and i_angle from target facing
        i_target->GetClosePoint(x, y, z, owner.GetObjectSize(), i_offset, i_angle);
    }

    if(!i_path)
        i_path = new PathFinder(&owner);

    // allow pets following their master to cheat while generating paths
    bool forceDest = (owner.GetTypeId() == TYPEID_UNIT && (&owner)->ToCreature()->isPet()
                        && owner.HasUnitState(UNIT_STAT_FOLLOW));

    i_path->calculate(x, y, z, forceDest);
    if(i_path->getPathType() & PATHFIND_NOPATH)
        return;

    D::_addUnitStateMove(owner);
    i_targetReached = false;
    i_recalculateTravel = false;

    Movement::MoveSplineInit init(owner);
    init.MovebyPath(i_path->getPath());
    init.SetWalk(((D*)this)->EnableWalking());
    init.Launch();
}

template<>
void TargetedMovementGeneratorMedium<Player,ChaseMovementGenerator<Player> >::UpdateFinalDistance(float /*fDistance*/)
{
    // nothing to do for Player
}

template<>
void TargetedMovementGeneratorMedium<Player,FollowMovementGenerator<Player> >::UpdateFinalDistance(float /*fDistance*/)
{
    // nothing to do for Player
}

template<>
void TargetedMovementGeneratorMedium<Creature,ChaseMovementGenerator<Creature> >::UpdateFinalDistance(float fDistance)
{
    i_offset = fDistance;
    i_recalculateTravel = true;
}

template<>
void TargetedMovementGeneratorMedium<Creature,FollowMovementGenerator<Creature> >::UpdateFinalDistance(float fDistance)
{
    i_offset = fDistance;
    i_recalculateTravel = true;
}

template<class T, typename D>
bool TargetedMovementGeneratorMedium<T,D>::Update(T &owner, const uint32 & time_diff)
{
    if(!i_target.isValid() || !i_target->IsInWorld())
        return false;

    if(!owner.isAlive())
        return true;

    if(owner.HasUnitState(UNIT_STAT_NOT_MOVE))
    {
        D::_clearUnitStateMove(owner);
        return true;
    }

    // prevent movement while casting spells with cast time or channel time
    if(owner.HasUnitState(UNIT_STAT_CASTING))
    {
        if(!owner.IsStopped())
        {
            switch(owner.GetEntry())
            {
                case 36633: // Ice Sphere (Lich King)
                case 37562: // Volatile Ooze and Gas Cloud (Putricide)
                case 37697:
                    break;
                default:
                    owner.StopMoving();
            }
        }
        return true;
    }

    // prevent crash after creature killed pet
    if(static_cast<D*>(this)->_lostTarget(owner))
    {
        D::_clearUnitStateMove(owner);
        return true;
    }

    i_recheckDistance.Update(time_diff);
    if(i_recheckDistance.Passed())
    {
        i_recheckDistance.Reset(50);
        //More distance let have better performance, less distance let have more sensitive reaction at target move.
        float allowed_dist = (owner.GetObjectSize() - 0.5f) + sWorld->getRate(RATE_TARGET_POS_RECALCULATION_RANGE);

        G3D::Vector3 dest = owner.movespline->FinalDestination();

        bool targetMoved = false;
        if(owner.GetTypeId() == TYPEID_UNIT && (&owner)->ToCreature()->canFly())
            targetMoved = !i_target->IsWithinDist3d(dest.x, dest.y, dest.z, allowed_dist);
        else
            targetMoved = !i_target->IsWithinDist2d(dest.x, dest.y, allowed_dist);

        if(targetMoved)
            _setTargetLocation(owner);
    }

    if(owner.movespline->Finalized())
    {
        static_cast<D*>(this)->MovementInform(owner);
        if(i_angle == 0.f && !owner.HasInArc(0.01f, i_target.getTarget()))
            owner.SetInFront(i_target.getTarget());

        if(!i_targetReached)
        {
            i_targetReached = true;
            static_cast<D*>(this)->_reachTarget(owner);
        }
    }
    else
    {
        if(i_recalculateTravel)
            _setTargetLocation(owner);
    }
    return true;
}

//-----------------------------------------------//
template<class T>
void ChaseMovementGenerator<T>::_reachTarget(T &owner)
{
    if(owner.IsWithinMeleeRange(this->i_target.getTarget()))
        owner.Attack(this->i_target.getTarget(),true);
}

template<>
void ChaseMovementGenerator<Player>::Initialize(Player &owner)
{
    owner.AddUnitState(UNIT_STAT_CHASE|UNIT_STAT_CHASE_MOVE);
    _setTargetLocation(owner);
}

template<>
void ChaseMovementGenerator<Creature>::Initialize(Creature &owner)
{
    owner.SetWalk(false);
    owner.AddUnitState(UNIT_STAT_CHASE|UNIT_STAT_CHASE_MOVE);
    _setTargetLocation(owner);
}

template<class T>
void ChaseMovementGenerator<T>::Finalize(T &owner)
{
    owner.ClearUnitState(UNIT_STAT_CHASE|UNIT_STAT_CHASE_MOVE);
}

template<class T>
void ChaseMovementGenerator<T>::Reset(T &owner)
{
    Initialize(owner);
}

template<class T>
void ChaseMovementGenerator<T>::MovementInform(T & /*unit*/)
{
}

template<>
void ChaseMovementGenerator<Creature>::MovementInform(Creature &unit)
{
    // Pass back the GUIDLow of the target. If it is pet's owner then PetAI will handle
    if(unit.AI())
        unit.AI()->MovementInform(CHASE_MOTION_TYPE, i_target.getTarget()->GetGUIDLow());
}

//-----------------------------------------------//
template<>
bool FollowMovementGenerator<Creature>::EnableWalking() const
{
    return i_target.isValid() && i_target->IsWalking();
}

template<>
bool FollowMovementGenerator<Player>::EnableWalking() const
{
    return false;
}

template<>
void FollowMovementGenerator<Player>::_updateSpeed(Player &/*u*/)
{
    // nothing to do for Player
}

template<>
void FollowMovementGenerator<Creature>::_updateSpeed(Creature &u)
{
    // pet only sync speed with owner
    if(!((Creature&)u).isPet() || !i_target.isValid() || i_target->GetGUID() != u.GetOwnerGUID())
        return;

    u.UpdateSpeed(MOVE_RUN,true);
    u.UpdateSpeed(MOVE_WALK,true);
    u.UpdateSpeed(MOVE_SWIM,true);
}

template<>
void FollowMovementGenerator<Player>::Initialize(Player &owner)
{
    owner.AddUnitState(UNIT_STAT_FOLLOW|UNIT_STAT_FOLLOW_MOVE);
    _updateSpeed(owner);
    _setTargetLocation(owner);
}

template<>
void FollowMovementGenerator<Creature>::Initialize(Creature &owner)
{
    owner.AddUnitState(UNIT_STAT_FOLLOW|UNIT_STAT_FOLLOW_MOVE);
    _updateSpeed(owner);
    _setTargetLocation(owner);
}

template<class T>
void FollowMovementGenerator<T>::Finalize(T &owner)
{
    owner.ClearUnitState(UNIT_STAT_FOLLOW|UNIT_STAT_FOLLOW_MOVE);
    _updateSpeed(owner);
}

template<class T>
void FollowMovementGenerator<T>::Reset(T &owner)
{
    Initialize(owner);
}

template<class T>
void FollowMovementGenerator<T>::MovementInform(T & /*unit*/) { }

template<>
void FollowMovementGenerator<Creature>::MovementInform(Creature &unit)
{
    // Pass back the GUIDLow of the target. If it is pet's owner then PetAI will handle
    if(unit.AI())
        unit.AI()->MovementInform(FOLLOW_MOTION_TYPE, i_target.getTarget()->GetGUIDLow());
}

//-----------------------------------------------//
template void TargetedMovementGeneratorMedium<Player,ChaseMovementGenerator<Player> >::_setTargetLocation(Player &);
template void TargetedMovementGeneratorMedium<Player,FollowMovementGenerator<Player> >::_setTargetLocation(Player &);
template void TargetedMovementGeneratorMedium<Creature,ChaseMovementGenerator<Creature> >::_setTargetLocation(Creature &);
template void TargetedMovementGeneratorMedium<Creature,FollowMovementGenerator<Creature> >::_setTargetLocation(Creature &);
template bool TargetedMovementGeneratorMedium<Player,ChaseMovementGenerator<Player> >::Update(Player &, const uint32 &);
template bool TargetedMovementGeneratorMedium<Player,FollowMovementGenerator<Player> >::Update(Player &, const uint32 &);
template bool TargetedMovementGeneratorMedium<Creature,ChaseMovementGenerator<Creature> >::Update(Creature &, const uint32 &);
template bool TargetedMovementGeneratorMedium<Creature,FollowMovementGenerator<Creature> >::Update(Creature &, const uint32 &);

template void ChaseMovementGenerator<Player>::_reachTarget(Player &);
template void ChaseMovementGenerator<Creature>::_reachTarget(Creature &);
template void ChaseMovementGenerator<Player>::Finalize(Player &);
template void ChaseMovementGenerator<Creature>::Finalize(Creature &);
template void ChaseMovementGenerator<Player>::Reset(Player &);
template void ChaseMovementGenerator<Creature>::Reset(Creature &);
template void ChaseMovementGenerator<Player>::MovementInform(Player &unit);

template void FollowMovementGenerator<Player>::Finalize(Player &);
template void FollowMovementGenerator<Creature>::Finalize(Creature &);
template void FollowMovementGenerator<Player>::Reset(Player &);
template void FollowMovementGenerator<Creature>::Reset(Creature &);
template void FollowMovementGenerator<Player>::MovementInform(Player &unit);

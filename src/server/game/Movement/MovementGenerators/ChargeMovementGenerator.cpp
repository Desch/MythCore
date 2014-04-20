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

#include "ChargeMovementGenerator.h"
#include "Errors.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "World.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"

//----- Point Movement Generator
template<class T>
void ChargeMovementGenerator<T>::Initialize(T &unit)
{
    if(!unit.IsStopped())
        unit.StopMoving();

    unit.AddUnitState(UNIT_STAT_ROAMING|UNIT_STAT_ROAMING_MOVE|UNIT_STAT_CHARGING);
    Movement::MoveSplineInit init(unit);
    init.MoveTo(i_x, i_y, i_z, true, true);
    if(speed > 0.0f)
        init.SetVelocity(speed);
    init.Launch();
}

template<class T>
bool ChargeMovementGenerator<T>::Update(T &unit, const uint32 &/*diff*/)
{
    if(!&unit)
        return false;

    if(unit.HasUnitState(UNIT_STAT_ROOT | UNIT_STAT_STUNNED))
    {
        unit.ClearUnitState(UNIT_STAT_ROAMING_MOVE);
        return true;
    }

    unit.AddUnitState(UNIT_STAT_ROAMING_MOVE);
    return !unit.movespline->Finalized();
}

template<class T>
void ChargeMovementGenerator<T>::Finalize(T &unit)
{
    unit.ClearUnitState(UNIT_STAT_ROAMING|UNIT_STAT_ROAMING_MOVE|UNIT_STAT_CHARGING);

    if(targetGUID != 0)
        if(Unit* target = ObjectAccessor::GetUnit(unit, targetGUID))
            unit.SetFacingToObject(target);

    if(unit.movespline->Finalized())
        MovementInform(unit);
}

template<class T>
void ChargeMovementGenerator<T>::Reset(T &unit)
{
    if(!unit.IsStopped())
        unit.StopMoving();

    unit.AddUnitState(UNIT_STAT_ROAMING|UNIT_STAT_ROAMING_MOVE|UNIT_STAT_CHARGING);
}

template<class T>
void ChargeMovementGenerator<T>::MovementInform(T & /*unit*/) { }

template <> void ChargeMovementGenerator<Creature>::MovementInform(Creature &unit)
{
    if(unit.AI())
        unit.AI()->MovementInform(POINT_MOTION_TYPE, id);
}

template void ChargeMovementGenerator<Player>::Initialize(Player&);
template void ChargeMovementGenerator<Creature>::Initialize(Creature&);
template void ChargeMovementGenerator<Player>::Finalize(Player&);
template void ChargeMovementGenerator<Creature>::Finalize(Creature&);
template void ChargeMovementGenerator<Player>::Reset(Player&);
template void ChargeMovementGenerator<Creature>::Reset(Creature&);
template bool ChargeMovementGenerator<Player>::Update(Player &, const uint32 &);
template bool ChargeMovementGenerator<Creature>::Update(Creature&, const uint32 &);

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

#include "HomeMovementGenerator.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "WorldPacket.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"

void HomeMovementGenerator<Creature>::Initialize(Creature &owner)
{
    owner.RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
    owner.AddUnitState(UNIT_STAT_EVADE);
    _setTargetLocation(owner);
}

void HomeMovementGenerator<Creature>::Reset(Creature &) { }

void HomeMovementGenerator<Creature>::_setTargetLocation(Creature &owner)
{
    if(!&owner)
        return;

    if(owner.HasUnitState(UNIT_STAT_ROOT | UNIT_STAT_STUNNED | UNIT_STAT_DISTRACTED))
        return;

    Movement::MoveSplineInit init(owner);
    float x, y, z, o;

    owner.GetHomePosition(x, y, z, o);
    init.SetFacing(o);
    init.MoveTo(x, y, z, true);
    init.SetWalk(false);
    init.Launch();

    owner.ClearUnitState(UNIT_STAT_ALL_STATE & ~UNIT_STAT_EVADE);
}

bool HomeMovementGenerator<Creature>::Update(Creature &owner, const uint32 /*time_diff*/)
{
    return !owner.movespline->Finalized();
}

void HomeMovementGenerator<Creature>::Finalize(Creature &owner)
{
    //owner.ClearUnitState(UNIT_STAT_EVADE);
    owner.SetWalk(true);
    owner.LoadCreaturesAddon(true);
    if(owner.AI())
        owner.AI()->JustReachedHome();
    owner.ClearUnitState(UNIT_STAT_EVADE);
}

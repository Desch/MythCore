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

#ifndef TRINITY_HOMEMOVEMENTGENERATOR_H
#define TRINITY_HOMEMOVEMENTGENERATOR_H

#include "MovementGenerator.h"

class Creature;

template < class T > class HomeMovementGenerator;

template <>
class HomeMovementGenerator<Creature> : public MovementGeneratorMedium<Creature, HomeMovementGenerator<Creature> >
{
public:
    HomeMovementGenerator() { }
    ~HomeMovementGenerator() { }

    void Initialize(Creature &);
    void Finalize(Creature &);
    void Reset(Creature &);
    bool Update(Creature &, const uint32);
    MovementGeneratorType GetMovementGeneratorType() { return HOME_MOTION_TYPE; }

private:
    void _setTargetLocation(Creature &);
};

#endif


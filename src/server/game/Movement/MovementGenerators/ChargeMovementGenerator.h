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

#ifndef TRINITY_CHARGEMOVEMENTGENERATOR_H
#define TRINITY_CHARGEMOVEMENTGENERATOR_H

#include "MovementGenerator.h"
#include "FollowerReference.h"

template<class T>
class ChargeMovementGenerator
: public MovementGeneratorMedium< T, ChargeMovementGenerator<T> >
{
    public:
        ChargeMovementGenerator(uint32 _id, float _x, float _y, float _z, float _speed = 0.0f, uint64 _targetGUID = 0) : id(_id),
            i_x(_x), i_y(_y), i_z(_z), speed(_speed), targetGUID(_targetGUID) { }

        void Initialize(T &);
        void Finalize(T &);
        void Reset(T &);
        bool Update(T &, const uint32 &);

        void MovementInform(T &);

        MovementGeneratorType GetMovementGeneratorType() { return POINT_MOTION_TYPE; }

        bool GetDestination(float& x, float& y, float& z) const { x=i_x; y=i_y; z=i_z; return true; }
    private:
        uint32 id;
        float i_x, i_y, i_z;
        float speed;
        uint64 targetGUID;
};

#endif


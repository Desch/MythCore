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

#ifndef TRINITY_POINTMOVEMENTGENERATOR_H
#define TRINITY_POINTMOVEMENTGENERATOR_H

#include "MovementGenerator.h"
#include "FollowerReference.h"

template<class T>
class PointMovementGenerator
: public MovementGeneratorMedium< T, PointMovementGenerator<T> >
{
    public:
        PointMovementGenerator(uint32 _id, float _x, float _y, float _z, float _speed = 0.0f) : id(_id),
            i_x(_x), i_y(_y), i_z(_z), speed(_speed) { }

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
};

class AssistanceMovementGenerator
: public PointMovementGenerator<Creature>
{
    public:
        AssistanceMovementGenerator(float _x, float _y, float _z) :
            PointMovementGenerator<Creature>(0, _x, _y, _z) { }

        MovementGeneratorType GetMovementGeneratorType() { return ASSISTANCE_MOTION_TYPE; }
        void Finalize(Unit &);
};

// Does almost nothing - just doesn't allows previous movegen interrupt current effect.
class EffectMovementGenerator : public MovementGenerator
{
    public:
        explicit EffectMovementGenerator(uint32 Id) : m_Id(Id) { }
        void Initialize(Unit &) { }
        void Finalize(Unit &unit);
        void Reset(Unit &) { }
        bool Update(Unit &u, const uint32&);
        MovementGeneratorType GetMovementGeneratorType() { return EFFECT_MOTION_TYPE; }
    private:
        uint32 m_Id;
};

#endif


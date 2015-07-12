/*
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Copyright (C) 2012 SymphonyArt <http://symphonyart.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#include "MovementChecker.h"
#include "Unit.h"
#include "Creature.h"
#include "World.h"

void PlayerMovementChecker::prepare(MovementInfo *movementInfo, uint16 opcode)
{
    if(_player->GetSession()->GetSecurity() >= AccountTypes(sWorld->getIntConfig(CONFIG_MOVEMENT_CHECKS_ACCESSLEVEL))) //skip unneeded calculations
        return;

    _movementInfo = movementInfo;
    _opcode = opcode;

    _currServerTime = getMSTime();

    if(_lastServerTime == 0)
        _lastServerTime = _currServerTime;

    _timediff = sWorld->GetUpdateTime();

    _mover = _player->m_mover;

    _vehicle = (_mover->GetVehicleKit()) ? _mover->GetVehicleKit()->GetBase() : NULL;

    _lastXCoord = _mover->GetPositionX();
    _lastYCoord = _mover->GetPositionY();
    _lastZCoord = _mover->GetPositionZ();

    _mapZ = _mover->GetMap()->GetHeight(_lastXCoord, _lastYCoord, MAX_HEIGHT);
    _floorZ = _mover->GetMap()->GetHeight(_lastXCoord, _lastYCoord, _lastZCoord, true, MAX_FALL_DISTANCE);

    _distance2D = _mover->GetExactDist2d(_movementInfo->pos.m_positionX, _movementInfo->pos.m_positionY);
    _distance3D = _mover->GetExactDist(_movementInfo->pos.m_positionX, _movementInfo->pos.m_positionY, _movementInfo->pos.m_positionZ);

    if(_lastClientTime == 0)
        _lastClientTime = _movementInfo->time;

    if(_opcode == MSG_MOVE_FALL_LAND)
        _alreadyJumped = false;

    _moverSize = _mover->GetObjectSize();

    _hasFlyingFlags = (_movementInfo->flags & (MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING));

    _moverHeight = 0.0f;

    _justTurn = false;

    if(_movementInfo->flags & (MOVEMENTFLAG_MASK_TURNING))
    {
        float deltaX = abs(_movementInfo->pos.m_positionX - _lastXCoord);
        float deltaY = abs(_movementInfo->pos.m_positionY - _lastYCoord);
        float deltaZ = abs(_movementInfo->pos.m_positionZ - _lastZCoord);

        if(deltaX <= 0.1f && deltaY <= 0.1f && deltaZ <= 0.1f)
            _justTurn = true;
        else
            _justTurn = false;
    }

    _isInWater = _player->IsInWater();

    uint32 waterTimeDelta = _currServerTime - _lastInWaterTime;

    _justWasInWater = waterTimeDelta < _latency * 2 + _timediff * 2 + 1000;

    if(_isInWater)
        _lastInWaterTime = _currServerTime;

    if(Player* plMover = _mover->ToPlayer())
        _latency = plMover->GetSession()->GetLatency();
    else
        _latency = _player->GetSession()->GetLatency();
}

bool PlayerMovementChecker::canMove()
{
    if(_player->GetSession()->GetSecurity() >= AccountTypes(sWorld->getIntConfig(CONFIG_MOVEMENT_CHECKS_ACCESSLEVEL)))
        return true;
    if(/*_player->HasAura(46598) ||*/ _player->HasAura(63163) || _player->HasAura(51852))
        return true;
    if(_player->IsPolymorphed() && _player->GetMap()->IsBattlegroundOrArena())
        return true;
    if(!isFlyingOk())
        return false;
    if(!isPlaneOk())
        return false;
    if(!isJumpOk())
        return false;
    if(!isFeatherFallOk())
        return false;
    if(!isWaterWalkOk())
        return false;
    if(!isSpeedOk())
        return false;
    if(!isZCoordOk())
        return false;
    return true;
}

bool PlayerMovementChecker::isFlyingOk()
{
    if(!sWorld->getBoolConfig(CONFIG_MOVEMENT_CHECKS_FLYING))
        return true;

    const bool flying = _hasFlyingFlags && !(_movementInfo->flags & (MOVEMENTFLAG_SWIMMING)); // allow X button in water

    if(!flying)
        return true;

    _justWasInAir = true;

    bool vehicleCanFly = false;
    if(_vehicle)
    {
       if(Creature* pCreature = _vehicle->ToCreature())
           vehicleCanFly = pCreature->canFly();
    }

    const bool canFly = (_player->HasAuraType(SPELL_AURA_FLY) || _player->HasAuraType(SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED)
                || _player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) || _player->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED)
                || _player->HasAuraType(SPELL_AURA_MOD_MOUNTED_FLIGHT_SPEED_ALWAYS) || vehicleCanFly);

    uint32 flyTimeDelta = _currServerTime - _lastFlyingTime;

    const bool justHadFlyer = flyTimeDelta < _latency * 2 + _timediff * 2 + 1000;

    const bool passed = canFly || (_player->HasUnitState(UNIT_STAT_ROAMING|UNIT_STAT_ROAMING_MOVE|UNIT_STAT_CHARGING) || _justCharged || justHadFlyer);

    if(canFly)
        _lastFlyingTime = _currServerTime;

    if(!passed)
        sLog->outBasic("MovementChecker: flying. Player: %s [GUID: %u]. canFly: %s, flyDelta: %u",
            _player->GetName(), _player->GetGUIDLow(), canFly ? "yes" : "no", flyTimeDelta);

    return passed;
}

bool PlayerMovementChecker::isPlaneOk()
{
    if(!sWorld->getBoolConfig(CONFIG_MOVEMENT_CHECKS_PLANE))
        return true;

    if(_movementInfo->pos.m_positionZ > 0.0001f || _movementInfo->pos.m_positionZ < -0.0001f)
        return true;

    if(_player->HasUnitState(UNIT_STAT_ROAMING|UNIT_STAT_ROAMING_MOVE|UNIT_STAT_CHARGING))
        return true;

    if(_hasFlyingFlags || _isInWater || _justWasInWater || (_movementInfo->flags & (MOVEMENTFLAG_WATERWALKING)))
        return true;

    float newCoordMapZ = _player->GetMap()->GetHeight(_movementInfo->pos.m_positionX, _movementInfo->pos.m_positionY, MAX_HEIGHT);
    newCoordMapZ = (newCoordMapZ < -500.0f) ? 0.0f : newCoordMapZ; //check holes in heigth map

    float heightForChecking = newCoordMapZ - _movementInfo->pos.m_positionZ;

    if(heightForChecking > 0.1f || heightForChecking < -0.1f)
    {
        sLog->outBasic("MovementChecker: plane. Player: %s [GUID: %u].", _player->GetName(), _player->GetGUIDLow());
        return false;
    }

    return true;
}

bool PlayerMovementChecker::isJumpOk()
{
    if(!sWorld->getBoolConfig(CONFIG_MOVEMENT_CHECKS_MULTIJUMP))
        return true;

    if(_opcode != MSG_MOVE_JUMP || _isInWater)
    {
        _alreadyJumped = false;
        return true;
    }

    if(_alreadyJumped)
    {
        sLog->outBasic("MovementChecker: jump. Player: %s [GUID: %u].", _player->GetName(), _player->GetGUIDLow());
        return false;
    }

    _alreadyJumped = true;

    return true;
}

bool PlayerMovementChecker::isFeatherFallOk()
{
    if(_justTurn)
        return true;

    if(!(_movementInfo->flags & (MOVEMENTFLAG_FALLING_SLOW)))
        return true;

    const bool hasFeatherFall = _mover->HasAuraType(SPELL_AURA_FEATHER_FALL);

    uint32 featherFallTimeDelta = _currServerTime - _lastFeatherFallTime;

    const bool justHadFeaterFall = featherFallTimeDelta < _latency * 2 + _timediff * 2 + 1000;

    if(_isInWater)
        _lastInWaterTime = _currServerTime;

    if(!hasFeatherFall && !justHadFeaterFall && !(_player->HasUnitState(UNIT_STAT_ROAMING|UNIT_STAT_ROAMING_MOVE|UNIT_STAT_CHARGING) || _justCharged))
    {
        sLog->outBasic("MovementChecker: FeatherFall. Player: %s [GUID: %u].", _player->GetName(), _player->GetGUIDLow());
        return false;
    }

    if(hasFeatherFall)
        _lastFeatherFallTime = _currServerTime;

    return true;
}

bool PlayerMovementChecker::isWaterWalkOk()
{
    if(!sWorld->getBoolConfig(CONFIG_MOVEMENT_CHECKS_WATERWALK))
        return true;

    if(!(_movementInfo->flags & (MOVEMENTFLAG_WATERWALKING)))
        return true;

    const bool hasWaterwalkAura = (_mover->HasAuraType(SPELL_AURA_WATER_WALK) || _mover->HasAuraType(SPELL_AURA_GHOST));

    uint32 waterWalkTimeDelta = _currServerTime - _lastWaterwalkTime;

    const bool justHadWaterwalkAura = waterWalkTimeDelta < _latency * 2 + _timediff * 2 + 1000;

    if(!hasWaterwalkAura && !justHadWaterwalkAura && !(_player->HasUnitState(UNIT_STAT_ROAMING|UNIT_STAT_ROAMING_MOVE|UNIT_STAT_CHARGING) || _justCharged))
    {
        sLog->outBasic("MovementChecker: waterwalking. Player: %s [GUID: %u].", _player->GetName(), _player->GetGUIDLow());
        return false;
    }

    if(hasWaterwalkAura)
        _lastWaterwalkTime = _currServerTime;

    return true;
}

bool PlayerMovementChecker::isSpeedOk()
{
    if(!sWorld->getBoolConfig(CONFIG_MOVEMENT_CHECKS_SPEED) || _movementInfo->flags & MOVEMENTFLAG_ONTRANSPORT || _justTurn)
    {
        _lastClientTime = _movementInfo->time;
        _lastServerTime = _currServerTime;
        return true;
    }

    UnitMoveType moveType = GetMovementType();

    if(_vehicle)
        return true;
    else
        _currSpeed = _mover->GetSpeed(moveType);

    uint32 clientTimeDelta = _movementInfo->time - _lastClientTime + (_latency * 2);
    const uint32 serverTimeDelta = _currServerTime - _lastServerTime + _latency + _timediff;

    if(clientTimeDelta > 1000)
        clientTimeDelta += _timediff;

    _lastClientTime = _movementInfo->time;
    _lastServerTime = _currServerTime;

    //const float totalTimeDelta = (clientTimeDelta < serverTimeDelta) ? (float)clientTimeDelta * 0.001f : (float)serverTimeDelta * 0.001f;
    const float totalTimeDelta = (float)clientTimeDelta * 0.001f;

    float speedToCheck = (_currSpeed < _lastSpeed) ? _lastSpeed : _currSpeed;

    if(_lastSpeed != _currSpeed)
    {
        _lastSpeed = _currSpeed;
        _speedChanged = true;
    } else _speedChanged = false;

    _justWasInAir = (_justWasInAir) ? isInAir() : false;

    bool charging = _mover->HasUnitState(UNIT_STAT_CHARGING) || _justCharged;

    if(_opcode == MSG_MOVE_FALL_LAND || _movementInfo->fallTime != _lastFallTime || _justWasInAir || charging)
        speedToCheck = SPEED_CHARGE;

    _justCharged = (!_mover->HasUnitState(UNIT_STAT_CHARGING) && _justCharged) ? false : _mover->HasUnitState(UNIT_STAT_CHARGING);

    _lastFallTime = _movementInfo->fallTime;

    float allowedDistance = (speedToCheck * totalTimeDelta) + _moverSize;

    if(_distance2D > allowedDistance)
    {
        sLog->outBasic("MovementChecker: speed. Player: %s [GUID: %u]. Allowed dist: %f, client dist: %f, 3D dist: %f, clientDelta: %u, serverDelta: %u, moveType: %u, speed: %f",
            _player->GetName(), _player->GetGUIDLow(), allowedDistance, _distance2D, _distance3D, clientTimeDelta, serverTimeDelta, moveType, speedToCheck);
        return false;
    }

    return true;
}

bool PlayerMovementChecker::isZCoordOk()
{
    if(_hasFlyingFlags || _justTurn || _player->isInFlight() || (_movementInfo->flags & (MOVEMENTFLAG_SWIMMING|MOVEMENTFLAG_ONTRANSPORT)))
        return true;

    if(!isInAir())
        return true;

    float deltaZ = _movementInfo->pos.m_positionZ - _lastZCoord;

    if(deltaZ > _moverSize + 10.0f)
    {
        sLog->outBasic("MovementChecker: ZCoord. Player: %s [GUID: %u], deltaZ: %f, lastZ: %f, floorZ: %f, height: %f",
            _player->GetName(), _player->GetGUIDLow(), deltaZ, _lastZCoord, _floorZ, _moverHeight);
        return false;
    }

    return true;
}

bool PlayerMovementChecker::isInAir()
{
    if(_hasFlyingFlags || _player->isInFlight() || (_movementInfo->flags & (MOVEMENTFLAG_SWIMMING)))
        return false;

    _isAboveFloor = (_movementInfo->pos.m_positionZ > (_floorZ + _moverSize * 3));

    _moverHeight = _movementInfo->pos.m_positionZ - _floorZ + 0.01f;

    return _isAboveFloor;
}

UnitMoveType PlayerMovementChecker::GetMovementType()
{
    UnitMoveType moveType;

    switch(_movementInfo->flags & (MOVEMENTFLAG_FLYING | MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_WALKING))
    {
        case MOVEMENTFLAG_FLYING:
            moveType = _movementInfo->flags & MOVEMENTFLAG_BACKWARD ? MOVE_FLIGHT_BACK : MOVE_FLIGHT;
            break;
        case MOVEMENTFLAG_SWIMMING:
            moveType = _movementInfo->flags & MOVEMENTFLAG_BACKWARD ? MOVE_SWIM_BACK : MOVE_SWIM;
            break;
        case MOVEMENTFLAG_WALKING:
            moveType = MOVE_WALK;
            break;
        default:
            moveType = _movementInfo->flags & MOVEMENTFLAG_BACKWARD ? MOVE_RUN_BACK : MOVE_RUN;
            break;
    }

    return moveType;
}

void PlayerMovementChecker::doMovementCorrection()
{
    WorldPacket data;

    _mover->SetUnitMovementFlags(MOVEMENTFLAG_NONE);

    Player *plMover = _mover->ToPlayer();

    if(plMover && plMover != _player)
        plMover->SendTeleportAckPacket();

    _player->SendTeleportAckPacket();

    _mover->BuildHeartBeatMsg(&data);
    _mover->SendMessageToSet(&data, true);
}

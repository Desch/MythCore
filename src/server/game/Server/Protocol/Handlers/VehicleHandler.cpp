/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Vehicle.h"
#include "Player.h"
#include "Log.h"
#include "ObjectAccessor.h"

void WorldSession::HandleDismissControlledVehicle(WorldPacket &recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_DISMISS_CONTROLLED_VEHICLE");
    recv_data.hexlike();

    uint64 vehicleGUID = _player->GetCharmGUID();

    if(!vehicleGUID)                                       // something wrong here...
    {
        recv_data.rfinish();                                // prevent warnings spam
        return;
    }

    uint64 guid;

    recv_data.readPackGUID(guid);

    MovementInfo mi;
    mi.guid = guid;
    ReadMovementInfo(recv_data, &mi);

    _player->m_movementInfo = mi;

    _player->ExitVehicle();
}

void WorldSession::HandleChangeSeatsOnControlledVehicle(WorldPacket &recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE");
    recv_data.hexlike();

    Unit* vehicle_base = GetPlayer()->GetVehicleBase();
    if(!vehicle_base)
    {
        recv_data.rfinish();                                // prevent warnings spam
        return;
    }

    VehicleSeatEntry const* seat = GetPlayer()->GetVehicle()->GetSeatForPassenger(GetPlayer());
    if(!seat->CanSwitchFromSeat())
    {
        recv_data.rfinish();                                // prevent warnings spam
        sLog->outError("HandleChangeSeatsOnControlledVehicle, Opcode: %u, Player %u tried to switch seats but current seatflags %u don't permit that.",
            recv_data.GetOpcode(), GetPlayer()->GetGUIDLow(), seat->m_flags);
        return;
    }

    switch(recv_data.GetOpcode())
    {
        case CMSG_REQUEST_VEHICLE_PREV_SEAT:
            GetPlayer()->ChangeSeat(-1, false);
            break;
        case CMSG_REQUEST_VEHICLE_NEXT_SEAT:
            GetPlayer()->ChangeSeat(-1, true);
            break;
        case CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE:
        {
            uint64 guid;        // current vehicle guid
            recv_data.readPackGUID(guid);

            ReadMovementInfo(recv_data, &vehicle_base->m_movementInfo);

            uint64 accessory;        //  accessory guid
            recv_data.readPackGUID(accessory);

            int8 seatId;
            recv_data >> seatId;

            if(vehicle_base->GetGUID() != guid)
                return;

            if(!accessory)
                GetPlayer()->ChangeSeat(-1, seatId > 0); // prev/next
            else if(Unit* vehUnit = Unit::GetUnit(*GetPlayer(), accessory))
            {
                if(Vehicle* vehicle = vehUnit->GetVehicleKit())
                    if(vehicle->HasEmptySeat(seatId))
                        vehUnit->HandleSpellClick(GetPlayer(), seatId);
            }
            break;
        }
        case CMSG_REQUEST_VEHICLE_SWITCH_SEAT:
        {
            uint64 guid;        // current vehicle guid
            recv_data.readPackGUID(guid);

            int8 seatId;
            recv_data >> seatId;

            if(vehicle_base->GetGUID() == guid)
                GetPlayer()->ChangeSeat(seatId);
            else if(Unit* vehUnit = Unit::GetUnit(*GetPlayer(), guid))
                if(Vehicle* vehicle = vehUnit->GetVehicleKit())
                    if(vehicle->HasEmptySeat(seatId))
                        vehUnit->HandleSpellClick(GetPlayer(), seatId);
            break;
        }
        default:
            break;
    }
}

void WorldSession::HandleEnterPlayerVehicle(WorldPacket &data)
{
    // Read guid
    uint64 guid;
    data >> guid;

    if(Player* pl = ObjectAccessor::FindPlayer(guid))
    {
        if(!pl->GetVehicleKit())
            return;
        if(!pl->IsInRaidWith(_player))
            return;
        if(!pl->IsWithinDistInMap(_player, INTERACTION_DISTANCE))
            return;

        _player->EnterVehicle(pl);
    }
}

void WorldSession::HandleEjectPassenger(WorldPacket &data)
{
    Vehicle* vehicle = _player->GetVehicleKit();
    if(!vehicle)
    {
        data.rfinish();                                     // prevent warnings spam
        sLog->outError("HandleEjectPassenger: Player %u is not in a vehicle!", GetPlayer()->GetGUIDLow());
        return;
    }

    uint64 guid;
    data >> guid;

    if(IS_PLAYER_GUID(guid))
    {
        Player* plr = ObjectAccessor::FindPlayer(guid);
        if(!plr)
        {
            sLog->outError("Player %u tried to eject player %u from vehicle, but the latter was not found in world!", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        if(!plr->IsOnVehicle(vehicle->GetBase()))
        {
            sLog->outError("Player %u tried to eject player %u, but they are not in the same vehicle", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(plr);
        ASSERT(seat);
        if(seat->IsEjectable())
            plr->ExitVehicle();
        else
            sLog->outError("Player %u attempted to eject player %u from non-ejectable seat.", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
    }

    else if(IS_CREATURE_GUID(guid))
    {
        Unit* unit = ObjectAccessor::GetUnit(*_player, guid);
        if(!unit) // creatures can be ejected too from player mounts
        {
            sLog->outError("Player %u tried to eject creature guid %u from vehicle, but the latter was not found in world!", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        if(!unit->IsOnVehicle(vehicle->GetBase()))
        {
            sLog->outError("Player %u tried to eject unit %u, but they are not in the same vehicle", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(unit);
        ASSERT(seat);
        if(seat->IsEjectable())
        {
            ASSERT(GetPlayer() == vehicle->GetBase());
            unit->ExitVehicle();
        }
        else
            sLog->outError("Player %u attempted to eject creature GUID %u from non-ejectable seat.", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
    }
    else
        sLog->outError("HandleEjectPassenger: Player %u tried to eject invalid GUID "UI64FMTD, GetPlayer()->GetGUIDLow(), guid);
}

void WorldSession::HandleRequestVehicleExit(WorldPacket &recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_REQUEST_VEHICLE_EXIT");
    recv_data.hexlike();

    if(Vehicle* vehicle = GetPlayer()->GetVehicle())
    {
        if(VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(GetPlayer()))
        {
            if(seat->CanEnterOrExit())
                GetPlayer()->ExitVehicle();
            else
                sLog->outError("Player %u tried to exit vehicle, but seatflags %u (ID: %u) don't permit that.",
                GetPlayer()->GetGUIDLow(), seat->m_ID, seat->m_flags);
        }
    }
}

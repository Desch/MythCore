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

#ifndef __TRINITY_VEHICLE_H
#define __TRINITY_VEHICLE_H

#include "ObjectDefines.h"

struct VehicleEntry;
struct VehicleSeatEntry;
struct Position;
class Unit;

enum PowerType
{
    POWER_STEAM                                  = 61,
    POWER_PYRITE                                 = 41,
    POWER_HEAT                                   = 101,
    POWER_OOZE                                   = 121,
    POWER_BLOOD                                  = 141,
    POWER_WRATH                                  = 142
};

enum VehicleFlags
{
    VEHICLE_FLAG_NO_STRAFE                       = 0x00000001,           // Sets MOVEFLAG2_NO_STRAFE
    VEHICLE_FLAG_NO_JUMPING                      = 0x00000002,           // Sets MOVEFLAG2_NO_JUMPING
    VEHICLE_FLAG_FULLSPEEDTURNING                = 0x00000004,           // Sets MOVEFLAG2_FULLSPEEDTURNING
    VEHICLE_FLAG_ALLOW_PITCHING                  = 0x00000010,           // Sets MOVEFLAG2_ALLOW_PITCHING
    VEHICLE_FLAG_FULLSPEEDPITCHING               = 0x00000020,           // Sets MOVEFLAG2_FULLSPEEDPITCHING
    VEHICLE_FLAG_CUSTOM_PITCH                    = 0x00000040,           // If set use pitchMin and pitchMax from DBC, otherwise pitchMin = -pi/2, pitchMax = pi/2
    VEHICLE_FLAG_ADJUST_AIM_ANGLE                = 0x00000400,           // Lua_IsVehicleAimAngleAdjustable
    VEHICLE_FLAG_ADJUST_AIM_POWER                = 0x00000800,           // Lua_IsVehicleAimPowerAdjustable
};

enum VehicleSpells
{
    VEHICLE_SPELL_RIDE_HARDCODED                 = 46598,
    VEHICLE_SPELL_PARACHUTE                      = 45472
};

struct VehicleSeat
{
    explicit VehicleSeat(VehicleSeatEntry const *seatInfo) : SeatInfo(seatInfo), Passenger(0) { }
    VehicleSeatEntry const *SeatInfo;
    uint64 Passenger;
};

struct VehicleAccessory
{
    VehicleAccessory(uint32 entry, int8 seatId, bool isMinion, uint8 summonType, uint32 summonTime) :
        AccessoryEntry(entry), SeatId(seatId), IsMinion(isMinion), SummonedType(summonType), SummonTime(summonTime) { }
    uint32 AccessoryEntry;
    int8 SeatId;
    uint32 IsMinion;
    uint8 SummonedType;
    uint32 SummonTime;
};

typedef std::vector<VehicleAccessory> VehicleAccessoryList;
typedef std::map<uint32, VehicleAccessoryList> VehicleAccessoryMap;
typedef std::map<int8, VehicleSeat> SeatMap;

class Vehicle
{
    friend class Unit;
    friend class WorldSession;

    public:
        explicit Vehicle(Unit* unit, VehicleEntry const* vehInfo, uint32 creatureEntry);
        virtual ~Vehicle();

        void Install();
        void Uninstall();
        void Reset(bool evading = false);
        void InstallAllAccessories(bool evading);
        void ApplyAllImmunities();

        Unit* GetBase() const { return _me; }
        VehicleEntry const* GetVehicleInfo() const { return _vehicleInfo; }
        uint32 const& GetCreatureEntry() const { return _creatureEntry; }

        bool HasEmptySeat(int8 seatId) const;
        Unit* GetPassenger(int8 seatId) const;
        int8 GetNextEmptySeat(int8 seatId, bool next) const;
        uint8 GetAvailableSeatCount() const;

        bool AddPassenger(Unit* passenger, int8 seatId = -1);
        void EjectPassenger(Unit* passenger, Unit* controller);
        void RemovePassenger(Unit* passenger);
        void RelocatePassengers(float x, float y, float z, float ang);
        void RemoveAllPassengers();
        void Dismiss();
        void Relocate(Position pos);
        bool IsVehicleInUse() { return Seats.begin() != Seats.end(); }

        SeatMap Seats;

    protected:
        uint16 GetExtraMovementFlagsForBase() const;
        VehicleSeatEntry const* GetSeatForPassenger(Unit* passenger);

    private:
        SeatMap::iterator GetSeatIteratorForPassenger(Unit* passenger);
        void InitMovementInfoForBase();
        void InstallAccessory(uint32 entry, int8 seatId, bool minion, uint8 type, uint32 summonTime);

        Unit* _me;
        VehicleEntry const* _vehicleInfo;
        uint32 _usableSeatNum;         // Number of seats that match VehicleSeatEntry::UsableByPlayer, used for proper display flags
        uint32 _creatureEntry;         // Can be different than me->GetBase()->GetEntry() in case of players
};

#endif

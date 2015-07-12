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

#ifndef TRINITYCORE_TEMPSUMMON_H
#define TRINITYCORE_TEMPSUMMON_H

#include "Creature.h"

class TempSummon : public Creature
{
public:
    explicit TempSummon(SummonPropertiesEntry const *properties, Unit* owner, bool isWorldObject);
    virtual ~TempSummon();
    void Update(uint32 time);
    virtual void InitStats(uint32 lifetime);
    virtual void InitSummon();
    void UnSummon(uint32 msTime = 0);
    void RemoveFromWorld();
    void SetTempSummonType(TempSummonType type);
    void SaveToDB(uint32 /*mapid*/, uint8 /*spawnMask*/, uint32 /*phaseMask*/) { }
    Unit* GetSummoner() const;
    uint64 const& GetSummonerGUID() { return m_summonerGUID; }
    TempSummonType const& GetSummonType() { return m_type; }

    const SummonPropertiesEntry * const m_Properties;
private:
    TempSummonType m_type;
    uint32 m_timer;
    uint32 m_lifetime;
    uint64 m_summonerGUID;
};

class Minion : public TempSummon
{
public:
    Minion(SummonPropertiesEntry const *properties, Unit* owner, bool isWorldObject);
    void InitStats(uint32 duration);
    void RemoveFromWorld();
    Unit* GetOwner() { return m_owner; }
    float GetFollowAngle() const { return m_followAngle; }
    void SetFollowAngle(float angle) { m_followAngle = angle; }
    bool IsPetGhoul() const {return GetEntry() == 26125;} // Ghoul may be guardian or pet
    bool IsPetGargoyle() const {return GetEntry() == 27829;}
    bool IsGuardianPet() const;
protected:
    Unit* const m_owner;
    float m_followAngle;
};

class Guardian : public Minion
{
public:
    Guardian(SummonPropertiesEntry const *properties, Unit* owner, bool isWorldObject);
    void InitStats(uint32 duration);
    bool InitStatsForLevel(uint8 level);
    void InitSummon();

    bool UpdateStats(Stats stat);
    bool UpdateAllStats();
    void UpdateResistances(uint32 school);
    void UpdateArmor();
    void UpdateMaxHealth();
    void UpdateMaxPower(Powers power);
    void UpdateAttackPowerAndDamage(bool ranged = false);
    void UpdateDamagePhysical(WeaponAttackType attType);

    int32 GetBonusDamage() { return m_bonusSpellDamage; }
    void SetBonusDamage(int32 damage);
protected:
    int32   m_bonusSpellDamage;
    float   m_statFromOwner[MAX_STATS];
};

class Puppet : public Minion
{
public:
    Puppet(SummonPropertiesEntry const *properties, Unit* owner);
    void InitStats(uint32 duration);
    void InitSummon();
    void Update(uint32 time);
    void RemoveFromWorld();
protected:
    Player* m_owner;
};

class ForcedUnsummonDelayEvent : public BasicEvent
{
public:
    ForcedUnsummonDelayEvent(TempSummon& owner) : BasicEvent(), m_owner(owner) { }
    bool Execute(uint64 e_time, uint32 p_time);

private:
    TempSummon& m_owner;
};

#endif
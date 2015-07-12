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

#ifndef __BATTLEGROUNDDS_H
#define __BATTLEGROUNDDS_H

class Battleground;

enum BattlegroundDSObjectTypes
{
    BG_DS_OBJECT_DOOR_1         = 0,
    BG_DS_OBJECT_DOOR_2         = 1,
    BG_DS_OBJECT_WATER_1        = 2,
    BG_DS_OBJECT_WATER_2        = 3,
    BG_DS_OBJECT_BUFF_1         = 4,
    BG_DS_OBJECT_BUFF_2         = 5,
    BG_DS_OBJECT_MAX            = 6
};

enum BattlegroundDSObjects
{
    BG_DS_OBJECT_TYPE_DOOR_1    = 192642,
    BG_DS_OBJECT_TYPE_DOOR_2    = 192643,
    BG_DS_OBJECT_TYPE_WATER_1   = 194395, // Collision
    BG_DS_OBJECT_TYPE_WATER_2   = 191877, // Visual
    BG_DS_OBJECT_TYPE_BUFF_1    = 184663,
    BG_DS_OBJECT_TYPE_BUFF_2    = 184664
};

enum BattlegroundDSCreatureTypes
{
    BG_DS_NPC_WATERFALL_KNOCKBACK = 0,
    BG_DS_NPC_PIPE_KNOCKBACK_1    = 1,
    BG_DS_NPC_PIPE_KNOCKBACK_2    = 2,
    BG_DS_NPC_MAX                 = 3
};

enum BattlegroundDSCreatures
{
    BG_DS_NPC_TYPE_WATER_SPOUT    = 28567,
};

enum BattlegroundDSSpells
{
    BG_DS_SPELL_FLUSH             = 57405, // Visual and target selector for the starting knockback from the pipe
    BG_DS_SPELL_FLUSH_KNOCKBACK   = 61698, // Knockback effect for previous spell (triggered, not need to be casted)
    BG_DS_SPELL_WATER_SPOUT       = 58873, // Knockback effect of the central waterfall
};

enum BattlegroundDSData
{ // These values are NOT blizzlike... need the correct data!
    BG_DS_WATERFALL_TIMER_MIN                    = 30000,
    BG_DS_WATERFALL_TIMER_MAX                    = 60000,
    BG_DS_WATERFALL_WARNING_DURATION             = 5000,
    BG_DS_WATERFALL_DURATION                     = 10000,
    BG_DS_WATERFALL_KNOCKBACK_TIMER              = 1500,

    BG_DS_PIPE_KNOCKBACK_FIRST_DELAY             = 5000,
    BG_DS_PIPE_KNOCKBACK_DELAY                   = 3000,
    BG_DS_PIPE_KNOCKBACK_TOTAL_COUNT             = 2,

    BG_DS_WATERFALL_STATUS_WARNING               = 1, // Water starts to fall, but neither LoS blocking nor movement blocking
    BG_DS_WATERFALL_STATUS_ON                    = 2, // LoS and Movement blocking active
    BG_DS_WATERFALL_STATUS_OFF                   = 3,
};

class BattlegroundDSScore : public BattlegroundScore
{
public:
    BattlegroundDSScore() { };
    virtual ~BattlegroundDSScore() { };
};

class BattlegroundDS : public Battleground
{
    friend class BattlegroundMgr;
public:
    BattlegroundDS();
    ~BattlegroundDS();
    void Update(uint32 diff);

    /* inherited from BattlegroundClass */
    virtual void AddPlayer(Player* plr);
    virtual void StartingEventCloseDoors();
    virtual void StartingEventOpenDoors();

    void RemovePlayer(Player* plr, uint64 guid, uint32 team);
    void HandleAreaTrigger(Player* Source, uint32 uiTrigger);
    bool SetupBattleground();
    virtual void Reset();
    virtual void FillInitialWorldStates(WorldPacket &d);
    void HandleKillPlayer(Player* pVictim, Player* pKiller);
    void KnockBackPlayer(Unit* pPlayer, float angle, float horizontalSpeed, float verticalSpeed);
    bool HandlePlayerUnderMap(Player* plr);
protected:
    inline uint32 getWaterFallStatus() { return m_waterFallStatus; };
    inline void setWaterFallStatus(uint8 status) { m_waterFallStatus = status; };
    inline uint32 getWaterFallTimer() { return m_waterFallTimer; };
    inline void setWaterFallTimer(uint32 timer) { m_waterFallTimer = timer; };
    inline uint32 getWaterFallKnockbackTimer() { return m_waterFallKnockbackTimer; };
    inline void setWaterFallKnockbackTimer(uint32 timer) { m_waterFallKnockbackTimer = timer; };
    inline uint8 getPipeKnockBackCount() { return m_pipeKnockBackCount; };
    inline void setPipeKnockBackCount(uint8 count) { m_pipeKnockBackCount = count; };
    inline uint32 getPipeKnockBackTimer() { return m_pipeKnockBackTimer; };
    inline void setPipeKnockBackTimer(uint32 timer) { m_pipeKnockBackTimer = timer; };
private:
    uint32 m_waterFallTimer;
    uint32 m_waterFallKnockbackTimer;
    uint32 m_pipeKnockBackTimer;
    uint8 m_waterFallStatus;
    uint8 m_pipeKnockBackCount;
};
#endif

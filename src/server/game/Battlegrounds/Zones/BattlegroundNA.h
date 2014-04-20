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

#ifndef __BATTLEGROUNDNA_H
#define __BATTLEGROUNDNA_H

class Battleground;

enum BattlegroundNAObjectTypes
{
    BG_NA_OBJECT_DOOR_1         = 0,
    BG_NA_OBJECT_DOOR_2         = 1,
    BG_NA_OBJECT_DOOR_3         = 2,
    BG_NA_OBJECT_DOOR_4         = 3,
    BG_NA_OBJECT_BUFF_1         = 4,
    BG_NA_OBJECT_BUFF_2         = 5,
    BG_NA_OBJECT_MAX            = 6
};

enum BattlegroundNAObjects
{
    BG_NA_OBJECT_TYPE_DOOR_1    = 183978,
    BG_NA_OBJECT_TYPE_DOOR_2    = 183980,
    BG_NA_OBJECT_TYPE_DOOR_3    = 183977,
    BG_NA_OBJECT_TYPE_DOOR_4    = 183979,
    BG_NA_OBJECT_TYPE_BUFF_1    = 184663,
    BG_NA_OBJECT_TYPE_BUFF_2    = 184664
};

class BattlegroundNAScore : public BattlegroundScore
{
    public:
        BattlegroundNAScore() { };
        virtual ~BattlegroundNAScore() { };
        //TODO fix me
};

class BattlegroundNA : public Battleground
{
    friend class BattlegroundMgr;

    private:
        uint32 m_uiTeleport;
    public:
        BattlegroundNA();
        ~BattlegroundNA();
        void Update(uint32 diff);

        /* inherited from BattlegroundClass */
        virtual void AddPlayer(Player* plr);
        virtual void StartingEventCloseDoors();
        virtual void StartingEventOpenDoors();

        void RemovePlayer(Player* plr, uint64 guid, uint32 team);
        void HandleAreaTrigger(Player* Source, uint32 Trigger);
        bool SetupBattleground();
        virtual void Reset();
        virtual void FillInitialWorldStates(WorldPacket &d);
        void HandleKillPlayer(Player* player, Player* killer);
        bool HandlePlayerUnderMap(Player* plr);
};
#endif

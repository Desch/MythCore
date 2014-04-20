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

#ifndef __BATTLEGROUNDRB_H
#define __BATTLEGROUNDRB_H

class Battleground;

class BattlegroundRBScore : public BattlegroundScore
{
    public:
        BattlegroundRBScore() { };
        virtual ~BattlegroundRBScore() { };
};

class BattlegroundRB : public Battleground
{
    friend class BattlegroundMgr;

    public:
        BattlegroundRB();
        ~BattlegroundRB();
        void Update(uint32 diff);

        virtual void AddPlayer(Player* plr);
        virtual void StartingEventCloseDoors();
        virtual void StartingEventOpenDoors();

        void RemovePlayer(Player* plr, uint64 guid, uint32 team);
        void HandleAreaTrigger(Player* Source, uint32 Trigger);

        /* Scorekeeping */
        void UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor = true);

    private:
};

#endif

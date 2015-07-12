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

#include "ScriptMgr.h"
#include "BattlegroundAB.h"
#include "BattlegroundWS.h"
#include "BattlegroundIC.h"
#include "BattlegroundSA.h"
#include "BattlegroundAV.h"

class achievement_everything_counts : public AchievementCriteriaScript
{
public:
    achievement_everything_counts() : AchievementCriteriaScript("achievement_everything_counts") { }

    bool OnCheck(Player* pSource, Unit* /*pTarget*/)
    {
        Battleground* pBG = pSource->GetBattleground();
        if(!pBG)
            return false;

        if(pBG->GetTypeID(true) != BATTLEGROUND_AV)
            return false;

        if(static_cast<BattlegroundAV*>(pBG)->IsBothMinesControlledByTeam(pSource->GetTeam()))
            return true;

        return false;
    }
};

class achievement_bg_av_perfection : public AchievementCriteriaScript
{
public:
    achievement_bg_av_perfection() : AchievementCriteriaScript("achievement_bg_av_perfection") { }

    bool OnCheck(Player* pSource, Unit* /*pTarget*/)
    {
        Battleground* pBG = pSource->GetBattleground();
        if(!pBG)
            return false;

        if(pBG->GetTypeID(true) != BATTLEGROUND_AV)
            return false;

        if(static_cast<BattlegroundAV*>(pBG)->IsAllTowersControlledAndCaptainAlive(pSource->GetTeam()))
            return true;

        return false;
    }
};

class achievement_storm_glory : public AchievementCriteriaScript
{
public:
    achievement_storm_glory() : AchievementCriteriaScript("achievement_storm_glory") { }

    bool OnCheck(Player* pSource, Unit* /*pTarget*/)
    {
        if(pSource->GetBattlegroundTypeId() != BATTLEGROUND_EY)
            return false;

        Battleground* pBG = pSource->GetBattleground();
        if(!pBG)
            return false;

        return pBG->IsAllNodesConrolledByTeam(pSource->GetTeam());
    }
};

class achievement_resilient_victory : public AchievementCriteriaScript
{
public:
    achievement_resilient_victory() : AchievementCriteriaScript("achievement_resilient_victory") { }

    bool OnCheck(Player* pSource, Unit* /*pTarget*/)
    {
        Battleground* pBG = pSource->GetBattleground();
        if(!pBG)
            return false;

        if(pBG->GetTypeID(true) != BATTLEGROUND_AB)
            return false;

        if(!static_cast<BattlegroundAB*>(pBG)->IsTeamScores500Disadvantage(pSource->GetTeam()))
            return false;

        return true;
    }
};

class achievement_bg_control_all_nodes : public AchievementCriteriaScript
{
public:
    achievement_bg_control_all_nodes() : AchievementCriteriaScript("achievement_bg_control_all_nodes") { }

    bool OnCheck(Player* pSource, Unit* /*pTarget*/)
    {
        Battleground* pBG = pSource->GetBattleground();
        if(!pBG)
            return false;

        if(!pBG->IsAllNodesConrolledByTeam(pSource->GetTeam()))
            return false;

        return true;
    }
};

class achievement_save_the_day : public AchievementCriteriaScript
{
public:
    achievement_save_the_day() : AchievementCriteriaScript("achievement_save_the_day") { }

    bool OnCheck(Player* pSource, Unit* pTarget)
    {
        if(!pTarget)
            return false;

        if(Player const* pPlayer = pTarget->ToPlayer())
        {
            Battleground* pBG = pSource->GetBattleground();
            if(!pBG)
                return false;

            if(pBG->GetTypeID(true) != BATTLEGROUND_WS)
                return false;

            if(static_cast<BattlegroundWS*>(pBG)->GetFlagState(pPlayer->GetTeam()) == BG_WS_FLAG_STATE_ON_BASE)
                return true;
        }
        return false;
    }
};

class achievement_bg_ic_resource_glut : public AchievementCriteriaScript
{
public:
    achievement_bg_ic_resource_glut() : AchievementCriteriaScript("achievement_bg_ic_resource_glut") { }

    bool OnCheck(Player* pSource, Unit* /*pTarget*/)
    {
        if(pSource->HasAura(SPELL_OIL_REFINERY) && pSource->HasAura(SPELL_QUARRY))
            return true;
         return false;
    }
};

class achievement_bg_ic_glaive_grave : public AchievementCriteriaScript
{
public:
    achievement_bg_ic_glaive_grave() : AchievementCriteriaScript("achievement_bg_ic_glaive_grave") { }

    bool OnCheck(Player* pSource, Unit* /*pTarget*/)
    {
        if(Creature* pCreature_Vehice = pSource->GetVehicleCreatureBase())
        {
            if(pCreature_Vehice->GetEntry() == NPC_GLAIVE_THROWER_H ||  pCreature_Vehice->GetEntry() == NPC_GLAIVE_THROWER_A)
                return true;
        }

        return false;
    }
};

class achievement_bg_ic_mowed_down : public AchievementCriteriaScript
{
public:
    achievement_bg_ic_mowed_down() : AchievementCriteriaScript("achievement_bg_ic_mowed_down") { }

    bool OnCheck(Player* pSource, Unit* /*pTarget*/)
    {
        if(Creature* pCreature_Vehice = pSource->GetVehicleCreatureBase())
        {
            if(pCreature_Vehice->GetEntry() == NPC_KEEP_CANNON)
                return true;
        }

        return false;
    }
};

class achievement_bg_sa_artillery : public AchievementCriteriaScript
{
public:
    achievement_bg_sa_artillery() : AchievementCriteriaScript("achievement_bg_sa_artillery") { }

    bool OnCheck(Player* pSource, Unit* /*pTarget*/)
    {
        if(Creature* pCreature_Vehice = pSource->GetVehicleCreatureBase())
        {
            if(pCreature_Vehice->GetEntry() == NPC_ANTI_PERSONNAL_CANNON)
                return true;
        }

        return false;
    }
};

class achievement_arena_kills : public AchievementCriteriaScript
{
public:
    achievement_arena_kills(char const* name, uint8 arenaType) : AchievementCriteriaScript(name),
        _arenaType(arenaType) { }

    bool OnCheck(Player* pSource, Unit* /*pTarget*/)
    {
        if(!pSource->InArena())
            return false;

        return pSource->GetBattleground()->GetArenaType() == _arenaType;
    }

private:
    uint8 const _arenaType;
};

class achievement_sickly_gazelle : public AchievementCriteriaScript
{
public:
    achievement_sickly_gazelle() : AchievementCriteriaScript("achievement_sickly_gazelle") { }

    bool OnCheck(Player* /*pSource*/, Unit* pTarget)
    {
        if(!pTarget)
            return false;

        if(Player* pPlayer = pTarget->ToPlayer())
            if(pPlayer->IsMounted())
                return true;

        return false;
    }
};

enum ArgentTournamentAreas
{
    AREA_ARGENT_TOURNAMENT_FIELDS   = 4658,
    AREA_RING_OF_ASPIRANTS          = 4670,
    AREA_RING_OF_ARGENT_VALIANTS    = 4671,
    AREA_RING_OF_ALLIANCE_VALIANTS  = 4672,
    AREA_RING_OF_HORDE_VALIANTS     = 4673,
    AREA_RING_OF_CHAMPIONS          = 4669
};

class achievement_tilted : public AchievementCriteriaScript
{
public:
    achievement_tilted() : AchievementCriteriaScript("achievement_tilted") { }

    bool OnCheck(Player* pSource, Unit* /*pTarget*/)
    {
        bool checkArea = pSource->GetAreaId() == AREA_ARGENT_TOURNAMENT_FIELDS  ||
                        pSource->GetAreaId() == AREA_RING_OF_ASPIRANTS          ||
                        pSource->GetAreaId() == AREA_RING_OF_ARGENT_VALIANTS    ||
                        pSource->GetAreaId() == AREA_RING_OF_ALLIANCE_VALIANTS  ||
                        pSource->GetAreaId() == AREA_RING_OF_HORDE_VALIANTS     ||
                        pSource->GetAreaId() == AREA_RING_OF_CHAMPIONS;

        return pSource && checkArea && pSource->duel && pSource->duel->isMounted;
    }
};

void AddSC_achievement_scripts()
{
    new achievement_storm_glory;
    new achievement_resilient_victory;
    new achievement_bg_control_all_nodes;
    new achievement_save_the_day;
    new achievement_bg_ic_resource_glut;
    new achievement_bg_ic_glaive_grave;
    new achievement_bg_ic_mowed_down;
    new achievement_bg_sa_artillery;
    new achievement_sickly_gazelle;
    new achievement_everything_counts;
    new achievement_bg_av_perfection;
    new achievement_arena_kills("achievement_arena_2v2_kills", ARENA_TYPE_2v2);
    new achievement_arena_kills("achievement_arena_3v3_kills", ARENA_TYPE_3v3);
    new achievement_arena_kills("achievement_arena_5v5_kills", ARENA_TYPE_5v5);
    new achievement_tilted;
}
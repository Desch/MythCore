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

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "icecrown_citadel.h"
#include "Group.h"
#include "MapManager.h"

#define GOSSIP_MENU 10600
//#define GOSSIP_MENU "Long have I waited for this day, hero. Are you and your allies prepared to bring the Lich King to justice? We charge on your command!"
#define GOSSIP_START_EVENT "Nous sommes prêts, généralissime. Laissez-nous la bataille pour le destin d'Azeroth! Pour la lumière de l'aube!"

enum eEnums
{
    SOUND_ENDING_7_KING                   = 17360,
    MOVIE_ID_ARTHAS_DEATH                 = 16
};

enum Yells
{
    SAY_INTRO_1_KING         = -1810001,
    SAY_INTRO_2_TIRION       = -1810002,
    SAY_INTRO_3_KING         = -1810003,
    SAY_INTRO_4_TIRION       = -1810004,
    SAY_INTRO_5_KING         = -1810005,
    SAY_AGGRO                = -1810006,
    SAY_REMORSELESSS_WINTER  = -1810007,
    SAY_RANDOM_1             = -1810008,
    SAY_RANDOM_2             = -1810009,
    SAY_KILL_1               = -1810010,
    SAY_KILL_2               = -1810011,
    SAY_BERSERK              = -1810012,
    SAY_ENDING_1_KING        = -1810013,
    SAY_ENDING_2_KING        = -1810014,
    SAY_ENDING_3_KING        = -1810015,
    SAY_ENDING_4_KING        = -1810016,
    SAY_ENDING_5_TIRION      = -1810017,
    SAY_ENDING_6_KING        = -1810018,
    SAY_ENDING_8_TIRION      = -1810020,
    SAY_ENDING_9_FATHER      = -1810021,
    SAY_ENDING_10_TIRION     = -1810022,
    SAY_ENDING_11_FATHER     = -1810023,
    SAY_ENDING_12_KING       = -1810024,
    SAY_DEATH_KING           = -1810025,
    SAY_ESCAPE_FROSTMOURNE   = -1810026,
    SAY_HARVEST_SOUL         = -1810027,
    SAY_DEVOURED_FROSTMOURNE = -1810028,
    SAY_SUMMON_VALKYR        = -1810029,
    SAY_BROKEN_ARENA         = -1810030,
    SAY_10_PERCENT           = -1810031,
    SAY_EMOTE_DEFILE         = -1810032,
    SAY_GREET_PLAYER         = -1810035,
    SAY_PLAYER_TO_ESCAPE     = -1810036,
    SAY_ASK_PLAYER_FOR_AID   = -1810037,
};

enum ePhases
{
    PHASE_1 = 1,
    PHASE_2_TRANSITION,
    PHASE_3,
    PHASE_4_TRANSITION,
    PHASE_5,
    PHASE_6_ENDING
};

enum eEvents
{
    EVENT_SPEECH = 1,
    EVENT_BERSERK,
    EVENT_CHECK_ALIVE_PLAYERS,
    EVENT_SUMMON_SHAMBLING_HORROR,
    EVENT_SUMMON_DRUDGE_GHOULS,
    EVENT_INFEST,
    EVENT_NECROTIC_PLAGUE,
    EVENT_SHADOW_TRAP,
    //Transition phase events
    EVENT_SUMMON_RAGING_SPIRIT,
    EVENT_SUMMON_ICE_SPHERE,
    EVENT_TRANSITION_PHASE_END,
    EVENT_PAIN_AND_SUFFERING,
    //Phase three events
    EVENT_SUMMON_VAL_KYR_SHADOWGUARD,
    EVENT_DEFILE,
    EVENT_SOUL_REAPER,
    //Phase five events
    EVENT_SUMMON_VILE_SPIRITS,
    EVENT_HARVEST_SOUL,
    EVENT_KILL_FROSTMOURNE_PLAYERS
};

enum Spells
{
    SPELL_DEATH_GRIP                 = 49560, // DK's Death Grip, here just for immunities
    SPELL_SUMMON_SHAMBLING_HORROR    = 70372,
    SPELL_SUMMON_DRUDGE_GHOULS       = 70358,
    SPELL_SUMMON_ICE_SPEHERE         = 69104,
    SPELL_SUMMON_RAGING_SPIRIT       = 69200,
    SPELL_SUMMON_VALKYR_10           = 69037,
    SPELL_SUMMON_VALKYR_25           = 74361,
    SPELL_SUMMON_DEFILE              = 72762,
    SPELL_SUMMON_VILE_SPIRIT         = 70498, //????????
    SPELL_SUMMON_SHADOW_TRAP         = 73539,
    SPELL_SHADOW_TRAP_INTRO          = 73530,
    SPELL_SHADOW_TRAP_PERIODIC       = 73525,
    SPELL_SHADOW_TRAP_EFFECT         = 73529,
    SPELL_INFEST_10_N                = 70541,
    SPELL_INFEST_25_N                = 73779,
    SPELL_INFEST_10_H                = 73780,
    SPELL_INFEST_25_H                = 73781,
    SPELL_NECROTIC_PLAGUE_10_N       = 70337, //70337 - initial cast
    SPELL_NECROTIC_PLAGUE_25_N       = 73912,
    SPELL_NECROTIC_PLAGUE_10_H       = 73913,
    SPELL_NECROTIC_PLAGUE_25_H       = 73914,
    SPELL_NECROTIC_PLAGUE_IMMUNITY   = 72846,
    SPELL_NECROTIC_PLAGUE_EFFECT_10_N = 70338,
    SPELL_NECROTIC_PLAGUE_EFFECT_25_N = 73785,
    SPELL_NECROTIC_PLAGUE_EFFECT_10_H = 73786,
    SPELL_NECROTIC_PLAGUE_EFFECT_25_H = 73787,
    SPELL_PLAGUE_SIPHON              = 74074,
    SPELL_REMORSELESS_WINTER_10_N    = 68981,
    SPELL_REMORSELESS_WINTER_25_N    = 74270,
    SPELL_REMORSELESS_WINTER_10_H    = 74271,
    SPELL_REMORSELESS_WINTER_25_H    = 74272,
    SPELL_REMORSELESS_WINTER_DAMAGE_10_N  = 68983,
    SPELL_REMORSELESS_WINTER_DAMAGE_25_N  = 73791,
    SPELL_REMORSELESS_WINTER_DAMAGE_10_H  = 73792,
    SPELL_REMORSELESS_WINTER_DAMAGE_25_H  = 73793,
    SPELL_PAIN_AND_SUFFERING_10_N    = 72133,
    SPELL_PAIN_AND_SUFFERING_25_N    = 73788,
    SPELL_PAIN_AND_SUFFERING_10_H    = 73789,
    SPELL_PAIN_AND_SUFFERING_25_H    = 73790,
    SPELL_WINGS_OF_THE_DAMNED        = 74352,
    SPELL_SOUL_REAPER                = 69409,
    SPELL_SOUL_REAPER_HASTE_AURA     = 69410,
    SPELL_HARVEST_SOUL_TELEPORT      = 71372, //72597
    SPELL_HARVEST_SOULS              = 68980,
    SPELL_HARVESTED_SOUL_NORMAL      = 74321,
    SPELL_HARVESTED_SOUL_HEROIC      = 74323,
    SPELL_HARVEST_SOUL_HEROIC_FROSTMOURNE_PLAYER_DEBUFF = 73655,
    SPELL_HARVESTED_SOUL_FROSTMOURNE_PLAYER_BUFF = 74322,
    SPELL_FROSTMOURNE_ROOM_TELEPORT_VISUAL = 73078,
    SPELL_QUAKE                      = 72262,
    SPELL_RAISE_DEAD                 = 71769,
    SPELL_RAISE_DEAD_EFFECT          = 72376,
    SPELL_BROKEN_FROSTMOURNE         = 72398,
    SPELL_ICEBLOCK_TRIGGER           = 71614,
    SPELL_TIRION_LIGHT               = 71797,
    SPELL_FROSTMOURNE_TRIGGER        = 72405,
    SPELL_DISENGAGE                  = 61508,
    SPELL_FURY_OF_FROSTMOURNE        = 72350,
    SPELL_FURY_OF_FROSTMOURNE_NORES  = 72351,
    SPELL_REVIVE                     = 72429,
    SPELL_REVIVE_EFFECT              = 72423,
    SPELL_CLONE_PLAYER               = 57507,
    SPELL_DEFILE                     = 72743,
    SPELL_DEFILE_DAMAGE              = 72754,
    SPELL_DEFILE_INCREASE            = 72756,
    SPELL_ICE_PULSE                  = 69091,
    SPELL_ICE_BURST                  = 69108,
    SPELL_LIFE_SIPHON                = 73783,
    SPELL_SPIRIT_BURST               = 70503,
    SPELL_VILE_SPIRIT_DISTANCE_CHECK = 70502,
    SPELL_ICE_BURST_DISTANCE_CHECK   = 69109,
    SPELL_VILE_SPIRIT_ACTIVE         = 72130,
    SPELL_RAGING_VISUAL              = 69198,
    SPELL_REMOVE_WEAPON              = 72399,
    SPELL_SUMMON_BROKEN_FROSTMOURNE  = 72406,
    SPELL_DROP_FROSTMOURNE           = 73017,
    SPELL_SUMMON_FROSTMOURNE_TRIGGER = 72407,
    SPELL_WMO_INTACT                 = 50176,
    SPELL_WMO_DESTROY                = 50177,
    SPELL_WMO_REBUILD                = 50178,
    SPELL_PLAY_MOVIE                 = 73159,
    SPELL_SUMMON_MENETHIL            = 72420, //caster - 38584
    SPELL_MENETHIL_VISUAL            = 72372,
    SPELL_VALKYR_CARRY_CAN_CAST      = 74506,
    SPELL_VALKYR_GRAB_PLAYER         = 68985, //74445,
    SPELL_RIDE_VEHICLE               = 46598,
    SPELL_VALKYR_TARGET_SEARCH       = 69030,
    SPELL_VALKYR_CHARGE              = 74399,
    SPELL_VALKYR_EJECT_PASSENGER     = 68576,
    SPELL_LIGHTS_BLESSING            = 71773,
    SPELL_EMOTE_SHOUT                = 73213,
    SPELL_RAGING_GHOUL_VISUAL        = 69636,
    SPELL_RISEN_WITCH_DOCTOR_SPAWN   = 69639,
    SPELL_ICE_SPHERE_VISUAL          = 69090,
    SPELL_TIRION_JUMP                = 71811,
    SPELL_CANT_RESSURECT_AURA        = 72431,
    SPELL_BOOM_VISUAL                = 72726,
    SPELL_SUMMON_FROSTMOURNE         = 74081,
    SPELL_SOUL_EFFECT                = 72305,
    SPELL_IN_FROSTMOURNE_ROOM        = 74276,
    SPELL_VILE_SPIRIT_TARGET_SEARCH  = 70501,
    SPELL_SOUL_RIP                   = 69397,
    SPELL_DESTROY_SOUL               = 74086,
    SPELL_DARK_HUNGER                = 69383,
    SPELL_DARK_HUNGER_HEAL_EFFECT    = 69384,
    SPELL_LIGHT_S_FAVOR              = 69382,
    SPELL_RESTORE_SOUL               = 72595,
    SPELL_FEIGN_DEATH                = 5384,
};

enum eActions
{
    ACTION_PHASE_SWITCH_1        = 1, //phase 1 and 3
    ACTION_PHASE_SWITCH_2,            //phase 2 and 4
    ACTION_START_EVENT,
    ACTION_RESET,
    ACTION_CANCEL_ALL_TRANSITION_EVENTS,
    ACTION_DESPAWN,
    ACTION_CHARGE_PLAYER,
    ACTION_PREPARE_FROSTMOURNE_ROOM,
    ACTION_ATTACK_SPIRIT_WARDEN,
    ACTION_ATTACK_TERENAS_FIGHTER,
};

enum eSetGuid
{
    TYPE_VICTIM = 1
};

enum ePoints
{
    POINT_START_EVENT_1              = 3659700,
    POINT_PLATFORM_CENTER            = 3659701,
    POINT_PLATFORM_END               = 3659702,
    POINT_VALKYR_END                 = 3659703,
    POINT_VALKYR_ZET                 = 3659704,
    POINT_VALKYR_CONTINUE_FLYING     = 3659705
};

enum eLKData
{
    DATA_PHASE = 2
};

struct Position StartEvent[]=
{
    {465.0731f, -2123.473f, 1040.8569f, 0.0f},
    {462.8351f, -2123.673f, 1040.9082f, 0.0f},
    {461.5851f, -2123.673f, 1041.4082f, 0.0f},
    {445.5851f, -2123.673f, 1056.1582f, 0.0f},
    {436.0851f, -2123.673f, 1064.6582f, 0.0f}
};

struct Position MovePos[]=
{
    {461.792f, -2125.85f, 1040.860f, 0.0f}, // move
    {503.156f, -2124.51f, 1040.860f, 0.0f}, // move center X: 505.2118 Y: -2124.353 Z: 840.9403
    {490.110f, -2124.98f, 1040.860f, 0.0f}, // move tirion frostmourne
    {467.069f, -2123.58f, 1040.857f, 0.0f}, // move tirion attack
    {498.004f, 2201.57f, 1046.093f, 0.0f},  // move valkyr
    {489.297f, -2124.84f, 1040.857f, 0.0f}, //start event tirion move 1
    {503.682f, -2126.63f, 1040.940f, 0.0f}, //boss escapes after wipe
    {508.989f, -2124.55f, 1045.356f, 0.0f}, //boss levitates above the frostmourne
    {505.212f, -2124.35f, 1040.94f, 3.14159f}
};

struct Position FrostmourneRoom[] =
{
    {520.0f, -2524.0f, 1051.0f, 0.0f}, //Place where player is teleported to
    {495.0f, -2546.0f, 1051.0f, 1.5708f}, //Home position of the Spirit Warden
    {495.0f, -2502.0f, 1051.0f, 4.7124f}, //Home position of Terenas Menethil
};

typedef std::list<Player*> TPlayerList;

TPlayerList GetPlayersInTheMap(Map *pMap)
{
    TPlayerList players;
    const Map::PlayerList &PlayerList = pMap->GetPlayers();
    if(!PlayerList.isEmpty())
        for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if(Player* player = i->getSource())
                players.push_back(player);
    return players;
}

TPlayerList GetAttackablePlayersInTheMap(Map *pMap)
{
    TPlayerList players = GetPlayersInTheMap(pMap);
    for(TPlayerList::iterator it = players.begin(); it != players.end();)
    {
        if(!(*it)->isTargetableForAttack())
            players.erase(it++);
        else
            ++it;
    }
    return players;
}

Player* SelectRandomPlayerFromList(TPlayerList &players)
{
    if(players.empty())
        return NULL;
    TPlayerList::iterator it = players.begin();
    std::advance(it, urand(0, players.size()-1));
    return *it;
}

Player* SelectRandomPlayerInTheMap(Map *pMap)
{
    TPlayerList players = GetPlayersInTheMap(pMap);
    return SelectRandomPlayerFromList(players);
}

Player* SelectRandomAttackablePlayerInTheMap(Map* pMap)
{
    TPlayerList players = GetAttackablePlayersInTheMap(pMap);
    return SelectRandomPlayerFromList(players);
}

uint32 GetPhase(const EventMap &em)
{
    switch(em.GetPhaseMask())
    {
        case 0x01: return 0;
        case 0x02: return 1;
        case 0x04: return 2;
        case 0x08: return 3;
        case 0x10: return 4;
        case 0x20: return 5;
        case 0x40: return 6;
        case 0x80: return 7;
        default:
            return 0;
    }
}

class boss_the_lich_king : public CreatureScript
{
public:
    boss_the_lich_king() : CreatureScript("boss_the_lich_king") { }

    struct boss_the_lich_kingAI : public BossAI
    {
        boss_the_lich_kingAI(Creature* creature) : BossAI(creature, DATA_THE_LICH_KING), summons(me), uiStage(1)
        {
            me->SetStandState(UNIT_STAND_STATE_SIT);
        }

        uint32 GetData(uint32 dataId)
        {
            if(dataId == DATA_PHASE)
                return GetPhase(events);
            return BossAI::GetData(dataId);
        }

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            //SetEquipmentSlots(false, 49706, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);    //2 swords are too much

            // fixing the platform edge bug
            // we now rebuild the platform at LK reset
            InstanceScript* _instance = me->GetInstanceScript();
            me->CastSpell(me, SPELL_WMO_INTACT, true);
            me->CastSpell(me, SPELL_WMO_REBUILD, true);

            //Rebuilding ice shards
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_ICE_SHARD_1)))
                go->SetGoState(GO_STATE_READY);
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_ICE_SHARD_2)))
                go->SetGoState(GO_STATE_READY);
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_ICE_SHARD_3)))
                go->SetGoState(GO_STATE_READY);
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_ICE_SHARD_4)))
                go->SetGoState(GO_STATE_READY);
            //Hiding edge destroy warning
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_EDGE_DESTROY_WARNING)))
                go->SetGoState(GO_STATE_READY);
            //Hiding inner waterfall
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_FROSTY_EDGE_INNER)))
                go->SetGoState(GO_STATE_READY);
            //Showing outer waterfall
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_FROSTY_EDGE_OUTER)))
                go->SetGoState(GO_STATE_ACTIVE);

            if(me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                me->GetMotionMaster()->MovementExpired();
             summons.DespawnAll();
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            events.Reset();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            if(uiStage > 1)
                return;
            instance->SetData(DATA_BEEN_WAITING_ACHIEVEMENT, NOT_STARTED);
            instance->SetData(DATA_NECK_DEEP_ACHIEVEMENT, DONE);

            uiEndingTimer = 1000;
            uiStage = 1;
            uiTirionGUID = 0;
            isSwitching = false;
            events.Reset();
            events.SetPhase(PHASE_1);

            events.ScheduleEvent(EVENT_BERSERK, 900000);
            events.ScheduleEvent(EVENT_CHECK_ALIVE_PLAYERS, 5000);

            events.ScheduleEvent(EVENT_SUMMON_DRUDGE_GHOULS, 10000, 0, PHASE_1);
            //About 5 seconds after the encounter starts properly Lich King will cast his first Infest
            events.ScheduleEvent(EVENT_INFEST, 5000, 0, PHASE_1);
            events.ScheduleEvent(EVENT_SUMMON_SHAMBLING_HORROR, 20000, 0, PHASE_1);
            events.ScheduleEvent(EVENT_NECROTIC_PLAGUE, 35000, 0, PHASE_1);
            if(IsHeroic())
                events.ScheduleEvent(EVENT_SHADOW_TRAP, 10000, 0, PHASE_1);
            DoScriptText(SAY_AGGRO, me);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
            DoZoneInCombat();
            DoCast(me, SPELL_NECROTIC_PLAGUE_IMMUNITY);

            uiTirionGUID = instance->GetData64(GUID_TIRION);
            instance->SetBossState(DATA_THE_LICH_KING, IN_PROGRESS);
        }

        void Cleanup()
        {
            instance->DoRemoveAurasDueToSpellOnPlayers(RAID_MODE(SPELL_INFEST_10_N, SPELL_INFEST_25_N, SPELL_INFEST_10_H, SPELL_INFEST_25_H));
            instance->DoCastSpellOnPlayers(FROZEN_THRONE_TELEPORT);
            instance->DoRemoveAurasDueToSpellOnPlayers(RAID_MODE(SPELL_NECROTIC_PLAGUE_10_N, SPELL_NECROTIC_PLAGUE_25_N, SPELL_NECROTIC_PLAGUE_10_H, SPELL_NECROTIC_PLAGUE_25_H));
            instance->DoRemoveAurasDueToSpellOnPlayers(RAID_MODE(SPELL_NECROTIC_PLAGUE_EFFECT_10_N, SPELL_NECROTIC_PLAGUE_EFFECT_25_N, SPELL_NECROTIC_PLAGUE_EFFECT_10_H, SPELL_NECROTIC_PLAGUE_EFFECT_25_H));
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ICE_PULSE);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_IN_FROSTMOURNE_ROOM);
            summons.DespawnAll();
        }

        void JustDied(Unit* killer)
        {
            BossAI::JustDied(killer);
            if(killer->GetTypeId() ==  TYPEID_PLAYER)
            {
                if(killer->ToPlayer()->GetGroup())
                {
                    Group* group = killer->ToPlayer()->GetGroup();
                    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                    {
                        Player* pl = itr->getSource();

                        if (!pl || !pl->GetSession())
                            continue;

                        uint32 achiev;
                        achiev = 0;
                        switch(GetDifficulty())
                        {
                            case RAID_DIFFICULTY_10MAN_NORMAL:
                                achiev = 4530;
                                break;
                            case RAID_DIFFICULTY_25MAN_NORMAL:
                                achiev = 4597;
                                break;
                            case RAID_DIFFICULTY_10MAN_HEROIC:
                                achiev = 4583;
                                break;
                            case RAID_DIFFICULTY_25MAN_HEROIC:
                                achiev = 4584;
                                break;
                            default:
                                break;
                        }
                        
                        if(AchievementEntry const* pAchievment = GetAchievementStore()->LookupEntry(achiev))
                            pl->CompletedAchievement(pAchievment);                    
                    }
                }
            }

            Cleanup();
            DoCastAOE(SPELL_PLAY_MOVIE);
            if(Creature* pFather = me->FindNearestCreature(NPC_TERENAS_MENETHIL, 25.0f, true))
                pFather->SetVisible(false);
            if(Creature* pTirion = Unit::GetCreature(*me, uiTirionGUID))
            {
                pTirion->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pTirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                pTirion->RemoveAllAuras();
            }
            me->SetDisableGravity(false);
            me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
            me->SetPosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()+200, me->GetOrientation());
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if(type != POINT_MOTION_TYPE)
                return;
            if(instance->GetBossState(DATA_THE_LICH_KING) == DONE)
                return;
            switch(id)
            {
                case POINT_PLATFORM_CENTER:
                {
                    me->GetMotionMaster()->MovementExpired();
                    uint32 curPhase = GetPhase(events);
                    events.ScheduleEvent(EVENT_PAIN_AND_SUFFERING, 2500+2000, 0, curPhase);
                    events.ScheduleEvent(EVENT_SUMMON_ICE_SPHERE, 2500+7000, 0, curPhase);
                    events.ScheduleEvent(EVENT_SUMMON_RAGING_SPIRIT, 6000, 0, curPhase);
                    events.ScheduleEvent(EVENT_TRANSITION_PHASE_END, 60000, 0, curPhase);
                    DoScriptText(SAY_REMORSELESSS_WINTER, me);
                    DoCast(me, RAID_MODE(SPELL_REMORSELESS_WINTER_10_N, SPELL_REMORSELESS_WINTER_25_N, SPELL_REMORSELESS_WINTER_10_H, SPELL_REMORSELESS_WINTER_25_H), true);
                    break;
                }
            }
        }

        void JustReachedHome()
        {
            me->SetStandState(UNIT_STAND_STATE_SIT);
            instance->SetBossState(DATA_THE_LICH_KING, FAIL);
            Cleanup();
            if(Creature* tirion = ObjectAccessor::GetCreature(*me, instance->GetData64(GUID_TIRION)))
                tirion->AI()->DoAction(ACTION_RESET);
            events.Reset();
            uiStage = 1;
        }

        void KilledUnit(Unit* victim)
        {
            if(GetPhase(events) != PHASE_6_ENDING && victim->GetTypeId() == TYPEID_PLAYER)
                DoScriptText(RAND(SAY_KILL_1, SAY_KILL_2), me);
        }

        void JustSummoned(Creature* pSummoned)
        {
            summons.Summon(pSummoned);
            switch(pSummoned->GetEntry())
            {
                case NPC_RAGING_SPIRIT:
                    pSummoned->CastSpell(pSummoned, SPELL_NECROTIC_PLAGUE_IMMUNITY, true);
                    if(Unit* target = pSummoned->SelectNearbyTarget())
                    {
                        target->CastSpell(pSummoned, SPELL_RAGING_VISUAL, true);
                        pSummoned->AI()->AttackStart(target);
                    }
                    else if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 20.0f, true))
                    {
                        target->CastSpell(pSummoned, SPELL_RAGING_VISUAL, true);
                        pSummoned->AI()->AttackStart(target);
                    }
                    else
                    {
                        //There is no target - unsummon raging spirit
                        pSummoned->Kill(pSummoned);
                        pSummoned->DespawnOrUnsummon();
                    }
                    break;
                case NPC_DEFILE:
                {
                    Position pos;
                    pSummoned->GetPosition(&pos);
                    pos.m_positionZ += 0.75f;
                    pSummoned->SetPosition(pos, true);
                    pSummoned->SetInCombatWithZone();
                    break;
                }
                case NPC_TRIGGER:
                    pSummoned->AI()->AttackStart(me);
                    pSummoned->SetVisible(false);
                    pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    break;
                case NPC_FROSTMOURNE_TRIGGER:
                    pSummoned->CastSpell(pSummoned, SPELL_BROKEN_FROSTMOURNE, false);
                    pSummoned->CastSpell(pSummoned, SPELL_FROSTMOURNE_TRIGGER, false);
                    break;
                case NPC_TERENAS_MENETHIL:
                    DoScriptText(SAY_ENDING_9_FATHER, pSummoned);
                    break;
                case NPC_VALKYR:
                    if(Unit* valkyrTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                        pSummoned->AI()->SetGUID(valkyrTarget->GetGUID(), TYPE_VICTIM);
                    break;
                case NPC_DRUDGE_GHOUL:
                    pSummoned->CastSpell(pSummoned, SPELL_RAGING_GHOUL_VISUAL, true);
                    if(Unit* pVictim = SelectTarget(SELECT_TARGET_RANDOM))
                        pSummoned->AI()->AttackStart(pVictim);
                    break;
                case NPC_SHAMBLING_HORROR:
                    pSummoned->CastSpell(pSummoned, SPELL_RISEN_WITCH_DOCTOR_SPAWN, true);
                    if(Unit* pVictim = SelectTarget(SELECT_TARGET_RANDOM))
                        pSummoned->AI()->AttackStart(pVictim);
                    break;
                case NPC_ICE_SPHERE:
                    if(Unit* victim = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                        if(pSummoned->AI())
                            pSummoned->AI()->SetGUID(victim->GetGUID(), TYPE_VICTIM);
                    break;
            }
        }

        void DoAction(const int32 action)
        {
            switch(action)
            {
                case ACTION_PREPARE_FROSTMOURNE_ROOM:
                {
                    Creature* terenasFighter = ObjectAccessor::GetCreature(*me, instance->GetData64(GUID_TERENAS_FIGHTER));
                    Creature* spiritWarden = ObjectAccessor::GetCreature(*me, instance->GetData64(GUID_SPIRIT_WARDEN));
                    if(terenasFighter && spiritWarden)
                    {
                        terenasFighter->Respawn();
                        spiritWarden->Respawn();
                        terenasFighter->AI()->DoAction(ACTION_ATTACK_SPIRIT_WARDEN);
                        spiritWarden->AI()->DoAction(ACTION_ATTACK_TERENAS_FIGHTER);
                        events.ScheduleEvent(EVENT_KILL_FROSTMOURNE_PLAYERS, 60000, 0, PHASE_5);
                    }
                    break;
                }
                case ACTION_CANCEL_ALL_TRANSITION_EVENTS:
                {
                    events.CancelEvent(EVENT_PAIN_AND_SUFFERING);
                    events.CancelEvent(EVENT_SUMMON_ICE_SPHERE);
                    events.CancelEvent(EVENT_SUMMON_RAGING_SPIRIT);
                    events.CancelEvent(EVENT_TRANSITION_PHASE_END);
                    break;
                }
                case ACTION_PHASE_SWITCH_1:
                {
				    if(GetPhase(events) == PHASE_1) events.CancelEvent(EVENT_NECROTIC_PLAGUE);
                    uint32 nextPhase = PHASE_2_TRANSITION;
                    if(GetPhase(events) == PHASE_3)
                        nextPhase = PHASE_4_TRANSITION;
                    events.SetPhase(nextPhase);
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
                    SetCombatMovement(false);
                    me->SetInCombatWithZone();
                    me->GetMotionMaster()->MovePoint(POINT_PLATFORM_CENTER, MovePos[1]);
                    instance->DoRemoveAurasDueToSpellOnPlayers(RAID_MODE(SPELL_NECROTIC_PLAGUE_10_N, SPELL_NECROTIC_PLAGUE_25_N, SPELL_NECROTIC_PLAGUE_10_H, SPELL_NECROTIC_PLAGUE_25_H));
                    instance->DoRemoveAurasDueToSpellOnPlayers(RAID_MODE(SPELL_NECROTIC_PLAGUE_EFFECT_10_N, SPELL_NECROTIC_PLAGUE_EFFECT_25_N, SPELL_NECROTIC_PLAGUE_EFFECT_10_H, SPELL_NECROTIC_PLAGUE_EFFECT_25_H));
                    break;
                }
                case ACTION_PHASE_SWITCH_2:
                {
                    if(GetPhase(events) == PHASE_2_TRANSITION)
                    {
                        events.SetPhase(PHASE_3);
                        events.ScheduleEvent(EVENT_SUMMON_VAL_KYR_SHADOWGUARD, 13000, 0, PHASE_3);
                        events.ScheduleEvent(EVENT_SOUL_REAPER, 35000, 0, PHASE_3);
                        events.ScheduleEvent(EVENT_DEFILE, 32000, 0, PHASE_3);
                        events.ScheduleEvent(EVENT_INFEST, 8000, 0, PHASE_3);
                    }
                    else
                    {
                        events.SetPhase(PHASE_5);
                        events.ScheduleEvent(EVENT_SUMMON_VILE_SPIRITS, 15000, 0, PHASE_5);
                        events.ScheduleEvent(EVENT_SOUL_REAPER, 35000, 0, PHASE_5);
                        events.ScheduleEvent(EVENT_DEFILE, 32000, 0, PHASE_5);
                        events.ScheduleEvent(EVENT_HARVEST_SOUL, 7000, 0, PHASE_5);
                    }
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->RemoveAurasDueToSpell(RAID_MODE<uint32>(SPELL_PAIN_AND_SUFFERING_10_N, SPELL_PAIN_AND_SUFFERING_25_N, SPELL_PAIN_AND_SUFFERING_10_H, SPELL_PAIN_AND_SUFFERING_25_H));
                    DoZoneInCombat();
                    SetCombatMovement(true);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
                    isSwitching = false;
                    break;
                }
            }
        }

        void DamageTaken(Unit* /*done_by*/, uint32& /*damage*/)
        {
            switch(GetPhase(events))
            {
                case PHASE_1:
                    if(!HealthAbovePct(71) && !isSwitching)
                    {
                        isSwitching = true;
                        DoAction(ACTION_PHASE_SWITCH_1);
                        break;
                    }
                case PHASE_3:
                    if(!HealthAbovePct(41) && !isSwitching)
                    {
                        isSwitching = true;
                        DoAction(ACTION_PHASE_SWITCH_1);
                        break;
                    }
                case PHASE_5:
                    if(!HealthAbovePct(11) && !isSwitching)
                    {
                        isSwitching = true;
                        me->SummonCreature(NPC_TRIGGER, MovePos[6], TEMPSUMMON_CORPSE_DESPAWN, 900000);
                        summons.DespawnAll();
                        events.Reset();
                        events.SetPhase(PHASE_6_ENDING);
                    }
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(GetPhase(events) != PHASE_6_ENDING
                && (!UpdateVictim() || !CheckInRoom()))
                return;

            events.Update(diff);

            if(me->HasUnitState(UNIT_STAT_CASTING) && GetPhase(events) != PHASE_6_ENDING)
                return;
            while(uint32 eventId = events.ExecuteEvent())
            {
                //Remove players from threat list if they fall down
                if(eventId == EVENT_CHECK_ALIVE_PLAYERS)
                {
                    if(Creature* pTirion = ObjectAccessor::GetCreature(*me, instance->GetData64(GUID_TIRION)))
                    {
                        TPlayerList players = GetPlayersInTheMap(me->GetMap());
                        for(TPlayerList::iterator it = players.begin(); it != players.end(); ++it)
                        {
                            if((*it)->GetDistanceZ(pTirion) > 10.0f && !(*it)->HasAura(SPELL_IN_FROSTMOURNE_ROOM))
                            {
                                (*it)->RemoveAllAuras();
                                me->DealDamage(*it, (*it)->GetHealth());
                                pTirion->CastSpell(*it, FROZEN_THRONE_TELEPORT, true);
                            }
                        }
                    }
                    events.ScheduleEvent(EVENT_CHECK_ALIVE_PLAYERS, 5000);
                }
                switch(GetPhase(events))
                {
                    case PHASE_1:
                    {
                        switch(eventId)
                        {
                            case EVENT_SPEECH:
                            {
                                DoScriptText(RAND(SAY_RANDOM_1, SAY_RANDOM_2), me);
                                events.ScheduleEvent(EVENT_SPEECH, 33000, 0, PHASE_1);
                                break;
                            }
                            case EVENT_SUMMON_SHAMBLING_HORROR:
                            {
                                DoCast(SPELL_SUMMON_SHAMBLING_HORROR);
                                events.ScheduleEvent(EVENT_SUMMON_SHAMBLING_HORROR, RAID_MODE(60000, 60000, 35000, 35000), 0, PHASE_1);
                                break;
                            }
                            case EVENT_SUMMON_DRUDGE_GHOULS:
                            {
                                DoCast(SPELL_SUMMON_DRUDGE_GHOULS);
                                events.ScheduleEvent(EVENT_SUMMON_DRUDGE_GHOULS, 30000, 0, PHASE_1);
                                break;
                            }
                            case EVENT_INFEST:
                            {
                                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -SPELL_INFEST_10_N))
                                    DoCast(target, RAID_MODE(SPELL_INFEST_10_N, SPELL_INFEST_25_N, SPELL_INFEST_10_H, SPELL_INFEST_25_H));
                                events.ScheduleEvent(EVENT_INFEST, 20000, 0, PHASE_1);
                                break;
                            }
                            case EVENT_NECROTIC_PLAGUE:
                            {
                                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -EVENT_NECROTIC_PLAGUE))
                                    DoCast(target, RAID_MODE(SPELL_NECROTIC_PLAGUE_10_N, SPELL_NECROTIC_PLAGUE_25_N, SPELL_NECROTIC_PLAGUE_10_H, SPELL_NECROTIC_PLAGUE_25_H));
                                events.ScheduleEvent(EVENT_NECROTIC_PLAGUE, 25000, 0, PHASE_1);
                                break;
                            }
                            case EVENT_SHADOW_TRAP:
                            {
                                ASSERT(IsHeroic());
                                //First, try to select somebody far away from the boss
                                Unit* pTarget = NULL;
                                pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, -5.0f, true);
                                if(!pTarget)
                                    pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true);
                                if(pTarget)
                                    DoCast(pTarget, SPELL_SUMMON_SHADOW_TRAP, true);
                                events.ScheduleEvent(EVENT_SHADOW_TRAP, 30000, 0, PHASE_1);
                            }
                        }
                        break;
                    }
                    case PHASE_2_TRANSITION:
                    case PHASE_4_TRANSITION:
                    {
                        switch(eventId)
                        {
                            case EVENT_PAIN_AND_SUFFERING:
                            {
                                if(Player* randomPlayer = SelectRandomAttackablePlayerInTheMap(me->GetMap()))
                                {
                                    me->SetFacingToObject(randomPlayer);
                                    DoCast(randomPlayer, RAID_MODE<uint32>(SPELL_PAIN_AND_SUFFERING_10_N, SPELL_PAIN_AND_SUFFERING_25_N, SPELL_PAIN_AND_SUFFERING_10_H, SPELL_PAIN_AND_SUFFERING_25_H), true);
                                }
                                events.ScheduleEvent(EVENT_PAIN_AND_SUFFERING, 1500, 0, GetPhase(events));
                                break;
                            }
                            case EVENT_SUMMON_RAGING_SPIRIT:
                            {
                                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                    DoCast(target, SPELL_SUMMON_RAGING_SPIRIT);
                                events.RescheduleEvent(EVENT_PAIN_AND_SUFFERING, 3000, 0, PHASE_2_TRANSITION);
                                events.ScheduleEvent(EVENT_SUMMON_RAGING_SPIRIT, RAID_MODE<uint32>(60000, 40000, 60000, 40000), 0, GetPhase(events));
                                break;
                            }
                            case EVENT_SUMMON_ICE_SPHERE:
                            {
                                events.RescheduleEvent(EVENT_PAIN_AND_SUFFERING, 3000, 0, GetPhase(events));
                                DoCast(SPELL_SUMMON_ICE_SPEHERE);
                                events.ScheduleEvent(EVENT_SUMMON_ICE_SPHERE, urand(6000, 8000), 0, GetPhase(events));
                                break;
                            }
                            case EVENT_BERSERK:
                            {
                                events.Reset();
                                DoScriptText(SAY_BERSERK, me);
                                DoCast(me, SPELL_BERSERK2);
                                break;
                            }
                        }
                        break;
                    }
                    case PHASE_3:
                    {
                        switch(eventId)
                        {
                            case EVENT_SUMMON_VAL_KYR_SHADOWGUARD:
                            {
                                DoScriptText(SAY_SUMMON_VALKYR, me);
                                if(instance->instance->Is25ManRaid())
                                    DoCast(me, SPELL_SUMMON_VALKYR_25, true);
                                else
                                    DoCastAOE(SPELL_SUMMON_VALKYR_10);
                                events.ScheduleEvent(EVENT_SUMMON_VAL_KYR_SHADOWGUARD, urand(40000, 45000), 0, PHASE_3);
                                break;
                            }
                            case EVENT_DEFILE:
                            {
                                DoScriptText(SAY_EMOTE_DEFILE, me);
                                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                    DoCast(target, SPELL_SUMMON_DEFILE);
                                events.ScheduleEvent(EVENT_DEFILE, urand(40000, 50000), 0, PHASE_3);
                                break;
                            }
                            case EVENT_SOUL_REAPER:
                            {
                                DoCastVictim(SPELL_SOUL_REAPER);
                                DoCast(SPELL_SOUL_REAPER_HASTE_AURA);
                                events.ScheduleEvent(EVENT_SOUL_REAPER, 30000, 0, PHASE_3);
                                break;
                            }
                            case EVENT_INFEST:
                            {
                                if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -SPELL_INFEST_10_N))
                                    DoCast(pTarget, RAID_MODE(SPELL_INFEST_10_N, SPELL_INFEST_25_N, SPELL_INFEST_10_H, SPELL_INFEST_25_H));
                                events.ScheduleEvent(EVENT_INFEST, 20000, 0, PHASE_3);
                                break;
                            }
                            case EVENT_BERSERK:
                            {
                                DoScriptText(SAY_BERSERK, me);
                                DoCast(me, SPELL_BERSERK2);
                                break;
                            }
                        }
                        break;
                    }
                    case PHASE_5:
                    {
                        switch(eventId)
                        {
                            case EVENT_DEFILE:
                            {
                                DoScriptText(SAY_EMOTE_DEFILE, me);
                                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                    DoCast(target, SPELL_SUMMON_DEFILE);
                                events.ScheduleEvent(EVENT_DEFILE, 35000, 0, PHASE_5);
                                break;
                            }
                            case EVENT_SOUL_REAPER:
                            {
                                DoCastVictim(SPELL_SOUL_REAPER);
                                DoCast(SPELL_SOUL_REAPER_HASTE_AURA);
                                events.ScheduleEvent(EVENT_SOUL_REAPER, 30000, 0, PHASE_5);
                                break;
                            }
                            case EVENT_SUMMON_VILE_SPIRITS:
                            {
                                DoCast(me, SPELL_SUMMON_VILE_SPIRIT);
                                events.ScheduleEvent(EVENT_SUMMON_VILE_SPIRITS, 30000, 0, PHASE_5);
                                break;
                            }
                            case EVENT_HARVEST_SOUL:
                            {
                                DoScriptText(SAY_HARVEST_SOUL, me);
                                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                                    DoCast(target, SPELL_HARVEST_SOULS);
                                events.ScheduleEvent(EVENT_HARVEST_SOUL, 75000, 0, PHASE_5);
                                break;
                            }
                            case EVENT_BERSERK:
                            {
                                DoScriptText(SAY_BERSERK, me);
                                DoCast(me, SPELL_BERSERK2);
                                break;
                            }
                        }
                        break;
                    }
                }
            }

            if(GetPhase(events) == PHASE_6_ENDING)
            {
                if(uiStage > 24)
                    return;
                if(uiEndingTimer <= diff)
                {
                    switch(uiStage)
                    {
                        case 1:
                        {
                            //Teleport all players who are inside Frostmourne back to Frozen Throne platform
                            TPlayerList players = GetPlayersInTheMap(me->GetMap());
                            for(TPlayerList::iterator it = players.begin(); it != players.end(); ++it)
                                if((*it)->HasAura(SPELL_IN_FROSTMOURNE_ROOM))
                                {
                                    (*it)->RemoveAurasDueToSpell(SPELL_IN_FROSTMOURNE_ROOM);
                                    TeleportPlayerToFrozenThrone(*it);
                                }
                            if(Creature* terenasFighter = ObjectAccessor::GetCreature(*me, GUID_TERENAS_FIGHTER))
                                terenasFighter->AI()->DoAction(ACTION_DESPAWN);
                            if(Creature* spiritWarden = ObjectAccessor::GetCreature(*me, GUID_SPIRIT_WARDEN))
                                spiritWarden->AI()->DoAction(ACTION_DESPAWN);
                            me->GetMotionMaster()->MoveIdle();
                            me->SetReactState(REACT_PASSIVE);
                            me->AttackStop();
                            me->CastStop();
                            me->SetInCombatWithZone();
                            if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                me->SetInCombatWith(tirion);
                            DoScriptText(SAY_10_PERCENT, me);
                            //Wait for everyone who is inside the Frostmourne Room to get teleported to the Platform
                            uiEndingTimer = 5000;
                            break;
                        }
                        case 2:
                        {
                            DoCast(me, SPELL_FURY_OF_FROSTMOURNE_NORES);
                            DoCast(me, SPELL_FURY_OF_FROSTMOURNE);
                            uiEndingTimer = 11000;
                        }
                        case 3:
                        {
                            DoScriptText(SAY_ENDING_1_KING, me);
                            uiEndingTimer = 24000;
                            break;
                        }
                        case 4:
                        {
                            DoScriptText(SAY_ENDING_2_KING, me);
                            uiEndingTimer = 25000;
                            break;
                        }
                        case 5:
                        {
                            me->GetMotionMaster()->MovePoint(0, MovePos[1]);
                            uiEndingTimer = 4000;
                            break;
                        }
                        case 6:
                        {
                            DoScriptText(SAY_ENDING_3_KING, me);
                            DoCast(me, SPELL_RAISE_DEAD);
                            me->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
                            uiEndingTimer = 28000;
                            break;
                        }
                        case 7:
                        {
                            DoScriptText(SAY_ENDING_4_KING, me); //I delight the irony.
                            uiEndingTimer = 8000;
                            break;
                        }
                        case 8:
                        {
                            if(uiTirionGUID)
                                if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                    DoScriptText(SAY_ENDING_5_TIRION, tirion); //Light grant me one final blessing
                            uiEndingTimer = 11000;
                            break;
                        }
                        case 9:
                        {
                            if(uiTirionGUID)
                                if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                    tirion->CastSpell(tirion, SPELL_TIRION_LIGHT, true);
                            uiEndingTimer = 7000;
                            break;
                        }
                        case 10:
                        {
                            if(uiTirionGUID)
                                if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                    tirion->GetMotionMaster()->MovePoint(0, MovePos[2]);
                            uiEndingTimer = 2000;
                            break;
                        }
                        case 11:
                        {
                            if(uiTirionGUID)
                            {
                                if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                {
                                    tirion->SetTarget(me->GetGUID());
                                    tirion->GetMotionMaster()->MoveJump(517.482910f, -2124.905762f, 1040.861328f, 10.0f, 15.0f);
                                    tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, 375);
                                }
                            }
                            me->RemoveAura(SPELL_RAISE_DEAD);
                            me->CastSpell(me, SPELL_BOOM_VISUAL, false);
                            uiEndingTimer = 1000;
                            break;
                        }
                        case 12:
                        {
                            if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                tirion->SetFacingToObject(me);
                            uiEndingTimer = 1000;
                            break;
                        }
                        case 13:
                        {
                            me->CastSpell(me, SPELL_SUMMON_BROKEN_FROSTMOURNE, false);
                            uiEndingTimer = 4000;
                            break;
                        }
                        case 14:
                        {
                            me->CastSpell(me, SPELL_DROP_FROSTMOURNE, false);
                            uiEndingTimer = 1000;
                            break;
                        }
                        case 15:
                        {
                            DoCast(me, SPELL_SOUL_EFFECT);
                            DoPlaySoundToSet(me, SOUND_ENDING_7_KING);
                            uiEndingTimer = 1000;
                            break;
                        }
                        case 16:
                        {
                            SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                            DoScriptText(SAY_ENDING_6_KING, me);
                            uiEndingTimer = 3000;
                            break;
                        }
                        case 17:
                        {
                            DoCast(me, SPELL_SUMMON_FROSTMOURNE_TRIGGER);
                            uiEndingTimer = 2000;
                            break;
                        }
                        case 18:
                        {
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 473);
                            SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                            uiEndingTimer = 5000;
                            break;
                        }
                        case 19:
                        {
                            if(uiTirionGUID)
                                if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                    DoScriptText(SAY_ENDING_8_TIRION, tirion);
                            uiEndingTimer = 6000;
                            break;
                        }
                        case 20:
                        {
                            if(Creature* frostmourne = me->FindNearestCreature(NPC_FROSTMOURNE_TRIGGER, 25.0f, true))
                                frostmourne->CastSpell(frostmourne, SPELL_SUMMON_MENETHIL, true);
                            uiEndingTimer = 3000;
                            break;
                        }
                        case 21:
                        {
                            if(Creature* father = me->FindNearestCreature(NPC_TERENAS_MENETHIL, 25.0f, true))
                            {
                                DoScriptText(SAY_ENDING_11_FATHER, father);
                                father->SetFacingToObject(me);
                                father->CastSpell(father, SPELL_MENETHIL_VISUAL, true);
                                father->CastSpell(father, SPELL_REVIVE, true);
                            }
                            uiEndingTimer = 6000;
                            break;
                        }
                        case 22:
                        {
                            if(uiTirionGUID)
                                if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                    DoScriptText(SAY_ENDING_10_TIRION, tirion);
                            uiEndingTimer = 5000;
                            break;
                        }
                        case 23:
                        {
                            DoScriptText(SAY_ENDING_12_KING, me);
                            me->GetMotionMaster()->MovePoint(0, MovePos[6]);
                            if(uiTirionGUID)
                                if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                    tirion->AI()->AttackStart(me);
                            if(Creature* father = me->FindNearestCreature(NPC_TERENAS_MENETHIL, 25.0f, true))
                                father->AI()->AttackStart(me);
                            uiEndingTimer = 10000;
                            break;
                        }
                        case 24:
                        {
                            DoScriptText(SAY_DEATH_KING, me);
                            break;
                        }
                    }
                    ++uiStage;
                } else uiEndingTimer -= diff;
            }
            switch(GetPhase(events))
            {
                case PHASE_1:
                case PHASE_3:
                case PHASE_5:
                    DoMeleeAttackIfReady();
                    break;
            }
        }
    private:
        uint32 uiEndingTimer;
        uint32 uiSummonShamblingHorrorTimer;
        uint32 uiSummonDrudgeGhoulsTimer;
        uint32 uiSummonShadowTrap;
        uint32 uiInfestTimer;
        uint32 uiNecroticPlagueTimer;
        uint32 uiSummonValkyrTimer;
        uint32 uiSoulReaperTimer;
        uint32 uiDefileTimer;
        uint32 uiHarvestSoulTimer;
        uint32 uiSummonVileSpiritTimer;
        uint32 uiIcePulsSummonTimer;
        uint32 uiSummonSpiritTimer;
        uint64 uiTirionGUID;
        bool isSwitching;
        SummonList summons;
        EventMap events;

        uint8 uiStage;
        uint8 uiPhase;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<boss_the_lich_kingAI>(pCreature);
    }
};

class npc_tirion_icc : public CreatureScript
{
public:
    npc_tirion_icc() : CreatureScript("npc_tirion_icc") { }

    struct npc_tirion_iccAI : public ScriptedAI
    {
        npc_tirion_iccAI(Creature* pCreature): ScriptedAI(pCreature), _instance(pCreature->GetInstanceScript()) { }

        void Reset()
        {
            uiIntroTimer = 1000;
            uiStage = 1;
            uiLichKingGUID = 0;
            bIntro = false;

            me->RemoveAllAuras();
            me->SetReactState(REACT_PASSIVE);
            me->SetSpeed(MOVE_RUN, 1.8f);
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            me->GetMotionMaster()->MovePoint(POINT_PLATFORM_CENTER, MovePos[8]);
            DoCast(me, SPELL_REVIVE, true);
            DoCast(SPELL_WMO_INTACT);
            me->CastSpell(me, SPELL_WMO_REBUILD, true);
            //Rebuilding ice shards
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_ICE_SHARD_1)))
                go->SetGoState(GO_STATE_READY);
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_ICE_SHARD_2)))
                go->SetGoState(GO_STATE_READY);
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_ICE_SHARD_3)))
                go->SetGoState(GO_STATE_READY);
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_ICE_SHARD_4)))
                go->SetGoState(GO_STATE_READY);
            //Hiding edge destroy warning
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_EDGE_DESTROY_WARNING)))
                go->SetGoState(GO_STATE_READY);
            //Hiding inner waterfall
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_FROSTY_EDGE_INNER)))
                go->SetGoState(GO_STATE_READY);
            //Showing outer waterfall
            if(GameObject* go = ObjectAccessor::GetGameObject(*me, _instance->GetData64(GUID_FROSTY_EDGE_OUTER)))
                go->SetGoState(GO_STATE_ACTIVE);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if(type != POINT_MOTION_TYPE)
                return;
            switch(id)
            {
                case POINT_PLATFORM_CENTER:
                {
                    if(me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                        me->GetMotionMaster()->MovementExpired();
                    me->SetPosition(MovePos[8]);
                    me->SetOrientation(3.1416f);
                    me->SetFacingTo(3.1416f);
                    break;
                }
            }
        }

        void DoAction(const int32 action)
        {
            switch(action)
            {
                case ACTION_START_EVENT:
                {
                    bIntro = true;
                    if(_instance)
                        uiLichKingGUID = _instance->GetData64(DATA_THE_LICH_KING);
                    break;
                }
                case ACTION_RESET:
                {
                    Reset();
                    break;
                }
            }
        }

        void SpellHit(Unit* /*pCaster*/, const SpellEntry * spell)
        {
            if(spell->Id == SPELL_LIGHTS_BLESSING)
            {
                me->RemoveAurasDueToSpell(SPELL_ICEBLOCK_TRIGGER);
                if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                    me->SetFacingToObject(lich);
            }
            if(spell->Id == SPELL_ICEBLOCK_TRIGGER)
            {
                me->SetOrientation(3.1416f);
                me->SetFacingTo(3.1416f);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!bIntro || !uiLichKingGUID)
                return;
            if(uiStage > 11)
                return;
            if(uiIntroTimer <= diff)
            {
                switch(uiStage)
                {
                    case 1:
                    {
                        if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                        {
                            lich->SetStandState(UNIT_STAND_STATE_STAND);
                            lich->GetMotionMaster()->MovePoint(POINT_START_EVENT_1, MovePos[0]);
                            me->SetFacingToObject(lich);
                        }
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                        uiIntroTimer = 3000;
                        break;
                    }
                    case 2:
                    {
                        if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                        {
                            lich->SetFacingToObject(me);
                            lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                            me->SetTarget(lich->GetGUID());
                            lich->SetTarget(me->GetGUID());
                            DoScriptText(SAY_INTRO_1_KING, lich);
                        }
                        uiIntroTimer = 14000;
                        break;
                    }
                    case 3:
                    {
                        if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                            lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                        DoScriptText(SAY_INTRO_2_TIRION, me);
                        uiIntroTimer = 9000;
                        break;
                    }
                    case 4:
                    {
                        if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                        {
                            lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_LAUGH);
                            DoScriptText(SAY_INTRO_3_KING, lich);
                        }
                        uiIntroTimer = 3000;
                        break;
                    }
                    case 5:
                    {
                        if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                            lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_POINT_NO_SHEATHE);
                        uiIntroTimer = 2000;
                        break;
                    }
                    case 6:
                    {
                        if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                            lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                        uiIntroTimer = 18000;
                        break;
                    }
                    case 7:
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_POINT_NO_SHEATHE);
                        DoScriptText(SAY_INTRO_4_TIRION, me);
                        uiIntroTimer = 1000;
                        break;
                    case 8:
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                        me->GetMotionMaster()->MovePoint(0, MovePos[3]);
                        uiIntroTimer = 2000;
                        break;
                    case 9:
                    {
                        if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                            lich->CastSpell(me, SPELL_ICEBLOCK_TRIGGER, true);
                        uiIntroTimer = 2000;
                        break;
                    }
                    case 10:
                    {
                        if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                            DoScriptText(SAY_INTRO_5_KING, lich);
                        uiIntroTimer = 18000;
                        break;
                    }
                    case 11:
                    {
                        if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                        {
                            lich->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
                            lich->SetReactState(REACT_AGGRESSIVE);
                            if(Unit* target = lich->FindNearestPlayer(100.0f))
                                lich->AI()->AttackStart(target);
                        }
                        break;
                    }
                }
                ++uiStage;
            } else uiIntroTimer -= diff;
        }
        private:
            InstanceScript* _instance;

            uint64 uiLichKingGUID;
            uint32 uiIntroTimer;
            uint8 uiStage;
            bool bIntro;
    };

    bool OnGossipHello(Player* player, Creature* creature)
    {
        InstanceScript* _instance = creature->GetInstanceScript();
        if(!_instance)
            return false;

        Player* unfriendlyPlayer = NULL;
        const Map::PlayerList &PlayerList = creature->GetMap()->GetPlayers();
        if(!PlayerList.isEmpty())
            for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                if(Player* player = i->getSource())
                    if(!creature->IsFriendlyTo(player))
                    {
                        unfriendlyPlayer = player;
                        break;
                    }

        if(unfriendlyPlayer)
        {
            char buf[255] = {0};
            sprintf(buf, "Désolé, mais tout le monde dans le raid doit avoir au moins la réputation amicale avec la Croisade d'argent pour participer à la bataille finale. Le joueur '%s' ne répond pas à cette exigence.", unfriendlyPlayer->GetName());
            creature->MonsterSay(buf, LANG_UNIVERSAL, player->GetGUID());
            return true;
        }

        if(_instance->GetBossState(DATA_THE_LICH_KING) == DONE)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Le Roi liche a déjà été vaincu ici. Me téléporter au Marteau de la Lumière", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            player->SEND_GOSSIP_MENU(GOSSIP_MENU, creature->GetGUID());
            return true;
        }

        if((!player->GetGroup() || !player->GetGroup()->IsLeader(player->GetGUID())) && !player->isGameMaster())
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Désolé, je ne suis pas le chef de raid", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->SEND_GOSSIP_MENU(GOSSIP_MENU, creature->GetGUID());
            return true;
        }

        if((_instance->GetBossState(DATA_BLOOD_QUEEN_LANA_THEL) == DONE && _instance->GetBossState(DATA_PROFESSOR_PUTRICIDE) == DONE && _instance->GetBossState(DATA_SINDRAGOSA) == DONE) || player->isGameMaster())
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_START_EVENT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);

        player->SEND_GOSSIP_MENU(GOSSIP_MENU, creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+2:
                creature->MonsterSay("OK, je vais attendre le chef de raid", LANG_UNIVERSAL, player->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                creature->CastSpell(player,LIGHT_S_HAMMER_TELEPORT, true); player->GetGUID();
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                CAST_AI(npc_tirion_icc::npc_tirion_iccAI, creature->AI())->DoAction(ACTION_START_EVENT);
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                player->CLOSE_GOSSIP_MENU();
                break;
            default:
                creature->MonsterSay("Vous avez trouvé une erreur. Contactez administrateur du serveur et expliquez ce qu'il faut faire pour reproduire ce bug", LANG_UNIVERSAL, player->GetGUID());
                break;
        }
        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_tirion_iccAI(pCreature);
    }
};

class npc_valkyr_icc : public CreatureScript
{
enum eEvents
{
    EVENT_MOVE_TO_PLATFORM_EDGE = 1,
    EVENT_SIPHON_LIFE,
    EVENT_CHECK_AT_PLATFORM_EDGE,
    EVENT_CHARGE_PLAYER
};

static const float Z_FLY;
public:
    npc_valkyr_icc() : CreatureScript("npc_valkyr_icc") { }

    struct npc_valkyr_iccAI : public ScriptedAI
    {
        npc_valkyr_iccAI(Creature* pCreature): ScriptedAI(pCreature), vehicle(pCreature->GetVehicleKit()), m_victimGuid(0)
        {
            ASSERT(vehicle);
        }

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);
            m_moveUpdatePeriod  = 100;
            events.Reset();
            bCanCast = false;
            me->SetFlying(true);
            me->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING | MOVEMENTFLAG_CAN_FLY);
            me->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()+7.0f);
            events.ScheduleEvent(EVENT_CHARGE_PLAYER, 2000);
            DoCast(me, SPELL_WINGS_OF_THE_DAMNED, true);
        }

        void DamageTaken(Unit* /*done_by*/, uint32& /*damage*/)
        {
            if(!HealthAbovePct(50) && IsHeroic() && !bCanCast)
            {
                vehicle->RemoveAllPassengers();
                me->RemoveAllAuras();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->GetMotionMaster()->MovePoint(POINT_VALKYR_ZET, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()+8.0f);
                events.Reset();
            }
        }

        void SetGUID(const uint64& guid, int32 type)
        {
            if(type == TYPE_VICTIM)
                m_victimGuid = guid;
        }

        void DoAction(const int32 action)
        {
            switch(action)
            {
                case ACTION_CHARGE_PLAYER:
                {
                    events.ScheduleEvent(EVENT_CHARGE_PLAYER, 2000);
                    break;
                }
            }
        }

        void SpellHitTarget(Unit* pVictim, SpellEntry const* pSpellEntry)
        {
            if(pSpellEntry->Id == SPELL_VALKYR_CHARGE)
                if(Player* pPlayer = ObjectAccessor::GetPlayer(*me, m_victimGuid))
                    DoCast(pPlayer, SPELL_VALKYR_CARRY_CAN_CAST, true);
        }

        void SpellHit(Unit* attacker, const SpellEntry* pSpellEntry)
        {
            if(!pSpellEntry)
                return;

            switch(pSpellEntry->Id)
            {
                case SPELL_VALKYR_GRAB_PLAYER:
                {
                    float speedRate = me->GetSpeedRate(MOVE_RUN);
                    speedRate = 0.20f;
                    me->SetSpeed(MOVE_FLIGHT, speedRate);
                    me->SetSpeed(MOVE_RUN, speedRate);

                    float x, y, z;
                    me->GetPosition(x, y, z);
                    me->SetPosition(x, y, Z_FLY, 0.0f, true);

                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    SetCombatMovement(false);
                    me->SetInCombatWithZone();

                    me->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
                    me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_DEATH_GRIP, true);
                    me->ApplySpellImmune(0, IMMUNITY_ID, 33786, true);
                    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                    me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CHARM, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_FEAR, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_ROOT, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_PACIFY, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_SILENCE, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_TRANSFORM, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_SCALE, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_DISARM, true);
                    //The Val'kyrs always choose the closest edge of the platform).
                    std::list<Creature*> allEdgeStalkers;
                    GetCreatureListWithEntryInGrid(allEdgeStalkers, me, NPC_PLATFORM_DESTRUCTIBLE_EDGE_STALKER, 200.0f);
                    if(!allEdgeStalkers.empty())
                    {
                    //    me->Kill(me);
                    //    me->DespawnOrUnsummon();
                    //}
                    //else
                    //{
                        Creature* nearestEdgeStalker = allEdgeStalkers.front();
                        for(std::list<Creature*>::iterator it = allEdgeStalkers.begin(); it != allEdgeStalkers.end(); ++it)
                            if(me->GetDistance2d(nearestEdgeStalker) > me->GetDistance2d(*it))
                                nearestEdgeStalker = *it;
                        me->GetPosition(&m_pos);
                        m_pos.m_positionZ = Z_FLY;
                        me->SetPosition(m_pos, true);
                        float ex, ey, ez;
                        nearestEdgeStalker->GetPosition(ex, ey, ez);
                        float distanceToEdge = m_pos.GetExactDist2d(ex, ey);
                        distanceToEdge += 10.0f;
                        m_angle = m_pos.GetAngle(ex, ey);
                        me->GetNearPoint2D(ex, ey, distanceToEdge, m_angle);
                        me->SetFacingToObject(nearestEdgeStalker);
                        me->GetMotionMaster()->MovePoint(POINT_PLATFORM_END, ex, ey, Z_FLY);
                    }
                }
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if(type != POINT_MOTION_TYPE)
                return;
            if(bCanCast)
                me->GetMotionMaster()->Clear();
            switch(id)
            {
                case POINT_PLATFORM_END:
                {
                    DoCast(me, SPELL_VALKYR_EJECT_PASSENGER);
                    //Fly 15 feet upward, then despawn
                    me->GetPosition(&m_pos);
                    m_pos.m_positionZ = 1055.0f;
                    me->GetMotionMaster()->MovePoint(POINT_VALKYR_END, m_pos);
                    break;
                }
                case POINT_VALKYR_END:
                {
                    sLog->outErrorDb("TRALALA");
                    me->DespawnOrUnsummon();
                    break;
                }
                case POINT_VALKYR_ZET:
                {
                    events.ScheduleEvent(EVENT_SIPHON_LIFE, 3000);
                    bCanCast = true;
                    break;
                }
                case POINT_VALKYR_CONTINUE_FLYING:
                {
                    me->SetPosition(m_pos);
                    events.ScheduleEvent(EVENT_MOVE_TO_PLATFORM_EDGE, m_moveUpdatePeriod);
                    break;
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!me->isAlive() || me->HasUnitState(UNIT_STAT_CASTING))
                return;
            events.Update(diff);
            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_CHARGE_PLAYER:
                    {
                        if(Player* player = ObjectAccessor::GetPlayer(*me, m_victimGuid))
                            DoCast(player, SPELL_VALKYR_CHARGE, true);
                        //else
                        //    me->DespawnOrUnsummon();
                        break;
                    }
                    case EVENT_CHECK_AT_PLATFORM_EDGE:
                    {
                        if(me->GetDistance2d(MovePos[1].m_positionX, MovePos[1].m_positionY) > 55.0f)
                        {
                            events.Reset();
                            DoCast(me, SPELL_VALKYR_EJECT_PASSENGER);
                            //Fly 15 feet upward, then despawn
                            me->GetPosition(&m_pos);
                            m_pos.m_positionZ = 1055.0f;
                            me->GetMotionMaster()->MovePoint(POINT_VALKYR_END, m_pos);
                        }
                        else
                            events.ScheduleEvent(EVENT_CHECK_AT_PLATFORM_EDGE, 1000);
                    }
                    case EVENT_MOVE_TO_PLATFORM_EDGE:
                    {
                        me->GetPosition(&m_pos);
                        if(!me->HasAuraType(SPELL_AURA_MOD_STUN))
                        {
                            float flySpeed = me->GetSpeed(MOVE_RUN) * m_moveUpdatePeriod / 10000;
                            m_pos.m_positionX += flySpeed * cosf(m_angle);
                            m_pos.m_positionY += flySpeed * sinf(m_angle);
                            m_pos.m_positionZ = Z_FLY;
                        }
                        me->SetFacingTo(m_angle);
                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MovePoint(POINT_VALKYR_CONTINUE_FLYING, m_pos);
                        me->SetPosition(m_pos);
                        break;
                    }
                    case EVENT_SIPHON_LIFE:
                    {
                        DoCastVictim(SPELL_LIFE_SIPHON);
                        events.ScheduleEvent(EVENT_SIPHON_LIFE, 2500);
                    }
                }
            }
        }
    private:
        bool bCanCast;
        EventMap events;
        Position m_pos;
        float m_angle;
        uint32 m_moveUpdatePeriod;
        Vehicle* vehicle;
        uint64 m_victimGuid;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_valkyr_iccAI(pCreature);
    }
};

const float npc_valkyr_icc::Z_FLY = 1045.0f;

class npc_vile_spirit_icc : public CreatureScript
{
enum eEvents
{
    EVENT_BECOME_ACTIVE = 1,
    EVENT_DESPAWN,
    EVENT_MOVE_RANDOM,
    EVENT_CHECK_PLAYER_IN_FROSTMOURNE_ROOM
};

enum ePoints
{
    POINT_MOVE_NEAR_RANDOM = 1
};

public:
    static const float Z_VILE_SPIRIT;
    npc_vile_spirit_icc() : CreatureScript("npc_vile_spirit_icc") { }

    struct npc_vile_spirit_iccAI : public ScriptedAI
    {
        npc_vile_spirit_iccAI(Creature* pCreature): ScriptedAI(pCreature) { }

        void Reset()
        {
            events.ScheduleEvent(EVENT_BECOME_ACTIVE, 15000);
            //If they don't reach that player within around 30 seconds, they will despawn harmlessly.
            events.ScheduleEvent(EVENT_DESPAWN, 45000);
            SetCombatMovement(false);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlying(true);
            float x, y, z;
            me->GetPosition(x, y, z);
            me->SetPosition(x, y, npc_vile_spirit_icc::Z_VILE_SPIRIT, true);
            Position randomPos;
            float dist = float(rand_norm() * 10);
            me->GetRandomNearPosition(randomPos, dist);
            randomPos.m_positionZ = Z_VILE_SPIRIT;
            me->GetMotionMaster()->MovePoint(POINT_MOVE_NEAR_RANDOM, randomPos);
            m_bActive = false;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if(type != POINT_MOTION_TYPE)
                return;
            if(id == POINT_MOVE_NEAR_RANDOM)
            {
                me->GetMotionMaster()->MovementExpired();
                if(!m_bActive)
                    events.ScheduleEvent(EVENT_MOVE_RANDOM, 1000);
            }
        }

        void DoAction(const int32 action)
        {
            switch(action)
            {
                case ACTION_DESPAWN:
                {
                    me->RemoveAura(SPELL_VILE_SPIRIT_DISTANCE_CHECK);
                    events.ScheduleEvent(EVENT_DESPAWN, 1000);
                    me->SetReactState(REACT_PASSIVE);
                    SetCombatMovement(false);
                    break;
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);
            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_MOVE_RANDOM:
                    {
                        Position randomPos;
                        float dist = float(rand_norm() * 10);
                        me->GetRandomNearPosition(randomPos, dist);
                        randomPos.m_positionZ = Z_VILE_SPIRIT;
                        me->GetMotionMaster()->MovePoint(POINT_MOVE_NEAR_RANDOM, randomPos);
                        break;
                    }
                    case EVENT_BECOME_ACTIVE:
                    {
                        events.CancelEvent(EVENT_MOVE_RANDOM);
                        m_bActive = true;
                        SetCombatMovement(true);
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->GetMotionMaster()->MovementExpired();
                        DoCast(me, SPELL_VILE_SPIRIT_ACTIVE, true);
                        //DoCast(me, SPELL_VILE_SPIRIT_TARGET_SEARCH, true);
                        DoCast(me, SPELL_VILE_SPIRIT_DISTANCE_CHECK, true);
                        events.ScheduleEvent(EVENT_CHECK_PLAYER_IN_FROSTMOURNE_ROOM, 1000);
                        break;
                    }
                    case EVENT_CHECK_PLAYER_IN_FROSTMOURNE_ROOM:
                    {
                        Unit* curVictim = me->getVictim();
                        if(!curVictim)
                            curVictim = SelectRandomAttackablePlayerInTheMap(me->GetMap());
                        if(!curVictim)
                        {
                            me->DespawnOrUnsummon();
                            return;
                        }

                        events.ScheduleEvent(EVENT_CHECK_PLAYER_IN_FROSTMOURNE_ROOM, 2000);
                        break;
                    }
                    case EVENT_DESPAWN:
                    {
                        me->DespawnOrUnsummon();
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    private:
        EventMap events;
        bool m_bActive;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_vile_spirit_iccAI(pCreature);
    }
};

const float npc_vile_spirit_icc::Z_VILE_SPIRIT = 1050.0f;

class spell_lich_king_pain_and_suffering_effect : public SpellScriptLoader
{
    public:
        spell_lich_king_pain_and_suffering_effect() : SpellScriptLoader("spell_lich_king_pain_and_suffering_effect") { }

        class spell_lich_king_pain_and_suffering_effect_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lich_king_pain_and_suffering_effect_SpellScript);

            class AnyAlivePetOrPlayerInObjectFrontalConeCheck
            {
                public:
                    AnyAlivePetOrPlayerInObjectFrontalConeCheck(WorldObject const* obj) : i_obj(obj) { }
                    bool operator()(Unit* u)
                    {
                        if(u->GetTypeId() != TYPEID_PLAYER)
                            return true;
                        if(!u->isTargetableForAttack())
                            return true;
                        if(!u->isAlive())
                            return true;
                        Position myPos, uPos;
                        i_obj->GetPosition(&myPos);
                        u->GetPosition(&uPos);
                        float orientation = i_obj->GetOrientation();
                        float angle = myPos.GetAngle(&uPos);
                        float coneAngle = M_PI / 180 * 15.0f;
                        angle = MapManager::NormalizeOrientation(orientation - angle);
                        if(((0.0f <= angle) && (angle <= coneAngle / 2)) || (((2 * M_PI - coneAngle / 2) <= angle) && (angle <= (2 * M_PI))))
                            return false;
                        return true;
                    }
                private:
                    WorldObject const* i_obj;
            };

            void FilterTargets(std::list<Unit*>& unitList)
            {
                Unit* caster = GetCaster();
                if(!caster || !caster->isAlive())
                    return;
                unitList.remove_if(AnyAlivePetOrPlayerInObjectFrontalConeCheck(caster));
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_lich_king_pain_and_suffering_effect_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_TARGET_ENEMY);
                OnUnitTargetSelect += SpellUnitTargetFn(spell_lich_king_pain_and_suffering_effect_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_AREA_PATH);
                OnUnitTargetSelect += SpellUnitTargetFn(spell_lich_king_pain_and_suffering_effect_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_AREA_PATH);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lich_king_pain_and_suffering_effect_SpellScript();
        }
};

class spell_lich_king_necrotic_plague : public SpellScriptLoader
{
    public:
        spell_lich_king_necrotic_plague() : SpellScriptLoader("spell_lich_king_necrotic_plague") { } //70338

        class spell_lich_king_necrotic_plague_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_necrotic_plague_AuraScript)

            class AnyAliveCreatureOrPlayerInObjectRangeCheck
            {
                public:
                    AnyAliveCreatureOrPlayerInObjectRangeCheck(WorldObject const* obj, float range) : i_obj(obj), i_range(range) { }
                    bool operator()(Unit* u)
                    {
                        if(!u->isTargetableForAttack())
                            return false;
                        //Ignore the object itself
                        if(u->GetGUID() == i_obj->GetGUID())
                            return false;
                        // Check contains checks for: live, non-selectable, non-attackable flags, flight check and GM check, ignore totems, ignore pets
                        if(!(u->isAlive() && i_obj->IsWithinDistInMap(u, i_range)))
                            return false;
                        if(u->GetTypeId() == TYPEID_PLAYER)
                            return true;
                        //Ignore non-creatures (players were already checked a line above)
                        if(u->GetTypeId() != TYPEID_UNIT)
                            return false;
                        //Ignore totems

                        if(((Creature*)u)->isTotem())
                            return false;
                        //Ignore pets
                        if(u->GetOwner() && u->GetOwner()->GetTypeId() == TYPEID_PLAYER)
                            return false;
                        //Ignore Tirion, The Lich King, Raging Spirits, too
                        uint32 entry = u->ToCreature()->GetEntry();
                        if(entry == NPC_THE_LICH_KING || entry == NPC_TIRION_ICC || entry == NPC_RAGING_SPIRIT)
                            return false;
                        return true;
                    }
                private:
                    WorldObject const* i_obj;
                    float i_range;
            };

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                if(!target)
                    return;
                if(GetStackAmount() >= 30)
                    if(InstanceScript *_instance = target->GetInstanceScript())
                        _instance->SetData(DATA_BEEN_WAITING_ACHIEVEMENT, uint32(true));
                CellPair p(Trinity::ComputeCellPair(target->GetPositionX(), target->GetPositionY()));
                Cell cell(p);
                cell.data.Part.reserved = ALL_DISTRICT;
                cell.SetNoCreate();

                float dist = 10.0f;
                AnyAliveCreatureOrPlayerInObjectRangeCheck checker(target, dist);
                Unit* newTarget = NULL;
                Trinity::UnitLastSearcher<AnyAliveCreatureOrPlayerInObjectRangeCheck> searcher(target, newTarget, checker);

                TypeContainerVisitor<Trinity::UnitLastSearcher<AnyAliveCreatureOrPlayerInObjectRangeCheck>, WorldTypeMapContainer > world_unit_searcher(searcher);
                TypeContainerVisitor<Trinity::UnitLastSearcher<AnyAliveCreatureOrPlayerInObjectRangeCheck>, GridTypeMapContainer >  grid_unit_searcher(searcher);

                cell.Visit(p, world_unit_searcher, *target->GetMap(), *target, dist);
                cell.Visit(p, grid_unit_searcher, *target->GetMap(), *target, dist);

                uint32 stacksTransferred = GetStackAmount();
                //If target is still alive and it's a player, it means that this spell was dispelled - increase stack amount then
                //I don't know the way to find out whether it's dispelled or not
                // -- then read this: GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_ENEMY_SPELL
                if(!target->isAlive())
                    ++stacksTransferred;
                else
                {
                    //Player was a target and is still alive - assume that the spell was dispelled
                    if(target->GetTypeId() == TYPEID_PLAYER)
                        if(stacksTransferred > 1)
                            --stacksTransferred;
                }
                if(stacksTransferred < 1)
                    stacksTransferred = 1;
                uint32 spellId = aurEff->GetSpellProto()->Id;
                InstanceScript *_instance = target->GetInstanceScript();
                if(_instance)
                {
                    Unit* lichKing = ObjectAccessor::GetCreature(*target, _instance->GetData64(DATA_THE_LICH_KING));
                    if(lichKing)
                    {
                        if(newTarget)
                        {
                            Aura *appAura = newTarget->GetAura(spellId);
                            if(!appAura)
                            {
                                Unit* newCaster = lichKing;
                                newCaster->CastSpell(newTarget, spellId, true);
                                appAura = newTarget->GetAura(spellId);
                                --stacksTransferred; //One stack is already transferred
                            }
                            if(appAura)
                            {
                                appAura->SetStackAmount(appAura->GetStackAmount() + stacksTransferred);
                                appAura->RefreshDuration();
                            }
                        }
                        Aura *plagueSiphon = lichKing->GetAura(SPELL_PLAGUE_SIPHON);
                        if(!plagueSiphon)
                        {
                            lichKing->CastSpell(lichKing, SPELL_PLAGUE_SIPHON, true);
                            plagueSiphon = lichKing->GetAura(SPELL_PLAGUE_SIPHON);
                        }
                        if(plagueSiphon)
                        {
                            plagueSiphon->ModStackAmount(1);
                            plagueSiphon->RefreshDuration();
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_lich_king_necrotic_plague_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        private:
            uint8 m_stackAmount;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_necrotic_plague_AuraScript();
        }
};

class spell_lich_king_defile : public SpellScriptLoader
{
    public:
        spell_lich_king_defile() : SpellScriptLoader("spell_lich_king_defile") { }

        class spell_lich_king_defile_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_defile_AuraScript)

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                Unit* caster = GetCaster();
                if(!caster || !caster->isAlive() || !caster->GetAI())
                    return;
                Map *pMap = caster->GetMap();
                //Radius increases by 10% per hit on heroic and by 5% if it's normal
                m_radius = 8.0f + m_hitCount;
                //Find targest
                std::list<Unit* > targets;
                Trinity::AnyUnfriendlyUnitInObjectRangeCheck checker(caster, caster, m_radius);

                Trinity::UnitListSearcher<Trinity::AnyUnfriendlyUnitInObjectRangeCheck> searcher(caster, targets, checker);

                TypeContainerVisitor<Trinity::UnitListSearcher<Trinity::AnyUnfriendlyUnitInObjectRangeCheck>, WorldTypeMapContainer > world_unit_searcher(searcher);
                TypeContainerVisitor<Trinity::UnitListSearcher<Trinity::AnyUnfriendlyUnitInObjectRangeCheck>, GridTypeMapContainer >  grid_unit_searcher(searcher);

                CellPair p(Trinity::ComputeCellPair(caster->GetPositionX(), caster->GetPositionY()));
                Cell cell(p);
                cell.data.Part.reserved = ALL_DISTRICT;
                cell.SetNoCreate();

                cell.Visit(p, world_unit_searcher, *pMap, *caster, 100.0f);
                cell.Visit(p, grid_unit_searcher, *pMap, *caster, 100.0f);

                if(targets.empty())
                    return;
                uint32 triggeredSpellId = SPELL_DEFILE_DAMAGE;
                int32 triggeredSpellBaseDamage = 3000;
                if(SpellEntry const* defileDamage = sSpellMgr->GetSpellForDifficultyFromSpell(sSpellStore.LookupEntry(SPELL_DEFILE_DAMAGE), caster))
                {
                    triggeredSpellId = defileDamage->Id;
                    triggeredSpellBaseDamage = (int32)(defileDamage->EffectBasePoints[EFFECT_0] * (1.0f + (pMap->IsHeroic() ? 0.1f : 0.05f) * m_hitCount));
                }

                values.AddSpellMod(SPELLVALUE_BASE_POINT0, ((int32)(triggeredSpellBaseDamage)));
                values.AddSpellMod(SPELLVALUE_RADIUS_MOD, ((int32)(m_radius * 50)));
                //values.AddSpellMod(SPELLVALUE_MAX_TARGETS, 1);

                bool increaseRadius = false;
                uint64 ownerGuid = (caster->GetOwner() ? caster->GetOwner()->GetGUID() : 0);
                Unit* curVictim = NULL;
                for(std::list<Unit*>::iterator it = targets.begin(); it != targets.end(); ++it)
                {
                    curVictim = *it;
                    if(curVictim->GetGUID() == ownerGuid)
                        continue;
                    if(curVictim->GetTypeId() != TYPEID_PLAYER)
                        continue;
                    if(curVictim->GetDistance2d(caster) > m_radius)
                        continue;
                    caster->CastCustomSpell(triggeredSpellId, values, curVictim, true, NULL, NULL, GetCasterGUID());
                    increaseRadius = true;
                }
                if(!increaseRadius)
                    return;
                if(SpellEntry const* defileIncrease = sSpellMgr->GetSpellForDifficultyFromSpell(sSpellStore.LookupEntry(SPELL_DEFILE_INCREASE), caster))
                {
                    caster->CastSpell(caster, defileIncrease->Id, true);
                    if(Aura *defileIncreaseAura = caster->GetAura(defileIncrease->Id))
                        m_hitCount = defileIncreaseAura->GetStackAmount();
                    else
                        ++m_hitCount;
                }
            }

            void Register()
            {
                m_hitCount = 0;
                m_radius = 0.0f;
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_defile_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        private:
            uint32 m_hitCount;
            float m_radius;
            CustomSpellValues values;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_defile_AuraScript();
        }
};

class spell_lich_king_infection : public SpellScriptLoader
{
    public:
        spell_lich_king_infection() : SpellScriptLoader("spell_lich_king_infection") { } //70541

        class spell_lich_king_infection_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_infection_AuraScript)

            void HandleTick(AuraEffect const*aurEff)
            {
                if(!GetTarget()->isAlive() || GetTarget()->GetHealthPct() >= 90)
                {
                    //Aura::ApplicationMap &appMap = const_cast<Aura::ApplicationMap&>(aurEff->GetBase()->GetApplicationMap());
                    //Aura::ApplicationMap::iterator it = appMap.find(GetTarget()->GetGUID());
                    //if(it != appMap.end())
                    //    appMap.erase(it);
                    PreventDefaultAction();
                    GetTarget()->RemoveAurasDueToSpell(aurEff->GetSpellProto()->Id);
                }
            }

            void OnCalcAmount(AuraEffect const* aurEff, int32 & amount, bool & /*canBeRecalculated*/)
            {
                amount = int32(1000 * powf(1.15f, (float)aurEff->GetTickNumber()));
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_infection_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_lich_king_infection_AuraScript::OnCalcAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_infection_AuraScript();
        }
};

class spell_lich_king_valkyr_summon : public SpellScriptLoader
{
    public:
        spell_lich_king_valkyr_summon() : SpellScriptLoader("spell_lich_king_valkyr_summon") { } //74361

        class spell_lich_king_valkyr_summon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_valkyr_summon_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if(Unit* caster = GetCaster())
                {
                    uint32 spawnMod = caster->GetMap()->GetSpawnMode();
                    GetTargetApplication()->GetBase()->SetDuration(spawnMod == 1 || spawnMod == 3 ? 3000 : 1000);
                }
            }

            void HandleTriggerSpell(AuraEffect const* aurEff)
            {
                PreventDefaultAction();
                if(Unit* caster = GetCaster())
                {
                    Position randomPos;
                    caster->GetRandomNearPosition(randomPos, 10.0f);
                    randomPos.m_positionZ = caster->GetPositionZ() + 6.0f;
                    uint32 triggerSpellId = GetSpellProto()->EffectTriggerSpell[aurEff->GetEffIndex()];
                    caster->CastSpell(randomPos.GetPositionX(), randomPos.GetPositionY(), randomPos.GetPositionZ(), triggerSpellId, true, NULL, NULL, GetCasterGUID(), caster);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_valkyr_summon_AuraScript::HandleTriggerSpell, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                OnEffectApply += AuraEffectApplyFn(spell_lich_king_valkyr_summon_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_valkyr_summon_AuraScript();
        }
};

class spell_lich_king_vile_spirit_summon : public SpellScriptLoader
{
    public:
        spell_lich_king_vile_spirit_summon() : SpellScriptLoader("spell_lich_king_vile_spirit_summon") { }

        class spell_lich_king_vile_spirit_summon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_vile_spirit_summon_AuraScript);

            void OnPeriodic(AuraEffect const*aurEff)
            {
                PreventDefaultAction();
                Unit* caster = GetCaster();
                if(!caster || !caster->isAlive())
                    return;
                InstanceScript *_instance = caster->GetInstanceScript();
                if(_instance)
                {
                    uint32 spawnMod = caster->GetMap()->GetSpawnMode();
                    uint32 maxSummoned;
                    if(spawnMod == 1 || spawnMod == 3)
                        maxSummoned = 10;
                    else
                        maxSummoned = 8;
                    if(aurEff->GetTickNumber() >= maxSummoned)
                        return;
                }
                Position pos;
                caster->GetRandomNearPosition(pos, 13.0f);
                pos.m_positionZ = npc_vile_spirit_icc::Z_VILE_SPIRIT;
                uint32 triggeredSpell = aurEff->GetSpellProto()->EffectTriggerSpell[aurEff->GetEffIndex()];
                caster->CastSpell(pos.m_positionX, pos.m_positionY, pos.m_positionZ, triggeredSpell, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_vile_spirit_summon_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_vile_spirit_summon_AuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_LINKED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_vile_spirit_summon_AuraScript();
        }
};

class spell_lich_king_vile_spirit_summon_visual : public SpellScriptLoader
{
    public:
        spell_lich_king_vile_spirit_summon_visual() : SpellScriptLoader("spell_lich_king_vile_spirit_summon_visual") { }

        class spell_lich_king_vile_spirit_summon_visual_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_vile_spirit_summon_visual_AuraScript);

            void OnPeriodic(AuraEffect const*aurEff)
            {
                PreventDefaultAction();
                Unit* caster = GetCaster();
                if(!caster || !caster->isAlive())
                    return;
                Position pos;
                caster->GetRandomNearPosition(pos, 13.0f);
                pos.m_positionZ = npc_vile_spirit_icc::Z_VILE_SPIRIT;
                uint32 triggeredSpell = aurEff->GetSpellProto()->EffectTriggerSpell[aurEff->GetEffIndex()];
                caster->CastSpell(pos.m_positionX, pos.m_positionY, pos.m_positionZ, triggeredSpell, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_vile_spirit_summon_visual_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_vile_spirit_summon_visual_AuraScript();
        }
};

class spell_lich_king_winter : public SpellScriptLoader
{
    public:
        spell_lich_king_winter() : SpellScriptLoader("spell_lich_king_winter") { } //68981

        class spell_lich_king_winter_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_winter_AuraScript)

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if(Unit* caster = GetCaster())
                {
                    if(UnitAI *pBossAI = caster->GetAI())
                    {
                        pBossAI->DoAction(ACTION_CANCEL_ALL_TRANSITION_EVENTS);
                        pBossAI->DoAction(ACTION_PHASE_SWITCH_2);
                    }

                    caster->CastSpell(caster, SPELL_QUAKE, true);
                    DoScriptText(SAY_BROKEN_ARENA, caster);
                }
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                if(!caster || !caster->isAlive())
                    return;
                InstanceScript *_instance = caster->GetInstanceScript();
                if(!_instance)
                    return;
                //Rebuild platform's edge only in second transition phase
                if(UnitAI *pAI = caster->GetAI())
                    if(pAI->GetData(DATA_PHASE) == PHASE_4_TRANSITION)
                    {
                        //Two spells should be casted in this very sequence to get working animation
                        caster->CastSpell(caster, SPELL_WMO_INTACT, true);
                        caster->CastSpell(caster, SPELL_WMO_REBUILD, true);
                    }
                //Destroying ice shards
                if(GameObject* go = ObjectAccessor::GetGameObject(*caster, _instance->GetData64(GUID_ICE_SHARD_1)))
                    go->SetGoState(GO_STATE_ACTIVE);
                if(GameObject* go = ObjectAccessor::GetGameObject(*caster, _instance->GetData64(GUID_ICE_SHARD_2)))
                    go->SetGoState(GO_STATE_ACTIVE);
                if(GameObject* go = ObjectAccessor::GetGameObject(*caster, _instance->GetData64(GUID_ICE_SHARD_3)))
                    go->SetGoState(GO_STATE_ACTIVE);
                if(GameObject* go = ObjectAccessor::GetGameObject(*caster, _instance->GetData64(GUID_ICE_SHARD_4)))
                    go->SetGoState(GO_STATE_ACTIVE);
                //Hiding platform destroy warning - it's bugged and is enabled somehow, need to disable it preventively
                if(GameObject* go = ObjectAccessor::GetGameObject(*caster, _instance->GetData64(GUID_EDGE_DESTROY_WARNING)))
                    go->SetGoState(GO_STATE_READY);
                //Hiding inner waterfall
                if(GameObject* go = ObjectAccessor::GetGameObject(*caster, _instance->GetData64(GUID_FROSTY_EDGE_INNER)))
                    go->SetGoState(GO_STATE_READY);
                //Showing outer waterfall
                if(GameObject* go = ObjectAccessor::GetGameObject(*caster, _instance->GetData64(GUID_FROSTY_EDGE_OUTER)))
                    go->SetGoState(GO_STATE_ACTIVE);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_lich_king_winter_AuraScript::OnApply, EFFECT_1, SPELL_AURA_MOD_ROOT, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_lich_king_winter_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_winter_AuraScript();
        }
};

class spell_lich_king_quake : public SpellScriptLoader
{
    public:
        spell_lich_king_quake() : SpellScriptLoader("spell_lich_king_quake") { } //72262

        class spell_lich_king_quake_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_quake_AuraScript)

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                if(!caster)
                    return;

                //Two spells should be casted in this very sequence
                caster->CastSpell(caster, SPELL_WMO_INTACT, true);
                caster->CastSpell(caster, SPELL_WMO_DESTROY, true);
                if(InstanceScript* _instance = GetTarget()->GetInstanceScript())
                {
                    //Hiding platform destroy warning
                    if(GameObject* go = ObjectAccessor::GetGameObject(*caster, _instance->GetData64(GUID_EDGE_DESTROY_WARNING)))
                        go->SetGoState(GO_STATE_READY);
                    //Showing inner waterfall
                    if(GameObject* go = ObjectAccessor::GetGameObject(*caster, _instance->GetData64(GUID_FROSTY_EDGE_INNER)))
                        go->SetGoState(GO_STATE_ACTIVE);
                    //Hiding outer waterfall
                    if(GameObject* go = ObjectAccessor::GetGameObject(*caster, _instance->GetData64(GUID_FROSTY_EDGE_OUTER)))
                        go->SetGoState(GO_STATE_READY);
                }
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                if(!caster || !caster->isAlive())
                    return;

                InstanceScript *_instance = caster->GetInstanceScript();
                if(!_instance)
                    return;
                //Showing visual warning
                if(GameObject* go = ObjectAccessor::GetGameObject(*caster, _instance->GetData64(GUID_EDGE_DESTROY_WARNING)))
                    go->SetGoState(GO_STATE_ACTIVE);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_lich_king_quake_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_lich_king_quake_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_quake_AuraScript();
        }
};

class spell_vile_spirit_distance_check : public SpellScriptLoader
{
    public:
        spell_vile_spirit_distance_check() : SpellScriptLoader("spell_vile_spirit_distance_check") { }

        class spell_vile_spirit_distance_check_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_vile_spirit_distance_check_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if(!(GetHitUnit() && GetHitUnit()->isAlive()))
                    return;

                if(Unit* caster = GetCaster())
                {
                    caster->CastSpell(caster, SPELL_SPIRIT_BURST, true);

                    if(InstanceScript* _instance = caster->GetInstanceScript())
                        _instance->SetData(DATA_NECK_DEEP_ACHIEVEMENT, FAIL);
                    caster->GetAI()->DoAction(ACTION_DESPAWN);
                }
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_vile_spirit_distance_check_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_vile_spirit_distance_check_SpellScript();
        }
};

class spell_ice_burst_distance_check : public SpellScriptLoader
{
    public:
        spell_ice_burst_distance_check() : SpellScriptLoader("spell_ice_burst_distance_check") { }

        class spell_ice_burst_distance_check_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ice_burst_distance_check_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if(!(GetHitUnit() && GetHitUnit()->isAlive()))
                    return;

                if(GetHitUnit()->GetTypeId() != TYPEID_PLAYER)
                    return;

                if(Unit* caster = GetCaster())
                    caster->CastSpell(caster, SPELL_ICE_BURST, true);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_ice_burst_distance_check_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ice_burst_distance_check_SpellScript();
        }
};

class spell_lich_king_play_movie : public SpellScriptLoader
{
    public:
        spell_lich_king_play_movie() : SpellScriptLoader("spell_lich_king_play_movie") { }

        class spell_lich_king_play_movie_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lich_king_play_movie_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* target = GetHitUnit();
                if(!(target && target->isAlive()))
                    return;

                if(target->GetTypeId() == TYPEID_PLAYER)
                    target->ToPlayer()->SendMovieStart(MOVIE_ID_ARTHAS_DEATH);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_lich_king_play_movie_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lich_king_play_movie_SpellScript();
        }
};

class spell_valkyr_carry_can_cast : public SpellScriptLoader
{
    public:
        spell_valkyr_carry_can_cast() : SpellScriptLoader("spell_valkyr_carry_can_cast") { }

        class spell_valkyr_carry_can_cast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_valkyr_carry_can_cast_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster(), *target = GetHitUnit();
                if(!(target && target->isAlive() && caster))
                    return;

                if(target->GetTypeId() == TYPEID_PLAYER)
                {
                    target->CastSpell(caster, SPELL_VALKYR_GRAB_PLAYER, true);
                }
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_valkyr_carry_can_cast_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_valkyr_carry_can_cast_SpellScript();
        }
};

class spell_valkyr_target_search : public SpellScriptLoader
{
    public:
        spell_valkyr_target_search() : SpellScriptLoader("spell_valkyr_target_search") { }

        class spell_valkyr_target_search_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_valkyr_target_search_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* target = GetHitUnit(), *caster = GetCaster();
                if(!(target && target->isAlive() && caster))
                    return;

                if(target->GetTypeId() == TYPEID_PLAYER)
                {
                    if(UnitAI *pAI = caster->GetAI())
                    {
                        pAI->SetGUID(target->GetGUID(), TYPE_VICTIM);
                        pAI->DoAction(ACTION_CHARGE_PLAYER);
                    }
                }
            }

            void FilterTargets(std::list<Unit*>& unitList)
            {
                for(std::list<Unit*>::iterator itr = unitList.begin(); itr != unitList.end();)
                {
                    if((*itr)->GetTypeId() != TYPEID_PLAYER)
                        unitList.erase(itr++);
                    else
                        ++itr;
                }

                std::list<Unit*>::iterator itr = unitList.begin();
                std::advance(itr, urand(0, unitList.size()-1));
                Unit* target = *itr;
                unitList.clear();
                unitList.push_back(target);
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_valkyr_target_search_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_AREA_ENEMY_SRC);
                OnEffect += SpellEffectFn(spell_valkyr_target_search_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_valkyr_target_search_SpellScript();
        }
};

class spell_valkyr_eject_passenger : public SpellScriptLoader
{
    public:
        spell_valkyr_eject_passenger() : SpellScriptLoader("spell_valkyr_eject_passenger") { }

        class spell_valkyr_eject_passenger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_valkyr_eject_passenger_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if(!(GetCaster() && GetCaster()->IsVehicle()))
                    return;

                if(Vehicle* vehicle = GetCaster()->GetVehicleKit())
                    vehicle->RemoveAllPassengers();
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_valkyr_eject_passenger_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_valkyr_eject_passenger_SpellScript();
        }
};

class spell_vile_spirit_target_search : public SpellScriptLoader
{
    public:
        spell_vile_spirit_target_search() : SpellScriptLoader("spell_vile_spirit_target_search") { }

        class spell_vile_spirit_target_search_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_vile_spirit_target_search_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if(!(GetHitUnit() && GetHitUnit()->isAlive() && GetCaster()))
                    return;

                if(GetHitUnit()->GetTypeId() == TYPEID_PLAYER)
                    GetCaster()->AddThreat(GetHitUnit(), 1000.0f);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_vile_spirit_target_search_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_vile_spirit_target_search_SpellScript();
        }
};

class npc_shambling_horror_icc: public CreatureScript
{
    enum eEvents
    {
        EVENT_ENRAGE = 1,
        EVENT_SHOCKWAVE
    };
    enum eSpells
    {
        SPELL_SHOCKWAVE                  = 72149,
        SPELL_ENRAGE                     = 72143,
        SPELL_FRENZY                     = 28747
    };
public:
    npc_shambling_horror_icc(): CreatureScript("npc_shambling_horror_icc") { }
    struct npc_shambling_horror_iccAI: public ScriptedAI
    {
        npc_shambling_horror_iccAI(Creature* pCreature): ScriptedAI(pCreature), _instance(pCreature->GetInstanceScript()) { }

        void EnterCombat(Unit* /*pWho*/)
        {
            events.Reset();
            events.ScheduleEvent(EVENT_ENRAGE, 15000);
            events.ScheduleEvent(EVENT_SHOCKWAVE, 10000);
            isFrenzied = false;
        }

        void DamageTaken(Unit* /*done_by*/, uint32& /*damage*/)
        {
            if(IsHeroic())
                if(HealthBelowPct(20) && !isFrenzied)
                {
                    isFrenzied = true;
                    DoCast(me, SPELL_FRENZY);
                }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;
            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;
            events.Update(diff);
            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_ENRAGE:
                    {
                        DoCast(me, SPELL_ENRAGE);
                        events.ScheduleEvent(EVENT_ENRAGE, urand(20000, 30000));
                        break;
                    }
                    case EVENT_SHOCKWAVE:
                    {
                        DoCast(me->getVictim(), SPELL_SHOCKWAVE);
                        events.ScheduleEvent(EVENT_SHOCKWAVE, 20000);
                        break;
                    }
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    private:
        bool isFrenzied;
        InstanceScript *_instance;
        EventMap events;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_shambling_horror_iccAI(pCreature);
    }
};

class npc_raging_spirit_icc: public CreatureScript
{
    enum eEvents
    {
        EVENT_SOUL_SHRIEK   = 1
    };

    enum eSpells
    {
        SPELL_SOUL_SHRIEK   = 69242
    };
public:
    npc_raging_spirit_icc(): CreatureScript("npc_raging_spirit_icc") { }
    struct npc_raging_spirit_iccAI: public ScriptedAI
    {
        npc_raging_spirit_iccAI(Creature* pCreature): ScriptedAI(pCreature) { }

        void EnterCombat(Unit* /*pWho*/)
        {
            events.Reset();
            events.ScheduleEvent(EVENT_SOUL_SHRIEK, 15000);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;
            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;
            events.Update(diff);
            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_SOUL_SHRIEK:
                    {
                        DoCast(me->getVictim(), SPELL_SOUL_SHRIEK);
                        events.ScheduleEvent(EVENT_SOUL_SHRIEK, 10000);
                        break;
                    }
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_raging_spirit_iccAI(pCreature);
    }
};

class spell_lich_king_tirion_mass_resurrection : public SpellScriptLoader
{
public:
    spell_lich_king_tirion_mass_resurrection() : SpellScriptLoader("spell_lich_king_tirion_mass_resurrection") { }

    class spell_lich_king_tirion_mass_resurrection_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_lich_king_tirion_mass_resurrection_SpellScript)

        void FilterTargets(std::list<Unit*>& unitList)
        {
            unitList.clear();
            const Map::PlayerList &PlayerList = GetCaster()->GetMap()->GetPlayers();

            if(!PlayerList.isEmpty())
                for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    if(Player* player = i->getSource())
                        unitList.push_back(player);
        }

        void Register()
        {
            OnUnitTargetSelect += SpellUnitTargetFn(spell_lich_king_tirion_mass_resurrection_SpellScript::FilterTargets, EFFECT_0, TARGET_SRC_CASTER);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_lich_king_tirion_mass_resurrection_SpellScript();
    }
};

class npc_ice_sphere_icc : public CreatureScript
{
    enum eEvents
    {
        EVENT_MOVE_FORWARD = 1,
        EVENT_ACTIVATE
    };
public:
    npc_ice_sphere_icc() : CreatureScript("npc_ice_sphere_icc") { }

    struct npc_ice_sphere_iccAI : public ScriptedAI
    {
        npc_ice_sphere_iccAI(Creature* pCreature): ScriptedAI(pCreature), m_victimGuid(0) { }

        void Reset()
        {
            events.ScheduleEvent(EVENT_MOVE_FORWARD, 2000);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->ApplySpellImmune(0, IMMUNITY_ID, RAID_MODE(SPELL_REMORSELESS_WINTER_DAMAGE_10_N, SPELL_REMORSELESS_WINTER_DAMAGE_25_N, SPELL_REMORSELESS_WINTER_DAMAGE_10_H, SPELL_REMORSELESS_WINTER_DAMAGE_25_H), true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_DEATH_GRIP, true);
            events.ScheduleEvent(EVENT_ACTIVATE, 2500);
        }

        void SetGUID(const uint64& guid, int32 type)
        {
            if(type == TYPE_VICTIM)
                m_victimGuid = guid;
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(Unit* victim = ObjectAccessor::GetUnit(*me, m_victimGuid))
                victim->RemoveAurasDueToSpell(SPELL_ICE_PULSE, me->GetGUID());
            me->DespawnOrUnsummon();
        }

        void KilledUnit(Unit* victim)
        {
            victim->RemoveAurasDueToSpell(SPELL_ICE_PULSE, me->GetGUID());
            me->DespawnOrUnsummon();
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);
            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_ACTIVATE:
                    {
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        me->CastSpell(me, SPELL_ICE_SPHERE_VISUAL, true);
                        me->SetDisplayId(30243); //Make sphere visible
                        me->CastSpell(me, SPELL_ICE_BURST_DISTANCE_CHECK, true);
                        me->SetReactState(REACT_AGGRESSIVE);
                        Unit* victim = ObjectAccessor::GetUnit(*me, m_victimGuid);
                        if(!victim)
                            victim = SelectRandomAttackablePlayerInTheMap(me->GetMap());
                        if(victim)
                        {
                            m_victimGuid = victim->GetGUID();
                            me->GetMotionMaster()->MoveChase(victim);
		                    AttackStart(victim);
                            //AttackStart(victim);
                            DoCast(victim, SPELL_ICE_PULSE, true);
                        }
                        else
                            me->DespawnOrUnsummon(); //There are no players to chase - just despawn the sphere
                        break;
                    }
                    case EVENT_MOVE_FORWARD:
                    {
                        Unit* victim = ObjectAccessor::GetUnit(*me, m_victimGuid);
                        if(!victim)
                            victim = SelectRandomAttackablePlayerInTheMap(me->GetMap());
                        if(victim && victim->isAlive() && victim->isTargetableForAttack())
                        {
                            me->SetFacingToObject(victim);
                            victim->GetPosition(&m_victimPos);
                            me->GetPosition(&m_newPos);
                            me->MovePosition(m_newPos, 0.20f, 0.0f);
                            me->SetPosition(m_newPos);
                        }
                        else
                            me->DespawnOrUnsummon();

                        events.ScheduleEvent(EVENT_MOVE_FORWARD, 100);
                    }
                    default:
                        break;
                }
            }
        }
    private:
        EventMap events;
        Position m_victimPos, m_newPos;
        uint64 m_victimGuid;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_ice_sphere_iccAI(pCreature);
    }
};

class npc_defile_icc : public CreatureScript
{
public:
    npc_defile_icc() : CreatureScript("npc_defile_icc") { }

    struct npc_defile_iccAI : public Scripted_NoMovementAI
    {
        npc_defile_iccAI(Creature* pCreature): Scripted_NoMovementAI(pCreature), alreadyReset(false), m_hitNumber(0) { }

        void JustRespawned()
        {
            me->SetInCombatWithZone();
            me->SetReactState(REACT_PASSIVE);
        }

        void Reset()
        {
            if(!alreadyReset)
            {
                if(SpellEntry const* defileAuraSpellEntry = sSpellMgr->GetSpellForDifficultyFromSpell(sSpellStore.LookupEntry(SPELL_DEFILE), me))
                    DoCast(me, defileAuraSpellEntry->Id, true);
                //UpdateDefileAura();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                alreadyReset = true;
            }
        }

        void UpdateAI(const uint32 /*diff*/) { }

    private:
        bool alreadyReset;
        int32 m_hitNumber, m_radiusMod;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_defile_iccAI(pCreature);
    }
};

class spell_lich_king_harvest_soul : public SpellScriptLoader
{
public:
    spell_lich_king_harvest_soul() : SpellScriptLoader("spell_lich_king_harvest_soul") { }

    class spell_lich_king_harvest_soul_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_lich_king_harvest_soul_AuraScript)

        class TeleportToFrostmourneRoom : public BasicEvent
        {
            public:
                TeleportToFrostmourneRoom(Player* player, uint8 attempts): pPlayer(player), attemptsLeft(attempts) { }

                bool Execute(uint64 /*eventTime*/, uint32 /*updateTime*/)
                {
                    pPlayer->NearTeleportTo(FrostmourneRoom[0].m_positionX, FrostmourneRoom[0].m_positionY, FrostmourneRoom[0].m_positionZ, FrostmourneRoom[0].m_orientation);
                    pPlayer->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH);
                    if(--attemptsLeft)
                        pPlayer->m_Events.AddEvent(new TeleportToFrostmourneRoom(pPlayer, attemptsLeft), pPlayer->m_Events.CalculateTime(uint64(1000)));
                    else
                        pPlayer->CastSpell(pPlayer, SPELL_FROSTMOURNE_ROOM_TELEPORT_VISUAL, true);
                    pPlayer->CastSpell(pPlayer, SPELL_IN_FROSTMOURNE_ROOM, true);
                    return true;
                }

            private:
                Player* pPlayer;
                uint8 attemptsLeft;
        };

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget(), *caster = GetCaster();
            if(target->GetTypeId() != TYPEID_PLAYER || !caster)
                return;
            Player* player = target->ToPlayer();
            bool isHeroic = caster->GetMap()->IsHeroic();
            if(!player || !player->isAlive())
            {
                //Player died - buff up boss with +200% (+500% on heroic mode)
                caster->CastSpell(caster, isHeroic ? SPELL_HARVESTED_SOUL_HEROIC : SPELL_HARVESTED_SOUL_NORMAL, true);
            }
            player->CastSpell(player, SPELL_HARVESTED_SOUL_FROSTMOURNE_PLAYER_BUFF, true);
            if(isHeroic)
                player->CastSpell(player, SPELL_HARVEST_SOUL_HEROIC_FROSTMOURNE_PLAYER_DEBUFF, true);

            //Should use Feign death to emulate player's death
            player->CastSpell(player, SPELL_FEIGN_DEATH, true);

            player->getThreatManager().clearReferences();
            player->GetMap()->LoadGrid(FrostmourneRoom[0].m_positionX, FrostmourneRoom[0].m_positionY);
            player->m_Events.AddEvent(new TeleportToFrostmourneRoom(player, 2), player->m_Events.CalculateTime(uint64(2000)));
            InstanceScript *_instance = player->GetInstanceScript();
            if(_instance)
            {
                if(Creature* lichKing = ObjectAccessor::GetCreature(*caster, _instance->GetData64(DATA_THE_LICH_KING)))
                    lichKing->AI()->DoAction(ACTION_PREPARE_FROSTMOURNE_ROOM);
            }
        }

        void Register()
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_lich_king_harvest_soul_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_lich_king_harvest_soul_AuraScript();
    }
};

class npc_spirit_warden_icc : public CreatureScript
{
enum eEvents
{
    EVENT_SOUL_RIP = 1,
    EVENT_DESTROY_SOUL,
    EVENT_CHECK_SOUL_RIP_DISPELLED
};

public:
    npc_spirit_warden_icc() : CreatureScript("npc_spirit_warden_icc") { }

    struct npc_spirit_warden_iccAI : public ScriptedAI
    {
        npc_spirit_warden_iccAI(Creature* pCreature): ScriptedAI(pCreature), _instance(pCreature->GetInstanceScript()) { }

        void Reset()
        {
            me->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CHARM, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_SILENCE, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_TRANSFORM, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_SCALE, true);
            DoCast(me, SPELL_DARK_HUNGER, true);
            me->SetReactState(REACT_PASSIVE);
        }

        void DamageDealt(Unit* /*pTarget*/, uint32& damage, DamageEffectType /*damageType*/)
        {
            me->CastCustomSpell(SPELL_DARK_HUNGER_HEAL_EFFECT, SPELLVALUE_BASE_POINT0, damage, me, true, NULL, NULL, me->GetGUID());
        }

        void JustDied(Unit* /*pKiller*/)
        {
            //Teleport all players who are inside Frostmourne back to Frozen Throne platform
            TPlayerList players = GetPlayersInTheMap(me->GetMap());
            for(TPlayerList::iterator it = players.begin(); it != players.end(); ++it)
            {
                if((*it)->HasAura(SPELL_IN_FROSTMOURNE_ROOM))
                {
                    if(Creature* terenasFighter = ObjectAccessor::GetCreature(*me, _instance->GetData64(GUID_TERENAS_FIGHTER)))
                        terenasFighter->CastSpell((*it), SPELL_RESTORE_SOUL, true);
                    (*it)->RemoveAurasDueToSpell(SPELL_IN_FROSTMOURNE_ROOM);
                    TeleportPlayerToFrozenThrone(*it);
                }
            }
            events.Reset();
        }

        void DoAction(const int32 action)
        {
            switch(action)
            {
                case ACTION_DESPAWN:
                {
                    me->DespawnOrUnsummon();
                    break;
                }
                case ACTION_ATTACK_TERENAS_FIGHTER:
                {
                    events.Reset();
                    me->NearTeleportTo(FrostmourneRoom[1].m_positionX, FrostmourneRoom[1].m_positionY, FrostmourneRoom[1].m_positionZ, FrostmourneRoom[1].m_orientation);
                    if(Creature* terenasFighter = ObjectAccessor::GetCreature(*me, _instance->GetData64(GUID_TERENAS_FIGHTER)))
                        AttackStart(terenasFighter);
                    me->SetHealth(me->GetMaxHealth());
                    events.ScheduleEvent(EVENT_SOUL_RIP, 5000);
                    events.ScheduleEvent(EVENT_DESTROY_SOUL, 60000);
                    events.ScheduleEvent(EVENT_CHECK_SOUL_RIP_DISPELLED, 1000);
                    break;
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!me->isAlive() || me->HasUnitState(UNIT_STAT_CASTING))
                return;

            events.Update(diff);
            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_CHECK_SOUL_RIP_DISPELLED:
                    {
                        if(Creature* terenasFighter = ObjectAccessor::GetCreature(*me, _instance->GetData64(GUID_TERENAS_FIGHTER)))
                            if(!terenasFighter->HasAura(SPELL_SOUL_RIP, me->GetGUID()))
                            {
                                me->InterruptNonMeleeSpells(false);
                                me->InterruptSpell(CURRENT_CHANNELED_SPELL, false);
                            }
                        events.ScheduleEvent(EVENT_CHECK_SOUL_RIP_DISPELLED, 1000);
                        break;
                    }
                    case EVENT_SOUL_RIP:
                    {
                        if(Creature* terenasFighter = ObjectAccessor::GetCreature(*me, _instance->GetData64(GUID_TERENAS_FIGHTER)))
                            DoCast(terenasFighter, SPELL_SOUL_RIP, true);
                        events.ScheduleEvent(EVENT_SOUL_RIP, 20000);
                        break;
                    }
                    case EVENT_DESTROY_SOUL:
                    {
                        //Player failed to help Terenas to defeat Spirit Warden within 60 seconds - kill Player forcibly
                        if(Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_THE_LICH_KING)))
                            DoCast(lichKing, IsHeroic() ? SPELL_HARVESTED_SOUL_HEROIC : SPELL_HARVESTED_SOUL_NORMAL, true);

                        //Teleport all players who are inside Frostmourne back to Frozen Throne platform
                        TPlayerList players = GetPlayersInTheMap(me->GetMap());
                        for(TPlayerList::iterator it = players.begin(); it != players.end(); ++it)
                        {
                            if((*it)->HasAura(SPELL_IN_FROSTMOURNE_ROOM))
                            {
                                (*it)->CastSpell((*it), SPELL_DESTROY_SOUL, true);
                                (*it)->RemoveAurasDueToSpell(SPELL_IN_FROSTMOURNE_ROOM);
                                TeleportPlayerToFrozenThrone(*it);
                            }
                        }
                        events.Reset();
                        break;
                    }
                }
            }
            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
        InstanceScript* _instance;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_spirit_warden_iccAI(pCreature);
    }
};

class npc_terenas_fighter_icc : public CreatureScript
{
enum eEvents
{
    EVENT_GREET_PLAYER = 1, //Yell "You have come to bring Arthas to justice? To see the Lich King destroyed?"
    EVENT_ENCOURAGE_PLAYER_TO_ESCAPE, //Yell "First, you must escape Frostmourne's hold or be damned as I am; trapped within this cursed blade for all eternity."
    EVENT_ASK_PLAYER_FOR_AID, //Yell "Aid me in destroying these tortured souls! Together we will loosen Frostmourne's hold and weaken the Lich King from within!"
    EVENT_CHECK_SPIRIT_WARDEN_HEALTH
};

public:
    npc_terenas_fighter_icc() : CreatureScript("npc_terenas_fighter_icc") { }

    struct npc_terenas_fighter_iccAI : public ScriptedAI
    {
        npc_terenas_fighter_iccAI(Creature* pCreature): ScriptedAI(pCreature), _instance(pCreature->GetInstanceScript()) { }

        void Reset()
        {
            DoCast(me, SPELL_LIGHT_S_FAVOR, true);
            me->SetReactState(REACT_PASSIVE);
            me->SetHealth(me->GetMaxHealth() / 2);
        }

        void DoAction(const int32 action)
        {
            switch(action)
            {
                case ACTION_DESPAWN:
                {
                    me->DespawnOrUnsummon();
                    break;
                }
                case ACTION_ATTACK_SPIRIT_WARDEN:
                {
                    me->NearTeleportTo(FrostmourneRoom[2].m_positionX, FrostmourneRoom[2].m_positionY, FrostmourneRoom[2].m_positionZ, FrostmourneRoom[2].m_orientation);
                    if(Creature* spiritWarden = ObjectAccessor::GetCreature(*me, _instance->GetData64(GUID_SPIRIT_WARDEN)))
                        AttackStart(spiritWarden);
                    me->SetHealth(me->GetMaxHealth() / 2);
                    events.ScheduleEvent(EVENT_GREET_PLAYER, 1000);
                    break;
                }
            }
        }

        void DamageDealt(Unit* /*pTarget*/, uint32& damage, DamageEffectType /*damageType*/)
        {
            //Damage scales with Terenas' health
            damage = damage * int32((100 + me->GetHealthPct()) / 100);
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_THE_LICH_KING)))
                DoCast(lichKing, IsHeroic() ? SPELL_HARVESTED_SOUL_HEROIC : SPELL_HARVESTED_SOUL_NORMAL, true);
            //Teleport all players who are inside Frostmourne back to Frozen Throne platform
            TPlayerList players = GetPlayersInTheMap(me->GetMap());
            for(TPlayerList::iterator it = players.begin(); it != players.end(); ++it)
            {
                if((*it)->HasAura(SPELL_IN_FROSTMOURNE_ROOM))
                {
                    (*it)->CastSpell((*it), SPELL_DESTROY_SOUL, true);
                    (*it)->RemoveAurasDueToSpell(SPELL_IN_FROSTMOURNE_ROOM);
                    TeleportPlayerToFrozenThrone(*it);
                }
            }
            events.Reset();
        }

        void UpdateAI(const uint32 diff)
        {
            if(!me->isAlive() || me->HasUnitState(UNIT_STAT_CASTING))
                return;
            events.Update(diff);
            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_GREET_PLAYER:
                    {
                        DoScriptText(SAY_GREET_PLAYER, me);
                        events.ScheduleEvent(EVENT_ENCOURAGE_PLAYER_TO_ESCAPE, 10000);
                        break;
                    }
                    case EVENT_ENCOURAGE_PLAYER_TO_ESCAPE:
                    {
                        DoScriptText(SAY_PLAYER_TO_ESCAPE, me);
                        events.ScheduleEvent(EVENT_ASK_PLAYER_FOR_AID, 10000);
                        break;
                    }
                    case EVENT_ASK_PLAYER_FOR_AID:
                    {
                        DoScriptText(SAY_ASK_PLAYER_FOR_AID, me);
                        break;
                    }
                    case EVENT_CHECK_SPIRIT_WARDEN_HEALTH:
                    {
                        if(Creature* spiritWarden = ObjectAccessor::GetCreature(*me, _instance->GetData64(GUID_SPIRIT_WARDEN)))
                        {
                            if(!spiritWarden->isAlive())
                                KilledUnit(spiritWarden);
                        }
                        break;
                    }
                }
            }
            DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
        InstanceScript* _instance;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_terenas_fighter_iccAI(pCreature);
    }
};

class npc_shadow_trap : public CreatureScript
{
    enum eEvents
    {
        EVENT_BECOME_ACTIVE = 1,
        EVENT_ACTIVE,
        EVENT_CHECK,
        EVENT_DESPAWN
    };

public:
    npc_shadow_trap() : CreatureScript("npc_shadow_trap") { }

    struct npc_shadow_trapAI : public Scripted_NoMovementAI
    {
        npc_shadow_trapAI(Creature* pCreature): Scripted_NoMovementAI(pCreature) { }

        void Reset()
        {
            events.ScheduleEvent(EVENT_BECOME_ACTIVE, 500);
            events.ScheduleEvent(EVENT_DESPAWN, 60000);
            SetCombatMovement(false);
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);
            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_BECOME_ACTIVE:
                    {
                        DoCast(me, SPELL_SHADOW_TRAP_INTRO, true);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        me->SetReactState(REACT_AGGRESSIVE);
                        events.ScheduleEvent(EVENT_ACTIVE, 1500);
                        break;
                    }
                    case EVENT_ACTIVE:
                    {
                        me->RemoveAura(SPELL_SHADOW_TRAP_INTRO);
                        DoCast(me, SPELL_SHADOW_TRAP_PERIODIC, true);
                        //If they don't reach that player within around 45 seconds, they will despawn harmlessly.
                        events.ScheduleEvent(EVENT_DESPAWN, 45000);
                        events.ScheduleEvent(EVENT_CHECK, 500);
                        break;
                    }
                    case EVENT_CHECK:
                    {
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            if(target->GetTypeId() == TYPEID_PLAYER)
                                me->CastSpell(target, SPELL_SHADOW_TRAP_EFFECT, true);
                        }
                        events.ScheduleEvent(EVENT_CHECK, 1500);
                        break;
                    }
                    case EVENT_DESPAWN:
                    {
                        DoCast(me, SPELL_SHADOW_TRAP_INTRO, true);
                        me->RemoveAura(SPELL_SHADOW_TRAP_PERIODIC);
                        me->DespawnOrUnsummon(2000);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_shadow_trapAI(pCreature);
    }
};

void AddSC_boss_lichking()
{
    new boss_the_lich_king;
    new npc_tirion_icc;
    new npc_valkyr_icc;
    new npc_vile_spirit_icc;
    new npc_shambling_horror_icc;
    new npc_raging_spirit_icc;
    new npc_ice_sphere_icc;
    new npc_defile_icc;
    new npc_spirit_warden_icc;
    new npc_terenas_fighter_icc;
    new npc_shadow_trap;
    new spell_lich_king_necrotic_plague;
    new spell_lich_king_infection;
    new spell_lich_king_valkyr_summon;
    new spell_lich_king_vile_spirit_summon;
    new spell_lich_king_vile_spirit_summon_visual;
    new spell_lich_king_winter;
    new spell_vile_spirit_distance_check;
    new spell_lich_king_pain_and_suffering_effect;
    new spell_ice_burst_distance_check;
    new spell_lich_king_quake;
    new spell_lich_king_play_movie;
    new spell_valkyr_carry_can_cast;
    new spell_valkyr_target_search;
    new spell_valkyr_eject_passenger;
    new spell_vile_spirit_target_search;
    new spell_lich_king_defile;
    new spell_lich_king_tirion_mass_resurrection;
    new spell_lich_king_harvest_soul;
}

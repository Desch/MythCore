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

#include "ScriptPCH.h"
#include "pit_of_saron.h"

enum Yells
{
    SAY_AMBUSH_1                    = -1658050,
    SAY_AMBUSH_2                    = -1658051,
    SAY_GAUNTLET_START              = -1658052,
    SAY_TYRANNUS_INTRO_1            = -1658053,
    SAY_GORKUN_INTRO_2              = -1658054,
    SAY_TYRANNUS_INTRO_3            = -1658055,

    SAY_AGGRO                       = -1658056,
    SAY_SLAY_1                      = -1658057,
    SAY_SLAY_2                      = -1658058,
    SAY_DEATH                       = -1658059,
    SAY_MARK_RIMEFANG_1             = -1658060,
    SAY_MARK_RIMEFANG_2             = -1658061,
    SAY_DARK_MIGHT_1                = -1658062,
    SAY_DARK_MIGHT_2                = -1658063,

    SAY_GORKUN_OUTRO_1              = -1658064,
    SAY_GORKUN_OUTRO_2              = -1658065,
    SAY_JAYNA_OUTRO_3               = -1658066,
    SAY_SYLVANAS_OUTRO_3            = -1658067,
    SAY_JAYNA_OUTRO_4               = -1658068,
    SAY_SYLVANAS_OUTRO_4            = -1658069,
    SAY_JAYNA_OUTRO_5               = -1658070,

    SAY_OPENING_1_T                 = -1658074,
    SAY_OPENING_2_T                 = -1658075,
    SAY_OPENING_3_S                 = -1658076,
    SAY_OPENING_3_J                 = -1658077,
    SAY_OPENING_4_T                 = -1658078,
    SAY_SOUND                       = 16750,
    SAY_OPENING_6_S                 = -1658080,
    SAY_OPENING_6_J                 = -1658081,
    SAY_OPENING_7_T                 = -1658082,
    SAY_OPENING_8_J                 = -1658084,
    SAY_OPENING_9_S                 = -1658085,
    SAY_OPENING_9_J                 = -1658086,
    SAY_OPENING_10_S                = -1658087,
    SAY_OPENING_10_J                = -1658088,
};

enum Spells
{
    SPELL_OVERLORD_BRAND            = 69172,
    SPELL_OVERLORD_BRAND_HEAL       = 69190,
    SPELL_OVERLORD_BRAND_DAMAGE     = 69189,
    SPELL_FORCEFUL_SMASH            = 69155,
    SPELL_UNHOLY_POWER              = 69167,
    SPELL_MARK_OF_RIMEFANG          = 69275,
    SPELL_HOARFROST                 = 69246,

    SPELL_ICY_BLAST                 = 69232,
    SPELL_ICY_BLAST_AURA            = 69238,

    SPELL_EJECT_ALL_PASSENGERS      = 50630,
    SPELL_FULL_HEAL                 = 43979,

    //cave
    SPELL_ICICLE_FALL               = 69428,
    SPELL_FALL_DAMAGE               = 62236,
    SPELL_ICICLE                    = 62234,

    //outro
    SPELL_BLINK_VISUAL              = 36937,
    SPELL_FROST_BOMB                = 70521,

    //opening
    SPELL_NECROMANTIC_POWER         = 69347,
    SPELL_DEATH_ARMY_VISUAL         = 67693,
    SPELL_BLIZZARD_JAINA            = 70132,
    SPELL_ICE_LANCE_JAINA           = 70464,
    SPELL_SHADOW_BOLT_SYLVANAS      = 70512,
    SPELL_MULTI_SHOT_SYLVANAS       = 70513,
};

enum Events
{
    EVENT_OVERLORD_BRAND    = 1,
    EVENT_FORCEFUL_SMASH    = 2,
    EVENT_UNHOLY_POWER      = 3,
    EVENT_MARK_OF_RIMEFANG  = 4,

    EVENT_MOVE_NEXT         = 5,
    EVENT_HOARFROST         = 6,
    EVENT_ICY_BLAST         = 7,

    EVENT_INTRO_1           = 8,
    EVENT_INTRO_2           = 9,
    EVENT_INTRO_3           = 10,
    EVENT_COMBAT_START      = 11,
};

enum Phases
{
    PHASE_NONE      = 0,
    PHASE_INTRO     = 1,
    PHASE_COMBAT    = 2,
    PHASE_OUTRO     = 3,
};

enum Actions
{
    ACTION_START_INTRO      = 1,
    ACTION_START_RIMEFANG   = 2,
    ACTION_START_OUTRO      = 3,
    ACTION_END_COMBAT       = 4,
};

#define GUID_HOARFROST 1

static const Position MoveLocations1[9] =
{
    {1064.217896f, 118.629662f, 628.156311f, 0.000000f},
    {1065.733276f, 126.342400f, 628.156128f, 0.000000f},
    {1060.914185f, 130.460403f, 628.156128f, 0.000000f},
    {1059.328003f, 120.532974f, 628.156128f, 0.000000f},
    {1052.488647f, 122.232979f, 628.156128f, 0.000000f},
    {1047.673950f, 121.389717f, 628.156128f, 0.000000f},
    {1043.781250f, 113.463493f, 628.156128f, 0.000000f},
    {1044.634521f, 109.196129f, 628.518188f, 0.000000f},
    {1052.443726f, 110.813431f, 628.156250f, 0.000000f},
};

static const Position MoveLocations2[9] =
{
    {1025.534790f, 129.039612f, 628.156189f, 0.000000f},
    {1035.394897f, 144.298599f, 628.156189f, 0.000000f},
    {1042.624390f, 156.986679f, 628.156189f, 0.000000f},
    {1037.534790f, 132.039612f, 628.156189f, 0.000000f},
    {1050.534790f, 140.039612f, 628.156189f, 0.000000f},
    {1043.534790f, 141.039612f, 628.156189f, 0.000000f},
    {1038.534790f, 130.039612f, 628.156189f, 0.000000f},
    {1029.534790f, 125.039612f, 628.156189f, 0.000000f},
    {1028.534790f, 158.039612f, 628.156189f, 0.000000f},
};

static const Position rimefangPos[10] =
{
    {1017.299f, 168.9740f, 642.9259f, 0.000000f},
    {1047.868f, 126.4931f, 665.0453f, 0.000000f},
    {1069.828f, 138.3837f, 665.0453f, 0.000000f},
    {1063.042f, 164.5174f, 665.0453f, 0.000000f},
    {1031.158f, 195.1441f, 665.0453f, 0.000000f},
    {1019.087f, 197.8038f, 665.0453f, 0.000000f},
    {967.6233f, 168.9670f, 665.0453f, 0.000000f},
    {969.1198f, 140.4722f, 665.0453f, 0.000000f},
    {986.7153f, 141.6424f, 665.0453f, 0.000000f},
    {1012.601f, 142.4965f, 665.0453f, 0.000000f},
};

static const Position triggerPos1 = {1048.629150f, 110.203377f, 628.224060f, 2.118303f};
static const Position triggerPos2 = {1063.679932f, 119.296852f, 628.156189f, 2.251821f};
static const Position sindraPos = {986.353271f, 174.938004f, 670.492798f, 0.000000f};
static const Position sylOrjainaPos = {1067.227173f, 96.177116f, 630.999023f, 2.440392f};
static const Position leaderPosOutro1 = {1064.217896f, 118.629662f, 628.156311f, 0.000000f};
static const Position leaderPosOutro2 = {988.998962f, 172.250290f, 628.156128f, 0.000000f};
static const Position miscPos = {1018.376f, 167.2495f, 628.2811f, 0.000000f};   //tyrannus combat start position

class boss_tyrannus : public CreatureScript
{
public:
    boss_tyrannus() : CreatureScript("boss_tyrannus") { }

    struct boss_tyrannusAI : public BossAI
    {
        boss_tyrannusAI(Creature* pCreature) : BossAI(pCreature, DATA_TYRANNUS) { }

        void InitializeAI()
        {
            if(!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != GetScriptId(PoSScriptName))
                me->IsAIEnabled = false;
            else if(instance->GetBossState(DATA_TYRANNUS) != DONE)
                Reset();
            else
                me->DespawnOrUnsummon();
        }

        void Reset()
        {
            events.Reset();
            events.SetPhase(PHASE_NONE);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            instance->SetBossState(DATA_TYRANNUS, NOT_STARTED);
        }

        Creature* GetRimefang()
        {
            return ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_RIMEFANG));
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoScriptText(SAY_AGGRO, me);
        }

        void AttackStart(Unit* victim)
        {
            if(me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                return;

            if(victim && me->Attack(victim, true) && !(events.GetPhaseMask() & (1 << PHASE_INTRO)))
                me->GetMotionMaster()->MoveChase(victim);
        }

        void EnterEvadeMode()
        {
            instance->SetBossState(DATA_TYRANNUS, FAIL);
            if(Creature* rimefang = GetRimefang())
                rimefang->AI()->EnterEvadeMode();

            me->DespawnOrUnsummon();
        }

        void KilledUnit(Unit* victim)
        {
            if(victim->GetTypeId() == TYPEID_PLAYER)
                DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2), me);
        }

        void JustDied(Unit* /*pKiller*/)
        {
            DoScriptText(SAY_DEATH, me);
            instance->SetBossState(DATA_TYRANNUS, DONE);

            if(TempSummon* summ = me->ToTempSummon())
                summ->SetTempSummonType(TEMPSUMMON_DEAD_DESPAWN);

            if(Creature* rimefang = GetRimefang())
                rimefang->AI()->DoAction(ACTION_END_COMBAT);
        }

        void DoAction(const int32 actionId)
        {
            if(actionId == ACTION_START_INTRO)
            {
                DoScriptText(SAY_TYRANNUS_INTRO_1, me);
                events.SetPhase(PHASE_INTRO);
                events.ScheduleEvent(EVENT_INTRO_1, 14000, 0, PHASE_INTRO);
                events.ScheduleEvent(EVENT_INTRO_2, 22000, 0, PHASE_INTRO);
                events.ScheduleEvent(EVENT_INTRO_3, 34000, 0, PHASE_INTRO);
                events.ScheduleEvent(EVENT_COMBAT_START, 36000, 0, PHASE_INTRO);
                instance->SetBossState(DATA_TYRANNUS, IN_PROGRESS);
                // summon npcs
                if(Creature* pSlave = me->SummonCreature(NPC_GORKUN_IRONSKULL_FREED, 1075.489868f, 20.001131f, 632.835938f, 1.659531f, TEMPSUMMON_DEAD_DESPAWN, 10000))
                {
                    pSlave->GetMotionMaster()->MovePoint(0, leaderPosOutro1);
                    pSlave->SetHomePosition(leaderPosOutro1);
                }

                for(uint8 i = 0; i < 9; ++i)
                {
                    if(Creature* pSlave = me->SummonCreature(NPC_FREED_HORDE_SLAVE1, 1086.112061f, 21.060266f, 631.892273f, 1.995682f, TEMPSUMMON_DEAD_DESPAWN, 30000))
                    {
                        pSlave->GetMotionMaster()->MovePoint(0, MoveLocations1[i]);
                        pSlave->SetHomePosition(MoveLocations1[i]);
                    }
                    ++i;
                    if(Creature* pSlave = me->SummonCreature(NPC_FREED_HORDE_SLAVE2, 1069.121582f, 18.495785f, 634.020203f, 1.573138f, TEMPSUMMON_DEAD_DESPAWN, 30000))
                    {
                        pSlave->GetMotionMaster()->MovePoint(0, MoveLocations1[i]);
                        pSlave->SetHomePosition(MoveLocations1[i]);
                    }
                    ++i;
                    if(Creature* pSlave = me->SummonCreature(NPC_FREED_HORDE_SLAVE3, 1075.489868f, 20.001131f, 632.835938f, 1.659531f, TEMPSUMMON_DEAD_DESPAWN, 30000))
                    {
                        pSlave->GetMotionMaster()->MovePoint(0, MoveLocations1[i]);
                        pSlave->SetHomePosition(MoveLocations1[i]);
                    }
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim() && !(events.GetPhaseMask() & (1 << PHASE_INTRO)))
                return;

            events.Update(diff);

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_INTRO_1:
                        if(Creature* GorkunOrVictus = me->GetCreature(*me, instance->GetData64(DATA_VICTUS_OR_GORKUN_FREED)))
                            DoScriptText(SAY_GORKUN_INTRO_2, GorkunOrVictus);
                        break;
                    case EVENT_INTRO_2:
                        DoScriptText(SAY_TYRANNUS_INTRO_3, me);
                        break;
                    case EVENT_INTRO_3:
                        me->ExitVehicle();
                        me->GetMotionMaster()->MovePoint(0, miscPos);
                        break;
                    case EVENT_COMBAT_START:
                        if(Creature* rimefang = me->GetCreature(*me, instance->GetData64(DATA_RIMEFANG)))
                            rimefang->AI()->DoAction(ACTION_START_RIMEFANG);    //set rimefang also infight
                        events.SetPhase(PHASE_COMBAT);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->SetReactState(REACT_AGGRESSIVE);
                        DoCast(me, SPELL_FULL_HEAL);
                        DoZoneInCombat();
                        events.ScheduleEvent(EVENT_OVERLORD_BRAND, urand(5000, 7000));
                        events.ScheduleEvent(EVENT_FORCEFUL_SMASH, urand(14000, 16000));
                        events.ScheduleEvent(EVENT_MARK_OF_RIMEFANG, urand(25000, 27000));
                        break;
                    case EVENT_OVERLORD_BRAND:
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                            DoCast(target, SPELL_OVERLORD_BRAND);
                        events.ScheduleEvent(EVENT_OVERLORD_BRAND, urand(11000, 12000));
                        break;
                    case EVENT_FORCEFUL_SMASH:
                        DoCastVictim(SPELL_FORCEFUL_SMASH);
                        events.ScheduleEvent(EVENT_UNHOLY_POWER, 1000);
                        break;
                    case EVENT_UNHOLY_POWER:
                        DoScriptText(SAY_DARK_MIGHT_1, me);
                        DoScriptText(SAY_DARK_MIGHT_2, me);
                        DoCast(me, SPELL_UNHOLY_POWER);
                        events.ScheduleEvent(EVENT_FORCEFUL_SMASH, urand(40000, 48000));
                        break;
                    case EVENT_MARK_OF_RIMEFANG:
                        DoScriptText(SAY_MARK_RIMEFANG_1, me);
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                        {
                            DoScriptText(SAY_MARK_RIMEFANG_2, me, target);
                            DoCast(target, SPELL_MARK_OF_RIMEFANG);
                        }
                        events.ScheduleEvent(EVENT_MARK_OF_RIMEFANG, urand(24000, 26000));
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_tyrannusAI(pCreature);
    }
};

class boss_rimefang : public CreatureScript
{
public:
    boss_rimefang() : CreatureScript("boss_rimefang") { }

    struct boss_rimefangAI : public ScriptedAI
    {
        boss_rimefangAI(Creature* pCreature): ScriptedAI(pCreature), _vehicle(pCreature->GetVehicleKit())
        {
            ASSERT(_vehicle);
        }

        void Reset()
        {
            _events.Reset();
            _events.SetPhase(PHASE_NONE);
            _currentWaypoint = 0;
            _hoarfrostTargetGUID = 0;
            me->SetFlying(true);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        void JustReachedHome()
        {
            _vehicle->InstallAllAccessories(false);
        }

            void DoAction(const int32 actionId)
        {
            if(actionId == ACTION_START_RIMEFANG)
            {
                _events.SetPhase(PHASE_COMBAT);
                DoZoneInCombat();
                _events.ScheduleEvent(EVENT_MOVE_NEXT, 500, 0, PHASE_COMBAT);
                _events.ScheduleEvent(EVENT_ICY_BLAST, 15000, 0, PHASE_COMBAT);
            }
            else if(actionId == ACTION_END_COMBAT)
                _EnterEvadeMode();
        }

        void SetGUID(const uint64& guid, int32 type)
        {
            if(type == GUID_HOARFROST)
            {
                _hoarfrostTargetGUID = guid;
                _events.ScheduleEvent(EVENT_HOARFROST, 1000);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim() && !(_events.GetPhaseMask() & (1 << PHASE_COMBAT)))
                return;

            _events.Update(diff);

            while(uint32 eventId = _events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_MOVE_NEXT:
                        if(_currentWaypoint >= 10 || _currentWaypoint == 0)
                            _currentWaypoint = 1;
                        me->GetMotionMaster()->MovePoint(0, rimefangPos[_currentWaypoint]);
                        ++_currentWaypoint;
                        _events.ScheduleEvent(EVENT_MOVE_NEXT, 2000, 0, PHASE_COMBAT);
                        break;
                    case EVENT_ICY_BLAST:
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(target, SPELL_ICY_BLAST);
                        _events.ScheduleEvent(EVENT_ICY_BLAST, 15000, 0, PHASE_COMBAT);
                        break;
                    case EVENT_HOARFROST:
                        if(Unit* target = me->GetUnit(*me, _hoarfrostTargetGUID))
                        {
                            DoCast(target, SPELL_HOARFROST);
                            _hoarfrostTargetGUID = 0;
                        }
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        Vehicle* _vehicle;
        uint64 _hoarfrostTargetGUID;
        EventMap _events;
        uint8 _currentWaypoint;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_rimefangAI(pCreature);
    }
};

class npc_tyrannus_icicle : public CreatureScript
{
public:
    npc_tyrannus_icicle() : CreatureScript("npc_tyrannus_icicle") { }

    struct npc_tyrannus_icicleAI : public Scripted_NoMovementAI
    {
        npc_tyrannus_icicleAI(Creature* pCreature): Scripted_NoMovementAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        void InitializeAI()
        {
            if(!pInstance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != GetScriptId(PoSScriptName))
                me->IsAIEnabled = false;
        }

        uint32 IcicleTimer;

        void Reset()
        {
            IcicleTimer = urand(3000, 10000);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(const uint32(diff))
        {
            if(pInstance->GetBossState(DATA_TYRANNUS) != DONE)
            {
                if(IcicleTimer <= diff)
                {
                    DoCast(me, SPELL_FALL_DAMAGE);
                    DoCast(me, SPELL_ICICLE_FALL);
                    DoCast(me, SPELL_ICICLE);
                    IcicleTimer = urand(3000, 10000);
                } else IcicleTimer -= diff;
            }
        }

    private:
        InstanceScript* pInstance;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_tyrannus_icicleAI(pCreature);
    }
};

class npc_jaina_or_sylvanas_outro : public CreatureScript
{
public:
    npc_jaina_or_sylvanas_outro() : CreatureScript("npc_jaina_or_sylvanas_outro") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_jaina_or_sylvanas_outroAI(pCreature);
    }

    struct npc_jaina_or_sylvanas_outroAI : public ScriptedAI
    {
        npc_jaina_or_sylvanas_outroAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
            Reset();
        }

        void Reset()
        {
            uiIntroTimer1 = 0;
            uiIntroPhase1 = 0;
            me->SetVisible(true);
        }

        void UpdateAI(const uint32 diff)
        {
            if(pInstance->GetData(DATA_TYRANNUS) == DONE)
            {
                if(uiIntroTimer1 <= diff)
                {
                    switch(uiIntroPhase1)
                    {
                        case 0:
                            //Move with npcs
                            if(Creature* GorkunOrVictus = me->GetCreature(*me, pInstance->GetData64(DATA_VICTUS_OR_GORKUN_FREED)))
                                GorkunOrVictus->GetMotionMaster()->MovePoint(0, leaderPosOutro2);

                            if(pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                            {
                                uint8 i = 0;
                                GetCreatureListWithEntryInGrid(Champions1, me, NPC_FREED_ALLIANCE_SLAVE1, 100.0f);
                                for(std::list<Creature*>::iterator itr = Champions1.begin(); itr != Champions1.end(); ++itr)
                                {
                                    Creature* slave = *itr;
                                    if(!slave)
                                        continue;

                                    if(slave->isAlive())
                                        slave->GetMotionMaster()->MovePoint(0, MoveLocations2[i]);
                                    ++i;
                                }

                                i = 4;
                                GetCreatureListWithEntryInGrid(Champions2, me, NPC_FREED_ALLIANCE_SLAVE2, 100.0f);
                                for(std::list<Creature*>::iterator itr = Champions2.begin(); itr != Champions2.end(); ++itr)
                                {
                                    Creature* slave = *itr;
                                    if(!slave)
                                        continue;

                                    if(slave->isAlive())
                                        slave->GetMotionMaster()->MovePoint(0, MoveLocations2[i]);
                                    ++i;
                                }

                                i = 7;
                                GetCreatureListWithEntryInGrid(Champions3, me, NPC_FREED_ALLIANCE_SLAVE3, 100.0f);
                                for(std::list<Creature*>::iterator itr = Champions3.begin(); itr != Champions3.end(); ++itr)
                                {
                                    Creature* slave = *itr;
                                    if(!slave)
                                        continue;

                                    if(slave->isAlive())
                                        slave->GetMotionMaster()->MovePoint(0, MoveLocations2[i]);
                                    ++i;
                                }
                            }
                            else
                            {
                                uint8 i = 0;
                                GetCreatureListWithEntryInGrid(Champions1, me, NPC_FREED_HORDE_SLAVE1, 100.0f);
                                for(std::list<Creature*>::iterator itr = Champions1.begin(); itr != Champions1.end(); ++itr)
                                {
                                    Creature* slave = *itr;
                                    if(!slave)
                                        continue;

                                    if(slave->isAlive())
                                        slave->GetMotionMaster()->MovePoint(0, MoveLocations2[i]);
                                    ++i;
                                }

                                i = 4;
                                GetCreatureListWithEntryInGrid(Champions2, me, NPC_FREED_HORDE_SLAVE2, 100.0f);
                                for(std::list<Creature*>::iterator itr = Champions2.begin(); itr != Champions2.end(); ++itr)
                                {
                                    Creature* slave = *itr;
                                    if(!slave)
                                        continue;

                                    if(slave->isAlive())
                                        slave->GetMotionMaster()->MovePoint(0, MoveLocations2[i]);
                                    ++i;
                                }

                                i = 7;
                                GetCreatureListWithEntryInGrid(Champions3, me, NPC_FREED_HORDE_SLAVE3, 100.0f);
                                for(std::list<Creature*>::iterator itr = Champions3.begin(); itr != Champions3.end(); ++itr)
                                {
                                    Creature* slave = *itr;
                                    if(!slave)
                                        continue;

                                    if(slave->isAlive())
                                        slave->GetMotionMaster()->MovePoint(0, MoveLocations2[i]);
                                    ++i;
                                }
                            }
                            ++uiIntroPhase1;
                            uiIntroTimer1 = 15000;
                            break;
                        case 1:
                            if(Creature* GorkunOrVictus = me->GetCreature(*me, pInstance->GetData64(DATA_VICTUS_OR_GORKUN_FREED)))
                                DoScriptText(SAY_GORKUN_OUTRO_1, GorkunOrVictus);
                            ++uiIntroPhase1;
                            uiIntroTimer1 = 15000;
                            break;
                        case 2:
                            if(Creature* GorkunOrVictus = me->GetCreature(*me, pInstance->GetData64(DATA_VICTUS_OR_GORKUN_FREED)))
                                DoScriptText(SAY_GORKUN_OUTRO_2, GorkunOrVictus);
                            ++uiIntroPhase1;
                            uiIntroTimer1 = 5000;
                            break;
                        case 3:
                            me->SetVisible(true);
                            if(Creature* pSindragosa = me->SummonCreature(NPC_SINDRAGOSA, sindraPos, TEMPSUMMON_CORPSE_DESPAWN, 3000))
                                pSindragosa->SetFlying(true);
                            ++uiIntroPhase1;
                            uiIntroTimer1 = 5000;
                            break;
                        case 4:
                            if(Creature* portal = me->SummonCreature(22517, triggerPos1, TEMPSUMMON_TIMED_DESPAWN, 30000))
                            {
                                portal->CastSpell(portal, 51807, false);
                                portal->SetDisplayId(17612);
                            }
                            if(Creature* portal = me->SummonCreature(22517, triggerPos2, TEMPSUMMON_TIMED_DESPAWN, 30000))
                            {
                                portal->CastSpell(portal, 51807, false);
                                portal->SetDisplayId(17612);
                            }
                            if(me->GetEntry() == NPC_SYLVANAS_PART2)
                                DoScriptText(SAY_SYLVANAS_OUTRO_3, me);
                            else
                                DoScriptText(SAY_JAYNA_OUTRO_3, me);

                            if(pInstance)
                            {
                                Map* pMap = me->GetMap();
                                if(!pMap)
                                    return;

                                Map::PlayerList const &lPlayers = pMap->GetPlayers();
                                for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                                {
                                    if(!itr->getSource()->isAlive())
                                        continue;
                                    me->CastSpell(itr->getSource(), SPELL_BLINK_VISUAL, true);
                                    itr->getSource()->NearTeleportTo(1065.114746f, 96.392105f, 630.999573f, 2.063386f);
                                }
                            }
                            if(Creature* GorkunOrVictus = me->GetCreature(*me, pInstance->GetData64(DATA_VICTUS_OR_GORKUN_FREED)))
                                if(Creature* pSindragosa = me->GetCreature(*me, pInstance->GetData64(DATA_SINDRAGOSA)))
                                    pSindragosa->CastSpell(GorkunOrVictus, SPELL_FROST_BOMB, true);

                            // kill leader
                            if(Creature* GorkunOrVictus = me->GetCreature(*me, pInstance->GetData64(DATA_VICTUS_OR_GORKUN_FREED)))
                                me->Kill(GorkunOrVictus, false);

                            for(std::list<Creature*>::iterator itr = Champions1.begin(); itr != Champions1.end(); ++itr)
                            {
                                Creature* slave = *itr;
                                if(!slave)
                                    continue;

                                if(slave->isAlive())
                                    slave->Kill(slave, false);
                            }

                            for(std::list<Creature*>::iterator itr = Champions2.begin(); itr != Champions2.end(); ++itr)
                            {
                                Creature* slave = *itr;
                                if(!slave)
                                    continue;

                                if(slave->isAlive())
                                    slave->Kill(slave, false);
                            }

                            for(std::list<Creature*>::iterator itr = Champions3.begin(); itr != Champions3.end(); ++itr)
                            {
                                Creature* slave = *itr;
                                if(!slave)
                                    continue;

                                if(slave->isAlive())
                                    slave->Kill(slave, false);
                            }
                            ++uiIntroPhase1;
                            uiIntroTimer1 = 5000;
                            break;
                        case 5:
                            if(Creature* pSindragosa = me->GetCreature(*me, pInstance->GetData64(DATA_SINDRAGOSA)))
                                pSindragosa->GetMotionMaster()->MoveCharge(804.957214f, 102.497406f, 728.966370f, 42.00f, 0);

                            if(me->GetEntry() == NPC_SYLVANAS_PART2)
                                DoScriptText(SAY_SYLVANAS_OUTRO_4, me);
                            else
                                DoScriptText(SAY_JAYNA_OUTRO_4, me);
                            ++uiIntroPhase1;
                            uiIntroTimer1 = 7000;
                            break;
                        case 6:
                            if(me->GetEntry() == NPC_JAINA_PART2)
                                DoScriptText(SAY_JAYNA_OUTRO_5, me);
                            ++uiIntroPhase1;
                            uiIntroTimer1 = 7000;
                            break;
                    return;
                    }
                } else uiIntroTimer1 -= diff;
            }
        }

    private:
        InstanceScript* pInstance;
        uint32 uiIntroTimer1;
        uint8 uiIntroPhase1;
        std::list<Creature*> Champions1;
        std::list<Creature*> Champions2;
        std::list<Creature*> Champions3;
    };
};

class npc_jaina_or_sylvanas_intro : public CreatureScript
{
public:
    npc_jaina_or_sylvanas_intro() : CreatureScript("npc_jaina_or_sylvanas_intro") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_jaina_or_sylvanas_introAI(pCreature);
    }

    struct npc_jaina_or_sylvanas_introAI : public ScriptedAI
    {
        npc_jaina_or_sylvanas_introAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
            Reset();
        }

        void Reset()
        {
            uiIntroTimer2 = 5000;
            uiIntroPhase2 = 0;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            preevent = true;
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(const uint32 diff)
        {
            if(pInstance->GetData(DATA_TYRANNUS_EVENT_PRE_INTRO) == IN_PROGRESS && preevent)
            {
                if(uiIntroTimer2 <= diff)
                {
                    switch(uiIntroPhase2)
                    {
                        case 0:
                            if(Creature* pTyrannusIntro = me->FindNearestCreature(NPC_TYRANNUS_EVENTS, 150.0f, true))
                            {
                                pTyrannusIntro->SetFlying(true);
                                pTyrannusIntro->GetMotionMaster()->MovePoint(0, 508.476593f, 230.144485f, 541.618103f);
                                DoScriptText(SAY_OPENING_1_T, pTyrannusIntro);
                            }
                            ++uiIntroPhase2;
                            uiIntroTimer2 = 5000;
                            break;
                        case 1:
                            if(Creature* pTyrannusIntro = me->FindNearestCreature(NPC_TYRANNUS_EVENTS, 150.0f, true))
                                DoScriptText(SAY_OPENING_2_T, pTyrannusIntro);
                            ++uiIntroPhase2;
                            uiIntroTimer2 = 15000;
                            break;
                        case 2:
                            if(me->GetEntry() == NPC_SYLVANAS_PART1)
                                DoScriptText(SAY_OPENING_3_S, me);
                            else
                                DoScriptText(SAY_OPENING_3_J, me);

                            if(pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                            {
                                GetCreatureListWithEntryInGrid(Champions1, me, NPC_CHAMPION_1_ALLIANCE, 150.0f);
                                for(std::list<Creature*>::iterator itr = Champions1.begin(); itr != Champions1.end(); ++itr)
                                {
                                    Creature* pChampion = *itr;
                                    if(!pChampion)
                                        continue;

                                    if(pChampion->isAlive())
                                        if(Creature* pNecrolyte = me->FindNearestCreature(NPC_NECROLYTE, 150.0f, true))
                                        {
                                            pChampion->Attack(pNecrolyte, true);
                                            pChampion->GetMotionMaster()->MoveChase(pNecrolyte);
                                        }
                                }

                                GetCreatureListWithEntryInGrid(Champions2, me, NPC_CHAMPION_2_ALLIANCE, 150.0f);
                                for(std::list<Creature*>::iterator itr = Champions2.begin(); itr != Champions2.end(); ++itr)
                                {
                                    Creature* pChampion = *itr;
                                    if(!pChampion)
                                        continue;

                                    if(pChampion->isAlive())
                                        if(Creature* pNecrolyte = me->FindNearestCreature(NPC_NECROLYTE, 150.0f, true))
                                        {
                                            pChampion->Attack(pNecrolyte, true);
                                            pChampion->GetMotionMaster()->MoveChase(pNecrolyte);
                                        }
                                }
                            }
                            else
                            {
                                GetCreatureListWithEntryInGrid(Champions1, me, NPC_CHAMPION_1_HORDE, 150.0f);
                                for(std::list<Creature*>::iterator itr = Champions1.begin(); itr != Champions1.end(); ++itr)
                                {
                                    Creature* pChampion = *itr;
                                    if(!pChampion)
                                        continue;

                                    if(pChampion->isAlive())
                                        if(Creature* pNecrolyte = me->FindNearestCreature(NPC_NECROLYTE, 150.0f, true))
                                        {
                                            pChampion->Attack(pNecrolyte, true);
                                            pChampion->GetMotionMaster()->MoveChase(pNecrolyte);
                                        }
                                }

                                GetCreatureListWithEntryInGrid(Champions2, me, NPC_CHAMPION_2_HORDE, 150.0f);
                                for(std::list<Creature*>::iterator itr = Champions2.begin(); itr != Champions2.end(); ++itr)
                                {
                                    Creature* pChampion = *itr;
                                    if(!pChampion)
                                        continue;

                                    if(pChampion->isAlive())
                                        if(Creature* pNecrolyte = me->FindNearestCreature(NPC_NECROLYTE, 150.0f, true))
                                        {
                                            pChampion->Attack(pNecrolyte, true);
                                            pChampion->GetMotionMaster()->MoveChase(pNecrolyte);
                                        }
                                }

                                GetCreatureListWithEntryInGrid(Champions3, me, NPC_CHAMPION_3_HORDE, 150.0f);
                                for(std::list<Creature*>::iterator itr = Champions3.begin(); itr != Champions3.end(); ++itr)
                                {
                                    Creature* pChampion = *itr;
                                    if(!pChampion)
                                        continue;

                                    if(pChampion->isAlive())
                                        if(Creature* pNecrolyte = me->FindNearestCreature(NPC_NECROLYTE, 150.0f, true))
                                        {
                                            pChampion->Attack(pNecrolyte, true);
                                            pChampion->GetMotionMaster()->MoveChase(pNecrolyte);
                                        }
                                }
                            }
                            ++uiIntroPhase2;
                            uiIntroTimer2 = 1000;
                            break;
                        case 3:
                            if(Creature* pTyrannusIntro = me->FindNearestCreature(NPC_TYRANNUS_EVENTS, 150.0f, true))
                                DoScriptText(SAY_OPENING_4_T, pTyrannusIntro);
                            ++uiIntroPhase2;
                            uiIntroTimer2 = 2000;
                            break;
                        case 4:
                            if(Creature* pTyrannusIntro = me->FindNearestCreature(NPC_TYRANNUS_EVENTS, 150.0f, true))
                                DoPlaySoundToSet(pTyrannusIntro, SAY_SOUND);

                            for(std::list<Creature*>::iterator itr = Champions1.begin(); itr != Champions1.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(pChampion->isAlive())
                                {
                                    pChampion->AttackStop();
                                    pChampion->GetMotionMaster()->MoveFall();
                                }
                            }

                            for(std::list<Creature*>::iterator itr = Champions2.begin(); itr != Champions2.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(pChampion->isAlive())
                                {
                                    pChampion->AttackStop();
                                    pChampion->GetMotionMaster()->MoveFall();
                                }
                            }

                            for(std::list<Creature*>::iterator itr = Champions3.begin(); itr != Champions3.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(pChampion->isAlive())
                                {
                                    pChampion->AttackStop();
                                    pChampion->GetMotionMaster()->MoveFall();
                                }
                            }
                            ++uiIntroPhase2;
                            uiIntroTimer2 = 5000;
                            break;
                        case 5:
                            if(Creature* pTyrannusIntro = me->FindNearestCreature(NPC_TYRANNUS_EVENTS, 150.0f, true))
                                pTyrannusIntro->CastSpell(pTyrannusIntro, SPELL_NECROMANTIC_POWER, false);

                            for(std::list<Creature*>::iterator itr = Champions1.begin(); itr != Champions1.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(pChampion->isAlive())
                                    pChampion->Kill(pChampion, false);
                            }

                            for(std::list<Creature*>::iterator itr = Champions2.begin(); itr != Champions2.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(pChampion->isAlive())
                                    pChampion->Kill(pChampion, false);
                            }

                            for(std::list<Creature*>::iterator itr = Champions3.begin(); itr != Champions3.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(pChampion->isAlive())
                                    pChampion->Kill(pChampion, false);
                            }

                            if(me->GetEntry() == NPC_SYLVANAS_PART1)
                                DoScriptText(SAY_OPENING_6_S, me);
                            else
                                DoScriptText(SAY_OPENING_6_J, me);
                            ++uiIntroPhase2;
                            uiIntroTimer2 = 5000;
                            break;
                        case 6:
                            for(std::list<Creature*>::iterator itr = Champions1.begin(); itr != Champions1.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                pChampion->Respawn();
                                pChampion->CastSpell(pChampion, SPELL_DEATH_ARMY_VISUAL, true);
                                if(me->GetEntry() == NPC_SYLVANAS_PART1)
                                    pChampion->UpdateEntry(NPC_HUNGERING_GHOUL, ALLIANCE);
                                else
                                    pChampion->UpdateEntry(NPC_HUNGERING_GHOUL, HORDE);
                                pChampion->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                pChampion->SetReactState(REACT_PASSIVE);
                            }

                            for(std::list<Creature*>::iterator itr = Champions2.begin(); itr != Champions2.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                pChampion->Respawn();
                                pChampion->CastSpell(pChampion, SPELL_DEATH_ARMY_VISUAL, true);
                                if(me->GetEntry() == NPC_SYLVANAS_PART1)
                                    pChampion->UpdateEntry(NPC_HUNGERING_GHOUL, ALLIANCE);
                                else
                                    pChampion->UpdateEntry(NPC_HUNGERING_GHOUL, HORDE);
                                pChampion->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                pChampion->SetReactState(REACT_PASSIVE);
                            }

                            for(std::list<Creature*>::iterator itr = Champions3.begin(); itr != Champions3.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                pChampion->Respawn();
                                pChampion->CastSpell(pChampion, SPELL_DEATH_ARMY_VISUAL, true);
                                if(me->GetEntry() == NPC_SYLVANAS_PART1)
                                    pChampion->UpdateEntry(NPC_HUNGERING_GHOUL, ALLIANCE);
                                else
                                    pChampion->UpdateEntry(NPC_HUNGERING_GHOUL, HORDE);
                                pChampion->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                pChampion->SetReactState(REACT_PASSIVE);
                            }

                            if(Creature* pTyrannusIntro = me->FindNearestCreature(NPC_TYRANNUS_EVENTS, 150.0f, true))
                                DoScriptText(SAY_OPENING_7_T, pTyrannusIntro);

                            for(std::list<Creature*>::iterator itr = Champions1.begin(); itr != Champions1.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(Creature* pHelper = me->FindNearestCreature(NPC_KORALEN, 150.0f, true))
                                {
                                    pChampion->Attack(pHelper, true);
                                    pChampion->GetMotionMaster()->MoveChase(pHelper);
                                }
                                else if(Creature* pHelper = me->FindNearestCreature(NPC_KORLAEN, 150.0f, true))
                                {
                                    pChampion->Attack(pHelper, true);
                                    pChampion->GetMotionMaster()->MoveChase(pHelper);
                                }
                            }

                            for(std::list<Creature*>::iterator itr = Champions2.begin(); itr != Champions2.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(Creature* pHelper = me->FindNearestCreature(NPC_KORALEN, 150.0f, true))
                                {
                                    pChampion->Attack(pHelper, true);
                                    pChampion->GetMotionMaster()->MoveChase(pHelper);
                                }
                                else if(Creature* pHelper = me->FindNearestCreature(NPC_KORLAEN, 150.0f, true))
                                {
                                    pChampion->Attack(pHelper, true);
                                    pChampion->GetMotionMaster()->MoveChase(pHelper);
                                }
                            }

                            for(std::list<Creature*>::iterator itr = Champions3.begin(); itr != Champions3.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(Creature* pHelper = me->FindNearestCreature(NPC_KORALEN, 150.0f, true))
                                {
                                    pChampion->Attack(pHelper, true);
                                    pChampion->GetMotionMaster()->MoveChase(pHelper);
                                }
                                else if(Creature* pHelper = me->FindNearestCreature(NPC_KORLAEN, 150.0f, true))
                                {
                                    pChampion->Attack(pHelper, true);
                                    pChampion->GetMotionMaster()->MoveChase(pHelper);
                                }
                            }
                            ++uiIntroPhase2;
                            uiIntroTimer2 = 3000;
                            break;
                        case 7:
                            for(std::list<Creature*>::iterator itr = Champions1.begin(); itr != Champions1.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(me->GetEntry() == NPC_SYLVANAS_PART1)
                                    me->CastSpell(pChampion, SPELL_MULTI_SHOT_SYLVANAS, true);
                                else
                                    me->CastSpell(pChampion, SPELL_BLIZZARD_JAINA, true);
                            }

                            for(std::list<Creature*>::iterator itr = Champions2.begin(); itr != Champions2.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(me->GetEntry() == NPC_SYLVANAS_PART1)
                                    me->CastSpell(pChampion, SPELL_MULTI_SHOT_SYLVANAS, true);
                                else
                                    me->CastSpell(pChampion, SPELL_BLIZZARD_JAINA, true);
                            }

                            for(std::list<Creature*>::iterator itr = Champions3.begin(); itr != Champions3.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(me->GetEntry() == NPC_SYLVANAS_PART1)
                                    me->CastSpell(pChampion, SPELL_MULTI_SHOT_SYLVANAS, true);
                                else
                                    me->CastSpell(pChampion, SPELL_BLIZZARD_JAINA, true);
                            }
                            ++uiIntroPhase2;
                            uiIntroTimer2 = 500;
                            break;
                        case 8:
                            for(std::list<Creature*>::iterator itr = Champions1.begin(); itr != Champions1.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(me->GetEntry() == NPC_SYLVANAS_PART1)
                                    me->CastSpell(pChampion, SPELL_SHADOW_BOLT_SYLVANAS, true);
                                else
                                    me->CastSpell(pChampion, SPELL_ICE_LANCE_JAINA, true);
                            }

                            for(std::list<Creature*>::iterator itr = Champions2.begin(); itr != Champions2.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(me->GetEntry() == NPC_SYLVANAS_PART1)
                                    me->CastSpell(pChampion, SPELL_SHADOW_BOLT_SYLVANAS, true);
                                else
                                    me->CastSpell(pChampion, SPELL_ICE_LANCE_JAINA, true);
                            }

                            for(std::list<Creature*>::iterator itr = Champions3.begin(); itr != Champions3.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                if(me->GetEntry() == NPC_SYLVANAS_PART1)
                                    me->CastSpell(pChampion, SPELL_SHADOW_BOLT_SYLVANAS, true);
                                else
                                    me->CastSpell(pChampion, SPELL_ICE_LANCE_JAINA, true);
                            }
                            ++uiIntroPhase2;
                            uiIntroTimer2 = 500;
                            break;
                        case 9:
                            for(std::list<Creature*>::iterator itr = Champions1.begin(); itr != Champions1.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;
                                me->Kill(pChampion, false);
                            }

                            for(std::list<Creature*>::iterator itr = Champions2.begin(); itr != Champions2.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                me->Kill(pChampion, false);
                            }

                            for(std::list<Creature*>::iterator itr = Champions3.begin(); itr != Champions3.end(); ++itr)
                            {
                                Creature* pChampion = *itr;
                                if(!pChampion)
                                    continue;

                                me->Kill(pChampion, false);
                            }
                            ++uiIntroPhase2;
                            uiIntroTimer2 = 3000;
                            break;
                        case 10:
                            if(me->GetEntry() == NPC_JAINA_PART1)
                                DoScriptText(SAY_OPENING_8_J, me);
                            ++uiIntroPhase2;
                            uiIntroTimer2 = 10000;
                        case 11:
                            if(me->GetEntry() == NPC_SYLVANAS_PART1)
                                DoScriptText(SAY_OPENING_9_S, me);
                            if(me->GetEntry() == NPC_JAINA_PART1)
                                DoScriptText(SAY_OPENING_9_J, me);
                            ++uiIntroPhase2;
                            uiIntroTimer2 = 10000;
                            break;
                        case 12:
                            if(Creature* pTyrannusIntro = me->FindNearestCreature(NPC_TYRANNUS_EVENTS, 150.0f, true))
                                pTyrannusIntro->DisappearAndDie();
                            pInstance->SetData(DATA_TYRANNUS_EVENT_PRE_INTRO, DONE);
                            preevent = false;
                            ++uiIntroPhase2;
                            uiIntroTimer2 = 10000;
                            break;
                        return;
                    }
                } else uiIntroTimer2 -= diff;
            }
        }

    private:
        InstanceScript* pInstance;
        uint32 uiIntroTimer2;
        uint8 uiIntroPhase2;
        bool preevent;
        std::list<Creature*> Champions1;
        std::list<Creature*> Champions2;
        std::list<Creature*> Champions3;
    };
};

class player_overlord_brandAI : public PlayerAI
{
public:
    player_overlord_brandAI(Player* player) : PlayerAI(player)
    {
        tyrannus = NULL;
    }

    void SetGUID(const uint64& guid, int32 /*type*/)
    {
        tyrannus = ObjectAccessor::GetCreature(*me, guid);
        if(!tyrannus)
            me->IsAIEnabled = false;
    }

    void DamageDealt(Unit* /*pVictim*/, uint32& damage, DamageEffectType /*damageType*/)
    {
        if(tyrannus->getVictim())
        me->CastCustomSpell(SPELL_OVERLORD_BRAND_DAMAGE, SPELLVALUE_BASE_POINT0, damage, tyrannus->getVictim(), true, NULL, NULL, tyrannus->GetGUID());
    }

    void HealDone(Unit* /*pTarget*/, uint32& addHealth)
    {
        me->CastCustomSpell(SPELL_OVERLORD_BRAND_HEAL, SPELLVALUE_BASE_POINT0, int32(addHealth*5.5f), tyrannus, true, NULL, NULL, tyrannus->GetGUID());
    }

    void UpdateAI(const uint32 /*diff*/) { }

    private:
        Creature* tyrannus;
};

class spell_tyrannus_overlord_brand : public SpellScriptLoader
{
public:
    spell_tyrannus_overlord_brand() : SpellScriptLoader("spell_tyrannus_overlord_brand") { }

    class spell_tyrannus_overlord_brand_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_tyrannus_overlord_brand_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if(GetTarget()->GetTypeId() != TYPEID_PLAYER)
                return;

            oldAI = GetTarget()->GetAI();
            GetTarget()->SetAI(new player_overlord_brandAI(GetTarget()->ToPlayer()));
            GetTarget()->GetAI()->SetGUID(GetCasterGUID());
            oldAIState = GetTarget()->IsAIEnabled;
            GetTarget()->IsAIEnabled = true;
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if(GetTarget()->GetTypeId() != TYPEID_PLAYER)
                return;

            delete GetTarget()->GetAI();
            GetTarget()->SetAI(oldAI);
            GetTarget()->IsAIEnabled = oldAIState;
        }

        void Register()
        {
             AfterEffectApply += AuraEffectApplyFn(spell_tyrannus_overlord_brand_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
             AfterEffectRemove += AuraEffectRemoveFn(spell_tyrannus_overlord_brand_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }

        UnitAI* oldAI;
        bool oldAIState;
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_tyrannus_overlord_brand_AuraScript();
    }
};

class spell_tyrannus_mark_of_rimefang : public SpellScriptLoader
{
public:
    spell_tyrannus_mark_of_rimefang() : SpellScriptLoader("spell_tyrannus_mark_of_rimefang") { }

    class spell_tyrannus_mark_of_rimefang_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_tyrannus_mark_of_rimefang_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* pCaster = GetCaster();
            if(!pCaster || pCaster->GetTypeId() != TYPEID_UNIT)
                return;

            if(InstanceScript* pInstance = pCaster->GetInstanceScript())
                if(Creature* pRimefang = ObjectAccessor::GetCreature(*pCaster, pInstance->GetData64(DATA_RIMEFANG)))
                    pRimefang->AI()->SetGUID(GetTarget()->GetGUID(), GUID_HOARFROST);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_tyrannus_mark_of_rimefang_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_tyrannus_mark_of_rimefang_AuraScript();
    }
};

class at_tyrannus_event_starter : public AreaTriggerScript
{
public:
    at_tyrannus_event_starter() : AreaTriggerScript("at_tyrannus_event_starter") { }

    bool OnTrigger(Player* player, const AreaTriggerEntry* /*pAT*/)
    {
        InstanceScript* instance = player->GetInstanceScript();
        if(player->isGameMaster() || !instance)
            return false;

        if(instance->GetBossState(DATA_TYRANNUS) != IN_PROGRESS && instance->GetBossState(DATA_TYRANNUS) != DONE)
            if(Creature* tyrannus = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_TYRANNUS)))
            {
                tyrannus->AI()->DoAction(ACTION_START_INTRO);
                return true;
            }

        return false;
    }
};

void AddSC_boss_tyrannus()
{
    new boss_tyrannus;
    new boss_rimefang;
    new spell_tyrannus_overlord_brand;
    new spell_tyrannus_mark_of_rimefang;
    new at_tyrannus_event_starter;
    new npc_tyrannus_icicle;
    new npc_jaina_or_sylvanas_outro;
    new npc_jaina_or_sylvanas_intro;
}
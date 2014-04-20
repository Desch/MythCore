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
#include "serpent_shrine.h"
#include "ScriptedSimpleAI.h"
#include "Spell.h"

#define SAY_INTRO                   -1548042
#define SAY_AGGRO1                  -1548043
#define SAY_AGGRO2                  -1548044
#define SAY_AGGRO3                  -1548045
#define SAY_AGGRO4                  -1548046
#define SAY_PHASE1                  -1548047
#define SAY_PHASE2                  -1548048
#define SAY_PHASE3                  -1548049
#define SAY_BOWSHOT1                -1548050
#define SAY_BOWSHOT2                -1548051
#define SAY_SLAY1                   -1548052
#define SAY_SLAY2                   -1548053
#define SAY_SLAY3                   -1548054
#define SAY_DEATH                   -1548055

#define SPELL_SURGE                 38044
#define SPELL_MULTI_SHOT            38310
#define SPELL_SHOCK_BLAST           38509
#define SPELL_ENTANGLE              38316
#define SPELL_STATIC_CHARGE_TRIGGER 38280
#define SPELL_FORKED_LIGHTNING      40088
#define SPELL_SHOOT                 40873
#define SPELL_POISON_BOLT           40095
#define SPELL_TOXIC_SPORES          38575
#define SPELL_MAGIC_BARRIER         38112

#define MIDDLE_X                    30.134f
#define MIDDLE_Y                    -923.65f
#define MIDDLE_Z                    42.9f

#define SPOREBAT_X                  30.977156f
#define SPOREBAT_Y                  -925.297761f
#define SPOREBAT_Z                  77.176567f
#define SPOREBAT_O                  5.223932f

#define SHIED_GENERATOR_CHANNEL       19870
#define ENCHANTED_ELEMENTAL           21958
#define TAINTED_ELEMENTAL             22009
#define COILFANG_STRIDER              22056
#define COILFANG_ELITE                22055
#define TOXIC_SPOREBAT                22140
#define TOXIC_SPORES_TRIGGER          22207

#define TEXT_NOT_INITIALIZED          "Instance script not initialized"
#define TEXT_ALREADY_DEACTIVATED      "Already deactivated"

float ElementPos[8][4] =
{
    {8.3f, -835.3f, 21.9f, 5.0f},
    {53.4f, -835.3f, 21.9f, 4.5f},
    {96.0f, -861.9f, 21.8f, 4.0f},
    {96.0f, -986.4f, 21.4f, 2.5f},
    {54.4f, -1010.6f, 22, 1.8f},
    {9.8f, -1012, 21.7f, 1.4f},
    {-35.0f, -987.6f, 21.5f, 0.8f},
    {-58.9f, -901.6f, 21.5f, 6.0f}
};

float ElementWPPos[8][3] =
{
    {71.700752f, -883.905884f, 41.097168f},
    {45.039848f, -868.022827f, 41.097015f},
    {14.585141f, -867.894470f, 41.097061f},
    {-25.415508f, -906.737732f, 41.097061f},
    {-11.801594f, -963.405884f, 41.097067f},
    {14.556657f, -979.051514f, 41.097137f},
    {43.466549f, -979.406677f, 41.097027f},
    {69.945908f, -964.663940f, 41.097054f}
};

float SporebatWPPos[8][3] =
{
    {31.6f, -896.3f, 59.1f},
    {9.1f,  -913.9f, 56.0f},
    {5.2f,  -934.4f, 52.4f},
    {20.7f, -946.9f, 49.7f},
    {41.0f, -941.9f, 51.0f},
    {47.7f, -927.3f, 55.0f},
    {42.2f, -912.4f, 51.7f},
    {27.0f, -905.9f, 50.0f}
};

float CoilfangElitePos[3][4] =
{
    {28.84f, -923.28f, 42.9f, 6.0f},
    {31.183281f, -953.502625f, 41.523602f, 1.640957f},
    {58.895180f, -923.124268f, 41.545307f, 3.152848f}
};

float CoilfangStriderPos[3][4] =
{
    {66.427010f, -948.778503f, 41.262245f, 2.584220f},
    {7.513962f, -959.538208f, 41.300422f, 1.034629f},
    {-12.843201f, -907.798401f, 41.239620f, 6.087094f}
};

float ShieldGeneratorChannelPos[4][4] =
{
    {49.6262f, -902.181f, 43.0975f, 3.95683f},
    {10.988f, -901.616f, 42.5371f, 5.4373f},
    {10.3859f, -944.036f, 42.5446f, 0.779888f},
    {49.3126f, -943.398f, 42.5501f, 2.40174f}
};

//Lady Vashj AI
class boss_lady_vashj : public CreatureScript
{
public:
    boss_lady_vashj() : CreatureScript("boss_lady_vashj") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_lady_vashjAI(pCreature);
    }

    struct boss_lady_vashjAI : public ScriptedAI
    {
        boss_lady_vashjAI (Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
            Intro = false;
            JustCreated = true;
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); //set it only once on Creature create (no need do intro if wiped)
        }

        InstanceScript* pInstance;

        uint64 ShieldGeneratorChannel[4];

        uint32 AggroTimer;
        uint32 ShockBlastTimer;
        uint32 EntangleTimer;
        uint32 StaticChargeTimer;
        uint32 ForkedLightningTimer;
        uint32 CheckTimer;
        uint32 EnchantedElementalTimer;
        uint32 TaintedElementalTimer;
        uint32 CoilfangEliteTimer;
        uint32 CoilfangStriderTimer;
        uint32 SummonSporebatTimer;
        uint32 SummonSporebatStaticTimer;
        uint8 EnchantedElementalPos;
        uint8 Phase;

        bool Entangle;
        bool Intro;
        bool CanAttack;
        bool JustCreated;

        void Reset()
        {
            AggroTimer              = 19000;
            ShockBlastTimer         = 1+rand()%60000;
            EntangleTimer           = 30000;
            StaticChargeTimer       = 10000+rand()%15000;
            ForkedLightningTimer    = 2000;
            CheckTimer              = 15000;
            EnchantedElementalTimer = 5000;
            TaintedElementalTimer   = 50000;
            CoilfangEliteTimer      = 45000+rand()%5000;
            CoilfangStriderTimer    = 60000+rand()%10000;
            SummonSporebatTimer     = 10000;
            SummonSporebatStaticTimer = 30000;
            EnchantedElementalPos   = 0;
            Phase                   = 0;

            Entangle = false;
            if(JustCreated)
            {
                CanAttack = false;
                JustCreated = false;
            } else CanAttack = true;

            for(uint8 i = 0; i < 4; ++i)
                if(Unit* remo = Unit::GetUnit(*me, ShieldGeneratorChannel[i]))
                    remo->setDeathState(JUST_DIED);

            if(pInstance)
                pInstance->SetData(DATA_LADYVASHJEVENT, NOT_STARTED);
            ShieldGeneratorChannel[0] = 0;
            ShieldGeneratorChannel[1] = 0;
            ShieldGeneratorChannel[2] = 0;
            ShieldGeneratorChannel[3] = 0;

            me->SetCorpseDelay(1000*60*60);
        }

        void EventTaintedElementalDeath()
        {
            if(TaintedElementalTimer > 50000)
                TaintedElementalTimer = 50000;
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(RAND(SAY_SLAY1, SAY_SLAY2, SAY_SLAY3), me);
        }

        void JustDied(Unit* /*pVictim*/)
        {
            DoScriptText(SAY_DEATH, me);

            if(pInstance)
                pInstance->SetData(DATA_LADYVASHJEVENT, DONE);
        }

        void StartEvent()
        {
            DoScriptText(RAND(SAY_AGGRO1, SAY_AGGRO2, SAY_AGGRO3, SAY_AGGRO4), me);

            Phase = 1;

            if(pInstance)
                pInstance->SetData(DATA_LADYVASHJEVENT, IN_PROGRESS);
        }

        void EnterCombat(Unit* pWho)
        {
            if(pInstance)
            {
                //remove old tainted cores to prevent cheating in phase 2
                Map* pMap = me->GetMap();
                Map::PlayerList const &PlayerList = pMap->GetPlayers();
                for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if(Player* pPlayer = i->getSource())
                        pPlayer->DestroyItemCount(31088, 1, true);
                }
            }
            StartEvent();//this is EnterCombat(), so were are 100% in combat, start the event

            if(Phase != 2)
                AttackStart(pWho);
        }

        void MoveInLineOfSight(Unit* pWho)
        {
            if(!Intro)
            {
                Intro = true;
                DoScriptText(SAY_INTRO, me);
            }
            if(!CanAttack)
                return;
            if(!pWho || me->getVictim())
                return;

            if(pWho->isTargetableForAttack() && pWho->isInAccessiblePlaceFor(me) && me->IsHostileTo(pWho))
            {
                float attackRadius = me->GetAttackDistance(pWho);
                if(me->IsWithinDistInMap(pWho, attackRadius) && me->GetDistanceZ(pWho) <= CREATURE_Z_ATTACK_RANGE && me->IsWithinLOSInMap(pWho))
                {
                    if(!me->isInCombat())
                        StartEvent();

                    if(Phase != 2)
                        AttackStart(pWho);
                }
            }
        }

        void CastShootOrMultishot()
        {
            switch(urand(0, 1))
            {
                case 0:
                    //Shoot
                    //Used in Phases 1 and 3 after Entangle or while having nobody in melee range. A shot that hits her target for 4097-5543 Physical damage.
                    DoCast(me->getVictim(), SPELL_SHOOT);
                    break;
                case 1:
                    //Multishot
                    //Used in Phases 1 and 3 after Entangle or while having nobody in melee range. A shot that hits 1 person and 4 people around him for 6475-7525 physical damage.
                    DoCast(me->getVictim(), SPELL_MULTI_SHOT);
                    break;
            }

            if(rand()%3)
                DoScriptText(RAND(SAY_BOWSHOT1, SAY_BOWSHOT2), me);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!CanAttack && Intro)
            {
                if(AggroTimer <= diff)
                {
                    CanAttack = true;
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    AggroTimer=19000;
                } else {
                    AggroTimer-=diff;
                    return;
                }
            }
            if(Phase == 2 && !me->getVictim() && me->isInCombat())
            {
                EnterEvadeMode();
                return;
            }
            if(!UpdateVictim())
                return;

            if(Phase == 1 || Phase == 3)
            {
                if(ShockBlastTimer <= diff)
                {
                    //Shock Burst
                    //Randomly used in Phases 1 and 3 on Vashj's target, it's a Shock spell doing 8325-9675 nature damage and stunning the target for 5 seconds, during which she will not attack her target but switch to the next person on the aggro list.
                    DoCast(me->getVictim(), SPELL_SHOCK_BLAST);
                    me->TauntApply(me->getVictim());

                    ShockBlastTimer = 1000+rand()%14000;       //random cooldown
                } else ShockBlastTimer -= diff;

                if(StaticChargeTimer <= diff)
                {

                    Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true);
                    if(target && !target->HasAura(SPELL_STATIC_CHARGE_TRIGGER))
                        DoCast(target, SPELL_STATIC_CHARGE_TRIGGER);

                    StaticChargeTimer = 10000+rand()%20000;
                } else StaticChargeTimer -= diff;

                if(EntangleTimer <= diff)
                {
                    if(!Entangle)
                    {
                        DoCast(me->getVictim(), SPELL_ENTANGLE);
                        Entangle = true;
                        EntangleTimer = 10000;
                    } else {
                        CastShootOrMultishot();
                        Entangle = false;
                        EntangleTimer = 20000+rand()%5000;
                    }
                } else EntangleTimer -= diff;

                if(Phase == 1)
                {
                    if(HealthBelowPct(70))
                    {
                        Phase = 2;

                        me->GetMotionMaster()->Clear();
                        DoTeleportTo(MIDDLE_X, MIDDLE_Y, MIDDLE_Z);

                        for(uint8 i = 0; i < 4; ++i)
                            if(Creature* pCreature = me->SummonCreature(SHIED_GENERATOR_CHANNEL, ShieldGeneratorChannelPos[i][0],  ShieldGeneratorChannelPos[i][1],  ShieldGeneratorChannelPos[i][2],  ShieldGeneratorChannelPos[i][3], TEMPSUMMON_CORPSE_DESPAWN, 0))
                                ShieldGeneratorChannel[i] = pCreature->GetGUID();

                        DoScriptText(SAY_PHASE2, me);
                    }
                } else {
                    if(SummonSporebatTimer <= diff)
                    {
                        if(Creature* pSporebat = me->SummonCreature(TOXIC_SPOREBAT, SPOREBAT_X, SPOREBAT_Y, SPOREBAT_Z, SPOREBAT_O, TEMPSUMMON_CORPSE_DESPAWN, 0))
                        {
                            if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                pSporebat->AI()->AttackStart(pTarget);
                        }

                        if(SummonSporebatStaticTimer > 1000)
                            SummonSporebatStaticTimer -= 1000;

                        SummonSporebatTimer = SummonSporebatStaticTimer;

                        if(SummonSporebatTimer < 5000)
                            SummonSporebatTimer = 5000;

                    } else SummonSporebatTimer -= diff;
                }

                DoMeleeAttackIfReady();

                if(CheckTimer <= diff)
                {
                    bool InMeleeRange = false;
                    std::list<HostileReference *> t_list = me->getThreatManager().getThreatList();
                    for(std::list<HostileReference *>::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                    {
                        Unit* target = Unit::GetUnit(*me, (*itr)->getUnitGuid());
                        if(target && target->IsWithinDistInMap(me, 5))
                        {
                            InMeleeRange = true;
                            break;
                        }
                    }

                    if(!InMeleeRange)
                        CastShootOrMultishot();

                    CheckTimer = 5000;
                } else CheckTimer -= diff;
            } else {
                if(ForkedLightningTimer <= diff)
                {
                    Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0);
                    if(!target)
                        target = me->getVictim();

                    DoCast(target, SPELL_FORKED_LIGHTNING);

                    ForkedLightningTimer = 2000+rand() % 6000;
                } else ForkedLightningTimer -= diff;

                if(EnchantedElementalTimer <= diff)
                {
                    me->SummonCreature(ENCHANTED_ELEMENTAL, ElementPos[EnchantedElementalPos][0], ElementPos[EnchantedElementalPos][1], ElementPos[EnchantedElementalPos][2], ElementPos[EnchantedElementalPos][3], TEMPSUMMON_CORPSE_DESPAWN, 0);

                    if(EnchantedElementalPos == 7)
                        EnchantedElementalPos = 0;
                    else
                        ++EnchantedElementalPos;

                    EnchantedElementalTimer = 10000+rand()%5000;
                } else EnchantedElementalTimer -= diff;

                if(TaintedElementalTimer <= diff)
                {
                    uint32 pos = rand()%8;
                    me->SummonCreature(TAINTED_ELEMENTAL, ElementPos[pos][0], ElementPos[pos][1], ElementPos[pos][2], ElementPos[pos][3], TEMPSUMMON_DEAD_DESPAWN, 0);

                    TaintedElementalTimer = 120000;
                } else TaintedElementalTimer -= diff;

                if(CoilfangEliteTimer <= diff)
                {
                    uint32 pos = rand()%3;
                    if(Creature* CoilfangElite = me->SummonCreature(COILFANG_ELITE, CoilfangElitePos[pos][0], CoilfangElitePos[pos][1], CoilfangElitePos[pos][2], CoilfangElitePos[pos][3], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000))
                    {
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            CoilfangElite->AI()->AttackStart(target);
                        else if(me->getVictim())
                            CoilfangElite->AI()->AttackStart(me->getVictim());
                    }
                    CoilfangEliteTimer = 45000+rand()%5000;
                } else CoilfangEliteTimer -= diff;

                if(CoilfangStriderTimer <= diff)
                {
                    uint32 pos = rand()%3;
                    if(Creature* CoilfangStrider = me->SummonCreature(COILFANG_STRIDER, CoilfangStriderPos[pos][0], CoilfangStriderPos[pos][1], CoilfangStriderPos[pos][2], CoilfangStriderPos[pos][3], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000))
                    {
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            CoilfangStrider->AI()->AttackStart(target);
                        else if(me->getVictim())
                            CoilfangStrider->AI()->AttackStart(me->getVictim());
                    }
                    CoilfangStriderTimer = 60000+rand()%10000;
                } else CoilfangStriderTimer -= diff;

                if(CheckTimer <= diff)
                {
                    if(pInstance && pInstance->GetData(DATA_CANSTARTPHASE3))
                    {
                        me->SetHealth(me->CountPctFromMaxHealth(50));
                        me->RemoveAurasDueToSpell(SPELL_MAGIC_BARRIER);
                        DoScriptText(SAY_PHASE3, me);

                        Phase = 3;
                        me->GetMotionMaster()->MoveChase(me->getVictim());
                    }
                    CheckTimer = 1000;
                } else CheckTimer -= diff;
            }
        }
    };
};

class mob_enchanted_elemental : public CreatureScript
{
public:
    mob_enchanted_elemental() : CreatureScript("mob_enchanted_elemental") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_enchanted_elementalAI(pCreature);
    }

    struct mob_enchanted_elementalAI : public ScriptedAI
    {
        mob_enchanted_elementalAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        uint32 move;
        uint32 phase;
        float x, y, z;

        uint64 VashjGUID;

        void Reset()
        {
            me->SetSpeed(MOVE_WALK, 0.6f);//walk
            me->SetSpeed(MOVE_RUN, 0.6f);//run
            move = 0;
            phase = 1;

            VashjGUID = 0;

            x = ElementWPPos[0][0];
            y = ElementWPPos[0][1];
            z = ElementWPPos[0][2];

            for(uint32 i = 1; i < 8; ++i)
            {
                if(me->GetDistance(ElementWPPos[i][0], ElementWPPos[i][1], ElementWPPos[i][2]) < me->GetDistance(x, y, z))
                {
                    x = ElementWPPos[i][0];
                    y = ElementWPPos[i][1];
                    z = ElementWPPos[i][2];
                }
            }

            if(pInstance)
                VashjGUID = pInstance->GetData64(DATA_LADYVASHJ);
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void MoveInLineOfSight(Unit* /*pWho*/) { }

        void UpdateAI(const uint32 diff)
        {
            if(!pInstance)
                return;

            if(!VashjGUID)
                return;

            if(move <= diff)
            {
                me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
                if(phase == 1 && x && y && z)
                {
                    me->GetMotionMaster()->MovePoint(0, x, y, z);
                    phase = 2;
                }
                if(phase == 2)
                {
                    me->GetMotionMaster()->MovePoint(0, MIDDLE_X, MIDDLE_Y, MIDDLE_Z);
                    phase = 3;
                }
                if(phase == 3)
                {
                    me->GetMotionMaster()->MovePoint(0, MIDDLE_X, MIDDLE_Y, MIDDLE_Z);
                    if(me->IsWithinDist3d(MIDDLE_X, MIDDLE_Y, MIDDLE_Z, 3))
                        DoCast(me, SPELL_SURGE);
                }
                if(Creature* pVashj = Unit::GetCreature(*me, VashjGUID))
                {
                    if(!pVashj->isInCombat() || CAST_AI(boss_lady_vashj::boss_lady_vashjAI, pVashj->AI())->Phase != 2 || pVashj->isDead())
                        me->Kill(me);
                }
                move = 1000;
            } else move -= diff;
        }
    };
};

class mob_tainted_elemental : public CreatureScript
{
public:
    mob_tainted_elemental() : CreatureScript("mob_tainted_elemental") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_tainted_elementalAI(pCreature);
    }

    struct mob_tainted_elementalAI : public ScriptedAI
    {
        mob_tainted_elementalAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;

        uint32 PoisonBolt_Timer;
        uint32 Despawn_Timer;

        void Reset()
        {
            PoisonBolt_Timer = 5000+rand()%5000;
            Despawn_Timer = 30000;
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(!pInstance)
                return;
            Creature* pVashj = Unit::GetCreature((*me), pInstance->GetData64(DATA_LADYVASHJ));
                CAST_AI(boss_lady_vashj::boss_lady_vashjAI, pVashj->AI())->EventTaintedElementalDeath();
        }

        void EnterCombat(Unit* who)
        {
            me->AddThreat(who, 0.1f);
        }

        void UpdateAI(const uint32 diff)
        {
            if(PoisonBolt_Timer <= diff)
            {
                Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0);

                if(target && target->IsWithinDistInMap(me, 30))
                    DoCast(target, SPELL_POISON_BOLT);

                PoisonBolt_Timer = 5000+rand()%5000;
            } else PoisonBolt_Timer -= diff;

            if(Despawn_Timer <= diff)
            {
                me->setDeathState(DEAD);

                Despawn_Timer = 1000;
            } else Despawn_Timer -= diff;
        }
    };
};

class mob_toxic_sporebat : public CreatureScript
{
public:
    mob_toxic_sporebat() : CreatureScript("mob_toxic_sporebat") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_toxic_sporebatAI(pCreature);
    }

    struct mob_toxic_sporebatAI : public ScriptedAI
    {
        mob_toxic_sporebatAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
            EnterEvadeMode();
        }

        InstanceScript* pInstance;

        uint32 movement_timer;
        uint32 ToxicSpore_Timer;
        uint32 bolt_timer;
        uint32 Check_Timer;

        void Reset()
        {
            me->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
            me->setFaction(14);
            movement_timer      = 0;
            ToxicSpore_Timer    = 5000;
            bolt_timer          = 5500;
            Check_Timer         = 1000;
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void MoveInLineOfSight(Unit* /*pWho*/) { }

        void MovementInform(uint32 type, uint32 id)
        {
            if(type != POINT_MOTION_TYPE)
                return;

            if(id == 1)
                movement_timer = 0;
        }

        void UpdateAI (const uint32 diff)
        {
            if(movement_timer <= diff)
            {
                uint32 rndpos = rand()%8;
                me->GetMotionMaster()->MovePoint(1, SporebatWPPos[rndpos][0], SporebatWPPos[rndpos][1], SporebatWPPos[rndpos][2]);
                movement_timer = 6000;
            } else movement_timer -= diff;

            if(bolt_timer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    if(Creature* trig = me->SummonCreature(TOXIC_SPORES_TRIGGER, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 30000))
                    {
                        trig->setFaction(14);
                        trig->CastSpell(trig, SPELL_TOXIC_SPORES, true);
                    }
                }
                bolt_timer = 10000+rand()%5000;
            } else bolt_timer -= diff;

            if(Check_Timer <= diff)
            {
                if(pInstance)
                {
                    Unit* Vashj = Unit::GetUnit((*me), pInstance->GetData64(DATA_LADYVASHJ));
                    if(!Vashj || (Vashj && !Vashj->isAlive()) || (Vashj && CAST_AI(boss_lady_vashj::boss_lady_vashjAI, CAST_CRE(Vashj)->AI())->Phase != 3))
                    {
                        me->setDeathState(DEAD);
                        me->RemoveCorpse();
                        me->setFaction(35);
                    }
                }

                Check_Timer = 1000;
            } else Check_Timer -= diff;
        }
    };
};

class mob_coilfang_elite : public CreatureScript
{
public:
    mob_coilfang_elite() : CreatureScript("mob_coilfang_elite") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        SimpleAI* ai = new SimpleAI(pCreature);

        ai->Spell[0].Enabled            = true;
        ai->Spell[0].Spell_Id           = 31345;
        ai->Spell[0].Cooldown           = 15000;
        ai->Spell[0].CooldownRandomAddition = 5000;
        ai->Spell[0].First_Cast         = 5000;
        ai->Spell[0].Cast_Target_Type   = CAST_HOSTILE_RANDOM;

        ai->EnterEvadeMode();

        return ai;
    }
};

class mob_coilfang_strider : public CreatureScript
{
public:
    mob_coilfang_strider() : CreatureScript("mob_coilfang_strider") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        SimpleAI* ai = new SimpleAI(pCreature);

        ai->Spell[0].Enabled                = true;
        ai->Spell[0].Spell_Id               = 41374;
        ai->Spell[0].Cooldown               = 30000;
        ai->Spell[0].CooldownRandomAddition = 10000;
        ai->Spell[0].First_Cast             = 8000;
        ai->Spell[0].Cast_Target_Type       = CAST_HOSTILE_TARGET;

        ai->EnterEvadeMode();

        return ai;
    }
};

class mob_shield_generator_channel : public CreatureScript
{
public:
    mob_shield_generator_channel() : CreatureScript("mob_shield_generator_channel") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_shield_generator_channelAI(pCreature);
    }

    struct mob_shield_generator_channelAI : public ScriptedAI
    {
        mob_shield_generator_channelAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        uint32 Check_Timer;
        bool Casted;
        void Reset()
        {
            Check_Timer = 0;
            Casted      = false;
            me->SetDisplayId(11686);  //invisible

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void MoveInLineOfSight(Unit* /*pWho*/) { }

        void UpdateAI (const uint32 diff)
        {
            if(!pInstance)
                return;

            if(Check_Timer <= diff)
            {
                Unit* Vashj = Unit::GetUnit((*me), pInstance->GetData64(DATA_LADYVASHJ));
                if(Vashj && Vashj->isAlive())
                {
                    if(!Casted || !Vashj->HasAura(SPELL_MAGIC_BARRIER))
                    {
                        DoCast(Vashj, SPELL_MAGIC_BARRIER, true);
                        Casted = true;
                    }
                }
                Check_Timer = 1000;
            } else Check_Timer -= diff;
        }
    };
};

class item_tainted_core : public ItemScript
{
public:
    item_tainted_core() : ItemScript("item_tainted_core") { }

    bool OnUse(Player* pPlayer, Item* /*_Item*/, SpellCastTargets const& targets)
    {
        InstanceScript* pInstance = pPlayer->GetInstanceScript();

        if(!pInstance)
        {
            pPlayer->GetSession()->SendNotification(TEXT_NOT_INITIALIZED);
            return true;
        }

        Creature* Vashj = Unit::GetCreature((*pPlayer), pInstance->GetData64(DATA_LADYVASHJ));
        if(Vashj && (CAST_AI(boss_lady_vashj::boss_lady_vashjAI, Vashj->AI())->Phase == 2))
        {
            if(GameObject* pGo = targets.GetGOTarget())
            {
                uint32 identifier;
                uint8 channelIdentifier;
                switch(pGo->GetEntry())
                {
                    case 185052:    identifier = DATA_SHIELDGENERATOR1; channelIdentifier = 0;  break;
                    case 185053:    identifier = DATA_SHIELDGENERATOR2; channelIdentifier = 1;  break;
                    case 185051:    identifier = DATA_SHIELDGENERATOR3; channelIdentifier = 2;  break;
                    case 185054:    identifier = DATA_SHIELDGENERATOR4; channelIdentifier = 3;  break;
                    default:
                        return true;
                }

                if(pInstance->GetData(identifier))
                {
                    pPlayer->GetSession()->SendNotification(TEXT_ALREADY_DEACTIVATED);
                    return true;
                }

                if(Unit* Channel = Unit::GetCreature(*Vashj, CAST_AI(boss_lady_vashj::boss_lady_vashjAI, Vashj->AI())->ShieldGeneratorChannel[channelIdentifier]))
                    Channel->setDeathState(JUST_DIED);

                pInstance->SetData(identifier, 1);
                pPlayer->DestroyItemCount(31088, 1, true);
                return true;
            } else if(targets.GetUnitTarget()->GetTypeId() == TYPEID_UNIT)
                return false;
            else if(targets.GetUnitTarget()->GetTypeId() == TYPEID_PLAYER)
            {
                pPlayer->DestroyItemCount(31088, 1, true);
                pPlayer->CastSpell(targets.GetUnitTarget(), 38134, true);
                return true;
            }
        }
        return true;
    }
};

void AddSC_boss_lady_vashj()
{
    new boss_lady_vashj;
    new mob_enchanted_elemental;
    new mob_tainted_elemental;
    new mob_toxic_sporebat;
    new mob_coilfang_elite;
    new mob_coilfang_strider;
    new mob_shield_generator_channel;
    new item_tainted_core;
}
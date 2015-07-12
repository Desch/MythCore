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
#include "ScriptedEscortAI.h"

enum Spells
{
    SPELL_RAT               = 50894,
    SPELL_SUMMON_GORGED     = 50928
};

class npc_lurking_basilisk : public CreatureScript
{
public:
    npc_lurking_basilisk() : CreatureScript("npc_lurking_basilisk") { }

    struct npc_lurking_basiliskAI : public ScriptedAI
    {
        npc_lurking_basiliskAI(Creature* pCreature): ScriptedAI(pCreature) { }

        void SpellHit(Unit* /*pCaster*/, const SpellEntry* pSpell)
        {
            if(pSpell->Id == SPELL_RAT)
                if(Aura* auraRat = me->GetAura(SPELL_RAT))
                {
                    if(auraRat->GetStackAmount() == 5)
                    {
                        me->CastSpell(me, SPELL_SUMMON_GORGED, true);
                        me->ForcedDespawn();
                    }
                }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_lurking_basiliskAI(pCreature);
    }
};

enum eDrakuruShackles
{
    SPELL_LEFT_CHAIN           = 59951,
    SPELL_RIGHT_CHAIN          = 59952,
    SPELL_UNLOCK_SHACKLE       = 55083,
    SPELL_FREE_RAGECLAW        = 55223,

    NPC_RAGECLAW               = 29686,
    QUEST_TROLLS_IS_GONE_CRAZY = 12861,
};

class npc_drakuru_shackles : public CreatureScript
{
public:
    npc_drakuru_shackles() : CreatureScript("npc_drakuru_shackles") { }

    struct npc_drakuru_shacklesAI : public ScriptedAI
    {
        npc_drakuru_shacklesAI(Creature* pCreature): ScriptedAI(pCreature) { }

        uint64 RageclawGUID;

        void Reset()
        {
            RageclawGUID = 0;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            float x, y, z;
            me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 0.1f);

            if(Unit* summon = me->SummonCreature(NPC_RAGECLAW, x, y, z,
                0, TEMPSUMMON_DEAD_DESPAWN, 1000))
            {
                RageclawGUID = summon->GetGUID();
                LockRageclaw();
            }
        }

        void LockRageclaw()
        {
            Unit* Rageclaw = Unit::GetCreature(*me, RageclawGUID);
            // pointer check not needed
            me->SetInFront(Rageclaw);
            Rageclaw->SetInFront(me);

            DoCast(Rageclaw, SPELL_LEFT_CHAIN, true);
            DoCast(Rageclaw, SPELL_RIGHT_CHAIN, true);
        }

        void UnlockRageclaw(Unit* who)
        {
            if(!who)
                return;

            Creature* Rageclaw = Unit::GetCreature(*me, RageclawGUID);
            // pointer check not needed
            DoCast(Rageclaw, SPELL_FREE_RAGECLAW, true);

            me->setDeathState(DEAD);
        }

        void JustDied(Unit* pKiller)
        {
            if(!pKiller)
                return;
            if(pKiller->hasQuest(12713))
                me->SummonGameObject(191458, 6174.500977f, -2018.233398f, 590.878235f, 0, 0, 0, 0, 0, 1);
        }

        void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
        {
            if(pSpell->Id == SPELL_UNLOCK_SHACKLE)
            {
                if(pCaster->ToPlayer()->GetQuestStatus(QUEST_TROLLS_IS_GONE_CRAZY) == QUEST_STATUS_INCOMPLETE)
                {
                    if(Creature* pRageclaw = Unit::GetCreature(*me, RageclawGUID))
                    {
                        UnlockRageclaw(pCaster);
                        pCaster->ToPlayer()->KilledMonster(pRageclaw->GetCreatureInfo(), RageclawGUID);
                        me->DisappearAndDie();
                    }
                    else
                        me->setDeathState(JUST_DIED);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_drakuru_shacklesAI(pCreature);
    }
};

enum eRageclaw
{
    SPELL_UNSHACKLED           = 55085,
    SPELL_KNEEL                = 39656
};

const char * SAY_RAGECLAW_1 =      "Je chie sur vous, trolls !";
const char * SAY_RAGECLAW_2 =      "ARRRROOOOGGGGAAAA!";
const char * SAY_RAGECLAW_3 =      "Pas plus Monsieur gentil varleu!";

#define SAY_RAGECLAW RAND(SAY_RAGECLAW_1, SAY_RAGECLAW_2, SAY_RAGECLAW_3)

class npc_captured_rageclaw : public CreatureScript
{
public:
    npc_captured_rageclaw() : CreatureScript("npc_captured_rageclaw") { }

    struct npc_captured_rageclawAI : public ScriptedAI
    {
        npc_captured_rageclawAI(Creature* pCreature): ScriptedAI(pCreature) { }

        uint32 DespawnTimer;
        bool Despawn;

        void Reset()
        {
            Despawn = false;
            DespawnTimer = 0;
            me->setFaction(35);
            DoCast(me, SPELL_KNEEL, true); // Little Hack for kneel - Thanks Illy :P
        }

        void MoveInLineOfSight(Unit* /*pWho*/){ }

        void SpellHit(Unit* /*pCaster*/, const SpellEntry* pSpell)
        {
            if(pSpell->Id == SPELL_FREE_RAGECLAW)
            {
                me->RemoveAurasDueToSpell(SPELL_LEFT_CHAIN);

                me->RemoveAurasDueToSpell(SPELL_RIGHT_CHAIN);

                me->RemoveAurasDueToSpell(SPELL_KNEEL);

                me->setFaction(me->GetCreatureInfo()->faction_H);

                DoCast(me, SPELL_UNSHACKLED, true);
                me->MonsterSay(SAY_RAGECLAW, LANG_UNIVERSAL, 0);
                me->GetMotionMaster()->MoveRandom(10);

                DespawnTimer = 10000;
                Despawn = true;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(UpdateVictim())
            {
                DoMeleeAttackIfReady();
                return;
            }

            if(!Despawn)
                return;

            if(DespawnTimer <= diff)
                me->DisappearAndDie();
            else DespawnTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_captured_rageclawAI(pCreature);
    }
};

#define    GOSSIP_ITEM_G "Je suis prêt, Gymer. Allons-y !"

enum eGymer
{
    QUEST_STORM_KING_VENGEANCE    = 12919,
    SPELL_GYMER                   = 55568
};

class npc_gymer : public CreatureScript
{
public:
    npc_gymer() : CreatureScript("npc_gymer") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        if(player->GetQuestStatus(QUEST_STORM_KING_VENGEANCE) == QUEST_STATUS_INCOMPLETE)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_G, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(13640, creature->GetGUID());
        }

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*pCreature*/, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        if(uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            player->CLOSE_GOSSIP_MENU();
            player->CastSpell(player, SPELL_GYMER, true);
        }

        return true;
    }
};

enum eGurgthock
{
    QUEST_AMPHITHEATER_ANGUISH_TUSKARRMAGEDDON    = 12935,
    QUEST_AMPHITHEATER_ANGUISH_KORRAK_BLOODRAGER  = 12936,
    QUEST_AMPHITHEATER_ANGUISH_YGGDRAS_2          = 12954,
    QUEST_AMPHITHEATER_ANGUISH_YGGDRAS_1          = 12932,
    QUEST_AMPHITHEATER_ANGUISH_MAGNATAUR          = 12933,
    QUEST_AMPHITHEATER_ANGUISH_FROM_BEYOND        = 12934,

    NPC_ORINOKO_TUSKBREAKER                       = 30020,
    NPC_KORRAK_BLOODRAGER                         = 30023,
    NPC_YGGDRAS                                   = 30014,
    NPC_STINKBEARD                                = 30017,
    NPC_AZ_BARIN                                  = 30026, // air
    NPC_DUKE_SINGEN                               = 30019, // fire
    NPC_ERATHIUS                                  = 30025, // earth
    NPC_GARGORAL                                  = 30024, // water
    NPC_FIEND_WATER                               = 30044,
    NPC_FIEND_AIR                                 = 30045,
    NPC_FIEND_FIRE                                = 30042,
    NPC_FIEND_EARTH                               = 30043,

    SAY_QUEST_ACCEPT_TUSKARRMAGEDON               = -1571031,
    SAY_QUEST_ACCEPT_KORRAK_1                     = -1571033,
    SAY_QUEST_ACCEPT_KORRAK_2                     = -1571034,
    SAY_QUEST_ACCEPT_MAGNATAUR                    = -1571035,

    EMOTE_YGGDRAS_SPAWN                           = -1571039,
    SAY_STINKBEARD_SPAWN                          = -1571040,
    SAY_GURGTHOCK_ELEMENTAL_SPAWN                 = -1571041,

    SPELL_CRASHING_WAVE                           = 55909, // water
    SPELL_SHOCKWAVE                               = 55918, // earth
    SPELL_BLAST_OF_AIR                            = 55912, // air
    SPELL_MAGMA_WAVE                              = 55916, // fire

    SPELL_ORB_OF_WATER                             = 55888, // fiend of water spell
    SPELL_ORB_OF_STORMS                            = 55882, // fiend of air spell
    SPELL_BOULDER                                  = 55886, // fiend of earth spell
    SPELL_ORB_OF_FLAME                             = 55872, // fiend of fire spell
};

struct BossAndAdd
{
    uint32 uiBoss;
    uint32 uiAdd;
    uint32 uiSpell;
    uint32 uiAddSpell;
};

static BossAndAdd Boss[]=
{
    {NPC_GARGORAL, NPC_FIEND_WATER, SPELL_CRASHING_WAVE, SPELL_ORB_OF_WATER},
    {NPC_AZ_BARIN, NPC_FIEND_AIR, SPELL_BLAST_OF_AIR, SPELL_ORB_OF_STORMS},
    {NPC_DUKE_SINGEN, NPC_FIEND_FIRE, SPELL_MAGMA_WAVE, SPELL_ORB_OF_FLAME},
    {NPC_ERATHIUS, NPC_FIEND_EARTH, SPELL_SHOCKWAVE, SPELL_BOULDER},
};

const Position SpawnPosition[] =
{
    {5754.692f, -2939.46f, 286.276123f, 5.156380f}, // stinkbeard || orinoko || korrak
    {5762.054199f, -2954.385010f, 273.826955f, 5.108289f},  //yggdras
    {5776.855f, -2989.77979f, 272.96814f, 5.194f} // elementals
};

const Position AddSpawnPosition[] =
{
    {5722.487f, -3010.75f, 312.751648f, 0.478f}, // caster location
    {5724.983f, -2969.89551f, 286.359619f, 0.478f},
    {5733.76025f, -3000.34644f, 286.359619f, 0.478f},
    {5739.8125f, -2981.524f, 290.7671f, 0.478f}, // caster location
    {5742.101f, -2950.75586f, 286.2643f, 5.21f},
    {5743.305f, -3011.29736f, 290.7671f, 0.478f}, // caster location
    {5744.417f, -3025.528f, 286.35965f, 0.478f},
    {5763.189f, -3029.67529f, 290.7671f, 0.478f},
    {5769.401f, -2935.121f, 286.335754f, 5.21f},
    {5793.061f, -2934.593f, 286.359619f, 3.53f},
    {5797.32129f, -2955.26855f, 290.7671f, 3.53f}, // caster location
    {5813.94531f, -2956.74683f, 286.359619f, 3.53f},
    {5816.85547f, -2974.476f, 290.7671f, 3.53f}, // caster location
    {5820.30859f, -3002.83716f, 290.7671f, 3.53f}, // caster location
    {5828.50244f, -2981.737f, 286.359619f, 3.53f},
    {5828.899f, -2960.15479f, 312.751648f, 3.53f}, // caster location
};

class npc_gurgthock : public CreatureScript
{
public:
    npc_gurgthock() : CreatureScript("npc_gurgthock") { }

    struct npc_gurgthockAI : public ScriptedAI
    {
        npc_gurgthockAI(Creature* pCreature): ScriptedAI(pCreature) { }

        uint64 SummonGUID;
        uint64 uiPlayerGUID;

        uint32 uiTimer;
        uint32 uiPhase;
        uint32 uiRemoveFlagTimer;
        uint32 uiQuest;
        uint8 uiBossRandom;

        bool bRemoveFlag;

        void Reset()
        {
            SummonGUID = 0;
            uiPlayerGUID = 0;

            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            uiTimer = 0;
            uiPhase = 0;
            uiQuest = 0;
            uiRemoveFlagTimer = 5000;

            uiBossRandom = 0;

            bRemoveFlag = false;
        }

        void SetGUID(const uint64 &guid, int32 /*id*/)
        {
            uiPlayerGUID = guid;
        }

        void SetData(uint32 uiId, uint32 uiValue)
        {
            bRemoveFlag = true;
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);

            switch(uiId)
            {
                case 1:
                    switch(uiValue)
                    {
                        case QUEST_AMPHITHEATER_ANGUISH_TUSKARRMAGEDDON:
                            DoScriptText(SAY_QUEST_ACCEPT_TUSKARRMAGEDON, me);
                            uiPhase = 1;
                            uiTimer = 4000;
                            break;
                        case QUEST_AMPHITHEATER_ANGUISH_KORRAK_BLOODRAGER:
                            DoScriptText(SAY_QUEST_ACCEPT_KORRAK_1, me);
                            uiPhase = 3;
                            uiTimer = 3000;
                            break;
                        case QUEST_AMPHITHEATER_ANGUISH_YGGDRAS_2:
                        case QUEST_AMPHITHEATER_ANGUISH_YGGDRAS_1:
                            uiPhase = 6;
                            uiTimer = 3000;
                            break;
                        case QUEST_AMPHITHEATER_ANGUISH_MAGNATAUR:
                            uiTimer = 5000;
                            uiPhase = 7;
                            break;
                        case QUEST_AMPHITHEATER_ANGUISH_FROM_BEYOND:
                            uiTimer = 2000;
                            uiPhase = 12;
                            break;
                    }
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            ScriptedAI::UpdateAI(diff);

            if(bRemoveFlag)
            {
                if(uiRemoveFlagTimer <= diff)
                {
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    bRemoveFlag = false;

                    uiRemoveFlagTimer = 10000;
                } else uiRemoveFlagTimer -= diff;
            }

            if(uiPhase)
            {
                Player* player = me->GetPlayer(*me, uiPlayerGUID);

                if(uiTimer <= diff)
                {
                    switch(uiPhase)
                    {
                        case 1:
                            if(Creature* summon = me->SummonCreature(NPC_ORINOKO_TUSKBREAKER, SpawnPosition[0], TEMPSUMMON_CORPSE_DESPAWN, 1000))
                                SummonGUID = summon->GetGUID();
                            uiPhase = 2;
                            uiTimer = 4000;
                            break;
                         case 2:
                            if(Creature* summon = Unit::GetCreature(*me, SummonGUID))
                                summon->GetMotionMaster()->MoveJump(5776.319824f, -2981.005371f, 273.100037f, 10.0f, 20.0f);
                            uiPhase = 0;
                            SummonGUID = 0;
                            break;
                        case 3:
                            DoScriptText(SAY_QUEST_ACCEPT_KORRAK_2, me);
                            uiTimer = 3000;
                            uiPhase = 4;
                            break;
                        case 4:
                            if(Creature* summon = me->SummonCreature(NPC_KORRAK_BLOODRAGER, SpawnPosition[0], TEMPSUMMON_CORPSE_DESPAWN, 1000))
                                SummonGUID = summon->GetGUID();
                            uiTimer = 3000;
                            uiPhase = 0;
                            break;
                        case 6:
                            {
                                if(!player)
                                    return;

                                std::string sText = ("Le grand amphithéâtre de l'Angoisse attend, " + std::string(player->GetName()) + ". Rappelez-vous, qu'une fois la bataille commencée, vous devez rester dans la zone. Vaincre ou mourir !");

                                me->MonsterSay(sText.c_str(), LANG_UNIVERSAL, 0);
                                uiTimer = 5000;
                                uiPhase = 9;
                            }
                            break;
                        case 7:
                            {
                               if(!player)
                                   return;

                                std::string sText = ("Préparez-vous à vous démarquer, " + std::string(player->GetName()) + "! Entrez dans l'amphithéâtre et nous serons prêts! Rappelez-vous, vous et votre adversaire devez rester dans l'arène à tout moment, ou vous serez disqualifié !");
                                me->MonsterSay(sText.c_str(), LANG_UNIVERSAL, 0);
                                uiTimer = 3000;
                                uiPhase = 8;
                            }
                            break;
                        case 8:
                            DoScriptText(SAY_QUEST_ACCEPT_MAGNATAUR, me);
                            uiTimer = 5000;
                            uiPhase = 11;
                            break;
                        case 9:
                            {
                                if(!player)
                                    return;

                                std::string sText = ("Nous voici ici une fois de plus, mesdames et messieurs. La lutte épique entre la vie et la mort dans l'amphithéâtre de l'Angoisse! Pour ce matches, nous avons " + std::string(player->GetName()) + " contre le hulking jormungar, Yg... Yggd? Yggdoze? Qui vient avec ce noms ?! " + std::string(player->GetName()) + " contre gros ver!");
                                me->MonsterYell(sText.c_str(), LANG_UNIVERSAL, 0);
                                uiTimer = 10000;
                                uiPhase = 10;
                            }
                            break;
                        case 10:
                            me->SummonCreature(NPC_YGGDRAS, SpawnPosition[1], TEMPSUMMON_CORPSE_DESPAWN, 1000);
                            DoScriptText(EMOTE_YGGDRAS_SPAWN, me);
                            uiPhase = 0;
                            break;
                        case 11:
                            if(Creature* creature = me->SummonCreature(NPC_STINKBEARD, SpawnPosition[0], TEMPSUMMON_CORPSE_DESPAWN, 1000))
                                DoScriptText(SAY_STINKBEARD_SPAWN, creature);
                            uiPhase = 0;
                            break;
                        case 12:
                        {
                            if(!player)
                                return;

                            std::string sText = ("Préparez-vous à vous démarquer, " + std::string(player->GetName()) + "! Entrez dans l'amphithéâtre et nous serons prêts! Rappelez-vous, vous et votre adversaire devez rester dans l'arène à tout moment, ou vous serez disqualifié !");
                            me->MonsterSay(sText.c_str(), LANG_UNIVERSAL, 0);
                            uiTimer = 5000;
                            uiPhase = 13;
                        }
                        break;
                        case 13:
                            DoScriptText(SAY_GURGTHOCK_ELEMENTAL_SPAWN, me);
                            uiTimer = 3000;
                            uiPhase = 14;
                            break;
                        case 14:
                            uiBossRandom = urand(0, 3);
                            if(Creature* creature = me->SummonCreature(Boss[uiBossRandom].uiBoss, SpawnPosition[2], TEMPSUMMON_CORPSE_DESPAWN, 1000))
                                creature->AI()->SetData(1, uiBossRandom);
                            uiPhase = 0;
                            break;
                    }
                } else uiTimer -= diff;
            }
        }
    };

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* pQuest)
    {
        switch(pQuest->GetQuestId())
        {
            case QUEST_AMPHITHEATER_ANGUISH_TUSKARRMAGEDDON:
                creature->AI()->SetData(1, pQuest->GetQuestId());
                break;
            case QUEST_AMPHITHEATER_ANGUISH_KORRAK_BLOODRAGER:
                creature->AI()->SetData(1, pQuest->GetQuestId());
                break;
            case QUEST_AMPHITHEATER_ANGUISH_YGGDRAS_2:
            case QUEST_AMPHITHEATER_ANGUISH_YGGDRAS_1:
                creature->AI()->SetData(1, pQuest->GetQuestId());
                break;
            case QUEST_AMPHITHEATER_ANGUISH_MAGNATAUR:
                creature->AI()->SetData(1, pQuest->GetQuestId());
                break;
            case QUEST_AMPHITHEATER_ANGUISH_FROM_BEYOND:
                creature->AI()->SetData(1, pQuest->GetQuestId());
                break;
        }

        creature->AI()->SetGUID(player->GetGUID());

        return false;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_gurgthockAI(pCreature);
    }
};

enum eOrinokoTuskbreaker
{
    SPELL_BATTLE_SHOUT      = 32064,
    SPELL_FISHY_SCENT       = 55937,
    SPELL_IMPALE            = 55929,
    SPELL_SUMMON_WHISKER    = 55946,

    NPC_WHISKER             = 30113,
    NPC_HUNGRY_PENGUIN      = 30110,

    SAY_CALL_FOR_HELP       = -1571032
};

class npc_orinoko_tuskbreaker : public CreatureScript
{
public:
    npc_orinoko_tuskbreaker() : CreatureScript("npc_orinoko_tuskbreaker") { }

    struct npc_orinoko_tuskbreakerAI : public ScriptedAI
    {
        npc_orinoko_tuskbreakerAI(Creature* pCreature): ScriptedAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            me->SetReactState(REACT_PASSIVE);
        }

        bool bSummoned;
        bool bBattleShout;
        bool bFishyScent;

        uint32 uiBattleShoutTimer;
        uint32 uiFishyScentTimer;

        uint64 AffectedGUID;
        uint64 uiWhisker;

        void Reset()
        {
            bSummoned           = false;
            bBattleShout        = false;
            bFishyScent         = false;
            uiBattleShoutTimer  = 0;
            uiFishyScentTimer   = 20000;
            uiWhisker           = 0;
            AffectedGUID        = 0;
        }

        void EnterEvadeMode()
        {
            if(Creature* pWhisker = me->GetCreature(*me, uiWhisker))
                pWhisker->RemoveFromWorld();
        }

        void MovementInform(uint32 uiType, uint32 /*uiId*/)
        {
            if(uiType != EFFECT_MOTION_TYPE)
                return;

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            me->SetReactState(REACT_AGGRESSIVE);
            me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
            uiBattleShoutTimer  = 7000;
        }

        void EnterCombat(Unit* who)
        {
            DoCast(who, SPELL_IMPALE);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(!bBattleShout && uiBattleShoutTimer <= diff)
            {
                DoCast(me, SPELL_BATTLE_SHOUT);
                bBattleShout = true;
            } else uiBattleShoutTimer -= diff;

            if(uiFishyScentTimer <= diff)
            {
                if(Unit* pAffected = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    DoCast(pAffected, SPELL_FISHY_SCENT);
                    AffectedGUID = pAffected->GetGUID();
                }
                uiFishyScentTimer = 20000;
            } else uiFishyScentTimer -= diff;

            if(!bSummoned && !HealthAbovePct(50))
            {
                DoScriptText(SAY_CALL_FOR_HELP, me);

                if(Creature* pWhisker = me->SummonCreature(NPC_WHISKER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 0))
                    uiWhisker = pWhisker->GetGUID();
                bSummoned = true;
            }

            DoMeleeAttackIfReady();
        }

        void JustSummoned(Creature* summon)
        {
            switch(summon->GetEntry())
            {
                case NPC_WHISKER:
                    summon->AI()->AttackStart(me->getVictim());
                    break;
                case NPC_HUNGRY_PENGUIN:
                    if(Unit* pAffected = Unit::GetUnit(*me, AffectedGUID))
                    {
                        if(pAffected->isAlive())
                            summon->AI()->AttackStart(pAffected);
                    }
                    break;
            }
        }

        void JustDied(Unit* killer)
        {
            if(uiWhisker)
                if(Creature* pWhisker = me->GetCreature(*me, uiWhisker))
                    pWhisker->RemoveFromWorld();

            if(killer->GetTypeId() == TYPEID_PLAYER)
                killer->GetCharmerOrOwnerPlayerOrPlayerItself()->GroupEventHappens(QUEST_AMPHITHEATER_ANGUISH_TUSKARRMAGEDDON, killer);

        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_orinoko_tuskbreakerAI(pCreature);
    }
};

enum eKorrakBloodrager
{
    SPELL_GROW     = 55948,
    SPELL_CHARGE   = 24193,
    SPELL_UPPERCUT = 30471,
    SPELL_ENRAGE   = 42745
};

class npc_korrak_bloodrager : public CreatureScript
{
public:
    npc_korrak_bloodrager() : CreatureScript("npc_korrak_bloodrager") { }

    struct npc_korrak_bloodragerAI : public npc_escortAI
    {
        npc_korrak_bloodragerAI(Creature* pCreature) : npc_escortAI(pCreature)
        {
            Start(true, true, 0, NULL);
            SetDespawnAtEnd(false);
        }

        uint32 uiChargeTimer;
        uint32 uiUppercutTimer;

        bool bEnrage;

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            me->SetReactState(REACT_PASSIVE);
            uiChargeTimer = 15000;
            uiUppercutTimer = 12000;
            bEnrage = false;
        }

        void WaypointReached(uint32 uiI)
        {
            switch(uiI)
            {
                case 6:
                    me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                    me->SetReactState(REACT_AGGRESSIVE);
                    break;
            }
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoCast(me, SPELL_GROW);
        }

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);

            if(!UpdateVictim())
                return;

            if(uiUppercutTimer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_NEAREST, 0))
                    DoCast(target, SPELL_UPPERCUT);
                uiUppercutTimer = 12000;
            } else uiUppercutTimer -= diff;

            if(uiChargeTimer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 0))
                    DoCast(target, SPELL_CHARGE);
                uiChargeTimer = 15000;
            } else uiChargeTimer -= diff;

            if(!bEnrage && !HealthAbovePct(20))
            {
                DoCast(me, SPELL_ENRAGE);
                bEnrage = true;
            }
            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* pKiller)
        {
            if(Player* pPlayer = pKiller->GetCharmerOrOwnerPlayerOrPlayerItself())
                pPlayer->GroupEventHappens(QUEST_AMPHITHEATER_ANGUISH_KORRAK_BLOODRAGER, pKiller);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_korrak_bloodragerAI(pCreature);
    }
};

enum eYggdras
{
    SPELL_CLEAVE            = 40504,
    SPELL_CORRODE_FLESH     = 57076,
    SPELL_JORMUNGAR_SPAWN   = 55859
};

class npc_yggdras : public CreatureScript
{
public:
    npc_yggdras() : CreatureScript("npc_yggdras") { }

    struct npc_yggdrasAI : public ScriptedAI
    {
        npc_yggdrasAI(Creature* pCreature): ScriptedAI(pCreature) { }

        uint32 uiCleaveTimer;
        uint32 uiCorrodeFleshTimer;

        void Reset()
        {
            uiCleaveTimer = 9000;
            uiCorrodeFleshTimer = 6000;
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(me->getVictim()->GetPositionZ() >= 286.276f)
            {
                std::list<HostileReference *> t_list = me->getThreatManager().getThreatList();
                for(std::list<HostileReference *>::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                {
                    if(Unit* pUnit = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                    {
                        if(pUnit->GetPositionZ() <= 286.276f)
                        {
                            me->getThreatManager().resetAllAggro();
                            me->AddThreat(pUnit, 5.0f);
                            break;
                        }
                        EnterEvadeMode();
                    }
                }
            }

            if(uiCleaveTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CLEAVE);
                uiCleaveTimer = 9000;
            } else uiCleaveTimer -= diff;

            if(uiCorrodeFleshTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CORRODE_FLESH);
                uiCorrodeFleshTimer = 6000;
            } else uiCorrodeFleshTimer -= diff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* killer)
        {
            if(Unit* summoner = me->ToTempSummon()->GetSummoner())
            {
                std::string sText = (std::string(killer->GetName()) + " a vaincu Yg ..  Yggg-vraiment un gros ver !");
                summoner->MonsterYell(sText.c_str(), LANG_UNIVERSAL, 0);
            }

            if(Player* player = killer->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                player->GroupEventHappens(QUEST_AMPHITHEATER_ANGUISH_YGGDRAS_1, killer);
                player->GroupEventHappens(QUEST_AMPHITHEATER_ANGUISH_YGGDRAS_2, killer);
            }

            for(uint8 i = 0; i < 3; ++i)
                DoCast(killer, SPELL_JORMUNGAR_SPAWN, true);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_yggdrasAI(pCreature);
    }
};

enum eStinkbeard
{
    SPELL_ENRAGE_STINKBEARD = 50420,
    SPELL_KNOCK_AWAY        = 31389,
    SPELL_STINKY_BEARD      = 55867,
    SPELL_THUNDERBLADE      = 55866,
    SPELL_THUNDERCLAP       = 15588
};

class npc_stinkbeard : public CreatureScript
{
public:
    npc_stinkbeard() : CreatureScript("npc_stinkbeard") { }

    struct npc_stinkbeardAI : public npc_escortAI
    {
        npc_stinkbeardAI(Creature* pCreature) : npc_escortAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            me->SetReactState(REACT_PASSIVE);
            Start(true, true, 0, NULL);
            SetDespawnAtEnd(false);
        }

        uint32 uiKnockAwayTimer;
        uint32 uiStinkyBeardTimer;

        bool bEnrage;
        bool bThunderClap;

        void Reset()
        {
            me->AddAura(SPELL_THUNDERBLADE, me);
            uiKnockAwayTimer   = 10000;
            uiStinkyBeardTimer = 15000;
            bEnrage = false;
            bThunderClap = false;
        }

        void WaypointReached(uint32 uiI)
        {
            switch(uiI)
            {
                case 7:
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);

            if(!UpdateVictim())
                return;

            if(Unit* victim = me->getVictim())
            {
                if(victim->GetPositionZ() >= 286.276f)
                {
                    std::list<HostileReference *> t_list = me->getThreatManager().getThreatList();
                    for(std::list<HostileReference *>::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                    {
                        if(Unit* pUnit = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                        {
                            if(pUnit->GetPositionZ() <= 286.276f)
                            {
                                me->getThreatManager().resetAllAggro();
                                me->AddThreat(pUnit, 5.0f);
                                break;
                            }
                            EnterEvadeMode();
                        }
                    }
                }
            }

            if(bThunderClap && !HealthAbovePct(10))
            {
                DoCastAOE(SPELL_THUNDERCLAP);
                bThunderClap = true;
            }

            if(uiKnockAwayTimer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    if(target && target->isAlive())
                        DoCast(target, SPELL_KNOCK_AWAY);
                }
                uiKnockAwayTimer = 10000;
            } else uiKnockAwayTimer -= diff;

            if(uiStinkyBeardTimer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    if(target && target->isAlive())
                        DoCast(target, SPELL_STINKY_BEARD);
                }
                uiStinkyBeardTimer = 15000;
            } else uiStinkyBeardTimer -= diff;

            if(!bEnrage && !HealthAbovePct(20))
            {
                DoCast(me, SPELL_ENRAGE_STINKBEARD);
                bEnrage = true;
            }
            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* killer)
        {
            if(Player* player = killer->GetCharmerOrOwnerPlayerOrPlayerItself())
                player->GetCharmerOrOwnerPlayerOrPlayerItself()->GroupEventHappens(QUEST_AMPHITHEATER_ANGUISH_MAGNATAUR, killer);

            std::string sText = ("Et avec autorité, " + std::string(killer->GetName()) + " domine le seigneur magnataur! Le Clan Stinkbeard va être rappelé dans la région des Désolation des dragons!");
            me->MonsterYell(sText.c_str(), LANG_UNIVERSAL, 0);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_stinkbeardAI(pCreature);
    }
};

class npc_elemental_lord : public CreatureScript
{
public:
    npc_elemental_lord() : CreatureScript("npc_elemental_lord") { }

    struct npc_elemental_lordAI : public ScriptedAI
    {
        npc_elemental_lordAI(Creature* pCreature): ScriptedAI(pCreature) { }

        std::list<uint64> SummonList;

        uint32 uiElementalSpellTimer;

        uint8 uiBossRandom;
        uint32 uiSpellEntry;

        bool bAddAttack;

        void Reset()
        {
            uiBossRandom = 0;
            uiSpellEntry = 0;
            uiElementalSpellTimer = urand(5000, 8000);

            bAddAttack = false;
        }

        void SetData(uint32 uiData, uint32 uiValue)
        {
            if(uiData == 1)
            {
                uiBossRandom = uiValue;
                SummonAdds();
            }
        }

        void SummonAdds()
        {
            if(!Boss[uiBossRandom].uiAdd)
                return;

            SummonList.clear();

            for(uint8 uiI = 0; uiI < 16 ; uiI++)
            {
                if(Creature* summon = me->SummonCreature(Boss[uiBossRandom].uiAdd, AddSpawnPosition[uiI]))
                {
                    summon->AI()->SetData(1, uiBossRandom);
                    SummonList.push_back(summon->GetGUID());
                }
            }

        }

        void EnterCombat(Unit* pUnit)
        {
            if(!SummonList.empty())
                for(std::list<uint64>::const_iterator itr = SummonList.begin(); itr != SummonList.end(); ++itr)
                {
                    if(Creature* pTemp = Unit::GetCreature(*me, *itr))
                    {
                        pTemp->m_CombatDistance = 100.0f; // ugly hack? we are not in a instance sorry. :(
                        pTemp->AI()->AttackStart(pUnit);
                    }
                }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(me->getVictim()->GetPositionZ() >= 286.276f)
            {
                std::list<HostileReference *> t_list = me->getThreatManager().getThreatList();
                for(std::list<HostileReference *>::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                {
                    if(Unit* pUnit = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                    {
                        if(pUnit->GetPositionZ() <= 286.276f)
                        {
                            me->getThreatManager().resetAllAggro();
                            me->AddThreat(pUnit, 5.0f);
                            break;
                        }
                        EnterEvadeMode();
                    }
                }
            }

            if(uiElementalSpellTimer <= diff)
            {
                DoCastVictim(Boss[uiBossRandom].uiSpell);

                uiElementalSpellTimer = urand(5000, 8000);
            } else uiElementalSpellTimer -= diff;

            if(!bAddAttack && !HealthAbovePct(20))
            {
                if(!SummonList.empty())
                    for(std::list<uint64>::const_iterator itr = SummonList.begin(); itr != SummonList.end(); ++itr)
                    {
                        if(Creature* pTemp = Unit::GetCreature(*me, *itr))
                        {
                            if(pTemp->GetPositionZ() >= 287.00f)
                                continue;

                            if(pTemp->getVictim())
                                pTemp->GetMotionMaster()->MoveChase(pTemp->getVictim());
                        }
                    }

                bAddAttack = true;
            }

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* killer)
        {
            if(!SummonList.empty())
                for(std::list<uint64>::const_iterator itr = SummonList.begin(); itr != SummonList.end(); ++itr)
                    if(Creature* pTemp = Unit::GetCreature(*me, *itr))
                        pTemp->DespawnOrUnsummon();

            if(Player* player = killer->GetCharmerOrOwnerPlayerOrPlayerItself())
                player->GetCharmerOrOwnerPlayerOrPlayerItself()->GroupEventHappens(QUEST_AMPHITHEATER_ANGUISH_FROM_BEYOND, killer);

            std::string sText = (std::string(killer->GetName()) + " est victorieux une fois de plus !");

            if(Unit* summoner = me->ToTempSummon()->GetSummoner())
                summoner->MonsterYell(sText.c_str(), LANG_UNIVERSAL, 0);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_elemental_lordAI(pCreature);
    }
};

class npc_fiend_elemental : public CreatureScript
{
public:
    npc_fiend_elemental() : CreatureScript("npc_fiend_elemental") { }

    struct npc_fiend_elementalAI : public ScriptedAI
    {
        npc_fiend_elementalAI(Creature* pCreature): ScriptedAI(pCreature) { }

        uint32 uiMissleTimer;
        uint32 uiSpell;

        void Reset()
        {
            if(me->GetPositionZ() >= 287.0f)
                me->GetMotionMaster()->MoveIdle();

            uiSpell = 0;
            uiMissleTimer = urand(2000, 7000);
        }

        void AttackStart(Unit* who)
        {
            if(!who)
                return;

            AttackStartNoMove(who);
        }

        void SetData(uint32 uiData, uint32 uiValue)
        {
            if(uiData == 1)
                uiSpell = Boss[uiValue].uiAddSpell;

        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(me->GetPositionZ() >= 287.0f)
            {
                if(uiMissleTimer <= diff)
                {
                    DoCast(me, uiSpell); // this spell is not supported ... YET!
                    uiMissleTimer = urand(2000, 7000);

                } else uiMissleTimer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_fiend_elementalAI(pCreature);
    }
};

class npc_released_offspring_harkoa : public CreatureScript
{
public:
    npc_released_offspring_harkoa() : CreatureScript("npc_released_offspring_harkoa") { }

    struct npc_released_offspring_harkoaAI : public ScriptedAI
    {
        npc_released_offspring_harkoaAI(Creature* pCreature): ScriptedAI(pCreature) { }

        void Reset()
        {
            float x, y, z;
            me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 25.0f);
            me->GetMotionMaster()->MovePoint(0, x, y, z);
        }

        void MovementInform(uint32 uiType, uint32 /*uiId*/)
        {
            if(uiType != POINT_MOTION_TYPE)
                return;
            me->DisappearAndDie();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_released_offspring_harkoaAI(pCreature);
    }
};

enum eCrusade_recruit
{
    SPELL_QUEST_CREDIT                            = 50633,

    QUEST_TROLL_PATROL_INTESTINAL_FORTITUDE       = 12509,

    GOSSIP_CRUSADE_TEXT                           = 13069,

    SAY_RECRUIT_1                                 = -1571036,
    SAY_RECRUIT_2                                 = -1571037,
    SAY_RECRUIT_3                                 = -1571038
};

#define GOSSIP_ITEM_1 "Allez-y et faites-en sorte que ce Fléau ne renaisse jamais !"

class npc_crusade_recruit : public CreatureScript
{
public:
    npc_crusade_recruit() : CreatureScript("npc_crusade_recruit") { }

    struct npc_crusade_recruitAI : public ScriptedAI
    {
        npc_crusade_recruitAI(Creature* pCreature): ScriptedAI(pCreature) { }

        uint8 m_uiPhase;                  //The current phase we are in
        uint32 m_uiTimer;                 //Timer until phase transition
        float m_heading;                  //Store creature heading

        void Reset()
        {
            m_uiTimer = 0;
            m_uiPhase = 0;
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_COWER);
            m_heading = me->GetOrientation();
        }

        void UpdateAI(const uint32 diff)
        {
            if(m_uiPhase)
            {
                if(m_uiTimer <= diff)
                {
                    switch(m_uiPhase)
                    {
                        case 1:
                            // say random text
                            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                            DoScriptText(RAND(SAY_RECRUIT_1, SAY_RECRUIT_2, SAY_RECRUIT_3), me);
                            m_uiTimer = 3000;
                            m_uiPhase = 2;
                            break;
                        case 2:
                            // walk forward
                            me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
                            me->GetMotionMaster()->MovePoint(0, me->GetPositionX() + (cos(m_heading) * 10), me->GetPositionY() + (sin(m_heading) * 10), me->GetPositionZ());
                            m_uiTimer = 5000;
                            m_uiPhase = 3;
                            break;
                        case 3:
                            // despawn
                            me->DisappearAndDie();
                            m_uiTimer = 0;
                            m_uiPhase = 0;
                            break;
                    }
                } else m_uiTimer -= diff;
            }
            ScriptedAI::UpdateAI(diff);

            if(!UpdateVictim())
                return;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_crusade_recruitAI(pCreature);
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(player->GetQuestStatus(QUEST_TROLL_PATROL_INTESTINAL_FORTITUDE) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        player->SEND_GOSSIP_MENU(GOSSIP_CRUSADE_TEXT, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        if(uiAction == GOSSIP_ACTION_INFO_DEF +1)
        {
            player->CLOSE_GOSSIP_MENU();
            creature->CastSpell(player, SPELL_QUEST_CREDIT, true);
            CAST_AI(npc_crusade_recruit::npc_crusade_recruitAI, (creature->AI()))->m_uiPhase = 1;
            creature->SetInFront(player);
            creature->SendMovementFlagUpdate();
        }

        return true;
    }
};

enum eScourgeEnclosure
{
    QUEST_OUR_ONLY_HOPE                           = 12916,
    NPC_GYMER_DUMMY                               = 29928   //from quest template
};

class go_scourge_enclosure : public GameObjectScript
{
public:
    go_scourge_enclosure() : GameObjectScript("go_scourge_enclosure") { }

    bool OnGossipHello(Player* player, GameObject* pGO)
    {
        if(player->GetQuestStatus(QUEST_OUR_ONLY_HOPE) == QUEST_STATUS_INCOMPLETE)
        {
            Creature* pGymerDummy = pGO->FindNearestCreature(NPC_GYMER_DUMMY, 20.0f);
            if(pGymerDummy)
            {
                pGO->UseDoorOrButton();
                player->KilledMonsterCredit(pGymerDummy->GetEntry(), pGymerDummy->GetGUID());
                pGymerDummy->CastSpell(pGymerDummy, 55529, true);
                pGymerDummy->DisappearAndDie();
            }
        }
        return true;
    }
};

enum Nass
{
    SPELL_KICK              = 51866,
    SPELL_KILL_CREDIT       = 51871,
    SPELL_GATHER_VISUAL     = 51870,

    CREATURE_TARGET_BUNNY   = 28523
};

class npc_nass : public CreatureScript
{
public:
    npc_nass() : CreatureScript("npc_nass") { }

    struct npc_nassAI : public ScriptedAI
    {
        npc_nassAI(Creature* pCreature): ScriptedAI(pCreature) { }

        void SpellHit(Unit* /*pCaster*/, const SpellEntry* pSpell)
        {
            if(pSpell->Id == SPELL_KICK) {
                if(Creature* target = me->FindNearestCreature(CREATURE_TARGET_BUNNY, 30.0f)) {
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MovePoint(0, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                    target->ForcedDespawn(3000);
                }
            }
        }

        void MovementInform(uint32 uiType, uint32 /*uiId*/)
        {
            if(uiType != POINT_MOTION_TYPE)
                return;

            me->GetMotionMaster()->Clear();
            DoCast(SPELL_GATHER_VISUAL);
            DoCast(SPELL_KILL_CREDIT);
            if(Unit* owner = me->ToTempSummon()->GetSummoner())
                me->GetMotionMaster()->MoveFollow(owner, 1.2f, owner->GetAngle(me));
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_nassAI(pCreature);
    }
};

class go_musty_coffin : public GameObjectScript
{
public:
    go_musty_coffin() : GameObjectScript("go_musty_coffin") { }

    bool OnGossipHello(Player* player, GameObject* pGO)
    {
        if(player->GetQuestStatus(12710) == QUEST_STATUS_INCOMPLETE)
        {
            player->KilledMonsterCredit(28929, 0);
        }
        return true;
    }
};

void AddSC_zuldrak()
{
    new npc_drakuru_shackles;
    new npc_captured_rageclaw;
    new npc_gymer;
    new npc_gurgthock;
    new npc_orinoko_tuskbreaker;
    new npc_korrak_bloodrager;
    new npc_yggdras;
    new npc_stinkbeard;
    new npc_released_offspring_harkoa;
    new npc_crusade_recruit;
    new npc_elemental_lord;
    new npc_fiend_elemental;
    new go_scourge_enclosure;
    new npc_nass;
    new npc_lurking_basilisk;
    new go_musty_coffin;
}

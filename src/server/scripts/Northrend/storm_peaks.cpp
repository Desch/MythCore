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

#define SAY_AGGRO                  -1571003
#define GOSSIP_AGNETTA             "Passes l'entrainement soeur ... ou as-tu peur de faire face à quelqu'un de ta taille ?"

enum eAgnetta
{
    QUEST_ITS_THAT_YOUR_GOBLIN      = 12969,
    FACTION_HOSTILE_AT1             = 45
};

class npc_agnetta_tyrsdottar : public CreatureScript
{
public:
    npc_agnetta_tyrsdottar() : CreatureScript("npc_agnetta_tyrsdottar") { }

    struct npc_agnetta_tyrsdottarAI : public ScriptedAI
    {
        npc_agnetta_tyrsdottarAI(Creature* pCreature): ScriptedAI(pCreature) { }

        void Reset()
        {
            me->RestoreFaction();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_agnetta_tyrsdottarAI(pCreature);
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(player->GetQuestStatus(QUEST_ITS_THAT_YOUR_GOBLIN) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_AGNETTA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        player->SEND_GOSSIP_MENU(13691, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        if(uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            DoScriptText(SAY_AGGRO, creature);
            player->CLOSE_GOSSIP_MENU();
            creature->setFaction(FACTION_HOSTILE_AT1);
            creature->AI()->AttackStart(player);
        }

        return true;
    }
};

#define GOSSIP_ITEM1    "Êtes-vous d'accord? Je suis venu pour vous ramener à Frosthold si vous pouvez le supporter."
#define GOSSIP_ITEM2    "Je suis désolé, je n'ai pas venir plus tôt. Qu'est-il arrivé ?"
#define GOSSIP_ITEM3    "Je vais aller chercher de l'aide. Accrochez-vous."

enum eFrostbornScout
{
    QUEST_MISSING_SCOUTS  =  12864
};

class npc_frostborn_scout : public CreatureScript
{
public:
    npc_frostborn_scout() : CreatureScript("npc_frostborn_scout") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(player->GetQuestStatus(QUEST_MISSING_SCOUTS) == QUEST_STATUS_INCOMPLETE)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->PlayerTalkClass->SendGossipMenu(13611, creature->GetGUID());
        }

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                player->PlayerTalkClass->SendGossipMenu(13612, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
                player->PlayerTalkClass->SendGossipMenu(13613, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->PlayerTalkClass->SendGossipMenu(13614, creature->GetGUID());
                player->AreaExploredOrEventHappens(QUEST_MISSING_SCOUTS);
                break;
        }

        return true;
    }
};

#define GOSSIP_HN "Thorim?"
#define GOSSIP_SN1 "Pouvez-vous me dire ce qu'est devenu Sif ?"
#define GOSSIP_SN2 "Il ne fait plus que cela, Thorim. Il contrôle désormais Ulduar."
#define GOSSIP_SN3 "Il ne doit pas finir comme ça."

enum eThorim
{
    QUEST_SIBLING_RIVALRY = 13064,
    NPC_THORIM = 29445,
    GOSSIP_TEXTID_THORIM1 = 13799,
    GOSSIP_TEXTID_THORIM2 = 13801,
    GOSSIP_TEXTID_THORIM3 = 13802,
    GOSSIP_TEXTID_THORIM4 = 13803
};

class npc_thorim : public CreatureScript
{
public:
    npc_thorim() : CreatureScript("npc_thorim") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if(player->GetQuestStatus(QUEST_SIBLING_RIVALRY) == QUEST_STATUS_INCOMPLETE) {
            player->ADD_GOSSIP_ITEM(0, GOSSIP_HN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_THORIM1, creature->GetGUID());
            return true;
        }
        return false;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SN1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_THORIM2, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SN2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_THORIM3, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->ADD_GOSSIP_ITEM(0, GOSSIP_SN3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_THORIM4, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                player->CLOSE_GOSSIP_MENU();
                player->CompleteQuest(QUEST_SIBLING_RIVALRY);
                break;
        }
        return true;
    }
};

enum eGoblinPrisoner
{
    GO_RUSTY_CAGE = 191544
};

class npc_goblin_prisoner : public CreatureScript
{
public:
    npc_goblin_prisoner() : CreatureScript("npc_goblin_prisoner") { }

    struct npc_goblin_prisonerAI : public ScriptedAI
    {
        npc_goblin_prisonerAI(Creature* pCreature): ScriptedAI(pCreature){ }

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);

            if(GameObject* pGO = me->FindNearestGameObject(GO_RUSTY_CAGE, 5.0f))
            {
                if(pGO->GetGoState() == GO_STATE_ACTIVE)
                    pGO->SetGoState(GO_STATE_READY);
            }
        }

    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_goblin_prisonerAI(pCreature);
    }
};

#define GOSSIP_CHALLENGER            "Faisons-le, ma soeur."

enum eVictoriousChallenger
{
    QUEST_TAKING_ALL_CHALLENGERS    = 12971,
    QUEST_DEFENDING_YOUR_TITLE      = 13423,

    SPELL_SUNDER_ARMOR              = 11971,
    SPELL_REND_VC                   = 11977
};

class npc_victorious_challenger : public CreatureScript
{
public:
    npc_victorious_challenger() : CreatureScript("npc_victorious_challenger") { }

    struct npc_victorious_challengerAI : public ScriptedAI
    {
        npc_victorious_challengerAI(Creature* pCreature): ScriptedAI(pCreature) { }

        uint32 SunderArmorTimer;
        uint32 RendTimer;

        void Reset()
        {
            me->RestoreFaction();

            SunderArmorTimer = 10000;
            RendTimer        = 15000;
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if(!UpdateVictim())
                return;

            if(RendTimer < diff)
            {
                DoCast(me->getVictim(), SPELL_REND_VC, true);
                RendTimer = 15000;
            } else RendTimer -= diff;

            if(SunderArmorTimer < diff)
            {
                DoCast(me->getVictim(), SPELL_SUNDER_ARMOR, true);
                SunderArmorTimer = 10000;
            } else SunderArmorTimer -= diff;

            DoMeleeAttackIfReady();
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            me->RestoreFaction();
        }

    };

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if(player->GetQuestStatus(QUEST_TAKING_ALL_CHALLENGERS) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_DEFENDING_YOUR_TITLE) == QUEST_STATUS_INCOMPLETE)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHALLENGER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        return false;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        if(uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            player->CLOSE_GOSSIP_MENU();
            creature->setFaction(14);
            creature->AI()->AttackStart(player);
        }

        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_victorious_challengerAI(pCreature);
    }
};

#define GOSSIP_LOKLIRACRONE     "Parlez-moi de cette proposition"
#define GOSSIP_LOKLIRACRONE1    "Qu'est-il arrivé alors ?"
#define GOSSIP_LOKLIRACRONE2    "Vous voulez que je prenne part à Hyldsmeet pour mettre fin à la guerre ?"
#define GOSSIP_LOKLIRACRONE3    "Très bien. Je vais prendre part à cette compétition."

enum eLokliraCrone
{
    QUEST_HYLDSMEET     = 12970,

    GOSSIP_TEXTID_LOK1  = 13778,
    GOSSIP_TEXTID_LOK2  = 13779,
    GOSSIP_TEXTID_LOK3  = 13780
};

class npc_loklira_crone : public CreatureScript
{
public:
    npc_loklira_crone() : CreatureScript("npc_loklira_crone") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if(player->GetQuestStatus(QUEST_HYLDSMEET) == QUEST_STATUS_INCOMPLETE)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOKLIRACRONE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }
        return false;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOKLIRACRONE1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_LOK1, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOKLIRACRONE2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_LOK2, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_LOKLIRACRONE3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_LOK3, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                player->CLOSE_GOSSIP_MENU();
                player->CompleteQuest(QUEST_HYLDSMEET);
                break;
        }
        return true;
    }
};

enum eInjuredGoblin
{
    QUEST_BITTER_DEPARTURE     = 12832,
    SAY_QUEST_ACCEPT           =  -1800042,
    SAY_END_WP_REACHED         =  -1800043
};

#define GOSSIP_ITEM_1       "Je suis prêt, vous pouvez sortir d'ici"

class npc_injured_goblin : public CreatureScript
{
public:
    npc_injured_goblin() : CreatureScript("npc_injured_goblin") { }

    struct npc_injured_goblinAI : public npc_escortAI
    {
        npc_injured_goblinAI(Creature* pCreature) : npc_escortAI(pCreature) { }

        void WaypointReached(uint32 i)
        {
            Player* player = GetPlayerForEscort();
            switch(i)
            {
                case 26:
                    DoScriptText(SAY_END_WP_REACHED, me, player);
                    break;
                case 27:
                    if(player)
                        player->GroupEventHappens(QUEST_BITTER_DEPARTURE, me);
                    break;
            }
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void Reset() { }

        void JustDied(Unit* /*pKiller*/)
        {
            Player* player = GetPlayerForEscort();
            if(HasEscortState(STATE_ESCORT_ESCORTING) && player)
                player->FailQuest(QUEST_BITTER_DEPARTURE);
        }

       void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);
            if(!UpdateVictim())
                return;
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_injured_goblinAI(pCreature);
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if(player->GetQuestStatus(QUEST_BITTER_DEPARTURE) == QUEST_STATUS_INCOMPLETE)
        {
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->PlayerTalkClass->SendGossipMenu(9999999, creature->GetGUID());
        }
        else
            player->SEND_GOSSIP_MENU(999999, creature->GetGUID());
        return true;
    }

    bool OnQuestAccept(Player* /*pPlayer*/, Creature* creature, Quest const *quest)
    {
        if(quest->GetQuestId() == QUEST_BITTER_DEPARTURE)
            DoScriptText(SAY_QUEST_ACCEPT, creature);

        return false;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        npc_escortAI* pEscortAI = CAST_AI(npc_injured_goblin::npc_injured_goblinAI, creature->AI());

        if(uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            pEscortAI->Start(true, true, player->GetGUID());
            creature->setFaction(113);
        }
        return true;
    }
};

#define SPELL_MECHANO_HOG           60866
#define SPELL_MEKGINEERS_CHOPPER    60867

class npc_roxi_ramrocket : public CreatureScript
{
public:
    npc_roxi_ramrocket() : CreatureScript("npc_roxi_ramrocket") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        //Quest Menu
        if(creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        //Trainer Menu
        if(creature->isTrainer() )
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, GOSSIP_TEXT_TRAIN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRAIN);

        //Vendor Menu
        if(creature->isVendor() )
            if(player->HasSpell(SPELL_MECHANO_HOG) || player->HasSpell(SPELL_MEKGINEERS_CHOPPER))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch(action)
        {
            case GOSSIP_ACTION_TRAIN:
                player->GetSession()->SendTrainerList(creature->GetGUID());
                break;
            case GOSSIP_ACTION_TRADE:
                player->GetSession()->SendListInventory(creature->GetGUID());
                break;
        }
        return true;
    }
};

class npc_icefang : public CreatureScript
{
public:
    npc_icefang() : CreatureScript("npc_icefang") { }

    struct npc_icefangAI : public npc_escortAI
    {
        npc_icefangAI(Creature* pCreature) : npc_escortAI(pCreature) { }

        void AttackStart(Unit* /*pWho*/) { }

        void EnterCombat(Unit* /*pWho*/) { }

        void EnterEvadeMode() { }

        void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply)
        {
            if(who->GetTypeId() == TYPEID_PLAYER)
            {
                if(apply)
                    Start(false, true, who->GetGUID());
            }
        }

        void WaypointReached(uint32 /*wp*/) { }

        void JustDied(Unit* /*pKiller*/) { }

        void OnCharmed(bool /*apply*/) { }

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);

            if(!UpdateVictim())
                return;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_icefangAI(pCreature);
    }
};

enum BrunnhildarPrisoner {
    SPELL_ICE_PRISON           = 54894,
    SPELL_ICE_LANCE            = 55046,
    SPELL_FREE_PRISONER        = 55048,
    SPELL_RIDE_DRAKE           = 55074,
    SPELL_SHARD_IMPACT         = 55047
};

class npc_brunnhildar_prisoner : public CreatureScript
{
public:
    npc_brunnhildar_prisoner() : CreatureScript("npc_brunnhildar_prisoner") { }

    struct npc_brunnhildar_prisonerAI : public ScriptedAI
    {
        npc_brunnhildar_prisonerAI(Creature* creature) : ScriptedAI(creature) {}

        bool freed;

        void Reset()
        {
            freed = false;
            me->CastSpell(me, SPELL_ICE_PRISON, true);
        }

        void JustRespawned()
        {
            Reset();
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if(!freed)
                return;

            if(!me->HasUnitState(UNIT_STAT_ONVEHICLE))
                me->Kill(me);
        }

        void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
        {
            if(pSpell->Id != SPELL_ICE_LANCE)
                return;

            if(pCaster->GetVehicleKit()->GetAvailableSeatCount() != 0)
            {
                me->CastSpell(me, SPELL_FREE_PRISONER, true);
                me->CastSpell(pCaster, SPELL_RIDE_DRAKE, true);
                me->CastSpell(me, SPELL_SHARD_IMPACT, true);
                freed = true;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_brunnhildar_prisonerAI(creature);
    }
};

enum FreedProtoDrake
{
    AREA_VALLEY_OF_ANCIENT_WINTERS      = 4437,
    TEXT_EMOTE                          = 0,
    SPELL_KILL_CREDIT_PRISONER          = 55144,
    SPELL_SUMMON_LIBERATED              = 55073,
    SPELL_KILL_CREDIT_DRAKE             = 55143
};

const Position FreedDrakeWaypoints[16] =
{
    {7294.96f, -2418.733f, 823.869f, 0.0f},
    {7315.984f, -2331.46f, 826.3972f, 0.0f},
    {7271.826f, -2271.479f, 833.5917f, 0.0f},
    {7186.253f, -2218.475f, 847.5632f, 0.0f},
    {7113.195f, -2164.288f, 850.2301f, 0.0f},
    {7078.018f, -2063.106f, 854.7581f, 0.0f},
    {7073.221f, -1983.382f, 861.9246f, 0.0f},
    {7061.455f, -1885.899f, 865.119f, 0.0f},
    {7033.32f, -1826.775f, 876.2578f, 0.0f},
    {6999.902f, -1784.012f, 897.4521f, 0.0f},
    {6954.913f, -1747.043f, 897.4521f, 0.0f},
    {6933.856f, -1720.698f, 882.2022f, 0.0f},
    {6932.729f, -1687.306f, 866.1189f, 0.0f},
    {6952.458f, -1663.802f, 849.8133f, 0.0f},
    {7002.819f, -1651.681f, 831.397f, 0.0f},
    {7026.531f, -1649.239f, 828.8406f, 0.0f}
};


class npc_freed_protodrake : public CreatureScript
{
public:
    npc_freed_protodrake() : CreatureScript("npc_freed_protodrake") { }

    struct npc_freed_protodrakeAI : public VehicleAI
    {
        npc_freed_protodrakeAI(Creature* creature) : VehicleAI(creature) {}

        bool autoMove;
        bool wpReached;
        uint16 CheckTimer;
        uint16 countWP;

        void Reset()
        {
            autoMove = false;
            wpReached = false;
            CheckTimer = 5000;
            countWP = 0;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if(type != POINT_MOTION_TYPE)
                return;

            if(id < 15)
            {
                ++countWP;
                wpReached = true;
            } else { // drake reached village
                // get player that rides drake (from seat 0)
                Unit* player = me->GetVehicleKit()->GetPassenger(0);
                if(player && player->GetTypeId() == TYPEID_PLAYER)
                {
                    // for each prisoner on drake,give credit
                    for(uint8 i = 1; i < 4; ++i)
                        if(Unit* prisoner = me->GetVehicleKit()->GetPassenger(i))
                        {
                            if(prisoner->GetTypeId() != TYPEID_UNIT)
                                return;
                            player->CastSpell(player, SPELL_KILL_CREDIT_PRISONER, true);
                            prisoner->CastSpell(prisoner, SPELL_SUMMON_LIBERATED, true);
                            prisoner->ExitVehicle();
                        }
                    me->CastSpell(me, SPELL_KILL_CREDIT_DRAKE, true);
                    player->ExitVehicle();
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!autoMove)
            {
                if(CheckTimer < diff)
                {
                    CheckTimer = 5000;
                    if(me->GetAreaId() == AREA_VALLEY_OF_ANCIENT_WINTERS)
                    {
                        Talk(TEXT_EMOTE, me->GetVehicleKit()->GetPassenger(0)->GetGUID());
                        autoMove = true;
                        wpReached = true;
                    }
                }
                else
                    CheckTimer -= diff;
            }

            if(wpReached && autoMove)
            {
                wpReached = false;
                me->GetMotionMaster()->MovePoint(countWP, FreedDrakeWaypoints[countWP]);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_freed_protodrakeAI(creature);
    }
};

void AddSC_storm_peaks()
{
    new npc_agnetta_tyrsdottar;
    new npc_frostborn_scout;
    new npc_thorim;
    new npc_goblin_prisoner;
    new npc_victorious_challenger;
    new npc_loklira_crone;
    new npc_injured_goblin;
    new npc_roxi_ramrocket;
    new npc_icefang;
    new npc_brunnhildar_prisoner;
    new npc_freed_protodrake;
}

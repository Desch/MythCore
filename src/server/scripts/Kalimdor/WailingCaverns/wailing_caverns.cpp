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
#include "wailing_caverns.h"

enum eEnums
{
    //say
    SAY_MAKE_PREPARATIONS         = -1043001,
    SAY_TEMPLE_OF_PROMISE         = -1043002,
    SAY_MUST_CONTINUE             = -1043003,
    SAY_BANISH_THE_SPIRITS        = -1043004,
    SAY_CAVERNS_PURIFIED          = -1043005,
    SAY_BEYOND_THIS_CORRIDOR      = -1043006,
    SAY_EMERALD_DREAM             = -1043007,
    SAY_MUTANUS_THE_DEVOURER      = -1043012,
    SAY_NARALEX_AWAKES            = -1043014,
    SAY_THANK_YOU                 = -1043015,
    SAY_FAREWELL                  = -1043016,
    SAY_ATTACKED                  = -1043017,
    //yell
    SAY_AT_LAST                   = -1043000,
    SAY_I_AM_AWAKE                = -1043013,
    //emote
    EMOTE_AWAKENING_RITUAL        = -1043008,
    EMOTE_TROUBLED_SLEEP          = -1043009,
    EMOTE_WRITHE_IN_AGONY         = -1043010,
    EMOTE_HORRENDOUS_VISION       = -1043011,
    //spell
    SPELL_MARK_OF_THE_WILD_RANK_2 = 5232,
    SPELL_SERPENTINE_CLEANSING    = 6270,
    SPELL_NARALEXS_AWAKENING      = 6271,
    SPELL_FLIGHT_FORM             = 33943,
    //npc entry
    NPC_DEVIATE_RAVAGER           = 3636,
    NPC_DEVIATE_VIPER             = 5755,
    NPC_DEVIATE_MOCCASIN          = 5762,
    NPC_NIGHTMARE_ECTOPLASM       = 5763,
    NPC_MUTANUS_THE_DEVOURER      = 3654,
};

#define GOSSIP_ID_START_1       698  //Naralex sleeps again!
#define GOSSIP_ID_START_2       699  //The fanglords are dead!
#define GOSSIP_ITEM_NARALEX     "Let the event begin!"
#define ACHIEVEMENT_WAILING_CAVERNS 630

class npc_disciple_of_naralex : public CreatureScript
{
public:
    npc_disciple_of_naralex() : CreatureScript("npc_disciple_of_naralex") { }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        InstanceScript* pInstance = pCreature->GetInstanceScript();
        if(uiAction == GOSSIP_ACTION_INFO_DEF + 1)
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            if(pInstance)
                pInstance->SetData(TYPE_NARALEX_EVENT, IN_PROGRESS);

            DoScriptText(SAY_MAKE_PREPARATIONS, pCreature);

            pCreature->setFaction(250);
            pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);

            CAST_AI(npc_escortAI, (pCreature->AI()))->Start(false, false, pPlayer->GetGUID());
            CAST_AI(npc_escortAI, (pCreature->AI()))->SetDespawnAtFar(false);
            CAST_AI(npc_escortAI, (pCreature->AI()))->SetDespawnAtEnd(false);
        }
        return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        InstanceScript* pInstance = pCreature->GetInstanceScript();

        if(pInstance)
        {
            pCreature->CastSpell(pPlayer, SPELL_MARK_OF_THE_WILD_RANK_2, true);
            if((pInstance->GetData(TYPE_LORD_COBRAHN) == DONE) && (pInstance->GetData(TYPE_LORD_PYTHAS) == DONE) &&
                (pInstance->GetData(TYPE_LADY_ANACONDRA) == DONE) && (pInstance->GetData(TYPE_LORD_SERPENTIS) == DONE))
            {
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NARALEX, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_ID_START_2, pCreature->GetGUID());

                if(!pInstance->GetData(TYPE_NARALEX_YELLED))
                {
                    DoScriptText(SAY_AT_LAST, pCreature);
                    pInstance->SetData(TYPE_NARALEX_YELLED, 1);
                }
            } else pPlayer->SEND_GOSSIP_MENU(GOSSIP_ID_START_1, pCreature->GetGUID());
        }
        return true;
    }

    struct npc_disciple_of_naralexAI : public npc_escortAI
    {
        npc_disciple_of_naralexAI(Creature* pCreature) : npc_escortAI(pCreature)
        {
            pInstance       = pCreature->GetInstanceScript();
            eventTimer      = 0;
            currentEvent    = 0;
            eventProgress   = 0;
            me->setActive(true);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        }

        uint32 eventTimer;
        uint32 currentEvent;
        uint32 eventProgress;
        InstanceScript* pInstance;

        void WaypointReached(uint32 i)
        {
            if(!pInstance)
                return;

            switch(i)
            {
                case 4:
                    eventProgress = 1;
                    currentEvent = TYPE_NARALEX_PART1;
                    pInstance->SetData(TYPE_NARALEX_PART1, IN_PROGRESS);
                    break;
                case 5:
                    DoScriptText(SAY_MUST_CONTINUE, me);
                    pInstance->SetData(TYPE_NARALEX_PART1, DONE);
                    break;
                case 11:
                    eventProgress = 1;
                    currentEvent = TYPE_NARALEX_PART2;
                    pInstance->SetData(TYPE_NARALEX_PART2, IN_PROGRESS);
                    break;
                case 19:
                    DoScriptText(SAY_BEYOND_THIS_CORRIDOR, me);
                    break;
                case 24:
                    eventProgress = 1;
                    currentEvent = TYPE_NARALEX_PART3;
                    pInstance->SetData(TYPE_NARALEX_PART3, IN_PROGRESS);
                    break;
                default:
                    break;
            }
        }

        void Reset() { }

        void EnterCombat(Unit* pWho)
        {
            DoScriptText(SAY_ATTACKED, me, pWho);
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if(!pInstance)
                return;

            pInstance->SetData(TYPE_NARALEX_EVENT, FAIL);
            pInstance->SetData(TYPE_NARALEX_PART1, FAIL);
            pInstance->SetData(TYPE_NARALEX_PART2, FAIL);
            pInstance->SetData(TYPE_NARALEX_PART3, FAIL);
        }

        void JustSummoned(Creature* pSummoned)
        {
            pSummoned->AI()->AttackStart(me);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!pInstance)
                return;

            if(currentEvent != TYPE_NARALEX_PART3)
                npc_escortAI::UpdateAI(diff);

            if(eventTimer <= diff)
            {
                eventTimer = 0;
                if(pInstance->GetData(currentEvent) == IN_PROGRESS)
                {
                    switch(currentEvent)
                    {
                        case TYPE_NARALEX_PART1:
                        {
                            switch(eventProgress)
                            {
                                case 1:
                                    DoScriptText(SAY_TEMPLE_OF_PROMISE, me);
                                    me->SummonCreature(NPC_DEVIATE_RAVAGER, -82.1763f, 227.874f, -93.3233f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                                    me->SummonCreature(NPC_DEVIATE_RAVAGER, -72.9506f, 216.645f, -93.6756f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                                    ++eventProgress;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        }
                        case TYPE_NARALEX_PART2:
                        {
                            switch(eventProgress)
                            {
                                case 1:
                                    DoScriptText(SAY_BANISH_THE_SPIRITS, me);
                                    DoCast(me, SPELL_SERPENTINE_CLEANSING);
                                    me->SummonCreature(NPC_DEVIATE_VIPER, -61.5261f, 273.676f, -92.8442f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                                    me->SummonCreature(NPC_DEVIATE_VIPER, -58.4658f, 280.799f, -92.8393f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                                    me->SummonCreature(NPC_DEVIATE_VIPER, -50.002f,  278.578f, -92.8442f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                                    eventTimer = 30000;
                                    ++eventProgress;
                                    break;
                                case 2:
                                    DoScriptText(SAY_CAVERNS_PURIFIED, me);
                                    pInstance->SetData(TYPE_NARALEX_PART2, DONE);
                                    if(me->HasAura(SPELL_SERPENTINE_CLEANSING))
                                        me->RemoveAura(SPELL_SERPENTINE_CLEANSING);
                                    break;
                                default:
                                    break;
                            }
                            break;
                        }
                        case TYPE_NARALEX_PART3:
                        {
                            switch(eventProgress)
                            {
                                case 1:
                                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                    DoScriptText(SAY_EMERALD_DREAM, me);
                                    eventTimer = 4000;
                                    ++eventProgress;
                                    break;
                                case 2:
                                    DoCast(me, SPELL_NARALEXS_AWAKENING, true);
                                    DoScriptText(EMOTE_AWAKENING_RITUAL, me);
                                    eventTimer = 15000;
                                    ++eventProgress;
                                    break;
                                case 3:
                                    DoScriptText(EMOTE_TROUBLED_SLEEP, me);
                                    me->SummonCreature(NPC_DEVIATE_MOCCASIN, 135.943f, 199.701f, -103.529f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                    me->SummonCreature(NPC_DEVIATE_MOCCASIN, 151.08f,  221.13f,  -103.609f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                    me->SummonCreature(NPC_DEVIATE_MOCCASIN, 128.007f, 227.428f, -97.421f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                    eventTimer = 15000;
                                    ++eventProgress;
                                    break;
                                case 4:
                                    DoScriptText(EMOTE_WRITHE_IN_AGONY, me);
                                    me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 133.413f, 207.188f, -102.469f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                    me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 142.857f, 218.645f, -102.905f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                    me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 105.102f, 227.211f, -102.752f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                    me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 153.372f, 235.149f, -102.826f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                    me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 149.524f, 251.113f, -102.558f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                    me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 136.208f, 266.466f, -102.977f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                    me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 126.167f, 274.759f, -102.962f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                    eventTimer = 30000;
                                    ++eventProgress;
                                    break;
                                case 5:
                                    DoScriptText(EMOTE_HORRENDOUS_VISION, me);
                                    me->SummonCreature(NPC_MUTANUS_THE_DEVOURER, 150.872f, 262.905f, -103.503f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 300000);
                                    DoScriptText(SAY_MUTANUS_THE_DEVOURER, me);
                                    pInstance->SetData(TYPE_MUTANUS_THE_DEVOURER, IN_PROGRESS);
                                    ++eventProgress;
                                case 6:
                                    if(pInstance->GetData(TYPE_MUTANUS_THE_DEVOURER) == DONE)
                                    {
                                        eventTimer = 3000;
                                        ++eventProgress;
                                        if(AchievementEntry const* pAchievement = GetAchievementStore()->LookupEntry(ACHIEVEMENT_WAILING_CAVERNS))
                                        {
                                            Map* pMap = me->GetMap();
                                            if(pMap && pMap->IsDungeon())
                                            {
                                                Map::PlayerList const &players = pMap->GetPlayers();
                                                for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                                                    itr->getSource()->CompletedAchievement(pAchievement);
                                            }
                                        }
                                        if(me->HasAura(SPELL_NARALEXS_AWAKENING))
                                            me->RemoveAura(SPELL_NARALEXS_AWAKENING);
                                        me->SetStandState(UNIT_STAND_STATE_STAND);
                                        DoScriptText(SAY_I_AM_AWAKE, me);
                                        DoScriptText(SAY_NARALEX_AWAKES, me);
                                    }
                                    break;
                                case 7:
                                    DoScriptText(SAY_THANK_YOU, me);
                                    eventTimer = 6000;
                                    ++eventProgress;
                                    break;
                                case 8:
                                    DoScriptText(SAY_FAREWELL, me);
                                    me->AddAura(SPELL_FLIGHT_FORM, me);
                                    SetRun();
                                    me->SetStandState(UNIT_STAND_STATE_STAND);
                                    me->AddAura(SPELL_FLIGHT_FORM, me);
                                    eventTimer = 8000;
                                    ++eventProgress;
                                    break;
                                case 9:
                                    me->GetMotionMaster()->MovePoint(25, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                                    eventTimer = 1500;
                                    ++eventProgress;
                                    break;
                                case 10:
                                    me->GetMotionMaster()->MovePoint(0, 117.095512f, 247.107971f, -96.167870f);
                                    me->GetMotionMaster()->MovePoint(1, 90.388809f, 276.135406f, -83.389801f);
                                    me->GetMotionMaster()->MovePoint(26, 117.095512f, 247.107971f, -96.167870f);
                                    me->GetMotionMaster()->MovePoint(27, 144.375443f, 281.045837f, -82.477135f);
                                    eventTimer = 2500;
                                    ++eventProgress;
                                    break;
                                case 11:
                                    me->SetVisible(false);
                                    pInstance->SetData(TYPE_NARALEX_PART3, DONE);
                                    break;
                                default:
                                    break;
                            }
                            break;
                        }
                    }
                }
            } else eventTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_disciple_of_naralexAI(pCreature);
    }
};

void AddSC_wailing_caverns()
{
    new npc_disciple_of_naralex;
}

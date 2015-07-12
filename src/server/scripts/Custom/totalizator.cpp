#include "ScriptPCH.h"

#define C_TOTALIZATORMOB_COUNT 6
#define SPELL_FIREWORK 11544

const uint32 TotalizatorMobIDs[] =
{
    99110, //cat
    99111, //2
    99112, //3
    99113, //4
    99114, //5
    99115, //6
};

const Position PosMobStart[] =
{
    {5763.6f, 606.6f, 609.8f, 0.88f},
    {5766.8f, 604.0f, 609.8f, 0.88f},
    {5769.9f, 601.4f, 609.8f, 0.88f},
    {5773.0f, 598.9f, 609.8f, 0.88f},
    {5776.3f, 596.2f, 609.8f, 0.88f},
    {5780.0f, 593.3f, 609.8f, 0.88f},
};

const Position PosMobEnd[] =
{
    {5829.7f, 686.8f, 609.8f, 3.9f},
    {5832.8f, 683.7f, 609.8f, 3.9f},
    {5836.0f, 680.7f, 609.8f, 3.9f},
    {5839.5f, 677.4f, 609.8f, 3.9f},   
    {5842.2f, 674.9f, 609.8f, 3.9f},
    {5844.8f, 672.4f, 609.8f, 3.9f},
};

struct ITRate
{
    Player* m_player;
    uint32 m_rate;
    uint8 m_mob;
};

class custom_totalizator_controler : public CreatureScript
{
public:
    custom_totalizator_controler() : CreatureScript("custom_totalizator_controler") { }

    struct custom_totalizator_controlerAI : public ScriptedAI
    {
        custom_totalizator_controlerAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            for (uint32 i = 0; i < C_TOTALIZATORMOB_COUNT; ++i)
                if (Creature *mob = me->SummonCreature(TotalizatorMobIDs[i], PosMobStart[i], TEMPSUMMON_MANUAL_DESPAWN, 0, 0))
                    TotalizatorMobs.push_back(mob);
        }

        std::vector<Creature*> TotalizatorMobs;
        std::vector<ITRate> Rates;

        bool bStarted;
        bool bReset;
        bool bAnnounce5, bAnnounce4, bAnnounce3,  bAnnounce2, bAnnounce1, bAnnounce0;
        uint64 uiWinner;
        uint64 uiWinnerIdx;

        uint32 uiTotalizator_Timer;
        uint32 uiReset_Timer;

        bool IsStarted()
        {
            return bStarted;
        }

        void Reset()
        {
            bStarted = false;
            bReset = true;
            bAnnounce5 = false;
            bAnnounce4 = false;
            bAnnounce3 = false;
            bAnnounce2 = false;
            bAnnounce1 = false;
            bAnnounce0 = false;
            uiWinner = 0;
            uiWinnerIdx = 0;
            uiTotalizator_Timer = 5*MINUTE*IN_MILLISECONDS;
            uiReset_Timer = 20*IN_MILLISECONDS;
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiTotalizator_Timer <= diff)
            {
                StartTotalizator();
                uiTotalizator_Timer = 5*MINUTE*IN_MILLISECONDS;
            } else uiTotalizator_Timer -= diff;

            if (uiTotalizator_Timer <= 5*MINUTE*IN_MILLISECONDS && !bAnnounce5)
            {
                me->MonsterSay("До старта осталось 5 минут...", LANG_UNIVERSAL, NULL);
                bAnnounce5 = true;
            }
            else if (uiTotalizator_Timer <= 4*MINUTE*IN_MILLISECONDS && !bAnnounce4)
            {
                me->MonsterSay("До старта осталось 4 минуты...", LANG_UNIVERSAL, NULL);
                bAnnounce4 = true;
            }
            else if (uiTotalizator_Timer <= 3*MINUTE*IN_MILLISECONDS && !bAnnounce3)
            {
                me->MonsterSay("До старта осталось 3 минуты...", LANG_UNIVERSAL, NULL);
                bAnnounce3 = true;
            }
            else if (uiTotalizator_Timer <= 2*MINUTE*IN_MILLISECONDS && !bAnnounce2)
            {
                me->MonsterSay("До старта осталось 2 минуты...", LANG_UNIVERSAL, NULL);
                bAnnounce2 = true;
            }
            else if (uiTotalizator_Timer <= 1*MINUTE*IN_MILLISECONDS && !bAnnounce1)
            {
                me->MonsterSay("До старта осталось 1 минута...", LANG_UNIVERSAL, NULL);
                bAnnounce1 = true;
            }
            else if (uiTotalizator_Timer <= 10*IN_MILLISECONDS && !bAnnounce0)
            {
                me->MonsterSay("До старта осталось 10 секунд...", LANG_UNIVERSAL, NULL);
                bAnnounce0 = true;
            }

            if (!bReset)
                if (uiReset_Timer <= diff)
                {
                    ResetTotalizator();
                    bReset = true;
                    uiReset_Timer = 20*IN_MILLISECONDS;
                } else uiReset_Timer -= diff;
        }

        void AddRate(Player* player, uint32 money, uint32 mob)
        {
            if (!GetRates(player))
            {
                ITRate annulled;
                annulled.m_player = player;
                annulled.m_rate = money;
                annulled.m_mob = (uint8)mob;

                Rates.push_back(annulled);
            }
            else
                me->MonsterWhisper("Вы успешно аннулировали свою ставку, шекели успешно отправлены Адептегу на пропитание. Аве!", player->GetGUID(), false);
        }

        uint32 GetRates(Player* player)
        {
            for (std::vector<ITRate>::const_iterator itr = Rates.begin(); itr != Rates.end(); ++itr)
                if ((*itr).m_player == player)
                    return (*itr).m_rate;

            return 0;
        }

        uint32 GetMobRates(Player* player)
        {
            for (std::vector<ITRate>::const_iterator itr = Rates.begin(); itr != Rates.end(); ++itr)
                if ((*itr).m_player == player)
                    return (*itr).m_mob;

            return 0;
        }

        void ProcessRates(uint32 winner)
        {
            for (std::vector<ITRate>::const_iterator itr = Rates.begin(); itr != Rates.end(); ++itr)
                if (Player* player = (*itr).m_player)
                    if (player && (*itr).m_mob == winner)
                        RewardRate(player);

            ClearRates(); // Удалить все
        }

        void RewardRate(Player* player)
        {
            if (uint32 rate = GetRates(player))
                if (rate > 0)
                {
                    uint32 itemchancef = rate/50;
                    if (itemchancef > 45)
                        itemchancef = 45; // Max. 45%

                    bool item = (urand(0, 100) <= itemchancef) ? true : false;

                    if (!item) // Нет Item = Отправить деньги
                    {
                        std::string subject = "Тотализатор";
                        std::string text    = "Вы выиграли Гонку, денежный приз!";
                        int32 money = (int32)(rate * 2.5f + (int32)rate/10)*10000;

                        MailSender sender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM);
                        SQLTransaction trans = CharacterDatabase.BeginTransaction();

                        MailDraft(subject, text)
                            .AddMoney(money)
                            .SendMailTo(trans, MailReceiver(player,GUID_LOPART(player->GetGUID())),sender);

                        CharacterDatabase.CommitTransaction(trans);
                    }
                    else
                    {
                        uint32 chance = urand(0, 100);
                        uint32 item_id = 0;
                        uint32 rate_ch = urand(0, 100);
                        uint32 rate_c = 0;

                        if (chance <= 13)
                            item_id = 49426; // Эмблема льда
                        else
                            item_id = 47241; // Эмблема Триумфа
                        
                        if (rate_ch <= 90)
                            rate_c = 1;
                        else
                            rate_c = 10;

                        std::string subject = "Тотализатор";
                        std::string text    = "Вы выиграли Гонку - ваш приз!";
                        int32 money = (int32)(rate * 2.5f + (int32)rate/10)*10000;

                        MailSender sender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM);
                        MailDraft draft(subject, text);

                        SQLTransaction trans = CharacterDatabase.BeginTransaction();

                        if (Item* item = Item::CreateItem(item_id, rate_c, player))
                        {
                            item->SaveToDB(trans);
                            draft.AddItem(item);
                        }
                        draft.AddMoney(money);

                        draft.SendMailTo(trans, MailReceiver(player,GUID_LOPART(player->GetGUID())),sender);
                        CharacterDatabase.CommitTransaction(trans);
                    }

                    player->SaveToDB();

                    // награда
                    /*char buff[2048];
                    sprintf(buff, "- %s выиграл(а) что-то.", player->GetName());
                    me->MonsterSay(buff, LANG_UNIVERSAL, NULL);*/
                }
        }

        void ClearRates()
        {
            Rates.clear();    
        }

        void StartTotalizator()
        {
            bStarted = true;
            int i = 0;
            for (std::vector<Creature*>::const_iterator itr = TotalizatorMobs.begin(); itr != TotalizatorMobs.end(); ++itr)
            {
                if (!(*itr))
                    continue;

                float speed = (rand()/(float(RAND_MAX)+1)) * (2.4f - 0.12f) + 0.12f;//* (max - min) + min;

                (*itr)->SetHomePosition(PosMobEnd[i].GetPositionX(), PosMobEnd[i].GetPositionY(), PosMobEnd[i].GetPositionZ(), 0.88f);
                (*itr)->SetSpeed(MOVE_WALK, speed, true);
                (*itr)->SetSpeed(MOVE_RUN, speed, true);

                /*char buff[2048];
                sprintf(buff, "%s %f", (*itr)->GetName(), speed);
                me->MonsterSay(buff, LANG_UNIVERSAL, NULL);*/

                (*itr)->GetMotionMaster()->MoveTargetedHome();
                ++i;
            }
        }

        void StopTotalizator()
        {
            bStarted = false;
            for (std::vector<Creature*>::const_iterator itr = TotalizatorMobs.begin(); itr != TotalizatorMobs.end(); ++itr)
            {
                if (!(*itr))
                    continue;

                (*itr)->GetMotionMaster()->Clear(true);
                (*itr)->StopMoving();
            }
        }

        void ResetTotalizator()
        {
            bStarted = false;

            int i = 0;
            for (std::vector<Creature*>::const_iterator itr = TotalizatorMobs.begin(); itr != TotalizatorMobs.end(); ++itr)
            {
                if (!(*itr))
                    continue;

                (*itr)->SetHomePosition(PosMobStart[i].GetPositionX(),PosMobStart[i].GetPositionY(),PosMobStart[i].GetPositionZ(), 0.88f);
                (*itr)->GetMotionMaster()->MoveTargetedHome();
                ++i;
            }
        }

        void Reached(uint64 uiGUID, uint32 uiEntry)
        {
            for (std::vector<Creature*>::const_iterator itr = TotalizatorMobs.begin(); itr != TotalizatorMobs.end(); ++itr)
            {
                if (!(*itr))
                    continue;

                if (uiGUID == (*itr)->GetGUID() && bStarted)
                {
                    StopTotalizator();
                    uiWinner = uiGUID;

                    switch (uiEntry) //creature_template entry
                    {
                    case 99110:
                        uiWinnerIdx = 1; // Котэ
                        break;
                    case 99111:
                        uiWinnerIdx = 2; // Паукан
                        break;
                    case 99112:
                        uiWinnerIdx = 3; // Танчик
                        break;
                    case 99113:
                        uiWinnerIdx = 4; // Саламандра 
                        break;
                    case 99114:
                        uiWinnerIdx = 5; // Мургл
                        break;
                    case 99115:
                        uiWinnerIdx = 6; // Ангелочек
                        break;
                    default:
                        uiWinnerIdx = 0;
                        break;
                    }

                    (*itr)->CastSpell((*itr), SPELL_FIREWORK, true);

                    char buff[2048];
                    sprintf(buff, "У нас есть победитель! %s выиграл(а) гонку!", (*itr)->GetName());
                    me->MonsterSay(buff, LANG_UNIVERSAL, NULL);

                    ProcessRates(uiWinnerIdx);

                    bReset = false;
                    bAnnounce5 = false;
                    bAnnounce4 = false;
                    bAnnounce3 = false;
                    bAnnounce2 = false;
                    bAnnounce1 = false;
                    bAnnounce0 = false;
                }
                else if (!bStarted)
                {
                    (*itr)->SetOrientation(0.88f);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new custom_totalizator_controlerAI(pCreature);
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {

        if(CAST_AI(custom_totalizator_controler::custom_totalizator_controlerAI, pCreature->AI())->IsStarted())
        {
            pCreature->MonsterWhisper("Нельзя делать ставки после старта!", pPlayer->GetGUID());
            pPlayer->CLOSE_GOSSIP_MENU();
            return false;
        }

        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Ваша ставка в ЗОЛОТЕ:", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        //pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "DEBUG: Вознаграждение", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();

        switch (uiAction)
        {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "На кого вы хотите сделать ставку? 1 ед. = 1 золотому", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

            pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "Котэ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+11, "Сколько золота вы хотите положить на ставку?", 0, true);
            pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "Паукан", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+12, "Сколько золота вы хотите положить на ставку?", 0, true);
            pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "Танчик", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+13, "Сколько золота вы хотите положить на ставку?", 0, true);
            pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "Саламандра", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+14, "Сколько золота вы хотите положить на ставку?", 0, true);
            pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "Мургл", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+15, "Сколько золота вы хотите положить на ставку?", 0, true);
            pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "Ангелочек", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+16, "Сколько золота вы хотите положить на ставку?", 0, true);

            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->CLOSE_GOSSIP_MENU();
            //CAST_AI(custom_totalizator_controler::custom_totalizator_controlerAI, pCreature->AI())->RewardRate(pPlayer);
            break;
        }

        return true;
    }

    bool OnGossipSelectCode(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction, const char* code)
    {
        pPlayer->PlayerTalkClass->ClearMenus();

        if(CAST_AI(custom_totalizator_controler::custom_totalizator_controlerAI, pCreature->AI())->IsStarted())
        {
            pCreature->MonsterWhisper("Нельзя делать ставки после старта!", pPlayer->GetGUID());
            pPlayer->CLOSE_GOSSIP_MENU();
            return false;
        }

        if (uiSender == GOSSIP_SENDER_MAIN)
        {
            if (uiAction > GOSSIP_ACTION_INFO_DEF+10 && uiAction < GOSSIP_ACTION_INFO_DEF+17 && atoi(code) > 0)
            {
                if (pPlayer->GetMoney() >= (uint32)atol(code)*10000)
                {
                    pPlayer->ModifyMoney(-1 * (int32)atoi(code)*10000);
                    uint32 idx = uiAction - 1010;
                    CAST_AI(custom_totalizator_controler::custom_totalizator_controlerAI, pCreature->AI())->AddRate(pPlayer, (uint32)atol(code), idx);

                    char* mobname = "";
                    switch (idx) //creature_template entry
                    {
                    case 1: mobname = "Котэ"; break;
                    case 2: mobname = "Паукан"; break;
                    case 3: mobname = "Танчик"; break;
                    case 4: mobname = "Саламандра"; break;
                    case 5: mobname = "Мургл"; break;
                    case 6: mobname = "Ангелочек"; break;
                    default: mobname = "";
                    }
                    char buff[2048];
                    sprintf(buff, "Вы поставили: %s золота, на гонщика %s. Ожидайте старта...", code, mobname);
                    pCreature->MonsterWhisper(buff, pPlayer->GetGUID());
                }
                else
                    pCreature->MonsterWhisper("Вы не можете позволить это себе!", pPlayer->GetGUID());
                pPlayer->CLOSE_GOSSIP_MENU();
            }
            return true;
        }

        return false;
    }
};

class custom_totalizator_participant : public CreatureScript
{
public:
    custom_totalizator_participant() : CreatureScript("custom_totalizator_participant") { }

    struct custom_totalizator_participantAI : public ScriptedAI
    {
        custom_totalizator_participantAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
        }

        void JustReachedHome()
        {
            if (Creature* cControler = me->FindNearestCreature(99116, 500.0f))
                CAST_AI(custom_totalizator_controler::custom_totalizator_controlerAI, cControler->AI())->Reached(me->GetGUID(), me->GetEntry());
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new custom_totalizator_participantAI(pCreature);
    }
};

void AddSC_custom_totalizator()
{
    new custom_totalizator_controler();
    new custom_totalizator_participant();
}

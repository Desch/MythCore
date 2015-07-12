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

enum eEnums
{
    SAY_PROGRESS_1      = -1000264,
    SAY_PROGRESS_2      = -1000265,
    SAY_PROGRESS_3      = -1000266,
    EMOTE_PROGRESS_4    = -1000267,
    SAY_AGGRO           = -1000268,
    SAY_PROGRESS_5      = -1000269,
    SAY_PROGRESS_6      = -1000270,
    SAY_PROGRESS_7      = -1000271,
    EMOTE_PROGRESS_8    = -1000272,
    SAY_PROGRESS_9      = -1000273,

    QUEST_SUNKEN_TREASURE   = 665,
    MOB_VENGEFUL_SURGE      = 2776
};

class npc_professor_phizzlethorpe : public CreatureScript
{
    public:
        npc_professor_phizzlethorpe() : CreatureScript("npc_professor_phizzlethorpe") { }

        struct npc_professor_phizzlethorpeAI : public npc_escortAI
        {
            npc_professor_phizzlethorpeAI(Creature* pCreature) : npc_escortAI(pCreature) { }

            void WaypointReached(uint32 uiPointId)
            {
                Player* player = GetPlayerForEscort();

                if(!player)
                    return;

                switch(uiPointId)
                {
                    case 4:DoScriptText(SAY_PROGRESS_2, me, player);break;
                    case 5:DoScriptText(SAY_PROGRESS_3, me, player);break;
                    case 8:DoScriptText(EMOTE_PROGRESS_4, me);break;
                    case 9:
                    {
                        me->SummonCreature(MOB_VENGEFUL_SURGE, -2052.96f, -2142.49f, 20.15f, 1.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
                        me->SummonCreature(MOB_VENGEFUL_SURGE, -2052.96f, -2142.49f, 20.15f, 1.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
                        break;
                    }
                    case 10:DoScriptText(SAY_PROGRESS_5, me, player);break;
                    case 11:
                        DoScriptText(SAY_PROGRESS_6, me, player);
                        SetRun();
                        break;
                    case 19:DoScriptText(SAY_PROGRESS_7, me, player); break;
                    case 20:
                        DoScriptText(EMOTE_PROGRESS_8, me);
                        DoScriptText(SAY_PROGRESS_9, me, player);
                        if(player)
                            CAST_PLR(player)->GroupEventHappens(QUEST_SUNKEN_TREASURE, me);
                        break;
                }
            }

            void JustSummoned(Creature* summoned)
            {
                summoned->AI()->AttackStart(me);
            }

            void EnterCombat(Unit* /*pWho*/)
            {
                DoScriptText(SAY_AGGRO, me);
            }

            void UpdateAI(const uint32 diff)
            {
                npc_escortAI::UpdateAI(diff);
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_professor_phizzlethorpeAI(pCreature);
        }

        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
        {
            if(quest->GetQuestId() == QUEST_SUNKEN_TREASURE)
            {
                DoScriptText(SAY_PROGRESS_1, creature, player);
                if(npc_escortAI* pEscortAI = CAST_AI(npc_professor_phizzlethorpeAI, (creature->AI())))
                    pEscortAI->Start(false, false, player->GetGUID(), quest);

                creature->setFaction(113);
            }
            return true;
        }
};

void AddSC_arathi_highlands()
{
    new npc_professor_phizzlethorpe;
}

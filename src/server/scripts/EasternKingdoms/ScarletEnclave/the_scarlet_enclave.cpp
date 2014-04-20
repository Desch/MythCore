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

#define SPELL_REVIVE 51918
#define VALK_WHISPER "It is not yet your time, champion. Rise! Rise and fight once more!"

class npc_valkyr_battle_maiden : public CreatureScript
{
public:
    npc_valkyr_battle_maiden() : CreatureScript("npc_valkyr_battle_maiden") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_valkyr_battle_maidenAI(pCreature);
    }

    struct npc_valkyr_battle_maidenAI : public PassiveAI
    {
        npc_valkyr_battle_maidenAI(Creature* c) : PassiveAI(c) { }

        uint32 FlyBackTimer;
        float x, y, z;
        uint32 phase;

        void Reset()
        {
            me->setActive(true);
            me->SetVisible(false);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetFlying(true);
            FlyBackTimer = 500;
            phase = 0;

            me->GetPosition(x, y, z);
            z += 4.0f;
            x -= 3.5f;
            y -= 5.0f;
            me->GetMotionMaster()->Clear(false);
            me->GetMap()->CreatureRelocation(me, x, y, z, 0.0f);
        }

        void UpdateAI(const uint32 diff)
        {
            if(FlyBackTimer <= diff)
            {
                Player* player = NULL;
                if(me->isSummon())
                    if(Unit* summoner = me->ToTempSummon()->GetSummoner())
                        if(summoner->GetTypeId() == TYPEID_PLAYER)
                            player = CAST_PLR(summoner);

                if(!player)
                    phase = 3;

                switch(phase)
                {
                    case 0:
                        me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                        me->HandleEmoteCommand(EMOTE_STATE_FLYGRABCLOSED);
                        FlyBackTimer = 500;
                        break;
                    case 1:
                        player->GetClosePoint(x, y, z, me->GetObjectSize());
                        z += 2.5f;
                        x -= 2.0f;
                        y -= 1.5f;
                        me->GetMotionMaster()->MovePoint(0, x, y, z);
                        me->SetTarget(player->GetGUID());
                        me->SetVisible(true);
                        FlyBackTimer = 4500;
                        break;
                    case 2:
                        if(!player->isRessurectRequested())
                        {
                            me->HandleEmoteCommand(EMOTE_ONESHOT_CUSTOM_SPELL_01);
                            DoCast(player, SPELL_REVIVE, true);
                            me->MonsterWhisper(VALK_WHISPER, player->GetGUID());
                        }
                        FlyBackTimer = 5000;
                        break;
                    case 3:
                        me->SetVisible(false);
                        FlyBackTimer = 3000;
                        break;
                    case 4:
                        me->DisappearAndDie();
                        break;
                    default:
                        //Nothing To DO
                        break;
                }
                ++phase;
            } else FlyBackTimer-=diff;
        }
    };
};

class npc_acherus_teleporter :  public CreatureScript
{
public:
    npc_acherus_teleporter() : CreatureScript("npc_acherus_teleporter") {}

    CreatureAI* GetAI(Creature* pCreature)
    {
        return new npc_acherus_teleporterAI(pCreature);
    }

private:
    struct npc_acherus_teleporterAI : public Scripted_NoMovementAI
    {
        npc_acherus_teleporterAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) { }

        uint32 check_timer;
        uint32 check2_timer;

        void Reset()
        {
            check_timer = 5000;
            check2_timer = 300000;
        }

        void UpdateAI(const uint32 diff)
        {
            if(check_timer < diff)
            {
                switch(me->GetEntry())
                {
                //Map 609
                case 29580:
                    if(!me->HasAuraEffect(54700,0))
                        me->CastSpell(me,54700,true);
                    break;
                case 29581:
                    if(!me->HasAuraEffect(54724,0))
                        me->CastSpell(me,54724,true);
                    break;
                //Map 0
                case 29589:
                    if(!me->HasAuraEffect(54745,0))
                        me->CastSpell(me,54745,true);
                    break;
                case 29588:
                    if(!me->HasAuraEffect(54742,0))
                        me->CastSpell(me,54742,true);
                    break;
                }

                check_timer = 5000;
            } else check_timer -= diff;

            if(check2_timer < diff)
            {
                me->RemoveAurasDueToSpell(54700);
                me->RemoveAurasDueToSpell(54745);
                me->RemoveAurasDueToSpell(54724);
                me->RemoveAurasDueToSpell(54744);
                check2_timer = 300000;
            } else check2_timer -= diff;
        }
    };
};

void AddSC_the_scarlet_enclave()
{
    new npc_valkyr_battle_maiden;
    new npc_acherus_teleporter;
}

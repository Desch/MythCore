/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Copyright (C) 2012 SymphonyArt <http://symphonyart.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "blackrock_spire.h"

#define ALTAR 175706

enum Spells
{
    SPELL_FIRENOVA                  = 23462,
    SPELL_FLAMEBUFFET               = 23341,
    SPELL_PYROBLAST                 = 17274,
};

enum Events
{
    EVENT_FIRENOVA                  = 1,
    EVENT_FLAMEBUFFET               = 2,
    EVENT_PYROBLAST                 = 3,
};

class boss_pyroguard_emberseer : public CreatureScript
{
public:
    boss_pyroguard_emberseer() : CreatureScript("boss_pyroguard_emberseer") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_pyroguard_emberseerAI(pCreature);
    }

    struct boss_pyroguard_emberseerAI : public BossAI
    {
        boss_pyroguard_emberseerAI(Creature* pCreature) : BossAI(pCreature, DATA_PYROGAURD_EMBERSEER)
        {
            pInstance = pCreature->GetInstanceScript();
            AltarGUID = pInstance->GetData64(ALTAR);
            pAltar = pInstance->instance->GetGameObject(AltarGUID);
        }

        InstanceScript* pInstance;
        GameObject* pAltar;
        uint64 AltarGUID;

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_FIRENOVA, 6*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_FLAMEBUFFET, 3*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_PYROBLAST, 14*IN_MILLISECONDS);
        }

        void JustSummoned(Unit* /*pUnit*/)
        {
            pAltar->SetGoState(GO_STATE_ACTIVE);
            pAltar->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE|GO_FLAG_INTERACT_COND|GO_FLAG_IN_USE);
        }

        void JustDied(Unit* /*pWho*/)
        {
            _JustDied();
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            events.Update(diff);

            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_FIRENOVA:
                        DoCast(me->getVictim(), SPELL_FIRENOVA);
                        events.ScheduleEvent(EVENT_FIRENOVA, 6*IN_MILLISECONDS);
                        break;
                    case EVENT_FLAMEBUFFET:
                        DoCast(me->getVictim(), SPELL_FLAMEBUFFET);
                        events.ScheduleEvent(EVENT_FLAMEBUFFET, 14*IN_MILLISECONDS);
                        break;
                    case EVENT_PYROBLAST:
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            DoCast(target, SPELL_PYROBLAST);
                        events.ScheduleEvent(EVENT_PYROBLAST, 15*IN_MILLISECONDS);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_pyroguard_emberseer()
{
    new boss_pyroguard_emberseer;
}

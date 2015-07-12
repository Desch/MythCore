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
#include "naxxramas.h"

#define SAY_AGGRO           RAND(-1533109,-1533110,-1533111)
#define SAY_SLAY            -1533112
#define SAY_TAUNT           RAND(-1533113,-1533114,-1533115,-1533116,-1533117)
#define SAY_DEATH           -1533118

#define SPELL_SPELL_DISRUPTION  29310
#define SPELL_DECREPIT_FEVER    RAID_MODE(29998, 55011)
#define SPELL_PLAGUE_CLOUD      29350

enum Events
{
    EVENT_NONE,
    EVENT_DISRUPT,
    EVENT_FEVER,
    EVENT_ERUPT,
    EVENT_PHASE
};

enum Phases
{
    PHASE_FIGHT = 1,
    PHASE_DANCE
};

enum Achievment
{
    ACHIEVEMENT_THE_SAFETY_DANCE_10 = 1996,
    ACHIEVEMENT_THE_SAFETY_DANCE_25 = 2139
};

#define DATA_SAFETY_DANCE 19962139

class boss_heigan : public CreatureScript
{
public:
    boss_heigan() : CreatureScript("boss_heigan") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_heiganAI(pCreature);
    }

    struct boss_heiganAI : public BossAI
    {
        boss_heiganAI(Creature* c) : BossAI(c, BOSS_HEIGAN) { }

        uint32 eruptSection;
        bool eruptDirection;
        bool safetyDance;
        Phases phase;

        void Reset()
        {
            _Reset();
        }

        void KilledUnit(Unit* pWho)
        {
            if(!(rand()%5))
                DoScriptText(SAY_SLAY, me);
            if(pWho->GetTypeId() == TYPEID_PLAYER)
                safetyDance = false;
        }

        void SetData(uint32 id, uint32 data)
        {
            if(id == DATA_SAFETY_DANCE)
                safetyDance = data ? true : false;
        }

        uint32 GetData(uint32 type)
        {
            if(type == DATA_SAFETY_DANCE)
                return safetyDance ? 1 : 0;

            return 0;
        }

        void JustDied(Unit* /*pKiller*/)
        {
            _JustDied();
            DoScriptText(SAY_DEATH, me);

            if(instance && GetData(DATA_SAFETY_DANCE) == 1)
                instance->DoCompleteAchievement(RAID_MODE(ACHIEVEMENT_THE_SAFETY_DANCE_10, ACHIEVEMENT_THE_SAFETY_DANCE_25));
        }

        void TeleportHeiganCheaters()
        {
            float x, y, z;
            me->GetPosition(x, y, z);

            uint64 tempDoorGuid_1 = instance->GetData64(DATA_GO_ROOM_HEIGAN);
            uint64 tempDoorGuid_2 = instance->GetData64(DATA_GO_PASSAGE_HEIGAN);
            std::list<HostileReference*> &m_threatlist = me->getThreatManager().getThreatList();
            for(std::list<HostileReference*>::iterator itr = m_threatlist.begin(); itr != m_threatlist.end(); ++itr)
            if((*itr)->getTarget()->GetTypeId() == TYPEID_PLAYER)
                if(Player* player = (*itr)->getTarget()->ToPlayer())
                {
                    if(GameObject* door_1 = GameObject::GetGameObject(*me,tempDoorGuid_1))
                    {
                        if(player->GetPositionX() > door_1->GetPositionX())
                            player->NearTeleportTo(x, y, z, 0);

                        continue;
                    }

                    if(GameObject* door_2 = GameObject::GetGameObject(*me,tempDoorGuid_2))
                    {
                        if(player->GetPositionY() < door_2->GetPositionY())
                            player->NearTeleportTo(x, y, z, 0);

                        continue;
                    }
                }
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            _EnterCombat();
            TeleportHeiganCheaters();
            DoScriptText(SAY_AGGRO, me);
            EnterPhase(PHASE_FIGHT);
            safetyDance = true;
        }

        void EnterPhase(Phases newPhase)
        {
            phase = newPhase;
            events.Reset();
            eruptSection = 3;
            if(phase == PHASE_FIGHT)
            {
                me->GetMotionMaster()->MoveChase(me->getVictim());
                events.ScheduleEvent(EVENT_DISRUPT, urand(10000, 25000));
                events.ScheduleEvent(EVENT_FEVER, urand(15000, 20000));
                events.ScheduleEvent(EVENT_PHASE, 90000);
                events.ScheduleEvent(EVENT_ERUPT, 15000);
            } else {
                float x, y, z, o;
                me->GetHomePosition(x, y, z, o);
                me->NearTeleportTo(x, y, z, o);
                me->GetMotionMaster()->MoveIdle();
                DoCastAOE(SPELL_PLAGUE_CLOUD);
                events.ScheduleEvent(EVENT_PHASE, 45000);
                events.ScheduleEvent(EVENT_ERUPT, 8000);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim() || !CheckInRoom())
                return;

            events.Update(diff);

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_DISRUPT:
                        DoCastAOE(SPELL_SPELL_DISRUPTION);
                        events.ScheduleEvent(EVENT_DISRUPT, urand(5000, 10000));
                        break;
                    case EVENT_FEVER:
                        DoCastAOE(SPELL_DECREPIT_FEVER);
                        events.ScheduleEvent(EVENT_FEVER, urand(20000, 25000));
                        break;
                    case EVENT_PHASE:
                        EnterPhase(phase == PHASE_FIGHT ? PHASE_DANCE : PHASE_FIGHT);
                        break;
                    case EVENT_ERUPT:
                        instance->SetData(DATA_HEIGAN_ERUPT, eruptSection);
                        TeleportHeiganCheaters();

                        if(eruptSection == 0)
                            eruptDirection = true;
                        else if(eruptSection == 3)
                            eruptDirection = false;

                        eruptDirection ? ++eruptSection : --eruptSection;

                        events.ScheduleEvent(EVENT_ERUPT, phase == PHASE_FIGHT ? 10000 : 4000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

class spell_heigan_eruption : public SpellScriptLoader
{
    public:
        spell_heigan_eruption() : SpellScriptLoader("spell_heigan_eruption") { }

        class spell_heigan_eruption_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_heigan_eruption_SpellScript);

            void HandleScript(SpellEffIndex /*eff*/)
            {
                Unit* caster = GetCaster();
                if(!caster || !GetHitPlayer())
                    return;

                if(GetHitDamage() >= int32(GetHitPlayer()->GetHealth()))
                    if(InstanceScript* instance = caster->GetInstanceScript())
                        if(Creature* Heigan = ObjectAccessor::GetCreature(*caster, instance->GetData64(DATA_HEIGAN)))
                            Heigan->AI()->SetData(DATA_SAFETY_DANCE, 0);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_heigan_eruption_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_heigan_eruption_SpellScript();
        }
};

class achievement_safety_dance : public AchievementCriteriaScript
{
    public:
        achievement_safety_dance() : AchievementCriteriaScript("achievement_safety_dance") { }

        bool OnCheck(Player* /*pPlayer*/, Unit* target)
        {
            if(!target)
                return false;

            if(Creature* Heigan = target->ToCreature())
                if(Heigan->AI()->GetData(DATA_SAFETY_DANCE) == 1)
                    return true;

            return false;
        }
};

void AddSC_boss_heigan()
{
    new boss_heigan;
    new spell_heigan_eruption;
    new achievement_safety_dance;
}
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

enum eSpells
{
    SPELL_FIREBALL              = 69583,
    SPELL_HELLFIRE              = 69586,
    SPELL_TACTICAL_BLINK        = 69584,
    SPELL_FROST_BREATH          = 69527,
    SPELL_BLINDING_DIRT         = 70302,
    SPELL_PUNCTURE_WOUND        = 70278,
    SPELL_SHOVELLED             = 69572,
    SPELL_LEAPING_FACE_MAUL     = 69504,
};

enum eEvents
{
    EVENT_FIREBALL              = 1,
    EVENT_TACTICAL_BLINK        = 2,

    EVENT_BLINDING_DIRT         = 3,
    EVENT_PUNCTURE_WOUND        = 4,
    EVENT_SHOVELLED             = 5,
};

class mob_ymirjar_flamebearer : public CreatureScript
{
public:
    mob_ymirjar_flamebearer() : CreatureScript("mob_ymirjar_flamebearer") { }

    struct mob_ymirjar_flamebearerAI: public ScriptedAI
    {
        mob_ymirjar_flamebearerAI(Creature* pCreature): ScriptedAI(pCreature) { }

        void Reset()
        {
                _events.Reset();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
                _events.ScheduleEvent(EVENT_FIREBALL, 4000);
                _events.ScheduleEvent(EVENT_TACTICAL_BLINK, 15000);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

                _events.Update(diff);

            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;

                while(uint32 eventId = _events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_FIREBALL:
                            if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_FIREBALL);
                            _events.RescheduleEvent(EVENT_FIREBALL, 5000);
                            break;
                    case EVENT_TACTICAL_BLINK:
                            if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_TACTICAL_BLINK);
                            DoCast(me, SPELL_HELLFIRE);
                            _events.RescheduleEvent(EVENT_TACTICAL_BLINK, 12000);
                            break;
                        default:
                            break;
                }
            }

            DoMeleeAttackIfReady();
        }

        private:
            EventMap _events;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_ymirjar_flamebearerAI(pCreature);
    }
};

class mob_iceborn_protodrake : public CreatureScript
{
public:
    mob_iceborn_protodrake() : CreatureScript("mob_iceborn_protodrake") { }

    struct mob_iceborn_protodrakeAI: public ScriptedAI
    {
        mob_iceborn_protodrakeAI(Creature* pCreature): ScriptedAI(pCreature), _vehicle(pCreature->GetVehicleKit())
        {
            ASSERT(_vehicle);
        }

        void Reset()
        {
            _frostBreathCooldown = 5000;
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            _vehicle->RemoveAllPassengers();
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(_frostBreathCooldown < diff)
            {
                DoCastVictim(SPELL_FROST_BREATH);
                _frostBreathCooldown = 10000;
            }
            else
                _frostBreathCooldown -= diff;

            DoMeleeAttackIfReady();
        }

        private:
            Vehicle* _vehicle;
            uint32 _frostBreathCooldown;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_iceborn_protodrakeAI(pCreature);
    }
};

class mob_wrathbone_laborer : public CreatureScript
{
public:
    mob_wrathbone_laborer() : CreatureScript("mob_wrathbone_laborer") { }

    struct mob_wrathbone_laborerAI: public ScriptedAI
    {
        mob_wrathbone_laborerAI(Creature* pCreature): ScriptedAI(pCreature) { }

        void Reset()
        {
            _events.Reset();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            _events.ScheduleEvent(EVENT_BLINDING_DIRT, 8000);
            _events.ScheduleEvent(EVENT_PUNCTURE_WOUND, 9000);
            _events.ScheduleEvent(EVENT_SHOVELLED, 5000);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

                _events.Update(diff);

            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;

            while(uint32 eventId = _events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_BLINDING_DIRT:
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 10.0f, true))
                            DoCast(target, SPELL_BLINDING_DIRT);
                        _events.RescheduleEvent(EVENT_BLINDING_DIRT, 10000);
                        return;
                    case EVENT_PUNCTURE_WOUND:
                        DoCastVictim(SPELL_PUNCTURE_WOUND);
                        _events.RescheduleEvent(EVENT_PUNCTURE_WOUND, 9000);
                        return;
                    case EVENT_SHOVELLED:
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, -5.0f))
                            DoCast(target, SPELL_SHOVELLED);
                        _events.RescheduleEvent(EVENT_SHOVELLED, 7000);
                        return;
                }
            }

            DoMeleeAttackIfReady();
        }

        private:
            EventMap _events;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_wrathbone_laborerAI(pCreature);
    }
};

class mob_geist_ambusher : public CreatureScript
{
public:
    mob_geist_ambusher() : CreatureScript("mob_geist_ambusher") { }

    struct mob_geist_ambusherAI: public ScriptedAI
    {
        mob_geist_ambusherAI(Creature* pCreature): ScriptedAI(pCreature) { }

        void Reset()
        {
            _leapingFaceMaulCooldown = 36000;
        }

        void MoveInLineOfSight(Unit* who)
        {
            if(who->GetTypeId() != TYPEID_PLAYER)
                return;

            if(me->IsWithinDistInMap(who, 10.0f))
                DoCast(who, SPELL_LEAPING_FACE_MAUL);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

                if(_leapingFaceMaulCooldown < diff)
                {
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 12000, 4.0f, true))
                        DoCast(target, SPELL_LEAPING_FACE_MAUL);
                    _leapingFaceMaulCooldown = urand(36000, 48000);
                }
                else
                    _leapingFaceMaulCooldown -= diff;

            DoMeleeAttackIfReady();
        }

        private:
            uint32 _leapingFaceMaulCooldown;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_geist_ambusherAI(pCreature);
    }
};

class spell_trash_mob_glacial_strike : public SpellScriptLoader
{
public:
    spell_trash_mob_glacial_strike() : SpellScriptLoader("spell_trash_mob_glacial_strike") { }

    class spell_trash_mob_glacial_strike_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_trash_mob_glacial_strike_AuraScript);

        void PeriodicTick(AuraEffect const* /*aurEff*/)
        {
            if(GetTarget()->IsFullHealth())
            {
                GetTarget()->RemoveAura(GetId(), AURA_REMOVE_BY_ENEMY_SPELL);
                PreventDefaultAction();
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_trash_mob_glacial_strike_AuraScript::PeriodicTick, EFFECT_2, SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_trash_mob_glacial_strike_AuraScript();
    }
};

void AddSC_pit_of_saron()
{
    new mob_ymirjar_flamebearer;
    new mob_wrathbone_laborer;
    new mob_iceborn_protodrake;
    new mob_geist_ambusher;
    new spell_trash_mob_glacial_strike;
}
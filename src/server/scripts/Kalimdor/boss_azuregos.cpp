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

#define SAY_TELEPORT            -1000100

#define SPELL_MARKOFFROST        23182
#define SPELL_MANASTORM          21097
#define SPELL_CHILL              21098
#define SPELL_FROSTBREATH        21099
#define SPELL_REFLECT            22067
#define SPELL_CLEAVE              8255                      //Perhaps not right ID
#define SPELL_ENRAGE             23537

class boss_azuregos : public CreatureScript
{
public:
    boss_azuregos() : CreatureScript("boss_azuregos") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_azuregosAI(pCreature);
    }

    struct boss_azuregosAI : public ScriptedAI
    {
        boss_azuregosAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 MarkOfFrost_Timer;
        uint32 ManaStorm_Timer;
        uint32 Chill_Timer;
        uint32 Breath_Timer;
        uint32 Teleport_Timer;
        uint32 Reflect_Timer;
        uint32 Cleave_Timer;
        uint32 Enrage_Timer;
        bool Enraged;

        void Reset()
        {
            MarkOfFrost_Timer = 35000;
            ManaStorm_Timer = 5000 + rand()%12000;
            Chill_Timer = 10000 + rand()%20000;
            Breath_Timer = 2000 + rand()%6000;
            Teleport_Timer = 30000;
            Reflect_Timer = 15000 + rand()%15000;
            Cleave_Timer = 7000;
            Enrage_Timer = 0;
            Enraged = false;
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if(!UpdateVictim())
                return;

            if(Teleport_Timer <= diff)
            {
                DoScriptText(SAY_TELEPORT, me);
                std::list<HostileReference*>& m_threatlist = me->getThreatManager().getThreatList();
                std::list<HostileReference*>::const_iterator i = m_threatlist.begin();
                for(i = m_threatlist.begin(); i!= m_threatlist.end(); ++i)
                {
                    Unit* pUnit = Unit::GetUnit((*me), (*i)->getUnitGuid());
                    if(pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
                    {
                        DoTeleportPlayer(pUnit, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()+3, pUnit->GetOrientation());
                    }
                }

                DoResetThreat();
                Teleport_Timer = 30000;
            } else Teleport_Timer -= diff;

            if(Chill_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CHILL);
                Chill_Timer = 13000 + rand()%12000;
            } else Chill_Timer -= diff;

            if(Breath_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FROSTBREATH);
                Breath_Timer = 10000 + rand()%5000;
            } else Breath_Timer -= diff;

            if(ManaStorm_Timer <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_MANASTORM);
                ManaStorm_Timer = 7500 + rand()%5000;
            } else ManaStorm_Timer -= diff;

            if(Reflect_Timer <= diff)
            {
                DoCast(me, SPELL_REFLECT);
                Reflect_Timer = 20000 + rand()%15000;
            } else Reflect_Timer -= diff;

            if(Cleave_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CLEAVE);
                Cleave_Timer = 7000;
            } else Cleave_Timer -= diff;

            if(HealthBelowPct(26) && !Enraged)
            {
                DoCast(me, SPELL_ENRAGE);
                Enraged = true;
            }

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_azuregos()
{
    new boss_azuregos;
}

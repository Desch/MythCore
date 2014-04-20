/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: boss_archaedas
SD%Complete: 100
SDComment: Archaedas is activated when 1 person (was 3, changed in 3.0.8) clicks on his altar.
Every 10 seconds he will awaken one of his minions along the wall.
At 66%, he will awaken the 6 Guardians.
At 33%, he will awaken the Vault Walkers
On his death the vault door opens.
EndScriptData */

#include "ScriptPCH.h"
#include "uldaman.h"

#define SAY_AGGRO           "Who dares awaken Archaedas? Who dares the wrath of the makers!"
#define SOUND_AGGRO         5855

#define SAY_SUMMON          "Awake ye servants, defend the discs!"
#define SOUND_SUMMON        5856

#define SAY_SUMMON2         "To my side, brothers. For the makers!"
#define SOUND_SUMMON2       5857

#define SAY_KILL            "Reckless mortal."
#define SOUND_KILL          5858

enum eSpells
{
    SPELL_GROUND_TREMOR              = 6524,
    SPELL_ARCHAEDAS_AWAKEN           = 10347,
    SPELL_BOSS_OBJECT_VISUAL         = 11206,
    SPELL_BOSS_AGGRO                 = 10340,
    SPELL_SUB_BOSS_AGGRO             = 11568,
    SPELL_AWAKEN_VAULT_WALKER        = 10258,
    SPELL_AWAKEN_EARTHEN_GUARDIAN    = 10252,
};

class boss_archaedas : public CreatureScript
{
public:
    boss_archaedas() : CreatureScript("boss_archaedas") { }

    struct boss_archaedasAI : public ScriptedAI
    {
        boss_archaedasAI(Creature* pCreature): ScriptedAI(pCreature)
        {
            pInstance = me->GetInstanceScript();
        }

        uint32 uiTremorTimer;
        int32  iAwakenTimer;
        uint32 uiWallMinionTimer;
        bool bWakingUp;

        bool bGuardiansAwake;
        bool bVaultWalkersAwake;
        InstanceScript* pInstance;

        void Reset()
        {
            uiTremorTimer = 60000;
            iAwakenTimer = 0;
            uiWallMinionTimer = 10000;

            bWakingUp = false;
            bGuardiansAwake = false;
            bVaultWalkersAwake = false;

            if(pInstance)
                pInstance->SetData(0, 5);    // respawn any dead minions
            me->setFaction(35);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
        }

        void ActivateMinion(uint64 uiGuid, bool bFlag)
        {
            Unit* pMinion = Unit::GetUnit(*me, uiGuid);

            if(pMinion && pMinion->isAlive())
            {
                DoCast(pMinion, SPELL_AWAKEN_VAULT_WALKER, bFlag);
                pMinion->CastSpell(pMinion, SPELL_ARCHAEDAS_AWAKEN, true);
            }
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            me->setFaction(14);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
        }

        void SpellHit(Unit* /*pCaster*/, const SpellEntry* spell)
        {
            // Being woken up from the altar, start the awaken sequence
            if(spell == GetSpellStore()->LookupEntry(SPELL_ARCHAEDAS_AWAKEN))
            {
                me->MonsterYell(SAY_AGGRO, LANG_UNIVERSAL, 0);
                DoPlaySoundToSet(me, SOUND_AGGRO);
                iAwakenTimer = 4000;
                bWakingUp = true;
            }
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            me->MonsterYell(SAY_KILL, LANG_UNIVERSAL, 0);
            DoPlaySoundToSet(me, SOUND_KILL);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!pInstance)
                return;
            // we're still doing awaken animation
            if(bWakingUp && iAwakenTimer >= 0)
            {
                iAwakenTimer -= diff;
                return;        // dont do anything until we are done
            } else if(bWakingUp && iAwakenTimer <= 0) {
                bWakingUp = false;
                AttackStart(Unit::GetUnit(*me, pInstance->GetData64(0)));
                return;     // dont want to continue until we finish the AttackStart method
            }
             //Return since we have no target
            if(!UpdateVictim())
                return;

            // wake a wall minion
            if(uiWallMinionTimer <= diff)
            {
                pInstance->SetData (DATA_MINIONS, IN_PROGRESS);

                uiWallMinionTimer = 10000;
            } else uiWallMinionTimer -= diff;

            //If we are <66 summon the guardians
            if(!bGuardiansAwake && !HealthAbovePct(66))
            {
                ActivateMinion(pInstance->GetData64(5), true);   // EarthenGuardian1
                ActivateMinion(pInstance->GetData64(6), true);   // EarthenGuardian2
                ActivateMinion(pInstance->GetData64(7), true);   // EarthenGuardian3
                ActivateMinion(pInstance->GetData64(8), true);   // EarthenGuardian4
                ActivateMinion(pInstance->GetData64(9), true);   // EarthenGuardian5
                ActivateMinion(pInstance->GetData64(10), false); // EarthenGuardian6
                me->MonsterYell(SAY_SUMMON, LANG_UNIVERSAL, 0);
                DoPlaySoundToSet(me, SOUND_SUMMON);
                bGuardiansAwake = true;
            }

            //If we are <33 summon the vault walkers
            if(!bVaultWalkersAwake && !HealthAbovePct(33))
            {
                ActivateMinion(pInstance->GetData64(1), true);    // VaultWalker1
                ActivateMinion(pInstance->GetData64(2), true);    // VaultWalker2
                ActivateMinion(pInstance->GetData64(3), true);    // VaultWalker3
                ActivateMinion(pInstance->GetData64(4), false);   // VaultWalker4
                me->MonsterYell(SAY_SUMMON2, LANG_UNIVERSAL, 0);
                DoPlaySoundToSet(me, SOUND_SUMMON2);
                bVaultWalkersAwake = true;
            }

            if(uiTremorTimer <= diff)
            {
                //Cast
                DoCast(me->getVictim(), SPELL_GROUND_TREMOR);

                //45 seconds until we should cast this agian
                uiTremorTimer  = 45000;
            } else uiTremorTimer  -= diff;

            DoMeleeAttackIfReady();
        }

        void JustDied (Unit* /*pKiller*/)
        {
            if(pInstance)
            {
                pInstance->SetData(DATA_ANCIENT_DOOR, DONE);      // open the vault door
                pInstance->SetData(DATA_MINIONS, SPECIAL);        // deactivate his minions
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_archaedasAI(pCreature);
    }
};

#define SPELL_ARCHAEDAS_AWAKEN  10347

class mob_archaedas_minions : public CreatureScript
{
public:
    mob_archaedas_minions() : CreatureScript("mob_archaedas_minions") { }

    struct mob_archaedas_minionsAI : public ScriptedAI
    {
        mob_archaedas_minionsAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = me->GetInstanceScript();
        }

        uint32 uiArcing_Timer;
        int32 iAwakenTimer;
        bool bWakingUp;

        bool bAmIAwake;
        InstanceScript* pInstance;

        void Reset()
        {
            uiArcing_Timer = 3000;
            iAwakenTimer = 0;

            bWakingUp = false;
            bAmIAwake = false;

            me->setFaction(35);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->RemoveAllAuras();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            me->setFaction (14);
            me->RemoveAllAuras();
            me->RemoveFlag (UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->RemoveFlag (UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            bAmIAwake = true;
        }

        void SpellHit (Unit* /*pCaster*/, const SpellEntry* pSpell)
        {
            // time to wake up, start animation
            if(pSpell == GetSpellStore()->LookupEntry(SPELL_ARCHAEDAS_AWAKEN))
            {
                iAwakenTimer = 5000;
                bWakingUp = true;
            }
        }

        void MoveInLineOfSight(Unit* who)
        {
            if(bAmIAwake)
                ScriptedAI::MoveInLineOfSight(who);
        }

        void UpdateAI(const uint32 diff)
        {
            // we're still in the awaken animation
            if(bWakingUp && iAwakenTimer >= 0)
            {
                iAwakenTimer -= diff;
                return;        // dont do anything until we are done
            } else if(bWakingUp && iAwakenTimer <= 0) {
                bWakingUp = false;
                bAmIAwake = true;
                return;     // dont want to continue until we finish the AttackStart method
            }

            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_archaedas_minionsAI(pCreature);
    }
};

#define SPELL_SELF_DESTRUCT 9874

class mob_stonekeepers : public CreatureScript
{
public:
    mob_stonekeepers() : CreatureScript("mob_stonekeepers") { }

    struct mob_stonekeepersAI : public ScriptedAI
    {
        mob_stonekeepersAI(Creature* pCreature): ScriptedAI(pCreature)
        {
            pInstance = me->GetInstanceScript();
        }

        InstanceScript* pInstance;
         void Reset()
        {
            me->setFaction(35);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->RemoveAllAuras();
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            me->setFaction(14);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*pAttacker*/)
        {
            DoCast (me, SPELL_SELF_DESTRUCT, true);
            if(pInstance)
                pInstance->SetData(DATA_STONE_KEEPERS, IN_PROGRESS);    // activate next stonekeeper
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_stonekeepersAI(pCreature);
    }
};
#define SPELL_BOSS_OBJECT_VISUAL    11206

class go_altar_of_archaedas : public GameObjectScript
{
public:
    go_altar_of_archaedas() : GameObjectScript("go_altar_of_archaedas") { }

    bool OnGossipHello(Player* player, GameObject* /*pGo*/)
    {
        InstanceScript* pInstance = player->GetInstanceScript();
        if(!pInstance)
            return false;

        player->CastSpell(player, SPELL_BOSS_OBJECT_VISUAL, false);
        pInstance->SetData64(0, player->GetGUID());     // Activate Archaedas
        return false;
    }
};

#define SPELL_BOSS_OBJECT_VISUAL    11206

class go_altar_of_the_keepers : public GameObjectScript
{
public:
    go_altar_of_the_keepers() : GameObjectScript("go_altar_of_the_keepers") { }

    bool OnGossipHello(Player* player, GameObject* /*pGo*/)
    {
        InstanceScript* pInstance = player->GetInstanceScript();
        if(!pInstance)
            return false;

        player->CastSpell(player, SPELL_BOSS_OBJECT_VISUAL, false);

        pInstance->SetData(DATA_STONE_KEEPERS, IN_PROGRESS); // activate the Stone Keepers
        return false;
    }
};

void AddSC_boss_archaedas()
{
    new boss_archaedas;
    new mob_archaedas_minions;
    new mob_stonekeepers;
    new go_altar_of_archaedas;
    new go_altar_of_the_keepers;
}
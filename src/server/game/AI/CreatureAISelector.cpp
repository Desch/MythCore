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

#include "Creature.h"
#include "CreatureAISelector.h"
#include "PassiveAI.h"

#include "MovementGenerator.h"
#include "Pet.h"
#include "TemporarySummon.h"
#include "CreatureAIFactory.h"
#include "ScriptMgr.h"

namespace FactorySelector
{
    CreatureAI* selectAI(Creature* pCreature)
    {
        const CreatureAICreator* ai_factory = NULL;
        CreatureAIRegistry& ai_registry(*CreatureAIRepository::instance());

        if(pCreature->isPet())
            ai_factory = ai_registry.GetRegistryItem("PetAI");

        //scriptname in db
        if(!ai_factory)
            if(CreatureAI* scriptedAI = sScriptMgr->GetCreatureAI(pCreature))
                return scriptedAI;

        // AIname in db
        std::string ainame = pCreature->GetAIName();
        if(!ai_factory && !ainame.empty())
            ai_factory = ai_registry.GetRegistryItem(ainame);

        // select by NPC flags
        if(!ai_factory)
        {
            if(pCreature->IsVehicle())
                ai_factory = ai_registry.GetRegistryItem("VehicleAI");
            else if(pCreature->HasUnitTypeMask(UNIT_MASK_CONTROLABLE_GUARDIAN) && ((Guardian*)pCreature)->GetOwner()->GetTypeId() == TYPEID_PLAYER)
                ai_factory = ai_registry.GetRegistryItem("PetAI");
            else if(pCreature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK))
                ai_factory = ai_registry.GetRegistryItem("NullCreatureAI");
            else if(pCreature->isGuard())
                ai_factory = ai_registry.GetRegistryItem("GuardAI");
            else if(pCreature->HasUnitTypeMask(UNIT_MASK_CONTROLABLE_GUARDIAN))
                ai_factory = ai_registry.GetRegistryItem("PetAI");
            else if(pCreature->isTotem())
                ai_factory = ai_registry.GetRegistryItem("TotemAI");
            else if(pCreature->isTrigger())
            {
                if(pCreature->m_spells[0])
                    ai_factory = ai_registry.GetRegistryItem("TriggerAI");
                else
                    ai_factory = ai_registry.GetRegistryItem("NullCreatureAI");
            }
            else if(pCreature->GetCreatureType() == CREATURE_TYPE_CRITTER && !pCreature->HasUnitTypeMask(UNIT_MASK_GUARDIAN))
                ai_factory = ai_registry.GetRegistryItem("CritterAI");
        }

        // select by permit check
        if(!ai_factory)
        {
            int best_val = -1;
            typedef CreatureAIRegistry::RegistryMapType RMT;
            RMT const &l = ai_registry.GetRegisteredItems();
            for(RMT::const_iterator iter = l.begin(); iter != l.end(); ++iter)
            {
                const CreatureAICreator *factory = iter->second;
                const SelectableAI *p = dynamic_cast<const SelectableAI *>(factory);
                ASSERT(p != NULL);
                int val = p->Permit(pCreature);
                if(val > best_val)
                {
                    best_val = val;
                    ai_factory = p;
                }
            }
        }

        // select NullCreatureAI if not another cases
        ainame = (ai_factory == NULL) ? "NullCreatureAI" : ai_factory->key();

        sLog->outDebug(LOG_FILTER_TSCR, "Creature %u used AI is %s.", pCreature->GetGUIDLow(), ainame.c_str());
        return (ai_factory == NULL ? new NullCreatureAI(pCreature) : ai_factory->Create(pCreature));
    }

    MovementGenerator* selectMovementGenerator(Creature* creature)
    {
        MovementGeneratorRegistry& mv_registry(*MovementGeneratorRepository::instance());
        ASSERT(creature->GetCreatureInfo() != NULL);
        const MovementGeneratorCreator *mv_factory = mv_registry.GetRegistryItem(creature->GetDefaultMovementType());

        /* if(mv_factory == NULL)
        {
            int best_val = -1;
            StringVector l;
            mv_registry.GetRegisteredItems(l);
            for(StringVector::iterator iter = l.begin(); iter != l.end(); ++iter)
            {
            const MovementGeneratorCreator *factory = mv_registry.GetRegistryItem((*iter).c_str());
            const SelectableMovement *p = dynamic_cast<const SelectableMovement *>(factory);
            ASSERT(p != NULL);
            int val = p->Permit(creature);
            if(val > best_val)
            {
                best_val = val;
                mv_factory = p;
            }
            }
        }*/

        return (mv_factory == NULL ? NULL : mv_factory->Create(creature));

    }

    GameObjectAI* SelectGameObjectAI(GameObject* go)
    {
        const GameObjectAICreator *ai_factory = NULL;
        GameObjectAIRegistry& ai_registry(*GameObjectAIRepository::instance());

        ai_factory = ai_registry.GetRegistryItem(go->GetAIName());

        //future goAI types go here

        std::string ainame = (ai_factory == NULL || go->GetScriptId()) ? "NullGameObjectAI" : ai_factory->key();

        sLog->outDebug(LOG_FILTER_TSCR, "GameObject %u used AI is %s.", go->GetGUIDLow(), ainame.c_str());

        return (ai_factory == NULL ? new NullGameObjectAI(go) : ai_factory->Create(go));
    }
}


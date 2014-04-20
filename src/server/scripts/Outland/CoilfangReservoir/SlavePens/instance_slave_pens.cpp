/*
 * Copyright (C) 2012 Izb00shka Core <http://izbooshka.net>
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
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

#include "ScriptPCH.h"
#include "slave_pens.h"

#define MAX_ENCOUNTER   1

class instance_slave_pens : public InstanceMapScript
{
public:
    instance_slave_pens() : InstanceMapScript("instance_slave_pens", 547) { }

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new instance_slave_pens_InstanceMapScript(pMap);
    }

    struct instance_slave_pens_InstanceMapScript : public InstanceScript
    {
        instance_slave_pens_InstanceMapScript(Map* pMap) : InstanceScript(pMap) { SetBossNumber(MAX_ENCOUNTER); }

        void OnCreatureCreate(Creature *c)
        {
            switch(c->GetEntry())
            {
            case NPC_FIRE_NOVA_TOTEM:
                c->setFaction(35);
                break;
            }
        }
    };
};

void AddSC_instance_slave_pens()
{
    new instance_slave_pens();
}
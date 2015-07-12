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

#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "ulduar.h"
#include "InstanceScript.h"

/*
The teleporter appears to be active and stable.

- Expedition Base Camp
- Formation Grounds
- Colossal Forge
- Scrapyard
- Antechamber of Ulduar
- Shattered Walkway
- Conservatory of Life
*/

enum UlduarTeleporter
{
    BASE_CAMP                                    = 200,
    GROUNDS                                      = 201,
    FORGE                                        = 202,
    SCRAPYARD                                    = 203,
    ANTECHAMBER                                  = 204,
    WALKWAY                                      = 205,
    CONSERVATORY                                 = 206,
    SPARK_IMAGINATION                            = 207,
    DESCENT_MADNESS                              = 208
};

class ulduar_teleporter : public GameObjectScript
{
    public:
        ulduar_teleporter() : GameObjectScript("ulduar_teleporter") { }

        bool OnGossipSelect(Player* player, GameObject* /*gameObject*/, uint32 sender, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            if(sender != GOSSIP_SENDER_MAIN)
                return false;
            if(!player->getAttackers().empty())
                return false;

            switch(action)
            {
                case BASE_CAMP:
                    player->TeleportTo(603, -706.122f, -92.6024f, 429.876f, 0.0f);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case GROUNDS:
                    player->TeleportTo(603, 131.248f, -35.3802f, 409.804f, 0.0f);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case FORGE:
                    player->TeleportTo(603, 553.233f, -12.3247f, 409.679f, 0.0f);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case SCRAPYARD:
                    player->TeleportTo(603, 926.292f, -11.4635f, 418.595f, 0.0f);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case ANTECHAMBER:
                    player->TeleportTo(603, 1498.09f, -24.246f, 420.967f, 0.0f);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case WALKWAY:
                    player->TeleportTo(603, 1859.45f, -24.1f, 448.9f, 0.0f);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case CONSERVATORY:
                    player->TeleportTo(603, 2086.27f, -24.3134f, 421.239f, 0.0f);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case SPARK_IMAGINATION:
                    player->TeleportTo(603, 2518.16f, 2569.03f, 412.299f, 0);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case DESCENT_MADNESS:
                    player->TeleportTo(603, 1854.82f, -11.5608f, 334.175f, 0);
                    player->CLOSE_GOSSIP_MENU();
                    break;
            }

            return true;
        }

        bool OnGossipHello(Player* player, GameObject* gameObject)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Téléportation vers le Camp de base de l'Expédition.", GOSSIP_SENDER_MAIN, BASE_CAMP);
            if(InstanceScript* instance = gameObject->GetInstanceScript())
            {
                if(instance->GetData(DATA_COLOSSUS) >= 2 || player->isGameMaster()) //count of 2 collossus death
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Téléportation vers le Champ d'Entraînement.", GOSSIP_SENDER_MAIN, GROUNDS);
                if(instance->GetBossState(BOSS_LEVIATHAN) == DONE || player->isGameMaster())
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Téléportation vers la Forge Colossale.", GOSSIP_SENDER_MAIN, FORGE);
                if(instance->GetBossState(BOSS_XT002) == DONE || player->isGameMaster())
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Téléportation vers la Ferraillerie.", GOSSIP_SENDER_MAIN, SCRAPYARD);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Téléportation vers L'Antichambre d'Ulduar.", GOSSIP_SENDER_MAIN, ANTECHAMBER);
                }
                if(instance->GetBossState(BOSS_KOLOGARN) == DONE || player->isGameMaster())
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Téléportation vers le Passage Brisé.", GOSSIP_SENDER_MAIN, WALKWAY);
                if(instance->GetBossState(BOSS_AURIAYA) == DONE || player->isGameMaster())
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Téléportation vers le Conservatoire de Vie.", GOSSIP_SENDER_MAIN, CONSERVATORY);
                if(instance->GetBossState(BOSS_MIMIRON) == FAIL || instance->GetBossState(BOSS_MIMIRON) == DONE || player->isGameMaster())
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Téléportation vers l'étincelle de l'Imagination", GOSSIP_SENDER_MAIN, SPARK_IMAGINATION);
                    if(instance->GetBossState(BOSS_VEZAX) == DONE || player->isGameMaster())
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Téléportation vers la Descente dans la Folie", GOSSIP_SENDER_MAIN, DESCENT_MADNESS);
                }
            }

            player->SEND_GOSSIP_MENU(gameObject->GetGOInfo()->GetGossipMenuId(), gameObject->GetGUID());
            return true;
        }
};

void AddSC_ulduar_teleporter()
{
    new ulduar_teleporter;
}

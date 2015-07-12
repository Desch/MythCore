

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "InstanceSaveMgr.h"

std::string GetTimeStringA(uint64 time)
{
    uint64 days = time / DAY, hours = (time % DAY) / HOUR, minute = (time % HOUR) / MINUTE;
    std::ostringstream ss;
    if (days) ss << days << "d ";
    if (hours) ss << hours << "h ";
    ss << minute << "m";
    return ss.str();
}


class Item_reset_instance : public ItemScript
{
    public:

        Item_reset_instance()
            : ItemScript("item_reset_instance")
        {
        }

        bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)
        {
			if(!player)
				return false;

			uint16 counter = 0;
			uint16 MapId = 0;
			int8 diff = -1;
            for(uint8 i = 0; i < MAX_DIFFICULTY; ++i)
			{
				Player::BoundInstancesMap &binds = player->GetBoundInstances(Difficulty(i));
				for(Player::BoundInstancesMap::iterator itr = binds.begin(); itr != binds.end();)
				{
					InstanceSave *save = itr->second.save;
					if(itr->first != player->GetMapId() && (!MapId || MapId == itr->first) && (diff == -1 || diff == save->GetDifficulty()))
					{
						std::string timeleft = GetTimeStringA(save->GetResetTime() - time(NULL));
						sLog->outDetail("unbinding map: %d inst: %d perm: %s diff: %d canReset: %s TTR: %s", itr->first, save->GetInstanceId(), itr->second.perm ? "yes" : "no", save->GetDifficulty(), save->CanReset() ? "yes" : "no", timeleft.c_str());
						player->UnbindInstance(itr, Difficulty(i));
						counter++;
					}
					else
						++itr;
				}
			}

			sLog->outDetail("instances unbound: %d", counter);
			if(counter)
				player->DestroyItemCount(item->GetEntry(),1,true);
            return true;
        }
};

void AddSC_Item_reset_instance()
{
    new Item_reset_instance();
}
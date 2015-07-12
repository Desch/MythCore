#include "ScriptPCH.h" 
#include "ScriptMgr.h" 
#include "Config.h" 
 
class duel_reset : public PlayerScript 
{ 
public: 
    duel_reset() : PlayerScript("duel_reset") {} 
 
    void OnDuelEnd(Player *winner, Player *looser, DuelCompleteType type) 
    { 
        if(sWorld->getBoolConfig(CONFIG_DUEL_RESET_COOLDOWN_ON_FINISH)) 
        {
            if(sWorld->getBoolConfig(CONFIG_DUEL_RESET_ONLY_IN_WE_NEED)) 
            {
                if(winner->GetAreaId() == 4570 || winner->GetAreaId() == 4556 || winner->GetAreaId() == 12 || winner->GetAreaId() == 14)
                { 
                    if(type == DUEL_WON)
                    { 
                        winner->RemoveArenaSpellCooldowns(); 
                        looser->RemoveArenaSpellCooldowns();
                        if(sWorld->getBoolConfig(CONFIG_DUEL_RESET_HP_MP_RESTORE))
                        {
                            winner->SetHealth(winner->GetMaxHealth()); 
                            looser->SetHealth(looser->GetMaxHealth()); 

                            if(winner->getPowerType() == POWER_MANA)    
                            winner->SetPower(POWER_MANA, winner->GetMaxPower(POWER_MANA)); 
                            if(looser->getPowerType() == POWER_MANA)    
                            looser->SetPower(POWER_MANA, looser->GetMaxPower(POWER_MANA)); 
                        }
                    } 
                    winner->HandleEmoteCommand(EMOTE_ONESHOT_CHEER); 
                } 
            } 
 
            else 
                if (type == DUEL_WON) 
                { 
                    winner->RemoveArenaSpellCooldowns(); 
                    looser->RemoveArenaSpellCooldowns(); 
                    if(sWorld->getBoolConfig(CONFIG_DUEL_RESET_HP_MP_RESTORE))
                    {
                        winner->SetHealth(winner->GetMaxHealth()); 
                        looser->SetHealth(looser->GetMaxHealth()); 

                        if(winner->getPowerType() == POWER_MANA)    
                            winner->SetPower(POWER_MANA, winner->GetMaxPower(POWER_MANA)); 
                        if(looser->getPowerType() == POWER_MANA)    
                            looser->SetPower(POWER_MANA, looser->GetMaxPower(POWER_MANA)); 
                    }
                } 
                winner->HandleEmoteCommand(EMOTE_ONESHOT_CHEER); 
        } 
    } 
 
    void OnDuelStart(Player *player1, Player *player2) 
    { 
        if(sWorld->getBoolConfig(CONFIG_DUEL_RESET_COOLDOWN_ON_START)) 
        { 
            if(sWorld->getBoolConfig(CONFIG_DUEL_RESET_ONLY_IN_WE_NEED)) 
            { 
                if(player1->GetAreaId() == 4570 || player1->GetAreaId() == 4556 || player1->GetAreaId() == 12 || player1->GetAreaId() == 14) 
                { 
                    player1->RemoveArenaSpellCooldowns(); 
                    player2->RemoveArenaSpellCooldowns();

                    if(sWorld->getBoolConfig(CONFIG_DUEL_RESET_HP_MP_RESTORE))
                    {
                        player1->SetHealth(player1->GetMaxHealth());
                        player2->SetHealth(player2->GetMaxHealth());

                        if(player1->getPowerType() == POWER_MANA)
                            player1->SetPower(POWER_MANA, player1->GetMaxPower(POWER_MANA));
                        if(player2->getPowerType() == POWER_MANA) 
                            player2->SetPower(POWER_MANA, player2->GetMaxPower(POWER_MANA));

                        if(sWorld->getBoolConfig(CONFIG_DUEL_RESET_COOLDOWN_RESET_ENERGY_ON_START)) 
                        { 
                            switch (player1->getPowerType()) 
                            { 
                            case POWER_RAGE: 
                                player1->SetPower(POWER_RAGE, 0); 
                                break; 
                            case POWER_RUNIC_POWER: 
                                player1->SetPower(POWER_RUNIC_POWER, 0); 
                                break; 
                            default: 
                                break; 
                            } 
     
                            switch (player2->getPowerType()) 
                            { 
                            case POWER_RAGE: 
                                player2->SetPower(POWER_RAGE, 0); 
                                break; 
                            case POWER_RUNIC_POWER: 
                                player2->SetPower(POWER_RUNIC_POWER, 0); 
                                break; 
                            default: 
                                break; 
                            }
                        }
                    }
                } 
            } 
 
            else 
            { 
                player1->RemoveArenaSpellCooldowns(); 
                player2->RemoveArenaSpellCooldowns();

                if(sWorld->getBoolConfig(CONFIG_DUEL_RESET_HP_MP_RESTORE))
                {
                    player1->SetHealth(player1->GetMaxHealth()); 
                    player2->SetHealth(player2->GetMaxHealth()); 
                    
                    if(player1->getPowerType() == POWER_MANA)
                        player1->SetPower(POWER_MANA, player1->GetMaxPower(POWER_MANA));
                    if(player2->getPowerType() == POWER_MANA) 
                        player2->SetPower(POWER_MANA, player2->GetMaxPower(POWER_MANA));
                }
            } 
        } 
    } 
}; 
 
void AddSC_DuelReset() 
{ 
    new duel_reset(); 
}  
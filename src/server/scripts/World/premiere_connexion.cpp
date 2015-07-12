#include "ScriptPCH.h"


class bienvenue_players_script : public PlayerScript
{
        public:
                bienvenue_players_script() : PlayerScript("bienvenue_players_script") { }

                char msg[255];
                void OnLogin(Player* player)
                {
                        if (player->GetLevelPlayedTime() <=  90)
                        {
                                sprintf(msg, "Bonjour %s ! Toutes l'équipe d'Aeternam-Warlord te souhaite la bienvenue sur le Royaume. Pour rejoindre le channel général tapes /join 5 ", player->GetName());
                                sWorld->SendWorldText(LANG_AUTO_BROADCAST, msg);
                        }
                }
};

void AddSC_bienvenue()
{
        new bienvenue_players_script;
}

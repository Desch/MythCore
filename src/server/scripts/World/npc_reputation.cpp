#include "ScriptPCH.h"
#include "Config.h"
#include "UnicodeConvert.h"

#define EM_SCEAU_DE_CHAMPION 44990

extern LoginDatabaseWorkerPool LoginDatabase;
extern CharacterDatabaseWorkerPool CharacterDatabase;

class npc_reputation : public CreatureScript
{
public:
	npc_reputation() : CreatureScript("npc_reputation") { }

	bool OnGossipHello(Player * pPlayer, Creature * _Creature)
	{
		pPlayer->ADD_GOSSIP_ITEM(6, "Devenir exalte au Verdict des Cendres pour < 10 > sceaux de champion" , GOSSIP_SENDER_MAIN, 2);
		pPlayer->ADD_GOSSIP_ITEM(6, "Devenir exalte aux Fils de Hodir pour < 10 > sceaux de champion" , GOSSIP_SENDER_MAIN, 3);
		pPlayer->ADD_GOSSIP_ITEM(6, "Devenir exalte au Chevaliers de la Lame d'ebene pour < 10 > sceaux de champion" , GOSSIP_SENDER_MAIN, 4);
		pPlayer->ADD_GOSSIP_ITEM(6, "Devenir exalte au Kirin Thor pour < 10 > sceaux de champion" , GOSSIP_SENDER_MAIN, 5);
		pPlayer->ADD_GOSSIP_ITEM(6, "Devenir exalte avec l'Accord du Repos du Ver pour < 10 > sceaux de champion" , GOSSIP_SENDER_MAIN, 14);
		pPlayer->ADD_GOSSIP_ITEM(6, "Devenir exalte a la Croisade d'Argent pour < 10 > sceaux de champion" , GOSSIP_SENDER_MAIN, 6);
		pPlayer->ADD_GOSSIP_ITEM(6, "Devenir exalte aux Kalu'aks pour < 10 > sceaux de champion" , GOSSIP_SENDER_MAIN, 7);
		pPlayer->ADD_GOSSIP_ITEM(6, "Devenir exalte aux Oracles pour < 10 > sceaux de champion" , GOSSIP_SENDER_MAIN, 8);
		pPlayer->ADD_GOSSIP_ITEM(6, "Devenir exalte a la Tribu Frenecoeur pour < 10 > sceaux de champion" , GOSSIP_SENDER_MAIN, 9);
		pPlayer->ADD_GOSSIP_ITEM(6, "Devenir exalte a l'Expedition de la Horde pour < 10 > sceaux de champion" , GOSSIP_SENDER_MAIN, 12);
        pPlayer->ADD_GOSSIP_ITEM(6, "Devenir exalte a l'Expedition de la Bravoure pour < 10 > sceaux de champion" , GOSSIP_SENDER_MAIN, 10);
        pPlayer->ADD_GOSSIP_ITEM(6, "Devenir exalte a la Horde pour < 50 > sceaux de champion" , GOSSIP_SENDER_MAIN, 13);
	    pPlayer->ADD_GOSSIP_ITEM(6, "Devenir exalte a l'Alliance pour < 50 > sceaux de champion" , GOSSIP_SENDER_MAIN, 11);
		pPlayer->ADD_GOSSIP_ITEM(6, "Quitter", GOSSIP_SENDER_MAIN, 15);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player *pPlayer, Creature *_Creature, uint32 sender, uint32 action )
	{
		// Main menu
		char msg[255];
		if (sender == GOSSIP_SENDER_MAIN)
		{
			QueryResult result ;
			uint32 count = 0;
			switch(action)
			{
			case 2:// Verdicte des cendres
			{
			//result = CharacterDatabase.PQuery("SELECT standing FROM character_reputation WHERE guid = %u and faction = %u", pPlayer->GetGUID(), 1156);
		//	Field *fields = result->Fetch();
		//	count = fields[0].GetUInt32();
		    //pCreature->MonsterWhisper(count, pPlayer->GetGUID());
			//sLog->outString(count);
			//	sLog->outChat("pPlayer->GetReputation(1156) %u",pPlayer->GetReputation(1156));
			    if (pPlayer->GetReputation(	1156) < 42000)
			    {
                    if (pPlayer->HasItemCount(EM_SCEAU_DE_CHAMPION, 10 ))
                    {
                        ChatHandler chathandler(pPlayer);
					    pPlayer->SetReputation(1156, 42999);
                        pPlayer->DestroyItemCount(EM_SCEAU_DE_CHAMPION, 10 , true);
                        char mess[] = "Vous êtes maintenant exalté auprès du Verdict des cendres";
                        ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                        _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());

                    }
                    else
                    {
						 char mess[] = "Nombre de sceaux de champions requis insuffisant!";
                         ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                         _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                      //  _Creature->MonsterTextEmote("Nombre de sceaux de champions requis insuffisant!", pPlayer->GetGUID());
                    }
			    }
			    else
			    {
							  char mess[] = "Vous avez déjà cette réputation exalté";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
			    }
				break;
			}
			case 3:// Fils de Hodir
			{
			  //  result = CharacterDatabase.PQuery("SELECT standing FROM character_reputation WHERE guid = %u and faction = %u", pPlayer->GetGUID(), 1119);
			  //	 Field *fields = result->Fetch();
			 //    count = fields[0].GetUInt32();
				// sLog->outChat("[ADDON] Player %s sends: %u",count);
			    if (pPlayer->GetReputation(1119) < 42000)
			    {
                    if (pPlayer->HasItemCount(EM_SCEAU_DE_CHAMPION, 10 ))
                    {
                        ChatHandler chathandler(pPlayer);
                        pPlayer->SetReputation(1119, 42999);
                        pPlayer->DestroyItemCount(EM_SCEAU_DE_CHAMPION, 10 , true);
                            char mess[] = "Vous êtes maintenant exalté auprès des Fils de Hodir";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                        

                    }
                    else
                    {
                       		 char mess[] = "Nombre de sceaux de champions requis insuffisant!";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                    }
			    }
			    else
			    {
			       			 char mess[] = "Vous avez déjà cette réputation exalté";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
			    }
				break;
			}
			case 4:// Chevaliers de la Lame d''ébène
			{
			   // result = CharacterDatabase.PQuery("SELECT standing FROM character_reputation WHERE guid = %u and faction = %u", pPlayer->GetGUID(), 1098);
				// Field *fields = result->Fetch();
			    // count = fields[0].GetUInt32();
			    if (pPlayer->GetReputation(1098) <  42000)
			    {
                    if (pPlayer->HasItemCount(EM_SCEAU_DE_CHAMPION, 10 ))
                    {
                        ChatHandler chathandler(pPlayer);
                       pPlayer->SetReputation(1098, 42999);
                        
                            pPlayer->DestroyItemCount(EM_SCEAU_DE_CHAMPION, 10 , true);
                            char mess[] = "Vous êtes maintenant exalté auprès des Chevaliers de la Lame d'ébène";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                        

                    }
                    else
                    {
                              char mess[] = "Nombre de sceaux de champions requis insuffisant!";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                    }
			    }
			    else
			    {
			        		 char mess[] = "Vous avez déjà cette réputation exalté";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
			    }
				break;
			}
			case 5:// Kirin Thor
			{
			    //result = CharacterDatabase.PQuery("SELECT standing FROM character_reputation WHERE guid = %u and faction = %u", 1090, pPlayer->GetGUID());
				 //Field *fields = result->Fetch();
			     //count = fields[0].GetUInt32();	
			    if (pPlayer->GetReputation(1090) < 42000)
			    {
                    if (pPlayer->HasItemCount(EM_SCEAU_DE_CHAMPION, 10 ))
                    {
                        ChatHandler chathandler(pPlayer);
                        pPlayer->SetReputation(1090, 42999);
                        pPlayer->DestroyItemCount(EM_SCEAU_DE_CHAMPION, 10 , true);
                        char mess[] = "Vous êtes maintenant exalté auprès des Kirin Thor";
                        ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                        _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                        

                    }
                    else
                    {
                        char mess[] = "Nombre de sceaux de champions requis insuffisant!";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                    }
			    }
			    else
			    {
			         char mess[] = "Vous avez déjà cette réputation exalté";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
			    }
				break;
			}
			case 6:// La Croisade d''argent
			{
			    //result = CharacterDatabase.PQuery("SELECT standing FROM character_reputation WHERE guid = %u and faction = %u", 1106, pPlayer->GetGUID());
				 //Field *fields = result->Fetch();
			     //count = fields[0].GetUInt32();		
			    if (pPlayer->GetReputation(1106) < 42000)
			    {
                    if (pPlayer->HasItemCount(EM_SCEAU_DE_CHAMPION, 10 ))
                    {
                        ChatHandler chathandler(pPlayer);
                       pPlayer->SetReputation(1106, 42999);
                        
                            pPlayer->DestroyItemCount(EM_SCEAU_DE_CHAMPION, 10 , true);
                            char mess[] = "Vous êtes maintenant exalté auprès de la Croisade d'Argent";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                       

                    }
                    else
                    {
                        	 char mess[] = "Nombre de sceaux de champions requis insuffisant!";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                    }
			    }
			    else
			    {
			         char mess[] = "Vous avez déjà cette réputation exalté";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
			    }
				break;
			}
			case 7:// Les Kalu''aks
			{
			    //result = CharacterDatabase.PQuery("SELECT standing FROM character_reputation WHERE guid = %u and faction = %u", 1073, pPlayer->GetGUID());
				// Field *fields = result->Fetch();
			    // count = fields[0].GetUInt32();
			    if (pPlayer->GetReputation(1073) <  42000)
			    {
                    if (pPlayer->HasItemCount(EM_SCEAU_DE_CHAMPION, 10 ))
                    {
                       ChatHandler chathandler(pPlayer);
                       pPlayer->SetReputation(1073, 42999);
                       pPlayer->DestroyItemCount(EM_SCEAU_DE_CHAMPION, 10 , true);
                       char mess[] = "Vous êtes maintenant exalté auprès des Kalu'aks";
                       ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                       _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                        

                    }
                    else
                    {
                      		 char mess[] = "Nombre de sceaux de champions requis insuffisant!";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                    }
			    }
			    else
			    {
			         char mess[] = "Vous avez déjà cette réputation exalté";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
			    }
				break;
			}
			case 8:// Les Oracles
			{
			    //result = CharacterDatabase.PQuery("SELECT standing FROM character_reputation WHERE guid = %u and faction = %u", 1105, pPlayer->GetGUID());
				 //Field *fields = result->Fetch();
			     //count = fields[0].GetUInt32();
			    if (pPlayer->GetReputation(1105) < 42000)
			    {
                    if (pPlayer->HasItemCount(EM_SCEAU_DE_CHAMPION, 10 ))
                    {
                        ChatHandler chathandler(pPlayer);
                       pPlayer->SetReputation(1105, 42999);
                           pPlayer->DestroyItemCount(EM_SCEAU_DE_CHAMPION, 10 , true);
                            char mess[] = "Vous êtes maintenant exalté auprès des Oracles";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                        

                    }
                    else
                    {
                           char mess[] = "Nombre de sceaux de champions requis insuffisant!";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                    }
			    }
			    else
			    {
			         char mess[] = "Vous avez déjà cette réputation exalté";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
			    }
				break;
			}
			case 9:// Tribu Frénécoeur
			{
			    //result = CharacterDatabase.PQuery("SELECT standing FROM character_reputation WHERE guid = %u and faction = %u", 1104, pPlayer->GetGUID());

				// Field *fields = result->Fetch();
			    // count = fields[0].GetUInt32();
				 
				 if (pPlayer->GetReputation(1104) < 42000)
			    {
                    if (pPlayer->HasItemCount(EM_SCEAU_DE_CHAMPION, 10 ))
                    {
                        ChatHandler chathandler(pPlayer);
                        pPlayer->SetReputation(1104, 42999);
                        
                            pPlayer->DestroyItemCount(EM_SCEAU_DE_CHAMPION, 10 , true);
                            char mess[] = "Vous êtes maintenant exalté auprès de la Tribu Frénecoeur";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                       

                    }
                    else
                    {
                            char mess[] = "Nombre de sceaux de champions requis insuffisant!";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                    }
			    }
			    else
			    {
			         char mess[] = "Vous avez déjà cette réputation exalté";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
			    }
				break;
			}
			case 10:// Expédition de la bravoure 
			{
			   
                   // result = CharacterDatabase.PQuery("SELECT standing FROM character_reputation WHERE guid = %u and faction = %u", 1050, pPlayer->GetGUID());
				 //Field *fields = result->Fetch();
			     //count = fields[0].GetUInt32();
                    if (pPlayer->GetReputation(1050) <  42000)
                    {
                        if (pPlayer->HasItemCount(EM_SCEAU_DE_CHAMPION, 10 ))
                        {
                            ChatHandler chathandler(pPlayer);
                            pPlayer->SetReputation(1050, 99999);
                            
                                pPlayer->DestroyItemCount(EM_SCEAU_DE_CHAMPION, 10 , true);
                                char mess[] = "Vous êtes maintenant exalté auprès de l'Expédition de la Bravoure";
                                ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                                _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                           

                        }
                        else
                        {
                              char mess[] = "Nombre de sceaux de champions requis insuffisant!";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                        }
                    }
                    else
                    {
                         char mess[] = "Vous avez déjà cette réputation exalté";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                    }
					break;
			}		
			
           
			case 11:// alliance 
			{
			    
                 //result = CharacterDatabase.PQuery("SELECT standing FROM character_reputation WHERE guid = %u and faction = %u", 72, pPlayer->GetGUID());
				 //Field *fields = result->Fetch();
			     //count = fields[0].GetUInt32();
                    if (pPlayer->GetReputation(72)  < 42000)
                    {
                        if (pPlayer->HasItemCount(EM_SCEAU_DE_CHAMPION, 10 ))
                        {
                            ChatHandler chathandler(pPlayer);
                            pPlayer->SetReputation(72, 99999);
                           
                                pPlayer->DestroyItemCount(EM_SCEAU_DE_CHAMPION, 10 , true);
                                char mess[] = "Vous êtes maintenant exalté auprès de l'Alliance";
                                ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                                _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                            

                        }
                        else
                        {
                           	 char mess[] = "Nombre de sceaux de champions requis insuffisant!";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                        }
                    }
                    else
                    {
                         char mess[] = "Vous avez déjà cette réputation exalté";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                    }
                
                break;
			}
			
			 case 12:   // Expédition de la horde
            {   
                   // result = CharacterDatabase.PQuery("SELECT standing FROM character_reputation WHERE guid = %u and faction = %u", 1052, pPlayer->GetGUID());
				 //Field *fields = result->Fetch();
			     //count = fields[0].GetUInt32();
                    if (pPlayer->GetReputation(1052) < 42000)
                    {
                        if (pPlayer->HasItemCount(EM_SCEAU_DE_CHAMPION, 10 ))
                        {
                            ChatHandler chathandler(pPlayer);
                           pPlayer->SetReputation(1052, 99999);
                            
                                pPlayer->DestroyItemCount(EM_SCEAU_DE_CHAMPION, 10 , true);
                                char mess[] = "Vous êtes maintenant exalté auprès de l'expédition de la Horde";
                                ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                                _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                            

                        }
                        else
                        {
                             char mess[] = "Nombre de sceaux de champions requis insuffisant!";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                        }
                    }
                    else
                    {
                         char mess[] = "Vous avez déjà cette réputation exalté";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                    }
                    break;
                
			}
			
			 case 13:   // Horde
            {   
                   // result = CharacterDatabase.PQuery("SELECT standing FROM character_reputation WHERE guid = %u and faction = %u", 1052, pPlayer->GetGUID());
				 //Field *fields = result->Fetch();
			     //count = fields[0].GetUInt32();
                    if (pPlayer->GetReputation(76) < 42000)
                    {
                        if (pPlayer->HasItemCount(EM_SCEAU_DE_CHAMPION, 10 ))
                        {
                            ChatHandler chathandler(pPlayer);
                           pPlayer->SetReputation(76, 99999);
                            
                                pPlayer->DestroyItemCount(EM_SCEAU_DE_CHAMPION, 10 , true);
                                char mess[] = "Vous êtes maintenant exalté auprès de la Horde";
                                ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                                _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                            

                        }
                        else
                        {
                             char mess[] = "Nombre de sceaux de champions requis insuffisant!";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                        }
                    }
                    else
                    {
                         char mess[] = "Vous avez déjà cette réputation exalté";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                    }
                    break;
                
			}

			case 14:   // Accord du repos du ver
            {   
                   // result = CharacterDatabase.PQuery("SELECT standing FROM character_reputation WHERE guid = %u and faction = %u", 1052, pPlayer->GetGUID());
				 //Field *fields = result->Fetch();
			     //count = fields[0].GetUInt32();
                    if (pPlayer->GetReputation(1091) < 42000)
                    {
                        if (pPlayer->HasItemCount(EM_SCEAU_DE_CHAMPION, 10 ))
                        {
                            ChatHandler chathandler(pPlayer);
                           pPlayer->SetReputation(1091, 99999);
                            
                                pPlayer->DestroyItemCount(EM_SCEAU_DE_CHAMPION, 10 , true);
                                char mess[] = "Vous êtes maintenant exalté auprès de l'Accord du repos du ver";
                                ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                                _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                            

                        }
                        else
                        {
                             char mess[] = "Nombre de sceaux de champions requis insuffisant!";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                        }
                    }
                    else
                    {
                         char mess[] = "Vous avez déjà cette réputation exalté";
                            ConvertASCIIToUTF8(mess,strlen(mess)+1,msg);
                            _Creature->MonsterTextEmote(msg, pPlayer->GetGUID());
                    }
                    break;
                
			}
			
		}
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		return true;
	}
};


void AddSC_reputation()
{
	new npc_reputation();
}

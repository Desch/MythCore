
/*******************************************************
		Event de Hurlevent Attaque capital
 *******************************************************/

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"
#include "GameEventMgr.h"
#include "attaquecapitalA2.h"


//Roi Varan de Hurlevent
class npcevent : public CreatureScript
{
	public:
		npcevent() : CreatureScript("npcevent") {}
		
	struct npceventAI : public npc_escortAI
	{    
		uint32 Phase;
		uint32 uiPhaseTimer, uiStep;
		bool bStepping;
		char msg[255];
		bool debug;

		npceventAI(Creature* creature) : npc_escortAI(creature),summons(me)
        {
            me->SetReactState(REACT_DEFENSIVE);
            //me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
			Reset();
		}
		
		SummonList summons;
		

        void Reset()
		{
			Phase = PHASE_NEUTRE;
			bStepping = false;
			uiStep = 0;
			uiPhaseTimer = 1000;
			debug = true;
		}
		
		void SummonedCreatureDespawn(Creature* summon)
        {
            summons.Despawn(summon);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }
		
		void JustDied(Unit* /*killer*/)
		{
		    summons.DespawnAll();
            sGameEventMgr->StartEvent(91);
            sWorld->ForceGameEventUpdate(); 
        }    

        void JumpToNextStep(uint32 uiTimer)
        {
            uiPhaseTimer = uiTimer;
            ++uiStep;

			if(debug)
			{
				//sprintf(msg, " UiStep : %u", uiStep);
				sLog->outString("DEBUG: ROIuistep est a %u", uiStep);
			}
        }


        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
                case 1:
                        SetEscortPaused(true);
                        me->HandleEmoteCommand(378);
                        uiStep=0;
                        JumpToNextStep(5000);
						break;
				case 3:
                        me->HandleEmoteCommand(378);
						me->MonsterSay("Il vous faudra pour repousser ces deux attaques frontales! Séparer vos forces en deux Groupes.\nUn qui ira consolider Les Portes de Notre Cité, et l'autre qui iront unir leurs forces pour repousser les flottes arrivant sur notre Port.", LANG_UNIVERSAL, 0);
                        me->HandleEmoteCommand(378);
                        SetEscortPaused(true);
                        uiStep=0;
                        JumpToNextStep(8000);
						break;
                case 4:
                        me->SummonCreature(100019, -8486.429688f, 389.700989f, 108.610001f, 5.392970f);
                        break;
				case 5:
                        me->HandleEmoteCommand(378);
						me->MonsterSay("Le groupe qui se portera volontaire pour défendre notre Port de l'invasion des Gobelins seront sous les ordres de Dame Jaina.", LANG_UNIVERSAL, 0);
                        me->HandleEmoteCommand(378);
                        SetEscortPaused(true);
                        uiStep=0;
                        JumpToNextStep(6000);
						break;
                case 7:
                        me->HandleEmoteCommand(378);
                        me->MonsterSay("Le groupe qui se portera volontaire pour défendre Les Portes de Notre Cité de l'invasion des Défias seront sous les ordres du Général Marcus.", LANG_UNIVERSAL, 0);
                        me->HandleEmoteCommand(378);
                        me->PlayDirectSound(3900);// Music intro
                        SetEscortPaused(true);
                        JumpToNextStep(22000);
						break;
			}
		}

		void SetupMovement(uint32 variation)
            {
                switch (variation)
                {
                    case 0:
                        AddWaypoint(1, -8444.54f, 337.17f, 121.74f);
						AddWaypoint(2, -8434.74f, 343.93f, 120.88f);
						AddWaypoint(3, -8436.08f, 342.91f, 121.05f);
                        AddWaypoint(4, -8451.41f, 330.90f, 121.32f);
						AddWaypoint(5, -8449.09f, 332.76f, 121.74f);
						AddWaypoint(6, -8440.50f, 332.16f, 123.19f);
						AddWaypoint(7, -8442.23f, 334.26f, 122.57f);
                        break;
                    default:
                        sLog->outError("Unexpected movement variation (%i) in npceventAI::SetupMovement call!", variation);
                        return;
                }
                    if (npceventAI* pEscortAI = CAST_AI(npceventAI, me->AI()))
                {
                    if (variation > 0)
                        pEscortAI->Start(true, true, NULL, NULL, false, true);
                    else
                    {
                        pEscortAI->Start(true, true);
                        pEscortAI->SetDespawnAtEnd(false);
                    }
                        pEscortAI->SetDespawnAtFar(false);
                        me->setActive(true);
                }
            }


        void UpdateAI(const uint32 diff)
		{
			npc_escortAI::UpdateAI(diff);
			
            if(bStepping)
            {
				if(uiPhaseTimer <= diff)
				{
					switch(uiStep)
					{
						case 0:
						        me->PlayDirectSound(3900);// Music intro
                                me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
								me->HandleEmoteCommand(412);
                                me->MonsterSay("Partisans de l'Alliance, l'heure est grave. Nos ennemies juré les Défias ont réussi à monter contre nous les Gobelins.", LANG_UNIVERSAL, 0);
								JumpToNextStep(500); 
                                break;
                        case 1:
                                SetEscortPaused(false);
                                break;
                        case 2:
                                me->HandleEmoteCommand(412);
                                me->MonsterSay("Les chiens! Dame Jaina est en route à sont arrivée Héros nous formerons les rangs.", LANG_UNIVERSAL, 0);
                                me->SummonCreature(100025, -8504.08f, 353.67f, 120.88f, 4.60f);
                                JumpToNextStep(5000);
                                break;
                        case 3:
                                me->HandleEmoteCommand(412);
                                me->MonsterSay("Général Marcus voici les Héros qui seront sous votre commandemant pour défendre les portes de notre cité.", LANG_UNIVERSAL, 0);
                                JumpToNextStep(25000);
                                break;
                        case 4:
                                me->HandleEmoteCommand(412);
                                me->MonsterSay("Héros Former les Rangs! Les Nains seront nos alliés sur le Champ de bataille, portez leur assistances si nécessaire!", LANG_UNIVERSAL, 0);
                                JumpToNextStep(10000);
                                break;
                        case 5:
                                me->HandleEmoteCommand(412);
                                me->MonsterSay("La défaite n'est pas envisageable Héros! Pour l'Alliance! Pour l'Honneur!", LANG_UNIVERSAL, 0);
                                JumpToNextStep(5000);
                                break;
                        case 6: 
                                me->HandleEmoteCommand(412);
                                me->MonsterSay("Triomphez de nos ennemis, et vous serez grandement récompensés! Hurlevent ne doit pas Tomber!", LANG_UNIVERSAL, 0);
                                me->SetReactState(REACT_DEFENSIVE);  
                                JumpToNextStep(5000); 
                                break;    
					}
				}else uiPhaseTimer -= diff;
			}
		}
	};

       CreatureAI* GetAI(Creature* creature) const
    {
        return new npceventAI(creature);
    }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM( 9, "Nous avons entendu Votre Appel Sir!", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM( 4, "Quitter", GOSSIP_SENDER_MAIN, 2);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}
	
	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action )
	{
		if(sender == GOSSIP_SENDER_MAIN)
		{
			SendDefaultMenu(player, creature, action );
			player->CLOSE_GOSSIP_MENU();
		}
		return true;
	}

	void SendDefaultMenu(Player* player, Creature *creature, uint32 action )
	{
		npceventAI* NPCAI = CAST_AI(npcevent::npceventAI, creature->AI());

		switch(action)
		{
			case 1:
					
					player->CLOSE_GOSSIP_MENU();
                    NPCAI->SetupMovement(0);
					NPCAI->bStepping = true;
					break;
			case 2:
					player->CLOSE_GOSSIP_MENU();
					break;
			default:
					player->CLOSE_GOSSIP_MENU();
					break;
		}
	}
};




//Général Marcus Hurlevent


class npcmarcus : public CreatureScript
{
	public:
		npcmarcus() : CreatureScript("npcmarcus") {}
		
	struct npcmarcusAI : public npc_escortAI
	{
        
		uint32 Phase;
		uint32 uiPhaseTimer, uiStep;
        uint32 uiWave;
		uint32 WavesCounter;
		uint64 uiWaveGUID[ENCOUNTER_WAVES_MAX_SPAWNS];
		bool bStepping;
		char msg[255];
        bool WaveDone;
		bool debug;

		npcmarcusAI(Creature* creature) : npc_escortAI(creature),summons(me)
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
			Reset();
		}
		
		SummonList summons;
	

       void Reset()
		{
			Phase = PHASE_NEUTRE;
			bStepping = false;
			uiStep = 0;
			uiPhaseTimer = 1000;
			debug = true;  
            SetupMovement(0);
            WavesCounter = 0;
			uiWave = 0;
			for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i) uiWaveGUID[i] = 0;
            WaveDone = false;
		}
		
		
		void SummonedCreatureDespawn(Creature* summon)
        {
            summons.Despawn(summon);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }
		

        void SpawnWaveGroup(uint32 waveID, uint64* guidVector)
        {
            for(uint32 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
            {
                if((uint32)HurleventBaseWaves[waveID][i][0] == 0) break;

                if(Creature* pTemp = me->SummonCreature((uint32)HurleventBaseWaves[waveID][i][0], HurleventBaseWaves[waveID][i][1], HurleventBaseWaves[waveID][i][2], HurleventBaseWaves[waveID][i][3], HurleventBaseWaves[waveID][i][4], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000))
                {
                    guidVector[i] = pTemp->GetGUID();
                    
                }
            }
			SendCrierWarning(waveID);
        }


        void SendCrierWarning(uint8 waveNumber)
        {
			char* textId = "";
			
			switch(waveNumber)
			{
				case 0: textId = "1er Assaut aux Portes de Hurlevent en Approche !";
				        me->SummonCreature(100037, -8999.35f, 401.86f, 72.92f, 2.23f);
				        me->SummonCreature(100038, -8995.03f, 400.50f, 72.92f, 2.23f);
				        me->SummonCreature(100039, -8995.75f, 404.55f, 73.12f, 2.23f);
				        me->SummonCreature(100040, -9072.96f, 505.81f, 76.01f, 5.42f);
				        me->SummonCreature(100041, -9077.67f, 506.20f, 76.53f, 5.42f);
				        me->SummonCreature(100042, -9077.82f, 501.85f, 76.84f, 5.42f);
				        break; 
				case 1: textId = "2ème Assaut aux Portes de Hurlevent en Approche !";
				        me->SummonCreature(100037, -8999.35f, 401.86f, 72.92f, 2.23f);
				        me->SummonCreature(100038, -8995.03f, 400.50f, 72.92f, 2.23f);
				        me->SummonCreature(100039, -8995.75f, 404.55f, 73.12f, 2.23f);
				        me->SummonCreature(100040, -9072.96f, 505.81f, 76.01f, 5.42f);
				        me->SummonCreature(100041, -9077.67f, 506.20f, 76.53f, 5.42f);
				        me->SummonCreature(100042, -9077.82f, 501.85f, 76.84f, 5.42f);
				        break;
				case 2: textId = "3ème Assaut aux Portes de Hurlevent en Approche !";
				        me->SummonCreature(100037, -8999.35f, 401.86f, 72.92f, 2.23f);
				        me->SummonCreature(100038, -8995.03f, 400.50f, 72.92f, 2.23f);
				        me->SummonCreature(100039, -8995.75f, 404.55f, 73.12f, 2.23f);
				        me->SummonCreature(100040, -9072.96f, 505.81f, 76.01f, 5.42f);
				        me->SummonCreature(100041, -9077.67f, 506.20f, 76.53f, 5.42f);
				        me->SummonCreature(100042, -9077.82f, 501.85f, 76.84f, 5.42f);
				        break;
				case 3: textId = "4ème Assaut aux Portes de Hurlevent en Approche !";
				        me->SummonCreature(100037, -8999.35f, 401.86f, 72.92f, 2.23f);
				        me->SummonCreature(100038, -8995.03f, 400.50f, 72.92f, 2.23f);
				        me->SummonCreature(100039, -8995.75f, 404.55f, 73.12f, 2.23f);
				        me->SummonCreature(100040, -9072.96f, 505.81f, 76.01f, 5.42f);
				        me->SummonCreature(100041, -9077.67f, 506.20f, 76.53f, 5.42f);
				        me->SummonCreature(100042, -9077.82f, 501.85f, 76.84f, 5.42f);
				        break;
				case 4: textId = "5ème Assaut aux Portes de Hurlevent en Approche !";
				        me->SummonCreature(100037, -8999.35f, 401.86f, 72.92f, 2.23f);
				        me->SummonCreature(100038, -8995.03f, 400.50f, 72.92f, 2.23f);
				        me->SummonCreature(100039, -8995.75f, 404.55f, 73.12f, 2.23f);
				        me->SummonCreature(100040, -9072.96f, 505.81f, 76.01f, 5.42f);
				        me->SummonCreature(100041, -9077.67f, 506.20f, 76.53f, 5.42f);
				        me->SummonCreature(100042, -9077.82f, 501.85f, 76.84f, 5.42f);
				        break;
				case 5: textId = "6ème Assaut aux Portes de Hurlevent en Approche !";
				        me->SummonCreature(100037, -8999.35f, 401.86f, 72.92f, 2.23f);
				        me->SummonCreature(100038, -8995.03f, 400.50f, 72.92f, 2.23f);
				        me->SummonCreature(100039, -8995.75f, 404.55f, 73.12f, 2.23f);
				        me->SummonCreature(100040, -9072.96f, 505.81f, 76.01f, 5.42f);
				        me->SummonCreature(100041, -9077.67f, 506.20f, 76.53f, 5.42f);
				        me->SummonCreature(100042, -9077.82f, 501.85f, 76.84f, 5.42f);
				        break;
				case 6: textId = "7ème Assaut aux Portes de Hurlevent en Approche !";
				        me->SummonCreature(100037, -8999.35f, 401.86f, 72.92f, 2.23f);
				        me->SummonCreature(100038, -8995.03f, 400.50f, 72.92f, 2.23f);
				        me->SummonCreature(100039, -8995.75f, 404.55f, 73.12f, 2.23f);
				        me->SummonCreature(100040, -9072.96f, 505.81f, 76.01f, 5.42f);
				        me->SummonCreature(100041, -9077.67f, 506.20f, 76.53f, 5.42f);
				        me->SummonCreature(100042, -9077.82f, 501.85f, 76.84f, 5.42f);
				        break;
				case 7: textId = "8ème Assaut aux Portes de Hurlevent en Approche !";
				        me->SummonCreature(100037, -8999.35f, 401.86f, 72.92f, 2.23f);
				        me->SummonCreature(100038, -8995.03f, 400.50f, 72.92f, 2.23f);
				        me->SummonCreature(100039, -8995.75f, 404.55f, 73.12f, 2.23f);
				        me->SummonCreature(100040, -9072.96f, 505.81f, 76.01f, 5.42f);
				        me->SummonCreature(100041, -9077.67f, 506.20f, 76.53f, 5.42f);
				        me->SummonCreature(100042, -9077.82f, 501.85f, 76.84f, 5.42f);
				        break;
                case 8: textId = "Attention, Le général des Défias est en approche Préparez-vous !";
                        me->SummonCreature(100037, -8999.35f, 401.86f, 72.92f, 2.23f);
				        me->SummonCreature(100038, -8995.03f, 400.50f, 72.92f, 2.23f);
				        me->SummonCreature(100039, -8995.75f, 404.55f, 73.12f, 2.23f);
				        me->SummonCreature(100040, -9072.96f, 505.81f, 76.01f, 5.42f);
				        me->SummonCreature(100041, -9077.67f, 506.20f, 76.53f, 5.42f);
				        me->SummonCreature(100042, -9077.82f, 501.85f, 76.84f, 5.42f);
                        break;
			}
			
			sWorld->SendWorldText(LANG_SYSTEMMESSAGE, textId);
		}


       void JumpToNextStep(uint32 uiTimer)
        {
            uiPhaseTimer = uiTimer;
            ++uiStep;

			if(debug)
			{
				//sprintf(msg, " UiStep : %u", uiStep);
				sLog->outString("DEBUG: Marcusuistep est a %u", uiStep);
			}
        }

       void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
                case 3:
                        SetEscortPaused(true);
                        me->HandleEmoteCommand(66);
                        uiStep=0;
                        JumpToNextStep(8000);
						break;
                case 4:
                        SetEscortPaused(true);
						me->MonsterYell("Mon Seigneur, nos éclaireurs nous signalent des mouvements massives de troupes Défias arrivant aux Portes de Hurlevent! ", LANG_UNIVERSAL, 0);
                        me->HandleEmoteCommand(378);
                        uiStep=0;
                        JumpToNextStep(5000);
						break;
                case 6: 
                        SetEscortPaused(true);
						me->MonsterYell("Les Défias ont eut vent que nos Forces étaient dispersés dans tous le Royaume de l'Est et profitent de ce moment de faiblesse! ", LANG_UNIVERSAL, 0);
                        me->HandleEmoteCommand(378);
                        uiStep=0;
                        JumpToNextStep(60000);
                        break;       
                case 8:
                        SetEscortPaused(true);
                        me->PlayDirectSound(11708);
						me->MonsterYell("Que le Raid soutenant les Portes de la Cité me suive! ", LANG_UNIVERSAL, 0);
                        me->HandleEmoteCommand(378);
                        JumpToNextStep(12000);                       
                        break;
				case 12:
						SetEscortPaused(true);
						me->MonsterSay("Nous avons du chemin a parcourir, tous sur vos montures, et suivez moi!", LANG_UNIVERSAL, 0);
						me->HandleEmoteCommand(378);
						JumpToNextStep(5000);
						break;
                case 36:
						SetEscortPaused(true);
						me->MonsterSay("Même si nous devons combattre jusqu'à notre derniers souffle, ils ne doivent pas franchir nos portes!", LANG_UNIVERSAL, 0);
						me->HandleEmoteCommand(378);
						JumpToNextStep(3000);
						break;
						

            }
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -8456.42f, 352.02f, 120.65f);
						AddWaypoint(2, -8455.47f, 350.84f, 120.88f);
						AddWaypoint(3, -8447.88f, 341.35f, 120.88f);
                        AddWaypoint(4, -8445.10f, 337.70f, 121.74f);
                        AddWaypoint(5, -8442.91f, 339.76f, 121.74f);
                        AddWaypoint(6, -8442.64f, 337.58f, 122.16f);
                        AddWaypoint(7, -8439.29f, 334.71f, 122.57f);
                        AddWaypoint(8, -8439.60f, 335.10f, 122.57f);
                        AddWaypoint(9, -8452.75f, 351.36f, 120.88f);
                        AddWaypoint(10, -8482.81f, 389.76f, 109.15f);
                        AddWaypoint(11, -8505.37f, 417.90f, 108.38f);
                        AddWaypoint(12, -8545.20f, 462.13f, 104.36f);
                        AddWaypoint(13, -8568.4f, 468.337f, 104.504f);
						AddWaypoint(14, -8583.17f, 482.146f, 104.213f);
						AddWaypoint(15, -8603.67f, 505.009f, 103.721f);
						AddWaypoint(16, -8609.77f, 507.466f, 103.714f);
						AddWaypoint(17, -8629.16f, 531.799f, 100.907f);
						AddWaypoint(18, -8658.65f, 553.873f, 96.9133f);
						AddWaypoint(19, -8681.25f, 547.595f, 97.5431f);
						AddWaypoint(20, -8705.67f, 525.537f, 97.636f);
						AddWaypoint(21, -8713.04f, 520.017f, 97.206f);
						AddWaypoint(22, -8733.59f, 544.436f, 101.129f);
						AddWaypoint(23, -8748.84f, 561.603f, 97.4001f);
						AddWaypoint(24, -8737.77f, 574.672f, 97.4109f);
						AddWaypoint(25, -8770.42f, 609.147f, 97.223f);
						AddWaypoint(26, -8797.18f, 587.918f, 97.3234f);
						AddWaypoint(27, -8827.5f, 625.675f, 93.8948f);
						AddWaypoint(28, -8889.26f, 572.961f, 92.5874f);
						AddWaypoint(29, -8929.35f, 541.402f, 94.312f);
						AddWaypoint(30, -8948.85f, 564.596f, 93.7837f);
						AddWaypoint(31, -8952.81f, 569.59f, 93.7835f);
						AddWaypoint(32, -8977.3f, 551.201f, 93.8421f);
						AddWaypoint(33, -8954.45f, 520.903f, 96.3575f);
						AddWaypoint(34, -9074.69f, 425.621f, 93.0563f);
						AddWaypoint(35, -9062.91f, 414.596f, 93.2958f);
						AddWaypoint(36, -9064.25f, 416.472f, 93.2958f);
                        break;
				default:
                        sLog->outError("Unexpected movement variation (%i) in npcmarcusAI::SetupMovement call!", variation);
                        return;
			}
			if(npcmarcusAI* pEscortAI = CAST_AI(npcmarcusAI, me->AI()))
			{
				pEscortAI->Start(false, true);
				pEscortAI->SetDespawnAtEnd(false);
				pEscortAI->SetDespawnAtFar(false);
				me->setActive(true);
				bStepping = true;
			}
		}


        void UpdateAI(const uint32 diff)
		{
			npc_escortAI::UpdateAI(diff);
			
			if(sGameEventMgr->IsActiveEvent(90))
            {
				if(bStepping)
				{
					if(uiPhaseTimer <= diff)
					{
						switch(uiStep)
						{
							case 0:
									SetRun(true);
									JumpToNextStep(2000); 
									break;
							case 1:
									SetEscortPaused(false);
									break;
							case 2:	//go au Waypoint 12
									SetEscortPaused(false);
									break;
							case 3:
									me->Mount(MOUNT_MODEL);
									me->SetSpeed(MOVE_RUN, SPEED_MOUNT);
									JumpToNextStep(5000);
									break;
							case 4:
									me->MonsterSay("En avant Héros.", LANG_UNIVERSAL, 0);
									JumpToNextStep(10000);
							case 5:
									SetEscortPaused(false);
									break;
							case 6:
									me->Unmount();
									me->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
									me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
									me->SetReactState(REACT_PASSIVE);
									JumpToNextStep(5000);
									break;
							case 7:
						            me->SummonCreature(100044, -9040.17f, 440.30f, 93.29f, 3.50f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000);
						            me->SummonCreature(100044, -9053.37f, 457.79f, 93.28f, 4.00f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000);
						       	    me->MonsterSay("Voici deux cannon encore en état de marchent pour vous aider dans votre Combat", LANG_UNIVERSAL, 0);
						       	    JumpToNextStep(5000);
						       	    break;
						    case 8:
						            me->MonsterSay("Placez-y vos deux meilleurs Artilleurs Héros. Ils vous seront d'un grands soutiens!", LANG_UNIVERSAL, 0);
						       	    JumpToNextStep(5000);
						       	    break;		
							case 9: 
									me->MonsterYell("Que Dieu vous protèges mes Frères, et pas de quartier.", LANG_UNIVERSAL, 0);
									JumpToNextStep(5000);
									me->PlayDirectSound(11803);
									
									if(!WaveDone)
									{
										SpawnWaveGroup(uiWave, uiWaveGUID);
										uiWave++;
										WavesCounter++;
									}

									break;
							case 10:
									if(!WaveDone)
									{
										uint32 mobCounter = 0 , deadCounter = 0;

										for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
										{
											if(uiWaveGUID[i] == 0) break;
											++mobCounter;
											Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
											if(!pTemp || pTemp->isDead()) ++deadCounter;
										} 
										
										if(mobCounter <= deadCounter) JumpToNextStep(0);
										else uiPhaseTimer = 1000;
									}
									else JumpToNextStep(2000);
									break;
							case 11:
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Premier Assaut Stoppé aux Portes de Hurlevent! ");                              
									me->MonsterTextEmote("Premier Assaut Stoppé aux Portes de Hurlevent!", 0, true);
									JumpToNextStep(4000);
									break;
							case 12:
									if(!WaveDone)
									{
										SpawnWaveGroup(uiWave, uiWaveGUID);
										uiWave++;
										WavesCounter++;
									}

									JumpToNextStep(2000);
									break;
							case 13:
									if(!WaveDone)
									{
										uint32 mobCounter = 0, deadCounter = 0;

										for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
										{
											if(uiWaveGUID[i] == 0) break;
											++mobCounter;
											Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
											if(!pTemp || pTemp->isDead()) ++deadCounter;
										}

										if(mobCounter <= deadCounter) JumpToNextStep(0);
										else uiPhaseTimer = 1000;
									}
									else JumpToNextStep(2000);
									break;
							case 14: 
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Deuxième Assaut Stoppé aux Portes de Hurlevent! ");
									me->MonsterTextEmote("Deuxième Assaut Stoppé aux Portes de Hurlevent!", 0, true);
									JumpToNextStep(4000);
									break;
							case 15:
									if(!WaveDone)
									{
										SpawnWaveGroup(uiWave, uiWaveGUID);
										uiWave++;
										WavesCounter++;
									}

									JumpToNextStep(2000);
									me->MonsterYell("Courage Héros, il faut tenir bon!", LANG_UNIVERSAL, 0);
								    me->HandleEmoteCommand(378);
									break;
							case 16:
									if(!WaveDone)
									{
										uint32 mobCounter = 0, deadCounter = 0;

										for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
										{
											if(uiWaveGUID[i] == 0) break;
											++mobCounter;
											Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
											if(!pTemp || pTemp->isDead()) ++deadCounter;
										}

										if(mobCounter <= deadCounter) JumpToNextStep(0);
										else uiPhaseTimer = 1000;
									}
									else JumpToNextStep(2000);
									break;
							case 17: 
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Troisième Assaut Stoppé aux Portes de Hurlevent! ");
									me->MonsterTextEmote("Troisième Assaut Stoppé aux Portes de Hurlevent!", 0, true);
									JumpToNextStep(4000);
									break;
							case 18:
									if(!WaveDone)
									{
										SpawnWaveGroup(uiWave, uiWaveGUID);
										uiWave++;
										WavesCounter++;
									}

									JumpToNextStep(2000);
									break;
							case 19:
									if(!WaveDone)
									{
										uint32 mobCounter = 0, deadCounter = 0;

										for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
										{
											if(uiWaveGUID[i] == 0) break;
											++mobCounter;
											Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
											if(!pTemp || pTemp->isDead()) ++deadCounter;
										}

										if(mobCounter <= deadCounter) JumpToNextStep(0);
										else uiPhaseTimer = 1000;
									}
									else JumpToNextStep(2000);
									break;
							case 20: 
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Quatrième Assaut Stoppé aux Portes de Hurlevent! ");
									me->MonsterTextEmote("Quatrième Assaut Stoppé aux Portes de Hurlevent!", 0, true);
									JumpToNextStep(4000);
									break;
							case 21:
									if(!WaveDone)
									{
										SpawnWaveGroup(uiWave, uiWaveGUID);
										uiWave++;
										WavesCounter++;
									}

									JumpToNextStep(2000);
									me->MonsterYell("Les Défias ne doivent pas franchir nos Portes!!", LANG_UNIVERSAL, 0);
								    me->HandleEmoteCommand(378);
									break;
							case 22:
									if(!WaveDone)
									{
										uint32 mobCounter = 0, deadCounter = 0;

										for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
										{
											if(uiWaveGUID[i] == 0) break;
											++mobCounter;
											Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
											if(!pTemp || pTemp->isDead()) ++deadCounter;
										}

										if(mobCounter <= deadCounter) JumpToNextStep(0);
										else uiPhaseTimer = 1000;
									}
									else JumpToNextStep(2000);
									break;
							case 23:
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Cinquième Assaut Stoppé aux Portes de Hurlevent! "); 
									me->MonsterTextEmote("Cinquième Assaut Stoppé aux Portes de Hurlevent!", 0, true);
									JumpToNextStep(4000);
									break;
							case 24:
									if(!WaveDone)
									{
										SpawnWaveGroup(uiWave, uiWaveGUID);
										uiWave++;
										WavesCounter++;
									}

									JumpToNextStep(2000);
									break;
							case 25:
									if(!WaveDone)
									{
										uint32 mobCounter = 0, deadCounter = 0;

										for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
										{
											if(uiWaveGUID[i] == 0) break;
											++mobCounter;
											Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
											if(!pTemp || pTemp->isDead()) ++deadCounter;
										}

										if(mobCounter <= deadCounter) JumpToNextStep(0);
										else uiPhaseTimer = 1000;
									}
									else JumpToNextStep(2000);
									break;
							case 26:
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Sixième Assaut Stoppé aux Portes de Hurlevent! "); 
									me->MonsterTextEmote("Sixième Assaut Stoppé aux Portes de Hurlevent!", 0, true);
									JumpToNextStep(4000);
									break;
							case 27:
									if(!WaveDone)
									{
										SpawnWaveGroup(uiWave, uiWaveGUID);
										uiWave++;
										WavesCounter++;
									}

									JumpToNextStep(2000);
									me->MonsterYell("Ne faiblissez pas dans l'effort! Il faut sauver notre Roi à tout prix!", LANG_UNIVERSAL, 0);
								    me->HandleEmoteCommand(378);
									break;
							case 28:
									if(!WaveDone)
									{
										uint32 mobCounter = 0, deadCounter = 0;

										for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
										{
											if(uiWaveGUID[i] == 0) break;
											++mobCounter;
											Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
											if(!pTemp || pTemp->isDead()) ++deadCounter;
										}

										if(mobCounter <= deadCounter) JumpToNextStep(0);
										else uiPhaseTimer = 1000;
									}
									else JumpToNextStep(2000);
									break;
							case 29: 
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Septième Assaut Stoppé aux Portes de Hurlevent! ");
									me->MonsterTextEmote("Septième Assaut Stoppé aux Portes de Hurlevent!", 0, true);
									JumpToNextStep(4000);
									break;
							case 30:
									if(!WaveDone)
									{
										SpawnWaveGroup(uiWave, uiWaveGUID);
										uiWave++;
										WavesCounter++;
									}

									JumpToNextStep(2000);
									break;
							case 31:
									if(!WaveDone)
									{
										uint32 mobCounter = 0, deadCounter = 0;

										for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
										{
											if(uiWaveGUID[i] == 0) break;
											++mobCounter;
											Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
											if(!pTemp || pTemp->isDead()) ++deadCounter;
										}

										if(mobCounter <= deadCounter) JumpToNextStep(0);
										else uiPhaseTimer = 1000;
									}
									else JumpToNextStep(2000);
									break;
							case 32:
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Huitème Assaut Stoppé aux Portes de Hurlevent! "); 
									me->MonsterTextEmote("Huitème Assaut Stoppé aux Portes de Hurlevent!", 0, true);
									JumpToNextStep(4000);
									break;
							case 33:
									if(!WaveDone)
									{
										SpawnWaveGroup(uiWave, uiWaveGUID);
										uiWave++;
										WavesCounter++;
									}

									JumpToNextStep(5000);
									me->PlayDirectSound(11803);
									me->MonsterYell("Bravo Héros, vous venez de repousser les assauts des Défias! Préparez-vous ! Leur Général arrive!", LANG_UNIVERSAL, 0);
                                    me->HandleEmoteCommand(378);
									break;
							case 34:
									//me->MonsterTextEmote("Attention, Le général des Défias est en approche Préparez-vous !", 0, true);
									if(!WaveDone)
									{
										uint32 mobCounter = 0, deadCounter = 0;

										for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
										{
											if(uiWaveGUID[i] == 0) break;
											++mobCounter;
											Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
											if(!pTemp || pTemp->isDead()) ++deadCounter;
										}

										if(mobCounter <= deadCounter) JumpToNextStep(0);
										else uiPhaseTimer = 1000;
									}
									else JumpToNextStep(2000);
									break;
							case 35: 
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "L'Alliance a Défendu Vaillament Les Portes de Hurvelent!");
									me->MonsterTextEmote("L'Alliance a Défendu Vaillament Les Portes de Hurvelent!", 0, true);
									JumpToNextStep(4000);
							case 36:
									JumpToNextStep(4000);
									break;
							case 37:
							        me->MonsterYell("Bravo Héros, vous venez de repousser les assauts des Défias! Hurlevent vous sera indéfiniment redevable!", LANG_UNIVERSAL, 0);
									JumpToNextStep(4000);
									break;
							case 38:
							        break;		
									

							
						}
					}else uiPhaseTimer -= diff;
				}
			}
			else
			{
			summons.DespawnAll();	//despawn ici
			}
		}
	};

	CreatureAI* GetAI(Creature* creature) const
    {
        return new npcmarcusAI(creature);
    }
};










/************************/
/* Dame Jaina Hurlevent */
/************************/

class npcjaina : public CreatureScript
{
	public:
		npcjaina() : CreatureScript("npcjaina") {}
		
	struct npcjainaAI : public npc_escortAI
	{
        
		uint32 Phase;
		uint32 uiPhaseTimer, uiStep;
		uint32 uiWave;
		uint32 WavesCounter;
		uint32 variation;
		uint64 uiWaveGUID[ENCOUNTER_WAVES_MAX_SPAWNS];
		bool bStepping;
		char msg[255];
		bool debug;
		bool WaveDone;
		
		

		npcjainaAI(Creature* creature) : npc_escortAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
			Reset();
		}

		void Reset()
		{
			Phase = PHASE_NEUTRE;
			bStepping = false;
			uiStep = 0;
			uiPhaseTimer = 1000;
			debug = true;
			variation = 0;
            SetupMovement(0);
            WavesCounter = 0;
			uiWave = 0;
			for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i) uiWaveGUID[i] = 0;
            WaveDone = false;
            SetRun(true);
		}
		
		void SpawnWaveGroup(uint32 waveID, uint64* guidVector)
        {
            for(uint32 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
            {
                if((uint32)PortBaseWaves[waveID][i][0] == 0) break;

                if(Creature* pTemp = me->SummonCreature((uint32)PortBaseWaves[waveID][i][0], PortBaseWaves[waveID][i][1], PortBaseWaves[waveID][i][2], PortBaseWaves[waveID][i][3], PortBaseWaves[waveID][i][4], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000))
                {
                    guidVector[i] = pTemp->GetGUID();   
                }
            }
			SendCrierWarning(waveID);
        }


        void SendCrierWarning(uint8 waveNumber)
        {
			char* textId = "";
			
			switch(waveNumber)
			{
				case 0: textId = "1er Accostage au Port de Hurlevent en Approche !";
				        me->SummonCreature(100031, -8335.28f, 1217.61f, 5.17f, 6.28f);
				        me->SummonCreature(100032, -8340.17f, 1214.10f, 5.18f, 6.28f);
				        me->SummonCreature(100033, -8335.68f, 1210.92f, 5.26f, 6.28f);
				        me->SummonCreature(100034, -8241.97f, 1218.29f, 5.12f, 3.13f);
				        me->SummonCreature(100035, -8238.20f, 1215.05f, 5.16f, 3.13f);
				        me->SummonCreature(100036, -8241.96f, 1211.93f, 5.24f, 3.13f);
				        break; 
				case 1: textId = "2ème Accostage au Port de Hurlevent en Approche !";
				        me->SummonCreature(100031, -8335.28f, 1217.61f, 5.17f, 6.28f);
				        me->SummonCreature(100032, -8340.17f, 1214.10f, 5.18f, 6.28f);
				        me->SummonCreature(100033, -8335.68f, 1210.92f, 5.26f, 6.28f);
				        me->SummonCreature(100034, -8241.97f, 1218.29f, 5.12f, 3.13f);
				        me->SummonCreature(100035, -8238.20f, 1215.05f, 5.16f, 3.13f);
				        me->SummonCreature(100036, -8241.96f, 1211.93f, 5.24f, 3.13f);
				        break;
				case 2: textId = "3ème Accostage au Port de Hurlevent en Approche !";
				        me->SummonCreature(100031, -8335.28f, 1217.61f, 5.17f, 6.28f);
				        me->SummonCreature(100032, -8340.17f, 1214.10f, 5.18f, 6.28f);
				        me->SummonCreature(100033, -8335.68f, 1210.92f, 5.26f, 6.28f);
				        me->SummonCreature(100034, -8241.97f, 1218.29f, 5.12f, 3.13f);
				        me->SummonCreature(100035, -8238.20f, 1215.05f, 5.16f, 3.13f);
				        me->SummonCreature(100036, -8241.96f, 1211.93f, 5.24f, 3.13f);
				        break;
				case 3: textId = "4ème Accostage au Port de Hurlevent en Approche !";
				        me->SummonCreature(100031, -8335.28f, 1217.61f, 5.17f, 6.28f);
				        me->SummonCreature(100032, -8340.17f, 1214.10f, 5.18f, 6.28f);
				        me->SummonCreature(100033, -8335.68f, 1210.92f, 5.26f, 6.28f);
				        me->SummonCreature(100034, -8241.97f, 1218.29f, 5.12f, 3.13f);
				        me->SummonCreature(100035, -8238.20f, 1215.05f, 5.16f, 3.13f);
				        me->SummonCreature(100036, -8241.96f, 1211.93f, 5.24f, 3.13f);
				        break;
				case 4: textId = "5ème Accostage au Port de Hurlevent en Approche !";
				        me->SummonCreature(100031, -8335.28f, 1217.61f, 5.17f, 6.28f);
				        me->SummonCreature(100032, -8340.17f, 1214.10f, 5.18f, 6.28f);
				        me->SummonCreature(100033, -8335.68f, 1210.92f, 5.26f, 6.28f);
				        me->SummonCreature(100034, -8241.97f, 1218.29f, 5.12f, 3.13f);
				        me->SummonCreature(100035, -8238.20f, 1215.05f, 5.16f, 3.13f);
				        me->SummonCreature(100036, -8241.96f, 1211.93f, 5.24f, 3.13f);
				        break;
				case 5: textId = "6ème Accostage au Port de Hurlevent en Approche !";
				        me->SummonCreature(100031, -8335.28f, 1217.61f, 5.17f, 6.28f);
				        me->SummonCreature(100032, -8340.17f, 1214.10f, 5.18f, 6.28f);
				        me->SummonCreature(100033, -8335.68f, 1210.92f, 5.26f, 6.28f);
				        me->SummonCreature(100034, -8241.97f, 1218.29f, 5.12f, 3.13f);
				        me->SummonCreature(100035, -8238.20f, 1215.05f, 5.16f, 3.13f);
				        me->SummonCreature(100036, -8241.96f, 1211.93f, 5.24f, 3.13f);
				        break;
				case 6: textId = "7ème Accostage au Port de Hurlevent en Approche !";
				        me->SummonCreature(100031, -8335.28f, 1217.61f, 5.17f, 6.28f);
				        me->SummonCreature(100032, -8340.17f, 1214.10f, 5.18f, 6.28f);
				        me->SummonCreature(100033, -8335.68f, 1210.92f, 5.26f, 6.28f);
				        me->SummonCreature(100034, -8241.97f, 1218.29f, 5.12f, 3.13f);
				        me->SummonCreature(100035, -8238.20f, 1215.05f, 5.16f, 3.13f);
				        me->SummonCreature(100036, -8241.96f, 1211.93f, 5.24f, 3.13f);
				        break;
				case 7: textId = "8ème Accostage au Port de Hurlevent en Approche !";
				        me->SummonCreature(100031, -8335.28f, 1217.61f, 5.17f, 6.28f);
				        me->SummonCreature(100032, -8340.17f, 1214.10f, 5.18f, 6.28f);
				        me->SummonCreature(100033, -8335.68f, 1210.92f, 5.26f, 6.28f);
				        me->SummonCreature(100034, -8241.97f, 1218.29f, 5.12f, 3.13f);
				        me->SummonCreature(100035, -8238.20f, 1215.05f, 5.16f, 3.13f);
				        me->SummonCreature(100036, -8241.96f, 1211.93f, 5.24f, 3.13f);
				        break;
                case 8: textId = "Attention, Le Roi des Gobelins est en approche Préparez-vous !";
                        me->SummonCreature(100031, -8335.28f, 1217.61f, 5.17f, 6.28f);
				        me->SummonCreature(100032, -8340.17f, 1214.10f, 5.18f, 6.28f);
				        me->SummonCreature(100033, -8335.68f, 1210.92f, 5.26f, 6.28f);
				        me->SummonCreature(100034, -8241.97f, 1218.29f, 5.12f, 3.13f);
				        me->SummonCreature(100035, -8238.20f, 1215.05f, 5.16f, 3.13f);
				        me->SummonCreature(100036, -8241.96f, 1211.93f, 5.24f, 3.13f);
                        break;
			}
			
			sWorld->SendWorldText(LANG_SYSTEMMESSAGE, textId);
		}

		void JumpToNextStep(uint32 uiTimer)
        {
            uiPhaseTimer = uiTimer;
            ++uiStep;

			if(debug)
			{
				//sprintf(msg, " UiStep : %u", uiStep);
				sLog->outString("DEBUG: Jainauistep est a %u", uiStep);
			}
        }

		void WaypointReached(uint32 uiPointId)
		{
			switch(variation)
			{
				case 0:
						switch(uiPointId)
						{
							case 2:
									SetEscortPaused(true);
									me->MonsterYell("Sir! Mes sentinelles m'informent de l'arrivé des gobelins au large du Port!", LANG_UNIVERSAL, 0);
									me->HandleEmoteCommand(378);
									uiStep=0;
									JumpToNextStep(3000);
									break;
							case 4:
									SetEscortPaused(true);
									me->MonsterYell("Il nous faut agir au plus vite Seigneur, et organiser la défense de Hurlevent avec toutes les Forces disponibles!", LANG_UNIVERSAL, 0);
									me->HandleEmoteCommand(378);
									uiStep=0;
									JumpToNextStep(36000);
									break;
							case 6: 
									SetEscortPaused(true);
									me->MonsterYell("Que le Raid repoussant les flottes gobelines du Port me suive!", LANG_UNIVERSAL, 0);
									me->HandleEmoteCommand(378);
									JumpToNextStep(10000);
									break;
							case 10:
									SetEscortPaused(true);
									me->MonsterSay("Nous devons empécher cela à tout prix! Vite dépéchez-vous en Monture!", LANG_UNIVERSAL, 0);
									me->HandleEmoteCommand(378);
									JumpToNextStep(5000);
									break;
							case 62:
									SetEscortPaused(true);
									me->MonsterSay("Force de l'Alliance je vous en conjure, stopper cette invasion coute que coute!", LANG_UNIVERSAL, 0);
									me->HandleEmoteCommand(378);
									JumpToNextStep(8000);
									break;
							case 109:
							        SetEscortPaused(true);
							        me->MonsterSay("Mes Archimages m'ont signalés une grande perturbation de l'équilibre dans cette zone!", LANG_UNIVERSAL, 0);
							        me->HandleEmoteCommand(378);
							        JumpToNextStep(8000);
							        break;		
						}
						break;
				
				case 1:
						switch(uiPointId)
						{
							case 1:
							        
									break;
							
						}
						break;
			} 
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
                        AddWaypoint(1, -8505.74f, 338.43f, 120.88f);
						AddWaypoint(2, -8455.39f, 332.38f, 120.88f);
						AddWaypoint(3, -8451.86f, 339.78f, 120.88f);
                        AddWaypoint(4, -8445.71f, 335.10f, 122.16f);
                        AddWaypoint(5, -8443.05f, 331.72f, 122.57f);
                        AddWaypoint(6, -8443.33f, 332.07f, 122.57f);
                        AddWaypoint(7, -8455.73f, 347.521f, 120.886f);
						AddWaypoint(8, -8487.24f, 386.833f, 109.046f);
						AddWaypoint(9, -8508.07f, 412.822f, 108.386f);
						AddWaypoint(10, -8548.51f, 462.886f, 104.59f); //monture
						AddWaypoint(11, -8553.96f, 466.65f, 104.528f);
						AddWaypoint(12, -8565.60f, 466.135f, 104.587f);
						AddWaypoint(13, -8573.41f, 472.469f, 104.244f);
						AddWaypoint(14, -8600.62f, 501.892f, 103.722f);
						AddWaypoint(15, -8611.36f, 511.205f, 103.561f);
						AddWaypoint(16, -8606.52f, 518.516f, 105.105f);
						AddWaypoint(17, -8578.11f, 543.958f, 101.783f);
						AddWaypoint(18, -8587.42f, 575.51f, 102.936f);
						AddWaypoint(19, -8575.72f, 601.985f, 103.266f);
						AddWaypoint(20, -8565.79f, 614.294f, 102.339f);
						AddWaypoint(21, -8556.67f, 615.66f, 102.297f);
						AddWaypoint(22, -8532.01f, 634.339f, 100.217f);
						AddWaypoint(23, -8510.08f, 650.943f, 100.29f);
						AddWaypoint(24, -8525.93f, 675.358f, 102.186f);
						AddWaypoint(25, -8534.79f, 688.946f, 97.6612f);
						AddWaypoint(26, -8552.11f, 680.849f, 97.0807f);
						AddWaypoint(27, -8561.20f, 674.058f, 97.0152f);
						AddWaypoint(28, -8596.46f, 715.665f, 96.6519f);
						AddWaypoint(29, -8612.86f, 705.959f, 96.7221f);
						AddWaypoint(30, -8663.27f, 765.048f, 96.6868f);
						AddWaypoint(31, -8634.25f, 787.311f, 96.6514f);
						AddWaypoint(32, -8640.97f, 823.247f, 96.637f);
						AddWaypoint(33, -8651.84f, 838.927f, 96.7948f);
						AddWaypoint(34, -8682.63f, 878.474f, 97.0153f);
						AddWaypoint(35, -8632.03f, 920.573f, 99.3651f);
						AddWaypoint(36, -8620.61f, 946.143f, 99.4453f);
						AddWaypoint(37, -8579.24f, 983.826f, 97.4567f);
						AddWaypoint(38, -8571.82f, 990.742f, 96.3002f);
						AddWaypoint(39, -8548.19f, 967.142f, 96.3968f);
						AddWaypoint(40, -8519.73f, 955.623f, 95.8422f);
						AddWaypoint(41, -8493.30f, 965.382f, 95.8787f);
						AddWaypoint(42, -8488.60f, 968.94f, 95.847f);
						AddWaypoint(43, -8480.56f, 975.026f, 88.083f);
						AddWaypoint(44, -8466.75f, 986.202f, 79.0201f);
						AddWaypoint(45, -8470.92f, 991.49f, 79.0695f);
						AddWaypoint(46, -8477.21f, 986.69f, 76.0614f);
						AddWaypoint(47, -8484.66f, 981.222f, 72.7376f);
						AddWaypoint(48, -8492.17f, 991.405f, 68.8016f);
						AddWaypoint(49, -8505.16f, 1008.42f, 59.4794f);
						AddWaypoint(50, -8457.17f, 1030.51f, 59.4744f);
						AddWaypoint(51, -8435.02f, 1050.24f, 44.09f);
						AddWaypoint(52, -8416.70f, 1066.25f, 31.6304f);
						AddWaypoint(53, -8407.04f, 1071.18f, 31.6304f);
						AddWaypoint(54, -8404.41f, 1080.00f, 31.63f);
						AddWaypoint(55, -8397.09f, 1111.99f, 17.9447f);
						AddWaypoint(56, -8394.95f, 1138.16f, 17.9614f);
						AddWaypoint(57, -8301.80f, 1136.89f, 18.0951f);
						AddWaypoint(58, -8300.89f, 1177.78f, 18.4219f);
						AddWaypoint(59, -8300.83f, 1196.48f, 5.61882f);
						AddWaypoint(60, -8284.50f, 1243.21f, 5.23046f);
						AddWaypoint(61, -8272.42f, 1242.64f, 5.23058f);
						AddWaypoint(62, -8276.02f, 1242.51f, 5.23058f);
						AddWaypoint(63, -8281.31f, 1233.69f, 5.23063f);
						AddWaypoint(64, -8309.31f, 1209.43f, 5.37823f);
						AddWaypoint(65, -8450.17f, 1207.11f, 5.2094f);
						AddWaypoint(66, -8531.66f, 1214.22f, 5.16238f);
						AddWaypoint(67, -8566.33f, 1214.1f, 5.12533f);
						AddWaypoint(68, -8572.91f, 1205.89f, 5.20212f);
						AddWaypoint(69, -8574.04f, 1193.53f, 5.7967f);
						AddWaypoint(70, -8574.68f, 1186.56f, 11.4009f);
						AddWaypoint(71, -8575.35f, 1177.67f, 18.4339f);
						AddWaypoint(72, -8577.61f, 1147.56f, 18.0425f);
						AddWaypoint(73, -8582.94f, 1119.74f, 17.9446f);
						AddWaypoint(74, -8587.04f, 1108.23f, 23.5617f);
						AddWaypoint(75, -8591.75f, 1092.06f, 31.7895f);
						AddWaypoint(76, -8590.55f, 1081.19f, 34.9716f);
						AddWaypoint(77, -8581.23f, 1069.06f, 37.5896f);
						AddWaypoint(78, -8556.99f, 1036.63f, 58.1066f);
						AddWaypoint(79, -8552.07f, 1036.05f, 59.3701f);
						AddWaypoint(80, -8540.53f, 1034.77f, 59.5499f);
						AddWaypoint(81, -8506.75f, 1004.00f, 59.4777f);
						AddWaypoint(82, -8499.61f, 997.493f, 62.8523f);
						AddWaypoint(83, -8491.04f, 988.066f, 71.3926f);
						AddWaypoint(84, -8486.22f, 982.801f, 72.7378f);
						AddWaypoint(85, -8480.07f, 987.677f, 74.4484f);
						AddWaypoint(86, -8472.26f, 992.418f, 79.06f);
						AddWaypoint(87, -8467.01f, 985.859f, 79.0153f);
						AddWaypoint(88, -8472.19f, 981.732f, 80.4022f);
						AddWaypoint(89, -8487.00f, 969.618f, 95.1357f);
						AddWaypoint(90, -8489.09f, 968.22f, 95.8661f);
						AddWaypoint(91, -8510.74f, 955.085f, 95.8028f);
						AddWaypoint(92, -8536.74f, 960.804f, 96.039f);
						AddWaypoint(93, -8551.91f, 968.695f, 96.4246f);
						AddWaypoint(94, -8572.70f, 993.264f, 96.2515f);
						AddWaypoint(95, -8606.88f, 966.778f, 99.4458f);
						AddWaypoint(96, -8628.97f, 949.664f, 99.4458f);
						AddWaypoint(97, -8645.33f, 950.136f, 100.145f);
						AddWaypoint(98, -8689.67f, 924.848f, 101.074f);
						AddWaypoint(99, -8709.18f, 911.939f, 101.24f);
						AddWaypoint(100, -8721.28f, 926.965f, 100.662f);
						AddWaypoint(101, -8736.84f, 946.412f, 101.379f);
						AddWaypoint(102, -8744.55f, 956.053f, 100.058f);
						AddWaypoint(103, -8729.78f, 975.011f, 98.5435f);
						AddWaypoint(104, -8731.45f, 1005.82f, 95.6333f);
						AddWaypoint(105, -8737.59f, 1041.12f, 92.3999f);
						AddWaypoint(106, -8741.53f, 1060.28f, 89.7957f);
						AddWaypoint(107, -8748.27f, 1069.01f, 90.1458f);
						AddWaypoint(108, -8751.61f, 1079.31f, 90.7793f);
						AddWaypoint(109, -8750.30f, 1074.79f, 90.6682f);
                        break;
                case 1:
                        
                        AddWaypoint(1, -8281.31f, 1233.69f, 5.23063f);
						AddWaypoint(2, -8309.31f, 1209.43f, 5.37823f);
						AddWaypoint(3, -8450.17f, 1207.11f, 5.2094f);
						AddWaypoint(4, -8531.66f, 1214.22f, 5.16238f);
						AddWaypoint(5, -8566.33f, 1214.1f, 5.12533f);
						AddWaypoint(6, -8572.91f, 1205.89f, 5.20212f);
						AddWaypoint(7, -8574.04f, 1193.53f, 5.7967f);
						AddWaypoint(8, -8574.68f, 1186.56f, 11.4009f);
						AddWaypoint(9, -8575.35f, 1177.67f, 18.4339f);
						AddWaypoint(10, -8577.61f, 1147.56f, 18.0425f);
						AddWaypoint(11, -8582.94f, 1119.74f, 17.9446f);
						AddWaypoint(12, -8587.04f, 1108.23f, 23.5617f);
						AddWaypoint(13, -8591.75f, 1092.06f, 31.7895f);
						AddWaypoint(14, -8590.55f, 1081.19f, 34.9716f);
						AddWaypoint(15, -8581.23f, 1069.06f, 37.5896f);
						AddWaypoint(16, -8556.99f, 1036.63f, 58.1066f);
						AddWaypoint(17, -8552.07f, 1036.05f, 59.3701f);
						AddWaypoint(18, -8540.53f, 1034.77f, 59.5499f);
						AddWaypoint(19, -8506.75f, 1004.00f, 59.4777f);
						AddWaypoint(20, -8499.61f, 997.493f, 62.8523f);
						AddWaypoint(21, -8491.04f, 988.066f, 71.3926f);
						AddWaypoint(22, -8486.22f, 982.801f, 72.7378f);
						AddWaypoint(23, -8480.07f, 987.677f, 74.4484f);
						AddWaypoint(24, -8472.26f, 992.418f, 79.06f);
						AddWaypoint(25, -8467.01f, 985.859f, 79.0153f);
						AddWaypoint(26, -8472.19f, 981.732f, 80.4022f);
						AddWaypoint(27, -8487.00f, 969.618f, 95.1357f);
						AddWaypoint(28, -8489.09f, 968.22f, 95.8661f);
						AddWaypoint(29, -8510.74f, 955.085f, 95.8028f);
						AddWaypoint(30, -8536.74f, 960.804f, 96.039f);
						AddWaypoint(31, -8551.91f, 968.695f, 96.4246f);
						AddWaypoint(32, -8572.70f, 993.264f, 96.2515f);
						AddWaypoint(33, -8606.88f, 966.778f, 99.4458f);
						AddWaypoint(34, -8628.97f, 949.664f, 99.4458f);
						AddWaypoint(35, -8645.33f, 950.136f, 100.145f);
						AddWaypoint(36, -8689.67f, 924.848f, 101.074f);
						AddWaypoint(37, -8709.18f, 911.939f, 101.24f);
						AddWaypoint(38, -8721.28f, 926.965f, 100.662f);
						AddWaypoint(39, -8736.84f, 946.412f, 101.379f);
						AddWaypoint(40, -8744.55f, 956.053f, 100.058f);
						AddWaypoint(41, -8729.78f, 975.011f, 98.5435f);
						AddWaypoint(42, -8731.45f, 1005.82f, 95.6333f);
						AddWaypoint(43, -8737.59f, 1041.12f, 92.3999f);
						AddWaypoint(44, -8741.53f, 1060.28f, 89.7957f);
						AddWaypoint(45, -8748.27f, 1069.01f, 90.1458f);
						AddWaypoint(46, -8751.61f, 1079.31f, 90.7793f);
						AddWaypoint(47, -8750.30f, 1074.79f, 90.6682f);
                        break;       
				default:
                        sLog->outError("Unexpected movement variation (%i) in npcjainaAI::SetupMovement call!", variation);
                        return;
			}
			if(npcjainaAI* pEscortAI = CAST_AI(npcjainaAI, me->AI()))
			{
				pEscortAI->Start(true, true);
				pEscortAI->SetDespawnAtEnd(false);
				pEscortAI->SetDespawnAtFar(false);
				me->setActive(true);
				bStepping = true;
			}
		}

        void UpdateAI(const uint32 diff)
		{
			npc_escortAI::UpdateAI(diff);
			
            if(bStepping)
            {
				if(uiPhaseTimer <= diff)
				{
					switch(uiStep)
					{
						case 0:
                                SetRun(true);
								JumpToNextStep(2000); 
                                break;
                        case 1:
                                SetEscortPaused(false);
                                break;
                        case 2:
                                SetEscortPaused(false);
                                break;
                        case 3:
                                me->Mount(MOUNT_MODEL2);
								me->SetSpeed(MOVE_RUN, SPEED_MOUNT);
								JumpToNextStep(5000);
								break;
						case 4:
								me->MonsterSay("En avant Héros.", LANG_UNIVERSAL, 0);
								JumpToNextStep(10000);
						case 5:
								SetEscortPaused(false);
								break;
					    case 6:
								me->Unmount();
                                me->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
								me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                me->SetReactState(REACT_PASSIVE);
                                JumpToNextStep(5000);
								break;
						case 7:
						        
						        me->SummonCreature(100026, -8271.69f, 1233.43f, 5.23f, 3.05f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000);
						        me->SummonCreature(100026, -8271.81f, 1228.20f, 5.23f, 3.05f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000);
						       	me->MonsterSay("Voici deux véhicules encore en état de marchent pour vous aider dans votre Combat", LANG_UNIVERSAL, 0);
						       	JumpToNextStep(5000);
						       	break;
						case 8:
						        me->MonsterSay("Placez-y vos deux meilleurs Pilotes Héros. Ils vous seront d'un grands soutiens!", LANG_UNIVERSAL, 0);
						       	JumpToNextStep(5000);
						       	break;       		
                        case 9: 
                                me->MonsterYell("Nous ferons payer aux Gobelins cette trahison!", LANG_UNIVERSAL, 0);
                                me->PlayDirectSound(11803);
                                me->HandleEmoteCommand(378);
								JumpToNextStep(5000);
								
						        if(!WaveDone)
								{
									SpawnWaveGroup(uiWave, uiWaveGUID);
									uiWave++;
									WavesCounter++;
								}
								break;
						case 10:
						        
								if(!WaveDone)
								{
									uint32 mobCounter = 0 , deadCounter = 0;

									for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
									{
										if(uiWaveGUID[i] == 0) break;
										++mobCounter;
										Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
										if(!pTemp || pTemp->isDead()) ++deadCounter;
									} 
									
									if(mobCounter <= deadCounter) JumpToNextStep(0);
									else uiPhaseTimer = 1000;
								}
								else JumpToNextStep(2000);
								break;
						case 11:
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Premier Accostage au Port de Hurlevent Stoppé !");                              
								me->MonsterTextEmote("Premier Accostage au Port de Hurlevent Stoppé !", 0, true);
								JumpToNextStep(4000);
								break;
						case 12:
								if(!WaveDone)
								{
									SpawnWaveGroup(uiWave, uiWaveGUID);
									uiWave++;
									WavesCounter++;
								}

								JumpToNextStep(2000);
								break;
						case 13:
						        
								if(!WaveDone)
								{
									uint32 mobCounter = 0, deadCounter = 0;

									for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
									{
										if(uiWaveGUID[i] == 0) break;
										++mobCounter;
										Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
										if(!pTemp || pTemp->isDead()) ++deadCounter;
									}

									if(mobCounter <= deadCounter) JumpToNextStep(0);
									else uiPhaseTimer = 1000;
								}
								else JumpToNextStep(2000);
								break;
						case 14: 
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Deuxième Accostage au Port de Hurlevent Stoppé !");
								me->MonsterTextEmote("Deuxième Accostage au Port de Hurlevent Stoppé !", 0, true);
								JumpToNextStep(4000);
								break;
                        case 15:
								if(!WaveDone)
								{
									SpawnWaveGroup(uiWave, uiWaveGUID);
									uiWave++;
									WavesCounter++;
								}

								JumpToNextStep(2000);
								me->MonsterYell("Courage Héros, il faut tenir bon!", LANG_UNIVERSAL, 0);
								me->HandleEmoteCommand(378);
								break;
						case 16:
								if(!WaveDone)
								{
									uint32 mobCounter = 0, deadCounter = 0;

									for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
									{
										if(uiWaveGUID[i] == 0) break;
										++mobCounter;
										Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
										if(!pTemp || pTemp->isDead()) ++deadCounter;
									}

									if(mobCounter <= deadCounter) JumpToNextStep(0);
									else uiPhaseTimer = 1000;
								}
								else JumpToNextStep(2000);
								break;
						case 17: 
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Troisième Accostage au Port de Hurlevent Stoppé !");
								me->MonsterTextEmote("Troisième Accostage au Port de Hurlevent Stoppé !", 0, true);
								JumpToNextStep(4000);
								break;
                        case 18:
								if(!WaveDone)
								{
									SpawnWaveGroup(uiWave, uiWaveGUID);
									uiWave++;
									WavesCounter++;
								}

								JumpToNextStep(2000);
								break;
						case 19:
								if(!WaveDone)
								{
									uint32 mobCounter = 0, deadCounter = 0;

									for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
									{
										if(uiWaveGUID[i] == 0) break;
										++mobCounter;
										Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
										if(!pTemp || pTemp->isDead()) ++deadCounter;
									}

									if(mobCounter <= deadCounter) JumpToNextStep(0);
									else uiPhaseTimer = 1000;
								}
								else JumpToNextStep(2000);
								break;
						case 20: 
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Quatrième Accostage au Port de Hurlevent Stoppé !");
								me->MonsterTextEmote("Quatrième Accostage au Port de Hurlevent Stoppé !", 0, true);
								JumpToNextStep(4000);
								break;
                        case 21:
								if(!WaveDone)
								{
									SpawnWaveGroup(uiWave, uiWaveGUID);
									uiWave++;
									WavesCounter++;
								}

								JumpToNextStep(2000);
								me->MonsterYell("Ne faiblissez pas dans l'effort! Il faut sauver notre Roi à tout prix!", LANG_UNIVERSAL, 0);
								me->HandleEmoteCommand(378);
								break;
						case 22:
								if(!WaveDone)
								{
									uint32 mobCounter = 0, deadCounter = 0;

									for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
									{
										if(uiWaveGUID[i] == 0) break;
										++mobCounter;
										Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
										if(!pTemp || pTemp->isDead()) ++deadCounter;
									}

									if(mobCounter <= deadCounter) JumpToNextStep(0);
									else uiPhaseTimer = 1000;
								}
								else JumpToNextStep(2000);
								break;
						case 23:
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Cinquième Accostage au Port de Hurlevent Stoppé !"); 
								me->MonsterTextEmote("Cinquième Accostage au Port de Hurlevent Stoppé !", 0, true);
								JumpToNextStep(4000);
								break;
                        case 24:
								if(!WaveDone)
								{
									SpawnWaveGroup(uiWave, uiWaveGUID);
									uiWave++;
									WavesCounter++;
								}

								JumpToNextStep(2000);
								break;
						case 25:
								if(!WaveDone)
								{
									uint32 mobCounter = 0, deadCounter = 0;

									for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
									{
										if(uiWaveGUID[i] == 0) break;
										++mobCounter;
										Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
										if(!pTemp || pTemp->isDead()) ++deadCounter;
									}

									if(mobCounter <= deadCounter) JumpToNextStep(0);
									else uiPhaseTimer = 1000;
								}
								else JumpToNextStep(2000);
								break;
						case 26:
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Sixième Accostage au Port de Hurlevent Stoppé !"); 
								me->MonsterTextEmote("Sixième Accostage au Port de Hurlevent Stoppé !", 0, true);
								JumpToNextStep(4000);
								break;
                        case 27:
								if(!WaveDone)
								{
									SpawnWaveGroup(uiWave, uiWaveGUID);
									uiWave++;
									WavesCounter++;
								}

								JumpToNextStep(2000);
								me->MonsterYell("Les Gobelins sont de redoutables Guerriers, mais vous Héros, vous l'êtes encore plus! Continuez !", LANG_UNIVERSAL, 0);
								me->HandleEmoteCommand(378);
								break;
						case 28:
								if(!WaveDone)
								{
									uint32 mobCounter = 0, deadCounter = 0;

									for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
									{
										if(uiWaveGUID[i] == 0) break;
										++mobCounter;
										Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
										if(!pTemp || pTemp->isDead()) ++deadCounter;
									}

									if(mobCounter <= deadCounter) JumpToNextStep(0);
									else uiPhaseTimer = 1000;
								}
								else JumpToNextStep(2000);
								break;
						case 29: 
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Septième Accostage au Port de Hurlevent Stoppé !");
								me->MonsterTextEmote("Septième Accostage au Port de Hurlevent Stoppé !", 0, true);
								JumpToNextStep(4000);
								break;
                        case 30:
								if(!WaveDone)
								{
									SpawnWaveGroup(uiWave, uiWaveGUID);
									uiWave++;
									WavesCounter++;
								}

								JumpToNextStep(2000);
								break;
						case 31:
								if(!WaveDone)
								{
									uint32 mobCounter = 0, deadCounter = 0;

									for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
									{
										if(uiWaveGUID[i] == 0) break;
										++mobCounter;
										Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
										if(!pTemp || pTemp->isDead()) ++deadCounter;
									}

									if(mobCounter <= deadCounter) JumpToNextStep(0);
									else uiPhaseTimer = 1000;
								}
								else JumpToNextStep(2000);
								break;
						case 32:
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Huitème Accostage au Port de Hurlevent Stoppé !"); 
								me->MonsterTextEmote("Huitème Accostage au Port de Hurlevent Stoppé !", 0, true);
								JumpToNextStep(4000);
								break;
                        case 33:
								if(!WaveDone)
								{
									SpawnWaveGroup(uiWave, uiWaveGUID);
									uiWave++;
									WavesCounter++;
								}
                                me->MonsterYell("Bravo Héros, vous venez de repousser les assauts des gobelins! Préparez-vous ! Leur chef arrive!", LANG_UNIVERSAL, 0);
                                me->HandleEmoteCommand(378);
                                me->PlayDirectSound(11803);
								JumpToNextStep(6000);
							    break;
						case 34:
								if(!WaveDone)
								{
									uint32 mobCounter = 0, deadCounter = 0;

									for(uint8 i = 0; i < ENCOUNTER_WAVES_MAX_SPAWNS; ++i)
									{
										if(uiWaveGUID[i] == 0) break;
										++mobCounter;
										Unit* pTemp = Unit::GetCreature(*me, uiWaveGUID[i]);
										if(!pTemp || pTemp->isDead()) ++deadCounter;
									}

									if(mobCounter <= deadCounter) JumpToNextStep(0);
									else uiPhaseTimer = 1000;
								}
								else JumpToNextStep(2000);
								break;
						case 35: 
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "L'Alliance a Défendu Vaillament Le Port de Hurvelent!");
								me->MonsterTextEmote("L'Alliance a Défendu Vaillament Le Port de Hurvelent!", 0, true);
								JumpToNextStep(4000);
								break;
						case 36:
						         /* if (CAST_AI(npc_lycanthrope::npc_lycanthropeAI, me->AI())->Die1 == true && (CAST_AI(npc_bomberman::npc_bombermanAI, me->AI())->Die == true)   
						        { */
						          me->MonsterYell("Bravo Héros, vous venez à nouveau de prouver votre valeur! L'Assauts des Gobelins est stoppé. Hurlevent vous sera indéfiniment redevable!", LANG_UNIVERSAL, 0);
						          JumpToNextStep(4000);
								  break;
						case 37:
						        SetEscortPaused(true);
						        JumpToNextStep(4000);
								break;	
						case 38: 
						        break;
					}
				}else uiPhaseTimer -= diff;
			}
		}
	};

	CreatureAI* GetAI(Creature* creature) const
    {
        return new npcjainaAI(creature);
    }
};





















//Partie Event Aux Portes de Hurlevent

/***************************************/
/* Archer Défias Wave Portes Hurlevent */
/***************************************/


class npc_archer_defias : public CreatureScript
{
public:
    npc_archer_defias() : CreatureScript("npc_archer_defias") { }

    struct npc_archer_defiasAI : public npc_escortAI
    {
        npc_archer_defiasAI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}
            uint32 MultiTimer;
            uint32 VolleyTimer;
            uint32 SnakeTimer;
            uint32 PoisonTimer;
            uint32 TrapTimer;
            uint32 HealTimer;
            bool Haste;
            bool Healed;

        void Reset()
        {
            MultiTimer = 8000;
            VolleyTimer = 15000;
            SnakeTimer = 40000;
            PoisonTimer = 12000;
            TrapTimer = 20000;
            HealTimer = 60000;
            SetupMovement(0);
            Haste = false;
            Healed = false;   
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_ARCHERDEFIAS_AGGRO, LANG_UNIVERSAL, 0);
            //me->AddAura(BEWITCHING_AURA, me);
            me->AddAura(AURA_ROT, me);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_ARCHERDEFIAS_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_ARCHERDEFIAS_DEATH, LANG_UNIVERSAL, 0);
        }

        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 61:
					    SetEscortPaused(true);
				        break;
			}
		}
		
		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -9170.11f, 345.345f, 85.7247f);
						AddWaypoint(2, -9153.16f, 364.028f, 90.1565f);
						AddWaypoint(3, -9135.25f, 380.996f, 90.5976f);
						AddWaypoint(4, -9106.17f, 405.2f, 92.6421f);
						AddWaypoint(5, -9081.44f, 422.146f, 92.5755f);
						AddWaypoint(6, -9060.34f, 437.889f, 93.056f);
						AddWaypoint(7, -9033.02f, 458.75f, 93.056f);
						AddWaypoint(8, -9015.99f, 472.29f, 95.8848f);
						AddWaypoint(9, -8987.66f, 494.498f, 96.5194f);
						AddWaypoint(10, -8955.94f, 519.774f, 96.3553f);
						AddWaypoint(11, -8945.66f, 510.554f, 96.2942f);
						AddWaypoint(12, -8928.87f, 493.523f, 93.8399f);
						AddWaypoint(13, -8908.4f, 508.351f, 93.8436f);
						AddWaypoint(14, -8928.32f, 542.118f, 94.3018f);
						AddWaypoint(15, -8908.70f, 556.803f, 93.5057f);
						AddWaypoint(16, -8877.68f, 581.834f, 92.9949f);
						AddWaypoint(17, -8842.08f, 612.808f, 92.8053f);
						AddWaypoint(18, -8831.73f, 623.458f, 93.8528f);
						AddWaypoint(19, -8847.10f, 649.193f, 96.6171f);
						AddWaypoint(20, -8853.81f, 659.963f, 96.9273f);
						AddWaypoint(21, -8827.03f, 677.331f, 97.5745f);
						AddWaypoint(22, -8806.49f, 684.495f, 100.478f);
						AddWaypoint(23, -8789.04f, 681.654f, 101.98f);
						AddWaypoint(24, -8782.71f, 676.574f, 102.787f);
						AddWaypoint(25, -8762.14f, 649.328f, 103.78f);
						AddWaypoint(26, -8758.06f, 631.053f, 102.498f);
						AddWaypoint(27, -8763.01f, 617.151f, 98.8399f);
						AddWaypoint(28, -8770.30f, 609.288f, 97.2278f);
						AddWaypoint(29, -8756.67f, 595.185f, 97.2162f);
						AddWaypoint(30, -8737.25f, 574.747f, 97.4011f);
						AddWaypoint(31, -8749.82f, 561.009f, 97.3976f);
						AddWaypoint(32, -8745.01f, 556.247f, 97.8508f);
						AddWaypoint(33, -8732.55f, 543.003f, 101.109f);
						AddWaypoint(34, -8713.34f, 519.655f, 97.1719f);
						AddWaypoint(35, -8721.46f, 511.546f, 96.6055f);
						AddWaypoint(36, -8692.95f, 474.571f, 95.3211f);
						AddWaypoint(37, -8715.8f, 452.022f, 97.1091f);
						AddWaypoint(38, -8723.69f, 439.663f, 97.4109f);
						AddWaypoint(39, -8720.37f, 422.488f, 97.8295f);
						AddWaypoint(40, -8709.94f, 420.396f, 98.7172f);
						AddWaypoint(41, -8702.78f, 405.761f, 99.9562f);
						AddWaypoint(42, -8690.99f, 395.894f, 101.636f);
						AddWaypoint(43, -8677.40f, 400.412f, 103.016f);
						AddWaypoint(44, -8651.10f, 421.955f, 101.795f);
						AddWaypoint(45, -8633.99f, 444.006f, 102.217f);
						AddWaypoint(46, -8632.42f, 459.523f, 102.271f);
						AddWaypoint(47, -8646.30f, 477.77f, 102.506f);
						AddWaypoint(48, -8609.14f, 507.258f, 103.721f);
						AddWaypoint(49, -8598.22f, 500.361f, 103.739f);
						AddWaypoint(50, -8575.24f, 473.326f, 104.213f);
						AddWaypoint(51, -8560.99f, 464.884f, 104.546f);
						AddWaypoint(52, -8547.79f, 468.032f, 104.502f);
						AddWaypoint(53, -8547.34f, 467.431f, 104.517f);
						AddWaypoint(54, -8536.53f, 452.953f, 104.917f);
						AddWaypoint(55, -8527.78f, 441.832f, 105.024f);
						AddWaypoint(56, -8508.96f, 417.942f, 108.187f);
						AddWaypoint(57, -8488.37f, 392.205f, 108.386f);
						AddWaypoint(58, -8485.37f, 388.45f, 109.019f);
						AddWaypoint(59, -8457.35f, 353.414f, 120.233f);
						AddWaypoint(60, -8454.04f, 349.276f, 120.886f);
						AddWaypoint(61, -8444.30f, 337.051f, 121.746f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_archer_defiasAI::SetupMovement call!", variation); 
                         return;
			}
			
			
		    if(npc_archer_defiasAI* pEscortAI = CAST_AI(npc_archer_defiasAI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}	
			    
        void UpdateAI(const uint32 uiDiff)
        {
			npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            // Tire Multiple toutes les 8 secondes
            if(MultiTimer <= uiDiff)
            {
                if(Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, MULTISHOT);
                MultiTimer = 8000;
            } else MultiTimer -= uiDiff;

            //  Volley toutes les 15 secondes sur une cible random
            if(VolleyTimer <= uiDiff)
            {
                DoCastAOE(VOLLEY);
                VolleyTimer = 15000;
            } else VolleyTimer -= uiDiff;

            // Pièges toutes les 10 secondes sur une cible random
            if(SnakeTimer <= uiDiff)
            {
                if(Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
                    for(uint8 i = 1; i <= 3; i++)
                    {
                        me->SummonCreature(SUMMONED_SNAKE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                    }
                SnakeTimer = 40000;
            } else SnakeTimer -= uiDiff;  

            // AOE Poison toutes les 12 secondes
         /*   if (PoisonTimer <= uiDiff)
            {
                DoCastAOE(BOLT_VOLLEY);
                PoisonTimer = 12000;
            } else PoisonTimer -= uiDiff; */

            // Trap de glace sur le tank toutes les 20 secondes
            if(TrapTimer <= uiDiff)
            {
                me->MonsterYell(SAY_ARCHERDEFIAS_ICE, LANG_UNIVERSAL, 0);
                DoCast(me->getVictim(), ICE_TRAP);
                TrapTimer = 20000;
            } else TrapTimer -= uiDiff;

            // Se Heal toutes les 25 secondes
            if(HealTimer <= uiDiff)
            {
                DoCast(me, REJUVENATION);
                HealTimer = 60000;
            } else HealTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

         CreatureAI* GetAI(Creature* pCreature) const
        {
         return new npc_archer_defiasAI (pCreature);
        }
};



/***************************************/
/* Mage Défias Wave portes Hurlevent   */
/***************************************/


class npc_mage_defias : public CreatureScript
{
public:
    npc_mage_defias() : CreatureScript("npc_mage_defias") { }

    struct npc_mage_defiasAI : public npc_escortAI
    {
        npc_mage_defiasAI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}
		
            uint32 BurstTimer;
            uint32 GriefTimer;
            uint32 FlameTimer;
            uint32 PiercingTimer;
            uint32 HarvestTimer;

        void Reset()
        {
            BurstTimer = 30000;
            GriefTimer = 20000;
            FlameTimer = 15000;
            PiercingTimer = 10000;
            HarvestTimer = 45000;
            SetupMovement(0);			
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_MAGEDEFIAS_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_MAGEDEFIAS_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_MAGEDEFIAS_DEATH, LANG_UNIVERSAL, 0);
        }

		void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 61:
					    SetEscortPaused(true);
				        break;
			}
		}
		
		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -9170.11f, 345.345f, 85.7247f);
						AddWaypoint(2, -9153.16f, 364.028f, 90.1565f);
						AddWaypoint(3, -9135.25f, 380.996f, 90.5976f);
						AddWaypoint(4, -9106.17f, 405.2f, 92.6421f);
						AddWaypoint(5, -9081.44f, 422.146f, 92.5755f);
						AddWaypoint(6, -9060.34f, 437.889f, 93.056f);
						AddWaypoint(7, -9033.02f, 458.75f, 93.056f);
						AddWaypoint(8, -9015.99f, 472.29f, 95.8848f);
						AddWaypoint(9, -8987.66f, 494.498f, 96.5194f);
						AddWaypoint(10, -8955.94f, 519.774f, 96.3553f);
						AddWaypoint(11, -8945.66f, 510.554f, 96.2942f);
						AddWaypoint(12, -8928.87f, 493.523f, 93.8399f);
						AddWaypoint(13, -8908.4f, 508.351f, 93.8436f);
						AddWaypoint(14, -8928.32f, 542.118f, 94.3018f);
						AddWaypoint(15, -8908.70f, 556.803f, 93.5057f);
						AddWaypoint(16, -8877.68f, 581.834f, 92.9949f);
						AddWaypoint(17, -8842.08f, 612.808f, 92.8053f);
						AddWaypoint(18, -8831.73f, 623.458f, 93.8528f);
						AddWaypoint(19, -8847.10f, 649.193f, 96.6171f);
						AddWaypoint(20, -8853.81f, 659.963f, 96.9273f);
						AddWaypoint(21, -8827.03f, 677.331f, 97.5745f);
						AddWaypoint(22, -8806.49f, 684.495f, 100.478f);
						AddWaypoint(23, -8789.04f, 681.654f, 101.98f);
						AddWaypoint(24, -8782.71f, 676.574f, 102.787f);
						AddWaypoint(25, -8762.14f, 649.328f, 103.78f);
						AddWaypoint(26, -8758.06f, 631.053f, 102.498f);
						AddWaypoint(27, -8763.01f, 617.151f, 98.8399f);
						AddWaypoint(28, -8770.30f, 609.288f, 97.2278f);
						AddWaypoint(29, -8756.67f, 595.185f, 97.2162f);
						AddWaypoint(30, -8737.25f, 574.747f, 97.4011f);
						AddWaypoint(31, -8749.82f, 561.009f, 97.3976f);
						AddWaypoint(32, -8745.01f, 556.247f, 97.8508f);
						AddWaypoint(33, -8732.55f, 543.003f, 101.109f);
						AddWaypoint(34, -8713.34f, 519.655f, 97.1719f);
						AddWaypoint(35, -8721.46f, 511.546f, 96.6055f);
						AddWaypoint(36, -8692.95f, 474.571f, 95.3211f);
						AddWaypoint(37, -8715.80f, 452.022f, 97.1091f);
						AddWaypoint(38, -8723.69f, 439.663f, 97.4109f);
						AddWaypoint(39, -8720.37f, 422.488f, 97.8295f);
						AddWaypoint(40, -8709.94f, 420.396f, 98.7172f);
						AddWaypoint(41, -8702.78f, 405.761f, 99.9562f);
						AddWaypoint(42, -8690.99f, 395.894f, 101.636f);
						AddWaypoint(43, -8677.40f, 400.412f, 103.016f);
						AddWaypoint(44, -8651.10f, 421.955f, 101.795f);
						AddWaypoint(45, -8633.99f, 444.006f, 102.217f);
						AddWaypoint(46, -8632.42f, 459.523f, 102.271f);
						AddWaypoint(47, -8646.30f, 477.77f, 102.506f);
						AddWaypoint(48, -8609.14f, 507.258f, 103.721f);
						AddWaypoint(49, -8598.22f, 500.361f, 103.739f);
						AddWaypoint(50, -8575.24f, 473.326f, 104.213f);
						AddWaypoint(51, -8560.99f, 464.884f, 104.546f);
						AddWaypoint(52, -8547.79f, 468.032f, 104.502f);
						AddWaypoint(53, -8547.34f, 467.431f, 104.517f);
						AddWaypoint(54, -8536.53f, 452.953f, 104.917f);
						AddWaypoint(55, -8527.78f, 441.832f, 105.024f);
						AddWaypoint(56, -8508.96f, 417.942f, 108.187f);
						AddWaypoint(57, -8488.37f, 392.205f, 108.386f);
						AddWaypoint(58, -8485.37f, 388.45f, 109.019f);
						AddWaypoint(59, -8457.35f, 353.414f, 120.233f);
						AddWaypoint(60, -8454.04f, 349.276f, 120.886f);
						AddWaypoint(61, -8444.30f, 337.051f, 121.746f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_mage_defiasAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_mage_defiasAI* pEscortAI = CAST_AI(npc_mage_defiasAI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}
		
        void UpdateAI(const uint32 uiDiff)
        {
			npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            if(BurstTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                me->DeleteThreatList();
                DoCast(me->getVictim(), SHADOW_BURST);
                BurstTimer = urand(25000,35000);
            } else BurstTimer -= uiDiff;
            
            if(GriefTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                if(Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
                    DoCast(pTarget, STORM_OF_GRIEF);
                    GriefTimer = 20000;
            } else GriefTimer -= uiDiff;
            
            if(PiercingTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                if(Unit *pTarget = SelectTarget(SELECT_TARGET_TOPAGGRO,0))
                    DoCast(pTarget, PIERCING_SHADOW);
                    PiercingTimer = 10000;
            } else PiercingTimer -= uiDiff;
           
            if(FlameTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                DoCastAOE(SHADOW_FLAME);
                FlameTimer = 15000;
            } else FlameTimer -= uiDiff;
            
            if(HarvestTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                me->MonsterYell(SAY_MAGEDEFIAS_SOUL, LANG_UNIVERSAL, 0);
                DoCastAOE(HARVEST_SOUL);
                HarvestTimer = 45000;
            } else HarvestTimer -= uiDiff;

            DoSpellAttackIfReady(SHADOW_BOLT);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mage_defiasAI (pCreature);
    }
};


/*********************************************/
/* Infanterie Défias Wave portes Hurlevent   */
/*********************************************/


class npc_infanterie_defias : public CreatureScript
{
public:
    npc_infanterie_defias() : CreatureScript("npc_infanterie_defias") { }

    struct npc_infanterie_defiasAI : public npc_escortAI
    {
        npc_infanterie_defiasAI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}

            uint32 EnrageTimer;
            uint32 WoundTimer;
            uint32 FeverTimer;
            uint32 BiteTimer;
            bool Enraged;

        void Reset()
        {
            EnrageTimer = 15000;
            WoundTimer = 5000;
            FeverTimer = 12000;
            BiteTimer = 60000;
			SetupMovement(0);
            Enraged = false;
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_INFANTERIEDEFIAS_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_INFANTERIEDEFIAS_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_INFANTERIEDEFIAS_DEATH, LANG_UNIVERSAL, 0);
        }
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 61:
					    SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -9170.11f, 345.345f, 85.7247f);
						AddWaypoint(2, -9153.16f, 364.028f, 90.1565f);
						AddWaypoint(3, -9135.25f, 380.996f, 90.5976f);
						AddWaypoint(4, -9106.17f, 405.20f, 92.6421f);
						AddWaypoint(5, -9081.44f, 422.146f, 92.5755f);
						AddWaypoint(6, -9060.34f, 437.889f, 93.056f);
						AddWaypoint(7, -9033.02f, 458.75f, 93.056f);
						AddWaypoint(8, -9015.99f, 472.29f, 95.8848f);
						AddWaypoint(9, -8987.66f, 494.498f, 96.5194f);
						AddWaypoint(10, -8955.94f, 519.774f, 96.3553f);
						AddWaypoint(11, -8945.66f, 510.554f, 96.2942f);
						AddWaypoint(12, -8928.87f, 493.523f, 93.8399f);
						AddWaypoint(13, -8908.40f, 508.351f, 93.8436f);
						AddWaypoint(14, -8928.32f, 542.118f, 94.3018f);
						AddWaypoint(15, -8908.7f, 556.803f, 93.5057f);
						AddWaypoint(16, -8877.68f, 581.834f, 92.9949f);
						AddWaypoint(17, -8842.08f, 612.808f, 92.8053f);
						AddWaypoint(18, -8831.73f, 623.458f, 93.8528f);
						AddWaypoint(19, -8847.10f, 649.193f, 96.6171f);
						AddWaypoint(20, -8853.81f, 659.963f, 96.9273f);
						AddWaypoint(21, -8827.03f, 677.331f, 97.5745f);
						AddWaypoint(22, -8806.49f, 684.495f, 100.478f);
						AddWaypoint(23, -8789.04f, 681.654f, 101.98f);
						AddWaypoint(24, -8782.71f, 676.574f, 102.787f);
						AddWaypoint(25, -8762.14f, 649.328f, 103.78f);
						AddWaypoint(26, -8758.06f, 631.053f, 102.498f);
						AddWaypoint(27, -8763.01f, 617.151f, 98.8399f);
						AddWaypoint(28, -8770.30f, 609.288f, 97.2278f);
						AddWaypoint(29, -8756.67f, 595.185f, 97.2162f);
						AddWaypoint(30, -8737.25f, 574.747f, 97.4011f);
						AddWaypoint(31, -8749.82f, 561.009f, 97.3976f);
						AddWaypoint(32, -8745.01f, 556.247f, 97.8508f);
						AddWaypoint(33, -8732.55f, 543.003f, 101.109f);
						AddWaypoint(34, -8713.34f, 519.655f, 97.1719f);
						AddWaypoint(35, -8721.46f, 511.546f, 96.6055f);
						AddWaypoint(36, -8692.95f, 474.571f, 95.3211f);
						AddWaypoint(37, -8715.80f, 452.022f, 97.1091f);
						AddWaypoint(38, -8723.69f, 439.663f, 97.4109f);
						AddWaypoint(39, -8720.37f, 422.488f, 97.8295f);
						AddWaypoint(40, -8709.94f, 420.396f, 98.7172f);
						AddWaypoint(41, -8702.78f, 405.761f, 99.9562f);
						AddWaypoint(42, -8690.99f, 395.894f, 101.636f);
						AddWaypoint(43, -8677.40f, 400.412f, 103.016f);
						AddWaypoint(44, -8651.10f, 421.955f, 101.795f);
						AddWaypoint(45, -8633.99f, 444.006f, 102.217f);
						AddWaypoint(46, -8632.42f, 459.523f, 102.271f);
						AddWaypoint(47, -8646.30f, 477.770f, 102.506f);
						AddWaypoint(48, -8609.14f, 507.258f, 103.721f);
						AddWaypoint(49, -8598.22f, 500.361f, 103.739f);
						AddWaypoint(50, -8575.24f, 473.326f, 104.213f);
						AddWaypoint(51, -8560.99f, 464.884f, 104.546f);
						AddWaypoint(52, -8547.79f, 468.032f, 104.502f);
						AddWaypoint(53, -8547.34f, 467.431f, 104.517f);
						AddWaypoint(54, -8536.53f, 452.953f, 104.917f);
						AddWaypoint(55, -8527.78f, 441.832f, 105.024f);
						AddWaypoint(56, -8508.96f, 417.942f, 108.187f);
						AddWaypoint(57, -8488.37f, 392.205f, 108.386f);
						AddWaypoint(58, -8485.37f, 388.45f, 109.019f);
						AddWaypoint(59, -8457.35f, 353.414f, 120.233f);
						AddWaypoint(60, -8454.04f, 349.276f, 120.886f);
						AddWaypoint(61, -8444.30f, 337.051f, 121.746f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_infanterie_defiasAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_infanterie_defiasAI* pEscortAI = CAST_AI(npc_infanterie_defiasAI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;
            
            if(EnrageTimer <= uiDiff)
            {
                DoCast(me, ENRAGER);
                EnrageTimer = urand(20000,25000);
            } else EnrageTimer -= uiDiff;

            //Lance Tourbillion sur une cible en random
            if(BiteTimer <= uiDiff)
            {
                DoCast(me, WORGEN_BIT);
                //if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,1))
                //me->AddAura(WORGEN_CAL, pTarget);
                BiteTimer = 60000;
            } else BiteTimer -= uiDiff;
            
            if(WoundTimer <= uiDiff)
            {
                DoCast(me->getVictim(), MORTALE_WOUND);
                WoundTimer = 5000;
            } else WoundTimer -= uiDiff;
            
            if(FeverTimer <= uiDiff)
            {
                DoCastAOE(DECREPIT);
                FeverTimer = 20000;
            } else FeverTimer -= uiDiff;

            // Deviens enrager arrivé à 15% hp  (augmente 50% attaque  50% domage)
            if(HealthBelowPct(15) && !Enraged)
            {
                DoCast(me, FRENZI);
                Enraged = true;
            } 

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_infanterie_defiasAI (pCreature);
    }
};




/*******************************************/
/* Boss Défias Entrée Porte de Hurlevent   */
/*******************************************/




class npc_lycanthrope : public CreatureScript
{
    public:

    npc_lycanthrope() : CreatureScript("npc_lycanthrope") { }

    struct npc_lycanthropeAI : public npc_escortAI
    {
        npc_lycanthropeAI(Creature *pCreature) : npc_escortAI(pCreature), summons(me)
		{
			Reset();
		}

            uint32 EnrageTimer;
            uint32 WounTimer;
            uint32 BiteTimer;
            uint32 FeverTimer;
            SummonList summons;
            bool Enraged;
            bool Worgen;
            bool Haste;
            bool health_45;
            bool health_25;
            bool Die1;

        void Reset()
        {
            summons.DespawnAll();
            me->RestoreDisplayId();
            EnrageTimer = 15000;
            WounTimer = 5000;
            BiteTimer = 30000;
            FeverTimer = 12000;
            Enraged = false;
            Worgen = false;
            Haste = false;
            health_45 = false;
            health_25 = false;
            Die1 = false;
            SetupMovement(0);
        }


        void SummonedCreatureDespawn(Creature* summon)
        {
            summons.Despawn(summon);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_LYCANTHROPE_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_LYCANTHROPE_SLAY, LANG_UNIVERSAL, 0);
            // En cas de mort d'un joueurs, spawn d'un Worgen sur le corps
            if(victim->GetTypeId() == TYPEID_PLAYER)
                victim->SummonCreature(SUMMON_WORGEN, victim->GetPositionX(), victim->GetPositionY(), victim->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
        }

        void JustDied(Unit* /*killer*/) 
        {
            me->MonsterYell(SAY_LYCANTHROPE_DEATH, LANG_UNIVERSAL, 0);
            summons.DespawnAll();
            me->RestoreDisplayId();
            Die1 = true;
        }

        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 61:
					    SetEscortPaused(true);
				        break;       
			}
		}
	
		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -9170.11f, 345.345f, 85.7247f);
						AddWaypoint(2, -9153.16f, 364.028f, 90.1565f);
						AddWaypoint(3, -9135.25f, 380.996f, 90.5976f);
						AddWaypoint(4, -9106.17f, 405.20f, 92.6421f);
						AddWaypoint(5, -9081.44f, 422.146f, 92.5755f);
						AddWaypoint(6, -9060.34f, 437.889f, 93.056f);
						AddWaypoint(7, -9033.02f, 458.75f, 93.056f);
						AddWaypoint(8, -9015.99f, 472.29f, 95.8848f);
						AddWaypoint(9, -8987.66f, 494.498f, 96.5194f);
						AddWaypoint(10, -8955.94f, 519.774f, 96.3553f);
						AddWaypoint(11, -8945.66f, 510.554f, 96.2942f);
						AddWaypoint(12, -8928.87f, 493.523f, 93.8399f);
						AddWaypoint(13, -8908.40f, 508.351f, 93.8436f);
						AddWaypoint(14, -8928.32f, 542.118f, 94.3018f);
						AddWaypoint(15, -8908.70f, 556.803f, 93.5057f);
						AddWaypoint(16, -8877.68f, 581.834f, 92.9949f);
						AddWaypoint(17, -8842.08f, 612.808f, 92.8053f);
						AddWaypoint(18, -8831.73f, 623.458f, 93.8528f);
						AddWaypoint(19, -8847.10f, 649.193f, 96.6171f);
						AddWaypoint(20, -8853.81f, 659.963f, 96.9273f);
						AddWaypoint(21, -8827.03f, 677.331f, 97.5745f);
						AddWaypoint(22, -8806.49f, 684.495f, 100.478f);
						AddWaypoint(23, -8789.04f, 681.654f, 101.98f);
						AddWaypoint(24, -8782.71f, 676.574f, 102.787f);
						AddWaypoint(25, -8762.14f, 649.328f, 103.78f);
						AddWaypoint(26, -8758.06f, 631.053f, 102.498f);
						AddWaypoint(27, -8763.01f, 617.151f, 98.8399f);
						AddWaypoint(28, -8770.30f, 609.288f, 97.2278f);
						AddWaypoint(29, -8756.67f, 595.185f, 97.2162f);
						AddWaypoint(30, -8737.25f, 574.747f, 97.4011f);
						AddWaypoint(31, -8749.82f, 561.009f, 97.3976f);
						AddWaypoint(32, -8745.01f, 556.247f, 97.8508f);
						AddWaypoint(33, -8732.55f, 543.003f, 101.109f);
						AddWaypoint(34, -8713.34f, 519.655f, 97.1719f);
						AddWaypoint(35, -8721.46f, 511.546f, 96.6055f);
						AddWaypoint(36, -8692.95f, 474.571f, 95.3211f);
						AddWaypoint(37, -8715.80f, 452.022f, 97.1091f);
						AddWaypoint(38, -8723.69f, 439.663f, 97.4109f);
						AddWaypoint(39, -8720.37f, 422.488f, 97.8295f);
						AddWaypoint(40, -8709.94f, 420.396f, 98.7172f);
						AddWaypoint(41, -8702.78f, 405.761f, 99.9562f);
						AddWaypoint(42, -8690.99f, 395.894f, 101.636f);
						AddWaypoint(43, -8677.40f, 400.412f, 103.016f);
						AddWaypoint(44, -8651.10f, 421.955f, 101.795f);
						AddWaypoint(45, -8633.99f, 444.006f, 102.217f);
						AddWaypoint(46, -8632.42f, 459.523f, 102.271f);
						AddWaypoint(47, -8646.30f, 477.77f, 102.506f);
						AddWaypoint(48, -8609.14f, 507.258f, 103.721f);
						AddWaypoint(49, -8598.22f, 500.361f, 103.739f);
						AddWaypoint(50, -8575.24f, 473.326f, 104.213f);
						AddWaypoint(51, -8560.99f, 464.884f, 104.546f);
						AddWaypoint(52, -8547.79f, 468.032f, 104.502f);
						AddWaypoint(53, -8547.34f, 467.431f, 104.517f);
						AddWaypoint(54, -8536.53f, 452.953f, 104.917f);
						AddWaypoint(55, -8527.78f, 441.832f, 105.024f);
						AddWaypoint(56, -8508.96f, 417.942f, 108.187f);
						AddWaypoint(57, -8488.37f, 392.205f, 108.386f);
						AddWaypoint(58, -8485.37f, 388.45f, 109.019f);
						AddWaypoint(59, -8457.35f, 353.414f, 120.233f);
						AddWaypoint(60, -8454.04f, 349.276f, 120.886f);
						AddWaypoint(61, -8444.30f, 337.051f, 121.746f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_lycanthropeAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_lycanthropeAI* pEscortAI = CAST_AI(npc_lycanthropeAI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
            npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            // Enrage dure 8 secondes occurance entre 20et 25 secondes
            if(EnrageTimer <= uiDiff)
            {
                DoCast(me, ENRAGE);
                EnrageTimer = urand(20000,25000);
            } else EnrageTimer -= uiDiff;

            // dure 5 secondes reduit de 5% les efet de soins  (max 15 stack)
            if(WounTimer <= uiDiff)
            {
                DoCast(me->getVictim(), MORTAL_WOUN);
                WounTimer = 5000;
            } else WounTimer -= uiDiff;

            // dure 20 secondes
            if(FeverTimer <= uiDiff)
            {
                DoCastAOE(DECREPIT_FEVER);
                FeverTimer = 20000;
            } else FeverTimer -= uiDiff;

            // Transformation dure  seconde et transforme en Worgen
            // impossibilité de cast de sort 
            if(BiteTimer <= uiDiff)
            {
                me->MonsterTextEmote(EMOTE_WORGEN_BITE, 0, true);
                DoCast(me, WORGEN_BITE);
                if(Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,1))
                    me->AddAura(WORGEN_CALL, pTarget);
                    BiteTimer = 30000;
            } else BiteTimer -= uiDiff;

            // Arrivé à 75% hp transformation en Worgen
            if(HealthBelowPct(75) && !Worgen)
            {
                me->SetDisplayId(26787);
                me->MonsterYell(SAY_LYCANTHROPE_MORPH, LANG_UNIVERSAL, 0);
                DoCast(me, ENRAGE);
                EnrageTimer = urand(25000,35000);
                Worgen = true;
            }

            // Arrivé à 45 %hp redeviens normale
            if(HealthBelowPct(45) && !health_45)
            {
                me->MonsterYell(SAY_LYCANTHROPE_DEMORPH, LANG_UNIVERSAL, 0);
                me->SetDisplayId(2029);
                health_45 = true;
                Worgen = true;
            }

            // Arrivé à 25% hp transformation en Worgen
            if(HealthBelowPct(25) && !health_25)
            {
                me->SetDisplayId(26787);
                me->MonsterYell(SAY_LYCANTHROPE_MORPH, LANG_UNIVERSAL, 0);
                DoCast(me, ENRAGE);
                EnrageTimer = urand(25000,35000);
                health_25 = true;
                Worgen = true;
            }

            // S'enrage à 15% de vie (buff 50% vitesse d'attaque 50% dommage)
            if(HealthBelowPct(15) && !Enraged)
            {
                DoCast(me, FRENZY);
                Enraged = true;
            }

             // Spawn sont trash arriver à 50% HP
            if(HealthBelowPct(50) && !Haste )
            {
                me->MonsterYell(SAY_TRASHLYCAN_SPAWN, LANG_UNIVERSAL, 0);
                me->SummonCreature(SUMMON_TRASHLYCAN, me->GetPositionX()+1, me->GetPositionY()+1, me->GetPositionZ(), 0 /*, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1*IN_MILLISECONDS */);
                Haste = true;
            }

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_lycanthropeAI (pCreature);
    }
};




/*******************************************/
/* Minion du Boss Entrée Porte Hurlevent   */
/*******************************************/




class npc_trash_lycan : public CreatureScript
{
public:

    npc_trash_lycan() : CreatureScript("npc_trash_lycan") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_trash_lycanAI (pCreature);
    }

    struct npc_trash_lycanAI : public ScriptedAI
    {
        npc_trash_lycanAI(Creature *pCreature) : ScriptedAI(pCreature), summons(me){}


            uint32 MultiTimer;
            uint32 VolleyTimer;
            uint32 VortexTimer;
            uint32 PoisonTimer;
            uint32 TrapTimer;
            uint32 HealTimer;
            SummonList summons;
            bool Haste;
            bool Healed;
        

        void Reset()
        {
            summons.DespawnAll();
            MultiTimer = 2000;
            VolleyTimer = 15000;
            VortexTimer = 30000;
            PoisonTimer = 12000;
            TrapTimer = 20000;
            HealTimer = 25000;
            Haste = false;
            Healed = false;
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_TRASHLYCAN_AGGRO, LANG_UNIVERSAL, 0);
            me->AddAura(SPELL_BEWITCHING_AURA, me);
            me->AddAura(SPELL_AURA_ROT, me);
            me->RemoveAurasDueToSpell(SPELL_ENERGY_STORM);
        }

        void EnterEvadeMode()
        {
            summons.DespawnAll();
            _EnterEvadeMode();
            Reset();
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_TRASHLYCAN_SLAY, LANG_UNIVERSAL, 0);
            me->RemoveAurasDueToSpell(SPELL_ENERGY_STORM);
            summons.DespawnAll();
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_TRASHLYCAN_DEATH, LANG_UNIVERSAL, 0);
            me->RemoveAurasDueToSpell(SPELL_ENERGY_STORM);
            summons.DespawnAll();
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            summons.Despawn(summon);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if(!UpdateVictim())
                return;

            if(me->HasUnitState(UNIT_STAT_CASTING))
                return;

            // Multishot ogni 2 sec
            if(MultiTimer <= uiDiff)
            {
                if(Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_MULTISHOT);
                    MultiTimer = 8000;
            } else MultiTimer -= uiDiff;

            // Ogni 10 sec casta Volley su un target random
            if(VolleyTimer <= uiDiff)
            {
                DoCastAOE(SPELL_VOLLEY);
                VolleyTimer = 15000;
            } else VolleyTimer -= uiDiff;

            // Spawn des 4 Vortex 
           if(VortexTimer <= uiDiff)
            {
                if(Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
                {
                        me->MonsterTextEmote(EMOTE_TRASHLYCAN_VORTEX, 0, true);
                    for(uint8 i = 0; i <= 5; i++)
                    {
                        me->GetMotionMaster()->Clear();
                        DoCast(me, SPELL_ENERGY_STORM, true); // aura ennemi
                        
                        Creature* Vortex = DoSpawnCreature(SUMMONED_VORTEX, 0, 0, 0, 0,TEMPSUMMON_TIMED_DESPAWN, 30000);
                        {
                            if(Vortex)
                            {
                                Vortex->CastSpell(Vortex, SPELL_CYCLONE_PASSIVE, true);
                                Vortex->CastSpell(Vortex, SPELL_CYCLONE_VISUAL, true);
                                Vortex->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                                Vortex->SetSpeed(MOVE_RUN, 1.0f);
                                Vortex->AI()->AttackStart(SelectTarget(SELECT_TARGET_RANDOM, 0));
                                Vortex->SetInCombatWithZone();
                            }
                        }
                    }
                                VortexTimer = 40000;
                }
            } else VortexTimer -= uiDiff;

            // Poison ad area ogni 10 sec
            if(PoisonTimer <= uiDiff)
            {
                DoCastAOE(SPELL_BOLT_VOLLEY);
                PoisonTimer = 12000;
            } else PoisonTimer -= uiDiff;

            // Ogni 20 sec casta Frost Trap sul tank
            if(TrapTimer <= uiDiff)
            {
                me->MonsterYell(SAY_TRASHLYCAN_ICE, LANG_UNIVERSAL, 0);
                DoCast(me->getVictim(), SPELL_ICE_TRAP);
                TrapTimer = 20000;
            } else TrapTimer -= uiDiff;

            // Ogni 25 sec casta Rejuvenation
            if(HealTimer <= uiDiff)
            {
                DoCast(me, SPELL_REJUVENATION);
                HealTimer = 25000;
            } else HealTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

};


/***************************************************************************/
/* Creature pour dégat des tornades du Minion Boss Entrée Porte Hurlevent   */
/***************************************************************************/

class mob_lycan_vortex : public CreatureScript
{
    public:

        mob_lycan_vortex()
            : CreatureScript("mob_lycan_vortex")
        {
        }

        struct mob_lycan_vortexAI : public ScriptedAI
        {
            mob_lycan_vortexAI(Creature* c) : ScriptedAI(c) {}

            void Reset() {}

            void EnterCombat(Unit* /*pTarget*/) {}

            void SpellHit(Unit* caster, const SpellEntry* spell)
            {
                if(spell->Id == SPELL_ZAP_INFORM)
                    DoCast(caster, SPELL_ZAP_DAMAGE, true);
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                //Si le vortex perds la cible, il check une autre cible
                if(me->IsWithinMeleeRange(me->getVictim()))
                    AttackStart(SelectTarget(SELECT_TARGET_RANDOM, 0));
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_lycan_vortexAI(creature);
        }
};



/******************************************************************************************************************************************************************************************************
/*****************************************************************************************************************************************************************************************************/



//Partie Event Au Port de Hurlevent


/**********************************************/
/* Infanterie Gobelin WavePort de Hurlevent   */
/**********************************************/


class npc_infanterie_gobelin : public CreatureScript
{
public:
    npc_infanterie_gobelin() : CreatureScript("npc_infanterie_gobelin") { }

    struct npc_infanterie_gobelinAI : public npc_escortAI
    {
        npc_infanterie_gobelinAI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}

            uint32 RageTimer;
            uint32 TourbillonTimer;
            uint32 ChaineTimer;
            uint32 HurlementTimer;
            bool health_15;
            bool Enraged;

        void Reset()
        {
            RageTimer =       30000;
            TourbillonTimer = 18000;
            ChaineTimer =     15000;
            HurlementTimer =  8000;
            health_15 = false;
            Enraged = false;
            SetupMovement(0);
        }

        void EnterCombat(Unit* /*who*/)    
        {
            me->MonsterYell(SAY_INFANTERIEGOBELIN_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_INFANTERIEGOBELIN_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_INFANTERIEGOBELIN_DEATH, LANG_UNIVERSAL, 0);
            
        }
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 81:
					    SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
				
						AddWaypoint(1, -8292.07f, 1302.51f, 5.23119f);
                        AddWaypoint(2, -8292.57f, 1265.33f, 5.23119f);
                        AddWaypoint(3, -8292.82f, 1242.08f, 5.23071f);
                        AddWaypoint(4, -8300.48f, 1195.53f, 5.62398f);
                        AddWaypoint(5, -8300.66f, 1187.58f, 10.7239f);
                        AddWaypoint(6, -8300.96f, 1177.47f, 18.4496f);
                        AddWaypoint(7, -8300.67f, 1168.57f, 17.6878f);
                        AddWaypoint(8, -8301.41f, 1135.61f, 18.142f);
                        AddWaypoint(9, -8344.40f, 1135.74f, 18.7538f);
                        AddWaypoint(10, -8395.27f, 1135.89f, 17.9899f);
                        AddWaypoint(11, -8441.01f, 1136.17f, 18.8788f);
                        AddWaypoint(12, -8486.81f, 1135.76f, 18.0337f);
                        AddWaypoint(13, -8526.46f, 1133.86f, 18.2078f);
                        AddWaypoint(14, -8573.86f, 1131.58f, 17.9448f);
						AddWaypoint(15, -8580.71f, 1118.37f, 17.9448f);
						AddWaypoint(16, -8586.96f, 1105.54f, 25.0302f);
						AddWaypoint(17, -8592.05f, 1092.98f, 31.534f);
						AddWaypoint(18, -8586.94f, 1078.16f, 35.6756f);
						AddWaypoint(19, -8575.93f, 1063.11f, 41.4909f);
						AddWaypoint(20, -8556.01f, 1038.89f, 57.7437f);
						AddWaypoint(21, -8550.60f, 1036.61f, 59.3816f);
						AddWaypoint(22, -8537.39f, 1035.38f, 59.6563f);
						AddWaypoint(23, -8520.49f, 1017.92f, 59.9289f);
						AddWaypoint(24, -8504.76f, 1003.97f, 59.4779f);
						AddWaypoint(25, -8492.47f, 990.335f, 69.3196f);
						AddWaypoint(26, -8487.73f, 984.923f, 72.7377f);
						AddWaypoint(27, -8486.04f, 982.774f, 72.7377f);
						AddWaypoint(28, -8479.34f, 988.42f, 75.4748f);
						AddWaypoint(29, -8472.62f, 993.994f, 79.0666f);
						AddWaypoint(30, -8466.89f, 987.086f, 79.0392f);
						AddWaypoint(31, -8472.30f, 981.689f, 80.4825f);
						AddWaypoint(32, -8487.22f, 969.506f, 95.3373f);
						AddWaypoint(33, -8503.03f, 956.192f, 95.7726f);
						AddWaypoint(34, -8530.41f, 956.374f, 95.979f);
						AddWaypoint(35, -8545.89f, 962.205f, 96.5852f);
						AddWaypoint(36, -8571.21f, 992.509f, 96.2303f);
						AddWaypoint(37, -8590.40f, 977.718f, 97.7525f);
						AddWaypoint(38, -8632.35f, 944.139f, 99.52f);
						AddWaypoint(39, -8628.55f, 927.301f, 100.021f);
						AddWaypoint(40, -8639.78f, 915.532f, 99.2767f);
						AddWaypoint(41, -8682.28f, 878.042f, 97.0173f);
						AddWaypoint(42, -8650.31f, 837.51f, 96.7324f);
						AddWaypoint(43, -8633.90f, 814.968f, 96.63f);
						AddWaypoint(44, -8635.92f, 795.093f, 96.6511f);
						AddWaypoint(45, -8611.89f, 762.076f, 96.7211f);
						AddWaypoint(46, -8630.45f, 743.696f, 96.8469f);
						AddWaypoint(47, -8637.82f, 736.079f, 96.7511f);
						AddWaypoint(48, -8614.08f, 707.148f, 96.7303f);
						AddWaypoint(49, -8597.65f, 718.661f, 96.6746f);
						AddWaypoint(50, -8562.42f, 674.358f, 97.0155f);
						AddWaypoint(51, -8538.72f, 689.058f, 97.6675f);
						AddWaypoint(52, -8534.28f, 686.213f, 97.678f);
						AddWaypoint(53, -8525.75f, 674.779f, 102.326f);
						AddWaypoint(54, -8513.67f, 657.729f, 101.134f);
						AddWaypoint(55, -8510.26f, 652.719f, 100.292f);
						AddWaypoint(56, -8508.31f, 650.421f, 100.292f);
						AddWaypoint(57, -8489.67f, 674.499f, 100.593f);
						AddWaypoint(58, -8474.67f, 693.871f, 99.5743f);
						AddWaypoint(59, -8469.26f, 686.765f, 99.3762f);
						AddWaypoint(60, -8443.32f, 654.567f, 92.9822f);
						AddWaypoint(61, -8459.60f, 638.725f, 91.8592f);
						AddWaypoint(62, -8474.07f, 627.534f, 93.6498f);
						AddWaypoint(63, -8473.15f, 619.384f, 94.4642f);
						AddWaypoint(64, -8451.50f, 586.494f, 94.1368f);
						AddWaypoint(65, -8460.44f, 577.074f, 94.8139f);
						AddWaypoint(66, -8479.75f, 562.473f, 97.2927f);
						AddWaypoint(67, -8503.78f, 533.833f, 97.6536f);
						AddWaypoint(68, -8542.97f, 501.789f, 98.6207f);
						AddWaypoint(69, -8539.00f, 494.086f, 99.6874f);
						AddWaypoint(70, -8535.82f, 487.926f, 101.081f);
						AddWaypoint(71, -8538.93f, 482.365f, 101.795f);
						AddWaypoint(72, -8547.08f, 471.504f, 104.281f);
						AddWaypoint(73, -8545.23f, 464.217f, 104.435f);
						AddWaypoint(74, -8536.71f, 453.436f, 104.917f);
						AddWaypoint(75, -8520.64f, 432.804f, 106.221f);
						AddWaypoint(76, -8504.19f, 412.137f, 108.386f);
						AddWaypoint(77, -8488.66f, 392.631f, 108.386f);
						AddWaypoint(78, -8476.69f, 377.605f, 112.491f);
						AddWaypoint(79, -8457.82f, 353.765f, 120.091f);
						AddWaypoint(80, -8455.13f, 350.37f, 120.886f);
						AddWaypoint(81, -8446.93f, 339.988f, 121.33f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_infanterie_gobelinAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_infanterie_gobelinAI* pEscortAI = CAST_AI(npc_infanterie_gobelinAI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}
		
		
		
		
		void SetInCombat()
        {
            Map *map = me->GetMap();
                return;

            Map::PlayerList const &PlayerList = map->GetPlayers();
            for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if(Player* i_pl = i->getSource())
                    if(!i_pl->isGameMaster() && i_pl->isAlive() && me->GetDistance(i_pl) <= 100)
                    {
                        me->SetInCombatWith(i_pl);
                        i_pl->SetInCombatWith(me);
                        me->AddThreat(i_pl, 1.0f);
                    }
            }
        }
		

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;
            
            if(RageTimer <= uiDiff)
            {
                DoCast(me, RAGE);
                RageTimer = urand(28000,30000);
            } else RageTimer -= uiDiff;

            //Lance Tourbillion 
            if(TourbillonTimer <= uiDiff)
            {
                DoCast(me, TOURBILLON);
                TourbillonTimer = 18000;
            } else TourbillonTimer -= uiDiff;
            
            //Lance Chain de Cristal sur sa cible
            if(ChaineTimer <= uiDiff)
            {
                DoCast(me->getVictim(), CHAINCRISTAL);
                ChaineTimer = 15000;
            } else ChaineTimer -= uiDiff;
            
            
             //Lance Hurlement sur une cible random
            if(HurlementTimer <= uiDiff)
            {
            
            if(Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,1))
                 DoCast(pTarget, HURLEMENT);
                 HurlementTimer = 8000;
            } else HurlementTimer -= uiDiff;
            
      
            // Deviens enrager arrivé à 15% hp  (augmente 50% attaque  50% domage)
            if(HealthBelowPct(15) && !Enraged)
            {
                DoCast(me, FRENZ);
                Enraged = true;
            } 

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_infanterie_gobelinAI (pCreature);
    }
};







/**********************************************/
/* Mage Gobelin WavePort de Hurlevent         */
/**********************************************/


class npc_mage_gobelin : public CreatureScript
{
public:
    npc_mage_gobelin() : CreatureScript("npc_mage_gobelin") { }

    struct npc_mage_gobelinAI : public npc_escortAI
    {
        npc_mage_gobelinAI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}

            uint32 ChainTimer;
            uint32 LavaTimer;
            uint32 FlameTimer;
            uint32 ShockTimer;
            bool Haste;
            bool Healed;

        void Reset()
        {
            ChainTimer = 8000;
            LavaTimer  = 15000;
            FlameTimer = 4000;
            ShockTimer = 20000;
            Haste = false;
            Healed = false;
            SetupMovement(0);
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_MAGEGOBELIN_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_MAGEGOBELIN_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_MAGEGOBELIN_DEATH, LANG_UNIVERSAL, 0);
        }
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 81:
					    SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
                        AddWaypoint(1, -8292.07f, 1302.51f, 5.23119f);
                        AddWaypoint(2, -8292.57f, 1265.33f, 5.23119f);
                        AddWaypoint(3, -8292.82f, 1242.08f, 5.23071f);
                        AddWaypoint(4, -8300.48f, 1195.53f, 5.62398f);
                        AddWaypoint(5, -8300.66f, 1187.58f, 10.7239f);
                        AddWaypoint(6, -8300.96f, 1177.47f, 18.4496f);
                        AddWaypoint(7, -8300.67f, 1168.57f, 17.6878f);
                        AddWaypoint(8, -8301.41f, 1135.61f, 18.142f);
                        AddWaypoint(9, -8344.40f, 1135.74f, 18.7538f);
                        AddWaypoint(10, -8395.27f, 1135.89f, 17.9899f);
                        AddWaypoint(11, -8441.01f, 1136.17f, 18.8788f);
                        AddWaypoint(12, -8486.81f, 1135.76f, 18.0337f);
                        AddWaypoint(13, -8526.46f, 1133.86f, 18.2078f);
                        AddWaypoint(14, -8573.86f, 1131.58f, 17.9448f);
						AddWaypoint(15, -8580.71f, 1118.37f, 17.9448f);
						AddWaypoint(16, -8586.96f, 1105.54f, 25.0302f);
						AddWaypoint(17, -8592.05f, 1092.98f, 31.534f);
						AddWaypoint(18, -8586.94f, 1078.16f, 35.6756f);
						AddWaypoint(19, -8575.93f, 1063.11f, 41.4909f);
						AddWaypoint(20, -8556.01f, 1038.89f, 57.7437f);
						AddWaypoint(21, -8550.60f, 1036.61f, 59.3816f);
						AddWaypoint(22, -8537.39f, 1035.38f, 59.6563f);
						AddWaypoint(23, -8520.49f, 1017.92f, 59.9289f);
						AddWaypoint(24, -8504.76f, 1003.97f, 59.4779f);
						AddWaypoint(25, -8492.47f, 990.335f, 69.3196f);
						AddWaypoint(26, -8487.73f, 984.923f, 72.7377f);
						AddWaypoint(27, -8486.04f, 982.774f, 72.7377f);
						AddWaypoint(28, -8479.34f, 988.42f, 75.4748f);
						AddWaypoint(29, -8472.62f, 993.994f, 79.0666f);
						AddWaypoint(30, -8466.89f, 987.086f, 79.0392f);
						AddWaypoint(31, -8472.30f, 981.689f, 80.4825f);
						AddWaypoint(32, -8487.22f, 969.506f, 95.3373f);
						AddWaypoint(33, -8503.03f, 956.192f, 95.7726f);
						AddWaypoint(34, -8530.41f, 956.374f, 95.979f);
						AddWaypoint(35, -8545.89f, 962.205f, 96.5852f);
						AddWaypoint(36, -8571.21f, 992.509f, 96.2303f);
						AddWaypoint(37, -8590.40f, 977.718f, 97.7525f);
						AddWaypoint(38, -8632.35f, 944.139f, 99.52f);
						AddWaypoint(39, -8628.55f, 927.301f, 100.021f);
						AddWaypoint(40, -8639.78f, 915.532f, 99.2767f);
						AddWaypoint(41, -8682.28f, 878.042f, 97.0173f);
						AddWaypoint(42, -8650.31f, 837.51f, 96.7324f);
						AddWaypoint(43, -8633.90f, 814.968f, 96.63f);
						AddWaypoint(44, -8635.92f, 795.093f, 96.6511f);
						AddWaypoint(45, -8611.89f, 762.076f, 96.7211f);
						AddWaypoint(46, -8630.45f, 743.696f, 96.8469f);
						AddWaypoint(47, -8637.82f, 736.079f, 96.7511f);
						AddWaypoint(48, -8614.08f, 707.148f, 96.7303f);
						AddWaypoint(49, -8597.65f, 718.661f, 96.6746f);
						AddWaypoint(50, -8562.42f, 674.358f, 97.0155f);
						AddWaypoint(51, -8538.72f, 689.058f, 97.6675f);
						AddWaypoint(52, -8534.28f, 686.213f, 97.678f);
						AddWaypoint(53, -8525.75f, 674.779f, 102.326f);
						AddWaypoint(54, -8513.67f, 657.729f, 101.134f);
						AddWaypoint(55, -8510.26f, 652.719f, 100.292f);
						AddWaypoint(56, -8508.31f, 650.421f, 100.292f);
						AddWaypoint(57, -8489.67f, 674.499f, 100.593f);
						AddWaypoint(58, -8474.67f, 693.871f, 99.5743f);
						AddWaypoint(59, -8469.26f, 686.765f, 99.3762f);
						AddWaypoint(60, -8443.32f, 654.567f, 92.9822f);
						AddWaypoint(61, -8459.60f, 638.725f, 91.8592f);
						AddWaypoint(62, -8474.07f, 627.534f, 93.6498f);
						AddWaypoint(63, -8473.15f, 619.384f, 94.4642f);
						AddWaypoint(64, -8451.50f, 586.494f, 94.1368f);
						AddWaypoint(65, -8460.44f, 577.074f, 94.8139f);
						AddWaypoint(66, -8479.75f, 562.473f, 97.2927f);
						AddWaypoint(67, -8503.78f, 533.833f, 97.6536f);
						AddWaypoint(68, -8542.97f, 501.789f, 98.6207f);
						AddWaypoint(69, -8539.00f, 494.086f, 99.6874f);
						AddWaypoint(70, -8535.82f, 487.926f, 101.081f);
						AddWaypoint(71, -8538.93f, 482.365f, 101.795f);
						AddWaypoint(72, -8547.08f, 471.504f, 104.281f);
						AddWaypoint(73, -8545.23f, 464.217f, 104.435f);
						AddWaypoint(74, -8536.71f, 453.436f, 104.917f);
						AddWaypoint(75, -8520.64f, 432.804f, 106.221f);
						AddWaypoint(76, -8504.19f, 412.137f, 108.386f);
						AddWaypoint(77, -8488.66f, 392.631f, 108.386f);
						AddWaypoint(78, -8476.69f, 377.605f, 112.491f);
						AddWaypoint(79, -8457.82f, 353.765f, 120.091f);
						AddWaypoint(80, -8455.13f, 350.37f, 120.886f);
						AddWaypoint(81, -8446.93f, 339.988f, 121.33f);
						
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_mage_gobelinAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_mage_gobelinAI* pEscortAI = CAST_AI(npc_mage_gobelinAI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;
            
            // Chain éclair toutes les 10/15 secondes
            if(ChainTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                DoCast(me->getVictim(), CHAIN_LIGHTNING);
                ChainTimer = urand(10000,15000);
            } else ChainTimer -= uiDiff;

            // cast lava burst toutes les 15 secondes sur une cible random
            if(LavaTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                if(Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,1))
                    DoCast(pTarget, LAVA_BURST);
                    DoCast(me, LIGHTNING_SHIELD);
                    LavaTimer = 15000;
            } else LavaTimer -= uiDiff;

            // cast tremblement de terre toutes les 20 secondes sur une cible random
            if(ShockTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                if(Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,1))
                    DoCast(pTarget, EARTH_SHOCK);
                    DoCast(me, LIGHTNING_RING);
                    ShockTimer = 20000;
            } else ShockTimer -= uiDiff;

            // cast flame chock sur le tank toutes les 12 secondes
            if(FlameTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                DoCast(me->getVictim(), FLAME_SHOCK);
                FlameTimer = 12000;
            } else FlameTimer -= uiDiff;

            // A 25% hp se soigne avec healing wave
            if(HealthBelowPct(25) && !Healed)
            {
                me->InterruptNonMeleeSpells(false);
                DoCast(me, HEALING_WAVE);
                ChainTimer = 8000;
                LavaTimer = 10000;
                FlameTimer = 4000;
                ShockTimer = 15000;
                Healed = true;
            }

            // spawn des elementaire ) 50% hp
            if(HealthBelowPct(50) && !Haste)
            {
                me->InterruptNonMeleeSpells(false);
                DoCast(me, BLOODLUST);
                me->MonsterYell(SAY_MAGEGOBELIN_BLOOD, LANG_UNIVERSAL, 0);
                me->SummonCreature(SUMMON_AIR_ELEMENTAL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                Haste = true;
            }

            DoSpellAttackIfReady(LIGHTNING_BOLT);
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mage_gobelinAI (pCreature);
    }
};





/**********************************************/
/* Ingenieur Gobelin WavePort de Hurlevent   */
/**********************************************/



// script de l'image du joueur

class npc_stolen_soul : public CreatureScript
{
public:
    npc_stolen_soul() : CreatureScript("npc_stolen_soul") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_stolen_soulAI (creature);
    }

    struct npc_stolen_soulAI : public ScriptedAI
    {
        npc_stolen_soulAI(Creature* c) : ScriptedAI(c) { }

        uint8 myClass;
        uint32 Class_Timer;

        void Reset()
        {
            Class_Timer = 1000;
        }

        void EnterCombat(Unit* /*pWho*/)
        { }

        void SetMyClass(uint8 myclass)
        {
            myClass = myclass;
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if(!UpdateVictim())
                return;

            if(Class_Timer <= uiDiff)
            {
                switch(myClass)
                {
                    case CLASS_WARRIOR:
                        DoCast(me->getVictim(), MORTAL_STRIKE);
                        Class_Timer = 6000;
                        break;
                    case CLASS_PALADIN:
                        DoCast(me->getVictim(), HAMMER_OF_JUSTICE);
                        Class_Timer = 6000;
                        break;
                    case CLASS_HUNTER:
                        DoCast(me->getVictim(), FREEZING_TRAP);
                        Class_Timer = 20000;
                        break;
                    case CLASS_ROGUE:
                        DoCast(me->getVictim(), HEMORRHAGE);
                        Class_Timer = 10000;
                        break;
                    case CLASS_PRIEST:
                        DoCast(me->getVictim(), MIND_FLAY);
                        Class_Timer = 5000;
                        break;
                    case CLASS_SHAMAN:
                        DoCast(me->getVictim(), FROSTSHOCK);
                        Class_Timer = 8000;
                        break;
                    case CLASS_MAGE:
                        DoCast(me->getVictim(), FIREBALL);
                        Class_Timer = 5000;
                        break;
                    case CLASS_WARLOCK:
                        DoCast(me->getVictim(), CURSE_OF_AGONY);
                        Class_Timer = 20000;
                        break;
                    case CLASS_DRUID:
                        DoCast(me->getVictim(), MOONFIRE);
                        Class_Timer = 10000;
                        break;
                }
            } else Class_Timer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };
};





class npc_ingenieur_gobelin : public CreatureScript
{
public:
    npc_ingenieur_gobelin() : CreatureScript("npc_ingenieur_gobelin") { }

    struct npc_ingenieur_gobelinAI : public npc_escortAI
    {
        npc_ingenieur_gobelinAI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}

            uint8  soulclass;
            uint32 soulmodel;
            uint64 soulholder;
            uint32 StolenSoul_Timer;
            uint32 ExplosionTimer;
            uint32 CriTimer;
            uint32 RocketTimer;
            uint32 BombesTimer;
        
        void Reset()
        {
            soulmodel = 0;
            soulholder = 0;
            soulclass = 0;
            StolenSoul_Timer = 25000 + rand()% 10000;
            ExplosionTimer = 8000;
            CriTimer = 35000;
            RocketTimer = 12000;
            BombesTimer = 42000;
            SetupMovement(0);
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_INGENIEURGOBELIN_AGGRO, LANG_UNIVERSAL, 0);
        }
        
        void JustSummoned(Creature* summoned)
        {
            if(summoned->GetEntry() == SUMMON_STOLEN_SOUL)
            {
                summoned->CastSpell(summoned, STOLEN_SOUL_VISUAL, false);
                summoned->SetDisplayId(soulmodel);
                summoned->setFaction(me->getFaction());

                if(Unit* target = Unit::GetUnit(*me, soulholder))
                {
                CAST_AI(npc_stolen_soul::npc_stolen_soulAI, summoned->AI())->SetMyClass(soulclass);
                 summoned->AI()->AttackStart(target);
                }
            }
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_INGENIEURGOBELIN_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_INGENIEURGOBELIN_DEATH, LANG_UNIVERSAL, 0);
        }
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 81:SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -8292.07f, 1302.51f, 5.23119f);
                        AddWaypoint(2, -8292.57f, 1265.33f, 5.23119f);
                        AddWaypoint(3, -8292.82f, 1242.08f, 5.23071f);
                        AddWaypoint(4, -8300.48f, 1195.53f, 5.62398f);
                        AddWaypoint(5, -8300.66f, 1187.58f, 10.7239f);
                        AddWaypoint(6, -8300.96f, 1177.47f, 18.4496f);
                        AddWaypoint(7, -8300.67f, 1168.57f, 17.6878f);
                        AddWaypoint(8, -8301.41f, 1135.61f, 18.142f);
                        AddWaypoint(9, -8344.40f, 1135.74f, 18.7538f);
                        AddWaypoint(10, -8395.27f, 1135.89f, 17.9899f);
                        AddWaypoint(11, -8441.01f, 1136.17f, 18.8788f);
                        AddWaypoint(12, -8486.81f, 1135.76f, 18.0337f);
                        AddWaypoint(13, -8526.46f, 1133.86f, 18.2078f);
                        AddWaypoint(14, -8573.86f, 1131.58f, 17.9448f);
						AddWaypoint(15, -8580.71f, 1118.37f, 17.9448f);
						AddWaypoint(16, -8586.96f, 1105.54f, 25.0302f);
						AddWaypoint(17, -8592.05f, 1092.98f, 31.534f);
						AddWaypoint(18, -8586.94f, 1078.16f, 35.6756f);
						AddWaypoint(19, -8575.93f, 1063.11f, 41.4909f);
						AddWaypoint(20, -8556.01f, 1038.89f, 57.7437f);
						AddWaypoint(21, -8550.60f, 1036.61f, 59.3816f);
						AddWaypoint(22, -8537.39f, 1035.38f, 59.6563f);
						AddWaypoint(23, -8520.49f, 1017.92f, 59.9289f);
						AddWaypoint(24, -8504.76f, 1003.97f, 59.4779f);
						AddWaypoint(25, -8492.47f, 990.335f, 69.3196f);
						AddWaypoint(26, -8487.73f, 984.923f, 72.7377f);
						AddWaypoint(27, -8486.04f, 982.774f, 72.7377f);
						AddWaypoint(28, -8479.34f, 988.42f, 75.4748f);
						AddWaypoint(29, -8472.62f, 993.994f, 79.0666f);
						AddWaypoint(30, -8466.89f, 987.086f, 79.0392f);
						AddWaypoint(31, -8472.30f, 981.689f, 80.4825f);
						AddWaypoint(32, -8487.22f, 969.506f, 95.3373f);
						AddWaypoint(33, -8503.03f, 956.192f, 95.7726f);
						AddWaypoint(34, -8530.41f, 956.374f, 95.979f);
						AddWaypoint(35, -8545.89f, 962.205f, 96.5852f);
						AddWaypoint(36, -8571.21f, 992.509f, 96.2303f);
						AddWaypoint(37, -8590.40f, 977.718f, 97.7525f);
						AddWaypoint(38, -8632.35f, 944.139f, 99.52f);
						AddWaypoint(39, -8628.55f, 927.301f, 100.021f);
						AddWaypoint(40, -8639.78f, 915.532f, 99.2767f);
						AddWaypoint(41, -8682.28f, 878.042f, 97.0173f);
						AddWaypoint(42, -8650.31f, 837.51f, 96.7324f);
						AddWaypoint(43, -8633.90f, 814.968f, 96.63f);
						AddWaypoint(44, -8635.92f, 795.093f, 96.6511f);
						AddWaypoint(45, -8611.89f, 762.076f, 96.7211f);
						AddWaypoint(46, -8630.45f, 743.696f, 96.8469f);
						AddWaypoint(47, -8637.82f, 736.079f, 96.7511f);
						AddWaypoint(48, -8614.08f, 707.148f, 96.7303f);
						AddWaypoint(49, -8597.65f, 718.661f, 96.6746f);
						AddWaypoint(50, -8562.42f, 674.358f, 97.0155f);
						AddWaypoint(51, -8538.72f, 689.058f, 97.6675f);
						AddWaypoint(52, -8534.28f, 686.213f, 97.678f);
						AddWaypoint(53, -8525.75f, 674.779f, 102.326f);
						AddWaypoint(54, -8513.67f, 657.729f, 101.134f);
						AddWaypoint(55, -8510.26f, 652.719f, 100.292f);
						AddWaypoint(56, -8508.31f, 650.421f, 100.292f);
						AddWaypoint(57, -8489.67f, 674.499f, 100.593f);
						AddWaypoint(58, -8474.67f, 693.871f, 99.5743f);
						AddWaypoint(59, -8469.26f, 686.765f, 99.3762f);
						AddWaypoint(60, -8443.32f, 654.567f, 92.9822f);
						AddWaypoint(61, -8459.60f, 638.725f, 91.8592f);
						AddWaypoint(62, -8474.07f, 627.534f, 93.6498f);
						AddWaypoint(63, -8473.15f, 619.384f, 94.4642f);
						AddWaypoint(64, -8451.50f, 586.494f, 94.1368f);
						AddWaypoint(65, -8460.44f, 577.074f, 94.8139f);
						AddWaypoint(66, -8479.75f, 562.473f, 97.2927f);
						AddWaypoint(67, -8503.78f, 533.833f, 97.6536f);
						AddWaypoint(68, -8542.97f, 501.789f, 98.6207f);
						AddWaypoint(69, -8539.00f, 494.086f, 99.6874f);
						AddWaypoint(70, -8535.82f, 487.926f, 101.081f);
						AddWaypoint(71, -8538.93f, 482.365f, 101.795f);
						AddWaypoint(72, -8547.08f, 471.504f, 104.281f);
						AddWaypoint(73, -8545.23f, 464.217f, 104.435f);
						AddWaypoint(74, -8536.71f, 453.436f, 104.917f);
						AddWaypoint(75, -8520.64f, 432.804f, 106.221f);
						AddWaypoint(76, -8504.19f, 412.137f, 108.386f);
						AddWaypoint(77, -8488.66f, 392.631f, 108.386f);
						AddWaypoint(78, -8476.69f, 377.605f, 112.491f);
						AddWaypoint(79, -8457.82f, 353.765f, 120.091f);
						AddWaypoint(80, -8455.13f, 350.37f, 120.886f);
						AddWaypoint(81, -8446.93f, 339.988f, 121.33f);
						
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_ingenieur_gobelinAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_ingenieur_gobelinAI* pEscortAI = CAST_AI(npc_ingenieur_gobelinAI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if (!UpdateVictim())
                return;
            
            //Crée une image d'un des joueurs en random
            if(StolenSoul_Timer <= uiDiff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    if(target->GetTypeId() == TYPEID_PLAYER)
                    {
                        if(me->IsNonMeleeSpellCasted(false))
                           me->InterruptNonMeleeSpells(true);
                           soulmodel = target->GetDisplayId();
                           soulholder = target->GetGUID();
                           soulclass = target->getClass();
                           DoCast(target, STOLEN_SOUL);
                           me->SummonCreature(SUMMON_STOLEN_SOUL, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                           StolenSoul_Timer = 20000 + rand()% 10000;
                    } else StolenSoul_Timer = 1000;
                }
             } else StolenSoul_Timer -= uiDiff;
             
             
             //Cast une vague explosive autour de lui
             if(ExplosionTimer <= uiDiff)
            {
               DoCast(me, EXPLOSION);
               ExplosionTimer = 8000;
            } else ExplosionTimer -= uiDiff;
            
            //Lance Peur sur une clible random et ceux autour de la cible du random 
            if(CriTimer <= uiDiff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                  DoCast(target, CRI);
                  CriTimer = 35000;
                }   
            } else CriTimer -= uiDiff;
            
            //Lance une rocket sur une cible random 
            if(RocketTimer <= uiDiff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                  DoCast(target, ROCKET);
                  RocketTimer = 12000;
                }   
            } else RocketTimer -= uiDiff;
            
            
            //Lance une Bombe qui stun sur une cible random 
            if(BombesTimer <= uiDiff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                  DoCast(target, BOMBES);
                  BombesTimer = 42000;
                }   
            } else BombesTimer -= uiDiff;
             
        
             DoMeleeAttackIfReady();
          }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_ingenieur_gobelinAI (pCreature);
    }
};




/*******************************************/
/* Boss Gobelin Port de Hurlevent          */
/*******************************************/



class npc_bomberman : public CreatureScript
{
public:
    npc_bomberman() : CreatureScript("npc_bomberman") { }

    CreatureAI* GetAI(Creature* pCreature) 
    {
        return new npc_bombermanAI (pCreature);
    }

    struct npc_bombermanAI : public npc_escortAI
    {
        npc_bombermanAI(Creature *pCreature) : npc_escortAI(pCreature), Summons(me)
        {
            Reset();
        }
             

        uint32 Timer;
        uint32 BerserkTimer;
        uint32 WoundTimer;
        uint32 BombTimer;
        uint32 NetTimer;
        uint32 BladeTimer;
        uint32 ShrinkRayTimer;

        uint32 TowerTimer;
        uint32 AlarmTimer;

        SummonList Summons;
        Unit*  pTarget;
        bool Die;
  
/********************************************************************************************************************************************
		Fonction a tester pour une suite à l'event (essayer de récuperer 2 variables dans 2 class différente pour les utiliser dans une 3 iem
 ********************************************************************************************************************************************/        
/*        struct AnyDeadUnitCheck
        {
             bool operator()(Unit* u) { return !u->isAlive(); }
        };     
*/        

        void JustSummoned(Creature* summon)
        {
            Summons.Summon(summon);
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            Summons.Despawn(summon);
        }

        void Reset()
        {
            BerserkTimer = 600000; /* Berserk au bout de 10min */
            WoundTimer = 5000 ; /* Débuff tanks pour switch */
            BombTimer = 30000 ; /* Bombe vivante */
            NetTimer = 30000 ;  /* Piège la cible de Bombe vivante */
            BladeTimer = 10000 ; /* Coup au cac */
            ShrinkRayTimer = 15000 ; /* Cône de débuff */
            TowerTimer = 60000 ; /* Invoque Tower */
            AlarmTimer = 45000 ; /* Invoque Alarm */
            Summons.DespawnAll();
            SetupMovement(0);
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_BOMBERMAN_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            switch (urand(0, 2))
            {
                case 0:
                       me->MonsterYell(SAY_KILL_ONE, LANG_UNIVERSAL, 0);
                       break;
                case 1:
                       me->MonsterYell(SAY_KILL_TWO, LANG_UNIVERSAL, 0);
                       break;
                case 2:
                       me->MonsterYell(SAY_KILL_THREE, LANG_UNIVERSAL, 0);
                       break;
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_BOMBERMAN_DEATH, LANG_UNIVERSAL, 0);
            Summons.DespawnAll();
            //me->CAST_AI(npc_bomberman::npc_bombermanAI, me->AI())->Die == true;
            Die = true;  
        }
        
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 81:
					    SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -8292.07f, 1302.51f, 5.23119f);
                        AddWaypoint(2, -8292.57f, 1265.33f, 5.23119f);
                        AddWaypoint(3, -8292.82f, 1242.08f, 5.23071f);
                        AddWaypoint(4, -8300.48f, 1195.53f, 5.62398f);
                        AddWaypoint(5, -8300.66f, 1187.58f, 10.7239f);
                        AddWaypoint(6, -8300.96f, 1177.47f, 18.4496f);
                        AddWaypoint(7, -8300.67f, 1168.57f, 17.6878f);
                        AddWaypoint(8, -8301.41f, 1135.61f, 18.142f);
                        AddWaypoint(9, -8344.40f, 1135.74f, 18.7538f);
                        AddWaypoint(10, -8395.27f, 1135.89f, 17.9899f);
                        AddWaypoint(11, -8441.01f, 1136.17f, 18.8788f);
                        AddWaypoint(12, -8486.81f, 1135.76f, 18.0337f);
                        AddWaypoint(13, -8526.46f, 1133.86f, 18.2078f);
                        AddWaypoint(14, -8573.86f, 1131.58f, 17.9448f);
						AddWaypoint(15, -8580.71f, 1118.37f, 17.9448f);
						AddWaypoint(16, -8586.96f, 1105.54f, 25.0302f);
						AddWaypoint(17, -8592.05f, 1092.98f, 31.534f);
						AddWaypoint(18, -8586.94f, 1078.16f, 35.6756f);
						AddWaypoint(19, -8575.93f, 1063.11f, 41.4909f);
						AddWaypoint(20, -8556.01f, 1038.89f, 57.7437f);
						AddWaypoint(21, -8550.60f, 1036.61f, 59.3816f);
						AddWaypoint(22, -8537.39f, 1035.38f, 59.6563f);
						AddWaypoint(23, -8520.49f, 1017.92f, 59.9289f);
						AddWaypoint(24, -8504.76f, 1003.97f, 59.4779f);
						AddWaypoint(25, -8492.47f, 990.335f, 69.3196f);
						AddWaypoint(26, -8487.73f, 984.923f, 72.7377f);
						AddWaypoint(27, -8486.04f, 982.774f, 72.7377f);
						AddWaypoint(28, -8479.34f, 988.42f, 75.4748f);
						AddWaypoint(29, -8472.62f, 993.994f, 79.0666f);
						AddWaypoint(30, -8466.89f, 987.086f, 79.0392f);
						AddWaypoint(31, -8472.30f, 981.689f, 80.4825f);
						AddWaypoint(32, -8487.22f, 969.506f, 95.3373f);
						AddWaypoint(33, -8503.03f, 956.192f, 95.7726f);
						AddWaypoint(34, -8530.41f, 956.374f, 95.979f);
						AddWaypoint(35, -8545.89f, 962.205f, 96.5852f);
						AddWaypoint(36, -8571.21f, 992.509f, 96.2303f);
						AddWaypoint(37, -8590.40f, 977.718f, 97.7525f);
						AddWaypoint(38, -8632.35f, 944.139f, 99.52f);
						AddWaypoint(39, -8628.55f, 927.301f, 100.021f);
						AddWaypoint(40, -8639.78f, 915.532f, 99.2767f);
						AddWaypoint(41, -8682.28f, 878.042f, 97.0173f);
						AddWaypoint(42, -8650.31f, 837.51f, 96.7324f);
						AddWaypoint(43, -8633.90f, 814.968f, 96.63f);
						AddWaypoint(44, -8635.92f, 795.093f, 96.6511f);
						AddWaypoint(45, -8611.89f, 762.076f, 96.7211f);
						AddWaypoint(46, -8630.45f, 743.696f, 96.8469f);
						AddWaypoint(47, -8637.82f, 736.079f, 96.7511f);
						AddWaypoint(48, -8614.08f, 707.148f, 96.7303f);
						AddWaypoint(49, -8597.65f, 718.661f, 96.6746f);
						AddWaypoint(50, -8562.42f, 674.358f, 97.0155f);
						AddWaypoint(51, -8538.72f, 689.058f, 97.6675f);
						AddWaypoint(52, -8534.28f, 686.213f, 97.678f);
						AddWaypoint(53, -8525.75f, 674.779f, 102.326f);
						AddWaypoint(54, -8513.67f, 657.729f, 101.134f);
						AddWaypoint(55, -8510.26f, 652.719f, 100.292f);
						AddWaypoint(56, -8508.31f, 650.421f, 100.292f);
						AddWaypoint(57, -8489.67f, 674.499f, 100.593f);
						AddWaypoint(58, -8474.67f, 693.871f, 99.5743f);
						AddWaypoint(59, -8469.26f, 686.765f, 99.3762f);
						AddWaypoint(60, -8443.32f, 654.567f, 92.9822f);
						AddWaypoint(61, -8459.60f, 638.725f, 91.8592f);
						AddWaypoint(62, -8474.07f, 627.534f, 93.6498f);
						AddWaypoint(63, -8473.15f, 619.384f, 94.4642f);
						AddWaypoint(64, -8451.50f, 586.494f, 94.1368f);
						AddWaypoint(65, -8460.44f, 577.074f, 94.8139f);
						AddWaypoint(66, -8479.75f, 562.473f, 97.2927f);
						AddWaypoint(67, -8503.78f, 533.833f, 97.6536f);
						AddWaypoint(68, -8542.97f, 501.789f, 98.6207f);
						AddWaypoint(69, -8539.00f, 494.086f, 99.6874f);
						AddWaypoint(70, -8535.82f, 487.926f, 101.081f);
						AddWaypoint(71, -8538.93f, 482.365f, 101.795f);
						AddWaypoint(72, -8547.08f, 471.504f, 104.281f);
						AddWaypoint(73, -8545.23f, 464.217f, 104.435f);
						AddWaypoint(74, -8536.71f, 453.436f, 104.917f);
						AddWaypoint(75, -8520.64f, 432.804f, 106.221f);
						AddWaypoint(76, -8504.19f, 412.137f, 108.386f);
						AddWaypoint(77, -8488.66f, 392.631f, 108.386f);
						AddWaypoint(78, -8476.69f, 377.605f, 112.491f);
						AddWaypoint(79, -8457.82f, 353.765f, 120.091f);
						AddWaypoint(80, -8455.13f, 350.37f, 120.886f);
						AddWaypoint(81, -8446.93f, 339.988f, 121.33f);
						
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_bombermanAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_bombermanAI* pEscortAI = CAST_AI(npc_bombermanAI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
                    Die = false;
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
            npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            // Berserk
            if(BerserkTimer <= uiDiff)
            {
                DoCast(me, BERSERK);
                BerserkTimer = 600000;
            } else BerserkTimer -= uiDiff;

            if(WoundTimer <= uiDiff)
            {
                DoCast(me->getVictim(), MORTA_WOUND);
                WoundTimer = 5000;
            } else WoundTimer -= uiDiff;

            // Bombe dure 5 secondes avant de péter
            if(BombTimer <= uiDiff)
            {
                me->MonsterYell(SAY_LIVING_BOMB, LANG_UNIVERSAL, 0);
                if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM,1))
				{
					me->AddAura(LIVING_BOMB, pTarget);
					me->AddAura(SLOW_FALL, pTarget);
				}
				BombTimer = 10000;
            } else BombTimer -= uiDiff;

            // Piège dure 5 secondes
            if(NetTimer <= uiDiff)
            {
                if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM,1))
                    DoCast(pTarget, ELECTRIFIED_NET);
                    NetTimer = 30000;
            } else NetTimer -= uiDiff;

            // Coup au cac toutes les 10s
            if(BladeTimer <= uiDiff)
            {
                DoCast(me->getVictim(), SAW_BLADE);
                BladeTimer = 10000;
            } else BladeTimer -= uiDiff;

            // Débuff au cac dure 15s
            if(ShrinkRayTimer <= uiDiff)
            {
                DoCast(me->getVictim(), SUPER_SHRINK_RAY);
                ShrinkRayTimer = 15000;
            } else ShrinkRayTimer -= uiDiff;


            // Spawn des Adds

            if(TowerTimer <= uiDiff)
            {
                me->MonsterYell(SAY_TOWER_SPAWN, LANG_UNIVERSAL, 0);
                if(Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,1))
                    pTarget->SummonCreature(SUMMON_TOWER, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                    TowerTimer = 60000;
            } else TowerTimer -= uiDiff;

            if(AlarmTimer <= uiDiff)
            {
                if(Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,1))
                    pTarget->SummonCreature(SUMMON_ALARM, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                    AlarmTimer = 45000;
            } else AlarmTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };
    
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_bombermanAI (pCreature);
    } 
};





/*********************************************************/
/* 1er Minion du Boss Gobelin Port de Hurlevent          */ 
/*********************************************************/


class npc_add_bomberman_tower : public CreatureScript
{
public:
    npc_add_bomberman_tower() : CreatureScript("npc_add_bomberman_tower") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_add_bomberman_towerAI (pCreature);
    }

    struct npc_add_bomberman_towerAI : public ScriptedAI
    {
        npc_add_bomberman_towerAI(Creature *pCreature) : ScriptedAI(pCreature){}

            uint32 Timer;
            uint32 NovaTimer;
            uint32 WaterTimer;
		    uint32 DefenseTimer;

        void Reset()
        {
            NovaTimer = 9000; /* Nova toutes les 9sec */
            WaterTimer = 10000; /* Bolt toutes les 10sec */
			DefenseTimer = 15000; /* Boubou toutes les 15sec */
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_TOWER_DEATH, LANG_UNIVERSAL, 0);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if(!UpdateVictim())
                return;

            if(NovaTimer <= uiDiff)
            {
                DoCastAOE(ICE_NOVA);
                NovaTimer = 9000;
            } else NovaTimer -= uiDiff;

            if(WaterTimer <= uiDiff)
            {
                DoCast(me->getVictim(), WATER_SPRAY);
                WaterTimer = 10000;
            } else WaterTimer -= uiDiff;

            if(HealthBelowPct(20))
            {
				if(DefenseTimer <= uiDiff)
				{
	                DoCast(me, DEFENSE_MATRIX_TOWER);
					DefenseTimer = 15000;
				} else DefenseTimer -= uiDiff;
            }

			if(HealthBelowPct(1))
			{
				DoCast(me, EXPLOSION_TOWER);
			}

           DoMeleeAttackIfReady();
        }
    };

};


/*********************************************************/
/* 2ème Minion du Boss Gobelin Port de Hurlevent          */ 
/*********************************************************/


class npc_add_bomberman_alarm : public CreatureScript
{
public:
    npc_add_bomberman_alarm() : CreatureScript("npc_add_bomberman_alarm") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_add_bomberman_alarmAI (pCreature);
    }

    struct npc_add_bomberman_alarmAI : public ScriptedAI
    {
        npc_add_bomberman_alarmAI(Creature *pCreature) : ScriptedAI(pCreature), Summons(me){}

        uint32 Timer;
        uint32 AlertTimer;
		uint32 DefenseTimer;

        SummonList Summons;

        void JustSummoned(Creature* summon)
        {
            Summons.Summon(summon);
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            Summons.Despawn(summon);
        }

        void Reset()
        {
            AlertTimer = 10000;
			DefenseTimer = 15000; /* Boubou toutes les 15sec */
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_TOWER_DEATH, LANG_UNIVERSAL, 0);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if(!UpdateVictim())
                return;

            /* Lance une alerte et spawn une bombe */
            if(AlertTimer <= uiDiff)
            {
                me->MonsterYell(SAY_ALARM_ALERT, LANG_UNIVERSAL, 0);
                me->SummonCreature(SUMMON_BOMB, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                AlertTimer = 10000;
            } else AlertTimer -= uiDiff;

            if(HealthBelowPct(20))
            {
				if(DefenseTimer <= uiDiff)
				{
	                DoCast(me, DEFENSE_MATRIX_ALARM);
					DefenseTimer = 15000;
				} else DefenseTimer -= uiDiff;
            }

			if(HealthBelowPct(1))
			{
				DoCast(me, EXPLOSION_ALARM);
			}

           DoMeleeAttackIfReady();
        }
    };

};


/*********************************************************/
/* 3ème Minion du Boss Gobelin Port de Hurlevent          */ 
/*********************************************************/



class npc_add_alarm_bomb : public CreatureScript
{
public:
    npc_add_alarm_bomb() : CreatureScript("npc_add_alarm_bomb") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_add_alarm_bombAI (pCreature);
    }

    struct npc_add_alarm_bombAI : public ScriptedAI
    {
        npc_add_alarm_bombAI(Creature *pCreature) : ScriptedAI(pCreature), Summons(me){}

        uint32 Timer;
		uint32 ExplosionTimer;

        SummonList Summons;

        void JustSummoned(Creature* summon)
        {
            Summons.Summon(summon);
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            Summons.Despawn(summon);
        }

        void Reset()
        {
            ExplosionTimer = 7000;
        }
		
		void EnterCombat(Unit* /*who*/)
        {
            me->AddAura(LIVING_BOMB_MINE, me);
        }

        /* Spawn une mine à sa mort, despawn au bout de 5min */
        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_BOMB_DEATH, LANG_UNIVERSAL, 0);
            me->SummonCreature(SUMMON_MINE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 300000);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if(!UpdateVictim())
                return;

			if(ExplosionTimer <= uiDiff)
				{
	                DoCast(me, EXPLOSION_BOMB);
					ExplosionTimer = 7000;
				} else ExplosionTimer -= uiDiff;

           DoMeleeAttackIfReady();
        }
    };

};



/*********************************************/
/* Mage de Bataille A2 Wave port Hurlevent   */
/*********************************************/


class npc_mage_bataille : public CreatureScript
{
public:
    npc_mage_bataille() : CreatureScript("npc_mage_bataille") { }

    struct npc_mage_batailleAI : public npc_escortAI
    {
        npc_mage_batailleAI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}


        void Reset()
        {
			SetupMovement(0);
        }

        
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 2:SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -8299.42f, 1217.07f, 5.1512f);
						AddWaypoint(2, -8299.25f, 1234.92f, 5.23036f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_mage_batailleAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_mage_batailleAI* pEscortAI = CAST_AI(npc_mage_batailleAI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mage_batailleAI (pCreature);
    }
};





/*********************************************/
/* Mage de Bataille1 A2 Wave port Hurlevent   */
/*********************************************/


class npc_mage_bataille1 : public CreatureScript
{
public:
    npc_mage_bataille1() : CreatureScript("npc_mage_bataille1") { }

    struct npc_mage_bataille1AI : public npc_escortAI
    {
        npc_mage_bataille1AI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}


        void Reset()
        {
			SetupMovement(0);
        }

        
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 2:SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -8296.72f, 1213.33f, 5.26202f);
                        AddWaypoint(2, -8296.69f, 1231.59f, 5.23146f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_mage_bataille1AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_mage_bataille1AI* pEscortAI = CAST_AI(npc_mage_bataille1AI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mage_bataille1AI (pCreature);
    }
};




/*********************************************/
/* Mage de Bataille2 A2 Wave port Hurlevent   */
/*********************************************/


class npc_mage_bataille2 : public CreatureScript
{
public:
    npc_mage_bataille2() : CreatureScript("npc_mage_bataille2") { }

    struct npc_mage_bataille2AI : public npc_escortAI
    {
        npc_mage_bataille2AI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}


        void Reset()
        {
			SetupMovement(0);
        }

        
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 2:
					    SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -8293.85f, 1210.68f, 5.3449f);
						AddWaypoint(2, -8293.36f, 1233.89f, 5.23072f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_mage_bataille2AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_mage_bataille2AI* pEscortAI = CAST_AI(npc_mage_bataille2AI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mage_bataille2AI (pCreature);
    }
};



/*********************************************/
/* Mage de Bataille3 A2 Wave port Hurlevent   */
/*********************************************/


class npc_mage_bataille3 : public CreatureScript
{
public:
    npc_mage_bataille3() : CreatureScript("npc_mage_bataille3") { }

    struct npc_mage_bataille3AI : public npc_escortAI
    {
        npc_mage_bataille3AI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}


        void Reset()
        {
			SetupMovement(0);
        }

        
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 2:
					    SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -8280.58f, 1218.55f, 5.1211f);
                        AddWaypoint(2, -8280.87f, 1235.17f, 5.23041f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_mage_bataille3AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_mage_bataille3AI* pEscortAI = CAST_AI(npc_mage_bataille3AI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mage_bataille3AI (pCreature);
    }
};


/*********************************************/
/* Mage de Bataille4 A2 Wave port Hurlevent   */
/*********************************************/


class npc_mage_bataille4 : public CreatureScript
{
public:
    npc_mage_bataille4() : CreatureScript("npc_mage_bataille4") { }

    struct npc_mage_bataille4AI : public npc_escortAI
    {
        npc_mage_bataille4AI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}


        void Reset()
        {
			SetupMovement(0);
        }

        
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 2:
					    SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -8284.78f, 1215.06f, 5.19952f);
                        AddWaypoint(2, -8284.36f, 1231.50f, 5.2307f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_mage_bataille4AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_mage_bataille4AI* pEscortAI = CAST_AI(npc_mage_bataille4AI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mage_bataille4AI (pCreature);
    }
};



/*********************************************/
/* Mage de Bataille5 A2 Wave port Hurlevent   */
/*********************************************/


class npc_mage_bataille5 : public CreatureScript
{
public:
    npc_mage_bataille5() : CreatureScript("npc_mage_bataille5") { }

    struct npc_mage_bataille5AI : public npc_escortAI
    {
        npc_mage_bataille5AI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}


        void Reset()
        {
			SetupMovement(0);
        }

        
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 2:
					    SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -8288.11f, 1211.59f, 5.31996f);
                        AddWaypoint(2, -8287.50f, 1235.10f, 5.23063f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_mage_bataille5AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_mage_bataille5AI* pEscortAI = CAST_AI(npc_mage_bataille5AI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mage_bataille5AI (pCreature);
    }
};





/*********************************************/
/* Nain de Bataille A2 Wave Entrée Hurlevent */
/*********************************************/


class npc_nain_bataille : public CreatureScript
{
public:
    npc_nain_bataille() : CreatureScript("npc_nain_bataille") { }

    struct npc_nain_batailleAI : public npc_escortAI
    {
        npc_nain_batailleAI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}


        void Reset()
        {
			SetupMovement(0);
        }

        
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
			    case 6:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -9004.03f, 407.834f, 72.8211f);
						AddWaypoint(2, -9015.10f, 421.896f, 79.7957f);
						AddWaypoint(3, -9029.61f, 440.307f, 90.1843f);
						AddWaypoint(4, -9033.94f, 445.817f, 93.2958f);
						AddWaypoint(5, -9047.18f, 435.771f, 93.0561f);
						AddWaypoint(6, -9058.13f, 427.734f, 93.0561f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_nain_batailleAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_nain_batailleAI* pEscortAI = CAST_AI(npc_nain_batailleAI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_nain_batailleAI (pCreature);
    }
};




/*********************************************/
/* Nain de Bataille1 A2 Wave Entrée Hurlevent */
/*********************************************/


class npc_nain_bataille1 : public CreatureScript
{
public:
    npc_nain_bataille1() : CreatureScript("npc_nain_bataille1") { }

    struct npc_nain_bataille1AI : public npc_escortAI
    {
        npc_nain_bataille1AI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}


        void Reset()
        {
			SetupMovement(0);
        }

        
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 7:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -9000.62f, 407.729f, 73.1527f);
						AddWaypoint(2, -9011.71f, 421.691f, 78.8454f);
						AddWaypoint(3, -9028.47f, 442.946f, 90.8426f);
						AddWaypoint(4, -9030.82f, 445.986f, 92.8048f);
						AddWaypoint(5, -9033.92f, 449.784f, 93.2956f);
						AddWaypoint(6, -9048.57f, 438.357f, 93.0561f);
						AddWaypoint(7, -9057.44f, 431.337f, 93.0561f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_nain_bataille1AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_nain_bataille1AI* pEscortAI = CAST_AI(npc_nain_bataille1AI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_nain_bataille1AI (pCreature);
    }
};




/*********************************************/
/* Nain de Bataille2 A2 Wave Entrée Hurlevent */
/*********************************************/


class npc_nain_bataille2 : public CreatureScript
{
public:
    npc_nain_bataille2() : CreatureScript("npc_nain_bataille2") { }

    struct npc_nain_bataille2AI : public npc_escortAI
    {
        npc_nain_bataille2AI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}


        void Reset()
        {
			SetupMovement(0);
        }

        
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 6:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -8999.22f, 409.852f, 73.1341f);
						AddWaypoint(2, -9001.87f, 413.883f, 73.4745f);
						AddWaypoint(3, -9018.91f, 435.859f, 85.8151f);
						AddWaypoint(4, -9026.35f, 445.440f, 91.1945f);
						AddWaypoint(5, -9033.34f, 454.592f, 93.0559f);
						AddWaypoint(6, -9061.54f, 432.395f, 93.0559f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_nain_bataille2AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_nain_bataille2AI* pEscortAI = CAST_AI(npc_nain_bataille2AI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_nain_bataille2AI (pCreature);
    }
};









/*********************************************/
/* Nain de Bataille3 A2 Wave Entrée Hurlevent */
/*********************************************/


class npc_nain_bataille3 : public CreatureScript
{
public:
    npc_nain_bataille3() : CreatureScript("npc_nain_bataille3") { }

    struct npc_nain_bataille3AI : public npc_escortAI
    {
        npc_nain_bataille3AI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}


        void Reset()
        {
			SetupMovement(0);
        }

        
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 6:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -9067.66f, 499.437f, 75.3264f);
						AddWaypoint(2, -9065.04f, 496.191f, 75.9691f);
						AddWaypoint(3, -9043.15f, 468.842f, 91.2948f);
						AddWaypoint(4, -9035.90f, 458.572f, 93.0563f);
						AddWaypoint(5, -9053.11f, 445.273f, 93.0563f);
                        AddWaypoint(6, -9064.57f, 436.427f, 93.0563f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_nain_bataille3AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_nain_bataille3AI* pEscortAI = CAST_AI(npc_nain_bataille3AI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_nain_bataille3AI (pCreature);
    }
};





/*********************************************/
/* Nain de Bataille4 A2 Wave Entrée Hurlevent */
/*********************************************/


class npc_nain_bataille4 : public CreatureScript
{
public:
    npc_nain_bataille4() : CreatureScript("npc_nain_bataille4") { }

    struct npc_nain_bataille4AI : public npc_escortAI
    {
        npc_nain_bataille4AI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}


        void Reset()
        {
			SetupMovement(0);
        }

        
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 6:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -9068.89f, 495.839f, 75.6429f);
						AddWaypoint(2, -9063.18f, 489.000f, 78.9743f);
						AddWaypoint(3, -9045.65f, 466.878f, 91.4865f);
						AddWaypoint(4, -9040.13f, 459.739f, 93.0562f);
						AddWaypoint(5, -9056.63f, 446.661f, 93.0562f);
						AddWaypoint(6, -9064.04f, 440.398f, 93.0562f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_nain_bataille4AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_nain_bataille4AI* pEscortAI = CAST_AI(npc_nain_bataille4AI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);

            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_nain_bataille4AI (pCreature);
    }
};






/*********************************************/
/* Nain de Bataille5 A2 Wave Entrée Hurlevent */
/*********************************************/


class npc_nain_bataille5 : public CreatureScript
{
public:
    npc_nain_bataille5() : CreatureScript("npc_nain_bataille5") { }

    struct npc_nain_bataille5AI : public npc_escortAI
    {
        npc_nain_bataille5AI(Creature *pCreature) : npc_escortAI(pCreature)
		{
			Reset();
		}


        void Reset()
        {
			SetupMovement(0);
        }

        
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 6:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, -9071.22f, 492.501f, 76.7766f);
						AddWaypoint(2, -9069.09f, 489.541f, 77.4318f);
						AddWaypoint(3, -9049.99f, 465.758f, 90.9363f);
						AddWaypoint(4, -9047.13f, 461.725f, 93.2955f);
						AddWaypoint(5, -9060.08f, 447.311f, 93.0559f);
						AddWaypoint(6, -9067.96f, 441.067f, 93.0559f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_nain_bataille5AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_nain_bataille5AI* pEscortAI = CAST_AI(npc_nain_bataille5AI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

        void UpdateAI(const uint32 uiDiff)
        {
		    npc_escortAI::UpdateAI(uiDiff);
            if(!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_nain_bataille5AI (pCreature);
    }
};




/*              void OnGameObjectCreate(GameObject* go)
        {
            switch(go->GetEntry())
            {
                case 
                    break;
               
               
                case GO_TRIBUNAL_CONSOLE:
                    uiTribunalConsole = go->GetGUID();
                    break;
                case GO_TRIBUNAL_CHEST:
                case GO_TRIBUNAL_CHEST_HERO:
                    uiTribunalChest = go->GetGUID();
                    if(m_auiEncounter[2] == DONE)
                        go->RemoveFlag(GAMEOBJECT_FLAGS,GO_FLAG_INTERACT_COND);
                    break;
                case GO_TRIBUNAL_SKYROOM_FLOOR:
                    uiTribunalSkyFloor = go->GetGUID();
                    break;
            }
        }

*/




/*                  void SetInCombat()
        {
            Map *map = me->GetMap();
            if(!map->IsDungeon())
                return;

            Map::PlayerList const &PlayerList = map->GetPlayers();
            for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if(Player* i_pl = i->getSource())
                    if(!i_pl->isGameMaster() && i_pl->isAlive() && me->GetDistance(i_pl) <= 100)
                    {
                        me->SetInCombatWith(i_pl);
                        i_pl->SetInCombatWith(me);
                        me->AddThreat(i_pl, 1.0f);
                    }
            }
        }
*/


void AddSC_npcevent()
{
	new npcevent;
    new npcmarcus;
    new npcjaina; 
    new npc_archer_defias;
    new npc_mage_defias;
    new npc_infanterie_defias;
    new npc_lycanthrope;
    new npc_trash_lycan;
    new mob_lycan_vortex;
    new npc_mage_gobelin;
    new npc_infanterie_gobelin;
    new npc_ingenieur_gobelin;
    new npc_stolen_soul;
    new npc_bomberman;
    new npc_add_bomberman_tower;
    new npc_add_bomberman_alarm;
    new npc_add_alarm_bomb;
    new npc_mage_bataille;
    new npc_mage_bataille1;
    new npc_mage_bataille2;
    new npc_mage_bataille3;
    new npc_mage_bataille4;
    new npc_mage_bataille5;
    new npc_nain_bataille;
    new npc_nain_bataille1;
    new npc_nain_bataille2;
    new npc_nain_bataille3;
    new npc_nain_bataille4;
    new npc_nain_bataille5;
}

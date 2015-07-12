
/*******************************************************
		Event de Orgrimmar Attaque capital
 *******************************************************/

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"
#include "GameEventMgr.h"
#include "attaquecapitalH2.h"


//Roi Thrall de Orgrimmar
class npcthrall : public CreatureScript
{
	public:
		npcthrall() : CreatureScript("npcthrall") {}
		
	struct npcthrallAI : public npc_escortAI
	{    
		uint32 Phase;
		uint32 uiPhaseTimer, uiStep;
		bool bStepping;
		char msg[255];
		bool debug;

		npcthrallAI(Creature* creature) : npc_escortAI(creature),summons(me)
        {
            me->SetReactState(REACT_AGGRESSIVE);
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
		    sGameEventMgr->StartEvent(93);
            sWorld->ForceGameEventUpdate();  
        }    

        void JumpToNextStep(uint32 uiTimer)
        {
            uiPhaseTimer = uiTimer;
            ++uiStep;

			if(debug)
			{
				//sprintf(msg, " UiStep : %u", uiStep);
				sLog->outString("DEBUG: npcthralluistep est a %u", uiStep);
			}
        }


        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
                case 2:
                        SetEscortPaused(true);
                        me->HandleEmoteCommand(378);
                        //me->HandleEmoteCommand(412);
                        me->MonsterSay("Frêre de la Horde, Merci d'avoir répondu à mon appel. L'aube d'Argent et la Garde Foudrepique ont décidés de s'allier afin de raser notre Forteresse.", LANG_UNIVERSAL, 0);
                        uiStep=0;
                        JumpToNextStep(8000);
						break;
				case 5:
                        me->HandleEmoteCommand(378);
						me->MonsterSay("Il vous faudra pour repousser ces deux attaques frontales! Séparer vos forces en deux Groupes.\nUn qui ira consolider les Portes de notre Forteresse, et l'autre qui iront unir leurs forces pour repousser les assaults de l'entrée Ouest!", LANG_UNIVERSAL, 0);
                        me->HandleEmoteCommand(378);
                        SetEscortPaused(true);
                        uiStep=0;
                        JumpToNextStep(7000);
						break;		
                case 7:
                        me->HandleEmoteCommand(378);
						me->MonsterSay("Le groupe qui se portera volontaire pour défendre l'entrée de notre forteresse seront sous les ordres du Général Goderik.", LANG_UNIVERSAL, 0);
						me->SummonCreature(100046, 1925.598022f, -4207.887695f, 42.321892f, 1.647730f);
                        me->HandleEmoteCommand(378);
                        SetEscortPaused(true);
                        uiStep=0;
                        JumpToNextStep(7000);
						break;
				case 10:
                        me->HandleEmoteCommand(378);
                        me->MonsterSay("Le groupe qui se portera volontaire pour défendre entrée Ouest seront sous les ordres du Général Vol'jin.", LANG_UNIVERSAL, 0);
                        me->HandleEmoteCommand(378);
                        me->PlayDirectSound(3900);// Music intro
                        SetEscortPaused(true);
                        JumpToNextStep(26000);
						break;		        		
				
			}
		}

		void SetupMovement(uint32 variation)
            {
                switch (variation)
                {
                    case 0:       
                           AddWaypoint(1, 1919.93f, -4125.46f, 43.4266f);
						   AddWaypoint(2, 1920.30f, -4130.21f, 43.1400f);
                           AddWaypoint(3, 1920.87f, -4137.43f, 40.5499f);
                           AddWaypoint(4, 1914.98f, -4142.03f, 40.6245f);
                           AddWaypoint(5, 1915.58f, -4142.40f, 40.6245f);
                           AddWaypoint(6, 1929.12f, -4142.12f, 40.6289f);
                           AddWaypoint(7, 1927.31f, -4142.90f, 40.6236f);
                           AddWaypoint(8, 1920.77f, -4136.42f, 40.5337f);
                           AddWaypoint(9, 1920.21f, -4128.98f, 43.1401f);
                           AddWaypoint(10, 1920.29f, -4130.32f, 43.1402f);
                        break;
                    default:
                        sLog->outError("Unexpected movement variation (%i) in npcthrallAI::SetupMovement call!", variation);
                        return;
                }
                    if (npcthrallAI* pEscortAI = CAST_AI(npcthrallAI, me->AI()))
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
								JumpToNextStep(500); 
                                break;
                        case 1:
                                SetEscortPaused(false);
                                break;
                        case 2:
                                me->HandleEmoteCommand(412);
                                me->MonsterSay("Les chiens! Le Général Vol'jin est en route à sont arrivé Héros nous formerons les rangs.", LANG_UNIVERSAL, 0);
                                me->SummonCreature(100047, 1925.598022f, -4207.887695f, 42.321892f, 1.647730f);
                                JumpToNextStep(5000);
                                break;
                        case 3:
                                me->HandleEmoteCommand(412);
                                me->MonsterSay("Général Goderik voici les Héros qui seront sous votre commandemant pour défendre les portes de notre Forteresse.", LANG_UNIVERSAL, 0);
                                JumpToNextStep(25000);
                                break;
                        case 4:
                                me->HandleEmoteCommand(412);
                                me->MonsterSay("Héros Former les Rangs! Les Trolls seront nos alliés sur le Champ de bataille, portez leur assistances si nécessaire!", LANG_UNIVERSAL, 0);
                                JumpToNextStep(10000);
                                break;
                        case 5:
                                me->HandleEmoteCommand(412);
                                me->MonsterSay("La défaite n'est pas envisageable Héros! Pour la Horde! Pour l'Honneur!", LANG_UNIVERSAL, 0);
                                JumpToNextStep(5000);
                                break;
                        case 6: 
                                me->HandleEmoteCommand(412);
                                me->MonsterSay("Triomphez de nos ennemis, et vous serez grandement récompensés! Orgrimmar ne doit pas Tomber! Lok'tar Ogar!", LANG_UNIVERSAL, 0);
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
        return new npcthrallAI(creature);
    }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM( 9, "Nous avons entendu votre appel!", GOSSIP_SENDER_MAIN, 1);
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
		npcthrallAI* NPCAI = CAST_AI(npcthrall::npcthrallAI, creature->AI());

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

//Général Goderik Orgrimmar


class npcgoderik : public CreatureScript
{
	public:
		npcgoderik() : CreatureScript("npcgoderik") {}
		
	struct npcgoderikAI : public npc_escortAI
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

		npcgoderikAI(Creature* creature) : npc_escortAI(creature),summons(me)
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
                if((uint32)OrgrimmarBaseWaves[waveID][i][0] == 0) break;

                if(Creature* pTemp = me->SummonCreature((uint32)OrgrimmarBaseWaves[waveID][i][0], OrgrimmarBaseWaves[waveID][i][1], OrgrimmarBaseWaves[waveID][i][2], OrgrimmarBaseWaves[waveID][i][3], OrgrimmarBaseWaves[waveID][i][4], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000))
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
				case 0: textId = "1er Assaut aux Portes de Orgrimmar en Approche !";
				        me->SummonCreature(100059, 1426.0357f, -4383.5664f, 25.4622f, 1.65f);
				        me->SummonCreature(100060, 1428.7165f, -4385.6040f, 25.4629f, 1.65f);
				        me->SummonCreature(100061, 1430.0234f, -4382.8876f, 25.4629f, 1.65f);
				        me->SummonCreature(100062, 1431.8599f, -4377.2456f, 25.4629f, 1.65f);
				        me->SummonCreature(100063, 1434.3652f, -4379.2373f, 25.4629f, 1.65f);
				        me->SummonCreature(100064, 1436.4489f, -4376.9970f, 25.4629f, 1.65f);
				        break; 
				case 1: textId = "2ème Assaut aux Portes de Orgrimmar en Approche !";
				        me->SummonCreature(100059, 1426.0357f, -4383.5664f, 25.4622f, 1.65f);
				        me->SummonCreature(100060, 1428.7165f, -4385.6040f, 25.4629f, 1.65f);
				        me->SummonCreature(100061, 1430.0234f, -4382.8876f, 25.4629f, 1.65f);
				        me->SummonCreature(100062, 1431.8599f, -4377.2456f, 25.4629f, 1.65f);
				        me->SummonCreature(100063, 1434.3652f, -4379.2373f, 25.4629f, 1.65f);
				        me->SummonCreature(100064, 1436.4489f, -4376.9970f, 25.4629f, 1.65f);
				        break;
				case 2: textId = "3ème Assaut aux Portes de Orgrimmar en Approche !";
				        me->SummonCreature(100059, 1426.0357f, -4383.5664f, 25.4622f, 1.65f);
				        me->SummonCreature(100060, 1428.7165f, -4385.6040f, 25.4629f, 1.65f);
				        me->SummonCreature(100061, 1430.0234f, -4382.8876f, 25.4629f, 1.65f);
				        me->SummonCreature(100062, 1431.8599f, -4377.2456f, 25.4629f, 1.65f);
				        me->SummonCreature(100063, 1434.3652f, -4379.2373f, 25.4629f, 1.65f);
				        me->SummonCreature(100064, 1436.4489f, -4376.9970f, 25.4629f, 1.65f);
				        break;
				case 3: textId = "4ème Assaut aux Portes de Orgrimmar en Approche !";
				        me->SummonCreature(100059, 1426.0357f, -4383.5664f, 25.4622f, 1.65f);
				        me->SummonCreature(100060, 1428.7165f, -4385.6040f, 25.4629f, 1.65f);
				        me->SummonCreature(100061, 1430.0234f, -4382.8876f, 25.4629f, 1.65f);
				        me->SummonCreature(100062, 1431.8599f, -4377.2456f, 25.4629f, 1.65f);
				        me->SummonCreature(100063, 1434.3652f, -4379.2373f, 25.4629f, 1.65f);
				        me->SummonCreature(100064, 1436.4489f, -4376.9970f, 25.4629f, 1.65f);
				        break;
				case 4: textId = "5ème Assaut aux Portes de Orgrimmar en Approche !";
				        me->SummonCreature(100059, 1426.0357f, -4383.5664f, 25.4622f, 1.65f);
				        me->SummonCreature(100060, 1428.7165f, -4385.6040f, 25.4629f, 1.65f);
				        me->SummonCreature(100061, 1430.0234f, -4382.8876f, 25.4629f, 1.65f);
				        me->SummonCreature(100062, 1431.8599f, -4377.2456f, 25.4629f, 1.65f);
				        me->SummonCreature(100063, 1434.3652f, -4379.2373f, 25.4629f, 1.65f);
				        me->SummonCreature(100064, 1436.4489f, -4376.9970f, 25.4629f, 1.65f);
				        break;
				case 5: textId = "6ème Assaut aux Portes de Orgrimmar en Approche !";
				        me->SummonCreature(100059, 1426.0357f, -4383.5664f, 25.4622f, 1.65f);
				        me->SummonCreature(100060, 1428.7165f, -4385.6040f, 25.4629f, 1.65f);
				        me->SummonCreature(100061, 1430.0234f, -4382.8876f, 25.4629f, 1.65f);
				        me->SummonCreature(100062, 1431.8599f, -4377.2456f, 25.4629f, 1.65f);
				        me->SummonCreature(100063, 1434.3652f, -4379.2373f, 25.4629f, 1.65f);
				        me->SummonCreature(100064, 1436.4489f, -4376.9970f, 25.4629f, 1.65f);
				        break;
				case 6: textId = "7ème Assaut aux Portes de Orgrimmar en Approche !";
				        me->SummonCreature(100059, 1426.0357f, -4383.5664f, 25.4622f, 1.65f);
				        me->SummonCreature(100060, 1428.7165f, -4385.6040f, 25.4629f, 1.65f);
				        me->SummonCreature(100061, 1430.0234f, -4382.8876f, 25.4629f, 1.65f);
				        me->SummonCreature(100062, 1431.8599f, -4377.2456f, 25.4629f, 1.65f);
				        me->SummonCreature(100063, 1434.3652f, -4379.2373f, 25.4629f, 1.65f);
				        me->SummonCreature(100064, 1436.4489f, -4376.9970f, 25.4629f, 1.65f);
				        break;
				case 7: textId = "8ème Assaut aux Portes de Orgrimmar en Approche !";
				        me->SummonCreature(100059, 1426.0357f, -4383.5664f, 25.4622f, 1.65f);
				        me->SummonCreature(100060, 1428.7165f, -4385.6040f, 25.4629f, 1.65f);
				        me->SummonCreature(100061, 1430.0234f, -4382.8876f, 25.4629f, 1.65f);
				        me->SummonCreature(100062, 1431.8599f, -4377.2456f, 25.4629f, 1.65f);
				        me->SummonCreature(100063, 1434.3652f, -4379.2373f, 25.4629f, 1.65f);
				        me->SummonCreature(100064, 1436.4489f, -4376.9970f, 25.4629f, 1.65f);
				        break;
                case 8: textId = "Attention, Le général de L'aube d'Argent est en approche Préparez-vous !";
                        me->SummonCreature(100059, 1426.0357f, -4383.5664f, 25.4622f, 1.65f);
				        me->SummonCreature(100060, 1428.7165f, -4385.6040f, 25.4629f, 1.65f);
				        me->SummonCreature(100061, 1430.0234f, -4382.8876f, 25.4629f, 1.65f);
				        me->SummonCreature(100062, 1431.8599f, -4377.2456f, 25.4629f, 1.65f);
				        me->SummonCreature(100063, 1434.3652f, -4379.2373f, 25.4629f, 1.65f);
				        me->SummonCreature(100064, 1436.4489f, -4376.9970f, 25.4629f, 1.65f);

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
				sLog->outString("DEBUG: Goderikuistep est a %u", uiStep);
			}
        }

       void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
			    case 2:
			           SetEscortPaused(true);
			           uiStep=0;
			           JumpToNextStep(6000);
			           break;
                case 3:
                        SetEscortPaused(true);
                        me->HandleEmoteCommand(66);
                        uiStep=0;
                        JumpToNextStep(8000);
						break;
                case 4:
                        SetEscortPaused(true);
						me->MonsterYell("Mon Seigneur, nos éclaireurs nous signalent des mouvements massives de troupes de l'Aube d'Argent arrivant aux Portes de Orgrimmar! ", LANG_UNIVERSAL, 0);
                        me->HandleEmoteCommand(378);
                        uiStep=0;
                        JumpToNextStep(5000);
						break;
                case 6: 
                        SetEscortPaused(true);
						me->MonsterYell("L'Aube d'Argent ont eut vent que nos Forces étaient dispersés dans tous le Royaume de Kalimdor et profitent de ce moment de faiblesse! ", LANG_UNIVERSAL, 0);
                        me->HandleEmoteCommand(378);
                        uiStep=0;
                        JumpToNextStep(60000);
                        break;       
                case 8:
                        SetEscortPaused(true);
                        me->PlayDirectSound(11708);
						me->MonsterYell("Que le Raid soutenant les Portes de la Forteresse me suive! ", LANG_UNIVERSAL, 0);
                        me->HandleEmoteCommand(378);
                        JumpToNextStep(12000);                       
                        break;
		    	case 12:
						SetEscortPaused(true);
						me->MonsterSay("Tous en Montures mes frêres, et suivez moi!", LANG_UNIVERSAL, 0);
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
					   AddWaypoint(1, 1923.52f, -4176.77f, 41.1298f);
                       AddWaypoint(2, 1920.96f, -4144.89f, 40.6291f);
                       AddWaypoint(3, 1920.8f, -4137.24f, 40.5463f);
                       AddWaypoint(4, 1920.36f, -4133.18f, 42.0747f);
                       AddWaypoint(5, 1917.41f, -4132.52f, 42.5904f);
                       AddWaypoint(6, 1918.2f, -4132.00f, 42.9418f);
                       AddWaypoint(7, 1916.47f, -4128.43f, 43.1914f);
                       AddWaypoint(8, 1917.77f, -4132.38f, 42.6024f);
                       AddWaypoint(9, 1923.62f, -4178.9f, 41.1494f);
                       AddWaypoint(10, 1924.29f, -4187.27f, 42.1695f);
                       AddWaypoint(11, 1928.21f, -4248.2f, 41.501f);
                       AddWaypoint(12, 1932.73f, -4277.89f, 30.2976f);
                       AddWaypoint(13, 1937.23f, -4379.42f, 20.7319f);
                       AddWaypoint(14, 1943.95f, -4416.88f, 23.0321f);
                       AddWaypoint(15, 1935.8f, -4452.15f, 23.4811f);
                       AddWaypoint(16, 1913.64f, -4480.76f, 22.1305f);
                       AddWaypoint(17, 1890.07f, -4480.92f, 20.0355f);
                       AddWaypoint(18, 1877.64f, -4479.76f, 19.1636f);
                       AddWaypoint(19, 1859.41f, -4488.97f, 20.2728f);
                       AddWaypoint(20, 1826.46f, -4520.06f, 21.2107f);
                       AddWaypoint(21, 1772.35f, -4528.45f, 24.8683f);
                       AddWaypoint(22, 1734.77f, -4495.89f, 28.8264f);
                       AddWaypoint(23, 1730.63f, -4468.52f, 34.2398f);
                       AddWaypoint(24, 1742.2f, -4441.72f, 37.6663f);
                       AddWaypoint(25, 1738.8f, -4421.34f, 36.3932f);
                       AddWaypoint(26, 1721.77f, -4391.26f, 32.7598f);
                       AddWaypoint(27, 1707.21f, -4372.27f, 26.665f);
                       AddWaypoint(28, 1688.07f, -4383.71f, 24.2503f);
                       AddWaypoint(29, 1656.84f, -4410.43f, 17.3051f);
                       AddWaypoint(30, 1600.68f, -4438.82f, 6.5072f);
                       AddWaypoint(31, 1511.97f, -4414.5f, 18.8587f);
                       AddWaypoint(32, 1440.41f, -4421.62f, 25.4632f);
                       AddWaypoint(33, 1430.87f, -4366.99f, 25.4632f);
                       AddWaypoint(34, 1376.85f, -4371.16f, 26.0247f);
                       AddWaypoint(35, 1363.56f, -4397.79f, 29.2606f);
                       AddWaypoint(36, 1363.02f, -4393.03f, 29.3431f);
                        break;
				default:
                        sLog->outError("Unexpected movement variation (%i) in npcgoderikAI::SetupMovement call!", variation);
                        return;
			}
			if(npcgoderikAI* pEscortAI = CAST_AI(npcgoderikAI, me->AI()))
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
			
			if(sGameEventMgr->IsActiveEvent(92))
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
						            me->SummonCreature(100048, 1387.3317f, -4373.9150f, 27.2426f, 3.2704f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000);
						            me->SummonCreature(100048, 1385.8017f, -4363.7065f, 27.2426f, 3.3411f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000);
						       	    me->MonsterSay("Voici deux cannon encore en état de marchent pour vous aider dans votre Combat", LANG_UNIVERSAL, 0);
						       	    JumpToNextStep(5000);
						       	    break;
						    case 8:
						            me->MonsterSay("Placez-y vos deux meilleurs Artilleurs Héros. Ils vous seront d'un grands soutiens!", LANG_UNIVERSAL, 0);
						       	    JumpToNextStep(5000);
						       	    break;		
							case 9: 
									me->MonsterYell("Que Dieu vous protèges Frères de la Horde, Lok'tar Ogar!", LANG_UNIVERSAL, 0);
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
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Premier Assaut Stoppé aux Portes de Orgrimmar! ");                              
									me->MonsterTextEmote("Premier Assaut Stoppé aux Portes de Orgrimmar!", 0, true);
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
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Deuxième Assaut Stoppé aux Portes de Orgrimmar! ");
									me->MonsterTextEmote("Deuxième Assaut Stoppé aux Portes de Orgrimmar!", 0, true);
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
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Troisième Assaut Stoppé aux Portes de Orgrimmar! ");
									me->MonsterTextEmote("Troisième Assaut Stoppé aux Portes de Orgrimmar!", 0, true);
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
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Quatrième Assaut Stoppé aux Portes de Orgrimmar! ");
									me->MonsterTextEmote("Quatrième Assaut Stoppé aux Portes de Orgrimmar!", 0, true);
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
									me->MonsterYell("L'Aube d'Argent ne doivent pas franchir nos Portes!!", LANG_UNIVERSAL, 0);
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
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Cinquième Assaut Stoppé aux Portes de Orgrimmar! "); 
									me->MonsterTextEmote("Cinquième Assaut Stoppé aux Portes de Orgrimmar!", 0, true);
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
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Sixième Assaut Stoppé aux Portes de Orgrimmar! "); 
									me->MonsterTextEmote("Sixième Assaut Stoppé aux Portes de Orgrimmar!", 0, true);
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
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Septième Assaut Stoppé aux Portes de Orgrimmar! ");
									me->MonsterTextEmote("Septième Assaut Stoppé aux Portes de Orgrimmar!", 0, true);
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
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Huitème Assaut Stoppé aux Portes de Orgrimmar! "); 
									me->MonsterTextEmote("Huitème Assaut Stoppé aux Portes de Orgrimmar!", 0, true);
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
									me->MonsterYell("Bravo Héros, vous venez de repousser les assauts de l'Aube d'Argent! Préparez-vous ! Leur Général arrive!", LANG_UNIVERSAL, 0);
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
									sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "La Horde a Défendu Vaillament Les Portes de Orgrimmar!");
									me->MonsterTextEmote("La Horde a Défendu Vaillament Les Portes de Orgrimmar!", 0, true);
									JumpToNextStep(4000);
							case 36:
									JumpToNextStep(4000);
									break;
							case 37:
							        me->MonsterYell("Bravo Héros, vous venez de repousser les assauts de l'Aube d'Argent! Orgrimmar vous sera indéfiniment redevable!", LANG_UNIVERSAL, 0);
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
        return new npcgoderikAI(creature);
    }
};



/*****************************/
/* Général Vol'jin Orgrimmar */
/*****************************/

class npcvoljin : public CreatureScript
{
	public:
		npcvoljin() : CreatureScript("npcvoljin") {}
		
	struct npcvoljinAI : public npc_escortAI
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
		
		

		npcvoljinAI(Creature* creature) : npc_escortAI(creature)
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
                if((uint32)OuestBaseWaves[waveID][i][0] == 0) break;

                if(Creature* pTemp = me->SummonCreature((uint32)OuestBaseWaves[waveID][i][0], OuestBaseWaves[waveID][i][1], OuestBaseWaves[waveID][i][2], OuestBaseWaves[waveID][i][3], OuestBaseWaves[waveID][i][4], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000))
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
				case 0: textId = "1er Assaut à l'entrée Ouest de Orgrimmar en Approche !";
				        me->SummonCreature(100065, 1753.5770f, -3987.4057f, 48.8544f, 3.60f);
				        me->SummonCreature(100066, 1756.5201f, -3988.6333f, 49.0605f, 3.60f);
				        me->SummonCreature(100067, 1755.1279f, -3991.0725f, 48.7217f, 3.60f);
				        me->SummonCreature(100068, 1728.6073f, -3999.0375f, 51.2886f, 0.46f);
				        me->SummonCreature(100069, 1726.7478f, -4001.9130f, 51.8847f, 0.46f);
				        me->SummonCreature(100070, 1729.9404f, -4002.9021f, 50.9726f, 0.46f);
				        break; 
				case 1: textId = "2ème Assaut à l'entrée Ouest de Orgrimmar en Approche !";
				        me->SummonCreature(100065, 1753.5770f, -3987.4057f, 48.8544f, 3.60f);
				        me->SummonCreature(100066, 1756.5201f, -3988.6333f, 49.0605f, 3.60f);
				        me->SummonCreature(100067, 1755.1279f, -3991.0725f, 48.7217f, 3.60f);
				        me->SummonCreature(100068, 1728.6073f, -3999.0375f, 51.2886f, 0.46f);
				        me->SummonCreature(100069, 1726.7478f, -4001.9130f, 51.8847f, 0.46f);
				        me->SummonCreature(100070, 1729.9404f, -4002.9021f, 50.9726f, 0.46f);
				        break;
				case 2: textId = "3ème Assaut à l'entrée Ouest de Orgrimmar en Approche !";
				        me->SummonCreature(100065, 1753.5770f, -3987.4057f, 48.8544f, 3.60f);
				        me->SummonCreature(100066, 1756.5201f, -3988.6333f, 49.0605f, 3.60f);
				        me->SummonCreature(100067, 1755.1279f, -3991.0725f, 48.7217f, 3.60f);
				        me->SummonCreature(100068, 1728.6073f, -3999.0375f, 51.2886f, 0.46f);
				        me->SummonCreature(100069, 1726.7478f, -4001.9130f, 51.8847f, 0.46f);
				        me->SummonCreature(100070, 1729.9404f, -4002.9021f, 50.9726f, 0.46f);
				        break;
				case 3: textId = "4ème Assaut à l'entrée Ouest de Orgrimmar en Approche !";
				        me->SummonCreature(100065, 1753.5770f, -3987.4057f, 48.8544f, 3.60f);
				        me->SummonCreature(100066, 1756.5201f, -3988.6333f, 49.0605f, 3.60f);
				        me->SummonCreature(100067, 1755.1279f, -3991.0725f, 48.7217f, 3.60f);
				        me->SummonCreature(100068, 1728.6073f, -3999.0375f, 51.2886f, 0.46f);
				        me->SummonCreature(100069, 1726.7478f, -4001.9130f, 51.8847f, 0.46f);
				        me->SummonCreature(100070, 1729.9404f, -4002.9021f, 50.9726f, 0.46f);
				        break;
				case 4: textId = "5ème Assaut à l'entrée Ouest de Orgrimmar en Approche !";
				        me->SummonCreature(100065, 1753.5770f, -3987.4057f, 48.8544f, 3.60f);
				        me->SummonCreature(100066, 1756.5201f, -3988.6333f, 49.0605f, 3.60f);
				        me->SummonCreature(100067, 1755.1279f, -3991.0725f, 48.7217f, 3.60f);
				        me->SummonCreature(100068, 1728.6073f, -3999.0375f, 51.2886f, 0.46f);
				        me->SummonCreature(100069, 1726.7478f, -4001.9130f, 51.8847f, 0.46f);
				        me->SummonCreature(100070, 1729.9404f, -4002.9021f, 50.9726f, 0.46f);
				        break;
				case 5: textId = "6ème Assaut à l'entrée Ouest de Orgrimmar en Approche !";
				        me->SummonCreature(100065, 1753.5770f, -3987.4057f, 48.8544f, 3.60f);
				        me->SummonCreature(100066, 1756.5201f, -3988.6333f, 49.0605f, 3.60f);
				        me->SummonCreature(100067, 1755.1279f, -3991.0725f, 48.7217f, 3.60f);
				        me->SummonCreature(100068, 1728.6073f, -3999.0375f, 51.2886f, 0.46f);
				        me->SummonCreature(100069, 1726.7478f, -4001.9130f, 51.8847f, 0.46f);
				        me->SummonCreature(100070, 1729.9404f, -4002.9021f, 50.9726f, 0.46f);
				        break;
				case 6: textId = "7ème Assaut à l'entrée Ouest de Orgrimmar en Approche !";
				        me->SummonCreature(100065, 1753.5770f, -3987.4057f, 48.8544f, 3.60f);
				        me->SummonCreature(100066, 1756.5201f, -3988.6333f, 49.0605f, 3.60f);
				        me->SummonCreature(100067, 1755.1279f, -3991.0725f, 48.7217f, 3.60f);
				        me->SummonCreature(100068, 1728.6073f, -3999.0375f, 51.2886f, 0.46f);
				        me->SummonCreature(100069, 1726.7478f, -4001.9130f, 51.8847f, 0.46f);
				        me->SummonCreature(100070, 1729.9404f, -4002.9021f, 50.9726f, 0.46f);
				        break;
				case 7: textId = "8ème Assaut à l'entrée Ouest de Orgrimmar en Approche !";
				        me->SummonCreature(100065, 1753.5770f, -3987.4057f, 48.8544f, 3.60f);
				        me->SummonCreature(100066, 1756.5201f, -3988.6333f, 49.0605f, 3.60f);
				        me->SummonCreature(100067, 1755.1279f, -3991.0725f, 48.7217f, 3.60f);
				        me->SummonCreature(100068, 1728.6073f, -3999.0375f, 51.2886f, 0.46f);
				        me->SummonCreature(100069, 1726.7478f, -4001.9130f, 51.8847f, 0.46f);
				        me->SummonCreature(100070, 1729.9404f, -4002.9021f, 50.9726f, 0.46f);
				        break;
                case 8: textId = "Attention, Le Roi de la Gardes Foudrepique est en approche Préparez-vous !";
                        me->SummonCreature(100065, 1753.5770f, -3987.4057f, 48.8544f, 3.60f);
				        me->SummonCreature(100066, 1756.5201f, -3988.6333f, 49.0605f, 3.60f);
				        me->SummonCreature(100067, 1755.1279f, -3991.0725f, 48.7217f, 3.60f);
				        me->SummonCreature(100068, 1728.6073f, -3999.0375f, 51.2886f, 0.46f);
				        me->SummonCreature(100069, 1726.7478f, -4001.9130f, 51.8847f, 0.46f);
				        me->SummonCreature(100070, 1729.9404f, -4002.9021f, 50.9726f, 0.46f);
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
				sLog->outString("DEBUG: Voljinuistep est a %u", uiStep);
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
									me->MonsterYell("Sir! Mes sentinelles m'informent de l'arrivé des Gardes Foudrepique à entrée Ouest!", LANG_UNIVERSAL, 0);
									me->HandleEmoteCommand(378);
									uiStep=0;
									JumpToNextStep(6000);
									break;
							case 4:
									SetEscortPaused(true);
									me->MonsterYell("Il nous faut agir au plus vite Seigneur, et organiser la défense de Orgrimmar avec toutes les Forces disponibles!", LANG_UNIVERSAL, 0);
									me->HandleEmoteCommand(378);
									uiStep=0;
									JumpToNextStep(36000);
									break;
							case 6: 
									SetEscortPaused(true);
									me->MonsterYell("Que le Raid repoussant les forces de l'entrée Ouest me suive!", LANG_UNIVERSAL, 0);
									me->HandleEmoteCommand(378);
									JumpToNextStep(10000);
									break;
							case 10:
									SetEscortPaused(true);
									me->MonsterSay("Nous devons empécher cela à tout prix! Vite dépéchez-vous en Monture!", LANG_UNIVERSAL, 0);
									me->HandleEmoteCommand(378);
									JumpToNextStep(5000);
									break;
							case 56:
									SetEscortPaused(true);
									me->MonsterSay("Force de la Horde je vous en conjure, stopper cette invasion coute que coute!", LANG_UNIVERSAL, 0);
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
                        AddWaypoint(1, 1922.92f, -4169.02f, 41.1287f);
                        AddWaypoint(2, 1920.83f, -4139.05f, 40.5743f);
                        AddWaypoint(3, 1922.58f, -4132.8f, 42.4854f);
                        AddWaypoint(4, 1922.22f, -4131.66f, 42.9641f);
                        AddWaypoint(5, 1923.68f, -4128.39f, 43.1868f);
                        AddWaypoint(6, 1922.98f, -4132.24f, 42.5701f);
                        AddWaypoint(7, 1924.00f, -4177.52f, 41.1381f);
                        AddWaypoint(8, 1927.13f, -4233.31f, 42.212f);
                        AddWaypoint(9, 1929.12f, -4268.86f, 33.8055f);
                        AddWaypoint(10, 1928.74f, -4278.55f, 30.1563f);
                        AddWaypoint(11, 1923.00f, -4286.92f, 29.0653f);
                        AddWaypoint(12, 1908.47f, -4283.99f, 30.4107f);
                        AddWaypoint(13, 1896.56f, -4285.11f, 30.3345f);
                        AddWaypoint(14, 1887.49f, -4277.57f, 31.2886f);
                        AddWaypoint(15, 1873.69f, -4264.47f, 32.7104f);
                        AddWaypoint(16, 1861.71f, -4253.88f, 39.4624f);
                        AddWaypoint(17, 1853.44f, -4246.67f, 41.5356f);
                        AddWaypoint(18, 1843.07f, -4244.33f, 41.5404f);
                        AddWaypoint(19, 1830.95f, -4240.86f, 40.9158f);
                        AddWaypoint(20, 1807.87f, -4233.71f, 39.9508f);
                        AddWaypoint(21, 1791.92f, -4228.77f, 39.4907f);
                        AddWaypoint(22, 1771.12f, -4220.1f, 40.3989f);
                        AddWaypoint(23, 1755.89f, -4214.85f, 42.2805f);
                        AddWaypoint(24, 1744.94f, -4215.56f, 43.9258f);
                        AddWaypoint(25, 1732.81f, -4221.06f, 45.131f);
                        AddWaypoint(26, 1721.02f, -4227.24f, 46.9779f);
                        AddWaypoint(27, 1705.16f, -4233.48f, 49.6747f);
                        AddWaypoint(28, 1693.72f, -4241.16f, 51.2892f);
                        AddWaypoint(29, 1680.99f, -4248.99f, 51.2178f);
                        AddWaypoint(30, 1666.5f, -4256.06f, 51.5647f);
                        AddWaypoint(31, 1655.92f, -4263.72f, 50.7199f);
                        AddWaypoint(32, 1639.36f, -4272.36f, 47.8174f);
                        AddWaypoint(33, 1630.24f, -4269.85f, 47.0499f);
                        AddWaypoint(34, 1610.78f, -4250.17f, 45.8264f);
                        AddWaypoint(35, 1591.43f, -4227.24f, 44.5667f);
                        AddWaypoint(36, 1583.06f, -4201.97f, 41.8674f);
                        AddWaypoint(37, 1578.5f, -4188.23f, 39.9917f);
                        AddWaypoint(38, 1581.07f, -4169.96f, 37.3638f);
                        AddWaypoint(39, 1583.17f, -4154.93f, 34.6844f);
                        AddWaypoint(40, 1595.6f, -4136.41f, 32.0771f);
                        AddWaypoint(41, 1606.22f, -4120.58f, 29.6183f);
                        AddWaypoint(42, 1632.51f, -4110.23f, 29.9468f);
                        AddWaypoint(43, 1668.9f, -4095.9f, 35.3224f);
                        AddWaypoint(44, 1694.81f, -4087.99f, 38.4731f);
                        AddWaypoint(45, 1711.78f, -4082.8f, 40.9801f);
                        AddWaypoint(46, 1728.75f, -4071.02f, 44.1838f);
                        AddWaypoint(47, 1735.78f, -4062.00f, 44.4613f);
                        AddWaypoint(48, 1742.95f, -4035.15f, 44.968f);
                        AddWaypoint(49, 1747.14f, -4019.47f, 45.7663f);
                        AddWaypoint(50, 1733.48f, -3984.77f, 48.1909f);
                        AddWaypoint(51, 1726.64f, -3967.39f, 48.9136f);
                        AddWaypoint(52, 1712.98f, -3943.59f, 49.6634f);
                        AddWaypoint(53, 1701.83f, -3923.75f, 51.7773f);
                        AddWaypoint(54, 1687.67f, -3901.63f, 51.0631f);
                        AddWaypoint(55, 1695.54f, -3896.68f, 51.0629f);
                        AddWaypoint(56, 1694.55f, -3897.3f, 51.0629f);
                        break;
				default:
                        sLog->outError("Unexpected movement variation (%i) in npcvoljinAI::SetupMovement call!", variation);
                        return;
			}
			if(npcvoljinAI* pEscortAI = CAST_AI(npcvoljinAI, me->AI()))
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
						        
						        me->SummonCreature(100049, 1696.2297f, -3906.9270f, 51.7777f, 2.09f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000);
						        me->SummonCreature(100049, 1689.3312f, -3912.1164f, 51.7774f, 2.13f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000);
						       	me->MonsterSay("Voici deux véhicules encore en état de marchent pour vous aider dans votre Combat", LANG_UNIVERSAL, 0);
						       	JumpToNextStep(5000);
						       	break;
						case 8:
						        me->MonsterSay("Placez-y vos deux meilleurs Pilotes Héros. Ils vous seront d'un grands soutiens!", LANG_UNIVERSAL, 0);
						       	JumpToNextStep(5000);
						       	break;       		
                        case 9: 
                                me->MonsterYell("Nous ferons payer à la Gardes Foudrepique cette trahison!", LANG_UNIVERSAL, 0);
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
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Premier Assaut à la Porte Ouest d'Orgrimmar Stoppé !");                              
								me->MonsterTextEmote("Premier Assaut à la Porte Ouest d'Orgrimmar Stoppé !", 0, true);
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
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Deuxième Assaut à la Porte Ouest d'Orgrimmar Stoppé !");
								me->MonsterTextEmote("Deuxième Assaut à la Porte Ouest d'Orgrimmar Stoppé !", 0, true);
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
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Troisième Assaut à la Porte Ouest d'Orgrimmar Stoppé !");
								me->MonsterTextEmote("Troisième Assaut à la Porte Ouest d'Orgrimmar Stoppé !", 0, true);
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
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Quatrième Assaut à la Porte Ouest d'Orgrimmar Stoppé !");
								me->MonsterTextEmote("Quatrième Assaut à la Porte Ouest d'Orgrimmar Stoppé !", 0, true);
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
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Cinquième Assaut à la Porte Ouest d'Orgrimmar Stoppé !"); 
								me->MonsterTextEmote("Cinquième Assaut à la Porte Ouest d'Orgrimmar Stoppé !", 0, true);
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
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Sixième Assaut à la Porte Ouest d'Orgrimmar Stoppé !"); 
								me->MonsterTextEmote("Sixième Assaut à la Porte Ouest d'Orgrimmar Stoppé !", 0, true);
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
								me->MonsterYell("La Garde Foudrepique sont de redoutables Guerriers, mais vous Héros, vous l'êtes encore plus! Continuez !", LANG_UNIVERSAL, 0);
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
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Septième Assaut à la Porte Ouest d'Orgrimmar Stoppé !");
								me->MonsterTextEmote("Septième Assaut à la Porte Ouest d'Orgrimmar Stoppé !", 0, true);
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
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "Huitème Assaut à la Porte Ouest d'Orgrimmar Stoppé !"); 
								me->MonsterTextEmote("Huitème Assaut à la Porte Ouest d'Orgrimmar Stoppé !", 0, true);
								JumpToNextStep(4000);
								break;
                        case 33:
								if(!WaveDone)
								{
									SpawnWaveGroup(uiWave, uiWaveGUID);
									uiWave++;
									WavesCounter++;
								}
                                me->MonsterYell("Bravo Héros, vous venez de repousser les assauts de la Garde de Foudrepique! Préparez-vous ! Leur chef arrive!", LANG_UNIVERSAL, 0);
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
                                sWorld->SendWorldText(LANG_SYSTEMMESSAGE, "La Horde a Défendu Vaillament L'Entrée Ouest d'Orgrimmar!");
								me->MonsterTextEmote("La Horde a Défendu Vaillament L'Entrée Ouest d'Orgrimmar!", 0, true);
								JumpToNextStep(4000);
								break;
						case 36:
/*						          if (CAST_AI(npc_lycanthrope::npc_lycanthropeAI, me->AI())->Die1 == true && (CAST_AI(npc_bomberman::npc_bombermanAI, me->AI())->Die == true)   
						        { */
						          me->MonsterYell("Bravo Héros, vous venez à nouveau de prouver votre valeur! L'Assauts de la Garde de Foudrepique est stoppé. Orgrimmar vous sera indéfiniment redevable!", LANG_UNIVERSAL, 0);
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
        return new npcvoljinAI(creature);
    }
};





//Partie Event Aux Portes de Orgrimmar


/*******************************************/
/* Boss Shaman Entrée Porte Orgrimmar      */
/*******************************************/

struct TransformStruct
{
    std::string text;
    uint32 spell, unaura;
};

static TransformStruct Transform[4] =
{
    {YELL_TRANSFORM_BEAR, SPELL_SHAPE_OF_THE_BEAR, SPELL_WHIRLWIND},
    {YELL_TRANSFORM_EAGLE, SPELL_SHAPE_OF_THE_EAGLE, SPELL_SHAPE_OF_THE_BEAR},
    {YELL_TRANSFORM_LYNX, SPELL_SHAPE_OF_THE_LYNX, SPELL_SHAPE_OF_THE_EAGLE},
    {YELL_TRANSFORM_DRAGONHAWK, SPELL_SHAPE_OF_THE_DRAGONHAWK, SPELL_SHAPE_OF_THE_LYNX}
};


class boss_shaman : public CreatureScript
{
    public:

        boss_shaman() : CreatureScript("boss_shaman") { }

        struct boss_shamanAI : public npc_escortAI
        {
            boss_shamanAI(Creature *pCreature) : npc_escortAI(pCreature), Summons(me)
            {
               Reset();
            }

            uint64 SpiritGUID[4];
            uint64 ClawTargetGUID;
            uint64 TankGUID;

            uint32 Phase;
            uint32 health_20;

            uint32 Intro_Timer;
            uint32 Berserk_Timer;

            uint32 Whirlwind_Timer;
            uint32 Grievous_Throw_Timer;

            uint32 Creeping_Paralysis_Timer;
            uint32 Overpower_Timer;

            uint32 Claw_Rage_Timer;
            uint32 Lynx_Rush_Timer;
            uint32 Claw_Counter;
            uint32 Claw_Loop_Timer;

            uint32 Flame_Whirl_Timer;
            uint32 Flame_Breath_Timer;
            uint32 Pillar_Of_Fire_Timer;
            uint32 EclairTimer;

            SummonList Summons;

            void Reset()
            {

                Phase = 0;
                health_20 = me->CountPctFromMaxHealth(20);
                Intro_Timer = 37000;
                Berserk_Timer = 600000;
                Whirlwind_Timer = 7000;
                Grievous_Throw_Timer = 8000;
                Creeping_Paralysis_Timer = 7000;
                Overpower_Timer = 0;
                Claw_Rage_Timer = 5000;
                Lynx_Rush_Timer = 14000;
                Claw_Loop_Timer = 0;
                Claw_Counter = 0;
                Flame_Whirl_Timer = 5000;
                Flame_Breath_Timer = 6000;
                Pillar_Of_Fire_Timer = 7000;
                ClawTargetGUID = 0;
                TankGUID = 0;
                EclairTimer = 25000;
                SetupMovement(0);

                Summons.DespawnAll();

                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 33975);
            }

            void EnterCombat(Unit* /*who*/)
            {
                me->MonsterYell(YELL_INTRO, LANG_UNIVERSAL, 0);
                EnterPhase(0);
            }

            void KilledUnit(Unit* /*victim*/)
            {
                if (Intro_Timer)
                    return;

                switch (urand(0, 1))
                {
                    case 0:
                        me->MonsterYell(YELL_KILL_ONE, LANG_UNIVERSAL, 0);
                        break;
                    case 1:
                        me->MonsterYell(YELL_KILL_TWO, LANG_UNIVERSAL, 0);
                        break;
                }
            }

            void JustDied(Unit* /*Killer*/)
            {
                Summons.DespawnAll();
                me->MonsterYell(YELL_DEATH, LANG_UNIVERSAL, 0);
                Summons.DespawnEntry(CREATURE_COLUMN_OF_FIRE);

                if (Unit* Temp = Unit::GetUnit(*me, SpiritGUID[3]))
                    Temp->SetUInt32Value(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_DEAD);
            }

            void AttackStart(Unit* who)
            {
                if (Phase == 2)
                    AttackStartNoMove(who);
                else
                    ScriptedAI::AttackStart(who);
            }

            void DoMeleeAttackIfReady()
            {
                if (!me->IsNonMeleeSpellCasted(false))
                {
                    if (me->isAttackReady() && me->IsWithinMeleeRange(me->getVictim()))
                    {
                        if (Phase == 1 && !Overpower_Timer)
                        {
                            uint32 health = me->getVictim()->GetHealth();
                            me->AttackerStateUpdate(me->getVictim());
                            if (me->getVictim() && health == me->getVictim()->GetHealth())
                            {
                                DoCast(me->getVictim(), SPELL_OVERPOWER, false);
                                Overpower_Timer = 5000;
                            }
                        } else me->AttackerStateUpdate(me->getVictim());
                        me->resetAttackTimer();
                    }
                }
            }

            void JustSummoned(Creature* summon)
            {
                Summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                Summons.Despawn(summon);
            }

            void EnterPhase(uint32 NextPhase)
            {
                switch(NextPhase)
                {
                case 0:
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                     // a n'utiliser que si on veut le placer à un endroit fix pour son changement de forme
                    //DoTeleportTo(CENTER_X, CENTER_Y, CENTER_Z, 100);
                    DoResetThreat();
                    me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
                    me->RemoveAurasDueToSpell(Transform[Phase].unaura);
                    DoCast(me, Transform[Phase].spell);
                    me->MonsterYell(Transform[Phase].text.c_str(), LANG_UNIVERSAL, 0);

                    if (Phase > 0)
                    {
                        if (Unit* Temp = Unit::GetUnit(*me, SpiritGUID[Phase - 1]))
                            Temp->SetUInt32Value(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_DEAD);
                    }
                    if (Unit* Temp = Unit::GetUnit(*me, SpiritGUID[NextPhase - 1]))
                        Temp->CastSpell(me, SPELL_SIPHON_SOUL, false); 
                    if (NextPhase == 2)
                    {
                        me->GetMotionMaster()->Clear();
                        DoCast(me, SPELL_ENERGY_STORM, true); // aura ennemi
                        for (uint8 i = 0; i < 4; ++i)
                        {
                            Creature* Vortex = DoSpawnCreature(CREATURE_FEATHER_VORTEX, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                            if (Vortex)
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
                    else
                        me->AI()->AttackStart(me->getVictim());
                    if (NextPhase == 3)
                    {
                        me->RemoveAurasDueToSpell(SPELL_ENERGY_STORM);
                        Summons.DespawnEntry(CREATURE_FEATHER_VORTEX);
                        me->GetMotionMaster()->MoveChase(me->getVictim());
                    }
                    break;
                default:
                    break;
                }
                Phase = NextPhase;
            }
            
            void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 53:
					    SetEscortPaused(true);
				        break;
			}
		}
		
		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, 1221.62f, -4422.77f, 21.9209f);
                        AddWaypoint(2, 1270.31f, -4401.52f, 26.3033f);
                        AddWaypoint(3, 1314.47f, -4385.09f, 26.2462f);
                        AddWaypoint(4, 1354.59f, -4376.00f, 26.1279f);
                        AddWaypoint(5, 1391.1f, -4369.96f, 26.4445f);
                        AddWaypoint(6, 1427.95f, -4365.39f, 25.4634f);
                        AddWaypoint(7, 1436.8f, -4423.09f, 25.4634f);
                        AddWaypoint(8, 1459.23f, -4419.92f, 25.4634f);
                        AddWaypoint(9, 1491.15f, -4415.4f, 23.5301f);
                        AddWaypoint(10, 1522.6f, -4410.96f, 15.284f);
                        AddWaypoint(11, 1548.36f, -4420.03f, 9.85442f);
                        AddWaypoint(12, 1570.69f, -4428.89f, 6.76123f);
                        AddWaypoint(13, 1587.59f, -4436.54f, 6.39865f);
                        AddWaypoint(14, 1594.33f, -4439.59f, 6.10432f);
                        AddWaypoint(15, 1624.02f, -4428.67f, 12.692f);
                        AddWaypoint(16, 1643.96f, -4417.93f, 16.077f);
                        AddWaypoint(17, 1674.51f, -4399.74f, 19.6353f);
                        AddWaypoint(18, 1687.26f, -4386.08f, 24.2474f);
                        AddWaypoint(19, 1695.7f, -4377.03f, 24.7986f);
                        AddWaypoint(20, 1712.69f, -4379.26f, 29.4186f);
                        AddWaypoint(21, 1728.64f, -4401.44f, 34.7188f);
                        AddWaypoint(22, 1741.14f, -4418.82f, 36.3628f);
                        AddWaypoint(23, 1741.00f, -4441.28f, 37.7394f);
                        AddWaypoint(24, 1732.12f, -4461.09f, 35.5236f);
                        AddWaypoint(25, 1728.8f, -4479.2f, 32.2259f);
                        AddWaypoint(26, 1739.49f, -4499.83f, 28.2859f);
                        AddWaypoint(27, 1746.6f, -4511.67f, 27.1155f);
                        AddWaypoint(28, 1769.48f, -4527.51f, 25.1248f);
                        AddWaypoint(29, 1800.91f, -4530.41f, 21.3775f);
                        AddWaypoint(30, 1829.76f, -4532.82f, 20.5663f);
                        AddWaypoint(31, 1852.41f, -4534.71f, 23.6605f);
                        AddWaypoint(32, 1875.44f, -4528.23f, 26.0842f);
                        AddWaypoint(33, 1893.17f, -4519.57f, 27.5799f);
                        AddWaypoint(34, 1915.07f, -4494.98f, 24.0922f);
                        AddWaypoint(35, 1924.15f, -4477.02f, 23.2576f);
                        AddWaypoint(36, 1942.12f, -4441.48f, 23.2129f);
                        AddWaypoint(37, 1940.97f, -4424.16f, 22.6779f);
                        AddWaypoint(38, 1937.82f, -4376.4f, 20.7328f);
                        AddWaypoint(39, 1936.42f, -4355.16f, 19.8516f);
                        AddWaypoint(40, 1934.46f, -4325.38f, 20.265f);
                        AddWaypoint(41, 1932.59f, -4300.46f, 24.0374f);
                        AddWaypoint(42, 1931.05f, -4278.44f, 30.2063f);
                        AddWaypoint(43, 1929.64f, -4258.21f, 38.4621f);
                        AddWaypoint(44, 1928.68f, -4244.47f, 42.1286f);
                        AddWaypoint(45, 1927.07f, -4227.58f, 42.3219f);
                        AddWaypoint(46, 1926.18f, -4218.05f, 41.1723f);
                        AddWaypoint(47, 1925.46f, -4210.38f, 42.3221f);
                        AddWaypoint(48, 1924.33f, -4192.34f, 42.1687f);
                        AddWaypoint(49, 1923.24f, -4174.75f, 41.1292f);
                        AddWaypoint(50, 1921.88f, -4153.46f, 40.6131f);
                        AddWaypoint(51, 1920.87f, -4138.67f, 40.5688f);
                        AddWaypoint(52, 1920.48f, -4133.08f, 42.2368f);
                        AddWaypoint(53, 1920.37f, -4131.49f, 42.9961f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in boss_shamanAI::SetupMovement call!", variation); 
                         return;
			}
			
			
		    if(boss_shamanAI* pEscortAI = CAST_AI(boss_shamanAI, me->AI()))
			{
                    pEscortAI->Start(true, true);
                    me->setActive(true);
			}
		}

            void UpdateAI(const uint32 diff)
            {
                npc_escortAI::UpdateAI(diff);
                if (!TankGUID)
                {
                    if (!UpdateVictim())
                        return;

                    if (me->GetHealth() < health_20 * (4 - Phase))
                        EnterPhase(Phase + 1);
                }

                if (Berserk_Timer <= diff)
                {
                    DoCast(me, SPELL_BERSERK, true);
                    me->MonsterYell(YELL_BERSERK, LANG_UNIVERSAL, 0);
                    Berserk_Timer = 60000;
                } else Berserk_Timer -= diff;

                switch (Phase)
                {
                case 0:
                    if (Intro_Timer)
                    {
                        if (Intro_Timer <= diff)
                        {
                            me->MonsterYell(YELL_AGGRO, LANG_UNIVERSAL, 0);
                            Intro_Timer = 0;
                        } else Intro_Timer -= diff;
                    }

                    if (Whirlwind_Timer <= diff)
                    {
                        DoCast(me, SPELL_WHIRLWIND);
                        Whirlwind_Timer = 15000 + rand()%5000;
                    } else Whirlwind_Timer -= diff;

                    if (Grievous_Throw_Timer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            DoCast(target, SPELL_GRIEVOUS_THROW, false);
                        Grievous_Throw_Timer = 10000;
                    } else Grievous_Throw_Timer -= diff;
                    break;
                case 1:
                    if (Creeping_Paralysis_Timer <= diff)
                    {
                        DoCast(me, SPELL_CREEPING_PARALYSIS);
                        Creeping_Paralysis_Timer = 20000;
                    } else Creeping_Paralysis_Timer -= diff;

                    if (Overpower_Timer <= diff)
                    {
                        // implemented in DoMeleeAttackIfReady()
                        Overpower_Timer = 0;
                    } else Overpower_Timer -= diff;
                    break;
                case 2:
                       if(EclairTimer <= diff)
                        {
                           if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(pTarget, SPELL_ECLAIR);
                                EclairTimer = 25000;
                        } else EclairTimer -= diff;
                    return;
                case 3:
                    if (Claw_Rage_Timer <= diff)
                    {
                        if (!TankGUID)
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            {
                                TankGUID = me->getVictim()->GetGUID();
                                me->SetSpeed(MOVE_RUN, 5.0f);
                                AttackStart(target); // change de cible
                                Claw_Rage_Timer = 0;
                                Claw_Loop_Timer = 500;
                                Claw_Counter = 0;
                            }
                        }
                        else if (!Claw_Rage_Timer) 
                        {
                            if (Claw_Loop_Timer <= diff)
                            {
                                Unit* target = me->getVictim();
                                if (!target || !target->isTargetableForAttack()) target = Unit::GetUnit(*me, TankGUID);
                                if (!target || !target->isTargetableForAttack()) target = SelectTarget(SELECT_TARGET_RANDOM, 0);
                                if (target)
                                {
                                    AttackStart(target);
                                    if (me->IsWithinMeleeRange(target))
                                    {
                                        DoCast(target, SPELL_CLAW_RAGE_DAMAGE, true);
                                        ++Claw_Counter;
                                        if (Claw_Counter == 12)
                                        {
                                            Claw_Rage_Timer = 15000 + rand()%5000;
                                            me->SetSpeed(MOVE_RUN, 1.2f);
                                            AttackStart(Unit::GetUnit(*me, TankGUID));
                                            TankGUID = 0;
                                            return;
                                        }
                                        else
                                            Claw_Loop_Timer = 500;
                                    }
                                }
                                else
                                {
                                    EnterEvadeMode(); // if (target)
                                    return;
                                }
                            } else Claw_Loop_Timer -= diff;
                        } //if (TankGUID)
                    } else Claw_Rage_Timer -= diff;

                    if (Lynx_Rush_Timer <= diff)
                    {
                        if (!TankGUID)
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            {
                                TankGUID = me->getVictim()->GetGUID();
                                me->SetSpeed(MOVE_RUN, 5.0f);
                                AttackStart(target); // Change de cible
                                Lynx_Rush_Timer = 0;
                                Claw_Counter = 0;
                            }
                        }
                        else if (!Lynx_Rush_Timer)
                        {
                            Unit* target = me->getVictim();
                            if (!target || !target->isTargetableForAttack())
                            {
                                target = SelectTarget(SELECT_TARGET_RANDOM, 0);
                                AttackStart(target);
                            }
                            if (target)
                            {
                                if (me->IsWithinMeleeRange(target))
                                {
                                    DoCast(target, SPELL_LYNX_RUSH_DAMAGE, true);
                                    ++Claw_Counter;
                                    if (Claw_Counter == 9)
                                    {
                                        Lynx_Rush_Timer = 15000 + rand()%5000;
                                        me->SetSpeed(MOVE_RUN, 1.2f);
                                        AttackStart(Unit::GetUnit(*me, TankGUID));
                                        TankGUID = 0;
                                    }
                                    else
                                        AttackStart(SelectTarget(SELECT_TARGET_RANDOM, 0));
                                }
                            }
                            else
                            {
                                EnterEvadeMode(); // if (target)
                                return;
                            }
                        } //if (TankGUID)
                    } else Lynx_Rush_Timer -= diff;

                    break;
                case 4:
                    if (Flame_Whirl_Timer <= diff)
                    {
                        DoCast(me, SPELL_FLAME_WHIRL);
                        Flame_Whirl_Timer = 12000;
                    }Flame_Whirl_Timer -= diff;

                    if (Pillar_Of_Fire_Timer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(target, SPELL_SUMMON_PILLAR);
                        Pillar_Of_Fire_Timer = 10000;
                    } else Pillar_Of_Fire_Timer -= diff;

                    if (Flame_Breath_Timer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            me->SetInFront(target);
                        DoCast(me, SPELL_FLAME_BREATH);
                        Flame_Breath_Timer = 10000;
                    } else Flame_Breath_Timer -= diff;
                    break;
                default:
                    break;
                }

                if (!TankGUID)
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_shamanAI(creature);
        }
};

class mob_shaman_vortex : public CreatureScript
{
    public:

        mob_shaman_vortex()
            : CreatureScript("mob_shaman_vortex")
        {
        }

        struct mob_shaman_vortexAI : public ScriptedAI
        {
            mob_shaman_vortexAI(Creature* c) : ScriptedAI(c) {}

            void Reset() {}

            void EnterCombat(Unit* /*pTarget*/) {}

            void SpellHit(Unit* caster, const SpellEntry* spell)
            {
                if (spell->Id == SPELL_ZAP_INFORM)
                    DoCast(caster, SPELL_ZAP_DAMAGE, true);
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                //Si le vortex perds la cible, il check une autre cible
                if (me->IsWithinMeleeRange(me->getVictim()))
                    AttackStart(SelectTarget(SELECT_TARGET_RANDOM, 0));
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_shaman_vortexAI(creature);
        }
};









/*#####################################################
  #  Boss Général Garde Foudrepique Porte Orgrimmar   #
  #####################################################*/



class npcgeneralfourdrepique : public CreatureScript
{
public:

    npcgeneralfourdrepique() : CreatureScript("npcgeneralfourdrepique") { }

    struct npcgeneralfourdrepiqueAI : public npc_escortAI
    {
        npcgeneralfourdrepiqueAI(Creature *pCreature) : npc_escortAI(pCreature),summons(me)
		{
			Reset();
		}

    
        uint32 EnrageTimer;
		uint32 ChillingWaveTimer;
		uint32 WaterBoltVolleyTimer;
		uint32 WaterBlastTimer;
		uint32 CleaveTimer;
		uint32 GripTimer;
		uint32 SonicBurstTimer;
        SummonList summons;
        bool Enraged;
		bool Seegeant;
		bool health_75;
        bool health_40;
        bool health_25;

        void Reset()
        {
            summons.DespawnAll();
            me->RestoreDisplayId();
            EnrageTimer = 15000;
			ChillingWaveTimer = 30000;
			WaterBoltVolleyTimer = 25000;
			WaterBlastTimer = 10000;
			SonicBurstTimer = 53000;
			GripTimer = 50000;
			CleaveTimer = 8000;
            Enraged = false;
			Seegeant = false;
			health_75 = false;
            health_40 = false;
            health_25 = false;
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
            //SetEscortPaused(true);
            me->MonsterYell(SAY_GENERALFOUDREPIQUE_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_GENERALFOUDREPIQUE_SLAY, LANG_UNIVERSAL, 0);
            // En cas de mort d'un joueurs, spawn d'un Elementaire sur le corps
            if (victim->GetTypeId() == TYPEID_PLAYER)
                victim->SummonCreature(SUMMON_WATERELEMENTAL, victim->GetPositionX(), victim->GetPositionY(), victim->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
        }

        void JustDied(Unit* /*killer*/) 
        {
            me->MonsterYell(SAY_GENERALFOUDREPIQUE_DEATH, LANG_UNIVERSAL, 0);
            summons.DespawnAll();
            me->RestoreDisplayId();
        }

        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 70:
				        SetEscortPaused(true);
				        break;
				        
			}
		}
	
		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, 1545.52f, -3855.42f, 39.4522f);
                        AddWaypoint(2, 1560.25f, -3853.77f, 45.0848f);
                        AddWaypoint(3, 1577.09f, -3851.11f, 49.7605f);
                        AddWaypoint(4, 1597.96f, -3853.65f, 50.1645f);
                        AddWaypoint(5, 1612.32f, -3849.98f, 50.5267f);
                        AddWaypoint(6, 1625.4f, -3840.04f, 50.0284f);
                        AddWaypoint(7, 1633.12f, -3838.2f, 50.1711f);
                        AddWaypoint(8, 1643.5f, -3838.75f, 51.0747f);
                        AddWaypoint(9, 1647.55f, -3842.78f, 52.7441f);
                        AddWaypoint(10, 1653.57f, -3851.82f, 59.9768f);
                        AddWaypoint(11, 1662.6f, -3865.3f, 64.4771f);
                        AddWaypoint(12, 1672.16f, -3879.54f, 63.2845f);
                        AddWaypoint(13, 1676.89f, -3886.35f, 60.1993f);
                        AddWaypoint(14, 1685.88f, -3899.3f, 51.0708f);
                        AddWaypoint(15, 1695.84f, -3913.56f, 51.7773f);
                        AddWaypoint(16, 1703.56f, -3925.25f, 51.7773f);
                        AddWaypoint(17, 1709.67f, -3934.51f, 50.1544f);
                        AddWaypoint(18, 1725.52f, -3962.83f, 48.8759f);
                        AddWaypoint(19, 1736.38f, -3984.52f, 47.2716f);
                        AddWaypoint(20, 1738.96f, -3997.09f, 47.2195f);
                        AddWaypoint(21, 1744.59f, -4024.54f, 45.6987f);
                        AddWaypoint(22, 1740.68f, -4043.64f, 44.4844f);
                        AddWaypoint(23, 1737.47f, -4059.31f, 44.3825f);
                        AddWaypoint(24, 1727.31f, -4075.26f, 43.2196f);
                        AddWaypoint(25, 1708.32f, -4085.56f, 39.9161f);
                        AddWaypoint(26, 1685.14f, -4091.96f, 37.0901f);
                        AddWaypoint(27, 1656.55f, -4099.85f, 33.3951f);
                        AddWaypoint(28, 1632.1f, -4109.15f, 29.7992f);
                        AddWaypoint(29, 1602.63f, -4123.81f, 29.8898f);
                        AddWaypoint(30, 1589.84f, -4142.75f, 32.8694f);
                        AddWaypoint(31, 1577.11f, -4171.72f, 37.2662f);
                        AddWaypoint(32, 1579.53f, -4199.61f, 41.4442f);
                        AddWaypoint(33, 1589.97f, -4223.85f, 44.4204f);
                        AddWaypoint(34, 1605.23f, -4244.94f, 45.2712f);
                        AddWaypoint(35, 1620.69f, -4261.81f, 46.6678f);
                        AddWaypoint(36, 1632.14f, -4270.29f, 47.2125f);
                        AddWaypoint(37, 1640.85f, -4272.47f, 48.1457f);
                        AddWaypoint(38, 1654.74f, -4264.77f, 50.602f);
                        AddWaypoint(39, 1667.68f, -4255.38f, 51.4956f);
                        AddWaypoint(40, 1683.79f, -4247.68f, 51.2676f);
                        AddWaypoint(41, 1693.73f, -4238.3f, 50.8625f);
                        AddWaypoint(42, 1710.45f, -4231.4f, 48.7469f);
                        AddWaypoint(43, 1730.3f, -4222.56f, 45.4007f);
                        AddWaypoint(44, 1739.87f, -4216.73f, 44.3453f);
                        AddWaypoint(45, 1747.3f, -4214.68f, 43.4378f);
                        AddWaypoint(46, 1762.03f, -4217.22f, 41.7605f);
                        AddWaypoint(47, 1785.69f, -4225.57f, 39.3759f);
                        AddWaypoint(48, 1806.41f, -4233.16f, 39.8077f);
                        AddWaypoint(49, 1824.98f, -4239.19f, 40.8322f);
                        AddWaypoint(50, 1850.05f, -4245.24f, 41.6494f);
                        AddWaypoint(51, 1859.39f, -4251.23f, 40.2888f);
                        AddWaypoint(52, 1866.67f, -4258.31f, 36.1059f);
                        AddWaypoint(53, 1874.24f, -4265.62f, 32.4813f);
                        AddWaypoint(54, 1887.84f, -4280.86f, 30.8224f);
                        AddWaypoint(55, 1897.96f, -4284.08f, 30.3276f);
                        AddWaypoint(56, 1913.27f, -4285.86f, 30.1993f);
                        AddWaypoint(57, 1924.72f, -4285.74f, 29.1809f);
                        AddWaypoint(58, 1929.71f, -4279.28f, 30.0934f);
                        AddWaypoint(59, 1930.11f, -4272.29f, 31.9134f);
                        AddWaypoint(60, 1929.27f, -4259.35f, 38.1011f);
                        AddWaypoint(61, 1928.6f, -4249.1f, 41.1866f);
                        AddWaypoint(62, 1928.13f, -4241.87f, 42.1701f);
                        AddWaypoint(63, 1926.98f, -4227.64f, 42.3229f);
                        AddWaypoint(64, 1926.23f, -4218.46f, 41.1732f);
                        AddWaypoint(65, 1925.66f, -4211.36f, 42.3222f);
                        AddWaypoint(66, 1923.96f, -4184.43f, 42.1629f);
                        AddWaypoint(67, 1923.67f, -4178.94f, 41.1508f);
                        AddWaypoint(68, 1922.69f, -4160.52f, 40.6109f);
                        AddWaypoint(69, 1921.01f, -4140.74f, 40.6021f);
                        AddWaypoint(70, 1920.61f, -4136.1f, 40.5292f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npcgeneralfourdrepiqueAI::SetupMovement call!", variation); 
                         return;
			}
			
			if (npcgeneralfourdrepiqueAI* pEscortAI = CAST_AI(npcgeneralfourdrepiqueAI, me->AI()))
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
			/* ##########################################
			   # Debut Sorts pendant forme Foudrepique  #
			   ########################################## */
            // Enrage dure 8 secondes occurance entre 20et 25 secondes
            if (EnrageTimer <= uiDiff)
            {
                DoCast(me, ENRAGE);
                EnrageTimer = urand(20000,25000);
            } else EnrageTimer -= uiDiff;
			if (!Seegeant)
			{
				// Cleave les 2ennemis devant le général
				if (CleaveTimer <= uiDiff)
				{
					DoCastVictim(SPELL_CLEAVE);
					CleaveTimer = urand(10000,15000);
				} else CleaveTimer -= uiDiff;
				// Attire tous les joueurs sur le boss
				if (GripTimer <= uiDiff)
				{
					me->MonsterTextEmote(EMOTE_GENERALFOUDREPIQUE_GRIP, 0, true);
					DoCast(me, SPELL_GRIP);
					GripTimer = 40000;
				} else GripTimer -= uiDiff;
				// Lance une onde qui silence tous les joueurs à moins de 15 mètres pendant 5sec
				if (SonicBurstTimer <= uiDiff)
				{
					DoCast(me, SONIC_BURST);
					SonicBurstTimer = 40000;
				} else SonicBurstTimer -= uiDiff;	
				// Arrivé à 75% hp transformation en Géant des mers
				if (HealthBelowPct(75) && !health_75) //:Seegeant
				{
					me->SetDisplayId(170);
					me->MonsterTextEmote(SAY_GENERALFOUDREPIQUE_MORPH, 0, true);
					DoCast(me, ENRAGE);
					EnrageTimer = urand(25000,35000);
					health_75 = true;
					Seegeant = true;
				}
				// Arrivé à 25% hp transformation en Géant des mers
				if (HealthBelowPct(25) && !health_25)
				{
					me->SetDisplayId(170);
					me->MonsterTextEmote(SAY_GENERALFOUDREPIQUE_MORPH, 0, true);
					DoCast(me, ENRAGE);
					EnrageTimer = urand(25000,35000);
					health_25 = true;
					Seegeant = true;
				}
			}	
			/* ##########################################
			   #  Fin Sorts pendant forme Foudrepique   #
			   ########################################## */
			
			/* ##########################################
			   #Debut Sorts pendant forme Géant des mers#
			   ########################################## */
			if (Seegeant)
			{	
				// Lance un trait d'eau sur un ennemi random
				if (WaterBlastTimer <= uiDiff)
				{
					float PctHp;
					PctHp = me->GetHealthPct();
					if (((int)PctHp%2) == 1)
					{
						// Lance un trait d'eau qui inflige 2775 à 3225 de dégats et fait tomber l'ennemi a la renverse
						if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(pTarget, WATER_BLAST_NORMAL);
						WaterBlastTimer = urand(8000,12000);
					}
					else
					{
						// Lance un trait d'eau qui inflige 5550 à 6450 de dégats et fait tomber l'ennemi a la renverse
						if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(pTarget, WATER_BLAST_HERO);
						WaterBlastTimer = urand(12000,15000);
					}
				} else WaterBlastTimer -= uiDiff;
				// Lance un trait d'eau sur tous les ennemis à moins de 45mètres
				if (WaterBoltVolleyTimer <= uiDiff)
				{
					DoCast(me, WATER_BOLT_VOLLEY);
					WaterBoltVolleyTimer = urand(20000,25000);
				} else WaterBoltVolleyTimer -= uiDiff;
				// Inflige des dégats dans un cone de 65mètres devant le lanceur de sort
				if (ChillingWaveTimer <= uiDiff)
				{
					DoCast(me, CHILLING_WAVE);
					ChillingWaveTimer = urand(20000,30000);
				} else ChillingWaveTimer -= uiDiff;
				// Arrivé à 40 %hp redeviens normale
				if (HealthBelowPct(40) && !health_40)
				{
					me->MonsterTextEmote(SAY_GENERALFOUDREPIQUE_DEMORPH, 0, true);
					me->RestoreDisplayId();
					health_40 = true;
					Seegeant = false;
				}
				// S'enrage à 15% de vie (buff 50% vitesse d'attaque 50% dommage)
				if (HealthBelowPct(15) && !Enraged)
				{
					DoCast(me, FRENZY);
					Enraged = true;
				}
			}	
			/* ##########################################
			   # Fin Sorts pendant forme Géant des mers #
			   ########################################## */

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npcgeneralfourdrepiqueAI (pCreature);
    }
};



class spell_foudrepique_icy_grip : public SpellScriptLoader
{
public:
    spell_foudrepique_icy_grip() : SpellScriptLoader("spell_foudrepique_icy_grip") { }

    class spell_foudrepique_icy_grip_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_foudrepique_icy_grip_SpellScript);

        bool Validate(SpellEntry const* /*pSpell*/)
        {
            if(!sSpellStore.LookupEntry(SPELL_ICY_GRIP_JUMP))
                return false;
            return true;
        }

        void HandleScript(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if(Unit* pUnit = GetHitUnit())
                if(pUnit->isAlive())
                {
                    float x, y, z;
                    GetCaster()->GetPosition(x, y, z);
                    float speedXY = pUnit->GetExactDist2d(x, y) * 10.0f;
                    pUnit->GetMotionMaster()->MoveJump(x, y, z+1.0f, speedXY, 1.0f);
                }
            GetHitUnit()->CastSpell(GetCaster(), SPELL_ICY_GRIP_JUMP, true);
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_foudrepique_icy_grip_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_foudrepique_icy_grip_SpellScript();
    }
};



//Partie Event Aux Portes de Orgrimmar

/***************************************/
/* Archer Argent Wave Portes Orgrimmar */
/***************************************/


class npc_archer_argent : public CreatureScript
{
public:
    npc_archer_argent() : CreatureScript("npc_archer_argent") { }

    struct npc_archer_argentAI : public npc_escortAI
    {
        npc_archer_argentAI(Creature *pCreature) : npc_escortAI(pCreature)
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
            me->MonsterYell(SAY_ARCHERARGENT_AGGRO, LANG_UNIVERSAL, 0);
            //me->AddAura(BEWITCHING_AURA, me);
            me->AddAura(AURA_ROT, me);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_ARCHERARGENT_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_ARCHERARGENT_DEATH, LANG_UNIVERSAL, 0);
        }

        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 53:
					    SetEscortPaused(true);
				        break;
			}
		}
		
		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, 1221.62f, -4422.77f, 21.9209f);
                        AddWaypoint(2, 1270.31f, -4401.52f, 26.3033f);
                        AddWaypoint(3, 1314.47f, -4385.09f, 26.2462f);
                        AddWaypoint(4, 1354.59f, -4376.00f, 26.1279f);
                        AddWaypoint(5, 1391.1f, -4369.96f, 26.4445f);
                        AddWaypoint(6, 1427.95f, -4365.39f, 25.4634f);
                        AddWaypoint(7, 1436.8f, -4423.09f, 25.4634f);
                        AddWaypoint(8, 1459.23f, -4419.92f, 25.4634f);
                        AddWaypoint(9, 1491.15f, -4415.4f, 23.5301f);
                        AddWaypoint(10, 1522.6f, -4410.96f, 15.284f);
                        AddWaypoint(11, 1548.36f, -4420.03f, 9.85442f);
                        AddWaypoint(12, 1570.69f, -4428.89f, 6.76123f);
                        AddWaypoint(13, 1587.59f, -4436.54f, 6.39865f);
                        AddWaypoint(14, 1594.33f, -4439.59f, 6.10432f);
                        AddWaypoint(15, 1624.02f, -4428.67f, 12.692f);
                        AddWaypoint(16, 1643.96f, -4417.93f, 16.077f);
                        AddWaypoint(17, 1674.51f, -4399.74f, 19.6353f);
                        AddWaypoint(18, 1687.26f, -4386.08f, 24.2474f);
                        AddWaypoint(19, 1695.7f, -4377.03f, 24.7986f);
                        AddWaypoint(20, 1712.69f, -4379.26f, 29.4186f);
                        AddWaypoint(21, 1728.64f, -4401.44f, 34.7188f);
                        AddWaypoint(22, 1741.14f, -4418.82f, 36.3628f);
                        AddWaypoint(23, 1741.00f, -4441.28f, 37.7394f);
                        AddWaypoint(24, 1732.12f, -4461.09f, 35.5236f);
                        AddWaypoint(25, 1728.8f, -4479.2f, 32.2259f);
                        AddWaypoint(26, 1739.49f, -4499.83f, 28.2859f);
                        AddWaypoint(27, 1746.6f, -4511.67f, 27.1155f);
                        AddWaypoint(28, 1769.48f, -4527.51f, 25.1248f);
                        AddWaypoint(29, 1800.91f, -4530.41f, 21.3775f);
                        AddWaypoint(30, 1829.76f, -4532.82f, 20.5663f);
                        AddWaypoint(31, 1852.41f, -4534.71f, 23.6605f);
                        AddWaypoint(32, 1875.44f, -4528.23f, 26.0842f);
                        AddWaypoint(33, 1893.17f, -4519.57f, 27.5799f);
                        AddWaypoint(34, 1915.07f, -4494.98f, 24.0922f);
                        AddWaypoint(35, 1924.15f, -4477.02f, 23.2576f);
                        AddWaypoint(36, 1942.12f, -4441.48f, 23.2129f);
                        AddWaypoint(37, 1940.97f, -4424.16f, 22.6779f);
                        AddWaypoint(38, 1937.82f, -4376.4f, 20.7328f);
                        AddWaypoint(39, 1936.42f, -4355.16f, 19.8516f);
                        AddWaypoint(40, 1934.46f, -4325.38f, 20.265f);
                        AddWaypoint(41, 1932.59f, -4300.46f, 24.0374f);
                        AddWaypoint(42, 1931.05f, -4278.44f, 30.2063f);
                        AddWaypoint(43, 1929.64f, -4258.21f, 38.4621f);
                        AddWaypoint(44, 1928.68f, -4244.47f, 42.1286f);
                        AddWaypoint(45, 1927.07f, -4227.58f, 42.3219f);
                        AddWaypoint(46, 1926.18f, -4218.05f, 41.1723f);
                        AddWaypoint(47, 1925.46f, -4210.38f, 42.3221f);
                        AddWaypoint(48, 1924.33f, -4192.34f, 42.1687f);
                        AddWaypoint(49, 1923.24f, -4174.75f, 41.1292f);
                        AddWaypoint(50, 1921.88f, -4153.46f, 40.6131f);
                        AddWaypoint(51, 1920.87f, -4138.67f, 40.5688f);
                        AddWaypoint(52, 1920.48f, -4133.08f, 42.2368f);
                        AddWaypoint(53, 1920.37f, -4131.49f, 42.9961f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_archer_argentAI::SetupMovement call!", variation); 
                         return;
			}
			
			
		    if(npc_archer_argentAI* pEscortAI = CAST_AI(npc_archer_argentAI, me->AI()))
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
                me->MonsterYell(SAY_ARCHERARGENT_ICE, LANG_UNIVERSAL, 0);
                DoCast(me->getVictim(), ICE_TRAP);
                TrapTimer = 20000;
            } else TrapTimer -= uiDiff;

            // Se Heal toutes les 60 secondes
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
         return new npc_archer_argentAI (pCreature);
        }
};



/***************************************/
/* Mage Argent Wave portes Orgrimmar   */
/***************************************/


class npc_mage_argent : public CreatureScript
{
public:
    npc_mage_argent() : CreatureScript("npc_mage_argent") { }

    struct npc_mage_argentAI : public npc_escortAI
    {
        npc_mage_argentAI(Creature *pCreature) : npc_escortAI(pCreature)
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
            me->MonsterYell(SAY_MAGEARGENT_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_MAGEARGENT_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_MAGEARGENT_DEATH, LANG_UNIVERSAL, 0);
        }

		void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 53:
					    SetEscortPaused(true);
				        break;
			}
		}
		
		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, 1221.62f, -4422.77f, 21.9209f);
                        AddWaypoint(2, 1270.31f, -4401.52f, 26.3033f);
                        AddWaypoint(3, 1314.47f, -4385.09f, 26.2462f);
                        AddWaypoint(4, 1354.59f, -4376.00f, 26.1279f);
                        AddWaypoint(5, 1391.1f, -4369.96f, 26.4445f);
                        AddWaypoint(6, 1427.95f, -4365.39f, 25.4634f);
                        AddWaypoint(7, 1436.8f, -4423.09f, 25.4634f);
                        AddWaypoint(8, 1459.23f, -4419.92f, 25.4634f);
                        AddWaypoint(9, 1491.15f, -4415.4f, 23.5301f);
                        AddWaypoint(10, 1522.6f, -4410.96f, 15.284f);
                        AddWaypoint(11, 1548.36f, -4420.03f, 9.85442f);
                        AddWaypoint(12, 1570.69f, -4428.89f, 6.76123f);
                        AddWaypoint(13, 1587.59f, -4436.54f, 6.39865f);
                        AddWaypoint(14, 1594.33f, -4439.59f, 6.10432f);
                        AddWaypoint(15, 1624.02f, -4428.67f, 12.692f);
                        AddWaypoint(16, 1643.96f, -4417.93f, 16.077f);
                        AddWaypoint(17, 1674.51f, -4399.74f, 19.6353f);
                        AddWaypoint(18, 1687.26f, -4386.08f, 24.2474f);
                        AddWaypoint(19, 1695.7f, -4377.03f, 24.7986f);
                        AddWaypoint(20, 1712.69f, -4379.26f, 29.4186f);
                        AddWaypoint(21, 1728.64f, -4401.44f, 34.7188f);
                        AddWaypoint(22, 1741.14f, -4418.82f, 36.3628f);
                        AddWaypoint(23, 1741.00f, -4441.28f, 37.7394f);
                        AddWaypoint(24, 1732.12f, -4461.09f, 35.5236f);
                        AddWaypoint(25, 1728.8f, -4479.2f, 32.2259f);
                        AddWaypoint(26, 1739.49f, -4499.83f, 28.2859f);
                        AddWaypoint(27, 1746.6f, -4511.67f, 27.1155f);
                        AddWaypoint(28, 1769.48f, -4527.51f, 25.1248f);
                        AddWaypoint(29, 1800.91f, -4530.41f, 21.3775f);
                        AddWaypoint(30, 1829.76f, -4532.82f, 20.5663f);
                        AddWaypoint(31, 1852.41f, -4534.71f, 23.6605f);
                        AddWaypoint(32, 1875.44f, -4528.23f, 26.0842f);
                        AddWaypoint(33, 1893.17f, -4519.57f, 27.5799f);
                        AddWaypoint(34, 1915.07f, -4494.98f, 24.0922f);
                        AddWaypoint(35, 1924.15f, -4477.02f, 23.2576f);
                        AddWaypoint(36, 1942.12f, -4441.48f, 23.2129f);
                        AddWaypoint(37, 1940.97f, -4424.16f, 22.6779f);
                        AddWaypoint(38, 1937.82f, -4376.4f, 20.7328f);
                        AddWaypoint(39, 1936.42f, -4355.16f, 19.8516f);
                        AddWaypoint(40, 1934.46f, -4325.38f, 20.265f);
                        AddWaypoint(41, 1932.59f, -4300.46f, 24.0374f);
                        AddWaypoint(42, 1931.05f, -4278.44f, 30.2063f);
                        AddWaypoint(43, 1929.64f, -4258.21f, 38.4621f);
                        AddWaypoint(44, 1928.68f, -4244.47f, 42.1286f);
                        AddWaypoint(45, 1927.07f, -4227.58f, 42.3219f);
                        AddWaypoint(46, 1926.18f, -4218.05f, 41.1723f);
                        AddWaypoint(47, 1925.46f, -4210.38f, 42.3221f);
                        AddWaypoint(48, 1924.33f, -4192.34f, 42.1687f);
                        AddWaypoint(49, 1923.24f, -4174.75f, 41.1292f);
                        AddWaypoint(50, 1921.88f, -4153.46f, 40.6131f);
                        AddWaypoint(51, 1920.87f, -4138.67f, 40.5688f);
                        AddWaypoint(52, 1920.48f, -4133.08f, 42.2368f);
                        AddWaypoint(53, 1920.37f, -4131.49f, 42.9961f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_mage_argentAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_mage_argentAI* pEscortAI = CAST_AI(npc_mage_argentAI, me->AI()))
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
                me->MonsterYell(SAY_MAGEARGENT_SOUL, LANG_UNIVERSAL, 0);
                DoCastAOE(HARVEST_SOUL);
                HarvestTimer = 45000;
            } else HarvestTimer -= uiDiff;

            DoSpellAttackIfReady(SHADOW_BOLT);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mage_argentAI (pCreature);
    }
};


/*********************************************/
/* Infanterie Argent Wave portes Orgrimmar   */
/*********************************************/


class npc_infanterie_argent : public CreatureScript
{
public:
    npc_infanterie_argent() : CreatureScript("npc_infanterie_argent") { }

    struct npc_infanterie_argentAI : public npc_escortAI
    {
        npc_infanterie_argentAI(Creature *pCreature) : npc_escortAI(pCreature)
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
            me->MonsterYell(SAY_INFANTERIEARGENT_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_INFANTERIEARGENT_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_INFANTERIEARGENT_DEATH, LANG_UNIVERSAL, 0);
        }
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 53:
					    SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, 1221.62f, -4422.77f, 21.9209f);
                        AddWaypoint(2, 1270.31f, -4401.52f, 26.3033f);
                        AddWaypoint(3, 1314.47f, -4385.09f, 26.2462f);
                        AddWaypoint(4, 1354.59f, -4376.00f, 26.1279f);
                        AddWaypoint(5, 1391.1f, -4369.96f, 26.4445f);
                        AddWaypoint(6, 1427.95f, -4365.39f, 25.4634f);
                        AddWaypoint(7, 1436.8f, -4423.09f, 25.4634f);
                        AddWaypoint(8, 1459.23f, -4419.92f, 25.4634f);
                        AddWaypoint(9, 1491.15f, -4415.4f, 23.5301f);
                        AddWaypoint(10, 1522.6f, -4410.96f, 15.284f);
                        AddWaypoint(11, 1548.36f, -4420.03f, 9.85442f);
                        AddWaypoint(12, 1570.69f, -4428.89f, 6.76123f);
                        AddWaypoint(13, 1587.59f, -4436.54f, 6.39865f);
                        AddWaypoint(14, 1594.33f, -4439.59f, 6.10432f);
                        AddWaypoint(15, 1624.02f, -4428.67f, 12.692f);
                        AddWaypoint(16, 1643.96f, -4417.93f, 16.077f);
                        AddWaypoint(17, 1674.51f, -4399.74f, 19.6353f);
                        AddWaypoint(18, 1687.26f, -4386.08f, 24.2474f);
                        AddWaypoint(19, 1695.7f, -4377.03f, 24.7986f);
                        AddWaypoint(20, 1712.69f, -4379.26f, 29.4186f);
                        AddWaypoint(21, 1728.64f, -4401.44f, 34.7188f);
                        AddWaypoint(22, 1741.14f, -4418.82f, 36.3628f);
                        AddWaypoint(23, 1741.00f, -4441.28f, 37.7394f);
                        AddWaypoint(24, 1732.12f, -4461.09f, 35.5236f);
                        AddWaypoint(25, 1728.8f, -4479.2f, 32.2259f);
                        AddWaypoint(26, 1739.49f, -4499.83f, 28.2859f);
                        AddWaypoint(27, 1746.6f, -4511.67f, 27.1155f);
                        AddWaypoint(28, 1769.48f, -4527.51f, 25.1248f);
                        AddWaypoint(29, 1800.91f, -4530.41f, 21.3775f);
                        AddWaypoint(30, 1829.76f, -4532.82f, 20.5663f);
                        AddWaypoint(31, 1852.41f, -4534.71f, 23.6605f);
                        AddWaypoint(32, 1875.44f, -4528.23f, 26.0842f);
                        AddWaypoint(33, 1893.17f, -4519.57f, 27.5799f);
                        AddWaypoint(34, 1915.07f, -4494.98f, 24.0922f);
                        AddWaypoint(35, 1924.15f, -4477.02f, 23.2576f);
                        AddWaypoint(36, 1942.12f, -4441.48f, 23.2129f);
                        AddWaypoint(37, 1940.97f, -4424.16f, 22.6779f);
                        AddWaypoint(38, 1937.82f, -4376.4f, 20.7328f);
                        AddWaypoint(39, 1936.42f, -4355.16f, 19.8516f);
                        AddWaypoint(40, 1934.46f, -4325.38f, 20.265f);
                        AddWaypoint(41, 1932.59f, -4300.46f, 24.0374f);
                        AddWaypoint(42, 1931.05f, -4278.44f, 30.2063f);
                        AddWaypoint(43, 1929.64f, -4258.21f, 38.4621f);
                        AddWaypoint(44, 1928.68f, -4244.47f, 42.1286f);
                        AddWaypoint(45, 1927.07f, -4227.58f, 42.3219f);
                        AddWaypoint(46, 1926.18f, -4218.05f, 41.1723f);
                        AddWaypoint(47, 1925.46f, -4210.38f, 42.3221f);
                        AddWaypoint(48, 1924.33f, -4192.34f, 42.1687f);
                        AddWaypoint(49, 1923.24f, -4174.75f, 41.1292f);
                        AddWaypoint(50, 1921.88f, -4153.46f, 40.6131f);
                        AddWaypoint(51, 1920.87f, -4138.67f, 40.5688f);
                        AddWaypoint(52, 1920.48f, -4133.08f, 42.2368f);
                        AddWaypoint(53, 1920.37f, -4131.49f, 42.9961f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_infanterie_argentAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_infanterie_argentAI* pEscortAI = CAST_AI(npc_infanterie_argentAI, me->AI()))
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
        return new npc_infanterie_argentAI (pCreature);
    }
};








/*********************************************/
/* Orc de Bataille H2 Wave Entrée Orgrimmar  */
/*********************************************/


class npc_orc_bataille : public CreatureScript
{

public:
    npc_orc_bataille() : CreatureScript("npc_orc_bataille") { }

    struct npc_orc_batailleAI : public npc_escortAI
    {
        npc_orc_batailleAI(Creature *pCreature) : npc_escortAI(pCreature)
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
			    case 3:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, 1424.79f, -4369.48f, 25.4634f);
                        AddWaypoint(2, 1398.2f, -4373.26f, 25.4634f);
                        AddWaypoint(3, 1371.76f, -4378.33f, 26.0585f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_orc_batailleAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_orc_batailleAI* pEscortAI = CAST_AI(npc_orc_batailleAI, me->AI()))
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
        return new npc_orc_batailleAI (pCreature);
    }
};




/*********************************************/
/* Orc de Bataille1 H2 Wave Entrée Orgrimmar */
/*********************************************/


class npc_orc_bataille1 : public CreatureScript
{

public:
    npc_orc_bataille1() : CreatureScript("npc_orc_bataille1") { }

    struct npc_orc_bataille1AI : public npc_escortAI
    {
        npc_orc_bataille1AI(Creature *pCreature) : npc_escortAI(pCreature)
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
				case 3:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, 1426.5f, -4368.83f, 25.4625f);
                        AddWaypoint(2, 1396.02f, -4371.97f, 25.4625f);
                        AddWaypoint(3, 1373.73f, -4375.51f, 26.0448f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_orc_bataille1AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_orc_bataille1AI* pEscortAI = CAST_AI(npc_orc_bataille1AI, me->AI()))
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
        return new npc_orc_bataille1AI (pCreature);
    }
};




/*********************************************/
/* Orc de Bataille2 H2 Wave Entrée Orgrimmar */
/*********************************************/


class npc_orc_bataille2 : public CreatureScript
{

public:
    npc_orc_bataille2() : CreatureScript("npc_orc_bataille2") { }

    struct npc_orc_bataille2AI : public npc_escortAI
    {
        npc_orc_bataille2AI(Creature *pCreature) : npc_escortAI(pCreature)
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
				case 3:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, 1428.45f, -4366.02f, 25.4625f);
                        AddWaypoint(2, 1397.36f, -4369.63f, 25.4625f);
                        AddWaypoint(3, 1370.62f, -4373.39f, 26.0536f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_orc_bataille2AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_orc_bataille2AI* pEscortAI = CAST_AI(npc_orc_bataille2AI, me->AI()))
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
        return new npc_orc_bataille2AI (pCreature);
    }
};









/*********************************************/
/* Orc de Bataille3 H2 Wave Entrée Orgimmar  */
/*********************************************/


class npc_orc_bataille3 : public CreatureScript
{

public:
    npc_orc_bataille3() : CreatureScript("npc_orc_bataille3") { }

    struct npc_orc_bataille3AI : public npc_escortAI
    {
        npc_orc_bataille3AI(Creature *pCreature) : npc_escortAI(pCreature)
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
				case 3:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
						AddWaypoint(1, 1429.85f, -4362.61f, 25.4625f);
                        AddWaypoint(2, 1396.36f, -4366.83f, 25.4625f);
                        AddWaypoint(3, 1370.15f, -4370.2f, 26.0556f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_orc_bataille3AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_orc_bataille3AI* pEscortAI = CAST_AI(npc_orc_bataille3AI, me->AI()))
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
        return new npc_orc_bataille3AI (pCreature);
    }
};





/*********************************************/
/* Orc de Bataille4 H2 Wave Entrée Orgimmar  */
/*********************************************/


class npc_orc_bataille4 : public CreatureScript
{

public:
    npc_orc_bataille4() : CreatureScript("npc_orc_bataille4") { }

    struct npc_orc_bataille4AI : public npc_escortAI
    {
        npc_orc_bataille4AI(Creature *pCreature) : npc_escortAI(pCreature)
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
				case 3:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
					   AddWaypoint(1, 1432.12f, -4360.27f, 25.4628f);
                       AddWaypoint(2, 1396.02f, -4366.02f, 25.4628f);
                       AddWaypoint(3, 1372.98f, -4370.12f, 26.0388f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_orc_bataille4AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_orc_bataille4AI* pEscortAI = CAST_AI(npc_orc_bataille4AI, me->AI()))
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
        return new npc_orc_bataille4AI (pCreature);
    }
};






/*********************************************/
/* Orc de Bataille5 H2 Wave Entrée Orgimmar  */
/*********************************************/


class npc_orc_bataille5 : public CreatureScript
{

public:
    npc_orc_bataille5() : CreatureScript("npc_orc_bataille5") { }

    struct npc_orc_bataille5AI : public npc_escortAI
    {
        npc_orc_bataille5AI(Creature *pCreature) : npc_escortAI(pCreature)
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
				case 3:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
					   AddWaypoint(1, 1434.76f, -4360.51f, 25.4624f);
                       AddWaypoint(2, 1395.91f, -4364.25f, 25.4624f);
                       AddWaypoint(3, 1369.64f, -4368.31f, 26.0521f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_orc_bataille5AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_orc_bataille5AI* pEscortAI = CAST_AI(npc_orc_bataille5AI, me->AI()))
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
        return new npc_orc_bataille5AI (pCreature);
    }
};









































/******************************************************************************************************************************************************************************************************
/*****************************************************************************************************************************************************************************************************/



//Partie Event Porte Ouest de Orgrimmar


/**********************************************************/
/* Infanterie Foudrepique Wave Porte Ouest de Orgrimmar   */
/**********************************************************/


class npc_infanterie_foudrepique : public CreatureScript
{

public:
    npc_infanterie_foudrepique() : CreatureScript("npc_infanterie_foudrepique") { }

    struct npc_infanterie_foudrepiqueAI : public npc_escortAI
    {
        npc_infanterie_foudrepiqueAI(Creature *pCreature) : npc_escortAI(pCreature)
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
            me->MonsterYell(SAY_INFANTERIEFOUDREPIQUE_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_INFANTERIEFOUDREPIQUE_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_INFANTERIEFOUDREPIQUE_DEATH, LANG_UNIVERSAL, 0);
            
        }
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 70:
					    SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
					   AddWaypoint(1, 1545.52f, -3855.42f, 39.4522f);
                       AddWaypoint(2, 1560.25f, -3853.77f, 45.0848f);
                       AddWaypoint(3, 1577.09f, -3851.11f, 49.7605f);
                       AddWaypoint(4, 1597.96f, -3853.65f, 50.1645f);
                       AddWaypoint(5, 1612.32f, -3849.98f, 50.5267f);
                       AddWaypoint(6, 1625.4f, -3840.04f, 50.0284f);
                       AddWaypoint(7, 1633.12f, -3838.2f, 50.1711f);
                       AddWaypoint(8, 1643.5f, -3838.75f, 51.0747f);
                       AddWaypoint(9, 1647.55f, -3842.78f, 52.7441f);
                       AddWaypoint(10, 1653.57f, -3851.82f, 59.9768f);
                       AddWaypoint(11, 1662.6f, -3865.3f, 64.4771f);
                       AddWaypoint(12, 1672.16f, -3879.54f, 63.2845f);
                       AddWaypoint(13, 1676.89f, -3886.35f, 60.1993f);
                       AddWaypoint(14, 1685.88f, -3899.3f, 51.0708f);
                       AddWaypoint(15, 1695.84f, -3913.56f, 51.7773f);
                       AddWaypoint(16, 1703.56f, -3925.25f, 51.7773f);
                       AddWaypoint(17, 1709.67f, -3934.51f, 50.1544f);
                       AddWaypoint(18, 1725.52f, -3962.83f, 48.8759f);
                       AddWaypoint(19, 1736.38f, -3984.52f, 47.2716f);
                       AddWaypoint(20, 1738.96f, -3997.09f, 47.2195f);
                       AddWaypoint(21, 1744.59f, -4024.54f, 45.6987f);
                       AddWaypoint(22, 1740.68f, -4043.64f, 44.4844f);
                       AddWaypoint(23, 1737.47f, -4059.31f, 44.3825f);
                       AddWaypoint(24, 1727.31f, -4075.26f, 43.2196f);
                       AddWaypoint(25, 1708.32f, -4085.56f, 39.9161f);
                       AddWaypoint(26, 1685.14f, -4091.96f, 37.0901f);
                       AddWaypoint(27, 1656.55f, -4099.85f, 33.3951f);
                       AddWaypoint(28, 1632.1f, -4109.15f, 29.7992f);
                       AddWaypoint(29, 1602.63f, -4123.81f, 29.8898f);
                       AddWaypoint(30, 1589.84f, -4142.75f, 32.8694f);
                       AddWaypoint(31, 1577.11f, -4171.72f, 37.2662f);
                       AddWaypoint(32, 1579.53f, -4199.61f, 41.4442f);
                       AddWaypoint(33, 1589.97f, -4223.85f, 44.4204f);
                       AddWaypoint(34, 1605.23f, -4244.94f, 45.2712f);
                       AddWaypoint(35, 1620.69f, -4261.81f, 46.6678f);
                       AddWaypoint(36, 1632.14f, -4270.29f, 47.2125f);
                       AddWaypoint(37, 1640.85f, -4272.47f, 48.1457f);
                       AddWaypoint(38, 1654.74f, -4264.77f, 50.602f);
                       AddWaypoint(39, 1667.68f, -4255.38f, 51.4956f);
                       AddWaypoint(40, 1683.79f, -4247.68f, 51.2676f);
                       AddWaypoint(41, 1693.73f, -4238.3f, 50.8625f);
                       AddWaypoint(42, 1710.45f, -4231.4f, 48.7469f);
                       AddWaypoint(43, 1730.3f, -4222.56f, 45.4007f);
                       AddWaypoint(44, 1739.87f, -4216.73f, 44.3453f);
                       AddWaypoint(45, 1747.3f, -4214.68f, 43.4378f);
                       AddWaypoint(46, 1762.03f, -4217.22f, 41.7605f);
                       AddWaypoint(47, 1785.69f, -4225.57f, 39.3759f);
                       AddWaypoint(48, 1806.41f, -4233.16f, 39.8077f);
                       AddWaypoint(49, 1824.98f, -4239.19f, 40.8322f);
                       AddWaypoint(50, 1850.05f, -4245.24f, 41.6494f);
                       AddWaypoint(51, 1859.39f, -4251.23f, 40.2888f);
                       AddWaypoint(52, 1866.67f, -4258.31f, 36.1059f);
                       AddWaypoint(53, 1874.24f, -4265.62f, 32.4813f);
                       AddWaypoint(54, 1887.84f, -4280.86f, 30.8224f);
                       AddWaypoint(55, 1897.96f, -4284.08f, 30.3276f);
                       AddWaypoint(56, 1913.27f, -4285.86f, 30.1993f);
                       AddWaypoint(57, 1924.72f, -4285.74f, 29.1809f);
                       AddWaypoint(58, 1929.71f, -4279.28f, 30.0934f);
                       AddWaypoint(59, 1930.11f, -4272.29f, 31.9134f);
                       AddWaypoint(60, 1929.27f, -4259.35f, 38.1011f);
                       AddWaypoint(61, 1928.6f, -4249.1f, 41.1866f);
                       AddWaypoint(62, 1928.13f, -4241.87f, 42.1701f);
                       AddWaypoint(63, 1926.98f, -4227.64f, 42.3229f);
                       AddWaypoint(64, 1926.23f, -4218.46f, 41.1732f);
                       AddWaypoint(65, 1925.66f, -4211.36f, 42.3222f);
                       AddWaypoint(66, 1923.96f, -4184.43f, 42.1629f);
                       AddWaypoint(67, 1923.67f, -4178.94f, 41.1508f);
                       AddWaypoint(68, 1922.69f, -4160.52f, 40.6109f);
                       AddWaypoint(69, 1921.01f, -4140.74f, 40.6021f);
                       AddWaypoint(70, 1920.61f, -4136.1f, 40.5292f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_infanterie_foudrepiqueAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_infanterie_foudrepiqueAI* pEscortAI = CAST_AI(npc_infanterie_foudrepiqueAI, me->AI()))
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
        return new npc_infanterie_foudrepiqueAI (pCreature);
    }
};



/********************************************************/
/* Mage Foudrepique WavePorte Ouest de Orgrimmar        */
/********************************************************/


class npc_mage_foudrepique : public CreatureScript
{

public:
    npc_mage_foudrepique() : CreatureScript("npc_mage_foudrepique") { }

    struct npc_mage_foudrepiqueAI : public npc_escortAI
    {
        npc_mage_foudrepiqueAI(Creature *pCreature) : npc_escortAI(pCreature)
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
            me->MonsterYell(SAY_MAGEFOUDREPIQUE_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_MAGEFOUDREPIQUE_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_MAGEFOUDREPIQUE_DEATH, LANG_UNIVERSAL, 0);
        }
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 70:
					    SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
					   AddWaypoint(1, 1545.52f, -3855.42f, 39.4522f);
                       AddWaypoint(2, 1560.25f, -3853.77f, 45.0848f);
                       AddWaypoint(3, 1577.09f, -3851.11f, 49.7605f);
                       AddWaypoint(4, 1597.96f, -3853.65f, 50.1645f);
                       AddWaypoint(5, 1612.32f, -3849.98f, 50.5267f);
                       AddWaypoint(6, 1625.4f, -3840.04f, 50.0284f);
                       AddWaypoint(7, 1633.12f, -3838.2f, 50.1711f);
                       AddWaypoint(8, 1643.5f, -3838.75f, 51.0747f);
                       AddWaypoint(9, 1647.55f, -3842.78f, 52.7441f);
                       AddWaypoint(10, 1653.57f, -3851.82f, 59.9768f);
                       AddWaypoint(11, 1662.6f, -3865.3f, 64.4771f);
                       AddWaypoint(12, 1672.16f, -3879.54f, 63.2845f);
                       AddWaypoint(13, 1676.89f, -3886.35f, 60.1993f);
                       AddWaypoint(14, 1685.88f, -3899.3f, 51.0708f);
                       AddWaypoint(15, 1695.84f, -3913.56f, 51.7773f);
                       AddWaypoint(16, 1703.56f, -3925.25f, 51.7773f);
                       AddWaypoint(17, 1709.67f, -3934.51f, 50.1544f);
                       AddWaypoint(18, 1725.52f, -3962.83f, 48.8759f);
                       AddWaypoint(19, 1736.38f, -3984.52f, 47.2716f);
                       AddWaypoint(20, 1738.96f, -3997.09f, 47.2195f);
                       AddWaypoint(21, 1744.59f, -4024.54f, 45.6987f);
                       AddWaypoint(22, 1740.68f, -4043.64f, 44.4844f);
                       AddWaypoint(23, 1737.47f, -4059.31f, 44.3825f);
                       AddWaypoint(24, 1727.31f, -4075.26f, 43.2196f);
                       AddWaypoint(25, 1708.32f, -4085.56f, 39.9161f);
                       AddWaypoint(26, 1685.14f, -4091.96f, 37.0901f);
                       AddWaypoint(27, 1656.55f, -4099.85f, 33.3951f);
                       AddWaypoint(28, 1632.1f, -4109.15f, 29.7992f);
                       AddWaypoint(29, 1602.63f, -4123.81f, 29.8898f);
                       AddWaypoint(30, 1589.84f, -4142.75f, 32.8694f);
                       AddWaypoint(31, 1577.11f, -4171.72f, 37.2662f);
                       AddWaypoint(32, 1579.53f, -4199.61f, 41.4442f);
                       AddWaypoint(33, 1589.97f, -4223.85f, 44.4204f);
                       AddWaypoint(34, 1605.23f, -4244.94f, 45.2712f);
                       AddWaypoint(35, 1620.69f, -4261.81f, 46.6678f);
                       AddWaypoint(36, 1632.14f, -4270.29f, 47.2125f);
                       AddWaypoint(37, 1640.85f, -4272.47f, 48.1457f);
                       AddWaypoint(38, 1654.74f, -4264.77f, 50.602f);
                       AddWaypoint(39, 1667.68f, -4255.38f, 51.4956f);
                       AddWaypoint(40, 1683.79f, -4247.68f, 51.2676f);
                       AddWaypoint(41, 1693.73f, -4238.3f, 50.8625f);
                       AddWaypoint(42, 1710.45f, -4231.4f, 48.7469f);
                       AddWaypoint(43, 1730.3f, -4222.56f, 45.4007f);
                       AddWaypoint(44, 1739.87f, -4216.73f, 44.3453f);
                       AddWaypoint(45, 1747.3f, -4214.68f, 43.4378f);
                       AddWaypoint(46, 1762.03f, -4217.22f, 41.7605f);
                       AddWaypoint(47, 1785.69f, -4225.57f, 39.3759f);
                       AddWaypoint(48, 1806.41f, -4233.16f, 39.8077f);
                       AddWaypoint(49, 1824.98f, -4239.19f, 40.8322f);
                       AddWaypoint(50, 1850.05f, -4245.24f, 41.6494f);
                       AddWaypoint(51, 1859.39f, -4251.23f, 40.2888f);
                       AddWaypoint(52, 1866.67f, -4258.31f, 36.1059f);
                       AddWaypoint(53, 1874.24f, -4265.62f, 32.4813f);
                       AddWaypoint(54, 1887.84f, -4280.86f, 30.8224f);
                       AddWaypoint(55, 1897.96f, -4284.08f, 30.3276f);
                       AddWaypoint(56, 1913.27f, -4285.86f, 30.1993f);
                       AddWaypoint(57, 1924.72f, -4285.74f, 29.1809f);
                       AddWaypoint(58, 1929.71f, -4279.28f, 30.0934f);
                       AddWaypoint(59, 1930.11f, -4272.29f, 31.9134f);
                       AddWaypoint(60, 1929.27f, -4259.35f, 38.1011f);
                       AddWaypoint(61, 1928.6f, -4249.1f, 41.1866f);
                       AddWaypoint(62, 1928.13f, -4241.87f, 42.1701f);
                       AddWaypoint(63, 1926.98f, -4227.64f, 42.3229f);
                       AddWaypoint(64, 1926.23f, -4218.46f, 41.1732f);
                       AddWaypoint(65, 1925.66f, -4211.36f, 42.3222f);
                       AddWaypoint(66, 1923.96f, -4184.43f, 42.1629f);
                       AddWaypoint(67, 1923.67f, -4178.94f, 41.1508f);
                       AddWaypoint(68, 1922.69f, -4160.52f, 40.6109f);
                       AddWaypoint(69, 1921.01f, -4140.74f, 40.6021f);
                       AddWaypoint(70, 1920.61f, -4136.1f, 40.5292f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_mage_foudrepiqueAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_mage_foudrepiqueAI* pEscortAI = CAST_AI(npc_mage_foudrepiqueAI, me->AI()))
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
                me->MonsterYell(SAY_MAGEFOUDREPIQUE_BLOOD, LANG_UNIVERSAL, 0);
                me->SummonCreature(SUMMON_AIR_ELEMENTAL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                Haste = true;
            }

            DoSpellAttackIfReady(LIGHTNING_BOLT);
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mage_foudrepiqueAI (pCreature);
    }
};





/*******************************************************/
/* Ingenieur Foudrepique WavePort Ouest de Orgrimmar   */
/*******************************************************/



// script de l'image du joueur

class npc_stolen_soul2 : public CreatureScript
{

public:
    npc_stolen_soul2() : CreatureScript("npc_stolen_soul2") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_stolen_soul2AI (creature);
    }

    struct npc_stolen_soul2AI : public ScriptedAI
    {
        npc_stolen_soul2AI(Creature* c) : ScriptedAI(c) { }

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





class npc_ingenieur_foudrepique : public CreatureScript
{

public:
    npc_ingenieur_foudrepique() : CreatureScript("npc_ingenieur_foudrepique") { }

    struct npc_ingenieur_foudrepiqueAI : public npc_escortAI
    {
        npc_ingenieur_foudrepiqueAI(Creature *pCreature) : npc_escortAI(pCreature)
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
            me->MonsterYell(SAY_INGENIEURFOUDREPIQUE_AGGRO, LANG_UNIVERSAL, 0);
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
                CAST_AI(npc_stolen_soul2::npc_stolen_soul2AI, summoned->AI())->SetMyClass(soulclass);
                 summoned->AI()->AttackStart(target);
                }
            }
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_INGENIEURFOUDREPIQUE_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_INGENIEURFOUDREPIQUE_DEATH, LANG_UNIVERSAL, 0);
        }
        void WaypointReached(uint32 uiPointId)
		{
			switch(uiPointId)
			{
				case 70:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
			           AddWaypoint(1, 1545.52f, -3855.42f, 39.4522f);
                       AddWaypoint(2, 1560.25f, -3853.77f, 45.0848f);
                       AddWaypoint(3, 1577.09f, -3851.11f, 49.7605f);
                       AddWaypoint(4, 1597.96f, -3853.65f, 50.1645f);
                       AddWaypoint(5, 1612.32f, -3849.98f, 50.5267f);
                       AddWaypoint(6, 1625.4f, -3840.04f, 50.0284f);
                       AddWaypoint(7, 1633.12f, -3838.2f, 50.1711f);
                       AddWaypoint(8, 1643.5f, -3838.75f, 51.0747f);
                       AddWaypoint(9, 1647.55f, -3842.78f, 52.7441f);
                       AddWaypoint(10, 1653.57f, -3851.82f, 59.9768f);
                       AddWaypoint(11, 1662.6f, -3865.3f, 64.4771f);
                       AddWaypoint(12, 1672.16f, -3879.54f, 63.2845f);
                       AddWaypoint(13, 1676.89f, -3886.35f, 60.1993f);
                       AddWaypoint(14, 1685.88f, -3899.3f, 51.0708f);
                       AddWaypoint(15, 1695.84f, -3913.56f, 51.7773f);
                       AddWaypoint(16, 1703.56f, -3925.25f, 51.7773f);
                       AddWaypoint(17, 1709.67f, -3934.51f, 50.1544f);
                       AddWaypoint(18, 1725.52f, -3962.83f, 48.8759f);
                       AddWaypoint(19, 1736.38f, -3984.52f, 47.2716f);
                       AddWaypoint(20, 1738.96f, -3997.09f, 47.2195f);
                       AddWaypoint(21, 1744.59f, -4024.54f, 45.6987f);
                       AddWaypoint(22, 1740.68f, -4043.64f, 44.4844f);
                       AddWaypoint(23, 1737.47f, -4059.31f, 44.3825f);
                       AddWaypoint(24, 1727.31f, -4075.26f, 43.2196f);
                       AddWaypoint(25, 1708.32f, -4085.56f, 39.9161f);
                       AddWaypoint(26, 1685.14f, -4091.96f, 37.0901f);
                       AddWaypoint(27, 1656.55f, -4099.85f, 33.3951f);
                       AddWaypoint(28, 1632.1f, -4109.15f, 29.7992f);
                       AddWaypoint(29, 1602.63f, -4123.81f, 29.8898f);
                       AddWaypoint(30, 1589.84f, -4142.75f, 32.8694f);
                       AddWaypoint(31, 1577.11f, -4171.72f, 37.2662f);
                       AddWaypoint(32, 1579.53f, -4199.61f, 41.4442f);
                       AddWaypoint(33, 1589.97f, -4223.85f, 44.4204f);
                       AddWaypoint(34, 1605.23f, -4244.94f, 45.2712f);
                       AddWaypoint(35, 1620.69f, -4261.81f, 46.6678f);
                       AddWaypoint(36, 1632.14f, -4270.29f, 47.2125f);
                       AddWaypoint(37, 1640.85f, -4272.47f, 48.1457f);
                       AddWaypoint(38, 1654.74f, -4264.77f, 50.602f);
                       AddWaypoint(39, 1667.68f, -4255.38f, 51.4956f);
                       AddWaypoint(40, 1683.79f, -4247.68f, 51.2676f);
                       AddWaypoint(41, 1693.73f, -4238.3f, 50.8625f);
                       AddWaypoint(42, 1710.45f, -4231.4f, 48.7469f);
                       AddWaypoint(43, 1730.3f, -4222.56f, 45.4007f);
                       AddWaypoint(44, 1739.87f, -4216.73f, 44.3453f);
                       AddWaypoint(45, 1747.3f, -4214.68f, 43.4378f);
                       AddWaypoint(46, 1762.03f, -4217.22f, 41.7605f);
                       AddWaypoint(47, 1785.69f, -4225.57f, 39.3759f);
                       AddWaypoint(48, 1806.41f, -4233.16f, 39.8077f);
                       AddWaypoint(49, 1824.98f, -4239.19f, 40.8322f);
                       AddWaypoint(50, 1850.05f, -4245.24f, 41.6494f);
                       AddWaypoint(51, 1859.39f, -4251.23f, 40.2888f);
                       AddWaypoint(52, 1866.67f, -4258.31f, 36.1059f);
                       AddWaypoint(53, 1874.24f, -4265.62f, 32.4813f);
                       AddWaypoint(54, 1887.84f, -4280.86f, 30.8224f);
                       AddWaypoint(55, 1897.96f, -4284.08f, 30.3276f);
                       AddWaypoint(56, 1913.27f, -4285.86f, 30.1993f);
                       AddWaypoint(57, 1924.72f, -4285.74f, 29.1809f);
                       AddWaypoint(58, 1929.71f, -4279.28f, 30.0934f);
                       AddWaypoint(59, 1930.11f, -4272.29f, 31.9134f);
                       AddWaypoint(60, 1929.27f, -4259.35f, 38.1011f);
                       AddWaypoint(61, 1928.6f, -4249.1f, 41.1866f);
                       AddWaypoint(62, 1928.13f, -4241.87f, 42.1701f);
                       AddWaypoint(63, 1926.98f, -4227.64f, 42.3229f);
                       AddWaypoint(64, 1926.23f, -4218.46f, 41.1732f);
                       AddWaypoint(65, 1925.66f, -4211.36f, 42.3222f);
                       AddWaypoint(66, 1923.96f, -4184.43f, 42.1629f);
                       AddWaypoint(67, 1923.67f, -4178.94f, 41.1508f);
                       AddWaypoint(68, 1922.69f, -4160.52f, 40.6109f);
                       AddWaypoint(69, 1921.01f, -4140.74f, 40.6021f);
                       AddWaypoint(70, 1920.61f, -4136.1f, 40.5292f);
						break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_ingenieur_foudrepiqueAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_ingenieur_foudrepiqueAI* pEscortAI = CAST_AI(npc_ingenieur_foudrepiqueAI, me->AI()))
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
        return new npc_ingenieur_foudrepiqueAI (pCreature);
    }
};










/*****************************************************/
/* Troll de Bataille H2 Wave Entrée Ouest Orgrimmar  */
/*****************************************************/


class npc_troll_bataille : public CreatureScript
{

public:
    npc_troll_bataille() : CreatureScript("npc_troll_bataille") { }

    struct npc_troll_batailleAI : public npc_escortAI
    {
        npc_troll_batailleAI(Creature *pCreature) : npc_escortAI(pCreature)
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
			    case 4:
				       SetEscortPaused(true);
				       break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
				       AddWaypoint(1, 1745.14f, -3989.92f, 48.0638f);
                       AddWaypoint(2, 1728.03f, -3958.56f, 49.0743f);
                       AddWaypoint(3, 1707.02f, -3923.25f, 51.7779f);
                       AddWaypoint(4, 1699.83f, -3911.07f, 51.7779f);
					   break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_troll_batailleAI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_troll_batailleAI* pEscortAI = CAST_AI(npc_troll_batailleAI, me->AI()))
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
        return new npc_troll_batailleAI (pCreature);
    }
};




/*****************************************************/
/* Troll de Bataille1 H2 Wave Entrée Ouest Orgrimmar */
/*****************************************************/


class npc_troll_bataille1 : public CreatureScript
{

public:
    npc_troll_bataille1() : CreatureScript("npc_troll_bataille1") { }

    struct npc_troll_bataille1AI : public npc_escortAI
    {
        npc_troll_bataille1AI(Creature *pCreature) : npc_escortAI(pCreature)
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
				case 4:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
				       AddWaypoint(1, 1743.18f, -3992.9f, 47.2279f);
                       AddWaypoint(2, 1721.81f, -3954.04f, 48.9854f);
                       AddWaypoint(3, 1705.56f, -3924.89f, 51.7776f);
                       AddWaypoint(4, 1698.84f, -3914.23f, 51.7776f);
					   break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_troll_bataille1AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_troll_bataille1AI* pEscortAI = CAST_AI(npc_troll_bataille1AI, me->AI()))
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
        return new npc_troll_bataille1AI (pCreature);
    }
};




/*****************************************************/
/* Troll de Bataille2 H2 Wave Entrée Ouest Orgrimmar */
/*****************************************************/


class npc_troll_bataille2 : public CreatureScript
{

public:
    npc_troll_bataille2() : CreatureScript("npc_troll_bataille2") { }

    struct npc_troll_bataille2AI : public npc_escortAI
    {
        npc_troll_bataille2AI(Creature *pCreature) : npc_escortAI(pCreature)
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
				case 4:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
				       AddWaypoint(1, 1741.43f, -3997.52f, 46.5256f);
					   AddWaypoint(2, 1724.34f, -3956.23f, 48.9661f);
                       AddWaypoint(3, 1703.84f, -3925.29f, 51.7775f);
                       AddWaypoint(4, 1695.87f, -3912.99f, 51.7775f);
					   break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_troll_bataille2AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_troll_bataille2AI* pEscortAI = CAST_AI(npc_troll_bataille2AI, me->AI()))
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
        return new npc_troll_bataille2AI (pCreature);
    }
};









/*****************************************************/
/* Troll de Bataille3 H2 Wave Entrée Ouest Orgimmar  */
/*****************************************************/


class npc_troll_bataille3 : public CreatureScript
{

public:
    npc_troll_bataille3() : CreatureScript("npc_troll_bataille3") { }

    struct npc_troll_bataille3AI : public npc_escortAI
    {
        npc_troll_bataille3AI(Creature *pCreature) : npc_escortAI(pCreature)
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
				case 4:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
				       AddWaypoint(1, 1734.33f, -3996.45f, 48.9352f);
                       AddWaypoint(2, 1719.93f, -3962.88f, 50.2552f);
                       AddWaypoint(3, 1699.7f, -3929.69f, 51.7778f);
                       AddWaypoint(4, 1690.6f, -3916.17f, 51.7778f);
					   break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_troll_bataille3AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_troll_bataille3AI* pEscortAI = CAST_AI(npc_troll_bataille3AI, me->AI()))
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
        return new npc_troll_bataille3AI (pCreature);
    }
};





/*****************************************************/
/* Troll de Bataille4 H2 Wave Entrée Ouest Orgimmar  */
/*****************************************************/


class npc_troll_bataille4 : public CreatureScript
{

public:
    npc_troll_bataille4() : CreatureScript("npc_troll_bataille4") { }

    struct npc_troll_bataille4AI : public npc_escortAI
    {
        npc_troll_bataille4AI(Creature *pCreature) : npc_escortAI(pCreature)
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
				case 4:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
				       AddWaypoint(1, 1737.05f, -3997.99f, 47.975f);
                       AddWaypoint(2, 1722.08f, -3962.49f, 49.707f);
                       AddWaypoint(3, 1700.77f, -3928.34f, 51.7774f);
                       AddWaypoint(4, 1693.28f, -3917.18f, 51.7774f);
					   break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_troll_bataille4AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_troll_bataille4AI* pEscortAI = CAST_AI(npc_troll_bataille4AI, me->AI()))
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
        return new npc_troll_bataille4AI (pCreature);
    }
};






/*****************************************************/
/* Troll de Bataille5 H2 Wave Entrée Ouest Orgimmar  */
/*****************************************************/


class npc_troll_bataille5 : public CreatureScript
{

public:
    npc_troll_bataille5() : CreatureScript("npc_troll_bataille5") { }

    struct npc_troll_bataille5AI : public npc_escortAI
    {
        npc_troll_bataille5AI(Creature *pCreature) : npc_escortAI(pCreature)
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
				case 4:
				        SetEscortPaused(true);
				        break;
			}
		}

		void SetupMovement(uint32 variation)
		{
			switch (variation)
			{
				case 0:
				       AddWaypoint(1, 1740.45f, -3998.83f, 46.7284f);
                       AddWaypoint(2, 1723.55f, -3961.89f, 49.2986f);
                       AddWaypoint(3, 1702.07f, -3927.28f, 51.7778f);
                       AddWaypoint(4, 1693.65f, -3914.03f, 51.7778f);
					   break;
				default: sLog->outError("Unexpected movement variation (%i) in npc_troll_bataille5AI::SetupMovement call!", variation); 
                         return;
			}
			
			if(npc_troll_bataille5AI* pEscortAI = CAST_AI(npc_troll_bataille5AI, me->AI()))
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
        return new npc_troll_bataille5AI (pCreature);
    }
};





































void AddSC_npcevent2()
{
	new npcthrall;
	new npcgoderik;
	new npcvoljin;
	new boss_shaman;
    new mob_shaman_vortex;
    new npcgeneralfourdrepique;
    new spell_foudrepique_icy_grip;
    new npc_infanterie_argent;
    new npc_mage_argent;
    new npc_archer_argent;
    new npc_stolen_soul2;
    new npc_orc_bataille;
    new npc_orc_bataille1;
    new npc_orc_bataille2;
    new npc_orc_bataille3;
    new npc_orc_bataille4;
    new npc_orc_bataille5;
    new npc_troll_bataille;
    new npc_troll_bataille1;
    new npc_troll_bataille2;
    new npc_troll_bataille3;
    new npc_troll_bataille4;
    new npc_troll_bataille5;
    new npc_ingenieur_foudrepique;
    new npc_mage_foudrepique;
    new npc_infanterie_foudrepique;
}

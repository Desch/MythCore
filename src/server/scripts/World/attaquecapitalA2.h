
#define SPEED_WALK                           (0.5f)
#define SPEED_RUN                            (1.0f)
#define SPEED_MOUNT                          (1.6f)
#define MOUNT_MODEL                          28918
#define MOUNT_MODEL2                         21974

// Texte Archer Défias
#define SAY_ARCHERDEFIAS_AGGRO               "La Confrérie Vaincra!"
#define SAY_ARCHERDEFIAS_SLAY                "Nul ne pourra s'opposer!"
#define SAY_ARCHERDEFIAS_DEATH               "Vous ne pourrez pas tenir..."
#define SAY_ARCHERDEFIAS_ICE                 "Et comme ceci , ce n'est pas mieux?"

// texte Mage Défias
#define SAY_MAGEDEFIAS_AGGRO                 "Vous ne ferez pas le poids contre nous les Défias!"
#define SAY_MAGEDEFIAS_SLAY                  "Vos futiles tentatives resterons Vaines!"
#define SAY_MAGEDEFIAS_DEATH                 "Notre Maitre aura raison de vous..."
#define SAY_MAGEDEFIAS_SOUL                  "Que les ténèbres vous recouvre!"

// Texte Infanterie defias
#define SAY_INFANTERIEDEFIAS_AGGRO           "Mes Frère Défias, Chargés!"
#define SAY_INFANTERIEDEFIAS_SLAY            "Encore un traitre qui trépasse!"
#define SAY_INFANTERIEDEFIAS_DEATH           "Gloire... et honneur.... pour les Défias....."

// Texte Général Défias
#define SAY_LYCANTHROPE_AGGRO                "Comment osez-vous défier la Confrérie des Défias!"
#define SAY_LYCANTHROPE_SLAY                 "Lycan Venez a moi!!!!"
#define SAY_LYCANTHROPE_DEATH                "Noooooonnnn, Comment es-ce pos...sssi..bllee..."
#define SAY_LYCANTHROPE_MORPH                "Esprit du Lycan investie MOI!"
#define SAY_LYCANTHROPE_DEMORPH              "Esprit du Lycan pourquoi abandonnes-tu ton Serviteur...."
#define SAY_TRASHLYCAN_SPAWN                 "Serviteur Viens à mon Aide!!"
#define EMOTE_WORGEN_BITE                    "Général Défias commence à lancer Tourbillon!"

// Texte Minion Général
#define SAY_TRASHLYCAN_AGGRO                 "Qui Ose s'en prendre au Maitre!"
#define SAY_TRASHLYCAN_SLAY                  "Vous ne pouvez me vaincre!!!"
#define SAY_TRASHLYCAN_DEATH                 "Maitre j'ai échoué..."
#define SAY_TRASHLYCAN_ICE                   "Cela Vous convient-il?"
#define EMOTE_TRASHLYCAN_VORTEX              "Serviteur Défias lance des Tornades dévastatrices Courez !!!"

// Texte Misc/Cosmétique
#define SPELL_ENERGY_STORM                   43983   // enemy area aura, trigger 42577
#define SPELL_ZAP_INFORM                     42577
#define SPELL_ZAP_DAMAGE                     43137   // 1250 damage
#define SPELL_SUMMON_CYCLONE                 43112   // Spawn vortex 
#define SPELL_CYCLONE_VISUAL                 43119   // trigger 43147 visual
#define SPELL_CYCLONE_PASSIVE                43120   // trigger 43121 4 aoe toutes les secondes

// texte Mage Gobelin
#define SAY_MAGEGOBELIN_AGGRO                "Venez affonter la Furie des éléments!"
#define SAY_MAGEGOBELIN_SLAY                 "Mettez un terme à votre agonie!"
#define SAY_MAGEGOBELIN_DEATH                "La magie des gobelins... est la plus puissante..."
#define SAY_MAGEGOBELIN_BLOOD                "Périssez dans la Souffrance!"

// texte infanterie Gobelin
#define SAY_INFANTERIEGOBELIN_AGGRO          "Frêre Gobelin Chargééééééééé!!!!!!!"
#define SAY_INFANTERIEGOBELIN_SLAY           "Vous trépasserez tous sous mes coups!"
#define SAY_INFANTERIEGOBELIN_DEATH          "N...o..n.... impo....ssible... Argh..."

// texte ingénieur Gobelin
#define SAY_INGENIEURGOBELIN_AGGRO          "Ma puissance n'a d'égale que mon intelligence !"
#define SAY_INGENIEURGOBELIN_SLAY           "Les Gobelins règneront!!!!"
#define SAY_INGENIEURGOBELIN_DEATH          "Maitre.... nous avons.... échoué...."
// texte Boss Gobelin
#define SAY_BOMBERMAN_AGGRO    "Ca fesait longtemps que je n'avais pas mit le feu aux poudres !"
#define SAY_BOMBERMAN_DEATH    "Je... reviendrais..."
#define SAY_BOMBERMAN_BERSERK  "Votre fin est... IMMINENTE !"
#define SAY_KILL_ONE           "Hasta la vista, baby"
#define SAY_KILL_TWO           "Tes rêves viennent de partir en fumée !" 
#define SAY_KILL_THREE         "Redeviens poussière !"
#define SAY_TOWER_SPAWN        "Voilà qui devrait vous rafraîchir les idées !"
#define SAY_LIVING_BOMB        "Goûtez aux flammes de l'Enfer !"

// texte Premier add Boss Gobelin: Tower
#define SAY_TOWER_DEATH     "B...oum..."

// texte Deuxième add Boss Gobelin : Tower
#define SAY_ALARM_DEATH     "B...oum..."
#define SAY_ALARM_ALERT     "Alerte ! Ennemis détéctés !"

// texte Troisième add Boss Gobelin : Tower
#define SAY_BOMB_DEATH "Ca va péter !"



// Gestion des Phases
enum Phases
{
    PHASE_NEUTRE = 0,
    PHASE_INTRO,
    PHASE_ONE,
    PHASE_TWO
};

// Trash Mobs et boss spawn dans les waves
enum eCreature
{
    INFANTERIE_DEFIAS    	    = 100020,
    MAGE_DEFIAS    	            = 100021,
    ARCHER_DEFIAS    	        = 100022,
    LYCAN		                = 100010,
    ROI                         = 100024,
    JAINA                       = 100025,
    GENERAL                     = 100019,
    MAGE_GOBELIN    	        = 100027,
    INFANTERIE_GOBELIN    	    = 100028,
    INGENIEUR_GOBELIN    	    = 100030,
    BOMBERMAN                   = 100012
};

enum EncounterData
{
    ENCOUNTER_WAVES_NUMBER 		= 9,
    ENCOUNTER_WAVES_MAX_SPAWNS	= 10
};

enum WorldStatesCoT
{
	WORLDSTATE_WAVE_COUNT = 3504
};


/*#####################################################
# Trash Vagues Hurlevent HurleventBaseWaves ARCHER_DEFIAS
######################################################*/

enum spellarcher
{
    MULTISHOT                                               = 40872,
    VOLLEY                                                  = 38633,
    SUMMONED_SNAKE                                          = 29444,
    BOLT_VOLLEY                                             = 34780,
    BEWITCHING_AURA                                         = 29486,
    REJUVENATION                                            = 71142,
    ICE_TRAP                                                = 71249,
    AURA_ROT                                                = 25818
};


/*#####################################################
# Trash Vagues Hurlevent HurleventBaseWaves MAGE_DEFIAS
######################################################*/

enum Spellmage
{
    STORM_OF_GRIEF                                          = 55847,
    SHADOW_BURST                                            = 34436,
    PIERCING_SHADOW                                         = 36698,
    SHADOW_FLAME                                            = 22539,
    HARVEST_SOUL                                            = 28679,
    SHADOW_BOLT                                             = 41957
};

/*#####################################################
# Trash Vagues Hurlevent HurleventBaseWaves INFANTERIE_DEFIAS
######################################################*/

enum spellinfanterie
{
    ENRAGER                                                 = 48138,
    FRENZI                                                  = 48142,
    MORTALE_WOUND                                           = 59265,
    DECREPIT                                                = 54098,
    WORGEN_BIT                                              = 55266
};


/*#####################################################
# Boss Général Défias Porte Hurlevent
######################################################*/

enum lycanthropeSpells
{
    ENRAGE                                                  = 48138,
    FRENZY                                                  = 48142,
    MORTAL_WOUN                                             = 59265,
    WORGEN_BITE                                             = 55266,
    WORGEN_CALL                                             = 53095,
    DECREPIT_FEVER                                          = 54098,
    SUMMON_WORGEN                                           = 27020,
    SUMMON_TRASHLYCAN                                       = 100004
};

/*#####################################################
# Minion du Général Défias
######################################################*/

enum TrashlycanSpells
{
    SPELL_MULTISHOT                                         = 40872,
    SPELL_VOLLEY                                            = 38633,
    SUMMONED_VORTEX                                         = 100017,
    SPELL_BOLT_VOLLEY                                       = 34780,
    SPELL_BEWITCHING_AURA                                   = 29486,
    SPELL_REJUVENATION                                      = 71142,
    SPELL_ICE_TRAP                                          = 71249,
    SPELL_AURA_ROT                                          = 25818
};

/*#####################################################
# Trash Vagues Hurlevent PortBaseWaves MAGE_GOBELIN
######################################################*/

enum MagegobelinSpells
{
    HEALING_WAVE                                            = 51586,
    LIGHTNING_BOLT                                          = 59024,
    BLOODLUST                                               = 41185,
    LIGHTNING_SHIELD                                        = 59025,
    SUMMON_AIR_ELEMENTAL                                    = 30418,
    LAVA_BURST                                              = 59519,
    CHAIN_LIGHTNING                                         = 54531,
    FLAME_SHOCK                                             = 43303,
    EARTH_SHOCK                                             = 43305,
    LIGHTNING_RING                                          = 59848
};

/*###########################################################
# Trash Vagues Hurlevent PortBaseWaves INFANTERIE_GOBELIN
############################################################*/

enum InfanteriegobelinSpells
{
    RAGE                                                    = 48138,
    TOURBILLON                                              = 55266,
    FRENZ                                                   = 48142,
    CHAINCRISTAL                                            = 47698,
    HURLEMENT                                               = 16838
};

/*###########################################################
# Trash Vagues Hurlevent PortBaseWaves INGENIEUR_GOBELIN
############################################################*/

enum IngenieurgobelinSpells
{
    SUMMON_STOLEN_SOUL                                = 100029,
    STOLEN_SOUL                                       = 32346,
    STOLEN_SOUL_VISUAL                                = 32395,
    MOONFIRE                                          = 37328,
    FIREBALL                                          = 37329,
    MIND_FLAY                                         = 37330,
    HEMORRHAGE                                        = 37331,
    FROSTSHOCK                                        = 37332,
    CURSE_OF_AGONY                                    = 37334,
    MORTAL_STRIKE                                     = 37335,
    FREEZING_TRAP                                     = 37368,
    HAMMER_OF_JUSTICE                                 = 37369,
    EXPLOSION                                         = 44314,
    CRI                                               = 19134,
    BOMBES                                            = 46184,
    ROCKET                                            = 46187
};

/*###########################################################
/* Boss Gobelin Port de Hurlevent          
############################################################*/

enum bombermanSpells
{
    BERSERK                                                 = 26662, /* Berserk au bout de 10min */
    MORTA_WOUND                                             = 54378, /* Blessure mortelle sur tank, force à switch */
    LIVING_BOMB                                             = 20475, /* Bombe vivante aléatoire toutes les x min */
    SLOW_FALL												= 130, /* Chute lente en même temps que Bombe vivante */
	ELECTRIFIED_NET                                         = 35107, /* Sarment toutes les x min */
    SAW_BLADE                                               = 31486, /* Dégât au cac */
    SUPER_SHRINK_RAY                                        = 31485, /* Réduit dégâts ennemis */
    SUMMON_TOWER                                            = 100013,
    SUMMON_ALARM                                            = 100014


};

/*###########################################################
/* Premier add : Tower 
############################################################*/

enum AddBombermanTowerSpells
{
    ICE_NOVA                = 64919, /* Réduit vitesse, augmente dégât givre */
    WATER_SPRAY             = 29923, /* Gros dégâts de givre */
    DEFENSE_MATRIX_TOWER    = 65070, /* Bouclier à 20%*/
    EXPLOSION_TOWER         = 25699 /* Explose à la mort */
};

/*###########################################################
/* Deuxième add : Tower 
############################################################*/

enum AddBombermanAlarmSpells
{
    DEFENSE_MATRIX_ALARM  = 65070, /* Bouclier à 20%*/
    EXPLOSION_ALARM       = 25699, /* Explose à la mort */
    SUMMON_BOMB           = 100015 /* Invoque porteur de bombes */
};

/*###########################################################
/* Troisième add : Tower 
############################################################*/

enum AddAlarmBombSpells
{
    LIVING_BOMB_MINE     = 20475, /* Bombe vivante sur lui pour s'auto-détruire */
    EXPLOSION_BOMB      = 25699, /* Explose à la mort */
    SUMMON_MINE          = 100016  /* Invoque mine */
};

// spawn des Wave 1-8
float HurleventBaseWaves[ENCOUNTER_WAVES_NUMBER][ENCOUNTER_WAVES_MAX_SPAWNS][5]= // gestion du spawn des trash Entrée Hurlevent
{
	{
        {ARCHER_DEFIAS,     -9180.14f, 341.82f, 83.88f, 0.80f },
		{MAGE_DEFIAS,       -9178.20f, 339.93f, 83.80f, 0.80f },
		{INFANTERIE_DEFIAS, -9175.77f, 338.08f, 83.67f, 0.80f },
		{MAGE_DEFIAS,       -9173.75f, 336.27f, 83.68f, 0.80f },
		{ARCHER_DEFIAS,     -9171.99f, 334.69f, 83.67f, 0.80f },
		{ARCHER_DEFIAS,     -9183.23f, 336.67f, 82.73f, 0.80f },
		{MAGE_DEFIAS,       -9181.29f, 334.90f, 82.61f, 0.80f },
		{INFANTERIE_DEFIAS, -9179.31f, 333.12f, 82.39f, 0.80f },
		{MAGE_DEFIAS,       -9177.43f, 331.74f, 82.30f, 0.80f },
		{ARCHER_DEFIAS,     -9174.75f, 329.75f, 82.17f, 0.80f }
	},
	{
        {ARCHER_DEFIAS,     -9180.14f, 341.82f, 83.88f, 0.80f },
		{MAGE_DEFIAS,       -9178.20f, 339.93f, 83.80f, 0.80f },
		{INFANTERIE_DEFIAS, -9175.77f, 338.08f, 83.67f, 0.80f },
		{MAGE_DEFIAS,       -9173.75f, 336.27f, 83.68f, 0.80f },
		{ARCHER_DEFIAS,     -9171.99f, 334.69f, 83.67f, 0.80f },
		{ARCHER_DEFIAS,     -9183.23f, 336.67f, 82.73f, 0.80f },
		{MAGE_DEFIAS,       -9181.29f, 334.90f, 82.61f, 0.80f },
		{INFANTERIE_DEFIAS, -9179.31f, 333.12f, 82.39f, 0.80f },
		{MAGE_DEFIAS,       -9177.43f, 331.74f, 82.30f, 0.80f },
		{ARCHER_DEFIAS,     -9174.75f, 329.75f, 82.17f, 0.80f }
	},
	{
		{ARCHER_DEFIAS,     -9180.14f, 341.82f, 83.88f, 0.80f },
		{MAGE_DEFIAS,       -9178.20f, 339.93f, 83.80f, 0.80f },
		{INFANTERIE_DEFIAS, -9175.77f, 338.08f, 83.67f, 0.80f },
		{MAGE_DEFIAS,       -9173.75f, 336.27f, 83.68f, 0.80f },
		{ARCHER_DEFIAS,     -9171.99f, 334.69f, 83.67f, 0.80f },
		{ARCHER_DEFIAS,     -9183.23f, 336.67f, 82.73f, 0.80f },
		{MAGE_DEFIAS,       -9181.29f, 334.90f, 82.61f, 0.80f },
		{INFANTERIE_DEFIAS, -9179.31f, 333.12f, 82.39f, 0.80f },
		{MAGE_DEFIAS,       -9177.43f, 331.74f, 82.30f, 0.80f },
		{ARCHER_DEFIAS,     -9174.75f, 329.75f, 82.17f, 0.80f }
	},
	{
		{ARCHER_DEFIAS,     -9180.14f, 341.82f, 83.88f, 0.80f },
		{MAGE_DEFIAS,       -9178.20f, 339.93f, 83.80f, 0.80f },
		{INFANTERIE_DEFIAS, -9175.77f, 338.08f, 83.67f, 0.80f },
		{MAGE_DEFIAS,       -9173.75f, 336.27f, 83.68f, 0.80f },
		{ARCHER_DEFIAS,     -9171.99f, 334.69f, 83.67f, 0.80f },
		{ARCHER_DEFIAS,     -9183.23f, 336.67f, 82.73f, 0.80f },
		{MAGE_DEFIAS,       -9181.29f, 334.90f, 82.61f, 0.80f },
		{INFANTERIE_DEFIAS, -9179.31f, 333.12f, 82.39f, 0.80f },
		{MAGE_DEFIAS,       -9177.43f, 331.74f, 82.30f, 0.80f },
		{ARCHER_DEFIAS,     -9174.75f, 329.75f, 82.17f, 0.80f }
	},
	{
		{ARCHER_DEFIAS,     -9180.14f, 341.82f, 83.88f, 0.80f },
		{MAGE_DEFIAS,       -9178.20f, 339.93f, 83.80f, 0.80f },
		{INFANTERIE_DEFIAS, -9175.77f, 338.08f, 83.67f, 0.80f },
		{MAGE_DEFIAS,       -9173.75f, 336.27f, 83.68f, 0.80f },
		{ARCHER_DEFIAS,     -9171.99f, 334.69f, 83.67f, 0.80f },
		{ARCHER_DEFIAS,     -9183.23f, 336.67f, 82.73f, 0.80f },
		{MAGE_DEFIAS,       -9181.29f, 334.90f, 82.61f, 0.80f },
		{INFANTERIE_DEFIAS, -9179.31f, 333.12f, 82.39f, 0.80f },
		{MAGE_DEFIAS,       -9177.43f, 331.74f, 82.30f, 0.80f },
		{ARCHER_DEFIAS,     -9174.75f, 329.75f, 82.17f, 0.80f }
	},
	{
		{ARCHER_DEFIAS,     -9180.14f, 341.82f, 83.88f, 0.80f },
		{MAGE_DEFIAS,       -9178.20f, 339.93f, 83.80f, 0.80f },
		{INFANTERIE_DEFIAS, -9175.77f, 338.08f, 83.67f, 0.80f },
		{MAGE_DEFIAS,       -9173.75f, 336.27f, 83.68f, 0.80f },
		{ARCHER_DEFIAS,     -9171.99f, 334.69f, 83.67f, 0.80f },
		{ARCHER_DEFIAS,     -9183.23f, 336.67f, 82.73f, 0.80f },
		{MAGE_DEFIAS,       -9181.29f, 334.90f, 82.61f, 0.80f },
		{INFANTERIE_DEFIAS, -9179.31f, 333.12f, 82.39f, 0.80f },
		{MAGE_DEFIAS,       -9177.43f, 331.74f, 82.30f, 0.80f },
		{ARCHER_DEFIAS,     -9174.75f, 329.75f, 82.17f, 0.80f }
	},
	{
		{ARCHER_DEFIAS,     -9180.14f, 341.82f, 83.88f, 0.80f },
		{MAGE_DEFIAS,       -9178.20f, 339.93f, 83.80f, 0.80f },
		{INFANTERIE_DEFIAS, -9175.77f, 338.08f, 83.67f, 0.80f },
		{MAGE_DEFIAS,       -9173.75f, 336.27f, 83.68f, 0.80f },
		{ARCHER_DEFIAS,     -9171.99f, 334.69f, 83.67f, 0.80f },
		{ARCHER_DEFIAS,     -9183.23f, 336.67f, 82.73f, 0.80f },
		{MAGE_DEFIAS,       -9181.29f, 334.90f, 82.61f, 0.80f },
		{INFANTERIE_DEFIAS, -9179.31f, 333.12f, 82.39f, 0.80f },
		{MAGE_DEFIAS,       -9177.43f, 331.74f, 82.30f, 0.80f },
		{ARCHER_DEFIAS,     -9174.75f, 329.75f, 82.17f, 0.80f }
	},
	{
		{ARCHER_DEFIAS,     -9180.14f, 341.82f, 83.88f, 0.80f },
		{MAGE_DEFIAS,       -9178.20f, 339.93f, 83.80f, 0.80f },
		{INFANTERIE_DEFIAS, -9175.77f, 338.08f, 83.67f, 0.80f },
		{MAGE_DEFIAS,       -9173.75f, 336.27f, 83.68f, 0.80f },
		{ARCHER_DEFIAS,     -9171.99f, 334.69f, 83.67f, 0.80f },
		{ARCHER_DEFIAS,     -9183.23f, 336.67f, 82.73f, 0.80f },
		{MAGE_DEFIAS,       -9181.29f, 334.90f, 82.61f, 0.80f },
		{INFANTERIE_DEFIAS, -9179.31f, 333.12f, 82.39f, 0.80f },
		{MAGE_DEFIAS,       -9177.43f, 331.74f, 82.30f, 0.80f },
		{ARCHER_DEFIAS,     -9174.75f, 329.75f, 82.17f, 0.80f }
	},
    {
		{LYCAN,             -9175.77f, 338.08f, 83.67f, 0.80f },
        {0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 }
	}
};

float PortBaseWaves[ENCOUNTER_WAVES_NUMBER][ENCOUNTER_WAVES_MAX_SPAWNS][5]= // gestion du spawn des trash Port de Hurlevent
{
	{
        {INGENIEUR_GOBELIN,  -8284.62f, 1318.90f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.61f, 1319.03f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.70f, 1319.14f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.55f, 1318.63f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.61f, 1318.66f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8284.03f, 1322.24f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.56f, 1322.28f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.91f, 1322.56f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.63f, 1322.27f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.66f, 1322.28f, 5.23f, 4.68f }
	},
    {
        {INGENIEUR_GOBELIN,  -8284.62f, 1318.90f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.61f, 1319.03f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.70f, 1319.14f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.55f, 1318.63f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.61f, 1318.66f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8284.03f, 1322.24f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.56f, 1322.28f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.91f, 1322.56f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.63f, 1322.27f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.66f, 1322.28f, 5.23f, 4.68f }
	},
	{
        {INGENIEUR_GOBELIN,  -8284.62f, 1318.90f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.61f, 1319.03f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.70f, 1319.14f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.55f, 1318.63f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.61f, 1318.66f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8284.03f, 1322.24f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.56f, 1322.28f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.91f, 1322.56f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.63f, 1322.27f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.66f, 1322.28f, 5.23f, 4.68f }
	},
	{
        {INGENIEUR_GOBELIN,  -8284.62f, 1318.90f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.61f, 1319.03f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.70f, 1319.14f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.55f, 1318.63f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.61f, 1318.66f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8284.03f, 1322.24f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.56f, 1322.28f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.91f, 1322.56f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.63f, 1322.27f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.66f, 1322.28f, 5.23f, 4.68f }
	},
	{
        {INGENIEUR_GOBELIN,  -8284.62f, 1318.90f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.61f, 1319.03f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.70f, 1319.14f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.55f, 1318.63f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.61f, 1318.66f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8284.03f, 1322.24f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.56f, 1322.28f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.91f, 1322.56f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.63f, 1322.27f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.66f, 1322.28f, 5.23f, 4.68f }
	},
	{
        {INGENIEUR_GOBELIN,  -8284.62f, 1318.90f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.61f, 1319.03f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.70f, 1319.14f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.55f, 1318.63f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.61f, 1318.66f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8284.03f, 1322.24f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.56f, 1322.28f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.91f, 1322.56f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.63f, 1322.27f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.66f, 1322.28f, 5.23f, 4.68f }
	}, 
	{
        {INGENIEUR_GOBELIN,  -8284.62f, 1318.90f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.61f, 1319.03f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.70f, 1319.14f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.55f, 1318.63f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.61f, 1318.66f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8284.03f, 1322.24f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.56f, 1322.28f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.91f, 1322.56f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.63f, 1322.27f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.66f, 1322.28f, 5.23f, 4.68f }
	},
	{
        {INGENIEUR_GOBELIN,  -8284.62f, 1318.90f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.61f, 1319.03f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.70f, 1319.14f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.55f, 1318.63f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.61f, 1318.66f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8284.03f, 1322.24f, 5.23f, 4.68f },
		{MAGE_GOBELIN,       -8287.56f, 1322.28f, 5.23f, 4.68f },
		{INFANTERIE_GOBELIN, -8290.91f, 1322.56f, 5.55f, 4.68f },
		{MAGE_GOBELIN,       -8293.63f, 1322.27f, 5.23f, 4.68f },
		{INGENIEUR_GOBELIN,  -8296.66f, 1322.28f, 5.23f, 4.68f }
	},
    {
		{BOMBERMAN,          -8290.70f, 1319.14f, 5.55f, 4.68f },
        {0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0 }
	}

};

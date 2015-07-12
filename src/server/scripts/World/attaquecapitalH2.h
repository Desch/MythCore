
#define SPEED_WALK                           (0.5f)
#define SPEED_RUN                            (1.0f)
#define SPEED_MOUNT                          (1.6f)
#define MOUNT_MODEL                          30070 //Monture Général Orc
#define MOUNT_MODEL2                         15289 //Monture Général Troll

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
    INFANTERIE_ARGENT    	    = 100050,
    MAGE_ARGENT    	            = 100051,
    ARCHER_ARGENT    	        = 100052,
    SHAMAN		                = 100006,
    MAGE_FOUDREPIQUE    	    = 100058,
    INFANTERIE_FOUDREPIQUE    	= 100057,
    INGENIEUR_FOUDREPIQUE    	= 100055,
    GENERAL                     = 100053
};

enum EncounterData
{
    ENCOUNTER_WAVES_NUMBER 		= 9,
    ENCOUNTER_WAVES_MAX_SPAWNS
};


//texte Boss Shaman Entrée Orgrimmar
const char* YELL_TRANSFORM_LYNX = "Laissez moi vous faire Goûter à ma forme Lynx!";
const char* YELL_TRANSFORM_BEAR = "Ma forme Ours aura raison de vous!";
const char* YELL_TRANSFORM_DRAGONHAWK = "Faucon-Dragon viens à moi!";
const char* YELL_TRANSFORM_EAGLE = "Personne n'échapera à mon regards!";

#define YELL_KILL_ONE                        "La mort n'est encore que trop douce!"
#define YELL_KILL_TWO                        "A qui le tour?!"
#define YELL_FIRE_BREATH                     "Souffrez dans les Flammes!"
#define YELL_AGGRO                           "Venez Goûter à ma Puissanceeeeeee!"
#define YELL_BERSERK                         "Vous n'avez réussi qu'a libérer ma Colère!!!!"
#define YELL_DEATH                           "Mes Forces m..e qu...it..teeeee.."
#define YELL_INTRO                           "Null ne pourra s'opposer à ma Vonlonté !"

//Transformations:
#define PHASE_BEAR 0
#define PHASE_EAGLE 1
#define PHASE_LYNX 2
#define PHASE_DRAGONHAWK 3
#define PHASE_TROLL 4

// Texte Archer Argent
#define SAY_ARCHERARGENT_AGGRO               "L'Aube d'Argent Vaincra!"
#define SAY_ARCHERARGENT_SLAY                "Nul ne pourra s'opposer!"
#define SAY_ARCHERARGENT_DEATH               "Vous ne pourrez pas tenir..."
#define SAY_ARCHERARGENT_ICE                 "Et comme ceci , ce n'est pas mieux?"

// texte Mage Argent
#define SAY_MAGEARGENT_AGGRO                 "Vous ne ferez pas le poids contre l'Aube d'argent!"
#define SAY_MAGEARGENT_SLAY                  "Vos futiles tentatives resterons Vaines!"
#define SAY_MAGEARGENT_DEATH                 "Notre Maitre aura raison de vous..."
#define SAY_MAGEARGENT_SOUL                  "Que les ténèbres vous recouvre!"

// Texte Infanterie Argent
#define SAY_INFANTERIEARGENT_AGGRO           "Mes Frère de l'Aube d'Argent, Chargés!"
#define SAY_INFANTERIEARGENT_SLAY            "Encore un traitre qui trépasse!"
#define SAY_INFANTERIEARGENT_DEATH           "Gloire... et honneur.... pour l'Aube d'Argent....."



// Texte Général Garde Foudrepique
#define SAY_GENERALFOUDREPIQUE_AGGRO         "La Garde Foudrepique Vaincra !"
#define SAY_GENERALFOUDREPIQUE_SLAY          "J'ai en ma possession toute la puissance de l'eau !"
#define SAY_GENERALFOUDREPIQUE_DEATH         "Ce n'...est p....as fini !!"
#define SAY_GENERALFOUDREPIQUE_MORPH         "Général Foudrepique boit une chope et se transforme !"
#define SAY_GENERALFOUDREPIQUE_DEMORPH       "Général Foudrepique reprend sa forme initiale."
#define EMOTE_GENERALFOUDREPIQUE_GRIP        "Général Foudrepique va lancer Explosion Sonore. Ecartez vous !!!"


// texte Mage Foudrepique
#define SAY_MAGEFOUDREPIQUE_AGGRO            "Venez affonter la Furie des éléments!"
#define SAY_MAGEFOUDREPIQUE_SLAY             "Mettez un terme à votre agonie!"
#define SAY_MAGEFOUDREPIQUE_DEATH            "La magie des gobelins... est la plus puissante..."
#define SAY_MAGEFOUDREPIQUE_BLOOD            "Périssez dans la Souffrance!"

// texte infanterie Foudrepique
#define SAY_INFANTERIEFOUDREPIQUE_AGGRO      "Frêre Gobelin Chargééééééééé!!!!!!!"
#define SAY_INFANTERIEFOUDREPIQUE_SLAY       "Vous trépasserez tous sous mes coups!"
#define SAY_INFANTERIEFOUDREPIQUE_DEATH      "N...o..n.... impo....ssible... Argh..."

// texte ingénieur foudrepique
#define SAY_INGENIEURFOUDREPIQUE_AGGRO       "Ma puissance n'a d'égale que mon intelligence !"
#define SAY_INGENIEURFOUDREPIQUE_SLAY        "Les Gobelins règneront!!!!"
#define SAY_INGENIEURFOUDREPIQUE_DEATH       "Maitre.... nous avons.... échoué...."


//Coordonnés pour lui donner un point fix pour changer de forme (selon l'endroit ou il est spawn)
/*#define CENTER_X 120.148811f
#define CENTER_Y 703.713684f
#define CENTER_Z 45.111477f*/




/*#####################################################
# Boss Shaman Porte Orgrimmar
######################################################*/

enum shamanSpells
{
    SPELL_WHIRLWIND                                         = 17207,
    SPELL_GRIEVOUS_THROW                                    = 43093,
    SPELL_CREEPING_PARALYSIS                                = 43095,
    SPELL_OVERPOWER                                         = 43456,
    SPELL_ENERGY_STORM                                      = 43983,
    SPELL_ZAP_INFORM                                        = 42577,
    SPELL_ZAP_DAMAGE                                        = 43137,
    SPELL_SUMMON_CYCLONE                                    = 43112,
    CREATURE_FEATHER_VORTEX                                 = 100005,
    SPELL_CYCLONE_VISUAL                                    = 43119,
    SPELL_CYCLONE_PASSIVE                                   = 43120,
    SPELL_ECLAIR                                            = 50215,
    SPELL_CLAW_RAGE_HASTE                                   = 42583,
    SPELL_CLAW_RAGE_TRIGGER                                 = 43149,
    SPELL_CLAW_RAGE_DAMAGE                                  = 43150,
    SPELL_LYNX_RUSH_HASTE                                   = 43152,
    SPELL_LYNX_RUSH_DAMAGE                                  = 43153,
    SPELL_FLAME_WHIRL                                       = 43213,
    SPELL_FLAME_BREATH                                      = 43215,
    SPELL_SUMMON_PILLAR                                     = 43216,
    CREATURE_COLUMN_OF_FIRE                                 = 24187,
    SPELL_PILLAR_TRIGGER                                    = 43218,
    SPELL_SPIRIT_AURA                                       = 42466,
    SPELL_SIPHON_SOUL                                       = 43501,
    SPELL_SHAPE_OF_THE_BEAR                                 = 42594,
    SPELL_SHAPE_OF_THE_EAGLE                                = 42606,
    SPELL_SHAPE_OF_THE_LYNX                                 = 42607,
    SPELL_SHAPE_OF_THE_DRAGONHAWK                           = 42608,
    SPELL_BERSERK                                           = 45078   
};


/*#####################################################
# Boss Général Foudrepique Porte Orgrimmar
######################################################*/

enum generalfoudrepiqueSpells
{
    ENRAGE                                                  = 48138,
    FRENZY                                                  = 48142,
	BONE_SLICE												= 69055, //Cleave
	CHILLING_WAVE											= 68778, //Vague Glaciale
	WATER_BLAST_NORMAL										= 54237, //Trait d'eau	
	WATER_BLAST_HERO										= 59520, //Trait d'eau
	WATER_BOLT_VOLLEY										= 59521, //Salve de traits d'eau
	SUMMON_WATERELEMENTAL                                   = 100054,
	SPELL_ICY_GRIP_JUMP                                     = 70122, // Appelé lors du grip
	SPELL_GRIP												= 70117, //Grip
	SONIC_BURST												= 29904, //Chocs sonore
	SPELL_CLEAVE                							= 19983 //Cleave
};



/*#####################################################
# Trash Vagues Orgrimmar EntréeBaseWaves ARCHER_ARGENT
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
# Trash Vagues Orgrimmar EntréeBaseWaves MAGE_Argent
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
# Trash Vagues Orgrimmar EntréeBaseWaves INFANTERIE_Argent
######################################################*/

enum spellinfanterie
{
    ENRAGER                                                 = 48138,
    FRENZI                                                  = 48142,
    MORTALE_WOUND                                           = 59265,
    DECREPIT                                                = 54098,
    WORGEN_BIT                                              = 55266
};


/*#########################################################
# Trash Vagues Orgimmar PortOuestBaseWaves MAGE_FOUDREPIQUE
#########################################################*/

enum MagefoudrepiqueSpells
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

/*################################################################
# Trash Vagues Orgimmar PortOuestBaseWaves INFANTERIE_FOUDREPIQUE
################################################################*/

enum InfanteriefoudrepiqueSpells
{
    RAGE                                                    = 48138,
    TOURBILLON                                              = 55266,
    FRENZ                                                   = 48142,
    CHAINCRISTAL                                            = 47698,
    HURLEMENT                                               = 16838
};

/*################################################################
# Trash Vagues Orgrimmar PortOuestBaseWaves INGENIEUR_FOUDREPIQUE
################################################################*/

enum IngenieurfoudrepiqueSpells
{
    SUMMON_STOLEN_SOUL                                      = 100029,
    STOLEN_SOUL                                             = 32346,
    STOLEN_SOUL_VISUAL                                      = 32395,
    MOONFIRE                                                = 37328,
    FIREBALL                                                = 37329,
    MIND_FLAY                                               = 37330,
    HEMORRHAGE                                              = 37331,
    FROSTSHOCK                                              = 37332,
    CURSE_OF_AGONY                                          = 37334,
    MORTAL_STRIKE                                           = 37335,
    FREEZING_TRAP                                           = 37368,
    HAMMER_OF_JUSTICE                                       = 37369,
    EXPLOSION                                               = 44314,
    CRI                                                     = 19134,
    BOMBES                                                  = 46184,
    ROCKET                                                  = 46187
};


float OrgrimmarBaseWaves[ENCOUNTER_WAVES_NUMBER][ENCOUNTER_WAVES_MAX_SPAWNS][5]= // gestion du spawn des trash Entrée Hurlevent
{
	{
        {ARCHER_ARGENT,     1225.3365f, -4456.7524f, 24.1668f, 1.70f },
		{MAGE_ARGENT,       1227.1671f, -4456.4125f, 24.2319f, 1.70f },
		{INFANTERIE_ARGENT, 1228.3095f, -4456.2001f, 24.2850f, 1.70f },
		{MAGE_ARGENT,       1229.6103f, -4455.9584f, 24.3538f, 1.70f },
		{ARCHER_ARGENT,     1230.9523f, -4455.7094f, 24.4499f, 1.70f },
		{ARCHER_ARGENT,     1226.5943f, -4459.0024f, 24.2356f, 1.70f },
		{MAGE_ARGENT,       1227.8137f, -4458.7460f, 24.2609f, 1.70f },
		{INFANTERIE_ARGENT, 1228.9234f, -4458.5126f, 24.2836f, 1.70f },
		{MAGE_ARGENT,       1230.0195f, -4458.2827f, 24.3269f, 1.70f },
		{ARCHER_ARGENT,     1231.3894f, -4457.9941f, 42.3971f, 1.70f }
	},
	{
        {ARCHER_ARGENT,     1225.3365f, -4456.7524f, 24.1668f, 1.70f },
		{MAGE_ARGENT,       1227.1671f, -4456.4125f, 24.2319f, 1.70f },
		{INFANTERIE_ARGENT, 1228.3095f, -4456.2001f, 24.2850f, 1.70f },
		{MAGE_ARGENT,       1229.6103f, -4455.9584f, 24.3538f, 1.70f },
		{ARCHER_ARGENT,     1230.9523f, -4455.7094f, 24.4499f, 1.70f },
		{ARCHER_ARGENT,     1226.5943f, -4459.0024f, 24.2356f, 1.70f },
		{MAGE_ARGENT,       1227.8137f, -4458.7460f, 24.2609f, 1.70f },
		{INFANTERIE_ARGENT, 1228.9234f, -4458.5126f, 24.2836f, 1.70f },
		{MAGE_ARGENT,       1230.0195f, -4458.2827f, 24.3269f, 1.70f },
		{ARCHER_ARGENT,     1231.3894f, -4457.9941f, 42.3971f, 1.70f }
	},
	{
		{ARCHER_ARGENT,     1225.3365f, -4456.7524f, 24.1668f, 1.70f },
		{MAGE_ARGENT,       1227.1671f, -4456.4125f, 24.2319f, 1.70f },
		{INFANTERIE_ARGENT, 1228.3095f, -4456.2001f, 24.2850f, 1.70f },
		{MAGE_ARGENT,       1229.6103f, -4455.9584f, 24.3538f, 1.70f },
		{ARCHER_ARGENT,     1230.9523f, -4455.7094f, 24.4499f, 1.70f },
		{ARCHER_ARGENT,     1226.5943f, -4459.0024f, 24.2356f, 1.70f },
		{MAGE_ARGENT,       1227.8137f, -4458.7460f, 24.2609f, 1.70f },
		{INFANTERIE_ARGENT, 1228.9234f, -4458.5126f, 24.2836f, 1.70f },
		{MAGE_ARGENT,       1230.0195f, -4458.2827f, 24.3269f, 1.70f },
		{ARCHER_ARGENT,     1231.3894f, -4457.9941f, 42.3971f, 1.70f }
	},
	{
		{ARCHER_ARGENT,     1225.3365f, -4456.7524f, 24.1668f, 1.70f },
		{MAGE_ARGENT,       1227.1671f, -4456.4125f, 24.2319f, 1.70f },
		{INFANTERIE_ARGENT, 1228.3095f, -4456.2001f, 24.2850f, 1.70f },
		{MAGE_ARGENT,       1229.6103f, -4455.9584f, 24.3538f, 1.70f },
		{ARCHER_ARGENT,     1230.9523f, -4455.7094f, 24.4499f, 1.70f },
		{ARCHER_ARGENT,     1226.5943f, -4459.0024f, 24.2356f, 1.70f },
		{MAGE_ARGENT,       1227.8137f, -4458.7460f, 24.2609f, 1.70f },
		{INFANTERIE_ARGENT, 1228.9234f, -4458.5126f, 24.2836f, 1.70f },
		{MAGE_ARGENT,       1230.0195f, -4458.2827f, 24.3269f, 1.70f },
		{ARCHER_ARGENT,     1231.3894f, -4457.9941f, 42.3971f, 1.70f }
	},
	{
		{ARCHER_ARGENT,     1225.3365f, -4456.7524f, 24.1668f, 1.70f },
		{MAGE_ARGENT,       1227.1671f, -4456.4125f, 24.2319f, 1.70f },
		{INFANTERIE_ARGENT, 1228.3095f, -4456.2001f, 24.2850f, 1.70f },
		{MAGE_ARGENT,       1229.6103f, -4455.9584f, 24.3538f, 1.70f },
		{ARCHER_ARGENT,     1230.9523f, -4455.7094f, 24.4499f, 1.70f },
		{ARCHER_ARGENT,     1226.5943f, -4459.0024f, 24.2356f, 1.70f },
		{MAGE_ARGENT,       1227.8137f, -4458.7460f, 24.2609f, 1.70f },
		{INFANTERIE_ARGENT, 1228.9234f, -4458.5126f, 24.2836f, 1.70f },
		{MAGE_ARGENT,       1230.0195f, -4458.2827f, 24.3269f, 1.70f },
		{ARCHER_ARGENT,     1231.3894f, -4457.9941f, 42.3971f, 1.70f }
	},
	{
		{ARCHER_ARGENT,     1225.3365f, -4456.7524f, 24.1668f, 1.70f },
		{MAGE_ARGENT,       1227.1671f, -4456.4125f, 24.2319f, 1.70f },
		{INFANTERIE_ARGENT, 1228.3095f, -4456.2001f, 24.2850f, 1.70f },
		{MAGE_ARGENT,       1229.6103f, -4455.9584f, 24.3538f, 1.70f },
		{ARCHER_ARGENT,     1230.9523f, -4455.7094f, 24.4499f, 1.70f },
		{ARCHER_ARGENT,     1226.5943f, -4459.0024f, 24.2356f, 1.70f },
		{MAGE_ARGENT,       1227.8137f, -4458.7460f, 24.2609f, 1.70f },
		{INFANTERIE_ARGENT, 1228.9234f, -4458.5126f, 24.2836f, 1.70f },
		{MAGE_ARGENT,       1230.0195f, -4458.2827f, 24.3269f, 1.70f },
		{ARCHER_ARGENT,     1231.3894f, -4457.9941f, 42.3971f, 1.70f }
	},
	{
		{ARCHER_ARGENT,     1225.3365f, -4456.7524f, 24.1668f, 1.70f },
		{MAGE_ARGENT,       1227.1671f, -4456.4125f, 24.2319f, 1.70f },
		{INFANTERIE_ARGENT, 1228.3095f, -4456.2001f, 24.2850f, 1.70f },
		{MAGE_ARGENT,       1229.6103f, -4455.9584f, 24.3538f, 1.70f },
		{ARCHER_ARGENT,     1230.9523f, -4455.7094f, 24.4499f, 1.70f },
		{ARCHER_ARGENT,     1226.5943f, -4459.0024f, 24.2356f, 1.70f },
		{MAGE_ARGENT,       1227.8137f, -4458.7460f, 24.2609f, 1.70f },
		{INFANTERIE_ARGENT, 1228.9234f, -4458.5126f, 24.2836f, 1.70f },
		{MAGE_ARGENT,       1230.0195f, -4458.2827f, 24.3269f, 1.70f },
		{ARCHER_ARGENT,     1231.3894f, -4457.9941f, 42.3971f, 1.70f }
	},
	{
		{ARCHER_ARGENT,     1225.3365f, -4456.7524f, 24.1668f, 1.70f },
		{MAGE_ARGENT,       1227.1671f, -4456.4125f, 24.2319f, 1.70f },
		{INFANTERIE_ARGENT, 1228.3095f, -4456.2001f, 24.2850f, 1.70f },
		{MAGE_ARGENT,       1229.6103f, -4455.9584f, 24.3538f, 1.70f },
		{ARCHER_ARGENT,     1230.9523f, -4455.7094f, 24.4499f, 1.70f },
		{ARCHER_ARGENT,     1226.5943f, -4459.0024f, 24.2356f, 1.70f },
		{MAGE_ARGENT,       1227.8137f, -4458.7460f, 24.2609f, 1.70f },
		{INFANTERIE_ARGENT, 1228.9234f, -4458.5126f, 24.2836f, 1.70f },
		{MAGE_ARGENT,       1230.0195f, -4458.2827f, 24.3269f, 1.70f },
		{ARCHER_ARGENT,     1231.3894f, -4457.9941f, 42.3971f, 1.70f }
	},
    {
		{SHAMAN,            1225.3365f, -4456.7524f, 24.1668f, 1.70f },
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

float OuestBaseWaves[ENCOUNTER_WAVES_NUMBER][ENCOUNTER_WAVES_MAX_SPAWNS][5]= // gestion du spawn des trash Port de Hurlevent
{
	{
        {INGENIEUR_FOUDREPIQUE,  1535.9388f, -3850.1210f, 38.4015f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.8411f, -3851.3984f, 38.2936f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1535.7525f, -3852.5573f, 38.1905f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.6815f, -3853.4855f, 38.1368f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1535.5833f, -3854.7697f, 38.1470f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.2680f, -3854.5925f, 37.9370f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.3486f, -3853.5385f, 37.9009f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1533.4201f, -3852.6032f, 37.9450f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.5002f, -3851.5563f, 37.9931f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.5893f, -3850.3908f, 38.0475f, 6.20f }
	},
    {
        {INGENIEUR_FOUDREPIQUE,  1535.9388f, -3850.1210f, 38.4015f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.8411f, -3851.3984f, 38.2936f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1535.7525f, -3852.5573f, 38.1905f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.6815f, -3853.4855f, 38.1368f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1535.5833f, -3854.7697f, 38.1470f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.2680f, -3854.5925f, 37.9370f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.3486f, -3853.5385f, 37.9009f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1533.4201f, -3852.6032f, 37.9450f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.5002f, -3851.5563f, 37.9931f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.5893f, -3850.3908f, 38.0475f, 6.20f }
	},
	{
        {INGENIEUR_FOUDREPIQUE,  1535.9388f, -3850.1210f, 38.4015f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.8411f, -3851.3984f, 38.2936f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1535.7525f, -3852.5573f, 38.1905f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.6815f, -3853.4855f, 38.1368f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1535.5833f, -3854.7697f, 38.1470f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.2680f, -3854.5925f, 37.9370f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.3486f, -3853.5385f, 37.9009f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1533.4201f, -3852.6032f, 37.9450f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.5002f, -3851.5563f, 37.9931f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.5893f, -3850.3908f, 38.0475f, 6.20f }
	},
	{
        {INGENIEUR_FOUDREPIQUE,  1535.9388f, -3850.1210f, 38.4015f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.8411f, -3851.3984f, 38.2936f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1535.7525f, -3852.5573f, 38.1905f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.6815f, -3853.4855f, 38.1368f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1535.5833f, -3854.7697f, 38.1470f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.2680f, -3854.5925f, 37.9370f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.3486f, -3853.5385f, 37.9009f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1533.4201f, -3852.6032f, 37.9450f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.5002f, -3851.5563f, 37.9931f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.5893f, -3850.3908f, 38.0475f, 6.20f }
	},
	{
        {INGENIEUR_FOUDREPIQUE,  1535.9388f, -3850.1210f, 38.4015f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.8411f, -3851.3984f, 38.2936f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1535.7525f, -3852.5573f, 38.1905f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.6815f, -3853.4855f, 38.1368f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1535.5833f, -3854.7697f, 38.1470f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.2680f, -3854.5925f, 37.9370f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.3486f, -3853.5385f, 37.9009f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1533.4201f, -3852.6032f, 37.9450f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.5002f, -3851.5563f, 37.9931f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.5893f, -3850.3908f, 38.0475f, 6.20f }
	},
	{
        {INGENIEUR_FOUDREPIQUE,  1535.9388f, -3850.1210f, 38.4015f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.8411f, -3851.3984f, 38.2936f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1535.7525f, -3852.5573f, 38.1905f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.6815f, -3853.4855f, 38.1368f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1535.5833f, -3854.7697f, 38.1470f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.2680f, -3854.5925f, 37.9370f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.3486f, -3853.5385f, 37.9009f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1533.4201f, -3852.6032f, 37.9450f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.5002f, -3851.5563f, 37.9931f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.5893f, -3850.3908f, 38.0475f, 6.20f }
	}, 
	{
        {INGENIEUR_FOUDREPIQUE,  1535.9388f, -3850.1210f, 38.4015f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.8411f, -3851.3984f, 38.2936f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1535.7525f, -3852.5573f, 38.1905f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.6815f, -3853.4855f, 38.1368f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1535.5833f, -3854.7697f, 38.1470f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.2680f, -3854.5925f, 37.9370f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.3486f, -3853.5385f, 37.9009f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1533.4201f, -3852.6032f, 37.9450f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.5002f, -3851.5563f, 37.9931f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.5893f, -3850.3908f, 38.0475f, 6.20f }
	},
	{
        {INGENIEUR_FOUDREPIQUE,  1535.9388f, -3850.1210f, 38.4015f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.8411f, -3851.3984f, 38.2936f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1535.7525f, -3852.5573f, 38.1905f, 6.20f },
		{MAGE_FOUDREPIQUE,       1535.6815f, -3853.4855f, 38.1368f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1535.5833f, -3854.7697f, 38.1470f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.2680f, -3854.5925f, 37.9370f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.3486f, -3853.5385f, 37.9009f, 6.20f },
		{INFANTERIE_FOUDREPIQUE, 1533.4201f, -3852.6032f, 37.9450f, 6.20f },
		{MAGE_FOUDREPIQUE,       1533.5002f, -3851.5563f, 37.9931f, 6.20f },
		{INGENIEUR_FOUDREPIQUE,  1533.5893f, -3850.3908f, 38.0475f, 6.20f }
	},
    {
		{GENERAL,                1535.9388f, -3850.1210f, 38.4015f, 6.20f },
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





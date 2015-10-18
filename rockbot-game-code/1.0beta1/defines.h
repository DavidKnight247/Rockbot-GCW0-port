#ifndef DEFINES_H
#define DEFINES_H


#define RES_W 320
#define RES_H 200

#define UNUSED(x) (void)x

#define PLAYER_ROCKBOT 1
#define PLAYER_BETABOT 2

// stages
/// @TODO: will be recplaced by file_game
enum STAGE_LIST {
	INTRO_STAGE, APEBOT, DAISIEBOT, SEAHORSEBOT, MUMMYBOT, SKULLCASTLE1, MAGEBOT, DYNAMITEBOT, SPIKEBOT, TECHNOBOT, SKULLCASTLE2, SKULLCASTLE3, SKULLCASTLE4, STAGE_COUNT
};


enum COLISION_TYPES { BLOCK_UNBLOCKED, BLOCK_X, BLOCK_Y, BLOCK_XY, BLOCK_STAIR_X, BLOCK_STAIR_Y, BLOCK_WATER };

enum JUMP_STATES { NO_JUMP, JUMP_UP, JUMP_DOWN };

#define TIME_MOVES 15

enum INPUT_COMMANDS { BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_JUMP, BTN_ATTACK, BTN_QUIT, BTN_START, BTN_L, BTN_R, BTN_DASH, BTN_SHIELD, BTN_JUMP_TIMER, BTN_COUNT };

enum ANIM_DIRECTION { ANIM_DIRECTION_LEFT, ANIM_DIRECTION_RIGHT, ANIM_DIRECTION_UP, ANIM_DIRECTION_DOWN, ANIM_DIRECTION_COUNT };
enum ANIM_TYPE { ANIM_TYPE_STAND,
				 ANIM_TYPE_WALK,
				 ANIM_TYPE_JUMP,
				 ANIM_TYPE_ATTACK,
				 ANIM_TYPE_TELEPORT,
				 ANIM_TYPE_JUMP_ATTACK,
				 ANIM_TYPE_WALK_ATTACK,
				 ANIM_TYPE_HIT,
				 ANIM_TYPE_STAIRS,
				 ANIM_TYPE_STAIRS_SEMI,
				 ANIM_TYPE_STAIRS_ATTACK,
				 ANIM_TYPE_STAIRS_MOVE,
				 ANIM_TYPE_TROW,
				 ANIM_TYPE_TURN,
				 ANIM_TYPE_MOVE_SEMI, // like turn, the start of the movement
				 ANIM_TYPE_WALK_AIR, // for those that can walk on air
				 ANIM_TYPE_INTRO, // for bosses
                 ANIM_TYPE_SPECIAL_ATTACK, // for bosses, charging weapon, etc
                 ANIM_TYPE_SLIDE,
				 ANIM_TYPE_SHIELD,
				 ANIM_TYPE_COUNT };
#define ANIM_FRAMES_COUNT 10 // max number of animation frames for each animation_type

#define MAP_W 200 // this size was determined by looking at common maps from games
#define MAP_H 13

#define TILESIZE 16

// for pre-loading


enum INTRO_SURFACES_ENUM {
	INTRO_SURFACES_UPPERLAND_PRESENTS, //presents.png
	INTRO_SURFACES_CITY_BG, //scenes/city_intro1.png
	INTRO_SURFACES_KANOTUS, //sprites/canotus.png
	INTRO_SURFACES_ROCKBOT, //sprites/p1.png
	INTRO_SURFACES_BETABOT, //sprites/p2.png
	INTRO_SURFACES_LAB_BG, //scenes/lab_intro1.png
	INTRO_SURFACES_INTRO_SCREEN, //logo.png
	INTRO_SURFACES_PLAYER_SELECT_BG, //backgrounds/player_select.png
	INTRO_SURFACES_PLAYER_SELECT_LIGHTS, //backgrounds/lights.png
	INTRO_SURFACES_CAPSULES, //backgrounds/capsules.png
	INTRO_SURFACES_ROCKBOT_BIG, //scenes/rockbot.png
	INTRO_SURFACES_ROCKBOT_BIG_HALF_SLEEP, //scenes/rockbot_half_sleep.png
	INTRO_SURFACES_ROCKBOT_BIG_FULL_SLEEP, //scenes/rockbot_full_sleep.png
	INTRO_SURFACES_BETABOT_BIG, //scenes/betabot.png
	INTRO_SURFACES_KANOTUS_BIG, //scenes/kanotus.png

	/// Last line
	INTRO_SURFACES_COUNT
};

enum STAGE_SELECT_ENUM {
	INTRO_SURFACES_STAGE_SELECT_BG,
	STAGE_SELECT_COUNT
};

// sound
enum SFX_LIST {
	SFX_NPC_HIT,
	SFX_NPC_KILLED,
	SFX_PLAYER_HIT,
	SFX_PLAYER_SHOT,
	SFX_PLAYER_JUMP,
	SFX_CURSOR,
	SFX_STAGE_SELECTED,
	SFX_GOT_ENERGY,
	SFX_GOT_ITEM,
	SFX_SHOT_REFLECTED,
	SFX_DOOR_OPEN,
	SFX_GOT_WEAPON,
	SFX_TELEPORT,
	SFX_IMPLOSION,
	SFX_PLAYER_DEATH,
	SFX_DESTRIN_SHIP,
	SFX_TRAIN_CHUNK,
	SFX_SKULL_CASTLE_INTRO,
	SFX_PLAYER_CHARGED_SHOT,
	SFX_CHARGING1,
	SFX_CHARGING2,
	SFX_BIG_EXPLOSION,
	SFX_COUNT // not used as sfx, this is a way to measure size of the enum list
};

enum TERREAIN_TYPES {
	TERRAIN_UNBLOCKED, TERRAIN_SOLID, TERRAIN_STAIR, TERRAIN_DOOR, TERRAIN_SPIKE, TERRAIN_WATER, TERRAIN_ICE, TERRAIN_MOVE_LEFT, TERRAIN_MOVE_RIGHT, TERRAIN_SAND,
	TERRAIN_SCROLL_LOCK, TERRAIN_CHECKPOINT, TERRAIN_TYPES_COUNT
};


#define CURSOR_SPACING 12

#define NPC_FRAME_NUMBER 50


enum ITEM_ICONS_ENUM {
	ENERGY_TANK,
	P1_1LIFE,
	P2_1LIFE,
	WEAPONS_TANK
};

enum WEAPON_ICONS_ENUM {
	WEAPON_DEFAULT, WEAPON_APEBOT, WEAPON_DAISIEBOT, WEAPON_DYNAMITEBOT, WEAPON_MUMMYBOT, WEAPON_SPIKEBOT, WEAPON_TECHNOBOT, WEAPON_MAGEBOT, WEAPON_SEAHORSEBOT, WEAPON_ITEM_COIL, WEAPON_ITEM_JET,
	WEAPON_COUNT
};

enum transition_types {
	NO_TRANSITION,
	TRANSITION_TOP_TO_BOTTOM,
	TRANSITION_BOTTOM_TO_TOP,
	TRANSITION_LEFT_TO_RIGHT,
	TRANSITION_RIGHT_TO_LEFT
};

enum OBJECT_TYPE {
	OBJ_MOVING_PLATFORM_UPDOWN,
	OBJ_MOVING_PLATFORM_LEFTRIGHT,
	OBJ_DISAPPEARING_BLOCK,
	OBJ_ENERGY_TANK,
	OBJ_WEAPON_TANK,
	OBJ_ENERGY_PILL_BIG,
	OBJ_WEAPON_PILL_BIG,
	OBJ_ENERGY_PILL_SMALL,
	OBJ_WEAPON_PILL_SMALL,
	OBJ_LIFE,
	OBJ_FALL_PLATFORM,
	OBJ_FLY_PLATFORM,
	OBJ_ITEM_FLY,
	OBJ_ITEM_JUMP,
	OBJ_TYPE_COUNT
};

enum OBJ_STATE {
	OBJ_STATE_STAND,
	OBJ_STATE_MOVE,
	OBJ_STATE_RETURN,
	OBJ_STATE_COUNT
};

enum LINK_TYPES { LINK_HORIZONTAL, LINK_VERTICAL, LINK_TELEPORTER};

#define DOOR_MOVE_STEP 2

#define TRANSITION_STEP 3

#define PLAYER_INITIAL_HP 28

#define HIT_BLINK_ANIMATION_LAPSE 100

#define WATER_SPEED_MULT 0.75

#define ATTACK_DELAY 300 // how many milisseconds the attack frame must last before returning to stand/jump/walk

enum PROJECTILE_TRAJECTORIES {
	TRAJECTORY_LINEAR,
	TRAJECTORY_ARC,
	TRAJECTORY_SIN,
	TRAJECTORY_CHAIN,
	TRAJECTORY_QUAKE,
	TRAJECTORY_BOMB,
	TRAJECTORY_FOLLOW,
	TRAJECTORY_PROGRESSIVE,
	TRAJECTORY_FREEZE,
	TRAJECTORY_DIAGONAL_UP,
	TRAJECTORY_DIAGONAL_DOWN,
	PROJECTILE_TRAJECTORIES_COUNT };

enum EDITOR_MODES {
	EDITMODE_NORMAL,
	EDITMODE_LOCK,
	EDITMODE_ERASER,
	EDITMODE_FILL,
	EDITMODE_LINK,
	EDITMODE_LINK_DEST,
	EDITMODE_NPC,
	EDITMODE_ADDNPC,
	EDITMODE_STAIRS,
    EDITMODE_OBJECT,
    EDITMODE_SET_BOSS
};

enum IA_TYPE_LIST { IA_STAND, IA_WAIT, IA_FOLLOW, IA_ZIGZAG, IA_SIDETOSIDE, IA_BAT, IA_ROOF_SHOOTER, IA_GROUND_SHOOTER, IA_SHOOT_AND_GO, IA_FLY_ZIG_ZAG, IA_BUTTERFLY, IA_GO_LEFT, IA_GO_RIGHT, IA_FIXED_JUMPER, IA_SIDE_SHOOTER, IA_GHOST, IA_FISH, IA_DOLPHIN, IA_TYPES_COUNT };


enum IA_STATES {
		IA_STATE_SLEEP,
		IA_STATE_FOLLOW,
		IA_STATE_RETURN,
		IA_STATE_SHOT_RAINBOW,
		IA_STATE_CROSS_SCREEN,
		IA_STATE_SHOT_FLOWER,
		IA_STATE_WAIT_TIME,
		IA_STATE_START_MOVE,
		IA_STATE_START_STAND,
		IA_STATE_INTRO,
		IA_STATE_JUMP_TO_ROOF,
		IA_STATE_JUMP_TO_PLAYER,
		IA_STATE_AIR_WALK,
		IA_STATE_QUAKE_ATTACK,
		IA_STATE_JUMP_FALL,
		IA_STATE_GROUND_ATTACK
};

enum IA_CONDITIONALS {
	IA_CONDITION_NO_CONDITION,
	IA_CONDITION_NEAR_PLAYER,
	IA_CONDITION_ABOUT_TO_DIE,
	IA_CONDITION_ABOVE_PLAYER,
	IA_CONDITION_UNDER_PLAYER
};

#define AI_MAX_STATES 12

enum e_shield_types { SHIELD_NO, SHIELD_FULL, SHIELD_FRONT, SHIELD_STAND };

enum e_energy_types { ENERGY_TYPE_HP, ENERGY_TYPE_WEAPON };

#define MAX_PLAYER_SPRITES 23
#define MAX_NPC_SPRITES 15
#define MAX_WEAPON_N 12
#define MAX_STAGES 16
#define MAX_MAP_NPC_N 30
#define GAME_MAX_OBJS 60
#define STAGE_MAX_LINKS 30
#define STAGE_MAX_MAPS 3
#define MAX_AI_TYPES 30

#define CHAR_NAME_SIZE 30
#define CHAR_FILENAME_SIZE 30

#define MAX_FILE_PLAYERS 5

#define MAX_FILE_PROJECTILES 20
#define DIALOG_LINE_LIMIT 30
#define MAX_PLAYERS 3

#ifdef PLAYSTATION2
	#define JOYVAL 12000
#else
	#define JOYVAL 30000
#endif

#ifndef PLAYSTATION
	#define JOY_TRIANGLE 0
	#define JOY_CIRCLE 1
	#define JOY_X 2
	#define JOY_SQUARE 3
	#define JOY_L2 4
	#define JOY_R2 5
	#define JOY_L1 6
	#define JOY_R1 7
	#define JOY_SELECT 8
	#define JOY_START 9
	#define JOY_L3 10
	#define JOY_R3 11
#else
	#define JOY_TRIANGLE 3
	#define JOY_CIRCLE 2
	#define JOY_X 1
	#define JOY_SQUARE 0
	#define JOY_L2 7
	#define JOY_R2 9
	#define JOY_L1 6
	#define JOY_R1 8
	#define JOY_SELECT 4
	#define JOY_START 5
	#define JOY_L3 10
	#define JOY_R3 11
#endif


#define ENERGY_ITEM_SMALL 3
#define ENERGY_ITEM_BIG 8

#define MOVING_GROUND 10 //TREADMILL


#endif // DEFINES_H

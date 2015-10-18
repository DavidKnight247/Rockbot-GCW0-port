#ifndef TYPES_H
#define TYPES_H

#define PROJECT_NAME "Rockbot 0.1 RC 1"

#include "format/file_structs.h"
#include "shared_vars.h"
#include <SDL/SDL_mixer.h>

short int scrollX=0, scrollY=0;

int currentGame=0;
int currentMap=0;
int currentStage=0;

#define PROJECTILE_SIZE 6
#define HIT_INVENCIBLE_TIME 1600
#define NPC_HIT_INVENCIBLE_TIME 500
#define INITIAL_HP 28

// declaração de variáveis
// constantes globais
// largura, altura e profundidade de cores

//Nintendo DS
#define MAX_W 320
#define MAX_H 200
#define RES_D 16
#define TILE_SIZE 16
#define PLAYER_H 24

#define ANIM_LEFT 0
#define ANIM_RIGHT 1
#define ANIM_LEFT_DIAGONAL 2
#define ANIM_RIGHT_DIAGONAL 3
#define ANIM_DOWN 5
#define ANIM_UP 6


enum ANIM_STATES { ANIM_STAND, ANIM_WALK, ANIM_JUMP, ANIM_ATTACK, ANIM_TELEPORT, ANIM_JUMP_ATTACK, ANIM_WALK_ATTACK,
				   ANIM_HIT, ANIM_STAIRS, ANIM_STAIRS_SEMI, ANIM_STAIRS_ATTACK, ANIM_STAIRS_MOVE, ANIM_TROW, ANIM_TURN,
				   ANIM_MOVE_SEMI, // like turn, the start of the movement
				   ANIM_WALK_AIR, // for those that can walk on air
				   ANIM_INTRO, // for bosses
				   ANIM_SPECIAL_ATTACK, // for bosses, charging weapon, etc
				   ANIM_STATES_N }; // this must be always the last one to be used as counter


#define ANIM_N1 2
#define ANIM_N2 ANIM_STATES_N
#define ANIM_N3 10

#define ANIM_STATUS 0
#define ANIM_PROCESS 1

int MOVE_LEFT[2] = {0,0};
int MOVE_RIGHT[2] = {0,0};
int MOVE_UP[2] = {0,0};
int MOVE_DOWN[2] = {0,0};

int RES_DIFF_W;
int RES_DIFF_H;

// quantos pixels os sprites movem-se por vez - o problema é que depende da cpu isso

#define MOVE_STEP_WATER 3
enum COLISION_TYPES { UNBLOCKED, BLOCK_X, BLOCK_Y, BLOCK_XY, BLOCK_STAIR_X, BLOCK_STAIR_Y, BLOCK_WATER };

#define RES_W 320
#define RES_H 200

int centerX, centerY; // used to center the image on screen

#define JUMP_SPEED_STEPS 3



#define MAX_JUMP_SPEED 5
#define MOVE_STEP 2
#define NPC_STEP 2
#define PROJECTILE_SPEED 6
#define BLINK_SPEED 6
#define ANIM_STARS_STEP 4
#define ANIM_STARS_SMALL_STEP 2
#define INTRO_STARS_NUMBER 30
#define TRANSITION_STEP 3

// used in stars
SDL_Surface *surface_star_big=NULL, *surface_star_bg_big=NULL, *surface_star_small=NULL, *surface_star_bg_small=NULL;

// como as posições das sprites é um array, precisamos definir que a posição zero é o sprite do player

#define TRANSITION_TOP_TO_BOTTOM 1
#define TRANSITION_BOTTOM_TO_TOP 2
#define TRANSITION_LEFT_TO_RIGHT 3
#define TRANSITION_RIGHT_TO_LEFT 4



#define ITEM_COIL SKULLCASTLE2
#define ITEM_JET SKULLCASTLE3
#define WEAPON_COUNT SKULLCASTLE4
// variáveis globais
SDL_Surface *offscreen = NULL; //Superfície auxiliar para apagar a tela.



SDL_Surface *tileset;

// arquivo de mapa
FILE *fp_map;

struct anim_state {
	short int move_type;
	short int move_status[ANIM_N3];
	short int move_frame[ANIM_N3];
};

struct anim_frame {
	SDL_Surface *frame;
	unsigned short int duration;
	unsigned int init;
};



struct sprite_struct {
	short int x;
	short int y;
	short int w;
	short int h;
	short int frame_pos[2];							// indicates what is the animation type and frame position
	short int frame_timer;							// timer is used to control the animation speed
	unsigned long int frame_animation[2];			// controls where in the animation process the sprite is.
													// First part indicates the status and second the position
													// (for example in jump status 0 is nothing, 1 is going up and position raises until reaches limit, then status changes to 2)
	// new animation variables
	short int direction;
	short int anim_type;
	short int anim_pos;
	long int move_timer;
	struct anim_state anim_status;
	struct anim_frame sprite_frames[ANIM_N1][ANIM_N2][ANIM_N3];
	short int visible;
};

#define COLOR_COUNT 67


SDL_Color colormap[COLOR_COUNT] = {
	{97,   97,  97},
	{39,   27, 143},
	{0,     0, 171},
	{71,    0, 159},
	{143,   0, 119},
	{171,   0,  19},
	{167,   0,   0},
	{127,  11,   0},
	{ 67,  47,   0},
	{  0,  71,   0},
	{  0,  81,   0},
	{  0,  63,  23},
	{ 27,  63,  95},
	{  0,   0,   0},
	{196,  93,   0},
	{255, 153, 102},
	{188, 188, 188},
	{  0, 115, 239},
	{ 35,  59, 239},
	{131,   0, 243},
	{191,   0, 191},
	{231,   0,  91},
	{219,  43,   0},
	{203,  79,  15},
	{139, 115,   0},
	{  0, 151,   0},
	{  0, 171,   0},
	{  0, 147,  59},
	{  0, 131, 139},
	{255, 102,   0},
	{255,  51,   0},
	{204, 204,   0},
	{235, 235, 235},
	{ 63, 191, 255},
	{ 95, 151, 255},
	{167, 139, 253},
	{247, 123, 255},
	{255, 119, 183},
	{255, 119,  99},
	{255, 155,  59},
	{243, 191,  63},
	{131, 211,  19},
	{ 79, 223,  75},
	{ 88, 248, 152},
	{  0, 235, 219},
	{102,  51, 204},
	{ 51, 102, 255},
	{ 81,  81,  81},
	{230, 255, 0},
	{171, 231, 255},
	{199, 215, 255},
	{215, 203, 255},
	{255, 199, 255},
	{255, 199, 219},
	{255, 191, 179},
	{255, 219, 171},
	{255, 231, 163},
	{227, 255, 163},
	{171, 243, 191},
	{179, 255, 207},
	{159, 255, 243},
	{110, 110, 110},
	{255, 192,   0},
	{255, 234,   0},
	{ 55, 255,   0},
	{ 255,  0, 255},
	{ 0,  255, 255}
};

SDL_Color colormap_original[COLOR_COUNT];

// posicionamento, etc de sprites
struct player_struct *player_list = NULL;



void play_sfx(int);

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
	SFX_COUNT // not used as sfx, this is a way to measure size of the enum list
};

	Mix_Chunk *sfx_list[SFX_COUNT];

	// posição do mapa na tela (scrolling)
	short int map_pos_x = 0;
	short int map_pos_y = 0;


#include "player_and_npc.h"


// game
struct file_game game;
struct file_map_v1 map;
struct file_map_v1 temp_map;

struct file_map_v1 stage_maps[3];

struct game_config_struct {
	short int two_players;
	short int sound_enabled;
	short int video_fullscreen;
	short int video_filter;
	short int selected_player;
};
struct game_config_struct game_config;






// ----------------------------------------------- ANIMATION ----------------------------------------------------- //

struct animation_frame {
        unsigned int duration;
	short int w;
	short int h;
	SDL_Surface *tile;
};

struct animation_sprite_struct {
	short int *x;
	short int *y;
	short int w;
	short int h;
	short int diffx;
	short int diffy;
	short int animation_pos;
	short int animation_loops;
	short int animation_loop_count;
	unsigned int init;
	struct animation_frame *frame;
	struct animation_sprite_struct *next;
};

// this is used to show a single sprite on the screen
struct simple_sprite {
	short int x;
	short int y;
	SDL_Surface *sprite;
};

struct simple_sprite *p1_warning;
struct simple_sprite *p2_warning;

struct animation_sprite_struct *animation_list=NULL;
struct animation_frame *hit_frame;
struct animation_frame *ready_frame;
struct animation_frame *kill_frame;



Uint8 *keyRead; //Estado do teclado
short int fim = 0;



// número total de teclas possíveis
#define KEYNUMBER 20

#include "headers.h"

struct struct_colorcycle {
	short int color_n;
	short int r;
	short int g;
	short int b;
        unsigned int duration;
	long int timer_ini;
	SDL_Color color;
	struct struct_colorcycle *next;
};

struct struct_colorcycle_list {
	struct struct_colorcycle *colorcycle;
	struct struct_colorcycle *colorcycle_current;
	short int map_color;
	struct struct_colorcycle_list *next;
};



struct struct_colorcycle_list *colorcycle_list = NULL;

int colorcycle_n;

SDL_Surface *normal_projectile = NULL;
SDL_Surface *special_projectile[10]; // extra position because zero is not used

enum projectile_types { PROJECTLE_TYPE_REGULAR, PROJECTLE_TYPE_DIAGONAL };


struct projectile {
	short int direction; // -1 for free, ANIM_LEFT or ANIM_RIGHT
	short int x;
	short int y;
	short int x0;
	short int y0;
	short int y1;
	short int map_x;
	short int map_y;
	short int owner; // PROJECTILE_PLAYER or PROJECTILE_NPC
	struct player_struct *owner_player;
	struct npc_struct *owner_npc;
	short int speed; // projectile moved pixels each cycle
	SDL_Surface *tile; // the graphic picture
	struct projectile *next;
	short int type; // type of weapon
	short int trajectory; // type of trajectory
	short int status; // status of animation (going/returning, etc)
	short int vdirection;
};



#define SIMULTANEOUS_PROJECTILES 3
//#define MAX_PROJECTILES 10
struct projectile *projectile_list;

// used to check if the wall is locked or not for each "level" of the map
int wall_scroll_lock[MAP_W];

short int PAUSED;

struct player_struct *p1Obj, *p2Obj;


short int PLATFORM_COUNTER = 0; // used to move player or control platform timer

#define _MIXER_THREAD_PRIORITY 42

enum ERROR_LEVEL { WARNING, CRITICAL };

int stage_start = 1; // used to identify when a stage just was loaded (so we can show a message first time player touches the gound, etc)

#define DIALOG_MAX_STAGE 20
#define DIALOG_MAX_PLACES 2
#define DIALOG_MAX_PARTS 4

struct struct_dialog {
	int face_n;
	char name[20];
	char phrase[3][23];
};

int STAGE_N = 0;

// n1 is the stage number (0 intro), n2 is the start stage or boss dialog, n4 is the phrase (max of 4)
struct struct_dialog dialog_list[DIALOG_MAX_STAGE][DIALOG_MAX_PLACES][DIALOG_MAX_PARTS];

// FOR SAVE
struct st_items {
	short int energy_tanks;
	short int weapon_tanks;
	short int lifes;
};

struct st_save {
	short int finished_stages; // number of already finished stages, just to simplify calc
	short int stages[DIALOG_MAX_STAGE]; // 0 still open, 1 finished
	struct st_items items;
};

struct st_save save_001;





// FOR SCALE 2X
#ifndef MAX
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif

#define READINT24(x)      ((x)[0]<<16 | (x)[1]<<8 | (x)[2])
#define WRITEINT24(x, i)  {(x)[0]=i>>16; (x)[1]=(i>>8)&0xff; x[2]=i&0xff; }







struct st_checkpoint {
    int map;
    int x;
	int y;
    int map_pos_x;
};
struct st_checkpoint checkpoint;


struct boss_room {
	int teleporter_n; // number of the map_link (this is hardcoded, must be rebuilt in 0.2)
	int finished; // indicates if already used
	int old_map_pos_x;
	struct pos old_player_pos;
	int active; // if true, then after beating the boss, the player must be teleported back to the teleporter origin
};

struct text_list {
	char text[33];
	int posY;
	struct text_list *next;
};
struct text_list *credits_list=NULL;

struct boss_room skull_castle_bosses[8];

int jump_locked=0; // indicates that players can't jump because of an earthqauke or something like that

int weapon_matrix[2][5];

struct links_list *map_links = NULL;


#endif // TYPES_H


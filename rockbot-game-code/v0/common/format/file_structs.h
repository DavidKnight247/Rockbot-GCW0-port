#ifndef FILE_STRUCTS_H
#define FILE_STRUCTS_H

#include <SDL/SDL.h>


// ----------------------------------- GAME ------------------------------------------ //

#define LINKS_NUMBER 300

enum LINK_TYPES { LINK_HORIZONTAL, LINK_VERTICAL, LINK_TELEPORTER};

// estrutura posicao
struct pos {
	short int x;
	short int y;
};


struct hit_points {
	short int total;
	short int current;
};

struct map_link {
	struct pos link_pos;				// position in map of origin
	struct pos link_dest;				// position in map of destiny
	short int map_origin;			// number of origin map
	short int map_dest;			// number of destiny map
	unsigned short int link_size;			// size of it, in tiles
	unsigned short int link_type;			// can be TELEPORTER, LINK_VERTICAL OR LINK_HORIZONTAL
	unsigned short int link_bidi;			// indicates if it is bidirecional
	unsigned short int is_door;				// if it is a door, it is not bidiand willmake door sound and show animation of door opening/closing
};


struct struct_stage {
	char boss_name[50];
	char boss_face_n;
	char boss_sprites_filename[50];
	char bgMusic[100];
	struct pos initial_point;
	unsigned short int initial_map_number;
	struct pos zone_point;
};


struct struct_zone {
	char bg_filename[100];					// background image filename
	struct struct_stage stages[8];
};


struct file_game {
	char name[50];
	struct map_link map_links[LINKS_NUMBER];// links from the map to another one (two for each map is more than enought)
	struct struct_zone zones[3];			// stage select, castle 1 and castle 2
};


struct map_link_v1 {
	short int stage;
	struct pos link_pos;				// position in map of origin
	struct pos link_dest;				// position in map of destiny
	short int map_origin;			// number of origin map
	short int map_dest;			// number of destiny map
	unsigned short int link_size;			// size of it, in tiles
	unsigned short int link_type;			// can be TELEPORTER, LINK_VERTICAL OR LINK_HORIZONTAL
	unsigned short int link_bidi;			// indicates if it is bidirecional
	unsigned short int is_door;				// if it is a door, it is not bidiand willmake door sound and show animation of door opening/closing
};



struct links_list {
	struct map_link_v1 link;
	struct links_list *next;
};


// ---------------------------------------------- NPC ------------------------------------------------------ //

#define NPC_FACE_LEFT 0
#define NPC_FACE_RIGHT 1

#define NPC_FRAME_NUMBER 50


struct file_npc_old {
	char name[50];
	struct hit_points hp;
	short int direction;              // defines how it behavies (kink of AI*)
	short int speed;               // defines the distances it can see enemies
	short int walk_range;                 // defines how long from the start point it can go
	short int facing;                     // defines the side npc is facing before start moving (also used by LINEWALK behavior)
	struct pos start_point;
	char filename[50];						// graphic file used on it
	//int filesize;
	int frameset[NPC_FRAME_NUMBER];				// holds information about the type of each frame
	int frameset_time[NPC_FRAME_NUMBER];		// the number of milisseconds this frame leasts
	int framesize_w;
	int framesize_h;
	int can_shoot;
	int have_shield;
	unsigned short int is_boss;
	int IA;										// IA types: no_move, wait, attack, zig-zag (loop), side to side (origin to direction)
};


// data that describes an NPC properties
struct file_npc {
	char name[50];
	struct hit_points hp;
	short int direction;              // defines how it behavies (kink of AI*)
	short int speed;               // defines the distances it can see enemies
	short int walk_range;                 // defines how long from the start point it can go
	short int facing;                     // defines the side npc is facing before start moving (also used by LINEWALK behavior)
	struct pos start_point;
	char filename[50];						// graphic file used on it
	//int filesize;
	int frameset[NPC_FRAME_NUMBER];				// holds information about the type of each frame
	int frameset_time[NPC_FRAME_NUMBER];		// the number of milisseconds this frame leasts
	int framesize_w;
	int framesize_h;
	int can_shoot;
	int have_shield;
	unsigned short int is_boss;
	int IA;										// IA types: no_move, wait, attack, zig-zag (loop), side to side (origin to direction)
	int projectile_id;							// indicates the projectile ID (relation with file_projectile)
};

// data that describes a projectile properties
struct file_projectile {
	char name[50];
	int trajectory_type;
	int is_destructible;
	int frame_w;
	int frame_h;
	char img_filename[255];
	int HP;
};


struct file_map_npc {
	short int id;
	struct pos start_point;
	unsigned short int direction;
};

struct list_map_npc {
	short int id;
	struct pos start_point;
	unsigned short int direction;
	struct list_map_npc *next;
};


struct npc_static_data_list {
	struct file_npc npc_data;
	struct sprite_struct *sprite_list;
	struct npc_static_data_list *next;
};

struct st_colorcycle_color {
	unsigned short int duration;
	short int r;
	short int g;
	short int b;
};

struct st_color {
	short int r;
	short int g;
	short int b;
};

#define COLORCYCLE_MAX_STEPS 5

struct st_colorcycle_file {
	struct st_color original;
	struct st_colorcycle_color current_color[COLORCYCLE_MAX_STEPS];
};

// TODO: create a file for dialogs and link it to a stage
struct st_file_stage {
	char stage_name[50];
	char boss_name[50];
	char boss_sprites_filename[255];
	char boss_face_filename[255];
	char bg_music_filename[255];
	struct st_colorcycle_file colorcycle[3];
};

enum STAGE_LIST {
	INTRO_STAGE, APEBOT, DAISIEBOT, SEAHORSEBOT, MUMMYBOT, SKULLCASTLE1, MAGEBOT, DYNAMITEBOT, SPIKEBOT, TECHNOBOT, SKULLCASTLE2, SKULLCASTLE3, SKULLCASTLE4, STAGE_COUNT
};

// -------------------------------------------- MAP ----------------------------------------------------- //

#define MAP_W 200 // this size was determined by looking at common maps from games
#define MAP_H 13
#define STAGE_NPC_LIMIT 50
#define STAGE_OBJECT_LIMIT 40

// "ground" types
#define TERRAIN_SOLID 1
#define TERRAIN_STAIR 2
#define TERRAIN_DOOR 3
#define TERRAIN_SPIKE 4
#define TERRAIN_WATER 5
#define TERRAIN_ICE 6
#define TERRAIN_MOVE_LEFT 7
#define TERRAIN_MOVE_RIGHT 8
#define TERRAIN_SAND 9
#define TERRAIN_SCROLL_LOCK 10
#define TERRAIN_CHECKPOINT 11


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
	OBJ_ITEM_FLY,
	OBJ_ITEM_JUMP
};



// information each point of map holds
struct map_tile {
	short int locked;
	struct pos tile1;                  // position of tile (x and y) in the graphic file (each tile is 16*16)
	struct pos tile2;                  // second level tile. this can be used to create more complex backgrounds
	struct pos tile3;                  // tird level, to be shown over player
};

struct file_map_v0 {
	char name[50];
	char filename[50];							// used tilesed
	short int bosss_door_x;						// tells what is the X position of boss door. this is used to not scrolling after this point
	struct map_tile tiles[MAP_W][MAP_H];		// map tiles
	struct file_npc npc_list[STAGE_NPC_LIMIT];	// we limit the number of enemies to 50 in the stage
};


struct file_map_v1 {
	char filename[50];							// used tilesed
	struct map_tile tiles[MAP_W][MAP_H];		// map tiles
};

// ----------------------------------------- OBJECT ----------------------------------------------------- //

struct stFileObject { // logic object. combination of data from the map object information and the object file
	char id[4]; // holds the number for the relative object file. eg '003' for 003.obj
	struct pos start_point;
};

struct struct_object { // same as the file object, but also contain pointers to be list and surface for graphics
	char id[4]; // holds the number for the relative object file. eg '003' for 003.obj
	char name[20];
	char filename[50];						// graphic file used on it
	short int type;
	int timer;				// used as time for disapearing block
	int speed;				// used as speed for moving platform
	int limit;				// used as range (pixels) for moving platform, visible time for disapearing block
	int direction;			// used to check if moving away from oiginalpoint or moving to it
	int distance;
	int framesize_w;
	int framesize_h;
	int frame;				// indicates what is the used frame
	struct pos start_point;
	struct pos position;
	SDL_Surface *sprite;
	struct struct_object *next;
};

// ----------------------------------------- STAGE ----------------------------------------------------- //




#endif // FILE_STRUCTS_H

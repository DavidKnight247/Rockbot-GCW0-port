#ifndef FILE_H
#define FILE_H

#include <SDL/SDL.h>
#include "format.h"



// ----------------------------------- STAGE ------------------------------------------ //

#define STAGE_MAX_MAPS 5
#define STAGE_COLORCYCLE_MAX 3

struct stFileStage {
	unsigned short int stage_id;
	int boss_id;
	char bgMusic[FILENAME_CHAR_SIZE];
	int map_list_id[STAGE_MAX_MAPS]; // default -1
	int colorcycle_list_id[STAGE_COLORCYCLE_MAX]; // default -1
};

// relational stStage(1) x (N)stMap
struct stFileStageMapList {
	unsigned short int stage_id;
	unsigned short int map_id;
};

// relational stStage(1) x (N)stColorcycle
struct stFileStageColorcycleList {
	unsigned short int stage_id;
	unsigned short int colorcycle_id;
};


// ----------------------------------- COLORCYCLE ------------------------------------------ //
struct stFileColorcycle { // KEY = colorcycle_id + initial_color (UNIQUE)
	int colorcycle_id;
	struct st_color initial_color;
	struct st_color current_color;
};

// ----------------------------------- MAP ------------------------------------------ //
struct stFileMap {
	unsigned short int map_id;
	struct st_point tile1[MAP_W][MAP_H];
	struct st_point tile2[MAP_W][MAP_H];
	struct st_point tile3[MAP_W][MAP_H];
};

// relational stMap(1) x (N)stNpc
struct stFileMapNpcList {
	unsigned short int map_id;
	unsigned short int npc_id;
	struct st_point position;
};

// relational stMap(1) x (N)stObject
struct stFileMapObjectList {
	unsigned short int map_id;
	unsigned short int object_id;
	struct st_point position;
};

// ----------------------------------- TELEPORTER ------------------------------------------ //
struct stFileTeleporter {
	unsigned short int teleporter_id;
	unsigned short int map_origin;
	unsigned short int map_destiny;
	struct st_point position;
	unsigned short int size; // default 1
	unsigned short int type; // default 0
	unsigned short int is_bidirecional; // default 1
	unsigned short int is_door; // default 0
};

// ----------------------------------- OBJECT ------------------------------------------ //
struct stFileObject {
	unsigned short int object_id;
	char name[NAME_CHAR_SIZE];
	char filename[FILENAME_CHAR_SIZE];
	unsigned short int type; // default 0
	unsigned short int speed; // default NPC_MOVE_STEP
	unsigned short int limit; // default RES_W * 3
	unsigned short int direction; // default ANIM_LEFT
	unsigned short int framesize; // default 8
	struct st_point initial_position;
};

// ----------------------------------- NPC ------------------------------------------ //

struct stFileFrame { // can be used both by npc or player
	short int npc_id; // default -1
	short int player_id; // default -1
	unsigned short int type; // default 0
	struct st_rect rect; // default x:0, y: 0, w: stNpc->size->w, h: stNpc->size->h
	unsigned short int duration;
};

struct stFileNpc {
	unsigned short int npc_id;
	char name[NAME_CHAR_SIZE];
	char filename[FILENAME_CHAR_SIZE];
	unsigned short int direction; // default ANIM_LEFT
	unsigned short int HP; // default 2
	struct st_size framesize; // default w: 8, h: 8 (used as base for stFrame->size->w and stFrame->size->h
	unsigned short int have_shield; // default 0
	unsigned short int ia_type; // default 0
};

struct stFileBoss {
	unsigned short int npc_id;
	char filename_face[FILENAME_CHAR_SIZE];
	// OBS: intro frames are stored in stFileFrame, with type INTRO
};

#endif // FILE_H

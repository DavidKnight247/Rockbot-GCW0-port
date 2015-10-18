#include "mediator.h"
#include <stdio.h>
#include <QFile>
#include <QDir>
#include "format/file_structs.h"
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"

#define EDIT_MODE_NEW 0
#define EDIT_MODE_EDIT 1

extern struct file_game game;

extern char FILEPATH[512];
extern struct file_map_v1 map;
extern struct file_map_v1 map_backup;
extern struct struct_object *obj_list;
extern struct struct_object *main_obj_list;

Mediator::Mediator() {
	int i;

	palleteX=0;
	palleteY=0;
	selectedTileset = "data/images/tilesets/default.png";
	editMode = EDITMODE_NORMAL;
	editTool = EDITMODE_NORMAL;
	npcGraphicSize_w = 16;
	npcGraphicSize_h = 16;
	// projectile
	projectileGraphicSize_w = 16;
	projectileGraphicSize_h = 16;

	NpcAddNumber=0;
	layerLevel=3;
	selectedNPC = -1;
	terrainType = 1;
	editModeNPC = EDIT_MODE_NEW;
	currentMap = 0;
	link_type = LINK_HORIZONTAL;
	object_type = 3;
	npc_direction = 0;
	map_links = NULL;

	for (i=0; i<NPC_FRAME_NUMBER; i++) {
		frameset[i]=-1;
		frameset_time[i] = 20;
	}
	zoom = 1;
	static_npc_list = NULL;
	npc_map_list = NULL;
	currentStage = 1;
	sprintf(addNpcFilename, "%c", '\0');
	sprintf(addProjectileFilename, "%c", '\0');
}

// set default values for game variable
void Mediator::initGameVar() {
	int j, k;
	strcpy (game.name, "My Game");
	sprintf(map.filename, "%s/data/images/tilesets/default.png", FILEPATH);
	for (j=0; j<MAP_W; j++) {
		for (k=0; k<MAP_H; k++) {
			map.tiles[j][k].locked=0;
			map.tiles[j][k].tile1.x=-1;
			map.tiles[j][k].tile1.y=-1;
			map.tiles[j][k].tile2.x=-1;
			map.tiles[j][k].tile2.y=-1;
			map.tiles[j][k].tile3.x=-1;
			map.tiles[j][k].tile3.y=-1;
		}
	}
	for (j=0; j<LINKS_NUMBER; j++) {
		game.map_links[j].link_pos.x= -1;
		game.map_links[j].link_pos.y= -1;
		game.map_links[j].link_dest.x= -1;
		game.map_links[j].link_dest.y= -1;
		game.map_links[j].map_origin = -1;
		game.map_links[j].map_dest = -1;
		game.map_links[j].link_size = 1;
		game.map_links[j].link_type = LINK_HORIZONTAL;
		game.map_links[j].link_bidi = 1;
	}
	loadGameNpcs(1);
	load_map_links();
}


int Mediator::getPalleteX() {
	return palleteX;
}

int Mediator::getPalleteY() {
	return palleteY;
}

void Mediator::setPalleteX(int value) {
	palleteX = value;
}

void Mediator::setPalleteY(int value) {
	palleteY = value;
}

char *Mediator::getPallete() {
   //return currentPallete;
	return selectedTileset;
}

void Mediator::setPallete(char *value) {
	selectedTileset = value;
}


// creates a list with all static-NPCs in a game
void Mediator::loadGameNpcs (int n) {
	struct npc_static_data_list *new_npc=NULL;
	struct file_npc temp_npc;
	FILE *fp;
	char directory[255];
	char buffer[255];
	int i;

	printf(">> loadGameNpcs.START <<\n");

	// free existing main_npc_list
	if (static_npc_list != NULL) {
		struct npc_static_data_list *temp_npc_list=static_npc_list;
		struct npc_static_data_list *old_npc_list=NULL;
		while (temp_npc_list) {
			old_npc_list = temp_npc_list;
			temp_npc_list = temp_npc_list->next;
			free(old_npc_list);
		}
		static_npc_list = NULL;
	}

	centNumberFormat(n);
	sprintf(directory, "%s/data/game/game_%s/", FILEPATH, centNumber);
	sprintf(buffer, "%s/main_list.npc", directory);



	fp = fopen(buffer, "rb");

	if (fp) {
		while (!feof(fp)) {
			if (fread(&temp_npc, sizeof(struct file_npc), 1, fp) != 1) {
				//printf("MainWindow::fillNpcList - Could not load npc from file -> interrupt.\n");
				break;
			}
			// create a chained list with all elements and copy data from file to it
			new_npc = add_static_npc();

			sprintf(new_npc->npc_data.name, "%s", temp_npc.name);
			sprintf(new_npc->npc_data.filename, "%s", temp_npc.filename);

			new_npc->npc_data.hp.total = temp_npc.hp.total;
			new_npc->npc_data.hp.current = temp_npc.hp.total;

			new_npc->npc_data.direction = temp_npc.direction;
			new_npc->npc_data.speed = temp_npc.speed;
			new_npc->npc_data.walk_range = temp_npc.walk_range;
			new_npc->npc_data.facing = temp_npc.facing;

			for (i=0; i<NPC_FRAME_NUMBER; i++) {
				new_npc->npc_data.frameset[i] = temp_npc.frameset[i];
				new_npc->npc_data.frameset_time[i] = temp_npc.frameset_time[i];
			}

			new_npc->npc_data.framesize_w = temp_npc.framesize_w;
			new_npc->npc_data.framesize_h = temp_npc.framesize_h;

			new_npc->npc_data.can_shoot = temp_npc.can_shoot;
			new_npc->npc_data.have_shield = temp_npc.have_shield;
			new_npc->npc_data.is_boss = temp_npc.is_boss;
			new_npc->npc_data.IA = temp_npc.IA;

			new_npc->sprite_list = NULL;

			new_npc->next = NULL;
		}
		fclose(fp);
	} else {
		printf(">>> could not load main npc list file '%s' <<<\n", buffer);
	}
}


struct npc_static_data_list *Mediator::get_main_npc(int pos) {
	// ------- SEARCH THE MAIN NPC LIST FOR THE GIVEN ID ------- //
	struct npc_static_data_list *temp_main_npc_list = static_npc_list;
	int n = 0;
	while (temp_main_npc_list) {
		if (pos == n) {
			return temp_main_npc_list;
		}
		n++;
		temp_main_npc_list = temp_main_npc_list->next;
	}
	return NULL;
}


// construct a list with all the npcs for a given map
void Mediator::loadMapNpcs(int game_n, int stage_n, int map_n) {
	struct list_map_npc *temp_list, *old_list;
	char temp_cent_number1[4], temp_cent_number2[4], map_cent_number[4];
	char filename[255];
	struct npc_static_data_list *temp_main_npc_list;
	FILE *fp_map_npc;
	int list_pos = 0;
	struct list_map_npc *temp_map_npc_list;

	centNumberFormat(game_n);
	sprintf(temp_cent_number1, "%s", centNumber);
	centNumberFormat(stage_n);
	sprintf(temp_cent_number2, "%s", centNumber);
	centNumberFormat(map_n);
	sprintf(map_cent_number, "%s", centNumber);

	// empty npc list
	temp_list = npc_map_list;
	if (temp_list != NULL) {
		while (temp_list) {
			old_list = temp_list;
			temp_list = temp_list->next;
			old_list->next = NULL;
			free(old_list);
			//printf(">> loadMap::loadMapNpcs: delete item from list\n");
			old_list = NULL;
		}
	}
	npc_map_list = NULL;

	sprintf(filename, "%s/data/game/game_%s/stage_%s/map_%s_npc_list.dat", FILEPATH, temp_cent_number1, temp_cent_number2, centNumber);
	printf(">> loadMapNpcs.filename: '%s', map_n: %d, map_cent_number: '%s' <<\n", filename, map_n, map_cent_number);

	fp_map_npc = fopen(filename, "rb");
	struct file_map_npc temp_map_npc;
	int npcCount = 0;

	if (fp_map_npc) {
		while (!feof(fp_map_npc)) {
			/*
			if (fread(&temp_map_npc, sizeof(struct file_map_npc), 1, fp_map_npc) != 1) {
				printf("loadMapNpcs - Could not load npc %d from file '%s' interrupt.\n", npcCount, filename);
				break;
			}
			*/
			fread(&temp_map_npc, sizeof(struct file_map_npc), 1, fp_map_npc);
			//printf(">> DEBUG - searching for NPC with ID %d <<\n", temp_map_npc.id);
			// ------- SEARCH THE MAIN NPC LIST FOR THE GIVEN ID ------- //
			temp_main_npc_list = static_npc_list;
			int npc_pos = -1;
			int n = 0;
			if (static_npc_list == NULL) {
				printf("** ERROR: static_npc_list is empty! **\n");
			}
			while (temp_main_npc_list) {
				//printf(">> main_list.search, n: %d <<\n", n);
				if (temp_map_npc.id == n) {
					npc_pos = n;
					break;
				}
				n++;
				temp_main_npc_list = temp_main_npc_list->next;
			}
			if (npc_pos == -1) {
				printf("+++ Mediator::loadMapNpcs - ERROR: Could not find NPC with id '%d' in the main object list\n", temp_map_npc.id);
				continue;
			}

			// ----- CREATE THE NEW NPC TO ADD TO MAP LIST ENDING ----- //
			struct list_map_npc *new_npc = NULL;
			new_npc = (struct list_map_npc *)malloc(sizeof(struct list_map_npc));
			new_npc->id = npc_pos;
			new_npc->start_point.x = temp_map_npc.start_point.x;
			new_npc->start_point.y = temp_map_npc.start_point.y;
			new_npc->direction = temp_map_npc.direction;
			new_npc->next = NULL;

			if (npc_map_list == NULL) {
				npc_map_list = new_npc;
			} else {
				temp_map_npc_list = npc_map_list;
				while (temp_map_npc_list->next) {
					temp_map_npc_list = temp_map_npc_list->next;
					list_pos++;
				}
				temp_map_npc_list->next = new_npc;
			}
			//printf(">>> loadMapNpcs - added NPC '%d' in map list, pos: %d <<<\n", npc_pos, list_pos);
			npcCount++;
		}
	} else {
		printf(">> loadMapNpcs - could not open map-npcs list file '%s'<<\n", filename);
	}


}



struct npc_static_data_list *Mediator::add_list_npc() {
	int i;
	struct npc_static_data_list *new_npc;

	new_npc = (struct npc_static_data_list *)malloc(sizeof(struct npc_static_data_list));
	sprintf(new_npc->npc_data.name, "%s", "\0");
	sprintf(new_npc->npc_data.filename, "%s", "\0");
	new_npc->npc_data.hp.total = 0;
	new_npc->npc_data.hp.current = 0;
	new_npc->npc_data.direction = 0;
	new_npc->npc_data.speed = 2;
	new_npc->npc_data.walk_range = 48;
	new_npc->npc_data.facing = 0;
	for (i=0; i<NPC_FRAME_NUMBER; i++) {
		new_npc->npc_data.frameset[i] = 0;
		new_npc->npc_data.frameset_time[i] = 0;
	}
	new_npc->npc_data.framesize_w = 0;
	new_npc->npc_data.framesize_h = 0;
	new_npc->npc_data.can_shoot = 0;
	new_npc->npc_data.have_shield = 0;
	new_npc->npc_data.is_boss = 0;
	new_npc->npc_data.IA = 0;
	new_npc->npc_data.projectile_id = -1;

	// create graphics from file
	new_npc->sprite_list = NULL;

	new_npc->next = NULL;
	return new_npc;
}


struct npc_static_data_list *Mediator::add_static_npc() {
	struct npc_static_data_list *new_npc = NULL;
	struct npc_static_data_list *temp_npc = NULL;

	new_npc = add_list_npc();
	new_npc->next = NULL;
	if (static_npc_list == NULL) {
		static_npc_list = new_npc;
	} else {
		temp_npc = static_npc_list;
		if (temp_npc != NULL) {
			while (temp_npc->next) {
				temp_npc = temp_npc->next;
			}
		}
		temp_npc->next = new_npc;
	}
	return new_npc;
}





void Mediator::loadStageMap(int game_n, int stage_n, int map_n) {
	long lSize;
	char filename[512];
	char temp_cent_number1[4], temp_cent_number2[4];

	printf(">> Mediator::loadStageMap - game_n: %d, stage_n: %d, map_n: %d<<\n", game_n, stage_n, map_n);

	centNumberFormat(game_n);
	sprintf(temp_cent_number1, "%s", centNumber);

	centNumberFormat(stage_n);
	sprintf(temp_cent_number2, "%s", centNumber);

	centNumberFormat(map_n);
	//printf("DEBUG.Mediator::loadMap - START\n");
	sprintf(filename, "%s/data/game/game_%s/stage_%s/%s_map.dat", FILEPATH, temp_cent_number1, temp_cent_number2, centNumber);
	currentGame = game_n;
	currentMap = map_n;
	//printf("DEBUG.Mediator::loadStageMap - filename: %s\n", filename);
	FILE *fp = fopen(filename, "rb");
	if (fp) {
		//printf("Loading map file...\n");
		fseek (fp , 0 , SEEK_END);
		lSize = ftell (fp);
		rewind (fp);
		fread(&map, sizeof(struct file_map_v1), 1, fp);
		fclose(fp);
	} else {
		printf("Mediator::loadMap - DEBUG - no map file (%s), ignoring load.\n", filename);
	}

	loadMapNpcs(game_n, stage_n, map_n);

	// empty object list
	struct struct_object *temp_obj_list, *old_obj_list;
	temp_obj_list = obj_list;
	if (temp_obj_list != NULL) {
		while (temp_obj_list) {
			old_obj_list = temp_obj_list;
			temp_obj_list = temp_obj_list->next;
			old_obj_list->next = NULL;
			free(old_obj_list);
			old_obj_list = NULL;
		}
	}
	obj_list = NULL;	// free player list
	sprintf(filename, "%s/data/game/game_%s/stage_%s/%s.map.obj", FILEPATH, temp_cent_number1, temp_cent_number2, centNumber);
	fp = fopen(filename, "rb");
	if (fp) {
		struct struct_object *new_obj;
		struct stFileObject tempFileObj;
		struct struct_object *temp_main_obj_list, *found_main_obj;
		while (!feof(fp)) {
			if (fread(&tempFileObj, sizeof(struct stFileObject), 1, fp) != 1) {
				//printf("LoadMap::loadMapObjs - Could not load file, finished.\n");
				break;
			}
			temp_main_obj_list = main_obj_list;
			found_main_obj = NULL;
			while (temp_main_obj_list) {
				if (strstr(temp_main_obj_list->id, tempFileObj.id)) {
					found_main_obj = temp_main_obj_list;
					break;
				}
				temp_main_obj_list = temp_main_obj_list->next;
			}
			if (found_main_obj == NULL) {
				//printf("Mediator::loadMap - ERROR: Could not find object with id '%s' in the main object list\n", tempFileObj.id);
				continue;
			}

			new_obj = (struct struct_object *)malloc(sizeof(struct struct_object));
			new_obj->direction = 0;
			new_obj->distance = 0;
			sprintf(new_obj->filename, "%s", "");
			new_obj->frame = 0;
			new_obj->framesize_h = 0;
			new_obj->framesize_w = 0;
			new_obj->limit = 0;
			sprintf(new_obj->name, "%s", "");
			new_obj->position.x = 0;
			new_obj->position.y = 0;
			new_obj->speed = 0;
			new_obj->start_point.x = 0;
			new_obj->start_point.y = 0;
			new_obj->timer = 0;
			new_obj->type = -1;



			// copy infomation from file struct to logic object structure
			new_obj->direction = found_main_obj->direction;

			new_obj->distance = found_main_obj->distance;
			sprintf(new_obj->filename, "%s", found_main_obj->filename);
			new_obj->frame = found_main_obj->frame;
			new_obj->framesize_h = found_main_obj->framesize_h;
			new_obj->framesize_w = found_main_obj->framesize_w;
			sprintf(new_obj->id, "%s", found_main_obj->id);

			new_obj->limit = found_main_obj->limit;
			sprintf(new_obj->name, "%s", found_main_obj->name);
			new_obj->position.x = found_main_obj->position.x;
			new_obj->position.y = found_main_obj->position.y;
			new_obj->speed = found_main_obj->speed;
			new_obj->start_point.x = tempFileObj.start_point.x;
			new_obj->start_point.y = tempFileObj.start_point.y;
			new_obj->timer = found_main_obj->timer;
			new_obj->type = found_main_obj->type;
			new_obj->next = NULL;

			//printf("LoadMap::loadMapObjs - Adding object, id: %s, pos.x: %d, pos.y: %d.\n", new_obj->id, new_obj->start_point.x, new_obj->start_point.y);


			if (obj_list == NULL) {
				obj_list = new_obj;
			} else {
				temp_obj_list = obj_list;
				while (temp_obj_list->next != NULL) {
					temp_obj_list = temp_obj_list->next;
				}
				temp_obj_list->next = new_obj;
			}
		}
	} else {
		printf(">> ERROR: Could not open file '%s' for loading <<\n", filename);
	}


}


void Mediator::loadGameObjects (int n) {
	char buffer[512], directory[512];
	int i;
	struct struct_object *new_obj;
	FILE *fp;
	struct struct_object *temp_obj_list=NULL;

	centNumberFormat(n);
	sprintf(directory, "%s/data/game/game_%s", FILEPATH, centNumber);

	for (i=1; i<999; i++) {
		centNumberFormat(i);
		sprintf(buffer, "%s/%s.obj", directory, centNumber);
		//printf("Mediator::loadGameObjects - filename: %s\n", buffer);
		fp = fopen(buffer, "rb");
		if (!fp) {
			//printf("Mediator::loadGameObjects - file %s not found\n", buffer);
			break;
		}

		if (fp) {
			new_obj = (struct struct_object *)malloc(sizeof(struct struct_object));
			new_obj->position.x = new_obj->start_point.x * 16;
			new_obj->position.y = new_obj->start_point.y * 16;
			fread(new_obj, sizeof(struct_object), 1, fp);
			new_obj->next = NULL;
			//printf("Mediator::loadGameObjects - id: %s, filename: %s\n", new_obj->id, new_obj->filename);
			if (main_obj_list == NULL) {
				main_obj_list = new_obj;
			} else {
				temp_obj_list = main_obj_list;
				while (temp_obj_list->next != NULL) {
					temp_obj_list = temp_obj_list->next;
				}
				temp_obj_list->next = new_obj;
			}
			fclose(fp);
		} else {
			//printf("DEBUG - Could not load file '%s'.\n", buffer);
		}

	}
}



void Mediator::loadGame(int n) {
	long lSize;
	char filename[512];

	centNumberFormat(n);
	sprintf(filename, "%s/data/game/%s.gme", FILEPATH, centNumber);
	FILE *fp = fopen(filename, "rb");
	if (fp) {
		fseek (fp , 0 , SEEK_END);
		lSize = ftell (fp);
		rewind (fp);
		fread(&game, sizeof(game), 1, fp);
		fclose(fp);
		loadGameObjects(n);
		printf(">> loadStageMap::CALL 3 - map: %d <<\n", 0);
		loadStageMap(n, 1, 0);
	}
}


void Mediator::createGame() {
	char buffer[512];
	FILE *fp;
	int n;
	//QDir("Folder").exists();
	//QDir().mkdir("MyFolder");
	//QFile f( "f.txt" );
	//if( !f.exists() )
	// position in the last
	// look for a empty game slot
	sprintf(buffer, "%s/data/game/", FILEPATH);
	n = freeSlot(buffer, "gme");

	printf("DEBUG.Mediator::createGame - centNumber: %s\n", centNumber);
	sprintf(buffer, "%s/data/game/game_%s", FILEPATH, centNumber);
	if (!QDir(buffer).exists()) {
		printf("Creating '%s folder\n", buffer);
		QDir().mkdir(buffer);
	} else {
		printf("Folder '%s' already exists", buffer);
	}
	initGameVar();
	// creates the game file
	sprintf(buffer, "data/game/%s.gme", centNumber);
	fp = fopen(buffer, "wb");
	if (fp) {
		fwrite(&game, sizeof(game), 1, fp);
		fclose(fp);
		printf("Mediator::createGame - Created game file '%s'\n", buffer);
	} else {
		printf("ERROR: Can't open file '%s' to write\n", buffer);
	}
	// creates the map file
	sprintf(buffer, "%s/data/game/game_%s/001.map", FILEPATH, centNumber);
	fp = fopen(buffer, "wb");
	if (fp) {
		fwrite(&map, sizeof(map), 1, fp);
		fclose(fp);
		printf("Mediator::createGame - Created map file '%s'\n", buffer);
	} else {
		printf("ERROR: Can't open file '%s' to write\n", buffer);
	}
}


void Mediator::centNumberFormat(int n) {
	if (n >= 100) {
		sprintf(centNumber, "%d\0", n);
	} else if (n >= 10) {
		sprintf(centNumber, "0%d\0", n);
	} else {
		sprintf(centNumber, "00%d\0", n);
	}
	//printf("DEBUG.Mediator::centNumberFormat - centNumber: %s\n", centNumber);
}

int Mediator::freeSlot(char *dir, char *extension) {
	int i;
        char filename[512];
	for (i=1; i<99; i++) {
		centNumberFormat(i);
		sprintf(filename, "%s/%s.%s", dir, centNumber, extension);
		//printf("DEBUG - searching for dir: '%s', number: '%s'', extension: '%s'', filename: '%s'\n", dir, centNumber, extension, filename);
		QFile file(filename);
		if (!file.exists()) {
			return i;
		}
	}
	return -1;
}

void Mediator::resetMap(int map_n) {
	int j, k;

	sprintf(map.filename, "%s/data/images/tilesets/default.png", FILEPATH);
	for (j=0; j<MAP_W; j++) {
		for (k=0; k<MAP_H; k++) {
			map.tiles[j][k].locked=0;
			map.tiles[j][k].tile1.x=-1;
			map.tiles[j][k].tile1.y=-1;
			map.tiles[j][k].tile2.x=-1;
			map.tiles[j][k].tile2.y=-1;
			map.tiles[j][k].tile3.x=-1;
			map.tiles[j][k].tile3.y=-1;
		}
	}
}

void Mediator::addMap() {
	char filename[512];
	char buffer[512];
	int n;
	FILE *fp;

	centNumberFormat(currentGame);
	sprintf(filename, "%s/data/game/game_%s/", FILEPATH, centNumber);
	printf("DEBUG.Mediator::addMap - filename: %s\n", filename);
	n = freeSlot(filename, "map");
	resetMap(n);
	// creates the map file
	centNumberFormat(n);
	sprintf(buffer, "%s/%s.map", filename, centNumber);
	printf("DEBUG.Mediator::addMap - buffer: %s\n", buffer);
	fp = fopen(buffer, "wb");
	if (fp) {
		fwrite(&map, sizeof(map), 1, fp);
		fclose(fp);
		printf("Mediator::createGame - Created map file '%s'\n", buffer);
	} else {
		printf("ERROR: Can't open file '%s' to write\n", buffer);
	}
	printf(">> loadStageMap::CALL 4 - map: %d <<\n", currentMap);
	loadStageMap(currentGame, currentStage, currentMap);
}

void Mediator::getGameName(int n) {
	long lSize;
        char filename[512];
	centNumberFormat(n);
	struct file_game temp_game;
	sprintf(filename, "%s/data/game/%s.gme", FILEPATH, centNumber);
	FILE *fp = fopen(filename, "rb");
	if (fp) {
		fseek (fp , 0 , SEEK_END);
		lSize = ftell (fp);
		rewind (fp);
		fread(&temp_game, sizeof(temp_game), 1, fp);
		fclose(fp);
		printf("DEBUG;Mediator::getGameName - temp_game.name: %s\n", temp_game.name);
		sprintf(gameName, "%s", temp_game.name);
	} else {
		printf("DEBUG.Mediator::getGameName - Error opening file '%s'.\n",filename);
		sprintf(gameName, "");
	}
}



void Mediator::placeNpc(int posx, int posy, int npc_n) {
	if (selectedNPC < 1) {
		printf(">>>> ERROR: selectedNPC '%d' invalid.\n", selectedNPC);
		return;
	} else {
		printf(">>>> WARNING: Adding selectedNPC '%d'.\n", selectedNPC);
	}

	// ----- CREATE THE NEW NPC TO ADD TO MAP LIST ENDING ----- //
	struct list_map_npc *new_npc = NULL;
	new_npc = (struct list_map_npc *)malloc(sizeof(struct list_map_npc));
	new_npc->id = selectedNPC-1;
	new_npc->start_point.x = posx;
	new_npc->start_point.y = posy;
	new_npc->direction = npc_direction;
	new_npc->next = NULL;

	struct list_map_npc *temp_map_npc_list;
	int new_pos=0;

	if (npc_map_list == NULL) {
		npc_map_list = new_npc;
	} else {
		temp_map_npc_list = npc_map_list;
		while (temp_map_npc_list->next) {
			new_pos++;
			temp_map_npc_list = temp_map_npc_list->next;
		}
		temp_map_npc_list->next = new_npc;
	}
	printf(">> added NPC, id: %d, dir: %d, pos: %d <<\n", new_npc->id, new_npc->direction, new_pos);
}


void Mediator::addObject(char name[20], char filename[50], unsigned short int type, unsigned short int timer, unsigned short int limit, unsigned short int framesize_w, unsigned short int framesize_h) {
        char buffer[512];
	struct struct_object temp_object;
	int n;

	sprintf(temp_object.name, "%s", name);
	sprintf(temp_object.filename, "%s", filename);
	temp_object.type = type;
	temp_object.timer = timer;
	temp_object.limit = limit;
	printf("DEBUG.Mediator::addObject - object.filename: '%s'', arg.filename: '%s'\n", temp_object.filename, filename);
	centNumberFormat(currentGame);
	sprintf(filename, "%s/data/game/game_%s", FILEPATH, centNumber);
	n = freeSlot(filename, "obj");
	sprintf(buffer, "%s/%s.obj", filename, centNumber);
	temp_object.framesize_w = framesize_w;
	temp_object.framesize_h = framesize_h;
	FILE *fp = fopen(buffer, "wb");
	if (fp) {
		fwrite(&temp_object, sizeof(struct_object), 1, fp);
		fclose(fp);
	} else {
		printf("DEBUG - no map file, ignoring load.\n");
	}
}

void Mediator::placeObject(unsigned short int posx, unsigned short int posy, struct struct_object *obj) {
	struct struct_object temp_object;
        char filename[512];
        char buffer[512];

	centNumberFormat(currentGame);
	sprintf(filename, "%s/data/game/game_%s", FILEPATH, centNumber);
	//printf("DEBUG.Mediator::placeObject - dir: %s\n", filename);
	if (selectedNPC < 1) {
		printf(">>>> ERROR: selectedObject '%d' invalid.\n", selectedNPC);
		return;
	}
	centNumberFormat(selectedNPC);
	sprintf(buffer, "%s/%s.obj", filename, centNumber);
	FILE *fp = fopen(buffer, "rb");
	if (fp) {
		fread(&temp_object, sizeof(struct_object), 1, fp);
		fclose(fp);
	} else {
		printf(">>>>>> DEBUG - no Object file, ignoring load.\n");
		return;
	}
	//printf("DEBUG.Mediator::placeObject - filename: %s, type: %d, posx: %d, posy: %d\n", buffer, temp_object.type, posx, posy);

	sprintf(obj->id, "%s", temp_object.id);
	sprintf(obj->filename, "%s", temp_object.filename);
	obj->framesize_w = temp_object.framesize_w;
	obj->framesize_h = temp_object.framesize_h;
	sprintf(obj->name, "%s", temp_object.name);
	obj->start_point.x = posx;
	obj->start_point.y = posy;

	obj->type = temp_object.type;
	obj->timer = temp_object.timer;
	obj->limit = temp_object.limit;
	obj->speed = temp_object.speed;
}


int Mediator::get_stage_n(const int map_n) {
	if (map_n < 10) {
		return map_n;
	}
	if (map_n < 19) {
		return map_n-9;
	}
	return map_n-18;
}

void Mediator::load_map_links(void) {
	struct links_list *new_link = NULL;
	struct map_link_v1 link;
	FILE *fp_links;
	char filename[255];

	printf(">>>>>>> load_map_links::START <<<<<<<\n");

	sprintf(filename, "%s/data/game/game_001/map_links.dat", FILEPATH);
	fp_links = fopen(filename, "rb");
	if (fp_links) {
		while (!feof(fp_links)) {
			if (fread(&link, sizeof(struct map_link_v1), 1, fp_links) != 1) {
				printf("load_map_links - Could not load link from file -> interrupt.\n");
				break;
			}
			new_link = add_map_link();
			new_link->link.link_pos.x = link.link_pos.x;
			new_link->link.link_pos.y = link.link_pos.y;
			new_link->link.link_dest.x = link.link_dest.x;
			new_link->link.link_dest.y = link.link_dest.y;

			new_link->link.map_origin = link.map_origin;
			new_link->link.map_dest = link.map_dest;
			new_link->link.stage = link.stage;

			new_link->link.link_size = link.link_size;
			new_link->link.link_type = link.link_type;
			new_link->link.link_bidi = link.link_bidi;
			new_link->link.is_door = link.is_door;

			//printf(">> load_map_links - added link - origin: %d, destiny: %d, stage: %d <<\n", new_link->link.map_origin, new_link->link.map_dest, new_link->link.stage);

		}
		fclose(fp_links);
	} else {
		printf(">> ERROR: Could not load map_links file '%s' <<\n", filename);
		return;
	}
}


struct links_list *Mediator::add_map_link(void) {
	struct links_list *new_link = NULL;
	struct links_list *temp_link = NULL;

	new_link = (struct links_list*)malloc(sizeof(struct links_list));
	new_link->next = NULL;
	if (map_links == NULL) {
		map_links = new_link;
	} else {
		temp_link = map_links;
		while (temp_link->next) {
			temp_link = temp_link->next;
		}
		temp_link->next = new_link;
	}
	return new_link;
}

void Mediator::remove_map_link(struct links_list *link) {
	struct links_list *temp_link, *ant_link;

	printf(">> remove_map_link::START <<\n");
	temp_link = NULL;
	ant_link = NULL;

	// error check
	if (map_links == NULL) {
		return;
	}

	if (link == map_links) {
		map_links = map_links->next;
	} else {
		// start on second element, as the first one was tested above
		ant_link = map_links;
		temp_link = map_links->next;
		while(temp_link) {
			if (temp_link == link) {
				ant_link->next = temp_link->next;
				ant_link = temp_link;
				free(ant_link);
				ant_link = NULL;
			}
			ant_link = temp_link;
			temp_link = temp_link->next;
		}
	}
}

void Mediator::saveMapLinks(void) {
	struct links_list *temp_link;
	char filename[255];
	struct map_link_v1 link;
	FILE *fp_links;

	sprintf(filename, "%s/data/game/game_001/map_links.dat", FILEPATH);
	fp_links = fopen(filename, "wb");
	if (fp_links) {
		printf(">> WARNING: Opened map links file '%s' for writting <<\n", filename);
		temp_link = map_links;
		while(temp_link) {
			// store data in file_maplink structure
			link.link_pos.x = temp_link->link.link_pos.x;
			link.link_pos.y = temp_link->link.link_pos.y;
			link.link_dest.x = temp_link->link.link_dest.x;
			link.link_dest.y = temp_link->link.link_dest.y;

			link.map_origin = temp_link->link.map_origin;
			link.map_dest = temp_link->link.map_dest;
			link.stage = temp_link->link.stage;

			link.link_size = temp_link->link.link_size;
			link.link_type = temp_link->link.link_type;
			link.link_bidi = temp_link->link.link_bidi;
			link.is_door = temp_link->link.is_door;
			fwrite(&link, sizeof(struct map_link_v1), 1, fp_links);
			//printf(">> added map link <<\n");

			temp_link = temp_link->next;
		}
		fclose(fp_links);
	} else {
		printf(">> ERROR: Could not open map links file '%s' <<\n", filename);
	}
}


void Mediator::loadStage(int game_n, int stage_n) {
	char filename[512];
	char game_cent_number[4];
	int seek_pos;

	centNumberFormat(game_n);
	sprintf(game_cent_number, "%s", centNumber);

	sprintf(filename, "%s/data/game/game_%s/stages.dat", FILEPATH, game_cent_number, centNumber);
	printf(">> DEBUG.Mediator::loadStage - filename: %s\n", filename);
	FILE *fp = fopen(filename, "rb");
	if (fp) {
		//printf("Loading stage file...\n");
		rewind(fp);
		seek_pos = (stage_n)*sizeof(struct st_file_stage);
		fseek(fp, seek_pos, SEEK_SET);
		if (fread(&stage, sizeof(struct st_file_stage), 1, fp) != 1) {
			printf(">> DEBUG.Mediator::loadStage - could not load from file <<\n");
			return;
		} else {
			printf(">> DEBUG.Mediator::loadStage - loaded stage %d from file <<\n", stage_n);
		}
		printf(">> WARNING: Loaded stage, boss name: '%s' <<\n", stage.boss_name);
		fclose(fp);
	} else {
		printf(">> Mediator::loadStage - could not open file '%s' for loading.\n", filename);
	}
}

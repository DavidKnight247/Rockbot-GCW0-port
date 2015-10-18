//#include <variables>		// variáveis comuns ao jogo e ao editor

struct struct_object *obj_list[3];

void show_sprite(struct sprite_struct *sprite_list, SDL_Surface *screen);
void init_sprite_vars(struct sprite_struct *sprite_list);

// FUNCTIONS PROTOTYPES

short int check_colision(short int obj_x, short int obj_y, short int obj_w, short int obj_h, short int x_inc, short int y_inc);
void damage_npc(struct npc_struct *temp_npc, short int damage);
struct sprite_struct *create_sprite_with_surface(short int x, short int y, short int sp_w, short int sp_h, SDL_Surface *screen);
void add_sprite(short int direction, short int anim_type, short int pos_x, short int pos_y, struct sprite_struct *sprite_list, short int duration, SDL_Surface *image);
void blank_screen(SDL_Surface *screen);
void draw_text(short int x, short int y, char *text, SDL_Surface *screen);
struct struct_colorcycle_list *add_colorcycle_list(short int color_n);
void add_colorcycle(struct struct_colorcycle_list *list, int duration, int r, int g, int b);
void exec_colorcycle(SDL_Surface *screen);
void change_colormap(SDL_Surface *screen, int pos, int r, int g, int b);
// comes from hacked SDL
SDL_Surface *surface_from_image(char filename[255]);
void drawMap(SDL_Surface *screen, const short int adjust);
void load_colorcycle(int map_n);

struct struct_object *main_obj_list=NULL; // object list from .obj files




// ------------------------ FUNÇÕES ----------------------------


unsigned int getTimer() {
	return SDL_GetTicks();
}

SDL_Surface *loadSpriteFile(const char *buffer) {
	SDL_RWops *rwop;
	SDL_Surface *spriteCopy;
	// carrega paleta
	rwop=SDL_RWFromFile(buffer, "rb");
	if (!rwop) {
		printf("DEBUG.loadSpriteFile - Error in loadSpriteFile: file '%s' not found\n", buffer);
		return NULL;
	}
	spriteCopy = IMG_Load_RW(rwop, 1);
	if (!spriteCopy) {
		printf("IMG_LoadPNG_RW: %s\n", IMG_GetError());
	}
	SDL_SetColors(spriteCopy, colormap, 0, COLOR_COUNT);
	return spriteCopy;
}


void move_screen(SDL_Surface *screen, const short int x_inc, const short int y_inc) {
	SDL_Rect src, dest;
	src.x = (map_pos_x*-1) + (x_inc*-1) + TILE_SIZE;
	src.y = (map_pos_y*-1) + (y_inc*-1) + TILE_SIZE;
	src.w = MAX_W;
	src.h = MAX_H;
	// parte que vai ser colada
	dest.x = 0;
	dest.y = 0;
	dest.w = MAX_W;
	dest.h = MAX_H;
	map_pos_x += x_inc;
	map_pos_y += y_inc;
	if (src.x < 0) {
		src.x = 0;
	}

	if (src.y < 0) {
		src.y = 0;
	}
	SDL_BlitSurface(offscreen, &src, screen, &dest);
}


// copia um tile de um lugar para outro
void copy_tile(const short int tx, const short int ty, const short int dx, const short int dy, SDL_Surface *screen_source, SDL_Surface *screen_dest) {
	SDL_Rect src, dest;
	src.x = tx*TILE_SIZE;
	src.y = ty*TILE_SIZE;
	src.w = TILE_SIZE;
	src.h = TILE_SIZE;
	// parte que vai ser colada
	dest.x = dx*TILE_SIZE;
	dest.y = dy*TILE_SIZE;
	dest.w = TILE_SIZE;
	dest.h = TILE_SIZE;
	SDL_BlitSurface (screen_source, &src, screen_dest, &dest);
}


// copia um tile de um lugar para outro
void copy_area(SDL_Surface *screen_source, int src_x, int src_y, int src_w, int src_h, int dest_x, int dest_y, SDL_Surface *screen_dest) {
	SDL_Rect src, dest;
	src.x = src_x;
	src.y = src_y;
	src.w = src_w;
	src.h = src_h;
	dest.x = dest_x;
	dest.y = dest_y;
	dest.w = src_w;
	dest.h = src_h;
	SDL_BlitSurface(screen_source, &src, screen_dest, &dest);
}

// copia um tile de um lugar para outro
void copy_area_debug(SDL_Surface *screen_source, const short int src_x, const short int src_y, const short int src_w, const short int src_h, const short int dest_x, const short int dest_y, SDL_Surface *screen_dest) {
	SDL_Rect src, dest;
	src.x = src_x;
	src.y = src_y;
	src.w = src_w;
	src.h = src_h;
	dest.x = dest_x;
	dest.y = dest_y;
	dest.w = src_w;
	dest.h = src_h;
	SDL_BlitSurface(screen_source, &src, screen_dest, &dest);
}

void draw_matrix(SDL_Surface *screen, const short int x, const short int y, const short int width, const short int height) {
	// desenha linhas pretas horizontais da matriz
	SDL_Rect dest;
	short int i;

        for (i=y; i<y+height; i+=TILE_SIZE) {
                dest.x = x;
		dest.y = i;
                dest.w = width;
		dest.h = 1;
		SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 180, 180, 180));
	}
	// desenha linhas pretas vericais da matriz
	for (i=x; i<x+width; i+=TILE_SIZE) {
		dest.x = i;
		dest.y = y;
		dest.w = 1;
		dest.h = height;
		SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 180, 180, 180));
	}
}




void waitTime(int wait_period) {
	int now_time=0;
	now_time = getTimer();
	wait_period = now_time + wait_period;
	while (now_time < wait_period) {
		now_time = getTimer();
		SDL_Delay(1);
		#ifdef PLAYSTATION
			RotateThreadReadyQueue(_MIXER_THREAD_PRIORITY);
		#endif
	}
}

void centNumber(const int number, char cent_char[4]) {
	if (number < 10) {
		sprintf(cent_char, "00%d", number);
	} else if (number < 100) {
		sprintf(cent_char, "0%d", number);
	} else {
		sprintf(cent_char, "%d", number);
	}
}


void loadGameObjects (int n) {
	char buffer[255], directory[255], centNumberC[4];
	int i;
	struct struct_object *new_obj;
	FILE *fp;
	struct struct_object *temp_obj_list=NULL;



	centNumber(n, centNumberC);
	sprintf(directory, "%sdata/game/game_%s", FILEPATH, centNumberC);


	for (i=1; i<999; i++) {
		centNumber(i, centNumberC);
		sprintf(buffer, "%s/%s.obj", directory, centNumberC);
		fp = fopen(buffer, "rb");
		if (!fp) {
			printf("ERROR in common.h::loadGameObjects - file '%s'' not found.\n", buffer);
			break;
		}

		if (fp) {
			new_obj = (struct struct_object *)malloc(sizeof(struct struct_object));
			fread(new_obj, sizeof(struct struct_object), 1, fp);
			new_obj->position.x = new_obj->start_point.x * 16;
			new_obj->position.y = new_obj->start_point.y * 16;
			new_obj->direction = ANIM_LEFT;
			new_obj->next = NULL;
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
		}
	}
}


struct npc_static_data_list *add_list_npc() {
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


struct npc_static_data_list *add_static_npc() {
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




void loadGameNpcs (int n) {
	struct npc_static_data_list *new_npc = NULL;
	struct file_npc temp_npc;
	FILE *fp;
	char centNumberC[4];
	char directory[255];
	char buffer[255];
	int i, j, k;
	SDL_Surface* image_surface = NULL;

	centNumber(n, centNumberC);
	sprintf(directory, "%sdata/game/game_%s", FILEPATH, centNumberC);
	sprintf(buffer, "%s/main_list.npc", directory);
	fp = fopen(buffer, "rb");

	if (fp) {
		while (!feof(fp)) {
			if (fread(&temp_npc, sizeof(struct file_npc), 1, fp) != 1) {
				printf("MainWindow::fillNpcList - Could not load npc from file '%s' -> interrupt.\n", buffer);
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


			sprintf(buffer, "%sdata/images/sprites/enemies/%s", FILEPATH, new_npc->npc_data.filename);
			image_surface = surfaceFromFile(buffer);

			if (image_surface != NULL) {
				new_npc->sprite_list = (struct sprite_struct*)malloc(sizeof(struct sprite_struct));
				init_sprite_vars(new_npc->sprite_list);
				new_npc->sprite_list->x = 0;
				new_npc->sprite_list->y = 0;
				new_npc->sprite_list->w = new_npc->npc_data.framesize_w;
				new_npc->sprite_list->h = new_npc->npc_data.framesize_h;
				//struct anim_frame sprite_frames[ANIM_N1][ANIM_N2][ANIM_N3];
				for (i=0; i<ANIM_N1; i++) {
					for (j=0; j<ANIM_N2; j++) {
						for (k=0; k<ANIM_N3; k++) {
							new_npc->sprite_list->sprite_frames[i][j][k].frame = NULL;
						}
					}
				}

				for (i=0; i<NPC_FRAME_NUMBER; i++) {
						if (new_npc->npc_data.frameset[i] != -1) {
							add_sprite(ANIM_LEFT, new_npc->npc_data.frameset[i], i, 0, new_npc->sprite_list, new_npc->npc_data.frameset_time[i], image_surface);
							add_sprite(ANIM_RIGHT, new_npc->npc_data.frameset[i], i, 1, new_npc->sprite_list, new_npc->npc_data.frameset_time[i], image_surface);
						}
				}
			} else {
				printf("ERROR: Could not load graphics from file '%s'.", buffer);
			}
			SDL_FreeSurface(image_surface);
			image_surface = NULL;

		}
		fclose(fp);
	}
}





void loadGame() {
	long lSize;
	char buffer[255];
	sprintf(buffer, "%sdata/game/001.gme", FILEPATH);

	FILE *fp = fopen(buffer, "rb");
	if (fp) {
		fseek (fp , 0 , SEEK_END);
		lSize = ftell (fp);
		rewind (fp);
		fread(&game, sizeof(game), 1, fp);
		loadGameObjects (1);
		fclose(fp);
	} else {
		printf("DEBUG - no game file '%s', ignoring load.\n", buffer);
	}
}


// free all colorcycling variables
void freeColorCycling() {

	SDL_SetColors(game_screen, colormap_original, 0, COLOR_COUNT);
	if (offscreen != NULL) {
		SDL_SetColors(offscreen, colormap_original, 0, COLOR_COUNT);
	}

	restore_colormap();

	struct struct_colorcycle_list *cl_list_temp=NULL, *cl_list_old=NULL;
	struct struct_colorcycle *cl_item_temp=NULL, *cl_item_old=NULL;
	cl_list_temp = colorcycle_list;
	if (cl_list_temp != NULL) {
		while (cl_list_temp) {
			cl_item_temp = cl_list_temp->colorcycle;
			while (cl_item_temp) {
				cl_item_old = cl_item_temp;
				cl_item_temp = cl_item_temp->next;
				free(cl_item_old);
			}
			cl_item_temp = NULL;
			cl_list_temp->colorcycle = NULL;
			cl_list_old = cl_list_temp;
			cl_list_temp = cl_list_temp->next;
			free(cl_list_old);
		}
	}
	move_screen(game_screen, 0, 0);
	colorcycle_list = NULL;
}


// TODO: adapt to be like loadMapObjects
void loadMapNpcs(const int game_n, const int stage_n, const int stage_pos) {
	struct npc_struct *temp_npc, *old_temp_npc, *new_npc;
	int npcCount=0;
	struct file_map_npc temp_map_npc;
	FILE *fp_map_npc = NULL;
	char cent1[4], cent2[4], cent_stage[4], filename[255];

	printf(">> loadMapNpcs - game_n: %d, stage_n: %d, stage_pos: %d <<<\n", game_n, stage_n, stage_pos);

	// free the old npc list
	if (npc_list[stage_pos] != NULL) {
		temp_npc = npc_list[stage_pos];
		while (temp_npc) {
			// no need to clean sprite_list, once those are handled by the main list
			old_temp_npc = temp_npc;
			temp_npc = temp_npc->next;
			free(old_temp_npc);
			old_temp_npc = NULL;
		}
		npc_list[stage_pos] = NULL;
	}



	centNumber(stage_n, cent_stage);
	centNumber(game_n, cent1);
	centNumber(stage_pos, cent2);
	sprintf(filename, "%sdata/game/game_%s/stage_%s/map_%s_npc_list.dat", FILEPATH, cent1, cent_stage, cent2);

	//printf(">> loadMapNpcs::filename: '%s' <<\n", filename);


	fp_map_npc = fopen(filename, "rb");

	if (fp_map_npc) {
		while (!feof(fp_map_npc)) {
			if (fread(&temp_map_npc, sizeof(struct file_map_npc), 1, fp_map_npc) != 1) {
				printf("loadMapNpcs - Could not load npc %d from file '%s' interrupt.\n", npcCount, filename);
				return;
			}

			//printf(">> loadMapNpcs adding NEW npc to stage_pos: %d <<\n", stage_pos);

			// NOTE: this method already add the npc to the map list
			new_npc = add_npc(temp_map_npc.start_point.x*16, temp_map_npc.start_point.y*16, stage_pos);
			if (new_npc == NULL) {
				printf(">> loadMapNpcs.ERROR: Could not create NPC %d <<\n", npcCount);
				return;
			}

			struct npc_static_data_list *found_npc = NULL;
			struct npc_static_data_list *temp_static_npc = NULL;

			temp_static_npc = static_npc_list;
			int temp_id = 0;
			while (temp_static_npc) {
				if (temp_id == temp_map_npc.id) {
					found_npc = temp_static_npc;
					break;
				}
				temp_id++;
				temp_static_npc = temp_static_npc->next;
			}

			if (found_npc == NULL) {
				printf(">> ERROR: Could not find NPC %d in NPC main list <<\n", temp_map_npc.id);
				continue;
			}

			new_npc->is_boss = found_npc->npc_data.is_boss;
			if (new_npc->is_boss) {
				new_npc->HP = INITIAL_HP;
			} else {
				new_npc->HP = found_npc->npc_data.hp.total;
			}

			new_npc->TOTAL_HP = new_npc->HP;
			new_npc->shield = found_npc->npc_data.have_shield;
			new_npc->IA = found_npc->npc_data.IA;
			new_npc->shots = 0;
			new_npc->can_shoot = found_npc->npc_data.can_shoot;
			new_npc->direction = found_npc->npc_data.direction;

			new_npc->speed = found_npc->npc_data.speed;
			new_npc->walk_range = found_npc->npc_data.walk_range;

			if (new_npc->direction != ANIM_LEFT && new_npc->direction != ANIM_RIGHT) {
				new_npc->direction = ANIM_LEFT;
			}
			sprintf(new_npc->name, "%s", found_npc->npc_data.name);
			new_npc->sprite_list = found_npc->sprite_list;
			new_npc->timer = 0;
			//printf(">> INFO: added NPC %d (%s) into stage: %d and map %d <<\n", temp_map_npc.id, stage_n, stage_pos);

			npcCount++;
		}
		fclose(fp_map_npc);
	} else {
		printf("DEBUG.EditorArea::saveMap.exportNPCList - ERROR opening filename: %s\n", filename);
	}
}


void loadMapObjects(const int game_n, const int stage_n, const int stage_pos) {
	char cent1[4];
	char cent2[4];
	char cent_stage[4];
	char filename[255];
	struct struct_object *new_obj;
	struct stFileObject tempFileObj;
	struct struct_object *temp_obj_list, *old_obj_list;
	struct struct_object *temp_main_obj_list=NULL, *found_main_obj=NULL;

	centNumber(game_n, cent1);
	centNumber(stage_pos, cent2);
	centNumber(stage_n, cent_stage);
	sprintf(filename, "%sdata/game/game_%s/stage_%s/%s.map.obj", FILEPATH, cent1, cent_stage, cent2);

	// first empty object list
	printf(">> WARNING::loadMapObjects - cleaning map_obj list for stage_pos: %d <<\n", stage_pos);
	temp_obj_list = obj_list[stage_pos];
	if (temp_obj_list != NULL) {
		while (temp_obj_list) {
			old_obj_list = temp_obj_list;
			temp_obj_list = temp_obj_list->next;
			SDL_FreeSurface(old_obj_list->sprite);
			free(old_obj_list);
		}
	}
	obj_list[stage_pos] = NULL;

	FILE* fp = fopen(filename, "rb");

	if (fp) {
		printf(">>> WARNING: opened map-objects file '%s' for reading <<<\n", filename);
		rewind (fp);

		int n = 1;

		while (!feof(fp)) {

			temp_main_obj_list=NULL;
			found_main_obj=NULL;

			if (fread(&tempFileObj, sizeof(struct stFileObject), 1, fp) != 1) {
				printf("*** loadMapObjects: Error loading objectnumber  %d from file, finishing read loop ***\n", n);
				break;
			}

			temp_main_obj_list = main_obj_list;
			found_main_obj = NULL;

			while (temp_main_obj_list) {
				if (strstr(tempFileObj.id, temp_main_obj_list->id)) {
					found_main_obj = temp_main_obj_list;
					break;
				}
				temp_main_obj_list = temp_main_obj_list->next;
			}
			if (!found_main_obj) {
				printf("*** ERROR: Object with id %d not found in the main Object list ***\n", tempFileObj.id);
				break;
			}

			new_obj = (struct struct_object *)malloc(sizeof(struct struct_object));
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
			new_obj->direction = ANIM_LEFT;

			// copy infomation from file struct to logic object structure


			sprintf(new_obj->filename, "%s", found_main_obj->filename);
			new_obj->frame = found_main_obj->frame;
			new_obj->framesize_h = found_main_obj->framesize_h;
			new_obj->framesize_w = found_main_obj->framesize_w;
			sprintf(new_obj->id, "%s", found_main_obj->id);
			new_obj->limit = found_main_obj->limit;
			sprintf(new_obj->name, "%s", found_main_obj->name);
			new_obj->speed = found_main_obj->speed;
			new_obj->start_point.x = tempFileObj.start_point.x;
			new_obj->start_point.y = tempFileObj.start_point.y;
			new_obj->type = found_main_obj->type;
			new_obj->next = NULL;

			new_obj->position.x = new_obj->start_point.x * TILE_SIZE;
			new_obj->position.y = new_obj->start_point.y * TILE_SIZE;


			if (new_obj->type != -1 && strstr(new_obj->filename, ".png") != NULL) {
				sprintf(filename, "%sdata/images/sprites/objects/%s", FILEPATH, new_obj->filename);
				new_obj->sprite = surface_region_from_image(filename, new_obj->framesize_w, new_obj->framesize_h);
				if (new_obj->sprite == NULL) {
					//printf("*** loadMap - error creating sprite for object, filename: %s ***\n", new_obj->filename);
					new_obj->type = -1;
				}
			} else {
				printf("*** ERROR: Object does not have an image associated with it. Image-File: '%s' ***\n", new_obj->filename);
			}



			if (obj_list[stage_pos] == NULL) {
				obj_list[stage_pos] = new_obj;
			} else {
				temp_obj_list = obj_list[stage_pos];
				while (temp_obj_list->next != NULL) {
					temp_obj_list = temp_obj_list->next;
				}
				temp_obj_list->next = new_obj;
			}
			n++;
		}
		fclose(fp);
	} else {
		printf(">>> ERROR: could not open map objects file '%s' <<<\n", filename);
	}
}

void loadMap(const int game_n, const int map_n) {
	int i, j;
	int column_locked = 1;

	printf("*************** WARNING: loadMap - loading map %d <<\n", map_n);
	map = stage_maps[map_n];
	for (i=0; i<MAP_W; i++) {
		column_locked = 1;
		for (j=0; j<MAP_H; j++) {
			if (map.tiles[i][j].locked != 1 && map.tiles[i][j].locked != TERRAIN_DOOR && map.tiles[i][j].locked != TERRAIN_SCROLL_LOCK) {
				column_locked = 0;
				break;
			}
		}
		wall_scroll_lock[i] = column_locked;
	}
	currentMap = map_n;
	currentGame = game_n;
	load_colorcycle(map_n);
	drawMap(offscreen, 0);
}

void loadStage(const int game_n, const int stage_n) {
	int k;
	long lSize;
	char cent1[4];
	char cent2[4];
	char cent3[4];
	char filename[255];
	int map_n;
	char debug_buffer[255];
	FILE *fp;

	restore_colormap();
	SDL_SetColors(tileset, colormap, 0, COLOR_COUNT);
	freeColorCycling(); //free old colorcycling

	for (k=0; k<3; k++) {
		map_n = stage_n + 9*k;

		draw_text(MAX_W*0.5-45-RES_DIFF_W, 12*(k+1), debug_buffer, game_screen);

		centNumber(game_n, cent1);
		centNumber(stage_n, cent2);
		centNumber(k, cent3);
		sprintf(filename, "%sdata/game/game_%s/stage_%s/%s_map.dat", FILEPATH, cent1, cent2, cent3);


		fp = fopen(filename, "rb");

		if (fp) {
			fseek (fp , 0 , SEEK_END);
			lSize = ftell (fp);
			rewind (fp);
			fread(&stage_maps[k], sizeof(struct file_map_v1), 1, fp);
			fclose(fp);
			fp = NULL;
			// TODO - this methods must receive the new map_n format (0, 1, 2)
			loadMapObjects(game_n, stage_n, k);
			loadMapNpcs(game_n, stage_n, k);
			currentStage = stage_n;
		} else {
			printf(">> ERROR: loadStage - Could not load map '%s' <<\n", filename);
			fp = NULL;
		}
	}
	loadMap(game_n, 0);
}





void loadTempMap(const int game_n, const int map_n) {
	char cent1[4];
	char cent2[4];
	char filename[255];

	centNumber(game_n, cent1);
	centNumber(map_n, cent2);
	sprintf(filename, "%sdata/game/game_%s/%s.map", FILEPATH, cent1, cent2);
	FILE *fp = fopen(filename, "rb");
	if (fp) {
		fseek (fp , 0 , SEEK_END);
		rewind (fp);
		fread(&temp_map, sizeof(struct file_map_v1), 1, fp);
		fclose(fp);
	} else {
		printf("ERROR - no map file, ignoring load.\n");
	}
}


void freeMap(struct file_map_v1 my_map) {
	int j, k;

	//sprintf(map.name, " ");
	//my_map.bosss_door_x=999;
	sprintf(map.filename, "%sdata/images/tilesets/default.png", FILEPATH);
	for (j=0; j<MAP_W; j++) {
		for (k=0; k<MAP_H; k++) {
			my_map.tiles[j][k].locked=0;
			my_map.tiles[j][k].tile1.x=-1;
			my_map.tiles[j][k].tile1.y=-1;
			my_map.tiles[j][k].tile2.x=-1;
			my_map.tiles[j][k].tile2.y=-1;
		}
	}
	/*
	for (j=0; j<STAGE_NPC_LIMIT; j++) {
		map.npc_list[j]
	}
	*/

}

void drawMap(SDL_Surface *screen, const short int adjust) {
	short int i, j, real_quad_x;
	//printf("DEBUG.drawMap 1\n");
	extern SDL_Surface *tileset;


	for (i=0; i<MAP_W; i++) {
		for (j=0; j<MAP_H; j++) {
			real_quad_x = i + adjust;
			if (map.tiles[i][j].tile1.x != -1 && map.tiles[i][j].tile1.y != -1) {
				if (!tileset) {
					//printf("DEBUG.drawMap - no tileset");
				}
				if (!screen) {
					//printf("DEBUG.drawMap - no screen");
				}
				if (!offscreen) {
					//printf("DEBUG.drawMap - no offscreen");
				}
				copy_area(tileset, map.tiles[i][j].tile1.x*TILE_SIZE, map.tiles[i][j].tile1.y*TILE_SIZE, TILE_SIZE, TILE_SIZE, real_quad_x*TILE_SIZE+TILE_SIZE, j*TILE_SIZE+TILE_SIZE, offscreen);
			}
			if (map.tiles[i][j].tile2.x != -1 && map.tiles[i][j].tile2.y != -1) {
				copy_area(tileset, map.tiles[i][j].tile2.x*TILE_SIZE, map.tiles[i][j].tile2.y*TILE_SIZE, TILE_SIZE, TILE_SIZE, real_quad_x*TILE_SIZE+TILE_SIZE, j*TILE_SIZE+TILE_SIZE, offscreen);
			}
		}
	}
	copy_area_debug(screen, 0, 0, MAP_W*TILE_SIZE, MAP_H*TILE_SIZE, 0, 0, offscreen);
}

void drawMap3rdLevel(SDL_Surface *screen) {
	extern SDL_Surface *tileset;
	int i, j;

	for (i=0; i<MAP_W; i++) {
		for (j=0; j<MAP_H; j++) {
			if (map.tiles[i][j].tile3.x != -1 && map.tiles[i][j].tile3.y != -1) {
				// must check if the position is visible in the screen
				//printf(">> common.h::drawMap3rdLevel - scrollX: %d, scrollY: %d, posX: %d, posY: %d, MAX_W: %d\n", scrollX, scrollY, (i*TILE_SIZE), (j*16),MAX_W);
				//printf(">> common.h::drawMap3rdLevel - scrollX: %d, map_pos_x:  %d\n", scrollX, map_pos_x);
				if (i*TILE_SIZE >= -map_pos_x-TILE_SIZE && i*TILE_SIZE <= -map_pos_x+MAX_W+TILE_SIZE) {
					//printf(">> common.h::drawMap3rdLevel - X OK\n");
					if (j*TILE_SIZE >= scrollY-TILE_SIZE && j*TILE_SIZE <= scrollY+MAX_H+TILE_SIZE) {
						//printf(">> common.h::drawMap3rdLevel - Y OK\n");
						copy_area(tileset, map.tiles[i][j].tile3.x*TILE_SIZE, map.tiles[i][j].tile3.y*TILE_SIZE, TILE_SIZE, TILE_SIZE, (i*TILE_SIZE)+map_pos_x, (j*TILE_SIZE)+map_pos_y, screen);
					}
				}
				//copy_area(tileset, map.tiles[i][j].tile2.x*TILE_SIZE, map.tiles[i][j].tile2.y*TILE_SIZE, TILE_SIZE, TILE_SIZE, i*TILE_SIZE, j*TILE_SIZE, offscreen);
			}
		}
	}

}


// set default values for game variable
void initGameVar() {
	short int j, k;
	strcpy (game.name, "My Game");
	//sprintf(map.name, "Map 00%d", 1);
	//map.bosss_door_x=200;
	sprintf(map.filename, "%sdata/images/tilesets/default.png", FILEPATH);
	for (j=0; j<MAP_W; j++) {
		for (k=0; k<MAP_H; k++) {
			map.tiles[j][k].locked=0;
			map.tiles[j][k].tile1.x=-1;
			map.tiles[j][k].tile1.y=-1;
			map.tiles[j][k].tile2.x=-1;
			map.tiles[j][k].tile2.y=-1;
		}
	}
}


void free_animationList(int free_frames) {
    struct animation_sprite_struct *temp_anim, *old_temp_anim;
    // free animation list
    temp_anim = animation_list;
    if (temp_anim != NULL) {
            while (temp_anim != NULL) {
                    old_temp_anim = temp_anim;
                    temp_anim = temp_anim->next;
                    if (free_frames == 1) {
                        if (old_temp_anim->frame->tile != NULL) {
                                free(old_temp_anim->frame->tile);
                                old_temp_anim->frame->tile = NULL;
                        }
                    }
                    free(old_temp_anim);
            }
    }
    animation_list = NULL;
}

// free variables
void leave_game() {
	int i, j, k;
	struct player_struct *temp, *temp_old;
	struct npc_struct *ntemp, *ntemp_old;

	unload_faces();
	upload_stage_select();

	// free player list
	temp = player_list;
	if (temp != NULL) {
		while (temp != NULL) {
			temp_old = temp;
			temp = temp->next;
			// free player sprite surfaces
			for (i=0; i<ANIM_N1; i++) {
				for (j=0; j<ANIM_N2; j++) {
					for (k=0; k<ANIM_N3; k++) {
						if (temp_old->sprite->sprite_frames[i][j][k].frame) {
							SDL_FreeSurface(temp_old->sprite->sprite_frames[i][j][k].frame);
						}
					}
				}
			}
			free(temp_old);
		}
	}
	free_animationList(1);
	// free projectiles
	SDL_FreeSurface(normal_projectile);
	if (keyState[0] != NULL) {
		free(keyState[0]);
	}
	if (keyState[1] != NULL) {
		free(keyState[1]);
	}

	// empty object list
	struct struct_object *temp_obj_list, *old_obj_list;
	for (i=0; i<3; i++) {
		temp_obj_list = obj_list[i];
		if (temp_obj_list != NULL) {
			while (temp_obj_list) {
				old_obj_list = temp_obj_list;
				temp_obj_list = temp_obj_list->next;
				SDL_FreeSurface(old_obj_list->sprite);
				free(old_obj_list);
			}
		}
	}


	// empty MAIN object list
	struct struct_object *temp_main_obj_list, *old_main_obj_list; // object list from .obj files
	temp_main_obj_list = main_obj_list;
	if (temp_main_obj_list != NULL) {
		while (temp_main_obj_list) {
			old_main_obj_list = temp_main_obj_list;
			temp_main_obj_list = temp_main_obj_list->next;
			free(old_main_obj_list);
		}
	}

	// empty npc map list
	for (i=0; i<3; i++) {
		ntemp = npc_list[i];
		if (ntemp != NULL) {
			while (ntemp != NULL) {
				if (ntemp->icon != NULL) {
					SDL_FreeSurface(ntemp->icon);
				}
				ntemp_old = ntemp;
				ntemp = ntemp->next;
				free(ntemp_old);
			}
		}
	}

	/*
	// empty MAIN npc list
	struct npc_static_data_list *temp_main_npc_list, *old_main_npc_list; // object list from .obj files
	temp_main_npc_list = main_obj_list;
	if (temp_main_npc_list != NULL) {
		while (temp_main_npc_list) {
			old_main_npc_list = temp_main_npc_list;
			temp_main_npc_list = temp_main_npc_list->next;
			// free NPC sprite surfaces
			if (old_main_npc_list->sprite_list) {
				for (i=0; i<ANIM_N1; i++) {
					for (j=0; j<ANIM_N2; j++) {
						for (k=0; k<ANIM_N3; k++) {
							// TODO: this area is crashing, needs to be fixed or will cause memory leaks
							if (old_main_npc_list->sprite_list->sprite_frames[i][j][k].frame != NULL) {
								SDL_FreeSurface(old_main_npc_list->sprite_list->sprite_frames[i][j][k].frame);
								old_main_npc_list->sprite_list->sprite_frames[i][j][k].frame = NULL;
							}
						}
					}
				}
				// TODO: this area is crashing, needs to be fixed or will cause memory leaks
				//free(old_main_npc_list->sprite_list);
				old_main_npc_list->sprite_list = NULL;
			}
			old_main_npc_list->sprite_list = NULL;
			if (old_main_npc_list != NULL) {
				// TODO: this area is crashing, needs to be fixed or will cause memory leaks
				//free(old_main_npc_list);
			}
			old_main_npc_list = NULL;
		}
	}
	*/


	SDL_FreeSurface(tileset);
	SDL_FreeSurface(game_screen);
	SDL_FreeSurface(offscreen);
	TTF_CloseFont(font);
	close_audio();
	TTF_Quit();
	SDL_QuitSubSystem(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK);


	// free stars surfaces
	if (surface_star_big != NULL) { SDL_FreeSurface(surface_star_big); }
	if (surface_star_bg_big != NULL) { SDL_FreeSurface(surface_star_bg_big); }
	if (surface_star_small != NULL) { SDL_FreeSurface(surface_star_small); }
	if (surface_star_bg_small != NULL) { SDL_FreeSurface(surface_star_bg_small); }

	exit(-1);
}

void init_sprite_vars(struct sprite_struct *sprite_list) {
	short int i, j, k;

	sprite_list->frame_pos[0]=0;
	sprite_list->frame_pos[1]=0;
	sprite_list->frame_timer=0;
	sprite_list->frame_animation[0]=0;
	sprite_list->frame_animation[1]=0;
	sprite_list->visible=1;
	// new animation system
	sprite_list->direction=0;
	sprite_list->anim_type=0;
	sprite_list->anim_pos=0;

	for (i=0; i<ANIM_N1; i++) {
		for (j=0; j<ANIM_N2; j++) {
			for (k=0; k<ANIM_N3; k++) {
				sprite_list->sprite_frames[i][j][k].frame=NULL;
				sprite_list->sprite_frames[i][j][k].duration=0;
				sprite_list->sprite_frames[i][j][k].init = 0;
			}
		}
	}
	sprite_list->anim_status.move_type = ANIM_STAND;
	for (i=0; i<ANIM_N3; i++) {
		sprite_list->anim_status.move_status[i]=0;
		sprite_list->anim_status.move_frame[i]=0;
	}
}

// ----------------------------------------------- ANIMATION ----------------------------------------------------- //

struct animation_frame *create_animation_frame(char filename[50], const short int w, const short int h) {
	struct animation_frame *new_frame;
	SDL_RWops *rwop;

	new_frame = (struct animation_frame*)malloc(sizeof(struct animation_frame));
	new_frame->duration=100;
	new_frame->w = w;
	new_frame->h = h;
	rwop=SDL_RWFromFile(filename, "rb");
	if (!rwop) {
		printf("Error in create_sprite: '%s' file not found\n", filename);
		exit(-1);
	}
	new_frame->tile = IMG_Load_RW(rwop, 1);
	return new_frame;
}


struct animation_frame *create_animation_frame_from_sdlarea(const short int w, const short int h) {
	struct animation_frame *new_frame;
	new_frame = (struct animation_frame*)malloc(sizeof(struct animation_frame));
	new_frame->duration=100;
	new_frame->w = w;
	new_frame->h = h;
	new_frame->tile = SDL_CreateRGBSurface(SDL_SWSURFACE , 30, 30, 8, 0, 0, 0, 0);
	return new_frame;
}


struct animation_sprite_struct *create_animation(short int *x, short int *y, struct animation_frame *frame, int diffx, int diffy) {
	struct animation_sprite_struct *new_anim, *temp;
	new_anim = (struct animation_sprite_struct*)malloc(sizeof(struct animation_sprite_struct));

	new_anim->x = x;
	new_anim->y = y;
	if (diffx > frame->tile->w) {
		new_anim->diffx = diffx;
		new_anim->w = diffx*2;
	} else {
		new_anim->diffx = 0;
		new_anim->w = frame->tile->w;
	}
	if (diffy > frame->tile->h) {
		new_anim->diffy = diffy;
		new_anim->h = diffy*2;
	} else {
		new_anim->diffy = 0;
		new_anim->h = frame->tile->h;
	}
	new_anim->animation_pos = 0;

	new_anim->animation_loops = 1;
	new_anim->init = 0;
	new_anim->animation_loop_count = 1;
	new_anim->next = NULL;
	new_anim->frame = frame;


	// the max animation frames is 5, but we must check is less than that by the image total width
	if (animation_list == NULL) {
		animation_list = new_anim;
	} else {
		temp = animation_list;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new_anim;
	}
	return new_anim;
}



void init_main_vars() {
	int i, j, k;
	game_config.two_players = 1;
	game_config.selected_player = 2;
	save_001.finished_stages = 0;
	for (i=0; i<=DIALOG_MAX_STAGE; i++) {
		save_001.stages[i] = 0;
	}
	save_001.items.energy_tanks = 0;
	save_001.items.weapon_tanks = 0;
	save_001.items.lifes = 3;

	checkpoint.map = 0;
	checkpoint.x = 50;
	checkpoint.y = -1;
	checkpoint.map_pos_x = 0;

	// HARDCODED VARS - BEGIN
	for (i=0; i<8; i++) {
		skull_castle_bosses[i].finished = 0;
		skull_castle_bosses[i].active = 0;
		skull_castle_bosses[i].old_map_pos_x = 0;
		skull_castle_bosses[i].old_player_pos.x = 0;
		skull_castle_bosses[i].old_player_pos.y = 0;
	}
	skull_castle_bosses[0].teleporter_n = 58;
	skull_castle_bosses[1].teleporter_n = 59;
	skull_castle_bosses[2].teleporter_n = 60;
	skull_castle_bosses[3].teleporter_n = 61;
	skull_castle_bosses[4].teleporter_n = 62;
	skull_castle_bosses[5].teleporter_n = 63;
	skull_castle_bosses[6].teleporter_n = 64;
	skull_castle_bosses[7].teleporter_n = 65;

	weapon_matrix[0][0] = APEBOT;
	weapon_matrix[0][1] = DAISIEBOT;
	weapon_matrix[0][2] = DYNAMITEBOT;
	weapon_matrix[0][3] = MUMMYBOT;
	weapon_matrix[0][4] = SPIKEBOT;

	weapon_matrix[1][0] = TECHNOBOT;
	weapon_matrix[1][1] = MAGEBOT;
	weapon_matrix[1][2] = SEAHORSEBOT;
	weapon_matrix[1][3] = APEBOT; // coil
	weapon_matrix[1][4] = MAGEBOT; // jet

	for (i=0; i<INTRO_SURFACES_COUNT; i++) {
		INTRO_SURFACES[i] = NULL;
	}
	for (i=0; i<FACES_COUNT; i++) {
		FACES_SURFACES[i] = NULL;
	}

	for (i=0; i<3; i++) {
		printf(">> set obj_list NULL 1 <<\n");
		obj_list[i] = NULL;
		npc_list[i] = NULL;

		//sprintf(stage_maps[i].name, "%s", "\0");
		sprintf(stage_maps[i].filename, "%s", "\0");
		//stage_maps[i].bosss_door_x = 0;
		for (j=0; j<MAP_W; j++) {
			for (k=0; k<MAP_H; k++) {
				stage_maps[i].tiles[j][k].tile1.x = 0;
				stage_maps[i].tiles[j][k].tile1.y = 0;
				stage_maps[i].tiles[j][k].tile2.x = 0;
				stage_maps[i].tiles[j][k].tile2.y = 0;
				stage_maps[i].tiles[j][k].tile3.x = 0;
				stage_maps[i].tiles[j][k].tile3.y = 0;
			}
		}
/*
		for (j=0; j<STAGE_NPC_LIMIT; j++) {
			stage_maps[i].npc_list[j].hp.total = -1;
			stage_maps[i].npc_list[j].hp.current = -1;
			sprintf(stage_maps[i].npc_list[j].filename, "%s", "\0");
			sprintf(stage_maps[i].npc_list[j].name, "%s", "\0");
		}
*/
	}



	// HARDCODED VARS - END
}


struct links_list *add_map_link(void) {
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


void load_map_links(void) {
	struct links_list *new_link = NULL;
	struct map_link_v1 link;
	FILE *fp_links;
	char filename[255];

	printf(">>>>>>> load_map_links::START <<<<<<<\n");

	sprintf(filename, "%sdata/game/game_001/map_links.dat", FILEPATH);
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

			//printf(">> load_map_links - added link - origin: %d, destiny: %d, stage: %d, type: %d <<\n", new_link->link.map_origin, new_link->link.map_dest, new_link->link.stage, new_link->link.link_type);

		}
		fclose(fp_links);
	} else {
		printf(">> ERROR: Could not load map_links file '%s' <<\n", filename);
		return;
	}


}

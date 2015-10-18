#include "input.h"
#include "npcs.h"

short int player_movement(struct player_struct *playerObj, int *myKeyState);
void return_to_stage_select(void);


void damagePlayer(struct player_struct *playerObj, int damage) {
	//printf(">> damagePlayer - damage: %d <<\n", damage);

	if (damage < 999 && playerObj->hit == 1 && playerObj->hit_timer > getTimer()) {
		return;
	} else if (playerObj->hit == 1 && playerObj->hit_timer < getTimer()) {
		playerObj->hit = 0;
	}

	playerObj->hit = 1;
	playerObj->hit_timer = getTimer()+HIT_INVENCIBLE_TIME;
	playerObj->HP -= damage;
	playerObj->sprite->anim_type = ANIM_HIT;
	playerObj->sprite->frame_animation[ANIM_PROCESS] = 0;
	int diffx = (playerObj->sprite->w - 16) *0.5;
	int diffy = (playerObj->sprite->h - 16) *0.5;
	create_hit_animation(&playerObj->sprite->x, &playerObj->sprite->y, diffx, diffy);
	play_sfx(SFX_PLAYER_HIT);

	if (playerObj->HP <= 0) {
		playerObj->hit = 0;
		playerObj->hit_timer = 0;
		//printf(">>>>>>>>>>>> death, lifes: %d<<\n", playerObj->items.lifes);
		free_animationList(0);
		play_sfx(SFX_PLAYER_DEATH);
		drawExplosion(playerObj->sprite->x+map_pos_x, playerObj->sprite->y+map_pos_y, 0);
		if (playerObj->items.lifes > 0) {
			//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> reduce lifes from %d", playerObj->items.lifes);
			playerObj->items.lifes--;
			//printf(" to %d <<\n", playerObj->items.lifes);
		} else { // TODO - add support for 2 players mode (the one that still have lifes must continue
			return_to_stage_select();
			return;
		}

		if (game_config.two_players == 2) {
			if (playerObj == p1Obj) {
				p1Obj->sprite->x = p2Obj->sprite->x;
			} else {
				p2Obj->sprite->x = p1Obj->sprite->x;
			}
			playerObj->sprite->y = TILE_SIZE;
			playerObj->x = 80;
			playerObj->y = TILE_SIZE;
			playerObj->HP = INITIAL_HP;
		} else {
			// reload map - hide player sprite
			remove_all_projectiles();
			// TODO: remove all hit animations also
			move_screen(game_screen, 0, 0);
			anim_npcs();
			anim_objects();
			// redraw

			show_animation(game_screen);
			drawMap3rdLevel(game_screen);
			draw_hp_bar(p1Obj);
			playerObj->sprite->x = checkpoint.x;
			playerObj->x = checkpoint.x;
			if (checkpoint.y == -1) {
				playerObj->sprite->y = TILE_SIZE;
				playerObj->y = TILE_SIZE;
			} else {
				playerObj->sprite->y = checkpoint.y;
				playerObj->y = checkpoint.y;
			}
			playerObj->HP = INITIAL_HP;
			playerObj->sprite->anim_type = ANIM_TELEPORT;

			scrollX = 0;
			map_pos_x = 0;
			map_pos_y = 0;
			scrollY = 0;

			blank_screen(game_screen);

			if (checkpoint.map != currentMap) {
				loadMap(1, checkpoint.map);
			}
			map_pos_x = checkpoint.map_pos_x;
			map_pos_y = 0;
			move_screen(game_screen, 0, 0);
			play_music();
			showReadyAnimation();
		}
	}
}


// check colision of Player with NPCs
short int colision_player_npc(struct player_struct *playerObj, const short int x_inc, const short int y_inc) {
	struct npc_struct *temp_npc;
	struct pos p1, p2, p3, p4;
	struct pos npc1, npc2, npc3, npc4;
	short int i;
	temp_npc = npc_list[currentMap];
	short int passX=0, passY=0;

	short int reduce = abs(TILE_SIZE-playerObj->sprite->w)*0.5;
	short int diff_h = playerObj->sprite->h - PLAYER_H;

	p1.x = playerObj->sprite->x + x_inc + reduce;
	p1.y = playerObj->sprite->y + y_inc + diff_h;

	// ponto 2, baixo/esquerda
	p2.x = playerObj->sprite->x + x_inc + reduce;
	p2.y = playerObj->sprite->y + y_inc + PLAYER_H;

	// ponto 3, topo/direita
	p3.x = playerObj->sprite->x + x_inc + playerObj->sprite->w - reduce;
	p3.y = playerObj->sprite->y + y_inc + diff_h;

	// ponto 4, baixo/direita
	p4.x = playerObj->sprite->x + x_inc + playerObj->sprite->w - reduce;
	p4.y = playerObj->sprite->y + y_inc + PLAYER_H;

	// fix for ground checking
	p1.y = p1.y - 1;
	p2.y = p2.y - 1;
	p3.y = p3.y - 1;
	p4.y = p4.y - 1;


	i = 2;
	if (temp_npc != NULL) {
		while (temp_npc != NULL) {
			if (temp_npc->sprite_list != NULL && temp_npc->HP > 0) {
				npc1.x = temp_npc->x;
				npc1.y = temp_npc->y;
				npc2.x = temp_npc->x;
				npc2.y = temp_npc->y+temp_npc->sprite_list->h;
				npc3.x = temp_npc->x+temp_npc->sprite_list->w;
				npc3.y = temp_npc->y;
				npc4.x = temp_npc->x+temp_npc->sprite_list->w;
				npc4.y = temp_npc->y+temp_npc->sprite_list->h;
				// any point inside NPC is a hit
				//printf("--------> colision_player_npc - playerObj->hit_timer: %d\n", playerObj->hit_timer);
				passY = 0;
				passX = 0;
				// check Y
				if ((p1.y >= npc1.y && p1.y <= npc4.y) || (p4.y >= npc1.y && p4.y <= npc4.y)) {
					passY=1;
				} else if ((npc1.y >= p1.y && npc1.y <= p4.y) || (npc4.y >= p1.y && npc4.y <= p4.y)) {
					passY=1;
				}

				if ((p1.x >= npc1.x && p1.x <= npc4.x) || (p4.x >= npc1.x && p4.x <= npc4.x)) {
					passX=1;
				} else if ((npc1.x >= p1.x && npc1.x <= p4.x) || (npc4.x >= p1.x && npc4.x <= p4.x)) {
					passX=1;
				}
				if (playerObj->HP > 0 && passX == 1 && passY == 1) {
					damagePlayer(playerObj, 3);
					return 1;
				}
			}
			i++;
			temp_npc = temp_npc->next;
		}
	}
	return 0;
}



short int checkDelay(struct player_struct *playerObj) {
    long int now_time=0;
    now_time = getTimer();
    if (now_time > playerObj->sprite->move_timer) {
            playerObj->sprite->move_timer=now_time+17;
            return 1;
    }
    return 0;
}



short int execute_attack(struct player_struct *pObj, int *myKeyState) {
	int res1, res2;
	if (myKeyState[KEYATTACK]== 1 && pObj->attackButtonReleased==0) {
		// ITEM JUMP
		if (pObj->selected_weapon == ITEM_COIL && pObj->items.weapons[pObj->selected_weapon] > 0) {
			//printf(">>>>>>>>>>>> execute_attack - ITEM_COIL <<<<<<<<<<<<<<<<<\n");
			if (pObj->platform == NULL) {
				int x_adjust;
				if (pObj->sprite->direction == ANIM_LEFT) {
					x_adjust = pObj->sprite->x - (TILE_SIZE * 2);
				} else {
					x_adjust = pObj->sprite->x + (TILE_SIZE * 2);
				}
				//printf(">>>>>>>>>>>> execute_attack - ITEM_COIL - DROP <<<<<<<<<<<<<<<<<\n");
				pObj->platform = dropItem(OBJ_ITEM_JUMP, x_adjust, pObj->sprite->y+TILE_SIZE);
				pObj->platform->direction = pObj->sprite->direction;
				printf(">> ITEM_COIL - player.platform: %p, item.direction: %d <<\n", pObj->platform, pObj->platform->direction);
			} else {
				return 0;
			}
		// ITEM JET
		} else if (pObj->selected_weapon == ITEM_JET && pObj->items.weapons[pObj->selected_weapon] > 0) {
			if (pObj->platform == NULL) {
				int x_adjust = 0;
				if (pObj->sprite->direction == ANIM_LEFT) {
					x_adjust = pObj->sprite->x - (TILE_SIZE * 2);
				} else {
					x_adjust = pObj->sprite->x + (TILE_SIZE * 2);
				}
				pObj->platform = dropItem(OBJ_ITEM_FLY, x_adjust, pObj->sprite->y+TILE_SIZE);
				pObj->platform->direction = pObj->sprite->direction;
			} else {
				return 0;
			}
		} else {
			res1 = throw_projectile_player(pObj, 0, 0);
			if (pObj->selected_weapon == -1 && ((game_config.two_players == 2 && pObj == p2Obj) || game_config.selected_player == 2)) {
				if (pObj->sprite->direction) {
					res2 = throw_projectile_player(pObj, -TILE_SIZE, 6);
				} else {
					res2 = throw_projectile_player(pObj, TILE_SIZE, 6);
				}
				if (res2) {
					play_sfx(SFX_PLAYER_SHOT);
				}
			}

			if (res1) {
				pObj->attackButtonReleased=1;
				play_sfx(SFX_PLAYER_SHOT);
				return 1;
			}
		}
	}
    return 0;
}


/*********** checks if there is a need to scroll the background **********/
void checkScrolling(const short int x_inc, const short int y_inc, struct player_struct *playerObj1) {
	int mapScrollX=0;
	int moveX=0, moveY=0;
	struct player_struct *playerObj2;

	if (playerObj1 == p1Obj) {
		playerObj2 = p2Obj;
	} else {
		playerObj2 = p1Obj;
	}

	if (scrollX >= 0) {
		mapScrollX = scrollX;
	} else {
		mapScrollX = MAP_W*TILE_SIZE -abs(scrollX) - MAX_W;
	}



	if (game_config.two_players == 2) {
		if (x_inc < 0) {
			if ((playerObj2->sprite->x+playerObj2->sprite->w-abs(map_pos_x)) >= MAX_W) {
				move_screen(game_screen, moveX, moveY);
				return;
			}
		}
	}

	// test if map is all locked in the column most to the right
	if (x_inc > 0) {
		//printf(">>>> moving to right, map_pos_x: %d, scrollX: %d, tile-0: %d, tile-end: %d\n", map_pos_x, scrollX, (abs(map_pos_x)/TILE_SIZE), (abs(map_pos_x)/TILE_SIZE+RES_W/TILE_SIZE));
		if (wall_scroll_lock[(abs(map_pos_x)/TILE_SIZE+RES_W/TILE_SIZE)-1] == 1) {
			move_screen(game_screen, moveX, moveY);
			return;
		}
	} else {
		if (wall_scroll_lock[(abs(map_pos_x)/TILE_SIZE)+1] == 1) {
			move_screen(game_screen, moveX, moveY);
			return;
		}
	}


	if (game_config.two_players == 2) {
		if (playerObj1->sprite->x+MAX_W*0.5+1 > (MAP_W*TILE_SIZE) || playerObj2->sprite->x+MAX_W*0.5+1 > (MAP_W*TILE_SIZE)) {
			move_screen(game_screen, moveX, moveY);
			return;
		}
		if (x_inc > 0 && playerObj1->sprite->x + map_pos_x >= RES_W*0.5 && (playerObj2->sprite->x+map_pos_x) > x_inc) {
			moveX = (x_inc)*-1;
		}
		if (x_inc < 0 && (playerObj1->sprite->x + map_pos_x) < RES_W*0.5 && map_pos_x < 0 && playerObj2->sprite->x+map_pos_x < (RES_W-playerObj2->sprite->w)) {
			moveX = (x_inc)*-1;
		}
		if (y_inc < 0 && (playerObj1->sprite->y + map_pos_y) < MAX_H*0.16666 && map_pos_y < 0 && playerObj2->sprite->y+map_pos_y+TILE_SIZE < (MAX_H-playerObj2->sprite->h)) {
			moveY = (y_inc)*-1;
		}
		if (y_inc > 0 && (playerObj1->sprite->y + map_pos_y) >= MAX_H-48 && ((map_pos_y*-1)+MAX_H+8 <= MAP_H*TILE_SIZE) && (playerObj2->sprite->y+map_pos_y > y_inc)) {
			moveY = (y_inc)*-1;
		}
		if (x_inc > 0 && playerObj1->sprite->x + map_pos_x >= RES_W*0.5 && playerObj2->sprite->x+map_pos_x > x_inc) {
			moveX = (x_inc)*-1;
		} else if (x_inc < 0 && (playerObj1->sprite->x + map_pos_x) < RES_W*0.5 && (map_pos_x < 0) && (playerObj1->sprite->x+map_pos_x < RES_W-playerObj2->sprite->w)) {
			moveX = (x_inc)*-1;
		}
	} else {
		if (playerObj1->sprite->x+RES_W*0.5+1 > (MAP_W*TILE_SIZE)) {
			move_screen(game_screen, moveX, moveY);
			return;
		}
		if (x_inc > 0 && playerObj1->sprite->x + map_pos_x >= RES_W*0.5) {
			moveX = (x_inc)*-1;
		}
		if (x_inc < 0 && (playerObj1->sprite->x + map_pos_x) < RES_W*0.5 && map_pos_x < 0) {
			moveX = (x_inc)*-1;
		}
		/*
		if (y_inc < 0 && (playerObj1->sprite->y + map_pos_y) < MAX_H*0.1666 && map_pos_y < 0) {
			moveY = (y_inc)*-1;
		}
		*/
		if (y_inc > 0 && (playerObj1->sprite->y + map_pos_y) >= MAX_H-48 && (map_pos_y*-1)+MAX_H <= MAP_H*TILE_SIZE-4) {
			//printf(">> move Y 1 <<\n");
			moveY = (y_inc)*-1;
		}
		if (x_inc > 0 && playerObj1->sprite->x + map_pos_x >= RES_W*0.5) {
			moveX = (x_inc)*-1;
		} else if (x_inc < 0 && (playerObj1->sprite->x + map_pos_x) < RES_W*0.5 && map_pos_x < 0) {
			moveX = (x_inc)*-1;
		}

		if (y_inc < 0 && (playerObj1->sprite->y + map_pos_y) < TILE_SIZE*2 && map_pos_y < 0) {
			//printf(">> move Y 2 <<\n");
			moveY = y_inc*-1;
		}

	}
	scrollX += moveX*-1;
	scrollY += moveY*-1;
	//printf("moveX: %d, moveY: %d, scrollX: %d, scrollY: %d\n", moveX, moveY, scrollX, scrollY);
	move_screen(game_screen, moveX, moveY);
}



void anim_npcs() {
	struct npc_struct *temp;
	temp = npc_list[currentMap];
	int n=1;

	if (npc_list[currentMap] == NULL) {
		printf(">> WARNING: No NPCs in this stage (%d) <<\n", currentMap);
	}

	while (temp != NULL) {
		//printf(">> animating NPC number %d <<\n", n);
		move_npc(temp);
		show_npc_sprite(game_screen, temp);
		anim_npc_projectile(temp);
		temp = temp->next;
		n++;
	}
}


void anim_objects() {

	struct struct_object *temp_obj_list = obj_list[currentMap];

	//printf(">> INFO::anim_objects - currentMap: %d <<\n", currentMap);

	while (temp_obj_list) {
		//printf(">> INFO::anim_objects - type: %d, filename: '%s'<<\n", temp_obj_list->type, temp_obj_list->filename);
		if (temp_obj_list->type != -1 && strstr(temp_obj_list->filename, ".png") != NULL) {
			moveObjects(temp_obj_list);
			show_object_sprites(game_screen, temp_obj_list);
		}
		temp_obj_list = temp_obj_list->next;
	}

	// removed the objects marked
	int fim = 0;
	while (fim == 0) {
		fim = 1;
		if (obj_list[currentMap] != NULL) {
			temp_obj_list = obj_list[currentMap];
			while (temp_obj_list) {
				if (temp_obj_list->type == -1) {
					remove_object(temp_obj_list);
					fim = 0;
					break; // we need to restart the cleaning every time one is removed, to avoid crashing because of ->next
				}
				temp_obj_list = temp_obj_list->next;
			}
		}
	}
}

void anim_players() {
	if (p1Obj->hit_timer < getTimer()) {
		show_sprite(p1Obj->sprite, game_screen);
	} else if (getTimer() % BLINK_SPEED == 0) {
		show_sprite(p1Obj->sprite, game_screen);
	}
	if (game_config.two_players == 2) {
		if (p2Obj->hit_timer < getTimer()) {
			show_sprite(p2Obj->sprite, game_screen);
		} else if (getTimer() % BLINK_SPEED == 0) {
			show_sprite(p2Obj->sprite, game_screen);
		}
	}
}


// verify if the player can move to the desired spot
void update_sprites(int mustMove) {
	if (mustMove == 1) {
		read_input();
	} else {
		clean_input(1);
	}
	if (PAUSED == 0) {
		if (keyState[0][KEYSTART] == 1) {
			PAUSED = 1;
			openInGameMenu(0);
		}
		if (keyState[1][KEYSTART] == 1) {
			PAUSED = 1;
			openInGameMenu(1);
		}


		// clear
		// move
		if (mustMove == 1) {
			move_player(p1Obj);
			if (game_config.two_players > 1) {
				move_player(p2Obj);
			}
		}
		anim_npcs();
		anim_objects();
		anim_players();
		anim_projectiles();
		// redraw

		show_animation(game_screen);
		drawMap3rdLevel(game_screen);
		draw_hp_bar(p1Obj);
		if (game_config.two_players == 2) {	// players
			draw_hp_bar(p2Obj);
		}

		if (boss_energy != NULL) {
			draw_npc_hp_bar(boss_energy);
		}
	}
	if (keyState[0][KEYSTART] != 0 || keyState[1][KEYSTART] != 0) printf (">> 2.update_sprites - keyStart[0]: '%d', keyStart[1]: '%d' <<\n", keyState[0][KEYSTART], keyState[1][KEYSTART]);
}

int getPlayerNFromObj(struct player_struct *pObj) {
    if (pObj == p1Obj) {
        return 0;
    } else {
        return 1;
    }
}


// TODO - impediro movimento enquanto o dialogo estiver ativo
void move_player(struct player_struct *pObj) {
    int player_n;
#ifdef PC // TODO: implement a decent FPS limit controller
	if (!checkDelay(pObj)) {
			return;
	}
#endif

	player_n = getPlayerNFromObj(pObj);

	if (keyState[0][KEYQUIT]== 1 || keyState[1][KEYQUIT]== 1) {
		fim = 1;
		return;
    } else {
		player_movement(pObj, keyState[player_n]);
		colision_player_npc(pObj, 0, 0);
    }
}



SDL_Surface *load_tileset(char *filename) {
	SDL_Surface *spriteCopy;
	char buffer[255];
	sprintf(buffer, "%sdata/images/tilesets/%s", FILEPATH, filename);


	spriteCopy = loadSpriteFile(buffer);
	if (!spriteCopy) {
		printf("ERROR: Could not find file '%s'\n", buffer);
		exit(-1);
	}
	return spriteCopy;
}


short int game_init(struct pos stage) {
	SDL_Surface *temp_surface;


	blank_screen(game_screen);
	draw_text(MAX_W*0.5-45-RES_DIFF_W, MAX_H*0.5-15,"LOADING...", game_screen);
	updateScreen(game_screen);
	waitScapeTime(50);

	PAUSED = 0;

	char buffer [255];

	tileset = load_tileset("default.png");
	if (!tileset) {
		draw_text(4, 16,"ERROR LOADING TILESET", game_screen);
		printf("ERRO: no tileset\n");
		exit(-1);
	}
	SDL_SetColors(tileset, colormap, 0, COLOR_COUNT);

	initGameVar();


	loadGame();



	loadStage(1, STAGE_N);
	loadMap(1, 0);

	blank_screen(game_screen);

	// creates offscreen. it is of the map size plus 32 for managing map-change scrolling
	offscreen = SDL_CreateRGBSurface(SDL_SWSURFACE , MAP_W*TILE_SIZE+32, MAP_H*TILE_SIZE+32, 8, 0, 0, 0, 0);
	SDL_SetColors(offscreen, colormap, 0, COLOR_COUNT);


	// new animation system
	//add_sprite(short int direction, short int anim_type, short int pos_x, short int pos_y, short int sprite_id)
	clear_area(game_screen, 0, 0, MAX_W, MAX_H, 0, 0, 0);

	if ((game_config.two_players == 1 && game_config.selected_player == 1) || game_config.two_players == 2) {
		sprintf(buffer, "%sdata/images/sprites/sprites_p1.png", FILEPATH);
		temp_surface = surfaceFromFile(buffer);
		clear_area(game_screen, 0, 0, MAX_W, MAX_H, 0, 0, 0);

		int p1n = add_player();
		p1Obj = getPlayerN(p1n);
		sprintf(p1Obj->name, "Rockbot");

		p1Obj->sprite = create_sprite(20, 0, 29, 32, game_screen);
		//add_sprite(short int direction, short int anim_type, short int pos_x, short int pos_y, short int sprite_id, short int duration)
		// STAND
		add_sprite(ANIM_RIGHT, ANIM_STAND, 3, 0, p1Obj->sprite, 5000, temp_surface);
		add_sprite(ANIM_RIGHT, ANIM_STAND, 4, 0, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_STAND, 3, 1, p1Obj->sprite, 5000, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_STAND, 4, 1, p1Obj->sprite, 150, temp_surface);
		// WALK
		add_sprite(ANIM_RIGHT, ANIM_WALK, 6, 0, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_RIGHT, ANIM_WALK, 7, 0, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_RIGHT, ANIM_WALK, 8, 0, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_WALK, 6, 1, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_WALK, 7, 1, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_WALK, 8, 1, p1Obj->sprite, 150, temp_surface);
		// JUMP
		add_sprite(ANIM_RIGHT, ANIM_JUMP, 9, 0, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_JUMP, 9, 1, p1Obj->sprite, 150, temp_surface);
		// ATTACK
		add_sprite(ANIM_RIGHT, ANIM_ATTACK, 11, 0, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_ATTACK, 11, 1, p1Obj->sprite, 150, temp_surface);
		// ATTACK + JUMP
		add_sprite(ANIM_RIGHT, ANIM_JUMP_ATTACK, 10, 0, p1Obj->sprite, 80, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_JUMP_ATTACK, 10, 1, p1Obj->sprite, 80, temp_surface);
		// ATTACK + WALK
		add_sprite(ANIM_RIGHT, ANIM_WALK_ATTACK, 12, 0, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_RIGHT, ANIM_WALK_ATTACK, 13, 0, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_RIGHT, ANIM_WALK_ATTACK, 14, 0, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_WALK_ATTACK, 12, 1, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_WALK_ATTACK, 13, 1, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_WALK_ATTACK, 14, 1, p1Obj->sprite, 150, temp_surface);
		// HIT
		add_sprite(ANIM_LEFT, ANIM_HIT, 15, 1, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_RIGHT, ANIM_HIT, 15, 0, p1Obj->sprite, 150, temp_surface);
		// TELEPORT
		add_sprite(ANIM_RIGHT, ANIM_TELEPORT, 0, 0, p1Obj->sprite, 750, temp_surface);
		add_sprite(ANIM_RIGHT, ANIM_TELEPORT, 1, 0, p1Obj->sprite, 300, temp_surface);
		add_sprite(ANIM_RIGHT, ANIM_TELEPORT, 2, 0, p1Obj->sprite, 150, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_TELEPORT, 0, 1, p1Obj->sprite, 750, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_TELEPORT, 1, 1, p1Obj->sprite, 300, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_TELEPORT, 2, 1, p1Obj->sprite, 150, temp_surface);
		// STAIRS
		add_sprite(ANIM_RIGHT, ANIM_STAIRS, 17, 0, p1Obj->sprite, 5000, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_STAIRS, 17, 1, p1Obj->sprite, 5000, temp_surface);
		// stairs semi
		add_sprite(ANIM_RIGHT, ANIM_STAIRS_SEMI, 18, 0, p1Obj->sprite, 5000, temp_surface);
		add_sprite(ANIM_RIGHT, ANIM_STAIRS_SEMI, 18, 1, p1Obj->sprite, 5000, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_STAIRS_SEMI, 18, 0, p1Obj->sprite, 5000, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_STAIRS_SEMI, 18, 1, p1Obj->sprite, 5000, temp_surface);
		add_sprite(ANIM_RIGHT, ANIM_STAIRS, 17, 0, p1Obj->sprite, 5000, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_STAIRS, 17, 1, p1Obj->sprite, 5000, temp_surface);
		// stairs + move
		add_sprite(ANIM_RIGHT, ANIM_STAIRS_MOVE, 17, 0, p1Obj->sprite, 200, temp_surface);
		add_sprite(ANIM_RIGHT, ANIM_STAIRS_MOVE, 17, 1, p1Obj->sprite, 200, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_STAIRS_MOVE, 17, 0, p1Obj->sprite, 200, temp_surface);
		add_sprite(ANIM_LEFT, ANIM_STAIRS_MOVE, 17, 1, p1Obj->sprite, 200, temp_surface);
		// stairs + attack
		add_sprite(ANIM_LEFT, ANIM_STAIRS_ATTACK, 19, 1, p1Obj->sprite, 200, temp_surface);
		add_sprite(ANIM_RIGHT, ANIM_STAIRS_ATTACK, 19, 0, p1Obj->sprite, 200, temp_surface);
		SDL_FreeSurface(temp_surface);

		p1Obj->sprite->direction = ANIM_RIGHT;
		p1Obj->sprite->anim_type = ANIM_TELEPORT;
		p1Obj->sprite->move_timer=getTimer()+17;
	}

	if ((game_config.two_players == 1 && game_config.selected_player == 2) || game_config.two_players == 2) {
		sprintf(buffer, "%sdata/images/sprites/sprites_p2.png", FILEPATH);
		temp_surface = surfaceFromFile(buffer);

		struct player_struct *destPObj;

		if (game_config.two_players == 2) {
			int p2n = add_player();
			p2Obj = getPlayerN(p2n);
			p2Obj->sprite = create_sprite(50, 0, 29, 32, game_screen);
			destPObj = p2Obj;
		} else {
			int p1n = add_player();
			p1Obj = getPlayerN(p1n);
			p1Obj->sprite = create_sprite(20, 0, 29, 32, game_screen);
			destPObj = p1Obj;
			p2Obj = NULL;
		}

		destPObj->jump_max = 58;
		destPObj->shots_max = 6;
		sprintf(destPObj->name, "Betabot");

		//add_sprite(short int direction, short int anim_type, short int pos_x, short int pos_y, short int sprite_id)
		// STAND
				add_sprite(ANIM_RIGHT, ANIM_STAND, 3, 0, destPObj->sprite, 5000, temp_surface);
				add_sprite(ANIM_RIGHT, ANIM_STAND, 4, 0, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_STAND, 3, 1, destPObj->sprite, 5000, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_STAND, 4, 1, destPObj->sprite, 300, temp_surface);
		// WALK
				add_sprite(ANIM_RIGHT, ANIM_WALK, 6, 0, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_RIGHT, ANIM_WALK, 7, 0, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_RIGHT, ANIM_WALK, 8, 0, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_WALK, 6, 1, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_WALK, 7, 1, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_WALK, 8, 1, destPObj->sprite, 300, temp_surface);
		// JUMP
				add_sprite(ANIM_RIGHT, ANIM_JUMP, 9, 0, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_JUMP, 9, 1, destPObj->sprite, 300, temp_surface);
		// ATTACK
				add_sprite(ANIM_RIGHT, ANIM_ATTACK, 11, 0, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_ATTACK, 11, 1, destPObj->sprite, 300, temp_surface);
		// ATTACK + JUMP
				add_sprite(ANIM_RIGHT, ANIM_JUMP_ATTACK, 10, 0, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_JUMP_ATTACK, 10, 1, destPObj->sprite, 300, temp_surface);

		// ATTACK + WALK
				add_sprite(ANIM_RIGHT, ANIM_WALK_ATTACK, 12, 0, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_RIGHT, ANIM_WALK_ATTACK, 13, 0, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_RIGHT, ANIM_WALK_ATTACK, 14, 0, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_WALK_ATTACK, 12, 1, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_WALK_ATTACK, 13, 1, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_WALK_ATTACK, 14, 1, destPObj->sprite, 300, temp_surface);
		// HIT
				add_sprite(ANIM_LEFT, ANIM_HIT, 15, 1, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_RIGHT, ANIM_HIT, 15, 0, destPObj->sprite, 300, temp_surface);

		// TELEPORT
				add_sprite(ANIM_RIGHT, ANIM_TELEPORT, 0, 0, destPObj->sprite, 400, temp_surface);
				add_sprite(ANIM_RIGHT, ANIM_TELEPORT, 1, 0, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_RIGHT, ANIM_TELEPORT, 2, 0, destPObj->sprite, 200, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_TELEPORT, 0, 1, destPObj->sprite, 750, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_TELEPORT, 1, 1, destPObj->sprite, 300, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_TELEPORT, 2, 1, destPObj->sprite, 150, temp_surface);
		// STAIRS
				add_sprite(ANIM_RIGHT, ANIM_STAIRS, 17, 0, destPObj->sprite, 5000, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_STAIRS, 17, 1, destPObj->sprite, 5000, temp_surface);
				add_sprite(ANIM_RIGHT, ANIM_STAIRS_SEMI, 18, 0, destPObj->sprite, 5000, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_STAIRS_SEMI, 18, 1, destPObj->sprite, 5000, temp_surface);

		// stairs + move
				add_sprite(ANIM_RIGHT, ANIM_STAIRS_MOVE, 17, 0, destPObj->sprite, 200, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_STAIRS_MOVE, 17, 0, destPObj->sprite, 200, temp_surface);
				add_sprite(ANIM_RIGHT, ANIM_STAIRS_MOVE, 17, 1, destPObj->sprite, 200, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_STAIRS_MOVE, 17, 1, destPObj->sprite, 200, temp_surface);
		// stairs + attack
				add_sprite(ANIM_RIGHT, ANIM_STAIRS_ATTACK, 19, 0, destPObj->sprite, 200, temp_surface);
				add_sprite(ANIM_LEFT, ANIM_STAIRS_ATTACK, 19, 1, destPObj->sprite, 200, temp_surface);

				destPObj->sprite->direction = ANIM_RIGHT;
				destPObj->sprite->anim_type = ANIM_TELEPORT;
				destPObj->sprite->move_timer=getTimer()+17;
			SDL_FreeSurface(temp_surface);
        }

	// load weapons into memory - TODO: load only the ones the player have won

	sprintf(buffer, "%sdata/images/projectiles/projectile_normal.png", FILEPATH);
	normal_projectile = surface_from_image(buffer);

	special_projectile[0] = NULL; // NOT USED

	special_projectile[APEBOT] = NULL; // ape

	sprintf(buffer, "%sdata/images/projectiles/rainbow.png", FILEPATH);
	special_projectile[DAISIEBOT] = surface_from_image(buffer); // daisie

	special_projectile[DYNAMITEBOT] = NULL; // dynamite

	sprintf(buffer, "%sdata/images/projectiles/desert_scorpion.png", FILEPATH);
	special_projectile[MUMMYBOT] = surface_from_image(buffer); // mummy

	sprintf(buffer, "%sdata/images/projectiles/spiked_chain.png", FILEPATH);
	special_projectile[SPIKEBOT] = surface_from_image(buffer); // spike

	sprintf(buffer, "%sdata/images/projectiles/timer_bomb.png", FILEPATH);
	special_projectile[TECHNOBOT] = surface_from_image(buffer); // techno

	sprintf(buffer, "%sdata/images/projectiles/magic_star.png", FILEPATH);
	special_projectile[MAGEBOT] = surface_from_image(buffer); // mage

	sprintf(buffer, "%sdata/images/projectiles/palometa_fish.png", FILEPATH);
	special_projectile[SEAHORSEBOT] = surface_from_image(buffer); // seahorse




	clear_area(game_screen, 0, 0, MAX_W, MAX_H, 0, 0, 0);
	sprintf(buffer, "%sdata/images/tilesets/hit.png", FILEPATH);

	hit_frame = create_animation_frame(buffer, TILE_SIZE, TILE_SIZE);
	hit_frame->duration = 300;

	clear_area(game_screen, 0, 0, MAX_W, MAX_H, 0, 0, 0);
	sprintf(buffer, "%sdata/images/tilesets/ready.png", FILEPATH);

	ready_frame = create_animation_frame(buffer, 60, 13);
	ready_frame->duration = 500;

	clear_area(game_screen, 0, 0, MAX_W, MAX_H, 0, 0, 0);
	sprintf(buffer, "%sdata/images/tilesets/explosion_16.png", FILEPATH);

	kill_frame = create_animation_frame(buffer, TILE_SIZE, TILE_SIZE);
	kill_frame->duration = 400;

	p1_warning = create_simple_sprite(TILE_SIZE, 10);
	draw_text(0, 0, "P1", p1_warning->sprite);
	p2_warning = create_simple_sprite(TILE_SIZE, 10);
	draw_text(0, 0, "P2", p2_warning->sprite);


	if (STAGE_N == 5) {
		p1Obj->sprite->x = RES_W/2-p1Obj->sprite->w;
		p1Obj->x = RES_W/2-p1Obj->sprite->w;
		if (p2Obj) {
			p1Obj->sprite->x = RES_W/2;
			p1Obj->x = RES_W/2;
		}
	}


	clear_area(game_screen, 0, 0, MAX_W, MAX_H, 0, 0, 0);
	drawMap(offscreen, 0);
	map_pos_x = 0;
	map_pos_y = 0;
	move_screen(game_screen, scrollX, scrollY);
	return 1;

}




short int checkStairs(const int px, const int py, const int pw, const int ph) {
	int map_tile_x, map_tile_y, diff_w;
	diff_w = (pw-TILE_SIZE)*0.5;
	map_tile_x = (px+diff_w)/TILE_SIZE;
	map_tile_y = (py+ph)/TILE_SIZE;
	if (map.tiles[map_tile_x][map_tile_y].locked == 2) {
		return map_tile_x;
	}
	map_tile_x = (px+pw-diff_w)/TILE_SIZE;
	if (map.tiles[map_tile_x][map_tile_y].locked == 2) {
		return map_tile_x;
	}
	return -1;
}

/*
test_map_colision - return values:
0 - free
1 - block X only
2 - block Y only
3 - block X and Y
4 - ??
5 - ??
6 - water
*/
short int test_map_colision(struct player_struct *playerObj, const short int incx, const short int incy) {
	// test colision with objects
	//printf(">> test_map_colision.START\n");
    int px = playerObj->sprite->x;
    int py = playerObj->sprite->y;
    int pw = playerObj->sprite->w;
    int ph = playerObj->sprite->h;
    short int diff=0, i, map_tile_x, map_tile_y, map_tile_y_curr=0, diff_w=0, diff_h=0;
	short int block=0; // holds the result. 0 is no blocking, 1 is blocking x, 2 is blocking y, 3 is blocking both x and y
    struct simple_sprite *warning;
    int diff_players;
	struct player_struct *playerObjAux;
    // first, test for xinc

	int res_colision_object = colision_player_object(playerObj, incx, incy);

	if (res_colision_object != 0) {
		//printf(">> test_map_colision - res_colision_object: %d\n", res_colision_object);
		block = res_colision_object;
		//return res_colision_object;
	}


	if (pw > 21) {
		diff_w = abs(ph-21);
    }
    if (ph > PLAYER_H) {
        diff_h = abs(ph-PLAYER_H);
	}
	if (incy < 0) {
		diff_h += 4;
	}

	//printf(">> test_map_colision - ph: %d, diff_h: %d <<\n", ph, diff_h);

	//printf("incx: %d, px: %d, map_pos_x: %d, scrollX: %d\n", incx, px, map_pos_x, scrollX);

	if (incx != 0 && py >= 0 && py <= MAX_H) {
        if (game_config.two_players == 2) {
            if (playerObj == p1Obj) {
                playerObjAux = p2Obj;
            } else {
                playerObjAux = p1Obj;
            }
            //if ((incx > 0 && abs(playerObj->sprite->x-(px+incx+pw)) >= MAX_W) || (incx < 0 && abs(playerObj->sprite->x+playerObj->sprite->w-(px+incx)) >= MAX_W)) {
            diff_players = abs(abs(px+incx)-abs(playerObjAux->sprite->x));
            if (diff_players > MAX_W-pw) {
				if (playerObj == p1Obj) {
					warning = p2_warning;
				} else {
					warning = p1_warning;
				}
				show_simple_sprite(game_screen, warning, playerObjAux->sprite->x+4, playerObjAux->sprite->y-10);
				block = 1;
            }
        }
		if (incx > 0 && px+incx > MAP_W*TILE_SIZE) {
				block = 1;
        } else if (incx < 0 && px+incx+map_pos_x <= 0) {
				block = 1;
        } else {
            if (incx > 0) {
				map_tile_x = (px+pw+incx-diff_w)/TILE_SIZE;
            } else {
				map_tile_x = (px+incx+diff_w)/TILE_SIZE;
            }
			map_tile_y = (py+diff_h)/TILE_SIZE;
			diff = (py-(incy+1)+ph)/TILE_SIZE -map_tile_y;
            for (i=0; i<=diff; i++) {
				// TODO DEAL WITH TERRAIN TYPE
				//printf(">> A. terrain.type: %d\n", map.tiles[map_tile_x][map_tile_y+i].locked);
				if (map.tiles[map_tile_x][map_tile_y+i].locked > 0 && map.tiles[map_tile_x][map_tile_y+i].locked != TERRAIN_SCROLL_LOCK && map.tiles[map_tile_x][map_tile_y+i].locked != TERRAIN_DOOR && map.tiles[map_tile_x][map_tile_y+i].locked != TERRAIN_STAIR && map.tiles[map_tile_x][map_tile_y+i].locked != TERRAIN_WATER && map.tiles[map_tile_x+i][map_tile_y].locked != TERRAIN_SPIKE && map.tiles[map_tile_x+i][map_tile_y].locked != TERRAIN_CHECKPOINT) {
					//printf(">> A. block 1, terrain.type: %d\n\n", map.tiles[map_tile_x][map_tile_y+i].locked);
					block = 1;
					break;
				} else if (incx > 0 && map.tiles[map_tile_x][map_tile_y+i].locked == TERRAIN_DOOR) {
					bossDoorHit(map_tile_x, map_tile_y+i, playerObj, 1);
					break;
				} else if (incx != 0 && map.tiles[map_tile_x][map_tile_y+i].locked == TERRAIN_SCROLL_LOCK) {
					bossDoorHit(map_tile_x, map_tile_y+i, playerObj, 0);
					break;
				} else if (map.tiles[map_tile_x+i][map_tile_y].locked == TERRAIN_SPIKE) {
					damagePlayer(playerObj, 999);
				} else if (map.tiles[map_tile_x+i][map_tile_y].locked == TERRAIN_WATER) {
					//printf(">> test_map_colision - WATER 1\n");
					block = BLOCK_WATER;
				} else if (map.tiles[map_tile_x+i][map_tile_y].locked == TERRAIN_CHECKPOINT) {
					checkpoint.x = playerObj->sprite->x;
					checkpoint.y = playerObj->sprite->y-diff_h;
					checkpoint.map = currentMap;
					checkpoint.map_pos_x = map_pos_x;
					break;
				}
            }
        }
    }

    if (incy != 0) {
			if (incy > 0 && py >= MAX_H) {
				// death because felt in a hole
				damagePlayer(playerObj, 999);
				// out of screen, do not try to find any tile type
			} else if (incy < 0 && py <= 0) {
				return block;
			} else {
					if (incy > 0) {
						map_tile_y = (py-1+ph+incy)/TILE_SIZE;
						map_tile_y_curr = (py-1+ph)/TILE_SIZE;
					} else {
						map_tile_y = (py+diff_h+incy)/TILE_SIZE;
						//map_tile_y = (py-1+incy)/TILE_SIZE;
                    }
					map_tile_x = (px+diff_w)/TILE_SIZE;
					diff = (px+pw-diff_w)/TILE_SIZE - map_tile_x;
                    for (i=0; i<=diff; i++) {
						//printf(">> B. terrain.type: %d\n", map.tiles[map_tile_x][map_tile_y+i].locked);
						if (map.tiles[map_tile_x+i][map_tile_y].locked != UNBLOCKED) {
							if (map.tiles[map_tile_x+i][map_tile_y].locked != TERRAIN_STAIR && map.tiles[map_tile_x+i][map_tile_y].locked != TERRAIN_WATER && map.tiles[map_tile_x+i][map_tile_y].locked != TERRAIN_SPIKE && map.tiles[map_tile_x+i][map_tile_y].locked != TERRAIN_CHECKPOINT) {
								//printf(">> B. block 1\n\n");
								// TODO DEAL WITH TERRAIN TYPE
								if (block == UNBLOCKED || block == BLOCK_WATER) {
									if (map.tiles[map_tile_x+i][map_tile_y].locked == TERRAIN_MOVE_LEFT) {
										if (PLATFORM_COUNTER < 8) {
											PLATFORM_COUNTER++;
										} else {
											playerObj->sprite->x -= 1;
											PLATFORM_COUNTER = 0;
										}
									} else if (map.tiles[map_tile_x+i][map_tile_y].locked == TERRAIN_MOVE_RIGHT) {
										if (PLATFORM_COUNTER < 8) {
											PLATFORM_COUNTER++;
										} else {
											playerObj->sprite->x += 1;
											PLATFORM_COUNTER = 0;
										}
									}
									block = 2;
								} else {
									block = 3;
								}
								break;
							} else if (map.tiles[map_tile_x+i][map_tile_y].locked == TERRAIN_SPIKE) {
								damagePlayer(playerObj, 999);
							} else if (map.tiles[map_tile_x+i][map_tile_y].locked == TERRAIN_WATER) {
								if (block == 0) {
									block = BLOCK_WATER;
								}
							} else if (map.tiles[map_tile_x+i][map_tile_y].locked == TERRAIN_STAIR && incy > 0 && map.tiles[map_tile_x+i][map_tile_y_curr].locked != TERRAIN_STAIR) {
								block = 4;
								break;
							}
						}
                    }
            }
	}
	//printf(">> test_map_colision, block: %d, incx: %d, incy: %d, map_tile_x: %d, map_tile_y: %d <<\n", block, incx, incy, map_tile_x, map_tile_y);
    return block;
}


void teleportPlayersUp() {
	int waitTimer = getTimer()+500;

	p1Obj->sprite->anim_type = ANIM_TELEPORT;
	if (p2Obj) {
		p2Obj->sprite->anim_type = ANIM_TELEPORT;
	}
	play_sfx(SFX_TELEPORT);
	while (p1Obj->sprite->y > -p1Obj->sprite->h) {
		move_screen(game_screen, 0, 0);
		p1Obj->sprite->y -= MOVE_STEP*3;
		show_sprite(p1Obj->sprite, game_screen);
		if (p2Obj) {
			p2Obj->sprite->y -= MOVE_STEP*3;
			show_sprite(p2Obj->sprite, game_screen);
		}
		drawMap3rdLevel(game_screen);
		updateScreen(game_screen);
		waitTime(20);
	}
}

void teleportPlayersStand() {
	int waitTimer = getTimer()+500;

	p1Obj->sprite->anim_type = ANIM_TELEPORT;
	if (p2Obj) {
		p2Obj->sprite->anim_type = ANIM_TELEPORT;
	}
	play_sfx(SFX_TELEPORT);
	while (waitTimer > getTimer()) {
		move_screen(game_screen, 0, 0);
		show_sprite(p1Obj->sprite, game_screen);
		if (p2Obj) {
			show_sprite(p2Obj->sprite, game_screen);
		}
		drawMap3rdLevel(game_screen);
		updateScreen(game_screen);
		waitTime(20);
	}
}


short int test_map_teleport() {
	int i, k, l;
	int lim1, lim2, lim3, lim4;
	int diff_w;
	int temp_x, temp_y, temp_map_n=0, player_x=0, player_y, transition_type=0;
	struct player_struct *playerObj, *pObj2;
	int MUST_TELEPORT = 0;
	int teleport_n = 0;
	int px, py, pw, ph;
	int player_in_n=0;
	struct simple_sprite *warning;
	int teleporter_dist=0;

	for (k=0; k<game_config.two_players; k++) {
		playerObj = getPlayerN(k);
		px = playerObj->sprite->x;
		py = playerObj->sprite->y;
		pw = playerObj->sprite->w;
		ph = playerObj->sprite->h;

		diff_w = (pw-TILE_SIZE)*0.5;
		px = px+(pw*0.5);
		py = py+(ph*0.5)+(ph*0.25);
		/*
		for (i=0; i<LINKS_NUMBER; i++) {
			if (game.map_links[i].map_origin != -1) {
				if ((game.map_links[i].map_origin == currentMap && game.map_links[i].link_pos.x != -1) || (game.map_links[i].map_dest == currentMap && game.map_links[i].link_bidi == 1 && game.map_links[i].link_dest.x != -1)) {
					if ((game.map_links[i].map_origin == currentMap && game.map_links[i].link_pos.x != -1)) {
						temp_x = game.map_links[i].link_pos.x;
						temp_y = game.map_links[i].link_pos.y;
						temp_map_n = game.map_links[i].map_dest;
						player_x = game.map_links[i].link_dest.x;
						player_y = game.map_links[i].link_dest.y;
						if (game.map_links[i].link_pos.y > game.map_links[i].link_dest.y) {
							transition_type = TRANSITION_TOP_TO_BOTTOM;
						} else if (game.map_links[i].link_pos.y < game.map_links[i].link_dest.y) {
							transition_type = TRANSITION_BOTTOM_TO_TOP;
						}
					} else {
						temp_x = game.map_links[i].link_dest.x;
						temp_y = game.map_links[i].link_dest.y;
						temp_map_n = game.map_links[i].map_origin;
						player_x = game.map_links[i].link_pos.x;
						player_y = game.map_links[i].link_pos.y;
						if (game.map_links[i].link_pos.y < game.map_links[i].link_dest.y) {
							transition_type = TRANSITION_TOP_TO_BOTTOM;
						} else if (game.map_links[i].link_pos.y > game.map_links[i].link_dest.y) {
							transition_type = TRANSITION_BOTTOM_TO_TOP;
						}
					}

					lim1 = temp_x*TILE_SIZE;
					lim2 = temp_x*TILE_SIZE + game.map_links[i].link_size*TILE_SIZE;
					lim3 = (temp_y)*TILE_SIZE;
					lim4 = ((temp_y)*TILE_SIZE)+TILE_SIZE;
					if ((px >= lim1 && px <= lim2) && ((py > lim3 && py < lim4))) {
						//printf(">>>>>>>>>>> test_map_teleport, lim1: %d, lim2: %d, px: %d\n", lim1, lim2, px);
						//printf("****> test_map_teleport: i: %d, origin: %d, destiny: %d, size: %d\n", i, game.map_links[i].map_origin, game.map_links[i].map_dest, game.map_links[i].link_size);
						if (playerObj->teleporter == -1) {
							//printf(">>>>>>>>>>> test_map_teleport - MUST TELEPORT i: %d, lim1: %d, lim2: %d, px: %d\n", i, lim1, lim2, px);
							teleporter_dist = abs(lim1 - px) - 8;
							MUST_TELEPORT++;
							teleport_n = i;
							player_in_n = k;
							playerObj->teleporter = i;
							break;
						}
					// only clean teleport when player is out of the teleporter
					} else {
						//printf(">> testing out of teleporter[%d] for player->teleporter[%d] <<\n", i, playerObj->teleporter);
						if (i == playerObj->teleporter) {
							//printf(">> test_map_teleport.clean_teleport <<\n");
							playerObj->teleporter = -1;
						}
					}
				}
			}
		}
		*/

		int link_type = 0;

		struct links_list *temp_link = map_links;
		i = 0;
		while (temp_link) {
			if (temp_link->link.stage == currentStage && temp_link->link.map_origin != -1) {
				if ((temp_link->link.map_origin == currentMap && temp_link->link.link_pos.x != -1) || (temp_link->link.map_dest == currentMap && temp_link->link.link_bidi == 1 && temp_link->link.link_dest.x != -1)) {
					if ((temp_link->link.map_origin == currentMap && temp_link->link.link_pos.x != -1)) {
						temp_x = temp_link->link.link_pos.x;
						temp_y = temp_link->link.link_pos.y;
						temp_map_n = temp_link->link.map_dest;
						player_x = temp_link->link.link_dest.x;
						player_y = temp_link->link.link_dest.y;
						if (temp_link->link.link_pos.y > temp_link->link.link_dest.y) {
							transition_type = TRANSITION_TOP_TO_BOTTOM;
						} else if (temp_link->link.link_pos.y < temp_link->link.link_dest.y) {
							transition_type = TRANSITION_BOTTOM_TO_TOP;
						}
					} else {
						temp_x = temp_link->link.link_dest.x;
						temp_y = temp_link->link.link_dest.y;
						temp_map_n = temp_link->link.map_origin;
						player_x = temp_link->link.link_pos.x;
						player_y = temp_link->link.link_pos.y;
						if (temp_link->link.link_pos.y < temp_link->link.link_dest.y) {
							transition_type = TRANSITION_TOP_TO_BOTTOM;
						} else if (temp_link->link.link_pos.y > temp_link->link.link_dest.y) {
							transition_type = TRANSITION_BOTTOM_TO_TOP;
						}
					}

					lim1 = temp_x*TILE_SIZE;
					lim2 = temp_x*TILE_SIZE + temp_link->link.link_size*TILE_SIZE;
					lim3 = (temp_y)*TILE_SIZE;
					lim4 = ((temp_y)*TILE_SIZE)+TILE_SIZE;
					if ((px >= lim1 && px <= lim2) && ((py > lim3 && py < lim4))) {
						//printf(">>>>>>>>>>> test_map_teleport, lim1: %d, lim2: %d, px: %d\n", lim1, lim2, px);
						//printf("****> test_map_teleport: i: %d, origin: %d, destiny: %d, size: %d\n", i, temp_link->link.map_origin, temp_link->link.map_dest, temp_link->link.link_size);
						if (playerObj->teleporter == -1) {
							//printf(">>>>>>>>>>> test_map_teleport - MUST TELEPORT i: %d, lim1: %d, lim2: %d, px: %d\n", i, lim1, lim2, px);
							printf("**** test_map_teleport::MUST TELEPORT - origin: %d, destiny: %d, size: %d, type: %d\n", temp_link->link.map_origin, temp_link->link.map_dest, temp_link->link.link_size, temp_link->link.link_type);
							teleporter_dist = abs(lim1 - px) - 8;
							MUST_TELEPORT++;
							teleport_n = i;
							player_in_n = k;
							playerObj->teleporter = i;
							link_type = temp_link->link.link_type;
							break;
						}
					// only clean teleport when player is out of the teleporter
					} else {
						//printf(">> testing out of teleporter[%d] for player->teleporter[%d] <<\n", i, playerObj->teleporter);
						if (i == playerObj->teleporter) {
							//printf(">> test_map_teleport.clean_teleport <<\n");
							playerObj->teleporter = -1;
						}
					}
				}
			}
			temp_link = temp_link->next;
			i++;
		}



		if (MUST_TELEPORT > 0 && MUST_TELEPORT != game_config.two_players) {
			if (player_in_n == 0) {
				playerObj = getPlayerN(1);
				warning = p2_warning;
			} else {
				playerObj = getPlayerN(0);
				warning = p1_warning;
			}
			show_simple_sprite(game_screen, warning, playerObj->sprite->x+4, playerObj->sprite->y-10);
		} else if (MUST_TELEPORT == game_config.two_players) {
			for (l=0; l<8; l++) {
				// TODO: this must be changed, we need to use some kind of ID or origin+destiny combination to detect a teleporter identity
				if (skull_castle_bosses[l].teleporter_n == teleport_n) {
					if (skull_castle_bosses[l].finished == 1) {
						printf(">> TELEPORTER %d is already consumed <<\n", l);
						return 0;
					} else {
						printf(">> CONSUME TELEPORTER %d <<\n", l);
						skull_castle_bosses[l].active = 1;
						skull_castle_bosses[l].old_map_pos_x = map_pos_x;
						skull_castle_bosses[l].old_player_pos.x = playerObj->sprite->x;
						skull_castle_bosses[l].old_player_pos.y = playerObj->sprite->y;
						break;
					}
				}
			}

			remove_all_projectiles();
			// --------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>
			// TODO - define according to the condition, try to maintain it
			//playerObj->sprite->anim_type = ANIM_STAIRS;
			// --------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>
			int dest_x = (player_x*TILE_SIZE) - scrollX;

			int max_pos_x = MAP_W*TILE_SIZE - MAX_W;
			if (dest_x > max_pos_x) {
				dest_x = max_pos_x;
			}
			if (dest_x < 0) {
				dest_x = 0;
			}

			// must move the map, so that the dest position in screen is equal to player_real_pos_x
			int new_map_pos_x;
			int playerRealXPos = playerObj->sprite->x + map_pos_x;

			new_map_pos_x = ((player_x*TILE_SIZE)) - playerRealXPos + teleporter_dist;
			if (new_map_pos_x < 0) {
				new_map_pos_x = 0;
			} else if (new_map_pos_x > MAP_W*TILE_SIZE) {
				new_map_pos_x = MAP_W*TILE_SIZE;
			}

			int diff_h=6;

			if (playerObj->sprite->w > 30) {
				diff_h = abs(playerObj->sprite->h-30);
			}
			new_map_pos_x -= diff_h;

			if (link_type == LINK_TELEPORTER) {
				teleportPlayersStand();
				blank_screen(game_screen);
				updateScreen(game_screen);
				waitTime(500);
				loadMap(currentGame, temp_map_n);
				new_map_pos_x = (game.map_links[teleport_n].link_dest.x * TILE_SIZE) - TILE_SIZE*2;
				playerObj->sprite->x = game.map_links[teleport_n].link_dest.x * TILE_SIZE;
				playerObj->sprite->y = game.map_links[teleport_n].link_dest.y * TILE_SIZE;
			} else {
				transitionScreen(transition_type, temp_map_n, new_map_pos_x);
				playerObj->sprite->x = playerRealXPos+new_map_pos_x;
			}

			map_pos_x = -new_map_pos_x;
			scrollX = 0;
			move_screen(game_screen, 0, 0);
			updateScreen(game_screen);


			if (k == 0) {
				pObj2 = getPlayerN(1);
			} else {
				pObj2 = getPlayerN(0);
			}
			if (link_type != LINK_TELEPORTER) {
				pObj2->sprite->x = playerRealXPos + new_map_pos_x;
			} else {
				pObj2->sprite->x = new_map_pos_x+ TILE_SIZE * 2;
			}
			playerObj->teleporter = i;
			pObj2->teleporter = i;
			return 1;
		}
	}
	return 0;
}



void player_jump(struct player_struct *playerObj, int *myKeyState, int x_inc, int y_inc, const short int animation_st_type, const short int animation_atk_type) {
	int res=0, res_before=0, i;
	float JUMP_SPEED_LIMIT, JUMP_LIMIT;
	int jump_speed;

	//#define JUMP_SYSTEM_METHOD 0

	if (jump_locked) {
		return;
	}

	JUMP_LIMIT = playerObj->jump_max;
	//JUMP_LIMIT = 20;
	if (playerObj == p1Obj && game_config.selected_player == 1) {
		JUMP_SPEED_LIMIT = 5;
	} else {
		JUMP_SPEED_LIMIT = 6;
	}

	if (playerObj->sprite->anim_type == ANIM_TELEPORT) {
        return;
	}

	res_before = test_map_colision(playerObj, 0, y_inc);
	if (res_before == BLOCK_WATER) {
		JUMP_LIMIT = RES_H;
		JUMP_SPEED_LIMIT = 3;
	}

	if (playerObj->sprite->anim_type == ANIM_JUMP_ATTACK) {
        if (getTimer() - playerObj->sprite->sprite_frames[playerObj->sprite->direction][playerObj->sprite->anim_type][playerObj->sprite->anim_pos].init > playerObj->sprite->sprite_frames[playerObj->sprite->direction][playerObj->sprite->anim_type][playerObj->sprite->anim_pos].duration - 1) {
			playerObj->sprite->anim_type = animation_st_type;
            playerObj->sprite->sprite_frames[playerObj->sprite->direction][playerObj->sprite->anim_type][playerObj->sprite->anim_pos].init = getTimer();
        }
    }
    // adjust speed. if more near jump max point, more slow

	// GOING UP
	if (playerObj->sprite->frame_animation[ANIM_STATUS] != 2) {
		jump_speed = (JUMP_LIMIT-playerObj->sprite->frame_animation[ANIM_PROCESS])/JUMP_SPEED_STEPS;
	// GOING DOWN
	} else {
		if (playerObj->sprite->frame_animation[ANIM_PROCESS] < playerObj->jump_mark) {
			jump_speed = (playerObj->jump_mark-playerObj->sprite->frame_animation[ANIM_PROCESS])/JUMP_SPEED_STEPS;
		} else {
			jump_speed = JUMP_LIMIT/JUMP_SPEED_STEPS;
		}
	}

    if (jump_speed <= 0) {
		jump_speed = 1;
	} else if (jump_speed > JUMP_SPEED_LIMIT) {
		jump_speed = JUMP_SPEED_LIMIT;
    }

    if (y_inc > 0) {
        y_inc = jump_speed;
    } else {
        y_inc = -jump_speed;
    }

	res = test_map_colision(playerObj, x_inc, y_inc);
    // if step is greater than one and is falling (y_inc > 0) and blocked, try again with lower steps
	if (y_inc > 0 && (res == 3 || res == 2 || res == 4)) {
        int n=1;
        for (i=y_inc; i>0; i--) {
			res = test_map_colision(playerObj, x_inc, i);
			if (res != 2 && res != 3 && res != 4) {
                y_inc=i;
                break;
            }
            n++;
        }
	}


	if (playerObj->sprite->anim_type == ANIM_STAIRS || playerObj->sprite->anim_type == ANIM_STAIRS_SEMI || playerObj->sprite->anim_type == ANIM_STAIRS_MOVE || playerObj->sprite->anim_type == ANIM_STAIRS_ATTACK) {
		if (myKeyState[KEYJUMP] == 1) {
			playerObj->sprite->frame_animation[ANIM_STATUS] = 2;
            playerObj->sprite->frame_animation[ANIM_PROCESS] = 0;
            playerObj->sprite->anim_type = ANIM_JUMP;
            return;
        }
        return;
    }
    // JUMP - check if button was released
	if (myKeyState[KEYJUMP] == 0 && playerObj->jumpButtonReleased == 1) {
			playerObj->jumpButtonReleased=0;
	} else if (myKeyState[KEYJUMP] == 0 && playerObj->sprite->frame_animation[ANIM_STATUS] == 1 && (playerObj->jump_timer + 300 > getTimer() || res == BLOCK_WATER) ) {
		playerObj->sprite->frame_animation[ANIM_STATUS] = 2;
		playerObj->jump_mark = playerObj->sprite->frame_animation[ANIM_PROCESS];
		playerObj->sprite->frame_animation[ANIM_PROCESS] = 0;
		return;
	}
	// jumping
	if (myKeyState[KEYJUMP]== 1 && playerObj->sprite->frame_animation[ANIM_STATUS] == 0 && playerObj->jumpButtonReleased == 0) {
			playerObj->sprite->frame_animation[ANIM_STATUS] = 1;
			playerObj->sprite->anim_type = ANIM_JUMP;
	// no ground, fall
    } else if (playerObj->sprite->frame_animation[ANIM_STATUS] == 0 && res != 2 && res != 3 && res != 4 && res != 5) {
            playerObj->sprite->frame_animation[ANIM_STATUS] = 2;
    // goind up in jump, free path
	} else if (playerObj->sprite->frame_animation[ANIM_STATUS] == 1 && res != 2 && res != 3) {
			// reached maximum on jump, only if not in the water
			if (playerObj->sprite->frame_animation[ANIM_PROCESS] >= JUMP_LIMIT) {
					playerObj->sprite->frame_animation[ANIM_STATUS] = ANIM_JUMP;
                    playerObj->sprite->frame_animation[ANIM_PROCESS] = 0;
					playerObj->jump_mark = JUMP_LIMIT;
            } else {
					playerObj->sprite->frame_animation[ANIM_PROCESS] = playerObj->sprite->frame_animation[ANIM_PROCESS] + abs(y_inc);
					//playerObj->sprite->frame_animation[ANIM_PROCESS] += 1;
            }
    // goind up in jump, hit something
	} else if (playerObj->sprite->frame_animation[ANIM_STATUS] == 1 && (res == 2 || res == 3)) {
			playerObj->sprite->frame_animation[ANIM_STATUS] = ANIM_JUMP;
            playerObj->sprite->frame_animation[ANIM_PROCESS] = 0;
            playerObj->jump_mark = playerObj->sprite->frame_animation[ANIM_PROCESS];
    // already falling, can fall more
    } else if (playerObj->sprite->frame_animation[ANIM_STATUS] == 2 && res != 2 && res != 3) {
			if (playerObj->sprite->anim_type != ANIM_JUMP && playerObj->sprite->anim_type != ANIM_JUMP_ATTACK) {
					playerObj->sprite->anim_type = ANIM_JUMP;
            }
            // do nothing
    // already falling, hit the ground
	} else if (playerObj->sprite->frame_animation[ANIM_STATUS] == 2 && (res == 2 || res == 3)) {
            playerObj->sprite->frame_animation[ANIM_STATUS] = 0;
            playerObj->sprite->frame_animation[ANIM_PROCESS] = 0;
			play_sfx(SFX_PLAYER_JUMP);
            if (playerObj->sprite->anim_type == ANIM_JUMP_ATTACK) {
					playerObj->sprite->anim_type = ANIM_STAND;
            }
    }
    // jumped over a stair
	if (playerObj->sprite->anim_type == ANIM_JUMP) {
			if (res == 4 || res == 5) {
					playerObj->sprite->frame_animation[ANIM_STATUS] = 0;
					playerObj->sprite->frame_animation[ANIM_PROCESS] = 0;
			}
	}
	if (y_inc != 0 && (res == 0 || res == 1 || res == BLOCK_WATER) && (y_inc > 0 || playerObj->sprite->y+playerObj->sprite->h-4 > 0)) {
		playerObj->sprite->y = playerObj->sprite->y + y_inc;
	}
}




short int player_movement(struct player_struct *playerObj, int *myKeyState) {
	short int x_inc=0;
	short int y_inc=0;
	short int step=MOVE_STEP;
	short int res;
	short int attacking=0;
	short int animation_st_type=ANIM_STAND;
	short int animation_atk_type=ANIM_ATTACK;

	short int MOVED = 0;

	// if was hit
	if (playerObj->sprite->anim_type == ANIM_HIT) {
		if (TILE_SIZE*0.5 < playerObj->sprite->frame_animation[ANIM_PROCESS]) {
			playerObj->sprite->anim_type = ANIM_STAND;
			playerObj->sprite->frame_animation[ANIM_PROCESS] = 0;
			playerObj->sprite->frame_animation[ANIM_STATUS] = 0;
			//playerObj->jumpButtonReleased=0;
		} else {
			res = test_map_colision(playerObj, x_inc, y_inc);
			if (res == BLOCK_WATER) {
				step = MOVE_STEP_WATER;
			}
			playerObj->sprite->frame_animation[ANIM_PROCESS] += step;
			if (playerObj->sprite->direction == ANIM_LEFT) {
				x_inc = step;
			} else {
				x_inc = -step;
			}
			y_inc = step*2;
			res = test_map_colision(playerObj, x_inc, y_inc);
			//0 is no blocking, 1 is blocking x, 2 is blocking y, 3 is blocking both x and y
			if (res != 1 && res != 3) {
				playerObj->x = playerObj->x + x_inc;
				playerObj->sprite->x = playerObj->sprite->x + x_inc;
				MOVED = 1;
			}
			if (res != 2 && res != 3) {
				playerObj->y = playerObj->y + y_inc;
				playerObj->sprite->y = playerObj->sprite->y + y_inc;
				MOVED = 2;
			}
		}
		move_screen(game_screen, 0, 0);
		return 0;
	}


	// only move horizontaly is player is not on teleport
	if (playerObj->sprite->anim_type != ANIM_TELEPORT) {
		if (myKeyState[KEYX] == 1) {
			x_inc = step;
		} else if (myKeyState[KEYX] == 2) {
			x_inc = -step;
		}
	}
	// if player is not jumping or above/on a staircase, he is always trying to fall
	y_inc = step*2;
	//printf(">> 1 - p.move.x: %c, x_inc: %d\n", myKeyState[KEYX], x_inc);

	// ATTACK - check if button was released
	if (myKeyState[KEYATTACK]== 0 && playerObj->attackButtonReleased==1) {
		playerObj->attackButtonReleased=0;
	}
	//verify if still not reached the jump maximum
	if (playerObj->sprite->anim_type == ANIM_TELEPORT) {
		y_inc = step*2;
	} else if (myKeyState[KEYJUMP]== 1 && playerObj->sprite->frame_animation[ANIM_STATUS] == 0 && playerObj->jumpButtonReleased == 0) {
		if (playerObj->sprite->anim_type == ANIM_TELEPORT || playerObj->sprite->anim_type == ANIM_STAIRS_MOVE || playerObj->sprite->anim_type == ANIM_STAIRS_SEMI || playerObj->sprite->anim_type == ANIM_STAIRS || playerObj->sprite->anim_type == ANIM_STAIRS_ATTACK) {
			playerObj->sprite->frame_animation[ANIM_STATUS] = 2;
		} else {
			playerObj->sprite->frame_animation[ANIM_STATUS] = 1;
		}
		playerObj->jumpButtonReleased=1;
		y_inc = -step*2;
		playerObj->sprite->anim_type = ANIM_JUMP;

		playerObj->sprite->frame_animation[ANIM_PROCESS] += step;
		MOVED = 1;
	} else if (playerObj->sprite->frame_animation[ANIM_STATUS] == 2) {
		// can fall
		y_inc = step*2;
	} else if (playerObj->sprite->frame_animation[ANIM_STATUS] == 1) {
		// can fall
		y_inc = -step*2;
	}

	if (x_inc != 0 || y_inc != 0) {
		res = test_map_colision(playerObj, x_inc, y_inc);
		if (playerObj->sprite->anim_type == ANIM_TELEPORT && res != 2 && res != 3 && res != 4) {
			playerObj->sprite->y = playerObj->sprite->y + y_inc;
			MOVED = 1;
		}
		if (test_map_teleport()) {
			res = test_map_colision(playerObj, x_inc, y_inc);
		}
		if (x_inc != 0 && (playerObj->sprite->anim_type == ANIM_STAIRS || playerObj->sprite->anim_type == ANIM_STAIRS_MOVE || playerObj->sprite->anim_type == ANIM_STAIRS_SEMI || playerObj->sprite->anim_type == ANIM_STAIRS_ATTACK)) {
			if (x_inc < 0) {
				playerObj->sprite->direction = ANIM_LEFT;
			} else {
				playerObj->sprite->direction = ANIM_RIGHT;
			}
			animation_atk_type = ANIM_STAIRS_ATTACK;
			animation_st_type = ANIM_STAIRS;
			MOVED = 1;
		} else if (x_inc == 0 && (playerObj->sprite->anim_type == ANIM_STAIRS || playerObj->sprite->anim_type == ANIM_STAIRS_MOVE || playerObj->sprite->anim_type == ANIM_STAIRS_SEMI || playerObj->sprite->anim_type == ANIM_STAIRS_ATTACK)) {
			animation_atk_type = ANIM_STAIRS_ATTACK;
			animation_st_type = ANIM_STAIRS;
			MOVED = 1;
		} else if (x_inc != 0 && (res == 0 || res == 2 || res == 4 || res == BLOCK_WATER)) {
			//printf(">> 3 - p.move.x: %c, x_inc: %d\n", myKeyState[KEYX], x_inc);
			if (x_inc < 0) {
				playerObj->sprite->direction = ANIM_LEFT;
			} else {
				playerObj->sprite->direction = ANIM_RIGHT;
			}
			playerObj->sprite->x = playerObj->sprite->x + x_inc;
			animation_st_type = ANIM_WALK;
			animation_atk_type = ANIM_WALK_ATTACK;
			MOVED = 1;
		} else if (playerObj->sprite->anim_type != ANIM_TELEPORT) {
			animation_atk_type = ANIM_ATTACK;
			animation_st_type = ANIM_STAND;
		}



		// checks if attack must be finished
		if (playerObj->sprite->anim_type == ANIM_WALK_ATTACK || playerObj->sprite->anim_type == ANIM_JUMP_ATTACK ||  playerObj->sprite->anim_type == ANIM_ATTACK || playerObj->sprite->anim_type == ANIM_STAIRS_ATTACK) {
			if (getTimer() - playerObj->sprite->sprite_frames[playerObj->sprite->direction][playerObj->sprite->anim_type][playerObj->sprite->anim_pos].init > playerObj->sprite->sprite_frames[playerObj->sprite->direction][playerObj->sprite->anim_type][playerObj->sprite->anim_pos].duration - 1) {
				if (playerObj->sprite->anim_type == ANIM_JUMP || playerObj->sprite->anim_type == ANIM_JUMP_ATTACK) {
					playerObj->sprite->anim_type = ANIM_JUMP;
				} else {
					playerObj->sprite->anim_type = animation_st_type;
				}
				playerObj->sprite->sprite_frames[playerObj->sprite->direction][playerObj->sprite->anim_type][playerObj->sprite->anim_pos].init = getTimer();
			}
		} else if (playerObj->sprite->anim_type != ANIM_TELEPORT && playerObj->sprite->anim_type != ANIM_STAIRS && playerObj->sprite->anim_type != ANIM_STAIRS_SEMI && playerObj->sprite->anim_type != ANIM_STAIRS_MOVE && playerObj->sprite->anim_type != ANIM_STAIRS_ATTACK && playerObj->sprite->frame_animation[ANIM_STATUS] == 0 && playerObj->sprite->anim_type != animation_st_type) {
			playerObj->sprite->anim_type = animation_st_type;
		}
		// check if player is attacking
		if (playerObj->sprite->anim_type != ANIM_TELEPORT && execute_attack(playerObj, myKeyState)) {
			attacking = 1;
		} else {
			if (myKeyState[KEYATTACK]== 0 && playerObj->attackButtonReleased==1) {
				playerObj->attackButtonReleased=0;
			}
			attacking = 0;
		}

		// teleporting end
		if (playerObj->sprite->anim_type == ANIM_TELEPORT && (res == 2 || res == 3 || res == 4)) {
			if (attacking == 1) {
				playerObj->sprite->sprite_frames[playerObj->sprite->direction][playerObj->sprite->anim_type][playerObj->sprite->anim_pos].init = getTimer();
				playerObj->sprite->anim_type = animation_atk_type;
			} else {
				playerObj->sprite->anim_type = animation_st_type;
			}
			// TODO - must set the two players to stand in order to show the dialog
			playerObj->sprite->frame_animation[ANIM_STATUS] = 0;
			playerObj->sprite->frame_animation[ANIM_PROCESS] = 0;
			move_screen(game_screen, 0, 0);
			show_sprite(playerObj->sprite, game_screen);
#ifndef QUICK_LOAD
			if (stage_start == 1) {
				drawMap3rdLevel(game_screen);
				show_dialog(STAGE_N, 0);
				stage_start = 0;
			}
#endif
		} else {
			player_jump(playerObj, myKeyState, x_inc, y_inc, animation_st_type, animation_atk_type);
			if (attacking == 1) {
				playerObj->sprite->sprite_frames[playerObj->sprite->direction][playerObj->sprite->anim_type][playerObj->sprite->anim_pos].init = getTimer();
				if (playerObj->sprite->anim_type == ANIM_STAIRS || playerObj->sprite->anim_type == ANIM_STAIRS_SEMI || playerObj->sprite->anim_type == ANIM_STAIRS_MOVE || playerObj->sprite->anim_type == ANIM_STAIRS_ATTACK) {
					playerObj->sprite->anim_type = ANIM_STAIRS_ATTACK;
					playerObj->sprite->frame_animation[ANIM_STATUS] = 0;
					playerObj->sprite->frame_animation[ANIM_PROCESS] = 0;
				} else if (playerObj->sprite->frame_animation[ANIM_STATUS] != 0) {
					playerObj->sprite->anim_type = ANIM_JUMP_ATTACK;
				} else {
					playerObj->sprite->anim_type = animation_atk_type;
					playerObj->sprite->frame_animation[ANIM_STATUS] = 0;
					playerObj->sprite->frame_animation[ANIM_PROCESS] = 0;
				}

			}
		}

		if (res != 3 && res != 5 && playerObj->sprite->anim_type != ANIM_STAIRS) {
			checkScrolling(x_inc, y_inc, playerObj);
		} else {
			move_screen(game_screen, 0, 0);
		}
		y_inc = 1;
		// check if out of stairs
		if (playerObj->sprite->anim_type == ANIM_STAIRS || playerObj->sprite->anim_type == ANIM_STAIRS_SEMI || playerObj->sprite->anim_type == ANIM_STAIRS_MOVE || playerObj->sprite->anim_type == ANIM_STAIRS_ATTACK) {
			if (checkStairs(playerObj->sprite->x, playerObj->sprite->y-y_inc, playerObj->sprite->w, playerObj->sprite->h) == -1 && checkStairs(playerObj->sprite->x, playerObj->sprite->y-y_inc, playerObj->sprite->w, playerObj->sprite->h) == -1) {
				printf(">>>>>>>>>> out of stairs 1 <<<<<<<<<<<\n");
				playerObj->sprite->anim_type = ANIM_JUMP;
			}
		}
		if (myKeyState[KEYY]== 1) {
			res = checkStairs(playerObj->sprite->x, playerObj->sprite->y+y_inc, playerObj->sprite->w, playerObj->sprite->h);
			if (res != -1) {
				if (checkStairs(playerObj->sprite->x, playerObj->sprite->y-y_inc-4, playerObj->sprite->w, playerObj->sprite->h) == -1) {
					playerObj->sprite->anim_type = ANIM_STAIRS_SEMI;
				} else {
					playerObj->sprite->anim_type = ANIM_STAIRS_MOVE;
				}
				playerObj->sprite->x = res*TILE_SIZE-6;
				checkScrolling(x_inc, y_inc+1, playerObj);
				playerObj->sprite->y = playerObj->sprite->y + y_inc+1;
				test_map_teleport();
			} else {
				if (playerObj->sprite->anim_type == ANIM_STAIRS || playerObj->sprite->anim_type == ANIM_STAIRS_SEMI || playerObj->sprite->anim_type == ANIM_STAIRS_MOVE || playerObj->sprite->anim_type == ANIM_STAIRS_ATTACK) {
					playerObj->sprite->anim_type = ANIM_STAND;
				}
			}
		} else if (myKeyState[KEYY]== 2) {
			res = checkStairs(playerObj->sprite->x, playerObj->sprite->y-y_inc, playerObj->sprite->w, playerObj->sprite->h);
			if (res != -1) {
				if (checkStairs(playerObj->sprite->x, playerObj->sprite->y-y_inc-(playerObj->sprite->h*0.5), playerObj->sprite->w, playerObj->sprite->h) == -1) {
					playerObj->sprite->anim_type = ANIM_STAIRS_SEMI;
				} else {
					playerObj->sprite->anim_type = ANIM_STAIRS_MOVE;
				}
				playerObj->sprite->x = res*TILE_SIZE-6;
				if (playerObj->sprite->y - y_inc + TILE_SIZE >= 0) {
					playerObj->sprite->y = playerObj->sprite->y - y_inc - 1;
					checkScrolling(x_inc, -(y_inc+1), playerObj);
					test_map_teleport();
				}
				MOVED = 1;
			} else {
				if (playerObj->sprite->anim_type == ANIM_STAIRS || playerObj->sprite->anim_type == ANIM_STAIRS_SEMI || playerObj->sprite->anim_type == ANIM_STAIRS_MOVE || playerObj->sprite->anim_type == ANIM_STAIRS_ATTACK) {
					playerObj->sprite->anim_type = ANIM_STAND;
					playerObj->sprite->anim_pos = 0;
				}
			}
		} else if (playerObj->sprite->anim_type == ANIM_STAIRS_MOVE) {
			playerObj->sprite->anim_type = ANIM_STAIRS;
			playerObj->sprite->anim_pos = 0;
		}

	} else {
		return 0;
	}
	return MOVED;
}


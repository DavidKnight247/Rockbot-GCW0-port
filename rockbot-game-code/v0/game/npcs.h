#ifndef NPCS_H
#define NPCS_H


#include "player_and_npc.h"
#include "ia.h"


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

#define SHOT_ARC 0
#define SHOT_PROGRESSIVE 1




void IAInvertDirection(struct npc_struct *temp_npc);
void shot_flower(struct npc_struct *temp_npc);
void anim_progressive_projectile(struct npc_struct *temp_npc);

#include "ia_bosses.h"


void show_npc_sprite(SDL_Surface *screen, struct npc_struct *temp) {
	SDL_Rect src, dest;

	if (temp->sprite_list == NULL) {
		return;
	}

	if (temp->hit_timer != 0) {
			if (temp->hit_timer < getTimer()) {
					if (temp->hit_timer > 0) {
						temp->hit_timer = 0;
					}
					return;
			} else if (getTimer() % BLINK_SPEED != 0) {
				return;
			}
	}

	// checks if the NPC is near the screen to show it
	if (temp->HP > 0 && temp->x+16 >= abs(map_pos_x) && temp->x-16 <= abs(map_pos_x)+RES_W) {

		// copia sprite para a tela
		src.x = 0;
		src.y = 0;
		src.w = temp->sprite_list->w;
		src.h = temp->sprite_list->h;
		// área da colagem
		dest.x = (temp->x + map_pos_x);
		dest.y = (temp->y + map_pos_y);
		dest.w = temp->sprite_list->w;
		dest.h = temp->sprite_list->h;

		// verifica se o frame da animação existe, caso contrário, usa o primeiro (0) da lista
		//printf("++ graphics.h::show_npc_sprites - direction: %d, type: %d, pos: %d\n", temp->sprite->direction, temp->sprite->anim_type,temp->sprite->anim_pos);
		if (temp->sprite_list->sprite_frames[temp->sprite_list->direction][temp->sprite_list->anim_type][temp->sprite_list->anim_pos].frame == NULL) {
			//printf("++ graphics.h::show_npc_sprites - frame NULL\n");
			if (temp->sprite_list->sprite_frames[temp->sprite_list->direction][temp->sprite_list->anim_type][0].frame != NULL) {
				//printf("++ graphics.h::show_npc_sprites - reset to pos ZERO\n");
				temp->sprite_list->anim_pos=0;
			} else {
				printf("DEBUG - erro: Não achou frame: direction: %d,  type: %d, pos: %d\n", temp->sprite_list->direction, temp->sprite_list->anim_type, temp->sprite_list->anim_pos);
				temp->sprite_list->anim_type = 0;
				temp->sprite_list->direction = 0;
				temp->sprite_list->anim_pos = 0;
				//return;
			}
		}
		SDL_BlitSurface(temp->sprite_list->sprite_frames[temp->direction][temp->sprite_list->anim_type][temp->sprite_list->anim_pos].frame, &src, screen, &dest);
		// avança a animação
		if (getTimer() - temp->sprite_list->sprite_frames[temp->sprite_list->direction][temp->sprite_list->anim_type][temp->sprite_list->anim_pos].init > abs(temp->sprite_list->sprite_frames[temp->sprite_list->direction][temp->sprite_list->anim_type][temp->sprite_list->anim_pos].duration)) {
			if (temp->sprite_list->sprite_frames[temp->sprite_list->direction][temp->sprite_list->anim_type][temp->sprite_list->anim_pos+1].frame != NULL) {
				//printf(">> show_npc_sprite - anim_pos inc <<\n");
				temp->sprite_list->anim_pos++;
				// if is last item of this anim_type, and NPC shoots, must shoot
			// reseta animação
			} else {
				//printf(">> show_npc_sprite - anim_pos RESET - anim_type: %d, anim_pos: %d, direction: %d <<\n", temp->sprite_list->anim_type, temp->sprite_list->anim_pos, temp->sprite_list->direction);
				temp->sprite_list->anim_pos=0;
				if (temp->sprite_list->anim_type == ANIM_TURN) {
					if (temp->direction == ANIM_LEFT) {
						temp->direction = ANIM_RIGHT;
					} else {
						temp->direction = ANIM_LEFT;
					}
					temp->sprite_list->anim_type = ANIM_STAND;
				}
			}
			temp->sprite_list->sprite_frames[temp->sprite_list->direction][temp->sprite_list->anim_type][temp->sprite_list->anim_pos].init=getTimer();
		}
	}
}




int dist_npc_player(struct npc_struct *temp_npc) {
	int dist;
	dist = sqrt(pow((temp_npc->x - (temp_npc->target->sprite->x + temp_npc->target->sprite->w * 0.5)), 2) + pow((temp_npc->y - (temp_npc->target->sprite->y + temp_npc->target->sprite->h * 0.5)), 2));
	return dist;
}


int move_npc_check_colision(struct npc_struct *temp_npc, const int incX, const int incY) {
	int map_tile_x;


	if (temp_npc->direction == ANIM_LEFT) {
		map_tile_x = (temp_npc->x+incX)/16;
	} else {
		map_tile_x = (temp_npc->x+incX+temp_npc->sprite_list->w)/16;
	}
	int map_tile_y = (temp_npc->y+incY+(temp_npc->sprite_list->w/2))/16;
	//printf(">> move_npc_check_colision, incX: %d, incY: %d <<\n", incX, incY);
	//printf(">> move_npc_check_colision, map_tile_x: %d, map_tile_y: %d <<\n", map_tile_x, map_tile_y);


	//printf(">> move_npc_check_colision - temp_npc->y: %d, map_tile_x: %d, map_tile_y: %d, locked: %d\n", temp_npc->y, map_tile_x, map_tile_y, map.tiles[map_tile_x][map_tile_y].locked);
	if (map.tiles[map_tile_x][map_tile_y].locked > 0 && map.tiles[map_tile_x][map_tile_y].locked != TERRAIN_WATER) {
		if (temp_npc->IA == IA_ZIGZAG || temp_npc->IA == IA_SHOOT_AND_GO || temp_npc->IA == IA_FLY_ZIG_ZAG) {

			if (temp_npc->direction == ANIM_LEFT) {
				temp_npc->direction = ANIM_RIGHT;
			} else {
				temp_npc->direction = ANIM_LEFT;
			}
			temp_npc->sprite_list->anim_pos = 0;
			temp_npc->move_info.distance = 0;
			temp_npc->move_info.time = getTimer()+200;
		}
		//printf("******* COLISION DETECTED - lock_type: %d ********\n", map.tiles[map_tile_x][map_tile_y].locked);
		return 1;
	}
	return 0;
}


int npc_gravity(struct npc_struct *temp_npc) {
	if (temp_npc->IA != IA_BAT && temp_npc->IA != IA_FLY_ZIG_ZAG && temp_npc->IA != IA_ROOF_SHOOTER && temp_npc->IA != IA_FIXED_JUMPER && temp_npc->IA != IA_SIDE_SHOOTER && temp_npc->IA != IA_GHOST && temp_npc->IA != IA_FISH && temp_npc->IA != IA_DOLPHIN && temp_npc->IA != IA_SIDETOSIDE && temp_npc->sprite_list != NULL) {
		// make NPCs fall
		int map_tile_x = temp_npc->x/16;
		int map_tile_y = (temp_npc->y-1+temp_npc->sprite_list->h)/16;
		if (map.tiles[map_tile_x][map_tile_y].locked == 0 || map.tiles[map_tile_x][map_tile_y].locked == TERRAIN_WATER) {
			temp_npc->y += NPC_STEP * 2;
			return 1; // moved
		} else {
			return 2; // blocked
		}
	}
	return 0; // not moved because of IA type
}



// executes the NPC sub-IA behavior

void move_npc(struct npc_struct *temp_npc) {
	int mapScrollX;
	// only move npcs that are in the current screen and are alive
	mapScrollX = -map_pos_x;

	//printf(">> INFO: move_npc::START - npc: %s<<\n", temp_npc->name);

	// checks if the npcs are frozen
	if ((p1Obj->selected_weapon == DYNAMITEBOT && p1Obj->weapon_state != -1)  || (game_config.two_players == 2 && p2Obj->selected_weapon == DYNAMITEBOT && p2Obj->weapon_state != -1)) {
			if (temp_npc->is_boss == 0 || (temp_npc->is_boss == 1 && strstr(temp_npc->name, "Mage Bot"))) {
			printf(">> INFO: move_npc - npc is frozen <<\n");
			return;
		}
	}

	if (temp_npc->move_info.time > getTimer()) {
		//printf(">> INFO: move_npc - NPC timer not yet reached <<\n");
		return;
	//} else {
		//printf(">> INFO: move_npc - NPC timer reached - temp_npc->HP: %d, temp_npc->x: %d >= %d && <= %d <<\n", temp_npc->HP, temp_npc->x, (mapScrollX-TILE_SIZE*2), (mapScrollX+RES_W+TILE_SIZE*2));
	}

	if (temp_npc->HP > 0 && temp_npc->x >= mapScrollX-TILE_SIZE*2 && temp_npc->x <= mapScrollX+RES_W+TILE_SIZE*2) {

		if (temp_npc->is_boss == 1) {
			if (temp_npc->first_run == 0) {
				// show boss dialogs here
				temp_npc->first_run = 1;
				boss_energy = temp_npc;
				play_boss_music();
				show_dialog(STAGE_N, 1);
			}
			if (strstr(temp_npc->name, "Daisie Bot")) {
				exec_IA_Daisiebot(temp_npc);
				npc_gravity(temp_npc);
				return;
			} else if (strstr(temp_npc->name, "Ape Bot")) {
				exec_IA_Apebot(temp_npc);
				return;
			} else {
				npc_gravity(temp_npc);
				return;
			}
		}

		//printf(">> npcs.h::move_npc - INFO: moving NPC.IA: %d, IA_GHOST: %d <<\n", temp_npc->IA, IA_GHOST);

		if (temp_npc->IA == IA_ZIGZAG) {
			exec_IA_ZIGZAG(temp_npc);
		} else if (temp_npc->IA == IA_BAT) {
			if (temp_npc->move_info.status == IA_STATE_FOLLOW) {
				// handle error on player object
				if (temp_npc->target == NULL) {
					temp_npc->move_info.status = IA_STATE_RETURN;
					return;
				}
				int dist  = dist_npc_player(temp_npc);
				if (dist > 250 || dist <= TILE_SIZE+4) { // player scaped or npc reached him
					temp_npc->move_info.status = IA_STATE_RETURN;
					return;
				}
				if (temp_npc->target->sprite->x > temp_npc->x) {
					temp_npc->x += 4;
				} else if (temp_npc->target->sprite->x < temp_npc->x) {
					temp_npc->x -= 4;
				}
				if (temp_npc->target->sprite->y > temp_npc->y) {
					temp_npc->y += 4;
				} else if (temp_npc->target->sprite->y < temp_npc->y) {
					temp_npc->y -= 4;
				}
			} else if (temp_npc->move_info.status == IA_STATE_RETURN) {
				if (temp_npc->target != NULL) {
					if (temp_npc->target->sprite->x > temp_npc->x) {
						temp_npc->x -= 2;
					} else if (temp_npc->target->sprite->x < temp_npc->x) {
						temp_npc->x += 2;
					}
				}
				temp_npc->y -= 4;
				int map_tile_x = temp_npc->x/16;
				int map_tile_y = temp_npc->y/16;
				if ((temp_npc->y < 0) || (map.tiles[map_tile_x][map_tile_y].locked > 0 && map.tiles[map_tile_x][map_tile_y].locked != TERRAIN_WATER)) {
					temp_npc->move_info.status = IA_STATE_SLEEP;
					temp_npc->sprite_list->anim_type = ANIM_STAND;
					return;
				}
			} else if (temp_npc->sprite_list->anim_type == ANIM_MOVE_SEMI) { // START MOVING
				temp_npc->move_info.status = IA_STATE_FOLLOW;
				temp_npc->sprite_list->anim_type = ANIM_WALK;
			} else { // IA_STATE_SLEEP
				struct struct_player_dist temp = dist_npc_players(temp_npc);
				if (temp.dist < 150) {

					temp_npc->sprite_list->anim_type = ANIM_MOVE_SEMI;
					temp_npc->move_info.status = IA_STATE_START_MOVE;

					temp_npc->target = temp.pObj;
				}
			}
			temp_npc->move_info.time = getTimer()+200;
		} else if (temp_npc->IA == IA_ROOF_SHOOTER) {
			if (temp_npc->move_info.status == 0) { // sleep -> regular
				temp_npc->sprite_list->anim_type = ANIM_ATTACK;
				// shoot regular
				throw_projectile_npc(temp_npc, PROJECTLE_TYPE_REGULAR, 2, 1, 1);
				temp_npc->move_info.status = 1;
				temp_npc->move_info.time = getTimer()+800;
			} else if (temp_npc->move_info.status == 1) { // regular -> diagonal
				// shoot diagonal
				throw_projectile_npc(temp_npc, PROJECTLE_TYPE_DIAGONAL, 2, 1, 1);
				temp_npc->move_info.status = 2;
				temp_npc->move_info.time = getTimer()+800;
			} else if (temp_npc->move_info.status == 2) { // diagonal -> sleep
				temp_npc->sprite_list->anim_type = ANIM_STAND;
				temp_npc->move_info.time = getTimer() + 2000;
				temp_npc->move_info.status = 0;
			}
		} else if (temp_npc->IA == IA_STAND && temp_npc->can_shoot == 1 && abs(p1Obj->sprite->x - temp_npc->x) <= 96) {
			// TODO: change direction to face player
			if (temp_npc->move_info.status == 0) {
				temp_npc->sprite_list->anim_type = ANIM_ATTACK;
				temp_npc->sprite_list->anim_pos = 0;
				temp_npc->sprite_list->move_timer = 0;
				temp_npc->move_info.time = getTimer() + 500;
				temp_npc->move_info.status = 1;
				if (temp_npc->direction == ANIM_LEFT && p1Obj->sprite->x > temp_npc->x) {
					temp_npc->direction = ANIM_RIGHT;
				} else if (temp_npc->direction == ANIM_RIGHT && p1Obj->sprite->x < temp_npc->x) {
					temp_npc->direction = ANIM_LEFT;
				}
			} else if (temp_npc->move_info.status == 1) {
				throw_projectile_npc(temp_npc, PROJECTLE_TYPE_REGULAR, 1, 0, 0);
				temp_npc->move_info.time = getTimer() + 500;
				temp_npc->move_info.status = 2;
			} else if (temp_npc->move_info.status == 2) {
				temp_npc->sprite_list->anim_type = ANIM_STAND;
				temp_npc->move_info.time = getTimer() + 1500;
				temp_npc->move_info.status = 0;
			}
		} else if (temp_npc->IA == IA_SHOOT_AND_GO) {
			// TODO: change direction to face player
			if (temp_npc->move_info.status == 0 && abs(p1Obj->sprite->x - temp_npc->x) <= 64) {
				temp_npc->sprite_list->anim_type = ANIM_ATTACK;
				temp_npc->sprite_list->anim_pos = 0;
				temp_npc->sprite_list->move_timer = 0;
				temp_npc->move_info.time = getTimer() + 500;
				temp_npc->move_info.status = 1;
				if (temp_npc->direction == ANIM_LEFT && p1Obj->sprite->x > temp_npc->x) {
					temp_npc->direction = ANIM_RIGHT;
				} else if (temp_npc->direction == ANIM_RIGHT && p1Obj->sprite->x < temp_npc->x) {
					temp_npc->direction = ANIM_LEFT;
				}
			} else if (temp_npc->move_info.status == 1) {
				throw_projectile_npc(temp_npc, PROJECTLE_TYPE_REGULAR, 1, 0, 0);
				temp_npc->move_info.time = getTimer() + 800;
				temp_npc->move_info.status = 2;
			} else if (temp_npc->move_info.status == 2) {

				if (temp_npc->direction == ANIM_LEFT) {
					if (move_npc_check_colision(temp_npc, -NPC_STEP, 0)) {
						return;
					}
					temp_npc->sprite_list->anim_type = ANIM_WALK;
					temp_npc->x -= NPC_STEP * 3;
				} else {
					if (move_npc_check_colision(temp_npc, NPC_STEP, 0)) {
						return;
					}
					temp_npc->sprite_list->anim_type = ANIM_WALK;
					temp_npc->x += NPC_STEP * 3;
				}
				temp_npc->move_info.time = getTimer() + 20;
			}
		} else if (temp_npc->IA == IA_FLY_ZIG_ZAG) {
			exec_IA_FLY_ZIG_ZAG(temp_npc);
		} else if (temp_npc->IA == IA_FIXED_JUMPER) {
			if (temp_npc->sprite_list->anim_type != ANIM_STAND) {
				temp_npc->sprite_list->anim_type = ANIM_STAND;
			}
			//printf(">> IA_FIXED_JUMPER - exec <<\n");
			if (temp_npc->move_info.status != 1) { // falling
				if (temp_npc->y < RES_H+TILE_SIZE) {
					temp_npc->y += 8;
					temp_npc->move_info.time = getTimer()+20;
				} else {
					temp_npc->sprite_list->anim_type = ANIM_STAND;
					temp_npc->sprite_list->anim_pos = 0;
					temp_npc->sprite_list->move_timer = 0;
					temp_npc->move_info.status = 1;
					temp_npc->move_info.time = getTimer()+1200;
				}
			} else { // jump up
				if (temp_npc->y > 20) {
					temp_npc->y -= 6;
					temp_npc->move_info.time = getTimer()+20;
				} else { // reached jump maximum, wait a bit and start falling
					temp_npc->move_info.status = 0;
					temp_npc->move_info.time = getTimer()+200;
				}
			}
		} else if (temp_npc->IA == IA_SIDE_SHOOTER) {
			if (temp_npc->move_info.status == 0) { // sleep -> shoot
				// shoot diagonal
				struct struct_player_dist temp = dist_npc_players(temp_npc);
				temp_npc->target = temp.pObj;
				// shoot down
				if (temp_npc->target->sprite->y > temp_npc->y+temp_npc->sprite_list->h) {
					throw_projectile_npc(temp_npc, PROJECTLE_TYPE_DIAGONAL, 1, 1, 1);
					temp_npc->sprite_list->anim_type = ANIM_ATTACK;
				// shoot up
				} else if (temp_npc->target->sprite->y+temp_npc->target->sprite->h < temp_npc->y) {
					throw_projectile_npc(temp_npc, PROJECTLE_TYPE_DIAGONAL, 1, 1, 0);
					temp_npc->sprite_list->anim_type = ANIM_STAND;
				// shoot regular
				} else {
					throw_projectile_npc(temp_npc, PROJECTLE_TYPE_REGULAR, 1, 1, 0);
					temp_npc->sprite_list->anim_type = ANIM_WALK;
				}
				temp_npc->move_info.status = 1;
				temp_npc->move_info.time = getTimer()+800;
			} else if (temp_npc->move_info.status == 1) { // shoot -> sleep
				temp_npc->move_info.time = getTimer() + 2000;
				temp_npc->move_info.status = 0;
			}
		} else if (temp_npc->IA == IA_GHOST) {
			exec_IA_GHOST(temp_npc);
		} else if (temp_npc->IA == IA_FISH) {
			exec_IA_FISH(temp_npc);
		} else if (temp_npc->IA == IA_DOLPHIN) {
			exec_IA_DOLPHIN(temp_npc);
		} else if (temp_npc->IA == IA_SIDETOSIDE) {
			exec_IA_SIDETOSIDE(temp_npc);
		} else {
			printf(">>>>>>>>>>>>>> UNDEFINED IA %d <<<<<<<<<<<<<<\n",temp_npc->IA);
		}
		npc_gravity(temp_npc);

	}
}



void shot_rainbow(struct npc_struct *temp_npc) {
	if (temp_npc->projectile.x != -1 && temp_npc->projectile.y != -1) {
		printf(">> shot_rainbow() - can't start\n");
		return;
	}
	char tempBuffer[255];

	temp_npc->projectile.type = SHOT_ARC;
	temp_npc->projectile.direction = temp_npc->direction;
	temp_npc->projectile.x = temp_npc->x;
	temp_npc->projectile.y = temp_npc->y + temp_npc->sprite_list->w;
	if (temp_npc->direction == ANIM_LEFT) {
		temp_npc->projectile.x0 = temp_npc->x - RES_W/3;
	} else {
		temp_npc->projectile.x0 = temp_npc->x + RES_W/3;
	}
	temp_npc->projectile.y0 = temp_npc->projectile.y;
	temp_npc->projectile.y1 = temp_npc->projectile.y;
	sprintf(tempBuffer, "%sdata/images/projectiles/rainbow.png", FILEPATH);
	temp_npc->projectile.sprite = surface_from_image(tempBuffer);
	temp_npc->projectile.timer = getTimer() + 10;
}


void anim_arc_projectile(struct npc_struct *temp_npc) {
	int tempY, diffY;
	//(X -Xo)² + (Y- Yo)² = R²
	//(x - a)2 + (y - b)2 = r2
	// y - y0 = raiz(raio² - (x-x0)²);
	//// y = raiz(raio² - (x-x0)²) + y0;
	if (temp_npc->projectile.x != -1 && temp_npc->projectile.y != -1) {
		if (temp_npc->projectile.timer > getTimer()) {
			copy_area(temp_npc->projectile.sprite, 0, 0, temp_npc->projectile.sprite->w, temp_npc->projectile.sprite->h, temp_npc->projectile.x+map_pos_x, temp_npc->projectile.y1, game_screen);
			return;
		}
		tempY = temp_npc->projectile.y;

		if (temp_npc->projectile.direction == ANIM_LEFT) {
			temp_npc->projectile.x-= MOVE_STEP;
		} else {
			temp_npc->projectile.x+= MOVE_STEP;
		}

		int part1, part2, part3;
		part1 = pow((RES_W/3),2);
		part2 = pow((temp_npc->projectile.x - temp_npc->projectile.x0), 2);
		part3 = temp_npc->projectile.y0;
		//printf(">> part1: %d, temp_npc->projectile.x: %d, temp_npc->projectile.x0: %d, part2: %d, part3: %d\n", part1, temp_npc->projectile.x, temp_npc->projectile.x0, part2, part3);

		temp_npc->projectile.y = sqrt(part1 - part2) + part3;
		diffY = tempY-temp_npc->projectile.y;

		// check colision
		//colid_enemy = check_colision_enemy(temp_list->x - map_pos_x, temp_list->y - map_pos_y, 4, 4, temp_list->speed, 0, temp_list->owner);
		//short int check_colision_enemy(const short int obj_x, const short int obj_y, const short int obj_w, const short int obj_h, const short int x_inc, const short int y_inc, const short int owner) {
		int colid_enemy = check_colision_enemy(temp_npc->projectile.x, temp_npc->projectile.y1, temp_npc->projectile.sprite->w, temp_npc->projectile.sprite->h, MOVE_STEP, diffY, PROJECTILE_NPC);
		if (colid_enemy > 0) {
			struct player_struct *playerObj;
			playerObj = getPlayerN(colid_enemy - 1);
			if (playerObj->hit_timer < getTimer() && playerObj->hit != 1) {
				damagePlayer(playerObj, 1);
			}
			temp_npc->projectile.x = -1;
			temp_npc->projectile.y = -1;
			SDL_FreeSurface(temp_npc->projectile.sprite);
			return;
		}


		temp_npc->projectile.y1 += diffY;
		if (temp_npc->projectile.y < 0) {
			temp_npc->projectile.x = -1;
			temp_npc->projectile.y = -1;
			SDL_FreeSurface(temp_npc->projectile.sprite);
			return;
		} else {
			temp_npc->projectile.timer = getTimer() + 10;
			copy_area(temp_npc->projectile.sprite, 0, 0, temp_npc->projectile.sprite->w, temp_npc->projectile.sprite->h, temp_npc->projectile.x+map_pos_x, temp_npc->projectile.y1, game_screen);
		}
	}
}

void anim_npc_projectile(struct npc_struct *temp_npc) {
	if (temp_npc->projectile.type == SHOT_PROGRESSIVE) {
		anim_progressive_projectile(temp_npc);
	} else if (temp_npc->projectile.type == SHOT_ARC) {
		anim_arc_projectile(temp_npc);
	}
}


void IAInvertDirection(struct npc_struct *temp_npc) {
	if (temp_npc->direction == ANIM_LEFT) {
		temp_npc->direction = ANIM_RIGHT;
	} else {
		temp_npc->direction = ANIM_LEFT;
	}
	temp_npc->move_info.status = IA_STAND;
}


int check_projectile_colision(struct npc_struct *temp_npc, int x, int y, int xinc, int yinc, int w, int h) {
	// check colision
	//colid_enemy = check_colision_enemy(temp_list->x - map_pos_x, temp_list->y - map_pos_y, 4, 4, temp_list->speed, 0, temp_list->owner);
	//short int check_colision_enemy(const short int obj_x, const short int obj_y, const short int obj_w, const short int obj_h, const short int x_inc, const short int y_inc, const short int owner) {
	int colid_enemy = check_colision_enemy(x, y, w, h, xinc, yinc, PROJECTILE_NPC);
	if (colid_enemy > 0) {
		struct player_struct *playerObj;
		playerObj = getPlayerN(colid_enemy - 1);
		if (playerObj->hit_timer < getTimer() && playerObj->hit != 1) {
			damagePlayer(playerObj, 1);
		}
		temp_npc->projectile.x = -1;
		temp_npc->projectile.y = -1;
		SDL_FreeSurface(temp_npc->projectile.sprite);
		return 1;
	}
	return 0;
}


void shot_flower(struct npc_struct *temp_npc) {
	printf(">> shot_flower()\n");
	if (temp_npc->projectile.x != -1 && temp_npc->projectile.y != -1) {
		printf(">> shot_flower() - can't start\n");
		return;
	}
	char tempBuffer[255];

	temp_npc->projectile.type = SHOT_PROGRESSIVE;
	temp_npc->projectile.direction = temp_npc->direction;
	temp_npc->projectile.x = temp_npc->x;
	sprintf(tempBuffer, "%sdata/images/projectiles/flower.png", FILEPATH);
	temp_npc->projectile.sprite = surface_from_image(tempBuffer);

	temp_npc->projectile.x = temp_npc->x + temp_npc->sprite_list->w/2;
	temp_npc->projectile.y = temp_npc->y + temp_npc->sprite_list->h - temp_npc->projectile.sprite->h/2;
	temp_npc->projectile.x0 = temp_npc->projectile.x;
	temp_npc->projectile.y0 = temp_npc->projectile.y;
	temp_npc->projectile.timer = getTimer() + 30;
	printf(">> shot_flower() - x: %d, y: %d\n", temp_npc->projectile.x, temp_npc->projectile.y);
}

void anim_progressive_projectile(struct npc_struct *temp_npc) {
	if (temp_npc->projectile.x == -1 && temp_npc->projectile.y == -1) {
		return;
	}
	//printf(">> anim_progressive_projectile() - x: %d, y: %d, status0: %d\n", temp_npc->projectile.x, temp_npc->projectile.y, temp_npc->projectile.status0);
	int frameW = temp_npc->projectile.sprite->w/5;
	int frameH = temp_npc->projectile.sprite->h/2;


	if (temp_npc->projectile.timer > getTimer()) {
		//printf("copy_area A - dest.x: %d, dest.y: %d\n", temp_npc->projectile.x, temp_npc->projectile.y);
		copy_area(temp_npc->projectile.sprite, temp_npc->projectile.status0*(frameW), 0, frameW, frameH, temp_npc->projectile.x+map_pos_x, temp_npc->projectile.y, game_screen);
		return;
	}


	if (check_projectile_colision(temp_npc, temp_npc->projectile.x, temp_npc->projectile.y, 0, 0, frameW, frameH)) {
		return;
	}

	int diffX = abs(temp_npc->projectile.x-temp_npc->projectile.x0);
	if (diffX < -TILE_SIZE || diffX > RES_W+TILE_SIZE) {
		//printf("*** destroy projectile\n");
		temp_npc->projectile.x = -1;
		temp_npc->projectile.y = -1;
		SDL_FreeSurface(temp_npc->projectile.sprite);
		return;
	}
	//printf("copy_area B - dest.x: %d, dest.y: %d, temp_npc->projectile.status0: %d\n", temp_npc->projectile.x, temp_npc->projectile.y, temp_npc->projectile.status0);
	copy_area(temp_npc->projectile.sprite, temp_npc->projectile.status0*(frameW), 0, frameW, frameH, temp_npc->projectile.x+map_pos_x, temp_npc->projectile.y, game_screen);

	temp_npc->projectile.timer = getTimer() + 120;

	if (temp_npc->projectile.status0 == 4) {
		temp_npc->projectile.status1 = 1;
	} else if (temp_npc->projectile.status0 == 0) {
		if (temp_npc->projectile.direction == ANIM_LEFT) {
			temp_npc->projectile.x-= TILE_SIZE*2;
		} else {
			temp_npc->projectile.x+= TILE_SIZE*2;
		}
		temp_npc->projectile.status1 = 0;
	}
	if (temp_npc->projectile.status1 == 0) {
		temp_npc->projectile.status0++;
	} else {
		temp_npc->projectile.status0--;
	}
}



#endif // NPCS_H



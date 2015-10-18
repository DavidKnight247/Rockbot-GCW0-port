//TODO checar mudanças, tem bastante coisa para ser concertada;
#include <math.h>
#ifndef PROJECTILES_H
#define PROJECTILES_H

#define PROJECTILE_PLAYER 0
#define PROJECTILE_NPC 1



short int check_colision_enemy(const short int obj_x, const short int obj_y, const short int obj_w, const short int obj_h, const short int x_inc, const short int y_inc, const short int owner) {
	struct pos p1, p2, p3, p4, npc1, npc2, npc3, npc4;
	struct player_struct *temp_player;
	struct npc_struct *temp_npc;
	short int i;
	int mapScrollX;
	//short int reduce = (obj_w-16)*0.5;
	short int reduce = 0;

	//printf(">>> check_colision_enemy - obj_x: %d, obj_y: %d, obj_w: %d, obj_h: %d, x_inc: %d, y_inc: %d\n", obj_x, obj_y, obj_w, obj_h, x_inc, y_inc);

	if (scrollX >= 0) {
		mapScrollX = scrollX;
	} else {
		mapScrollX = MAP_W*TILE_SIZE -abs(scrollX) - MAX_W;
	}


	p1.x = obj_x + x_inc + reduce;
	p1.y = obj_y + y_inc;

	// ponto 2, baixo/esquerda
	p2.x = obj_x + x_inc + reduce;
	p2.y = obj_y + y_inc + obj_h;

	// ponto 3, topo/direita
	p3.x = obj_x + x_inc + obj_w - reduce;
	p3.y = obj_y + y_inc;

	// ponto 4, baixo/direita
	p4.x = obj_x + x_inc + obj_w - reduce;
	p4.y = obj_y + y_inc + obj_h;

	if (owner == PROJECTILE_PLAYER) {
		// check colision with NPCs
		temp_npc = npc_list[currentMap];
		//printf("DEBUG.check_colision - NPC search start\n");
		i = 1;
		while (temp_npc != NULL) {
			if (temp_npc->sprite_list != NULL) {
				//printf("DEBUG.check_colision - NPC counter\n");
				if (temp_npc->HP > 0) {
					npc1.x = temp_npc->x;
					npc1.y = temp_npc->y;
					npc2.x = temp_npc->x + temp_npc->sprite_list->w;
					npc2.y = temp_npc->y;
					npc3.x = temp_npc->x;
					npc3.y = temp_npc->y + temp_npc->sprite_list->h;
					npc4.x = temp_npc->x + temp_npc->sprite_list->w;
					npc4.y = temp_npc->y + temp_npc->sprite_list->h;
					//printf(">>> check_colision_enemy - i: %d, p1.x: %d, p1.y: %d, npc1.x: %d, npc1.y: %d\n", i, p1.x, p1.y, npc1.x, npc1.y);
					//printf(">>> check_colision_enemy - i: %d, p4.x: %d, p4.y: %d, npc4.x: %d, npc4.y: %d\n", i, p4.x, p4.y, npc4.x, npc4.y);
					// any point inside NPC is a hit
					if (p1.x >= npc1.x && p1.x <= npc2.x && p1.y >= npc1.y && p1.y <= npc3.y) {
						//printf("DEBUG.check_collision - npc hit 1\n");
						return i;
					} else if (p2.x >= npc1.x && p2.x <= npc2.x && p2.y >= npc1.y && p2.y <= npc3.y) {
						//printf("DEBUG.check_collision - npc hit 2\n");
						return i;
					} else if (p3.x >= npc1.x && p3.x <= npc2.x && p3.y >= npc1.y && p3.y <= npc3.y) {
						//printf("DEBUG.check_collision - npc hit 4\n");
						return i;
					} else if (p4.x >= npc1.x && p4.x <= npc2.x && p4.y >= npc1.y && p4.y <= npc3.y) {
						//printf("DEBUG.check_collision - npc hit 4\n");
						return i;
					} else {
						//printf("DEBUG.check_collision - no npc hit\n");
					}
				}
			}
			i++;
			temp_npc = temp_npc->next;
		}
	} else if (owner == PROJECTILE_NPC) {
		// check colision with PLAYERS
		temp_player = player_list;
		//printf("DEBUG.check_colision - NPC search start\n");
		i = 1;
		while (temp_player != NULL) {
			//printf("DEBUG.check_colision PLAYER - x: %d, y: %d, HP: %d\n", temp_player->sprite->x, temp_player->sprite->y, temp_player->HP);
			if (temp_player->HP > 0) {
				int diff_h = 0;
				if (temp_player->sprite->h > PLAYER_H) {
					diff_h = temp_player->sprite->h-PLAYER_H;
				}
				npc1.x = temp_player->sprite->x;
				npc1.y = temp_player->sprite->y+diff_h;
				npc2.x = temp_player->sprite->x + temp_player->sprite->w;
				npc2.y = temp_player->sprite->y+diff_h;
				npc3.x = temp_player->sprite->x;
				npc3.y = temp_player->sprite->y + temp_player->sprite->h;
				npc4.x = temp_player->sprite->x + temp_player->sprite->w;
				npc4.y = temp_player->sprite->y + temp_player->sprite->h;
				//printf("DEBUG.checkcollsion PLAYER - i: %d, p1.x: %d, p1.y: %d, npc1.x: %d, npc1.y: %d\n", i, p1.x, p1.y, npc1.x, npc1.y);
				// any point inside NPC is a hit
                                if (p1.x >= npc1.x && p1.x <= npc2.x && p1.y >= npc1.y && p1.y <= npc3.y) {
					//printf("DEBUG.check_collision - PLAYER hit 1\n");
					return i;
                                } else if (p2.x >= npc1.x && p2.x <= npc2.x && p2.y >= npc1.y && p2.y <= npc3.y) {
					//printf("DEBUG.check_collision - PLAYER hit 2\n");
					return i;
                                } else if (p3.x >= npc1.x && p3.x <= npc2.x && p3.y >= npc1.y && p3.y <= npc3.y) {
					//printf("DEBUG.check_collision - PLAYER hit 4\n");
					return i;
                                } else if (p4.x >= npc1.x && p4.x <= npc2.x && p4.y >= npc1.y && p4.y <= npc3.y) {
					//printf("DEBUG.check_collision - PLAYER hit 4\n");
					return i;
				} else {
					//printf("DEBUG.check_collision - no PLAYER hit\n");
				}
			}
			i++;
			temp_player = temp_player->next;
		}
	}
	return 0;
}


void remove_projectile(struct projectile *projectile_address) {
	struct projectile *temp_list, *temp_ant, *temp_aux;

	temp_list = projectile_list;
	temp_ant = NULL;
	while (temp_list) {
		if (temp_list == projectile_address) {
			if (temp_list->owner_player != NULL) {
				temp_list->owner_player->shots = temp_list->owner_player->shots - 1;
			} else if (temp_list->owner_npc != NULL) {
				temp_list->owner_npc->shots = (temp_list->owner_npc->shots) - 1;
			}

			temp_aux = temp_list->next;
			free(temp_list);
			temp_list = NULL;
			temp_list = temp_aux;
			if (temp_ant != NULL) {
				temp_ant->next = temp_list;
			} else {
				projectile_list = temp_list;
			}
		} else {
			temp_ant = temp_list;
			temp_list = temp_list->next;
		}
	}
}

// TODO: add support for setting trajectory and tile from arguments
struct projectile *create_projectile(const int tempX, const int tempY, const int type, const short int direction, SDL_Surface *projectile_surface, int projectile_trajectory) {
	struct projectile *new_item, *temp_list;


	new_item = (struct projectile *)malloc(sizeof(struct projectile));
	new_item->tile = NULL;
	temp_list = projectile_list;


	if (type == -1 && projectile_surface == NULL) {
		new_item->tile = normal_projectile;
	} else {
		if (projectile_surface == NULL) {
			new_item->tile = special_projectile[type];
		} else {
			new_item->tile = projectile_surface;
		}
	}
	new_item->type = type;
	new_item->trajectory = TRAJECTORY_LINEAR;
	new_item->x = tempX;
	new_item->y = tempY;
	new_item->x0 = tempX;
	new_item->y0 = tempY;
	new_item->y1 = tempY;
	new_item->direction = direction;
	new_item->status = 0;
	new_item->speed = PROJECTILE_SPEED;

	if (type == DAISIEBOT) {
		new_item->trajectory = TRAJECTORY_ARC;
		if (new_item->direction == ANIM_LEFT) {
			new_item->x0 = tempX - RES_W/3;
		} else {
			new_item->x0 = tempX + RES_W/3;
		}
	} else if (type == SPIKEBOT) {
		new_item->trajectory = TRAJECTORY_CHAIN;
		if (new_item->direction == ANIM_LEFT) {
			new_item->x = tempX + new_item->tile->w - TILE_SIZE;
		} else {
			new_item->x = tempX;
		}
		new_item->x0 = new_item->x;
		//printf(">> 0. TRAJECTORY_CHAIN - diffX: %d\n", abs(new_item->x - new_item->x0));
		new_item->speed = 3;
	} else if (type == MAGEBOT) {
		new_item->trajectory = TRAJECTORY_SIN;
		if (new_item->direction != ANIM_LEFT) {
			new_item->x += TILE_SIZE;
		}
		new_item->x0 = 0;
	} else if (type == TECHNOBOT) {
		new_item->trajectory = TRAJECTORY_BOMB;
		new_item->x = tempX - map_pos_x;
		new_item->speed = 3;
		new_item->x0 = 0; // used as timer
	}
	if (projectile_trajectory != -1) {
		new_item->trajectory = projectile_trajectory;
	}

	new_item->next = NULL;

	if (!temp_list) {
		projectile_list = new_item;
	} else {
                while (temp_list->next != NULL) {
			temp_list = temp_list->next;
		}
		temp_list->next = new_item;
	}

	return new_item;
}

void show_projectile(SDL_Surface *screen, struct projectile *temp_list) {
	SDL_Rect src, dest;
	//printf("DEBUG.show_projectile - START\n");
	// copia fundo para underscreen
        int n=0;

            // copia tile para screen
            if (temp_list->trajectory == TRAJECTORY_CHAIN) {
                    if (temp_list->direction == ANIM_LEFT) {
                            src.x = 0;
                            src.y = temp_list->tile->h * 0.5;
                            src.w = abs(temp_list->x - temp_list->x0);
                            src.h = temp_list->tile->h * 0.5;
                            dest.x = temp_list->x - TILE_SIZE*2;
                    } else {
                            src.x = temp_list->tile->w - abs(temp_list->x - temp_list->x0);
                            src.y = 0;
                            src.w = abs(temp_list->x - temp_list->x0);
                            src.h = temp_list->tile->h * 0.5;
                            dest.x = temp_list->x0;
                    }
                    if (temp_list->owner_player != NULL && temp_list->owner_npc == NULL) {
                            dest.y = temp_list->owner_player->sprite->y;
                            if (temp_list->owner_player->sprite->anim_type == ANIM_JUMP_ATTACK || temp_list->owner_player->sprite->anim_type == ANIM_JUMP) {
                                    dest.y += 8;
                            } else {
                                    dest.y += 17;
                            }
                    } else if (temp_list->owner_player != NULL && temp_list->owner_npc == NULL) {
                            dest.y = temp_list->owner_npc->y;
							dest.y += temp_list->owner_npc->sprite_list->h * 0.5;
                    } else {
                            dest.y = temp_list->y - (abs(map_pos_y) - abs(temp_list->map_y));
                    }
            } else if (temp_list->trajectory == TRAJECTORY_BOMB) {
                    if (temp_list->x0 < 100) {
                            src.x = 0;
                    } else if (temp_list->x0 < 300) {
                            if ((temp_list->x0/6) % 2 == 0) {
                                    src.x = 17;
                            } else {
                                    src.x = 0;
                            }
                    } else { // explosion
                            remove_projectile(temp_list);
                            return;
                    }
                    src.y = 0;
                    src.w = temp_list->tile->w/2;
                    src.h = temp_list->tile->h;
                    dest.x = temp_list->x + map_pos_x;
                    //printf("*** TIMER_BOM - dest.x: %d, map_pos_x: %d ***\n", dest.x, map_pos_x);
                    if (dest.x < 0 || dest.x > RES_W + TILE_SIZE) {
                            remove_projectile(temp_list);
                            return;
                    }
                    dest.y = temp_list->y - (abs(map_pos_y) - abs(temp_list->map_y));
                    temp_list->x0++;
			} else {
				if (temp_list->tile) {
                    src.x = 0;
                    src.y = 0;
                    src.w = temp_list->tile->w;
                    src.h = temp_list->tile->h;
                    dest.x = temp_list->x;
                    dest.y = temp_list->y - (abs(map_pos_y) - abs(temp_list->map_y));
				} else {
					printf("ERROR: failed to find projectile graphic.\n");
					return;
				}
            }
            //printf("DEBUG.show_projectile - n: %d, proj.x: %d, proj.map_x: %d, map_x: %d\n", n, temp_list->x, temp_list->map_x, map_pos_x);
            n++;
            dest.w = temp_list->tile->w;
            dest.h = temp_list->tile->h;
            if (temp_list->tile != NULL) {
                    SDL_BlitSurface(temp_list->tile, &src, screen, &dest);
            } else {
                    printf("ERROR: weapon surface not found!\n");
					return;
            }
            //printf("DEBUG.show_projectile - END\n");
}

// TODO - limit also per player
short int throw_projectile_player(struct player_struct *playerObj, int xMod, int yMod) {
	short int selected_weapon;

	//printf(">> throw_projectile_player - START <<\n");

	if (playerObj->shots >= playerObj->shots_max) {
		return 0;
	}

	if (playerObj->selected_weapon != -1 && playerObj->items.weapons[playerObj->selected_weapon] <= 0) {
		return 0;
	}

	selected_weapon = playerObj->selected_weapon;

	// those types do not throw any projectile, but instead use a screen effect
	if ((selected_weapon == APEBOT && playerObj->sprite->anim_type != ANIM_JUMP) || selected_weapon == DYNAMITEBOT) {
		printf(">> effect weapon start <<\n");
		if (playerObj->weapon_state == -1) {
			playerObj->weapon_timer = 0;
			playerObj->weapon_state = 0;
			playerObj->items.weapons[playerObj->selected_weapon]--;
			return 1;
		} else {
			//printf(">> firing while weapon in effect <<\n");
			selected_weapon = -1; // -2 indicates that you are using an effect weapon that while active can shoot regular projectiles
			//return 0;
		}
	} else if (playerObj->selected_weapon == APEBOT && playerObj->sprite->anim_type == ANIM_JUMP) { // fired a ground weapon in air, no effect
		//printf(">>>>>>>>>>>>>>>>>>> throw_projectile_player - firing apebot in air\n");
		return 0;
	} else {
		playerObj->items.weapons[playerObj->selected_weapon]--;
	}
	//printf(">>>>>>>>>> throw_projectile_player - create projectile, selected_weapon: %d\n", selected_weapon);
	struct projectile *new_proj;
	int adjustX;
	if (playerObj->sprite->direction == ANIM_RIGHT) {
		adjustX = playerObj->sprite->w;
	} else {
		adjustX = 0;
	}
	new_proj = create_projectile((playerObj->sprite->x + map_pos_x + adjustX + xMod), (playerObj->sprite->y + map_pos_y + (TILE_SIZE) + yMod), selected_weapon, playerObj->sprite->direction, NULL, -1);
	playerObj->shots = playerObj->shots + 1;
	new_proj->owner_player = playerObj;
	new_proj->owner_npc = NULL;
	new_proj->owner = PROJECTILE_PLAYER;
	new_proj->map_x = map_pos_x;
	new_proj->map_y = map_pos_y;
	//printf(">> throw_projectile_player - END <<\n");
	return 1;
}

// TODO - limit also per player
// type can be normal or diagonal
// n is the number os projectiles to be shot
// alternated indicates if direction is the same for each shot or not
// vdirection indicates if (for diagonal shoots) up (0) or down (1)
void throw_projectile_npc(struct npc_struct *npcObj, int type, int n, int alternated, int vdirection) {
	short int i;
	short int last_direction = -1;

	for (i=0; i<n; i++) {
		// TODO: add support for NPC set of different weapons
		struct projectile *new_proj = create_projectile((npcObj->x + map_pos_x + (TILE_SIZE) *0.5), (npcObj->y + map_pos_y + npcObj->sprite_list->h * 0.5), -1, npcObj->direction, NULL, -1);
		new_proj->owner = PROJECTILE_NPC;
		new_proj->map_x = map_pos_x;
		new_proj->map_y = map_pos_y;
		npcObj->shots = npcObj->shots + 1;
		new_proj->owner_player = NULL;
		new_proj->owner_npc = npcObj;
		new_proj->speed = 6;
		new_proj->vdirection = vdirection;
		// invert direction if needed
		if (alternated == 1) {
			if (last_direction == -1) {
				last_direction = npcObj->direction;
			} else {
				if (new_proj->direction == ANIM_LEFT) {
					new_proj->direction = ANIM_RIGHT;
				} else {
					new_proj->direction = ANIM_LEFT;
				}
				last_direction = -1;
			}

		}
		if (type == PROJECTLE_TYPE_DIAGONAL) {
			if (new_proj->direction == ANIM_LEFT) {
				new_proj->direction = ANIM_LEFT_DIAGONAL;
			} else {
				new_proj->direction = ANIM_RIGHT_DIAGONAL;
			}
		}
		//printf("DEBUG.throw_projectile_npc - creating projectile\n");
	}
}



void anim_projectiles() {
	short colid_enemy;
	struct npc_struct *temp_npc;
	struct player_struct *playerObj;
	struct projectile *temp_list;
	temp_list = projectile_list;
	short int must_remove;

	while (temp_list != NULL) {
		must_remove = 0;
		show_projectile(game_screen, temp_list);

		if (temp_list->trajectory == TRAJECTORY_LINEAR) {
			if (temp_list->direction == ANIM_LEFT) {
				temp_list->x -= temp_list->speed;
			} else if (temp_list->direction == ANIM_RIGHT) {
				temp_list->x += temp_list->speed;
			} else if (temp_list->direction == ANIM_LEFT_DIAGONAL) {
				temp_list->x -= temp_list->speed;
				if (temp_list->vdirection == 0) {
					temp_list->y -= temp_list->speed;
				} else {
					temp_list->y += temp_list->speed;
				}
			} else if (temp_list->direction == ANIM_RIGHT_DIAGONAL) {
				temp_list->x += temp_list->speed;
				if (temp_list->vdirection == 0) {
					temp_list->y -= temp_list->speed;
				} else {
					temp_list->y += temp_list->speed;
				}
			}
		} else if (temp_list->trajectory == TRAJECTORY_ARC) {
			short int tempY1 = temp_list->y1;
			if (temp_list->direction == ANIM_LEFT) {
				temp_list->x-= temp_list->speed;
			} else {
				temp_list->x+= temp_list->speed;
			}
			int part1, part2, part3;
			part1 = pow((RES_W/3), 2);
			part2 = pow((temp_list->x - temp_list->x0), 2);
			part3 = temp_list->y0;
			//printf(">> TRAJECTORY_ARC - part1: %d, temp_list.x: %d, temp_list.x0: %d, part2: %d, part3: %d\n", part1, temp_list->x, temp_list->x0, part2, part3);

			temp_list->y1 = sqrt(part1 - part2) + part3;
			short int diffY = tempY1 - temp_list->y1;
			if (diffY > 0 || diffY > -20) {
				temp_list->y += diffY;
			}
			//printf(">> TRAJECTORY_ARC - x0: %d, x: %d, y1 (antes): %d, y1 (depois): %d, diffY: %d, y: %d\n", temp_list->x0, temp_list->x, tempY1, temp_list->y1, diffY, temp_list->y);
		} else if (temp_list->trajectory == TRAJECTORY_CHAIN) {
			if (temp_list->status == 0) { // going forward
				//printf(">> 1. TRAJECTORY_CHAIN - diffX: %d\n", abs(temp_list->x - temp_list->x0));
				if (temp_list->direction == ANIM_LEFT) {
					temp_list->x -= temp_list->speed;
					//printf(">> 2. TRAJECTORY_CHAIN - diffX: %d\n", abs(temp_list->x - temp_list->x0));
					if (abs(temp_list->x - temp_list->x0) > temp_list->tile->w) {
						temp_list->status = 1;
					}
				} else {
					temp_list->x += temp_list->speed;
					//printf(">> 2. TRAJECTORY_CHAIN - diffX: %d\n", abs(temp_list->x - temp_list->x0));
					if (abs(temp_list->x - temp_list->x0) > temp_list->tile->w + TILE_SIZE) {
						temp_list->status = 1;
					}
				}
			} else { // returning
				if (temp_list->direction == ANIM_LEFT) {
					temp_list->x+= temp_list->speed;
					if (temp_list->x >= temp_list->x0) { // finished movement, destroy projectile
						must_remove = 2;
					}
				} else {
					temp_list->x-= temp_list->speed;
					if (temp_list->x <= temp_list->x0) { // finished movement, destroy projectile
						must_remove = 2;
					}
				}
			}
		} else if (temp_list->trajectory == TRAJECTORY_SIN) {
			temp_list->x0 += temp_list->speed;
			if (temp_list->x0 > 360) {
				temp_list->x0 = 0;
			}
			if (temp_list->direction == ANIM_LEFT) {
				temp_list->x -= temp_list->speed;
			} else {
				temp_list->x += temp_list->speed;
			}
			temp_list->y = sin(temp_list->x0)*TILE_SIZE*2 + temp_list->y0;
		} else if (temp_list->trajectory == TRAJECTORY_BOMB) {
			// do nothing, it is a bomb, it just stays until explodes
		} else if (temp_list->trajectory == TRAJECTORY_FOLLOW) { // TODO: add support for player-2
			if (p1Obj->sprite->x > temp_list->x) {
				temp_list->x += temp_list->speed;
			}
			if (p1Obj->sprite->x < temp_list->x) {
				temp_list->x -= temp_list->speed;
			}
			if (p1Obj->sprite->y > temp_list->y) {
				temp_list->y += temp_list->speed;
			}
			if (p1Obj->sprite->y < temp_list->y) {
				temp_list->y -= temp_list->speed;
			}
		}

		colid_enemy = check_colision_enemy(temp_list->x - map_pos_x, temp_list->y - map_pos_y, 4, 4, temp_list->speed, 0, temp_list->owner);
		// remove sprites out of screen



		if (temp_list->x < (temp_list->speed * 2) || temp_list->x > MAX_W + temp_list->speed) {
			if (temp_list->trajectory != TRAJECTORY_BOMB) {
				must_remove = 1;
			}
			//check_colision(short int obj_x, short int obj_y, short int obj_w, short int obj_h, short int x_inc, short int y_inc)
			} else if (temp_list->y < (temp_list->speed * 2) || temp_list->y > MAX_H + temp_list->speed) {
				must_remove = 1;
			} else if (colid_enemy > 0) {
				if (temp_list->owner == PROJECTILE_PLAYER) {
					temp_npc = getNPCN(colid_enemy - 1);
					// checks if the npc have a shiled and if frame animation is #0 (stand with shield)
					if (temp_npc->shield == 0 || (temp_npc->shield == 1 && (temp_npc->sprite_list->anim_pos != 0 || temp_npc->sprite_list->anim_type != ANIM_STAND)) || (temp_npc->shield == 2 && (temp_list->direction == temp_npc->direction || temp_npc->sprite_list->anim_type != ANIM_STAND))) {
						int damage_points = 1;
						if (temp_list->type == DAISIEBOT && strstr(temp_npc->name, "Techno Bot")) {
							damage_points = temp_npc->TOTAL_HP/5;
						}
						if (temp_list->type == DYNAMITEBOT && strstr(temp_npc->name, "Mage Bot")) {
							damage_points = 2;
						}
						if (temp_list->type == MUMMYBOT && strstr(temp_npc->name, "Spike Bot")) {
							damage_points = temp_npc->TOTAL_HP/8;
						}
						if (temp_list->type == SPIKEBOT && strstr(temp_npc->name, "Ape Bot")) {
							damage_points = temp_npc->TOTAL_HP/7;
						}
						// DEBUG
						if (strstr(temp_npc->name, "Ape Bot")) {
							damage_points = 10;
						}
						if (temp_list->type == TECHNOBOT && strstr(temp_npc->name, "Mummy Bot")) {
							damage_points = temp_npc->TOTAL_HP/3;
						}
						if (temp_list->type == MAGEBOT && strstr(temp_npc->name, "Seahorse Bot")) {
							damage_points = temp_npc->TOTAL_HP/6;
						}
						if (temp_list->type == SEAHORSEBOT && strstr(temp_npc->name, "Daisie Bot")) {
							damage_points = temp_npc->TOTAL_HP/5;
						}

						//printf(">> npc.type: %d, npc.hp: %d, npc.name: '%s', damage_points: %d <<\n", temp_list->type, temp_npc->HP, temp_npc->name, damage_points);

						damage_npc(temp_npc, damage_points);
						must_remove = 1;
					} else {
						play_sfx(SFX_SHOT_REFLECTED);
						if (temp_list->direction == ANIM_LEFT) {
							temp_list->direction = ANIM_RIGHT_DIAGONAL;
						} else {
							temp_list->direction = ANIM_LEFT_DIAGONAL;
						}
					}
				} else if (temp_list->owner == PROJECTILE_NPC) {
					playerObj = getPlayerN(colid_enemy - 1);
					if (playerObj->hit_timer < getTimer() || playerObj->hit != 1) {
						damagePlayer(playerObj, 1);
					//} else {
						//printf(">> player can't be hit - hiy: %d, hit_timer: %d <<\n", playerObj->hit, playerObj->hit_timer);
					}
				}
			}
			if (must_remove == 1 && temp_list->trajectory != TRAJECTORY_CHAIN) {
				remove_projectile(temp_list);
				return;
			} else if (must_remove == 2) {
				remove_projectile(temp_list);
				return;
			}
			temp_list = temp_list->next;
	}

	// non projectile weapons
	anim_non_projectile_weapons(p1Obj);
	if (game_config.two_players == 2) {
		anim_non_projectile_weapons(p2Obj);
	}
}


void anim_non_projectile_weapons(struct player_struct *playerObj) {
	if (p1Obj->weapon_state != -1) {
		if (p1Obj->selected_weapon == APEBOT) {
			if (p1Obj->weapon_timer < getTimer()) {
				//printf(">> APEBOT weapon MOVE <<\n");
				if (p1Obj->weapon_state % 2) {
					//printf(">> APEBOT weapon LEFT <<\n");
					move_screen(game_screen, -10, 0);
				} else {
					if (p1Obj->weapon_state % 4) {
						//printf(">> APEBOT weapon DAMAGE <<\n");
						ground_damage_npcs(playerObj);
					}
					//printf(">> APEBOT weapon RIGHT <<\n");
					move_screen(game_screen, 10, 0);
				}
				p1Obj->weapon_state++;
				p1Obj->weapon_timer = getTimer() + 80;
				if (p1Obj->weapon_state >= 16) {
					//printf(">> APEBOT weapon FINISHED <<\n");
					p1Obj->weapon_state = -1;
				}
			}
		} else if (p1Obj->selected_weapon == DYNAMITEBOT) {
			if (p1Obj->weapon_state == 0) {
				p1Obj->weapon_timer = getTimer() + 5000;
				blink_screen(235, 235, 235);
				p1Obj->weapon_state = 1;
			} else if (p1Obj->weapon_state == 1 && p1Obj->weapon_timer < getTimer()) {
				p1Obj->weapon_state = -1;
			}
		}
	}
}

// --------------------------------------------------- //
//      damage all npcs that are in the ground         //
// --------------------------------------------------- //
void ground_damage_npcs(struct player_struct *playerObj) {
	struct npc_struct *temp;


	//printf(">> ground_damage_npcs::START\n");
	temp = npc_list[currentMap];

	while (temp != NULL) {
		if (temp->HP > 0 && temp->x+16 >= abs(map_pos_x) && temp->x-16 <= abs(map_pos_x)+RES_W) { // checks if npc is inside screen and alive
			// checks if npc is in the ground or above a platform
			int map_x = (temp->x)/TILE_SIZE;
			int map_y = (temp->y+temp->sprite_list->w+4)/TILE_SIZE;
			//printf(">> Found a NPC inside screen, map_x: %d, map_y: %d, lock: %d\n", map_x, map_y,map.tiles[map_x][map_y].locked);

			if (map.tiles[map_x][map_y].locked > 0) {
				//printf(">> NPC is in the ground, damage it\n");
				if (strstr(temp->name, "Dynamite Bot")) {
					damage_npc(temp, temp->TOTAL_HP/7);
				} else if (temp->is_boss == 1) {
					damage_npc(temp, 1);
				} else {
					damage_npc(temp, 4);
				}
			}
		}
		temp = temp->next;
	}
}




void remove_all_projectiles() {
	struct projectile *ant, *temp_list;

	//printf(">> 1. remove_all_projectiles() projectile_list: %p \n", projectile_list);

	temp_list = projectile_list;
	// position at the end, to remove the last element
	if (temp_list != NULL) {
		while (temp_list) {
			ant = temp_list;
			temp_list = temp_list->next;
			if (ant->owner_player) {
				ant->owner_player->shots = ant->owner_player->shots - 1;
				ant->owner_player = NULL;
			}
			if (ant->owner_npc) {
				ant->owner_npc->shots = ant->owner_npc->shots - 1;
				ant->owner_npc = NULL;
			}
			//SDL_FreeSurface(ant->underscreen);
			ant->tile = NULL;
			free(ant);
		}
	}
	projectile_list = NULL;
	//printf(">> 2. remove_all_projectiles() projectile_list: %p \n", projectile_list);
}


#endif // PROJECTILES_H

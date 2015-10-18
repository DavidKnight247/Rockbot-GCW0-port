#ifndef OBJECT_H
#define OBJECT_H

short int test_map_colision(struct player_struct *playerObj, const short int incx, const short int incy);


int remove_object(struct struct_object *obj) {
	struct struct_object *temp_obj_list = obj_list[currentMap];
	struct struct_object *obj_ant=NULL, *obj_aux=NULL;

	printf(">> WARNIBG: remove_object call <<\n");

	if (p1Obj->platform == obj) {
		p1Obj->platform = NULL;
	}
	if (p2Obj != NULL && p2Obj->platform == obj) {
		p2Obj->platform = NULL;
	}
	obj->type = -1;
	SDL_FreeSurface(obj->sprite);
	obj->sprite = NULL;

	while (temp_obj_list) {
		if (temp_obj_list == obj) {
			obj_aux = temp_obj_list->next;
			free(temp_obj_list);
			temp_obj_list = NULL;
			temp_obj_list = obj_aux;
			if (obj_ant != NULL) {
				obj_ant->next = temp_obj_list;
			} else {
				obj_list[currentMap] = temp_obj_list;
			}
			return 1;
		} else {
			obj_ant = temp_obj_list;
			temp_obj_list = temp_obj_list->next;
		}
	}
	return 0;
}

void show_object_sprites(SDL_Surface *screen, struct struct_object *obj) {
	SDL_Rect src, dest;

	// checks if the Object is near the screen to show it
	//printf("********* check draw object, x: %d, y: %d\n", obj->position.x, obj->position.y);
	if (obj->type == OBJ_DISAPPEARING_BLOCK && obj->direction == ANIM_RIGHT) {
		//printf("********* show_object_sprites - hidden object\n");
		return;
	}


	if (obj->position.x+16 >= abs(map_pos_x) && obj->position.x-16 <= abs(map_pos_x)+RES_W) {
		//printf("********* show_object_sprites - MUST DRAW\n");

		// animation
		//printf(">> show_object_sprites - sprite.w: %d, frame.w: %d <<\n", obj->sprite->w, obj->framesize_w);
		if (obj->framesize_w * 2 >= obj->sprite->w)  {
			if (obj->frame <= 0 || obj->frame > (obj->sprite->w/obj->framesize_w)-1) {
				obj->frame = 0;
			}
			src.x = obj->frame * obj->framesize_w;
			if (obj->timer < getTimer()) {
				obj->frame++;
				obj->timer = getTimer()+400;
			}
		} else {
			src.x = 0;
		}
		// direction
		//printf(">> obj->sprite->h: %d, obj->framesize_h*2: %d <<\n", obj->sprite->h, (obj->framesize_h*2));
		if (obj->framesize_h*2 <= obj->sprite->h)  {
			if (obj->direction != ANIM_RIGHT) {
				src.y = 0;
			} else {
				src.y = obj->framesize_h;
			}
		} else {
			src.y = 0;
		}

		//printf(">>> show_object_sprites - src.x: %d, src.y: %d\n", src.x, src.y);

		src.w = obj->framesize_w;
		src.h = obj->framesize_h;
		// parte que vai ser colada
		dest.x = obj->position.x + map_pos_x;
		dest.y = obj->position.y + map_pos_y;
		//printf(">>> show_object_sprites - dest.x: %d, dest.y: %d\n", dest.x, dest.y);
		dest.w = obj->framesize_w;
		dest.h = obj->framesize_h;
		//printf("*** 2 - show_object_sprites - dest.x: %d, dest.y: %d, dest.w: %d, dest.h: %d\n", dest.x, dest.y, dest.w, dest.h);
		SDL_BlitSurface(obj->sprite, &src, screen, &dest);
	// remove item that is out of vision
	} else if (obj->type == OBJ_ITEM_FLY || obj->type == OBJ_ITEM_JUMP) {
		printf(">> remove item <<\n");
		obj->type = -1;
	}
}


// check colision of Player with Objects
// returns 1 for x block, 2 for y block, 3 for x AND y block and 0 for non-blocking items
short int colision_player_object(struct player_struct *playerObj, const short int x_inc, const short int y_inc) {
	struct pos p1, p2, p3, p4;
	struct pos obj1, obj2, obj3, obj4;
	short int passX=0, passY=0;
	int blocked = 0;
	short int reduce = 0;


	//reduce = abs(16-playerObj->sprite->w)*0.5;

	// ponto 3, topo/esquerda
	p1.x = playerObj->sprite->x + reduce;
	p1.y = playerObj->sprite->y;

	//printf(">> colision_player_object, p1.x: %d, p1.y: %d, x_inc: %d, y_inc: %d\n", p1.x, p1.y, x_inc, y_inc);


	// ponto 2, baixo/esquerda
	p2.x = playerObj->sprite->x + reduce;
	p2.y = playerObj->sprite->y + 31;

	// ponto 3, topo/direita
	p3.x = playerObj->sprite->x + playerObj->sprite->w - reduce;
	p3.y = playerObj->sprite->y;

	// ponto 4, baixo/direita
	p4.x = playerObj->sprite->x + playerObj->sprite->w - reduce;
	p4.y = playerObj->sprite->y + 31;

	// fix for ground checking
	p1.y = p1.y - 1;
	p2.y = p2.y - 1;
	p3.y = p3.y - 1;
	p4.y = p4.y - 1;


	struct struct_object *temp_obj_list = obj_list[currentMap];
	while (temp_obj_list) {

		if (temp_obj_list->type != -1 && strstr(temp_obj_list->filename, ".png") != NULL && (temp_obj_list->type != OBJ_DISAPPEARING_BLOCK || temp_obj_list->direction != ANIM_RIGHT)) {
			//printf("*** colision_player_object - obj[%d] is good\n", i);
			obj1.x = temp_obj_list->position.x;
			obj1.y = temp_obj_list->position.y;
			obj2.x = temp_obj_list->position.x;
			obj2.y = temp_obj_list->position.y + temp_obj_list->framesize_h;
			obj3.x = temp_obj_list->position.x + temp_obj_list->framesize_w;
			obj3.y = temp_obj_list->position.y;
			obj4.x = temp_obj_list->position.x + temp_obj_list->framesize_w;
			obj4.y = temp_obj_list->position.y + temp_obj_list->framesize_h;


			//printf("*** colision_player_object - testing obj[%d], p1.y: %d, obj1.y: %d\n", i, p1.y, obj1.y);

			// any point inside NPC is a hit
			passY = 0;
			passX = 0;

			// check if player is on the X range of object
			if (playerObj->sprite->w > temp_obj_list->framesize_w) {
				if ((obj1.x >= p1.x && obj1.x <= p3.x) || (obj3.x >= p1.x && obj3.x <= p3.x)) {
					passX = 1;
				}
			} else {
				if ((p1.x >= obj1.x && p1.x <= obj3.x) || (p3.x >= obj1.x && p3.x <= obj3.x)) {
					passX = 1;
				}
			}

			// check if player is on the Y range of object
			if (playerObj->sprite->h > temp_obj_list->framesize_h) {
				if ((obj1.y >= p1.y && obj1.y <= p2.y) || (obj2.y >= p1.y && obj2.y <= p2.y)) {
					passY = 1;
				}
			} else {
				if ((p1.y >= obj1.y && p1.y <= obj2.y) || (p2.y >= obj1.y && p2.y <= obj3.y)) {
					passY = 1;
				}
			}

			// player is on the left of the object, and moving to right
			if (passY && p1.x >= obj4.x && p1.x+x_inc <= obj4.x) {
				//printf("block X1\n");
				blocked = 1;
			}

			// player is on the right of the object, and moving to left
			if (passY && p3.x <= obj1.x && p3.x+1 >= obj1.x) {
				//printf(">> block X2\n");
				blocked = 1;
			}

			// player is under the object and moving to top
			if (passX && p1.y >= obj4.y && p1.y+y_inc <= obj4.y) {
				if (blocked == 0) {
					blocked = 2;
				} else {
					blocked = 3;
				}
			}
			// player is over the object and moving to bottom
			if (passX && p4.y <= obj1.y && p4.y+y_inc >= obj1.y) {
				if (blocked == 0) {
					blocked = 2;
				} else {
					blocked = 3;
				}
			}

			if (temp_obj_list->type == OBJ_MOVING_PLATFORM_UPDOWN || temp_obj_list->type == OBJ_MOVING_PLATFORM_LEFTRIGHT || temp_obj_list->type == OBJ_DISAPPEARING_BLOCK  || temp_obj_list->type == OBJ_FALL_PLATFORM) {
				if ((blocked == 2 || blocked == 3) && p4.y <= obj1.y) {
					playerObj->platform = temp_obj_list;
					if (temp_obj_list->type == OBJ_FALL_PLATFORM) {
						temp_obj_list->direction = ANIM_DOWN;
					}
				} else if (blocked == 1) {
					playerObj->platform = temp_obj_list;
				}
				if (blocked != 0) {
					return blocked;
				}
			} else if (temp_obj_list->type == OBJ_ITEM_FLY) {
				//printf(">> testing fly, p4.y: %d, obj.y: %d <<\n", p4.y, obj1.y);
				if ((blocked == 2 || blocked == 3) && p4.y <= obj1.y) {
					playerObj->platform = temp_obj_list;
					if (temp_obj_list->distance == 0) {
						temp_obj_list->distance = 1;
						temp_obj_list->timer = getTimer()+30;
					}
				}
				if (blocked != 0) {
					return blocked;
				}
			} else if (temp_obj_list->type == OBJ_ITEM_JUMP) {
				//printf(">> testing fly, p4.y: %d, obj.y: %d <<\n", p4.y, obj1.y);
				if ((blocked == 2 || blocked == 3) && p4.y <= obj1.y) {
					playerObj->platform = temp_obj_list;
					if (temp_obj_list->distance == 0) {
						temp_obj_list->distance = 1;
						temp_obj_list->direction = ANIM_UP;
						temp_obj_list->timer = getTimer()+30;
					}
				}
				if (blocked != 0) {
					return blocked;
				}
			} else if (blocked != 0) {
				if (temp_obj_list->type == OBJ_ENERGY_PILL_SMALL && playerObj->HP < playerObj->HP_MAX) {
					if (playerObj->HP + 3 <= playerObj->HP_MAX) {
						playerObj->HP += 3;
					} else {
						playerObj->HP = playerObj->HP_MAX;
					}
					play_sfx(SFX_GOT_ENERGY);
				} else if (temp_obj_list->type == OBJ_ENERGY_PILL_BIG && playerObj->HP < playerObj->HP_MAX) {
					if (playerObj->HP + 8 <= playerObj->HP_MAX) {
						playerObj->HP += 8;
					} else {
						playerObj->HP = playerObj->HP_MAX;
					}
					play_sfx(SFX_GOT_ENERGY);
					play_sfx(SFX_GOT_ENERGY);
				} else if (temp_obj_list->type == OBJ_WEAPON_PILL_SMALL && playerObj->selected_weapon != -1) {
					if (playerObj->items.weapons[playerObj->selected_weapon] + 3 <= playerObj->HP_MAX) {
						playerObj->items.weapons[playerObj->selected_weapon] += 3;
					} else {
						playerObj->items.weapons[playerObj->selected_weapon] = playerObj->HP_MAX;
					}
					play_sfx(SFX_GOT_ENERGY);
				} else if (temp_obj_list->type == OBJ_WEAPON_PILL_BIG && playerObj->selected_weapon != -1) {
					if (playerObj->items.weapons[playerObj->selected_weapon] + 8 <= playerObj->HP_MAX) {
						playerObj->items.weapons[playerObj->selected_weapon] += 8;
					} else {
						playerObj->items.weapons[playerObj->selected_weapon] = playerObj->HP_MAX;
					}
					play_sfx(SFX_GOT_ENERGY);
					play_sfx(SFX_GOT_ENERGY);
				} else if (temp_obj_list->type == OBJ_ENERGY_TANK) {
					playerObj->items.energy_tanks++;
					play_sfx(SFX_GOT_ITEM);
				} else if (temp_obj_list->type == OBJ_WEAPON_TANK) {
					playerObj->items.weapon_tanks++;
					play_sfx(SFX_GOT_ITEM);
				} else if (temp_obj_list->type == OBJ_LIFE) {
					playerObj->items.lifes++;
					play_sfx(SFX_GOT_ITEM);
				}
				temp_obj_list->type = -1;
				return 0;
			}
		}

		temp_obj_list = temp_obj_list->next;
	}

	// got out of platform
	if (blocked == 0 && playerObj->platform != NULL) {
		//  for player item, platform must only be removed only if the item was already adtivated
		if (playerObj->platform->type == OBJ_ITEM_FLY || playerObj->platform->type == OBJ_ITEM_JUMP) {
			if (playerObj->platform->distance > 0 && y_inc != 0) {
				printf("colision_player_object: got out of jump/fly item, blocked: %d, type: %d, direction: %d\n", blocked, playerObj->platform->type, playerObj->platform->direction);
				playerObj->platform = NULL;
			} else {
				return 0;
			}
		} else {
			printf("colision_player_object: got out of platform, xinc: %d, yinc: %d, type: %d\n", x_inc, y_inc, playerObj->platform->type);
			playerObj->platform = NULL;
		}
	}
	return blocked;
}

// TODO - adicionar suporte a p2
// TODO - somente mover objetos que estejam na tela
// TODO - verificação de colisão (está dando sempre 1 por algum motivo)
void moveObjects(struct struct_object *obj) {
	int limit;
	if (obj->type == OBJ_MOVING_PLATFORM_LEFTRIGHT) {
		if (obj->timer < getTimer()) {
			// if linit is not defined, move 4 tiles
			if (obj->limit > 0) {
				limit = obj->limit/2;
			} else {
				limit = 4*TILE_SIZE;
			}
			if (obj->distance < limit)  {
				if (obj->direction == ANIM_RIGHT) {
					obj->position.x += 2;
					if (p1Obj->platform == obj && (p1Obj->sprite->y+p1Obj->sprite->h-1 <= obj->position.y || p1Obj->sprite->x > obj->position.x)) {
						int res = test_map_colision(p1Obj, 2, 0);
						if (res != 1 && res != 3) {
							p1Obj->sprite->x += 2;
							checkScrolling(2, 0, p1Obj);
						}
					}
					if (p2Obj != NULL && p2Obj->platform == obj && (p2Obj->sprite->y+p2Obj->sprite->h-1 <= obj->position.y || p2Obj->sprite->x > obj->position.x)) {
						int res = test_map_colision(p2Obj, 2, 0);
						if (res != 1 && res != 3) {
							p2Obj->sprite->x += 2;
							checkScrolling(2, 0, p2Obj);
						}
					}
				} else {
					obj->position.x -= 2;
					if (p1Obj->platform == obj && (p1Obj->sprite->y+p1Obj->sprite->h-1 <= obj->position.y || p1Obj->sprite->x < obj->position.x)) {
						int res = test_map_colision(p1Obj, -2, 0);
						if (res != 1 && res != 3) {
							p1Obj->sprite->x -= 2;
							checkScrolling(-2, 0, p1Obj);
						}
					}
					if (p2Obj != NULL && p2Obj->platform == obj && (p2Obj->sprite->y+p2Obj->sprite->h-1 <= obj->position.y || p2Obj->sprite->x < obj->position.x)) {
						int res = test_map_colision(p2Obj, -2, 0);
						if (res != 1 && res != 3) {
							p2Obj->sprite->x -= 2;
							checkScrolling(-2, 0, p2Obj);
						}
					}
				}
				obj->distance++;
				obj->timer = getTimer()+30;
			} else {
				obj->distance = 0;
				if (obj->direction == ANIM_RIGHT) {
					obj->direction = ANIM_LEFT;
				} else {
					obj->direction = ANIM_RIGHT;
				}
			}
		}
	} else if (obj->type == OBJ_MOVING_PLATFORM_UPDOWN) {
		if (obj->timer < getTimer()) {
			// for now, move 4 tiles
			if (obj->distance < 4*TILE_SIZE)  {
				if (obj->direction == ANIM_RIGHT) {
					obj->position.y += 2;
					if (p1Obj->platform == obj) {
						p1Obj->sprite->y += 2;
						checkScrolling(0, 2, p1Obj);
					}
					if (p2Obj != NULL && p2Obj->platform == obj) {
						p2Obj->sprite->y += 2;
						checkScrolling(0, 2, p2Obj);
					}
				} else {
					obj->position.y -= 2;
					if (p1Obj->platform == obj) {
						p1Obj->sprite->y -= 2;
						checkScrolling(0, -2, p1Obj);
					}
					if (p2Obj != NULL && p2Obj->platform == obj) {
						p2Obj->sprite->y -= 2;
						checkScrolling(0, -2, p2Obj);
					}
				}
				obj->distance++;
				obj->timer = getTimer()+30;
			} else {
				obj->distance = 0;
				if (obj->direction == ANIM_RIGHT) {
					obj->direction = ANIM_LEFT;
				} else {
					obj->direction = ANIM_RIGHT;
				}
			}
		}
	} else if (obj->type == OBJ_DISAPPEARING_BLOCK) {
		if (obj->timer < getTimer()) {
			//printf(">> OBJ_DISAPPEARING_BLOCK::EXEC <<\n");
			if (obj->direction == ANIM_RIGHT) {
				obj->direction = ANIM_LEFT;
				obj->timer = getTimer() + 3000;
			} else {
				obj->direction = ANIM_RIGHT;
				obj->timer = getTimer() + 4000;
				// difference of time between blocks
				if (obj->speed > 0) {
					obj->timer += obj->speed;
					obj->speed = 0;
				}
			}
		}
	} else if (obj->type == OBJ_FALL_PLATFORM) {
		if (obj->direction == ANIM_DOWN && obj->position.y < RES_H+TILE_SIZE) {
			if (obj->timer < getTimer()) {
				obj->position.y += 4;
				if (p1Obj->platform == obj) {
					int res = test_map_colision(p1Obj, 0, 4);
					if (res != 2 && res != 3) {
						p1Obj->sprite->y += 4;
						checkScrolling(0, 4, p1Obj);
					}
				}
				if (p2Obj != NULL && p2Obj->platform == obj) {
					int res = test_map_colision(p2Obj, 0, 4);
					if (res != 2 && res != 3) {
						p2Obj->sprite->y += 4;
						checkScrolling(0, 4, p2Obj);
					}
				}
				obj->timer = getTimer() + 30;
			}
		}

	} else if (obj->type == OBJ_ITEM_FLY) {
		if (obj->distance > 0 && obj->timer < getTimer()) {
			struct player_struct *playerObj = NULL;
			int xinc;
			if (obj->direction == ANIM_LEFT) {
				xinc = -6;
			} else {
				xinc = 6;
			}
			if (p1Obj->platform == obj) {
				int res = test_map_colision(p1Obj, xinc, 0);
				if (res != 1 && res != 3) {
					p1Obj->sprite->x += xinc;
					checkScrolling(xinc, 0, p1Obj);
					playerObj = p1Obj;
				}
			}
			if (p2Obj != NULL && p2Obj->platform == obj) {
				int res = test_map_colision(p2Obj, xinc, 0);
				if (res != 1 && res != 3) {
					p2Obj->sprite->x += xinc;
					checkScrolling(xinc, 0, p2Obj);
					playerObj = p2Obj;
				}
			}
			obj->position.x += xinc;
			obj->distance += abs(xinc);
			obj->limit++;
			// consume power
			int dist_mod = obj->limit % 8;
			printf(">> obj->distance: %d, mod(101): %d <<\n", obj->distance, (dist_mod));
			if (dist_mod == 0) { // 1 extra that is added to "start"
				if (playerObj != NULL && playerObj->platform == obj) {
					printf("Consume item, current: %d <<\n", playerObj->items.weapons[playerObj->selected_weapon]);
					if (playerObj->items.weapons[playerObj->selected_weapon] > 0) {
						playerObj->items.weapons[playerObj->selected_weapon]--;
					} else {
						// remove item
						obj->type = -1;
					}
				}
			}
			obj->timer = getTimer() + 30;
		// item was invoked, but not used, remove it
		} else if (obj->distance == 0 && obj->timer < getTimer()) {
			obj->type = -1;
		}


	} else if (obj->type == OBJ_ITEM_JUMP) {
		if (obj->distance > 0 && obj->timer < getTimer()) {
			struct player_struct *playerObj = NULL;
			int yinc;
			if (obj->direction == ANIM_UP) {
				yinc = -6;
			} else {
				yinc = 6;
			}
			if (p1Obj->platform == obj) {
				int res = test_map_colision(p1Obj, 0, yinc);
				if (res != 1 && res != 3) {
					p1Obj->sprite->y += yinc;
					checkScrolling(0, yinc, p1Obj);
					playerObj = p1Obj;
				}
			}
			if (p2Obj != NULL && p2Obj->platform == obj) {
				int res = test_map_colision(p2Obj, 0, yinc);
				if (res != 1 && res != 3) {
					p2Obj->sprite->y += yinc;
					checkScrolling(0, yinc, p2Obj);
					playerObj = p2Obj;
				}
			}
			obj->position.y += yinc;
			obj->distance += abs(yinc);
			obj->limit++;
			if (obj->direction == ANIM_UP && obj->limit > 25) {
				obj->direction = ANIM_DOWN;
			}
			obj->timer = getTimer() + 30;
		// item was invoked, but not used, remove it
		} else if (obj->distance == 0 && obj->timer < getTimer()) {
			obj->type = -1;
		}


	// gravity on other types
	} else {
		int mapX = (obj->position.x+TILE_SIZE*0.5)/TILE_SIZE;
		int mapY = (obj->position.y+obj->sprite->h)/TILE_SIZE;
		//printf("Non moving item, execute gravity. x: %d, y: %d, h: %d, mapX: %d, mapY: %d, lock: %d\n", obj->position.x, obj->position.y, obj->sprite->h, mapX, mapY, map.tiles[mapX][mapY].locked);
		if (map.tiles[mapX][mapY].locked == 0 || map.tiles[mapX][mapY].locked == TERRAIN_WATER) {
			obj->position.y += 2;
		}
	}
}


struct struct_object *dropItem(int obj_type, int x, int y) {
	char buffer[256];
	struct struct_object *temp_obj_list = NULL;

	struct struct_object *new_obj = (struct struct_object *)malloc(sizeof(struct struct_object));
	new_obj->direction = ANIM_LEFT;
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
	new_obj->next = NULL;

	if (obj_list[currentMap] == NULL) {
		//printf(">> dropItem - add to the obj_list START <<\n");
		obj_list[currentMap] = new_obj;
	} else {
		temp_obj_list = obj_list[currentMap];
		if (temp_obj_list) {
			while (temp_obj_list->next) {
				temp_obj_list = temp_obj_list->next;
			}
			//printf(">> dropItem - add to the obj_list END <<\n");
			temp_obj_list->next = new_obj;
		} else {
			obj_list[currentMap] = new_obj;
		}
	}

	printf(">> dropItem - obj_type: %d <<\n", obj_type);

	if (obj_type == OBJ_ENERGY_PILL_BIG) {
		sprintf(new_obj->filename, "energy_pill_big.png");
	} else if (obj_type == OBJ_WEAPON_PILL_BIG) {
		sprintf(new_obj->filename, "weapons_pill_big.png");
	} else if (obj_type == OBJ_ENERGY_PILL_SMALL) {
		sprintf(new_obj->filename, "energy_pill_small.png");
	} else if (obj_type == OBJ_WEAPON_PILL_SMALL) {
		sprintf(new_obj->filename, "weapons_pill_small.png");
	} else if (obj_type == OBJ_ITEM_JUMP) {
		sprintf(new_obj->filename, "item_jump.png");
		printf(">> using graphic for jump: %s <<\n", new_obj->filename);
	} else if (obj_type == OBJ_ITEM_FLY) {
		sprintf(new_obj->filename, "item_jet.png");
	} else {
		printf(">> dropItem - unknown item, ignoring <<");
		return NULL;
	}
	sprintf(buffer, "%sdata/images/sprites/objects/%s", FILEPATH, new_obj->filename);
	new_obj->sprite = NULL;
	new_obj->sprite = surface_region_from_image(buffer, TILE_SIZE, TILE_SIZE);
	new_obj->position.x = x;
	new_obj->position.y = y;
	new_obj->type = obj_type;
	if (obj_type == OBJ_ITEM_FLY) {
		new_obj->framesize_w = 37;
		new_obj->framesize_h = 12;
		new_obj->timer = getTimer()+4000;
	} else if (obj_type == OBJ_ITEM_JUMP) {
		new_obj->framesize_w = 22;
		new_obj->framesize_h = 18;
		new_obj->timer = getTimer()+4000;
	} else {
		new_obj->framesize_w = TILE_SIZE;
		new_obj->framesize_h = TILE_SIZE;
	}


	printf(">> dropItem - item created, graphic: '%s', x: %d, y: %d, pointer-address: %p <<\n", buffer, x, y, new_obj);
	return new_obj;
}


#endif // OBJECT_H

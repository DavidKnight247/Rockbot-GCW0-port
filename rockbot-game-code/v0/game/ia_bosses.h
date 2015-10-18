void shot_rainbow(struct npc_struct *temp_npc);
int npc_gravity(struct npc_struct *temp_npc);

// --------------------------------------------------- //
//      damage all players that are in the ground         //
// --------------------------------------------------- //
void ground_damage_players() {
	struct player_struct *temp;
	int i;

	for (i=0; i<game_config.two_players; i++) {
		temp = getPlayerN(i);
		if (temp->HP > 0) { // checks if npc is inside screen and alive
			// checks if npc is in the ground or above a platform
			int map_x = (temp->sprite->x)/TILE_SIZE;
			int map_y = (temp->sprite->y+temp->sprite->w+4)/TILE_SIZE;
			//printf(">> Found a NPC inside screen, map_x: %d, map_y: %d, lock: %d\n", map_x, map_y,map.tiles[map_x][map_y].locked);

			if (map.tiles[map_x][map_y].locked > 0) {
				printf(">> PLAYER is in the ground, damage it\n");
				damagePlayer(temp, 1);
			} else {
				printf(">> PLAYER is jumping, no damage taken\n");
			}
		}
	}
}

void exec_IA_Daisiebot(struct npc_struct *temp_npc) {
	//printf(">> execIADaisiebot.START, temp_npc->projectile.type: %d\n", temp_npc->projectile.type);

	unsigned short int shot_type = IA_STATE_SHOT_RAINBOW;

	if (temp_npc->move_info.status == IA_STATE_WAIT_TIME) {
		temp_npc->move_info.status = IA_STAND;
	}

	// stand -> shot rainbow -> cross screen -> use flowers
	if (temp_npc->move_info.status == IA_STAND) {
		temp_npc->move_info.status = IA_STATE_CROSS_SCREEN;
		temp_npc->sprite_list->anim_type = ANIM_WALK;
		return;
	}

	if (temp_npc->move_info.status == IA_STATE_SHOT_RAINBOW) {
		temp_npc->sprite_list->anim_type = ANIM_STAND;
		shot_rainbow(temp_npc);
		temp_npc->move_info.status = IA_STATE_CROSS_SCREEN;
		temp_npc->move_info.initial_position.x = temp_npc->x;
		temp_npc->move_info.initial_position.y = temp_npc->y;
		temp_npc->move_info.status = IA_STATE_WAIT_TIME;
		temp_npc->move_info.time = getTimer()+300;
	}

	if (temp_npc->move_info.status == IA_STATE_SHOT_FLOWER) {
		temp_npc->sprite_list->anim_type = ANIM_STAND;
		shot_flower(temp_npc);
		temp_npc->projectile.timer = getTimer()+60;
		temp_npc->move_info.initial_position.x = temp_npc->x;
		temp_npc->move_info.initial_position.y = temp_npc->y;
		temp_npc->move_info.status = IA_STATE_WAIT_TIME;
		temp_npc->move_info.time = getTimer()+3000;
	}

	// TODO: move to generic npc, once more will use it
	if (temp_npc->move_info.status == IA_STATE_CROSS_SCREEN) {
		// check if must stop/invert
		//printf(">> execIADaisiebot.MOVE\n");
		temp_npc->move_info.time = getTimer() + 10;
		if (temp_npc->direction == ANIM_LEFT) {
			int map_tile_x = (temp_npc->x-NPC_STEP)/16;
			int map_tile_y = temp_npc->y/16;
			//printf(">> execIADaisiebot.ANIM_LEFT - map.tiles[%d][%d].locked: %d\n", map_tile_x, map_tile_y, map.tiles[map_tile_x][map_tile_y].locked);
			if (temp_npc->move_info.distance < 18*TILE_SIZE && (temp_npc->x-NPC_STEP > 0 && (map.tiles[map_tile_x][map_tile_y].locked == 0 || map.tiles[map_tile_x][map_tile_y].locked == TERRAIN_WATER))) {
				temp_npc->x-= NPC_STEP;
				temp_npc->move_info.distance += NPC_STEP;
			} else {
				//printf(">> IA Invert LEFT -> RIGHT\n");
				temp_npc->move_info.distance = 0;
				IAInvertDirection(temp_npc);
				if (shot_type == IA_STATE_SHOT_RAINBOW) {
					shot_type = IA_STATE_SHOT_FLOWER;
				} else {
					shot_type = IA_STATE_SHOT_RAINBOW;
				}
				temp_npc->move_info.status = shot_type;
			}
		} else { // ANIM_RIGHT
			int map_tile_x = (temp_npc->x+temp_npc->sprite_list->w+NPC_STEP)/16;
			int map_tile_y = temp_npc->y/16;
			//printf(">> execIADaisiebot.ANIM_RIGHT - map.tiles[%d][%d].locked: %d, temp_npc->x: %d, NPC_STEP: %d, scrollX: %d\n", map_tile_x, map_tile_y, map.tiles[map_tile_x][map_tile_y].locked, temp_npc->x, NPC_STEP, scrollX);
			if (temp_npc->move_info.distance < 18*TILE_SIZE && (map.tiles[map_tile_x][map_tile_y].locked == 0 || map.tiles[map_tile_x][map_tile_y].locked == TERRAIN_WATER)) {
				temp_npc->x+= NPC_STEP;
				temp_npc->move_info.distance += NPC_STEP;
			} else {
				temp_npc->move_info.distance = 0;
				IAInvertDirection(temp_npc);
				temp_npc->move_info.status = shot_type;
			}
		}
	}
}

/*
 0 -> IA_STATE_INTRO (until last frame) -> RAND(2)

 1 -> IA_STATE_JUMP_TO_PLAYER (until reaches ground)


 2 -> IA_STATE_JUMP_TO_ROOF
 IA_STATE_JUMP_TO_ROOF (until reaches roof) -> IA_STATE_AIR_WALK
 IA_STATE_AIR_WALK (until reaches player x+w/2) -> IA_STATE_JUMP_FALL
 IA_STATE_JUMP_FALL (until reaches ground) -> IA_STATE_WAIT_TIME
 IA_STATE_WAIT_TIME (until reaches time) -> IA_STATE_INTRO

 3 -> IA_STATE_GROUND_ATTACK
 IA_STATE_GROUND_ATTACK (until finishes attack) -> IA_STATE_WAIT_TIME
 */
void exec_IA_Apebot(struct npc_struct *temp_npc) {
	int must_fall=0;

	// 0 -> IA_STATE_INTRO
	if (temp_npc->move_info.status == 0) {
		temp_npc->move_info.status = IA_STATE_INTRO;
		temp_npc->sprite_list->anim_type = ANIM_INTRO;
		temp_npc->sprite_list->anim_pos = 0;
		temp_npc->move_info.time = getTimer() + 10;
		return;
	}

	if (temp_npc->move_info.status == IA_STATE_GROUND_ATTACK) {
		if (temp_npc->timer < getTimer()) {
			temp_npc->move_info.status = IA_STATE_WAIT_TIME;
			temp_npc->sprite_list->anim_type = ANIM_STAND;
			jump_locked = 0;
			temp_npc->move_info.time = getTimer() + 1200;
		} else {
			if (temp_npc->move_info.sub_status % 2) {
				//printf(">> APEBOT weapon LEFT <<\n");
				move_screen(game_screen, -10, 0);
			} else {
				if (temp_npc->move_info.sub_status % 4) {
					jump_locked = 1;
					printf(">> APEBOT QUAKE DAMAGE <<\n");
					ground_damage_players();
				}
				//printf(">> APEBOT weapon RIGHT <<\n");
				move_screen(game_screen, 10, 0);
			}
			temp_npc->move_info.sub_status++;
			temp_npc->move_info.time = getTimer() + 80;
		}
	}


	// IA_STATE_INTRO (last frame) -> IA_STATE_JUMP_TO_ROOF or IA_STATE_JUMP_TO_PLAYER
	if (temp_npc->move_info.status == IA_STATE_INTRO) {
		if (temp_npc->sprite_list->sprite_frames[temp_npc->sprite_list->direction][temp_npc->sprite_list->anim_type][temp_npc->sprite_list->anim_pos+1].frame == NULL) {
			int rand_n = rand() % 100;
			int dist_npc_player = abs(temp_npc->x - p1Obj->sprite->x);

			if (rand_n < 15) {
				temp_npc->move_info.status = IA_STATE_GROUND_ATTACK;
				temp_npc->sprite_list->anim_type = ANIM_SPECIAL_ATTACK;
				temp_npc->timer = getTimer() + 2500;
				temp_npc->move_info.time = getTimer() + 10;
				return;
			} else if (rand_n < 50 && dist_npc_player > TILE_SIZE) {

				temp_npc->move_info.status = IA_STATE_JUMP_TO_PLAYER;
				//printf(">> IA_STATE_JUMP_TO_PLAYER - START <<\n");
				if (temp_npc->x > p1Obj->sprite->x) { // face player direction
					temp_npc->direction = ANIM_LEFT;
					temp_npc->move_info.initial_position.x = temp_npc->x - dist_npc_player/2;
				} else {
					temp_npc->direction = ANIM_RIGHT;
					temp_npc->move_info.initial_position.x = temp_npc->x + dist_npc_player/2;
				}
				temp_npc->move_info.initial_position.y = temp_npc->y;
				temp_npc->move_info.secondary_position.x = temp_npc->x;
				temp_npc->move_info.secondary_position.y = temp_npc->y;
				temp_npc->move_info.sub_status = pow(dist_npc_player, 2);
				//printf(">> npc.x: %d, p1.x: %d, dist_npc_player: %d, sub_status: %d <<\n", temp_npc->x, p1Obj->sprite->x, dist_npc_player, temp_npc->move_info.sub_status);
				temp_npc->sprite_list->anim_type = ANIM_JUMP;
			} else {
				temp_npc->move_info.status = IA_STATE_JUMP_TO_ROOF;
				temp_npc->sprite_list->anim_type = ANIM_JUMP;
			}
		}
		temp_npc->move_info.time = getTimer() + 10;
		return;
	}

	// IA_STATE_JUMP_TO_PLAYER (until reaches ground)
	if (temp_npc->move_info.status == IA_STATE_JUMP_TO_PLAYER) {
		//printf(">> IA_STATE_JUMP_TO_PLAYER <<\n");
		short int tempY1 = temp_npc->move_info.secondary_position.y;
		int map_tile_y = (temp_npc->y+temp_npc->sprite_list->h)/16;
		int map_tile_x;
		if (temp_npc->direction == ANIM_LEFT) {
			map_tile_x = (temp_npc->x-NPC_STEP)/16;
			if (temp_npc->x-NPC_STEP > 0 && (map.tiles[map_tile_x][map_tile_y].locked == 0 || map.tiles[map_tile_x][map_tile_y].locked == TERRAIN_WATER)) {
				temp_npc->x -= NPC_STEP-1;
			} else {
				temp_npc->x -= NPC_STEP-1;
				//printf(">> IA_STATE_JUMP_TO_PLAYER - must_fall 1 <<\n");
				must_fall = 1;
			}
		} else {
			map_tile_x = (temp_npc->x+NPC_STEP)/16;
			if (map.tiles[map_tile_x][map_tile_y].locked == 0 || map.tiles[map_tile_x][map_tile_y].locked == TERRAIN_WATER) {
				temp_npc->x += NPC_STEP-1;
			} else {
				//printf(">> IA_STATE_JUMP_TO_PLAYER - must_fall 2 <<\n");
				temp_npc->x += NPC_STEP-1;
				must_fall = 1;
			}
		}
		int part1, part2, part3;
		part1 = temp_npc->move_info.sub_status;
		part2 = pow((temp_npc->x - temp_npc->move_info.initial_position.x), 2);
		part3 = temp_npc->move_info.initial_position.y;

		//printf(">> TRAJECTORY_ARC - part1: %d, temp_npc.x: %d, temp_list.x0: %d, part2: %d, part3: %d\n", part1, temp_npc->x, temp_npc->move_info.initial_position.x, part2, part3);

		temp_npc->move_info.secondary_position.y = sqrt(part1 - part2) + part3;

		short int diffY = tempY1 -temp_npc->move_info.secondary_position.y;

		//printf(">> TRAJECTORY_ARC - y1: %d, diffY: %d <<\n", temp_npc->move_info.secondary_position.y, diffY);

		if (diffY > 0 || diffY > -20) {
			temp_npc->y += diffY;
		}
		//printf(">> diffY: %d, map_tile_x: %d, map_tile_y: %d, locked: %d <<\n", diffY, map_tile_x, map_tile_y, map.tiles[map_tile_x][map_tile_y].locked);
		if ((diffY > 0 && (map.tiles[map_tile_x][map_tile_y].locked != 0 && map.tiles[map_tile_x][map_tile_y].locked != TERRAIN_WATER)) || temp_npc->y >= RES_H) {
			printf(">> IA_STATE_JUMP_TO_PLAYER - FINISHED 2 <<\n");
			temp_npc->move_info.status = IA_STATE_WAIT_TIME;
			temp_npc->sprite_list->anim_type = ANIM_STAND;
			temp_npc->move_info.time = getTimer() + 1200;
		} else if (diffY > 0 && must_fall) {
			printf(">> IA_STATE_JUMP_TO_PLAYER - FINISHED 3 <<\n");
			temp_npc->move_info.status = IA_STATE_JUMP_FALL;
			temp_npc->move_info.time = getTimer() + 10;
		}
		return;
	}

	// IA_STATE_JUMP_TO_ROOF -> IA_STATE_AIR_WALK (until reaches player x+w/2)
	if (temp_npc->move_info.status == IA_STATE_JUMP_TO_ROOF) {
		int map_tile_x = temp_npc->x/16;
		int map_tile_y = (temp_npc->y-NPC_STEP)/16;
		if (temp_npc->y > 0 && (map.tiles[map_tile_x][map_tile_y].locked == 0 || map.tiles[map_tile_x][map_tile_y].locked == TERRAIN_WATER)) {
			temp_npc->y -= NPC_STEP;
			temp_npc->move_info.time = getTimer() + 10;
		} else {
			if (temp_npc->x > p1Obj->sprite->x) { // face player direction
				temp_npc->direction = ANIM_LEFT;
			} else {
				temp_npc->direction = ANIM_RIGHT;
			}
			temp_npc->move_info.status = IA_STATE_AIR_WALK;
			temp_npc->sprite_list->anim_type = ANIM_WALK_AIR;
			temp_npc->move_info.time = getTimer() + 800;
		}
		return;
	}

	// IA_STATE_AIR_WALK (until reaches player x+w/2) -> IA_STATE_JUMP_FALL
	// TODO: use p2Obj also (use npc->target)
	if (temp_npc->move_info.status == IA_STATE_AIR_WALK) {
		// change the animation type if time enought passed (hack to allow 2 kindsa of animations being used as one)
		if (temp_npc->timer < getTimer()) {
			if (temp_npc->sprite_list->anim_type == ANIM_WALK_AIR) {
				temp_npc->sprite_list->anim_type = ANIM_JUMP;
			} else {
				temp_npc->sprite_list->anim_type = ANIM_WALK_AIR;
			}
			temp_npc->timer = getTimer()+200;
		}
		if ((temp_npc->direction == ANIM_LEFT && temp_npc->x < p1Obj->sprite->x) || (temp_npc->direction == ANIM_RIGHT && temp_npc->x > p1Obj->sprite->x)) {
			temp_npc->move_info.status = IA_STATE_JUMP_FALL;
		} else {
			if (temp_npc->direction == ANIM_LEFT) {
				temp_npc->x -= NPC_STEP;
			} else {
				temp_npc->x += NPC_STEP;
			}
		}
		temp_npc->move_info.time = getTimer() + 10;
		return;
	}

	// IA_STATE_JUMP_FALL (until reaches ground) -> IA_STATE_WAIT_TIME
	if (temp_npc->move_info.status == IA_STATE_JUMP_FALL) {
		if (npc_gravity(temp_npc) == 2) {
			temp_npc->sprite_list->anim_type = ANIM_STAND;
			temp_npc->move_info.status = IA_STATE_WAIT_TIME;
			temp_npc->move_info.time = getTimer() + 1200;
		} else {
			temp_npc->move_info.time = getTimer() + 10;
		}
		return;
	}

	// IA_STATE_WAIT_TIME (until reaches time) -> IA_STATE_INTRO
	if (temp_npc->move_info.status == IA_STATE_WAIT_TIME) {
		temp_npc->move_info.status = IA_STATE_INTRO;
		temp_npc->sprite_list->anim_pos = 0;
		temp_npc->move_info.time = getTimer() + 10;
		return;
	}
}

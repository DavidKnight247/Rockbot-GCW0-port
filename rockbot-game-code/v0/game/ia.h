#ifndef IA_H
#define IA_H

#include "shared_vars.h"

struct struct_player_dist {
	int dist;
	struct player_struct *pObj;
};

// ****************************************************************************** //
// find the nearest player for a given NPC                                        //
// ****************************************************************************** //
struct struct_player_dist dist_npc_players(struct npc_struct *ia_npc) {
	int dist, temp;
	struct struct_player_dist res;
	res.pObj = p1Obj;
	dist = sqrt(pow((ia_npc->x - p1Obj->sprite->x), 2) + pow((ia_npc->y - p1Obj->sprite->y), 2));
	if (game_config.two_players == 2) {
		temp = sqrt(pow((ia_npc->x - p2Obj->sprite->x), 2) + pow((ia_npc->y - p2Obj->sprite->y), 2));
		if (temp < dist) {
			dist = temp;
			res.pObj = p2Obj;
		}
	}

	res.dist = dist;
	return res;
}

// ****************************************************************************** //
// FLOATER can be GHOST (cross walls) or FISH                                     //
// of returning to start point, it just stops where player scaped or was hit      //
// ****************************************************************************** //
void exec_IA_FLOATER(struct npc_struct *ia_npc, int cross_walls) {
	struct struct_player_dist dist_p;
	int colid = 0;

	//printf(">> WARNING::exec_IA_GHOST - ia_npc->move_info.status: %d <<\n", ia_npc->move_info.status);

	switch (ia_npc->move_info.status) {
		case 0: // initial status, stopped
			dist_p = dist_npc_players(ia_npc);
			printf(">> WARNING::exec_IA_GHOST - stopped - dist: %d, limit: %d <<\n", dist_p.dist, TILE_SIZE*7);
			if (dist_p.dist < TILE_SIZE*7) {
				ia_npc->move_info.status = 1;
				ia_npc->target = dist_p.pObj;
			}
			break;
		case 1: // following player
			if (ia_npc->target == NULL) { // error checking
				ia_npc->move_info.status = 0;
			}
			int dist  = dist_npc_player(ia_npc);
			if (dist > 250 || dist <= TILE_SIZE+4) { // player scaped or npc reached him, stop and wait 2.5 seconds
				ia_npc->move_info.status = 0;
				ia_npc->move_info.time = getTimer()+2500;
				return;
			}

			if (ia_npc->target->sprite->x > ia_npc->x) {
				if (ia_npc->direction != ANIM_RIGHT) {
					ia_npc->direction = ANIM_RIGHT;
				}
				printf(">> exec_IA_FLOATER.move_RIGHT.colid: %d <<\n", colid);
				colid = move_npc_check_colision(ia_npc, NPC_STEP, 0);
				if (cross_walls  || colid == 0) {
					ia_npc->x += NPC_STEP;
				}
			} else if (ia_npc->target->sprite->x < ia_npc->x) {
				if (ia_npc->direction != ANIM_LEFT) {
					ia_npc->direction = ANIM_LEFT;
				}
				printf(">> exec_IA_FLOATER.move_LEFT.colid: %d <<\n", colid);
				colid = move_npc_check_colision(ia_npc, -NPC_STEP, 0);
				if (cross_walls  || colid == 0) {
					ia_npc->x -= NPC_STEP;
				}
			}
			if (ia_npc->target->sprite->y > ia_npc->y) {
				colid = move_npc_check_colision(ia_npc, 0, NPC_STEP);
				if (cross_walls  || colid == 0) {
					ia_npc->y += NPC_STEP;
				}
			} else {
				colid = move_npc_check_colision(ia_npc, 0, -NPC_STEP);
				if (cross_walls  || colid == 0) {
					ia_npc->y -= NPC_STEP;
				}
			}
			ia_npc->move_info.time = getTimer()+150;
		default:
			break;
	}
}


// ****************************************************************************** //
// Ghost IA is very similar to the IA_BAT, but it will ignore walls and instead   //
// of returning to start point, it just stops where player scaped or was hit      //
// ****************************************************************************** //
void exec_IA_GHOST(struct npc_struct *ia_npc) {
	exec_IA_FLOATER(ia_npc, 1);
}

// ****************************************************************************** //
// Ghost IA is very similar to the BAT, but instead of returning to start point   //
// it just stops where player scaped or was hit                                   //
// ****************************************************************************** //
void exec_IA_FISH(struct npc_struct *ia_npc) {
	exec_IA_FLOATER(ia_npc, 0);
}

// ****************************************************************************** //
// Ghost IA dashes against front direction with acceleration and slowly returns   //
// ****************************************************************************** //
void exec_IA_DOLPHIN(struct npc_struct *ia_npc) {
	int blocked = 0;
	struct struct_player_dist dist_p;

	switch (ia_npc->move_info.status) {
		case 0: // initial status, stopped
			// sub_status is used as speed acceleration control
			if (ia_npc->sprite_list->anim_type != ANIM_TURN) {
				if (ia_npc->direction == ANIM_LEFT) {
					if (move_npc_check_colision(ia_npc, -NPC_STEP, 0)) {
						blocked = 1;
					} else {
						ia_npc->x -= NPC_STEP;
					}
				} else {
					if (move_npc_check_colision(ia_npc, NPC_STEP, 0)) {
						blocked = 1;
					} else {
						ia_npc->x += NPC_STEP;
					}
				}
				// set target, if needed
				if (ia_npc->target == NULL) {
					dist_p = dist_npc_players(ia_npc);
					ia_npc->target = dist_p.pObj;
				}
				if (ia_npc->target->sprite->y > ia_npc->y) {
					if (move_npc_check_colision(ia_npc, 0, NPC_STEP) == 0) {
						ia_npc->y += NPC_STEP;
					}
				} else if (ia_npc->target->sprite->y < ia_npc->y) {
					if (move_npc_check_colision(ia_npc, 0, -NPC_STEP) == 0) {
						ia_npc->y -= NPC_STEP;
					}
				}
				if (blocked == 0) {
					ia_npc->move_info.distance += NPC_STEP;
					if (ia_npc->move_info.sub_status > 15) {
						ia_npc->move_info.sub_status--;
					}
				}
			} else {
				ia_npc->move_info.sub_status = 200;
			}
			ia_npc->move_info.time = getTimer() + ia_npc->move_info.sub_status;
			if (ia_npc->move_info.distance > ia_npc->walk_range || blocked == 1) { // maximum is one-screen width
				ia_npc->sprite_list->anim_type = ANIM_TURN;
				ia_npc->sprite_list->anim_pos = 0;
				ia_npc->move_info.time = getTimer()+200;
				ia_npc->move_info.status = 1;
				ia_npc->target = NULL;
			}
			break;
		case 1: // return to origin backwards
			//printf(">> exec_IA_DOLPHIN::backwards - dist: %d \n", ia_npc->move_info.distance);
			if (ia_npc->move_info.distance > 0) {
				ia_npc->move_info.distance -= NPC_STEP;
			} else {
				ia_npc->move_info.status = 0;
				ia_npc->move_info.time = getTimer()+2000;
			}
			if (ia_npc->direction != ANIM_LEFT) {
				if (move_npc_check_colision(ia_npc, -NPC_STEP, 0)) {
					return;
				}
				ia_npc->x -= NPC_STEP;
			} else {
				if (move_npc_check_colision(ia_npc, NPC_STEP, 0)) {
					return;
				}
				ia_npc->x += NPC_STEP;
			}
			ia_npc->move_info.time = getTimer()+90;
			break;

		default:
			break;
	}
}

// ****************************************************************************** //
// This IA makes the NPC fly from one wall to another, where it will sit and wait //
// ****************************************************************************** //
void exec_IA_SIDETOSIDE(struct npc_struct *ia_npc) {

}


// ****************************************************************************** //
// This IA makes the NPC fly from one side to another and returns                 //
// ****************************************************************************** //
void exec_IA_GO_ZIG_ZAG(struct npc_struct *ia_npc) {
	if (ia_npc->sprite_list->anim_type != ANIM_TURN) {
		if (ia_npc->direction == ANIM_LEFT) {
			if (move_npc_check_colision(ia_npc, -NPC_STEP, 0)) {
				return;
			}
			ia_npc->x -= NPC_STEP;
		} else {
			if (move_npc_check_colision(ia_npc, NPC_STEP, 0)) {
				return;
			}
			ia_npc->x += NPC_STEP;
		}
		ia_npc->move_info.distance += NPC_STEP;
	}
	ia_npc->move_info.time = getTimer()+10;
	if (ia_npc->move_info.distance > ia_npc->walk_range) {
		ia_npc->sprite_list->anim_type = ANIM_TURN;
		ia_npc->sprite_list->anim_pos = 0;
		ia_npc->move_info.distance = 0;
		ia_npc->move_info.time = getTimer()+200;
	}

}



// ****************************************************************************** //
// This IA makes the NPC fly from one side to another and returns                 //
// ****************************************************************************** //
void exec_IA_FLY_ZIG_ZAG(struct npc_struct *ia_npc) {
	exec_IA_GO_ZIG_ZAG(ia_npc);
}

// ****************************************************************************** //
// This IA makes the NPC walk from one side to another and returns                 //
// ****************************************************************************** //
void exec_IA_ZIGZAG(struct npc_struct *ia_npc) {
	exec_IA_GO_ZIG_ZAG(ia_npc);
}


#endif // IA_H

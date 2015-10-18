#include <sstream>
#include "classnpc.h"
#include "graphicslib.h"
#include "stage.h"
#include "classplayer.h"
#include "classmap.h"
#include <math.h>

extern std::string FILEPATH;
std::vector<character*> *classnpc::player_list=NULL;

extern format_v_2_0_1::file_game game_data;

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
classnpc::classnpc() : graphic_filename(), walk_range(TILESIZE*6), first_run(true), IA_state(0), target(NULL), _is_player_friend(false)
{
	add_graphic();
	clean_projectiles();
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
classnpc::classnpc(std::string set_name) : graphic_filename(), walk_range(TILESIZE*6), first_run(true), IA_state(0), target(NULL), _is_player_friend(false)
{
	name = set_name;
	add_graphic();
}

void classnpc::build_basic_npc(int stage_id, int map_id, int main_id)
{
	_number = main_id;
	// TODO - usar operador igual e também para cópia de toda a classe para ela mesma
	int i;
	std::stringstream full_graphic_filename;
	graphicsLib_gSurface npc_sprite_surface;


	//std::cout << "NPC 1 - stage_id: " << stage_id << ", map_id: " << map_id << ", main_id: " << main_id << ", id: " << id << std::endl;

	name = std::string(game_data.game_npcs[main_id].name);

	add_graphic();


	// set sprites to be persistent. they will be freed only by game.cpp
	for (int i=0; i<ANIM_DIRECTION_COUNT; i++) {
		for (int j=0; j<ANIM_TYPE_COUNT; j++) {
			for (int k=0; k<ANIM_FRAMES_COUNT; k++) {
				(character_graphics_list.find(name)->second)[i][j][k].frameSurface.persistent = true;
			}
		}
	}


	hitPoints.total = game_data.game_npcs[main_id].hp.total;
	hitPoints.current = hitPoints.total;

	if (state.direction < 0 || state.direction > ANIM_DIRECTION_COUNT) {
		state.direction = ANIM_DIRECTION_RIGHT;
	}

	move_speed = game_data.game_npcs[main_id].speed;
	walk_range = game_data.game_npcs[main_id].walk_range;
	if (walk_range < 0 || walk_range > 1000) { // fix data errors by setting value to default
		walk_range = TILESIZE*6;
	}


	graphic_filename = game_data.game_npcs[main_id].graphic_filename;


	for (i=0; i<NPC_FRAME_NUMBER; i++) {
		frames[i].frameset = game_data.game_npcs[main_id].frames[i].state;
		frames[i].frameset_time = game_data.game_npcs[main_id].frames[i].duration;
		// TODO - change file format in roder to support different sizes for each frame
		frames[i].framesize.width = game_data.game_npcs[main_id].frame_size.width;
		frames[i].framesize.heigth = game_data.game_npcs[main_id].frame_size.width;
		if (game_data.game_npcs[main_id].frames[i].state >= 0) {
			//std::cout << "npc[" << game_data.game_npcs[main_id].name << "].frames[" << i << "] - frameset: " << game_data.game_npcs[main_id].frames[i].state << ", duration: " << game_data.game_npcs[main_id].frames[i].duration << std::endl;
			//std::cout << "npc[" << game_data.game_npcs[main_id].name << "].frames[" << i << "] - frameset.w: " << game_data.game_npcs[main_id].frame_size.width << ", frameset.h: " << game_data.game_npcs[main_id].frame_size.heigth << std::endl;
		}
	}


	frameSize.width = game_data.game_npcs[main_id].frame_size.width;
	frameSize.heigth = game_data.game_npcs[main_id].frame_size.heigth;
	can_shoot = (game_data.game_npcs[main_id].can_shoot != 0);
	shield_type = game_data.game_npcs[main_id].shield_type;
	_is_boss = false;

	IA_type = game_data.game_npcs[main_id].IA_type;


	// TODO - this logic can be passed to the editor
	if (IA_type == IA_BAT || IA_type == IA_FLY_ZIG_ZAG || IA_type == IA_BUTTERFLY || IA_type == IA_GHOST || IA_type == IA_FISH || IA_type == IA_DOLPHIN || IA_type == IA_ROOF_SHOOTER || IA_type == IA_FIXED_JUMPER || IA_type == IA_SIDE_SHOOTER) {
		//std::cout << "******** classnpc::set_file_data - npc: " << name << ", canfly: " << can_fly << std::endl;
		can_fly = true;
	}
	projectile_type_id = game_data.game_npcs[main_id].projectile_id[0];

	realPosition.x = 0;
	realPosition.y = 0;

	max_jump = 20;					// hardcoded, fix this in editor
	max_projectiles = 1;			// hardcoded, fix this in editor
	move_speed = 6;					// hardcoded, fix this in editor
	attack_state = 0;
	last_execute_time = 0;

	// load the graphic from file and set it into frameset
	full_graphic_filename.str(std::string());
	full_graphic_filename << FILEPATH << "data/images/sprites/enemies/" << std::string(graphic_filename);
	graphLib.surfaceFromFile(full_graphic_filename.str(), &npc_sprite_surface);
	if (npc_sprite_surface.gSurface == NULL) {
		std::cout << "initFrames - Error loading player surface from file\n";
		return;
	}

	for (i=0; i<NPC_FRAME_NUMBER; i++) {
		if (frames[i].frameset != -1) {
			//addSpriteFrame(int anim_direction, int anim_type, int posX, int posY, graphicsLib_gSurface &spritesSurface, int delay)
			addSpriteFrame(ANIM_DIRECTION_LEFT, frames[i].frameset, i, 0, npc_sprite_surface, frames[i].frameset_time);
			addSpriteFrame(ANIM_DIRECTION_RIGHT, frames[i].frameset, i, 1, npc_sprite_surface, frames[i].frameset_time);
		}
	}
	//std::cout << "end" << std::endl;
}

classnpc::classnpc(int stage_id, int map_id, int main_id, int id) : _is_player_friend(false) // map-loaded npc
{
	build_basic_npc(stage_id, map_id, main_id);
	facing = game_data.stages[stage_id].maps[map_id].map_npcs[id].direction;
	state.direction = facing;
	start_point.x = game_data.stages[stage_id].maps[map_id].map_npcs[id].start_point.x * TILESIZE;
	start_point.y = game_data.stages[stage_id].maps[map_id].map_npcs[id].start_point.y * TILESIZE;
	position.x = start_point.x;
	position.y = start_point.y;

}

classnpc::classnpc(int stage_id, int map_id, int main_id, st_position npc_pos, short int direction) : _is_player_friend(true) // spawned npc
{
	build_basic_npc(stage_id, map_id, main_id);
	facing = direction;
	state.direction = direction;
	start_point.x = npc_pos.x;
	start_point.y = npc_pos.y;
	position.x = npc_pos.x;
	position.y = npc_pos.y;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
classnpc::~classnpc()
{
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classnpc::initFrames()
{
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classnpc::execute()
{
	move();
	charMove();
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classnpc::set_file_data(format_v1_0::st_file_npc data) {
	// TODO - usar operador igual e também para cópia de toda a classe para ela mesma
	int i;
	std::stringstream full_graphic_filename;
	graphicsLib_gSurface npc_sprite_surface;


	// set sprites to be persistent. they will be freed only by game.cpp
	for (int i=0; i<ANIM_DIRECTION_COUNT; i++) {
		for (int j=0; j<ANIM_TYPE_COUNT; j++) {
			for (int k=0; k<ANIM_FRAMES_COUNT; k++) {
				(character_graphics_list.find(name)->second)[i][j][k].frameSurface.persistent = true;
			}
		}
	}

	name = data.name;
	hitPoints.total = data.hp.total;
	hitPoints.current = hitPoints.total;
	state.direction = data.facing;
	if (state.direction < 0 || state.direction > ANIM_DIRECTION_COUNT) {
		state.direction = ANIM_DIRECTION_RIGHT;
	}
	position.x = 0;
	position.y = 0;

	move_speed = data.speed;
	//std::cout << "classnpc::set_file_data - data.walk_range: " << data.walk_range << std::endl;
	walk_range = data.walk_range;
	if (walk_range < 0 || walk_range > 1000) { // fix data errors by setting value to default
		walk_range = TILESIZE*6;
	}
	facing = data.facing;
	start_point = data.start_point;
	graphic_filename = data.graphic_filename;
	for (i=0; i<NPC_FRAME_NUMBER; i++) {
		frames[i].frameset = data.frameset[i];
		frames[i].frameset_time = data.frameset_time[i];
		// TODO - change file format in roder to support different sizes for each frame
		frames[i].framesize.width = data.framesize_w;
		frames[i].framesize.heigth = data.framesize_h;
		if (data.frameset[i] >= 0) {
			//std::cout << "npc[" << data.name << "].frames[" << i << "] - frameset: " << data.frameset[i] << ", duration: " << data.frameset_time[i] << std::endl;
			//std::cout << "npc[" << data.name << "].frames[" << i << "] - frameset.w: " << frames[i].framesize.width << ", frameset.h: " << frames[i].framesize.heigth << std::endl;
		}
	}
	frameSize.width = data.framesize_w;
	frameSize.heigth = data.framesize_h;
	can_shoot = (data.can_shoot != 0);
	shield_type = data.shield_type;
	_is_boss = (data.is_boss != 0);
	IA_type = data.IA_type;

	// TODO - this logic can be passed to the editor
	if (IA_type == IA_BAT || IA_type == IA_FLY_ZIG_ZAG || IA_type == IA_BUTTERFLY || IA_type == IA_GHOST || IA_type == IA_FISH || IA_type == IA_DOLPHIN || IA_type == IA_ROOF_SHOOTER || IA_type == IA_FIXED_JUMPER) {
		std::cout << "classnpc::set_file_data - npc: " << name << ", canfly: " << can_fly << std::endl;
		can_fly = true;
	}
	projectile_type_id = data.projectile_type_id;

	realPosition.x = 0;
	realPosition.y = 0;

	max_jump = 20;					// hardcoded, fix this in editor
	max_projectiles = 1;			// hardcoded, fix this in editor
	move_speed = 6;					// hardcoded, fix this in editor
	attack_state = 0;
	last_execute_time = 0;

	// load the graphic from file and set it into frameset
	full_graphic_filename.str(std::string());
	full_graphic_filename << FILEPATH << "data/images/sprites/enemies/" << graphic_filename;
	graphLib.surfaceFromFile(full_graphic_filename.str(), &npc_sprite_surface);
	if (npc_sprite_surface.gSurface == NULL) {
		std::cout << "initFrames - Error loading player surface from file\n";
		return;
	}

/*
	spriteArea.x = posX*frameSize.width;
	spriteArea.y = posY*frameSize.heigth;
	spriteArea.w = frameSize.width;
	spriteArea.h = frameSize.heigth;

	for (int i=0; i<ANIM_FRAMES_COUNT; i++) {
		if ((character_graphics_list.find(name)->second)[anim_direction][anim_type][i].frameSurface.gSurface == NULL) {
			(character_graphics_list.find(name)->second)[anim_direction][anim_type][i].setSurface(graphLib.surfaceFromRegion(spriteArea, spritesSurface));
			(character_graphics_list.find(name)->second)[anim_direction][anim_type][i].delay = delay;
			//cout << "adding frame - direction: " << anim_direction << ", type: " << anim_type << ", state: " << i << "\n";
			break;
		}
	}
*/

	//struct anim_frame sprite_frames[ANIM_DIRECTION_COUNT][ANIM_TYPE_COUNT][ANIM_FRAMES_COUNT];

	for (i=0; i<NPC_FRAME_NUMBER; i++) {
		if (frames[i].frameset != -1) {
			//addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_STAIRS_ATTACK, 19, 0, playerSpriteSurface, 200);
			//std::cout << "NPC - adding frame of type " << frames[i].frameset << ", duration: " << frames[i].frameset_time << ", at[0][" << i << "]" << std::endl;
			addSpriteFrame(ANIM_DIRECTION_LEFT, frames[i].frameset, i, 0, npc_sprite_surface, frames[i].frameset_time);
			addSpriteFrame(ANIM_DIRECTION_RIGHT, frames[i].frameset, i, 1, npc_sprite_surface, frames[i].frameset_time);
		}
	}
	//std::cout << "end" << std::endl;

/*
 CHARACTER:
	struct st_characterMovements moveCommands;
*/
}



void classnpc::copy(classnpc *from)
{
	int i;

	facing = from->facing;
	start_point = from->start_point;

	walk_range = from->walk_range;
	graphic_filename = from->graphic_filename;
	for (i=0; i<ANIM_DIRECTION_COUNT; i++) {
		for (int j=0; j<ANIM_TYPE_COUNT; j++) {
			for (int k=0; k<ANIM_FRAMES_COUNT; k++) {
				(character_graphics_list.find(name)->second)[i][j][k] = from->character_graphics_list.find(name)->second[i][j][k];
			}
		}
	}
	can_shoot = from->can_shoot;
	shield_type = from->shield_type;

	_is_boss = from->_is_boss;
	IA_type = from->IA_type;
	projectile_type_id = from->projectile_type_id;

	hitPoints = from->hitPoints;
	name = from->name;
	position = from->position;
	realPosition = from->realPosition;
	last_execute_time = 0;
	frameSize = from->frameSize;
	moveCommands = from->moveCommands;
	max_jump = from->max_jump;
	max_projectiles = from->max_projectiles;
	move_speed = from->move_speed;
	can_fly = from->can_fly;
	attack_state = 0;
	state = from->state;
	jump_steps = 0;

	for (i=0; i<ANIM_DIRECTION_COUNT; i++) {
		for (int j=0; j<ANIM_TYPE_COUNT; j++) {
			for (int k=0; k<ANIM_FRAMES_COUNT; k++) {
				(character_graphics_list.find(name)->second)[i][j][k].frameSurface = from->character_graphics_list.find(name)->second[i][j][k].frameSurface;
			}
		}
	}
}



void classnpc::move_projectiles()
{
	//int i = 0;
	// animate projectiles
	std::vector<projectile*>::iterator it;
	for (it=projectile_list.begin(); it<projectile_list.end(); it++) {
		(*it)->draw();
		st_size moved = (*it)->move();
		// check colision agains players
		std::vector<character*>::iterator enemy_it;
		if ((*it)->is_reflected == true) {
			continue;
		}

		if (_is_player_friend == false) {
			for (enemy_it=map->_player_list.begin(); enemy_it != map->_player_list.end(); enemy_it++) {
				if ((*it)->check_colision(st_rectangle((*enemy_it)->getPosition().x, (*enemy_it)->getPosition().y, (*enemy_it)->get_size().width, (*enemy_it)->get_size().heigth), st_position(moved.width, moved.heigth)) == true) {
					std::cout << "is_shielded::CALL 1" << std::endl;
					if ((*enemy_it)->is_shielded((*it)->get_direction()) == true) {
						(*it)->reflect();
					} else {
						(*enemy_it)->damage((*it)->get_damage());
						(*it)->consume_projectile();
					}
				}
				if ((*it)->is_finished == true) {
					projectile_list.erase(it);
					break;
				}
			}
		} else {
			std::list<classnpc*>::iterator enemy_it;
			for (enemy_it=map->npc_list.begin(); enemy_it != map->npc_list.end(); enemy_it++) {
				//classnpc* enemy = (*enemy_it);
				if ((*it)->check_colision(st_rectangle((*enemy_it)->getPosition().x, (*enemy_it)->getPosition().y, (*enemy_it)->get_size().width, (*enemy_it)->get_size().heigth), st_position(moved.width, moved.heigth)) == true) {
					std::cout << "is_shielded::CALL 2" << std::endl;
					if ((*enemy_it)->is_shielded((*it)->get_direction()) == true) {
						(*it)->reflect();
					} else {
						(*enemy_it)->damage((*it)->get_damage());
						if ((*it)->get_move_type() != TRAJECTORY_CHAIN) { /// @TODO non-destructable types
							(*it)->consume_projectile();
						}
					}
				}
				if ((*it)->is_finished == true) {
					projectile_list.erase(it);
					break;
				}
			}
		}
	}
}


// executes the NPC sub-IA behavior
void classnpc::move() {
	int mapScrollX = map->getMapScrolling().x;
	int lock_point = map->getMapPointLock(st_position(position.x/16, position.y/16));
	struct struct_player_dist dist_players = dist_npc_players();

	// only move npcs that are in the current screen and are alive
	//mapScrollX = -map_pos_x;


	/// @TODO: this check must be placed in game.cpp and npc must have a set_frozen() method, for individual effect
	/*
	// checks if the npcs are frozen
	if ((p1Obj->selected_weapon == DYNAMITEBOT && p1Obj->weapon_state != -1)  || (game_config.two_players == 2 && p2Obj->selected_weapon == DYNAMITEBOT && p2Obj->weapon_state != -1)) {
			if (is_boss == 0 || (is_boss == 1 && strstr(name, "Mage Bot"))) {
			printf(">> INFO: move_npc - npc is frozen <<\n");
			return;
		}
	}
	*/

	move_projectiles();


	if (last_execute_time > timer.getTimer()) {
		return;
	}

	//std::cout << "classnpc::move()::START::name: " << name << ", hitPoints.current: " << hitPoints.current << ", position.x: " << position.x << ", mapScrollX: " << mapScrollX << std::endl;

	if (hitPoints.current > 0 && position.x >= mapScrollX-TILESIZE*2 && position.x <= mapScrollX+RES_W+TILESIZE*2) {


		//std::cout << "classnpc::move()::IA_type: " << IA_type << std::endl;

		if (IA_type == IA_ZIGZAG) {
			exec_IA_ZIGZAG();
		} else if (IA_type == IA_BAT) {
			if (IA_state == IA_STATE_FOLLOW) {
				// handle error on player object
				if (target == NULL) {
					IA_state = IA_STATE_RETURN;
					return;
				}

				if (dist_players.dist > 250 || dist_players.dist <= TILESIZE+4) { // player escaped or npc reached him
					IA_state = IA_STATE_RETURN;
					return;
				}
				if (target->getPosition().x > position.x) {
					position.x += 4;
				} else if (target->getPosition().x < position.x) {
					position.x -= 4;
				}
				if (target->getPosition().y > position.y) {
					position.y += 4;
				} else if (target->getPosition().y < position.y) {
					position.y -= 4;
				}
			} else if (IA_state == IA_STATE_RETURN) {
				if (target != NULL) {
					if (target->getPosition().x > position.x) {
						position.x -= 2;
					} else if (target->getPosition().x < position.x) {
						position.x += 2;
					}
				}
				position.y -= 4;
				if ((position.y < 0) || (lock_point > 0 && lock_point != TERRAIN_WATER)) {
					IA_state = IA_STATE_SLEEP;
					state.animation_type = ANIM_TYPE_STAND;
					return;
				}
			} else if (state.animation_type == ANIM_TYPE_MOVE_SEMI) { // START MOVING
				IA_state = IA_STATE_FOLLOW;
				state.animation_type = ANIM_TYPE_WALK;
			} else { // IA_STATE_SLEEP
				struct struct_player_dist temp = dist_npc_players();
				if (temp.dist < 150) {
					state.animation_type = ANIM_TYPE_MOVE_SEMI;
					IA_state = IA_STATE_START_MOVE;
					target = temp.pObj;
				}
			}
			last_execute_time = timer.getTimer() + 200;
		} else if (IA_type == IA_ROOF_SHOOTER) {
			if (IA_state == 0) { // sleep -> regular
				state.animation_type = ANIM_TYPE_ATTACK;
				// shoot regular
				throw_projectile_npc(TRAJECTORY_LINEAR, 2, 1);
				moveCommands.attack = 1; // TODO - attack must get projectile from a type
				IA_state = 1;
				last_execute_time = timer.getTimer() + 800;
			} else if (IA_state == 1) { // regular -> diagonal
				// shoot diagonal
				if (position.y > RES_H/2) {
					throw_projectile_npc(TRAJECTORY_DIAGONAL_UP, 2, 1);
				} else {
					throw_projectile_npc(TRAJECTORY_DIAGONAL_DOWN, 2, 1);
				}
				moveCommands.attack = 1; // TODO - attack must get projectile from a type
				IA_state = 2;
				last_execute_time = timer.getTimer() + 800;
			} else if (IA_state == 2) { // diagonal -> sleep
				state.animation_type = ANIM_TYPE_STAND;
				last_execute_time = timer.getTimer() + 2000;
				IA_state = 0;
			}
		} else if (IA_type == IA_STAND && can_shoot == 1 && abs(dist_players.pObj->getPosition().x - position.x) <= 96) {
			// TODO: change direction to face player
			if (IA_state == 0) {
				state.animation_type = ANIM_TYPE_ATTACK;
				state.animation_state = 0;
				state.animation_timer = 0;
				last_execute_time = timer.getTimer() + 500;
				IA_state = 1;
				if (state.direction == ANIM_DIRECTION_LEFT && dist_players.pObj->getPosition().x > position.x) {
					state.direction = ANIM_DIRECTION_RIGHT;
				} else if (state.direction == ANIM_DIRECTION_RIGHT && dist_players.pObj->getPosition().x < position.x) {
					state.direction = ANIM_DIRECTION_LEFT;
				}
			} else if (IA_state == 1) {
				throw_projectile_npc(TRAJECTORY_LINEAR, 1, 0);
				moveCommands.attack = 1;
				last_execute_time = timer.getTimer() + 500;
				IA_state = 2;
			} else if (IA_state == 2) {
				state.animation_type = ANIM_TYPE_STAND;
				last_execute_time = timer.getTimer() + 1500;
				IA_state = 0;
			}
		} else if (IA_type == IA_SHOOT_AND_GO) {
			// TODO: change direction to face player
			if (IA_state == 0 && abs(dist_players.pObj->getPosition().x - position.x) <= 64) {
				state.animation_type = ANIM_TYPE_ATTACK;
				state.animation_state = 0;
				state.animation_timer = 0;
				last_execute_time = timer.getTimer() + 500;
				IA_state = 1;
				if (state.direction == ANIM_DIRECTION_LEFT && dist_players.pObj->getPosition().x > position.x) {
					state.direction = ANIM_DIRECTION_RIGHT;
				} else if (state.direction == ANIM_DIRECTION_RIGHT && dist_players.pObj->getPosition().x < position.x) {
					state.direction = ANIM_DIRECTION_LEFT;
				}
			} else if (IA_state == 1) { // SHOOTING
				throw_projectile_npc(TRAJECTORY_LINEAR, 1, 0);
				last_execute_time = timer.getTimer() + 800;
				IA_state = 2;
			} else if (IA_state == 2) { // MOVING
				if (state.direction == ANIM_DIRECTION_LEFT) {
					if (!test_change_position(-move_speed, 0)) {
						state.animation_type = ANIM_TYPE_TURN;
						state.animation_state = 0;
						distance.width = 0;
						state.animation_timer = timer.getTimer() + 200;
						IA_state = 0;
						return;
					}
					state.animation_type = ANIM_TYPE_WALK;
					position.x -= move_speed;
				} else {
					if (!test_change_position(move_speed, 0)) {
						state.animation_type = ANIM_TYPE_TURN;
						state.animation_state = 0;
						distance.width = 0;
						state.animation_timer = timer.getTimer() + 200;
						IA_state = 0;
						return;
					}
					state.animation_type = ANIM_TYPE_WALK;
					position.x += move_speed;
				}
				last_execute_time = timer.getTimer() + 20;
			}
		} else if (IA_type == IA_FLY_ZIG_ZAG) {
			exec_IA_ZIGZAG();
		} else if (IA_type == IA_FIXED_JUMPER) {
			if (state.animation_type != ANIM_TYPE_STAND) {
				state.animation_type = ANIM_TYPE_STAND;
			}
			//std::cout << ">> IA_FIXED_JUMPER - exec.position.y: " << position.y << " <<" << std::endl;
			if (IA_state != 1) { // falling
				if (position.y < RES_H+TILESIZE) {
					position.y += 8;
					last_execute_time = timer.getTimer()+20;
				} else {
					state.animation_type = ANIM_TYPE_STAND;
					state.animation_state = 0;
					state.animation_timer = 0;
					IA_state = 1;
					last_execute_time = timer.getTimer()+1200;
				}
			} else { // jump up
				if (position.y > 20) {
					position.y -= 6;
					last_execute_time = timer.getTimer()+20;
				} else { // reached jump maximum, wait a bit and start falling
					IA_state = 0;
					last_execute_time = timer.getTimer()+200;
				}
			}
		} else if (IA_type == IA_SIDE_SHOOTER) {
			if (IA_state == 0) { // sleep -> shoot
				// shoot diagonal
				struct struct_player_dist temp = dist_npc_players();
				target = temp.pObj;
				// shoot down
				if (target->getPosition().y > position.y + frameSize.heigth) {
					throw_projectile_npc(TRAJECTORY_DIAGONAL_DOWN, 1, 1);
					state.animation_type = ANIM_TYPE_ATTACK;
				// shoot up
				} else if (target->getPosition().y + target->get_size().heigth < position.y) {
					throw_projectile_npc(TRAJECTORY_DIAGONAL_UP, 1, 1);
					state.animation_type = ANIM_TYPE_STAND;
				// shoot regular
				} else {
					throw_projectile_npc(TRAJECTORY_LINEAR, 1, 1);
					state.animation_type = ANIM_TYPE_WALK;
				}
				IA_state = 1;
				last_execute_time = timer.getTimer()+800;
			} else if (IA_state == 1) { // shoot -> sleep
				last_execute_time = timer.getTimer() + 2000;
				IA_state = 0;
			}
		} else if (IA_type == IA_GHOST) {
			exec_IA_GHOST();
		} else if (IA_type == IA_FISH) {
			exec_IA_FISH();
		} else if (IA_type == IA_DOLPHIN) {
			exec_IA_DOLPHIN();
		} else if (IA_type == IA_SIDETOSIDE) {
			exec_IA_SIDETOSIDE();
		} else {
			//std::cout << ">>>>>>>>>>>>>> UNDEFINED IA IA_type: " << IA_type << ", Npc.name: " << name << " <<<<<<<<<<<<<<" << std::endl;
		}
	}
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classnpc::exec_IA_ZIGZAG()
{
	bool must_turn = false;

	//std::cout << "classnpc::exec_IA_ZIGZAG() - position.x: " << position.x << ", position.y: " << position.y << std::endl;

	if (state.animation_type != ANIM_TYPE_TURN) {
		int xinc = 0;

		if (state.direction == ANIM_DIRECTION_LEFT) {
			xinc = -move_speed;
		} else {
			xinc = move_speed;
		}

		if (can_fly == false && test_change_position(xinc, TILESIZE*0.8) == true) {
			//std::cout << "*** classnpc::exec_IA_ZIGZAG() - HOLE ahead - pos.x: " << position.x << ", xinc: " << xinc << std::endl;
			must_turn = true;
		}
		if (must_turn == false && !change_position(xinc, 0)) { // change direction if colided
			//std::cout << ">>> classnpc::exec_IA_ZIGZAG() - BLOCK ahead" << std::endl;
			must_turn = true;
		}
		if (must_turn == true) {
			state.animation_type = ANIM_TYPE_TURN;
			state.animation_state = 0;
			distance.width = 0;
			state.animation_timer = timer.getTimer() + 200;
			return;
		}
		distance.width += move_speed;
		//std::cout << "classnpc::exec_IA_ZIGZAG() - name: " << name << ", distance.width: " << distance.width << ", walk_range: " << walk_range << ", state.animation_type: " << state.animation_type << std::endl;
		if (distance.width >= walk_range) {
			state.animation_type = ANIM_TYPE_TURN;
			state.animation_state = 0;
			distance.width = 0;
			state.animation_timer = timer.getTimer() + 200;
		}
	}
	last_execute_time = timer.getTimer() + 20;
}


// ****************************************************************************** //
// FLOATER can be GHOST (cross walls) or FISH                                     //
// of returning to start point, it just stops where player scaped or was hit      //
// ****************************************************************************** //
void classnpc::exec_IA_FLOATER(bool can_cross_walls) {
	bool can_move = false;

	//printf(">> WARNING::exec_IA_GHOST - IA_state: %d <<\n", IA_state);
	struct struct_player_dist dist_players = dist_npc_players();

	switch (IA_state) {
		case 0: // initial status, stopped
			printf(">> WARNING::exec_IA_GHOST - stopped - dist: %d, limit: %d <<\n", dist_players.dist, TILESIZE*7);
			if (dist_players.dist < TILESIZE*7) {
				IA_state = 1;
				target = dist_players.pObj;
			}
			break;
		case 1: // following player
			if (target == NULL) { // error checking
				IA_state = 0;
			}

			if (dist_players.dist > 250 || dist_players.dist <= TILESIZE+4) { // player escaped or npc reached him, stop and wait 2.5 seconds
				IA_state = 0;
				last_execute_time = timer.getTimer()+2500;
				return;
			}

			if (target->getPosition().x > position.x) {
				if (state.direction != ANIM_DIRECTION_RIGHT) {
					state.direction = ANIM_DIRECTION_RIGHT;
				}
				if (can_cross_walls == false) {
					can_move = test_change_position(move_speed, 0);
					//std::cout << ">> exec_IA_FLOATER.move_RIGHT.can_move: " << can_move << " <<" << std::endl;
				}
				if (can_cross_walls == true || can_move == true) {
					position.x += move_speed;
				}
			} else if (target->getPosition().x < position.x) {
				if (state.direction != ANIM_DIRECTION_LEFT) {
					state.direction = ANIM_DIRECTION_LEFT;
				}
				if (can_cross_walls == false) {
					can_move = test_change_position(-move_speed, 0);
					//std::cout << ">> exec_IA_FLOATER.move_LEFT.can_move: " << can_move << " <<" << std::endl;
				}
				if (can_cross_walls == true || can_move == true) {
					position.x -= move_speed;
				}
			}

			if (target->getPosition().y > position.y) {
				if (can_cross_walls == false) {
					can_move = test_change_position(0, move_speed);
				}
				if (can_cross_walls  || can_move == true) {
					position.y += move_speed;
				}
			} else {
				if (can_cross_walls == false) {
					can_move = test_change_position(0, -move_speed);
				}
				if (can_cross_walls  || can_move == true) {
					position.y -= move_speed;
				}
			}
			last_execute_time = timer.getTimer()+130;
		default:
			break;
	}
}




void classnpc::exec_IA_GHOST()
{
	exec_IA_FLOATER(true);
}

void classnpc::exec_IA_FISH()
{
	exec_IA_FLOATER(false);
}

void classnpc::exec_IA_DOLPHIN()
{
	int blocked = 0;
	struct struct_player_dist dist_p = dist_npc_players();

	switch (IA_state) {
		case 0: // initial status, stopped
			// sub_status is used as speed acceleration control
			if (state.animation_type != ANIM_TYPE_TURN) {
				if (state.direction == ANIM_DIRECTION_LEFT) {
					if (test_change_position(-move_speed, 0)) {
						blocked = 1;
					} else {
						position.x -= move_speed;
						distance.width -= move_speed;
					}
				} else {
					if (test_change_position(move_speed, 0)) {
						blocked = 1;
					} else {
						position.x += move_speed;
						distance.width += move_speed;
					}
				}
				// set target, if needed
				if (target == NULL) {
					target = dist_p.pObj;
				}
				if (target->getPosition().y > position.y) {
					if (test_change_position(0, move_speed) == 0) {
						position.y += move_speed;
						distance.heigth += move_speed;
					}
				} else if (target->getPosition().y < position.y) {
					if (test_change_position(0, -move_speed) == 0) {
						position.y -= move_speed;
						distance.heigth -= move_speed;
					}
				}
				/// @TODO: implement a kind of counter or IA_sub_status
				/*
				if (blocked == 0) {
					if (ia_npc->move_info.sub_status > 15) {
						ia_npc->move_info.sub_status--;
					}
				}
				*/
			} else {
				//ia_npc->move_info.sub_status = 200;
			}
			//last_execute_time = timer.getTimer() + ia_npc->move_info.sub_status;
			if (distance.width > walk_range || distance.heigth > walk_range || blocked == 1) { // maximum is one-screen width
				state.animation_type = ANIM_TYPE_TURN;
				state.animation_state = 0;
				last_execute_time = timer.getTimer()+200;
				IA_state = 1;
				target = NULL;
			}
			break;
		case 1: // return to origin backwards
			if (distance.width <= 0 && distance.heigth <= 0) { // reached origin
				IA_state = 0;
				last_execute_time = timer.getTimer()+2000;
			} else { /// @TODO: fix to use distance->width/height
				if (state.direction != ANIM_DIRECTION_LEFT) {
					if (test_change_position(-move_speed, 0)) {
						position.x -= move_speed;
					}
				} else {
					if (test_change_position(move_speed, 0)) {
						position.x += move_speed;
					}
				}
				last_execute_time = timer.getTimer()+90;
			}
			break;

		default:
			break;
	}
}

void classnpc::exec_IA_SIDETOSIDE()
{
	/// @TODO
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classnpc::set_player_list(std::vector<character*> *set_list)
{
	classnpc::player_list = set_list;
}


// ********************************************************************************************** //
// Find the nearest player and return a struct with distance and a reference to player            //
// ********************************************************************************************** //
struct struct_player_dist classnpc::dist_npc_players()
{
	int dist, temp;
	struct struct_player_dist res;
	if (!classnpc::player_list) {
		std::cout << "ERROR: trying to calc NPC-player distance before there is a player in the game or this NPC does not have set the player_list" << std::endl;
		exit(-1);
	}

	res.pObj = classnpc::player_list->at(0);
	dist = sqrt(pow((position.x - res.pObj->getPosition().x), 2) + pow((position.y - res.pObj->getPosition().y), 2));
	if (classnpc::player_list->size() == 2) {
		character *p2 = classnpc::player_list->at(1);
		temp = sqrt(pow((position.x - p2->getPosition().x), 2) + pow((position.y - p2->getPosition().y), 2));
		if (temp < dist) {
			dist = temp;
			res.pObj = player_list->at(1);
		}
	}

	res.dist = dist;
	return res;

}


bool classnpc::have_player_list()
{
	if (player_list != NULL) {
		return true;
	}
	return false;
}

void classnpc::death()
{
	state.jump_state = NO_JUMP;
	dead = true;
	//std::cout << "classnpc::death" << std::endl;
}

bool classnpc::is_boss()
{
	return _is_boss;
}


void classnpc::set_is_boss(bool set_boss)
{
	_is_boss = set_boss;
}

bool classnpc::is_player_friend()
{
	return _is_player_friend;
}







// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classnpc::set_file_data(format_v2_0::file_npc data) {
	// TODO - usar operador igual e também para cópia de toda a classe para ela mesma
	int i;
	std::stringstream full_graphic_filename;
	graphicsLib_gSurface npc_sprite_surface;


	// set sprites to be persistent. they will be freed only by game.cpp
	for (int i=0; i<ANIM_DIRECTION_COUNT; i++) {
		for (int j=0; j<ANIM_TYPE_COUNT; j++) {
			for (int k=0; k<ANIM_FRAMES_COUNT; k++) {
				(character_graphics_list.find(name)->second)[i][j][k].frameSurface.persistent = true;
			}
		}
	}

	name = std::string(data.name);
	hitPoints.total = data.hp.total;
	hitPoints.current = hitPoints.total;
	state.direction = data.facing;
	if (state.direction < 0 || state.direction > ANIM_DIRECTION_COUNT) {
		state.direction = ANIM_DIRECTION_RIGHT;
	}
	position.x = 0;
	position.y = 0;

	move_speed = data.speed;
	walk_range = data.walk_range;
	if (walk_range < 0 || walk_range > 1000) { // fix data errors by setting value to default
		walk_range = TILESIZE*6;
	}
	facing = data.facing;
	start_point = data.start_point;
	graphic_filename = data.graphic_filename;
	for (i=0; i<NPC_FRAME_NUMBER; i++) {
		frames[i].frameset = data.frames[i].state;
		frames[i].frameset_time = data.frames[i].duration;
		// TODO - change file format in roder to support different sizes for each frame
		frames[i].framesize.width = data.frame_size.width;
		frames[i].framesize.heigth = data.frame_size.width;
		if (data.frames[i].state >= 0) {
			//std::cout << "npc[" << data.name << "].frames[" << i << "] - frameset: " << data.frames[i].state << ", duration: " << data.frames[i].duration << std::endl;
			//std::cout << "npc[" << data.name << "].frames[" << i << "] - frameset.w: " << data.frame_size.width << ", frameset.h: " << data.frame_size.heigth << std::endl;
		}
	}
	frameSize.width = data.frame_size.width;
	frameSize.heigth = data.frame_size.heigth;
	can_shoot = (data.can_shoot != 0);
	shield_type = data.shield_type;
	_is_boss = false;
	//_is_boss = (data.is_boss != 0);
	IA_type = data.IA_type;

	// TODO - this logic can be passed to the editor
	if (IA_type == IA_BAT || IA_type == IA_FLY_ZIG_ZAG || IA_type == IA_BUTTERFLY || IA_type == IA_GHOST || IA_type == IA_FISH || IA_type == IA_DOLPHIN || IA_type == IA_ROOF_SHOOTER || IA_type == IA_FIXED_JUMPER || IA_type == IA_SIDE_SHOOTER) {
		//std::cout << "******** classnpc::set_file_data - npc: " << name << ", canfly: " << can_fly << std::endl;
		can_fly = true;
	}
	projectile_type_id = data.projectile_id[0];

	realPosition.x = 0;
	realPosition.y = 0;

	max_jump = 20;					// hardcoded, fix this in editor
	max_projectiles = 1;			// hardcoded, fix this in editor
	move_speed = 6;					// hardcoded, fix this in editor
	attack_state = 0;
	last_execute_time = 0;

	// load the graphic from file and set it into frameset
	full_graphic_filename.str(std::string());
	full_graphic_filename << FILEPATH << "data/images/sprites/enemies/" << std::string(graphic_filename);
	graphLib.surfaceFromFile(full_graphic_filename.str(), &npc_sprite_surface);
	if (npc_sprite_surface.gSurface == NULL) {
		std::cout << "initFrames - Error loading player surface from file\n";
		return;
	}

	for (i=0; i<NPC_FRAME_NUMBER; i++) {
		if (frames[i].frameset != -1) {
			//addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_STAIRS_ATTACK, 19, 0, playerSpriteSurface, 200);
			//std::cout << "NPC - adding frame of type " << frames[i].frameset << ", duration: " << frames[i].frameset_time << ", at[0][" << i << "]" << std::endl;
			addSpriteFrame(ANIM_DIRECTION_LEFT, frames[i].frameset, i, 0, npc_sprite_surface, frames[i].frameset_time);
			addSpriteFrame(ANIM_DIRECTION_RIGHT, frames[i].frameset, i, 1, npc_sprite_surface, frames[i].frameset_time);
		}
	}
	npc_sprite_surface.freeGraphic();
	//std::cout << "end" << std::endl;

}

void classnpc::invert_direction()
{
	if (state.direction == ANIM_DIRECTION_LEFT) {
		state.direction = ANIM_DIRECTION_RIGHT;
	} else {
		state.direction = ANIM_DIRECTION_LEFT;
	}
	IA_state = IA_STAND;
}

// TODO - limit also per player
// type can be normal, diagonal_up or diagonal_down
// n is the number os projectiles to be shot
// alternated indicates if direction is the same for each shot or not
void classnpc::throw_projectile_npc(int type, int n, int alternated) {
	short int last_direction = -1;
	short int proj_direction;
	for (int i=0; i<n; i++) {
		st_position proj_pos;
		if (last_direction == -1) {
			last_direction = state.direction;
			proj_direction = state.direction;
		} else if (last_direction == ANIM_DIRECTION_LEFT) {
			last_direction = state.direction;
			proj_direction = ANIM_DIRECTION_RIGHT;
		} else if (last_direction == ANIM_DIRECTION_RIGHT) {
			last_direction = state.direction;
			proj_direction = ANIM_DIRECTION_LEFT;
		}
		if (proj_direction == ANIM_DIRECTION_LEFT) {
			proj_pos = st_position(position.x, position.y+frameSize.heigth/2);
		} else {
			proj_pos = st_position(position.x+frameSize.width, position.y+frameSize.heigth/2);
		}
		projectile_list.push_back(new projectile(0, proj_direction, 8, proj_pos, map, 1, type));
		//std::cout << ">>>>>>>>>>>>>>>>>>>> classnpc::throw_projectile_npc - NPC projectile shot" << std::endl;
	}
}



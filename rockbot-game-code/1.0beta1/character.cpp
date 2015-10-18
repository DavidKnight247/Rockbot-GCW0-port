#include "character.h"
#include "game.h"
#include "timerlib.h"

extern game gameControl;
extern timerLib timer;

#include "soundlib.h"
extern soundLib soundManager;

#include "inputlib.h"
extern inputLib input;

extern struct format_v_2_0_1::st_checkpoint checkpoint;

// initialize static member
std::map<std::string, st_spriteFrame[ANIM_DIRECTION_COUNT][ANIM_TYPE_COUNT][ANIM_FRAMES_COUNT]> character::character_graphics_list;

// init character with default values
// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
character::character() : map(NULL), hitPoints(1, 1), last_hit_time(0), is_player_type(false), platform(NULL), hit_duration(800), hit_animation_timer(0), hit_moved_back_n(0), jump_button_released(true), attack_button_released(true), dead(false), charging_color_n(0), charging_color_timer(0), shield_type(0), _moving_platform_timer(0), position(), _number(0)
{
	move_speed = 3;

	accel_speed_y = 1;
	gravity_y = 0.25;
	max_speed = 9;

	position.y = 0;
	position.x = 0;
	can_fly = false;
	max_jump = 64;
	attack_state = ATTACK_NOT;
	max_projectiles = 1;
}


// init character with default values
// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
character::~character()
{
	clean_projectiles();
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void character::char_update_real_position() {
	if (map != NULL) {
		realPosition.x = position.x - map->getMapScrolling().x;
		realPosition.y = position.y - map->getMapScrolling().y;
		//std::cout << "////////////// CHARACTER [2] - map_scroll.x: " << map->getMapScrolling().x << ", position.x: " << position.x << ", realPosition.x: " << realPosition.x << std::endl;
	} else {
		realPosition.x = position.x;
		realPosition.y = position.y;
	}
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void character::charMove() {
    int mapLock = 0;
	bool moved = false;
	int temp_move_speed = move_speed;

	if (!map) {
		return; // error - can't execute this action without an associated map
	}

	if (timer.getTimer() < state.move_timer) {
		return;
	}


	if (state.animation_type == ANIM_TYPE_TELEPORT) {
		gravity(false);
		state.move_timer = timer.getTimer() + TIME_MOVES/2;
		return;
	}

	if (state.animation_type == ANIM_TYPE_HIT) {
		if (hit_moved_back_n < TILESIZE) {
			//std::cout << ">>>>> HIT - change direction" << std::endl;
			if (state.direction == ANIM_DIRECTION_LEFT) {
				moveCommands.left = 0;
				moveCommands.right = 1;
			} else {
				moveCommands.left = 1;
				moveCommands.right = 0;
			}
			temp_move_speed = 1;
			gravity(false);
			state.move_timer = timer.getTimer() + TIME_MOVES/2;
		} else {
			state.animation_type = ANIM_TYPE_STAND;
			hit_moved_back_n = 0;
		}
	}

	if (moveCommands.left == 1 && position.x > 0 && state.animation_type != ANIM_TYPE_SLIDE && is_in_stairs_frame() == false) {
		for (int i=temp_move_speed; i>=1; i--) {
			mapLock = map_colision(-i, 0, map->getMapScrolling());
			if (mapLock == BLOCK_UNBLOCKED || mapLock == BLOCK_WATER) {
				if (mapLock == BLOCK_UNBLOCKED || (mapLock == BLOCK_WATER && abs(i*WATER_SPEED_MULT) < 1)) {
					position.x -= i + map->get_last_scrolled().x;
				} else {
					position.x -= i*WATER_SPEED_MULT + map->get_last_scrolled().x;
				}
				if (state.animation_type != ANIM_TYPE_HIT) {
					state.direction = ANIM_DIRECTION_LEFT;
				} else {
					hit_moved_back_n += temp_move_speed;
					return;
				}
				state.move_timer = timer.getTimer() + TIME_MOVES;
				if (state.animation_type != ANIM_TYPE_WALK && state.animation_type != ANIM_TYPE_WALK_ATTACK) {
					state.animation_timer = 0;
				}
				if (state.animation_type != ANIM_TYPE_JUMP && state.animation_type != ANIM_TYPE_SLIDE && state.animation_type != ANIM_TYPE_JUMP_ATTACK && state.animation_type != ANIM_TYPE_HIT && (state.animation_type != ANIM_TYPE_WALK_ATTACK || (state.animation_type == ANIM_TYPE_WALK_ATTACK && state.attack_timer+ATTACK_DELAY < timer.getTimer()))) {
					state.animation_type = ANIM_TYPE_WALK;
				}
				moved = true;
				break;
			}
		}
	}
	if (moveCommands.left == 1 && state.direction != ANIM_DIRECTION_LEFT && (state.animation_type == ANIM_TYPE_SLIDE || is_in_stairs_frame() == true)) {
        state.direction = ANIM_DIRECTION_LEFT;
    }

	if (moveCommands.right == 1 && state.animation_type != ANIM_TYPE_SLIDE && is_in_stairs_frame() == false) {
		for (int i=temp_move_speed; i>=1; i--) {
			mapLock = map_colision(i, 0, map->getMapScrolling());
			//mapLock =  gameControl.getMapPointLock(st_position((position.x + frameSize.width + i)/TILESIZE, (position.y + frameSize.heigth/2)/TILESIZE));
			if (mapLock == BLOCK_UNBLOCKED || mapLock == BLOCK_WATER) {
				//std::cout << "character::charMove - temp_move_speed: " << temp_move_speed << ", map->get_last_scrolled().x: " << map->get_last_scrolled().x << std::endl;
				if (mapLock == TERRAIN_UNBLOCKED || (mapLock == BLOCK_WATER && abs(i*WATER_SPEED_MULT) < 1)) {
					position.x += i - map->get_last_scrolled().x;
				} else {
					position.x += i*WATER_SPEED_MULT - map->get_last_scrolled().x;
				}
				if (state.animation_type != ANIM_TYPE_HIT) {
					state.direction = ANIM_DIRECTION_RIGHT;
				} else {
					hit_moved_back_n += temp_move_speed;
					return;
				}
				if (state.animation_type != ANIM_TYPE_WALK && state.animation_type != ANIM_TYPE_WALK_ATTACK) {
					state.animation_timer = 0;
				}
				if (state.animation_type != ANIM_TYPE_JUMP && state.animation_type != ANIM_TYPE_SLIDE && state.animation_type != ANIM_TYPE_JUMP_ATTACK && state.animation_type != ANIM_TYPE_HIT && (state.animation_type != ANIM_TYPE_WALK_ATTACK || (state.animation_type == ANIM_TYPE_WALK_ATTACK && state.attack_timer+ATTACK_DELAY < timer.getTimer()))) {
					state.animation_type = ANIM_TYPE_WALK;
				}
				state.move_timer = timer.getTimer() + TIME_MOVES;
				moved = true;
				break;
			}
		}
	}
	if (moveCommands.right == 1 && state.direction != ANIM_DIRECTION_RIGHT && (state.animation_type == ANIM_TYPE_SLIDE || is_in_stairs_frame() == true)) {
        state.direction = ANIM_DIRECTION_RIGHT;
    }


	// check if character is on stairs
	if (moveCommands.up == 1) { // check stairs on middle
		st_position stairs_pos = is_on_stairs(st_rectangle(position.x, position.y+(frameSize.heigth/2), frameSize.width, frameSize.heigth));
		if (stairs_pos.x != -1) {
			if (state.animation_type != ANIM_TYPE_STAIRS_MOVE) {
				state.animation_type = ANIM_TYPE_STAIRS_MOVE;
			}
			position.y -= temp_move_speed/2;
			state.jump_state = NO_JUMP;
			position.x = stairs_pos.x * TILESIZE - 6;
		} else if (map->getMapPointLock(st_position(((position.x+frameSize.width/2)/TILESIZE), ((position.y+frameSize.heigth)/TILESIZE))) == TERRAIN_STAIR) { // check stairs bottom (leaving)
			state.animation_type = ANIM_TYPE_STAIRS_SEMI;
			position.y -= temp_move_speed/2;
			state.jump_state = NO_JUMP;
			//position.x = stairs_pos.x * TILESIZE - 6;
		} else if (state.animation_type == ANIM_TYPE_STAIRS_SEMI) {
			state.animation_type = ANIM_TYPE_STAND;
			position.y -= 1;
		}
	}


    if (moveCommands.down == 1 && state.animation_type != ANIM_TYPE_SLIDE) {
		st_position stairs_pos = is_on_stairs(st_rectangle(position.x, position.y+frameSize.heigth, frameSize.width, frameSize.heigth));
		if (stairs_pos.x != -1) {
			state.animation_type = ANIM_TYPE_STAIRS_MOVE;
			position.y += temp_move_speed/2;
			state.jump_state = NO_JUMP;
			position.x = stairs_pos.x * TILESIZE - 6;
        } else if (state.animation_type != ANIM_TYPE_JUMP && state.animation_type != ANIM_TYPE_JUMP_ATTACK) {
			state.animation_type = ANIM_TYPE_STAND;
		}
	}

	// is on stairs without moving
	if (moveCommands.down == 0 && moveCommands.up == 0 && state.animation_type == ANIM_TYPE_STAIRS_MOVE) {
		state.animation_type = ANIM_TYPE_STAIRS;
	}

	attack();

	bool res_slide = slide(moveCommands.jump, moveCommands.down, map->getMapScrolling());

    bool resJump = false;
    resJump = jump(moveCommands.jump, map->getMapScrolling());
    if (resJump == true || res_slide == true) {
		state.move_timer = timer.getTimer() + TIME_MOVES;
	} else { // if turning, do not fall
		gravity(false);
	}


    if (is_player_type && moved == false && resJump == false && res_slide == false) {
		if (is_in_stairs_frame() == false && state.animation_type != ANIM_TYPE_TELEPORT && state.animation_type != ANIM_TYPE_TELEPORT && state.animation_type != ANIM_TYPE_HIT && state.animation_type != ANIM_TYPE_SLIDE && (state.animation_type != ANIM_TYPE_ATTACK || (state.animation_type == ANIM_TYPE_ATTACK && state.attack_timer+ATTACK_DELAY < timer.getTimer()))) {
			//if (is_player()) std::cout << "********* reset to stand - on_stairs_frame: " << is_in_stairs_frame() << ", state.animation_type: " << state.animation_type << std::endl;
			state.animation_type = ANIM_TYPE_STAND;
			//state.animation_timer = 0;
		}
		//std::cout << "charMove - ANIM_TYPE_HIT: " << ANIM_TYPE_HIT << ", animation_type: " << state.animation_type << ", timer.getTimer(): " << timer.getTimer() << ", hit_duration+last_hit_time: " << hit_duration+last_hit_time << std::endl;
		if (state.animation_type == ANIM_TYPE_HIT && timer.getTimer() > hit_duration/2+last_hit_time) { // finished hit time
			//if (is_player()) std::cout << "state.animation_type SET to STAND " << std::endl;
			state.animation_type = ANIM_TYPE_STAND;
			//state.animation_timer = 0;
		}
	}


	map->reset_scrolled();

}

void character::clear_move_commands()
{
	moveCommands.up = 0;
	moveCommands.down = 0;
	moveCommands.left = 0;
	moveCommands.right = 0;
	moveCommands.attack = 0;
	moveCommands.jump = 0;
	moveCommands.start = 0;
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void character::change_char_color(st_color key, st_color new_color)
{
    for (int i=0; i<ANIM_DIRECTION_COUNT; i++) {
        for (int j=0; j<ANIM_TYPE_COUNT; j++) {
            for (int k=0; k<ANIM_FRAMES_COUNT; k++) {
                graphLib.change_surface_color(key, new_color, &(character_graphics_list.find(name)->second)[i][j][k].frameSurface);
            }
        }
    }
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
/// @TODO: this must be moved to player, as character attack must be very simple
void character::attack()
{
	//std::cout << "character::attack - START, name: " << name << ", max_projectiles: " << max_projectiles << ", projectile_list.size(): " << projectile_list.size() << std::endl;
	if (character_graphics_list.find(name) == character_graphics_list.end()) {
		return;
	}
	if (attack_state != ATTACK_NOT && timer.getTimer()-state.attack_timer >= (character_graphics_list.find(name)->second)[state.direction][state.animation_type][state.animation_state].delay) {
		//std::cout << "character::attack - shoot projectile END" << std::endl;
		attack_state = ATTACK_NOT;
	}

    int attack_id = -1;
    int attack_damage = 1;

	if (moveCommands.attack != 0 && max_projectiles > projectile_list.size() && (timer.getTimer()-state.attack_timer) > 100 && attack_button_released) {
        attack_id = 0;
    }
    // charged shot
    if (moveCommands.attack == 0 && (timer.getTimer()-state.attack_timer) > 2500 && attack_button_released == false) {
		attack_id = 10;
        attack_damage = 4;
    }
    // semi-charged shot
    if (moveCommands.attack == 0 && (timer.getTimer()-state.attack_timer) > 400 && (timer.getTimer()-state.attack_timer) < 2000 && attack_button_released == false) {
        attack_id = 11;
        attack_damage = 2;
    }

    if (moveCommands.attack == 0 && attack_button_released == false) {
        attack_button_released = true;
    }

    // change player colors if charging attack
    if ((timer.getTimer()-state.attack_timer) > 400 && (timer.getTimer()-state.attack_timer) < 2500 && attack_button_released == false) {
        if (color_keys[0].r != -1) {
            if (charging_color_n == 0) {
                change_char_color(color_keys[0], st_color(171, 0, 19));
                change_char_color(color_keys[1], st_color(231, 0, 91));
            } else if (charging_color_n == 1) {
                change_char_color(color_keys[0], st_color(231, 0, 91));
                change_char_color(color_keys[1], st_color(255, 119, 183));
            } else if (charging_color_n == 2) {
                change_char_color(color_keys[0], st_color(255, 119, 183));
                change_char_color(color_keys[1], st_color(171, 0, 19));
            }
            if (charging_color_timer < timer.getTimer()) {
                charging_color_n++;
                if (charging_color_n > 2) {
                    charging_color_n = 0;
                }
                charging_color_timer = timer.getTimer()+200;
            }
        }
    }
    if ((timer.getTimer()-state.attack_timer) >= 2500 && attack_button_released == false) {
        if (color_keys[0].r != -1) {
            if (charging_color_n == 0) {
                change_char_color(color_keys[0], st_color(219, 43, 0));
                change_char_color(color_keys[1], st_color(255, 155, 59));
            } else if (charging_color_n == 1) {
                change_char_color(color_keys[0], st_color(255, 155, 59));
                change_char_color(color_keys[1], st_color(255, 234, 0));
            } else if (charging_color_n == 2) {
                change_char_color(color_keys[0], st_color(255, 234, 0));
                change_char_color(color_keys[1], st_color(219, 43, 0));
            }
            if (charging_color_timer < timer.getTimer()) {
                charging_color_n++;
                if (charging_color_n > 2) {
                    charging_color_n = 0;
                }
                charging_color_timer = timer.getTimer()+100;
            }
        }
    }

    if (attack_id != -1) {
        attack_button_released = false; // coment out this line to get "turbo" in button
		//attack_state == ATTACK_NOT &&
		//std::cout << "character::attack - shoot projectile" << std::endl;
		st_position proj_pos;
		if (state.direction == ANIM_DIRECTION_LEFT) {
			proj_pos = st_position(position.x, position.y+frameSize.heigth/2);
		} else {
			proj_pos = st_position(position.x+frameSize.width, position.y+frameSize.heigth/2);
		}
		projectile_list.push_back(new projectile(attack_id, state.direction, 8, proj_pos, map, attack_damage, TRAJECTORY_LINEAR));

		attack_state = ATTACK_START;
		state.attack_timer = timer.getTimer();
		if (state.animation_type == ANIM_TYPE_STAND) {
			state.animation_type = ANIM_TYPE_ATTACK;
		} else if (state.animation_type == ANIM_TYPE_JUMP) {
			state.animation_type = ANIM_TYPE_JUMP_ATTACK;
		} else if (is_in_stairs_frame()) {
			state.animation_type = ANIM_TYPE_STAIRS_ATTACK;
		} else if (state.animation_type == ANIM_TYPE_WALK) {
			std::cout << "+++++++++++ CHARACTER - set animation to ANIM_TYPE_WALK_ATTACK" << std::endl;
			state.animation_type = ANIM_TYPE_WALK_ATTACK;
		}
		state.animation_timer = 0;
        if (color_keys[0].r != -1) {
            change_char_color(color_keys[0], color_keys[0]);
        }
        if (color_keys[1].r != -1) {
            change_char_color(color_keys[1], color_keys[1]);
        }
    }
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void character::show() {
	// update real position
	char_update_real_position();
	//cout << "character::show - direction: " << state.direction << ", type: " << state.animation_type << ", state: " << state.animation_state << "\n";
	//[ANIM_DIRECTION_COUNT][ANIM_TYPE_COUNT][ANIM_FRAMES_COUNT]
	if (state.direction < 0 || state.direction > ANIM_DIRECTION_COUNT) {
		//std::cout << "WARNING - character::show - (" << name << ") error, direction value " << state.direction << " is invalid" << std::endl;
		state.direction = ANIM_DIRECTION_LEFT;
		return;
	}
	if (state.animation_type < 0 || state.animation_type > ANIM_TYPE_COUNT) {
		std::cout << "character::show - error, type value " << state.direction << " is invalid" << std::endl;
		return;
	}
    if (state.animation_state < 0 || (is_player() && state.animation_state > MAX_PLAYER_SPRITES) || (!is_player() && state.animation_state > MAX_NPC_SPRITES)) {
		std::cout << "character::show - error, animation_state value " << state.animation_state << " is invalid. state.animation_type: " << state.animation_type << std::endl;
		state.animation_state = 0;
		return;
	}
	if ((character_graphics_list.find(name)->second)[state.direction][state.animation_type][state.animation_state].frameSurface.width == 0 || (character_graphics_list.find(name)->second)[state.direction][state.animation_type][state.animation_state].frameSurface.gSurface == NULL) { // reset state
		state.animation_state = 0;
	}
	/// blinking when hit
	unsigned int now_timer = timer.getTimer();
	if (now_timer < hit_duration+last_hit_time) {
		if (hit_animation_timer > now_timer) {
			return;
		} else if ((hit_animation_timer+HIT_BLINK_ANIMATION_LAPSE) < now_timer) {
			hit_animation_timer = now_timer+HIT_BLINK_ANIMATION_LAPSE;
		}
	}


	/*
	if (is_player()) {
		std::cout << "--------- realPosition.x: " << realPosition.x << "---------" << std::endl;
	}
	*/


	if (name == "Seahorse Bot") cout << "character::show - trying to show frame[" << state.direction << "][" << state.animation_type << "][" << state.animation_state << "]" << std::endl;

	// turn is a special case, if it does not exist, we must show stand instead
	if (state.animation_type == ANIM_TYPE_TURN && have_frame_graphic(state.direction, state.animation_type, state.animation_state) == false) {
		if (have_frame_graphic(state.direction, ANIM_TYPE_WALK, state.animation_state) == true) {
			if (name == "Seahorse Bot") cout << "character::show - 1" << std::endl;
			graphLib.showSurfaceAt(&(character_graphics_list.find(name)->second)[state.direction][ANIM_TYPE_WALK][state.animation_state].frameSurface, realPosition, false);
		} else {
			if (name == "Seahorse Bot") cout << "character::show - 2" << std::endl;
			graphLib.showSurfaceAt(&(character_graphics_list.find(name)->second)[state.direction][ANIM_TYPE_STAND][state.animation_state].frameSurface, realPosition, false);
		}
	} else if (have_frame_graphic(state.direction, state.animation_type, state.animation_state) == false) { // reset state
		if (name == "Seahorse Bot") cout << "character::show - RESET STATE" << std::endl;
		if (state.animation_type == ANIM_TYPE_WALK_ATTACK) {
			std::cout << "character::show - RESET" << std::endl;
		}
		if (state.animation_type != ANIM_TYPE_WALK) {
			state.animation_type = ANIM_TYPE_WALK;
		} else if (state.animation_type != ANIM_TYPE_HIT) {
			state.animation_type = ANIM_TYPE_STAND;
		}
	} else {
		if (have_frame_graphic(state.direction, state.animation_type, state.animation_state) == true) {
			if (name == "Seahorse Bot") cout << "character::show - 3 [" << state.direction << "][" << state.animation_type << "][" << state.animation_state << "]" << std::endl;
			graphLib.showSurfaceAt(&(character_graphics_list.find(name)->second)[state.direction][state.animation_type][state.animation_state].frameSurface, realPosition, false);
		} else {
			if (name == "Seahorse Bot") cout << "character::show - NO SHOW" << std::endl;
		}
	}

	if (state.animation_timer < timer.getTimer()) {
		// change animation state to next frame
		//if ((character_graphics_list.find(name)->second)[state.direction][state.animation_type][state.animation_state+1].frameSurface.gSurface != NULL) {
		if (have_frame_graphic(state.direction, state.animation_type, (state.animation_state+1))) {
			//if (state.animation_type == ANIM_TYPE_WALK_ATTACK) std::cout << "character::show - NEXT.OK" << std::endl;
			state.animation_state += 1;
		} else {
			//if (state.animation_type == ANIM_TYPE_WALK_ATTACK) std::cout << "character::show - NEXT.ERROR" << std::endl;
			if (state.animation_type == ANIM_TYPE_TURN)	{
				if (state.direction == ANIM_DIRECTION_LEFT) {
					state.direction = ANIM_DIRECTION_RIGHT;
				} else {
					state.direction = ANIM_DIRECTION_LEFT;
				}
				//if (is_player()) std::cout << "state.animation_type SET to STAND " << std::endl;
				state.animation_type = ANIM_TYPE_STAND;
			}
			if (state.animation_state > 0) {
				state.animation_state = 0;
			}
		}
		if (state.animation_type == ANIM_TYPE_WALK_ATTACK) {
			state.animation_timer = timer.getTimer() + 5000;
			if (is_player() && state.animation_type == ANIM_TYPE_WALK_ATTACK) std::cout << "### SET 1 - state.animation_timer: " << state.animation_timer << ", timer.getTimer(): " << timer.getTimer() << std::endl;
		} else {
			if (is_player() && state.animation_type == ANIM_TYPE_WALK_ATTACK) std::cout << "&&&&& SET 2 - state.animation_timer: " << state.animation_timer << ", timer.getTimer(): " << timer.getTimer() << std::endl;
			state.animation_timer = timer.getTimer() + (character_graphics_list.find(name)->second)[state.direction][state.animation_type][state.animation_state].delay;
		}
		//std::cout << "char: " << name << ", delay: " << (character_graphics_list.find(name)->second)[state.direction][state.animation_type][state.animation_state].delay << "\n";
		//state.animation_timer = timer.getTimer() + 200;
	}
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
bool character::gravity(bool boss_demo_mode=false)
{
	if (!map) {
		return false; // error - can't execute this action without an associated map
	}


	// ------------- NPC gravity ------------------ //
	if (!is_player()) {
		if (!map) {
			return false; // error: is not into a map, can't use gravity
		}
		if (!can_fly) {
			bool is_moved = false;
			for (int i=move_speed; i>0; i--) {
				if ((boss_demo_mode == true && position.y <= RES_H/2) || test_change_position(0, i) == true) {
					position.y += i;
					is_moved = true;
					break;
				}
			}
			return is_moved;
		}
		return false; // not moved because of IA type
	}



	// ------------ PLAYER gravity --------------------- //
	if (is_player() && position.y > RES_H+TILESIZE) {
		hitPoints.current = 0;
		death();
		return false;
	}

	if (is_in_stairs_frame()) {
		//std::cout << "gravity - leave because on stairs" << std::endl;
		return false;
	}

	//std::cout << "gravity - state.jump_state: " << state.jump_state << ", NO_JUMP: " << NO_JUMP << std::endl;

	if (state.jump_state == NO_JUMP && can_fly == false && position.y < RES_H+TILESIZE+1 + frameSize.heigth) {

		accel_speed_y += accel_speed_y*gravity_y;
		if (accel_speed_y < 1) {
			accel_speed_y = gravity_y;
		} else if (accel_speed_y > max_speed) {
			accel_speed_y = max_speed;
		}

		int adjusted_speed = accel_speed_y;

		if (state.animation_type == ANIM_TYPE_TELEPORT) {
			adjusted_speed = max_speed;
		}

		//std::cout << "gravity - accel_speed_y: " << accel_speed_y << ", gravity_y: " << gravity_y << ", adjusted_speed: " << adjusted_speed << std::endl;


		bool was_moved = false;
		for (int i=adjusted_speed; i>0; i--) {
			int mapLock = map_colision(0, i, map->getMapScrolling());

			//std::cout << "gravity - mapLock: " << mapLock << ", botom: " << position.y+frameSize.heigth << std::endl;

			if (position.y < 0 || position.y > RES_H || (state.animation_type == ANIM_TYPE_TELEPORT && position.y < RES_H/2)) {
				mapLock = BLOCK_UNBLOCKED;
			}

			if (mapLock == BLOCK_UNBLOCKED || mapLock == BLOCK_WATER || mapLock == BLOCK_STAIR_X || mapLock == BLOCK_STAIR_Y) {
				//std::cout << "character::gravity - FALL" << std::endl;
				if (mapLock != BLOCK_WATER || (mapLock == BLOCK_WATER && abs(i*WATER_SPEED_MULT) < 1)) {
					position.y += i;
				} else {
					position.y += i*WATER_SPEED_MULT;
				}
				if (state.animation_type != ANIM_TYPE_JUMP && state.animation_type != ANIM_TYPE_JUMP_ATTACK && state.animation_type != ANIM_TYPE_TELEPORT && state.animation_type != ANIM_TYPE_SLIDE && (state.animation_type != ANIM_TYPE_JUMP_ATTACK || (state.animation_type == ANIM_TYPE_JUMP_ATTACK && state.attack_timer+ATTACK_DELAY < timer.getTimer()))) {
					state.animation_type = ANIM_TYPE_JUMP;
				}
				was_moved = true;
				break;
			}
			if (i == 1) {
				accel_speed_y = 1;
			}
		}
		if (was_moved == false && (state.animation_type == ANIM_TYPE_JUMP || state.animation_type == ANIM_TYPE_JUMP_ATTACK) && state.animation_type != ANIM_TYPE_SLIDE) {
			state.animation_type = ANIM_TYPE_STAND;
			return true;
		} else if (was_moved == false && state.animation_type == ANIM_TYPE_TELEPORT && position.y >= RES_H/2) {
			state.animation_type = ANIM_TYPE_STAND;
			return true;
		}
		// teleport finished
		//std::cout << "NOT FALLING, RESET ACCEL_SPEED_Y" << std::endl;
	}
	return false;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
struct st_position character::getPosition()
{
	/// @TODO - this is crashing sometimes
	return position;
}

void character::set_position(struct st_position new_pos)
{
	//std::cout << "\\\\\\\\\\\\ character::set_position - pos.x: " << position.x << ", new_pos.x: " << new_pos.x << std::endl;
	position.x = new_pos.x;
	position.y = new_pos.y;
}



// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
bool character::slide(int command_jump, int command_down, st_position mapScrolling)
{
    // deal with cases player should not slide
    /// @TODO: share common code between jump and slide

    if (state.animation_type == ANIM_TYPE_TELEPORT || state.animation_type == ANIM_TYPE_JUMP || state.animation_type == ANIM_TYPE_JUMP_ATTACK) {
        return false;
    }

	if (is_in_stairs_frame()) {
		return false;
	}

	// if there is no ground, interrupts slide
	int fall_map_lock = map_colision(0, 1, map->getMapScrolling());



    int map_lock = map_colision(0, -6, map->getMapScrolling()); // this is minus six because of +4 adjustments in jump-up colision
    //std::cout << "character::slide - map_lock: " << map_lock << std::endl;

    // releasing down interrupts the slide
	if (command_down != 1 && state.animation_type == ANIM_TYPE_SLIDE && (map_lock == BLOCK_UNBLOCKED || map_lock == BLOCK_WATER)) {
        state.animation_type = ANIM_TYPE_STAND;
        return false;
    }

    // change jump button released state, if needed
    if (jump_button_released == false && command_jump == 0) {
        jump_button_released = true;
    }

    /*
    // check if button was not released between slides
    if (state.jump_state == NO_JUMP && state.jump_mark == -1) {
        if (jumpCommandStage == 0) {
            state.jump_mark = 0;
        } else {
            return false;
        }
    }
    */

	if (state.slide_distance > TILESIZE*5 && (map_lock == BLOCK_UNBLOCKED || map_lock == BLOCK_WATER)) {
        state.animation_type = ANIM_TYPE_STAND;
        state.slide_distance = 0;
        return false;
    }

    // no need to slide
    if (state.animation_type != ANIM_TYPE_SLIDE && command_jump != 1 && command_down != 1) {
        return false;
    // start slide
	} else if (state.animation_type != ANIM_TYPE_SLIDE && command_jump == 1 && command_down == 1 && jump_button_released == true) {
        state.animation_type = ANIM_TYPE_SLIDE;
        state.slide_distance = 0;
        jump_button_released = false;
        int adjust_x = -3;
        if (state.direction == ANIM_DIRECTION_LEFT) {
            adjust_x = frameSize.width+3;
        }
        map->add_animation(ANIMATION_STATIC, std::string("dash_dust.png"), position, st_position(adjust_x, frameSize.heigth-8), 100, 0, state.direction, st_size(8, 8));
    }

    if (state.animation_type != ANIM_TYPE_SLIDE) {
        return false;
    }

	if (fall_map_lock == BLOCK_UNBLOCKED || fall_map_lock == BLOCK_WATER) {
		state.animation_type = ANIM_TYPE_JUMP;
		state.slide_distance = 0;
		return false;
	}


    int res_move_x = 0;
    int mapLockAfter = BLOCK_UNBLOCKED;

    // reduce progressively the jump-move value in oder to deal with colision
    for (int i=move_speed*2; i>0; i--) {

        int temp_i;
        if (state.direction == ANIM_DIRECTION_LEFT) {
            temp_i = -i;
        } else {
            temp_i = i;
        }

        mapLockAfter = map_colision(temp_i, 0, mapScrolling);
        if (mapLockAfter == BLOCK_UNBLOCKED) {
            res_move_x = temp_i;
            break;
        } else if (mapLockAfter == BLOCK_WATER) {
            if (abs(i*WATER_SPEED_MULT) > 0) {
                res_move_x = temp_i*WATER_SPEED_MULT;
            } else {
                res_move_x = temp_i;
            }
            break;
        }
    }


    if (mapLockAfter == BLOCK_UNBLOCKED || mapLockAfter == BLOCK_WATER) {
        position.x += res_move_x;
        state.slide_distance += abs(res_move_x);
    }

    return true;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
bool character::jump(int jumpCommandStage, st_position mapScrolling)
{
	int mapLockAfter = BLOCK_UNBLOCKED;
	int mapLockBefore;
	int jumpYMove, tempJumpY;

	if (jump_button_released == false && jumpCommandStage == 0) {
		jump_button_released = true;
	}

	if (is_in_stairs_frame()) {
		//std::cout << "jump - leave because on stairs - jumpCommandStage: " << jumpCommandStage << ", jump_button_released: " << jump_button_released << std::endl;
		if (jumpCommandStage == 1 && jump_button_released == true) {
			state.jump_state = JUMP_DOWN;
			state.jump_speed = 1;
			state.jump_mark = 0;
			state.animation_type = ANIM_TYPE_JUMP;
			//std::cout << "jump - jump on stairs" << std::endl;
		}
		return false;
	}


    if (state.animation_type == ANIM_TYPE_SLIDE && jump_button_released == false) {
        return false;
    }


	/*
	// check if button was not released between jumps
	if (state.jump_state == NO_JUMP && state.jump_mark == -1) {
		if (jumpCommandStage == 0) {
			state.jump_mark = 0;
		} else {
			return false;
		}
	}
	*/


	// switch states, if needed
    if (jump_button_released == true && jumpCommandStage == 1 && state.jump_state == NO_JUMP) {
		state.jump_mark = 0;
		state.jump_timer = timer.getTimer();
		state.jump_speed = max_speed;
		state.jump_state = JUMP_UP;
        jump_button_released = false;
    }

	if (state.jump_state == JUMP_UP && jumpCommandStage == 0) { // interrupts jump
		state.jump_state = JUMP_DOWN;
		state.jump_speed = 1;
		state.jump_mark = 0;
	}


	if (state.jump_state == NO_JUMP) {
		return false;
	}


	// deal with cases player should not jump
	if (state.animation_type == ANIM_TYPE_TELEPORT) {
		return false;
	}



	if (state.jump_state == JUMP_UP) {
		mapLockBefore = map_colision(0, -1, mapScrolling);
	} else {
		mapLockBefore = map_colision(0, 1, mapScrolling);
	}

	float acceleration = 0.12;
	int calc_max_speed = max_speed;
	if (mapLockBefore == BLOCK_WATER) {
		acceleration = 0.025;
		calc_max_speed = max_speed*WATER_SPEED_MULT;
	}


	// calculate jump movement speeed
	if (state.jump_state == JUMP_UP) {
		state.jump_speed -= state.jump_speed*acceleration;
		//std::cout << "JUMP - acceleration: " << acceleration << ", calc_max_speed: " << calc_max_speed << ", state.jump_speed: " << state.jump_speed << std::endl;
		if (state.jump_speed <= 0.5) {
			// switch to jump_down
			state.jump_state = JUMP_DOWN;
		} else if (state.jump_speed > calc_max_speed) {
			state.jump_speed = calc_max_speed;
		}

		jumpYMove = state.jump_speed * -1;
	} else {
		/// @TODO: implement jump down
		if (state.jump_speed <= 1) {
			state.jump_speed = 2;
		} else {
			state.jump_speed += state.jump_speed*acceleration;
		}
		if (state.jump_speed > calc_max_speed) {
			state.jump_speed = calc_max_speed;
		}
		jumpYMove = state.jump_speed;
	}


	// reduce progressively the jump-move value in oder to deal with colision
	for (int i=abs(jumpYMove); i>0; i--) {
		if (jumpYMove < 0) {
			tempJumpY = i*-1;
		} else {
			tempJumpY = i;
		}
		mapLockAfter = map_colision(0, tempJumpY, mapScrolling);
		//if (is_player()) std::cout << "1. pos.x: " << position.x << ", pos.y: " << position.y << ", jumpYMove[" << i << "]: " << jumpYMove << ", mapLockAfter: " << mapLockAfter << std::endl;

		if (mapLockAfter == BLOCK_UNBLOCKED) {
			jumpYMove = tempJumpY;
			break;
		} else if (mapLockAfter == BLOCK_WATER) {
			if (abs(tempJumpY*WATER_SPEED_MULT) > 0) {
				jumpYMove = tempJumpY*WATER_SPEED_MULT;
			} else {
				jumpYMove = tempJumpY;
			}
			break;
		}
	}
	//std::cout << "2. jumpYMove: " << jumpYMove << ", mapLockAfter: " << mapLockAfter << std::endl;


	if (mapLockAfter == BLOCK_UNBLOCKED || mapLockAfter == BLOCK_WATER) {
		if (mapLockAfter == BLOCK_UNBLOCKED) {
			position.y += jumpYMove;
		} else {
			if (abs(jumpYMove*WATER_SPEED_MULT) > 0) {
				position.y += jumpYMove*WATER_SPEED_MULT;
			} else {
				position.y += jumpYMove;
			}
		}
		if (state.jump_state == JUMP_UP) {
			if (state.animation_type != ANIM_TYPE_JUMP && (state.animation_type != ANIM_TYPE_JUMP_ATTACK || (state.animation_type == ANIM_TYPE_JUMP_ATTACK && state.attack_timer+ATTACK_DELAY < timer.getTimer()))) {
				//std::cout << "jump - start" << std::endl;
				state.animation_type = ANIM_TYPE_JUMP;
			}
		}
	} else if (mapLockAfter != BLOCK_UNBLOCKED && mapLockAfter != BLOCK_WATER && state.jump_state == JUMP_DOWN) {
		soundManager.play_sfx(SFX_PLAYER_JUMP);
		state.animation_type = ANIM_TYPE_STAND;
		state.jump_state = NO_JUMP;
		//std::cout << "jump hit the ground" << std::endl;
		return true;
	} else if (mapLockAfter != BLOCK_UNBLOCKED && mapLockAfter != BLOCK_WATER && state.jump_state == JUMP_UP) {
		//std::cout << "jump hit the head" << std::endl;
		state.jump_state = JUMP_DOWN;
		state.jump_speed = 1;
		state.jump_mark = 0;
	}
	/*
	if (position.y < -frameSize.heigth) {
		state.jump_state = JUMP_DOWN;
		state.jump_speed = 1;
		state.jump_mark = 0;
	}
	*/
    return true;
}



// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
int character::rectangle_collision(st_rectangle rect1, st_rectangle rect2)
{
	if (rect1.x > rect2.x && rect1.x < rect2.x+rect2.w && rect1.y > rect2.y && rect1.y < rect2.y+rect2.h)
	{
		return 1;
	}
	if (rect1.x+rect1.w > rect2.x && rect1.x+rect1.w < rect2.x+rect2.w && rect1.y+rect1.h > rect2.y && rect1.y+rect1.h < rect2.y+rect2.h)
	{
		return 2;
	}
	return 0;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
short int character::map_colision(const short int incx, const short int incy, st_position mapScrolling) {
	// test colision with objects
	//printf(">> test_map_colision.START\n");
	int px = position.x;
	int py = position.y;
	int pw = frameSize.width;
	int ph = frameSize.heigth;
	short int diff=0, i, map_tile_x, map_tile_y, diff_w=0, diff_h=0;
	short int block=0; // holds the result. 0 is no blocking, 1 is blocking x, 2 is blocking y, 3 is blocking both x and y

	if (is_player_type) {
		if (pw > 21) {
            diff_w = abs(ph-21);
		}
        if (state.animation_type == ANIM_TYPE_SLIDE) {
            diff_h = abs(TILESIZE-1-ph);
        }
        if (incy < 0) {
			diff_h += 4;
		}
	}

	//if (is_player()) std::cout << "character::map_colision - "



	object_colision res_colision_object = map->colision_player_object(this, incx, incy, diff_w, diff_h);

	if (res_colision_object._block != 0) {
		if (!get_item(res_colision_object)) {
			block = res_colision_object._block;
		}
		//std::cout << "character::map_colision - res_colision_object._block: " << res_colision_object._block << std::endl;
	}

	short int res_colision_npc = map->colision_player_npcs(this, incx, incy, diff_w, diff_h);
	if (res_colision_npc == 1) {
		damage(3);
	}

	map_tile_x = (px+diff_w)/TILESIZE;

	if (incx != 0 && py >= 0 && py <= RES_H) {
		/*

		// TODO: must implement the logic for two player scrolling outside the character class

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
		*/
		if (incx > 0 && px+incx > MAP_W*TILESIZE) {
			block = 1;
		} else if (incx < 0 && px+incx+mapScrolling.x <= 0) {
			block = 1;
		} else {
			if (incx > 0) {
				map_tile_x = (px+pw+incx-diff_w)/TILESIZE;
			} else {
				map_tile_x = (px+incx+diff_w)/TILESIZE;
			}
			map_tile_y = (py+diff_h+6)/TILESIZE; // have a reducer to avoid being locked in the jump


			diff = (py-(incy+1)+ph)/TILESIZE -map_tile_y;

			//if (!is_player() && incy > 0) { cout << "character::map_colision - map_tile_x: " << map_tile_x << ", diff: " << diff << std::endl; }

			for (i=0; i<=diff; i++) {
				// DEAL WITH TERRAIN TYPE
				int pointLock = gameControl.getMapPointLock(st_position(map_tile_x, map_tile_y+i));
				int pointLock_incX = gameControl.getMapPointLock(st_position(map_tile_x+i, map_tile_y));

				//cout << "character::map_colision - map_tile_x: " << map_tile_x << ", map_tile_y: " << map_tile_y << ", map_tile_y+i: " << (map_tile_y+i) << ", pointLock: " << pointLock << "\n";

				if (pointLock > 0 && pointLock != TERRAIN_SCROLL_LOCK && pointLock != TERRAIN_DOOR && pointLock != TERRAIN_STAIR && pointLock != TERRAIN_WATER && pointLock != TERRAIN_SPIKE && pointLock != TERRAIN_CHECKPOINT) {
					//cout << ">>>>>>>>>>>>>> BLOCK 1\n";
					block = 1;
					break;
				} else if (is_player() && incx > 0 && pointLock == TERRAIN_DOOR) {
					short temp_direction = ANIM_DIRECTION_LEFT;
					if (incx > 0) {
						temp_direction = ANIM_DIRECTION_RIGHT;
					}
					gameControl.horizontal_screen_move(temp_direction, true, map_tile_x, map_tile_y);
					break;
				} else if (is_player() && incx < 0 && pointLock == TERRAIN_DOOR) {
					block = 1;
					break;
				} else if (is_player() && incx != 0 && pointLock == TERRAIN_SCROLL_LOCK) {
					short temp_direction = ANIM_DIRECTION_LEFT;
					if (incx > 0) {
						temp_direction = ANIM_DIRECTION_RIGHT;
					}
					gameControl.horizontal_screen_move(temp_direction, false, map_tile_x, map_tile_y);
					break;
				} else if (is_player() && pointLock_incX == TERRAIN_SPIKE) {
					damage(999);
				} else if (pointLock_incX == TERRAIN_WATER) {
					//printf(">> test_map_colision - WATER 1\n");
					block = BLOCK_WATER;
				} else if (is_player() && pointLock_incX == TERRAIN_CHECKPOINT) {
					checkpoint.x = position.x;
					checkpoint.y = position.y;
					checkpoint.map = map->get_number();
					checkpoint.map_scroll_x = map->getMapScrolling().x;
					//std::cout << "======= CHECKPOINT x: " << checkpoint.x << ", y: " << checkpoint.y << ", map: " << checkpoint.map << ", scroll_x: " << checkpoint.map_scroll_x << std::endl;
				}
			}
		}
	}

	if (incy != 0) {
		if (incy > 0 && py >= RES_H+TILESIZE*3) {
			// death because felt in a hole
			damage(999);
			// out of screen, do not try to find any tile type
		} else if (incy < 0 && py <= 0) {
			return block;
		} else {
			int old_map_y = (py+diff_h)/TILESIZE;
			if (incy > 0) {
				map_tile_y = (py-1+ph+incy)/TILESIZE;
			} else {
				map_tile_y = (py+diff_h+incy)/TILESIZE;
			}


			// move the x position a bit more inside of the char in order to prevent blocking by near/side walls
			if (state.direction == ANIM_DIRECTION_LEFT) {
				px += 4;
			} else {
				px -= 4;
			}
			//map_tile_x = (px+diff_w)/TILESIZE;
			diff = abs((px+pw-diff_w)/TILESIZE - map_tile_x);

			//if (is_player()) std::cout << "py: " << py << ", ph: " << ph << ", incy: " << incy << ", total: " << (py-1+ph+incy) << ", diff_h: " << diff_h << ", map_tile_y: " << map_tile_y << std::endl;


			//if (!is_player() && incy > 0) { cout << "character::map_colision - map_tile_x: " << map_tile_x << ", map_tile_y: " << map_tile_y << ", py: " << py << ", ph: " << ph << ", incy: " << incy << ", diff: " << diff << std::endl; }

			for (i=0; i<=diff; i++) {
				int pointLock_incX = gameControl.getMapPointLock(st_position(map_tile_x, map_tile_y));
				int old_map_lock = gameControl.getMapPointLock(st_position(map_tile_x, old_map_y));

				//if (!is_player() && incy > 0) { cout << "character::map_colision - i: " << i << ", pointLock_incX: " << pointLock_incX << std::endl; }

				//if (is_player()) cout << "character::map_colision::yCheck - diff_h: " << diff_h << ", map_tile_x: " << map_tile_x << ", map_tile_y: " << map_tile_y << ", map_tile_y+i: " << (map_tile_y+i) << ", pointLock: " << pointLock_incX << "\n";

				if (pointLock_incX != BLOCK_UNBLOCKED) {
					if (pointLock_incX != TERRAIN_WATER && pointLock_incX != TERRAIN_SPIKE && pointLock_incX != TERRAIN_CHECKPOINT && pointLock_incX != TERRAIN_STAIR) {
						// DEAL WITH TERRAIN TYPE

						// TODO - PLATFORM_COUNTER

						if (block == BLOCK_UNBLOCKED || block == BLOCK_WATER) { // free to move in the X axis
							if (pointLock_incX == TERRAIN_MOVE_LEFT && _moving_platform_timer < timer.getTimer()) {
								position.x -= move_speed-1;
								_moving_platform_timer = timer.getTimer()+MOVING_GROUND;
							} else if (pointLock_incX == TERRAIN_MOVE_RIGHT && _moving_platform_timer < timer.getTimer()) {
								position.x += move_speed-1;
								_moving_platform_timer = timer.getTimer()+MOVING_GROUND;
							}
							block = 2;
						} else {
							block = 3;
						}
						break;
					} else if (incy > 0 && pointLock_incX == TERRAIN_STAIR && old_map_lock != TERRAIN_STAIR) {
						block = 2;
					} else if (pointLock_incX == TERRAIN_SPIKE) {
						damage(999);
					} else if (pointLock_incX == TERRAIN_WATER) {
						if (block == 0) {
							block = BLOCK_WATER;
						}
					/*
					} else if (pointLock_incX == TERRAIN_STAIR && incy > 0 && moveCommands.down == 1) {
						if (gameControl.getMapPointLock(st_position(map_tile_x+i, map_tile_y-1)) != TERRAIN_STAIR) {
							//block = 4;
							//break;
						}
					*/
					}
				}
			}
		}
	}
	//if (is_player()) std::cout << "character::map_colision - block: " << block << std::endl;
	return block;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void character::addSpriteFrame(int anim_direction, int anim_type, int posX, int posY, graphicsLib_gSurface &spritesSurface, int delay)
{
	struct st_rectangle spriteArea;

	spriteArea.x = posX*frameSize.width;
	spriteArea.y = posY*frameSize.heigth;
	spriteArea.w = frameSize.width;
	spriteArea.h = frameSize.heigth;

	for (int i=0; i<ANIM_FRAMES_COUNT; i++) { // find the last free frame
		if ((character_graphics_list.find(name)->second)[anim_direction][anim_type][i].frameSurface.gSurface == NULL) {
			//if (name == "Giant Dolphin") cout << "character::addSpriteFrame - adding frame[" << anim_direction << "][" << anim_type << "][" << i << "]" << std::endl;
			(character_graphics_list.find(name)->second)[anim_direction][anim_type][i].setSurface(graphLib.surfaceFromRegion(spriteArea, spritesSurface));
			(character_graphics_list.find(name)->second)[anim_direction][anim_type][i].delay = delay;
			break;
		}
	}
}



// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void character::set_map(classMap *set_map)
{
	map = set_map;
	//st_position new_map_scroll = set_map->getMapScrolling();
	//std::cout << "////////////// CHARACTER - new_map_scroll.x: " << new_map_scroll.x << std::endl;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void character::set_is_player(bool set_player)
{
	is_player_type = set_player;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
bool character::is_player()
{
	return is_player_type;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
string character::getName(void)
{
	return name;
}

// ********************************************************************************************** //
// Returns true of character is over a staircase                                                  //
// ********************************************************************************************** //
st_position character::is_on_stairs(st_rectangle pos)
{
	int map_tile_x, map_tile_y, diff_w;

	diff_w = (pos.w-TILESIZE)*0.5;
	map_tile_x = (pos.x+diff_w)/TILESIZE;
	map_tile_y = (pos.y)/TILESIZE;

	if (map->getMapPointLock(st_position(map_tile_x, map_tile_y)) == TERRAIN_STAIR) {
		//std::cout << "is_on_stairs - TRUE 1" << std::endl;
		return st_position(map_tile_x, map_tile_y);
	}
	map_tile_x = (pos.x+pos.w-diff_w)/TILESIZE;

	if (map->getMapPointLock(st_position(map_tile_x, map_tile_y)) == TERRAIN_STAIR) {
		//std::cout << "is_on_stairs - TRUE 2" << std::endl;
		return st_position(map_tile_x, map_tile_y);
	}
	return st_position(-1, -1);
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
st_size character::get_size()
{
	return frameSize;
}


// ********************************************************************************************** //
// adds an entry into character_graphics_list map, if needed                                      //
// ********************************************************************************************** //
void character::add_graphic()
{
	std::map<std::string, st_spriteFrame[ANIM_DIRECTION_COUNT][ANIM_TYPE_COUNT][ANIM_FRAMES_COUNT]>::iterator it;
	st_spriteFrame temp_sprites[ANIM_DIRECTION_COUNT][ANIM_TYPE_COUNT][ANIM_FRAMES_COUNT];
	const std::string temp_name(name);

	for (int i=0; i<ANIM_DIRECTION_COUNT; i++) {
		for (int j=0; j<ANIM_TYPE_COUNT; j++) {
			for (int k=0; k<ANIM_FRAMES_COUNT; k++) {
				temp_sprites[i][j][k].frameSurface.gSurface = NULL;
			}
		}
	}


	it = character_graphics_list.find(name);
	if (it == character_graphics_list.end()) { // there is no graphic with this key yet, add it
		character_graphics_list.insert(pair<std::string, st_spriteFrame[ANIM_DIRECTION_COUNT][ANIM_TYPE_COUNT][ANIM_FRAMES_COUNT]>(temp_name, temp_sprites));
		//std::cout << "2. character::add_graphic - adding graphic entry for character '" << name << "', list.size: " << character_graphics_list.size() << std::endl;
	}
}

bool character::have_frame_graphic(int direction, int type, int pos)
{
	if ((character_graphics_list.find(name)->second)[direction][type][pos].frameSurface.width == 0 || (character_graphics_list.find(name)->second)[direction][type][pos].frameSurface.gSurface == NULL) {
		return false;
	}
	return true;
}

bool character::is_in_stairs_frame()
{
	if (state.animation_type == ANIM_TYPE_STAIRS || state.animation_type == ANIM_TYPE_STAIRS_MOVE || state.animation_type == ANIM_TYPE_STAIRS_SEMI || state.animation_type == ANIM_TYPE_STAIRS_ATTACK) {
		return true;
	}
	return false;
}

void character::recharge(e_energy_types _en_type, int value)
{
	//std::cout << "character::recharge";
	if (_en_type == ENERGY_TYPE_HP) {
		if (hitPoints.current < PLAYER_INITIAL_HP) {
			if (hitPoints.current + value <= PLAYER_INITIAL_HP) {
				hitPoints.current += value;
			} else {
				hitPoints.current = PLAYER_INITIAL_HP;
			}

			if (value > ENERGY_ITEM_SMALL) {
				soundManager.play_repeated_sfx(SFX_GOT_ENERGY, 1);
			} else {
				soundManager.play_sfx(SFX_GOT_ENERGY);
			}
		}
	}
}



bool character::get_item(object_colision& obj_info)
{
	bool res = false;
	// deal with non-blocking items
	if (obj_info._object != NULL && obj_info._object->finished() == false) {
		//std::cout << "character::get_item" << std::endl;
		switch (obj_info._object->get_type()) {
		case OBJ_ENERGY_PILL_SMALL:
			std::cout << "character::get_item - OBJ_ENERGY_PILL_SMALL" << std::endl;
			recharge(ENERGY_TYPE_HP, ENERGY_ITEM_SMALL);
			res = true;
			obj_info._object->set_finished(true);
			break;
		case OBJ_ENERGY_PILL_BIG:
			std::cout << "character::get_item - OBJ_ENERGY_PILL_BIG" << std::endl;
			recharge(ENERGY_TYPE_HP, ENERGY_ITEM_BIG);
			res = true;
			obj_info._object->set_finished(true);
			break;
		default:
			//std::cout << "character::get_item - unknown item type: " << obj_info._object->get_type() << std::endl;
			break;
		}
//	} else {
//		std::cout << "character::get_item - empty object" << std::endl;
	}
	return res;
}

// ********************************************************************************************** //
// set the object platform player is over, if any                                                 //
// ********************************************************************************************** //
void character::set_platform(object* obj)
{
	if (obj != NULL) {
		if (state.animation_type == ANIM_TYPE_JUMP) {
			state.animation_type = ANIM_TYPE_STAND;
			state.jump_state = 0;
		} else if (state.animation_type == ANIM_TYPE_JUMP_ATTACK) {
			state.animation_type = ANIM_TYPE_ATTACK;
			state.jump_state = 0;
		}
	}
	std::cout << "******** character::set_platform: " << obj << std::endl;
	platform = obj;
}

object* character::get_platform()
{
	return platform;
}

int character::get_direction()
{
	return state.direction;
}

void character::set_direction(int direction)
{
	state.direction = direction;
}

void character::clean_projectiles()
{
	// empty projectile list

	while (!projectile_list.empty()) {
		delete projectile_list.front();
		projectile_list.erase(projectile_list.begin());
	}
}

void character::damage(unsigned int damage_points)
{
	if (hitPoints.current <= 0) { /// already dead
		return;
	}
	unsigned int now_timer = timer.getTimer();
	if (now_timer < hit_duration+last_hit_time) { /// is still intangible from last hit
		return;
	}
	last_hit_time = now_timer;
	hit_animation_timer = now_timer+HIT_BLINK_ANIMATION_LAPSE;
	hitPoints.current -= damage_points;
	//std::cout << "character::damage - damage_points: " << damage_points << ", hitPoints.current: " << hitPoints.current << std::endl;
    if (is_player() == true && state.animation_type != ANIM_TYPE_HIT) {
		state.animation_type = ANIM_TYPE_HIT;
        map->add_animation(ANIMATION_DYNAMIC, std::string("hit.png"), position, st_position(0, 5), 150, 5, state.direction, st_size(24, 24));
	}
	hit_moved_back_n = 0;
	// TODO: add hit animation
	if (hitPoints.current <= 0) {
		hitPoints.current = 0;
		death();
	}
}

bool character::is_dead()
{
	return (hitPoints.current <= 0);
}

unsigned short int character::get_current_hp()
{
	return hitPoints.current;
}

struct st_position character::get_real_position()
{
	return realPosition;
}

void character::execute_jump_up()
{
	// fall intil reaching ground
	/// @TODO
	for (int i=0; i<100; i++) {
		char_update_real_position();
		gravity();
		map->showMap();
		show();
		map->showAbove();
		graphLib.updateScreen();
	}

	// reset command jump, if any
	jump(0, map->getMapScrolling());
	int initial_y = position.y;
	jump(1, map->getMapScrolling());
    //std::cout << "execute_jump::START - " << initial_y << ", position.y: " << position.y << std::endl;
	while (position.y != initial_y) {
        //std::cout << "execute_jump::LOOP - " << initial_y << ", position.y: " << position.y << std::endl;
		char_update_real_position();
		bool resJump = jump(1, map->getMapScrolling());
		if (resJump == true) {
			state.move_timer = timer.getTimer() + TIME_MOVES;
		}
		if (state.jump_state == JUMP_DOWN) {
			jump(0, map->getMapScrolling());
			state.jump_state = NO_JUMP;
			return;
		}
		map->showMap();
		show();
		map->showAbove();
		graphLib.updateScreen();
		SDL_Delay(20); /// @TODO REMOVE
	}
}

void character::execute_jump()
{
	// fall intil reaching ground
	fall();

	// reset command jump, if any
	jump(0, map->getMapScrolling());
	int initial_y = position.y;
	jump(1, map->getMapScrolling());
	std::cout << "execute_jump::START - " << initial_y << ", position.y: " << position.y << std::endl;
	while (position.y != initial_y) {
        //std::cout << "execute_jump::LOOP - " << initial_y << ", position.y: " << position.y << std::endl;
		char_update_real_position();
		bool resJump = jump(1, map->getMapScrolling());
		if (resJump == true) {
			state.move_timer = timer.getTimer() + TIME_MOVES;
		} else {
			gravity();
		}
		map->showMap();
		show();
		map->showAbove();
		graphLib.updateScreen();
		SDL_Delay(20);
	}
}


void character::fall()
{
	/// @TODO
	for (int i=0; i<100; i++) {
		char_update_real_position();
		if (gravity() == true) {
			map->showMap();
			show();
			map->showAbove();
			graphLib.updateScreen();
			return;
		}
		map->showMap();
		show();
		map->showAbove();
		graphLib.updateScreen();
		SDL_Delay(20); /// @TODO REMOVE
	}
}

void character::teleport_out() {
	soundManager.play_sfx(SFX_TELEPORT);
	state.animation_type = ANIM_TYPE_TELEPORT;
	while (position.y > -(frameSize.heigth+TILESIZE)) {
        //std::cout << "teleport_out - position.y: " << position.y << std::endl;
		position.y -= move_speed;
		char_update_real_position();
		map->showMap();
		show();
		map->showAbove();
		graphLib.updateScreen();
		SDL_Delay(TIME_MOVES/2);
	}
}


void character::set_animation_type(int type)
{
	std::cout << "set_animation_type - type: " << type << std::endl;
    state.animation_type = type;
    state.animation_timer = 0;
	state.animation_state = 0;
}

bool character::change_position(short xinc, short yinc)
{

	short int mapLock = map_colision(xinc, yinc, map->getMapScrolling());
	if (mapLock != BLOCK_UNBLOCKED && mapLock != BLOCK_WATER) {
		return false;
	}
	position.x += xinc;
	position.y += yinc;
	return true;
}

bool character::test_change_position(short xinc, short yinc)
{
	if (yinc > 0 && position.y < 0) {
		return true;
	}
	short int mapLock = map_colision(xinc, yinc, map->getMapScrolling());
	//if (yinc == 0) { std::cout << "character::test_change_position - pos.x: " << position.x << ", pos.y: " << position.y << ", xinc: " << xinc << ", yinc: " << yinc << ", mapLock: " << mapLock << std::endl; }
	if (mapLock != BLOCK_UNBLOCKED && mapLock != BLOCK_WATER) {
		return false;
	}
	return true;
}

bool character::is_shielded(int projectile_direction)
{
	//std::cout << "character::is_shielded::START" << std::endl;
	if (is_player()) {
		std::cout << "character::is_shielded - shield_type: " << shield_type << ", input.p1_input[BTN_SHIELD]: " << input.p1_input[BTN_SHIELD] << std::endl;
		if (shield_type == SHIELD_FULL || (shield_type == SHIELD_FRONT && input.p1_input[BTN_SHIELD] == 1)) {
			return true;
		}
		return false;
	} else {
		//std::cout << ">> classnpc::is_shielded[" << name << "] - shield_type: " << shield_type << ", projectile_direction: " << projectile_direction << ", state.direction: " << state.direction << std::endl;
		if (shield_type == SHIELD_FULL || (shield_type == SHIELD_FRONT && projectile_direction != state.direction) || (shield_type == SHIELD_STAND && state.animation_type == ANIM_TYPE_STAND)) {
			//std::cout << ">> classnpc::is_shielded[" << name << "] - TRUE" << std::endl;
			return true;
		}
		//std::cout << ">> classnpc::is_shielded[" << name << "] - FALSE" << std::endl;
		return false;
	}
}

void character::set_anim_type(short int anim_type)
{
	state.animation_state = 0;
	state.animation_timer = 0;
	state.animation_type = anim_type;
}

short character::get_anim_type()
{
	return state.animation_type;
}

graphicsLib_gSurface *character::get_char_frame(int direction, int type, int frame)
{
	if (character_graphics_list.find(name) == character_graphics_list.end()) {
		return NULL;
	} else {
		return &(character_graphics_list.find(name)->second)[direction][type][frame].frameSurface;
	}
}

st_color character::get_color_key(short key_n)
{
	return color_keys[key_n];
}

short character::get_number()
{
	return _number;
}

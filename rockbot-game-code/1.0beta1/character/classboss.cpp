#include "classboss.h"

#include "classmap.h"

#include "timerlib.h"
extern timerLib timer;

#include "scenes/dialogs.h"

#include "game.h"
extern game gameControl;

extern format_v_2_0_1::file_game game_data;

classboss::classboss(std::string set_name) : _initialized(0), _screen_blinked(false)
{
	name = set_name;
    this->set_is_boss(true);
	initial_position.y = -200;
	position.y = initial_position.y;
	//std::cout << "classboss::classboss [" << name << "] - is_boss: " << _is_boss << std::endl;
}

classboss::classboss(int stage_id, int map_id, int main_id, int id) : classnpc(stage_id, map_id, main_id, id)
{
	this->set_is_boss(true);
	initial_position.y = -50;
	position.y = initial_position.y;
	//std::cout << "BOSS 1 - stage_id: " << stage_id << ", map_id: " << map_id << ", main_id: " << main_id << ", id: " << id << std::endl;
}


void classboss::boss_move() {
	if (state.move_timer > timer.getTimer()) {
		return;
	}

	if (_initialized == 0) { /// @TODO: move this logic to map (player should not move while boss is presenting)
		_initialized++;
		gameControl.boss_intro();
		return;
	} else if (_initialized == 1) {
		if (position.x > RES_H/2 && gravity(true) == false) {
			_initialized++;
		}
		state.move_timer = timer.getTimer()+100;
		return;
	}


	//std::cout << "classboss::move [" << name << "]" << std::endl;
	/*
	int mapScrollX = map->getMapScrolling().x;
	int lock_point = map->getMapPointLock(st_position(position.x/16, position.y/16));
	struct struct_player_dist dist_players = dist_npc_players();
	*/

	if (first_run == 0) {
		// show boss dialogs here
		first_run = 1;
		//boss_energy = temp_npc;
		//play_boss_music();
		//show_dialog(STAGE_N, 1);
	}
	if (name == "Daisie Bot") {
		exec_daisiebot();
		//npc_gravity(temp_npc);
		return;
	} else if (name == "Ape Bot") {
		//exec_IA_Apebot(temp_npc);
        //exec_daisiebot();
		return;
	} else {
		gravity(false);
		return;
	}
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classboss::execute()
{
	if (hitPoints.current <= 0 || position.x < map->getMapScrolling().x-TILESIZE*2 || position.x > map->getMapScrolling().x+RES_W+TILESIZE*2) {
		return;
	}
	//std::cout << "classboss::execute" << std::endl;
	boss_move();
	charMove();
}


void classboss::exec_daisiebot()
{
	unsigned short int shot_type = IA_STATE_SHOT_RAINBOW;

	std::cout << "classboss::exec_daisiebot" << std::endl;

	if (IA_state == IA_STATE_WAIT_TIME) {
		IA_state = IA_STAND;
	}

	// stand -> shot rainbow -> cross screen -> use flowers
	if (IA_state == IA_STAND) {
		IA_state = IA_STATE_CROSS_SCREEN;
		state.animation_type = ANIM_TYPE_WALK;
		return;
	}

	if (IA_state == IA_STATE_SHOT_RAINBOW) {
		state.animation_type = ANIM_TYPE_STAND;
		/// @TODO (use attacks/weapons system) shot_rainbow();
		IA_state = IA_STATE_CROSS_SCREEN;

		initial_position.x = position.x;
		initial_position.y = position.y;
		IA_state = IA_STATE_WAIT_TIME;
		state.move_timer = timer.getTimer()+300;
	}

	if (IA_state == IA_STATE_SHOT_FLOWER) {
		state.animation_type = ANIM_TYPE_STAND;
		/// @TODO (use attacks/weapons system) shot_flower(temp_npc);
		/// temp_npc->projectile.timer = getTimer()+60;
		initial_position.x = position.x;
		initial_position.y = position.y;
		IA_state = IA_STATE_WAIT_TIME;
		state.move_timer = timer.getTimer()+3000;
	}

	// TODO: move to generic npc, once more will use it
	if (IA_state == IA_STATE_CROSS_SCREEN) {
		// check if must stop/invert
		state.move_timer = timer.getTimer() + 10;
		if (state.direction == ANIM_DIRECTION_LEFT) {
			int map_tile_x = (position.x-move_speed)/16;
			int map_tile_y = position.y/16;
			if (distance.width < 18*TILESIZE && (position.x-move_speed > 0 && (map->getMapPointLock(st_position(map_tile_x, map_tile_y)) == 0 || map->getMapPointLock(st_position(map_tile_x, map_tile_y)) == TERRAIN_WATER))) {
				position.x-= move_speed;
				distance.width += move_speed;
			} else {
				distance.width = 0;
				invert_direction();
				if (shot_type == IA_STATE_SHOT_RAINBOW) {
					shot_type = IA_STATE_SHOT_FLOWER;
				} else {
					shot_type = IA_STATE_SHOT_RAINBOW;
				}
				IA_state = shot_type;
			}
		} else { // ANIM_RIGHT
			int map_tile_x = (position.x+frameSize.width+move_speed)/16;
			int map_tile_y = position.y/16;
			//map->getMapPointLock(st_position(map_tile_x, map_tile_y))
			if (distance.width < 18*TILESIZE && (map->getMapPointLock(st_position(map_tile_x, map_tile_y)) == 0 || map->getMapPointLock(st_position(map_tile_x, map_tile_y)) == TERRAIN_WATER)) {
				position.x+= move_speed;
				distance.width += move_speed;
			} else {
				distance.width = 0;
				invert_direction();
				IA_state = shot_type;
			}
		}
	}

}

bool classboss::is_boss()
{
    return true;
}

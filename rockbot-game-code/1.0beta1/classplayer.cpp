#include <sstream>
#include <list>

#include "classplayer.h"
#include "inputlib.h"
extern inputLib input;
#include "class_config.h"
extern class_config config_manager;

#include "soundlib.h"
extern soundLib soundManager;

#include "game.h"
extern game gameControl;

extern format_v_2_0_1::file_game game_data;

#include "classmap.h"

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
classPlayer::classPlayer(std::string set_name, int playerNumber) : teleporter_n(-1), selected_weapon(WEAPON_DEFAULT), l_key_released(true), r_key_released(true)
{
	_number = playerNumber-1;
	max_projectiles = game_data.players[_number].max_shots;
	position.y = -TILESIZE;
	position.x = 80;
	hit_duration = 2500;
	hitPoints.total = game_data.players[_number].HP;
	hitPoints.current = hitPoints.total;
	name = set_name;
	add_graphic();
	init_weapon_colors();
	initFrames();
	shield_type = SHIELD_FRONT; /// @TODO: from editor
	_desert_scorpion_attack_n = -1;
}


void classPlayer::init_weapon_colors()
{
	for (int i=0; i<3; i++) {
		color_keys[i] = game_data.players[_number].color_keys[i];
	}
	for (int i=0; i<MAX_WEAPON_N; i++) {
		weapon_colors[i] = game_data.players[_number].weapon_colors[i];
	}
}

bool classPlayer::get_item(object_colision &obj_info)
{
	if (character::get_item(obj_info)) {
		return true;
	}

	bool res = false;
	// deal with non-blocking items
	if (obj_info._object != NULL && obj_info._object->finished() == false) {
		//std::cout << "classPlayer::get_item" << std::endl;
		switch (obj_info._object->get_type()) {
		case OBJ_ENERGY_TANK:
			std::cout << "classPlayer::get_item::OBJ_ENERGY_TANK - PRE.items.energy_tanks: " << items.energy_tanks << std::endl;
			items.energy_tanks++;
			std::cout << "classPlayer::get_item::OBJ_ENERGY_TANK - POS.items.energy_tanks: " << items.energy_tanks << std::endl;
			obj_info._object->set_finished(true);
			soundManager.play_sfx(SFX_GOT_ITEM);
			break;
		case OBJ_WEAPON_TANK:
			std::cout << "classPlayer::get_item::OBJ_WEAPON_TANK - PRE.items.weapon_tanks: " << items.weapon_tanks << std::endl;
			items.weapon_tanks++;
			std::cout << "classPlayer::get_item::OBJ_WEAPON_TANK - POS.items.weapon_tanks: " << items.weapon_tanks << std::endl;
			obj_info._object->set_finished(true);
			soundManager.play_sfx(SFX_GOT_ITEM);
			break;
		case OBJ_LIFE:
			items.lifes++;
			obj_info._object->set_finished(true);
			soundManager.play_sfx(SFX_GOT_ITEM);
			break;
		case OBJ_WEAPON_PILL_BIG:
			obj_info._object->set_finished(true);
			recharge(ENERGY_TYPE_WEAPON, ENERGY_ITEM_BIG);
			break;
		case OBJ_WEAPON_PILL_SMALL:
			obj_info._object->set_finished(true);
			recharge(ENERGY_TYPE_WEAPON, ENERGY_ITEM_SMALL);
			break;
		default:
			//std::cout << "classPlayer::get_item - unknown item type: " << obj_info._object->get_type() << std::endl;
			break;
		}
	}
	return res;
}

void classPlayer::recharge(e_energy_types _en_type, int value)
{
	if (_en_type == ENERGY_TYPE_HP) {
		character::recharge(_en_type, value);
	} else if (_en_type == ENERGY_TYPE_WEAPON) {
		if (items.weapons[selected_weapon] < PLAYER_INITIAL_HP) {
			if (items.weapons[selected_weapon] + value <= PLAYER_INITIAL_HP) {
				items.weapons[selected_weapon] += value;
			} else {
				items.weapons[selected_weapon] = PLAYER_INITIAL_HP;
			}
			soundManager.play_sfx(SFX_GOT_ENERGY);
			if (value > ENERGY_ITEM_SMALL) {
				soundManager.play_sfx(SFX_GOT_ENERGY);
			}
		}
	}
}

void classPlayer::attack()
{
	st_position proj_pos;
	short int weapon_id = 0;

	if (selected_weapon == WEAPON_DEFAULT) {
		character::attack();
		return;
	} else if (items.weapons[selected_weapon] <= 0) {
		return;
	}

	if (moveCommands.attack == 0 && attack_button_released == false) {
		attack_button_released = true;
		return;
	}

	if (max_projectiles <= projectile_list.size()) {
		std::cout << "attack - projectile limit reached - max_projectiles: " << max_projectiles << ", projectile_list.size(): " << projectile_list.size() << std::endl;
		return;
	}

	if (moveCommands.attack != 0 && (timer.getTimer()-state.attack_timer) > 100 && attack_button_released == true) {
		attack_button_released = false;
		switch (selected_weapon) {
		/// @TODO: must code from game_data, this should not be needed
			case WEAPON_APEBOT:
				weapon_id = 1;
				break;
			case WEAPON_DAISIEBOT:
				weapon_id = 2;
				break;
			case WEAPON_DYNAMITEBOT:
				weapon_id = 3;
				break;
			case WEAPON_MUMMYBOT:
				weapon_id = 4;
				if (_desert_scorpion_attack_n == -1) {
					set_scorpion_weapon_n();
				}
				map->spawn_map_npc(_desert_scorpion_attack_n, position, state.direction);
				return;
			case WEAPON_SPIKEBOT:
				weapon_id = 5;
				break;
			case WEAPON_TECHNOBOT:
				weapon_id = 6;
				break;
			case WEAPON_MAGEBOT:
				weapon_id = 7;
				break;
			case WEAPON_SEAHORSEBOT:
				weapon_id = 8;
				break;
			// items
			case WEAPON_ITEM_COIL:
				break;
			case WEAPON_ITEM_JET:
				break;
			default:
				break;
		}
		if (weapon_id == 0) { /// @TODO - this is a temporary exit to handle incomplete weapons
			return;
		}
		if (state.direction == ANIM_DIRECTION_LEFT) {
			proj_pos = st_position(position.x, position.y+frameSize.heigth/2);
		} else {
			proj_pos = st_position(position.x+frameSize.width, position.y+frameSize.heigth/2);
		}
		std::cout << "classPlayer::attack - weapon_id: " << weapon_id << ", game_data.weapons[weapon_id].id_projectile: " << game_data.weapons[weapon_id].id_projectile << std::endl;
		projectile_list.push_back(new projectile(game_data.weapons[weapon_id].id_projectile, state.direction, 8, proj_pos, map, game_data.weapons[weapon_id].damage, game_data.projectiles[game_data.weapons[weapon_id].id_projectile].projectile_type));

		if (selected_weapon != WEAPON_DEFAULT) {
			items.weapons[selected_weapon]--;
		}


		attack_state = ATTACK_START;
		state.attack_timer = timer.getTimer();
		if (state.animation_type == ANIM_TYPE_STAND) {
			state.animation_type = ANIM_TYPE_ATTACK;
		} else if (state.animation_type == ANIM_TYPE_JUMP) {
			state.animation_type = ANIM_TYPE_JUMP_ATTACK;
		} else if (state.animation_type == ANIM_TYPE_STAIRS || state.animation_type == ANIM_TYPE_STAIRS_SEMI || state.animation_type == ANIM_TYPE_STAIRS_MOVE) {
			state.animation_type = ANIM_TYPE_STAIRS_ATTACK;
		} else if (state.animation_type == ANIM_TYPE_WALK) {
			std::cout << "+++++++++++ PLAYER - set animation to ANIM_TYPE_WALK_ATTACK" << std::endl;
			state.animation_type = ANIM_TYPE_WALK_ATTACK;
		}
	}
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classPlayer::initFrames()
{
	frameSize.width = 29;
	frameSize.heigth = 32;

	graphicsLib_gSurface playerSpriteSurface;
	std::stringstream filename;
	filename << FILEPATH << "/data/images/sprites/p" << (_number+1) << ".png";
	graphLib.surfaceFromFile(filename.str(), &playerSpriteSurface);
	if (playerSpriteSurface.gSurface == NULL) {
		std::cout << "initFrames - Error loading player surface from file\n";
		return;
	}

	// STAND
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_STAND, 3, 0, playerSpriteSurface, 5000);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_STAND, 4, 0, playerSpriteSurface, 150);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_STAND, 3, 1, playerSpriteSurface, 5000);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_STAND, 4, 1, playerSpriteSurface, 150);
	// WALK
	short walk_frame_duration = 80;
	if (_number == 0) {
		walk_frame_duration = 120;
	}
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_WALK, 6, 0, playerSpriteSurface, walk_frame_duration);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_WALK, 7, 0, playerSpriteSurface, walk_frame_duration);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_WALK, 8, 0, playerSpriteSurface, walk_frame_duration);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_WALK, 6, 1, playerSpriteSurface, walk_frame_duration);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_WALK, 7, 1, playerSpriteSurface, walk_frame_duration);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_WALK, 8, 1, playerSpriteSurface, walk_frame_duration);
	// JUMP
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_JUMP, 9, 0, playerSpriteSurface, 150);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_JUMP, 9, 1, playerSpriteSurface, 150);
	// ATTACK
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_ATTACK, 11, 0, playerSpriteSurface, 150);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_ATTACK, 11, 1, playerSpriteSurface, 150);
	// ATTACK + JUMP
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_JUMP_ATTACK, 10, 0, playerSpriteSurface, 80);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_JUMP_ATTACK, 10, 1, playerSpriteSurface, 80);
	// ATTACK + WALK
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_WALK_ATTACK, 12, 0, playerSpriteSurface, 150);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_WALK_ATTACK, 13, 0, playerSpriteSurface, 150);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_WALK_ATTACK, 14, 0, playerSpriteSurface, 150);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_WALK_ATTACK, 12, 1, playerSpriteSurface, 150);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_WALK_ATTACK, 13, 1, playerSpriteSurface, 150);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_WALK_ATTACK, 14, 1, playerSpriteSurface, 150);
	// HIT
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_HIT, 15, 1, playerSpriteSurface, 150);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_HIT, 15, 0, playerSpriteSurface, 150);
	// TELEPORT
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_TELEPORT, 0, 0, playerSpriteSurface, 750);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_TELEPORT, 1, 0, playerSpriteSurface, 300);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_TELEPORT, 2, 0, playerSpriteSurface, 150);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_TELEPORT, 0, 1, playerSpriteSurface, 750);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_TELEPORT, 1, 1, playerSpriteSurface, 300);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_TELEPORT, 2, 1, playerSpriteSurface, 150);
	// STAIRS
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_STAIRS, 17, 0, playerSpriteSurface, 5000);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_STAIRS, 17, 1, playerSpriteSurface, 5000);
	// stairs semi
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_STAIRS_SEMI, 18, 0, playerSpriteSurface, 5000);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_STAIRS_SEMI, 18, 1, playerSpriteSurface, 5000);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_STAIRS_SEMI, 18, 0, playerSpriteSurface, 5000);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_STAIRS_SEMI, 18, 1, playerSpriteSurface, 5000);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_STAIRS, 17, 0, playerSpriteSurface, 5000);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_STAIRS, 17, 1, playerSpriteSurface, 5000);
	// stairs + move
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_STAIRS_MOVE, 17, 0, playerSpriteSurface, 200);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_STAIRS_MOVE, 17, 1, playerSpriteSurface, 200);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_STAIRS_MOVE, 17, 0, playerSpriteSurface, 200);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_STAIRS_MOVE, 17, 1, playerSpriteSurface, 200);
	// stairs + attack
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_STAIRS_ATTACK, 19, 1, playerSpriteSurface, 200);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_STAIRS_ATTACK, 19, 0, playerSpriteSurface, 200);

    // slide
    addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_SLIDE, 20, 1, playerSpriteSurface, 1000);
    addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_SLIDE, 20, 0, playerSpriteSurface, 1000);

	// shield
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_SHIELD, 22, 1, playerSpriteSurface, 100);
	addSpriteFrame(ANIM_DIRECTION_LEFT, ANIM_TYPE_SHIELD, 21, 1, playerSpriteSurface, 100);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_SHIELD, 22, 0, playerSpriteSurface, 100);
	addSpriteFrame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_SHIELD, 21, 0, playerSpriteSurface, 100);

	playerSpriteSurface.freeGraphic();
}


void classPlayer::execute()
{
	move();
	charMove();
}

void classPlayer::move()
{
	if (input.p1_input[BTN_DOWN] == 1) {
		moveCommands.down = 1;
	} else {
		moveCommands.down = 0;
	}
	if (input.p1_input[BTN_UP] == 1) {
		moveCommands.up = 1;
	} else {
		moveCommands.up = 0;
	}
	if (input.p1_input[BTN_LEFT] == 1) {
		moveCommands.left = 1;
	} else {
		moveCommands.left = 0;
	}
	if (input.p1_input[BTN_RIGHT] == 1) {
		moveCommands.right = 1;
	} else {
		moveCommands.right = 0;
	}
	if (input.p1_input[BTN_JUMP] == 1) {
		moveCommands.jump = 1;
	} else {
		moveCommands.jump = 0;
	}
	if (input.p1_input[BTN_ATTACK] == 1) {
		moveCommands.attack = 1;
	} else {
		moveCommands.attack = 0;
	}

	if (input.p1_input[BTN_L] != 1 && l_key_released == false) {
		l_key_released = true;
	}
	int wpn_max = WEAPON_COUNT;
	wpn_max--;
	if (input.p1_input[BTN_L] == 1 && l_key_released == true) {
		int selected_weapon_c = selected_weapon;
		selected_weapon_c--;
		if (selected_weapon_c < WEAPON_DEFAULT) {
			selected_weapon_c = wpn_max-1;
		}
		set_weapon((WEAPON_ICONS_ENUM)selected_weapon_c);
		l_key_released = false;
		std::cout << ">>> selected_weapon: " << selected_weapon << ", WEAPON_COUNT: " << WEAPON_COUNT << std::endl;
	}

	if (input.p1_input[BTN_R] != 1 && r_key_released == false) {
		r_key_released = true;
	}
	if (input.p1_input[BTN_R] == 1 && r_key_released == true) {
		int selected_weapon_c = selected_weapon;
		selected_weapon_c++;
		if (selected_weapon_c >= wpn_max) {
			selected_weapon_c = WEAPON_DEFAULT;
		}
		set_weapon((WEAPON_ICONS_ENUM)selected_weapon_c);
		r_key_released = false;
		std::cout << ">>> selected_weapon: " << selected_weapon << ", WEAPON_COUNT: " << WEAPON_COUNT << std::endl;
	}

	if (moveCommands.up == 0 && moveCommands.down == 0 && moveCommands.left == 0 && moveCommands.right == 0 && moveCommands.jump == 0 && moveCommands.attack == 0 && input.p1_input[BTN_SHIELD] == 1) {
		if (state.animation_type != ANIM_TYPE_SHIELD) {
			state.animation_type = ANIM_TYPE_SHIELD;
			state.animation_timer = 0;
			state.animation_state = 0;
		}
		return;
	} else if (state.animation_type == ANIM_TYPE_SHIELD) {
		state.animation_type = ANIM_TYPE_STAND;
	}


	// animate projectiles
	vector<projectile*>::iterator it;
	for (it=projectile_list.begin(); it<projectile_list.end(); it++) {
		if ((*it)->is_finished == true) {
			projectile_list.erase(it);
			break;
		}
		st_size moved = (*it)->move();
		/// @TODO projectiles that are tele-guided
		if ((*it)->get_move_type() == TRAJECTORY_CHAIN) {
			(*it)->set_y(position.y+frameSize.heigth/2);
		}
        (*it)->draw();
		if ((*it)->is_reflected == true) {
			continue;
		}
		// check colision agains enemies
		std::list<classnpc*>::iterator enemy_it;
		for (enemy_it=map->npc_list.begin(); enemy_it != map->npc_list.end(); enemy_it++) {
			if ((*it)->is_finished == true) {
				projectile_list.erase(it);
				break;
			}
			//classnpc* enemy = (*enemy_it);
			if ((*it)->check_colision(st_rectangle((*enemy_it)->getPosition().x, (*enemy_it)->getPosition().y, (*enemy_it)->get_size().width, (*enemy_it)->get_size().heigth), st_position(moved.width, moved.heigth)) == true) {
				if ((*enemy_it)->is_shielded((*it)->get_direction()) == true) {
					(*it)->reflect();
				} else {
					(*enemy_it)->damage((*it)->get_damage());
					if ((*it)->get_move_type() != TRAJECTORY_CHAIN) { /// @TODO non-destructable types
						(*it)->consume_projectile();
					}
				}
			}
		}
	}

}


void classPlayer::set_teleporter(int n)
{
	//std::cout << "******** classPlayer::set_teleporter - current: " << teleporter_n << ", new: " << n << std::endl;
	teleporter_n = n;
}

int classPlayer::get_teleporter()
{
	return teleporter_n;
}

void classPlayer::set_animation_type(int type)
{
	state.animation_type = type;
}


void classPlayer::teleport_stand()
{
	unsigned int waitTimer = timer.getTimer()+500;
	state.animation_state = ANIM_TYPE_TELEPORT;
	/*
	if (p2Obj) {
		p2Obj->sprite->anim_type = ANIM_TELEPORT;
	}
	*/
	soundManager.play_sfx(SFX_TELEPORT);
	while (waitTimer > timer.getTimer()) {
		graphLib.updateScreen();
		show();
		/*
		if (p2Obj) {
			show_sprite(p2Obj->sprite, game_screen);
		}
		*/
		//drawMap3rdLevel(game_screen);
		//updateScreen(game_screen);
		input.waitTime(20);
	}
}

void classPlayer::death()
{
	dead = true;
	state.jump_state = NO_JUMP;
	input.clean();
	gameControl.draw_explosion(realPosition.x, realPosition.y, false);
	//set_position(st_position(80, -TILESIZE));
}

void classPlayer::reset_hp()
{
	hitPoints.current = hitPoints.total;
}

format_v1_0::st_items* classPlayer::get_items()
{
	return &items;
}

void classPlayer::change_player_color()
{
	for (int i=0; i<ANIM_DIRECTION_COUNT; i++) {
		for (int j=0; j<ANIM_TYPE_COUNT; j++) {
			for (int k=0; k<ANIM_FRAMES_COUNT; k++) {
				if (weapon_colors[selected_weapon].color1.r != -1) {
					graphLib.change_surface_color(color_keys[0], weapon_colors[selected_weapon].color1, &(character_graphics_list.find(name)->second)[i][j][k].frameSurface);
				}
				if (weapon_colors[selected_weapon].color2.r != -1) {
					graphLib.change_surface_color(color_keys[1], weapon_colors[selected_weapon].color2, &(character_graphics_list.find(name)->second)[i][j][k].frameSurface);
				}
				if (weapon_colors[selected_weapon].color3.r != -1) {
					graphLib.change_surface_color(color_keys[2], weapon_colors[selected_weapon].color3, &(character_graphics_list.find(name)->second)[i][j][k].frameSurface);
				}
			}
		}
	}
	//static std::map<std::string, st_spriteFrame[ANIM_DIRECTION_COUNT][ANIM_TYPE_COUNT][ANIM_FRAMES_COUNT]> character_graphics_list;

}

void classPlayer::set_weapon(enum WEAPON_ICONS_ENUM weapon_n)
{
	selected_weapon = weapon_n;
	change_player_color();
}

void classPlayer::set_scorpion_weapon_n()
{
	for (int i=0; i<GAME_MAX_OBJS; i++) {
		std::cout << "classPlayer::set_scorpion_weapon_n - npc.name: " << std::string(game_data.game_npcs[i].name) << std::endl;
		if (std::string(game_data.game_npcs[i].name) == "Desert Scorpion") {
			_desert_scorpion_attack_n = i;
			return;
		}
	}
	_desert_scorpion_attack_n = 3; // in case none was found
}

short classPlayer::get_weapon_value(int weapon_n)
{
	if (weapon_n == 0) {
		return hitPoints.current;
	}
	return items.weapons[weapon_n];
}



format_v2_0::file_weapon_colors classPlayer::get_weapon_colors(short int weapon_n)
{
	return weapon_colors[weapon_n];
}

WEAPON_ICONS_ENUM classPlayer::get_selected_weapon()
{
	return selected_weapon;
}






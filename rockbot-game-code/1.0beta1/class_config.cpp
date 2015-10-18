#include "class_config.h"

#include "graphicslib.h"
extern graphicsLib graphLib;

#include "inputlib.h"
extern inputLib input;

#include "soundlib.h"
extern soundLib soundManager;


class_config::class_config() : ingame_menu_active(false)
{
	ingame_menu_pos.x = 0;
	ingame_menu_pos.y = 0;
}

void class_config::set_player_ref(classPlayer* set_player_ref)
{
	player_ref = set_player_ref;
	if (_player_surface.width == 0) {
		graphLib.initSurface(st_size(player_ref->get_char_frame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_ATTACK, 0)->width, player_ref->get_char_frame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_ATTACK, 0)->height), &_player_surface);
		graphLib.copyArea(st_position(0, 0), player_ref->get_char_frame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_ATTACK, 0), &_player_surface);
	}
}


// column1: normal, ape,    daisie, dynamite, mummy,     spike
// column2:         techno, mage,   seahorse, item coil, item jet
// column3: e-tank, w-tank *(must be changed, as currently are two rows)
void class_config::move_cursor(int x_inc, int y_inc) {
	// left/right: if position exists, just move. if not then go to first item in nexct column or stays in place
	bool moved = false;

	if (x_inc > 0) {
		if (ingame_menu_pos.x == 0) {
			ingame_menu_pos.x++;
		} else {
			ingame_menu_pos.x = 0;
		}
		moved = true;
	} else if (x_inc < 0) {
		if (ingame_menu_pos.x == 0) {
			ingame_menu_pos.x = 1;
		} else {
			ingame_menu_pos.x = 0;
		}
		moved = true;
	}
	if (y_inc > 0) {
		if (ingame_menu_pos.y < 5) {
			ingame_menu_pos.y++;
		} else {
			if (ingame_menu_pos.x == 0) {
				ingame_menu_pos.y = 0;
			} else {
				ingame_menu_pos.y = 1;
			}
		}
		moved = true;
	}
	if (y_inc < 0) {
		if ((ingame_menu_pos.x == 0 && ingame_menu_pos.y == 0) || (ingame_menu_pos.x == 1 && ingame_menu_pos.y == 1)) {
			ingame_menu_pos.y = 5;
		} else {
			ingame_menu_pos.y--;
		}
		moved = true;
	}
	if (moved == true) {
		input.waitTime(150);
		soundManager.play_sfx(SFX_CURSOR);
	}
}

void class_config::change_player_frame_color()
{
	std::cout << "class_config::change_player_frame_color" << std::endl;
	short int selected_weapon = convert_menu_pos_to_weapon_n(ingame_menu_pos);
	format_v2_0::file_weapon_colors colors = player_ref->get_weapon_colors(selected_weapon);

	if (colors.color1.r != -1) {
		graphLib.change_surface_color(player_ref->get_color_key(0), colors.color1, &_player_surface);
	}
	if (colors.color2.r != -1) {
		graphLib.change_surface_color(player_ref->get_color_key(1), colors.color2, &_player_surface);
	}
	if (colors.color3.r != -1) {
		graphLib.change_surface_color(player_ref->get_color_key(2), colors.color3, &_player_surface);
	}
	graphLib.copyArea(st_position(26, 150), &_player_surface, &graphLib.gameScreen);
}


bool class_config::execute_ingame_menu()
{
#define MENU_CHANGE_DELAY 100
	if (input.p1_input[BTN_START] == 1) {
		input.clean();
		input.waitTime(300);
		ingame_menu_active = !ingame_menu_active;
		if (ingame_menu_active) {
			graphLib.draw_weapon_menu_bg(player_ref->get_current_hp(), player_ref->get_items(), player_ref->get_char_frame(ANIM_DIRECTION_RIGHT, ANIM_TYPE_ATTACK, 0));
			graphLib.draw_weapon_icon(convert_menu_pos_to_weapon_n(ingame_menu_pos), ingame_menu_pos, true);
			graphLib.draw_weapon_cursor(ingame_menu_pos, player_ref->get_weapon_value(convert_menu_pos_to_weapon_n(ingame_menu_pos)), player_ref->get_number());
		} else {
			// change player color/weapon
			player_ref->set_weapon(convert_menu_pos_to_weapon_n(ingame_menu_pos));
		}
	}

	if (ingame_menu_active) {
		if (input.p1_input[BTN_UP] == 1) {
			graphLib.draw_weapon_cursor(ingame_menu_pos, player_ref->get_weapon_value(convert_menu_pos_to_weapon_n(ingame_menu_pos)), -1);
			graphLib.draw_weapon_icon(convert_menu_pos_to_weapon_n(ingame_menu_pos), ingame_menu_pos, false);
			move_cursor(0, -1);
			graphLib.draw_weapon_cursor(ingame_menu_pos, player_ref->get_weapon_value(convert_menu_pos_to_weapon_n(ingame_menu_pos)), player_ref->get_number());
			graphLib.draw_weapon_icon(convert_menu_pos_to_weapon_n(ingame_menu_pos), ingame_menu_pos, true);
			change_player_frame_color();
			input.waitTime(MENU_CHANGE_DELAY);
		}
		if (input.p1_input[BTN_DOWN] == 1) {
			graphLib.draw_weapon_cursor(ingame_menu_pos, player_ref->get_weapon_value(convert_menu_pos_to_weapon_n(ingame_menu_pos)), -1);
			graphLib.draw_weapon_icon(convert_menu_pos_to_weapon_n(ingame_menu_pos), ingame_menu_pos, false);
			move_cursor(0, 1);
			graphLib.draw_weapon_cursor(ingame_menu_pos, player_ref->get_weapon_value(convert_menu_pos_to_weapon_n(ingame_menu_pos)), player_ref->get_number());
			graphLib.draw_weapon_icon(convert_menu_pos_to_weapon_n(ingame_menu_pos), ingame_menu_pos, true);
			change_player_frame_color();
			input.waitTime(MENU_CHANGE_DELAY);
		}
		if (input.p1_input[BTN_LEFT] == 1) {
			graphLib.draw_weapon_cursor(ingame_menu_pos, player_ref->get_weapon_value(convert_menu_pos_to_weapon_n(ingame_menu_pos)), -1);
			graphLib.draw_weapon_icon(convert_menu_pos_to_weapon_n(ingame_menu_pos), ingame_menu_pos, false);
			move_cursor(-1, 0);
			graphLib.draw_weapon_cursor(ingame_menu_pos, player_ref->get_weapon_value(convert_menu_pos_to_weapon_n(ingame_menu_pos)), player_ref->get_number());
			graphLib.draw_weapon_icon(convert_menu_pos_to_weapon_n(ingame_menu_pos), ingame_menu_pos, true);
			change_player_frame_color();
			input.waitTime(MENU_CHANGE_DELAY);
		}
		if (input.p1_input[BTN_RIGHT] == 1) {
			graphLib.draw_weapon_cursor(ingame_menu_pos, player_ref->get_weapon_value(convert_menu_pos_to_weapon_n(ingame_menu_pos)), -1);
			graphLib.draw_weapon_icon(convert_menu_pos_to_weapon_n(ingame_menu_pos), ingame_menu_pos, false);
			move_cursor(1, 0);
			graphLib.draw_weapon_cursor(ingame_menu_pos, player_ref->get_weapon_value(convert_menu_pos_to_weapon_n(ingame_menu_pos)), player_ref->get_number());
			graphLib.draw_weapon_icon(convert_menu_pos_to_weapon_n(ingame_menu_pos), ingame_menu_pos, true);
			change_player_frame_color();
			input.waitTime(MENU_CHANGE_DELAY);
		}
	}
	return ingame_menu_active;
}

enum WEAPON_ICONS_ENUM class_config::convert_menu_pos_to_weapon_n(st_position menu_pos)
{
	if (menu_pos.x == 0 && menu_pos.y == 0) {
		return WEAPON_DEFAULT;
	}
	if (menu_pos.x == 0 && menu_pos.y == 1) {
		return WEAPON_APEBOT;
	}
	if (menu_pos.x == 0 && menu_pos.y == 2) {
		return WEAPON_DAISIEBOT;
	}
	if (menu_pos.x == 0 && menu_pos.y == 3) {
		return WEAPON_DYNAMITEBOT;
	}
	if (menu_pos.x == 0 && menu_pos.y == 4) {
		return WEAPON_MUMMYBOT;
	}
	if (menu_pos.x == 0 && menu_pos.y == 5) {
		return WEAPON_SPIKEBOT;
	}
	if (menu_pos.x == 1 && menu_pos.y == 1) {
		return WEAPON_TECHNOBOT;
	}
	if (menu_pos.x == 1 && menu_pos.y == 2) {
		return WEAPON_MAGEBOT;
	}
	if (menu_pos.x == 1 && menu_pos.y == 3) {
		return WEAPON_SEAHORSEBOT;
	}
	if (menu_pos.x == 1 && menu_pos.y == 4) {
		return WEAPON_ITEM_COIL;
	}
	if (menu_pos.x == 1 && menu_pos.y == 5) {
		return WEAPON_ITEM_JET;
	}
	return WEAPON_DEFAULT;
}

st_position class_config::convert_weapon_n_to_menu_pos(int wpn_n)
{
	st_position res_pos;
	switch (wpn_n) {
	case WEAPON_DEFAULT:
		res_pos.x = 0;
		res_pos.y = 0;
		break;
	case WEAPON_APEBOT:
		res_pos.x = 0;
		res_pos.y = 1;
		break;
	case WEAPON_DAISIEBOT:
		res_pos.x = 0;
		res_pos.y = 2;
		break;
	case WEAPON_DYNAMITEBOT:
		res_pos.x = 0;
		res_pos.y = 3;
		break;
	case WEAPON_MUMMYBOT:
		res_pos.x = 0;
		res_pos.y = 4;
		break;
	case WEAPON_SPIKEBOT:
		res_pos.x = 0;
		res_pos.y = 5;
		break;
	case WEAPON_TECHNOBOT:
		res_pos.x = 1;
		res_pos.y = 1;
		break;
	case WEAPON_MAGEBOT:
		res_pos.x = 1;
		res_pos.y = 2;
		break;
	case WEAPON_SEAHORSEBOT:
		res_pos.x = 1;
		res_pos.y = 3;
		break;
	case WEAPON_ITEM_COIL:
		res_pos.x = 1;
		res_pos.y = 4;
		break;
	case WEAPON_ITEM_JET:
		res_pos.x = 1;
		res_pos.y = 5;
		break;
	default:
		break;
	}
	return res_pos;
}

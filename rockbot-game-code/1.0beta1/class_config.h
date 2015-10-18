#ifndef CLASS_CONFIG_H
#define CLASS_CONFIG_H

#include "st_common.h"
#include "classplayer.h"

// ********************************************************************************************** //
// This class handles all configuration menus and in-game weapon and item selection screen        //
// ********************************************************************************************** //
class class_config
{
public:
	class_config();
	bool execute_ingame_menu();
	void set_player_ref(classPlayer* set_player_ref);
	enum WEAPON_ICONS_ENUM convert_menu_pos_to_weapon_n(st_position menu_pos);
	st_position convert_weapon_n_to_menu_pos(int wpn_n);

private:
	void move_cursor(int x_inc, int y_inc);
	void change_player_frame_color();

private:
	bool ingame_menu_active;
	st_position ingame_menu_pos;
	classPlayer* player_ref;
	graphicsLib_gSurface _player_surface;
};

#endif // CLASS_CONFIG_H

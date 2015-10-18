#ifndef CLASSPLAYER_H
#define CLASSPLAYER_H

#include "character.h"
#include "classnpc.h"

class classnpc; // advance declaration

class classPlayer : public character
{
public:
	classPlayer(std::string set_name, int player_n);
	void setNumber(int);
	void initFrames();
	void execute();
	void set_teleporter(int n);
	int get_teleporter();
	void set_animation_type(int type);
	void teleport_stand();
	void reset_hp();
	format_v1_0::st_items* get_items();
	void set_weapon(enum WEAPON_ICONS_ENUM weapon_n);
	void set_scorpion_weapon_n();
	short int get_weapon_value(int weapon_n);
	format_v2_0::file_weapon_colors get_weapon_colors(short int weapon_n);
	enum WEAPON_ICONS_ENUM get_selected_weapon();

private:
	void createSprites();
	void move();
	void death();
	void change_player_color();
	void init_weapon_colors();
	bool get_item(object_colision& obj_info);
	void recharge(e_energy_types _en_type, int value);
	void attack();


private:
	int teleporter_n;
	format_v1_0::st_items items;
	enum WEAPON_ICONS_ENUM selected_weapon;
	format_v2_0::file_weapon_colors weapon_colors[MAX_WEAPON_N];
	int _desert_scorpion_attack_n; /// @TODO - this must be defined in editor/file-format
	bool l_key_released;
	bool r_key_released;
	object* _platform;

};

#endif // CLASSPLAYER_H

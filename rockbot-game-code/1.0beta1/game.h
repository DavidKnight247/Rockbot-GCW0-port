#ifndef GAME_H
#define GAME_H

#include <map>
#include <vector>
#include <sstream>
#include <string>

class classPlayer;
class classnpc;


#include "classplayer.h"
#include "classnpc.h"
#include "sceneslib.h"
#include "stage.h"
#include "objects/object.h"
#include "class_config.h"
#include "scenes/dialogs.h"


class game
{
public:
    game();
	~game();
	void initGame();
	bool showIntro();
    void quick_load_game();
	void showGame(bool can_characters_move=true);
	int getMapPointLock(struct st_position);
	void framerate_limiter();
	st_position checkScrolling();
	void fps_count();
	void horizontal_transition_screen(int tileX, int tileY, bool is_door, character* playerObj);
	void horizontal_screen_move(short direction, bool is_door, short tileX, short tileY);
	void got_weapon(std::string);
	void draw_explosion(short int centerX, short int centerY, bool show_players);
	void draw_implosion(short int centerX, short int centerY);
	// PLAYER methods
	void show_player(short int player_n);
	void set_player_position(st_position pos, short int player_n);
	void change_player_position(short xinc, short yinc, short player_n);
	void set_player_anim_type(short int anim_type, short int player_n);
	st_position get_player_position(short int player_n);
	st_size get_player_size(short int player_n);
	void set_player_direction(short int direction, short int player_n);
	void player_fall(short int player_n);
	void boss_intro();
	object* get_player_platform();

private:
	void load_map_links();
	bool test_teleport(classPlayer *test_player);
	void remove_all_projectiles();
	void debug_draw_map_links();
	void start_stage();
	void restart_stage();
	void load_game_objects();
	std::string cent_number(int n);
	void transitionScreen(short int type, short int map_n, short int adjust_x, classPlayer *pObj);
	void set_current_map(int);
	short int get_current_map();

public:
	int currentStage;


private:
	stage* loaded_stage;
	std::vector<classPlayer> players;
	scenesLib scenes;
	unsigned int fps_timer;
	int fps_counter;
	std::stringstream fps_msg;
	struct format_v1_0::st_game_config config;
	std::vector<struct format_v1_0::st_map_link> map_link_list;
	st_position selected_stage;
	std::map<std::string, object> game_object_list;
	class_config config_manager;
	dialogs game_dialogs;
	// framerate timers
	int _t1;
	float _frame_duration;

};

#endif // GAME_H

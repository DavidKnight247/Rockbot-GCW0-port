#ifndef V_2_0_1_H
#define V_2_0_1_H

#include <fstream>
#include <iostream>
#include "v_2_0_0.h"

extern std::string FILEPATH;

namespace format_v_2_0_1 {

	struct st_map_background {
		short id;
		char filename[CHAR_FILENAME_SIZE];
		short adjust_y; // distance of the background from the screen top
		short speed; // 0 means will scroll in the same speed as foreground (so the bg is used only to ease map-creation)
		st_map_background()
		{
			id = -1;
			filename[0] = '\0';
			adjust_y = 0;
			speed = 0;
		}
	};

	struct file_map {
		short id;
		st_map_background backgrounds[2];
		st_color background_color;
		format_v2_0::map_tile tiles[MAP_W][MAP_H];		// map tiles
		format_v2_0::file_map_npc map_npcs[MAX_MAP_NPC_N];
		format_v2_0::file_map_object map_objects[MAX_MAP_NPC_N];
		file_map() {
			id = -1;
		}
	};

	struct file_stage {
		short id;
		char name[CHAR_NAME_SIZE];
		char bgmusic_filename[CHAR_FILENAME_SIZE];
		file_map maps[STAGE_MAX_MAPS];
		format_v2_0::file_link links[STAGE_MAX_LINKS];
		format_v2_0::file_boss boss;
		format_v2_0::file_dialog intro_dialog;
		format_v2_0::file_dialog boss_dialog;
		format_v2_0::file_colorcycle colorcycle;
		file_stage() {
			id = -1;
			boss = format_v2_0::file_boss();
			name[0] = '\0';
			bgmusic_filename[0] = '\0';
		}
	};


	struct file_game {
		float version;
		char name[CHAR_NAME_SIZE];
		format_v2_0::file_npc game_npcs[GAME_MAX_OBJS]; // 60 enemy types is more than good enought
		format_v2_0::file_object objects[GAME_MAX_OBJS];
		format_v2_0::file_projectile projectiles[MAX_FILE_PROJECTILES];
		format_v2_0::file_weapon weapons[MAX_WEAPON_N]; // 8 stage-bosses and 3 item-like
		format_v2_0::file_player players[MAX_FILE_PLAYERS]; // up to 5 different players the user can select from
		file_stage stages[MAX_STAGES];
		format_v2_0::file_artificial_inteligence ai_types[MAX_AI_TYPES];
		file_game() {
			/// *** hardcoded parts *** ///
			// game
			version = 0.91; // 0.9 means 1.0 beta, 0.91 means beta1
			sprintf(name, "%s", "Rockbot");
		}
	};



	// FOR SAVE
	struct st_items {
		unsigned short int energy_tanks;
		unsigned short int weapon_tanks;
		unsigned short int special_tanks;
		unsigned short int balancer;
		unsigned short int lifes;
		unsigned short int bolts;
		unsigned short int weapons[WEAPON_COUNT];
		st_items() {
			energy_tanks = 0;
			weapon_tanks = 0;
			special_tanks = 0;
			balancer = 0;
			lifes = 3;
			bolts = 0;
			for (int i=0; i<WEAPON_COUNT; i++) {
				weapons[i] = PLAYER_INITIAL_HP;
			}
		}
	};

	// TODO - use vectors instead of C/like fixed-size arrays
	struct st_save {
		short int finished_stages; // number of already finished stages, just to simplify calc
		short int stages[MAX_STAGES]; // 0 still open, 1 finished
		st_items items;
		st_save() {
			finished_stages = 0;
			for (int i=0; i<MAX_STAGES; i++) {
				//stages[i] = 0;
				stages[i] = 0; // DEBUG
			}
		}
	};

	struct st_checkpoint {
		int map;
		int x;
		int y;
		int map_scroll_x;
		st_checkpoint() {
			map = 0;
			x = 50;
			y = -1;
			map_scroll_x = 0;
		}
	};

}

#endif // V_2_0_1_H

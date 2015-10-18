#ifndef V_2_0_0_H
#define V_2_0_0_H

// new file format, a single file structured into limited-size arrays;
/// @NOTE: an id negative (-1) means the struct is empty/deleted
namespace format_v2_0 {



	enum ANIM_STATES { ANIM_STAND, ANIM_WALK, ANIM_JUMP, ANIM_ATTACK, ANIM_TELEPORT, ANIM_JUMP_ATTACK, ANIM_WALK_ATTACK,
					   ANIM_HIT, ANIM_STAIRS, ANIM_STAIRS_SEMI, ANIM_STAIRS_ATTACK, ANIM_STAIRS_MOVE, ANIM_TROW, ANIM_TURN,
					   ANIM_MOVE_SEMI, // like turn, the start of the movement
					   ANIM_WALK_AIR, // for those that can walk on air
					   ANIM_INTRO, // for bosses
					   ANIM_SPECIAL_ATTACK, // for bosses, charging weapon, etc
					   ANIM_STATES_N }; // this must be always the last one to be used as counter

	struct st_sprite_info { // stores information about each animation sprite
		unsigned int id;
		unsigned short duration;
		short state;
		short sprite_pos_x;
		st_sprite_info() {
			id = -1;
			duration = 0;
			state = 0;
			sprite_pos_x = 0;
		}
	};

	// ----------------------- START GAME SECTION ----------------------- //

	struct file_ai_group {
		short state; //enum IA_STATES state;
		enum IA_CONDITIONALS conditional; // controls if this state will be executed or not, kinda like an "IF"
		int parameter1;
		int parameter2;
		file_ai_group() {
			state = IA_STATE_SLEEP;
			conditional = IA_CONDITION_NO_CONDITION;
			parameter1 = -1;
			parameter2 = -1;
		}
	};

	struct file_artificial_inteligence {
		short id;
		char name[CHAR_NAME_SIZE];
		struct file_ai_group states[AI_MAX_STATES];
		file_artificial_inteligence() {
			id = -1;
			name[0] = '\0';
		}
	};


	struct file_npc { // DONE - Game
		short id;
		//unsigned int projectile_id;							// indicates the projectile ID (relation with file_projectile)
		short projectile_id[2]; // indicate the id of an attack the NCP can user
		char name[CHAR_NAME_SIZE];
		char graphic_filename[CHAR_FILENAME_SIZE];
		struct st_hit_points hp;
		short direction;              // defines how it behavies (kink of AI*)
		short speed;               // defines the distances it can see enemies
		short walk_range;                 // defines how long from the start point it can go
		short facing;                     // defines the side npc is facing before start moving (also used by LINEWALK behavior)
		struct st_position start_point;
		st_sprite_info frames[NPC_FRAME_NUMBER];		// holds information about the type of each frame
		st_size frame_size;
		bool can_shoot;
		short shield_type;
		short IA_type;										// IA types: no_move, wait, attack, zig-zag (loop), side to side (origin to direction)
		short death_spawn;									// if different than -1, the NPC will spawn the given npc ID after killed
		file_npc() {
			id = -1;
			name[0] = '\0';
			graphic_filename[0] = '\0';
			projectile_id[0] = -1;
			projectile_id[1] = -1;
			death_spawn = -1;
		}
	};

	struct file_object { // DONE - Game
		short id;
		char name[CHAR_NAME_SIZE];
		char graphic_filename[CHAR_FILENAME_SIZE];
		short type; ///@ TODO: use enum
		int timer;				// used as time for disapearing block
		short speed;				// used as speed for moving platform
		short limit;				// used as range (pixels) for moving platform, visible time for disapearing block
		short direction;			// used to check if moving away from oiginalpoint or moving to it
		short distance;
		st_size size;
		short frame;				// indicates what is the used frame
		file_object() {
			id = -1;
			name[0] = '\0';
			graphic_filename[0] = '\0';
		}
	};



	struct file_projectile { // DONE -> Game
		short id;
		char name[CHAR_NAME_SIZE];
		char graphic_filename[CHAR_FILENAME_SIZE];
		st_size size;
		bool is_destructible; // indicates if the projectile can be shot down, if true, uses HP
		unsigned short hp;
		enum PROJECTILE_TRAJECTORIES projectile_type;
		unsigned short max_shots; // number of maximum simultaneous projectiles, if 0, is infinite
		file_projectile() {
			id = -1;
			name[0] = '\0';
			graphic_filename[0] = '\0';
			is_destructible = false;
			hp = 1;
			size.width = 3;
			size.heigth = 2;
			projectile_type = TRAJECTORY_LINEAR;
			max_shots = 0;
		}
	};


	struct file_weapon_colors { // DONE -> Player
		short id_weapon;
		st_color color1;
		st_color color2;
		st_color color3;
		file_weapon_colors() {
			id_weapon = -1;
		}
	};

	struct file_weapon { // DONE - Game
		short id;
		unsigned short id_projectile;
		char name[CHAR_NAME_SIZE];
		unsigned int damage;
		file_weapon() {
			id = -1;
			name[0] = '\0';
			damage = 1;
		}
	};

	struct file_player { // DONE -> Game
		short id;
		char name[CHAR_NAME_SIZE];
		char graphic_filename[CHAR_FILENAME_SIZE];
		char face_filename[CHAR_FILENAME_SIZE];
		unsigned short HP;
		st_size sprite_size;				// size of sprite graphic
		st_rectangle sprite_hit_area;		// the area of the graphic where is used for hit/colision
		unsigned short move_speed;		// how many sprites move each step
		float jump_initial_speed;			// used to adjust jump speed/height
		float jump_gravity;						// used to adjust jump speed/height
		float fall_gravity;
		st_sprite_info sprites[MAX_PLAYER_SPRITES];
		file_weapon_colors weapon_colors[MAX_WEAPON_N];
		st_color color_keys[3];
		// habilities part
		bool have_shield;
		unsigned short max_shots;		// number of maximum simultaneous projectiles
		unsigned short simultaneous_shots; // number of projectiles shot at one button press
		bool double_jump;
		bool can_slide; // is false, dashes instead of sliding
		bool can_charge_shot;

		file_player() {
			id = -1;
			name[0] = '\0';
			graphic_filename[0] = '\0';
			face_filename[0] = '\0';
			double_jump = false;
			have_shield = true;
			can_slide = false;
			can_charge_shot = true;
		}
	};


	// ----------------------- START MAP SECTION ----------------------- //



	struct file_map_npc {
		short id_npc;
		struct st_position start_point;
		unsigned short direction;
		file_map_npc() {
			id_npc = -1;
		}
	};

	struct file_map_object {
		short id_object;
		struct st_position start_point;
		unsigned short direction;
		file_map_object() {
			id_object = -1;
		}
	};

	struct map_tile {
		short id;
		short locked;
		struct st_position tile1;                  // position of tile (x and y) in the graphic file (each tile is 16*16)
		struct st_position tile2;                  // second level tile. this can be used to create more complex backgrounds
		struct st_position tile3;                  // tird level, to be shown over player
		map_tile() {
			id = -1;
		}
	};

	struct file_map {
		short id;
		char bg1_filename[CHAR_FILENAME_SIZE];
		char bg2_filename[CHAR_FILENAME_SIZE];
		st_color bg_color;
		map_tile tiles[MAP_W][MAP_H];		// map tiles
		file_map_npc map_npcs[MAX_MAP_NPC_N];
		file_map_object map_objects[MAX_MAP_NPC_N];
		file_map() {
			id = -1;
			bg1_filename[0] = '\0';
			bg2_filename[0] = '\0';
		}
	};


	// ----------------------- END MAP SECTION ----------------------- //



	// ----------------------- START STAGE SECTION ----------------------- //


	#define COLORCYCLE_MAX_ITEMS 10
	enum COLOR_KEYS {
		COLOR_KEY_GREEN,
		COLOR_KEY_PURPLE,
		COLOR_KEY_CYAN,
		COLOR_KEY_LENGTH
	};
	struct file_colorcycle { // DONE - Stage
		unsigned short id;
		st_color color[COLORCYCLE_MAX_ITEMS][COLOR_KEY_LENGTH];
		short duration[COLORCYCLE_MAX_ITEMS][COLOR_KEY_LENGTH];
		file_colorcycle() {
			id = -1;
		}
	};


	struct file_boss { // DONE - Stage
		unsigned short id_npc;
		unsigned short id_weapon;
		char name[20];
		char face_graphics_filename[20];
		file_boss() {
			id_npc = -1;
			id_weapon = -1;
			name[0] = '\0';
			face_graphics_filename[0] = '\0';
		}
	};


	struct file_dialog { // DONE - Stage x2
		unsigned short id;
		char face_graphics_filename[20];
		bool top_side;
		char line1[3][DIALOG_LINE_LIMIT];
		char answer1[MAX_FILE_PLAYERS][3][DIALOG_LINE_LIMIT];
		char line2[3][DIALOG_LINE_LIMIT];
		char answer2[MAX_FILE_PLAYERS][3][DIALOG_LINE_LIMIT];

		file_dialog() {
			id = -1;
			top_side = true;
			face_graphics_filename[0] = '\0';
			for (int i=0; i<MAX_FILE_PLAYERS; i++) {
				for (int j=0; j<3; j++) {
					answer1[i][j][0] = '\0';
					answer2[i][j][0] = '\0';
				}
			}
			for (int j=0; j<3; j++) {
				line1[j][0] = '\0';
				line2[j][0] = '\0';
			}
		}
	};

	enum LINK_TYPE {
		HORIZONTAL,
		VERTICAL,
		TELEPORTER,
		LINK_TYPE_COUNT
	};
	struct file_link { // DONE - Stage
		short id_map_origin;
		short id_map_destiny;
		st_position pos_origin;
		st_position pos_destiny;
		unsigned int size;
		bool bidirecional;
		bool is_door;
		short type; /// @TODO: use enum LINK_TYPE when conversion is completed
		file_link() {
			id_map_origin = -1;
			id_map_destiny = -1;
		}
	};


	struct file_stage {
		short id;
		char name[CHAR_NAME_SIZE];
		char bgmusic_filename[CHAR_FILENAME_SIZE];
		file_map maps[STAGE_MAX_MAPS];
		file_link links[STAGE_MAX_LINKS];
		file_boss boss;
		file_dialog intro_dialog;
		file_dialog boss_dialog;
		file_colorcycle colorcycle;
		file_stage() {
			id = -1;
			boss = file_boss();
			name[0] = '\0';
			bgmusic_filename[0] = '\0';
		}
	};


	// ----------------------- END STAGE SECTION ----------------------- //

	struct file_game {
		int version;
		char name[CHAR_NAME_SIZE];
		file_npc game_npcs[GAME_MAX_OBJS]; // 60 enemy types is more than good enought
		file_object objects[GAME_MAX_OBJS];
		file_projectile projectiles[MAX_FILE_PROJECTILES];
		file_weapon weapons[MAX_WEAPON_N]; // 8 stage-bosses and 3 item-like
		file_player players[MAX_FILE_PLAYERS]; // up to 5 different players the user can select from
		file_stage stages[MAX_STAGES];
		file_artificial_inteligence ai_types[MAX_AI_TYPES];
		file_game() {
			/// *** hardcoded parts *** ///
			// game
			version = 1;
			sprintf(name, "%s", "Rockbot");
		}
	};
	// ----------------------- END GAME SECTION ----------------------- //




}


#endif // V_2_0_0_H

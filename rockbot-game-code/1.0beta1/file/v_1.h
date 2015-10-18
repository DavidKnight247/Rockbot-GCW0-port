#ifndef V_1_H
#define V_1_H

namespace format_v1_0 {

	// data that describes an NPC properties
	// TODO: change names to reflect new ones from classnpc.h
	struct st_file_npc {
		char name[50];
		struct st_hit_points hp;
		short int direction;              // defines how it behavies (kink of AI*)
		short int speed;               // defines the distances it can see enemies
		short int walk_range;                 // defines how long from the start point it can go
		short int facing;                     // defines the side npc is facing before start moving (also used by LINEWALK behavior)
		struct st_position start_point;
		char graphic_filename[50];						// graphic file used on it
		//int filesize;
		int frameset[NPC_FRAME_NUMBER];				// holds information about the type of each frame
		int frameset_time[NPC_FRAME_NUMBER];		// the number of milisseconds this frame leasts
		int framesize_w;
		int framesize_h;
		int can_shoot;
		int shield_type;
		unsigned short int is_boss;
		int IA_type;										// IA types: no_move, wait, attack, zig-zag (loop), side to side (origin to direction)
		int projectile_type_id;							// indicates the projectile ID (relation with file_projectile)
	};

	struct st_file_map_npc {
		unsigned short int id;
		struct st_position start_point;
		unsigned short int direction;
	};


	#define WEAPON_COUNT SKULLCASTLE4
	// FOR SAVE
	struct st_items {
		unsigned short int energy_tanks;
		unsigned short int weapon_tanks;
		unsigned short int special_tanks;
		unsigned short int balancer;
		unsigned short int lifes;
		unsigned short int bolts;
		unsigned short int weapons[WEAPON_COUNT]; // 0 position is not used, 5 is for skullcastle (not used too), 10 is for item coil and 11 for item jet
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


	#define DIALOG_MAX_STAGE 20
	// TODO - use vectors instead of C/like fixed-size arrays
	struct st_save {
		short int finished_stages; // number of already finished stages, just to simplify calc
		short int stages[DIALOG_MAX_STAGE]; // 0 still open, 1 finished
		st_items items;
		st_save() {
			finished_stages = 0;
			for (int i=0; i<DIALOG_MAX_STAGE; i++) {
				//stages[i] = 0;
				stages[i] = 0; // DEBUG
			}
		}
	};

	struct st_game_config {
		bool two_players;
		bool sound_enabled;
		bool video_fullscreen;
		short int video_filter;
		short int selected_player;
		st_game_config() {
			two_players = false;
			sound_enabled = true;
			video_fullscreen = false;
			video_filter = 0;
			selected_player = 2;
		}
	};


	struct st_map_link {
		short int stage;
		struct st_position link_pos;				// position in map of origin
		struct st_position link_dest;				// position in map of destiny
		short int map_origin;			// number of origin map
		short int map_dest;			// number of destiny map
		unsigned short int link_size;			// size of it, in tiles
		unsigned short int link_type;			// can be TELEPORTER, LINK_VERTICAL OR LINK_HORIZONTAL
		unsigned short int link_bidi;			// indicates if it is bidirecional
		unsigned short int is_door;				// if it is a door, it is not bidiand willmake door sound and show animation of door opening/closing
	};

	// TODO - must remove sprite and next from the file-struct
	struct st_object { // same as the file object, but also contain pointers to be list and surface for graphics
		char id[4]; // holds the number for the relative object file. eg '003' for 003.obj
		char name[20];
		char filename[50];						// graphic file used on it
		short int type;
		int timer;				// used as time for disapearing block
		int speed;				// used as speed for moving platform
		int limit;				// used as range (pixels) for moving platform, visible time for disapearing block
		int direction;			// used to check if moving away from oiginalpoint or moving to it
		int distance;
		int framesize_w;
		int framesize_h;
		int frame;				// indicates what is the used frame
		struct st_position start_point;
		struct st_position position;
		SDL_Surface *sprite;
		struct st_object *next;
		st_object()
		{
			sprite = NULL;
		}
	};

	struct st_file_object { // logic object. combination of data from the map object information and the object file
		char id[4]; // holds the number for the relative object file. eg '003' for 003.obj
		struct st_position start_point;
	};

	struct st_dialog {
		int face;
		bool top_side;
		std::string lines[3];
		st_dialog(int set_face, bool set_left, std::string line1, std::string line2, std::string line3)
		{
			face = set_face;
			top_side = set_left;
			lines[0] = line1;
			lines[1] = line2;
			lines[2] = line3;
		}
	};


	/// @TODO: revisar de acordo com novo sistema de colorcycle

	struct st_colorcycle_color {
		unsigned short int duration;
		short int r;
		short int g;
		short int b;
	};

	struct st_color {
		short int r;
		short int g;
		short int b;
	};

	#define COLORCYCLE_MAX_STEPS 5
	struct st_colorcycle_file {
		struct st_color original;
		struct st_colorcycle_color current_color[COLORCYCLE_MAX_STEPS];
	};

	struct st_file_stage {
		char stage_name[50];
		char boss_name[50];
		char boss_sprites_filename[255];
		char boss_face_filename[255];
		char bg_music_filename[255];
		struct st_colorcycle_file colorcycle[3];
	};

	struct map_tile {
		short int locked;
		struct st_position tile1;                  // position of tile (x and y) in the graphic file (each tile is 16*16)
		struct st_position tile2;                  // second level tile. this can be used to create more complex backgrounds
		struct st_position tile3;                  // tird level, to be shown over player
	};

	struct file_map_v1 {
		char filename[50];							// used tilesed
		struct map_tile tiles[MAP_W][MAP_H];		// map tiles
	};

	struct struct_zone {
		char bg_filename[100];					// background image filename
		struct st_file_stage stages[8];
	};

	#define LINKS_NUMBER 300
	struct file_game {
		char name[50];
		struct st_map_link map_links[LINKS_NUMBER];// links from the map to another one (two for each map is more than enought)
		struct struct_zone zones[3];			// stage select, castle 1 and castle 2
	};


	// data that describes a projectile properties
	struct st_file_projectile {
		char name[50];
		int trajectory_type;
		int is_destructible;
		int frame_w;
		int frame_h;
		char img_filename[255];
		int HP;
	};


	struct links_list {
		struct st_map_link link;
		struct links_list *next;
	};


	enum ANIM_STATES { ANIM_STAND, ANIM_WALK, ANIM_JUMP, ANIM_ATTACK, ANIM_TELEPORT, ANIM_JUMP_ATTACK, ANIM_WALK_ATTACK,
					   ANIM_HIT, ANIM_STAIRS, ANIM_STAIRS_SEMI, ANIM_STAIRS_ATTACK, ANIM_STAIRS_MOVE, ANIM_TROW, ANIM_TURN,
					   ANIM_MOVE_SEMI, // like turn, the start of the movement
					   ANIM_WALK_AIR, // for those that can walk on air
					   ANIM_INTRO, // for bosses
					   ANIM_SPECIAL_ATTACK, // for bosses, charging weapon, etc
					   ANIM_STATES_N }; // this must be always the last one to be used as counter


#define ANIM_N1 2
#define ANIM_N2 ANIM_STATES_N
#define ANIM_N3 10

	struct anim_state {
		short int move_type;
		short int move_status[ANIM_N3];
		short int move_frame[ANIM_N3];
	};

	struct anim_frame {
		SDL_Surface *frame;
		unsigned short int duration;
		unsigned int init;
	};



	struct sprite_struct {
		short int x;
		short int y;
		short int w;
		short int h;
		short int frame_pos[2];							// indicates what is the animation type and frame position
		short int frame_timer;							// timer is used to control the animation speed
		unsigned long int frame_animation[2];			// controls where in the animation process the sprite is.
														// First part indicates the status and second the position
														// (for example in jump status 0 is nothing, 1 is going up and position raises until reaches limit, then status changes to 2)
		// new animation variables
		short int direction;
		short int anim_type;
		short int anim_pos;
		long int move_timer;
		struct anim_state anim_status;
		struct anim_frame sprite_frames[ANIM_N1][ANIM_N2][ANIM_N3];
		short int visible;
	};

	struct npc_static_data_list {
		struct st_file_npc npc_data;
		struct sprite_struct *sprite_list;
		struct npc_static_data_list *next;
	};

	struct list_map_npc {
		short int id;
		struct st_position start_point;
		unsigned short int direction;
		struct list_map_npc *next;
	};
}

#endif // V_1_H

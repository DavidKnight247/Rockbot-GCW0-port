#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <string>
#include <map>

// local includes
#include "st_hitPoints.h"
#include "st_common.h"
#include "st_characterState.h"
#include "graphicslib.h"
#include "projectilelib.h"

extern graphicsLib graphLib;

class object; // forward declaration

enum ATTACK_STATES {
	ATTACK_NOT,
	ATTACK_START,
	ATTACK_WAIT
};

class classMap;		// advance declaration
struct object_colision;

struct st_spriteFrame {
	unsigned short int delay; // time in milisseconds this frame will be shown
	graphicsLib_gSurface frameSurface;
	st_spriteFrame() {
		frameSurface.gSurface = NULL;
		delay = 20;
	}
	void setDelay(int newDelay)
	{
		delay = newDelay;
	}
	void setSurface(graphicsLib_gSurface newSurface)
	{
		graphLib.initSurface(st_size(newSurface.width, newSurface.height), &frameSurface);
		graphLib.copyArea(st_position(0, 0), &newSurface, &frameSurface);
	}
};


struct st_characterMovements { // this instruction holds commands passed from player (joystick) and npc (AI) and is used to execute basic movements
	int up;
	int down;
	int left;
	int right;
	int attack;
	int jump;
	int start;
	st_characterMovements()
	{
		up = 0;
		down = 0;
		left = 0;
		right = 0;
		attack = 0;
		jump = 0;
		start = 0;
	}
};

class character
{

// methods
public:
	character();
	~character();
	void setName(std::string);
	std::string getName(void);
	void setHitPoints(st_hit_points);
	virtual void initFrames() = 0; // pure virtual
	virtual void execute() = 0; // pure virtual
	struct st_position getPosition();
	struct st_position get_real_position();
	void set_position(struct st_position);
	void addSpriteFrame(int, int, int, int, graphicsLib_gSurface&, int);
	void set_map(classMap *set_map);
	void set_is_player(bool set_player);
	bool is_player();
	void show();
	st_size get_size();
	void set_platform(object*);
	object* get_platform();
	int get_direction();
	void set_direction(int direction);
	classMap *map;										// reference to the map this npc is in
	void clean_projectiles();
	void char_update_real_position();
	void damage(unsigned int damage_points);
	bool is_dead();
	unsigned short int get_current_hp();
	void execute_jump();								// execute a complete jump
	void execute_jump_up();					// execute jump until reaches the maximum height
	void fall();								// falls until reaching ground or leaving screen /// @TODO
	void teleport_out();
    void set_animation_type(int type);
	bool change_position(short int xinc, short int yinc);
	bool test_change_position(short int xinc, short int yinc);
	bool is_shielded(int projectile_direction);
	void set_anim_type(short int anim_type);
	short get_anim_type();
	graphicsLib_gSurface* get_char_frame(int direction, int type, int frame);
	st_color get_color_key(short int key_n);
	short int get_number();
	void charMove();
	void clear_move_commands();


protected:
	bool gravity(bool boss_demo_mode);							// returns true if finished (reached ground)
	virtual void attack();
    void change_char_color(st_color key, st_color new_color);

    bool slide(int, int, st_position);
    bool jump(int, st_position);
	int rectangle_collision(st_rectangle, st_rectangle);
	short int map_colision(const short int incx, const short int incy, st_position mapScrolling);
	void add_graphic();
	virtual void death() = 0; // pure virtual, as death is different for player and NPC
	bool have_frame_graphic(int direction, int type, int pos);  // indicates if the given frame graphic exits
	bool is_in_stairs_frame(); // indicates if the character is on some of the STAIRS animation types
	virtual void recharge(e_energy_types _en_type, int value);
	virtual bool get_item(object_colision& obj_info);


// members
public:
	// projectile list
	std::vector<projectile*> projectile_list;
	//struct st_spriteFrame sprite_list[ANIM_DIRECTION_COUNT][ANIM_TYPE_COUNT][ANIM_FRAMES_COUNT]; // TODO - move to protected

protected:
	// members static that can be moved to use game_data
	std::string name;
	struct st_size frameSize;
	int max_jump; // maximum reach of the jump
	unsigned int max_projectiles; // maximum number of simultaneuous projectiles the character can shot
	int move_speed; // how many pixels the character moves by cycle (default value)

	// dynamic members
	st_hit_points hitPoints;
	unsigned int last_hit_time; /// time of last being hit
	unsigned int hit_duration; /// time the character is intangible after being hit (in milisseconds)
	unsigned int hit_animation_timer; /// used to control "blinking" effect when hit
	st_position position;
	st_position realPosition;
	unsigned int last_execute_time;
	struct st_characterMovements moveCommands;

	// characteristics members
	float accel_speed_y; // used for acceleration in jump/gravity
	float gravity_y; // used for acceleration in jump/gravity
	float max_speed; // used for acceleration in jump/gravity
	bool can_fly; // indicates if the character can fly in order to not be affected by gravity
	unsigned int attack_state; // indicates if the player is attacking and what is the stage of the attack
    bool attack_button_released; // indicates if attack button was released between shots
    int charging_color_n; // holds information used when changing char colors on charging shot
    unsigned int charging_color_timer; // holds information used when changing char colors on charging shot
    bool jump_button_released; // indicates if jump button was released between jumps
	st_position is_on_stairs(st_rectangle pos);		// check is character in over a staircase

	// control members
	st_characterState state;
	bool is_player_type; // used for some small tweaks


	// DEBUG
	int jump_steps;
	int hit_moved_back_n;
	// external members

	// TODO - graphics list map, used in order to avoid duplication of graphics
	static std::map<std::string, st_spriteFrame[ANIM_DIRECTION_COUNT][ANIM_TYPE_COUNT][ANIM_FRAMES_COUNT]> character_graphics_list;
	/// @TODO static std::map<std::string, graphicsLib_gSurface> _frames_surface;

	object* platform; // used to move player when object moves

	bool dead;

    st_color color_keys[3];

	short int shield_type; // same as for NPC, 0->no-shield, 1->full_shield, 2->front-shield

	int _moving_platform_timer; // used to control character movement when is over a moving platform

	int _number; // used for player or npc_id



};

#endif // CHARACTER_H

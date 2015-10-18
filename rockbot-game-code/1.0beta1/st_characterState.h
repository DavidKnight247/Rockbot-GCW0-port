#ifndef ST_CHARACTERSTATE_H
#define ST_CHARACTERSTATE_H

#include <vector>

#include "st_teleporter.h"
#include "st_projectile.h"
#include "st_platform.h"
#include "defines.h"

struct st_characterState {
	int hit_timer;
	int direction;
	std::vector<struct st_projectile> projectile_list;
	struct st_teleporter *last_used_teleporter;
	int jump_timer; // used to control time of jump
	int jump_mark; // holds the distance between jump origin and current position
	int jump_state; // holds the state of jump => 0: no jump, 1: jumping-up, 2: falling
	float jump_speed; // controls jump acceleration (1 pixels for 10 pixels moved)
	unsigned int attack_timer; // used to control time of attack
	unsigned int move_timer; // used to control cycles in order to limit speed
	struct st_platform *current_platform;
	int animation_type; // animation type as walk, jump, attack
	int animation_state; // current animation frame position
    unsigned int animation_timer; // used to control animation speed
    int slide_distance; // holds the amount of pixels character have slided
	// constructor
	st_characterState()
	{
		hit_timer = 0;
		direction = ANIM_DIRECTION_RIGHT;
		last_used_teleporter = NULL;
		jump_timer = 0;
		jump_mark = 0;
		jump_state = 0;
		current_platform = NULL;
		animation_type = ANIM_TYPE_STAND;
		animation_state = 0;
		animation_timer = 0;
		move_timer = 0;
		jump_speed = 1;
		attack_timer = 0;
        slide_distance = 0;
	}
};

#endif // ST_CHARACTERSTATE_H

#ifndef CLASSNPC_H
#define CLASSNPC_H

#include <string>
#include "character.h"
#include "file/format.h"


class classPlayer;




struct npc_frameset {
	int frameset;				// holds information about the type of this frame
	unsigned int frameset_time;		// the number of milisseconds this frame leasts
	struct st_size framesize;
};


struct struct_player_dist {
	int dist;
	character *pObj;
};


class classnpc : public character
{
public:
	classnpc();
	classnpc(std::string set_name);
	classnpc(int stage_id, int map_id, int main_id, int id); // load data from game_data and create a new npc
	classnpc(int stage_id, int map_id, int main_id, st_position npc_pos, short int direction); // spawned npc
	~classnpc();
	void initFrames();
	void execute();
	void set_file_data(format_v1_0::st_file_npc data);
	void set_file_data(format_v2_0::file_npc data);
	void copy(classnpc *from);
	void move();
	void move_projectiles();
	void set_player_list(std::vector<character*> *set_list);
	bool have_player_list();
    virtual bool is_boss();
	void set_is_boss(bool set_boss);
	bool is_player_friend();

protected:
	struct struct_player_dist dist_npc_players();
	void death();
	void invert_direction();
	void throw_projectile_npc(int type, int n, int alternated);
	// AI methods
	void exec_IA_ZIGZAG();
	void exec_IA_GHOST();
	void exec_IA_FISH();
	void exec_IA_DOLPHIN();
	void exec_IA_SIDETOSIDE();
	void exec_IA_FLOATER(bool can_cross_walls);
	void build_basic_npc(int stage_id, int map_id, int main_id);

protected:
	bool _is_boss;
	// W A R N I N G -----------------> new members must be reflected in copy() method
	short int facing;									// defines the side npc is facing before start moving (also used by LINEWALK behavior)
	struct st_position start_point;
	short int walk_range;								// defines how long from the start point it can go
	std::string graphic_filename;						// graphic file used on it
	struct npc_frameset frames[NPC_FRAME_NUMBER];
	bool can_shoot;										// if can shoot or not
	int IA_type;										// IA types: no_move, wait, attack, zig-zag (loop), side to side (origin to direction)
	int projectile_type_id;								// indicates the projectile ID (relation with file_projectile)
	bool first_run;
	int IA_state;										// state for IA
	character *target;									// number of the player that is target
	st_size distance;
	bool _is_player_friend;								// player spawned npcs must not hit him, but other npcs instead

	// "external" members
	static std::vector<character*> *player_list;					// reference to the players list
};

#endif // CLASSNPC_H

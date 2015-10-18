#ifndef CLASSBOSS_H
#define CLASSBOSS_H

#include "classnpc.h"
class classMap;		// advance declaration


class classboss : public classnpc
{
public:
	classboss(std::string set_name);
	classboss(int stage_id, int map_id, int main_id, int id); // load data from game_data and create a new npc
	void boss_move();
	void execute();
    bool is_boss();

private:
	void exec_daisiebot();

private:
	st_position initial_position;
	short _initialized;
	bool _screen_blinked;
};

#endif // CLASSBOSS_H

#ifndef STAGE_SELECT_H
#define STAGE_SELECT_H

#include "graphicslib.h"
#include "inputlib.h"
#include "timerlib.h"
#include "soundlib.h"
//#include "game.h"


class stage_select
{
public:
	stage_select(graphicsLib_gSurface stage_ref[STAGE_SELECT_COUNT]);
	struct st_position select();

private:
	void load_graphics();
	void move_highlight(short int x_inc, short int y_inc);
	void place_face(std::string face_file, std::string botname, st_position pos);
	bool animate_highlight();
	void draw_eyes(int x, int y, bool erase_eyes);

private:
	graphicsLib_gSurface s_light;
	graphicsLib_gSurface s_dark;
	graphicsLib_gSurface eyes_surface;
	graphicsLib_gSurface background;
	struct st_position highlight_pos;
	struct st_position select_pos;
	graphicsLib_gSurface *STAGE_SELECT_SURFACES[STAGE_SELECT_COUNT];
	bool highlight_animation_white;
	unsigned int timer_intro;
};

#endif // STAGE_SELECT_H

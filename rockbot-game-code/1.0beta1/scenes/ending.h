#ifndef ENDING_H
#define ENDING_H

#include "graphicslib.h"
#include <string>

class ending
{
public:
	ending();
	void start();

private:
	void boss();
	void train();
	void lab();
	void bosses();
	void credits();
	void draw_credits_text(graphicsLib_gSurface& surface);
	void draw_credit_line(graphicsLib_gSurface& surface, short int initial_line);


private:
	// from train
	graphicsLib_gSurface _train_surface;
	graphicsLib_gSurface _bg_surface;
	graphicsLib_gSurface _fence_surface;
	graphicsLib_gSurface _bushes_surface;
	std::vector<std::string> credits_list;

};

#endif // ENDING_H

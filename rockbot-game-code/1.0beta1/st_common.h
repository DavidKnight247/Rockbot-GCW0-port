#ifndef ST_COMMON_H
#define ST_COMMON_H

// some small structures that do not need a separated file for each one
#include <iostream>
#include <SDL/SDL.h>				//Include da SDL

struct st_position {
	short x;
	short y;
	st_position() {
		x = 0;
		y = 0;
	}
	st_position (int setX, int setY) {
		x = setX;
		y = setY;
	}
	st_position& operator=(const st_position set_pt)
	{
		x = set_pt.x;
		y = set_pt.y;
		return *this;
	}
};

struct st_size {
	short width;
	short heigth;
	st_size()
	{
		width = 0;
		heigth = 0;
	}
	st_size(int w, int h)
	{
		width = w;
		heigth = h;
	}
};


struct st_rectangle {
	short x;
	short y;
	short w;
	short h;
	st_rectangle () {
		x = 0;
		y = 0;
		w = 0;
		h = 0;
	}

	st_rectangle (int setX, int setY, int setW, int setH) {
		x = setX;
		y = setY;
		w = setW;
		h = setH;
	}
};

struct st_color {
	short r;
	short g;
	short b;
	st_color () {
		r = -1;
		g = -1;
		b = -1;
	}
	st_color (int setR, int setG, int setB) {
		r = setR;
		g = setG;
		b = setB;
	}
};

// structure that holds a graphic surface or memory section
struct graphicsLib_gSurface {
	SDL_Surface *gSurface;
	int width;
	int height;
	bool persistent;
	graphicsLib_gSurface()
	{
		gSurface = NULL;
		width = 0;
		height = 0;
		persistent = false;
	}
	// copy handler
	graphicsLib_gSurface(const graphicsLib_gSurface &original)
	{
		//std::cout << "graphicsLib_gSurface::COPY" << std::endl;
		width = original.width;
		height = original.height;
		persistent = false;
		if (original.width > 0) {
			gSurface = SDL_DisplayFormat(original.gSurface);
			//SDL_FreeSurface(original.gSurface);
		} else {
			gSurface = NULL;
		}
	}
	~graphicsLib_gSurface()
	{
		if (width > 0 && width < 100) { // 100 check is to handle invalid projectiles (trash in memory)
			SDL_FreeSurface(gSurface);
			width = -1;
			height = -1;
		}
		gSurface = NULL;
	}
	void freeGraphic()
	{
		if (width > 0 && width < 100) { // 100 check is to handle invalid projectiles (trash in memory)
			SDL_FreeSurface(gSurface);
			width = -1;
			height = -1;
		}
		gSurface = NULL;
	}
};






#endif // ST_COMMON_H

#define B sp[i - b]
#define D sp[i - (i>0?1:0)]
#define F sp[i + (i<wd?1:0)]
#define H sp[i + h]
#define E  sp[i]
#define E0 tp[i*2]
#define E1 tp[i*2 + 1]
#define E2 tp[i*2 + tpitch]
#define E3 tp[i*2 + 1 + tpitch]

#define WPN_COLUMN1_X 42
#define WPN_COLUMN2_X 182
#define WPN_COLUMN_Y 33
#define WEAPON_SPACING 16

#include <iostream>
using namespace std;


#include "defines.h"

extern string FILEPATH;

#include "inputlib.h"
extern inputLib input;

#include "timerlib.h"
extern timerLib timer;

#include "soundlib.h"
extern soundLib soundManager;


#include "graphicslib.h"

#include "file/format.h"
extern format_v_2_0_1::st_save game_save;

extern format_v_2_0_1::file_game game_data;


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
graphicsLib::graphicsLib() : _must_set_colors(true)
{
	tileset = NULL;
	stars_timer = 0;

    colormap[0].r = 97;
    colormap[0].g = 97;
    colormap[0].b = 97;
    colormap[1].r = 39;
    colormap[1].g = 27;
    colormap[1].b = 143;
    colormap[2].r = 0;
    colormap[2].g = 0;
    colormap[2].b = 171;
    colormap[3].r = 71;
    colormap[3].g = 0;
    colormap[3].b = 159;
    colormap[4].r = 143;
    colormap[4].g = 0;
    colormap[4].b = 119;
    colormap[5].r = 171;
    colormap[5].g = 0;
    colormap[5].b = 19;
    colormap[6].r = 167;
    colormap[6].g = 0;
    colormap[6].b = 0;
    colormap[7].r = 127;
    colormap[7].g = 11;
    colormap[7].b = 0;
    colormap[8].r = 67;
    colormap[8].g = 47;
    colormap[8].b = 0;
    colormap[9].r = 0;
    colormap[9].g = 71;
    colormap[9].b = 0;
    colormap[10].r = 0;
    colormap[10].g = 81;
    colormap[10].b = 0;
    colormap[11].r = 0;
    colormap[11].g = 63;
    colormap[11].b = 23;
    colormap[12].r = 27;
    colormap[12].g = 63;
    colormap[12].b = 95;
    colormap[13].r = 0;
    colormap[13].g = 0;
    colormap[13].b = 0;
    colormap[14].r = 196;
    colormap[14].g = 93;
    colormap[14].b = 0;
    colormap[15].r = 255;
    colormap[15].g = 153;
    colormap[15].b = 102;
    colormap[16].r = 188;
    colormap[16].g = 188;
    colormap[16].b = 188;
    colormap[17].r = 0;
    colormap[17].g = 115;
    colormap[17].b = 239;
    colormap[18].r = 35;
    colormap[18].g = 59;
    colormap[18].b = 239;
    colormap[19].r = 131;
    colormap[19].g = 0;
    colormap[19].b = 243;
    colormap[20].r = 191;
    colormap[20].g = 0;
    colormap[20].b = 191;
    colormap[21].r = 231;
    colormap[21].g = 0;
    colormap[21].b = 91;
    colormap[22].r = 219;
    colormap[22].g = 43;
    colormap[22].b = 0;
    colormap[23].r = 203;
    colormap[23].g = 79;
    colormap[23].b = 15;
    colormap[24].r = 139;
    colormap[24].g = 115;
    colormap[24].b = 0;
    colormap[25].r = 0;
    colormap[25].g = 151;
    colormap[25].b = 0;
    colormap[26].r = 0;
    colormap[26].g = 171;
    colormap[26].b = 0;
    colormap[27].r = 0;
    colormap[27].g = 147;
    colormap[27].b = 59;
    colormap[28].r = 0;
    colormap[28].g = 131;
    colormap[28].b = 139;
    colormap[29].r = 255;
    colormap[29].g = 102;
    colormap[29].b = 0;
    colormap[30].r = 255;
    colormap[30].g = 51;
    colormap[30].b = 0;
    colormap[31].r = 204;
    colormap[31].g = 204;
    colormap[31].b = 0;
    colormap[32].r = 235;
    colormap[32].g = 235;
    colormap[32].b = 235;
    colormap[33].r = 63;
    colormap[33].g = 191;
    colormap[33].b = 255;
    colormap[34].r = 95;
    colormap[34].g = 151;
    colormap[34].b = 255;
    colormap[35].r = 167;
    colormap[35].g = 139;
    colormap[35].b = 253;
    colormap[36].r = 247;
    colormap[36].g = 123;
    colormap[36].b = 255;
    colormap[37].r = 255;
    colormap[37].g = 119;
    colormap[37].b = 183;
    colormap[38].r = 255;
    colormap[38].g = 119;
    colormap[38].b = 99;
    colormap[39].r = 255;
    colormap[39].g = 155;
    colormap[39].b = 59;
    colormap[40].r = 243;
    colormap[40].g = 191;
    colormap[40].b = 63;
    colormap[41].r = 131;
    colormap[41].g = 211;
    colormap[41].b = 19;
    colormap[42].r = 79;
    colormap[42].g = 223;
    colormap[42].b = 75;
    colormap[43].r = 88;
    colormap[43].g = 248;
    colormap[43].b = 152;
    colormap[44].r = 0;
    colormap[44].g = 235;
    colormap[44].b = 219;
    colormap[45].r = 102;
    colormap[45].g = 51;
    colormap[45].b = 204;
    colormap[46].r = 51;
    colormap[46].g = 102;
    colormap[46].b = 255;
    colormap[47].r = 81;
    colormap[47].g = 81;
    colormap[47].b = 81;
    colormap[48].r = 230;
    colormap[48].g = 255;
    colormap[48].b = 0;
    colormap[49].r = 171;
    colormap[49].g = 231;
    colormap[49].b = 255;
    colormap[50].r = 199;
    colormap[50].g = 215;
    colormap[50].b = 255;
    colormap[51].r = 215;
    colormap[51].g = 203;
    colormap[51].b = 255;
    colormap[52].r = 255;
    colormap[52].g = 199;
    colormap[52].b = 255;
    colormap[52].r = 255;
	colormap[53].r = 255;
    colormap[53].g = 199;
    colormap[53].b = 219;
    colormap[54].r = 255;
    colormap[54].g = 191;
    colormap[54].b = 179;
    colormap[55].r = 255;
    colormap[55].g = 219;
    colormap[55].b = 171;
    colormap[56].r = 255;
    colormap[56].g = 231;
    colormap[56].b = 163;
    colormap[57].r = 227;
    colormap[57].g = 255;
    colormap[57].b = 163;
    colormap[58].r = 171;
    colormap[58].g = 243;
    colormap[58].b = 191;
    colormap[59].r = 179;
    colormap[59].g = 255;
    colormap[59].b = 207;
    colormap[60].r = 159;
    colormap[60].g = 255;
    colormap[60].b = 243;
    colormap[61].r = 112;
    colormap[61].g = 110;
    colormap[61].b = 110;
    colormap[62].r = 255;
    colormap[62].g = 192;
    colormap[62].b = 0;
    colormap[63].r = 255;
    colormap[63].g = 234;
    colormap[63].b = 0;
    colormap[64].r = 55;
    colormap[64].g = 255;
    colormap[64].b = 0;
    colormap[65].r = 255;
    colormap[65].g = 0;
    colormap[65].b = 255;
    colormap[66].r = 0;
    colormap[66].g = 255;
    colormap[66].b = 255;

	for (int i=0; i<COLOR_COUNT; i++) {
		colormap_original[i] = colormap[i];
	}


	RES_DIFF_W = 0;
	RES_DIFF_H = 0;

}

graphicsLib::~graphicsLib()
{
	SDL_FreeSurface(game_screen);
	ingame_menu.freeGraphic();
	config_menu.freeGraphic();
	dialog_surface.freeGraphic();
	config_menu_bg.freeGraphic();
	ready_message.freeGraphic();
	_btn_a_surface.freeGraphic();
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
bool graphicsLib::initGraphics()
{
	string filename;

	// GRAPHIC LIB
	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK|SDL_INIT_TIMER|SDL_INIT_AUDIO) < 0 ) {
		printf("SDL Error\n");
		printf("Unable to init SDL: %s\n", SDL_GetError());
		exit(-1);
	}
	atexit(SDL_Quit);

	printf("INFO: There are %d joysticks attached\n", SDL_NumJoysticks());
	input.init_joystick();

	// FONT
	TTF_Init();
	filename = FILEPATH + "data/fonts/kongtext.ttf";
	char *buffer = new char[filename.size()+1];
	strcpy(buffer, filename.c_str());
	SDL_RWops *fileRW;
	fileRW = SDL_RWFromFile(buffer, "rb");

	if (!fileRW) {
		printf("ERROR::initGraphics - could not open '%s' font\n", buffer);
		delete buffer;
		return false;
	} else {
		font = TTF_OpenFontRW(fileRW, 1, 8);
	}
	delete buffer;

	// GAME SCREEN
	SDL_ShowCursor( SDL_DISABLE );


#ifdef DINGUX
	game_screen = SDL_SetVideoMode(RES_W, RES_H, 8, SDL_SWSURFACE);
	SDL_SetColors(game_screen, colormap_original, 0, COLOR_COUNT);
#elif defined(OPEN_PANDORA)
	game_screen = SDL_SetVideoMode(RES_W, RES_H, 8, SDL_HWSURFACE|SDL_DOUBLEBUF);
	SDL_SetColors(game_screen, colormap_original, 0, COLOR_COUNT);
#elif !defined(SCALE)
	game_screen = SDL_SetVideoMode(RES_W, RES_H, 8, SDL_HWSURFACE|SDL_DOUBLEBUF);
	SDL_SetColors(game_screen, colormap_original, 0, COLOR_COUNT);
#else
	game_screen_scaled = SDL_SetVideoMode(RES_W*2, RES_H*2, 8, SDL_HWSURFACE|SDL_DOUBLEBUF);
	SDL_SetColors(game_screen_scaled, colormap_original, 0, COLOR_COUNT);
	SDL_Surface* temp_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, RES_W, RES_H, 8, 0, 0, 0, 255);
	game_screen = SDL_DisplayFormat(temp_surface);
	SDL_FreeSurface(temp_surface);
#endif

	SDL_SetColors(game_screen, colormap_original, 0, COLOR_COUNT);
	if (!game_screen) {
		cout << "FATAL-ERROR::initGraphics Could not create game_screen\n";
		exit(-1);
	}
	gameScreen.gSurface = game_screen;
	screen_pixel_format = *game_screen->format;

	gameScreen.width = RES_W;
	gameScreen.height = RES_H;

	// other loading methods
	load_icons();
	loadTileset();
	preload_faces();

	return true;
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::updateScreen()
{
	if (!game_screen) {
		cout << "FATAL-ERROR::updateScreen game_screen is NULL\n";
		return;
	}
	/*
	if (_screen_adjust.x != 0 || _screen_adjust.y != 0) {
		copyArea(_screen_adjust, &gameScreen, &gameScreen);
	}
	*/
	colorcycle_manager.execute();


	//change_colormap(COLOR_KEY_GREEN, colorcycle_manager.get_current_color_item(COLOR_KEY_GREEN));
	//change_colormap(COLOR_KEY_PURPLE, colorcycle_manager.get_current_color_item(COLOR_KEY_PURPLE));
	//change_colormap(COLOR_KEY_CYAN, colorcycle_manager.get_current_color_item(COLOR_KEY_CYAN));
#ifdef SCALE
	scale2x(game_screen, game_screen_scaled);
	if (_must_set_colors == true) {
		SDL_SetColors(game_screen_scaled, colormap, 0, COLOR_COUNT);
		_must_set_colors = false;
	}
	SDL_Flip(game_screen_scaled);
#else
	//game_screen->format = tileset->format;
	if (_must_set_colors == true) {
		SDL_SetColors(game_screen, colormap, 0, COLOR_COUNT);
		_must_set_colors = false;
	}
	SDL_Flip(game_screen);
#endif
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
SDL_Surface *graphicsLib::SDLSurfaceFromFile(string filename)
{
	SDL_RWops *rwop;
	SDL_Surface *spriteCopy;
	SDL_Surface *display_surface;
	char *buffer = new char[filename.size()+1];

	strcpy(buffer, filename.c_str());
	rwop=SDL_RWFromFile(buffer, "rb");
	delete buffer;
	if (!rwop) {
		cout << "DEBUG.loadSpriteFile - Error in loadSpriteFile: file '" << filename << "' not found\n";
		return NULL;
	}
	spriteCopy = IMG_Load_RW(rwop, 1);
	if (!spriteCopy) {
		printf("IMG_LoadPNG_RW: %s\n", IMG_GetError());
	}
	SDL_SetColors(spriteCopy, colormap_original, 0, COLOR_COUNT);
	display_surface = SDL_DisplayFormat(spriteCopy);
	SDL_FreeSurface(spriteCopy);

	return display_surface;
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::surfaceFromFile(string filename, struct graphicsLib_gSurface* res)
{
	if (res->width > 0 && res->gSurface != NULL) {
		SDL_FreeSurface(res->gSurface);
		res->gSurface = NULL;
		res->width = 0;
		res->height = 0;
	}
	res->gSurface = SDLSurfaceFromFile(filename);
	if (res->gSurface == NULL) {
		cout << "graphicsLib::surfaceFromFile - error loading SDL surface from file '" << filename << "'." << std::endl;
		exit(-1);
	} else {
		res->width = res->gSurface->w;
		res->height = res->gSurface->h;
	}
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::loadTileset()
{
	string filename = FILEPATH + "data/images/tilesets/default.png";

	tileset = SDLSurfaceFromFile(filename);
	if (tileset == NULL) {
		cout << "ERROR: Could not find file '" << filename << "'\n";
		exit(-1);
	}

	color_keys[COLOR_KEY_GREEN] = SDL_MapRGB(game_screen->format, 55, 255, 0);
	colorcycle_manager.set_color_key_number(COLOR_KEY_GREEN, color_keys[COLOR_KEY_GREEN]);

	color_keys[COLOR_KEY_PURPLE] = SDL_MapRGB(game_screen->format, 255, 0, 255);
	colorcycle_manager.set_color_key_number(COLOR_KEY_PURPLE, color_keys[COLOR_KEY_PURPLE]);

	color_keys[COLOR_KEY_CYAN] = SDL_MapRGB(game_screen->format, 0, 255, 255);
	colorcycle_manager.set_color_key_number(COLOR_KEY_CYAN, color_keys[COLOR_KEY_CYAN]);

	//colorcycle_manager.set_colormap(DAISIEBOT);

}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::copySDLArea(struct st_rectangle origin_rectangle, struct st_position destiny_pos, SDL_Surface* surfaceOrigin, SDL_Surface* surfaceDestiny, bool fix_colors=true)
{
	SDL_Rect src, dest;
	src.x = origin_rectangle.x;
	src.y = origin_rectangle.y;
	src.w = origin_rectangle.w;
	src.h = origin_rectangle.h;
	dest.x = destiny_pos.x;
	dest.y = destiny_pos.y;
	dest.w = src.w;
	dest.h = src.h;
	if (!surfaceOrigin) {
		cout << "copySDLArea - ERROR surfaceOrigin is NULL\n";
		exit(-1);
	}
	if (!surfaceDestiny) {
		cout << "copySDLArea - ERROR surfaceDestiny is NULL\n";
		exit(-1);
	}
	if (fix_colors == true) {
		SDL_SetColors(surfaceOrigin, colormap, 0, COLOR_COUNT);
	}
	//SDL_SetColors(surfaceDestiny, colormap, 0, COLOR_COUNT);
	SDL_BlitSurface(surfaceOrigin, &src, surfaceDestiny, &dest);
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::copyArea(struct st_rectangle origin_rectangle, struct st_position pos, struct graphicsLib_gSurface* surfaceOrigin, struct graphicsLib_gSurface* surfaceDestiny)
{
	copySDLArea(origin_rectangle, pos, surfaceOrigin->gSurface, surfaceDestiny->gSurface);
}

// ********************************************************************************************** //
// overload, use the whole width and height of given image
// ********************************************************************************************** //
void graphicsLib::copyArea(struct st_position pos, struct graphicsLib_gSurface* surfaceOrigin, struct graphicsLib_gSurface* surfaceDestiny)
{
	st_rectangle origin_rectangle(0, 0, surfaceOrigin->width, surfaceOrigin->height);
	copySDLArea(origin_rectangle, pos, surfaceOrigin->gSurface, surfaceDestiny->gSurface);
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::placeTile(struct st_position pos_origin, struct st_position pos_destiny, struct graphicsLib_gSurface* gSurface)
{
	struct st_rectangle origin_rectangle;

	origin_rectangle.x = pos_origin.x * TILESIZE;
	origin_rectangle.y = pos_origin.y * TILESIZE;
	origin_rectangle.w = TILESIZE;
	origin_rectangle.h = TILESIZE;
	copySDLArea(origin_rectangle, pos_destiny, tileset, gSurface->gSurface);

}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::showSurface(struct graphicsLib_gSurface* surfaceOrigin)
{
	struct st_rectangle origin_rectangle;
	struct st_position pos_destiny;

	origin_rectangle.x = 0;
	origin_rectangle.y = 0;
	origin_rectangle.w = RES_W;
	origin_rectangle.h = RES_H;
	pos_destiny.x = 0;
	pos_destiny.y = 0;
	copySDLArea(origin_rectangle, pos_destiny, surfaceOrigin->gSurface, game_screen);
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::showSurfaceRegion(struct graphicsLib_gSurface* surfaceOrigin, const struct st_rectangle origin_rectangle)
{
	struct st_position pos_destiny;

	pos_destiny.x = 0;
	pos_destiny.y = 0;
	showSurfaceRegionAt(surfaceOrigin, origin_rectangle, pos_destiny);
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::showSurfaceRegionAt(struct graphicsLib_gSurface* surfaceOrigin, const struct st_rectangle origin_rectangle, struct st_position pos_destiny)
{
	copySDLArea(origin_rectangle, pos_destiny, surfaceOrigin->gSurface, game_screen);
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::showSurfaceAt(struct graphicsLib_gSurface* surfaceOrigin, struct st_position pos, bool fix_colors=true)
{
	struct st_rectangle origin_rectangle;
	struct st_position pos_destiny;

	if (!surfaceOrigin->gSurface) {
		std::cout << "Error: no data in surfaceOrigin at graphicsLib::showSurfaceAt." << std::endl;
		//exit(-1);
		return;
	}

	origin_rectangle.x = 0;
	origin_rectangle.y = 0;
	origin_rectangle.w = surfaceOrigin->gSurface->w;
	origin_rectangle.h = surfaceOrigin->gSurface->h;
	pos_destiny.x = pos.x;
	pos_destiny.y = pos.y;

	copySDLArea(origin_rectangle, pos_destiny, surfaceOrigin->gSurface, game_screen, fix_colors);
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::initSurface(struct st_size size, struct graphicsLib_gSurface* gSurface)
{
	SDL_Surface* temp_surface = SDL_CreateRGBSurface(SDL_SWSURFACE , size.width, size.heigth, 8, 0, 0, 0, 0);
	SDL_SetColors(temp_surface, colormap_original, 0, COLOR_COUNT);
	SDL_FillRect(temp_surface, NULL, SDL_MapRGB(game_screen->format, 123, 123, 123));
	SDL_SetColorKey(temp_surface, SDL_SRCCOLORKEY, SDL_MapRGB(game_screen->format, 123, 123, 123));
	gSurface->gSurface = SDL_DisplayFormat(temp_surface);
	SDL_FreeSurface(temp_surface);
	gSurface->width = size.width;
	gSurface->height = size.heigth;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
struct graphicsLib_gSurface graphicsLib::surfaceFromRegion(struct st_rectangle rect_origin, struct graphicsLib_gSurface& originalSurface)
{
	struct st_position destiny_pos;

	destiny_pos.x = 0;
	destiny_pos.y = 0;
	struct graphicsLib_gSurface res;
	initSurface(st_size(rect_origin.w, rect_origin.h), &res);
	SDL_SetColors(res.gSurface, colormap_original, 0, COLOR_COUNT);
	copySDLArea(rect_origin, destiny_pos, originalSurface.gSurface, res.gSurface);
	SDL_SetColors(originalSurface.gSurface, colormap_original, 0, COLOR_COUNT);
	return res;
}



// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::blank_screen() {
	SDL_FillRect(game_screen, NULL, SDL_MapRGB(game_screen->format, 0, 0, 0));
}

void graphicsLib::blank_surface(graphicsLib_gSurface &surface)
{
	SDL_FillRect(surface.gSurface, NULL, SDL_MapRGB(game_screen->format, 0, 0, 0));
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::blank_area(short int x, short int y, short int w, short int h) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	SDL_FillRect(game_screen, &dest, SDL_MapRGB(game_screen->format, 0, 0, 0));
}

void graphicsLib::blank_area(short x, short y, short w, short h, graphicsLib_gSurface &surface)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	SDL_FillRect(surface.gSurface, &dest, SDL_MapRGB(game_screen->format, 0, 0, 0));
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
// returns if was cut (by user  pressing a key) or not
int graphicsLib::draw_progressive_text(short int x, short int y, string text, bool interrupt, bool show_stars, bool update_screen) {
	//SDL_Color font_color = {255,255,255};
	string temp_text;
	char temp_char;
	int text_x = 0;
	int text_y = 0;
	int cut = 0;
	unsigned int i;

	if (!font) {
		printf("ERROR - no fount found - TTF_OpenFont: %s\n", TTF_GetError());
		exit(-1);
	}


	for (i=0; i<text.size(); i++) {
		temp_char = text.at(i);

		temp_text = "";
		temp_text += temp_char;

		draw_text(text_x*9+x, text_y*11+y, temp_text);
		text_x++;
		if (temp_char == '\n') {
				text_x = 0;
				text_y++;
		}
		if (update_screen == true) {
			updateScreen();
		}
        if (interrupt) {
            if (show_stars) {
                cut = waitTimeWithStars(5);
            } else {
                cut = input.waitScapeTime(5);
            }
			if (cut) { return 1; }
		} else {
            if (show_stars) {
                waitTimeWithStars(5);
            } else {
                input.waitTime(5);
            }
		}
	}
	return 0;
}





// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::draw_text(short int x, short int y, string text) {
	if (text.length() <= 0) {
		return;
	}
	SDL_Color font_color;
	font_color.r = 255;
	font_color.g = 255;
	font_color.b = 255;
	SDL_Rect text_pos={x, y, 0, 0};
	if (!font) {
		printf("graphicsLib::draw_text - TTF_OpenFont: %s\n", TTF_GetError());
		exit(-1);
		// handle error
	}
	SDL_Surface* textSF = TTF_RenderText_Solid(font, text.c_str(), font_color);
	SDL_Surface* textSF_format = SDL_DisplayFormat(textSF);
	SDL_FreeSurface(textSF);
	SDL_BlitSurface(textSF_format, 0, game_screen, &text_pos);
	SDL_FreeSurface(textSF_format);
}

void graphicsLib::draw_text(short x, short y, string text, graphicsLib_gSurface &surface)
{
	SDL_Color font_color;
	font_color.r = 255;
	font_color.g = 255;
	font_color.b = 255;
	SDL_Rect text_pos={x, y, 0, 0};
	if (!font) {
		printf("graphicsLib::draw_text - TTF_OpenFont: %s\n", TTF_GetError());
		exit(-1);
		// handle error
	}
	SDL_Surface* textSF = TTF_RenderText_Solid(font, text.c_str(), font_color);
	SDL_Surface* textSF_format = SDL_DisplayFormat(textSF);
	SDL_FreeSurface(textSF);
	SDL_BlitSurface(textSF_format, 0, surface.gSurface, &text_pos);
	SDL_FreeSurface(textSF_format);
}

void graphicsLib::draw_centered_text(short y, string text)
{
	draw_centered_text(y, text, gameScreen);
}

void graphicsLib::draw_centered_text(short y, string text, graphicsLib_gSurface &surface)
{
	SDL_Color font_color;
	font_color.r = 255;
	font_color.g = 255;
	font_color.b = 255;
	SDL_Rect text_pos={0, y, 0, 0};
	if (!font) {
		printf("graphicsLib::draw_text - TTF_OpenFont: %s\n", TTF_GetError());
		exit(-1);
		// handle error
	}
	SDL_Surface* textSF = TTF_RenderText_Solid(font, text.c_str(), font_color);
	if (text.size() > 0) {
		text_pos.x = RES_W/2 - textSF->w/2;
	}
	SDL_Surface* textSF_format = SDL_DisplayFormat(textSF);
	SDL_FreeSurface(textSF);
	SDL_BlitSurface(textSF_format, 0, surface.gSurface, &text_pos);
	SDL_FreeSurface(textSF);
}



// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
int graphicsLib::getColorNumber(int r, int g, int b) {
	return SDL_MapRGB(game_screen->format, r, g, b);
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::drawCursor(st_position pos) {
	draw_text(pos.x, pos.y, ">");
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::eraseCursor(st_position pos) {
	blank_area(pos.x, pos.y, CURSOR_SPACING, CURSOR_SPACING);
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::blink_screen(int r, int g, int b) {
	int i;
	struct graphicsLib_gSurface tmp, screen_copy;

	initSurface(st_size(gameScreen.width, gameScreen.height), &screen_copy);
	copyArea(st_position(0, 0), &gameScreen, &screen_copy);
	//copy_area(game_screen, 0, 0, game_screen->w, game_screen->h, 0, 0, screen_copy);

	tmp.gSurface = SDL_DisplayFormat(game_screen);
	SDL_FillRect(tmp.gSurface, 0, SDL_MapRGB(game_screen->format, r, g, b));
	for (i=0; i<4; i++) {
		SDL_BlitSurface(tmp.gSurface, 0, game_screen, 0);
		updateScreen();
		input.waitTime(80);

		SDL_BlitSurface(screen_copy.gSurface, 0, game_screen, 0);
		updateScreen();
		input.waitTime(80);
	}
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::load_icons()
{
	struct graphicsLib_gSurface tmp;
	std::string filename = FILEPATH + "data/images/items.png";

	surfaceFromFile(filename, &tmp);
	for (int i=0; i<(tmp.width/16); i++) {
		item_icons.push_back(graphicsLib_gSurface());
		initSurface(st_size(16, 16), &item_icons.at(item_icons.size()-1));
		copyArea(st_rectangle(i*16, 0, 16, 16), st_position(0, 0), &tmp, &(item_icons.at(item_icons.size()-1)));
	}

	// big icon
	filename = FILEPATH + "data/images/icons.png";
	surfaceFromFile(filename, &tmp);
	for (int i=0; i<(tmp.width/14); i++) {
		weapon_icons.push_back(graphicsLib_gSurface());
		initSurface(st_size(14, 28), &weapon_icons.at(weapon_icons.size()-1));
		copyArea(st_rectangle(i*14, 0, 14, 28), st_position(0, 0), &tmp, &(weapon_icons.at(weapon_icons.size()-1)));
	}

	// small icons
	filename = FILEPATH + "data/images/icons_small.png";
	surfaceFromFile(filename, &tmp);
	for (int i=0; i<(tmp.width/8); i++) {
		small_weapon_icons.push_back(graphicsLib_gSurface());
		initSurface(st_size(8, 8), &small_weapon_icons.at(small_weapon_icons.size()-1));
		copyArea(st_rectangle(i*8, 0, 8, 8), st_position(0, 0), &tmp, &(small_weapon_icons.at(small_weapon_icons.size()-1)));
	}



	filename = FILEPATH + "data/images/backgrounds/menu.png";
	surfaceFromFile(filename, &config_menu);

	initSurface(st_size(config_menu.width, 50), &dialog_surface);
	copyArea(st_rectangle(0, 0, config_menu.width, 25), st_position(0, 0), &config_menu, &dialog_surface);
	copyArea(st_rectangle(0, config_menu.height-25, config_menu.width, 25), st_position(0, 25), &config_menu, &dialog_surface);

	filename = FILEPATH + "data/images/backgrounds/weapon_menu.png";
	surfaceFromFile(filename, &ingame_menu);

	filename = FILEPATH + "data/images/tilesets/ready.png";
	surfaceFromFile(filename, &ready_message);

	filename = FILEPATH + "data/images/backgrounds/btn_a.png";
	surfaceFromFile(filename, &_btn_a_surface);

}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::draw_item_icon(enum ITEM_ICONS_ENUM item_n, st_position pos)
{
	showSurfaceAt(&(item_icons.at(item_n)), pos);
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::draw_weapon_icon(enum WEAPON_ICONS_ENUM wpn_n, st_position point, bool active)
{
	st_position pos;

	if (point.x == 0) {
		pos.x = WPN_COLUMN1_X;
	} else {
		pos.x = WPN_COLUMN2_X;
	}

	pos.y = WPN_COLUMN_Y-3+point.y*16;

	clear_area(pos.x, pos.y, 14, 16, 0, 0, 0);
	if (active == true) {
		showSurfaceRegionAt(&weapon_icons.at(wpn_n), st_rectangle(0, 0, 14, 14), pos);
	} else {
		showSurfaceRegionAt(&weapon_icons.at(wpn_n), st_rectangle(0, 14, 14, 14), pos);
	}
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::draw_weapon_menu_bg(unsigned short int current_hp, format_v1_0::st_items* item_ref, graphicsLib_gSurface* player_frame) {
	showSurfaceAt(&ingame_menu, st_position((RES_W-ingame_menu.width)*0.5, (RES_H-ingame_menu.height)*0.5));

	showSurfaceRegionAt(&weapon_icons.at(0), st_rectangle(0, 14, 14, 14), st_position(WPN_COLUMN1_X, 30));
	draw_horizontal_hp_bar(WPN_COLUMN_Y, 2, current_hp);

	for (int i=0; i<5; i++) {
		showSurfaceRegionAt(&weapon_icons.at(i+1), st_rectangle(0, 14, 14, 14), st_position(WPN_COLUMN1_X, 30+(i+1)*16));
		draw_horizontal_hp_bar(WPN_COLUMN_Y+(i+1)*WEAPON_SPACING, 2, item_ref->weapons[i+1]);
	}
	for (int i=5; i<10; i++) {
		showSurfaceRegionAt(&weapon_icons.at(i+1), st_rectangle(0, 14, 14, 14), st_position(182, 30+(i-4)*16));
		draw_horizontal_hp_bar(WPN_COLUMN_Y+(i-4)*WEAPON_SPACING, 3, item_ref->weapons[i+1]);
	}


	copyArea(st_position(26, 150), player_frame, &gameScreen);
	//draw_horizontal_hp_bar(st_position(56, 160), current_hp);

	std::stringstream ss;

	ss.str(std::string());
	ss << "0" << item_ref->lifes;
	draw_text(100, 177, ss.str());


	ss.str(std::string());
	ss << "0" << item_ref->energy_tanks;
	draw_text(136, 177, ss.str());

	ss.str(std::string());
	ss << "0" << item_ref->weapon_tanks;
	draw_text(160, 177, ss.str());

	ss.str(std::string());
	ss << "0" << item_ref->special_tanks;
	draw_text(185, 177, ss.str());

	ss.str(std::string());
	ss << "0" << item_ref->balancer;
	draw_text(209, 177, ss.str());

	ss.str(std::string());
	ss << "0" << item_ref->bolts;
	draw_text(259, 177, ss.str());

	//std::cout << "item_ref->energy_tanks: " << item_ref->energy_tanks << ", item_ref->weapon_tanks: " << item_ref->weapon_tanks << std::endl;

	updateScreen();
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::show_ready()
{
	st_position dest_pos((RES_W/2)-26, (RES_H/2)-6);
	graphicsLib_gSurface ready_bg;

	initSurface(st_size(ready_message.width, ready_message.height), &ready_bg);
	copyArea(st_rectangle((RES_W/2)-26, (RES_H/2)-6, ready_message.width, ready_message.height), st_position(0, 0), &gameScreen, &ready_bg);
	for (int i=0; i<4; i++) {
		copyArea(dest_pos, &ready_message, &gameScreen);
		updateScreen();
		input.waitTime(400);
		copyArea(dest_pos, &ready_bg, &gameScreen);
		updateScreen();
		input.waitTime(400);
	}
	ready_bg.freeGraphic();
}

void graphicsLib::scale2x(SDL_Surface* surface, SDL_Surface* dest)
{
	register int i, j;
	int b, h;
	int bpp = surface->format->BytesPerPixel;
	if (SDL_MUSTLOCK(dest) != 0)
	{
	 if (SDL_LockSurface(dest) < 0)
	 {
	   fprintf(stderr, "dest locking failedn");
	   return;
	 }
	}

	const int wd = ((dest->w / 2) < (surface->w)) ? (dest->w / 2) : (surface->w);
	const int hg = ((dest->h) < (surface->h*2)) ? (dest->h / 2) : (surface->h);

	switch (bpp)
	{
	case 1:
	{
		int tpitch = dest->pitch;
		int spitch = surface->pitch;
		Uint8* tp = (Uint8*) dest->pixels;
		Uint8* sp = (Uint8*) surface->pixels;

		for (j = 0; j < hg; ++j)
		{
			b = j>0?spitch:0;
			h = j<hg?spitch:0;

			for (i = 0; i < wd; ++i)
			{
				if (B != H && D != F) {
					//E0 = D == B ? D : E;
					//E1 = B == F ? F : E;
					//E2 = D == H ? D : E;
					//E3 = H == F ? F : E;
					E0 = E;
					E1 = E;
					E2 = E;
					E3 = E;
				} else {
					E0 = E;
					E1 = E;
					E2 = E;
					E3 = E;
				}
			}
			tp += 2*tpitch;
			sp += spitch;
		}

		break;
	}

	 case 2:
	 {
	   int tpitch = dest->pitch / 2;
	   int spitch = surface->pitch / 2;
	   Uint16* tp = (Uint16*) dest->pixels;
	   Uint16* sp = (Uint16*) surface->pixels;

	   for (j = 0; j < hg; ++j)
	   {
		b = j>0?spitch:0;
		h = j<hg?spitch:0;
		 for (i = 0; i < wd; ++i)
		 {
			   if (B != H && D != F) {
					   E0 = D == B ? D : E;
					   E1 = B == F ? F : E;
					   E2 = D == H ? D : E;
					   E3 = H == F ? F : E;
			   } else {
					   E0 = E;
					   E1 = E;
					   E2 = E;
					   E3 = E;
			   }
		 }
		 tp += 2*tpitch;
		 sp += spitch;
	   }

	   break;
	 }
	 case 4:
	 {
	   int tpitch = dest->pitch / 4;
	   int spitch = surface->pitch / 4;
	   Uint32* tp = (Uint32*) dest->pixels;
	   Uint32* sp = (Uint32*) surface->pixels;

	   for (j = 0; j < hg; ++j)
	   {
		b = j>0?spitch:0;
		h = j<hg?spitch:0;
		 for (i = 0; i < wd; ++i)
		 {
			   if (B != H && D != F) {
					   E0 = D == B ? D : E;
					   E1 = B == F ? F : E;
					   E2 = D == H ? D : E;
					   E3 = H == F ? F : E;
			   } else {
					   E0 = E;
					   E1 = E;
					   E2 = E;
					   E3 = E;
			   }
		 }
		 tp += 2*tpitch;
		 sp += spitch;
	   }

	   break;
	 }
	 default:
	   std::cout << "Unsupported bitdepth.n(" << bpp << ")" << std::endl;
	   break;
	}


	if (SDL_MUSTLOCK(dest) != 0)
	{
	 SDL_UnlockSurface(dest);
	}
}


void graphicsLib::change_colormap(unsigned int color_key, st_color new_color)
{
	if (!game_screen) {
		std::cout << "graphicsLib::change_colormap - LEAVE because there is no game_screen" << std::endl;
		return;
	}
	colormap[color_keys[color_key]].r = new_color.r;
	colormap[color_keys[color_key]].g = new_color.g;
	colormap[color_keys[color_key]].b = new_color.b;
	//std::cout << "change_colormap - color_key: " << color_key << ", res: " << res << std::endl;
}

void graphicsLib::draw_hp_bar(short int hp, short int player_n, short int weapon_n)
{
	short int y, i;

	st_color color1(255, 51, 0);
	if (weapon_n == WEAPON_DEFAULT) {
		if (player_n == 0) {
			color1.r = 127;
			color1.g = 11;
			color1.b = 0;
		}
	} else {
		color1.r = game_data.players[player_n].weapon_colors[weapon_n].color1.r;
		color1.g = game_data.players[player_n].weapon_colors[weapon_n].color1.g;
		color1.b = game_data.players[player_n].weapon_colors[weapon_n].color1.b;
	}
	st_color color2(188, 188, 188);
	st_color color3(235, 235, 235);
	if (weapon_n == WEAPON_MUMMYBOT || weapon_n == WEAPON_ITEM_JET) {
		color2.r = 97;
		color2.g = 97;
		color2.b = 97;
		color3.r = 188;
		color3.g = 188;
		color3.b = 188;
	}


	if (weapon_n == WEAPON_APEBOT) {
		color1.r = 255;
		color1.g = 100;
		color1.b = 100;
	}

	st_position bar_pos(1, 2);
	if (weapon_n != WEAPON_DEFAULT) {
		bar_pos.x = 10;
	}


	clear_area(bar_pos.x, bar_pos.y, 8, 57, color1.r, color1.g, color1.b);

	showSurfaceRegionAt(&small_weapon_icons.at(weapon_n), st_rectangle(0, 0, 8, 8), st_position(bar_pos.x, 59));


	for (i=0; i<hp; i++) {
		y = ((28-i)*2+1)+bar_pos.y-2;
		clear_area(bar_pos.x+1, y, 2, 1, color2.r, color2.g, color2.b);
		clear_area(bar_pos.x+3, y, 2, 1, color3.r, color3.g, color3.b);
		clear_area(bar_pos.x+5, y, 2, 1, color2.r, color2.g, color2.b);
	}
}

void graphicsLib::clear_area(short int x, short int y, short int w, short int h, short int r, short int g, short int b) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	SDL_FillRect(game_screen, &dest, SDL_MapRGB(game_screen->format, r, g, b));
}

void graphicsLib::clear_surface_area(short int x, short int y, short int w, short int h, short int r, short int g, short int b, struct graphicsLib_gSurface& surface) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	SDL_FillRect(surface.gSurface, &dest, SDL_MapRGB(surface.gSurface->format, r, g, b));
}

void graphicsLib::set_colormap(int map_n) {
	change_colormap(COLOR_KEY_GREEN, st_color(55, 255, 0));
	change_colormap(COLOR_KEY_PURPLE, st_color(255, 0, 255));
	change_colormap(COLOR_KEY_CYAN, st_color(0, 255, 255));

	SDL_SetColors(game_screen, colormap_original, 0, COLOR_COUNT);
	colorcycle_manager.set_colormap(map_n);
}

void graphicsLib::show_config_bg(int position) // 0 - centered, 1 - top, 2 - bottom
{
	int posX = (RES_W-config_menu.width)*0.5;
	int posY;

	if (position == 0) {
		posY = (RES_H-config_menu.height)*0.5;
	} else if (position == 1) {
		posY = 3;
	} else {
		posY = RES_H - config_menu.height - 25;
	}

	st_position bg_pos(posX, posY);
	st_rectangle bg_rect(posX, posY, config_menu.width, config_menu.height);

	initSurface(st_size(config_menu.width, config_menu.height), &config_menu_bg);
    copyArea(bg_rect, st_position(0, 0), &gameScreen, &config_menu_bg);
    copyArea(bg_pos, &config_menu, &gameScreen);
}

void graphicsLib::hide_config_bg(int position)
{
	int posX = (RES_W-config_menu.width)*0.5;
	int posY;

	if (position == 0) {
		posY = (RES_H-config_menu.height)*0.5;
	} else if (position == 1) {
		posY = 9;
	} else {
		posY = RES_H - config_menu.height - 25;
	}

	st_position bg_pos(posX, posY);

    copyArea(bg_pos, &config_menu_bg, &gameScreen);
}

void graphicsLib::show_dialog(int position, bool show_btn)
{
	int posX = (RES_W-config_menu.width)*0.5;
	int posY;

	if (position == 0) {
		posY = (RES_H-config_menu.height)*0.5;
	} else if (position == 1) {
		posY = 3;
	} else {
		posY = RES_H - config_menu.height - 25;
	}

	dialog_pos.x = posX;
	dialog_pos.y = posY;

	st_position bg_pos(posX, posY);
	copyArea(bg_pos, &dialog_surface, &gameScreen);
	if (show_btn == true) {
		show_btn_a(st_position(posX+config_menu.width-_btn_a_surface.width-2, posY+dialog_surface.height-_btn_a_surface.height-2));
	}
}

st_position graphicsLib::get_dialog_pos() {
	return dialog_pos;
}

void graphicsLib::draw_horizontal_hp_bar(short int y_adjust, short int right, short int hp, short int player_n) {
	short int x;
	short int r, g, b;
	// armas - coluna1: 42, 26, 182, coluna2: 26

	if (right == 0) {
		x = WPN_COLUMN1_X+18;
		r = 127;
		g = 11;
		b = 0;
	} else if (right == 1) {
		x = WPN_COLUMN2_X+18;
		r = 255;
		g = 102;
		b = 0;
	} else if (right == 2) {
		x = WPN_COLUMN1_X+18;
		r = 0;
		g = 0;
		b = 0;
	} else {
		x = WPN_COLUMN2_X+18;
		r = 0;
		g = 0;
		b = 0;
	}
	// border
	/*
	clear_area(weapon_menu_surface, x+1, y_adjust, 61, 2, 188, 188, 188);
	clear_area(weapon_menu_surface, x+1, y_adjust+7, 61, 2, 188, 188, 188);

	clear_area(weapon_menu_surface, x+1, y_adjust, 2, 9, 188, 188, 188);
	clear_area(weapon_menu_surface, x+60, y_adjust, 2, 9, 188, 188, 188);
	*/


	if (right < 2) {
		clear_area(x+1, 1+y_adjust, 61, 9, r, g, b);
	}

	draw_horizontal_hp_bar(st_position(x, y_adjust), hp, player_n);

}


void graphicsLib::draw_horizontal_hp_bar(st_position pos, short int hp, short player_n) {
	for (int i=0; i<hp; i++) {
		int y = ((28-i)*2+1);
		if (player_n == 0) {
			clear_area(pos.x+y+1, 2+pos.y, 1, 2, 27, 63, 95);
			clear_area(pos.x+y+1, 4+pos.y, 1, 2, 0, 131, 139);
			clear_area(pos.x+y+1, 6+pos.y, 1, 2, 27, 63, 95);
		} else if (player_n == 1) {
			clear_area(pos.x+y+1, 2+pos.y, 1, 2, 203, 79, 15);
			clear_area(pos.x+y+1, 4+pos.y, 1, 2, 255, 155, 59);
			clear_area(pos.x+y+1, 6+pos.y, 1, 2, 203, 79, 15);
		} else {
			clear_area(pos.x+y+1, 2+pos.y, 1, 2, 188, 188, 188);
			clear_area(pos.x+y+1, 4+pos.y, 1, 2, 255, 255, 255);
			clear_area(pos.x+y+1, 6+pos.y, 1, 2, 188, 188, 188);
		}
	}
}





void graphicsLib::draw_weapon_cursor(st_position pos, short int hp, short int player_n)
{
	int pos_y, pos_x;

	if (pos.x == 0) {
		pos_x = WPN_COLUMN1_X + 18;
	} else {
		pos_x = WPN_COLUMN2_X + 18;
	}



	if (pos.y == 0) {
		pos_y = WPN_COLUMN_Y;
	} else if (pos.y == 6 && pos.x == 0) {
		pos_y = 159;
		pos_x = 119;
	} else if (pos.y == 6 && pos.x == 1) {
		pos_y = 159;
		pos_x = 143;
	} else {
		pos_y = WEAPON_SPACING*pos.y + 33;
	}
	draw_horizontal_hp_bar(st_position(pos_x, pos_y), hp, player_n);
	updateScreen();
}


void graphicsLib::draw_star(short int x, short int y, int size) {
	clear_area(x, y, size, size, 188, 188, 188);
}

void graphicsLib::erase_star(short int x, short int y, int size) {
	clear_area(x, y, size, size, 0, 0, 0);
}


// random a position for each star
void graphicsLib::init_stars() {
	int i;
	for (i=0; i<INTRO_STARS_NUMBER; i++) {
		star_list[i].x = rand() % 320;
		if (i % 2 == 0) {
			star_list[i].y = rand() % 46;
		} else {
			star_list[i].y = rand() % 53 + 145;
		}
	}
	for (i=0; i<INTRO_STARS_NUMBER; i++) {
		small_star_list[i].x = rand() % 320;
		if (i % 2 == 0) {
			small_star_list[i].y = rand() % 46;
		} else {
			small_star_list[i].y = rand() % 53 + 145;
		}
	}
	for (i=0; i<INTRO_STARS_NUMBER/2; i++) {
		big_star_list[i].x = rand() % 320;
		if (i % 2 == 0) {
			big_star_list[i].y = rand() % 46;
		} else {
			big_star_list[i].y = rand() % 53 + 145;
		}
	}
}

void graphicsLib::anim_stars() {
	int i;

	/// @TODO - add timer to control stars speed
	if (stars_timer > timer.getTimer()) {
		return;
	}

	for (i=0; i<INTRO_STARS_NUMBER; i++) {
		erase_star(star_list[i].x, star_list[i].y, 2);
		erase_star(small_star_list[i].x, small_star_list[i].y, 1);
		star_list[i].x -= ANIM_STARS_STEP;
		if (star_list[i].x <= 0) {
			star_list[i].x = 320+ANIM_STARS_STEP;
			if (i % 2 == 0) {
				star_list[i].y = rand() % 46;
			} else {
				star_list[i].y = rand() % 53 + 145;
			}
		}
		small_star_list[i].x -= ANIM_STARS_SMALL_STEP;
		if (small_star_list[i].x <= 0) {
			small_star_list[i].x = 320+ANIM_STARS_SMALL_STEP;
			if (i % 2 == 0) {
				small_star_list[i].y = rand() % 46;
			} else {
				small_star_list[i].y = rand() % 53 + 145;
			}
		}
	}

	for (i=0; i<INTRO_STARS_NUMBER/2; i++) {
		erase_star(big_star_list[i].x, big_star_list[i].y, 3);
		big_star_list[i].x -= ANIM_STARS_BIG_STEP;
		if (big_star_list[i].x <= 0) {
			big_star_list[i].x = 320+ANIM_STARS_BIG_STEP;
			if (i % 2 == 0) {
				big_star_list[i].y = rand() % 46;
			} else {
				big_star_list[i].y = rand() % 53 + 145;
			}
		}
	}

	for (i=0; i<INTRO_STARS_NUMBER; i++) {
		draw_star(star_list[i].x, star_list[i].y, 2);
		draw_star(small_star_list[i].x, small_star_list[i].y, 1);
	}
	for (i=0; i<INTRO_STARS_NUMBER/2; i++) {
		draw_star(big_star_list[i].x, big_star_list[i].y, 3);
	}
	stars_timer = timer.getTimer()+25;
	updateScreen();
}

bool graphicsLib::waitTimeWithStars(int wait_period) {
	int now_time=0;
	now_time = timer.getTimer();
	wait_period = now_time + wait_period;
	while (now_time < wait_period) {
		now_time = timer.getTimer();
        if (input.waitScapeTime(1)) {
            return true;
        }
        input.waitTime(5);
		anim_stars();
	}
	return false;
}

void graphicsLib::show_btn_a(st_position btn_pos)
{
	showSurfaceAt(&_btn_a_surface, btn_pos);
}

void graphicsLib::set_screen_adjust(st_position adjust)
{
	_screen_adjust.x = adjust.x;
	_screen_adjust.y = adjust.y;
}

void graphicsLib::update_colors()
{
	_must_set_colors = true;
}

void graphicsLib::boss_intro(int pos_n) {
	graphicsLib_gSurface spriteCopy, bgCopy, boss_graphics, boss_bg;
	st_position boss_pos, sprite_size;
	unsigned int intro_frames_n = 1;
	int intro_frames_rollback=0;
	boss_pos.x = 20;
	boss_pos.y = -37;
	int text_x = RES_W*0.5 - 60;
	unsigned int i;
	int j;
	std::string filename;
	std::string botname;
	std::string buffer;

	/*
	if (pos_n == SKULLCASTLE1) {
		 int ship_x;
		 int dest_x, dest_y, dest_w, dest_h;
		// show dr. destrin scaping in the capsule
		blank_screen(game_screen);
		sprintf(buffer, "%sdata/images/sprites/enemies/destrin_capsule_small.png", FILEPATH);
		spriteCopy = loadSpriteFile(buffer);
		ship_x = RES_W + TILE_SIZE;

		play_timed_sfx(SFX_DESTRIN_SHIP, 8000);

		dest_w = spriteCopy->w/3;
		dest_h = spriteCopy->h;
		dest_y = RES_H*0.5;
		bgCopy = SDL_CreateRGBSurface(SDL_SWSURFACE , dest_w, dest_h, 8, 0, 0, 0, 0);
		SDL_SetColors(bgCopy, colormap, 0, COLOR_COUNT);
		copy_area(game_screen, 0, 0, dest_w, dest_h, 0, 0, bgCopy);

		// part 1 - shi moves from right to screen center
		while (ship_x > RES_W*0.5 - spriteCopy->h/2) {
			dest_x = ship_x + MOVE_STEP;
			copy_area(bgCopy, 0, 0, dest_w, dest_h, dest_x, dest_y, game_screen);
			copy_area(spriteCopy, 0, 0, dest_w, dest_h, ship_x, dest_y, game_screen);
			updateScreen(game_screen);
			waitTime(10);
			ship_x -= MOVE_STEP;
		}
		// part 2 - show ship without cover
		waitTime(10);
		dest_x = ship_x += MOVE_STEP;
		copy_area(bgCopy, 0, 0, dest_w, dest_h, dest_x, dest_y, game_screen);
		copy_area(spriteCopy, 0, 0, dest_w, dest_h, ship_x, dest_y, game_screen);
		updateScreen(game_screen);
		waitTime(800);
		copy_area(bgCopy, 0, 0, dest_w, dest_h, dest_x, dest_y, game_screen);
		// part 3 - show eyebrowns animation
		for (i=0; i<3; i++) {
			copy_area(spriteCopy, (spriteCopy->w/3)*2, 0, dest_w, dest_h, ship_x, dest_y, game_screen);
			updateScreen(game_screen);
			waitTime(200);
			copy_area(spriteCopy, spriteCopy->w/3, 0, dest_w, dest_h, ship_x, dest_y, game_screen);
			updateScreen(game_screen);
			waitTime(200);
		}
		updateScreen(game_screen);
		waitTime(1200);
		// part 4 - move ship from center to left
		while (ship_x > -spriteCopy->w) {
			copy_area(bgCopy, 0, 0, dest_w, dest_h, ship_x, dest_y, game_screen);
			ship_x -= MOVE_STEP;
			copy_area(spriteCopy, 0, 0, dest_w, dest_h, ship_x, dest_y, game_screen);
			updateScreen(game_screen);
			waitTime(10);
		}
		SDL_FreeSurface(spriteCopy);
		SDL_FreeSurface(bgCopy);


		sprintf(buffer, "%sdata/images/backgrounds/skull_castle.png", FILEPATH);
		spriteCopy = loadSpriteFile(buffer);
		copy_area(spriteCopy, 0, 0, spriteCopy->w, spriteCopy->h, 0, 0, game_screen);
		SDL_FreeSurface(spriteCopy);
		updateScreen(game_screen);
		play_sfx(SFX_SKULL_CASTLE_INTRO);
		waitTime(7550);
		return;
	}
	*/


	if (pos_n == SPIKEBOT && game_save.stages[pos_n] == 0) {
		buffer = FILEPATH + "data/images/sprites/enemies/spikebot.png";
		sprite_size.x = 38;
		sprite_size.y = 34;
		intro_frames_n = 3;
		botname = "SPIKE BOT";
	} else if (pos_n == DAISIEBOT && game_save.stages[pos_n] == 0) {
		buffer = FILEPATH + "data/images/sprites/enemies/daisiebot.png";
		sprite_size.x = 36;
		sprite_size.y = 31;
		intro_frames_n = 3;
		botname = "DAISIE BOT";
	} else if (pos_n == DYNAMITEBOT && game_save.stages[pos_n] == 0) {
		buffer = FILEPATH + "data/images/sprites/enemies/dynamitebot.png";
		sprite_size.x = 44;
		sprite_size.y = 48;
		intro_frames_n = 3;
		intro_frames_rollback = 1;
		botname = "DYNAMITE BOT";
	} else if (pos_n == TECHNOBOT && game_save.stages[pos_n] == 0) {
		buffer = FILEPATH + "data/images/sprites/enemies/technobot.png";
		sprite_size.x = 51;
		sprite_size.y = 47;
		intro_frames_n = 3;
		botname = "TECHNO BOT";
		intro_frames_rollback = 1;
	} else if (pos_n == MUMMYBOT && game_save.stages[pos_n] == 0) {
		buffer = FILEPATH + "data/images/sprites/enemies/mummybot.png";
		sprite_size.x = 33;
		sprite_size.y = 33;
		intro_frames_n = 3;
		botname = "MUMMY BOT";
	} else if (pos_n == APEBOT && game_save.stages[pos_n] == 0) {
		buffer = FILEPATH + "data/images/sprites/enemies/apebot.png";
		sprite_size.x = 40;
		sprite_size.y = 39;
		intro_frames_n = 5;
		botname = "APE BOT";
	} else if (pos_n == SEAHORSEBOT && game_save.stages[pos_n] == 0) {
		buffer = FILEPATH + "data/images/sprites/enemies/seahorsebot.png";
		sprite_size.x = 31;
		sprite_size.y = 38;
		intro_frames_n = 1;
		botname = "SEAHORSE BOT";
	} else if (pos_n == MAGEBOT && game_save.stages[pos_n] == 0) {
		buffer = FILEPATH + "data/images/sprites/enemies/magebot.png";
		sprite_size.x = 26;
		sprite_size.y = 33;
		intro_frames_n = 4;
		botname = "MAGE BOT";
	} else {
		std::cout << "boss_intro - stage already finished" << std::endl;
		return;
	}


	soundManager.play_sfx(SFX_STAGE_SELECTED);
	blank_screen();

	filename = FILEPATH + "data/images/backgrounds/stage_boss_intro.png";
	surfaceFromFile(filename, &spriteCopy);
	copyArea(st_position(0, 0), &spriteCopy, &gameScreen);

	filename = FILEPATH + "data/images/backgrounds/stage_boss_intro.png";
	surfaceFromFile(filename, &bgCopy);
	copyArea(st_position(0, 0), &bgCopy, &gameScreen);

	updateScreen();


	initSurface(st_size(sprite_size.x, sprite_size.y), &boss_graphics);
	surfaceFromFile(buffer, &boss_graphics);
	initSurface(st_size(sprite_size.x, sprite_size.y), &boss_bg);
	copyArea(st_rectangle(boss_pos.x, 80, sprite_size.x, sprite_size.y), st_position(0, 0), &bgCopy, &boss_bg);


	init_stars();

	while (boss_pos.y < 80) {
		copyArea(st_rectangle(0, 0, sprite_size.x, sprite_size.y), st_position(boss_pos.x, boss_pos.y), &boss_bg, &gameScreen);
		boss_pos.y += 4;
		copyArea(st_rectangle(boss_pos.x, boss_pos.y, sprite_size.x, sprite_size.y), st_position(0, 0), &bgCopy, &boss_bg);
		copyArea(st_rectangle(0, 0, sprite_size.x, sprite_size.y), st_position(boss_pos.x, boss_pos.y), &boss_graphics, &gameScreen);
		updateScreen();
		waitTimeWithStars(5);
	}
	waitTimeWithStars(500);
	if (intro_frames_n > 1) {
		for (i=0; i<intro_frames_n; i++) {
			copyArea(st_rectangle(0, 0, spriteCopy.width, spriteCopy.height), st_position(boss_pos.x, boss_pos.y), &boss_bg, &gameScreen);
			copyArea(st_rectangle(i*sprite_size.x, 0, sprite_size.x, sprite_size.y), st_position(boss_pos.x, boss_pos.y), &boss_graphics, &gameScreen);
			waitTimeWithStars(200);
		}
		if (intro_frames_rollback == 1) {
			for (j=intro_frames_n-1; j>=0; j--) {
				copyArea(st_rectangle(0, 0, spriteCopy.width, spriteCopy.height), st_position(boss_pos.x, boss_pos.y), &boss_bg, &gameScreen);
				copyArea(st_rectangle(j*sprite_size.x, 0, sprite_size.x, sprite_size.y), st_position(boss_pos.x, boss_pos.y), &boss_graphics, &gameScreen);
				waitTimeWithStars(200);
			}
		}
	}
	for (i=0; i<botname.size(); i++) {
		waitTimeWithStars(100);
		draw_text(text_x, 118, &botname.at(i));
		text_x += 8;
	}
	waitTimeWithStars(2500);
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::preload_faces() {

	/// @TODO: do this from directory listing
	/*
	string filename;
	int i;

	for (i=0; i<FACES_COUNT; i++) {
		filename = FILEPATH + "data/images/faces/" + face_name[i];
		surfaceFromFile(filename, FACES_SURFACES[i]);
	}
	*/
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void graphicsLib::unload_faces() {
	/*
	std::map<std::string, graphicsLib_gSurface>::iterator it;

	for (it = FACES_SURFACES.begin(); it != FACES_SURFACES.end(); it++) {
		delete (*it).second;
		FACES_SURFACES.erase(it);
	}
	*/
}



//void stage_select::place_face(int face_n, char botname[20], short int posx, short int posy) {
void graphicsLib::place_face(std::string face_file, st_position pos) {
	std::map<std::string, graphicsLib_gSurface>::iterator it;

	it = FACES_SURFACES.find(face_file);

	if (it == FACES_SURFACES.end()) {
		std::string filename = FILEPATH + "data/images/faces/" + face_file;
		surfaceFromFile(filename, &FACES_SURFACES[face_file]);
	}
	copyArea(st_position(pos.x, pos.y), &FACES_SURFACES[face_file], &gameScreen);
}

void graphicsLib::change_surface_color(st_color key, st_color new_color, struct graphicsLib_gSurface* surface)
{
	if (!surface->gSurface) {
		return;
	}
	SDL_Color new_colormap[COLOR_COUNT];
	for (int i=0; i<COLOR_COUNT; i++) {
		new_colormap[i] = surface->gSurface->format->palette->colors[i];
	}
	Uint32 key_n = SDL_MapRGB(tileset->format, key.r, key.g, key.b);
	new_colormap[key_n].r = new_color.r;
	new_colormap[key_n].g = new_color.g;
	new_colormap[key_n].b = new_color.b;
	SDL_SetColors(surface->gSurface, new_colormap, 0, COLOR_COUNT);
}


void graphicsLib::boss_credits(int pos_n) {
	graphicsLib_gSurface spriteCopy, bgCopy, boss_graphics, boss_bg;
	st_position boss_pos, sprite_size;
	unsigned int intro_frames_n = 1;
	int intro_frames_rollback=0;
	boss_pos.x = 20;
	boss_pos.y = -37;
	int text_x = RES_W*0.5 - 90;
	unsigned int i;
	int j;
	std::string filename;
	std::string botname;
	std::string buffer;
	std::string concept_author;
	std::string design_author;



	if (pos_n == SPIKEBOT) {
		buffer = FILEPATH + "data/images/sprites/enemies/spikebot.png";
		sprite_size.x = 38;
		sprite_size.y = 34;
		intro_frames_n = 3;
		botname = "#5 - SPIKE BOT";
		concept_author = "CONCEPT: IURI FIEDORUK";
		design_author = "DESIGN: IURI FIEDORUK";
	} else if (pos_n == DAISIEBOT) {
		buffer = FILEPATH + "data/images/sprites/enemies/daisiebot.png";
		sprite_size.x = 36;
		sprite_size.y = 31;
		intro_frames_n = 3;
		botname = "#4 - DAISIE BOT";
		concept_author = "CONCEPT: ARIS K.S.F.";
		design_author = "DESIGN: ARIS K.S.F.";
	} else if (pos_n == DYNAMITEBOT) {
		buffer = FILEPATH + "data/images/sprites/enemies/dynamitebot.png";
		sprite_size.x = 44;
		sprite_size.y = 48;
		intro_frames_n = 3;
		intro_frames_rollback = 1;
		botname = "#3 - DYNAMITE BOT";
		concept_author = "CONCEPT: KOBOLD SACCOL";
		design_author = "DESIGN: IURI FIEDORUK";
	} else if (pos_n == TECHNOBOT) {
		buffer = FILEPATH + "data/images/sprites/enemies/technobot.png";
		sprite_size.x = 51;
		sprite_size.y = 47;
		intro_frames_n = 3;
		botname = "#2 - TECHNO BOT";
		intro_frames_rollback = 1;
		concept_author = "CONCEPT: CHIKAGO SACCOL";
		design_author = "DESIGN: IURI FIEDORUK";
	} else if (pos_n == MUMMYBOT) {
		buffer = FILEPATH + "data/images/sprites/enemies/mummybot.png";
		sprite_size.x = 33;
		sprite_size.y = 33;
		intro_frames_n = 3;
		botname = "#6 - MUMMY BOT";
		concept_author = "CONCEPT: ARIS K.S.F.";
		design_author = "DESIGN: ARIS K.S.F.";
	} else if (pos_n == APEBOT) {
		buffer = FILEPATH + "data/images/sprites/enemies/apebot.png";
		sprite_size.x = 40;
		sprite_size.y = 39;
		intro_frames_n = 5;
		botname = "#1 - APE BOT";
		concept_author = "CONCEPT: IVAN FIEDORUK";
		design_author = "DESIGN: IURI FIEDORUK";
	} else if (pos_n == SEAHORSEBOT) {
		buffer = FILEPATH + "data/images/sprites/enemies/seahorsebot.png";
		sprite_size.x = 31;
		sprite_size.y = 38;
		intro_frames_n = 1;
		botname = "#7 - SEAHORSE BOT";
		concept_author = "CONCEPT: ARIS K.S.F.";
		design_author = "DESIGN: IURI FIEDORUK";
	} else if (pos_n == MAGEBOT) {
		buffer = FILEPATH + "data/images/sprites/enemies/magebot.png";
		sprite_size.x = 26;
		sprite_size.y = 33;
		intro_frames_n = 4;
		botname = "#8 - MAGE BOT";
		concept_author = "CONCEPT: ARIS K.S.F.";
		design_author = "DESIGN: ARIS K.S.F.";
	} else {
		return;
	}


	blank_screen();

	filename = FILEPATH + "data/images/backgrounds/stage_boss_intro.png";
	surfaceFromFile(filename, &spriteCopy);
	copyArea(st_position(0, 0), &spriteCopy, &gameScreen);

	filename = FILEPATH + "data/images/backgrounds/stage_boss_intro.png";
	surfaceFromFile(filename, &bgCopy);
	copyArea(st_position(0, 0), &bgCopy, &gameScreen);

	updateScreen();


	initSurface(st_size(sprite_size.x, sprite_size.y), &boss_graphics);
	surfaceFromFile(buffer, &boss_graphics);
	initSurface(st_size(sprite_size.x, sprite_size.y), &boss_bg);
	copyArea(st_rectangle(boss_pos.x, RES_H/2-boss_bg.height/2, sprite_size.x, sprite_size.y), st_position(0, 0), &bgCopy, &boss_bg);


	init_stars();

	boss_pos.y = RES_H/2-boss_bg.height/2;

	waitTimeWithStars(500);
	if (intro_frames_n > 0) {
		for (i=0; i<intro_frames_n; i++) {
			copyArea(st_rectangle(0, 0, spriteCopy.width, spriteCopy.height), st_position(boss_pos.x, boss_pos.y), &boss_bg, &gameScreen);
			copyArea(st_rectangle(i*sprite_size.x, 0, sprite_size.x, sprite_size.y), st_position(boss_pos.x, boss_pos.y), &boss_graphics, &gameScreen);
			waitTimeWithStars(200);
		}
		if (intro_frames_rollback == 1) {
			for (j=intro_frames_n-1; j>=0; j--) {
				copyArea(st_rectangle(0, 0, spriteCopy.width, spriteCopy.height), st_position(boss_pos.x, boss_pos.y), &boss_bg, &gameScreen);
				copyArea(st_rectangle(j*sprite_size.x, 0, sprite_size.x, sprite_size.y), st_position(boss_pos.x, boss_pos.y), &boss_graphics, &gameScreen);
				waitTimeWithStars(200);
			}
		}
	}
	draw_progressive_text(text_x, RES_H/2-15, botname, false, true);
	draw_progressive_text(text_x, RES_H/2-3, concept_author, false, true);
	draw_progressive_text(text_x, RES_H/2+9, design_author, false, true);

	waitTimeWithStars(2500);
}

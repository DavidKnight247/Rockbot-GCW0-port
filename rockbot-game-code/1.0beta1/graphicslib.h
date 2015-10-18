#ifndef GRAPHICSLIB_H
#define GRAPHICSLIB_H

#include <iostream>
#include <string>
#include <vector>
#include <map>


#include <SDL/SDL.h>				//Include da SDL
#include <SDL/SDL_image.h>		//Include da biblioteca SDL_Image
#include <SDL/SDL_ttf.h>		// Include da biblioteca SDL_ttf
#include <SDL/SDL_endian.h>

#include "defines.h"
#include "st_common.h"
#include "class_colorcycle.h"
#include "file/format.h"

#if !defined(PLAYSTATION2) && !defined(HANDHELD)
	#define SCALE 2
#endif


#define INTRO_STARS_NUMBER 10
#define ANIM_STARS_BIG_STEP 6
#define ANIM_STARS_STEP 4
#define ANIM_STARS_SMALL_STEP 2

// ****************************************************************************** //
// graphicsLib is a layer to handle graphics. for future ports that do not use    //
// SDL, you can clone this class and use other platform-specific methods          //
// ****************************************************************************** //



class graphicsLib
{
public:
    graphicsLib();
	~graphicsLib();

	bool initGraphics();
	void updateScreen();
	void loadTileset();
	void placeTile(struct st_position, struct st_position, struct graphicsLib_gSurface*);

	void copyArea(struct st_rectangle rect, struct st_position, struct graphicsLib_gSurface*, struct graphicsLib_gSurface*);
	void copyArea(struct st_position, struct graphicsLib_gSurface*, struct graphicsLib_gSurface*); // overload, use the whole width and height of given image

	void surfaceFromFile(std::string, graphicsLib_gSurface *);
	void showSurface(struct graphicsLib_gSurface*);
	void showSurfaceRegion(struct graphicsLib_gSurface*, const struct st_rectangle origin_rectangle);
	void showSurfaceRegionAt(struct graphicsLib_gSurface* surfaceOrigin, const struct st_rectangle origin_rectangle, struct st_position pos_destiny);
	void showSurfaceAt(struct graphicsLib_gSurface*, struct st_position, bool fix_colors);
	void initSurface(struct st_size, graphicsLib_gSurface *);
	struct graphicsLib_gSurface surfaceFromRegion(struct st_rectangle, struct graphicsLib_gSurface&);
	void blank_screen();
	void blank_surface(struct graphicsLib_gSurface& surface);
	void blank_area(short int x, short int y, short int w, short int h);
	void blank_area(short int x, short int y, short int w, short int h, struct graphicsLib_gSurface& surface);
	int draw_progressive_text(short int x, short int y, std::string text, bool interrupt, bool show_stars, bool update_screen=true);
	void draw_text(short int x, short int y, std::string text);
	void draw_text(short int x, short int y, std::string text, struct graphicsLib_gSurface& surface);

	void draw_centered_text(short int y, std::string text);
	void draw_centered_text(short int y, std::string text, struct graphicsLib_gSurface& surface);

	int getColorNumber(int r, int g, int b);
	void drawCursor(st_position);
	void eraseCursor(st_position);
	void blink_screen(int r, int g, int b);
	void load_icons();
	void draw_weapon_icon(enum WEAPON_ICONS_ENUM, st_position menu_pos, bool active);
	void draw_item_icon(enum ITEM_ICONS_ENUM, st_position pos);
	void draw_weapon_menu_bg(unsigned short int current_hp, format_v1_0::st_items* item_ref, graphicsLib_gSurface *player_frame);
	void show_ready();
	void draw_hp_bar(short int hp, short player_n, short weapon_n);
	void clear_area(short int x, short int y, short int w, short int h, short int r, short int g, short int b);
	void clear_surface_area(short int x, short int y, short int w, short int h, short int r, short int g, short int b, struct graphicsLib_gSurface& surface);
	void change_colormap(unsigned int color_key, st_color new_color);
	void set_colormap(int map_n);
	void show_config_bg(int position);
	void hide_config_bg(int position);
	void draw_weapon_cursor(st_position old_pos, short hp, short player_n);
	void boss_intro(int pos_n);
	void boss_credits(int pos_n);
	void show_dialog(int position, bool show_btn=false);
	void place_face(std::string face_file, st_position pos);
	st_position get_dialog_pos();
	void change_surface_color(st_color key, st_color new_color, struct graphicsLib_gSurface* surface);
	void change_surface_colormap(SDL_Color new_colormap[], struct graphicsLib_gSurface* surface);
    void init_stars();
    void anim_stars();
    bool waitTimeWithStars(int wait_period);
	void show_btn_a(st_position btn_pos);
	void set_screen_adjust(st_position adjust);
	void update_colors();

private:
	void copySDLArea(struct st_rectangle, struct st_position, SDL_Surface*, SDL_Surface*, bool fix_colors);
	SDL_Surface *SDLSurfaceFromFile(std::string filename);
	void scale2x(SDL_Surface *src, SDL_Surface *dst);
	void draw_horizontal_hp_bar(short int y_adjust, short int right, short int hp, short int player_n=3);
	void draw_horizontal_hp_bar(st_position pos, short int hp, short int player_n=3);
	void draw_vertical_hp_bar(short int player_n, short int weapon_n);
	void draw_star(short int x, short int y, int size);
	void erase_star(short int x, short int y, int size);
	void preload_faces();
	void unload_faces();


public:
	int RES_DIFF_W;
	int RES_DIFF_H;
	struct graphicsLib_gSurface gameScreen;
	st_position big_star_list[INTRO_STARS_NUMBER/2];
	st_position star_list[INTRO_STARS_NUMBER];
	st_position small_star_list[INTRO_STARS_NUMBER];


private:
	#define COLOR_COUNT 67
	SDL_Color colormap[COLOR_COUNT];
	SDL_Color colormap_original[COLOR_COUNT];
	class_colorcycle colorcycle_manager;
	unsigned int color_keys[3];
	std::map<std::string, graphicsLib_gSurface> FACES_SURFACES;
	// TODO: free those pointers
	TTF_Font *font;
	SDL_Surface *game_screen;									// we do not put this into a graphicsLib_gSurface because this is meant to be used only internally
#if defined (SCALE)
	SDL_Surface *game_screen_scaled;
#endif
	SDL_Surface *tileset;										// we do not put this into a graphicsLib_gSurface because this is meant to be used only internally
	std::vector<struct graphicsLib_gSurface> faces;				// faces for players and npcs
	std::vector<struct graphicsLib_gSurface> weapon_icons;		// weapon icons, used in menu and energy bars
	std::vector<struct graphicsLib_gSurface> small_weapon_icons;		// weapon icons, used in menu and energy bars
	std::vector<struct graphicsLib_gSurface> item_icons;		// item icons, used in menu and energy bars

	struct graphicsLib_gSurface ingame_menu;
	struct graphicsLib_gSurface config_menu;
	struct graphicsLib_gSurface dialog_surface;
	struct graphicsLib_gSurface config_menu_bg; /// backup of the background the menu will hide
	struct graphicsLib_gSurface ready_message;
	struct graphicsLib_gSurface _btn_a_surface;
	st_position dialog_pos;
	unsigned int stars_timer;
	bool _must_set_colors;

	st_position _screen_adjust;

	SDL_PixelFormat screen_pixel_format;

};

#endif // GRAPHICSLIB_H

#ifndef MEDIATOR_H
#define MEDIATOR_H
#include <QString>
#include "../file/format.h"


struct st_player_graphics_data {
	st_size frame_size;
	st_rectangle hit_area;
	std::string graphics_filename;

	st_player_graphics_data()
	{

		frame_size.width = 29;
		frame_size.heigth = 32;
		hit_area.x = 4;
		hit_area.y = 8;
		hit_area.w = 21;
		hit_area.h = 24;
		graphics_filename = std::string("p1.png");
	}
};

class Mediator {
private:
	int palleteX;
	int palleteY;
	char *selectedTileset;

public:
	char gameName[50];
	char centNumber[4];
	int currentGame;
	int currentMap;
	int currentStage;
	//QString *NpcPreviewFile;
	char addNpcFilename[512];
	int NpcAddNumber;
	int layerLevel;
	int selectedNPC;
	int editMode;
	int editTool;
	int editModeNPC;
	int frameset[NPC_FRAME_NUMBER];				// holds information about the type of each frame
	int frameset_time[NPC_FRAME_NUMBER];		// the number of milisseconds this frame leasts
	int object_type;
	int terrainType;
	int zoom;
	int link_type;
	int npc_direction;
	bool show_background_color;
	bool show_bg1;
	bool show_bg2;


	struct format_v1_0::list_map_npc *npc_map_list;
	struct format_v1_0::npc_static_data_list *static_npc_list;

	Mediator();
	int getPalleteX();
	int getPalleteY();
	void setPalleteX(int value);
	void setPalleteY(int value);
	char *getPallete();
	void setPallete(char *value);
	void initGameVar();
	void loadGame(int n);

	int get_stage_n(const int map_n);

	// auxiliary functions for map_links


	int npcGraphicSize_w;
	int npcGraphicSize_h;

	// projectile
	int projectileGraphicSize_w;
	int projectileGraphicSize_h;
	char addProjectileFilename[512];

	bool link_bidi;
	bool link_is_door;


	void createGame();
	void centNumberFormat(int n);
	void getGameName(int n);
	void resetMap(int);

	struct st_player_graphics_data player_graphics_data;

	int picked_color_n;
	st_color colormap[67]; /// @TODO: colormap should be a combination of three enums
	int current_weapon;
	int current_player;

};


#endif // MEDIATOR_H

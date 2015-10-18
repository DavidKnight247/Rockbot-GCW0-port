#ifndef MEDIATOR_H
#define MEDIATOR_H
#include <QString>
#include "format/file_structs.h"


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


	struct list_map_npc *npc_map_list;
	struct npc_static_data_list *static_npc_list;
	struct links_list *map_links;

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

	struct npc_static_data_list *add_list_npc();
	struct npc_static_data_list *add_static_npc();
	void loadGameNpcs (int n);
	struct npc_static_data_list *get_main_npc(int);

	struct st_file_stage stage;
	void loadStage(int game_n, int stage_n);
	void loadMap(int game_n, int map_n);
	void loadStageMap(int game_n, int stage_n, int map_n);
	void loadMapNpcs(int game_n, int stage_n, int map_n);
	void load_map_links(void);
	// auxiliary functions for map_links
	struct links_list *add_map_link(void);
	void remove_map_link(struct links_list *link);
	void saveMapLinks(void);


	int npcGraphicSize_w;
	int npcGraphicSize_h;

	// projectile
	int projectileGraphicSize_w;
	int projectileGraphicSize_h;
	char addProjectileFilename[512];


	void createGame();
	int freeSlot(char *dir, char *extension);
	void centNumberFormat(int n);
	void getGameName(int n);
	void addMap();
	void resetMap(int);
	void placeNpc(int posx, int posy, int npc_n);
	void addObject(char name[20], char filename[50], unsigned short int type, unsigned short int timer, unsigned short int limit, unsigned short int, unsigned short int);
	void placeObject(unsigned short int , unsigned short int, struct struct_object *obj);
	void loadGameObjects(int n);

};


#endif // MEDIATOR_H

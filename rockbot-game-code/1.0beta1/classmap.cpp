#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

#include "classmap.h"
#include "objects/object.h"
#include "character.h"
#include "character/classboss.h"
#include "colision_detection.h"

extern string FILEPATH;
extern graphicsLib graphLib;

#include "game.h"
extern game gameControl;

#include "inputlib.h"
extern inputLib input;

extern format_v_2_0_1::file_game game_data;
extern bool freeze_weapon_effect;



// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
classMap::classMap() : stage_number(-1), number(-1), bg1_scroll(0), bg2_scroll(0), _platform_leave_counter(0)
{

	//std::cout << "classMap::classMap ******** backgrounds[0].filename: " << game_data.stages[0].maps[0].backgrounds[0].filename << std::endl;
	//std::cout << "classMap::classMap ******** backgrounds[1].filename: " << game_data.stages[0].maps[0].backgrounds[1].filename << std::endl;

	int i;
	graphLib.initSurface(st_size(MAP_W*TILESIZE, MAP_H*TILESIZE), &mapSurface);
	for (i=0; i<MAP_W; i++) {
		for (int j=0; j<MAP_H; j++) {
			map_tiles.tiles[i][j].tile1.x = -1;
			map_tiles.tiles[i][j].tile1.y = -1;
		}
	}
	for (i=0; i<MAP_W; i++) {
		wall_scroll_lock[i] = false;
	}
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
classMap::~classMap()
{
	if (mapSurface.width != 0 && mapSurface.gSurface != NULL) {
		mapSurface.width = 0;
		SDL_FreeSurface(mapSurface.gSurface);
		mapSurface.gSurface = NULL;
	}
	clean_map();
	bg1_surface.freeGraphic();
	bg2_surface.freeGraphic();
}

void classMap::clean_map()
{
	// remove npc list
	// TODO: find a fix for this
	while (!npc_list.empty()) {
		delete npc_list.front();
		npc_list.pop_front();
	}
	_player_list.erase(_player_list.begin(), _player_list.end());
	object_list.erase(object_list.begin(), object_list.end());
    animation_list.erase(animation_list.begin(), animation_list.end());
	bg1_surface.freeGraphic();
	bg2_surface.freeGraphic();
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classMap::setStageNumber(int setStageN) {
	//std::cout << "classMap::setStageNumber - setStageN: " << setStageN << std::endl;
	stage_number = setStageN;
}

short classMap::get_stage_number()
{
	return stage_number;
}

short classMap::get_number()
{
	return number;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classMap::setMapNumber(int setMapN) {
	number = setMapN;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classMap::loadMap(std::map<std::string, object> &game_object_list)
{
	if (stage_number == -1) {
		cout << "ERROR::map::loadMap - stage number was not set, can't load it before setting the number.\n";
		return;
	}
	if (number == -1) {
		cout << "ERROR::map::loadMap - map number was not set, can't load it before setting the number.\n";
		return;
	}
	game_data.stages[stage_number].maps[number].backgrounds[0].speed = 0.0; // background #1 is ALWAYS static
/*
	//#ifdef DINGUX
		game_data.stages[stage_number].maps[number].backgrounds[0].speed = 0.0;
		game_data.stages[stage_number].maps[number].backgrounds[1].speed = 0.0;
	//#endif
*/

	clean_map();

	//std::cout << "loading map[" << number << "] from stage[" << stage_number << "]" << std::endl;
	for (int i=0; i<MAP_W; i++) {
		for (int j=0; j<MAP_H; j++) {
			map_tiles.tiles[i][j].locked = game_data.stages[stage_number].maps[number].tiles[i][j].locked;
			map_tiles.tiles[i][j].tile1.x = game_data.stages[stage_number].maps[number].tiles[i][j].tile1.x;
			map_tiles.tiles[i][j].tile1.y = game_data.stages[stage_number].maps[number].tiles[i][j].tile1.y;
			map_tiles.tiles[i][j].tile2.x = game_data.stages[stage_number].maps[number].tiles[i][j].tile2.x;
			map_tiles.tiles[i][j].tile2.y = game_data.stages[stage_number].maps[number].tiles[i][j].tile2.y;
			map_tiles.tiles[i][j].tile3.x = game_data.stages[stage_number].maps[number].tiles[i][j].tile3.x;
			map_tiles.tiles[i][j].tile3.y = game_data.stages[stage_number].maps[number].tiles[i][j].tile3.y;
		}
	}
	bool column_locked = true;
	for (int i=0; i<MAP_W; i++) {
		column_locked = true;
		for (int j=0; j<MAP_H; j++) {
			if (map_tiles.tiles[i][j].locked != 1 && map_tiles.tiles[i][j].locked != TERRAIN_DOOR && map_tiles.tiles[i][j].locked != TERRAIN_SCROLL_LOCK) {
				column_locked = false;
				break;
			}
		}
		wall_scroll_lock[i] = column_locked;
	}

	load_map_npcs();

	load_map_objects(game_object_list);

	if (strlen(game_data.stages[stage_number].maps[number].backgrounds[0].filename) > 0) {
		std::string bg1_filename(FILEPATH+"data/images/map_backgrounds/" + game_data.stages[stage_number].maps[number].backgrounds[0].filename);
		//std::cout << "stage_number: " << stage_number << ", map_number: " << number << ", bg1_filename: " << bg1_filename.c_str() << std::endl;
		graphLib.surfaceFromFile(bg1_filename, &bg1_surface);
	}
	if (strlen(game_data.stages[stage_number].maps[number].backgrounds[1].filename) > 0) {
		std::string bg2_filename(FILEPATH+"data/images/map_backgrounds/"+ game_data.stages[stage_number].maps[number].backgrounds[1].filename);
		std::cout << "classmap::loadMap - loading bg2: '" << bg2_filename << std::endl;
		std::cout << "bg2_filename: " << bg2_filename.c_str() << std::endl;
		graphLib.surfaceFromFile(bg2_filename, &bg2_surface);
	}


	drawMap();
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classMap::drawMap()
{

	if (bg1_surface.width > 0 && game_data.stages[stage_number].maps[number].backgrounds[0].speed == 0.0) {
		for (int i=0; i<mapSurface.width/bg1_surface.width+1; i++) {
			graphLib.copyArea(st_position(i*bg1_surface.width, game_data.stages[stage_number].maps[number].backgrounds[0].adjust_y), &bg1_surface, &mapSurface);
		}
		bg1_surface.freeGraphic();
	}
	if (bg2_surface.width > 0 && game_data.stages[stage_number].maps[number].backgrounds[1].speed == 0.0) {
		std::cout << "classmap::loadMap - showing bg2 as static" << std::endl;
		for (int i=0; i<mapSurface.width/bg2_surface.width+1; i++) {
			graphLib.copyArea(st_position(i*bg2_surface.width, game_data.stages[stage_number].maps[number].backgrounds[1].adjust_y), &bg2_surface, &mapSurface);
		}
		bg2_surface.freeGraphic();
	}
	struct st_position pos_origin;
	struct st_position pos_destiny;
	for (int i=0; i<MAP_W; i++) {
		for (int j=0; j<MAP_H; j++) {
			// level 1
			pos_origin.x = map_tiles.tiles[i][j].tile1.x;
			pos_origin.y = map_tiles.tiles[i][j].tile1.y;
			pos_destiny.x = i*TILESIZE;
			pos_destiny.y = j*TILESIZE;
			graphLib.placeTile(pos_origin, pos_destiny, &mapSurface);
			// level 2
			pos_origin.x = map_tiles.tiles[i][j].tile2.x;
			pos_origin.y = map_tiles.tiles[i][j].tile2.y;
			graphLib.placeTile(pos_origin, pos_destiny, &mapSurface);
		}
	}
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classMap::showMap()
{
	std::list<animation>::iterator animation_it;
	struct st_rectangle origin_rectangle;
	int i=0;

	origin_rectangle.x = scroll.x;
	origin_rectangle.y = scroll.y;
	origin_rectangle.w = origin_rectangle.x + RES_W;
	origin_rectangle.h = origin_rectangle.y + RES_H;


	draw_dynamic_backgrounds();
	graphLib.showSurfaceRegion(&mapSurface, origin_rectangle);
	i=0;



	/// @TODO: remove "finished" animations
    for (animation_it = animation_list.begin(); animation_it != animation_list.end(); animation_it++) {
        if ((*animation_it).finished() == true) {
            animation_list.erase(animation_it);
            break;
        } else {
            (*animation_it).execute(); // TODO: must pass scroll map to npcs somwhow...
        }
    }
}

// ********************************************************************************************** //
// show the third level of tiles                                                                  //
// ********************************************************************************************** //
void classMap::showAbove()
{
	// draw 3rd tile level
	struct st_position pos_origin;
	struct st_position pos_destiny;
	for (int i=0; i<MAP_W; i++) {
		for (int j=0; j<MAP_H; j++) {
			if (map_tiles.tiles[i][j].tile3.x != -1 && map_tiles.tiles[i][j].tile3.y != -1) {
				pos_origin.x = map_tiles.tiles[i][j].tile3.x;
				pos_origin.y = map_tiles.tiles[i][j].tile3.y;
				pos_destiny.x = (i*TILESIZE)-scroll.x;
				pos_destiny.y = (j*TILESIZE)-scroll.y;
				graphLib.placeTile(pos_origin, pos_destiny, &graphLib.gameScreen);
			}
		}
	}
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
int classMap::getMapPointLock(st_position pos)
{
	if (pos.x < 0 || pos.y < 0 || pos.y > RES_H/TILESIZE || pos.x > MAP_W) {
		return TERRAIN_UNBLOCKED;
	}
	return map_tiles.tiles[pos.x][pos.y].locked;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classMap::changeScrolling(st_position pos, bool check_lock)
{
	if (pos.x != 0) {
		//scrolled = pos;
	}
	float bg1_speed = (float)game_data.stages[stage_number].maps[number].backgrounds[0].speed/10;
	float bg2_speed = (float)game_data.stages[stage_number].maps[number].backgrounds[1].speed/10;

	//std::cout << fixed << "bg1_speed: " << bg1_speed << std::endl;
	//std::cout << fixed << "bg2_speed: " << bg2_speed << std::endl;

	// moving player to right, screen to left
	if (pos.x > 0 && ((scroll.x/TILESIZE+RES_W/TILESIZE)-1 < MAP_W)) {
		if (pos.x < TILESIZE) { // if change is too big, do not update (TODO: must check all wall until lock)
			int tile_x = (scroll.x+RES_W-TILESIZE+2)/TILESIZE;
			if (check_lock == false || wall_scroll_lock[tile_x] == false) {
				//std::cout << "classMap::changeScrolling - 1 - tile-tested: " << ((scroll.x/TILESIZE+RES_W/TILESIZE)-1) << std::endl;
				scroll.x += pos.x;
				bg1_scroll -= ((float)pos.x*bg1_speed);
				//std::cout << fixed << "bg1_scroll: " << bg1_scroll << std::endl;
				if (bg1_scroll < -bg1_surface.width) {
					bg1_scroll = 0;
				}
				bg2_scroll -= ((float)pos.x*bg2_speed);
				//std::cout << "Change RIGHT-BG2POS to:" << bg2_scroll << std::endl;
				if (bg2_scroll < -bg2_surface.width) {
					//std::cout << "RIGHT RESET BG2POS" << bg2_scroll << std::endl;
					bg2_scroll = 0;
				}
			}
		}
	} else if (pos.x < 0) {
		if (pos.x > -TILESIZE && scroll.x/TILESIZE >= 0) { // if change is too big, do not update (TODO: must check all wall until lock)
			int tile_x = (scroll.x+TILESIZE-2)/TILESIZE;
			//std::cout << "changeScrolling - scroll.x: " << scroll.x << ", testing tile_x: " << tile_x << std::endl;
			if (check_lock == false || wall_scroll_lock[tile_x] == false) {
				//std::cout << "classMap::changeScrolling - 2" << std::endl;
				scroll.x += pos.x;
				bg1_scroll -= ((float)pos.x*bg1_speed);
				if (bg1_scroll > bg1_surface.width) {
					bg1_scroll = 0;
				}
				bg2_scroll -= ((float)pos.x*bg2_speed);
				//std::cout << "Change LEFT-BG2POS to:" << bg2_scroll << std::endl;
				if (bg2_scroll > bg2_surface.width) {
					//std::cout << "LEFT RESET BG2POS" << bg2_scroll << std::endl;
					bg2_scroll = 0;
				}
			}
		}
	}

	scroll.y += pos.y;
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classMap::set_scrolling(st_position pos)
{
	//std::cout << "------- classMap::set_scrolling - map: " << number << ", pos.x: " << pos.x << "-------" << std::endl;
	scrolled = pos;
	scroll.x = pos.x;
	scroll.y = pos.y;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
st_position classMap::getMapScrolling()
{
    return scroll;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classMap::load_map_npcs()
{
	struct format_v1_0::st_file_map_npc temp_map_npc;
	classnpc* new_npc;

	// remove all elements currently in the list
	std::list<classnpc*>::iterator it;
	for (it=npc_list.begin(); it != npc_list.end(); it++) {
		npc_list.erase(it);
	}


	//std::cout << "classmap::load_map_npcs - stage: " << stage_number << ", map: " << number << std::endl;

	for (int i=0; i<MAX_MAP_NPC_N; i++) {
		if (game_data.stages[stage_number].maps[number].map_npcs[i].id_npc != -1) {


			if (game_data.stages[stage_number].boss.id_npc == game_data.stages[stage_number].maps[number].map_npcs[i].id_npc) {
				new_npc = new classboss(stage_number, number, game_data.stages[stage_number].maps[number].map_npcs[i].id_npc, i);
			} else {
				new_npc = new classnpc(stage_number, number, game_data.stages[stage_number].maps[number].map_npcs[i].id_npc, i);
			}
			new_npc->set_map(this);
			new_npc->set_player_list(&_player_list);
			npc_list.push_back(new_npc); // insert new npc at the list-end
		}
	}
}

void classMap::draw_dynamic_backgrounds()
{
	//std::cout << "classMap::draw_dynamic_backgrounds stage_number: " << stage_number << ", map_number: " << number << ", bg.r: " << game_data.stages[stage_number].maps[number].background_color.r << ", bg.g: " << game_data.stages[stage_number].maps[number].background_color.g << ", bg.b: " << game_data.stages[stage_number].maps[number].background_color.b << std::endl;
	graphLib.clear_surface_area(0, 0, mapSurface.width, mapSurface.height, game_data.stages[stage_number].maps[number].background_color.r, game_data.stages[stage_number].maps[number].background_color.g, game_data.stages[stage_number].maps[number].background_color.b, graphLib.gameScreen);
	if (bg1_surface.width > 0 && game_data.stages[stage_number].maps[number].backgrounds[0].speed != 0) {
		int repeat_n = RES_W/bg1_surface.width+1;
		if (repeat_n <= 2) {
			repeat_n = 3;
		}
		for (int i=-1; i<repeat_n; i++) {
			graphLib.copyArea(st_position(bg1_scroll+i*bg1_surface.width, game_data.stages[stage_number].maps[number].backgrounds[0].adjust_y), &bg1_surface, &graphLib.gameScreen);
		}
	}
	if (bg2_surface.width > 0 && game_data.stages[stage_number].maps[number].backgrounds[1].speed != 0) {
		int repeat_n = RES_W/bg2_surface.width+1;
		if (repeat_n <= 2) {
			repeat_n = 3;
		}
		//std::cout << "classmap::loadMap - showing bg2 as DYNAMIC - repeat_n: " << repeat_n << std::endl;
		for (int i=-1; i<repeat_n; i++) {
			//int pos_x = bg2_scroll+i*bg2_surface.width;
			//std::cout << "classMap::draw_dynamic_backgrounds - draw bg2 YES - bg2_scroll: " << bg2_scroll << ", pos_x: " << (bg2_scroll+i*bg2_surface.width) << ", width: " << bg2_surface.width << std::endl;
			graphLib.copyArea(st_position(bg2_scroll+i*bg2_surface.width, game_data.stages[stage_number].maps[number].backgrounds[1].adjust_y), &bg2_surface, &graphLib.gameScreen);
		}
	}
}



// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classMap::load_map_objects(std::map<std::string, object> &game_object_list) {
	std::map<std::string, object>::iterator it;

	// remove all elements currently in the list
	std::list<object>::iterator it_main_list;
	for (it_main_list=object_list.begin(); it_main_list != object_list.end(); it_main_list++) {
		object_list.erase(it_main_list);
	}
    std::list<animation>::iterator it_animation;
    for (it_animation=animation_list.begin(); it_animation != animation_list.end(); it_animation++) {
        animation_list.erase(it_animation);
    }

	for (int i=0; i<MAX_MAP_NPC_N; i++) {
		if (game_data.stages[stage_number].maps[number].map_objects[i].id_object != -1) {
            //int temp_id = game_data.stages[stage_number].maps[number].map_objects[i].id_object;
			std::string temp_id_str = cent_number(game_data.stages[stage_number].maps[number].map_objects[i].id_object);
			it = game_object_list.find(temp_id_str);
			if (it != game_object_list.end()) {
				//std::cout << "********* temp_id_str: " << temp_id_str << ", id_object: " << game_data.stages[stage_number].maps[number].map_objects[i].id_object << ", x: " << game_data.stages[stage_number].maps[number].map_objects[i].start_point.x << ", y: " << game_data.stages[stage_number].maps[number].map_objects[i].start_point.y << std::endl;

				//object temp_obj(game_data.stages[stage_number].maps[number].map_objects[i].id_object, game_object_list.at(temp_id_str), this, game_data.stages[stage_number].maps[number].map_objects[i].start_point);
				object temp_obj(game_data.stages[stage_number].maps[number].map_objects[i].id_object, (*it).second, this, game_data.stages[stage_number].maps[number].map_objects[i].start_point);
				object_list.push_back(temp_obj);
			}
		}
	}
}



std::string classMap::cent_number(int n) {
	std::stringstream ss;

	if (n < 10) {
		ss << "00" << n;
	} else if (n < 100) {
		ss << "0" << n;
	} else {
		ss << n;
	}
	return ss.str();
}

st_position classMap::get_last_scrolled()
{
	return scrolled;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void classMap::reset_scrolled()
{
	scrolled.x = 0;
	scrolled.y = 0;
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
object_colision classMap::colision_player_object(character* playerObj, const short int x_inc, const short int y_inc, short int reduce_x, short int reduce_y)
{
	//struct st_position p1, p2, p3, p4;
	//struct st_position obj1, obj2, obj3, obj4;
	//short int passX=0, passY=0;
	int blocked = 0;
	std::list<object>::iterator it;
	object* res_obj = NULL;


	/// @TODO: decrease y_inc til 1
	colision_detection rect_colision_obj;

	// only check all platforms if player it not above one
	if (playerObj->get_platform() == NULL) {
		for (it=object_list.begin(); it!=object_list.end(); it++) {
			object& temp_obj = (*it);
			st_rectangle obj_rect(temp_obj.get_position().x, temp_obj.get_position().y, temp_obj.get_size().width, temp_obj.get_size().heigth);

			if (x_inc > 0) {
				st_rectangle p_rect(playerObj->getPosition().x, playerObj->getPosition().y, playerObj->get_size().width+x_inc-reduce_x, playerObj->get_size().heigth);
				if (rect_colision_obj.rect_overlap(obj_rect, p_rect) == true) {
					blocked = 1;
				}
			} else if (x_inc < 0) {
				st_rectangle p_rect(playerObj->getPosition().x+x_inc-reduce_x-1, playerObj->getPosition().y, playerObj->get_size().width-reduce_x-1, playerObj->get_size().heigth);
				if (rect_colision_obj.rect_overlap(obj_rect, p_rect) == true) {
					blocked = 1;
				}
			}
			// fall over platform
			short temp_py = playerObj->getPosition().y+playerObj->get_size().heigth;
			if (y_inc > 0 && temp_py >= temp_obj.get_position().y && temp_py < temp_obj.get_position().y+temp_obj.get_size().heigth) {
				st_rectangle p_rect(playerObj->getPosition().x, playerObj->getPosition().y, playerObj->get_size().width, playerObj->get_size().heigth+y_inc);
				if (rect_colision_obj.rect_overlap(obj_rect, p_rect) == true) {
					if (blocked == 0) {
						blocked = 2;
					} else {
						blocked = 3;
					}
				}
			}



			if (temp_obj.get_type() == OBJ_MOVING_PLATFORM_UPDOWN || temp_obj.get_type() == OBJ_MOVING_PLATFORM_LEFTRIGHT || temp_obj.get_type() == OBJ_DISAPPEARING_BLOCK) {
				if (playerObj->get_platform() == NULL && blocked == 2 || blocked == 3) {
					playerObj->set_platform(&temp_obj);
					if (temp_obj.get_type() == OBJ_FALL_PLATFORM) {
						temp_obj.set_direction(ANIM_DIRECTION_LEFT);
					}
				} else if (playerObj->get_platform() == NULL && blocked == 1) {
					playerObj->set_platform(&temp_obj);
				}
				if (blocked != 0) {
					return object_colision(blocked, &(*it));
				}
			} else if (temp_obj.get_type() == OBJ_ITEM_FLY) {
				//printf(">> testing fly, p4.y: %d, obj.y: %d <<\n", p4.y, obj1.y);
				if (playerObj->get_platform() == NULL && (blocked == 2 || blocked == 3) && y_inc > 0) {
					playerObj->set_platform(&temp_obj);
					if (temp_obj.get_distance() == 0) {
						temp_obj.set_distance(1);
						temp_obj.set_timer(timer.getTimer()+30);
					}
				}
				if (blocked != 0) {
					return object_colision(blocked, &(*it));
				}
			} else if (temp_obj.get_type() == OBJ_ITEM_JUMP) {
				//printf(">> testing fly, p4.y: %d, obj.y: %d <<\n", p4.y, obj1.y);
				if (playerObj->get_platform() == NULL && (blocked == 2 || blocked == 3) && y_inc > 0) {
					playerObj->set_platform(&temp_obj);
					if (temp_obj.get_distance() == 0) {
						temp_obj.set_distance(1);
						temp_obj.set_direction(ANIM_DIRECTION_UP);
						temp_obj.set_timer(timer.getTimer()+30);
					}
				}
				if (blocked != 0) {
					return object_colision(blocked, &(*it));
				}
			} else if (playerObj->get_platform() == NULL && (blocked == 2 || blocked == 3) && temp_obj.get_type() == OBJ_FALL_PLATFORM) {
				playerObj->set_platform(&temp_obj);
				if (temp_obj.get_state() == OBJ_STATE_STAND) {
					temp_obj.set_state(OBJ_STATE_MOVE);
				}
				temp_obj.set_timer(timer.getTimer()+30);
				return object_colision(blocked, &(*it));
			} else if (playerObj->get_platform() == NULL && (blocked == 2 || blocked == 3) && temp_obj.get_type() == OBJ_FLY_PLATFORM) {
				playerObj->set_platform(&temp_obj);
				temp_obj.set_timer(timer.getTimer()+30);
				if (temp_obj.get_state() == OBJ_STATE_STAND) {
					temp_obj.set_state(OBJ_STATE_MOVE);
				}
				return object_colision(blocked, &(*it));
			} else if (blocked != 0) {
				res_obj = &(*it);
				break;
			}
		}
	} else {
		object* temp_obj = playerObj->get_platform();
		st_rectangle obj_rect(temp_obj->get_position().x, temp_obj->get_position().y, temp_obj->get_size().width, temp_obj->get_size().heigth);
		short temp_py = playerObj->getPosition().y+playerObj->get_size().heigth;
		if (y_inc > 0 && temp_py >= temp_obj->get_position().y && temp_py < temp_obj->get_position().y+temp_obj->get_size().heigth) {
			st_rectangle p_rect(playerObj->getPosition().x, playerObj->getPosition().y, playerObj->get_size().width, playerObj->get_size().heigth+y_inc);
			if (rect_colision_obj.rect_overlap(obj_rect, p_rect) == true) {
				if (blocked == 0) {
					blocked = 2;
				} else {
					blocked = 3;
				}
			}
		}
	}


	//std::cout << "classMap::colision_player_object - blocked: " << blocked << std::endl;
	// got out of platform
	if (blocked == 0 && playerObj->get_platform() != NULL) {
		//  for player item, platform must only be removed only if the item was already adtivated
		if (playerObj->get_platform()->get_type() == OBJ_ITEM_FLY || playerObj->get_platform()->get_type() == OBJ_ITEM_JUMP) {
			if (playerObj->get_platform()->get_distance() > 0 && y_inc != 0) {
				printf("colision_player_object: got out of jump/fly item, blocked: %d, type: %d, direction: %d\n", blocked, playerObj->get_platform()->get_type(), playerObj->get_platform()->get_direction());
				playerObj->set_platform(NULL);
			} else {
				return object_colision(0, NULL);
			}
		} else {
			_platform_leave_counter++;
			if (_platform_leave_counter > 40) {
				if ((playerObj->get_platform()->get_type() == OBJ_FALL_PLATFORM || playerObj->get_platform()->get_type() == OBJ_FLY_PLATFORM) && playerObj->get_platform()->get_state() == OBJ_STATE_MOVE) {
					playerObj->get_platform()->set_state(OBJ_STATE_RETURN);
				}
				printf("colision_player_object: got out of platform, xinc: %d, yinc: %d, type: %d\n", x_inc, y_inc, playerObj->get_platform()->get_type());
				playerObj->set_platform(NULL);
				_platform_leave_counter = 0;
			}
		}
	} else if (blocked != 0 && playerObj->get_platform() != NULL) {
		_platform_leave_counter = 0;
	}
	return object_colision(blocked, res_obj);
}


void classMap::clean_map_npcs_projectiles()
{
	std::list<classnpc*>::iterator it;
	for (it=npc_list.begin(); it!=npc_list.end(); it++) {
		(*it)->clean_projectiles();
	}
}

graphicsLib_gSurface* classMap::get_map_surface()
{
	return &mapSurface;
}

bool classMap::get_map_point_wall_lock(int x)
{
	return wall_scroll_lock[x/TILESIZE];
}

void classMap::move_map(const short int move_x, const short int move_y)
{
	set_scrolling(st_position(scroll.x+move_x, scroll.y+move_y));
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
short int classMap::colision_player_npcs(character* playerObj, const short int x_inc, const short int y_inc, short int reduce_x, short int reduce_y)
{
	struct st_position p1, p2, p3, p4;
	struct st_position obj1, obj2, obj3, obj4;
	short int passX=0, passY=0;
	int blocked = 0;
	std::list<classnpc*>::iterator npc_it;


	//reduce = abs(16-playerObj->sprite->w)*0.5;

	// ponto 3, topo/esquerda
	p1.x = playerObj->getPosition().x + reduce_x;
	p1.y = playerObj->getPosition().y;

	//printf(">> colision_player_object, p1.x: %d, p1.y: %d, x_inc: %d, y_inc: %d\n", p1.x, p1.y, x_inc, y_inc);


	// ponto 2, baixo/esquerda
	p2.x = playerObj->getPosition().x + reduce_x;
	p2.y = playerObj->getPosition().y + 31;

	// ponto 3, topo/direita
	p3.x = playerObj->getPosition().x + playerObj->get_size().width - reduce_x;
	p3.y = playerObj->getPosition().y;

	// ponto 4, baixo/direita
	p4.x = playerObj->getPosition().x + playerObj->get_size().width - reduce_x;
	p4.y = playerObj->getPosition().y + 31;

	// fix for ground checking
	p1.y = p1.y - 1;
	p2.y = p2.y - 1;
	p3.y = p3.y - 1;
	p4.y = p4.y - 1;

	//std::cout << "colision_player_npcs - p1.x: " << p1.x << ", p1.y: " << p1.y << std::endl;

	for (npc_it = npc_list.begin(); npc_it != npc_list.end(); npc_it++) {
		classnpc* temp_obj = (*npc_it);
		if ((*npc_it)->is_player_friend() == true) {
			continue;
		}
		obj1.x = temp_obj->getPosition().x;
		obj1.y = temp_obj->getPosition().y;
		obj2.x = temp_obj->getPosition().x;
		obj2.y = temp_obj->getPosition().y + temp_obj->get_size().heigth;
		obj3.x = temp_obj->getPosition().x + temp_obj->get_size().width;
		obj3.y = temp_obj->getPosition().y;
		obj4.x = temp_obj->getPosition().x + temp_obj->get_size().width;
		obj4.y = temp_obj->getPosition().y + temp_obj->get_size().heigth;

		//std::cout << "colision_player_npcs - npc.x: " << obj1.x << ", npc.y: " << obj1.y << std::endl;

		// any point inside object is a hit
		passY = 0;
		passX = 0;

		// check if player is on the X range of object
		if (playerObj->get_size().width-reduce_x*2 > temp_obj->get_size().width) {
			if ((obj1.x >= p1.x && obj1.x <= p3.x) || (obj3.x >= p1.x && obj3.x <= p3.x)) {
				passX = 1;
			}
		} else {
			if ((p1.x >= obj1.x && p1.x <= obj3.x) || (p3.x >= obj1.x && p3.x <= obj3.x)) {
				passX = 1;
			}
		}

		// check if player is on the Y range of object
		if (playerObj->get_size().heigth > temp_obj->get_size().heigth) {
			if ((obj1.y >= p1.y && obj1.y <= p2.y) || (obj2.y >= p1.y && obj2.y <= p2.y)) {
				passY = 1;
			}
		} else {
			if ((p1.y >= obj1.y && p1.y <= obj2.y) || (p2.y >= obj1.y && p2.y <= obj3.y)) {
				passY = 1;
			}
		}

		//std::cout << "<<<<<<<<<<<<<<< passX: " << passX << ", passY: " << passY << std::endl;

		// player is on the left of the object, and moving to right
		if (x_inc < 0 && passY && p1.x >= obj4.x && p1.x+x_inc <= obj4.x) {
			//printf("block X1\n");
			blocked = 1;
		}

		// player is on the right of the object, and moving to left
		if (x_inc > 0 && passY && p3.x <= obj1.x && p3.x+1 >= obj1.x) {
			//printf(">> block X2\n");
			blocked = 1;
		}

		//std::cout << "classMap::colision_player_object - passX: " << passX << std::endl;

		// player is under the object and moving to top
		if (passX && p1.y >= obj4.y && p1.y+y_inc <= obj4.y) {
			if (blocked == 0) {
				blocked = 2;
			} else {
				blocked = 3;
			}
		}
		// player is over the object and moving to bottom
		if (passX && p4.y <= obj1.y && p4.y+y_inc >= obj1.y) {
			if (blocked == 0) {
				blocked = 2;
			} else {
				blocked = 3;
			}
		}

		if (passX != 0 && passY != 0) {
			return 1;
		}
	}
	return 0;
}

/// @TODO: fix animation. investigate a better way for drawing it (code is way too confusing)
void classMap::redraw_boss_door(bool is_close, int nTiles, int tileX, short player_number) {
	int k, tileCount;
	//is_close = false; // THIS IS A TEMPORARY FIX

	//std::cout << "classMap::redraw_boss_door - is_close: " << is_close << std::endl;

	input.waitTime(10);
	//for (k=0; k<tilePieces; k++) {
	for (k=0; k<nTiles; k++) {
		tileCount = nTiles;
		//if (is_close == false) { std::cout << "classMap::redraw_boss_door - nTiles: " << nTiles << ", tilePieces: " << tilePieces << ", tileCount: " << tileCount << std::endl; }
		// redraw screen
		showMap();
		graphLib.updateScreen();
		int tiles_showed;
		if (is_close == false) {
			tiles_showed = k;
		} else {
			tiles_showed = 0;
		}
		for (int i=0; i<MAP_W; i++) {
			for (int j=0; j<MAP_H; j++) {
				if (map_tiles.tiles[i][j].tile3.x != -1 && map_tiles.tiles[i][j].tile3.y != -1) {
						if (i == tileX && map_tiles.tiles[i][j].locked == TERRAIN_DOOR) {
							//std::cout << "****** redraw_boss_door - k: " << k << ", tiles_showed: " << tiles_showed << ", nTiles: " << nTiles << std::endl;
							if (is_close == false) {
								if (tiles_showed < nTiles) {
									graphLib.placeTile(st_position(map_tiles.tiles[i][j].tile3.x, map_tiles.tiles[i][j].tile3.y), st_position((i*TILESIZE)-scroll.x, (j*TILESIZE)-scroll.y), &graphLib.gameScreen);
									graphLib.updateScreen();
									tiles_showed++;
								}
							} else {
								if (tiles_showed < k) {
									graphLib.placeTile(st_position(map_tiles.tiles[i][j].tile3.x, map_tiles.tiles[i][j].tile3.y), st_position((i*TILESIZE)-scroll.x, (j*TILESIZE)-scroll.y), &graphLib.gameScreen);
									graphLib.updateScreen();
									tiles_showed++;
								}
							}
						} else {
							graphLib.placeTile(st_position(map_tiles.tiles[i][j].tile3.x, map_tiles.tiles[i][j].tile3.y), st_position((i*TILESIZE)+scroll.x, (j*TILESIZE)-scroll.y), &graphLib.gameScreen);
						}
				}
			}
		}
		_player_list.at(0)->show();
		graphLib.draw_hp_bar(_player_list.at(0)->get_current_hp(), player_number, WEAPON_DEFAULT);
		//show_sprite(p1Obj->sprite, game_screen);
		//draw_hp_bar(p1Obj);
		graphLib.updateScreen();
		input.waitTime(100);
	}
	input.waitTime(100);
}

void classMap::add_animation(ANIMATION_TYPES pos_type, std::string filename, const st_position &pos, st_position adjust_pos, unsigned int frame_time, unsigned int repeat_times, int direction, st_size framesize)
{
	animation_list.push_back(animation(pos_type, filename, pos, adjust_pos, frame_time, repeat_times, direction, framesize, &scroll));
}

void classMap::set_player_list(std::vector<character*> player_list)
{
	_player_list = player_list;
}

void classMap::spawn_map_npc(short npc_id, st_position npc_pos, short int direction)
{
	classnpc* new_npc;
	new_npc = new classnpc(stage_number, number, npc_id, npc_pos, direction);
	new_npc->set_map(this);
	new_npc->set_player_list(&_player_list);
	npc_list.push_back(new_npc); // insert new npc at the list-end
}

void classMap::move_npcs() /// @TODO - check out of screen
{
	int i = 0;
	std::list<classnpc*>::iterator npc_it;
	//std::cout << "*************** classMap::showMap - npc_list.size: " << npc_list.size() << std::endl;
	for (npc_it = npc_list.begin(); npc_it != npc_list.end(); npc_it++) {
		//std::cout << ">>>>>>>>>>>>>>>>>> classMap::showMap - executing npc[" << i << "] '" << (*npc_it)->getName() << "'" << std::endl;
		bool temp_is_boss = (*npc_it)->is_boss();

		if (freeze_weapon_effect == false) {
			(*npc_it)->execute(); // TODO: must pass scroll map to npcs somwhow...
		}
		//(*npc_it)->show();
		if ((*npc_it)->is_dead() == true) {
			std::string temp_name = (*npc_it)->getName();
			if (!temp_is_boss) {
				add_animation(ANIMATION_STATIC, std::string("explosion_32.png"), st_position((*npc_it)->getPosition().x, (*npc_it)->getPosition().y), st_position(-8, -8), 80, 2, (*npc_it)->get_direction(), st_size(32, 32));
			}
			//std::cout << "Stage[" << stage_number << "].map[" << number << "] - Killed npc[" << i << "] '" << (*npc_it)->getName() << "'" << ", is_boss: " << temp_is_boss << std::endl;
			st_position npc_pos = (*npc_it)->get_real_position();
			delete (*npc_it);
			npc_list.erase(npc_it);
			if (temp_is_boss) {
				//std::cout << "classMap::showMap - killed boss" << std::endl;
				gameControl.draw_explosion(npc_pos.x, npc_pos.y, true);
				gameControl.got_weapon(temp_name);
			}
			return;
		}
		i++;
	}
}

void classMap::show_npcs() /// @TODO - check out of screen
{
	std::list<classnpc*>::iterator npc_it;
	//std::cout << "*************** classMap::showMap - npc_list.size: " << npc_list.size() << std::endl;
	for (npc_it = npc_list.begin(); npc_it != npc_list.end(); npc_it++) {
		//std::cout << ">>>>>>>>>>>>>>>>>> classMap::showMap - executing npc[" << i << "] '" << (*npc_it)->getName() << "'" << std::endl;
		(*npc_it)->show();
	}
}

void classMap::move_objects()
{
	std::list<object>::iterator object_it;
	for (object_it = object_list.begin(); object_it != object_list.end(); object_it++) {
		if ((*object_it).finished() == true) {
			object_list.erase(object_it);
			break;
		} else {
			(*object_it).execute(); // TODO: must pass scroll map to npcs somwhow...
		}
	}
}



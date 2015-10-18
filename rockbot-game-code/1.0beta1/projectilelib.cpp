#include "projectilelib.h"
#include "timerlib.h"
#include "classmap.h"
#include <math.h>

#include "soundlib.h"
extern soundLib soundManager;

extern timerLib timer;
extern format_v_2_0_1::file_game game_data;

#define QUAKE_TIMER 80
#define QUAKE_SCREEN_MOVE 5
extern bool freeze_weapon_effect;
#define FREEZE_DURATION 3500

#include "game.h"
extern game gameControl;


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
projectile::projectile(int projectile_id, int set_direction, int set_speed, st_position set_position, classMap *set_map, int damage, short int move_type) : animation_pos(0), animation_timer(0), animation_speed(100), diagonal_flag(false), _max_frames(1), angle(0.0), status(0), _effect_timer(0), _points(1)
{
    _id = projectile_id;
	map = set_map;
	direction = set_direction;
	speed = set_speed;
	position = set_position;
	move_timer = 0;
	move_delay = 20;
	is_finished = false;
	// TODO: the graphic must be determined by a type that comes from editor-data
	std::string filename = FILEPATH + std::string("/data/images/projectiles/") + std::string(game_data.projectiles[_id].graphic_filename);
    load_graphic(filename);
    _size = game_data.projectiles[_id].size;
    position.y -= _size.heigth/2;
	animation_timer = timer.getTimer() + animation_speed; // used to control each frame duration
	//std::cout << "projectile::START - position.x: " << position.x << ", position.y: " << position.y << std::endl;
    _damage = damage;
	_move_type = move_type;
	//std::cout << ">> projectile::projectile - _move_type: " << _move_type << std::endl;
	_max_frames = graphic_surface.width / _size.width;

	position0.x = position.x;
	position0.y = position.y;
	position1.x = position.x;
	position1.y = position.y;

	if (_id == 10) {
		soundManager.play_sfx(SFX_PLAYER_CHARGED_SHOT);
	} else if (_id == 11) {
		soundManager.play_sfx(SFX_PLAYER_CHARGED_SHOT);
	} else {
		soundManager.play_sfx(SFX_PLAYER_SHOT);
	}

	if (_move_type == TRAJECTORY_ARC) {
		if (direction == ANIM_DIRECTION_LEFT) {
			position0.x = position.x - RES_W/3;
		} else {
			position0.x = position.x + RES_W/3;
		}
	} else if (_move_type == TRAJECTORY_SIN) {
		if (direction == ANIM_DIRECTION_LEFT) {
			position0.x = 0;
		}
	}
	if (_move_type == TRAJECTORY_QUAKE) { // shake the screen
		// initialize quake
		_quake_info.timer = timer.getTimer() + QUAKE_TIMER;
		_quake_info.counter1 = 1;
		_quake_info.counter2 = 0;
	} else if (_move_type == TRAJECTORY_FREEZE) {
		if (freeze_weapon_effect == false) {
			freeze_weapon_effect = true;
			_quake_info.timer = timer.getTimer() + FREEZE_DURATION;
			std::cout << "projectile::projectile - freeze started, should stop at " << _quake_info.timer << std::endl;
		} else {
			std::cout << "projectile::projectile - freeze can't start, still running" << std::endl;
			is_finished = true; // can't shot two
		}
	}
	_effect_timer = timer.getTimer() + FREEZE_DURATION;
	_effect_n = 0;
	is_reflected = false;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
projectile::~projectile()
{
	if (_move_type == TRAJECTORY_FREEZE) {
		std::cout << "projectile::projectile - freeze projectile finishing" << std::endl;
		freeze_weapon_effect = false;
	}
}

void projectile::load_graphic(std::string filename) {
	graphLib.surfaceFromFile(filename, &graphic_surface);
	if (graphic_surface.gSurface == NULL) {
		std::cout << "initFrames - Error loading projectile surface from file\n";
		exit(-1);
	}
}

st_size projectile::move() {
	st_size moved;

	if (move_timer >= timer.getTimer()) {
		return st_size(0, 0);
	}
	move_timer = timer.getTimer()+move_delay;

	if (_move_type == TRAJECTORY_LINEAR || _move_type == TRAJECTORY_DIAGONAL_UP || _move_type == TRAJECTORY_DIAGONAL_DOWN) {
		if (direction == ANIM_DIRECTION_LEFT) {
			position.x -= speed;
			moved.width -= speed;
		} else {
			position.x += speed;
			moved.width += speed;
		}

		if (diagonal_flag == false) {
			if (_move_type == TRAJECTORY_DIAGONAL_UP) {
				position.y -= speed;
				moved.heigth -= speed;
			} else if (_move_type == TRAJECTORY_DIAGONAL_DOWN) {
				position.y += speed;
				moved.heigth += speed;
			}
			diagonal_flag = true;
		} else {
			diagonal_flag = false;
		}

	} else if (_move_type == TRAJECTORY_ARC) {
		if (direction == ANIM_DIRECTION_LEFT) {
			position.x -= speed;
			moved.width -= speed;
		} else {
			position.x += speed;
			moved.width += speed;
		}

		short int tempY1 = position1.y;
		int part1 = pow(static_cast<double>(RES_W/3), static_cast<double>(2));
		int part2 = pow(static_cast<double>(position.x - position0.x), static_cast<double>(2));
		int part3 = position0.y;
		position1.y = sqrt(static_cast<double>(part1 - part2)) + part3;
		short int diffY = tempY1 - position1.y;
		if (diffY > 0 || diffY > -20) {
			position.y += diffY;
		}
	} else if (_move_type == TRAJECTORY_SIN) {
		angle += 0.5;
		if (angle > 360.0) {
			angle = 0.0;
		}
		if (direction == ANIM_DIRECTION_LEFT) {
			position.x -= speed;
			moved.width -= speed;
		} else {
			position.x += speed;
			moved.width += speed;
		}
		position.y = sin(angle)*TILESIZE*2 + position0.y;
	} else if (_move_type == TRAJECTORY_BOMB) {
		if (_effect_n == 0 && _effect_timer < timer.getTimer()) {
			std::cout << "BOMB - TRANSFORM into explosion" << std::endl;
			/// morph into a bigger explosion
			_points = 5000;
			_effect_timer = timer.getTimer()+3600;
			graphic_surface.freeGraphic();
			std::string filename = FILEPATH + std::string("/data/images/tilesets/big_boss_explosion.png");
			load_graphic(filename);
			_size.width = 56;
			_size.heigth = 56;
			position.x -= 28;
			position.y -= 28;
			_max_frames = graphic_surface.width / _size.width;
			_effect_n++;
			soundManager.play_repeated_sfx(SFX_BIG_EXPLOSION, 3);
			//soundManager.play_timed_sfx(SFX_BIG_EXPLOSION, 3600);
			//is_finished = true;
		} else if (_effect_n == 1 && _effect_timer < timer.getTimer()) {
			std::cout << "BOMB - FINISH" << std::endl;
			is_finished = true;
		} else if (_effect_n == 0) {
			std::cout << "BOMB - EXECUTE" << std::endl;
		} else if (_effect_n == 1) {
			std::cout << "BOMB - EXPLOSION::EXECUTE" << std::endl;
		} else {
			std::cout << "BOMB - INVALID STATE - _effect_n: " << _effect_n << std::endl;
		}
		//std::cout << "projectile::move - BOMB" << std::endl;
		// do nothing, it is a bomb, it just stays until explodes
	} else if (_move_type == TRAJECTORY_CHAIN) {
		if (status == 0) { // increasing
			moved.width += speed;
			if (direction == ANIM_DIRECTION_LEFT) {
				position.x -= speed;
			} else {
				position.x += speed;
			}
			if (abs(position0.x-position.x) > _size.width) {
				status = 1;
			}
		} else { // returning
			moved.width -= speed;
			if (direction == ANIM_DIRECTION_LEFT) {
				position.x += speed;
			} else {
				position.x -= speed;
			}
			if (abs(position0.x-position.x) <= 0) {
				is_finished = true;
			}
		}
	} else if (_move_type == TRAJECTORY_QUAKE) { // shake the screen
		if (_quake_info.timer < timer.getTimer()) {
			if (_quake_info.counter1 == 1) {
				_quake_info.counter1 = 2;
				graphLib.set_screen_adjust(st_position(-QUAKE_SCREEN_MOVE, 0));
			} else {
				_quake_info.counter1 = 1;
				graphLib.set_screen_adjust(st_position(QUAKE_SCREEN_MOVE, 0));
			}
			_quake_info.counter2++;
			if (_quake_info.counter2 > 20) {
				_quake_info.counter1 = 0;
				graphLib.set_screen_adjust(st_position(0, 0));
				is_finished = true;
			}
			_quake_info.timer = timer.getTimer() + QUAKE_TIMER;
		}
	} else if (_move_type == TRAJECTORY_FREEZE) {
		std::cout << "projectile::projectile - freeze RUNNING, timer: " << timer.getTimer() << ", should stop at " << _quake_info.timer << std::endl;
		if (_quake_info.timer < timer.getTimer()) {
			std::cout << "projectile::projectile - freeze STOPPED, should stop at " << _quake_info.timer << std::endl;
			freeze_weapon_effect = false;
			is_finished = true;
		}
	} else {
		std::cout << "projectile::move - UNKNOWN TRAJECTORY #" << _move_type << std::endl;
	}

	realPosition.x = position.x - map->getMapScrolling().x;
	realPosition.y = position.y - map->getMapScrolling().y;
	// check out of screen
	if (_move_type != TRAJECTORY_FREEZE && _move_type != TRAJECTORY_QUAKE) { // special effect weapons can work out of screen
		if (realPosition.x > RES_W || realPosition.x < 0 || realPosition.y > RES_H || realPosition.y < 0) {
			//std::cout << ">> projectile::move - projectile is out of screen (" << realPosition.x << ", " << realPosition.y << ")" << std::endl;
			is_finished = true;
		}
	}
	return moved;
}

void projectile::draw() {
	if (_move_type == TRAJECTORY_QUAKE || _move_type == TRAJECTORY_FREEZE) { /// QTODO: freeze could use some "sparkling" effect
		return;
	}
	if (direction == ANIM_DIRECTION_LEFT && graphic_surface.height >= _size.heigth*2) {
		graphLib.showSurfaceRegionAt(&graphic_surface, st_rectangle(animation_pos*_size.width, _size.heigth, _size.width, _size.heigth), realPosition);
	} else {
        graphLib.showSurfaceRegionAt(&graphic_surface, st_rectangle(animation_pos*_size.width, 0, _size.width, _size.heigth), realPosition);
	}

	//graphLib.showSurfaceAt(&graphic_surface, realPosition);
    if (animation_timer < timer.getTimer()) {
        animation_pos++;
        animation_timer = timer.getTimer() + animation_speed;
		if (animation_pos >= _max_frames) {
			animation_pos = 0;
		}
	}
}

// TODO: width/height must come from editor instead of using graphic_surface
bool projectile::check_colision(st_rectangle enemy_pos, st_position pos_inc)
{
	st_position p1, p2, p3, p4;
	st_position enemy1, enemy2, enemy3, enemy4;
	// point 1, top/left
	p1.x = position.x + pos_inc.x;
	p1.y = position.y + pos_inc.y;

	// ponto 2, baixo/esquerda
	p2.x = position.x + pos_inc.x;
    p2.y = position.y + pos_inc.y + _size.heigth;

	// ponto 3, topo/direita
    p3.x = position.x + pos_inc.x + _size.width;
	p3.y = position.y + pos_inc.y;

	// ponto 4, baixo/direita
    p4.x = position.x + pos_inc.x + _size.width;
    p4.y = position.y + pos_inc.y + _size.heigth;

	enemy1.x = enemy_pos.x;
	enemy1.y = enemy_pos.y;
	enemy2.x = enemy_pos.x + enemy_pos.w;
	enemy2.y = enemy_pos.y;
	enemy3.x = enemy_pos.x;
	enemy3.y = enemy_pos.y + enemy_pos.h;
	enemy4.x = enemy_pos.x + enemy_pos.w;
	enemy4.y = enemy_pos.y + enemy_pos.h;

	//std::cout << "projectile::check_colision - p1: " << p1.x << "." << p1.y << " p2: " << p2.x << "." << p2.y << " p3: " << p3.x << "." << p3.y << " p4: " << p4.x << "." << p4.y << std::endl;
	//std::cout << "projectile::check_colision - enemy1: " << enemy1.x << "." << enemy1.y << " enemy2: " << enemy2.x << "." << enemy2.y << " enemy3: " << enemy3.x << "." << enemy3.y << " enemy4: " << enemy4.x << "." << enemy4.y << std::endl << std::endl;

	/// check if any of the PROJECTILE points is inside NPC
	if (p1.x >= enemy1.x && p1.x <= enemy2.x && p1.y >= enemy1.y && p1.y <= enemy3.y) {
		//std::cout << "projectile::check_colision - HIT 1" << std::endl;
		return true;
	} else if (p2.x >= enemy1.x && p2.x <= enemy2.x && p2.y >= enemy1.y && p2.y <= enemy3.y) {
		//std::cout << "projectile::check_colision - HIT 2" << std::endl;
		return true;
	} else if (p3.x >= enemy1.x && p3.x <= enemy2.x && p3.y >= enemy1.y && p3.y <= enemy3.y) {
		//std::cout << "projectile::check_colision - HIT 3" << std::endl;
		return true;
	} else if (p4.x >= enemy1.x && p4.x <= enemy2.x && p4.y >= enemy1.y && p4.y <= enemy3.y) {
		//std::cout << "projectile::check_colision - HIT 4" << std::endl;
		return true;
	}
	// npc inside projectile
	if (p1.x >= enemy1.x && p1.x <= enemy2.x && p1.y <= enemy1.y && p2.y >= enemy2.y) {
		//std::cout << "projectile::check_colision - HIT 5" << std::endl;
		return true;
	}
	if (p3.x >= enemy1.x && p3.x <= enemy2.x && p3.y <= enemy1.y && p4.y >= enemy2.y) {
		//std::cout << "projectile::check_colision - HIT 6" << std::endl;
		return true;
	}
	//std::cout << "projectile::check_colision - missed" << std::endl;
    return false;
}

int projectile::get_damage()
{
	return _damage;
}

int projectile::get_direction()
{
	return direction;
}

void projectile::reflect()
{
	if (direction == ANIM_DIRECTION_LEFT) {
		direction = ANIM_DIRECTION_RIGHT;
	} else {
		direction = ANIM_DIRECTION_LEFT;
	}
	if (position.y%2) {
		_move_type = TRAJECTORY_DIAGONAL_UP;
	} else {
		_move_type = TRAJECTORY_DIAGONAL_DOWN;
	}
	is_reflected = true;
	soundManager.play_sfx(SFX_SHOT_REFLECTED);
}

short projectile::get_move_type()
{
	return _move_type;
}

void projectile::set_y(int sety)
{
	position.y = sety;
	position0.y = sety;
	position1.y = sety;
}

void projectile::consume_projectile()
{
	_points--;
	//std::cout << "projectile::consume_projectile - _points: " << _points << std::endl;
	if (_points <= 0) {
		is_finished = true;
	}
}

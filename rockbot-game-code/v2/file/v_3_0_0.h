#ifndef V_3_0_0_H
#define V_3_0_0_H

#include <stdio.h>
#include <iostream>
#include "defines.h"


// @NOTE: all defines related to filesystem must contain a "FS_" prefix




namespace format_v_3_0_0 {




struct st_map_background {
    char filename[FS_CHAR_FILENAME_SIZE];
    Sint16 adjust_y;                             // distance of the background from the screen top
    Sint8 speed;                                // 0 means will scroll in the same speed as foreground (so the bg is used only to ease map-creation)
    bool auto_scroll;                           // automatic move background even if player isn't moving
    st_map_background()
    {
        filename[0] = '\0';
        adjust_y = 0;
        speed = 0;
        auto_scroll = false;
        sprintf(filename, "%s", "");
    }
};

struct map_tile {
    Sint8 locked;
    struct st_position_int8 tile1;                  // position of tile (x and y) in the graphic file (each tile is 16*16)
    struct st_position_int8 tile2;                  // second level tile. this can be used to create more complex backgrounds
    struct st_position_int8 tile3;                  // tird level, to be shown over player
    map_tile() {
        locked = 0;
        tile1.x = -1;
        tile1.y = -1;
        tile2.x = -1;
        tile2.y = -1;
        tile3.x = -1;
        tile3.y = -1;
    }
};

struct file_map_npc {
    Sint8 id_npc;
    struct st_position start_point;
    Uint8 direction;
    file_map_npc() {
        id_npc = -1;
        direction = 0;
    }
};

struct file_map_object {
    Sint8 id_object;
    struct st_position start_point;
    Uint8 direction;
    file_map_object() {
        id_object = -1;
        direction = 0;
    }
};

struct file_map {
    st_map_background backgrounds[2];
    st_color background_color;
    map_tile tiles[MAP_W][MAP_H];		// map tiles
    file_map_npc map_npcs[FS_MAX_MAP_NPCS];
    file_map_object map_objects[FS_MAX_MAP_OBJECTS];
    file_map() {
        for (int i=0; i<FS_MAX_MAP_NPCS; i++) {
            map_npcs[i].id_npc = -1;
        }
        for (int i=0; i<FS_MAX_MAP_OBJECTS; i++) {
            map_objects[i].id_object = -1;
        }
        // add ground to new map
        for (int i=0; i<MAP_W; i++) {
            tiles[i][MAP_H-2].locked = TERRAIN_SOLID;
            tiles[i][MAP_H-1].locked = TERRAIN_SOLID;
        }
    }
};




struct st_items {
    Uint8 energy_tanks;
    Uint8 weapon_tanks;
    Uint8 special_tanks;
    Uint8 balancer;
    Uint8 lifes;
    Uint8 bolts;
    Uint8 weapons[WEAPON_COUNT];
    // items that can be bought on shop
    bool exit;                                              // leave a completed stage
    bool shock_guard;                                       // do not move back when hit
    bool half_damage;                                       // takes only half the damage
    bool spike_guard;                                       // take gerular damage on spikes instead of dying
    bool speed_up;                                          // faster movement
    bool hyper_jump;                                        // bigger jump
    bool energy_saver;                                      // weapons consume half the wpn-energy
    bool speed_shot;                                        // normal shots become faster (can shoot more often)
    bool power_shot;                                        // charged shots causes more damage

    st_items() {
        energy_tanks = 0;
        weapon_tanks = 0;
        special_tanks = 0;
        balancer = 0;
        lifes = 3;
        bolts = 0;
        for (int i=0; i<WEAPON_COUNT; i++) {
            weapons[i] = PLAYER_INITIAL_HP;
        }
        // shop items
        exit = false;
        shock_guard = false;
        half_damage = false;
        speed_up = false;
        hyper_jump = false;
        energy_saver = false;
        speed_shot = false;
        power_shot = false;
        spike_guard = false;
    }
};

struct st_save {
    Sint8 finished_stages;                                         // number of already finished stages, just to simplify calc
    Sint8 stages[MAX_STAGES];                                      // 0 still open, 1 finished
    st_items items;
    Sint8 selected_player;
    Sint8 difficulty;                                              // 0 - easy, 1 - normal, 2 - hard
    Sint8 defeated_enemies_count;                                  // number of defeated enemies
    bool used_countinue;
    st_save() {
        finished_stages = 0;
        for (int i=0; i<MAX_STAGES; i++) {
            //stages[i] = 0;
            stages[i] = 0; // DEBUG
        }
        selected_player = 1;
        difficulty = 1;
        defeated_enemies_count = 0;
        used_countinue = false;
    }
};


struct st_game_config {
    bool sound_enabled;
    bool video_fullscreen;
    Sint8 video_filter;
    E_INPUT_TYPES input_type; // if keyboard or joystick
    E_INPUT_MODES input_mode; // inf directional is joypad-digital, analog sick or hat
    E_PLATFORM platform;      // if changed, must reset config to default
    int keys_codes[BTN_COUNT]; // number indicator for the keyboard-keys
    int button_codes[BTN_COUNT]; // number indicator for the keyboard-keys

    void set_default_keys() {
#ifdef PLAYSTATION2
        input_type = INPUT_TYPE_JOYSTICK;
        input_mode = INPUT_MODE_DIGITAL;
        for (int i=0; i<BTN_COUNT; i++) {
            keys_codes[i] = -1;
        }
        button_codes[BTN_SHIELD] = 3;
        button_codes[BTN_DASH] = 2;
        button_codes[BTN_JUMP] = 1;
        button_codes[BTN_ATTACK] = 0;
        button_codes[BTN_L] = 6;
        button_codes[BTN_R] = 7;
        button_codes[BTN_QUIT] = -1;
        button_codes[BTN_START] = 5;
        button_codes[BTN_LEFT] = -1; // uses default axis
        button_codes[BTN_RIGHT] = -1; // uses default axis
        button_codes[BTN_UP] = -1; // uses default axis
        button_codes[BTN_DOWN] = -1; // uses default axis
        //std::cout << "v_2_1_2 - input_mode: " << input_mode << ", BTN_JUMP: " << button_codes[BTN_JUMP] << std::endl;
#elif PSP
        input_type = INPUT_TYPE_JOYSTICK;
        input_mode = INPUT_MODE_DIGITAL;
        for (int i=0; i<BTN_COUNT; i++) {
            keys_codes[i] = -1;
        }
        button_codes[BTN_SHIELD] = 3;
        button_codes[BTN_DASH] = 0;
        button_codes[BTN_JUMP] = 1;
        button_codes[BTN_ATTACK] = 2;
        button_codes[BTN_L] = 4;
        button_codes[BTN_R] = 5;
        button_codes[BTN_QUIT] = -1;
        button_codes[BTN_START] = 11;
        button_codes[BTN_LEFT] = 7; // PSP maps axis as buttons. Will use thos unless you pick analog mode
        button_codes[BTN_RIGHT] = 9; // PSP maps axis as buttons. Will use thos unless you pick analog mode
        button_codes[BTN_UP] = 8; // PSP maps axis as buttons. Will use thos unless you pick analog mode
        button_codes[BTN_DOWN] = 6; // PSP maps axis as buttons. Will use thos unless you pick analog mode
#elif WII
        input_type = INPUT_TYPE_JOYSTICK;
        input_mode = INPUT_MODE_DIGITAL;
        for (int i=0; i<BTN_COUNT; i++) {
            keys_codes[i] = -1;
        }
        button_codes[BTN_SHIELD] = 1;
        button_codes[BTN_DASH] = 0;
        button_codes[BTN_JUMP] = 3;
        button_codes[BTN_ATTACK] = 2;
        button_codes[BTN_L] = 4;
        button_codes[BTN_R] = 5;
        button_codes[BTN_QUIT] = -1;
        button_codes[BTN_START] = 6;
#elif DINGUX
        input_type = INPUT_TYPE_DOUBLE;
        input_mode = INPUT_MODE_DIGITAL;
        for (int i=0; i<BTN_COUNT; i++) {
            keys_codes[i] = -1;
        }
        keys_codes[BTN_SHIELD] = SDLK_LSHIFT;
        keys_codes[BTN_DASH] = SDLK_SPACE;
        keys_codes[BTN_JUMP] = SDLK_LCTRL;
        keys_codes[BTN_ATTACK] = SDLK_LALT;
        keys_codes[BTN_L] = SDLK_TAB;
        keys_codes[BTN_R] = SDLK_BACKSPACE;
        keys_codes[BTN_QUIT] = SDLK_ESCAPE;
        keys_codes[BTN_START] = SDLK_RETURN;
        keys_codes[BTN_LEFT] = SDLK_LEFT;
        keys_codes[BTN_RIGHT] = SDLK_RIGHT;
        keys_codes[BTN_UP] = SDLK_UP;
        keys_codes[BTN_DOWN] = SDLK_DOWN;
#elif OPEN_PANDORA
        input_type = INPUT_TYPE_KEYBOARD;
        input_mode = INPUT_MODE_DIGITAL;
        for (int i=0; i<BTN_COUNT; i++) {
            keys_codes[i] = -1;
        }
        keys_codes[BTN_SHIELD] = SDLK_PAGEUP;
        keys_codes[BTN_DASH] = SDLK_HOME;
        button_codes[BTN_JUMP] = SDLK_PAGEDOWN;
        keys_codes[BTN_ATTACK] = SDLK_END;
        keys_codes[BTN_L] = SDLK_RSHIFT;
        keys_codes[BTN_R] = SDLK_RCTRL;
        keys_codes[BTN_QUIT] = SDLK_LCTRL;
        keys_codes[BTN_START] = SDLK_LALT;
        keys_codes[BTN_LEFT] = SDLK_LEFT;
        keys_codes[BTN_RIGHT] = SDLK_RIGHT;
        keys_codes[BTN_UP] = SDLK_UP;
        keys_codes[BTN_DOWN] = SDLK_DOWN;
#else
        for (int i=0; i<BTN_COUNT; i++) {
            button_codes[i] = -1;
        }
        input_type = INPUT_TYPE_DOUBLE;
        button_codes[BTN_ATTACK] = 2;
        button_codes[BTN_JUMP] = 1;
        button_codes[BTN_DASH] = 0;
        button_codes[BTN_SHIELD] = 3;
        button_codes[BTN_L] = 6;
        button_codes[BTN_R] = 7;
        button_codes[BTN_QUIT] = 8;
        button_codes[BTN_START] = 9;
        keys_codes[BTN_ATTACK] = SDLK_a;
        keys_codes[BTN_JUMP] = SDLK_x;
        keys_codes[BTN_DASH] = SDLK_c;
        keys_codes[BTN_SHIELD] = SDLK_z;
        keys_codes[BTN_L] = SDLK_q;
        keys_codes[BTN_R] = SDLK_w;
        keys_codes[BTN_QUIT] = SDLK_ESCAPE;
        keys_codes[BTN_START] = SDLK_RETURN;
        keys_codes[BTN_LEFT] = SDLK_LEFT;
        keys_codes[BTN_RIGHT] = SDLK_RIGHT;
        keys_codes[BTN_UP] = SDLK_UP;
        keys_codes[BTN_DOWN] = SDLK_DOWN;
#endif
    }

    E_PLATFORM get_current_platform() const {
#ifdef DINGUX
        return PLATFORM_DINGUX;
#elif LINUX
        return PLATFORM_LINUX;
#elif OPEN_PANDORA
        return PLATFORM_PANDORA;
#elif PLAYSTATION2
        return PLATFORM_PS2;
#elif PSP
        return PLATFORM_PSP;
#elif WII
        return PLATFORM_WII;
#else
        return PLATFORM_WINDOWS;
#endif
    }

    void reset() {
        sound_enabled = true;
        video_fullscreen = false;
        video_filter = VIDEO_FILTER_NOSCALE;
        // SET KEYS/TYPES - triangle=shield; circle=dash; x=jump; square=attack; start=start; select=quit
        set_default_keys();
        platform = get_current_platform();
    }


    st_game_config() {
        reset();
    }
};




enum LINK_TYPE {
    LINK_TYPE_HORIZONTAL,
    LINK_TYPE_VERTICAL,
    LINK_TYPE_TELEPORTER,
    LINK_TYPE_COUNT
};


struct file_link { // DONE - Stage
    Sint8 id_map_origin;
    Sint8 id_map_destiny;
    st_position_uint8 pos_origin;
    st_position_uint8 pos_destiny;
    Uint8 size;
    bool bidirecional;
    bool is_door;
    Sint8 type;
    file_link() {
        id_map_origin = -1;
        id_map_destiny = -1;
        size = 1;
        bidirecional = false;
        is_door = false;
        type = LINK_TYPE_VERTICAL;
    }
};

struct file_boss { // DONE - Stage
    Sint8 id_npc;
    Sint8 id_weapon;
    char name[20];
    char face_graphics_filename[20];
    file_boss() {
        id_npc = -1;
        id_weapon = -1;
        sprintf(name, "%s", "Boss");
        face_graphics_filename[0] = '\0';
    }
};

struct file_dialog { // DONE - Stage x2
    char face_graphics_filename[20];
    bool top_side;
    char line1[3][DIALOG_LINE_LIMIT];
    char answer1[FS_MAX_PLAYERS][3][DIALOG_LINE_LIMIT];
    char line2[3][DIALOG_LINE_LIMIT];
    char answer2[FS_MAX_PLAYERS][3][DIALOG_LINE_LIMIT];
    file_dialog() {
        top_side = true;
        face_graphics_filename[0] = '\0';
        for (int i=0; i<FS_MAX_PLAYERS; i++) {
            for (int j=0; j<3; j++) {
                answer1[i][j][0] = '\0';
                answer2[i][j][0] = '\0';
            }
        }
        for (int j=0; j<3; j++) {
            line1[j][0] = '\0';
            line2[j][0] = '\0';
        }
    }
};

struct file_colorcycle { // DONE - Stage
    st_color color[FS_COLORCYCLE_MAX_ITEMS][COLOR_KEY_LENGTH];
    Sint16 duration[FS_COLORCYCLE_MAX_ITEMS][COLOR_KEY_LENGTH];
};





struct file_projectile {
    char name[CHAR_NAME_SIZE];
    char graphic_filename[CHAR_FILENAME_SIZE];
    st_size_int8 size;
    bool is_destructible;                                   // indicates if the projectile can be shot down, if true, uses HP
    Uint8 hp;                                               // how much danage needs to be taken to destruct the projectile
    enum PROJECTILE_TRAJECTORIES trajectory;                // RENAMED from projectile_type in 2.1
    Uint8 max_shots;                                        // number of maximum simultaneous projectiles, if 0, is infinite
    Uint8 speed;                                            // now many pixels are moved into a step
    Uint8 damage;                                           // how many hitpoints will be reduced from target
    file_projectile() {
        sprintf(name, "%s", "Projectile");
        graphic_filename[0] = '\0';
        is_destructible = false;
        hp = 1;
        size.width = 3;
        size.height = 2;
        trajectory = TRAJECTORY_LINEAR;
        max_shots = 3;
        speed = PROJECTILE_DEFAULT_SPEED;
        damage = PROJECTILE_DEFAULT_DAMAGE;
    }
};



struct st_sprite_data {
    bool used;                                                  // if true, the sprite exists
    Uint16 duration;
    Uint8 sprite_graphic_pos_x;
    st_rectangle colision_rect;
    st_sprite_data() {
        used = false;
        duration = 0;
        sprite_graphic_pos_x = 0;
    }

    st_sprite_data & operator=(st_sprite_data new_value) {
        used = new_value.used;
        duration = new_value.duration;
        sprite_graphic_pos_x = new_value.sprite_graphic_pos_x;
        colision_rect = new_value.colision_rect;
        return *this;
    }
};


struct file_weapon_colors { // DONE -> Player
    st_color color1;
    st_color color2;
    st_color color3;
};

struct file_player { // DONE -> Game
    char name[CHAR_NAME_SIZE];
    char graphic_filename[CHAR_FILENAME_SIZE];
    char face_filename[CHAR_FILENAME_SIZE];
    Uint8 HP;
    st_size_int8 sprite_size;                               // size of sprite graphic
    st_rectangle sprite_hit_area;                           // the area of the graphic where is used for hit/colision
    Uint8 move_speed;                             // how many sprites move each step
    float jump_initial_speed;                               // used to adjust jump speed/height
    float jump_gravity;                                     // used to adjust jump speed/height
    float touch_damage_reduction;
    st_sprite_data sprites[ANIM_TYPE_COUNT][ANIM_FRAMES_COUNT];
    file_weapon_colors weapon_colors[MAX_WEAPON_N];
    st_color color_keys[3];
    // habilities part
    bool have_shield;
    Uint8 max_shots;                              // number of maximum simultaneous projectiles
    Uint8 simultaneous_shots;                     // number of projectiles shot at one button press
    bool double_jump;
    bool can_slide;                                         // if false, dashes instead of sliding
    bool can_charge_shot;
    Uint8 full_charged_projectile_id;

    file_player() {
        sprintf(name, "%s", "Player");
        graphic_filename[0] = '\0';
        face_filename[0] = '\0';
        HP = 0;
        move_speed = 2.0;
        jump_initial_speed = 11;
        jump_gravity = 0.13;
        touch_damage_reduction = 0;
        max_shots = 3;
        simultaneous_shots = 3;
        double_jump = false;
        have_shield = false;
        can_slide = false;
        can_charge_shot = false;
        full_charged_projectile_id = 0;
    }
};


struct file_weapon { // DONE - Game
    Uint8 id_projectile;
    char name[CHAR_NAME_SIZE];
    Uint8 damage;
    file_weapon() {
        sprintf(name, "%s", "Weapon");
        damage = 1;
        id_projectile = 0;
    }
};

struct file_object { // DONE - Game
    char name[CHAR_NAME_SIZE];
    char graphic_filename[CHAR_FILENAME_SIZE];
    Sint8 type;													///@ TODO: use enum
    int timer;													// used as time for disapearing block
    Sint8 speed;												// used as speed for moving platform
    Sint16 limit;												// used as range (pixels) for moving platform, visible time for disapearing block
    Sint8 direction;											// used to check if moving away from oiginalpoint or moving to it
    Sint16 distance;
    st_size_int8 size;
    Sint16 frame_duration;										// indicates what is the used frame
    bool animation_auto_start;                                  // false means it will not be animated until started
    bool animation_reverse;                                     // start animating
    bool animation_loop;                                        // true = animation will loop, false = animation will run once
    file_object() {
        sprintf(name, "%s", "Object");
        graphic_filename[0] = '\0';
        animation_auto_start = true;
        animation_reverse = false;
        animation_loop = true;
        frame_duration = 100;
    }
};


struct st_weakness {
    Sint8 weapon_id;                                           // weapon number of weakness, if -1, no weakness
    Sint8 damage_multiplier;                                   // how many time more damage is taken, if zero, no damage from this weapon is taken
    st_weakness() {
        weapon_id = -1;
        damage_multiplier = 1;
    }
};


struct file_npc { // DONE - Game
    Sint8 id;                                                  // serial number
    //unsigned int projectile_id;                               // indicates the projectile ID (relation with file_projectile)
    Sint8 projectile_id[2];                                    // indicate the id of an attack the NCP can user
    char name[CHAR_NAME_SIZE];
    char graphic_filename[CHAR_FILENAME_SIZE];
    struct st_hit_points hp;
    Sint8 direction;                                           // defines how it behavies (kink of AI*)
    Sint8 speed;                                               // defines the distances it can see enemies
    Sint16 walk_range;                                         // defines how long from the start point it can go
    Sint8 facing;                                              // defines the side npc is facing before start moving (also used by LINEWALK behavior)
    struct st_position start_point;
    st_sprite_data sprites[ANIM_TYPE_COUNT][ANIM_FRAMES_COUNT]; // changed in 2.0.4
    st_size_int8 frame_size;
    bool is_ghost;
    Sint8 shield_type;
    Sint8 IA_type;                                             // IA types. For custom (edited) ones, use IA_TYPES_COUNT + position (in the array)
    Sint8 fly_flag;                                            // 0 - can't fly, 1 - flyer (...)
    st_weakness weakness[9];                                    // each npc have weaknesses for each boss weapon, plus the normal weapon
    char bg_graphic_filename[CHAR_FILENAME_SIZE];               // holds a static background
    st_position sprites_pos_bg;                                 // holds position of sprites in relation with background
    bool is_boss;                                               // indicates if this NPC is a boss
    Sint8 gravity_level;


/**
 * @brief
 *
 */
    file_npc() {
        id = -1;
        sprintf(name, "%s", "NPC");
        graphic_filename[0] = '\0';
        projectile_id[0] = -1;
        projectile_id[1] = -1;
        bg_graphic_filename[0] = '\0';
        gravity_level = 0;
        walk_range = 160;
        fly_flag = 0;
        IA_type = 0;
        is_boss = false;
        speed = 3;
    }

};

struct file_ai_action {
    int chance;
    int action;
    int go_to;                                                  // action number to be executed once this one is finished, -1 indicates that must return to CHANCES (random)
    int go_to_delay;                                            // delay before going to the next action
    int extra_parameter;										// left, right, etc

/**
 * @brief
 *
 */
    file_ai_action()
    {
        chance = 0;
        action = 0;
        go_to = 0;
        go_to_delay = 500;
        extra_parameter = 0;
    }
};

struct reaction {
    int action;
    int go_to;                                                  // action number to be executed once this one is finished, -1 indicates that must return to CHANCES (random)
    int go_to_delay;                                            // delay before going to the next action
    int extra_parameter;										// left, right, etc
    reaction() {
        action = -1;
        go_to = 0;
        go_to_delay = 500;
        extra_parameter = 0;
    }
};

struct file_artificial_inteligence {
    char name[CHAR_NAME_SIZE];
    struct file_ai_action states[AI_MAX_STATES];
    struct reaction reactions[MAX_AI_REACTIONS];                    // reactions near, hit and dead

    file_artificial_inteligence() {
        sprintf(name, "%s", "A.I.");
    }
};

enum e_trophy_conditions {
    TROPHY_CONDITIONS_NO_DAMAGE,                                    // finish a stage without being hit
    TROPHY_CONDITIONS_END_GAME,                                     // finish the game
    TROPHY_CONDITIONS_KILLER,                                       // kill 1000 enemies @TODO - add counter to save
    TROPHY_CONDITIONS_NO_CONTINUES,                                 // finish the game without using any continue @TODO - add flag in save
    TROPHY_CONDITIONS_COUNT
};

struct file_trophy {
    e_trophy_conditions condition;
    char name[FS_NAME_SIZE];
    char filename[FS_FILENAME_SIZE];
};


// defines a castle intro and details
struct file_castle {
    st_position points[CASTLE_STAGES_MAX];
    char filename[FS_FILENAME_SIZE];
    file_dialog boss_defeated_dialog;                                       // boss defeated dialog
};


struct file_game {
    float version;
    char name[FS_CHAR_NAME_SIZE];
    file_npc game_npcs[FS_GAME_MAX_OBJS];                           // 60 enemy types is more than good enought
    file_object objects[FS_GAME_MAX_OBJS];
    file_projectile projectiles[FS_MAX_PROJECTILES];
    file_weapon weapons[FS_MAX_WEAPONS];                            // 8 stage-bosses and 3 item-like
    file_player players[FS_MAX_PLAYERS];                            // up to 4 different players the user can select from
    file_artificial_inteligence ai_types[FS_MAX_AI_TYPES];

    // game properties @TODO - revise to organize as an structure
    Sint8 semi_charged_projectile_id;                              // common to all players
    Sint8 player_items[2];                                         // common to all players -> to be used in add_coil_object and add_jet_object
    char stage_face_filename[MAX_STAGES][20];

    file_trophy trophies[TROPHIES_MAX];

    // CONSTRUCTOR //
    file_game() {
        /// *** hardcoded parts *** ///
        version = 2.00;
        sprintf(name, "%s", "Rockbot");
        semi_charged_projectile_id = 0;
        player_items[0] = 0;
        player_items[1] = 0;
        for (int i=0; i<MAX_STAGES; i++) {
            stage_face_filename[i][0] = '\0';
        }
        for (int i=0; i<FS_GAME_MAX_OBJS; i++) {
            sprintf(game_npcs[i].name, "NPC [%d]", i);
        }
        for (int i=0; i<FS_GAME_MAX_OBJS; i++) {
            sprintf(objects[i].name, "Object [%d]", i);
        }
        for (int i=0; i<FS_MAX_PROJECTILES; i++) {
            sprintf(projectiles[i].name, "Projectile [%d]", i);
        }
        for (int i=0; i<FS_MAX_WEAPONS; i++) {
            sprintf(projectiles[i].name, "Projectile [%d]", i);
        }
        for (int i=0; i<FS_MAX_PLAYERS; i++) {
            sprintf(players[i].name, "Player [%d]", i);
        }
        for (int i=0; i<FS_MAX_AI_TYPES; i++) {
            sprintf(ai_types[i].name, "A.I. [%d]", i);
        }
    }
};


struct file_stage {
    char name[FS_CHAR_NAME_SIZE];
    char bgmusic_filename[FS_CHAR_FILENAME_SIZE];
    file_map maps[FS_STAGE_MAX_MAPS];
    file_link links[FS_STAGE_MAX_LINKS];
    file_boss boss;
    file_dialog intro_dialog;
    file_dialog boss_dialog;
    file_colorcycle colorcycle;
    file_colorcycle colorcycle_beaten; ///@NEWFIELD

    // CONSTRUCTOR //
    file_stage() {
       boss = file_boss();
        sprintf(name, "%s", "Stage");
        bgmusic_filename[0] = '\0';
    }
};



struct file_intro {
    int initial_delay;                                  // delay before showing text and effects
    char bg_filename[FS_CHAR_FILENAME_SIZE];            // background image
    int transition_effect;                              // how we transition to next screen
    // text (3 lines x2)
};

/**
 * @brief
 *
 */
struct file_stages {
    file_stage stages[FS_MAX_STAGES];
    file_stages() {
        for (int i=0; i<FS_MAX_STAGES; i++) {
            sprintf(stages[i].name, "Stage [%d]", i);
        }
    }
};

struct st_checkpoint {
    int map; /**< TODO */
    int x; /**< TODO */
    int y; /**< TODO */
    int map_scroll_x; /**< TODO */
            st_checkpoint() {
                    map = 0;
                    x = 50;
                    y = -1;
                    map_scroll_x = 0;
            }
    };



}

#endif // V_3_0_0_H

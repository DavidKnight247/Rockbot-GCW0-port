#ifndef DRAW_H
#define DRAW_H

#include "graphicslib.h"


#define SNOW_PARTICLES_NUMBER 20

struct st_snow_particle {
    st_float_position position;
    float speed;                    // vertical speed. horizontal speed is the same for all
    float x_dist;                   // used to check how many pixels were moved in balance to change direction
    short direction;                // if balancing left or right
    st_snow_particle(st_float_position pos, float spd)
    {
        position = pos;
        speed = spd;
        x_dist = 0;
        if ((int)position.x % 2 == 0) {
            direction = ANIM_DIRECTION_LEFT;
        } else {
            direction = ANIM_DIRECTION_RIGHT;
        }
    }
};

/**
 * @brief
 *
 */
class draw
{
public:
/**
 * @brief
 *
 */
    draw();

public:
    void preload();
    void update_screen(); // replaces external calls to graphLib.updateScreen
    void update_colorcycle() const;
    void set_rain_enabled(bool enabled);
    void show_rain();
    void generate_snow_particles();
    void show_snow_effect();
    void show_boss_intro_sprites(short boss_id, bool show_fall);
    void show_ready();
    void show_bubble(int x, int y);
    void reset_teleporter_colors();
    void set_teleport_small_colors(st_color color1, st_color color2);
    void show_teleport_small(int x, int y);
    void show_credits();
    void create_credits_text(graphicsLib_gSurface& surface);
    graphicsLib_gSurface* get_object_graphic(int obj_id);
    void remove_object_graphic(int obj_id);
    void show_ingame_warning(std::vector<std::string> message);

private:
    /**
     * @brief
     *
     * @param surface
     * @param initial_line
     */
    void draw_credit_line(graphicsLib_gSurface& surface, unsigned short initial_line);


private:
    graphicsLib_gSurface rain_obj;
    unsigned int _effect_timer;
    short int _rain_pos;
    bool _rain_enabled;
    struct graphicsLib_gSurface ready_message;
    graphicsLib_gSurface _bubble_gfx;
    // USED IN CREDITS
    std::vector<std::string> credits_list; /**< TODO */
    graphicsLib_gSurface _teleport_small_gfx;

    // USED IN SNOW EFFECT
    bool _snow_effect_enabled;
    std::vector<st_snow_particle> _snow_particles;

    // GRAPHICS LISTS
    std::map<unsigned int, graphicsLib_gSurface> objects_sprite_list; // object_id, graphic




};

#endif // DRAW_H

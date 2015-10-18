#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include "st_common.h"
#include "graphicslib.h"

enum ANIMATION_TYPES {
    ANIMATION_STATIC,
    ANIMATION_DYNAMIC,
    ANIMATION_COUNT
};

class animation
{
public:
    animation(ANIMATION_TYPES pos_type, std::string filename, const st_position &pos, st_position adjust_pos, unsigned int frame_time, unsigned int repeat_times, int direction, st_size framesize, st_position* map_scroll);
    bool finished();
    void execute();

private:
    const st_position* ref_pos;
    st_position _adjust_pos;
    graphicsLib_gSurface surface;
    unsigned short int _repeat_times;
    unsigned short int _executed_times;
    unsigned short int _frame_time;
    bool _finished;
    int _frames_number;
    int _direction;
    st_size _framesize;

    unsigned short int _current_frame;
    unsigned short int _current_frame_timer;
    unsigned short int _max_repeat;
    unsigned short int _repeated_times;
    st_position* _map_scroll;
};


#endif // ANIMATION_H

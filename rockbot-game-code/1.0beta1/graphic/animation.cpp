#include "animation.h"

#include "timerlib.h"
extern timerLib timer;

extern graphicsLib graphLib;

extern std::string FILEPATH;

animation::animation(ANIMATION_TYPES pos_type, std::string filename, const st_position &pos, st_position adjust_pos, unsigned int frame_time, unsigned int repeat_times, int direction, st_size framesize, st_position* map_scroll) : _finished(false), _current_frame(0), _repeated_times(0)
{
    if (pos_type == ANIMATION_DYNAMIC) {
        ref_pos = &pos;
    } else {
        ref_pos = new st_position(pos.x, pos.y);
    }
    std::string full_filename = FILEPATH + std::string("data/images/tilesets/") + filename;
	graphLib.surfaceFromFile(full_filename, &surface);
    _repeat_times = repeat_times;
    _frame_time = frame_time;
    _direction = direction;
    _framesize = framesize;
    _frames_number = surface.width/framesize.width;
    _max_repeat = repeat_times;
    _map_scroll = map_scroll;
    _adjust_pos = adjust_pos;
    _current_frame_timer = timer.getTimer() + _frame_time;
}

bool animation::finished()
{
    return _finished;
}

void animation::execute()
{
    graphLib.showSurfaceRegionAt(&surface, st_rectangle(_current_frame*_framesize.width, 0, _framesize.width, _framesize.heigth), st_position(ref_pos->x-_map_scroll->x+_adjust_pos.x, ref_pos->y+_adjust_pos.y));
    if (_current_frame_timer < timer.getTimer()) {
        _current_frame++;
        _current_frame_timer = timer.getTimer() + _frame_time;
        if (_current_frame > _frames_number) {
            _current_frame = 0;
            _repeated_times++;
            if (_repeated_times >= _max_repeat) {
                _finished = true;
            }
        }
    }
}

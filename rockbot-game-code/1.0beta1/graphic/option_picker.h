#ifndef OPTION_PICKER_H
#define OPTION_PICKER_H

#include "../st_common.h"
#include <string>
#include <vector>


class option_picker
{
public:
    option_picker(bool draw_border, st_position pos, std::vector<std::string> options);
    int pick();
    void remove();
	void set_picker_initial_pos(int pick_pos);

private:
    st_position _position;
    std::vector<std::string> _items;
    bool _draw_border;
	short _pick_pos;
};

#endif // OPTION_PICKER_H

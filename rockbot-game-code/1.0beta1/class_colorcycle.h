#ifndef CLASS_COLORCYCLE_H
#define CLASS_COLORCYCLE_H

#include <vector>
#include <map>
#include "st_common.h"

enum COLOR_KEYS {
	COLOR_KEY_GREEN,
	COLOR_KEY_PURPLE,
	COLOR_KEY_CYAN,
	COLOR_KEY_LENGTH
};

struct colorcycle_item {
	st_color color[COLOR_KEY_LENGTH];
	unsigned int duration[COLOR_KEY_LENGTH];

	colorcycle_item() {
		color[COLOR_KEY_GREEN] = st_color();
		color[COLOR_KEY_PURPLE] = st_color();
		color[COLOR_KEY_CYAN] = st_color();
		duration[COLOR_KEY_GREEN] = 0;
		duration[COLOR_KEY_PURPLE] = 0;
		duration[COLOR_KEY_CYAN] = 0;
	}

	colorcycle_item(st_color set_green, int duration_green, st_color set_purple, int duration_purple, st_color set_cyan, int duration_cyan)
	{
		color[COLOR_KEY_GREEN] = set_green;
		color[COLOR_KEY_PURPLE] = set_purple;
		color[COLOR_KEY_CYAN] = set_cyan;
		duration[COLOR_KEY_GREEN] = duration_green;
		duration[COLOR_KEY_PURPLE] = duration_purple;
		duration[COLOR_KEY_CYAN] = duration_cyan;
	}
};

struct colorcycle_list {
	bool can_loop; // indicates if the list can go back to first item when finished
	std::vector<struct colorcycle_item> colors;
	colorcycle_list  () {
		colors.empty();
		can_loop = true;
	}
};

class class_colorcycle
{
public:
	class_colorcycle();
	void execute(void);
	unsigned int get_current_colormap_timer(unsigned int key);
	struct st_color get_current_color_item(unsigned int key);
	void set_colormap(int colormap_n);
	bool have_colorcycle(unsigned int map_key);
	void set_color_key_number(unsigned int map_key, int color_number);

private:
	void add(int index, struct colorcycle_list items);

public:
private:
	int current_colormap;
	unsigned int current_position[COLOR_KEY_LENGTH];
	int color_keys[3];
	int time_count[COLOR_KEY_LENGTH];
	std::map<int, struct colorcycle_list> colorcycle_map;
};

#endif // CLASS_COLORCYCLE_H

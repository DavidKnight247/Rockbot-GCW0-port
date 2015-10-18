#include "class_colorcycle.h"

#include "defines.h"
#include "timerlib.h"
extern timerLib timer;

#include "graphicslib.h"
extern graphicsLib graphLib;

class_colorcycle::class_colorcycle() : current_colormap(-1)
{
	for (int i=0; i<COLOR_KEY_LENGTH; i++) {
		color_keys[i] = 0;
		current_position[i] = 0;
		time_count[i] = 0;
	}

	// add default map
	struct colorcycle_list temp_list;
	temp_list.can_loop = false;
	// default colorcycle is -1 and is empty
	add(-1, temp_list);
	temp_list.colors.clear();

	// -2 is the intro colorcycle first part (city)
	temp_list.colors.push_back(colorcycle_item(st_color(219, 43, 0), 4400, st_color(255, 119, 99), 4400, st_color(243, 191, 63), 4400));
	temp_list.colors.push_back(colorcycle_item(st_color(167, 0, 0), 4400, st_color(219, 43, 0), 4400, st_color(255, 155, 59), 4400));
	temp_list.colors.push_back(colorcycle_item(st_color(127, 11, 0), 4400, st_color(167, 0, 0), 4400, st_color(255, 102, 0), 4400));
	temp_list.colors.push_back(colorcycle_item(st_color(27, 63, 95), 4400, st_color(127, 11, 0), 4400, st_color(203, 79, 15), 4400));
	temp_list.colors.push_back(colorcycle_item(st_color(0, 0, 0), 9999, st_color(27, 63, 95), 9999, st_color(81, 81, 81), 9999));
	add(-2, temp_list);
	temp_list.colors.clear();


	// -3 is the intro colorcycle second part (lab)
	temp_list.colors.push_back(colorcycle_item(st_color(0, 151, 0), 1300, st_color(143, 0, 119), 1800, st_color(255, 219, 171), 1200));
	temp_list.colors.push_back(colorcycle_item(st_color(219, 43, 0), 1600, st_color(255, 102, 0), 1200, st_color(255, 155, 59), 1600));
	temp_list.colors.push_back(colorcycle_item(st_color(0, 0, 171), 1400, st_color(230, 255, 0), 1600, st_color(219, 43, 0), 1800));
	add(-3, temp_list);
	temp_list.colors.clear();


	// stages colorcycles
	temp_list.can_loop = true;
	// each item has purple, cyan, magenta

	temp_list.colors.push_back(colorcycle_item(st_color(255, 155, 59), 300, st_color(219, 43, 0), 300, st_color(255, 219, 171), 300));
	temp_list.colors.push_back(colorcycle_item(st_color(219, 43, 0), 300, st_color(255, 219, 171), 300, st_color(255, 155, 59), 300));
	temp_list.colors.push_back(colorcycle_item(st_color(255, 219, 171), 300, st_color(255, 155, 59), 300, st_color(219, 43, 0), 300));
	add(APEBOT, temp_list);
	temp_list.colors.clear();


	temp_list.colors.push_back(colorcycle_item(st_color(255, 51, 0), 1500, st_color(0, 0, 0), 100, st_color(0, 0, 0), 1500));
	temp_list.colors.push_back(colorcycle_item(st_color(0, 0, 0), 1000, st_color(230, 255, 0), 200, st_color(79, 223, 75), 1000));
	add(TECHNOBOT, temp_list);
	temp_list.colors.clear();

	temp_list.colors.push_back(colorcycle_item(st_color(255, 234, 0), 100, st_color(0, 0, 0), 800, st_color(255, 234, 0), 100));
	temp_list.colors.push_back(colorcycle_item(st_color(0, 0, 0), 200, st_color(255, 234, 0), 100, st_color(0, 0, 0), 500));
	temp_list.colors.push_back(colorcycle_item(st_color(255, 234, 0), 100, st_color(0, 0, 0), 200, st_color(255, 234, 0), 100));
	temp_list.colors.push_back(colorcycle_item(st_color(0, 0, 0), 1100, st_color(255, 234, 0), 100, st_color(0, 0, 0), 400));
	add(MAGEBOT, temp_list);
	temp_list.colors.clear();

	int water_timer = 130;
	temp_list.colors.push_back(colorcycle_item(st_color(171, 231, 255), water_timer, st_color(80, 109, 142), water_timer, st_color(235, 235, 235), water_timer));
	temp_list.colors.push_back(colorcycle_item(st_color(80, 109, 142), water_timer, st_color(235, 235, 235), water_timer, st_color(171, 231, 255), water_timer));
	temp_list.colors.push_back(colorcycle_item(st_color(235, 235, 235), water_timer, st_color(171, 231, 255), water_timer, st_color(80, 109, 142), water_timer));
	add(SEAHORSEBOT, temp_list);
	add(DAISIEBOT, temp_list);
	temp_list.colors.clear();




	//temp_list.colors.push_back(colorcycle_item(st_color(), , st_color(), , st_color(), ));

}

void class_colorcycle::set_colormap(int colormap_n)
{
	/// @TODO: restore colormap to original
	current_colormap = colormap_n;
}

void class_colorcycle::execute(void)
{
	if (colorcycle_map[current_colormap].colors.size() == 0) {
		return;
	}
	int now_time = timer.getTimer();

	for (int i=0; i<COLOR_KEY_LENGTH; i++) {
		if (colorcycle_map[current_colormap].colors.size() > current_position[i]) {
			int temp_duration = colorcycle_map[current_colormap].colors.at(current_position[i]).duration[i];
			if ((time_count[i] + temp_duration) < now_time) {
				graphLib.update_colors();
				current_position[i]++;
				if (current_position[i] >= colorcycle_map[current_colormap].colors.size()) {
					current_position[i] = 0;
				}
				time_count[i] = now_time;
			}
		} else {
			current_position[i] = 0;
		}
	}

	//std::cout << "class_colorcycle::execute - current_position[0]: " << current_position[0] << std::endl;
	//std::cout << "class_colorcycle::execute - current_colormap: " << current_colormap << ", colorcycle_map[current_colormap].colors.size(): " << colorcycle_map[current_colormap].colors.size() << std::endl;
	if (colorcycle_map[current_colormap].colors.size() > 0) {
		graphLib.change_colormap(COLOR_KEY_GREEN, get_current_color_item(COLOR_KEY_GREEN));
		graphLib.change_colormap(COLOR_KEY_PURPLE, get_current_color_item(COLOR_KEY_PURPLE));
		graphLib.change_colormap(COLOR_KEY_CYAN, get_current_color_item(COLOR_KEY_CYAN));
	}
}

void class_colorcycle::add(int index, struct colorcycle_list items)
{
	colorcycle_map[index] = items;
}

struct st_color class_colorcycle::get_current_color_item(unsigned int key)
{
	struct colorcycle_list current_map = colorcycle_map[current_colormap];
	struct colorcycle_item temp_item = current_map.colors[current_position[key]];
	return temp_item.color[key];
}

bool class_colorcycle::have_colorcycle(unsigned int map_key)
{
	return (colorcycle_map.count(map_key > 0));
}

void class_colorcycle::set_color_key_number(unsigned int map_key, int color_number)
{
	color_keys[map_key] = color_number;
}


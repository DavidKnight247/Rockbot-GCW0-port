#ifndef DIALOGS_H
#define DIALOGS_H

#include <string>
#include <map>
#include <vector>
#include "defines.h"
#include "st_common.h"
#include "file/format.h"

class dialogs
{
public:
    dialogs();
	void show_stage_dialog(int stage);
	void show_boss_dialog(int stage);
	void show_dialog(std::string face_file, bool is_left, std::string lines[3]);
	void show_timed_dialog(std::string face_file, bool is_left, std::string lines[3], short int timer);

private:
	void draw_dialog_bg();

private:
	std::string player_name;
	std::string player_face;
	bool is_showing_dialog_bg;
};

#endif // DIALOGS_H

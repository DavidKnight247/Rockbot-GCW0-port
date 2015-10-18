#include "dialogs.h"


extern std::string FILEPATH;

#include "graphicslib.h"
extern graphicsLib graphLib;

#include "inputlib.h"
extern inputLib input;

extern format_v1_0::st_game_config game_config;

extern format_v_2_0_1::file_game game_data;

dialogs::dialogs() : is_showing_dialog_bg(false)
{
	if (game_config.two_players == 2 || game_config.selected_player == 1) {
		player_name = "Rockbot";
		player_face = "rockbot.png";
	} else {
		player_name = "Betabot";
		player_face = "betabot.png";
	}
}


void dialogs::show_stage_dialog(int stage)
{
	std::cout << "DEBUG - dialogs::show_stage_dialog::stage: " << stage << ", player_n: " << (game_config.selected_player-1) << std::endl;
	if (strlen(game_data.stages[stage].intro_dialog.face_graphics_filename) <= 0) {
		return;
	}

	std::string lines[3];
	for (int i=0; i<3; i++) {
		lines[i] = std::string(game_data.stages[stage].intro_dialog.line1[i]);
	}
	show_dialog(game_data.stages[stage].intro_dialog.face_graphics_filename, game_data.stages[stage].intro_dialog.top_side, lines);
	for (int i=0; i<3; i++) {
		lines[i] = std::string(game_data.stages[stage].intro_dialog.answer1[game_config.selected_player-1][i]);
	}
	show_dialog(game_data.players[game_config.selected_player-1].face_filename, game_data.stages[stage].intro_dialog.top_side, lines); /// @TODO: create "extern" for player number
	if (strlen(game_data.stages[stage].intro_dialog.line2[0]) > 0) {
		for (int i=0; i<3; i++) {
			lines[i] = std::string(game_data.stages[stage].intro_dialog.line2[i]);
		}
		show_dialog(game_data.stages[stage].intro_dialog.face_graphics_filename, game_data.stages[stage].intro_dialog.top_side, lines);
	}
}



void dialogs::show_boss_dialog(int stage)
{
	std::cout << "DEBUG - dialogs::show_stage_dialog::stage: " << stage << ", player_n: " << (game_config.selected_player-1) << std::endl;
	if (strlen(game_data.stages[stage].intro_dialog.face_graphics_filename) <= 0) {
		return;
	}

	std::string lines[3];
	for (int i=0; i<3; i++) {
		lines[i] = std::string(game_data.stages[stage].boss_dialog.line1[i]);
	}
	std::cout << "stage: " << stage << ", boss_face: '" << game_data.stages[stage].boss.face_graphics_filename << "'" << std::endl;
	if (strlen(game_data.stages[stage].boss.face_graphics_filename) <= 0) {
		sprintf(game_data.stages[stage].boss.face_graphics_filename, "%s", "dr_kanotus.png");
	}
	show_dialog(game_data.stages[stage].boss.face_graphics_filename, game_data.stages[stage].boss_dialog.top_side, lines);
	for (int i=0; i<3; i++) {
		lines[i] = std::string(game_data.stages[stage].boss_dialog.answer1[game_config.selected_player-1][i]);
	}
	show_dialog(game_data.players[game_config.selected_player-1].face_filename, game_data.stages[stage].boss_dialog.top_side, lines); /// @TODO: create "extern" for player number
	if (strlen(game_data.stages[stage].boss_dialog.line2[0]) > 0) {
		for (int i=0; i<3; i++) {
			lines[i] = std::string(game_data.stages[stage].boss_dialog.line2[i]);
		}
		show_dialog(game_data.stages[stage].boss_dialog.face_graphics_filename, game_data.stages[stage].boss_dialog.top_side, lines);
	}
}

void dialogs::show_dialog(std::string face_file, bool top_side, std::string lines[3])
{
	std::string temp_text;
	char temp_char;

	draw_dialog_bg();
	graphLib.updateScreen();
	st_position dialog_pos = graphLib.get_dialog_pos();
	graphLib.place_face(face_file, st_position(dialog_pos.x+8, dialog_pos.y+9));
	graphLib.updateScreen();

	/// @TODO: usar show_config_bg e hide_config_bg da graphLib - modificar para aceitar centered (que é o atual) ou top ou bottom
	for (int i=0; i<3; i++) {
		for (int j=0; j<lines[i].size(); j++) {
			temp_char = lines[i].at(j);
			temp_text = "";
			temp_text += temp_char;

			graphLib.draw_text(j*9+(dialog_pos.x+45), i*11+(dialog_pos.y+9), temp_text);
			graphLib.updateScreen();
			input.waitTime(15);
		}
	}

	input.wait_keypress();
}

void dialogs::show_timed_dialog(std::string face_file, bool is_left, std::string lines[], short timer)
{
	std::string temp_text;
	char temp_char;

	draw_dialog_bg();
	graphLib.updateScreen();
	st_position dialog_pos = graphLib.get_dialog_pos();
	graphLib.place_face(face_file, st_position(dialog_pos.x+8, dialog_pos.y+9));
	graphLib.updateScreen();

	/// @TODO: usar show_config_bg e hide_config_bg da graphLib - modificar para aceitar centered (que é o atual) ou top ou bottom
	for (int i=0; i<3; i++) {
		for (int j=0; j<lines[i].size(); j++) {
			temp_char = lines[i].at(j);
			temp_text = "";
			temp_text += temp_char;

			graphLib.draw_text(j*9+(dialog_pos.x+45), i*11+(dialog_pos.y+9), temp_text);
			//graphLib.draw_text(j*9+53, i*11+9, "A");
			graphLib.updateScreen();
			input.waitTime(15);
		}
	}
	input.waitTime(timer);
}




void dialogs::draw_dialog_bg()
{
	if (is_showing_dialog_bg == true) {
		return;
	}
	graphLib.show_dialog(1, true);
}


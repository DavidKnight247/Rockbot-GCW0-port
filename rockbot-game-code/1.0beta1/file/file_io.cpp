#include "file_io.h"

extern std::string FILEPATH;

file_io::file_io()
{
}

void file_io::write_game(format_v2_0::file_game& data_in) {
	std::ofstream fp;
	std::string filename = std::string(FILEPATH) + "data/game_001.dat";

	fp.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::ate);
	if (!fp.is_open()) {
		std::cout << "ERROR::write_game - could not write to file '" << filename << std::endl;
		return;
	}

	std::cout << "############ file_io::write_game - graphic_filename: " << data_in.game_npcs[0].graphic_filename << std::endl;

	fp.write(reinterpret_cast<char *>(&data_in), sizeof(struct format_v2_0::file_game));
	fp.close();
}

void file_io::read_game(format_v2_0::file_game& data_out) {
	std::ifstream fp;
	std::string filename = std::string(FILEPATH) + "data/game_001.dat";

	//std::cout << "file_io::read_game - filename: " << filename << std::endl;

	fp.open(filename.c_str(), std::ios::in | std::ios::binary | std::ios::app);
	if (!fp.is_open()) {
		std::cout << "ERROR::read_game - could not load file '" << filename << std::endl;
		exit(-1);
	}

	fp.read(reinterpret_cast<char *>(&data_out), sizeof(struct format_v2_0::file_game));

	std::cout << "############ file_io::write_game - graphic_filename: " << data_out.game_npcs[0].graphic_filename << std::endl;

	fp.close();
}

namespace format_v_2_0_1 {

	file_io::file_io()
	{
	}

	void file_io::write_game(format_v_2_0_1::file_game& data_in) {
		std::ofstream fp;
		std::string filename = std::string(FILEPATH) + "data/game_v201_001.dat";

		fp.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::ate);
		if (!fp.is_open()) {
			std::cout << "ERROR::write_game - could not write to file '" << filename << std::endl;
			return;
		}

		std::cout << "############ file_io::write_game(format_v_2_0_1) - graphic_filename: " << data_in.game_npcs[0].graphic_filename << std::endl;
		fp.write(reinterpret_cast<char *>(&data_in), sizeof(struct format_v_2_0_1::file_game));
		fp.close();
	}


	void file_io::read_game(format_v_2_0_1::file_game& data_out) {
		std::ifstream fp;
		std::string filename = std::string(FILEPATH) + "data/game_v201_001.dat";

		//std::cout << "file_io::read_game - filename: " << filename << std::endl;

		fp.open(filename.c_str(), std::ios::in | std::ios::binary | std::ios::app);
		if (!fp.is_open()) {
			std::cout << "ERROR::read_game - could not load file '" << filename << std::endl;
			exit(-1);
		}

		fp.read(reinterpret_cast<char *>(&data_out), sizeof(struct format_v_2_0_1::file_game));

		fp.close();
	}



	bool file_io::save_exists()
	{
		std::ifstream fp;
		std::string filename = std::string(FILEPATH) + "data/game.sav";

		std::cout << "file_io::read_game - save_exists: " << filename << std::endl;

		fp.open(filename.c_str(), std::ios::in | std::ios::binary | std::ios::app);
		if (!fp.is_open()) {
			std::cout << "WARNING::save_exists - could not open save file '" << filename << std::endl;
			return false;
		}
		return true;
	}

	void file_io::read_save(format_v_2_0_1::st_save& data_out)
	{
		std::ifstream fp;
		std::string filename = std::string(FILEPATH) + "data/game.sav";

		std::cout << "file_io::read_game - save_exists: " << filename << std::endl;

		fp.open(filename.c_str(), std::ios::in | std::ios::binary | std::ios::app);
		if (!fp.is_open()) {
			std::cout << "ERROR::read_save - could not open save file '" << filename << std::endl;
			return;
		}
		fp.read(reinterpret_cast<char *>(&data_out), sizeof(struct format_v_2_0_1::st_save));

		fp.close();
	}

	void file_io::write_save(format_v_2_0_1::st_save& data_in)
	{
		std::ofstream fp;
		std::string filename = std::string(FILEPATH) + "data/game.sav";

		fp.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::ate);
		if (!fp.is_open()) {
			std::cout << "ERROR::write_save - could not write to save file '" << filename << std::endl;
			return;
		}

		fp.write(reinterpret_cast<char *>(&data_in), sizeof(struct format_v_2_0_1::st_save));
		fp.close();
	}




}

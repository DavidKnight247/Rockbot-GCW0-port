#ifndef FILE_IO_H
#define FILE_IO_H

#include "format.h"
#include <string>
#include <fstream>
#include <iostream>

extern std::string FILEPATH;


class file_io
{
public:
    file_io();
	void read_game(format_v2_0::file_game& data_out);
	void write_game(format_v2_0::file_game& data_in);
};

namespace format_v_2_0_1 {
	class file_io
	{
	public:
		file_io();
		void read_game(format_v_2_0_1::file_game& data_out);
		void write_game(format_v_2_0_1::file_game& data_in);
		bool save_exists();
		void read_save(format_v_2_0_1::st_save& data_out);
		void write_save(format_v_2_0_1::st_save& data_in);
	};
}

#endif // FILE_IO_H

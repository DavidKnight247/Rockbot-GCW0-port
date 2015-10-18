#ifndef FILE_IO_H
#define FILE_IO_H

#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "format.h"

#ifdef WII
#include <fat.h>
#endif


extern std::string FILEPATH; /**< TODO */


/**
 * @brief
 *
 */
class file_io
{
public:


};








namespace CURRENT_FILE_FORMAT {
    /**
     * @brief
     *
     */
    class file_io
    {
    public:
/**
 * @brief
 *
 */
        file_io();
        /**
         * @brief
         *
         * @param data_out
         */
        void read_game(CURRENT_FILE_FORMAT::file_game& data_out) const;
        /**
         * @brief
         *
         * @param data_in
         */
        void write_game(CURRENT_FILE_FORMAT::file_game& data_in) const;

        /**
         * @brief
         *
         * @param stages_data_in
         */
        void write_all_stages(CURRENT_FILE_FORMAT::file_stages& stages_data_in) const;
        /**
         * @brief
         *
         * @param stages_data_out
         */
        void read_all_stages(file_stages &stages_data_out);
        /**
         * @brief
         *
         * @param stages_data_out
         * @param stage_n
         */
        void read_stage(file_stage& stages_data_out, short stage_n);

        bool file_exists(std::string filename) const;

        void check_conversion() const;

        bool write_save(CURRENT_FILE_FORMAT::st_save& data_in);
        void read_save(CURRENT_FILE_FORMAT::st_save& data_out) const;
        bool save_exists() const;

        void load_config(st_game_config &config);
        void save_config(st_game_config &config) const;
        int read_stage_boss_id(int stage_n);

        void read_colormap(SDL_Color (&colormap)[COLOR_COUNT]);

    private:
        std::vector<std::string> split(std::string str,std::string sep);


    };


}


#endif // FILE_IO_H

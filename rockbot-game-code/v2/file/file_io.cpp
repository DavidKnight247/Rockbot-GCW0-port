#include <string.h>

#include "file_io.h"
#include "convert.h"
#include "timerlib.h"


extern std::string FILEPATH;
extern std::string SAVEPATH;

extern CURRENT_FILE_FORMAT::st_game_config game_config;




namespace CURRENT_FILE_FORMAT {

    file_io::file_io()
    {

    }


    void file_io::write_game(CURRENT_FILE_FORMAT::file_game& data_in) const {
        std::ofstream fp;
        std::string filename = std::string(FILEPATH) + "data/game_v3.dat";

        fp.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::ate);
        if (!fp.is_open()) {
            std::cout << "ERROR::write_game - could not write to file '" << filename << "'. Will create new one." << std::endl;
            fp.open(filename.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
            //return;
        } else {
            std::cout << "fio::write_game - recorded to file '" << filename << std::endl;
        }

        fp.write(reinterpret_cast<char *>(&data_in), sizeof(struct CURRENT_FILE_FORMAT::file_game));
        fp.close();

        // ----------------------- STAGES ----------------------- //

    }

    void file_io::write_all_stages(CURRENT_FILE_FORMAT::file_stages &stages_data_in) const
    {
        std::ofstream fp;
        std::string filename = std::string(FILEPATH) + "data/stages_v3.dat";
        fp.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::ate);
        if (!fp.is_open()) {
            std::cout << "ERROR::write_all_stages - could not write to file '" << filename << "'. Will create new one." << std::endl;
            fp.open(filename.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
        } else {
            std::cout << "fio::write_game - recorded to file '" << filename << std::endl;
        }
        fp.write(reinterpret_cast<char *>(&stages_data_in), sizeof(struct CURRENT_FILE_FORMAT::file_stages));
        fp.close();
    }



    void file_io::read_game(CURRENT_FILE_FORMAT::file_game& data_out) const {
        FILE *fp;
        std::string filename = std::string(FILEPATH) + "data/game_v3.dat";
        fp = fopen(filename.c_str(), "rb");
        if (!fp) {
            std::cout << ">>file_io::read_game - file '" << filename << "' not found." << std::endl;
            fflush(stdout);
            return;
        }
        int read_result = fread(&data_out, sizeof(struct CURRENT_FILE_FORMAT::file_game), 1, fp);
        if (read_result  == -1) {
            std::cout << ">>file_io::read_game - Error reading struct data from game file '" << filename << "'." << std::endl;
            fflush(stdout);
            exit(-1);
        }
        fclose(fp);
    }


    void file_io::read_all_stages(CURRENT_FILE_FORMAT::file_stages& stages_data_out)
    {
        std::ifstream fp;
        std::string filename = std::string(FILEPATH) + "data/stages_v3.dat";
        fp.open(filename.c_str(), std::ios::in | std::ios::binary | std::ios::app);
        if (!fp.is_open()) {
            std::cout << "ERROR::read_game - could not load file '" << filename << "'" << std::endl;
            return;
        }
        fp.read(reinterpret_cast<char *>(&stages_data_out), sizeof(struct format_v_3_0_0::file_stages));
        fp.close();
    }

    void file_io::read_stage(CURRENT_FILE_FORMAT::file_stage &stages_data_out, short stage_n)
    {
        FILE *fp;
        std::string filename = std::string(FILEPATH) + "data/stages_v3.dat";
        fp = fopen(filename.c_str(), "rb");
        if (!fp) {
            printf("ERROR.read_stage: Could not read stage '%s'\n", filename.c_str());
            fflush(stdout);
            return;
        }
        fseek(fp, sizeof(CURRENT_FILE_FORMAT::file_stage) * stage_n, SEEK_SET);
        int read_result = fread(&stages_data_out, sizeof(struct CURRENT_FILE_FORMAT::file_stage), 1, fp);
        if (read_result == -1) {
            printf(">>file_io::read_game - Error reading struct data from stage file.\n");
            fflush(stdout);
            exit(-1);
        }
        fclose(fp);
    }

    bool file_io::file_exists(std::string filename) const
    {
        bool res = false;
        FILE *fp;
        fp = fopen(filename.c_str(), "rb");
        if (fp) {
            res = true;
            fclose(fp);
        }
        return res;
    }

    void file_io::check_conversion() const
    {
        /*
        std::string filename_v211 = std::string(FILEPATH) + "data/game_v211.dat";
        std::string filename_v3 = std::string(FILEPATH) + "data/game_v3.dat";
        if (file_exists(filename_v3) == false && file_exists(filename_v211) == true) {
            convert convert_obj;
            convert_obj.v211_to_v3();
        }
        */
    }

    bool file_io::save_exists() const
    {
        std::string filename = std::string(SAVEPATH) + "/game_v3.sav";
        std::ifstream fp(filename.c_str());
        if (fp.good()) {
            return true;
        }
        return false;
    }

    void file_io::load_config(CURRENT_FILE_FORMAT::st_game_config& config)
    {
        FILE *fp;
        std::string filename = std::string(SAVEPATH) + "/config_v3.sav";
        fp = fopen(filename.c_str(), "rb");
        if (!fp) {
            std::cout << "WARNING: Could not read config file '" << filename.c_str() << "'." << std::endl;
        } else {
            int read_result = fread(&config, sizeof(struct CURRENT_FILE_FORMAT::st_game_config), 1, fp);
            if (read_result  == -1) {
                printf(">>file_io::read_game - Error reading struct data from game file '%s'.\n", filename.c_str());
                fflush(stdout);
                exit(-1);
            }
            fclose(fp);
        }
        if (config.get_current_platform() != config.platform) {
            config.reset();
        }
#ifndef PC
        config.video_filter = VIDEO_FILTER_NOSCALE;
        std::cout << "IO::load_config - SET video_filter to " << VIDEO_FILTER_NOSCALE << ", value: " << config.video_filter << std::endl;
#endif
        // ------- DEBUG ------- //
        //config.video_filter = VIDEO_FILTER_NOSCALE;
        // ------- DEBUG ------- //
    }

    void file_io::save_config(st_game_config &config) const
    {
        FILE *fp;
        std::string filename = std::string(SAVEPATH) + "/config_v3.sav";
        fp = fopen(filename.c_str(), "wb");
        if (!fp) {
            std::cout << "Error: Could not open config file '" << filename << "'." << std::endl;
            exit(-1);
        }
        fwrite(&config, sizeof(struct CURRENT_FILE_FORMAT::st_game_config), 1, fp);
        fclose(fp);
    }

    void file_io::read_save(CURRENT_FILE_FORMAT::st_save& data_out) const
    {
        FILE *fp;
        std::string filename = std::string(SAVEPATH) + "/game_v3.sav";
        fp = fopen(filename.c_str(), "rb");
        if (!fp) {
            std::cout << "ERROR: Could not read save" << std::endl;
            exit(-1);
        }
        int read_result = fread(&data_out, sizeof(struct CURRENT_FILE_FORMAT::st_save), 1, fp);
        if (read_result  == -1) {
            printf(">>file_io::read_game - Error reading struct data from game file '%s'.\n", filename.c_str());
            fflush(stdout);
            exit(-1);
        }
        // ------- DEBUG ------- //
        for (int i=0; i<9; i++) {
            data_out.stages[i] = 1;
        }
        for (int i=9; i<STAGE_COUNT; i++) {
            data_out.stages[i] = 0;
        }

        /*
        */
        //data_out.stages[INTRO_STAGE] = 1;
        data_out.stages[SPIKEBOT] = 0;
        //data_out.stages[SEAHORSEBOT] = 1;
        //data_out.stages[DAISIEBOT] = 1;
        //data_out.stages[MUMMYBOT] = 1;
        //data_out.items.lifes = 0;
        // ------- DEBUG ------- //


        if (data_out.items.lifes > 9) {
            data_out.items.lifes = 3;
        }

        fclose(fp);
    }

    bool file_io::write_save(CURRENT_FILE_FORMAT::st_save& data_in)
    {
        FILE *fp;
        std::string filename = std::string(SAVEPATH) + "/game_v3.sav";
        fp = fopen(filename.c_str(), "wb");
        if (!fp) {
            std::cout << "Error: Could not open save-file '" << filename << "'." << std::endl;
            return false;
        }
        fwrite(&data_in, sizeof(struct CURRENT_FILE_FORMAT::st_save), 1, fp);
        fclose(fp);
        return true;
    }


    int file_io::read_stage_boss_id(int stage_n)
    {
        FILE *fp;
        std::string filename = std::string(FILEPATH) + "data/stages_v3.dat";
        fp = fopen(filename.c_str(), "rb");
        if (!fp) {
            printf("ERROR.read_stage: Could not read stage '%s'\n", filename.c_str());
            std::fflush(stdout);
            exit(-1);
        }

        // WHY is it missing by 2 bytes?
        int extra_seek = 2 + sizeof(short) + sizeof(char)*CHAR_NAME_SIZE + sizeof(char)*CHAR_FILENAME_SIZE + sizeof(CURRENT_FILE_FORMAT::file_map)*STAGE_MAX_MAPS + sizeof(CURRENT_FILE_FORMAT::file_link)*STAGE_MAX_LINKS;
        fseek(fp, sizeof(CURRENT_FILE_FORMAT::file_stage) * stage_n + extra_seek, SEEK_SET);
        CURRENT_FILE_FORMAT::file_boss boss;
        int read_result = fread(&boss, sizeof(struct CURRENT_FILE_FORMAT::file_boss), 1, fp);
        if (read_result == -1) {
            printf(">>file_io::read_game - Error reading struct data from stage file.\n");
            std::fflush(stdout);
            exit(-1);
        }
        fclose(fp);
        return boss.id_npc;
    }

    void file_io::read_colormap(SDL_Color (&colormap)[COLOR_COUNT])
    {
        int line_number = 0;
        FILE *fp;
        std::string filename = std::string(FILEPATH) + "data/rockbot.gpl";

        fp = fopen(filename.c_str(), "rb");

        if (!fp) {
            printf("ERROR.read_stage: Could not read stage '%s'\n", filename.c_str());
            std::fflush(stdout);
            exit(-1);
        }

        char line[128];
        int n = 0;                                      // used to ignore first 4 lines
        while (fgets(line, sizeof line, fp) != NULL) {  // get a line from text
            //std::cout << ">>>>> file_io::read_colormap - line: " << line << std::endl;
            if (n <= 3) {
                n++;
                continue;
            }
            // now split the string by its spaces
            std::vector<std::string> splited_line = split(std::string(line), std::string(" "));
            std::vector<std::string>::iterator it;
            int i = 0;
            for (it=splited_line.begin(); it!=splited_line.end(); it++) {
                //std::cout << "'" << (*it) << "', ";

                int number = 0;
                std::istringstream is((*it));
                is >> number;


                if (i == 0) {
                    colormap[line_number].r = number;
                } else if (i == 1) {
                    colormap[line_number].g = number;
                } else if (i == 2) {
                    colormap[line_number].b = number;
                } else if (i > 2) { // limit to the first 3 elements
                    break;
                }
                //std::cout << "'" << number << "', ";
                i++;
            }
            //std::cout << std::endl;

            line_number++;
            n++;
        }


    }

    std::vector<std::string> file_io::split(std::string str, std::string sep)
    {
        char* cstr = const_cast<char*>(str.c_str());
        char* current;
        std::vector<std::string> arr;
        current = strtok(cstr, sep.c_str());
        while(current != NULL){
            arr.push_back(current);
            current=strtok(NULL, sep.c_str());
        }
        return arr;
    }
}



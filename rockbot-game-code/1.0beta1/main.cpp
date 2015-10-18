#include <iostream>
using namespace std;


#include <SDL/SDL.h>				//Include da SDL
#include <SDL/SDL_image.h>		//Include da biblioteca SDL_Image
#include <SDL/SDL_mixer.h>	// Include da biblioteca SDL_Mixer
#include <SDL/SDL_ttf.h>		// Include da biblioteca SDL_ttf
#include <SDL/SDL_endian.h>
#include <SDL/SDL_mixer.h>


// GLOBAL/EXTERN VARIABLES
string FILEPATH;
SDL_Event event;

#include "graphicslib.h"
#include "inputlib.h"
#include "timerlib.h"
#include "soundlib.h"
#include "game.h"
#include "scenes/ending.h"

#ifdef DINGUX
std::string EXEC_NAME("rockbot.dge");
#elif defined(WIN32)
std::string EXEC_NAME("rockbot.exe");
#else
std::string EXEC_NAME("rockbot");
#endif

// INTERNAL GLOBALS
timerLib timer;
inputLib input;
soundLib soundManager;
graphicsLib graphLib;
game gameControl;
format_v_2_0_1::st_save game_save;
struct format_v1_0::st_game_config game_config;
bool freeze_weapon_effect = false;


#include "file/file_io.h"
format_v_2_0_1::file_game game_data;

bool quick_load;

struct format_v_2_0_1::st_checkpoint checkpoint;

int main(int argc, char *argv[])
{
	quick_load = false;
	UNUSED(argc);
	gameControl.currentStage = TECHNOBOT;
	game_config.selected_player = 1;

	//strncpy (FILEPATH, argv[0], strlen(argv[0])-11);

	string argvString = string(argv[0]);

	FILEPATH = argvString.substr(0, argvString.size()-EXEC_NAME.size());
	//std::cout << "main - FILEPATH: " << FILEPATH << std::endl;

	format_v_2_0_1::file_io fio;
	fio.read_game(game_data);

	// INIT GRAPHICS
	if (graphLib.initGraphics() != true) {
		exit(-1);
	}

	// INIT GAME
	if (quick_load == false) {
		if (gameControl.showIntro() == false) {
			return 0;
		}
	} else {
		gameControl.quick_load_game();
		//ending game_ending;
		//game_ending.start();
	}

	input.clean();
	input.p1_input[BTN_START] = 0;
	input.waitTime(200);
	input.clean();
	while (true) {
		input.readInput();
		if (input.p1_input[BTN_QUIT] == 1) {
			exit(-1);
		}
		gameControl.showGame();
		graphLib.updateScreen();
	}
	/// @TODO: sdl quit sub-systems
	SDL_Quit();
	return 1;
}

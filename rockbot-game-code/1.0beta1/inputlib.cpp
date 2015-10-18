#include "inputlib.h"

#include <SDL/SDL_joystick.h>

extern SDL_Event event;

#include "timerlib.h"
extern timerLib timer;

#include "graphicslib.h"
extern graphicsLib graphLib;

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
inputLib::inputLib() : _used_keyboard(false)
{
	for (int i=0; i<BTN_COUNT; i++) {
		p1_input[i] = 0;
		p2_input[i] = 0;
	}
}

void inputLib::init_joystick()
{
	SDL_JoystickEventState(SDL_ENABLE);
	joystick1 = SDL_JoystickOpen(0);
	if (joystick1) {
		printf("Opened Joystick 1\n");
		printf("Name: %s\n", SDL_JoystickName(0));
		printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joystick1));
		printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joystick1));
		printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joystick1));
	} else {
		printf("Couldn't open Joystick 1. Error: '%s'\n", SDL_GetError());
	}
	joystick2 = SDL_JoystickOpen(1);
	if (joystick2) {
		printf("Opened Joystick 2\n");
	} else {
		printf("Couldn't open Joystick 2\n");
	}
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void inputLib::clean()
{
	for (int i=0; i<BTN_COUNT; i++) {
		p1_input[i] = 0;
		p2_input[i] = 0;
	}
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void inputLib::readInput()
{

	//std::cout << "****** inputLib::readInput - used_keyboard: " << _used_keyboard << std::endl;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
//#if !defined(DINGUX) && !defined(PLAYSTATION2) // key events are not needed for consoles with only joystick
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						p1_input[BTN_UP] = 1;
						_used_keyboard = true;
						break;
					case SDLK_DOWN:
						p1_input[BTN_DOWN] = 1;
						_used_keyboard = true;
						break;
					case SDLK_LEFT:
						p1_input[BTN_LEFT] = 1;
						_used_keyboard = true;
						break;
					case SDLK_RIGHT:
						p1_input[BTN_RIGHT] = 1;
						_used_keyboard = true;
						break;
					case SDLK_x:
						p1_input[BTN_JUMP] = 1;
						p1_input[BTN_JUMP_TIMER] = timer.getTimer();
						break;
					case SDLK_a:
						p1_input[BTN_ATTACK] = 1;
						break;
					case SDLK_RETURN:
						p1_input[BTN_START] = 1;
						break;
					case SDLK_q:
						p1_input[BTN_L] = 1;
						break;
					case SDLK_w:
						p1_input[BTN_R] = 1;
						break;
					case SDLK_z:
						p1_input[BTN_SHIELD] = 1;
						break;
					case SDLK_c:
						p1_input[BTN_DASH] = 1;
						break;
#ifndef OPEN_PANDORA
					// alternative Keys
					case SDLK_ESCAPE:
						p1_input[BTN_QUIT] = 1;
						break;
					case SDLK_LCTRL:
						p1_input[BTN_JUMP] = 1;
						p1_input[BTN_JUMP_TIMER] = timer.getTimer();
						break;
					case SDLK_LALT:
						p1_input[BTN_ATTACK] = 1;
						break;
#else
					case SDLK_LCTRL:
						p1_input[BTN_QUIT] = 1;
						break;
					case SDLK_PAGEDOWN:
						p1_input[BTN_JUMP] = 1;
						p1_input[BTN_JUMP_TIMER] = timer.getTimer();
						break;
					case SDLK_END:
						p1_input[BTN_ATTACK] = 1;
						break;
					case SDLK_RSHIFT:
						p1_input[BTN_L] = 1;
						break;
					case SDLK_RCTRL:
						p1_input[BTN_R] = 1;
						break;
					case SDLK_LALT:
						p1_input[BTN_START] = 1;
						break;
					case SDLK_HOME:
						p1_input[BTN_DASH] = 1;
						break;
					case SDLK_PAGEUP:
						p1_input[BTN_SHIELD] = 1;
						break;
#endif
					// default
					default:
						break;
				}
				break;

			case SDL_KEYUP:
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						p1_input[BTN_UP] = 0;
						break;
					case SDLK_DOWN:
						p1_input[BTN_DOWN] = 0;
						break;
					case SDLK_LEFT:
						p1_input[BTN_LEFT] = 0;
						break;
					case SDLK_RIGHT:
						p1_input[BTN_RIGHT] = 0;
						break;
					case SDLK_x:
						p1_input[BTN_JUMP] = 0;
						p1_input[BTN_JUMP_TIMER] = 0;
						break;
					case SDLK_a:
						p1_input[BTN_ATTACK] = 0;
						break;
					case SDLK_RETURN:
						p1_input[BTN_START] = 0;
						break;
					case SDLK_q:
						p1_input[BTN_L] = 0;
						break;
					case SDLK_w:
						p1_input[BTN_R] = 0;
						break;
					case SDLK_z:
						p1_input[BTN_SHIELD] = 0;
						break;
					case SDLK_c:
						p1_input[BTN_DASH] = 0;
						break;
						// alternative Keys
#ifndef OPEN_PANDORA
					case SDLK_LCTRL:
						p1_input[BTN_JUMP] = 0;
						p1_input[BTN_JUMP_TIMER] = 0;
						break;
					case SDLK_LALT:
						p1_input[BTN_ATTACK] = 0;
						break;
					case SDLK_ESCAPE:
						p1_input[BTN_QUIT] = 0;
						break;
#else
					case SDLK_LCTRL:
						p1_input[BTN_QUIT] = 0;
						break;
					case SDLK_PAGEDOWN:
						p1_input[BTN_JUMP] = 0;
						p1_input[BTN_JUMP_TIMER] = 0;
						break;
					case SDLK_END:
						p1_input[BTN_ATTACK] = 0;
						break;
					case SDLK_RSHIFT:
						p1_input[BTN_L] = 0;
						break;
					case SDLK_RCTRL:
						p1_input[BTN_R] = 0;
						break;
					case SDLK_LALT:
						p1_input[BTN_START] = 0;
						break;
					case SDLK_HOME:
						p1_input[BTN_DASH] = 0;
						break;
					case SDLK_PAGEUP:
						p1_input[BTN_SHIELD] = 0;
						break;
#endif
					// default
					default:
						break;
				}
//#endif
			case SDL_JOYAXISMOTION:
				if (_used_keyboard == true) {
					break;
				}
				//printf(">> SDL_JOYAXISMOTION - axis: %d, value: %d, which: %d <<\n", event.jaxis.axis, event.jaxis.value, event.jaxis.which);
				if (event.jaxis.axis == 0) {
					if (event.jaxis.value < -JOYVAL) {
						p1_input[BTN_RIGHT] = 0;
						p1_input[BTN_LEFT] = 1;
						_used_keyboard = false;
					} else if (event.jaxis.value > JOYVAL) {
						p1_input[BTN_RIGHT] = 1;
						p1_input[BTN_LEFT] = 0;
						_used_keyboard = false;
					} else if ((event.jaxis.value < JOYVAL || event.jaxis.value > -JOYVAL) && _used_keyboard == false) {
						p1_input[BTN_RIGHT] = 0;
						p1_input[BTN_LEFT] = 0;
					}
				}
				if (event.jaxis.axis == 1) {
					if (event.jaxis.value < -JOYVAL) {
						p1_input[BTN_DOWN] = 0;
						p1_input[BTN_UP] = 1;
						_used_keyboard = false;
					} else if (event.jaxis.value > JOYVAL) {
						p1_input[BTN_DOWN] = 1;
						p1_input[BTN_UP] = 0;
						_used_keyboard = false;
					} else if ((event.jaxis.value < JOYVAL || event.jaxis.value > -JOYVAL) && _used_keyboard == false) {
						p1_input[BTN_DOWN] = 0;
						p1_input[BTN_UP] = 0;
					}
				}
				break;

				// TODO - use SDL_JOYBUTTONUP to measure the time of jump
			case SDL_JOYBUTTONDOWN:
				if (_used_keyboard == true) {
					break;
				}
				if (event.jbutton.button == JOY_X) {
					/// @ TODO - support for joy2 using witch
					p1_input[BTN_JUMP] = 1;
					p1_input[BTN_JUMP_TIMER] = timer.getTimer();
				}
				if (event.jbutton.button == JOY_START) {
					p1_input[BTN_START] = 1;
				}
#ifndef PLAYSTATION2
				if (event.jbutton.button == JOY_SELECT) {
					p1_input[BTN_QUIT] = 1;
				}
#endif
				if (event.jbutton.button == JOY_SQUARE) {
					p1_input[BTN_ATTACK] = 1;
				}
				if (event.jbutton.button == JOY_L1) {
					p1_input[BTN_L] = 1;
				}
				if (event.jbutton.button == JOY_R1) {
					p1_input[BTN_R] = 1;
				}
				if (event.jbutton.button == JOY_CIRCLE) {
					p1_input[BTN_DASH] = 1;
				}
				if (event.jbutton.button == JOY_TRIANGLE) {
					p1_input[BTN_SHIELD] = 1;
				}

				break;
			case SDL_JOYBUTTONUP:
				if (_used_keyboard == true) {
					break;
				}
				//printf("DEBUG - button released, number: %d\n", event.jbutton.button);
				if (event.jbutton.button == JOY_X) {
					p1_input[BTN_JUMP] = 0;
					p1_input[BTN_JUMP_TIMER] = 0;
				}
				if (event.jbutton.button == JOY_SQUARE) {
					p1_input[BTN_ATTACK] = 0;
				}
				if (event.jbutton.button == JOY_CIRCLE) {
					p1_input[BTN_DASH] = 0;
				}
				if (event.jbutton.button == JOY_TRIANGLE) {
					p1_input[BTN_SHIELD] = 0;
				}
				if (event.jbutton.button == JOY_L1) {
					p1_input[BTN_L] = 0;
				}
				if (event.jbutton.button == JOY_R1) {
					p1_input[BTN_R] = 0;
				}
				break;
#ifndef PLAYSTATION2
			case SDL_QUIT:
				exit(-1);
				break;
#endif
			case SDL_JOYHATMOTION:
				if (_used_keyboard == true) {
					break;
				}
				// CODES: up - 1, right: 2, down: 4, left: 8
				if (event.jhat.value == 1 || event.jhat.value == 3 || event.jhat.value == 9) { // up
					p1_input[BTN_DOWN] = 0;
					p1_input[BTN_UP] = 1;
					_used_keyboard = false;
				}
				if (event.jhat.value == 2 || event.jhat.value == 3 || event.jhat.value == 6) { // right
					p1_input[BTN_RIGHT] = 1;
					p1_input[BTN_LEFT] = 0;
					_used_keyboard = false;
				}
				if (event.jhat.value == 4 || event.jhat.value == 6 || event.jhat.value == 12) { // down
					p1_input[BTN_DOWN] = 1;
					p1_input[BTN_UP] = 0;
					_used_keyboard = false;
				}
				if (event.jhat.value == 8 || event.jhat.value == 9 || event.jhat.value == 12) { // left
					p1_input[BTN_LEFT] = 1;
					p1_input[BTN_RIGHT] = 0;
					_used_keyboard = false;
				}
				if (event.jhat.value == 0 && _used_keyboard == false) {
					p1_input[BTN_LEFT] = 0;
					p1_input[BTN_RIGHT] = 0;
					p1_input[BTN_DOWN] = 0;
					p1_input[BTN_UP] = 0;
				}
				break;
			default:
				break;
		}

	}
	SDL_PumpEvents(); // check keyboard events
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void inputLib::waitTime(int wait_period) {
	int now_time=0;
	now_time = timer.getTimer();
	wait_period = now_time + wait_period;
	while (now_time < wait_period) {
		now_time = timer.getTimer();
		SDL_Delay(1);
		#ifdef PLAYSTATION
			RotateThreadReadyQueue(_MIXER_THREAD_PRIORITY);
		#endif
	}
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
int inputLib::waitScapeTime(int wait_period) {
	int now_time = 0;

	waitTime(50);
	now_time = timer.getTimer();
	wait_period = now_time + wait_period;

	while (now_time < wait_period) {
		readInput();
		if (p1_input[BTN_START] == 1 || p2_input[BTN_START] == 1) {
			return 1;
		} else if (p1_input[BTN_QUIT] == 1 || p2_input[BTN_QUIT] == 1) {
			exit(-1);
		}
		now_time = timer.getTimer();
		//graphLib.exec_colorcycle();
		#ifdef PLAYSTATION
			RotateThreadReadyQueue(_MIXER_THREAD_PRIORITY);
		#endif
	}
	return 0;
}

void inputLib::wait_keypress()
{
    bool fim = false;
    while (!fim) {
        readInput();
		if (p1_input[BTN_START] == 1 || p2_input[BTN_JUMP] == 1 || p1_input[BTN_JUMP] == 1 || p2_input[BTN_JUMP]) {
            fim = true;
        }
        waitTime(50);
    }
    clean();
}

#ifndef INPUTLIB_H
#define INPUTLIB_H

#include "defines.h"
#include "SDL/SDL.h"

class inputLib
{
public:
	inputLib();
	void init_joystick();
	void readInput();
	void waitTime(int wait_period);
	int waitScapeTime(int);
	void clean();
	void wait_keypress(); /// waits intil a key is pressed


protected:


private:

public:
	int p1_input[BTN_COUNT];
	int p2_input[BTN_COUNT];


private:
	SDL_Joystick *joystick1, *joystick2;
	bool _used_keyboard;
};

#endif // INPUTLIB_H

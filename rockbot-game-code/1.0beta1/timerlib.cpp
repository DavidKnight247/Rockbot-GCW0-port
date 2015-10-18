#include "timerlib.h"
#include <SDL/SDL.h>

timerLib::timerLib()
{
}

void timerLib::delay(int waitMS)
{
	SDL_Delay(waitMS);
}

unsigned int timerLib::getTimer()
{
	return SDL_GetTicks();
}

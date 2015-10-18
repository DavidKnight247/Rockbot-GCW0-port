#include "timerlib.h"
#include <SDL/SDL.h>

timerLib::timerLib()
{
}

void timerLib::delay(int waitMS) const
{
    /*
#ifdef PLAYSTATION2
    if (waitMS < 20) {
        return;
    }
#endif
    */
	SDL_Delay(waitMS);
}

unsigned int timerLib::getTimer() const
{
    return SDL_GetTicks();
}

void timerLib::start_ticker()
{
    _ticks = getTimer();
}

unsigned int timerLib::get_ticks()
{
    return getTimer() - _ticks;
}

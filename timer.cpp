#include "timer.hpp"
#include <SDL2/SDL.h>

Timer::Timer()
{
    Start();
}

void Timer::Start()
{
    started_at = SDL_GetTicks();
}

uint32_t Timer::Read() const
{
    return SDL_GetTicks() - started_at;
}

float Timer::ReadSec() const
{
    return float(SDL_GetTicks() - started_at) / 1000.0f;
}

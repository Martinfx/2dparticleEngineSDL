#include "perftimer.hpp"
#include "SDL2/SDL.h"

uint64_t PerfTimer::frequency = 0;

// ---------------------------------------------
PerfTimer::PerfTimer()
{
    if (frequency == 0)
        frequency = SDL_GetPerformanceFrequency();

    Start();
}

// ---------------------------------------------
void PerfTimer::Start()
{
    started_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
double PerfTimer::ReadMs() const
{
    return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}

// ---------------------------------------------
std::uint64_t PerfTimer::ReadTicks() const
{
    return SDL_GetPerformanceCounter() - started_at;
}

#include <iostream>
#include <stdint.h>
#include <list>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <pugixml.hpp>

#include "module.hpp"
#include "window.hpp"
#include "engine.hpp"
#include "input.hpp"

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0
        );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(3000);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


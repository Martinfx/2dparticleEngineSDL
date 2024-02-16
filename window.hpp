#pragma once

#include "module.hpp"

#include <SDL2/SDL.h>

struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:

    Window();

    // Destructor
    virtual ~Window();

    // Called before render is available
    bool Awake(pugi::xml_node&);

    // Called before quitting
    bool CleanUp();

    // Changae title
    void SetTitle(const char* new_title);

    // Retrive window size
    void GetWindowSize(uint& width, uint& height) const;

    // Retrieve window scale
    uint GetScale() const;

public:
    //The window we'll be rendering to
    SDL_Window* window;

    //The surface contained by the window
    SDL_Surface* screen_surface;

private:
    std::string	title;
    std::string	icon;

    // Screen parameters
    uint		width = 0u;
    uint		height = 0u;
    uint		scale = 0u;
};

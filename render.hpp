#pragma once

#include <SDL2/SDL.h>
#include "module.hpp"

class Render : public Module
{
public:

    Render();

    // Destructor
    virtual ~Render();

    // Called before render is available
    bool Awake(pugi::xml_node&);

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool PreUpdate();
    bool Update(float dt);
    bool PostUpdate();

    // Called before quitting
    bool CleanUp();

    // Load / Save
    bool Load(pugi::xml_node&);
    bool Save(pugi::xml_node&) const;

    // Blit
    void SetViewPort(const SDL_Rect& rect);
    void ResetViewPort();

    bool BlitParticle(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, const SDL_Rect* rectSize = NULL, float speed = 1.0f, double angle = 0) const;
    bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;
    bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
    bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
    bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = false, bool use_camera = true) const;

    // Set background color
    void SetBackgroundColor(SDL_Color color);

    //sSDL_Texture* const LoadSurface(SDL_Surface* surface);
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
public:

    SDL_Renderer*	renderer;
    SDL_Rect		camera;
    SDL_Rect		viewport;
    SDL_Color		background;
};

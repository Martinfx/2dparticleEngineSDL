#include "window.hpp"
#include "pugixml.hpp"
Window::Window() : Module()
{
    window = NULL;
    screen_surface = NULL;
    name = "window";
}

// Destructor
Window::~Window()
{
}

// Called before render is available
bool Window::Awake(pugi::xml_node& config)
{
    //LOG("Init SDL window & surface");
    bool ret = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        //LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
        ret = false;
    }
    else
    {
        //Create window
        Uint32 flags = SDL_WINDOW_SHOWN;
        bool fullscreen = config.child("fullscreen").attribute("value").as_bool(false);
        bool borderless = config.child("borderless").attribute("value").as_bool(false);
        bool resizable = config.child("resizable").attribute("value").as_bool(false);
        bool fullscreen_window = config.child("fullscreen_window").attribute("value").as_bool(false);

        width = config.child("resolution").attribute("width").as_int(640);
        height = config.child("resolution").attribute("height").as_int(480);
        scale = config.child("resolution").attribute("scale").as_int(1);

        if(fullscreen == true)
        {
            flags |= SDL_WINDOW_FULLSCREEN;
        }

        if(borderless == true)
        {
            flags |= SDL_WINDOW_BORDERLESS;
        }

        if(resizable == true)
        {
            flags |= SDL_WINDOW_RESIZABLE;
        }

        if(fullscreen_window == true)
        {
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }

        window = SDL_CreateWindow("2D Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

        if(window == NULL)
        {
            //LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            ret = false;
        }
        else
        {
            //Get window surface
            screen_surface = SDL_GetWindowSurface(window);
        }
    }

    return ret;
}

// Called before quitting
bool Window::CleanUp()
{
   /// LOG("Destroying SDL window and quitting all SDL systems");

    //Destroy window
    if(window != NULL)
    {
        SDL_DestroyWindow(window);
    }

    //Quit SDL subsystems
    SDL_Quit();
    return true;
}

// Set new window title
void Window::SetTitle(const char* new_title)
{
    //title.create(new_title);
    SDL_SetWindowTitle(window, new_title);
}

void Window::GetWindowSize(uint& width, uint& height) const
{
    width = this->width;
    height = this->height;
}

uint Window::GetScale() const
{
    return scale;
}

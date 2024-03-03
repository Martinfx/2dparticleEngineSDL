#include "render.hpp"
#include "engine.hpp"

#include <SDL2/SDL_image.h>

#define VSYNC true

Render::Render() : Module()
{
    window = NULL;
    screen_surface = NULL;

    name = "renderer";
    background.r = 0;
    background.g = 0;
    background.b = 0;
    background.a = 0;
}

Render::~Render()
{

}


// Called before render is available
bool Render::Awake(pugi::xml_node& config)
{
    bool ret = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL_VIDEO could not initialize! SDL_Error: %s\n" << SDL_GetError();
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
            std::cerr << "Window could not be created! SDL_Error: %s\n" << SDL_GetError();
            ret = false;
        }
        else
        {
            //Get window surface
          // screen_surface = SDL_GetWindowSurface(window);
        }
    }


    //LOG("Create SDL rendering context");
   // bool ret = true;
    // load flags
    Uint32 flags = SDL_RENDERER_ACCELERATED;

    if(config.child("vsync").attribute("value").as_bool(true) == true)
    {
        flags |= SDL_RENDERER_PRESENTVSYNC;
        //LOG("Using vsync");
    }

    renderer = SDL_CreateRenderer(window, -1, flags);

    if(renderer == NULL)
    {
        //LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
        std::cerr << "Could not create the renderer! SDL_Error: %s\n" << SDL_GetError();
                ret = false;
    }
    else
    {
        camera.w = 640;
        camera.h = 480;
        camera.x = 0;
        camera.y = 0;
    }

    return ret;
}

// Called before the first frame
bool Render::Start()
{
    std::cout  << "render start" << std::endl;
    // back background
    torchTex = Load("textures/torch.png");

    SDL_RenderGetViewport(renderer, &viewport);
    return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
    SDL_RenderClear(renderer);
    return true;
}

bool Render::Update(float dt)
{
    //if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
    //{
        int mxx = 0, myy = 0;
    //    App->input->GetMousePosition(mx, my);
        fPoint poss((float)mxx, (float)myy);
     //   pos.y -= 230.0f;
        ///eFire = App->psystem->AddEmiter(poss, EmitterType::EMITTER_TYPE_FIRE);
    //}

    int mx = 200, my = 300;
    //App->input->GetMousePosition(mx, my);
    fPoint pos((float)mx, (float)my);

    Blit(torchTex, pos.x - 43, pos.y - 270, &rect);

    if (eFire != nullptr)
    {
        int mx, my;
     //   App->input->GetMousePosition(mx, my);
        fPoint pos((float)mx, (float)my);
        pos.y -= 230.0f;
        eFire->MoveEmitter(pos);
    }


    return true;
}

bool Render::PostUpdate()
{    bool ret = true;
   // if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
    //    ret = false;

    SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
    SDL_RenderPresent(renderer);
    return true;
}

// Called before quitting
bool Render::CleanUp()
{
    eFire = nullptr;

    UnLoad(torchTex);
    torchTex = nullptr;

    if(window != NULL)
    {
        SDL_DestroyWindow(window);
    }

    //Quit SDL subsystems
    SDL_Quit();
    ///LOG("Destroying SDL render");
    SDL_DestroyRenderer(renderer);
    return true;
}

// Load Game State
bool Render::Load(pugi::xml_node& data)
{
    camera.x = data.child("camera").attribute("x").as_int();
    camera.y = data.child("camera").attribute("y").as_int();

    return true;
}

// Save Game State
bool Render::Save(pugi::xml_node& data) const
{
    pugi::xml_node cam = data.append_child("camera");

    cam.append_attribute("x") = camera.x;
    cam.append_attribute("y") = camera.y;

    return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
    background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
    SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
    SDL_RenderSetViewport(renderer, &viewport);
}

// Blit particle to screen
bool Render::BlitParticle(SDL_Texture* texture, int x, int y, const SDL_Rect* section, const SDL_Rect* rectSize, float speed, double angle) const
{
    bool ret = true;
    uint scale = GetScale();

    SDL_Rect rect;
    rect.x = (int)(camera.x * speed) + x * scale;
    rect.y = (int)(camera.y * speed) + y * scale;

    if (rectSize != NULL)
    {
        rect.w = rectSize->w;
        rect.h = rectSize->h;
    }
    else if (section != NULL)
    {
        rect.w = section->w;
        rect.h = section->h;
    }
    else
        SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

    int px = rect.w / 2;
    int py = rect.h / 2;

    rect.w *= scale;
    rect.h *= scale;

    SDL_Point* p = NULL;
    SDL_Point pivot;
    pivot.x = px;
    pivot.y = py;
    p = &pivot;


    if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, NULL, SDL_FLIP_NONE) != 0)
    {
        //LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
        ret = false;
    }

    return ret;
}

bool Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivot_x, int pivot_y) const
{
    bool ret = true;
    uint scale = GetScale();

    SDL_Rect rect;
    rect.x = (int)(camera.x * speed) + x * scale;
    rect.y = (int)(camera.y * speed) + y * scale;

    if (section != NULL)
    {
        rect.w = section->w;
        rect.h = section->h;
    }
    else
    {
        SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    }

    rect.w *= scale;
    rect.h *= scale;

    SDL_Point* p = NULL;
    SDL_Point pivot;

    if (pivot_x != INT_MAX && pivot_y != INT_MAX)
    {
        pivot.x = pivot_x;
        pivot.y = pivot_y;
        p = &pivot;
    }

    if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
    {
        std::cerr << "Cannot blit to screen. SDL_RenderCopy error: %s" <<  SDL_GetError();
        ret = false;
    }

    return ret;
}


bool Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
    bool ret = true;
    uint scale = GetScale();

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    SDL_Rect rec(rect);
    if (use_camera)
    {
        rec.x = (int)(camera.x + rect.x * scale);
        rec.y = (int)(camera.y + rect.y * scale);
        rec.w *= scale;
        rec.h *= scale;
    }

    int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

    if (result != 0)
    {
       // LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
        ret = false;
    }

    return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
    bool ret = true;
    uint scale = GetScale();

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    int result = -1;

    if (use_camera)
        result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
    else
        result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

    if (result != 0)
    {
       // LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
        ret = false;
    }

    return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
    bool ret = true;
    uint scale = GetScale();

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    int result = -1;

    if (filled)
    {
        for (int w = 0; w < radius * 2; w++)
        {
            for (int h = 0; h < radius * 2; h++)
            {
                int dx = radius - w; // horizontal offset
                int dy = radius - h; // vertical offset

                if ((dx*dx + dy*dy) <= (radius * radius))
                {
                    result = SDL_RenderDrawPoint(renderer, x + dx, y + dy);

                    if (result != 0)
                        break;
                }
            }
            if (result != 0)
                break;
        }
    }
    else
    {
        SDL_Point points[360];

        float factor = (float)M_PI / 180.0f;

        for (uint i = 0; i < 360; ++i)
        {
            points[i].x = (int)(x + radius * cos(i * factor));
            points[i].y = (int)(y + radius * sin(i * factor));
        }

        result = SDL_RenderDrawPoints(renderer, points, 360);
    }

    if (result != 0)
    {
        std::cerr << "Cannot draw quad to screen. SDL_RenderFillRect error: %s"<< SDL_GetError();
        //LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
        ret = false;
    }

    return ret;
}

// Load new texture from file path
SDL_Texture* const Render::Load(const char* path)
{
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = IMG_Load(path);

    if(surface == NULL)
    {
        std::cerr << "Could not load surface with path" << std::endl;
        //LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
    }
    else
    {
        texture = LoadSurface(surface);
        SDL_FreeSurface(surface);
    }

    return texture;
}

SDL_Texture* const Render::LoadSurface(SDL_Surface* surface)
{

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    if(texture == NULL)
    {
        std::cerr << "Unable to create texture from surface! SDL Error: %s\n" << SDL_GetError();
        //LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        textures.remove(texture);
    }

    return texture;
}
uint Render::GetScale() const
{
    return scale;
}

// Retrieve size of a texture
void Render::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
    SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*) &width, (int*) &height);
}

bool Render::UnLoad(SDL_Texture* texture)
{
    std::list<SDL_Texture*>::const_iterator item;

    for (item = textures.begin(); item != textures.end(); item = ++item)
    {
        if (texture == *item)
        {
            SDL_DestroyTexture(*item);
            textures.remove(*item);
            return true;
        }
    }

    UnLoad(particleAtlas);
    return false;
}

SDL_Texture* Render::GetParticleAtlas() const
{
    return particleAtlas;
}


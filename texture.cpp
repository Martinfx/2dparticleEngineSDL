#include "texture.hpp"
#include "SDL2/SDL_image.h"
#include "engine.hpp"
Textures::Textures() : Module()
{
    name = "textures";
}

Textures::~Textures()
{}

// Called before render is available
bool Textures::Awake(pugi::xml_node& config)
{
    //LOG("Init Image library");
    bool ret = true;
    // load support for the PNG image format
    int flags = IMG_INIT_PNG;
    int init = IMG_Init(flags);

    if((init & flags) != flags)
    {
        //LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
        ret = false;
    }

    return ret;
}

// Called before the first frame
bool Textures::Start()
{
   // LOG("start textures");
    bool ret = true;
   particleAtlas = Load("textures/particles.png");
    return ret;
}

// Called before quitting
bool Textures::CleanUp()
{
    //LOG("Freeing textures and Image library");
    std::list<SDL_Texture*>::const_iterator item;

    for (item = textures.begin(); item != textures.end(); ++item)
    {
        SDL_DestroyTexture(*item);
    }

    textures.clear();
    IMG_Quit();
    return true;
}

// Load new texture from file path
SDL_Texture* const Textures::Load(const char* path)
{
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = IMG_Load(path);

    if(surface == NULL)
    {
        //std::cerr << "Could not load surface with path" << std::endl;
        //LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
    }
    else
    {
        texture = LoadSurface(surface);
        SDL_FreeSurface(surface);
    }

    return texture;
}

// Unload texture
bool Textures::UnLoad(SDL_Texture* texture)
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

// Translate a surface into a texture
SDL_Texture* const Textures::LoadSurface(SDL_Surface* surface)
{
    SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

    if(texture == NULL)
    {
        //LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        textures.remove(texture);
    }

    return texture;
}

// Retrieve size of a texture
void Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
    SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*) &width, (int*) &height);
}

SDL_Texture* Textures::GetParticleAtlas() const
{
    return particleAtlas;
}


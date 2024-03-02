#include "scene.hpp"
#include "engine.hpp"
#include "SDL2/SDL_image.h"
Scene::Scene() : Module()
{
    name = "scene";
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
   // LOG("Loading Scene");
    bool ret = true;

    return ret;
}

// Load new texture from file path
SDL_Texture* const Scene::Load(const char* path)
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

SDL_Texture* const Scene::LoadSurface(SDL_Surface* surface)
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

// Called before the first frame
bool Scene::Start()
{
    torchTex = Load("textures/torch.png");
    return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
    return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
    if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
    {
        int mx, my;
        App->input->GetMousePosition(mx, my);
        fPoint pos((float)mx, (float)my);
        pos.y -= 230.0f;
        eFire = App->psystem->AddEmiter(pos, EmitterType::EMITTER_TYPE_FIRE);
    }

    /*TODO 5 - Tweak the xml parameters
        - Change the emitter data in order to get a flame.
        - Uncomment code in Scene update to blit the torch.
        - Optional: create a new one and try simulate smoke.*/

    int mx, my;
    App->input->GetMousePosition(mx, my);
    fPoint pos((float)mx, (float)my);

    App->render->Blit(torchTex, pos.x - 43, pos.y - 270, &rect);

    if (eFire != nullptr)
    {
        int mx, my;
        App->input->GetMousePosition(mx, my);
        fPoint pos((float)mx, (float)my);
        pos.y -= 230.0f;
        eFire->MoveEmitter(pos);
    }

    return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
    bool ret = true;

    if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
        ret = false;

    return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
    //LOG("Freeing scene");

    eFire = nullptr;

    //UnLoad(torchTex);
    torchTex = nullptr;

    return true;
}

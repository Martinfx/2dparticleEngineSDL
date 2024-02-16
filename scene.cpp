#include "scene.hpp"
#include "engine.hpp"

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

// Called before the first frame
bool Scene::Start()
{
    torchTex = App->tex->Load("textures/torch.png");
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

    App->tex->UnLoad(torchTex);
    torchTex = nullptr;

    return true;
}

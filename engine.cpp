#include "engine.hpp"

// Constructor
Engine::Engine(int argc, char* args[])/* : argc(argc), args(args)*/
{
    input = new Input();
    win = new Window();
    render = new Render();
    tex = new Textures();
    psystem = new ParticleSystem();
    scene = new Scene();


    // Ordered for awake / Start / Update
    // Reverse order of CleanUp
    AddModule(input);
    AddModule(win);
    AddModule(tex);
    AddModule(scene);
    AddModule(psystem);


    // Render last to swap buffer
    AddModule(render);
}

// Destructor
Engine::~Engine()
{
    // release modules
    std::list<Module*>::reverse_iterator it;
    for (it = modules.rbegin(); it != modules.rend(); ++it)
    {
        delete (*it);
    }
    modules.clear();
}

void Engine::AddModule(Module* module)
{
    module->Init();
    modules.push_back(module);
}

// Called before render is available
bool Engine::Awake()
{
    pugi::xml_document	config_file;
    pugi::xml_node		config;
    pugi::xml_node		app_config;

    bool ret = false;

    config = LoadConfig(config_file);

    if(config.empty() == false)
    {
        // self-config
        ret = true;
        app_config = config.child("app");
        title = app_config.child("title").child_value();
        organization = app_config.child("organization").child_value();
        int cap = app_config.attribute("framerate_cap").as_int();

        if (cap > 0)
        {
            capped_ms = 1000 / cap;
        }
    }

    std::list<Module*>::const_iterator item;

    for (item = modules.begin(); item != modules.end() && ret == true; ++item)
    {   auto child = config.child((*item)->name.data());
        ret = (*item)->Awake(child);
    }

   // PERF_PEEK(ptimer);

    return ret;
}

// Called before the first frame
bool Engine::Start()
{
   // PERF_START(ptimer);
    bool ret = true;
    std::list<Module*>::const_iterator item;

    for (item = modules.begin(); item != modules.end() && ret == true; ++item)
    {
        if ((*item)->active)
            ret = (*item)->Start();
    }

  //  PERF_PEEK(ptimer);

    return ret;
}

// Called each loop iteration
bool Engine::Update()
{
    bool ret = true;
    PrepareUpdate();

    if(input->GetWindowEvent(WE_QUIT) == true)
        ret = false;

    if(ret == true)
        ret = PreUpdate();

    if(ret == true)
        ret = DoUpdate();

    if(ret == true)
        ret = PostUpdate();

    FinishUpdate();
    return ret;
}

// ---------------------------------------------
pugi::xml_node Engine::LoadConfig(pugi::xml_document& config_file) const
{
    pugi::xml_node ret;

    pugi::xml_parse_result result = config_file.load_file("config.xml");

    /*if(result == NULL)
       // LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
    else*/
        ret = config_file.child("config");

    return ret;
}

// ---------------------------------------------
void Engine::PrepareUpdate()
{
    frame_count++;
    last_sec_frame_count++;

    dt = frame_time.ReadSec();
    frame_time.Start();
}

// ---------------------------------------------
void Engine::FinishUpdate()
{
    // Framerate calculations --

    if (last_sec_frame_time.Read() > 1000)
    {
        last_sec_frame_time.Start();
        prev_last_sec_frame_count = last_sec_frame_count;
        last_sec_frame_count = 0;
    }

    float avg_fps = float(frame_count) / startup_time.ReadSec();
    float seconds_since_startup = startup_time.ReadSec();
    uint32_t last_frame_ms = frame_time.Read();
    uint32_t frames_on_last_update = prev_last_sec_frame_count;

    static char title[256];
  /*  std::sprintf(title, 256, "Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu ",
              avg_fps, last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frame_count);
*/
    //App->win->SetTitle(title);

    if (capped_ms > 0 && last_frame_ms < capped_ms)
    {
        PerfTimer t;
       // SDL_Delay(capped_ms - last_frame_ms);
       // LOG("We waited for %d milliseconds and got back in %f", capped_ms - last_frame_ms, t.ReadMs());
    }
}

// Call modules before each loop iteration
bool Engine::PreUpdate()
{
    bool ret = true;

    std::list<Module*>::const_iterator item;
    Module* pModule = nullptr;

    for (item = modules.begin(); item != modules.end() && ret == true; ++item)
    {
        pModule = (*item);

        if (pModule->active == false) {
            continue;
        }

        ret = (*item)->PreUpdate();
    }

    return ret;
}

// Call modules on each loop iteration
bool Engine::DoUpdate()
{
    bool ret = true;

    std::list<Module*>::const_iterator item;
    Module* pModule = nullptr;

    for (item = modules.begin(); item != modules.end() && ret == true; ++item)
    {
        pModule = (*item);

        if (pModule->active == false) {
            continue;
        }

        ret = (*item)->Update(dt);
    }

    return ret;
}

// Call modules after each loop iteration
bool Engine::PostUpdate()
{
    bool ret = true;

    std::list<Module*>::const_iterator item;
    Module* pModule = nullptr;

    for (item = modules.begin(); item != modules.end() && ret == true; ++item)
    {
        pModule = (*item);

        if (pModule->active == false) {
            continue;
        }

        ret = (*item)->PostUpdate();
    }

    return ret;
}

// Called before quitting
bool Engine::CleanUp()
{
    //PERF_START(ptimer);
    bool ret = true;

    std::list<Module*>::reverse_iterator it;

    for (it = modules.rbegin(); it != modules.rend(); ++it)
    {
        ret = (*it)->CleanUp();
    }

    //PERF_PEEK(ptimer);
    return ret;
}

// ---------------------------------------
int Engine::GetArgc() const
{
    return argc;
}

// ---------------------------------------
const char* Engine::GetArgv(int index) const
{
    if(index < argc)
        return args[index];
    else
        return NULL;
}

// ---------------------------------------
const char* Engine::GetTitle() const
{
    return title.data();
}

// ---------------------------------------
const char* Engine::GetOrganization() const
{
    return organization.data();
}

pugi::xml_node Engine::LoadEmitters(pugi::xml_document& psystem_file) const
{
    pugi::xml_node ret;

    pugi::xml_parse_result result = psystem_file.load_file("psystem_config");

    /*if (result == NULL)
        LOG("Could not load xml file config.xml. pugi error: %s", result.description());
    else*/
        ret = psystem_file.child("emitters");
    return ret;
}

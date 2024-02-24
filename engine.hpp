#pragma once
#include <iostream>
#include <stdint.h>
#include <list>
#include <pugixml.hpp>

#include "module.hpp"
#include "window.hpp"
#include "input.hpp"
#include "render.hpp"
#include "texture.hpp"
#include "particlesystem.hpp"
#include "scene.hpp"
#include "timer.hpp"

class Engine
{
public:
    // Called before render is available
    bool Awake();

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool Update();

    // Called before quitting
    bool CleanUp();

    // Add a new module to handle
    void AddModule(Module* module);

    // Exposing some properties for reading
    int GetArgc() const;
    const char* GetArgv(int index) const;
    const char* GetTitle() const;
    const char* GetOrganization() const;

public:
    pugi::xml_node LoadEmitters(pugi::xml_document& psystem_file) const;

    Engine() { }
    Engine(Engine *App) ;
    Engine(int argc, char* args[]);
    ~Engine();

private:

    // Load config file
    pugi::xml_node LoadConfig(pugi::xml_document&) const;

    // Call modules before each loop iteration
    void PrepareUpdate();

    // Call modules before each loop iteration
    void FinishUpdate();

    // Call modules before each loop iteration
    bool PreUpdate();

    // Call modules on each loop iteration
    bool DoUpdate();

    // Call modules after each loop iteration
    bool PostUpdate();

public:

    // Modules
    Window*			 win = nullptr;
    Input*			 input = nullptr;
    Render*			 render = nullptr;
    Textures*			 tex = nullptr;
    ParticleSystem*	 psystem = nullptr;
    Scene*			 scene = nullptr;


private:

    std::list<Module*> modules;
    int					 argc;
    char**				 args;

    std::string			 title;
    std::string			 organization;

    PerfTimer			ptimer;
    uint64_t				frame_count = 0;
    Timer				startup_time;
    Timer				frame_time;
    Timer				last_sec_frame_time;
    uint32_t				last_sec_frame_count = 0;
    uint32_t				prev_last_sec_frame_count = 0;
    float				dt = 0.0f;
    int					capped_ms = -1;

};

static Engine* App;

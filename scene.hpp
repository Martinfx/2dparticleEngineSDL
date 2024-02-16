#pragma once

#include "module.hpp"
#include "particle.hpp"
#include "particlepool.hpp"
#include "emitter.hpp"
#include "point.hpp"

struct SDL_Texture;

class Scene : public Module
{
public:

    Scene();

    // Destructor
    virtual ~Scene();

    // Called before render is available
    bool Awake();

    // Called before the first frame
    bool Start();

    // Called before all Updates
    bool PreUpdate();

    // Called each loop iteration
    bool Update(float dt);

    // Called before all Updates
    bool PostUpdate();

    // Called before quitting
    bool CleanUp();

private:

    Emitter* eFire = nullptr;
    SDL_Rect rect = { 0, 0, 400, 400 };
    SDL_Texture* torchTex = nullptr;

};

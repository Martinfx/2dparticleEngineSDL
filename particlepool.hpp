#pragma once

#include "particle.hpp"
#include "emitter.hpp"
#include "point.hpp"

class Emitter;

class ParticlePool
{
private:

    int poolSize = 0;
    Particle* firstAvailable;
public:

    ParticlePool(Emitter* emitter);
    virtual ~ParticlePool();

    //Generates a new particle each time it's called
    void Generate(fPoint pos, float startSpeed, float endSpeed, float angle, float rotSpeed, float startSize, float endSize, uint life, SDL_Rect textureRect, SDL_Color startColor, SDL_Color endColor, SDL_BlendMode blendMode);

    // Update (move and draw) particles in the pool. If there are no particles alive returns false
    bool Update(float dt);
};

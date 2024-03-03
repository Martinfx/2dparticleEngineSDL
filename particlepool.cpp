#include "particlepool.hpp"

#include <cassert>

ParticlePool::ParticlePool(Emitter* emitter)
{
    // Convert particleArray into a free list
    Particle* currentParticle = &firstAvailable[0];
    firstAvailable = currentParticle;

    for (int i = 0; i < emitter->GetPoolSize() - 1; ++i)
    {
        Particle* nextParticle = &firstAvailable[i + 1];
        currentParticle->SetNext(nextParticle);
        currentParticle = nextParticle;
    }

    // Make the last particle point to nullptr
    currentParticle->SetNext(nullptr);
}

ParticlePool::~ParticlePool()
{
    // No cleanup necessary as particleArray is a member array
}

void ParticlePool::Generate(fPoint pos, float startSpeed, float endSpeed, float angle, float rotSpeed, float startSize, float endSize, uint life, SDL_Rect textureRect, SDL_Color startColor, SDL_Color endColor, SDL_BlendMode blendMode)
{
    // Check if the pool is not empty
    assert(firstAvailable != nullptr && "Particle pool is empty!");

    // Init the particle from the first available
    Particle* newParticle = firstAvailable;
    firstAvailable = newParticle->GetNext(); // Move the firstAvailable pointer to the next particle
    newParticle->Init(pos, startSpeed, endSpeed, angle, rotSpeed, startSize, endSize, life, textureRect, startColor, endColor, blendMode);
}

bool ParticlePool::Update(float dt)
{
    bool ret = false;

    // Update and draw living particles in the pool
   /* for (int i = 0; i < 1000; ++i)
    {
        if (particleArray[i].IsAlive())
        {
            particleArray[i].Update(dt);
            particleArray[i].Draw(); // Assuming there's a Draw method in the Particle class
            ret = true;
        }
        else
        {
            // If a particle is dead, it becomes the first available in the pool
            particleArray[i].SetNext(firstAvailable);
            firstAvailable = &particleArray[i];
        }
    }*/

    return ret;
}

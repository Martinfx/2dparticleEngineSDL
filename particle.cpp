#include "particle.hpp"
#include "engine.hpp"

#define PI 3.14159265359f
#define DEG_TO_RAD(a) a * PI / 180.0f
#define RAD_TO_DEG(a) a * 180.0f / PI

Particle::Particle():life(0) { }

void Particle::Init(fPoint pos, float startSpeed, float endSpeed, float angle, double rotSpeed, float startSize, float endSize, uint life, SDL_Rect textureRect, SDL_Color startColor, SDL_Color endColor, SDL_BlendMode blendMode)
{
    // Movement properties
    pState.pLive.pos = pos;
    pState.pLive.startVel.x = startSpeed * cos(DEG_TO_RAD(angle));
    pState.pLive.startVel.y = -startSpeed * sin(DEG_TO_RAD(angle));
    pState.pLive.endVel.x = endSpeed * cos(DEG_TO_RAD(angle));
    pState.pLive.endVel.y = -endSpeed * sin(DEG_TO_RAD(angle));
    pState.pLive.startRotSpeed = rotSpeed;
    pState.pLive.currentRotSpeed = rotSpeed;

    // Life properties
    this->life = pState.pLive.startLife = life;
    pState.pLive.currentSize = pState.pLive.startSize = startSize;
    pState.pLive.endSize = endSize;
    pState.pLive.t = 0.0f;

    // Color properties
    pState.pLive.startColor = startColor;
    pState.pLive.endColor = endColor;
    pState.pLive.blendMode = blendMode;
    pState.pLive.pRect = pState.pLive.rectSize = textureRect;

    // Add vortex to the system (optional and only one is allowed)
    AddVortex({ 250.0f, 200.0f }, 10.0f, 30.0f);
}

void Particle::Update(float dt)
{
    // Age ratio is used to interpolate between particle properties
    pState.pLive.ageRatio = (float)life / (float)pState.pLive.startLife;

    // Particle size interpolation
    pState.pLive.currentSize = InterpolateBetweenRange(pState.pLive.startSize, pState.pLive.t, pState.pLive.endSize);

    // Particle speed interpolation
    pState.pLive.currentVel.x = InterpolateBetweenRange(pState.pLive.startVel.x, pState.pLive.t, pState.pLive.endVel.x);
    pState.pLive.currentVel.y = InterpolateBetweenRange(pState.pLive.startVel.y, pState.pLive.t, pState.pLive.endVel.y);

    // Assign new size to particle rect
    pState.pLive.rectSize.w = pState.pLive.rectSize.h = pState.pLive.currentSize;

    // Calculating new particle position
    pState.pLive.pos.x += pState.pLive.currentVel.x * dt;
    pState.pLive.pos.y += pState.pLive.currentVel.y * dt;

    // BONUS CODE. Uncomment to unchain vortices
    //CalculateParticlePos(dt);

    // Decrementing particle life
    life--;
}

void Particle::Draw()
{
    // Calculations to determine the current center of particle texture
    SDL_Rect tmpRect = { (int)pState.pLive.startSize, (int)pState.pLive.startSize };
    float centerX = pState.pLive.pos.x + ((tmpRect.w - pState.pLive.rectSize.w) / 2.0f);
    float centerY = pState.pLive.pos.y + ((tmpRect.h - pState.pLive.rectSize.h) / 2.0f);

    // Color interpolation, only if the particle has enough life
    SDL_Color resColor;

    /* TODO 4.1 - Interpolate between start and end color
        - Create a method that takes to colors and a time step and returns the interpolated SDL_Color.
        - You can use pState.pLive.t as the time step.
        - For color use the pState.pLive.startColor and pState.pLive.endColor.
    */

    // Blitting particle on screen
    App->render->BlitParticle(App->render->GetParticleAtlas(), (int)centerX, (int)centerY, &pState.pLive.pRect, &pState.pLive.rectSize, 1.0f, pState.pLive.currentRotSpeed);

    // Calculating new rotation according to rotation speed
    pState.pLive.currentRotSpeed += pState.pLive.startRotSpeed;

    // Time step increment to interpolate colors
    pState.pLive.t += (1.0f / (float)pState.pLive.startLife);

    if (pState.pLive.t >= 1.0f)
        pState.pLive.t = 0.0f;
}

bool Particle::IsAlive()
{
    return life > 0;
}

Particle* Particle::GetNext()
{
    return pState.next;
}

void Particle::SetNext(Particle* next)
{
    pState.next = next;
}

float Particle::InterpolateBetweenRange(float min, float timeStep, float max)
{
    return min + (max - min) * timeStep;
}

void Particle::AddVortex(fPoint pos, float speed, float scale)
{
    vortex.pos = pos;
    vortex.speed = speed;
    vortex.scale = scale;
}

void Particle::CalculateParticlePos(float dt)
{
    float dx = pState.pLive.pos.x - vortex.pos.x;
    float dy = pState.pLive.pos.y - vortex.pos.y;
    float vx = -dy * vortex.speed;
    float vy = dx * vortex.speed;
    float factor = 1.0f / (1.0f + (dx * dx + dy * dy) / vortex.scale);

    pState.pLive.pos.x += (vx - pState.pLive.currentVel.x) * factor + pState.pLive.currentVel.x * dt;
    pState.pLive.pos.y += (vy - pState.pLive.currentVel.y) * factor + pState.pLive.currentVel.y * dt;
}

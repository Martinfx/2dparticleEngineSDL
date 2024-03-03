#include "particlesystem.hpp"
#include "engine.hpp"

ParticleSystem::ParticleSystem() : Module()
{
    name = "psystem";

   // LOG("Loading Languages");

}

ParticleSystem::~ParticleSystem()
{

}

bool ParticleSystem::Awake(pugi::xml_node& config)
{
    bool ret = true;

    pugi::xml_document	psystem_config;
    pugi::xml_node node = LoadEmitters(psystem_config);
    nameParticleAtlas = node.child("particleAtlas").attribute("name").as_string();

    for (pugi::xml_node emitters = node.child("particleAtlas").child("emitter");
         emitters && ret; emitters = emitters.next_sibling("emitter"))
    {
        /* TODO 1: Load emitter data into the emitter data vector:
            - We just want to load the emitter data from the fire emitter for now.
            - The for loop already parses the xml file for you. Just search for the emitter type “fire”.
            - Once you find it use LoadEmitterData() to fill the vector. Use EMITTER_TYPE_FIRE for the enum.
            */
        std::string emitterType = emitters.attribute("type").as_string();
        if (emitterType == "fire") {
            LoadEmitterData(emitters, EMITTER_TYPE_FIRE);
            // TODO 2: Vytvoření a přidání emitteru do seznamu emiterů
            // Použijte AddEmitter() metodu a předejte potřebné parametry, jako je pozice a typ emiteru
            AddEmiter(fPoint(20.0f, 20.0f), EMITTER_TYPE_FIRE);
        }
    }
    return ret;
}

bool ParticleSystem::Start()
{
    return true;
}

bool ParticleSystem::PreUpdate()
{
    return true;
}

bool ParticleSystem::Update(float dt)
{
    std::list<Emitter*>::const_iterator it;

    for (it = emittersList.begin(); it != emittersList.end(); ++it)
    {
        if ((*it) != nullptr)
            (*it)->Update(dt);
    }

    return true;
}

bool ParticleSystem::PostUpdate()
{
    std::list<Emitter*>::const_iterator it;

    for (it = emittersList.begin(); it != emittersList.end(); ++it)
    {
        if ((*it)->toDestroy)
        {
            delete (*it);
            emittersList.erase(it);
        }
    }

    return true;
}

bool ParticleSystem::CleanUp()
{
    //LOG("Freeing emitters from the system.");

    std::list<Emitter*>::const_iterator it;

    for (it = emittersList.begin(); it != emittersList.end(); ++it)
    {
        if ((*it) != nullptr)
            delete (*it);
    }

    emittersList.clear();

    return true;
}

Emitter* ParticleSystem::AddEmiter(fPoint pos, EmitterType type)
{
    Emitter* tmp_emitter = new Emitter(pos, vecEmitterData[type]);

    emittersList.push_back(tmp_emitter);

    return tmp_emitter;
}

bool ParticleSystem::RemoveEmitter(Emitter & emitter)
{
    std::list<Emitter*>::const_iterator it;

    for (it = emittersList.begin(); it != emittersList.end(); ++it)
    {
        if ((*it) == &emitter)
        {
            (*it)->toDestroy = true;
            return true;
        }
    }

    return false;
}

bool ParticleSystem::RemoveAllEmitters()
{
    bool ret = false;

    std::list<Emitter*>::const_iterator it;

    for (it = emittersList.begin(); it != emittersList.end(); ++it)
    {
        if ((*it) != nullptr) (*it)->toDestroy = true;
        ret = true;
    }

    return ret;
}

void ParticleSystem::LoadEmitterData(pugi::xml_node & emitter, EmitterType type)
{
    EmitterData tmp;

    // Angle range
    tmp.angleRange.x = emitter.child("angleRange").attribute("min").as_float();
    tmp.angleRange.y = emitter.child("angleRange").attribute("max").as_float();

    // Particle speeds
    tmp.rotSpeed = emitter.child("rotSpeed").attribute("value").as_double();
    tmp.startSpeed = emitter.child("startSpeed").attribute("value").as_float();
    tmp.endSpeed = emitter.child("endSpeed").attribute("value").as_float();

    // Random controls
    tmp.startSpeedRand.x = emitter.child("startSpeed").attribute("randMin").as_float();
    tmp.startSpeedRand.y = emitter.child("startSpeed").attribute("randMax").as_float();

    tmp.endSpeedRand.x = emitter.child("endSpeed").attribute("randMin").as_float();
    tmp.endSpeedRand.y = emitter.child("endSpeed").attribute("randMax").as_float();

    tmp.rotSpeedRand.x = emitter.child("rotSpeed").attribute("randMin").as_float();
    tmp.rotSpeedRand.y = emitter.child("rotSpeed").attribute("randMax").as_float();

    tmp.emitVarianceRand.x = emitter.child("emitVariance").attribute("randMin").as_float();
    tmp.emitVarianceRand.y = emitter.child("emitVariance").attribute("randMax").as_float();

    tmp.lifeRand.x = emitter.child("maxParticleLife").attribute("randMin").as_float();
    tmp.lifeRand.y = emitter.child("maxParticleLife").attribute("randMax").as_float();

    tmp.startSizeRand.x = emitter.child("startSize").attribute("randMin").as_float();
    tmp.startSizeRand.y = emitter.child("startSize").attribute("randMax").as_float();

    tmp.endSizeRand.x = emitter.child("endSizeRand").attribute("randMin").as_float();
    tmp.endSizeRand.y = emitter.child("endSizeRand").attribute("randMax").as_float();

    // Particle size
    tmp.startSize = emitter.child("startSize").attribute("value").as_float();
    tmp.endSize = emitter.child("endSize").attribute("value").as_float();

    // Emission properties
    tmp.emitNumber = emitter.child("emitNumber").attribute("value").as_uint();
    tmp.emitVariance = emitter.child("emitVariance").attribute("value").as_uint();

    // Particle life
    tmp.maxParticleLife = emitter.child("maxParticleLife").attribute("value").as_uint();

    // Rect from particle atlas
    tmp.textureRect.x = emitter.child("textureRect").attribute("x").as_int();
    tmp.textureRect.y = emitter.child("textureRect").attribute("y").as_int();
    tmp.textureRect.w = emitter.child("textureRect").attribute("w").as_int();
    tmp.textureRect.h = emitter.child("textureRect").attribute("h").as_int();

    // Lifetime of emitter
    tmp.lifetime = emitter.child("lifetime").attribute("value").as_double();

    // Start color
    tmp.startColor.r = emitter.child("startColor").attribute("r").as_uint();
    tmp.startColor.g = emitter.child("startColor").attribute("g").as_uint();
    tmp.startColor.b = emitter.child("startColor").attribute("b").as_uint();
    tmp.startColor.a = emitter.child("startColor").attribute("a").as_uint();

    // End color
    tmp.endColor.r = emitter.child("endColor").attribute("r").as_uint();
    tmp.endColor.g = emitter.child("endColor").attribute("g").as_uint();
    tmp.endColor.b = emitter.child("endColor").attribute("b").as_uint();
    tmp.endColor.a = emitter.child("endColor").attribute("a").as_uint();

    // Blend mode
    std::string blendModeString = emitter.child("blendMode").attribute("mode").as_string();

    if (blendModeString == "add")
        tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_ADD;
    else if (blendModeString == "blend")
        tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_BLEND;
    else if (blendModeString == "mod")
        tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_MOD;
    else if (blendModeString == "none")
        tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;

    vecEmitterData[type] = tmp;
}

pugi::xml_node ParticleSystem::LoadEmitters(pugi::xml_document& psystem_file) const
{
    pugi::xml_node ret;

    pugi::xml_parse_result result = psystem_file.load_file("psystem_config.xml");

    /*if (result == NULL)
        LOG("Could not load xml file config.xml. pugi error: %s", result.description());
    else*/
    ret = psystem_file.child("emitters");
    return ret;
}

/*
SDL_Texture* ParticleSystem::GetParticleAtlas() const
{
    return particleAtlas;
}*/

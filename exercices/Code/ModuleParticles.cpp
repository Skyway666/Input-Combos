#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL_timer.h"
#include "ModuleMap1.h"

ModuleParticles::ModuleParticles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;
}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Init()
{
	graphics = App->textures->Load("Assets/hadowken.png");
	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{

	return true;
}

// Update: draw background
update_status ModuleParticles::Update()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if (p == nullptr)
			continue;

		if (p->Update() == false)
		{
			delete p;
			active[i] = nullptr;
			continue;
		}
		else 
		{
				App->render->Blit(graphics, p->position.x, p->position.y,nullptr, 170, 170);
		}
	}

	
	return UPDATE_CONTINUE;
}



void ModuleParticles::AddParticle(const Particle& particle)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			active[i] = p;
			break;
		}
	}
}

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

bool Particle::Update()
{

	bool ret = true;

	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
		{
			ret = false;
		}
	}

	position += speed;

	return ret;
}


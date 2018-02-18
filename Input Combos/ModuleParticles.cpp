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
	//graphics = App->textures->Load("Assets/Hadowken.png");
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
				App->render->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));
		}
	}

	
	return UPDATE_CONTINUE;
}



void ModuleParticles::AddParticle(const Particle& particle, int x, int y,int speed_x, int speed_y)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->position.x = x;
			p->position.y = y;
			if (speed_x != 0)
			{
				p->speed.x = speed_x;
			}
			if (speed_y != 0)
			{
				p->speed.y = speed_y;
			}
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
	else
		if (anim.Finished())
			ret = false;

	return ret;
}


#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "p2Point.h"
#include <time.h>
#include <stdlib.h>

#define MAX_ACTIVE_PARTICLES 100
struct SDL_Texture;
struct Collider;
enum COLLIDER_TYPE;

struct Particle
{
	uint fx = 0;
	iPoint position;
	iPoint speed;
	Uint32 life = 0;
	Uint32 born = 0;

	Particle();
	Particle(iPoint _pos, iPoint _speed, int _life) :position(_pos), speed(_speed), life(_life)
	{
		born = SDL_GetTicks();
	}

	bool Update();
};

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Init();
	update_status Update();
	bool CleanUp();
	void AddParticle(const Particle& particle);
private:

	Particle* active[MAX_ACTIVE_PARTICLES];
	SDL_Texture* graphics;
};

#endif // __MODULEPARTICLES_H__
#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePlayer.h"
#include <time.h>
#include <stdlib.h>

#define MAX_ACTIVE_PARTICLES 10
struct SDL_Texture;
struct Collider;
enum COLLIDER_TYPE;

struct Particle
{
	Animation anim;
	uint fx = 0;
	fPoint position;
	iPoint speed;
	Uint32 life = 0;
	Uint32 born = 0;

	Particle();
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
	void AddParticle(const Particle& particle, int x, int y, int speed_x = 0, int speed_y = 0);
private:

	Particle* active[MAX_ACTIVE_PARTICLES];
	SDL_Texture* graphics;
};

#endif // __MODULEPARTICLES_H__
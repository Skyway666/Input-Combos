#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer();
	~ModulePlayer();
	bool Init();
	update_status Update();
	bool CleanUp();


public:

	SDL_Texture* graphics = nullptr;
	iPoint pos;
};

#endif
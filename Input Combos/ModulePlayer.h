#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;

#define MAX_INPUT_BUFFER 20
enum inputs
{
	NONE,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	PUNCH
};


class ModulePlayer : public Module
{
public:
	ModulePlayer();
	~ModulePlayer();
	bool Init();
	update_status Update();
	bool CleanUp();



private:

	SDL_Texture* graphics = nullptr;
	iPoint pos;
	Animation* current_animation;
	bool crouching = false;
	bool punching = false; 
	bool jumping = false;
	bool walking_back = false;
	bool walking_forward = false;
	bool hadowken = false;
	bool tatsumaki = false;

	Animation Idle;
	Animation Crouch;
	Animation Standing_punch;
	Animation Crouching_punch;
	Animation Walk_forward;
	Animation Walk_back;
	Animation Jump;
	Animation Hadowken;
	Animation Tatsumaki;

	inputs input_buffer[MAX_INPUT_BUFFER];

	bool Check_for_hadowken();
	bool Check_for_tatsumaki();

	void Push_into_buffer(inputs);
};

#endif
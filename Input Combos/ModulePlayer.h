#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;

#define MAX_INPUT_BUFFER 20
enum input
{
	NONE,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	PUNCH,
	HADOWKEN,
	TATSUMAKI,
};

enum character_state
{
	IDLE,
	WALKING_FORWARD,
	WALKING_BACK,
	CROUCHING,
	JUMPING,
	STANDING_PUNCHING,
	CROUCHING_PUNCHING,
	HADOWKENING,
	TATSUMAKING
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
	character_state current_state;
	input current_input;

	Animation Idle;
	Animation Crouch;
	Animation Standing_punch;
	Animation Crouching_punch;
	Animation Walk_forward;
	Animation Walk_back;
	Animation Jump;
	Animation Hadowken;
	Animation Tatsumaki;

	input input_buffer[MAX_INPUT_BUFFER];

	bool Check_for_hadowken();
	bool Check_for_tatsumaki();

	void Push_into_buffer(input);
};

#endif
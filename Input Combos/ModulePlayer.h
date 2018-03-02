#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"


#include "PugiXml/src/pugixml.hpp"

struct SDL_Texture;

#define MAX_INPUT_BUFFER 15
enum input
{
	NONE,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	PUNCH,
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
	HADOWKEN,
	TATSUMAKI,
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
	character_state wanted_state;

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

	bool Check_for_hadowken();			//Checks the input buffer looking for the hadowken button combination
	bool Check_for_tatsumaki();			//Checks the input buffer looking for the tatsumaki button combination
	bool Cancelable_current_state();	//Checks if the character's current state can be cancelled in other states (It should be some sort of mechanism that allows to compare wanted action with current action)

	void Push_into_buffer(input);		//Pushes an input into the buffer, and shifts all the others

	//Functions for clarity
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;
	void SetAnimations();
	void SetConfigData();
};

#endif
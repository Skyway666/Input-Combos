#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "ModuleParticles.h"
#include <list>
#include <string>
#include "PugiXml/src/pugixml.hpp"

struct SDL_Texture;

#define MAX_INPUT_BUFFER 100

enum input
{
	NONE,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	PUNCH,
	KICK
};

enum character_state_enum
{
	IDLE,
	WALKING_FORWARD,
	WALKING_BACK,
	CROUCHING,
	JUMPING,
	STANDING_PUNCHING,
	CROUCHING_PUNCHING,
	STANDING_KICKING,
	CROUCHING_KICKING,
	HADOWKEN,
	SUPER_HADOWKEN,
	TATSUMAKI
};

struct directions
{
	bool left, down, right = false;
};

struct character_state
{
	character_state_enum state;
	std::list<character_state_enum> cancelability;

	character_state(character_state_enum _state)
	{
		state = _state;
	}
	bool IsCancelableInto(character_state_enum wanted_state)
	{
		for (std::list<character_state_enum>::iterator it = cancelability.begin(); it != cancelability.end(); it++)
		{
			if (*it == wanted_state)
			return true;
		}
		return false;
	}
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
	//Essential
	SDL_Texture* graphics = nullptr;
	iPoint pos;
	Animation* current_animation;
	double speed;
	void requestState();
	void updateState();
	void update();
	void debugDraw();
	void draw();

	//State machine
	character_state_enum current_state;
	character_state_enum wanted_state;
	directions direction_inputs;

	//Animations
	Animation Idle;
	Animation Crouch;
	Animation Standing_punch;
	Animation Crouching_punch;
	Animation Standing_kick;
	Animation Crouching_kick;
	Animation Walk_forward;
	Animation Walk_back;
	Animation Jump;
	Animation Hadowken;
	Animation Tatsumaki;

	//States
	//Attacks 
	character_state S_Standing_punch = character_state(STANDING_PUNCHING); 
	character_state S_Crouching_punch = character_state(CROUCHING_PUNCHING);
	character_state S_Standing_kick = character_state(STANDING_KICKING);
	character_state S_Crouching_kick = character_state(CROUCHING_KICKING);
	character_state S_Hadowken = character_state(HADOWKEN);
	character_state S_Tatsumaki = character_state(TATSUMAKI);
	//Movement
	character_state S_Walk_forward = character_state(WALKING_FORWARD);
	character_state S_Walk_back = character_state(WALKING_BACK);
	character_state S_Crouch = character_state(CROUCHING);
	character_state S_Idle = character_state(IDLE);

	//Particles
	Particle hadowken;

	//Input buffer
	input input_buffer[MAX_INPUT_BUFFER];

	//Directional inputs 
	std::list<input> hadowken_inputs;
	std::list<input> tatsumaki_inputs;
	std::list<input> super_hadowken_directions;
	std::list<input> super_hadowken_simultaneous_attacks;
	int detection_delay;


	//Related to special moves
	bool Check_for_hadowken();			//Checks the input buffer looking for the hadowken button combination
	bool Check_for_tatsumaki();			//Checks the input buffer looking for the tatsumaki button combination
	bool Check_for_super_hadowken();	//Checks the input buffer looking for the super hadowken button combination
	bool Current_state_is_movement();	//Checks if the character's current state can be cancelled in other states (It should be some sort of mechanism that allows to compare wanted action with current action)

	//Related to the buffer
	void Push_into_buffer(input);		//Pushes an input into the buffer, and shifts all the others
	input Catch_first_attack_input_within(int window,int delay = 0);


	//Related to canceling
	bool Can_cancel_current_state_into(character_state_enum);
	int hadowken_cancelability_window;
	int tatsumaki_cancelability_window;
	int super_hadowken_cancelability_window;
	int normal_moves_cancelability_window;


	//Functions for clarity
	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;
	void SetAnimations();
	void SetConfigData();
	void FillInputListFromXMLIterator(std::list<input>&, pugi::xml_node&); 
	void FillStateListFromXMLIterator(std::list<character_state_enum>&, pugi::xml_node&);
	bool Is_attack_input(input);
	void Update_animation_depending_on_current_state();
};

#endif
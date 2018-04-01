#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"
#include "SDL\include\SDL_scancode.h"
#include "SDL\include\SDL_gamecontroller.h"
#include "SDL\include\SDL_joystick.h"

#define MAX_KEYS 300
#define MAX_BUTTONS 15
#define MAX_POSITIVE_JOYSTICK_POSITIONS 32767
#define MAX_NEGATIVE_JOYSTICK_POSITIONS -32768
#define MAX_JOYSTICK_POSITIONS 65535

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:

	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	const int JOYSTICK_DEAD_ZONE = 8000;
	KEY_STATE keyboard[MAX_KEYS];
	KEY_STATE gamepad[MAX_BUTTONS];
	KEY_STATE joystickpos[5];
	KEY_STATE gamepad2[MAX_BUTTONS];
	KEY_STATE joystickpos2[5];
	SDL_GameController* controller = NULL;
	SDL_Joystick* joystick = NULL;
	SDL_GameController* controller2 = NULL;
	SDL_Joystick* joystick2 = NULL;
	bool gpad_on = false;
	bool gpad = false;
	bool gpad2_on = false;
	bool gpad2 = false;
	bool p1moving = false;
	bool p2moving = false;
};

#endif // __ModuleInput_H__
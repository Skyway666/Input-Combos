#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

#define NUM_MODULES 7

class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleRender;
class Module;
class ModuleMap1;
class ModuleAudio1;
class ModuleWelcomeScreen;
class ModulePlayer;
class ModuleParticles;

class Application
{
public:

	Module* modules[NUM_MODULES];
	ModuleWindow* window;
	ModuleRender* render;
	ModuleInput* input;
	ModuleTextures* textures;
	ModuleAudio1 * audio;
	ModuleMap1 * map_1;

	ModuleWelcomeScreen* WelcomeScreen;
	ModulePlayer* player;
	ModuleParticles* particles;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__
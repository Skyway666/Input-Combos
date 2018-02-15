#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "Globals.h"

#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )
typedef unsigned char Uint8;

class ModuleAudio1 : public Module
{
public:

	ModuleAudio1();
	~ModuleAudio1();

	bool Init();
	Mix_Music* const Load(const char* path);
	update_status Update();
	bool CleanUp();
	bool Close();

public:
	Mix_Music* music;
	bool playing = false;
};

#endif // __ModuleAudio_H__
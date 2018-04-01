#ifndef __MODULEMAP1__
#define __MODULEMAP1__
#include "Globals.h"
#include "Module.h"
#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )


class ModuleMap1 : public Module
{
public:
	ModuleMap1();
	~ModuleMap1();

	bool Init();
	update_status Update();
	bool CleanUp();


};

#endif //__MODULEMAP1__
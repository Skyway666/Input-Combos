#include "Application.h"
#include "ModuleMap1.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio_1.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"


#include <stdio.h>

ModuleMap1::ModuleMap1() : Module()
{
	
}
ModuleMap1::~ModuleMap1()
{
   
}

bool ModuleMap1::Init()
{

	
	return true;
}

update_status ModuleMap1::Update()
{


	return UPDATE_CONTINUE;
}

bool ModuleMap1::CleanUp()
{
	return true;
}
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL.h"
#include "ModuleParticles.h"
#include "ModuleMap1.h"
#include <stdio.h>

ModulePlayer::ModulePlayer()
{

}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Init()
{
	LOG("Loading player");

	graphics = App->textures->Load("Assets/ryu.png");

	// Fill the input buffer with invalid inputs
	for (int i = 0; i < MAX_INPUT_BUFFER; i++)
	{
		input_buffer[i] = NONE;
	}

	pos.x = 425;
		
	pos.y = 400;
	

	Idle.PushBack({ 0,0,80,99 });
	Idle.PushBack({ 80,0,80,99 });
	Idle.PushBack({ 160,0,80,99 });
	Idle.PushBack({ 240,0,80,99 });

	Idle.loop = true;
	Idle.speed = 0.2;
	
	return true;
}


// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{


	// Check what inputs are being pressed in this frame, and push them into the input buffer (TODO 1)
	if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
	{
		Push_into_buffer(DOWN);
	}
	if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT)
	{
		Push_into_buffer(LEFT);
	}
	if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT)
	{
		Push_into_buffer(RIGHT);
	}
	if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_DOWN)
	{
		Push_into_buffer(UP);
	}
	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		Push_into_buffer(PUNCH);
	}

	//Check the input buffer to see if the player has preformed a combination (each combination should be loaded from an XML
	hadowken = Check_for_hadowken();
	tatsumaki = Check_for_tatsumaki();


	// If no special move has been performed, be sure to assign the correct action depending on the last input recieved
	if (!hadowken && !tatsumaki)
	{
		switch (input_buffer[MAX_INPUT_BUFFER - 1])
		{
			case DOWN:
			{
				crouching = true;
			}
			case LEFT:
			{
				walking_back = true;
			}
			case RIGHT:
			{
				walking_forward = true;
			}
			case UP:
			{
				jumping = true;
			}
			case PUNCH:
			{
				punching = true;
			}
		}
	}

	//Make ryu perform the right animation depending on the previous events

	current_animation = &Idle;

	/*if (walking_back && !jumping)
	{
		current_animation = &Walk_back;
		walking_back = false;
	}

	if (walking_forward && !jumping)
	{
		current_animation = &Walk_forward;
		walking_forward = false;
	}
	
	if (crouching && !jumping)
	{
		current_animation = &Crouch;
		crouching = false;
	}

	if (punching && !crouching && !jumping)
	{
		current_animation = &Standing_punch;

		if (current_animation->Finished())
		{
			punching = false;
		}

	}

	if (punching && crouching && !jumping)
	{
		current_animation = &Crouching_punch;
		if (current_animation->Finished())
		{
			punching = false;
		}
	}

	if (hadowken)
	{
		current_animation = &Hadowken;
		if (current_animation->Finished())
		{
			hadowken = false;
		}
	}
	 
	if (tatsumaki && !hadowken)
	{
		current_animation = &Tatsumaki;
		if (current_animation->Finished())
		{
			tatsumaki = false;
		}
	}*/


	App->render->Blit(graphics, pos.x, pos.y, &current_animation->GetCurrentFrame(), 80*4, 99*4);

	if(App->input->keyboard[SDL_SCANCODE_S])
	{ 
		pos.x++;
		pos.y++;
	}

	return UPDATE_CONTINUE;
}

bool ModulePlayer::Check_for_hadowken()
{
	int counter = 0;
	for (int i = 0; i < MAX_INPUT_BUFFER; i++)
	{
		input_buffer[i] = NONE;

		if (input_buffer[i] == DOWN && counter == 0)
		{
			counter++;
		}
		if (input_buffer[i] == RIGHT && counter == 1)
		{
			counter++;
		}
		if (input_buffer[i] == PUNCH && counter == 2)
		{
			counter++;
		}
	}

	if (counter == 3)
		return true;
	else
		return false;
}

bool ModulePlayer::Check_for_tatsumaki()
{
	return false;
}

void ModulePlayer::Push_into_buffer(inputs input)
{
	input_buffer[0] = NONE;

	for (int i = 0; i < 9; i++)
	{
		input_buffer[i] = input_buffer[i+1];
	}

	input_buffer[MAX_INPUT_BUFFER-1] = input;
}
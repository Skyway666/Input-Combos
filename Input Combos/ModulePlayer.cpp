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

	//Set ryu's initial position
	pos.x = 425;
	pos.y = 400;
	

	Idle.PushBack({ 0,0,80,99 });
	Idle.PushBack({ 80,0,80,99 });
	Idle.PushBack({ 160,0,80,99 });
	Idle.PushBack({ 240,0,80,99 });	
	
	Idle.loop = true;
	Idle.speed = 0.2;

	Walk_forward.PushBack({ 320,0,80,99 });
	Walk_forward.PushBack({ 400,0,80,99 });
	Walk_forward.PushBack({ 480,0,80,99 });
	Walk_forward.PushBack({ 560,0,80,99 });
	Walk_forward.PushBack({ 640,0,80,99 });
	
	Walk_forward.loop = true;
	Walk_forward.speed = 0.2;

	Walk_back.PushBack({ 640,0,80,99 });
	Walk_back.PushBack({ 560,0,80,99 });
	Walk_back.PushBack({ 480,0,80,99 });
	Walk_back.PushBack({ 400,0,80,99 });
	Walk_back.PushBack({ 320,0,80,99 });

	Walk_back.loop = true;
	Walk_back.speed = 0.2;
	
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
	bool button_pressed = false;
	if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
	{
		Push_into_buffer(DOWN);
		button_pressed = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT)
	{
		Push_into_buffer(LEFT);
		button_pressed = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT)
	{
		Push_into_buffer(RIGHT);
		button_pressed = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_DOWN)
	{
		Push_into_buffer(UP);
		button_pressed = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
 		Push_into_buffer(PUNCH);
		button_pressed = true;
	}
	//We need to push the buffer in all the frames in order to keep only the last 20 inputs, therefore if no button has been pressed, we push a "NONE" input into the buffer
	if(!button_pressed)
	{
		Push_into_buffer(NONE); 
	}

	//Check if a special move has been performed, as they have priority over all other moves. If it has, assign the wanted action to be executed in this frame (TODO 2)
	if (Check_for_hadowken())
	{
		wanted_state = HADOWKEN;
	}
	else if (Check_for_tatsumaki())
	{
		wanted_state = TATSUMAKI;
	}

	//If no special move has been performed, assign the wanted action depending on the last input that has been pressed (last one in the input buffer) (TODO 3)
	else
	{
		input current_input = input_buffer[MAX_INPUT_BUFFER - 1];

		switch (current_input)
		{
			case NONE:
			{
				wanted_state = IDLE;
				break;
			}
			case RIGHT:
			{
				wanted_state = WALKING_FORWARD;
				break;
			}
			case LEFT:
			{
				wanted_state = WALKING_BACK;
				break;
			}
			case DOWN:
			{
				wanted_state = CROUCHING;
				break;
			}
			case UP:
			{
				wanted_state = JUMPING;
				break;
			}
			case PUNCH:
			{
				if (current_state == CROUCHING)
					wanted_state = STANDING_PUNCHING;
				else
					wanted_state = CROUCHING_PUNCHING;
				break;
			}
		}
	}

	//Check if the current animation is cancelable with Cancelable_current_state(). If it is cancelable, assign the wanted_state as current_state and set the current animation. (TODO 4)
	
	if (Cancelable_current_state())
	{
		current_state = wanted_state;

		switch (current_state)
		{
			case IDLE:
			{
				current_animation = &Idle;
				break;
			}
			case WALKING_FORWARD: 
			{
				current_animation = &Walk_forward;
				pos.x+=5; //Don't forget to put limits to the movement
				break;
			}
			case WALKING_BACK:
			{
				current_animation = &Walk_back;
				pos.x-=5;
				break;
			}
			case CROUCHING:
			{
				current_animation = &Crouch;
				break;
			}
			case JUMPING:
			{
				current_animation = &Jump;
				break;
			}
			case STANDING_PUNCHING:
			{
				current_animation = &Standing_punch;
				break;
			}
			case CROUCHING_PUNCHING:
			{
				current_animation = &Crouching_punch;
				break;
			}
			case HADOWKEN:
			{
				current_animation = &Hadowken;
				break;
			}
			case TATSUMAKI:
			{
				current_animation = &Tatsumaki;
				break;
			}
			
		}
	}


	App->render->Blit(graphics, pos.x, pos.y, &current_animation->GetCurrentFrame(), 80*4, 99*4);


	return UPDATE_CONTINUE;
}

bool ModulePlayer::Check_for_hadowken()
{
	int counter = 0;
	for (int i = 0; (i < MAX_INPUT_BUFFER); i++)
	{

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

void ModulePlayer::Push_into_buffer(input input)
{
	input_buffer[0] = NONE;

	for (int i = 0; i < MAX_INPUT_BUFFER; i++)
	{
		input_buffer[i] = input_buffer[i+1];
	}

	input_buffer[MAX_INPUT_BUFFER-1] = input;
}

bool ModulePlayer::Cancelable_current_state()
{
	if (current_state == WALKING_FORWARD || current_state == WALKING_BACK || current_state == IDLE || current_state == CROUCHING)
		return true;
	else
		return false;
}
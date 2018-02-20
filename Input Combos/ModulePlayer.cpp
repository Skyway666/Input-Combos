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
	pos.y = 300;
	

	Idle.PushBack({ 0,0,130,123 });
	Idle.PushBack({ 130,0,130,123 });
	Idle.PushBack({ 130*2,0,130,123 });
	Idle.PushBack({ 130*3,0,130,123  });
	
	Idle.loop = true;
	Idle.speed = 0.2;

	Walk_forward.PushBack({ 130 * 4,0,130,123 });
	Walk_forward.PushBack({ 130 * 5,0,130,123 });
	Walk_forward.PushBack({ 130 * 6,0,130,123 });
	Walk_forward.PushBack({ 130 * 7,0,130,123 });
	Walk_forward.PushBack({ 130 * 8,0,130,123 });
	
	Walk_forward.loop = true;
	Walk_forward.speed = 0.2;

	Walk_back.PushBack({ 130 * 8,0,130,123 });
	Walk_back.PushBack({ 130 * 7,0,130,123 });
	Walk_back.PushBack({ 130 * 6,0,130,123 });
	Walk_back.PushBack({ 130 * 5,0,130,123 });
	Walk_back.PushBack({ 130 * 4,0,130,123 });

	Walk_back.loop = true;
	Walk_back.speed = 0.2;
	
	Crouch.PushBack({ 130 * 3,123,130,123 });
	Crouch.PushBack({ 130 * 4,123,130,123 });

	Crouch.loop = false;
	Crouch.speed = 0.2;

	Standing_punch.PushBack({ 130 * 5,123,130,123 });
	Standing_punch.PushBack({ 130 * 6,123,130,123 });
	Standing_punch.PushBack({ 130 * 7,123,130,123 });
	Standing_punch.PushBack({ 130 * 8,123,130,123 });
	Standing_punch.PushBack({ 130 * 9,123,130,123 });

	Standing_punch.loop = false;
	Standing_punch.speed = 0.2;

	Hadowken.PushBack({ 130 * 10,123  ,130,123 });
	Hadowken.PushBack({ 130 * 11,123  ,130,123 });
	Hadowken.PushBack({ 0		,123 * 2,130,123 });
	Hadowken.PushBack({ 130     ,123 * 2,130,123 });
	Hadowken.PushBack({ 130 * 2 ,123 * 2,130,123 });

	Hadowken.loop = false;
	Hadowken.speed = 0.2;

	current_animation = &Idle;
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
	if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
	{
		Push_into_buffer(DOWN);
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
	//We need to move the buffer in all the frames in order to keep only the last 20 inputs, therefore if no button has been pressed, we push a "NONE" input into the buffer
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
					wanted_state = CROUCHING_PUNCHING;
				else
					wanted_state = STANDING_PUNCHING;
				break;
			}
		}
	}

	// Check if the wanted state is different from the current state. If it is not, we do nothing. If it is, we check if the current state can be canceled with
	// Cancelable_current_state(), and if it is cancelable, we set the current state to the wanted one. If it is not cancelable, we need to make sure that when the current animation is finished, the current state goes back to IDLE,
	// and the animation which was being done is reset. After that, we update the animation depending on the current state. 
	// (TODO 4)
	if (wanted_state != current_state) {
		if (Cancelable_current_state())
		{
			current_state = wanted_state;
		}
		else
		{
			if (current_animation->Finished())
			{
				current_state = IDLE;
				Standing_punch.Reset();
				Hadowken.Reset();
			}
		}

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
				break;
			}
			case WALKING_BACK:
			{
				current_animation = &Walk_back;
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

	// Movement
	switch (current_state)
	{
		case WALKING_FORWARD:
		{
			pos.x += 5;
			break;
		}
		case WALKING_BACK:
		{
			pos.x -= 5;
			break;
		}
	}

	App->render->Blit(graphics, pos.x, pos.y, &current_animation->GetCurrentFrame(), 130*4, 123*4);



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
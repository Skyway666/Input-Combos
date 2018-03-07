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
	//Ser animations
	SetAnimations();
	//Set ryu's initial position
	pos.x = 425;
	pos.y = 300;
	//Set config data
	SetConfigData();
	
	
	
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

	if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
	{
		Push_into_buffer(DOWN);
		button_pressed = true;
	}
	else if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT)
	{
		Push_into_buffer(LEFT);
		button_pressed = true;
	}
	else if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT)
	{
		Push_into_buffer(RIGHT);
		button_pressed = true;
	}
	else if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_DOWN)
	{
		Push_into_buffer(UP);
		button_pressed = true;
	}


	if (App->input->keyboard[SDL_SCANCODE_A] == KEY_DOWN)
	{
 		Push_into_buffer(PUNCH);
		button_pressed = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_S] == KEY_DOWN)
	{
		Push_into_buffer(KICK);
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

	//If no special move has been performed, assign the wanted action depending on the last input that has been pressed (last one in the input buffer). Mind that 
	//depending on the current state, you may want different actions(TODO 3)
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
			case KICK:
			{
				if (current_state == CROUCHING)
					wanted_state = CROUCHING_KICKING;
				else
					wanted_state = STANDING_KICKING;
				break;
			}
		}
	}

	// Check if the wanted state is different from the current state. If it is, we check if the current state can be canceled with
	// Cancelable_current_state(), and if it is cancelable, we set the current state to the wanted one. If it is not cancelable, we need to make sure that when the current 
	// animation is finished, the current state goes back to the wanted state in this frame, and the animation which was being done is reset. After that, we update the 
	// animation depending on the current state. 
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
				current_state = wanted_state;
				Standing_punch.Reset();
				Hadowken.Reset();
				Crouching_punch.Reset();
				Tatsumaki.Reset();
				Standing_kick.Reset();
				Crouching_kick.Reset();
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
			case STANDING_KICKING:
			{
				current_animation = &Standing_kick;
				break;
			}
			case CROUCHING_KICKING:
			{
				current_animation = &Crouching_kick;
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
		case TATSUMAKI:
		{
			pos.x += 7;
			break;
		}
	}

	App->render->Blit(graphics, pos.x, pos.y, &current_animation->GetCurrentFrame(), 130*4, 123*4);



	return UPDATE_CONTINUE;
}

bool ModulePlayer::Check_for_hadowken()
{
	int counter = 0;
	auto input_iterator = hadowken_inputs.begin();

	for (int i = 0; (i < MAX_INPUT_BUFFER); i++)
	{

		if (input_buffer[i] == *input_iterator)
		{
			counter++;
			if(counter < hadowken_inputs.size()) //We don't want to access something out of the list
			input_iterator++;
		}
	}

	if (counter == hadowken_inputs.size())
		return true;
	else
		return false;
}

bool ModulePlayer::Check_for_tatsumaki()
{
	int counter = 0;
	auto input_iterator = tatsumaki_inputs.begin();

	for (int i = 0; (i < MAX_INPUT_BUFFER); i++)
	{

		if (input_buffer[i] == *input_iterator)
		{
			counter++;
			if (counter < tatsumaki_inputs.size()) //We don't want to access something out of the list
			input_iterator++;
		}
	}

	if (counter == tatsumaki_inputs.size())
		return true;
	else
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

void ModulePlayer::SetAnimations()
{
	Idle.PushBack({ 0,0,130,123 });
	Idle.PushBack({ 130,0,130,123 });
	Idle.PushBack({ 130 * 2,0,130,123 });
	Idle.PushBack({ 130 * 3,0,130,123 });

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


	Standing_kick.PushBack({ 130 * 5,123 * 3,130,123 });
	Standing_kick.PushBack({ 130 * 6,123  *3,130,123 });
	Standing_kick.PushBack({ 130 * 7,123 * 3,130,123 });
	Standing_kick.PushBack({ 130 * 8,123 * 3,130,123 });
	Standing_kick.PushBack({ 130 * 9,123 * 3,130,123 });

	Standing_kick.loop = false;
	Standing_kick.speed = 0.2;

	Crouching_kick.PushBack({ 130 * 10,123 * 3 ,130,123 });
	Crouching_kick.PushBack({ 130 * 11,123 * 3,130,123 });
	Crouching_kick.PushBack({ 0       ,123 * 4,130,123 });
	Crouching_kick.PushBack({ 130     ,123 * 4,130,123 });
	Crouching_kick.PushBack({ 130  * 2,123 * 4,130,123 });

	Crouching_kick.loop = false;
	Crouching_kick.speed = 0.2;

	Hadowken.PushBack({ 130 * 10,123  ,130,123 });
	Hadowken.PushBack({ 130 * 11,123  ,130,123 });
	Hadowken.PushBack({ 0		,123 * 2,130,123 });
	Hadowken.PushBack({ 130     ,123 * 2,130,123 });
	Hadowken.PushBack({ 130 * 2 ,123 * 2,130,123 });

	Hadowken.loop = false;
	Hadowken.speed = 0.2;


	Crouching_punch.PushBack({ 130 * 3,123 * 2,130,123 });
	Crouching_punch.PushBack({ 130 * 4,123 * 2,130,123 });
	Crouching_punch.PushBack({ 130 * 5,123 * 2,130,123 });
	Crouching_punch.PushBack({ 130 * 6,123 * 2,130,123 });
	Crouching_punch.PushBack({ 130 * 7,123 * 2,130,123 });

	Crouching_punch.loop = false;
	Crouching_punch.speed = 0.2;

	Tatsumaki.PushBack({ 130 * 8 ,123 * 2,130,123 });
	Tatsumaki.PushBack({ 130 * 9 ,123 * 2,130,123 });
	Tatsumaki.PushBack({ 130 * 10,123 * 2,130,123 });
	Tatsumaki.PushBack({ 130 * 11,123 * 2,130,123 });
	Tatsumaki.PushBack({ 0       ,123 * 3,130,123 });
	Tatsumaki.PushBack({ 130     ,123 * 3,130,123 });
	Tatsumaki.PushBack({ 130 * 9 ,123 * 2,130,123 });
	Tatsumaki.PushBack({ 130 * 10,123 * 2,130,123 });
	Tatsumaki.PushBack({ 130 * 11,123 * 2,130,123 });
	Tatsumaki.PushBack({ 0       ,123 * 3,130,123 });
	Tatsumaki.PushBack({ 130     ,123 * 3,130,123 });
	Tatsumaki.PushBack({ 130 * 2 ,123 * 3,130,123 });
	Tatsumaki.PushBack({ 130 * 3 ,123 * 3,130,123 });
	Tatsumaki.PushBack({ 130 * 4 ,123 * 3,130,123 });

	Tatsumaki.loop = false;
	Tatsumaki.speed = 0.2;
}

void ModulePlayer:: SetConfigData()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;

	config = LoadConfig(config_file);

	pugi::xml_node attack = config.child("directional_inputs").child("hadowken");

	pugi::xml_node iterator = attack.first_child();
	FillInputListFromXMLIterator(hadowken_inputs, iterator);

	attack = config.child("directional_inputs").child("tatsumaki");
	iterator = attack.first_child();
	FillInputListFromXMLIterator(tatsumaki_inputs, iterator);
	
	attack = config.child("cancel_values").child("standing_punch");
	iterator = attack.first_child();
	FillStateListFromXMLIterator(S_Standing_punch.cancelability, iterator);

	attack = config.child("cancel_values").child("crouching_punch");
	iterator = attack.first_child();
	FillStateListFromXMLIterator(S_Crouching_punch.cancelability, iterator);

	attack = config.child("cancel_values").child("standing_kick");
	iterator = attack.first_child();
	FillStateListFromXMLIterator(S_Standing_kick.cancelability, iterator);
	
	attack = config.child("cancel_values").child("crouching_kick");
	iterator = attack.first_child();
	FillStateListFromXMLIterator(S_Crouching_kick.cancelability, iterator);


}
pugi::xml_node ModulePlayer::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if (result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	}
	else
		ret = config_file.child("config");

	return ret;
}
void ModulePlayer::FillInputListFromXMLIterator(std::list<input>& list, pugi::xml_node& iterator)
{
	while (iterator != nullptr)
	{
		std::string input_name = iterator.attribute("value").as_string();

		if (input_name == "right")
		{
			list.push_back(RIGHT);
		}
		if (input_name == "left")
		{
			list.push_back(LEFT);
		}
		if (input_name == "down")
		{
			list.push_back(DOWN);
		}
		if (input_name == "punch")
		{
			list.push_back(PUNCH);
		}
		if (input_name == "kick")
		{
			list.push_back(KICK);
		}
		iterator = iterator.next_sibling();
	}
}
void ModulePlayer::FillStateListFromXMLIterator(std::list<character_state_enum>& list, pugi::xml_node& iterator)
{
	while (iterator != nullptr)
	{
		std::string attack_name = iterator.attribute("value").as_string();

		if (attack_name == "standing_punch")
		{
			list.push_back(STANDING_PUNCHING);
		}
		if (attack_name == "crouching_punch")
		{
			list.push_back(CROUCHING_PUNCHING);
		}
		if (attack_name == "standing_kick")
		{
			list.push_back(STANDING_KICKING);
		}
		if (attack_name == "crouching_kick")
		{
			list.push_back(CROUCHING_KICKING);
		}
		if (attack_name == "hadowken")
		{
			list.push_back(HADOWKEN);
		}
		if (attack_name == "tatsumaki")
		{
			list.push_back(TATSUMAKI);
		}

		iterator = iterator.next_sibling();
	}
}
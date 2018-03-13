# Research about combos: Theory

## What is a combo?

The term "combo" is an abreviation of "combination". It can have various meanings, but this reserch focuses on the meaning given mostly by games that imply hand on hand combat, such as Fighting games, Beat em ups and Hack and
Slash games. 

**A combo is an unescapable combination of attacks**

## Why are combos a good addition 

There are three basic reasons why combos make for a good player experience:

- They give the player a feeling of superiority over their enemies, as while preforming a combo, most of the time there is nothing that the opponent can do.

- There is a certain visual appeal in seeing a fighter perform flashy combinations of attacks.

- Combos make the player feel skillful, as they imply a lot of fast and powerful attacks. If the combo itself is difficult to execute, the effect is increased.

## Attack properties related to combos

To understand how combos are created, we first need to know which are the properties of attacks:

- **Startup**: Amount of time that it takes for the attack to be capable of doing damage

- **Hitstun**: Once the attack has delt damage, the amount of time that the enemy character is not able to move when he has been hit

- **Recovery**: After dealing damage, amount of time that has to pass in order for the attackant to do any action.

There are much more properties to attacks, but those are the basic ones to understand how combos work. Basicaly, to create a combo, we need to attack the opponent while they are not able to move because of the hitstun state.

## Ways of creating a combo

### Linking

The easier way to explain how linking works is to put an example:

Attack 1:

- Startup: 4 frames
- Hitstun: 5 frames
- Recovery: 2 frames

Attack 2:

- Startup: 2 frames
- Hitstun: 4 frames
- Recovery: 2 frames

Ok, lets do some maths. If we perform _attack 1_ and hit the opponent, after recovering from it, the opponent will still be in hitstun state for 3 frames (5 hitstun - 2 recovery). 

This means that if we do the _attack 2_ just after hitting _attack 1_, the _attack 2_ will 
connect because the opponent will still be in hitstun (2 startup <= 3 hitsun). 

However, if we were to do it the other way around, we wouldn't be able to, as after hitting _attack 2_ we only have 2 frames (4 hitstun - 2 recovery) for hitting the next attack, so we can't link _attack 1_ (4 startup > 2 hitstun) but we could 
link another _attack 2_ (2 startup <= 2 hitstun).

This is how linking works.

<iframe width="560" height="315" src="https://www.youtube.com/embed/m_kPN9QeHXs?rel=0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

### Cancelling

Cancelling reffers to cancel the animation of a move with another animation. Cancelling normally happens only when the attack has already done damage, meaning that we skip the recovery time of the move we cancel.

This would allow us to combo _attack 2_ with _attack 1_, as the histun that the attack 2 provides is equal to the startup of the attack 1, and we don't have to substract the recovery this time.

Most of the time, just some attacks can be cancelled into others, this is a essential part of the design of fighting games.

<iframe width="560" height="315" src="https://www.youtube.com/embed/fTF1z7SP0Xk?rel=0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

### Juggling

Juggling reffers to hitting the opponent while they are airborn in a vulnerable state, normally after a grounded attack that has launched them into the air.

<iframe width="560" height="315" src="https://www.youtube.com/embed/ZdnvoK2bdKc?rel=0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>


**Now that we know how to combo... We can can have a bit of fun!!**

<iframe width="560" height="315" src="https://www.youtube.com/embed/szbqQ_Lh72A?rel=0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

## Input combination

Mostly in fighting games, it is really common to have some actions performed by pressing a combination of inputs, as the game only focuses in a really little space and two characters, the number of actions performed must be extense,
as to make a interesting design, and this requires additional ways to input actions out of the basic ones.

For example, the most commun input combination is the "Hadouken" input, which requires the player to input:

-"down" -> "down-forward" -> "forward" -> "punch"

in order to trigger the special actions. Peaple tend to mistakenly call this a 
"combo", but while the special moves triggered by input combinations can be part of combos, they are not combos by themselves. 

![](Wiki%20images/Hadowken%20input%20image.png)

_Fist for the image found in worldartsme.com, FIRST PUNCH OUTLINE CLIPART IMAGE #1_

# Research about combos: Code implementation

**Asside from providing the concept of how to implement combos from a generic code perspective, I will provide examples to implement it specifically in C++**

## Input buffer 

In order to implement combos as well as directional inputs, we need to have an **input buffer**. The input buffer will store the inputs that the player has done the frames before the currently evaluated frane,
and we can read it in any way we want to determine what actions to take.

This is done in this way because of two reasons:

- Cancels and links require to be executed in a really specific frame. If we didn't implement the buffer, the player would have to introduce frame perfect inputs in order to perform them. On the other hand,
with the buffer we can read inputs performed before the time of cancelling, and if the player has introduced the input within a certain window, allow the cancel to happen. The narrower the window is, the more
precice the input has to be.

- Directional inputs can be managed in two ways if we didn't have the input buffer. We could require the player to do the combination in three consecutive frames, which is really hard to execute, or we could 
allow it to happen when the combination is performed, regardless of the time that has passed between the different inputs, which would result in undesired directional inputs performed while playing. 
As you can see, both desitions are bad ones, so what we need to do is read the input buffer and if the player has introduced the input combination in a certain amount of time, execute the special action.

The code implementation of an input buffer is basically the concept of how a queue works. My approach in C++ was to create a ordinary array, and with a function introduce new elements at the end of the array and shift the other ones. 
The first element of the array is erased and substituted by the second one. Every frame, a new input is introduced, and if no input is introduced the elements are shifted anyways. This way we have stored
all the inputs done previously

**Example**: We have the input buffer with 10 elements of length:

![](https://github.com/Skyway666/Input-Combos/raw/master/Wiki%20images/Input_buffer_shifting_1.png)

If we were to introduce a "left", the array would look like this:

![](https://github.com/Skyway666/Input-Combos/raw/master/Wiki%20images/Input_buffer_shifting_2.png)

This is the function used to do it:

```
void ModulePlayer::Push_into_buffer(input input)
{
	input_buffer[0] = NONE;

	for (int i = 0; i < MAX_INPUT_BUFFER; i++)
	{
		input_buffer[i] = input_buffer[i+1];
	}

	input_buffer[MAX_INPUT_BUFFER-1] = input;
}
```

## "Cancel" and "input combination" windows

As explained before, we need to give the player some time in order to perform a cancel or input combination. This is called a window, and it can be different for each cancel or directional input.

Larger windows makes for easier cancels/directional inputs, but can lead to undesired outcomes, while narrower windows provide more difficulty but also more presition. This can be used as a tool
by the designer to modify player experience, so the code has to be ready to support it. Fortunately, it is not really hard.

We can have the input buffer to be an array as large as we want it to be, and only read certain parts depending on the window of the specific move. Of course, we can't have a window to be larger than the array,
so make sure that the array is long enough.

You need to be able to perform fast iteration on this windows, so be sure to keep the value in a file and read it from the code, in order not to recompile it every time it is changed.

![](https://github.com/Skyway666/Input-Combos/raw/master/Wiki%20images/Input_buffer_window.png)

## Input combination collisions

There is times where two input combinations collide, meaning that with the same combination of moves can lead to two different combinations of inputs. 

The best way to explain this is with a practical example:

We have the normal hadouken and the improved hadouken:

- Hadouken: "down" -> "forward" -> "punch"

- Improved Hadouken: "down" -> "forward" -> "punch + punch"(two punches at the same time)

There would be no problem at handling simultaneous button presses, as we have the windows to manage it. The problem is that if we react to normal
Hadouken inmediatly, we would never be able to get the improved one, as upon recieving its sequence, it would reproduce normal Hadouken inmediatly. The only way
to handle this is by delaying the detection of smaller input combinations.

This means that if when we get the final "punch" to execute the hadowken, the input is the first one in the input buffer, we should wait some frames
to detect if another punch has been pressed. If it has, give priority to the improved Hadouken, by simply checking for it first instead of checking 
for the normal Hadouken. There should be a priority list created from a file that could easily be modified by the designer in order to stablish said 
priorities. The delay should not be used when reading the larger input combinations.

![](https://github.com/Skyway666/Input-Combos/raw/master/Wiki%20images/Input_buffer_delay.png)

## Reading input combinations

Input combinations need to be red from files in order to provide fast iteration. My approach to it is storing every combination in a list, so it can be as long as the designer pleases. When reading the buffer
we need to look for this combination and react to it, but only if it is within the wanted window. We also need to apply the delay needed for collisions of input combinations. 

Keep in mind that simultaneous button presses can be inputed in any order, so my approach is to store them in a separated lists, make a copy of the list
of simultaneous button presses and then remove the elements as they are found in the list, in order not to have the same input checked twice.


![](https://github.com/Skyway666/Input-Combos/raw/master/Wiki%20images/Input_buffer_detection_1.png)

_In this example, a hadouken is not detected_

![](https://github.com/Skyway666/Input-Combos/raw/master/Wiki%20images/Input_buffer_detection_2.png)

_In this example, a hadouken is detected_

**Example of input combination detection using delay.**
```
bool ModulePlayer::Check_for_hadowken()
{
	int counter = 0;
	int last_detection = 0;
	auto input_iterator = hadowken_inputs.begin();

	for (int i = MAX_INPUT_BUFFER - 1 - hadowken_cancelability_window; i < MAX_INPUT_BUFFER; i++)
	{

		if (input_buffer[i] == *input_iterator)
		{
			counter++;
			last_detection = i;
			if(counter < hadowken_inputs.size()) //We don't want to access something out of the list
			input_iterator++;
		}
	}

	if (counter == hadowken_inputs.size() && last_detection <= MAX_INPUT_BUFFER - 1 - detection_delay)
		return true;
	else
		return false;
}
```

**Example of input combination detection using simultaneous button presses. Delay is not needed as there is no input combination bigger than the
super hadouken in my demo, but if there was it should be applyed too.**
```
bool ModulePlayer::Check_for_super_hadowken()
{
	int counter = 0;
	//We create a copy of the simultaneous list to remove inputs when we find them
	auto super_hadowken_simultaneous_attacks_mock = super_hadowken_simultaneous_attacks;
	auto direction_input_iterator = super_hadowken_directions.begin();

	
	for (int i = MAX_INPUT_BUFFER - 1 - super_hadowken_cancelability_window; i < MAX_INPUT_BUFFER; i++)
	{
		//First detect directions, in which order is important
		if (input_buffer[i] == *direction_input_iterator && counter < 2)
		{
			counter++;
			if (counter < super_hadowken_directions.size()) //We don't want to access something out of the list
				direction_input_iterator++;
		}
		//Now detect attacks, in which order is irrelevant
		if (counter >= super_hadowken_directions.size())
		{

			for (std::list<input>::iterator it = super_hadowken_simultaneous_attacks_mock.begin(); it != super_hadowken_simultaneous_attacks_mock.end(); it++)
			{
				if (input_buffer[i] == *it)
				{
					counter++;
					super_hadowken_simultaneous_attacks_mock.remove(*it);
					break;
				}
			}
		}
	}

	if (counter == super_hadowken_directions.size() + super_hadowken_simultaneous_attacks.size())
		return true;
	else
		return false;
}
```

## Cancelling implementation

Now that we have all the tools to read the buffer in order to see what inputs were executed previously, and we know 
how to detect when a special move has happened, it's time to program the combo system.

First we should build a solid state machine, which will divide the inputs and the states of the character. This is 
specially important in fighting games, as characters have lots of states, and they react to inputs in a different way
depending on it.

### State machine

My approach is to create an _enum_ of "inputs" and another one with "states". We always have a wanted state and a current state,
and depending on the situation we will decide weather if to allow the wanted state to be the current one or not.

Detection of directions and attacks need to be separated, as they are used for different things. We will use _bools_
in order to keep track of the directions pressed.

### Requesting states

As said before, depending on the input and the state of the player, we will request different states. 

If some direction is pressed, we request movement inmediatly, and if an attack is pressed it will overwrite it.

We should only be pushing one direction in the buffer at a time, combinations of directions are supported as independent inputs, and when recieving opposite directions we 
should prioritize one of them. 

Then we detect attacks. Those are not requested inmediatly, instead they are pushed into the buffer, as we need to evaluate the buffer first.

Now we check the buffer, and if no input combination has been performed, we request attacks. Remember to apply the
delay for combo collision, otherwise you will detect input combinations while the player is already preforming a punch.

When requesting an attack, use the cancelability window of attacks already, meaning that you will set the wanted state
not depending on the attack pressed this frame but the ones that have been inputed before, this is really important 
for cancelling.

```
	//Manage movement
	//If movement is done it will be recorded as wanted state, but if an attack is preformed it will overwrite it. Also push input into the buffer, as movement input should be continuous

	//We use "else if" because we don't want the character to move back nor forward while crouching
	if (direction_inputs.down)
	{
		button_pressed = true;
		Push_into_buffer(DOWN);
		wanted_state = CROUCHING;
	}
	else if (direction_inputs.left)
	{
		button_pressed = true;
		Push_into_buffer(LEFT);
		wanted_state = WALKING_BACK;
	}
	else if (direction_inputs.right)
	{
		button_pressed = true;
		Push_into_buffer(RIGHT);
		wanted_state = WALKING_FORWARD;
	}
	else
	{
		wanted_state = IDLE;
	}

	//Read attack inputs
	//Check attack inputs and push them into the buffer
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

	//We need to move the buffer in all the frames in order to keep only the last inputs, therefore if no button has been pressed, we push a "NONE" input into the buffer
	if(!button_pressed)
	{
		Push_into_buffer(NONE); 
	}

	//Manage attacks
	//Check if a special move has been performed, as they have priority over all other moves. If it has, assign the wanted action to be executed in this frame 
	
	if (Check_for_super_hadowken())
	{
		wanted_state = SUPER_HADOWKEN;
	}
	else if (Check_for_tatsumaki())
	{
		wanted_state = TATSUMAKI;
	}
	else if (Check_for_hadowken())
	{
		wanted_state = HADOWKEN;
	}
	//If no special move has been performed, assign the wanted action depending on the last input that has been pressed (last one in the input buffer). Mind that 
	//depending on the current state, you may want different actions
	else
	{
		input current_input = Catch_first_attack_input_within(normal_moves_cancelability_window, detection_delay);

		switch (current_input)
		{
			case PUNCH:
			{
				if (direction_inputs.down)
					wanted_state = CROUCHING_PUNCHING;
				else
					wanted_state = STANDING_PUNCHING;
				break;
			}
			case KICK:
			{
				if (direction_inputs.down)
					wanted_state = CROUCHING_KICKING;
				else
					wanted_state = STANDING_KICKING;
				break;
			}
		}
	}
```
**Function used to get the attack inputs of previous frames**
```
input ModulePlayer::Catch_first_attack_input_within(int window, int delay)
{
	for (int i = (MAX_INPUT_BUFFER - window - 1); i < MAX_INPUT_BUFFER-delay; i++)
	{
		if (Is_attack_input(input_buffer[i]))
		{
			return input_buffer[i];
		}
	}
	return NONE;
}
```

### Assign current state

When assigning the current state from the wanted state, we first need to check the current state. If it is a neutral state,
such as normal movement, or crouching, which can be canceled at any time, we directly update the current state and assign the
new animation (with any other consequences the begining of the animation might have).

If on the other hand, the current state is not movement, things get trickier. As said before, cancels only happen in certain frames
of the animation, and certain moves can only be canceled in certain others. We need to store all this information,
and my approach is as follows:

- To store the cancelability values, we set up an _struct_ with a state and a list of states, that represent all the states
in which the state is cancelable. 

**Character state struct**
```
struct character_state
{
	character_state_enum state;
	std::list<character_state_enum> cancelability;
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
```
- To know if the animation frame is cancelable or not, we create a struct that will be managed by the class "Animation"
with a _SDL_Rect_ and an enum called _animation_state_, which will consist of STARTUP, ACTIVE and RECOVERY.
Then we create a function that returns the state of the animation. If it is ACTIVE we allow the cancel to happen. In most fighting games
cancels are only allowed if the attack has hit the opponent, but we'll ignore that for the sake of simplicity.


Finally, the code to manage the change of state would look like this:

**Code to manage change of state**
```
	if (Current_state_is_movement())
	{
		current_state = wanted_state;
		Update_animation_depending_on_current_state();
	}
	else
	{ 
		if (Can_cancel_current_state_into(wanted_state) && current_animation->GetState() == ACTIVE)
		{
			current_state = wanted_state;
			Update_animation_depending_on_current_state();
		}
		else if (current_animation->Finished())
		{
		current_state = IDLE;
		Standing_punch.Reset();
		Hadowken.Reset();
		Crouching_punch.Reset();
		Tatsumaki.Reset();
		Standing_kick.Reset();
		Crouching_kick.Reset();
		Update_animation_depending_on_current_state();
		}
	}

```

You can see my code demo [here](https://github.com/Skyway666/Input-Combos).

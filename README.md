﻿# Reserch about combos: Theory

## What is a combo?

The term "combo" is an abreviation of "combination". It can have various meanings, but the focus of this reserch is the meaning given mostly by games that imply hand on hand combat, such as Fighting games, Beat em ups and Hack and
Slash games. 

**A combo is an unescapable combination of attacks**

## Why are combos a good addition 

There are three basic reasons why combos make for a good playing experience:

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

- Startup: 4
- Hitstun: 5
- Recovery: 2

Attack 2:

- Startup: 2
- Hitstun: 4
- Recovery: 2

Ok, lets do some maths. If we perform attack 1 and hit the opponent, after recovering from it, the opponent will still be in hitstun state for 3 (5 hitstun - 2 recovery). This means that if we do the attack 2 just after hitting attack 1, the attack 2 will 
connect because the opponent will still be in hitstun (2 startup <= 3 hitsun). (_**insert visual support**_)

However, if we were to do it the other way around, we wouldn't be able to, as after hitting attack 2 we only have 2 (4 hitstun - 2 recovery) for hitting the next attack, se we can't link attack 1 (4 startup > 2 hitstun) but we could 
link another attack 2 (2 startup <= 2 hitstun).

This is how linking works.

### Cancelling

Cancelling reffers to cancel the animation of a move with another animation. Cancelling normally happens only when the attack has already done damage, meaning that we skip the recovery time of the move we cancel.

This would allow us to combo attack 2 with attack 1, as the histun that the attack 2 provides is equal to the startup of the attack 1, and we don't have to substract the recovery this time. (_**insert visual support**_)

Most of the time, just some attacks can be cancelled into others, this is a essential part of the design of fighting games.

### Juggling

Juggling reffers to hitting the opponent while they are airborn in a vulnerable state, normally after a grounded attack that has launched them into the air.


## Input combination

Mostly in fighting games, it is really common to have some actions performed by pressing a combination of inputs, as the game only focuses in a really little space and two characters, the number of actions performed must be extense,
as to make a interesting design, and this requires additional ways to input actions out of the basic ones.

For example, the most commun input combination is the "Hadouken" input, which requires the player to input "down" -> "down-forward" -> "forward" -> "punch" in order to trigger the special actions. Peaple tend to mistakenly call this a 
"combo", but while the special moves triggered by input combinations can be part of combos, they are not combos by themselves. (_**insert visual support**_)

# Reserch about combos: Code implementation

## Input buffer

In order to implement combos as well as directional inputs, we need to have an **input buffer**. The input buffer will store the inputs that the player has done the frames before the current evaluated friend,
and we can read it in any way we want to determine what actions to take.

This is done in this way because of two reasons:

- Cancels and links require to be executed in a really specific frame. If we didn't implement the buffer, the player would have to introduce frame perfect inputs in order to perform them. On the other hand,
with the buffer we can read inputs performed before the time of cancelling, and if the player has introduced the input within a certain window, allow the cancel to happen. The narrower the window is, the more
precice the input has to be.

- Directional inputs can be managed in two ways if we didn't have the input buffer. We could require the player to do the combination in three consecutive frames, which is really hard to execute, or we could 
allow it to happen when the combination is performed, regardless of the time that has passed between the different inputs, which would result in undesired directional inputs performed while playing.

As you can see, both desitions are bad ones, so what we need to do is read the input buffer and if the player has introduced the input combination in a certain amount of time, execute the special action.

The code implementation of an input buffer is basically the concept of how a queue works. My approach was to create a ordinary array, and with a function introduce new elements at the end of the array and shift the other ones. 
The first element of the array is erased and substituted by the second one. Every frame, a new input is introduced, and if no input is introduced the elements are shifted anyways. This way we have stored
all the inputs done previously

**Example**: We have the input buffer with 5 elements of length. 

1. None
2. Left
3. Right
4. None
5. Punch

If we were to introduce a "kick", the array would look like this:

1. Left
2. Right
3. None
4. Punch
5. Kick


## "Cancel" and "input combination" windows

As explained before, we need to give the player some time in order to perform a cancel or input combination. This is called a window, and it can be different for each cancel or directional input.

Larger windows makes for easier cancels/directional inputs, but can lead to undesired outcomes, while narrower windows provide more difficulty but also more presition. This can be used as a tool
by the designer to modify player experience, so the code has to be ready to support it. Fortunately, it is not really hard.

We can have the input buffer to be an array as large as we want it to be, and only read certain parts depending on the window of the specific move. Of course, we can't have a window to be larger than the array,
so make sure that the array is long enough.





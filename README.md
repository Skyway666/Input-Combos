# Reserch about combos

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

To understand how combos are created, we first need to know which are the properties of different attacks:

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
connect because the opponent will still be in hitstun (2 startup <= 3 hitsun). (insert visual support)

However, if we were to do it the other way around, we wouldn't be able to, as after hitting attack 2 we only have 2 (4 hitstun - 2 recovery) for hitting the next attack, se we can't link attack 1 (4 startup > 2 hitstun) but we could 
link another attack 2 (2 startup <= 2 hitstun).

This is how linking works.

### Cancelling


Cancelling reffers to cancel the animation of a move with another animation. Cancelling normally happens only when the attack has already done damage, meaning that we skip the recovery time of the move we cancel.

This would allow us to combo attack 2 with attack 1, as the histun that the attack 2 provides is equal to the startup of the attack 1, and we don't have to substract the recovery this time. (insert visual support)






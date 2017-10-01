#ifndef DRUNKARD_OWNED_STATES_H
#define DRUNKARD_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   DrunkardOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Drunkard class
//
//------------------------------------------------------------------------
#include "fsm/State.h"
#include "StateUtils.h"

class Drunkard;

#define DRUNKARD_STATE(state_name) ENTITY_STATE(state_name, Drunkard)

//------------------------------------------------------------------------
//
//  In this state, the drunkard rests at home until he is rested.
//  Once he is, he goes to the saloon to drink.
//------------------------------------------------------------------------
DRUNKARD_STATE(GoHomeAndSleepTilRested2);

//------------------------------------------------------------------------
//
//  In this state, the drunkard drinks until he is too tired and goes back
//  home.
//  If he gets drunk, he tries to fight with anyone in the saloon.
//------------------------------------------------------------------------
DRUNKARD_STATE(GoToSaloonAndDrink);

//------------------------------------------------------------------------
//
//  In this state, the drunkard has found someone to fight.
//  If he wins, he goes back to drinking, otherwhise, he goes home.
//------------------------------------------------------------------------
DRUNKARD_STATE(DrunkardFight);

#endif

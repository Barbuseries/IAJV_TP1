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

DRUNKARD_STATE(GoHomeAndSleepTilRested2);
DRUNKARD_STATE(GoToSaloonAndDrink);
DRUNKARD_STATE(DrunkardFight);
//DRUNKARD_STATE(GoOutsideToSleep);

#endif

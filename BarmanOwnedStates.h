#ifndef BARMAN_OWNED_STATES_H
#define BARMAN_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   BarmanOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Barman class
//
//------------------------------------------------------------------------
#include "fsm/State.h"
#include "StateUtils.h"

class Barman;

#define BARMAN_STATE(state_name) ENTITY_STATE(state_name, Barman)

//------------------------------------------------------------------------
//
//  In this state, the barman serve custormers their drink.
//  If a fight breaks out, he starts refereeing it.
//------------------------------------------------------------------------
BARMAN_STATE(ServeCustomers);

//------------------------------------------------------------------------
//
//  In this state, the barman is refereeing a fight. Once it is over,
//  he goes back to serve customers.
//------------------------------------------------------------------------
BARMAN_STATE(RefereeingAFight);

#endif
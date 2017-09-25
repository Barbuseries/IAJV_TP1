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

BARMAN_STATE(ServeCustomers);
BARMAN_STATE(RefereeingAFight);

#endif
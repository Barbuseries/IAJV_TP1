#ifndef MINERSWIFE_OWNED_STATES_H
#define MINERSWIFE_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinersWifeOwnedStates.h
//
//  Desc:   All the states that can be assigned to the MinersWife class
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "fsm/State.h"
#include "StateUtils.h"

class MinersWife;

#define WIFE_STATE(state_name) ENTITY_STATE(state_name, MinersWife)

//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
ENTITY_GLOBAL_STATE(WifesGlobalState, MinersWife);

//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
WIFE_STATE(DoHouseWork);

//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
WIFE_STATE(VisitBathroom);

//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
WIFE_STATE(CookStew);

#endif

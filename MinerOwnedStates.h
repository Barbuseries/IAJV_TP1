#ifndef MINER_OWNED_STATES_H
#define MINER_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinerOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Miner class.
//          Note that a global state has not been implemented.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "fsm/State.h"
#include "StateUtils.h"

class Miner;
struct Telegram;

#define MINER_STATE(state_name) ENTITY_STATE(state_name, Miner)

//------------------------------------------------------------------------
//
//  In this state the miner will walk to a goldmine and pick up a nugget
//  of gold. If the miner already has a nugget of gold he'll change state
//  to VisitBankAndDepositGold. If he gets thirsty he'll change state
//  to QuenchThirst
//------------------------------------------------------------------------
MINER_STATE(EnterMineAndDigForNugget);

//------------------------------------------------------------------------
//
//  Entity will go to a bank and deposit any nuggets he is carrying. If the 
//  miner is subsequently wealthy enough he'll walk home, otherwise he'll
//  keep going to get more gold
//------------------------------------------------------------------------
MINER_STATE(VisitBankAndDepositGold);

//------------------------------------------------------------------------
//
//  miner will go home and sleep until his fatigue is decreased
//  sufficiently
//------------------------------------------------------------------------
MINER_STATE(GoHomeAndSleepTilRested);

//------------------------------------------------------------------------
//
//  miner changes location to the saloon and keeps buying Whiskey until
//  his thirst is quenched. When satisfied he returns to the goldmine
//  and resumes his quest for nuggets.
//------------------------------------------------------------------------
MINER_STATE(QuenchThirst);

//------------------------------------------------------------------------
//
//  this is implemented as a state blip. The miner eats the stew, gives
//  Elsa some compliments and then returns to his previous state
//------------------------------------------------------------------------
MINER_STATE(EatStew);

//------------------------------------------------------------------------
//
//  miner fights an opponent in a deadly battle and goes home if he wins.
//  He goes back to the mine otherwhise.
//------------------------------------------------------------------------
MINER_STATE(Fight);

#endif

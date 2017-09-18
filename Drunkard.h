#ifndef DRUNKARD_H
#define DRUNKARD_H
//------------------------------------------------------------------------
//
//  Name:   Drunkard.h
//
//  Desc:   A class defining a drunkard.
//
//------------------------------------------------------------------------
#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "DrunkardOwnedStates.h"
#include "fsm/StateMachine.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

//above this value a drunkard is drunk
const int IntoxicationThreshold = 3;
//above this value a miner is sleepy
//const int TirednessThreshold = 5;


class Drunkard : public BaseGameEntity
{
private:

	//an instance of the state machine class
	StateMachine<Drunkard>*  m_pStateMachine;

	location_type         m_Location;

	//the higher the value, the more drunk the drunkard
	int					  m_iIntoxication;

	//the higher the value, the more tired the drunkard
	int                   m_iFatigue;

public:

	Drunkard(int id) :m_Location(shack),
		m_iIntoxication(0),
		m_iFatigue(0),
		BaseGameEntity(id)

	{
		//set up state machine
		m_pStateMachine = new StateMachine<Drunkard>(this);

		m_pStateMachine->SetCurrentState(GoHomeAndSleepTilRested2::Instance());

		/* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE DRUNKARD */
	}

	~Drunkard() { delete m_pStateMachine; }

	//this must be implemented
	void Update();

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);


	StateMachine<Drunkard>* GetFSM()const { return m_pStateMachine; }



	//-------------------------------------------------------------accessors
	location_type Location()const { return m_Location; }
	void          ChangeLocation(location_type loc) { m_Location = loc; }

	bool          Fatigued()const { return m_iFatigue > 5; };
	void          DecreaseFatigue() { m_iFatigue -= 1; }
	void          IncreaseFatigue() { m_iFatigue += 1; }

	bool		  Drunk() const { return m_iIntoxication > IntoxicationThreshold; };
	void		  DrinkBeer() { m_iIntoxication += 1;  }

};

#endif

#include "DrunkardOwnedStates.h"
#include "fsm/State.h"
#include "Drunkard.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested
GoHomeAndSleepTilRested2* GoHomeAndSleepTilRested2::Instance()
{
	static GoHomeAndSleepTilRested2 instance;

	return &instance;
}

void GoHomeAndSleepTilRested2::Enter(Drunkard* pDrunkard)
{
	if (pDrunkard->Location() != drunkard_shack) {
		pDrunkard->ChangeLocation(drunkard_shack);
		cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Goin' home! Swweet home...";
	}
}


void GoHomeAndSleepTilRested2::Execute(Drunkard* pDrunkard)
{
	if (pDrunkard->Rested()) {
		pDrunkard->GetFSM()->ChangeState(GoToSaloonAndDrink::Instance());
	}
	else {
		pDrunkard->DecreaseFatigue();
		pDrunkard->SoberUp();
		cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "*Snoring*";
	}
}


void GoHomeAndSleepTilRested2::Exit(Drunkard* pDrunkard)
{
	cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": "
		<< "Ah'm as good as new... Time to hit the road.";
}


bool GoHomeAndSleepTilRested2::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}


//------------------------------------------------------------------------methods for GoToSaloonAndDrink
GoToSaloonAndDrink* GoToSaloonAndDrink::Instance()
{
	static GoToSaloonAndDrink instance;

	return &instance;
}

void GoToSaloonAndDrink::Enter(Drunkard* pDrunkard)
{
	if (pDrunkard->Location() != saloon) {
		pDrunkard->ChangeLocation(saloon);
		cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "'Tis drinkin' time people.";
	}
}


void GoToSaloonAndDrink::Execute(Drunkard* pDrunkard)
{
	pDrunkard->DrinkBeer();
	pDrunkard->IncreaseFatigue();

	cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Nothin's better than a beer!";

	if (pDrunkard->Fatigued()) {
		pDrunkard->GetFSM()->ChangeState(GoHomeAndSleepTilRested2::Instance());
	}
	else {
		if (pDrunkard->Drunk()) {
			Dispatch->DispatchMessageToLocation(SEND_MSG_IMMEDIATELY,
				pDrunkard->ID(),
				pDrunkard->Location(),
				Msg_BringItOn,
				NULL);
			cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "If ah could, I'd fight ev'ry one of ya all.. *hic*";
		}
	}
}


void GoToSaloonAndDrink::Exit(Drunkard* pDrunkard)
{
	cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": "
		<< "Drinkin' time's over...";
}


bool GoToSaloonAndDrink::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}
#include "DrunkardOwnedStates.h"
#include "fsm/State.h"
#include "Drunkard.h"
#include "DrinkTypes.h"
#include "FightMove.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"
#include "Output.h"

#include <iostream>
using std::cout;

#define ASK_FOR_BEER(delay) Dispatch->DispatchMessage(delay,\
													  pDrunkard->ID(), \
													  ent_Barman, \
													  Msg_AskForDrink, \
													  (void *)DrinkType::Beer);

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

		Output output(pDrunkard->ID());

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

		Output output(pDrunkard->ID());

		cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "*Snoring*";
	}
}


void GoHomeAndSleepTilRested2::Exit(Drunkard* pDrunkard)
{
	Output output(pDrunkard->ID());

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

		Output output(pDrunkard->ID());

		cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "'Tis drinkin' time people.";
	}

	ASK_FOR_BEER(SEND_MSG_IMMEDIATELY);
}


void GoToSaloonAndDrink::Execute(Drunkard* pDrunkard)
{
	if (pDrunkard->Fatigued()) {
		pDrunkard->GetFSM()->ChangeState(GoHomeAndSleepTilRested2::Instance());
	}
	else {
		if (pDrunkard->Drunk()) {
			{
				Output output(pDrunkard->ID());

				cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "If ah could, I'd fight ev'ry one of ya all.. *hic*";
			}

			Dispatch->DispatchMessageToLocation(SEND_MSG_IMMEDIATELY,
				pDrunkard->ID(),
				pDrunkard->Location(),
				Msg_BringItOn,
				NULL);
		}
	}
}


void GoToSaloonAndDrink::Exit(Drunkard* pDrunkard)
{
	Output output(pDrunkard->ID());

	cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": "
		<< "Drinkin' time's over...";
}


bool GoToSaloonAndDrink::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_DrinkReady:
	{
		{
			Output output;

			cout << "\nMessage received by " << GetNameOfEntity(pDrunkard->ID()) <<
				" at time: " << Clock->GetCurrentTime();

			output.ChangeEntity(pDrunkard->ID());

			cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Nothin's better than a beer!";

		}

		pDrunkard->DrinkBeer();
		pDrunkard->IncreaseFatigue();

		// It's a drunkard, after all
		ASK_FOR_BEER(1);

		return true;
	}

	case Msg_ImOn:
	{
		{
			Output output;

			cout << "\nMessage received by " << GetNameOfEntity(pDrunkard->ID()) <<
				" at time: " << Clock->GetCurrentTime();

			output.ChangeEntity(pDrunkard->ID());

			cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "That's more like it!"
				<< " Barman, watch this!";
		}

		int fight[2];
		fight[0] = pDrunkard->ID();
		fight[1] = msg.Sender;

		pDrunkard->GetFSM()->ChangeState(DrunkardFight::Instance());

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
			pDrunkard->ID(),
			ent_Barman,
			Msg_RefereeFight,
			(void *)fight);

		return true;
	}

	}//end switch

	return false;
}

//------------------------------------------------------------------------DrunkardFight

DrunkardFight* DrunkardFight::Instance()
{
	static DrunkardFight instance;

	return &instance;
}

void DrunkardFight::Enter(Drunkard* pDrunkard)
{
	Output output(pDrunkard->ID());

	cout << "\n" << GetNameOfEntity(pDrunkard->ID())
		<< ": <INSERT FUNNY SENTENCE HERE>";
}

void DrunkardFight::Execute(Drunkard* pDrunkard)
{
}


void DrunkardFight::Exit(Drunkard* pDrunkard)
{
}


bool DrunkardFight::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_ChooseMove:
	{
		{
			Output output;

			cout << "\nMessage received by " << GetNameOfEntity(pDrunkard->ID()) <<
				" at time: " << Clock->GetCurrentTime();
		}

		int move;
		float r = 0.3f; // Ah'm drunk!

		if (r < 0.33) {
			move = FightMove::Rock;
		}
		else if (r < 0.66) {
			move = FightMove::Paper;
		}
		else {
			move = FightMove::Cisors;
		}

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
			pDrunkard->ID(),
			msg.Sender,
			Msg_ChooseMove,
			(void *)move);

		return true;
	}

	case Msg_Win:
	{
		{
			Output output;

			cout << "\nMessage received by " << GetNameOfEntity(pDrunkard->ID()) <<
				" at time: " << Clock->GetCurrentTime();

			output.ChangeEntity(pDrunkard->ID());

			cout << "\n" << GetNameOfEntity(pDrunkard->ID())
				<< ": Amateur!";
		}


		pDrunkard->GetFSM()->ChangeState(GoToSaloonAndDrink::Instance());

		return true;
	}

	case Msg_Lose:
	{
		{
			Output output;

			cout << "\nMessage received by " << GetNameOfEntity(pDrunkard->ID()) <<
				" at time: " << Clock->GetCurrentTime();

			output.ChangeEntity(pDrunkard->ID());

			cout << "\n" << GetNameOfEntity(pDrunkard->ID())
				<< ": Hey! Ya were s'pose to play cisors...";
		}

		pDrunkard->GetFSM()->ChangeState(GoHomeAndSleepTilRested2::Instance());

		return true;
	}

	}//end switch

	return false;
}
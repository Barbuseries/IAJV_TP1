#include "BarmanOwnedStates.h"
#include "fsm/State.h"
#include "Barman.h"
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


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


//------------------------------------------------------------------------methods for ServeCustormers
ServeCustomers* ServeCustomers::Instance()
{
	static ServeCustomers instance;

	return &instance;
}

void ServeCustomers::Enter(Barman* pBarman)
{
	if (pBarman->Location() != saloon) {
		pBarman->ChangeLocation(saloon);
	}

	{
		Output output(pBarman->ID());

		cout << "\n" << GetNameOfEntity(pBarman->ID()) << ": " << "Bar's open! Come 'n drink all ya want!";
	}
}


void ServeCustomers::Execute(Barman* pBarman)
{
	// Wait for messages
}


void ServeCustomers::Exit(Barman* pBarman)
{
	Output output(pBarman->ID());

	cout << "\n" << GetNameOfEntity(pBarman->ID()) << ": "
		<< "Bar's closed for now...";
}


bool ServeCustomers::OnMessage(Barman* pBarman, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_AskForDrink:
	{
		int drink = (int)(msg.ExtraInfo);

		{
			Output output;

			cout << "\nMessage received by " << GetNameOfEntity(pBarman->ID()) <<
				" at time: " << Clock->GetCurrentTime();

			output.ChangeEntity(pBarman->ID());

			cout << "\n" << GetNameOfEntity(pBarman->ID())
				<< ": A " << DrinkToStr(drink) << " for "
				<< GetNameOfEntity(msg.Sender) << ". Got it!";
		}

		//let customer know the drink is ready
		Dispatch->DispatchMessage(0.2,
			pBarman->ID(),
			msg.Sender,
			Msg_DrinkReady,
			NO_ADDITIONAL_INFO);

		return true;
	}

	case Msg_RefereeFight:
	{
		int *fight = (int *)msg.ExtraInfo;

		{
			Output output;

			cout << "\nMessage received by " << GetNameOfEntity(pBarman->ID()) <<
				" at time: " << Clock->GetCurrentTime();

			output.ChangeEntity(pBarman->ID());

			cout << "\n" << GetNameOfEntity(pBarman->ID())
				<< ": Fight between " << GetNameOfEntity(fight[0]) << " and "
				<< GetNameOfEntity(fight[1]) << "!";
		}

		pBarman->GetFSM()->ChangeState(RefereeingAFight::Instance());

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
			pBarman->ID(),
			fight[0],
			Msg_ChooseMove,
			NO_ADDITIONAL_INFO);

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
			pBarman->ID(),
			fight[1],
			Msg_ChooseMove,
			NO_ADDITIONAL_INFO);

		return true;
	}

	}//end switch

	return false;
}

//------------------------------------------------------------------------methods for RefereeingAFight
RefereeingAFight* RefereeingAFight::Instance()
{
	static RefereeingAFight instance;

	return &instance;
}

void RefereeingAFight::Enter(Barman* pBarman)
{
	Output output(pBarman->ID());

	cout << "\n" << GetNameOfEntity(pBarman->ID())
		<< ": FIGHT! FIGHT! FIGHT!";
}


void RefereeingAFight::Execute(Barman* pBarman)
{
	// Wait for messages
}


void RefereeingAFight::Exit(Barman* pBarman)
{
	Output output(pBarman->ID());

	cout << "\n" << GetNameOfEntity(pBarman->ID()) << ": "
		<< "Fight's over everybody... Go back to drinkin'!";
}


bool RefereeingAFight::OnMessage(Barman* pBarman, const Telegram& msg)
{
	static int oldMove = -1, oldSender;

	switch (msg.Msg)
	{
	case Msg_AskForDrink:
	{
		{
			Output output;

			cout << "\nMessage received by " << GetNameOfEntity(pBarman->ID()) <<
				" at time: " << Clock->GetCurrentTime();

			output.ChangeEntity(pBarman->ID());

			cout << "\n" << GetNameOfEntity(pBarman->ID())
				<< ": Not now, bar's closed!";
		}

		return true;
	}

	case Msg_ChooseMove:
	{
		{
			Output output;

			cout << "\nMessage received by " << GetNameOfEntity(pBarman->ID()) <<
				" at time: " << Clock->GetCurrentTime();

			output.ChangeEntity(pBarman->ID());
		}

		int move = (int)(msg.ExtraInfo);

		if (oldMove == -1) {
			oldMove = move;
			oldSender = msg.Sender;
		}
		else {
			if (oldMove == move) {
				{
					Output output(pBarman->ID());

					cout << "\n" << GetNameOfEntity(pBarman->ID())
						<< ": Ain,'t goin' to have a draw in here!";
				}

				oldMove = -1;
				oldSender = -1;

				Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
					pBarman->ID(),
					oldSender,
					Msg_ChooseMove,
					NO_ADDITIONAL_INFO);

				Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
					pBarman->ID(),
					msg.Sender,
					Msg_ChooseMove,
					NO_ADDITIONAL_INFO);
			}
			else {
				int winner, loser;

				if (((oldMove == FightMove::Rock) && (move == FightMove::Cisors)) ||
					((oldMove == FightMove::Paper) && (move == FightMove::Rock)) ||
					((oldMove == FightMove::Cisors) && (move == FightMove::Paper))) {
					winner = oldSender;
					loser = msg.Sender;
				}
				else {
					winner = msg.Sender;
					loser = oldSender;
				}

				oldMove = -1;
				oldSender = -1;

				{
					Output output(pBarman->ID());

					cout << "\n" << GetNameOfEntity(pBarman->ID())
						<< ": " << GetNameOfEntity(winner) << " wins! FATALITY!";

					cout << "\n" << GetNameOfEntity(pBarman->ID())
						<< ": " << GetNameOfEntity(loser) << " lost! Get out of ma saloon, ya punk!";
				}

				pBarman->GetFSM()->ChangeState(ServeCustomers::Instance());

				Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
					pBarman->ID(),
					winner,
					Msg_Win,
					NO_ADDITIONAL_INFO);

				Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
					pBarman->ID(),
					loser,
					Msg_Lose,
					NO_ADDITIONAL_INFO);
			}
		}

		return true;
	}
	}//end switch

	return false;
}

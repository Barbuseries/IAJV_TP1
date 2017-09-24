#include "BarmanOwnedStates.h"
#include "fsm/State.h"
#include "Barman.h"
#include "DrinkTypes.h"
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


//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested
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
			int drink = (int) (msg.ExtraInfo);

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
		}

		return true;

	}//end switch

	return false;
}
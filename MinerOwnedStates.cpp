#include "MinerOwnedStates.h"
#include "fsm/State.h"
#include "Miner.h"
#include "DrinkTypes.h"
#include "FightMove.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "misc/Utils.h"
#include "EntityNames.h"

#include "Output.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
	static EnterMineAndDigForNugget instance;

	return &instance;
}


void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	if (pMiner->Location() != goldmine)
	{
		Output output(pMiner->ID());

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' to the goldmine";

		pMiner->ChangeLocation(goldmine);
	}
}


void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pMiner->AddToGoldCarried(1);

	pMiner->IncreaseFatigue();

	{
		Output output(pMiner->ID());

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Pickin' up a nugget";
	}

	//if enough gold mined, go and put it in the bank
	if (pMiner->PocketsFull())
	{
		pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
	}

	if (pMiner->Thirsty())
	{
		pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
	Output output(pMiner->ID());

	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
		<< "Ah'm leavin' the goldmine with mah pockets full o' sweet gold";
}


bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
	static VisitBankAndDepositGold instance;

	return &instance;
}

void VisitBankAndDepositGold::Enter(Miner* pMiner)
{
	//on entry the miner makes sure he is located at the bank
	if (pMiner->Location() != bank)
	{
		Output output(pMiner->ID());

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Goin' to the bank. Yes siree";

		pMiner->ChangeLocation(bank);
	}
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
	//deposit the gold
	pMiner->AddToWealth(pMiner->GoldCarried());

	pMiner->SetGoldCarried(0);

	{
		Output output(pMiner->ID());

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
			<< "Depositing gold. Total savings now: " << pMiner->Wealth();
	}

	//wealthy enough to have a well earned rest?
	if (pMiner->Wealth() >= Miner::ComfortLevel)
	{
		{
			Output output(pMiner->ID());

			cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
				<< "WooHoo! Rich enough for now. Back home to mah li'lle lady";
		}

		pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
	
	}

	//otherwise get more gold
	else
	{
		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
	Output output(pMiner->ID());

	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Leavin' the bank";
}


bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
	static GoHomeAndSleepTilRested instance;

	return &instance;
}

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
	if (pMiner->Location() != shack)
	{
		{
			Output output(pMiner->ID());

			cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' home";
		}
		pMiner->ChangeLocation(shack);

		//let the wife know I'm home
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Elsa,            //ID of recipient
			Msg_HiHoneyImHome,   //the message
			NO_ADDITIONAL_INFO);
	}
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{
	//if miner is not fatigued start to dig for nuggets again.
	if (!pMiner->Fatigued())
	{
		{
			Output output(pMiner->ID());

			cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
				<< "All mah fatigue has drained away. Time to find more gold!";
		}

		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}

	else
	{
		//sleep
		pMiner->DecreaseFatigue();

		Output output(pMiner->ID());

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "ZZZZ... ";
	}
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{
}


bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
	switch (msg.Msg)
	{
		case Msg_StewReady:
		{
			Output output;

			cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
				<< " at time: " << Clock->GetCurrentTime();

			output.ChangeEntity(pMiner->ID());

			cout << "\n" << GetNameOfEntity(pMiner->ID())
				<< ": Okay Hun, ahm a comin'!";
		}

		pMiner->GetFSM()->ChangeState(EatStew::Instance());

		return true;

	}//end switch

	return false; //send message to global message handler
}

//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
	static QuenchThirst instance;

	return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
	if (pMiner->Location() != saloon)
	{
		pMiner->ChangeLocation(saloon);

		{
			Output output(pMiner->ID());

			cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Boy, ah sure is thusty! Walking to the saloon";
		}
	}

	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
							pMiner->ID(),        //ID of sender
							ent_Barman,            //ID of recipient
							Msg_AskForDrink,   //the message
							(void *)DrinkType::Whisky);
}

void QuenchThirst::Execute(Miner* pMiner)
{
}


void QuenchThirst::Exit(Miner* pMiner)
{
}


bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
	switch (msg.Msg)
	{
		case Msg_DrinkReady:
		{
			pMiner->BuyWhiskey();
			pMiner->DrinkWhiskey();

			{
				Output output;

				cout << "\nMessage received by " << GetNameOfEntity(pMiner->ID()) <<
					" at time: " << Clock->GetCurrentTime();

				output.ChangeEntity(pMiner->ID());
				
				cout << "\n" << GetNameOfEntity(pMiner->ID())
					<< ": That's mighty fine sippin' liquer";
			}

			pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());

			return true;
		}

		case Msg_BringItOn:
		{
			{
				Output output;

				cout << "\nMessage received by " << GetNameOfEntity(pMiner->ID()) <<
					" at time: " << Clock->GetCurrentTime();
			}

			if (RandBool()) {
				pMiner->GetFSM()->ChangeState(Fight::Instance());

				Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
					pMiner->ID(),
					msg.Sender,
					Msg_ImOn,
					NO_ADDITIONAL_INFO);			
			}

			return true;
		}

	}//end switch

	return false;
}

//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
	static EatStew instance;

	return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
	Output output(pMiner->ID());

	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Smells Reaaal goood Elsa!";
}

void EatStew::Execute(Miner* pMiner)
{
	{
		Output output(pMiner->ID());

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Tastes real good too!";
	}
	pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{
	Output output(pMiner->ID());

	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Thankya li'lle lady. Ah better get back to whatever ah wuz doin'";
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}


//------------------------------------------------------------------------Fight

Fight* Fight::Instance()
{
	static Fight instance;

	return &instance;
}

void Fight::Enter(Miner* pMiner)
{
	Output output(pMiner->ID());

	cout << "\n" << GetNameOfEntity(pMiner->ID())
	     << ": <INSERT FUNNY SENTENCE HERE>";
}

void Fight::Execute(Miner* pMiner)
{
}


void Fight::Exit(Miner* pMiner)
{
}

bool Fight::OnMessage(Miner* pMiner, const Telegram& msg)
{
	switch (msg.Msg)
	{
		case Msg_ChooseMove:
		{
			{
				Output output;

				cout << "\nMessage received by " << GetNameOfEntity(pMiner->ID()) <<
					" at time: " << Clock->GetCurrentTime();
			}

			int move = RandInt(FightMove::Rock, FightMove::Cisors);

			{
				Output output(pMiner->ID());

				cout << "\n" << GetNameOfEntity(pMiner->ID())
					 << ": *Plays " << FightMoveToStr(move) << "*";
			}

			Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
				pMiner->ID(),
				msg.Sender,
				Msg_ChooseMove,
				(void *) move);

			return true;
		}

		case Msg_Win:
		{
			{
				Output output;

				cout << "\nMessage received by " << GetNameOfEntity(pMiner->ID()) <<
					" at time: " << Clock->GetCurrentTime();

				output.ChangeEntity(pMiner->ID());

				cout << "\n" << GetNameOfEntity(pMiner->ID())
					 << ": The taste of victory!";
			}

			pMiner->DrinkWhiskey();

			pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());

			return true;
		}

		case Msg_Lose:
		{
			{
				Output output;

				cout << "\nMessage received by " << GetNameOfEntity(pMiner->ID()) <<
					" at time: " << Clock->GetCurrentTime();

				output.ChangeEntity(pMiner->ID());

				cout << "\n" << GetNameOfEntity(pMiner->ID())
					<< ": " << GetNameOfEntity(ent_Elsa)
					<< " ain't goin' to be happy 'bout that... Better go get some money!";
			}

			pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());

			return true;
		}

	}//end switch

	return false;
}

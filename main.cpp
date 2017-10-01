#include <fstream>
#include <time.h>

#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "Drunkard.h"
#include "Barman.h"

#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"

#include "Display.h"
#include <thread>

#define RUN_COUNT 30
#define SLEEP_TIME 800
#define FPS 60

using namespace std;
std::ofstream os;

int main()
{
	//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
	os.open("output.txt");
#endif
	//seed random number generator
	srand((unsigned)time(NULL));

	//create a miner
	Miner* Bob = new Miner(ent_Miner_Bob);

	//create his wife
	MinersWife* Elsa = new MinersWife(ent_Elsa);

	//create a drunkard
	Drunkard* Clay = new Drunkard(ent_Drunkard_Clay);

	//create _the_ barman
	Barman* TheBarman = new Barman(ent_Barman);

	//register them with the entity manager
	EntityMgr->RegisterEntity(Bob);
	EntityMgr->RegisterEntity(Elsa);
	EntityMgr->RegisterEntity(Clay);
	EntityMgr->RegisterEntity(TheBarman);

	thread bob(&Miner::Run, Bob, RUN_COUNT, SLEEP_TIME);
	thread elsa(&MinersWife::Run, Elsa, RUN_COUNT, SLEEP_TIME);
	thread clay(&Drunkard::Run, Clay, RUN_COUNT, SLEEP_TIME);
	thread barman(&Barman::Run, TheBarman, RUN_COUNT, SLEEP_TIME);

	thread messages(&MessageDispatcher::Run, Dispatch, SLEEP_TIME * 0.1);
	thread view(runDisplay, 1000.0f / FPS);

	bob.join();
	elsa.join();
	clay.join();
	barman.join();

	messages.join();
	view.join();

	//tidy up
	delete Bob;
	delete Elsa;
	delete Clay;
	delete TheBarman;

	//wait for a keypress before exiting
	PressAnyKeyToContinue();


	return 0;
}
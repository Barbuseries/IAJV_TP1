#include "BaseGameEntity.h"
#include "EntityManager.h"
#include "EntityNames.h"
#include "Output.h"
#include <cassert>
#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

int BaseGameEntity::m_iNextValidID = 0;



//----------------------------- SetID -----------------------------------------
//
//  this must be called within each constructor to make sure the ID is set
//  correctly. It verifies that the value passed to the method is greater
//  or equal to the next valid ID, before setting the ID and incrementing
//  the next valid ID
//-----------------------------------------------------------------------------
void BaseGameEntity::SetID(int val)
{
  //make sure the val is equal to or greater than the next available ID
  assert ( (val >= m_iNextValidID) && "<BaseGameEntity::SetID>: invalid ID");

  m_ID = val;
    
  m_iNextValidID = m_ID + 1;
}

void BaseGameEntity::Run(int count, int sleepTime) {
	for (int i = 0; i < count; ++i) {
		this->Update();
		Sleep(sleepTime);
	}

	{
		Output output;

		cout << "\nEntity " << GetNameOfEntity(this->ID()) << " has left this world...";
	}

	EntityMgr->RemoveEntity(this);
}
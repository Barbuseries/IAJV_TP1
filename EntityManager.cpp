#include "EntityManager.h"
#include "BaseGameEntity.h"


//--------------------------- Instance ----------------------------------------
//
//   this class is a singleton
//-----------------------------------------------------------------------------
EntityManager* EntityManager::Instance()
{
  static EntityManager instance;

  return &instance;
}

//------------------------- GetEntityFromID -----------------------------------
//-----------------------------------------------------------------------------
BaseGameEntity* EntityManager::GetEntityFromID(int id)const
{
  //find the entity
  EntityMap::const_iterator ent = m_EntityMap.find(id);

  if (ent == m_EntityMap.end()) {
	  return NULL;
  }

  return ent->second;
}

//--------------------------- RemoveEntity ------------------------------------
//-----------------------------------------------------------------------------
void EntityManager::RemoveEntity(BaseGameEntity* pEntity)
{    
  m_EntityMap.erase(m_EntityMap.find(pEntity->ID()));
} 

//---------------------------- RegisterEntity ---------------------------------
//-----------------------------------------------------------------------------
void EntityManager::RegisterEntity(BaseGameEntity* NewEntity)
{
  m_EntityMap.insert(std::make_pair(NewEntity->ID(), NewEntity));
}

std::vector<int> EntityManager::GetAllEntitiesAtLocation(location_type location) {
	std::vector<int> result;

	for (auto it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it) {
		if (it->second->Location() == location) {
			result.push_back(it->first);
		}
	}

	return result;
}
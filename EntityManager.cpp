#include "EntityManager.h"

#include "Map.h"

Entity::Entity() :
	_entityHandle()
{

}

Entity::Entity(entt::registry& Registry/*, Map& ParentMap*/) :
	//_registry(Registry),
	//_parentMap(ParentMap),
	//_id(Registry.create()),
	_entityHandle(Registry, Registry.create())
{

}

entt::handle Entity::Handle()
{
	return _entityHandle;
}

entt::entity EntityManager::CreateEntity()
{
	auto NewID = _entityRegistry.create();
	return NewID;
}

void EntityManager::DestroyEntity(entt::entity ID)
{
	_entityRegistry.destroy(ID);
}
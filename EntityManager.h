#pragma once

#include <entt/entt.hpp>

// Map.h
class Map;

class Entity;
class EntityManager;

class Entity
{
protected:
	//entt::registry& _registry;
	//Map& _parentMap;

	//entt::entity _id;

	entt::handle _entityHandle;
public:
	Entity();
	Entity(entt::registry &Registry);

	entt::handle Handle();

	template<class T> T* GetComponent()
	{
		return _entityHandle.try_get<T>();
		//return _registry.try_get<T>(_id);
	}
};

/// <summary>
/// Responsible for the creation and mangement 
/// </summary>
class EntityManager
{
	//friend class Entity;
private:
	entt::registry _entityRegistry;
public:
	entt::entity CreateEntity();
	void DestroyEntity(entt::entity ID);
};
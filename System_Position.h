#pragma once

#include <entt/entt.hpp>
#include <SDL2/SDL.h>

//#include "Component_Acceleration.h"
#include "Component_Collision.h"
#include "Component_Physics.h"
#include "Component_Position.h"
//#include "Component_Velocity.h"

// Rename to _Physics
class System_Position
{
private:
	/// <summary>
	/// Resolves changes in acceleration.
	/// </summary>
	/// <param name="Phys"></param>
	/// <param name="TimestepSeconds"></param>
	/// <param name="AccX"></param>
	/// <param name="AccY"></param>
	void _ResolveAcceleration(Component_Physics& Phys, float TimestepSeconds, 
		float& AccX,
		float& AccY);

	/// <summary>
	/// Resolves changes in velocity.
	/// </summary>
	/// <param name="Phys"></param>
	void _ResolveVelocity(Component_Physics& Phys, float TimestepSeconds);

	/// <summary>
	/// Resolves changes in position.
	/// </summary>
	/// <param name="Phys"></param>
	void _ResolvePosition(Component_Physics& Phys, Component_Position& Pos, float TimestepSeconds);

public:
	// time_passed = current_ticks - manager->lastticks;
	void Update(entt::registry& Registry, Uint32 TimestepMS);
};
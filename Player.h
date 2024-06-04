#pragma once

#include <entt/entt.hpp>
#include <SDL2/SDL.h>

#include "Camera.h"
#include "EntityManager.h"
#include "TextureAtlas.h"

//#include "Component_Acceleration.h"
#include "Component_Physics.h"
#include "Component_Position.h"
//#include "Component_Velocity.h"


/// <summary>
///	Represents the player, and stores all relevant stats
/// </summary>
class Player : public Entity
{
private:
	int _boundingBoxWidth = 0, _boundingBoxHeight = 0;
	Component_Position* _position = nullptr;
	//Camera _camera;

public:
	Player(entt::registry &Registry);

	SDL_Point GetPosition() const;
	SDL_FPoint GetPositionF() const;

	void SetAcceleration(float X, float Y);

	//void UpdateCamera();
};
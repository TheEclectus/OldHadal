#pragma once

#include <entt/entt.hpp>

#include "Camera.h"

// Game.h
class Game;
// Player.h
class Player;

/// <summary>
/// Interface between Game and Player.
/// This is where user input should be translate to in-game player action
/// </summary>
class PlayerController
{
private:
	// In future, maybe attach to an entity?
	Player* _attachedPlayer;
	Camera _camera;

public:
	PlayerController(Game& ParentGame);

	void Attach(Player* ControlledPlayer);
	void Update();
};
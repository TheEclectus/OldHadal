#pragma once

#include <string>

#include <SDL2/SDL.h>

// Game.h
class Game;

class ADebugTool
{
	friend class Game;

private:
	Game& _game;
	std::string _name = "";
	std::string _desc = "";
	bool _bIsActive = false;

protected:
	ADebugTool(const std::string& Name, const std::string& Desc, Game &ParentGame);

	void Activate();
	void Deactivate();
	Game& ParentGame();

public:
	const std::string& GetToolName() const;
	const std::string& GetToolDesc() const;
	bool IsActive() const;
	operator bool() const;

	virtual void OnActivate();
	virtual void OnDeactivate();

	// Called once in the logic loop, update mouse pos etc. with this
	virtual void Logic();

	// Return true if the input should be stopped from being passed to the game
	virtual bool ProcessInput(const SDL_Event& Event) = 0;

	// Draw in designated part of the screen on UI layer
	virtual void Render(SDL_Renderer* Renderer, SDL_Rect DrawArea) = 0;

	// Draw over the game map in lower res
	virtual void RenderGame(SDL_Renderer* Renderer, SDL_Rect DrawArea);
};
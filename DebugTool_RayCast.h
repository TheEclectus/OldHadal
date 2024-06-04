#pragma once

#include <vector>

#include "ADebugTool.h"
#include "Line.h"
#include "Raycast.h"

class DebugTool_RayCast : public ADebugTool
{
private:
	int _ModeIndex = 0;

	bool _bPenetrating = true;
	bool _bPointMode = true;

	std::vector<SDL_Point> _IntersectionPoints = {};
	std::vector<Line> _IntersectedLines = {};


	SDL_Point _LastMousePos = {-1, -1};

public:
	DebugTool_RayCast(Game& ParentGame);
	virtual void OnDeactivate() override;
	virtual void Logic() override;
	virtual bool ProcessInput(const SDL_Event& Event) override;
	// TODO: Supplant with draing UIScene or UIFrame or something
	virtual void Render(SDL_Renderer* Renderer, SDL_Rect DrawArea) override;
	virtual void RenderGame(SDL_Renderer* Renderer, SDL_Rect DrawArea) override;
};
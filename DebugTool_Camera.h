#pragma once

#include "ADebugTool.h"

class DebugTool_Camera : public ADebugTool
{
private:
	//bool _bDrawCameraRegion = false;

public:
	DebugTool_Camera(Game& ParentGame);
	virtual bool ProcessInput(const SDL_Event& Event) override;
	virtual void Render(SDL_Renderer* Renderer, SDL_Rect DrawArea) override;
	virtual void RenderGame(SDL_Renderer* Renderer, SDL_Rect DrawArea) override;
};
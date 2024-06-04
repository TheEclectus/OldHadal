#pragma once

#include "ADebugTool.h"



class DebugTool_Lighting : public ADebugTool
{
private:

public:
	DebugTool_Lighting(Game& ParentGame);
	virtual bool ProcessInput(const SDL_Event& Event) override;
	virtual void Render(SDL_Renderer* Renderer, SDL_Rect DrawArea) override;
};
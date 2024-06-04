#include "DebugTool_Lighting.h"

static const char* Name = "Depth Shading";
static const char* Desc = "+/- to increment depth shading by 100\n +ctrl, by 10\n +shift, by 1000";

DebugTool_Lighting::DebugTool_Lighting(Game& ParentGame) :
	ADebugTool(Name, Desc, ParentGame)
{

}

bool DebugTool_Lighting::ProcessInput(const SDL_Event& Event)
{
	return false;
}

void DebugTool_Lighting::Render(SDL_Renderer* Renderer, SDL_Rect DrawArea)
{

}
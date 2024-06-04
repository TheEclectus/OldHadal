#include "DebugTool_Placeholder.h"

#include "SDL_FontCache.h"

#include "FontAtlas.h"

static const char* Name = "Placeholder";
static const char* Desc = "Really this is just here to fill up space to test the tool cycling.\nBeep beep.\n( ' v ')\n";

DebugTool_Placeholder::DebugTool_Placeholder(Game& ParentGame) :
	ADebugTool(Name, Desc, ParentGame)
{
	
}

bool DebugTool_Placeholder::ProcessInput(const SDL_Event& Event)
{
	return false;
}

void DebugTool_Placeholder::Render(SDL_Renderer* Renderer, SDL_Rect DrawArea)
{
	static auto Font = FontAtlas::FindFont("default_bold");

	SDL_SetRenderDrawColor(Renderer, 200, 0, 0, 200);
	SDL_RenderDrawRect(Renderer, &DrawArea);

	FC_DrawBox(Font, Renderer, DrawArea, "And here's where the debug tool is allowed to render to. It's not exactly cramped real estate, but it's not boundless either.");
}
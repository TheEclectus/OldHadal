#include "DebugTool_Camera.h"

#include "Game.h"
#include "Line.h"

static const char* Name = "Camera";
static const char* Desc = "-";

DebugTool_Camera::DebugTool_Camera(Game& ParentGame) :
	ADebugTool(Name, Desc, ParentGame)
{

}

bool DebugTool_Camera::ProcessInput(const SDL_Event& Event)
{
	/*if (Event.type == SDL_EventType::SDL_KEYDOWN)
	{
		switch (Event.key.keysym.sym)
		{
		case SDLK_p:
			_bPenetrating = !_bPenetrating;
			return true;
			break;
		case SDLK_m:
			_ModeIndex = _ModeIndex == (static_cast<int>(Mode::_Count) - 1) ? 0 : (_ModeIndex + 1);
			return true;
			break;
		case SDLK_l:
			_bPointMode = !_bPointMode;
			return true;
			break;
		}
	}*/

	return false;
}

void DebugTool_Camera::Render(SDL_Renderer* Renderer, SDL_Rect DrawArea)
{

}

void DebugTool_Camera::RenderGame(SDL_Renderer* Renderer, SDL_Rect DrawArea)
{
	SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);

	auto& GameCam = this->ParentGame().GetMap()->GetCamera();

	SDL_Point PlayerPos = *this->ParentGame().GetMap()->GetControlledPlayer().GetComponent<Component_Position>();
	SDL_Point PlayerScreenPos = GameCam.WorldToScreen(PlayerPos);

	SDL_RenderDrawLine(Renderer, DrawArea.x, DrawArea.y, DrawArea.x + DrawArea.w - 1, DrawArea.y + DrawArea.h - 1);
	SDL_RenderDrawLine(Renderer, DrawArea.x, DrawArea.y + DrawArea.h - 1, DrawArea.x + DrawArea.w - 1, DrawArea.y);

	SDL_SetRenderDrawColor(Renderer, 0, 255, 0, 255);
	SDL_RenderDrawPoint(Renderer, PlayerScreenPos.x, PlayerScreenPos.y);
}
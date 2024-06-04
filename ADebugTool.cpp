#include "ADebugTool.h"

ADebugTool::ADebugTool(const std::string& Name, const std::string& Desc, Game& ParentGame) :
	_name(Name),
	_desc(Desc),
	_game(ParentGame)
{

}

void ADebugTool::Activate()
{
	_bIsActive = true;
	this->OnActivate();
}

void ADebugTool::Deactivate()
{
	_bIsActive = false;
	this->OnDeactivate();
}

Game& ADebugTool::ParentGame()
{
	return _game;
}

const std::string& ADebugTool::GetToolName() const
{
	return _name;
}

const std::string& ADebugTool::GetToolDesc() const
{
	return _desc;
}

bool ADebugTool::IsActive() const
{
	return _bIsActive;
}

ADebugTool::operator bool() const
{
	return this->IsActive();
}

void ADebugTool::OnActivate()
{

}

void ADebugTool::OnDeactivate()
{

}

void ADebugTool::Logic()
{

}

void ADebugTool::RenderGame(SDL_Renderer* Renderer, SDL_Rect DrawArea)
{

}
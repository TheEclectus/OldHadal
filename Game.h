#pragma once

#include <typeindex>
#include <vector>

#include <SDL2/SDL.h>

#include "SDL_FontCache.h"

#include "ADebugTool.h"

#include "Camera.h"
#include "Map.h"
#include "RenderTarget.h"
#include "Window.h"	

#include "Widget.h"

//#include "UIScene.h"

/// <summary>
/// For all intents and purposes, should represent the local instance of a game.
/// </summary>
class Game
{
private:
	bool _bExitRequested = false;
	int _Depth = 0;

	bool _bShowMap = false;
	int _mapOffset = 0;

	UIScene* _scene = nullptr;

	//Camera _camera;
	Map* _map = nullptr;

	//UIScene _sceneDebug;

	Window* _Window;
	int WindowWidth = 640, WindowHeight = 480;

	// (2x) white pixels opaque, transparent pixels transparent
	RenderTarget* _shadowTarget;
	// (2x) ADD'd onto shadow target.
	RenderTarget* _fullbrightTarget;
	// (2x) Foreground drawing, above shadow layer.
	RenderTarget* _uiTarget;
	// Window-sized target for debug UI
	RenderTarget* _debugUITarget;
	// Window-sized target for final drawing
	RenderTarget* _screenTarget;

	SDL_BlendMode WhiteToAlpha;

	bool _bDebugToolMode = false;
	std::vector<ADebugTool*> _dbgTools;
	std::unordered_map<std::type_index, ADebugTool*> _dbgToolRegistry;
	size_t _dbgToolIndex = 0u;

	ADebugTool* _currentDebugTool = nullptr;

	//DebugTool_Lighting* _dbgTool_Lighting = nullptr;
	//DebugTool_Placeholder* _dbgTool_Placeholder = nullptr;

	template<class T>
	void _RegDbgTool();

	template<class T>
	ADebugTool* _DbgTool();

public:
	Game(Window* Win);
	//Game(SDL_Window* Win);

	void CreateMap();

	void Input();
	void Logic();
	void Draw(SDL_Renderer* Ren/*, const SDL_Point& CameraPosCenter*/);

	bool HasExitBeenRequested() const;

	Window& GetWindow();
	Map* GetMap();
};
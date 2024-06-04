#include "DebugTool_RayCast.h"

#include <SDL2/SDL2_gfxPrimitives.h>

#include "FontAtlas.h"
#include "Game.h"
#include "Input.h"
#include "Polygon.h"
#include "Util.h"

// Mode 1 - Screenspace (lines)
static const Line ScreenspaceLines[4] = {
	{{ 80, 20 }, { 180, 20 }},	// Line 1
	{{ 80, 45 }, { 190, 25}},	// Line 2
	{{ 80, 60 }, { 190, 28}},	// Line 3
	{{ 35, 180 }, { 180, 200}},	// Line 4
};

// Mode 2 - Screenspace (polygons)
// 4-point polygon
static const Line Polygon1[4] = {
	{{90, 25}, {140, 20}},
	{{140, 20}, {130, 60}},
	{{130, 60}, {80, 40}},
	{{80, 40}, {90, 25}}
};

static const std::vector<SDL_Point> Pts = { {90, 25}, {140, 20}, {130, 60}, {80, 40}, {90, 25} };

static const Polygon Polygon2 = { Pts };

static const char* Name = "Raycasting";
static const char* Desc = "LMB - Raycast from player to mouse pos\nm - Cycle raycast modes\np - Toggle penetration\nl - Toggle line highlight";

enum class Mode : int
{
	ScreenspaceLines,
	ScreenspacePolygons,
	Terrain,

	_Count
};
static const char* ModeNames[static_cast<int>(Mode::_Count)] = {
	"Screenspace (lines)",
	"Screenspace (polys)",
	"Terrain"
};

DebugTool_RayCast::DebugTool_RayCast(Game& ParentGame) :
	ADebugTool(Name, Desc, ParentGame)
{
	
}

void DebugTool_RayCast::OnDeactivate()
{
	_IntersectionPoints.clear();
	_LastMousePos = { -1, -1 };
}

void DebugTool_RayCast::Logic()
{
	if (_IntersectionPoints.size() > 0)
		_IntersectionPoints.clear();
	if (_IntersectedLines.size() > 0)
		_IntersectedLines.clear();

	Uint32 MouseState = SDL_GetMouseState(&_LastMousePos.x, &_LastMousePos.y);
	bool bLMBPressed_ = MouseState & SDL_BUTTON(SDL_BUTTON_LEFT);
	
	bool bLMBPressed = Input::IsMouseDown(SDL_BUTTON_LEFT);
	_LastMousePos = Input::MousePosition();

	if (!bLMBPressed)
		_LastMousePos = { -1, -1 };
	else
	{
		//_LastMousePos /= 2;
		_LastMousePos = _LastMousePos / 2;
		/*_LastMousePos.x /= 2;
		_LastMousePos.y /= 2;*/
		//_IntersectionPoints.clear();

		if (_bPenetrating)
		{
			if (_ModeIndex == static_cast<int>(Mode::ScreenspaceLines))
			{
				// Update that list of intersections, boys
				for (auto CurScreenspaceLine : ScreenspaceLines)
				{
					SDL_Point Intersection = { 0, 0 };

					Line Ray = { {160, 120}, _LastMousePos };
					//Line Int = { CurScreenspaceLine[0], CurScreenspaceLine[1] };

					bool bIntersects = Raycast::Intersection(Ray, CurScreenspaceLine, &Intersection);
					//bool bIntersects = Util::DoLineSegmentsIntersect({ 160, 120 }, _LastMousePos, CurScreenspaceLine[0], CurScreenspaceLine[1], &Intersection);
					if (bIntersects)
					{
						_IntersectionPoints.push_back(Intersection);
						_IntersectedLines.push_back(CurScreenspaceLine);
					}
				}
			}
			else if (_ModeIndex == static_cast<int>(Mode::ScreenspacePolygons))
			{
				// Update that list of intersections, boys
				/*for (auto CurScreenspaceLine : Polygon1)
				{
					SDL_Point Intersection = { 0, 0 };

					Line Ray = { {160, 120}, _LastMousePos };
					bool bIntersects = Raycast::Intersection(Ray, CurScreenspaceLine, &Intersection);

					if (bIntersects)
					{
						_IntersectionPoints.push_back(Intersection);
						_IntersectedLines.push_back(CurScreenspaceLine);
					}
				}*/

				Line Ray = { {160, 120}, _LastMousePos };
				Raycast::Context Ctx(Ray, Polygon2);
				while (Ctx.FindNext())
				{
					_IntersectionPoints.push_back(Ctx.CurrentIntersection());
					_IntersectedLines.push_back(Ctx.GetCurrentLine());
				}
			}
			else if (_ModeIndex == static_cast<int>(Mode::Terrain))
			{
				auto& Game = ParentGame();

				auto Map = Game.GetMap();
				auto Player = Map->GetControlledPlayer();

				Camera& PlayerCam = Game.GetMap()->GetCamera();

				SDL_Point WorldMousePos = PlayerCam.ScreenToWorld(_LastMousePos);

				for (auto CurTB : Map->GetTerrainBlocks())
				{
					Line Ray{ *Player.GetComponent<Component_Position>(), WorldMousePos };
					Raycast::Context Ctx(Ray, CurTB->Shape());
					while (Ctx.FindNext())
					{
						SDL_Point ScreenPt = PlayerCam.WorldToScreen(Ctx.CurrentIntersection());
						_IntersectionPoints.push_back(ScreenPt);

						Line CurLine = Ctx.GetCurrentLine();
						Line ScreenLine = { PlayerCam.WorldToScreen(CurLine.Pt1), PlayerCam.WorldToScreen(CurLine.Pt2) };
						_IntersectedLines.push_back(ScreenLine);
					}
				}
			}
		}
		else
		{
			SDL_Point ClosestPoint = { -1, -1 };
			Line ClosestLine;
			// Update that list of intersections, boys
			if (_ModeIndex == static_cast<int>(Mode::ScreenspaceLines))
			{
				for (auto CurScreenspaceLine : ScreenspaceLines)
				{
					SDL_Point Intersection = { 0, 0 };
					Line Ray = { {160, 120}, _LastMousePos };
					//bool bIntersects = Util::DoLineSegmentsIntersect({ 160, 120 }, _LastMousePos, CurScreenspaceLine[0], CurScreenspaceLine[1], &Intersection);
					bool bIntersects = Raycast::Intersection(Ray, CurScreenspaceLine, &Intersection);
					
					if (bIntersects)
					{
						if (ClosestPoint.x == -1)
						{
							ClosestPoint = Intersection;
							ClosestLine = CurScreenspaceLine;
						}
							
						else
						{
							float ClosestPtLen = Util::GetLineLength({ 160, 120 }, ClosestPoint);
							float IntersPtLen = Util::GetLineLength({ 160, 120 }, Intersection);

							if (IntersPtLen < ClosestPtLen)
							{
								ClosestPoint = Intersection;
								ClosestLine = CurScreenspaceLine;
							}
						}
					}
				}
			}
			else if (_ModeIndex == static_cast<int>(Mode::ScreenspacePolygons))
			{
				//for (auto CurScreenspaceLine : Polygon1)
				//{
				//	SDL_Point Intersection = { 0, 0 };
				//	Line Ray = { {160, 120}, _LastMousePos };
				//	//bool bIntersects = Util::DoLineSegmentsIntersect({ 160, 120 }, _LastMousePos, CurScreenspaceLine.Pt1, CurScreenspaceLine.Pt2, &Intersection);
				//	bool bIntersects = Raycast::Intersection(Ray, CurScreenspaceLine, &Intersection);
				//	if (bIntersects)
				//	{
				//		if (ClosestPoint.x == -1)
				//		{
				//			ClosestPoint = Intersection;
				//			ClosestLine = CurScreenspaceLine;
				//		}
				//		else
				//		{
				//			float ClosestPtLen = Util::GetLineLength({ 160, 120 }, ClosestPoint);
				//			float IntersPtLen = Util::GetLineLength({ 160, 120 }, Intersection);

				//			if (IntersPtLen < ClosestPtLen)
				//			{
				//				ClosestPoint = Intersection;
				//				ClosestLine = CurScreenspaceLine;
				//			}
				//		}
				//	}
				//}

				SDL_Point Intersection = { 0, 0 };
				Line InterLine;
				Line Ray = { {160, 120}, _LastMousePos };
				if (Raycast::Intersection(Ray, Polygon2, &Intersection, &InterLine))
				{
					_IntersectionPoints.push_back(Intersection);
					_IntersectedLines.push_back(InterLine);
				}
			}
			else if (_ModeIndex == static_cast<int>(Mode::Terrain))
			{
				SDL_Point Intersection = { 0, 0 };
				Line InterLine;
				/*Line Ray = { {160, 120}, _LastMousePos };
				if (Raycast::Intersection(Ray, Polygon2, &Intersection, &InterLine))
				{
					_IntersectionPoints.push_back(Intersection);
					_IntersectedLines.push_back(InterLine);
				}*/

				auto& Game = ParentGame();

				auto Map = Game.GetMap();
				auto Player = Map->GetControlledPlayer();

				Camera& PlayerCam = ParentGame().GetMap()->GetCamera();

				SDL_Point WorldMousePos = PlayerCam.ScreenToWorld(_LastMousePos);

				for (auto CurTB : Map->GetTerrainBlocks())
				{
					SDL_Point PlayerPos = *Player.GetComponent<Component_Position>();
					Line Ray{ PlayerPos, WorldMousePos };
					if (Raycast::Intersection(Ray, CurTB->Shape(), &Intersection, &InterLine))
					{
						float ClosestPtLen = Util::GetLineLength(PlayerPos, Intersection);
						float IntersPtLen = Util::GetLineLength(PlayerPos, ClosestPoint);
						
						if (ClosestPoint.x == -1 || IntersPtLen < ClosestPtLen)
						{
							SDL_Point ScreenPt = PlayerCam.WorldToScreen(Intersection);
							ClosestPoint = ScreenPt;

							Line CurLine = InterLine;
							Line ScreenLine = { PlayerCam.WorldToScreen(CurLine.Pt1), PlayerCam.WorldToScreen(CurLine.Pt2) };
							ClosestLine = ScreenLine;
						}
					}
				}
			}

			if (ClosestPoint.x != -1)
			{
				_IntersectionPoints.push_back(ClosestPoint);
				_IntersectedLines.push_back(ClosestLine);
			}
		}
	}
}

bool DebugTool_RayCast::ProcessInput(const SDL_Event& Event)
{
	bool bShift = (Event.key.keysym.mod & (KMOD_SHIFT));

	if (Event.type == SDL_EventType::SDL_KEYDOWN)
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
	}

	return false;
}

void DebugTool_RayCast::Render(SDL_Renderer* Renderer, SDL_Rect DrawArea)
{
	static auto Font = FontAtlas::FindFont("default_bold");
	
	//auto TopBox = FC_DrawBox(Font, Renderer, DrawArea, "Mode: %s\nPenetrate: %s [0]\nIntersection(s):", _bPenetrating ? "true" : "false", ModeNames[_ModeIndex]);
	auto TopBox = FC_DrawColumn(Font, Renderer, DrawArea.x, DrawArea.y, DrawArea.w, "Mode: %s\nLine highlight: %s\nPenetrate: %s [0]\nIntersection(s):", ModeNames[_ModeIndex], _bPointMode ? "true" : "false", _bPenetrating ? "true" : "false");
	int OffsetFromTop = TopBox.h;
	if (_IntersectionPoints.size() == 0)
	{
		SDL_Rect Box = DrawArea;
		Box.y += OffsetFromTop;
		Box.h -= OffsetFromTop;

		OffsetFromTop += FC_DrawColumn(Font, Renderer, Box.x, Box.y, Box.w, "\t-").h;
	}
	else
	{
		for (auto& CurIntersection : _IntersectionPoints)
		{
			SDL_Rect Box = DrawArea;
			Box.y += OffsetFromTop;
			Box.h -= OffsetFromTop;

			SDL_Point Inter = CurIntersection;
			if (_ModeIndex == static_cast<int>(Mode::Terrain))
				Inter = ParentGame().GetMap()->GetCamera().ScreenToWorld(Inter);

			OffsetFromTop += FC_DrawColumn(Font, Renderer, Box.x, Box.y, Box.w, "\t[%d, %d]", Inter.x, Inter.y).h;
		}
	}
}

void DebugTool_RayCast::RenderGame(SDL_Renderer* Renderer, SDL_Rect DrawArea)
{
	static auto Font = FontAtlas::FindFont("default_bold");

	//FC_DrawBox(Font, Renderer, { 0, 0, 200, 200 }, "BEE EE E E E EE E E EE E E EE\n EEEE E E EEE EP");
	
	/*int x = 0, y = 0;
	Uint32 state = SDL_GetMouseState(&x, &y);
	x /= 2;
	y /= 2;*/

	auto MousePos = Input::MousePosition();
	MousePos = MousePos / 2;
	int x = MousePos.x;
	int y = MousePos.y;

	// Mode 1 - Screenspace (lines)
	if (_ModeIndex == static_cast<int>(Mode::ScreenspaceLines))
	{
		for (auto CurScreenspaceLine : ScreenspaceLines)
		{
			lineRGBA(Renderer,
				CurScreenspaceLine.Pt1.x, CurScreenspaceLine.Pt1.y,
				CurScreenspaceLine.Pt2.x, CurScreenspaceLine.Pt2.y,

				0, 0, 255, 255
			);
		}
	}
	else if (_ModeIndex == static_cast<int>(Mode::ScreenspacePolygons))
	{
		/*for (int i = 0; i < 4; i++)
		{
			const SDL_Point& Pt1 = Polygon1[i].Pt1;
			const SDL_Point& Pt2 = Polygon1[i].Pt2;

			lineRGBA(Renderer,
				Pt1.x, Pt1.y,
				Pt2.x, Pt2.y,

				0, 0, 255, 255
			);
		}*/

		polygonRGBAPts(Renderer, Polygon2.GetPoints(), Polygon2.GetNumPoints(), 0, 0, 255, 255);
	}

	//if (state & SDL_BUTTON(SDL_BUTTON_LEFT))
	if (Input::IsMouseDown(SDL_BUTTON_LEFT))
	{
		// Raytrace line
		if (_bPenetrating || _IntersectionPoints.size() == 0)
			lineRGBA(Renderer, 160, 120, x, y, 0, 255, 0, 255);
		else
			lineRGBA(Renderer, 160, 120, _IntersectionPoints[0].x, _IntersectionPoints[0].y, 0, 255, 0, 255);

		// Highlight intersections
		if (!_bPointMode)
		{
			for (auto& CurLine : _IntersectedLines)
			{
				lineRGBA(Renderer, CurLine.Pt1.x, CurLine.Pt1.y, CurLine.Pt2.x, CurLine.Pt2.y, 255, 0, 0, 255);
				//pixelRGBA(Renderer, CurLine.x, CurLine.y, 255, 0, 0, 255);
			}
		}

		for (auto& CurIntersection : _IntersectionPoints)
		{
			pixelRGBA(Renderer, CurIntersection.x, CurIntersection.y, 255, 255, 0, 255);
		}

		//lineRGBA(Renderer, 0, 200, 240, 200, 255, 0, 0, 255);	
	}
}
#include "Camera.h"

#include "Util.h"

Camera::Camera(int Width, int Height, SDL_Point Position) :
	_Viewport({Position.x, Position.y, Width, Height})
{
	
}

void Camera::SetPosition(SDL_Point Position)
{
	_Viewport.x = Position.x;
	_Viewport.y = Position.y;
}

void Camera::SetSize(int Width, int Height)
{
	_Viewport.w = Width;
	_Viewport.h = Height;
}

SDL_Point Camera::Position() const
{
	SDL_Point Pos = {
		_Viewport.x,
		_Viewport.y
	};
	return Pos;
}

SDL_Rect Camera::Viewport() const
{
	return _Viewport;
}

SDL_Point Camera::TopLeft() const
{
	SDL_Point TL = {
		_Viewport.x - (_Viewport.w / 2),	// x
		_Viewport.y - (_Viewport.h / 2)		// y
	};
	return TL;
}

SDL_Point Camera::BottomRight() const
{
	SDL_Point BR = {
		_Viewport.x + (_Viewport.w / 2),	// x
		_Viewport.y + (_Viewport.h / 2)		// y
	};
	return BR;
}

SDL_Rect Camera::WorldRect() const
{
	SDL_Rect RectWorld = {
		_Viewport.x - (_Viewport.w / 2),	// x
		_Viewport.y - (_Viewport.h / 2),	// y
		_Viewport.w,						// w
		_Viewport.h							// h
	};
	return RectWorld;
}

SDL_Point Camera::ScreenToWorld(const SDL_Point ScreenSpacePt) const
{
	return TopLeft() + ScreenSpacePt;
}

SDL_FPoint Camera::ScreenToWorld(const SDL_FPoint ScreenSpaceFPt) const
{
	SDL_Point TL = TopLeft();
	SDL_FPoint TLf = {
		static_cast<float>(ceilf(TL.x)),	// x
		static_cast<float>(ceilf(TL.y))		// y
	};

	return TLf + ScreenSpaceFPt;
}

SDL_Rect Camera::ScreenToWorld(const SDL_Rect& ScreenSpaceRect) const
{
	SDL_Point RectTL = { ScreenSpaceRect.x, ScreenSpaceRect.y };
	SDL_Point WorldTL = TopLeft() + RectTL;
	SDL_Rect WorldRect = {
		WorldTL.x,
		WorldTL.y,
		ScreenSpaceRect.w,
		ScreenSpaceRect.h
	};
	return WorldRect;
}

SDL_Point Camera::WorldToScreen(const SDL_Point WorldSpacePt) const
{
	return WorldSpacePt - TopLeft();
}
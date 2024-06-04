#pragma once

#include <SDL2/SDL.h>

// TODO: Account for world wrapping if that's an idea that's here to stay
// 65,535 <-> 0

/// <summary>
/// Represents a camera within worldspace. Managed on behalf of a Player by Map.
/// </summary>
class Camera
{
private:
	// x and y represent center of the viewport in worldspace
	SDL_Rect _Viewport;

public:
	Camera(int Width = 0, int Height = 0, SDL_Point Position = { 0, 0 });

	void SetPosition(SDL_Point Position);
	void SetSize(int Width, int Height);

	SDL_Point Position() const;
	SDL_Rect Viewport() const;

	/// <returns>Top-left coordinate of camera in world space.</returns>
	SDL_Point TopLeft() const;
	/// <returns>Bottom-right coordinate of camera in world space.</returns>
	SDL_Point BottomRight() const;
	/// <returns>Visible region of the map in world space.</returns>
	SDL_Rect WorldRect() const;

	SDL_Point ScreenToWorld(const SDL_Point ScreenSpacePt) const;
	SDL_FPoint ScreenToWorld(const SDL_FPoint ScreenSpaceFPt) const;
	SDL_Rect ScreenToWorld(const SDL_Rect& ScreenSpaceRect) const;
	SDL_FRect ScreenToWorld(const SDL_FRect& ScreenSpaceFRect) const;

	SDL_Point WorldToScreen(const SDL_Point WorldSpacePt) const;
	SDL_FPoint WorldToScreen(const SDL_FPoint WorldSpaceFPt) const;
	SDL_Rect WorldToScreen(const SDL_Rect& WorldSpaceRect) const;
	SDL_FRect WorldToScreen(const SDL_FRect& WorldSpaceFRect) const;
};
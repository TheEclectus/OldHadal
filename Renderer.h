#pragma once

#include <vector>

#include <SDL2/SDL.h>

/// <summary>
/// Meant to be an OOP replacement to and wrapper around SDL_Renderer
/// </summary>
class Renderer
{
private:
	const SDL_Color _DEFAULT_COLOR = { 0, 0, 0, 255 };

	SDL_Renderer* _Renderer;
	// TODO: update whenever the window resizes or dimensions change or anything.
	// Also the default viewport and clipping rect.
	SDL_Rect _Dimensions = { 0, 0, 0, 0 };
	std::vector<SDL_Color> _ColorStack;		// empty means rgba(0,0,0,255)
	std::vector<SDL_Rect> _ViewportStack;		// empty means rgba(0,0,0,255)
	std::vector<SDL_Rect> _ClippingStack;	// -1,-1,-1,-1 or empty means nullptr equivalent

public:
	// TODO: Perhaps have Renderer make the call to SDL_CreateRenderer
	Renderer(SDL_Renderer* Renderer);

	// ========= Drawing =========
	void Clear();
	void ClearColor(const SDL_Color& Color);

	void DrawLine(const SDL_Point& Pt1, const SDL_Point& Pt2);
	void DrawLineColor(const SDL_Point& Pt1, const SDL_Point& Pt2, const SDL_Color& Color);

	// ========= State Modifiers =========

	// SDL_SetRenderDrawColor
	void PushColor(const SDL_Color& Color);
	void PopColor();
	const SDL_Color& TopColor() const;

	// SDL_RenderSetViewport
	void PushViewport(const SDL_Rect& Viewport);
	void PopViewport();
	const SDL_Rect& TopViewport() const;

	// SDL_RenderSetClipRect
	void PushClippingRect(const SDL_Rect& Clip);
	void PopClippingRect();
	const SDL_Rect& TopClippingRect() const;
};

/*
	NOTES ABOUT SDL_Renderer's INTERNAL WORKINGS:
	- SDL_SetRenderDrawColor() is very cheap, assigning 4 integer values according
		to the RGBA values provided.
*/

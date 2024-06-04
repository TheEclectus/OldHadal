#include "Renderer.h"

#include <SDL2/SDL2_gfxPrimitives.h>

Renderer::Renderer(SDL_Renderer* Renderer) :
	_Renderer(Renderer)
{
	SDL_GetRendererOutputSize(_Renderer, &_Dimensions.w, &_Dimensions.h);

	_ColorStack.reserve(32);
	_ClippingStack.reserve(16);
}

void Renderer::Clear()
{
	ClearColor(this->TopColor());
}

void Renderer::ClearColor(const SDL_Color& Color)
{
	SDL_SetRenderDrawColor(_Renderer, Color.r, Color.g, Color.b, Color.a);
	SDL_RenderClear(_Renderer);
}

void Renderer::DrawLine(const SDL_Point& Pt1, const SDL_Point& Pt2)
{
	DrawLineColor(Pt1, Pt2, this->TopColor());
}

void Renderer::DrawLineColor(const SDL_Point& Pt1, const SDL_Point& Pt2, const SDL_Color& Color)
{
	SDL_SetRenderDrawColor(_Renderer, Color.r, Color.g, Color.b, Color.a);

	SDL_RenderDrawLine(_Renderer, Pt1.x, Pt1.y, Pt2.x, Pt2.y);
}

void Renderer::PushColor(const SDL_Color& Color)
{
	_ColorStack.push_back(Color);
}

void Renderer::PopColor()
{
	if (_ColorStack.size() > 0)
		_ColorStack.pop_back();
}

const SDL_Color& Renderer::TopColor() const
{
	if (_ColorStack.size() > 0)
		return _ColorStack.back();
	else
		return _DEFAULT_COLOR;
}

void Renderer::PushViewport(const SDL_Rect& Viewport)
{
	_ViewportStack.push_back(Viewport);
}

void Renderer::PopViewport()
{
	if (_ViewportStack.size() > 0)
		_ViewportStack.pop_back();
}

const SDL_Rect& Renderer::TopViewport() const
{
	if (_ViewportStack.size() > 0)
		return _ViewportStack.back();
	else
		return _Dimensions;
}

void Renderer::PushClippingRect(const SDL_Rect& Clip)
{
	_ClippingStack.push_back(Clip);
}

void Renderer::PopClippingRect()
{
	if (_ClippingStack.size() > 0)
		_ClippingStack.pop_back();
}

const SDL_Rect& Renderer::TopClippingRect() const
{
	if (_ClippingStack.size() > 0)
		return _ClippingStack.back();
	else
		return _Dimensions;
}
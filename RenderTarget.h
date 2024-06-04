#pragma once

#include <SDL2/SDL.h>

class RenderTargetScope;
class RenderTarget;

// Bind()s a RenderTarget on construction and Unbind()s
class RenderTargetScope
{
	friend class RenderTarget;
private:
	RenderTarget& _target;

	RenderTargetScope(RenderTarget& Target);
public:
	RenderTargetScope(RenderTargetScope&& RHS) = default;
	RenderTargetScope(const RenderTargetScope& RHS) = delete; // No copying
	~RenderTargetScope();
};

// Convenience class for wrapping around a render texture
class RenderTarget
{
private:
	SDL_Renderer* _parentRenderer = nullptr;
	// Scaling method might be set on creation? Bear in mind for shadow layer
	// NOTE: SDL_CreateTexture() seems to default to _NONE for blendmode
	SDL_Texture* _renderTexture = nullptr;

public:
	// Render-to-texture
	RenderTarget(SDL_Window* Window, int Width, int Height);

	RenderTargetScope ScopeGuard();

	void Bind();
	void Unbind();

	SDL_Texture* Texture() const;
	operator SDL_Texture* ();
};
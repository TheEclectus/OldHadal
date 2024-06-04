#include "RenderTarget.h"

RenderTargetScope::RenderTargetScope(RenderTarget& Target) :
	_target(Target)
{
	_target.Bind();
}

RenderTargetScope::~RenderTargetScope()
{
	_target.Unbind();
}

RenderTarget::RenderTarget(SDL_Window* Window, int Width, int Height)
{
	SDL_Renderer* WindowRen = SDL_GetRenderer(Window);
	SDL_RendererInfo WindowRenInfo;
	SDL_GetRendererInfo(WindowRen, &WindowRenInfo);
	if (!(WindowRenInfo.flags & SDL_RendererFlags::SDL_RENDERER_TARGETTEXTURE))
	{
		// Rendering to a texture isn't supported.
		return;
	}

	_parentRenderer = WindowRen;
	_renderTexture = SDL_CreateTexture(WindowRen,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		Width,
		Height
	);
}

RenderTargetScope RenderTarget::ScopeGuard()
{
	return RenderTargetScope(*this);
}

void RenderTarget::Bind()
{
	SDL_SetRenderTarget(_parentRenderer, _renderTexture);
}

void RenderTarget::Unbind()
{
	SDL_SetRenderTarget(_parentRenderer, nullptr);
}

SDL_Texture* RenderTarget::Texture() const
{
	return _renderTexture;
}

RenderTarget::operator SDL_Texture* ()
{
	return Texture();
}
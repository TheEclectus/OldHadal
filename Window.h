#pragma once

#include <string_view>

#include <SDL2/SDL.h>

// TODO: any event that affects the window should update the relevant local data.
// TODO: maybe make RenderTargets that are dependent on the window size update when it resizes?
class Window
{
private:
	SDL_Window* _Window = nullptr;
	SDL_Renderer* _Renderer = nullptr;

	// Logical size in pixels
	int _Width, _Height;

public:
	Window(int Width, int Height, std::string_view Name = nullptr);
	Window(std::string Name = "");

	void SetSize(int Width, int Height);

	const char* GetTitle() const;

	int GetWidth() const;
	int GetHeight() const;
	// TODO: maybe make a SDL_Point-based (or similar) return for both dimensions?

	// Implicit cast to underlying SDL_Window handle
	operator SDL_Window* ();
	// Implicit cast to default SDL_Renderer context
	operator SDL_Renderer* ();
};
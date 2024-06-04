#include "Window.h"

Window::Window(int Width, int Height, std::string_view Name) :
	_Width(Width),
	_Height(Height)
{
	std::string WindowName = "";

	if (!Name.empty())
		WindowName = Name;

	// TODO: error handling
	_Window = SDL_CreateWindow(WindowName.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		_Width,
		_Height,
		SDL_WINDOW_SHOWN
	);

	_Renderer = SDL_CreateRenderer(_Window, -1, SDL_RENDERER_ACCELERATED);
	//SDL_BlendMode Mode;
	//SDL_GetRenderDrawBlendMode(_Renderer, &Mode);
	SDL_SetRenderDrawBlendMode(_Renderer, SDL_BLENDMODE_BLEND);
}

Window::Window(std::string Name) :
	Window(640, 480, Name)
{
	
}

void Window::SetSize(int Width, int Height)
{
	SDL_SetWindowSize(_Window, Width, Height);
}

const char* Window::GetTitle() const
{
	return SDL_GetWindowTitle(_Window);
}

int Window::GetWidth() const
{
	return _Width;
}

int Window::GetHeight() const
{
	return _Height;
}

Window::operator SDL_Window* ()
{
	return _Window;
}

Window::operator SDL_Renderer* ()
{
	return _Renderer;
}
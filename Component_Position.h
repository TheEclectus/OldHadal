#pragma once

#include <SDL2/SDL.h>

struct Component_Position
{
	float x = 0.f;
	float y = 0.f;

	operator SDL_Point()
	{
		return { static_cast<int>(x), static_cast<int>(y) };
	}

	operator SDL_FPoint()
	{
		return { x, y };
	}
};
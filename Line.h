#pragma once

#include <SDL2/SDL.h>

struct Line
{
	SDL_Point Pt1, Pt2;

	float Length() const;
};
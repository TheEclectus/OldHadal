#pragma once

#include <SDL2/SDL.h>

namespace Draw
{
	void SetPixel(SDL_Surface* surf, int x, int y, SDL_Color col);
	void VertLine(SDL_Surface* surf, int x, int y1, int y2, SDL_Color col);
}
#pragma once

#include <SDL2/SDL.h>

/// <summary>
/// Interface that signifies a class can provide a sequence of points that form a
/// line.
/// </summary>
class IPolyPoints
{
public:
	virtual const SDL_Point* GetPoints() const = 0;
	virtual size_t GetNumPoints() const = 0;
};
#pragma once

#include <vector>

#include <SDL2/SDL.h>

#include "IPolyPoints.h"

/// <summary>
/// Represents a series of 2d points as a closed polygon.
/// </summary>
class Polygon : public IPolyPoints
{
private:
	std::vector<SDL_Point> _Points;
public:
	Polygon();
	/// <summary>
	/// Will automaticall call CloseLoop().
	/// </summary>
	/// <param name="Points"></param>
	Polygon(const std::vector<SDL_Point>& Points);

	void PushPoint(const SDL_Point& Point);
	void PopPoint();

	/// <summary>
	/// If points don't form a closed loop, add the point.
	/// </summary>
	void CloseLoop();

	/// <summary>
	/// Creates a new polygon that represents the points visible in a given region.
	/// </summary>
	/// <returns></returns>
	Polygon GetRegion(const SDL_Rect& WorldspaceRegion) const;

	// IPolyPoints
	const SDL_Point* GetPoints() const override;
	size_t GetNumPoints() const override;
};
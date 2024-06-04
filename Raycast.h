#pragma once

#include "IPolyPoints.h"
#include "Line.h"
#include "Polygon.h"

namespace Raycast
{
	// TODO: Make a C++ iterator? Maybe??
	class Context
	{
	private:
		Line _RayLine;
		const IPolyPoints& _Poly;
		// If -1, hasn't started yet.
		// [0, _Poly.NumPts-1]
		// Represents line between points (_CurLineIndex) and (... + 1)
		int _CurLineIndex = -1;
		SDL_Point _CurIntersection = { -1, -1 };

	public:
		Context(const Line& Ray, const IPolyPoints& Poly);
		inline bool IsFinished() const;
		bool FindNext();
		Line GetCurrentLine() const;
		SDL_Point CurrentIntersection() const;
	};

	/// <summary>
	/// Tests whether two line segments overlap.
	/// </summary>
	/// <param name="Ray">The ray being cast.</param>
	/// <param name="IntLine">The line to test for intersection.</param>
	/// <param name="Out">If not null, will be where the intersection point is stored.</param>
	/// <returns>True if Ray intersects IntLine, false if it does not.</returns>
	bool Intersection(const Line& Ray, const Line& IntLine, SDL_Point* Out = nullptr);

	/// <summary>
	/// Tests whether a a line segment overlaps a point sequence.
	/// </summary>
	/// <param name="Ray">The ray being cast.</param>
	/// <param name="Poly">The point sequence to test for intersection.</param>
	/// <param name="OutPt">If not null, will be where the intersection point closest to Ray's start (.Pt1) is stored.</param>
	/// <param name="OutLine">If not null, will be where the line of Poly that Ray intersects is stored.</param>
	/// <returns>True if Ray intersects Poly, false if it does not.</returns>
	bool Intersection(const Line& Ray, const IPolyPoints& Poly, SDL_Point* OutPt = nullptr, Line* OutLine = nullptr);
}

/// <summary>
/// Raycasting context
/// </summary>
//class Raycast
//{
//private:
//public:
//	Raycast();
//};
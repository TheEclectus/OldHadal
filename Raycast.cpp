#include "Raycast.h"

/*
	Naive implementation
*/
// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line_segment
static bool _IntersectionAlgorithm(const Line& Ray, const Line& IntLine, SDL_Point* Out)
{
	int Denominator = ((Ray.Pt1.x - Ray.Pt2.x) * (IntLine.Pt1.x - IntLine.Pt1.x)) - ((Ray.Pt1.y - Ray.Pt2.y) * (IntLine.Pt1.x - IntLine.Pt2.x));
	if (Denominator == 0) // Parallel?
		return false;

	int x1 = Ray.Pt1.x;
	int x2 = Ray.Pt2.x;
	int x3 = IntLine.Pt1.x;
	int x4 = IntLine.Pt2.x;
	int y1 = Ray.Pt1.y;
	int y2 = Ray.Pt2.y;
	int y3 = IntLine.Pt1.y;
	int y4 = IntLine.Pt2.y;

	// Gets the intersection point on first line
	float tuDenom = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));
	float t = (((x1 - x3) * (y3 - y4)) - ((y1 - y3) * (x3 - x4))) / tuDenom;
	float u = (((x2 - x1) * (y1 - y3)) - ((y2 - y1) * (x1 - x3))) / tuDenom;

	bool bIntersectsFirstLineSegment = (t >= 0.f && t <= 1.f);
	bool bIntersectsSecondLineSegment = (u >= 0.f && u <= 1.f);
	bool bIntersects = bIntersectsFirstLineSegment && bIntersectsSecondLineSegment;

	if (bIntersects && Out)
	{
		Out->x = static_cast<int>(roundf(x1 + (t * (x2 - x1))));
		Out->y = static_cast<int>(roundf(y1 + (t * (y2 - y1))));
	}

	return bIntersects;
}

namespace Raycast
{
	Context::Context(const Line& Ray, const IPolyPoints& Poly) :
		_RayLine(Ray),
		_Poly(Poly)
	{

	}

	inline bool Context::IsFinished() const
	{
		return (_CurLineIndex >= ((int)_Poly.GetNumPoints()) - 1);
	}

	bool Context::FindNext()
	{
		if (IsFinished())
			return false;
			
		bool bFoundNext = false;
		// IsFinished condition will only test negative after last loop
		while (!bFoundNext)
		{
			_CurLineIndex++;
			if (IsFinished())
				break;

			Line TestLine = GetCurrentLine();
			bFoundNext = Raycast::Intersection(_RayLine, TestLine, &_CurIntersection);
		}

		return bFoundNext;
	}

	SDL_Point Context::CurrentIntersection() const
	{
		return _CurIntersection;
	}

	Line Context::GetCurrentLine() const
	{
		if (IsFinished())
			return { {-1, -1}, {-1, -1} };
		else
		{
			// Return [_CurLineIndex -> _CurLineIndex + 1]
			Line NewLine;
			NewLine.Pt1 = _Poly.GetPoints()[_CurLineIndex];
			NewLine.Pt2 = _Poly.GetPoints()[_CurLineIndex + 1];

			return NewLine;
		}
	}

	bool Intersection(const Line& Ray, const Line& IntLine, SDL_Point* Out)
	{
		return _IntersectionAlgorithm(Ray, IntLine, Out);
	}

	bool Intersection(const Line& Ray, const IPolyPoints& Poly, SDL_Point* OutPt, Line* OutLine)
	{
		//// Just assume that Poly's points form a loop in case of polygons
		//auto Points = Poly.GetPoints();
		//size_t PointCt = Poly.GetNumPoints();

		//// Sanity check
		//if (PointCt <= 1)
		//	return false;

		//// There will be (#Pts - 1) lines
		//size_t LineCt = PointCt - 1;

		//for (size_t CurLineIndex = 0; CurLineIndex < LineCt; CurLineIndex++)
		//{
		//	Line CurLine = {
		//		Points[CurLineIndex],
		//		Points[CurLineIndex + 1]
		//	};

		//	bool Res = _IntersectionAlgorithm(Ray, CurLine, OutPt);
		//	if (Res)
		//	{
		//		if (OutLine)
		//			*OutLine = CurLine;
		//		return true;
		//	}
		//}

		bool bInitialPointFound = false;
		SDL_Point ClosestPoint; //= { -1, -1 };
		Line ClosestLine;

		Context Ctx(Ray, Poly);
		while (Ctx.FindNext())
		{
			if (!bInitialPointFound)
			{
				ClosestPoint = Ctx.CurrentIntersection();
				ClosestLine = Ctx.GetCurrentLine();
				bInitialPointFound = true;
			}
			else
			{
				// Look for the shortest distance between Ray.Pt1 and intersection
				SDL_Point NewPoint = Ctx.CurrentIntersection();

				Line NewLine = { Ray.Pt1, NewPoint };
				Line OldLine = { Ray.Pt1, ClosestPoint };

				if (NewLine.Length() < OldLine.Length())
				{
					ClosestPoint = Ctx.CurrentIntersection();
					ClosestLine = Ctx.GetCurrentLine();
				}
			}
		}

		if (bInitialPointFound)
		{
			if (OutPt)
				*OutPt = ClosestPoint;
			if (OutLine)
				*OutLine = ClosestLine;
		}

		return bInitialPointFound;
	}
}
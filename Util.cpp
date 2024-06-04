#include "Util.h"

namespace Util
{
	SDL_Color Lerp(const SDL_Color& From, const SDL_Color& To, float Normal)
	{
		int RDiff = static_cast<int>(To.r) - static_cast<int>(From.r);
		int GDiff = static_cast<int>(To.g) - static_cast<int>(From.g);
		int BDiff = static_cast<int>(To.b) - static_cast<int>(From.b);
		int ADiff = static_cast<int>(To.a) - static_cast<int>(From.a);

		// [0.f, 1.f]
		Normal = (Normal < 0.f ? 0.f : (Normal > 1.f ? 1.f : Normal));

		Uint8 R = From.r + (RDiff * Normal);
		Uint8 G = From.g + (GDiff * Normal);
		Uint8 B = From.b + (BDiff * Normal);
		Uint8 A = From.a + (ADiff * Normal);

		return {
			R,
			G,
			B,
			A
		};
	}

	void CenterRectOnPoint(SDL_Rect& Rect, const SDL_Point& Point)
	{
		Rect.x = Point.x - (Rect.w / 2);
		Rect.y = Point.y - (Rect.h / 2);
	}

	SDL_Rect CenterRectOnPoint(const int Width, const int Height, const SDL_Point& Point)
	{
		SDL_Rect Rect{
			Point.x - (Width / 2),
			Point.y - (Height / 2),
			Width,
			Height
		};

		return Rect;
	}

	float GetLineLength(SDL_Point Point1, SDL_Point Point2)
	{
		SDL_Point PointDiff = {
			Point2.x - Point1.x,
			Point2.y - Point1.y
		};

		float ASquared = powf(static_cast<float>(PointDiff.x), 2.f);
		float BSquared = powf(static_cast<float>(PointDiff.y), 2.f);
		float C = sqrtf(ASquared + BSquared);

		return C;
	}

	bool DoLinesIntersect(SDL_Point Line1Start, SDL_Point Line1End, SDL_Point Line2Start, SDL_Point Line2End, SDL_Point* Intersection)
	{
		int Denominator = ((Line1Start.x - Line1End.x) * (Line2Start.y - Line2End.y)) - ((Line1Start.y - Line1End.y) * (Line2Start.x - Line2End.x));
		if (Denominator == 0) // Parallel?
			return false;

		int x1 = Line1Start.x;
		int x2 = Line1End.x;
		int x3 = Line2Start.x;
		int x4 = Line2End.x;
		int y1 = Line1Start.y;
		int y2 = Line1End.y;
		int y3 = Line2Start.y;
		int y4 = Line2End.y;

		int X1Y2mY1X2 = (x1 * y2) - (y1 * x2);
		int X3Y4mY3X4 = (x3 * y4) - (y3 * x4);

		int X = (X1Y2mY1X2 * (x3 - x4)) - ((x1 - x2) * X3Y4mY3X4);
		X /= Denominator;

		int Y = (X1Y2mY1X2 * (y3 - y4)) - ((y1 - y2) * X3Y4mY3X4);
		Y /= Denominator;

		if (Intersection)
		{
			Intersection->x = X;
			Intersection->y = Y;
		}

		return true;
	}

	// Rounds coordinates to nearest integer value
	bool DoLineSegmentsIntersect(SDL_Point Line1Start, SDL_Point Line1End, SDL_Point Line2Start, SDL_Point Line2End, SDL_Point* Intersection)
	{
		float x1 = Line1Start.x;
		float x2 = Line1End.x;
		float x3 = Line2Start.x;
		float x4 = Line2End.x;
		float y1 = Line1Start.y;
		float y2 = Line1End.y;
		float y3 = Line2Start.y;
		float y4 = Line2End.y;

		// Gets the intersection point on first line
		float tuDenom = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));
		float t = (((x1 - x3) * (y3 - y4)) - ((y1 - y3) * (x3 - x4))) / tuDenom;
		float u = (((x2 - x1) * (y1 - y3)) - ((y2 - y1) * (x1 - x3))) / tuDenom;
		
		bool bIntersectsFirstLineSegment = (t >= 0.f && t <= 1.f);
		bool bIntersectsSecondLineSegment = (u >= 0.f && u <= 1.f);
		bool bIntersects = bIntersectsFirstLineSegment && bIntersectsSecondLineSegment;

		if (bIntersects && Intersection)
		{
			Intersection->x = static_cast<int>( roundf(x1 + (t * (x2 - x1)) ));
			Intersection->y = static_cast<int>( roundf(y1 + (t * (y2 - y1)) ));
		}

		return bIntersects;
	}
}

// SDL_Point operators
bool operator==(const SDL_Point LHS, const SDL_Point RHS)
{
	return (LHS.x == RHS.x && LHS.y == RHS.y);
}

SDL_Point operator+(const SDL_Point LHS, const SDL_Point RHS)
{
	return { LHS.x + RHS.x, LHS.y + RHS.y };
}

SDL_Point operator+(const SDL_Point LHS, const int Scalar)
{
	return { LHS.x + Scalar, LHS.y + Scalar };
}

SDL_Point operator-(const SDL_Point LHS, const SDL_Point RHS)
{
	return { LHS.x - RHS.x, LHS.y - RHS.y };
}

SDL_Point operator-(const SDL_Point LHS, const int Scalar)
{
	return { LHS.x - Scalar, LHS.y - Scalar };
}

SDL_Point operator/(const SDL_Point LHS, const int Scalar)
{
	return { LHS.x / 2, LHS.y / 2};
}

// SDL_FPoint operators
bool operator==(const SDL_FPoint LHS, const SDL_FPoint RHS)
{
	return (LHS.x == RHS.x && LHS.y == RHS.y);
}

SDL_FPoint operator+(const SDL_FPoint LHS, const SDL_FPoint RHS)
{
	return { LHS.x + RHS.x, LHS.y + RHS.y };
}

SDL_FPoint operator+(const SDL_FPoint LHS, const int Scalar)
{
	return { LHS.x + Scalar, LHS.y + Scalar };
}

SDL_FPoint operator-(const SDL_FPoint LHS, const SDL_FPoint RHS)
{
	return { LHS.x - RHS.x, LHS.y - RHS.y };
}

SDL_FPoint operator-(const SDL_FPoint LHS, const int Scalar)
{
	return { LHS.x - Scalar, LHS.y - Scalar };
}
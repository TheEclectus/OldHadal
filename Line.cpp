#include "Line.h"

float Line::Length() const
{
	SDL_Point PointDiff = {
			Pt2.x - Pt1.x,
			Pt2.y - Pt1.y
	};

	float ASquared = powf(static_cast<float>(PointDiff.x), 2.f);
	float BSquared = powf(static_cast<float>(PointDiff.y), 2.f);
	float C = sqrtf(ASquared + BSquared);

	return C;
}
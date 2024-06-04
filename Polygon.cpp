#include "Polygon.h"

#include "Util.h"

Polygon::Polygon() :
	_Points()
{

}

Polygon::Polygon(const std::vector<SDL_Point>& Points) :
	_Points(Points)
{



}

void Polygon::PushPoint(const SDL_Point& Point)
{
	_Points.push_back(Point);
}

void Polygon::PopPoint()
{
	_Points.pop_back();
}

void Polygon::CloseLoop()
{
	if (_Points.size() == 0)
		return;

	SDL_Point& FirstPt = _Points[0];
	SDL_Point& LastPt = _Points[_Points.size() - 1];

	// If the loop isn't already closed, go ahead and add the first point to the very end.
	if (!(FirstPt == LastPt))
		_Points.push_back(FirstPt);
}

Polygon Polygon::GetRegion(const SDL_Rect& WorldspaceRegion) const
{
	// Alright, get every point visible inside of WorldpaceRegion
	// TODO: do a binary search through the points
	for (auto& CurPoint : _Points)
	{

	}

	return Polygon();
}

const SDL_Point* Polygon::GetPoints() const
{
	return _Points.data();
}

size_t Polygon::GetNumPoints() const
{
	return _Points.size();
}
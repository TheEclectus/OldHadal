#pragma once

#include <SDL2/SDL.h>

namespace Util
{
	// Normal is internally clamped to [0.f, 1.f]
	SDL_Color Lerp(const SDL_Color& From, const SDL_Color& To, float Normal);

	void CenterRectOnPoint(SDL_Rect& Rect, const SDL_Point& Point);
	SDL_Rect CenterRectOnPoint(const int Width, const int Height, const SDL_Point& Point);

	//double Lerp(double From, double To, float Normal);

	float GetLineLength(SDL_Point Point1, SDL_Point Point2);

	bool DoLinesIntersect(SDL_Point Line1Start, SDL_Point Line1End, SDL_Point Line2Start, SDL_Point Line2End, SDL_Point* Intersection = nullptr);
	
	// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line_segment
	//bool DoLineSegmentsIntersect(SDL_Point Line1Start, SDL_Point Line1End, SDL_Point Line2Start, SDL_Point Line2End, SDL_Point* Intersection = nullptr);
}

// Global SDL_* utility operators for convenience
bool operator==(const SDL_Point LHS, const SDL_Point RHS);
SDL_Point operator+(const SDL_Point LHS, const SDL_Point RHS);
SDL_Point operator+(const SDL_Point LHS, const int Scalar);
SDL_Point operator-(const SDL_Point LHS, const SDL_Point RHS);
SDL_Point operator-(const SDL_Point LHS, const int Scalar);
SDL_Point operator/(const SDL_Point LHS, const int Scalar);

bool operator==(const SDL_FPoint LHS, const SDL_FPoint RHS);
SDL_FPoint operator+(const SDL_FPoint LHS, const SDL_FPoint RHS);
SDL_FPoint operator+(const SDL_FPoint LHS, const int Scalar);
SDL_FPoint operator-(const SDL_FPoint LHS, const SDL_FPoint RHS);
SDL_FPoint operator-(const SDL_FPoint LHS, const int Scalar);

bool operator==(const SDL_Rect& LHS, const SDL_Rect& RHS);
SDL_Rect operator+(const SDL_Rect& LHS, const SDL_Rect& RHS);
SDL_Rect operator+(const SDL_Rect& LHS, const SDL_Point Pos);
SDL_Rect operator-(const SDL_Rect& LHS, const SDL_Rect& RHS);
SDL_Rect operator-(const SDL_Rect& LHS, const SDL_Point Pos);
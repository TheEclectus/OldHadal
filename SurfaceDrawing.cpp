#include "SurfaceDrawing.h"

namespace Draw
{
	void SetPixel(SDL_Surface* surf, int x, int y, SDL_Color col)
	{
		int w = surf->w;
		int h = surf->h;

		if (x < 0 || x > w - 1)
			return;
		if (y < 0 || y > h - 1)
			return;
		
		// https://stackoverflow.com/questions/20070155/how-to-set-a-pixel-in-a-sdl-surface
		if (surf->format->BytesPerPixel == 4)
		{
			Uint8* PixelBytes = static_cast<Uint8*>(surf->pixels);
			Uint32* TargetPixel = (Uint32*)(&PixelBytes[
				(y * surf->pitch) + (x * sizeof(*TargetPixel))
			]);

			*TargetPixel = SDL_MapRGBA(surf->format, col.r, col.g, col.b, col.a);
		}
	}

	void VertLine(SDL_Surface* surf, int x, int y1, int y2, SDL_Color col)
	{
		int yStart = (y1 < y2 ? y1 : y2);
		int yEnd = (y1 < y2 ? y2 : y1);

		for (int y = yStart; y <= yEnd; y++)
		{
			SetPixel(surf, x, y, col);
		}
	}
}
#pragma once

#include <string>

#include "FontAtlas.h"
#include "Widget.h"

class UIWidget_Line : public AUIWidget
{
protected:
	SDL_Color _Color;
	// Vertical if false
	bool _bHorizontal = true;

public:
	UIWidget_Line(const SDL_Color& Color, bool bHorizontalLine = true);

	virtual void RecalculateSize(SDL_Rect SizeHint) override;
	virtual void Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition) override;
};
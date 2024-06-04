#pragma once

#include <string>

#include "FontAtlas.h"
#include "Widget.h"

class UIWidget_Label : public AUIWidget
{
protected:
	FC_Font* _FontHandle;
	SDL_Color _Color;
	// Vertical if false
	bool _bHorizontal = true;
	std::string _Text = "";
	FC_AlignEnum _Alignment = FC_ALIGN_LEFT;

public:
	UIWidget_Label(const std::string& Text, const SDL_Color& Color, FC_AlignEnum Alignment = FC_ALIGN_LEFT);

	void SetText(const std::string& Text);

	virtual void RecalculateSize(SDL_Rect SizeHint) override;
	virtual void Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition) override;
};
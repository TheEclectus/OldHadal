#pragma once

#include <functional>
#include <string>

#include "FontAtlas.h"
#include "Widget.h"

class UIWidget_Button : public AUIWidget
{
protected:
	int _Padding = 4;
	FC_Font* _FontHandle;
	std::string _Text;

	std::function<void(UIWidget_Button* Caller)> _OnClickCallback = nullptr;

public:
	UIWidget_Button(FC_Font* FontHandle, const std::string& Text = "", std::function<void(UIWidget_Button* Caller)> OnClick = nullptr);

	virtual void RecalculateSize(SDL_Rect SizeHint) override;
	virtual void Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition) override;

	virtual void OnMouseUp(const SDL_Point& Offset, int Times) override;
};
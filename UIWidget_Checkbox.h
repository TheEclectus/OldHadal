#pragma once

#include <string>

#include "FontAtlas.h"
#include "Widget.h"

// UIWidget_Button.h
class UIWidget_Button;

// UIWidget_Label.h
class UIWidget_Label;

class UIWidget_Checkbox : public AUIContainerWidget
{
protected:
	UIWidget_Button* _Button = nullptr;
	UIWidget_Label* _Label = nullptr;

public:
	UIWidget_Checkbox(const std::string& Text, const SDL_Color& Color);

	void SetText(const std::string& Text);

	virtual void RecalculateSize(SDL_Rect SizeHint) override;
	//virtual void Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition) override;
};
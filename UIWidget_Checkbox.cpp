#include "UIWidget_Checkbox.h"

#include "UIWidget_Button.h"
#include "UIWidget_Label.h";

UIWidget_Checkbox::UIWidget_Checkbox(const std::string& Text, const SDL_Color& Color) :
	_Button(new UIWidget_Button(FontAtlas::DefaultFont())),
	_Label(new UIWidget_Label(Text, Color))
{
	_AddWidget(_Button);
	_AddWidget(_Label);
}

void UIWidget_Checkbox::SetText(const std::string& Text)
{
	_Label->SetText(Text);
}

void UIWidget_Checkbox::RecalculateSize(SDL_Rect SizeHint)
{
	AUIWidget::RecalculateSize(SizeHint);

	// Checkbox button should be (Line height)x(Line height) pixels
	Uint16 ButtonHeight = FC_GetLineHeight(FontAtlas::DefaultFont()) * 1.5;
	_Button->RecalculateSize({ 0, 0, ButtonHeight, ButtonHeight });

	// Label's width should be a nominal spacing between the checkbox button
	int Spacing = 3;
	SDL_Point LabelPos{
		_Button->OffsetAndSize().x + _Button->OffsetAndSize().w + Spacing,
		0
	};

	_Label->RecalculateSize({
		LabelPos.x,
		LabelPos.y,
		SizeHint.w - (_Button->OffsetAndSize().x + _Button->OffsetAndSize().w + Spacing),
		-1
	});

	auto LabelSize = _Label->OffsetAndSize();
	auto ButtonSize = _Button->OffsetAndSize();

	if (LabelSize.h > ButtonSize.h)
		_Button->SetOffset({
			ButtonSize.x, (LabelSize.h / 2) - (ButtonSize.h / 2)
		});
	else if (LabelSize.h < ButtonSize.h)
		_Label->SetOffset({
			LabelSize.x, (ButtonSize.h / 2) - (LabelSize.h / 2)
		});

	Uint16 Height = std::max(ButtonSize.h, LabelSize.h);

	_OffsetAndSize.w = SizeHint.w;
	_OffsetAndSize.h = Height;
}
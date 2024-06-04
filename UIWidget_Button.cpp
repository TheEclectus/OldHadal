#include "UIWidget_Button.h"

UIWidget_Button::UIWidget_Button(FC_Font* FontHandle, const std::string& Text, std::function<void(UIWidget_Button* Caller)> OnClick) :
	_FontHandle(FontHandle),
	_Text(Text),
	_OnClickCallback(OnClick)
{

}

void UIWidget_Button::RecalculateSize(SDL_Rect SizeHint)
{
	AUIWidget::RecalculateSize(SizeHint);

	Uint16 Width = SizeHint.w == -1 ? FC_GetWidth(_FontHandle, _Text.c_str()) + (_Padding * 2) : SizeHint.w;
	Uint16 Height = SizeHint.h == -1 ?
		FC_GetColumnHeight(_FontHandle, Width, _Text.c_str()) + (_Padding * 2)
		: SizeHint.h;

	_OffsetAndSize.w = Width;
	_OffsetAndSize.h = Height;
}

void UIWidget_Button::Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition)
{
	SDL_Rect ScreenDims = this->ScreenPositionAndSize();
	//SDL_RenderSetClipRect(Renderer, &ScreenDims);

	// DEFAULT: Just draw a rectangle around the "widget's" bounds
	int Width = _OffsetAndSize.w;
	int Height = _OffsetAndSize.h;

	SDL_Rect Bounds = {
		ScreenPosition.x,
		ScreenPosition.y,
		Width,
		Height
	};

	

	if (!bIsMouseOver())
	{
		SDL_SetRenderDrawColor(Renderer, 170, 170, 170, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(Renderer, 100, 200, 100, 255);
	}
	SDL_RenderFillRect(Renderer, &Bounds);
	//SDL_RenderDrawRect(Renderer, &Bounds);

	//SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);

	//if (!bIsMouseOver())
	//SDL_RenderDrawRect(Renderer, &Bounds);
	//else
	//	SDL_RenderFillRect(Renderer, &Bounds);

	int X = Bounds.x + (Bounds.w / 2);
	int Y = Bounds.y + _Padding;

	FC_DrawColumnAlign(_FontHandle, Renderer, X, Y, _OffsetAndSize.w, FC_ALIGN_CENTER, _Text.c_str());

	//SDL_RenderSetClipRect(Renderer, nullptr);
}

void UIWidget_Button::OnMouseUp(const SDL_Point& Offset, int Times)
{
	if(_OnClickCallback)
		_OnClickCallback(this);
}
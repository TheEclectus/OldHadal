#include "UIWidget_Label.h"

UIWidget_Label::UIWidget_Label(const std::string& Text, const SDL_Color& Color, FC_AlignEnum Alignment) :
	_FontHandle(FontAtlas::DefaultFont()),
	_Text(Text),
	_Color(Color),
	_Alignment(Alignment)
{

}

void UIWidget_Label::SetText(const std::string& Text)
{
	_Text = Text;
	// TODO: currently assumes that width will remain constant between self-made RecalculateSize calls
	//RecalculateSize({ -1, -1, _OffsetAndSize.w, -1 });
	Dirty();
}

void UIWidget_Label::RecalculateSize(SDL_Rect SizeHint)
{
	AUIWidget::RecalculateSize(SizeHint);

	Uint16 Width = SizeHint.w == -1 ? FC_GetWidth(_FontHandle, _Text.c_str()) : SizeHint.w;
	Uint16 Height = SizeHint.h == -1 ?
		FC_GetColumnHeight(_FontHandle, Width, _Text.c_str())
		: SizeHint.h;

	_OffsetAndSize.w = Width;
	_OffsetAndSize.h = Height;
}

void UIWidget_Label::Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition)
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

	//SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
	//SDL_RenderDrawRect(Renderer, &Bounds);

	int X = Bounds.x;// +(ScreenDims.w / 2);
	if (_Alignment == FC_ALIGN_CENTER)
		X += (ScreenDims.w / 2);
	else if (_Alignment == FC_ALIGN_RIGHT)
		X += ScreenDims.w;
	int Y = Bounds.y;

	//FC_DrawColumnAlign(_FontHandle, Renderer, X, Y, _OffsetAndSize.w, _Alignment, _Text.c_str());
	FC_DrawBoxAlign(_FontHandle, Renderer, Bounds, _Alignment, _Text.c_str());

	//SDL_RenderSetClipRect(Renderer, nullptr);
}
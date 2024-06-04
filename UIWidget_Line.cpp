#include "UIWidget_Line.h"

#include "Util.h"

UIWidget_Line::UIWidget_Line(const SDL_Color& Color, bool bHorizontalLine) :
	_Color(Color),
	_bHorizontal(bHorizontalLine)
{

}

void UIWidget_Line::RecalculateSize(SDL_Rect SizeHint)
{
	AUIWidget::RecalculateSize(SizeHint);

	if (_bHorizontal)
	{
		_OffsetAndSize.w = (SizeHint.w == -1 ? 50 : SizeHint.w);
		_OffsetAndSize.h = (SizeHint.h == -1 ? 1 : SizeHint.h);
	}
	else
	{
		_OffsetAndSize.w = (SizeHint.w == -1 ? 1 : SizeHint.w);
		_OffsetAndSize.h = (SizeHint.h == -1 ? 50 : SizeHint.h);
	}
}

void UIWidget_Line::Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition)
{
	int Width = _OffsetAndSize.w;
	int Height = _OffsetAndSize.h;
	SDL_Point Start = ScreenPosition;
	SDL_Point End = { 0, 0 };

	if (_bHorizontal)
	{
		if (Height > 1)
			Start.y += (Height / 2);

		End.x = Start.x + (Width - 1);
		End.y = Start.y;
	}
	else
	{
		if (Width > 1)
			Start.x += (Width / 2);

		End.x = Start.x;
		End.y = Start.y + (Height - 1);
	}

	SDL_SetRenderDrawColor(Renderer, _Color.r, _Color.g, _Color.b, _Color.a);
	SDL_RenderDrawLine(Renderer, Start.x, Start.y, End.x, End.y);
}
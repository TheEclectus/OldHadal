#include "UIWidget_Pane.h"

#include "FontAtlas.h"
#include "UIWidget_Scrollbar.h"
#include "Util.h"

UIWidget_Pane::UIWidget_Pane(const std::string& Title, bool bDraggable, bool bShowVerticalScrollbar)// :
	//_bShowVerticalScrollbar(bShowVerticalScrollbar)
{
	_bDraggable = bDraggable;

	SetTitle(Title);

	FC_Font* Font = FontAtlas::DefaultFont();
	auto TitlebarBounds = FC_GetBounds(Font, 0, 0, FC_ALIGN_LEFT, FC_MakeScale(1.f, 1.f), _Title.c_str());
	TitlebarBounds.w += _TitlebarPadding * 2;
	TitlebarBounds.h += _TitlebarPadding * 2;

	_OffsetAndSize.x = 75;
	_OffsetAndSize.y = 150;
	_OffsetAndSize.w = 120;
	_OffsetAndSize.h = 170;

	//_DragZone.w = _OffsetAndSize.w;
	_DragZone.w = -1;
	_DragZone.h = TitlebarBounds.h;

	_VerticalScrollbar = new UIWidget_Scrollbar();
	_VerticalScrollbar->SetVisible(bShowVerticalScrollbar);
	AddWidget(_VerticalScrollbar);

	//SetShowVerticalScrollbar(false);
	//_VerticalScrollbar->RecalculateSize({_OffsetAndSize.w -})
}

const std::string& UIWidget_Pane::GetTitle() const
{
	return _Title;
}

void UIWidget_Pane::SetTitle(const std::string& Title)
{
	_Title = Title;
}

void UIWidget_Pane::SetHeight(int Height)
{
	_OffsetAndSize.h = Height;
}

void UIWidget_Pane::SetWidth(int Width)
{
	_OffsetAndSize.w = Width;
	_DragZone.w = Width;
	//RecalculateSize({ -1, -1, Width, -1 });
}

void UIWidget_Pane::SetPadding(int Padding)
{
	_Padding = Padding;
}

void UIWidget_Pane::SetShowVerticalScrollbar(bool bShowScrollbar)
{
	bool bScrollbarShown = _VerticalScrollbar->bIsVisible();
	if (bShowScrollbar != bScrollbarShown)
	{
		_VerticalScrollbar->SetVisible(bShowScrollbar);
		//_bShowVerticalScrollbar = bShowScrollbar;
		//RecalculateSize()
		Dirty();
	}
}

void UIWidget_Pane::Dirty()
{
	RecalculateSize({ -1, -1, _OffsetAndSize.w , -1 });
	// TODO: if the Pane has *finite* bounds (scrollable area included) and is a child widget, maybe update up parent's chain too?
}

void UIWidget_Pane::RecalculateSize(SDL_Rect SizeHint)
{
	// Set position
	AUIWidget::RecalculateSize(SizeHint);

	if (SizeHint.w != -1)
		_OffsetAndSize.w = SizeHint.w;
	if (SizeHint.h != -1)
		_OffsetAndSize.h = SizeHint.h;

	int DragZoneYOffset = _bDraggable ? _DragZone.h : 0;
	int ChildOffsetY = DragZoneYOffset + _VerticalPadding;

	bool bScrollbarShown = _VerticalScrollbar->bIsVisible();
	if (bScrollbarShown)
	{
		_VerticalScrollbar->RecalculateSize({ -1, -1, -1, _OffsetAndSize.h - DragZoneYOffset });
		_VerticalScrollbar->SetOffset({ _OffsetAndSize.w - _VerticalScrollbar->OffsetAndSize().w, DragZoneYOffset });
	}

	//const int ChildMaxWidth = _OffsetAndSize.w - (_Padding * 2);
	const int ChildMaxWidth = _OffsetAndSize.w - (_Padding * 2) - (bScrollbarShown ? _VerticalScrollbar->OffsetAndSize().w : 0);
	const int ChildOffsetX = _Padding; //_VerticalPadding;

	const auto* const LastChild = _Children.size() > 0 ? (_Children.end() - 1)->get() : nullptr;
	const bool bStretchLastChild = LastChild ? LastChild->bVerticalStretch() : false;
	const int MaxYOffset = _OffsetAndSize.h - _VerticalPadding;
	for (auto& CurChild : _Children)
	{
		if (CurChild.get() == _VerticalScrollbar)
			continue;

		if(bStretchLastChild && CurChild.get() == LastChild)
			CurChild->RecalculateSize({ ChildOffsetX, ChildOffsetY, ChildMaxWidth, MaxYOffset - ChildOffsetY });
		else
			CurChild->RecalculateSize({ ChildOffsetX, ChildOffsetY, ChildMaxWidth, -1 });

		ChildOffsetY += CurChild->OffsetAndSize().h + _VerticalPadding;
	}

	// If ChildOffsetY > (_OffsetAndSize.h - DragZoneYOffset), tack the difference onto the scrollbar
	int ScrollbarSteps = 0;
	if (ChildOffsetY > (_OffsetAndSize.h))
	{
		ScrollbarSteps = ChildOffsetY - (_OffsetAndSize.h);
	}
	_VerticalScrollbar->SetSteps(ScrollbarSteps);
}

void UIWidget_Pane::Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition)
{
	// DEFAULT: Just draw a rectangle around the "widget's" bounds
	int Width = _OffsetAndSize.w;
	int Height = _OffsetAndSize.h;

	SDL_Rect Bounds = {
		ScreenPosition.x,
		ScreenPosition.y,
		Width,
		Height
	};

	// Old transparent color
	//SDL_SetRenderDrawColor(Renderer, 50, 50, 50, 127);
	// New solid color
	//SDL_SetRenderDrawColor(Renderer, 77, 105, 121, 255);
	SDL_SetRenderDrawColor(Renderer, 91, 126, 145, 255);
	SDL_RenderFillRect(Renderer, &Bounds);

	//SDL_SetRenderDrawColor(Renderer, 70, 102, 119, 255);
	//SDL_RenderDrawRect(Renderer, &Bounds);

	if (bIsDraggable())
	{
		const SDL_Rect& DragZoneRef = DragZone();
		SDL_Rect DragZoneBounds = {
			Bounds.x + DragZoneRef.x,
			Bounds.y + DragZoneRef.y,
			(DragZoneRef.w == -1 ? Bounds.w : DragZoneRef.w),
			(DragZoneRef.h == -1 ? Bounds.h : DragZoneRef.h),
		};

		// Old, transparent color
		//SDL_SetRenderDrawColor(Renderer, 20, 20, 20, 140);
		// New solid color
		SDL_SetRenderDrawColor(Renderer, 70, 102, 119, 255);

		//SDL_RenderDrawRect(Renderer, &DragZoneBounds);
		SDL_RenderFillRect(Renderer, &DragZoneBounds);
		
		FC_Draw(FontAtlas::DefaultFont(), Renderer, DragZoneBounds.x + _TitlebarPadding, DragZoneBounds.y + _TitlebarPadding, _Title.c_str());
	}

	//if (_ParentContainer)
	{
		SDL_SetRenderDrawColor(Renderer, 255, 255, 0, 255);
		SDL_RenderDrawRect(Renderer, &Bounds);
	}

	SDL_Point ScrollbarOffsetPosition = ScreenPosition;
	int ScrollbarPos = _VerticalScrollbar->GetStep();
	ScrollbarOffsetPosition.y -= ScrollbarPos;

	//AUIContainerWidget::Draw(Renderer, ScrollbarOffsetPosition);
	{
		SDL_Rect ScreenDims = this->ScreenPositionAndSize();
		int DragZoneYOffset = _bDraggable ? _DragZone.h : 0;
		ScreenDims.y += DragZoneYOffset;
		ScreenDims.h -= DragZoneYOffset;

		SDL_RenderSetClipRect(Renderer, &ScreenDims);

		//SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
		//SDL_RenderDrawRect(Renderer, &ScreenDims);

		//AUIWidget::Draw(Renderer, ScreenPosition);

		for (auto& CurChild : _Children)
		{
			if (!CurChild->bIsVisible())
				continue;

			const SDL_Rect ChildDims = CurChild->OffsetAndSize();
			SDL_Point ChildPos;
			
			if(CurChild.get() != _VerticalScrollbar)
				ChildPos = {
					ChildDims.x + ScrollbarOffsetPosition.x,
					ChildDims.y + ScrollbarOffsetPosition.y
				};
			else
				ChildPos = {
					ChildDims.x + ScreenPosition.x,
					ChildDims.y + ScreenPosition.y
			};

			/*if (CurChild != _VerticalScrollbar)
				ChildPos = ChildPos + ScrollbarOffsetPosition;*/

			CurChild->Draw(Renderer, ChildPos);
		}

		SDL_RenderSetClipRect(Renderer, nullptr);
	}
}

AUIWidget* UIWidget_Pane::WidgetAtRelativePosition(const SDL_Point& Pos)
{
	return AUIPublicContainerWidget::WidgetAtRelativePosition({ Pos.x, Pos.y + _VerticalScrollbar->GetStep() });
}

//void UIWidget_Pane::OnDragMove(SDL_Point& NewPosition)
//{
//	// Restrict horizontal movement.
//	//NewPosition.x = this->OffsetAndSize().x;
//}
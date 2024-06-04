#include "UIWidget_Scrollbar.h"

void UIWidget_Scrollbar::_UIWidget_Scrollbar_Tracker::_CalculateScrubberSize()
{
	int MaxTrackSize = _MaximumSize - _MinimumSize;

	// The track can fit the scrubber with 1:1 pixel:step ratio
	if (_TraversalSize <= MaxTrackSize)
	{
		_StepsPerPixel = 1.f;
		_OffsetAndSize.h = _MaximumSize - _TraversalSize;
	}
	else
	{
		_OffsetAndSize.h = _MinimumSize;
		_StepsPerPixel = static_cast<float>(_TraversalSize) / static_cast<float>(_MaximumSize - _OffsetAndSize.h);
	}

	_TrackSize = _MaximumSize - _OffsetAndSize.h;
}

void UIWidget_Scrollbar::_UIWidget_Scrollbar_Tracker::_MoveTrackerToPosition()
{
	SDL_Point NewPos = {
		_OffsetAndSize.x,
		_Padding + _TrackerPosition
	};

	this->SetOffset(NewPos);
}

void UIWidget_Scrollbar::_UIWidget_Scrollbar_Tracker::_SetTrackerPositionRaw(int Pos)
{
	Pos = std::min(Pos, _TrackSize);
	Pos = std::max(Pos, 0);

	_TrackerPosition = Pos;
}

void UIWidget_Scrollbar::_UIWidget_Scrollbar_Tracker::_UpdateTrackerPosition()
{
	_TrackerPosition = static_cast<int>(static_cast<float>(_OffsetAndSize.y - _Padding) * _StepsPerPixel);
}

UIWidget_Scrollbar::_UIWidget_Scrollbar_Tracker::_UIWidget_Scrollbar_Tracker(int Padding, bool bVertical) :
	_bVertical(bVertical),
	_Padding(Padding)
{
	_bDraggable = true;
}

void UIWidget_Scrollbar::_UIWidget_Scrollbar_Tracker::SetTraverseSize(int Size)
{
	_TraversalSize = Size;
	_CalculateScrubberSize();
}

void UIWidget_Scrollbar::_UIWidget_Scrollbar_Tracker::SetTrackerPosition(int Pos)
{
	_SetTrackerPositionRaw(Pos);
	_MoveTrackerToPosition();
}

int UIWidget_Scrollbar::_UIWidget_Scrollbar_Tracker::GetTrackerPosition() const
{
	return static_cast<int>(static_cast<float>(_OffsetAndSize.y - _Padding) * _StepsPerPixel);
}

// REMEMBER: it'll pass the MAXIMUM sizes of both dimensions!!
void UIWidget_Scrollbar::_UIWidget_Scrollbar_Tracker::RecalculateSize(SDL_Rect SizeHint)
{
	AUIWidget::RecalculateSize(SizeHint);

	if (_bVertical)
	{
		_MaximumSize = SizeHint.h - (_Padding * 2);

		_CalculateScrubberSize();

		// Width is constrained, height is calculated
		_OffsetAndSize.w = SizeHint.w;
		//_OffsetAndSize.h = _MaximumSize - 1;	// TODO: calculate height

		//_TrackSize = SizeHint.h - _OffsetAndSize.h - _Padding;

		//_OffsetAndSize.h = _TrackSize;
	}
}

void UIWidget_Scrollbar::_UIWidget_Scrollbar_Tracker::Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition)
{
	SDL_Rect Bounds{ ScreenPosition.x, ScreenPosition.y, _OffsetAndSize.w, _OffsetAndSize.h };

	if(!this->bIsMouseOver())
		SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	else
		SDL_SetRenderDrawColor(Renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(Renderer, &Bounds);
}

void UIWidget_Scrollbar::_UIWidget_Scrollbar_Tracker::OnDragMove(SDL_Point& NewPosition)
{
	NewPosition.x = _OffsetAndSize.x;

	if (NewPosition.y < _Padding)
		NewPosition.y = _Padding;

	if (NewPosition.y > _Padding + _TrackSize)
		NewPosition.y = _Padding + _TrackSize;

	if (NewPosition.y != _OffsetAndSize.y)
	{
		//int NewTraverse = NewPosition.y - _Padding;
		//_SetTrackerPositionRaw(NewPosition);
		_UpdateTrackerPosition();
	}
}

UIWidget_Scrollbar::UIWidget_Scrollbar(bool bVertical) :
	_bVertical(bVertical)
{
	_Tracker = new _UIWidget_Scrollbar_Tracker(_ScrubberPadding, bVertical);
	_AddWidget(_Tracker);
}

void UIWidget_Scrollbar::SetSteps(int Steps)
{
	_Tracker->SetTraverseSize(Steps);
}

int UIWidget_Scrollbar::GetStep() const
{
	return _Tracker->GetTrackerPosition();
}

void UIWidget_Scrollbar::RecalculateSize(SDL_Rect SizeHint)
{
	AUIWidget::RecalculateSize(SizeHint);

	// Tracker size recalculation here
	// Fudging it for the time being
	int ScrollbarWidth = 7;

	int Width = (SizeHint.w == -1) ? (ScrollbarWidth + (_ScrubberPadding * 2)) : SizeHint.w;
	int Height = (SizeHint.h == -1) ? 50 : SizeHint.h;

	_OffsetAndSize.w = Width;
	_OffsetAndSize.h = Height;

	int MaxTrackerSize = _OffsetAndSize.h;
	_Tracker->RecalculateSize({ _ScrubberPadding, _ScrubberPadding, Width - (_ScrubberPadding * 2), MaxTrackerSize });
}

void UIWidget_Scrollbar::Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition)
{
	//SDL_Rect ScreenDims = this->ScreenPositionAndSize();
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

	SDL_SetRenderDrawColor(Renderer, 35, 35, 35, 127);
	SDL_RenderFillRect(Renderer, &Bounds);

	AUIContainerWidget::Draw(Renderer, ScreenPosition);

	//SDL_RenderSetClipRect(Renderer, nullptr);
}
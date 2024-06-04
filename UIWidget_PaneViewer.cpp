#include "UIWidget_PaneViewer.h"

#include "UIWidget_Pane.h"

UIWidget_PaneViewer::UIWidget_PaneViewer() :
	AUIWidget()
{
	SetVerticalStretch(true);
}

//UIWidget_PaneViewer::~UIWidget_PaneViewer()
//{
//	for
//}

size_t UIWidget_PaneViewer::AddPane(UIWidget_Pane* Pane, bool bSet)
{
	if (std::find_if(_Panes.begin(), _Panes.end(), [Pane](const std::unique_ptr<AUIWidget>& Cur) {
		return Cur.get() == Pane;
		}) == _Panes.end())
	{
		_Panes.push_back(UIWidgetHandle(Pane));
		int Index = _Panes.size() - 1;
		if (bSet)
			SetPane(Index);
		return Index;
	}

	return static_cast<size_t>(-1);
}

void UIWidget_PaneViewer::SetPane(size_t Index)
{
	if (Index < _Panes.size())
		_NextPane = static_cast<UIWidget_Pane*>(_Panes[Index].get());
}

void UIWidget_PaneViewer::SetPane(UIWidget_Pane* Pane)
{
	if (std::find_if(_Panes.begin(), _Panes.end(), [Pane](const std::unique_ptr<AUIWidget>& Cur) {
		return Cur.get() == Pane;
		}) != _Panes.end())
	{
		_NextPane = Pane;
	}
}

void UIWidget_PaneViewer::RecalculateSize(SDL_Rect SizeHint)
{
	AUIWidget::RecalculateSize(SizeHint);

	if (SizeHint.w != -1)
		_OffsetAndSize.w = SizeHint.w;
	if (SizeHint.h != -1)
		_OffsetAndSize.h = SizeHint.h;

	/*
		Here's where the fuckery begins.
		- Gonna use SetParentContainer() on the new Pane so the offset calculations will work
		- Pass SizeHint directly to the pane and RecalculateSize() it
		- WidgetAtRelativePosition() will return whatever the Pane returns.
		This should all culminate in the Pane behaving as if it were in the PaneViewer's place.
	*/
	if (_NextPane)
	{
		if(_CurrentPane)
			_CurrentPane->SetParentContainer(nullptr);
		_NextPane->SetParentContainer(this->ParentContainer());
		_CurrentPane = _NextPane;
		_NextPane = nullptr;
	}

	if(_CurrentPane)
		_CurrentPane->RecalculateSize(SizeHint);
}

void UIWidget_PaneViewer::Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition)
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
	SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(Renderer, &Bounds);

	if (_CurrentPane)
		_CurrentPane->Draw(Renderer, ScreenPosition);
}

AUIWidget* UIWidget_PaneViewer::WidgetAtRelativePosition(const SDL_Point& Pos)
{
	if (!_CurrentPane)
		return nullptr;
	else
		return _CurrentPane->WidgetAtRelativePosition(Pos);
}
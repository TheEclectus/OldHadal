#pragma once

#include <string>

#include "Widget.h"

// UIWidget_Scrollbar.h
class UIWidget_Scrollbar;

class UIWidget_Pane : public AUIPublicContainerWidget
{
protected:
	const int _TitlebarPadding = 3;
	std::string _Title = "Pane";
	int _Padding = 4;
	int _VerticalPadding = 4;

	//bool _bShowVerticalScrollbar = true;
	UIWidget_Scrollbar* _VerticalScrollbar = nullptr;

public:
	UIWidget_Pane(const std::string& Title = "Pane", bool bDraggable = false, bool bShowVerticalScrollbar = true);

	const std::string& GetTitle() const;
	void SetTitle(const std::string& Title);

	void SetHeight(int Height);
	void SetWidth(int Width);
	void SetPadding(int Padding);
	void SetShowVerticalScrollbar(bool bShowScrollbar);

	// Shorthand for "RecalculateSize() at current width"
	virtual void Dirty() override;

	virtual void RecalculateSize(SDL_Rect SizeHint) override;
	virtual void Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition) override;
	virtual AUIWidget* WidgetAtRelativePosition(const SDL_Point& Pos) override;

	//virtual void OnDragMove(SDL_Point& NewPosition) override;
};
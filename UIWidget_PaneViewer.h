#pragma once

#include <string>
#include <vector>

#include "Widget.h"

// UIWidget_Pane.h
class UIWidget_Pane;

// Basically just serves as a delegate for a Pane
// Makes one Pane visible at a time, and resizes it to fit the PaneViewer when made visible.
class UIWidget_PaneViewer : public AUIWidget // Not a AUIContainerWidget specifically because this is supposed to be a surrogate, not a container (which has a lot of functionality that would need to be overridden)
{
protected:
	std::vector<UIWidgetHandle> _Panes;		// OWNED
	UIWidget_Pane* _CurrentPane = nullptr;

	UIWidget_Pane* _NextPane = nullptr;

public:
	UIWidget_PaneViewer();
	//~UIWidget_PaneViewer();	// Free _Panes

	size_t AddPane(UIWidget_Pane* Pane, bool bSet = false);

	void SetPane(size_t Index);
	void SetPane(UIWidget_Pane* Pane);

	// If a pane change is pending, it'll wait until here to do it.
	virtual void RecalculateSize(SDL_Rect SizeHint) override;
	virtual void Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition) override;
	virtual AUIWidget* WidgetAtRelativePosition(const SDL_Point& Pos) override;
};
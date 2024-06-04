#pragma once

#include <functional>
#include <string>
#include <vector>

#include "ADebugTool.h"
#include "FontAtlas.h"
#include "Widget.h"

#include "UIWidget_Pane.h"

// UIWidget_Label.h
class UIWidget_Label;

// UIWidget_Line.h
class UIWidget_Line;

// UIWidget_PaneViewer.h

class UIWidget_DebugToolView : public UIWidget_Pane// AUIWidget
{
protected:
	//UIWidget_Pane* _Pane = nullptr;
	UIWidget_Label* _Label_Desc = nullptr;
	UIWidget_Line* _Line = nullptr;

public:
	UIWidget_DebugToolView();

	void SetDescription(const std::string& Description);

	void SetTool(ADebugTool* Tool);

	//virtual void RecalculateSize(SDL_Rect SizeHint) override;
	virtual void Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition) override;
	/*virtual AUIWidget* WidgetAtRelativePosition(const SDL_Point& Pos) override;

	virtual void OnMouseWheel(int Vertical) override;*/
};
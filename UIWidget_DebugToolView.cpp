#include "UIWidget_DebugToolView.h"

#include "UIWidget_Label.h"
#include "UIWidget_Line.h"
#include "UIWidget_Pane.h"

#include "UIWidget_Button.h"

#include "UIWidget_PaneViewer.h"

UIWidget_DebugToolView::UIWidget_DebugToolView() :
	UIWidget_Pane("( ' v ')", false, false)
{
	SetPadding(0);
	SetVerticalStretch(true);
	//SetShowVerticalScrollbar(false);

	//_Pane = new UIWidget_Pane("( ' v ')");
	//_Pane->SetPadding(0);
	//_Pane->SetWidth(200);

	_Label_Desc = new UIWidget_Label("Test label", { 255, 255, 255, 255 });
	this->AddWidget(_Label_Desc);

	_Line = new UIWidget_Line({ 155, 155, 155, 255 });
	this->AddWidget(_Line);

	//this->AddWidget(new UIWidget_Button(FontAtlas::DefaultFont(), "Test"));

	auto PV = new UIWidget_PaneViewer();
	auto PVP = new UIWidget_Pane();
	PVP->AddWidget(new UIWidget_Button(FontAtlas::DefaultFont(), "Test"));
	PV->AddPane(PVP, true);
	this->AddWidget(PV);
}

void UIWidget_DebugToolView::SetDescription(const std::string& Description)
{
	_Label_Desc->SetText(Description);
}

void UIWidget_DebugToolView::SetTool(ADebugTool* Tool)
{
	if (Tool)
	{
		_Label_Desc->SetText(Tool->GetToolDesc());
	}
	else
	{
		_Label_Desc->SetText("<No tool!>");
	}
}

//void UIWidget_DebugToolView::RecalculateSize(SDL_Rect SizeHint)
//{
//	AUIWidget::RecalculateSize(SizeHint);
//
//	_Pane->RecalculateSize(SizeHint);
//}

void UIWidget_DebugToolView::Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition)
{
	//_Pane->Draw(Renderer, ScreenPosition);
	UIWidget_Pane::Draw(Renderer, ScreenPosition);
}
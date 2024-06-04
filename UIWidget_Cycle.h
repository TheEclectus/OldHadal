#pragma once

#include <functional>
#include <string>
#include <vector>

#include "FontAtlas.h"
#include "Widget.h"

class UIWidget_Cycle : public AUIWidget
{
public:
	struct Item
	{
		std::string Label;
		void* Value;
	};

protected:
	int _PreferredItemCount = 3;
	int _VisibleItemCount = 0;	// Set by RecalculateSize

	int _Padding = 4;
	int _MinPaddingBetweenItems = 2;
	FC_Font* _FontHandle;
	
	int _ItemIndex = 0;
	std::vector<Item> _Items;

	std::function<void(UIWidget_Cycle* Caller, void* Userdata)> _OnItemChangeCB = nullptr;
	void* _OnItemChangeCBUserdata = nullptr;

	std::vector<std::string> _GetVisibleItemLabels() const;

public:
	UIWidget_Cycle(std::function<void(UIWidget_Cycle* Caller, void* Userdata)> OnItemChangeCB = nullptr, void* Userdata = nullptr);

	int ItemIndex() const;
	void PushItem(const std::string& Label, void* Userdata = nullptr);
	Item* SelectedItem();

	virtual void RecalculateSize(SDL_Rect SizeHint) override;
	virtual void Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition) override;

	virtual void OnMouseWheel(int Vertical) override;
};
#include "UIWidget_Cycle.h"

std::vector<std::string> UIWidget_Cycle::_GetVisibleItemLabels() const
{
	std::vector<std::string> Results;
	if (_Items.size() == 0)
	{
		for (int i = 0; i < _VisibleItemCount; i++)
			Results.push_back("<undefined>");
	}
	else
	{
		size_t PrevIndex = _ItemIndex == 0 ? _Items.size() - 1 : _ItemIndex - 1;
		size_t NextIndex = _ItemIndex == _Items.size() - 1 ? 0 : _ItemIndex + 1;

		Results.push_back(_Items[PrevIndex].Label);
		Results.push_back(_Items[_ItemIndex].Label);
		Results.push_back(_Items[NextIndex].Label);
	}

	return Results;
}

UIWidget_Cycle::UIWidget_Cycle(std::function<void(UIWidget_Cycle* Caller, void* Userdata)> OnItemChangeCB, void* Userdata) :
	_FontHandle(FontAtlas::DefaultFont()),
	_OnItemChangeCB(OnItemChangeCB),
	_OnItemChangeCBUserdata(Userdata)
{
	
}

int UIWidget_Cycle::ItemIndex() const
{
	return _ItemIndex;
}

void UIWidget_Cycle::PushItem(const std::string& Label, void* Userdata)
{
	_Items.push_back({ Label, Userdata });
	if (_Items.size() == 1 && _OnItemChangeCB) // First item added
		_OnItemChangeCB(this, _OnItemChangeCBUserdata);
}

UIWidget_Cycle::Item* UIWidget_Cycle::SelectedItem()
{
	if (_Items.size() == 0)
		return nullptr;
	else
		return &_Items[_ItemIndex];
}

void UIWidget_Cycle::RecalculateSize(SDL_Rect SizeHint)
{
	AUIWidget::RecalculateSize(SizeHint);

	Uint16 Width = 0u;
	if (SizeHint.w == -1)
	{
		// Get the widest item 
		size_t WidestItem = 0u;
		if (_Items.size() == 0)
			WidestItem = FC_GetWidth(_FontHandle, "<undefined>");
		else
		{
			for (auto& CurItem : _Items)
			{
				WidestItem = std::max(WidestItem, (size_t)FC_GetWidth(_FontHandle, CurItem.Label.c_str()));
			}
		}
	}
	else
		Width = SizeHint.w;

	Uint16 Height = 0u;
	if (SizeHint.h == -1)
	{
		// Just enough space to fit preferred number of items
		Height =
			(FC_GetLineHeight(_FontHandle) * _PreferredItemCount) // Item heights
			+ (_MinPaddingBetweenItems * (_PreferredItemCount - 1)) // Inter-item padding
			+ (_Padding * 2) // Top-bottom padding
			;

		_VisibleItemCount = _PreferredItemCount;
	}
	else
	{
		Height = SizeHint.h;

		const int LineHeight = FC_GetLineHeight(_FontHandle);
		// Calculate number of visible items
		int TestVisibleItemCount = 1;
		int VisibleItemHeight = (_Padding * 2) + (LineHeight * TestVisibleItemCount) + (_MinPaddingBetweenItems * (TestVisibleItemCount - 1));
		while (VisibleItemHeight < Height)
		{
			TestVisibleItemCount += 2;
			VisibleItemHeight = (_Padding * 2) + (LineHeight * TestVisibleItemCount) + (_MinPaddingBetweenItems * (TestVisibleItemCount - 1));
		}
		
		TestVisibleItemCount -= 2;
		TestVisibleItemCount = std::max(TestVisibleItemCount, 1);
	}

	_OffsetAndSize.w = Width;
	_OffsetAndSize.h = Height;
}

void UIWidget_Cycle::Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition)
{
	SDL_Rect ScreenDims = this->ScreenPositionAndSize();
	SDL_RenderSetClipRect(Renderer, &ScreenDims);

	// DEFAULT: Just draw a rectangle around the "widget's" bounds
	int Width = _OffsetAndSize.w;
	int Height = _OffsetAndSize.h;

	SDL_Rect Bounds = {
		ScreenPosition.x,
		ScreenPosition.y,
		Width,
		Height
	};



	if (!bIsMouseOver())
	{
		SDL_SetRenderDrawColor(Renderer, 170, 170, 170, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(Renderer, 220, 220, 220, 255);
	}
	SDL_RenderDrawRect(Renderer, &Bounds);

	//SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);

	//if (!bIsMouseOver())
	//SDL_RenderDrawRect(Renderer, &Bounds);
	//else
	//	SDL_RenderFillRect(Renderer, &Bounds);

	// Total extra space in addition to minimum padding
	int FreeSpace = (Height - (_Padding * 2)) 
		- (_VisibleItemCount * FC_GetLineHeight(_FontHandle));
	int PaddingPerItem = FreeSpace / (_VisibleItemCount - 1);

	auto Labels = _GetVisibleItemLabels();

	int Y = ScreenDims.y + _Padding;
	int X = ScreenDims.x + _Padding;
	int MiddleItemIndex = (_VisibleItemCount - 1) / 2;

	for (int i = 0; i < _VisibleItemCount; i++)
	{
		SDL_Color Color = { 255, 255, 255, 127 };
		if (i == MiddleItemIndex)
			Color.a = 255;
		//SDL_Rect DrawSize = FC_DrawColumnAlign(_FontHandle, Renderer, X, Y, _OffsetAndSize.w, FC_ALIGN_LEFT, Labels[i].c_str());
		SDL_Rect DrawSize = FC_DrawColor(_FontHandle, Renderer, X, Y, Color, Labels[i].c_str());

		Y += DrawSize.h + PaddingPerItem;
	}

	SDL_RenderSetClipRect(Renderer, nullptr);
}

void UIWidget_Cycle::OnMouseWheel(int Vertical)
{
	int NewIndex = (_ItemIndex - Vertical) % static_cast<int>(_Items.size());
	if (NewIndex < 0)
		NewIndex = static_cast<int>(_Items.size()) + NewIndex;
	
	_ItemIndex = NewIndex;
	if (_OnItemChangeCB)
		_OnItemChangeCB(this, _OnItemChangeCBUserdata);
}
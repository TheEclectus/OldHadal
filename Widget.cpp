#include "Input.h"
#include "Widget.h"

#include "Util.h"

AUIWidget* UIScene::_GetWidgetAtPosition(const SDL_Point& Point)
{
	AUIWidget* MouseOverCandidate = nullptr;
	//for (auto CurWidget : _Widgets)
	for(auto CurWidgetIter = _Widgets.rbegin(); CurWidgetIter != _Widgets.rend(); CurWidgetIter++)
	{
		auto& CurWidget = *CurWidgetIter;
		if (!CurWidget->bIsVisible())
			continue;

		const SDL_Rect OffsetAndSize = CurWidget->OffsetAndSize();
		if (SDL_PointInRect(&Point, &OffsetAndSize))
		{
			const SDL_Rect& CurOffsetAndSize = CurWidget->OffsetAndSize();
			SDL_Point RelativePos = {
				Point.x - CurOffsetAndSize.x,
				Point.y - CurOffsetAndSize.y
			};

			// Should get lowest widget in hierarchy
			MouseOverCandidate = CurWidget->WidgetAtRelativePosition(RelativePos);
			// WidgetAtRelativePosition() should never return an invisible widget, but check again here
			if (MouseOverCandidate && MouseOverCandidate->bIsVisible())
				break;
		}
	}

	return MouseOverCandidate;
}

void UIScene::_SetMousedOverWidget(AUIWidget* MousedOverWidget)
{
	if (MousedOverWidget != _MousedOverWidget)
	{
		// If any was moused over before, make it not so anymore
		if (_MousedOverWidget)
		{
			_MousedOverWidget->_bIsMouseOver = false;
		}

		if (MousedOverWidget)
		{
			MousedOverWidget->_bIsMouseOver = true;
		}

		_MousedOverWidget = MousedOverWidget;
	}
}

void UIScene::_MouseMoveEvent(SDL_MouseMotionEvent& Event)
{
	SDL_Point MousePos = { Event.x, Event.y };
	
	if (_DraggingWidget)
	{
		// Translate mouse position on screen to relative to widget
		const SDL_Rect& ScreenRect = _DraggingWidget->ScreenPositionAndSize();
		SDL_Point ScreenPos = { ScreenRect.x, ScreenRect.y };

		// New Offset = Mouse position relative to parent - Offset
		SDL_Point MousePosRelativeToParent = {};
		if (_DraggingWidget->_ParentContainer == nullptr)
		{
			MousePosRelativeToParent = MousePos;
		}
		else
		{
			const auto& ParentScreenPos = _DraggingWidget->_ParentContainer->ScreenPositionAndSize();
			SDL_Point ParentPos = { ParentScreenPos.x, ParentScreenPos.y };
			MousePosRelativeToParent = MousePos - ParentPos;
		}

		SDL_Point NewDragPos = MousePosRelativeToParent - _DraggingWidgetOffset;

		_DraggingWidget->OnDragMove(NewDragPos);
		_DraggingWidget->SetOffset(NewDragPos);
		
		//// Relative to the widget's origin inside the parent
		//SDL_Point RelativeMousePos = MousePos - ScreenPos;

		//// TODO: Prevent widgets from getting dragged offscreen
		//// This is assuming mouse position origin is (0,0) and not the TL corner of the parent widget
		//SDL_Point NewDragPos = RelativeMousePos - _DraggingWidgetOffset;
		//if (_DraggingWidget->ParentContainer())
		//{
		//	const auto& ParentScreenPos = _DraggingWidget->_ParentContainer->ScreenPositionAndSize();
		//	SDL_Point ParentPos = { ParentScreenPos.x, ParentScreenPos.y };
		//	NewDragPos = NewDragPos - ParentPos;
		//}
		////SDL_Point
		////NewDragPos = MousePos - _DraggingWidgetOffset;

		//_DraggingWidget->OnDragMove(NewDragPos);
		//_DraggingWidget->SetOffset(NewDragPos);

		return;
	}

	AUIWidget* MouseOverCandidate = _GetWidgetAtPosition(MousePos);
	//for (auto CurWidget : _Widgets)
	//{
	//	const SDL_Rect OffsetAndSize = CurWidget->OffsetAndSize();
	//	if (SDL_PointInRect(&MousePos, &OffsetAndSize))
	//	{
	//		const SDL_Rect& CurOffsetAndSize = CurWidget->OffsetAndSize();
	//		SDL_Point RelativePos = {
	//			MousePos.x - CurOffsetAndSize.x,
	//			MousePos.y - CurOffsetAndSize.y
	//		};
	//		// Should get lowest widget in hierarchy
	//		MouseOverCandidate = CurWidget->WidgetAtRelativePosition(RelativePos);
	//		if (MouseOverCandidate)
	//			break;
	//	}
	//}
	// There's some kind of change happening where _MousedOverWidget won't be moused over anymore
	//if (MouseOverCandidate != _MousedOverWidget)
	//{
	//	// If any was moused over before, make it not so anymore
	//	if (_MousedOverWidget)
	//	{
	//		_MousedOverWidget->_bIsMouseOver = false;
	//	}
	//	if (MouseOverCandidate)
	//	{
	//		MouseOverCandidate->_bIsMouseOver = true;
	//	}
	//	 
	//	_MousedOverWidget = MouseOverCandidate;
	//}

	_SetMousedOverWidget(MouseOverCandidate);

	// TODO: trigger a MouseMoveOver event or something here
}

void UIScene::_MouseButtonEvent(SDL_MouseButtonEvent& Event)
{
	const SDL_Point MousePos = {
		Event.x,
		Event.y
	};

	if (Event.button == SDL_BUTTON_LEFT)
	{
		if (Event.type == SDL_MOUSEBUTTONDOWN)
		{
			// Look to pick up a new dragging widget
			if (_MousedOverWidget && !_DraggingWidget)
			{
				// Check if mouse is hovering over the drag zone of the widget
				const SDL_Rect WidgetDimensions = _MousedOverWidget->ScreenPositionAndSize();
				bool bDraggable = _MousedOverWidget->bIsDraggable();
				if (bDraggable)
				{
					const SDL_Rect& WidgetDragZone = _MousedOverWidget->DragZone();
					SDL_Rect ScreenDragZone = {
						WidgetDimensions.x + WidgetDragZone.x,
						WidgetDimensions.y + WidgetDragZone.y,
						(WidgetDragZone.w == -1 ? WidgetDimensions.w : WidgetDragZone.w),
						(WidgetDragZone.h == -1 ? WidgetDimensions.h : WidgetDragZone.h)
					};

					if (SDL_PointInRect(&MousePos, &ScreenDragZone))
					{
						// Widget is now being dragged
						_DraggingWidget = _MousedOverWidget;
						_DraggingWidget->_bIsBeingDragged = true;

						//SDL_Point RelativeMousePos = 

						//_DraggingWidgetOffset = _DraggingWidget->Offset();

						// 
						_DraggingWidgetOffset = {
							MousePos.x - ScreenDragZone.x,
							MousePos.y - ScreenDragZone.y
						};
					}
				}
			}

			// Trigger MouseDown event if we're not dragging a widget now
			if (_MousedOverWidget && !_DraggingWidget)
			{
				//static int times = 0;
				//times++;

				SDL_Rect WidgetScreenPos = _MousedOverWidget->ScreenPositionAndSize();
				SDL_Point PosInWidget = {
					MousePos.x - WidgetScreenPos.x,
					MousePos.y - WidgetScreenPos.y
				};

				_MousedOverWidget->OnMouseDown(PosInWidget, Event.clicks);
			}
		}
		else if (Event.type == SDL_MOUSEBUTTONUP)
		{
			// Release dragging widget
			if (_DraggingWidget)
			{
				_DraggingWidget->_bIsBeingDragged = false;
				_DraggingWidget = nullptr;
				_DraggingWidgetOffset = { -1, -1 };
			}
			// Don't fire MouseUp event if a widget was being dragged
			else if (_MousedOverWidget)
			{
				SDL_Rect WidgetScreenPos = _MousedOverWidget->ScreenPositionAndSize();
				SDL_Point PosInWidget = {
					MousePos.x - WidgetScreenPos.x,
					MousePos.y - WidgetScreenPos.y
				};

				_MousedOverWidget->OnMouseUp(PosInWidget, Event.clicks);
			}
		}
	}
}

void UIScene::_MouseWheelEvent(SDL_MouseWheelEvent& Event)
{
	if (_MousedOverWidget)
	{
		int Y = Event.y * (Event.direction == SDL_MOUSEWHEEL_FLIPPED ? -1 : 1);

		_MousedOverWidget->OnMouseWheel(Y);
	}
}

void UIScene::_WidgetRemoved(AUIWidget* Widget)
{
	if (_MousedOverWidget == Widget)
	{
		// TODO: Maybe check under the mouse instead?
		_SetMousedOverWidget(nullptr);
	}
}

UIScene::UIScene(const SDL_Rect& Dimensions) :
	_Dimensions(Dimensions)
{
	
}

UIScene::~UIScene()
{
	// Not needed; because they're unique_ptr under the hood they'll be destroyed when _Widgets is cleared.
	/*for (auto& CurChild : _Widgets)
	{
		delete CurChild;
	}*/
	_Widgets.clear();
}

void UIScene::AddWidget(AUIWidget* Widget)
{
	auto Res = std::find_if(_Widgets.begin(), _Widgets.end(), [&Widget](const UIWidgetHandle& CurWidget) {
		return CurWidget.get() == Widget;
	});
	if (Res == _Widgets.end())	// Don't add the widget if it's already present
	{
		//_Widgets.push_back(std::make_unique<AUIWidget>(*Widget));
		_Widgets.push_back(std::unique_ptr<AUIWidget>(Widget));
		Widget->_ParentScene = this;
		Widget->RecalculateSize({ -1, -1, -1, -1 });
	}
}

void UIScene::AddWidget(UIWidgetHandle& Widget)
{
	auto Res = std::find_if(_Widgets.begin(), _Widgets.end(), [&Widget](const UIWidgetHandle& CurWidget) {
		return CurWidget == Widget;
	});
	if (Res == _Widgets.end())	// Don't add the widget if it's already present
	{
		Widget->_ParentScene = this;
		Widget->RecalculateSize({ -1, -1, -1, -1 });
		_Widgets.push_back(std::move(Widget));
	}
}

void UIScene::RemoveWidget(AUIWidget* Widget)
{
	auto Res = std::find_if(_Widgets.begin(), _Widgets.end(), [&Widget](const UIWidgetHandle& CurWidget) {
		return CurWidget.get() == Widget;
	});
	if (Res != _Widgets.end())	// Don't try and remove the widget if it's not present
	{
		_Widgets.erase(Res);
		Widget->_ParentScene = nullptr;
	}
}

void UIScene::Input(SDL_Event& Event)
{
	if (!_bVisible)
		return;

	if (Event.type == SDL_EventType::SDL_MOUSEMOTION)
		_MouseMoveEvent(Event.motion);
	else if (Event.type == SDL_EventType::SDL_MOUSEBUTTONDOWN || Event.type == SDL_EventType::SDL_MOUSEBUTTONUP)
		_MouseButtonEvent(Event.button);
	else if (Event.type == SDL_EventType::SDL_MOUSEWHEEL)
		_MouseWheelEvent(Event.wheel);
}

void UIScene::Draw(SDL_Renderer* Renderer)
{
	if (!_bVisible)
		return;

	for (auto& CurWidget : _Widgets)
	{
		if (!CurWidget->bIsVisible())
			continue;

		const SDL_Rect& WidgetBounds = CurWidget->OffsetAndSize();
		SDL_Point OffsetPosition = {
			_Dimensions.x + WidgetBounds.x,
			_Dimensions.y + WidgetBounds.y,
		};

		CurWidget->Draw(Renderer, OffsetPosition);
	}

	//SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
	//SDL_RenderDrawRect(Renderer, &_Dimensions);
}

void UIScene::SetVisibility(bool bIsVisible)
{
	/*
		If being made invisible:
		- Dragging should be ceased
		- MouseOver widget should be cleared

		When being made visible:
		- Scene should be polled for MouseOver again
	*/

	// Visible -> Invisible
	if (_bVisible && !bIsVisible)
	{
		_SetMousedOverWidget(nullptr);
	}
	else if (!_bVisible && bIsVisible)
	{
		_SetMousedOverWidget(_GetWidgetAtPosition(Input::MousePosition()));
	}

	_bVisible = bIsVisible;
}

bool UIScene::bIsVisible() const
{
	return _bVisible;
}

void UIScene::ValidateWidgetStates()
{
	if (_MousedOverWidget && _MousedOverWidget->ParentScene() == nullptr)
	{
		const SDL_Point& MousePos = Input::MousePosition();
		_MousedOverWidget = nullptr;	// Set to nullptr so next call won't segfault
		_SetMousedOverWidget(_GetWidgetAtPosition(MousePos));
	}
}

//void AUIWidget::_SetParentScene(UIScene* ParentScene)
//{
//	if (_ParentScene)
//		_ParentScene->_WidgetRemoved(this);
//
//	_ParentScene = ParentScene;
//}

AUIWidget::AUIWidget()
{

}

AUIWidget::~AUIWidget()
{
	/*if (_ParentScene)
	{
		_ParentScene->RemoveWidget(this);
	}*/

	auto Scene = ParentScene();
	if (Scene)
	{
		Scene->_WidgetRemoved(this);
	}

	/* 
		TODO: Currently, when a widget is removed from a container using _RemoveWidget(),
		it's erased from the container's list of children, and then the unique_ptr is allowed
		to call the destructor. _ParentContainer will still be non-null in those few calls
		between then and here. Should that be rectified somehow?
	*/
	if (_ParentContainer)
	{
		_ParentContainer->_RemoveWidget(this);
	}
}

bool AUIWidget::bIsMouseOver() const
{
	return _bIsMouseOver;
}

bool AUIWidget::bIsDraggable() const
{
	return _bDraggable;
}

const SDL_Point AUIWidget::Offset() const
{
	return { _OffsetAndSize.x, _OffsetAndSize.y };
}

// TODO: Maybe consider cacheing global position?
const SDL_Rect& AUIWidget::OffsetAndSize() const
{
	return _OffsetAndSize;
}

const SDL_Rect AUIWidget::ScreenPositionAndSize() const
{
	if (_ParentContainer == nullptr)
		return _OffsetAndSize;
	else
	{
		SDL_Rect ParentOffset = _ParentContainer->ScreenPositionAndSize();
		SDL_Rect ParentOffsetAdded = {
			ParentOffset.x + _OffsetAndSize.x,
			ParentOffset.y + _OffsetAndSize.y,
			_OffsetAndSize.w,
			_OffsetAndSize.h
		};

		return ParentOffsetAdded;
	}
}

const SDL_Rect& AUIWidget::DragZone() const
{
	return _DragZone;
}

bool AUIWidget::bIsVisible() const
{
	return _bIsVisible;
}

UIScene* AUIWidget::ParentScene()
{
	// If there's not a bound parent scene but there's a parent container, then grab that.

	if (!_ParentScene && _ParentContainer)
		return _ParentContainer->ParentScene();
	else
		return _ParentScene;
}

AUIContainerWidget* AUIWidget::ParentContainer()
{
	return _ParentContainer;
}

void AUIWidget::SetParentContainer(AUIContainerWidget* ParentContainer)
{
	_ParentContainer = ParentContainer;
}

void AUIWidget::SetOffset(const SDL_Point& Offset)
{
	//if(_OffsetAndSize.x != -1)
		_OffsetAndSize.x = Offset.x;
	//if (_OffsetAndSize.y != -1)
		_OffsetAndSize.y = Offset.y;
}

void AUIWidget::SetVisible(bool bIsVisible)
{
	_bIsVisible = bIsVisible;
}

void AUIWidget::Dirty()
{
	if (_ParentContainer)
		_ParentContainer->Dirty();
}

bool AUIWidget::bVerticalStretch() const
{
	return _bVerticalStretch;
}

void AUIWidget::SetVerticalStretch(bool bStretch)
{
	//_VerticalStretch = Stretch;
	_bVerticalStretch = bStretch;
}

void AUIWidget::RecalculateSize(SDL_Rect SizeHint)
{
	if(SizeHint.x != -1)
		_OffsetAndSize.x = SizeHint.x;
	if (SizeHint.y != -1)
		_OffsetAndSize.y = SizeHint.y;

	/*if (_DragZone.w == -1)
		_DragZone.w = _OffsetAndSize.w;
	if (_DragZone.h == -1)
		_DragZone.h = _OffsetAndSize.h;*/
}

void AUIWidget::Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition)
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

	SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);

	if (!bIsMouseOver())
		SDL_RenderDrawRect(Renderer, &Bounds);
	else
		SDL_RenderFillRect(Renderer, &Bounds);

	if (bIsDraggable())
	{
		const SDL_Rect& DragZoneRef = DragZone();
		SDL_Rect DragZoneBounds = {
			Bounds.x + DragZoneRef.x,
			Bounds.y + DragZoneRef.y,
			(DragZoneRef.w == -1 ? Bounds.w : DragZoneRef.w),
			(DragZoneRef.h == -1 ? Bounds.h : DragZoneRef.h),
		};

		SDL_SetRenderDrawColor(Renderer, 0, 0, 255, 255);
		SDL_RenderDrawRect(Renderer, &DragZoneBounds);
	}
}

AUIWidget* AUIWidget::WidgetAtRelativePosition(const SDL_Point& Pos)
{
	//if (_bDraggable == false)
	//{
	//	return this;
	//}

	//// Entire widget is draggable
	//else if (_DragZone.w == -1 && _DragZone.h == -1)
	//{
	//	return this;
	//}

	//else if(SDL_PointInRect(&Pos, &DragZone()))
	//{
	//	return this;
	//}

	//else
	//{
	//	return nullptr;
	//}

	return this;
}

void AUIWidget::OnMouseDown(const SDL_Point& Offset, int Times)
{

}

void AUIWidget::OnMouseUp(const SDL_Point& Offset, int Times)
{

}

void AUIWidget::OnMouseOver(const SDL_Point& Offset)
{

}

void AUIWidget::OnMouseWheel(int Vertical)
{

}

void AUIWidget::OnDragMove(SDL_Point& Movement)
{

}

AUIContainerWidget::AUIContainerWidget()
{

}

void AUIContainerWidget::_AddWidget(AUIWidget* Widget)
{
	//auto Res = std::find(_Children.begin(), _Children.end(), Widget);
	auto Res = std::find_if(_Children.begin(), _Children.end(), [Widget](const std::unique_ptr<AUIWidget>& Cur) {
		return Cur.get() == Widget;
	});

	if (Res == _Children.end())	// Don't add the widget if it's already present
	{
		_Children.push_back(std::unique_ptr<AUIWidget>(Widget));
		Widget->SetParentContainer(this);
		//Widget->RecalculateSize({ -1, -1, -1, -1 });
	}
}

void AUIContainerWidget::_RemoveWidget(AUIWidget* Widget)
{
	//auto Res = std::find(_Children.begin(), _Children.end(), Widget);
	const auto &Res = std::find_if(_Children.begin(), _Children.end(), [Widget](const std::unique_ptr<AUIWidget>& Cur) {
		return Cur.get() == Widget;
	});

	if (Res != _Children.end())	// Don't try and remove the widget if it's not present
	{
		//Widget->SetParentContainer(nullptr);
		// TODO: Maybe if Res.use_count() indicated this is the last reference, signal the scene that the widget's about to be removed
		//bool Unique = Res->use_count() == 1;
		//_Children.erase(Res);
		//Widget->RecalculateSize({ -1, -1, -1, -1 });

		// Remove the child without calling the destructor.
		UIWidgetHandle TempHandle = std::move(*Res);
		_Children.erase(Res);
		// *Now* TempHandle destroys the pointer.
	}
}
void AUIContainerWidget::Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition)
{
	SDL_Rect ScreenDims = this->ScreenPositionAndSize();

	//SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
	//SDL_RenderDrawRect(Renderer, &ScreenDims);

	//SDL_RenderSetClipRect(Renderer, &ScreenDims);

	//AUIWidget::Draw(Renderer, ScreenPosition);

	for (auto& CurChild : _Children)
	{
		if (!CurChild->bIsVisible())
			continue;

		// This draws it based on screen position alone
		/*const SDL_Rect ChildDims = CurChild->ScreenPositionAndSize();
		SDL_Point ChildPos = {
			ChildDims.x,
			ChildDims.y
		};

		CurChild->Draw(Renderer, ChildPos);*/

		const SDL_Rect ChildDims = CurChild->OffsetAndSize();
		SDL_Point ChildPos = {
			ChildDims.x + ScreenPosition.x,
			ChildDims.y + ScreenPosition.y
		};

		CurChild->Draw(Renderer, ChildPos);
	}

	//SDL_RenderSetClipRect(Renderer, nullptr);
}

AUIWidget* AUIContainerWidget::WidgetAtRelativePosition(const SDL_Point& Pos)
{
	if (!this->bIsVisible())
		return nullptr;

	for (auto& CurChild : _Children)
	{
		if (!CurChild->bIsVisible())
			continue;

		const SDL_Rect& CurOffsetAndSize = CurChild->OffsetAndSize();
		
		if (SDL_PointInRect(&Pos, &CurOffsetAndSize))
		{
			SDL_Point RelativePos = {
				Pos.x - CurOffsetAndSize.x,
				Pos.y - CurOffsetAndSize.y
			};

			AUIWidget* Candidate = CurChild->WidgetAtRelativePosition(RelativePos);
			if (Candidate && Candidate->bIsVisible())
				return Candidate;
		}
		
	}

	return AUIWidget::WidgetAtRelativePosition(Pos);
}

void AUIPublicContainerWidget::AddWidget(AUIWidget* Widget)
{
	_AddWidget(Widget);
}

void AUIPublicContainerWidget::RemoveWidget(AUIWidget* Widget)
{
	_RemoveWidget(Widget);
}

const std::vector<std::unique_ptr<AUIWidget>>& AUIPublicContainerWidget::Children() const
{
	return _Children;
}
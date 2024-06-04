#pragma once

#include <memory>
#include <vector>

#include <SDL2/SDL.h>

class UIScene;
class AUIWidget;
class AUIContainerWidget;

typedef std::unique_ptr<AUIWidget> UIWidgetHandle;

//class UIWidgetHandle
//{
//private:
//	AUIWidget* _RawPtr = nullptr;
//	std::unique_ptr<AUIWidget> _Ptr = nullptr;
//
//public:
//	bool bIsOwning() const;
//};

//template<typename T = AUIWidget>
//class UIWidgetHandle // : public std::unique_ptr<AUIWidget> 
//{
//private:
//	std::unique_ptr<AUIWidget> _Ptr;
//public:
//	UIWidgetHandle(std::unique_ptr<AUIWidget>&& Ptr)
//	{
//		_Ptr = Ptr;
//	}
//
//	void operator=(std::unique_ptr<AUIWidget>&& Ptr)
//	{
//		_Ptr = Ptr;
//	}
//
//	T& operator*() const
//	{
//		return *static_cast<T*>(_Ptr.get());
//	}
//	T* operator->() const
//	{
//		return static_cast<T*>(_Ptr.get());
//	}
//
//	operator std::unique_ptr<AUIWidget>&()
//	{
//		return &_Ptr;
//	}
//};


/*
	Any widget or container that's meant to graphically display widgets should
	recalculate their size whenever a new one is added.
*/

/// <summary>
/// Actually owns the widgets in question.
/// </summary>
// TODO: Add some means to 'pin' a child widget to a portion of the screen by one or more of its corners
class UIScene
{
	friend class AUIWidget;
	//friend class AUIContainerWidget;
private:
	// Screen position
	SDL_Rect _Dimensions;	// TODO: Clipping/input culling outside dimensions (::Draw(), rectangle outline)
	//std::vector<AUIWidget*> _Widgets;
	std::vector<UIWidgetHandle> _Widgets;

	bool _bVisible = true;

	AUIWidget* _MousedOverWidget = nullptr;
	AUIWidget* _DraggingWidget = nullptr;

	// Should be mouse position relative to dragging widget's parent (or the scene)
	SDL_Point _DraggingWidgetOffset = { -1, -1 };	// Where the drag was started from relative to the TL corner of the widget's drag space

	// Will never return a hidden widget
	AUIWidget* _GetWidgetAtPosition(const SDL_Point& Point);
	void _SetMousedOverWidget(AUIWidget* MousedOverWidget);

	void _MouseMoveEvent(SDL_MouseMotionEvent& Event);
	void _MouseButtonEvent(SDL_MouseButtonEvent& Event);
	void _MouseWheelEvent(SDL_MouseWheelEvent& Event);

	// A widget is being removed from the scene (destroyed, re-assigned, etc.)
	void _WidgetRemoved(AUIWidget* Widget);

public:
	UIScene(const SDL_Rect& Dimensions);
	~UIScene();
	// Uses make_unique()
	// TODO: Phase out
	void AddWidget(AUIWidget* Widget);
	void AddWidget(UIWidgetHandle& Widget);
	void RemoveWidget(AUIWidget* Widget);

	void Input(SDL_Event& Event);
	void Draw(SDL_Renderer* Renderer);

	void SetVisibility(bool bIsVisible);
	bool bIsVisible() const;

	// Checks if _MousedOverWidget, _DraggingWidget, etc. are still in the hierarchy; if they aren't, null the states. 
	void ValidateWidgetStates();
};

// No concept of children by default, that should be a special subclass of widget
class AUIWidget
{
	friend class UIScene;
private:
	UIScene* _ParentScene = nullptr;

	// ==== State flags (controlled by UIScene) ====
	bool _bIsMouseOver = false;
	bool _bIsBeingDragged = false;


protected:
	AUIContainerWidget* _ParentContainer = nullptr;

	// ==== Read-only flags and related data ====
	bool _bDraggable = false;	// TODO: Make incompatible with creating a drag-drop payload (as in if it's dragged from its drag zone, event will never fire)
	SDL_Rect _DragZone = { 0, 0, -1, -1 };	// w/h == -1/-1 means full widget

	// Updated by RecalculateSize().
	SDL_Rect _OffsetAndSize = { 0, 0, 50, 50 };

	// Proportion of free space that the widget should take up
	//int _VerticalStretch = 0;
	
	// Should widget fill remaining space if it's the last in a container?
	bool _bVerticalStretch = false;

	// Hidden widgets don't get drawn, receive logical updates, etc.
	bool _bIsVisible = true;

	//virtual void _SetParentScene(UIScene* ParentScene);

	// TOMORROW: drag-and-drop events/payloads
	// UIScene should behave different when a payload is active, like immediately forfeiting DragWidget etc.
	// 

public:
	AUIWidget();
	virtual ~AUIWidget();

	bool bIsMouseOver() const;
	bool bIsDraggable() const;
	const SDL_Point Offset() const;
	const SDL_Rect& OffsetAndSize() const;
	const SDL_Rect ScreenPositionAndSize() const;	// Used for drawing
	const SDL_Rect& DragZone() const;
	bool bIsVisible() const;

	UIScene* ParentScene();
	AUIContainerWidget* ParentContainer();
	// Should only be called by a container widget!
	void SetParentContainer(AUIContainerWidget* ParentContainer);

	void SetOffset(const SDL_Point& Offset);
	void SetVisible(bool bIsVisible);

	// Propogate up the chain that this widget has new display data and needs resized.
	virtual void Dirty();

	//void SetVerticalStretch(int Stretch = 0);
	bool bVerticalStretch() const;
	void SetVerticalStretch(bool bStretch = false);

	// Called on construction and whenever widget is repositioned or needs resized.
	// SizeHint .x and .y hint the widget's new position when != -1
	// If .w or .h are != -1, then it must fit into the given constraint
	// So, w/h being -1 ought to mean that something is dependent on a size being set
	// Default implementation sets hinted position where applicable and sets own DragZone size when w/h == -1
	virtual void RecalculateSize(SDL_Rect SizeHint);

	virtual void Draw(SDL_Renderer *Renderer, SDL_Point ScreenPosition);
	
	// Used for getting children and the like.
	// Returns this by default.
	// Base implementation returns this if _bDraggable == false (if it's true, only if Pos is in drag zone)
	// Draggable zone should always return this.
	// Should never return a widget that's hidden.
	virtual AUIWidget* WidgetAtRelativePosition(const SDL_Point& Pos);

	// ========= Events with blank default implementations =========
	// TODO: Maybe have OR'd flags for whether to even call an event? Maybe that's overkill?
	virtual void OnMouseDown(const SDL_Point& Offset, int Times);
	virtual void OnMouseUp(const SDL_Point& Offset, int Times);
	virtual void OnMouseOver(const SDL_Point& Offset);

	// Will be triggered on moused over widget
	virtual void OnMouseWheel(int Vertical);

	// Triggered when widget is about to be dragged around; can be used to limit movement.
	virtual void OnDragMove(SDL_Point& NewPosition);
};

// Widget that accounts for children
class AUIContainerWidget : public AUIWidget
{
	friend class AUIWidget;
protected:
	std::vector<UIWidgetHandle> _Children;
	//std::vector<AUIWidget*> _Children;

	// Just adds child to the list; doesn't call RecalculateSize()
	virtual void _AddWidget(AUIWidget* Widget);
	// Just removes child from list
	virtual void _RemoveWidget(AUIWidget* Widget);

	//virtual void _SetParentScene(UIScene* Scene) override;

public:
	AUIContainerWidget();
	
	// Both of these should be called (ex. AUIContainerWidget::AddWidget()) by deriving classes

	//RecalculateSize() would be a great place to reposition child widgets based on layout

	virtual void Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition) override;
	// Default behavior, but prioritized returning children first.
	virtual AUIWidget* WidgetAtRelativePosition(const SDL_Point& Pos) override;
};

// Exact same as AUIContainerWidget, but exposes Add- and RemoveWidget as public functions
class AUIPublicContainerWidget : public AUIContainerWidget
{
public:
	void AddWidget(AUIWidget* Widget);
	void RemoveWidget(AUIWidget* Widget);
	//const std::vector<AUIWidget*>& Children() const;
	const std::vector<std::unique_ptr<AUIWidget>>& Children() const;
};
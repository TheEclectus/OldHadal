#pragma once

#include <string>

//#include "FontAtlas.h"
#include "Widget.h"

class UIWidget_Scrollbar : public AUIContainerWidget
{
	// Scrollbar itself is like the track for the scrubber itself
private:
	class _UIWidget_Scrollbar_Tracker : public AUIWidget
	{
	private:
		bool _bVertical = true;
		int _Padding = 0;

		int _TrackerPosition = 0;

		int _MinimumSize = 20;	// The smallest it can be in the working dimension
		int _MaximumSize = -1;	// Calculated every RecalculateSize() call, full track height

		// Calculated by RecalculateSize; number of pixels the slider can move
		// Functionally the maximum y-offset that the scrubber can be from _Padding
		int _TrackSize = 0;	

		int _TraversalSize = 350;	// How many logical steps there are to traverse (NOT pixels)
		
		float _StepsPerPixel = 1.f;	// When TraversalSize > TrackSize, = TraversalSize / TrackSize

		void _CalculateScrubberSize();
		void _MoveTrackerToPosition();
		void _SetTrackerPositionRaw(int Pos);
		void _UpdateTrackerPosition();

	public:
		_UIWidget_Scrollbar_Tracker(int Padding, bool bVertical = true);

		void SetTraverseSize(int Size);
		void SetTrackerPosition(int Pos);
		int GetTrackerPosition() const;

		// SPECIAL CASE: SizeHint will pass the *maximum* size it can be.
		virtual void RecalculateSize(SDL_Rect SizeHint) override;
		virtual void Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition) override;

		virtual void OnDragMove(SDL_Point& NewPosition) override;
	};

protected:
	_UIWidget_Scrollbar_Tracker* _Tracker = nullptr;

	bool _bVertical = true;	// Otherwise horizontal
	//int _SidePadding = 2;
	int _ScrubberPadding = 1;

public:
	UIWidget_Scrollbar(bool bVertical = true);

	void SetSteps(int Steps);
	int GetStep() const;

	virtual void RecalculateSize(SDL_Rect SizeHint) override;
	virtual void Draw(SDL_Renderer* Renderer, SDL_Point ScreenPosition) override;

	//virtual AUIWidget* WidgetAtRelativePosition(const SDL_Point& Pos);

	//virtual void OnMouseUp(const SDL_Point& Offset, int Times) override;
};

#include "Input.h"

void Input::Update()
{
	// Move old current state to previous
	memcpy(_PrevKeyboardState, _KeyboardState, sizeof(_KeyboardState));
	// Update the current buffer
	memcpy(_KeyboardState, SDL_GetKeyboardState(nullptr), sizeof(_KeyboardState));

	// Maybe have buffers that store whether or not a key was pressed or released this frame?
	// !Prev[n] && Cur[n] => Pressed
	// Prev[n] && Cur[n] => Held
	// Prev[n] && !Cur[n] => Released

	// Update mouse state
	_MouseState = SDL_GetMouseState(&_MousePosition.x, &_MousePosition.y);
	bool bLeftMouseButton = _MouseState & SDL_BUTTON(SDL_BUTTON_LEFT);
	bool bMiddleMouseButton = _MouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE);
	bool bRightMouseButton = _MouseState & SDL_BUTTON(SDL_BUTTON_RIGHT);
}

bool Input::IsKeyDown(int Scancode)
{
	// Prune too-large values right away
	if (Scancode >= SDL_NUM_SCANCODES)
		return false;

	return _KeyboardState[Scancode];
}

bool Input::IsMouseDown(int MouseButton)
{
	return (_MouseState & SDL_BUTTON(MouseButton));
}

const SDL_Point& Input::MousePosition()
{
	return _MousePosition;
}

void Input::BlockAlphaKeys(bool Value)
{
	_bShouldBlockAlphaKeys = Value;
}

bool Input::ShouldBlockAlphaKeys()
{
	return _bShouldBlockAlphaKeys;
}

Uint8 Input::_PrevKeyboardState[SDL_NUM_SCANCODES];
Uint8 Input::_KeyboardState[SDL_NUM_SCANCODES];

SDL_Point Input::_MousePosition = { -1, -1 };
Uint32 Input::_MouseState = 0;

bool Input::_bShouldBlockAlphaKeys = false;
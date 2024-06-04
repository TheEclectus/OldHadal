#pragma once

#include <SDL2/SDL.h>

class Input
{
private:
	static Uint8 _PrevKeyboardState[SDL_NUM_SCANCODES], _KeyboardState[SDL_NUM_SCANCODES];
	
	static SDL_Point _MousePosition;
	static Uint32 _MouseState;

	// Block hints
	static bool _bShouldBlockAlphaKeys;	// Should A-Z be blocked?
	//static bool _bShouldBlockNumKeys;	// Should 0-9 be blocked?

public:
	static void Update();

	static bool IsKeyDown(int Scancode);
	static bool IsMouseDown(int MouseButton);
	static const SDL_Point& MousePosition();

	static void BlockAlphaKeys(bool Value);
	static bool ShouldBlockAlphaKeys();
};
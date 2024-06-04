#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "SDL_FontCache.h"

#include "3rd Party/rectpack2d/finders_interface.h"

class FontAtlas
{
private:
	static std::unordered_map<std::string, FC_Font*> _fonts;
	static FC_Font* _defaultFont;

public:
	static void RegisterFont(SDL_Renderer* Renderer, const std::string& Path, Uint32 PointSize, SDL_Color Color, int Style, const std::string& ID);
	
	static void SetDefaultFont(const std::string& ID);
	static FC_Font* DefaultFont();

	static FC_Font* FindFont(const std::string& ID);
};

class Font
{
private:
	TTF_Font* _Font = nullptr;
public:
	Font(const std::string& Path);

	int Height() const;
};
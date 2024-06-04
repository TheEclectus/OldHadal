#include "FontAtlas.h"

#include <string>

void FontAtlas::RegisterFont(SDL_Renderer* Renderer, const std::string& Path, Uint32 PointSize, SDL_Color Color, int Style, const std::string& ID)
{
	FC_Font* NewFont = FC_CreateFont();
	if (FC_LoadFont(NewFont, Renderer, Path.c_str(), PointSize, Color, Style) != 1 || _fonts.count(ID) != 0)
	{
		// Error
		FC_FreeFont(NewFont);
	}
	else
	{
		FC_SetFilterMode(NewFont, FC_FilterEnum::FC_FILTER_NEAREST);
		_fonts.insert({ ID, NewFont });
	}
}

void FontAtlas::SetDefaultFont(const std::string& ID)
{
	_defaultFont = FindFont(ID);
}

FC_Font* FontAtlas::DefaultFont()
{
	return _defaultFont;
}

FC_Font* FontAtlas::FindFont(const std::string& ID)
{
	auto Res = _fonts.find(ID);
	if (Res != _fonts.end())
		return Res->second;
	else
		return nullptr;
}

std::unordered_map<std::string, FC_Font*> FontAtlas::_fonts = {};
FC_Font* FontAtlas::_defaultFont = nullptr;

Font::Font(const std::string& Path)
{
	constexpr bool bAllowFlip = false;
	using SpacesType = rectpack2D::empty_spaces<bAllowFlip, rectpack2D::default_empty_spaces>;
	using RectType = rectpack2D::output_rect_t<SpacesType>;

	const int MaxSize = 1024;

	auto ReportSuccess = [](RectType& Rect) {
		return rectpack2D::callback_result::CONTINUE_PACKING;
	};

	auto ReportFailure = [](RectType& Rect) {
		return rectpack2D::callback_result::ABORT_PACKING;
	};

	std::vector<RectType> Rectangles;
	std::u16string Chars = uR"(,./;'[]\<>?:"{}|ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`1234567890-=~!@#$%^&*()_+)";
	TTF_Font* f = TTF_OpenFont(Path.c_str(), 17);
	//TTF_SetFontKerning(f, 0);
	int Height = TTF_FontHeight(f);

	for (auto& a : Chars)
	{
		int Advance = 0;

		if (TTF_GlyphMetrics(f, a, nullptr, nullptr, nullptr, nullptr, &Advance) == -1)
		{
			int sus = 0;
		}
		Rectangles.push_back(RectType(0, 0, Advance, Height));
	}

	auto ResultSize = rectpack2D::find_best_packing<SpacesType>(
		Rectangles,
		rectpack2D::make_finder_input(
			MaxSize,
			-4,
			ReportSuccess,
			ReportFailure,
			rectpack2D::flipping_option::DISABLED
		)
	);

	SDL_Surface* FontSurface = SDL_CreateRGBSurface(0, ResultSize.w, ResultSize.h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000);
	for (size_t Index = 0; Index < Rectangles.size(); Index++)
	{
		auto CurChar = Chars.at(Index);
		auto& CurRect = Rectangles.at(Index);

		SDL_Surface* TempSurf = TTF_RenderGlyph_Solid(f, CurChar, { 255, 255, 255, 255 });

		if (TempSurf->w != CurRect.w || TempSurf->h != CurRect.h)
		{
			// Error
		}

		SDL_Rect DestRect = {
			CurRect.x,
			CurRect.y,
			CurRect.w,
			CurRect.h
		};
		SDL_BlitSurface(TempSurf, nullptr, FontSurface, &DestRect);
		SDL_FreeSurface(TempSurf);
		//Rectangles.push_back(RectType(0, 0, Advance, Height));
	}

	//SDL_SaveBMP(FontSurface, "./FontOutput17__.bmp");
	SDL_FreeSurface(FontSurface);
}
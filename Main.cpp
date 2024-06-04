#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_gfxPrimitives_font.h>
#include <SDL2/SDL2_framerate.h>

#include "FontAtlas.h"
#include "Game.h"
#include "TextureAtlas.h"

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	// TODO: [ERROR] coders crux now crashes the engine with "can't read location 0x008" or something.
	// 	   In fact, SFIntermosaic seems to be one of the few if not the only that doesn't.
	Font* TestFont = new Font("./data/font/coders_crux.ttf");

	//FC_SetFilterMode()

	SDL_Surface *RockySurf = SDL_LoadBMP("./data/tex/rocky.bmp");

	//SDL_Window* Win = SDL_CreateWindow(
	//	"Hadal",
	//	SDL_WINDOWPOS_CENTERED,
	//	SDL_WINDOWPOS_CENTERED,
	//	640, //800,
	//	480, //600,
	//	SDL_WINDOW_SHOWN
	//);

	//SDL_Renderer* Ren = SDL_CreateRenderer(Win, -1, SDL_RENDERER_ACCELERATED);
	// Sets viewport too
	//SDL_RenderSetLogicalSize(Ren, 320, 240);
	//SDL_RenderSetScale(Ren, 2.f, 2.f);

	//SDL_Rect VP = { 0, 0, 160, 240 };
	//SDL_RenderGetViewport(Ren, &VP);
	//SDL_RenderSetViewport(Ren, &VP);

	// TODO: Need to create window and renderer state before texture atlas is created
	Window* Win = new Window("Hadal");

	gfxPrimitivesSetFont(gfxPrimitivesFontdata, 8, 8);

	SDL_Renderer* Ren = *Win; //world.GetWindow();
	TextureAtlas::RegisterTexture(Ren, "./data/tex/sub1_16x10.png", "sub1");
	TextureAtlas::RegisterTexture(Ren, "./data/tex/sub2_7x4.png", "sub2");

	TextureAtlas::RegisterTexture(Ren, "./data/tex/grad.png", "grad");
	TextureAtlas::RegisterTexture(Ren, "./data/tex/fadescreen.png", "fade");
	TextureAtlas::RegisterTexture(Ren, "./data/tex/smolscreen.png", "smol");
	TextureAtlas::RegisterTexture(Ren, "./data/tex/circle.png", "circle");
	TextureAtlas::RegisterTexture(Ren, "./data/tex/circle2.png", "circle2");
	TextureAtlas::RegisterTexture(Ren, "./data/tex/crc.png", "crc");
	TextureAtlas::RegisterTexture(Ren, "./data/tex/maskp.png", "p");
	TextureAtlas::RegisterTexture(Ren, "./data/tex/lump.png", "lump");
	
	TextureAtlas::RegisterSurface("./data/tex/rocky.bmp", "surf_rocky1");

	//FontAtlas::RegisterFont(Ren, "./data/font/coders_crux.ttf", 16, { 255, 255, 255, 255 }, TTF_STYLE_NORMAL, "default_bold");
	//FontAtlas::RegisterFont(Ren, "./data/font/coders_crux.ttf", 16, { 255, 255, 255, 255 }, TTF_STYLE_BOLD, "default_bold");
	//FontAtlas::RegisterFont(Ren, "./data/font/quaver.ttf", 8, { 255, 255, 255, 255 }, TTF_STYLE_NORMAL, "default_bold");
	//FontAtlas::RegisterFont(Ren, "./data/font/nokiafc22.ttf", 8, { 255, 255, 255, 255 }, TTF_STYLE_BOLD, "default_bold");
	FontAtlas::RegisterFont(Ren, "./data/font/SFIntermosaic.ttf", 8, { 255, 255, 255, 255 }, TTF_STYLE_BOLD, "default_bold");
	//FontAtlas::RegisterFont(Ren, "./data/font/coders_crux.ttf", 17, { 255, 255, 255, 255 }, TTF_STYLE_BOLD, "default_bold2");
	FontAtlas::SetDefaultFont("default_bold");

	//SDL_Renderer* BufRen = SDL_CreateRenderer(Win,)

	Game world(Win);// (Win);
	world.CreateMap();
	
	FPSmanager FPS;
	SDL_initFramerate(&FPS);
	SDL_setFramerate(&FPS, 60);

	bool bQuit = false;
	while (!bQuit)
	{
		world.Input();
		if (world.HasExitBeenRequested())
			bQuit = true;

		world.Logic();

		world.Draw(Ren);

		SDL_framerateDelay(&FPS);

		//SDL_RenderPresent(Ren);
	}

	//SDL_DestroyRenderer(Ren);
	//SDL_DestroyWindow(Win);

	return 0;
}
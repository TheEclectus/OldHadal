#include "Map.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <noise/noise.h>

#include "TextureAtlas.h"
#include "Util.h"

TerrainBlock::TerrainBlock(const std::vector<SDL_Point>& Points)
{
	int Highest = 999999;
	int Lowest = -999999;

	for (auto CurPoint : Points)
	{
		//_Points.push_back(CurPoint);
		_Shape.PushPoint(CurPoint);

		//_XPoints.push_back(CurPoint.x);
		//_YPoints.push_back(CurPoint.y);

		if (CurPoint.y < Highest)
			Highest = CurPoint.y;
		
		if (CurPoint.y > Lowest)
			Lowest = CurPoint.y;
	}

	_InclusionRect.y = Highest;
}

const Polygon& TerrainBlock::Shape() const
{
	return _Shape;
}

void TerrainBlock::Generate(Map& ParentMap, int Width)
{
	std::vector<SDL_Point> Points;

	const double OceanFloorOffset = 2.9;

	int XSteps = (Width) / ParentMap.GetOceanFloorGranularity();
	for (int X = 0; X <= XSteps; X++)
	{
		// Should be total number of XSteps to this point
		const double CurX = static_cast<double>(X);

		// [-1, 1]
		double YVal = ParentMap.GetPerlinValue(OceanFloorOffset + (CurX * ParentMap.GetOceanFloorStep()), 10.1, 1.0);
		// [0, 2]
		YVal += 1.0;
		// [0, 1]
		YVal /= 2.0;
		
		YVal *= ParentMap.GetOceanFloorAmplitude();
		//YVal += 150;	// Additional 300ft down

		const double CurXOffset = static_cast<double>((X * ParentMap.GetOceanFloorGranularity())/* + GetStartingOffset()*/);
		Points.push_back({ static_cast<int>(CurXOffset), static_cast<int>(YVal) });
	}

	int Shallowest = 999999;
	int Deepest = -999999;

	//_Points.clear();
	
	for (auto CurPoint : Points)
	{
		//_Points.push_back(CurPoint);
		_Shape.PushPoint(CurPoint);

		if (CurPoint.y < Shallowest)
			Shallowest = CurPoint.y;

		if (CurPoint.y > Deepest)
			Deepest = CurPoint.y;
	}

	_InclusionRect.y = Shallowest;
	_InclusionRect.h = Deepest - Shallowest;

	// Bottom-left
	//_Points.insert(_Points.begin(), { 0, Deepest });
	// Bottom-right
	//_Points.push_back({ XSteps * ParentMap.GetOceanFloorGranularity(), Deepest });

	// Bottom-right
	_Shape.PushPoint({ XSteps * ParentMap.GetOceanFloorGranularity(), Deepest });
	// Bottom-left
	_Shape.PushPoint({ 0, Deepest });

	_Shape.CloseLoop();
}

void TerrainBlock::Render(SDL_Renderer* Renderer, const SDL_Rect& Cam)
{
	/*SDL_Rect CameraTopLeft = {
		Cam.x,
		Cam.y,
		Cam.w,
		Cam.h
	};*/

	//SDL_Rect CameraTopLeft = Cam

	/*int RelativeTopY = HighestPoint() - CameraTopLeft.y;
	if (relativetopy > 0 && relativetopy < camera.h)
	{
		sdl_setrenderdrawcolor(renderer, 255, 0, 0, 255);
		sdl_renderdrawline(renderer, relativechunkstartx, relativetopy, relativechunkendx, relativetopy);
	}*/
	
	//std::vector<Sint16> xpointsbatch, ypointsbatch;
	std::vector<SDL_Point> PointsBatch;

	//for (int i = 0; i < _Points.size(); i++)
	for (int i = 0; i < _Shape.GetNumPoints(); i++)
	{
		//const SDL_Point& CurPt = _Points[i];
		const SDL_Point& CurPt = _Shape.GetPoints()[i];
		SDL_Point ScreenPoint = {
			CurPt.x - Cam.x,
			CurPt.y - Cam.y
		};

		PointsBatch.push_back(ScreenPoint);
	}
	
	static auto _rocktexture = TextureAtlas::FindSurface("surf_rocky1");
	
	texturedPolygonPts(Renderer, PointsBatch.data(), PointsBatch.size(), _rocktexture->GetTexture(), -Cam.x, Cam.y);
	//polygonRGBAPts(Renderer, PointsBatch.data(), PointsBatch.size(), 255, 0, 0, 255);
	//polygonrgba(renderer, xpointsbatch.data(), ypointsbatch.data(), ypointsbatch.size(), 255, 0, 0, 255);
}

//Chunk::Chunk(Map& Parent, Chunk* PreviousChunk) :
//	_ParentMap(Parent),
//	_PrevChunk(PreviousChunk)
//{
//	if (_PrevChunk == nullptr)
//		_Index = 0u;
//	else
//		_Index = _PrevChunk->_Index + 1;
//}
//
//int Chunk::GetStartingOffset() const
//{
//	return _Index * (Chunk::SIZE - 1);
//}
//
//int Chunk::GetEndOffset() const
//{
//	return GetStartingOffset() + (Chunk::SIZE - 1);
//}
//
//// For now, only generates the topmost layer of the seabed, the rest needs interpolated
//void Chunk::Generate(bool bLastChunk)
//{
//	std::vector<SDL_Point> Points;
//
//	const double OceanFloorOffset = 2.9;
//
//	int XSteps = (Chunk::SIZE - 1) / _ParentMap.GetOceanFloorGranularity();
//	for (int X = 0; X <= XSteps; X++)
//	{
//		// Should be total number of XSteps to this point
//		const double CurX = static_cast<double>(X + (GetStartingOffset() / _ParentMap.GetOceanFloorGranularity()));
//
//		// [-1, 1]
//		double YVal = _ParentMap.GetPerlinValue(OceanFloorOffset + (CurX * _ParentMap.GetOceanFloorStep()), 10.1, 1.0);
//		// [0, 2]
//		YVal += 1.0;
//		// [0, 1]
//		YVal /= 2.0;
//		YVal *= _ParentMap.GetOceanFloorAmplitude();
//		YVal += 300;	// Additional 300ft down
//
//		const double CurXOffset = static_cast<double>((X * _ParentMap.GetOceanFloorGranularity())/* + GetStartingOffset()*/);
//		Points.push_back({ static_cast<int>(CurXOffset), static_cast<int>(YVal) });
//	}
//
//	// Get that last point
//	{
//		double X = static_cast<double>(XSteps) + 0.75;
//		// Should be total number of XSteps to this point
//		const double CurX = static_cast<double>(X + (GetStartingOffset() / _ParentMap.GetOceanFloorGranularity()));
//
//		// [-1, 1]
//		double YVal = _ParentMap.GetPerlinValue(OceanFloorOffset + (CurX * _ParentMap.GetOceanFloorStep()), 10.1, 1.0);
//		// [0, 2]
//		YVal += 1.0;
//		// [0, 1]
//		YVal /= 2.0;
//		YVal *= _ParentMap.GetOceanFloorAmplitude();
//		//YVal += 300;	// Additional 300ft down
//
//		const double CurXOffset = static_cast<double>((X * _ParentMap.GetOceanFloorGranularity()) /*+ GetStartingOffset()*/);
//		Points.push_back({ static_cast<int>(CurXOffset), static_cast<int>(YVal) });
//	}
//
//	// Add control points for chunk
//	Points.insert(Points.begin(), { 0, 600 });	// Bottom-left
//	Points.push_back({ 255, 600 });	// Bottom-right
//	//Points.push_back({ 0, 600 });	// Close the loop
//
//	TerrainBlock Block(Points);
//	Block.Generate(_ParentMap, 1024);
//
//	_TerrainBlocks.push_back(std::move(Block));
//}
//
//void Chunk::Render(SDL_Renderer* Renderer, SDL_Rect Camera)
//{
//	SDL_Rect CameraTopLeft = {
//		Camera.x - (Camera.w / 2),
//		Camera.y - (Camera.h / 2),
//		Camera.w,
//		Camera.h
//	};
//
//	int ChunkStartX = GetStartingOffset();
//	int ChunkEndX = GetEndOffset();
//	
//	int RelativeChunkStartX = ChunkStartX - CameraTopLeft.x;
//	int RelativeChunkEndX = ChunkEndX - CameraTopLeft.x;
//
//	/*if (ChunkStartX >= CameraTopLeft.x || ChunkStartX <= (CameraTopLeft.x + CameraTopLeft.w))
//	{
//		SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
//		SDL_RenderDrawLine(Renderer, RelativeChunkStartX, 0, RelativeChunkStartX, CameraTopLeft.h);
//	}
//
//	if (ChunkEndX >= CameraTopLeft.x || ChunkEndX <= (CameraTopLeft.x + CameraTopLeft.w))
//	{
//		SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
//		SDL_RenderDrawLine(Renderer, RelativeChunkEndX, 0, RelativeChunkEndX, CameraTopLeft.h);
//	}*/
//
//	for (auto& CurTerrainBlock : _TerrainBlocks)
//	{
//		int RelativeTopY = CurTerrainBlock.HighestPoint() - CameraTopLeft.y;
//		if (RelativeTopY > 0 && RelativeTopY < Camera.h)
//		{
//			SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
//			SDL_RenderDrawLine(Renderer, RelativeChunkStartX, RelativeTopY, RelativeChunkEndX, RelativeTopY);
//		}
//
//		std::vector<Sint16> XPointsBatch, YPointsBatch;
//
//		for (int i = 0; i < CurTerrainBlock.XPoints().size(); i++)
//		{
//			int XPos = CurTerrainBlock.XPoints()[i] + RelativeChunkStartX; // +(Camera.w / 2);
//
//			int YCrest = CurTerrainBlock.YPoints()[i] - CameraTopLeft.y;	// 300 px down
//
//			XPointsBatch.push_back(XPos);
//			YPointsBatch.push_back(YCrest);
//		}
//		//_RockTexture->GetTexture(), -Camera.x, Camera.y
//		static auto _RockTexture = TextureAtlas::FindSurface("surf_rocky1");
//		texturedPolygon(Renderer, XPointsBatch.data(), YPointsBatch.data(), YPointsBatch.size(), _RockTexture->GetTexture(), -Camera.x, Camera.y);
//		//polygonRGBA(Renderer, XPointsBatch.data(), YPointsBatch.data(), YPointsBatch.size(), 255, 0, 0, 255);
//	}
//
//}

const SDL_Color Map::_GetSkyColor() const
{
	const SDL_Color SkyColor = { 131, 201, 239, 255 };
	return SkyColor;
}

const SDL_Color Map::_GetOceanColor(int Depth) const
{
	const int HadalDepth = 1200;
	const SDL_Color SurfaceCol = { 104, 160, 191, 255 };
	const SDL_Color HadalCol = { 0, 0, 0, 255 };

	float Dividend =
		static_cast<float>(Depth - 100); // First 200 pixels don't affect sky color

	float Divisor = static_cast<float>(HadalDepth);

	float ColorPercent = Dividend / Divisor;

	return Util::Lerp(SurfaceCol, HadalCol, ColorPercent);
}

Map::Map(Game& ParentGame, int Seed) :
	_parentGame(ParentGame),
	_camera(0, 0)
{
	_perlin.SetSeed(Seed);

	//auto testChunk = new Chunk(*this, nullptr);
	//testChunk->Generate();

	//auto testChunk2 = new Chunk(*this, testChunk);
	//testChunk2->Generate();

	//_chunkList.push_back(testChunk);
	//_chunkList.push_back(testChunk2);

	auto terrainBlock1 = new TerrainBlock({});
	terrainBlock1->Generate(*this, 256 * 4);
	_terrainBlockList.push_back(terrainBlock1);

	//_RockTexture = SDL_LoadBMP("./data/tex/rocky.bmp");
	_RockTexture = TextureAtlas::FindSurface("surf_rocky1");

	/*_player = new Player(_EntityRegistry);
	auto s = _player->GetComponent<Component_Position>();*/

	_player = Entity(_EntityRegistry);
	auto& Pos = _player.Handle().emplace<Component_Position>();
	auto& Phys = _player.Handle().emplace<Component_Physics>();
	Phys.VelX = 0.f;
	Phys.VelY = 0.f;
	Phys.VelXMax = 25.f;
	Phys.VelYMax = 25.f;
}

double Map::GetPerlinValue(double X, double Y, double Z) const
{
	return _perlin.GetValue(X, Y, Z);
}

int Map::GetOceanFloorGranularity() const
{
	return _OceanFloorGranularity;
}

int Map::GetOceanFloorAmplitude() const
{
	return _OceanFloorAmplitude;
}

double Map::GetOceanFloorStep() const
{
	return _OceanFloorStep;
}

Camera& Map::GetCamera()
{
	return _camera;
}

//Player* Map::GetControlledPlayer()
//{
//	return _player;
//}

Entity Map::GetControlledPlayer()
{
	return _player;
}

std::vector<TerrainBlock*>& Map::GetTerrainBlocks()
{
	return _terrainBlockList;
}

void Map::SetViewport(int Width, int Height)
{
	_camera.SetSize(Width, Height);
}

void Map::Update(Uint32 TimestepMS)
{
	_SystemPos.Update(_EntityRegistry, TimestepMS);

	// Camera update
	_camera.SetPosition(*_player.GetComponent<Component_Position>());
}

void Map::Generate(int Width, int Height)
{
	// TODO: Relegate generation algo to a class?
	_Width = Width;
	_Height = Height;

	noise::module::Perlin perlin;
	perlin.SetSeed(8008);

	int XSteps = (_Width / _OceanFloorGranularity);
	// [0, N] where N*granularity = Width
	for (int X = 0; X <= XSteps; X++)
	{
		// Big curves
		// [-1, 1]
		double Val1 = perlin.GetValue(2.9 + (static_cast<double>(X) * _OceanFloorStep), 10.1, 1.0);
		// [0, 2]
		Val1 += 1.0;
		// [0, 1]
		Val1 /= 2.0;

		_XPoints.push_back(X * _OceanFloorGranularity);
		_YPoints.push_back(Val1 * _OceanFloorAmplitude);
	}
}

// Camera x and y are the CENTER of the camera!!!
void Map::Draw(SDL_Renderer* Renderer/*, const Camera& Cam*/ /*SDL_Rect Camera*/)
{
	// HACK
	/*SDL_Point PlayerPos = _player->GetPosition();
	Camera.x = PlayerPos.x;
	Camera.y = PlayerPos.y;*/

	const Camera& Cam = _camera;

	// Depth vs camera pos: camera can shake, slide, etc.
	// Only use camera pos for perspective tricks. Depth should be favored elsewise.
	//SDL_Point CameraTopLeft = { Camera.x - (Camera.w / 2), Camera.y - (Camera.h / 2) };
	SDL_Point CameraTopLeft = Cam.TopLeft();

	// TODO: [cool] have a "surface" effect as you approach the surface (perspective)

	// 1. Set the screen to the dominant ocean color.
	// TODO: Ocean color uses Camera position for now! BAAAAAAD
	SDL_Color OceanColor = _GetOceanColor(/*Camera.y*/ Cam.Position().y);
	SDL_SetRenderDrawColor(Renderer, OceanColor.r, OceanColor.g, OceanColor.b, OceanColor.a);
	SDL_RenderClear(Renderer);


	// 2. Fill the top of the screen with an appropriately-colored sky, if it's visible.
	if (CameraTopLeft.y < 0)
	{
		SDL_Color SkyColor = _GetSkyColor();
		SDL_SetRenderDrawColor(Renderer, SkyColor.r, SkyColor.g, SkyColor.b, SkyColor.a);

		int SkyHeight = -CameraTopLeft.y;
		SDL_Rect SkyRect = { 0, 0, /*Camera.w*/ Cam.Viewport().w, SkyHeight };
		SDL_RenderFillRect(Renderer, &SkyRect);
	}
	

	// 3. Draw base at pos for shiggles
	static SDL_Texture* BaseTexture = nullptr;
	if (!BaseTexture)
	{
		SDL_Surface* BaseSurface = IMG_Load("./data/tex/base1_26x16.png");
		if (BaseSurface)
		{
			BaseTexture = SDL_CreateTextureFromSurface(Renderer, BaseSurface);
			SDL_FreeSurface(BaseSurface);
		}
	}

	SDL_Rect BasePos = { 20 - CameraTopLeft.x - 13, 180 - CameraTopLeft.y - 16, 26, 16 };
	SDL_RenderCopy(Renderer, BaseTexture, nullptr, &BasePos);

	static SDL_Texture* StrutTex = nullptr;
	if (!StrutTex)
	{
		SDL_Surface* StrutSurf = IMG_Load("./data/tex/strut1_6x16.png");
		if (StrutSurf)
		{
			StrutTex = SDL_CreateTextureFromSurface(Renderer, StrutSurf);
			SDL_FreeSurface(StrutSurf);
		}
	}

	for (int i = 0; i < 10; i++)
	{
		SDL_Rect LeftLeg = { BasePos.x, BasePos.y + 16 + (16*i), 6, 16 };
		SDL_Rect RightLeg = { BasePos.x + 26 - 6, BasePos.y + 16 + (16*i), 6, 16 };
		SDL_RenderCopy(Renderer, StrutTex, nullptr, &LeftLeg);
		SDL_RenderCopy(Renderer, StrutTex, nullptr, &RightLeg);
	}
	
	


	// 4. Draw rocky ocean bottom
	// Get index of first point to look for
	//int FirstIndex = (CameraTopLeft.x / _OceanFloorGranularity) - 1;	// Should get off the left side of screen
	//if (FirstIndex < 0)
	//	FirstIndex = 0;
	//int LastIndex = FirstIndex + 1 + (Camera.w / _OceanFloorGranularity) + 1;
	//if (LastIndex > _YPoints.size() - 1)
	//	LastIndex = _YPoints.size() - 1;
	//// Sweep from left to right and get floor points
	//std::vector<Sint16> XPointsBatch, YPointsBatch;
	//// Bottom-left side first
	//if(FirstIndex > 0)
	//	XPointsBatch.push_back(0);
	//else
	//	XPointsBatch.push_back(-CameraTopLeft.x);
	//YPointsBatch.push_back(Camera.h);
	//for (int i = FirstIndex; i <= LastIndex; i++)
	//{
	//	int XPos = _XPoints[i] - CameraTopLeft.x; // +(Camera.w / 2);
	//	int YCrest = (Camera.h - 1) - _YPoints[i] - CameraTopLeft.y + 300;	// 300 px down
	//	XPointsBatch.push_back(XPos);
	//	YPointsBatch.push_back(YCrest);
	//}
	//// Bottom-right side too
	//XPointsBatch.push_back(Camera.w - 1);
	//YPointsBatch.push_back(Camera.h);

	/*SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
	for (int i = 0; i < XPointsBatch.size(); i++)
		SDL_RenderDrawPoint(Renderer, XPointsBatch[i], YPointsBatch[i]);*/

	//filledPolygonRGBA(Renderer, XPointsBatch.data(), YPointsBatch.data(), YPointsBatch.size(), 255, 0, 0, 255);

	// 4a. Set rock shading
	SDL_Color RockLerp = Util::Lerp({ 255, 255, 255, 255 }, { 0,0,0,255 }, static_cast<float>(/*Camera.y*/ Cam.Position().y - 100) / 1200.f);
	SDL_SetSurfaceColorMod(_RockTexture->GetTexture(), RockLerp.r, RockLerp.g, RockLerp.b);
	// TODO: [cool] inverted y delta fives a pretty cool "falling sand" effect
	//texturedPolygon(Renderer, XPointsBatch.data(), YPointsBatch.data(), YPointsBatch.size(), _RockTexture->GetTexture(), -Camera.x, Camera.y);

	/*for (auto CurChunk : _chunkList)
	{
		CurChunk->Render(Renderer, Camera);
	}*/

	for (auto CurBlock : _terrainBlockList)
	{
		CurBlock->Render(Renderer, Cam.WorldRect());
	}

	static auto SubTexture = TextureAtlas::FindTexture("sub2");
	//SDL_Rect SubRect = { (Camera.w / 2) - (SubTexture->Width()/2), (Camera.h / 2) - (SubTexture->Height() / 2), SubTexture->Width(), SubTexture->Height() };
	SDL_Rect SubRect = { (Cam.Viewport().w / 2) - (SubTexture->Width()/2), (Cam.Viewport().h / 2) - (SubTexture->Height() / 2), SubTexture->Width(), SubTexture->Height() };
	
	SDL_RendererFlip Flip = signbit(_player.GetComponent<Component_Physics>()->VelX) ? SDL_RendererFlip::SDL_FLIP_HORIZONTAL : SDL_RendererFlip::SDL_FLIP_NONE;

	SDL_RenderCopyEx(Renderer, SubTexture->GetTexture(), nullptr, &SubRect, 0.0, nullptr, Flip);
	//SDL_Rect SubRect = { (Camera.w / 2) - 8, (Camera.h / 2) - 5, 16, 10 };

	/*static SDL_Texture* SubTexture = nullptr;
	if(!SubTexture)
	{
		SDL_Surface* SubSurface = IMG_Load("./data/tex/sub1_16x10.png");
		if (SubSurface)
		{
			SubTexture = SDL_CreateTextureFromSurface(Renderer, SubSurface);
			SDL_FreeSurface(SubSurface);
		}
	}

	SDL_Rect SubRect = { (Camera.w / 2) - 8, (Camera.h / 2) - 5, 16, 10 };
	SDL_RenderCopy(Renderer, SubTexture->GetTexture(), nullptr, &SubRect);
	*/

	//SDL_RenderPresent(Renderer);
}
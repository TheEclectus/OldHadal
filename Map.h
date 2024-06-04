#pragma once

#include <vector>

#include <entt/entt.hpp>

#include <noise/noise.h>

#include <SDL2/SDL.h>

// Naming will need addressed, conflicts with a lot of variable names
//#include "Camera.h"
#include "Player.h"
#include "Polygon.h"
#include "System_Position.h"
#include "TextureAtlas.h"

class TerrainBlock;
class Chunk;
class Map;

// Game.h
class Game;

/// <summary>
/// The vertices needed to assemble a single piece of terrain that fits inside of a chunk
/// </summary>
// TODO: wide bounding box? To test whether it's even worth hit testing/raytracing/etc
// TODO: Maxima and minima of where a line could possibly intersect should define the range of points to check
class TerrainBlock
{
private:
	// Positions are all relative to the parent chunk's starting offset
	// Maximum x = (Chunk::SIZE - 1)
	// TODO: [worldgen] If not the last chunk laterally, perhaps smooth nearby points?
	// Do lookahead to point after last increment and interpolate
	// Stored at Sint16 for convenience's sake; VALUES STILL LATCHED TO [0, Chunk::SIZE]
	//std::vector<Sint16> _XPoints;
	//std::vector<Sint16> _YPoints;
	
	//std::vector<SDL_Point> _Points;

	Polygon _Shape;
	SDL_Rect _InclusionRect = { -1,-1,-1,-1 }, _ExclusionRect = { -1,-1,-1,-1 };

public:
	TerrainBlock(const std::vector<SDL_Point>& Points);
	const Polygon& Shape() const;
	size_t PointsCount() const;
	const std::vector<SDL_Point> Points() const;
	//const std::vector<Sint16>& XPoints() const;
	//const std::vector<Sint16>& YPoints() const;

	void Generate(Map& ParentMap, int Width);
	// Might be a hack? Eventually this should probably just be handled
	// by the map, and TerrainBlock just stores the data
	void Render(SDL_Renderer* Renderer, const SDL_Rect& Cam);

	// HACK
	int HighestPoint() { return _InclusionRect.y; }
};

/*
class Chunk
{
private:
	Map& _ParentMap;
	Chunk* _PrevChunk = nullptr, *_NextChunk = nullptr;
	std::vector<TerrainBlock> _TerrainBlocks;

	// _Index * (Size-1) == starting offset
	// Index should be assigned on creation, relative to previous chunk
	int _Index = -1;

public:
	// if Prev is null, it's the first chunk.
	// Not generated 
	Chunk(Map& Parent, Chunk* PreviousChunk = nullptr);
	int GetStartingOffset() const;	// _Index * (SIZE-1) == starting offset
	int GetEndOffset() const;

	// TODO: set _NextChunk to first in map if bLastChunk is true
	void Generate(bool bLastChunk = false);

	// CAMERA IS CENTERED
	void Render(SDL_Renderer* Renderer, SDL_Rect Camera);

	static const int SIZE = 256;
};*/

/// <summary>
/// Represents the game map.
/// </summary>
class Map
{
private:
	Game& _parentGame;

	// Store relative perlin noise generation data
	noise::module::Perlin _perlin;

	// Maybe store loaded chunks here?
	//LoadedChunkList
	//std::vector<Chunk*> _chunkList;
	std::vector<TerrainBlock*> _terrainBlockList;

	// Size in pixels
	int _Width = 0, _Height = 0;
	// How far apart to generate the floor points in pixels
	int _OceanFloorGranularity = 4;
	int _OceanFloorAmplitude = 240;
	double _OceanFloorStep = 0.005;

	// TODO: Split into chunks? Split chunks into layers?
	std::vector<int> _XPoints, _YPoints;

	entt::registry _EntityRegistry;
	System_Position _SystemPos;

	Camera _camera;
	// Player *_player;
	Entity _player;

	// TODO: [multiplayer] Player pool goes here

	//SDL_Texture* _SubTexture = nullptr;
	//SDL_Surface* _RockTexture = nullptr;
	Texture<SDL_Texture>* _SubTexture = nullptr;
	Texture<SDL_Surface>* _RockTexture = nullptr;

	const SDL_Color _GetOceanColor(int Depth) const;

	// TODO: Maybe change this based on time of day?
	const SDL_Color _GetSkyColor() const;

public:
	Map(Game& ParentGame, int Seed);

	// Map generation functions
	double GetPerlinValue(double X, double Y, double Z) const;
	int GetOceanFloorGranularity() const;
	int GetOceanFloorAmplitude() const;
	double GetOceanFloorStep() const;

	Camera& GetCamera();
	/// <returns>The Player instance owned by the current user.</returns>
	//Player* GetControlledPlayer();
	Entity GetControlledPlayer();
	std::vector<TerrainBlock*>& GetTerrainBlocks();

	void SetViewport(int Width, int Height);

	// Update entities based on their systems
	void Update(Uint32 TimestepMS);
	void Generate(int Width, int Height);
	void Draw(SDL_Renderer *Renderer /*const Camera& Cam*/ /*SDL_Rect Camera*/);
};

/*
*	Cave system will most likely operate in layers, so there should eventually be a 
*	series of top and bottom points for each, and will be filled in logically between those.
*	(i.e., if top point is visible but bottom isn't, set it to bottom of the screen instead,
*	maybe optimize and make sequential non-visible points a single line)
*/
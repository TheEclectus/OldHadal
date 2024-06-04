#pragma once

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

template<class T>
class Texture
{
	friend class TextureAtlas;

private:
	T* _texture = nullptr;
	int _textureWidth = 0;
	int _textureHeight = 0;
	std::string _id = "";
	std::string _path = "";

	Texture(const std::string& ID, const std::string& Path, void*);
	void _FreeResources();
public:
	~Texture();
	// Returns true if it points to a valid resource
	operator bool() const;

	T* GetTexture();
	int Height() const;
	int Width() const;
	void GetSize(int& Width, int& Height) const;
	const std::string& ID() const;
	const std::string& Path() const;
};

class TextureAtlas
{
private:
	static std::unordered_map<std::string, Texture<SDL_Texture>*> _textures;
	static std::unordered_map<std::string, Texture<SDL_Surface>*> _surfaces;
public:
	static void RegisterTexture(SDL_Renderer* Renderer, const std::string& Path, const std::string& ID);
	static void RegisterSurface(const std::string& Path, const std::string& ID);

	static Texture<SDL_Texture>* FindTexture(const std::string& ID);
	static Texture<SDL_Surface>* FindSurface(const std::string& ID);
};
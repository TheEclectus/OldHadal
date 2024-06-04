#include "TextureAtlas.h"

// Generics first
template<class T>
Texture<T>::~Texture()
{
	_FreeResources();
}

template<class T>
Texture<T>::operator bool() const
{
	return _texture != nullptr;
}

template<class T>
T* Texture<T>::GetTexture()
{
	return _texture;
}

template<class T>
int Texture<T>::Height() const
{
	return _textureHeight;
}

template<class T>
int Texture<T>::Width() const
{
	return _textureWidth;
}

template<class T>
void Texture<T>::GetSize(int& Width, int& Height) const
{
	Width = _textureWidth;
	Height = _textureHeight;
}

template<class T>
const std::string& Texture<T>::ID() const
{
	return _id;
}

template<class T>
const std::string& Texture<T>::Path() const
{
	return _path;
}

// SDL_Texture specialization
Texture<SDL_Texture>::Texture(const std::string& ID, const std::string& Path, void* RendererPtr) :
	_id(ID),
	_path(Path)
{
	SDL_Surface* TempSurf = IMG_Load(Path.c_str());
	if (TempSurf == nullptr)
	{
		// There's been an error; stop everything
		// TODO: maybe grab a copy of the error from IMG_GetError()
	}
	else
	{
		_textureWidth = TempSurf->w;
		_textureHeight = TempSurf->h;
		SDL_Renderer* Renderer = static_cast<SDL_Renderer*>(RendererPtr);
		_texture = SDL_CreateTextureFromSurface(Renderer, TempSurf);
		if (!_texture)
		{
			// There's been an error; stop everything
			// TODO: maybe grab a copy of the error from IMG_GetError()
		}
		else
		{
			// Success!
			SDL_FreeSurface(TempSurf);
		}
	}
}

void Texture<SDL_Texture>::_FreeResources()
{
	if (_texture)
	{
		SDL_DestroyTexture(_texture);
		_texture = nullptr;
	}
}

// SDL_Surface specialization
Texture<SDL_Surface>::Texture(const std::string& ID, const std::string& Path, void*) :
	_id(ID),
	_path(Path)
{
	_texture = IMG_Load(Path.c_str());
	if (_texture == nullptr)
	{
		// There's been an error; stop everything
		// TODO: maybe grab a copy of the error from IMG_GetError()
	}
	else
	{
		// Success!
		_textureWidth = _texture->w;
		_textureHeight = _texture->h;
	}
}

void Texture<SDL_Surface>::_FreeResources()
{
	if (_texture)
	{
		SDL_FreeSurface(_texture);
		_texture = nullptr;
	}
}



void TextureAtlas::RegisterTexture(SDL_Renderer* Renderer, const std::string& Path, const std::string& ID)
{
	if (_textures.count(ID) > 0)
	{
		// ID already in use
		return;
	}

	Texture<SDL_Texture>* NewTex = new Texture<SDL_Texture>(ID, Path, Renderer);
	if (!(*NewTex))
	{
		// Encountered an error
		delete NewTex;
	}
	else
	{
		_textures.insert({ ID, NewTex });
	}
}

void TextureAtlas::RegisterSurface(const std::string& Path, const std::string& ID)
{
	if (_surfaces.count(ID) > 0)
	{
		// ID already in use
		return;
	}

	Texture<SDL_Surface>* NewTex = new Texture<SDL_Surface>(ID, Path, nullptr);
	if (!(*NewTex))
	{
		// Encountered an error
		delete NewTex;
	}
	else
	{
		_surfaces.insert({ ID, NewTex });
	}
}

Texture<SDL_Texture>* TextureAtlas::FindTexture(const std::string& ID)
{
	auto Res = _textures.find(ID);
	if (Res == _textures.end())
		return nullptr;
	else
		return Res->second;
}

Texture<SDL_Surface>* TextureAtlas::FindSurface(const std::string& ID)
{
	auto Res = _surfaces.find(ID);
	if (Res == _surfaces.end())
		return nullptr;
	else
		return Res->second;
}

template class Texture<SDL_Texture>;
template class Texture<SDL_Surface>;

std::unordered_map<std::string, Texture<SDL_Texture>*> TextureAtlas::_textures = {};
std::unordered_map<std::string, Texture<SDL_Surface>*> TextureAtlas::_surfaces = {};
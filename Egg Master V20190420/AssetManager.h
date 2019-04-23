#pragma once
#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"
#include "SDL_ttf.h"


class AssetManager {
private:
	Manager* manager;

	map<string, SDL_Texture*> textures;
	map<string, TTF_Font*> fonts;

	map<string, Mix_Chunk*> effects;
	map<string, Mix_Music*> playlist;

public:
	AssetManager(Manager* man);
	~AssetManager();

	void CreateProjectile(Vector2D, Vector2D, int, string);

	void addTexture(string, const char*);
	SDL_Texture* getTexture(string);

	void addFont(string, string, int);
	TTF_Font* getFont(string);
};

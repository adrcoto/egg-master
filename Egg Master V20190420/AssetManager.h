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
public:
	AssetManager(Manager* man);
	~AssetManager();

	void CreateProjectile(Vector2D, Vector2D, int, int, string);

	void addTexture(string, const char*);
	SDL_Texture* getTexture(string);

	void addFont(string, string, int);
	TTF_Font* getFont(string);
};

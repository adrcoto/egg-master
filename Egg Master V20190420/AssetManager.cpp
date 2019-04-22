#include "AssetManager.h"
#include "ECS/Components.h"


AssetManager::AssetManager(Manager* man) : manager(man){

}


AssetManager::~AssetManager() {
	cout << "Asset manager  - DECONS";
}

void AssetManager::addTexture(string id, const char* path) {
	textures.emplace(id, TextureManager::loadTexture(path));
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, string id) {
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y);
	projectile.addComponent<SpriteComponent>(id);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>(id);
}

SDL_Texture* AssetManager::getTexture(string id) {
	return textures[id];
}

void AssetManager::addFont(string id, string path, int size) {
	fonts.emplace(id, TTF_OpenFont(path.c_str(), size));
}

TTF_Font* AssetManager::getFont(string id) {
	return fonts[id];
}
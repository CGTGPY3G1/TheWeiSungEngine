#include "AssetManager.h"
#include <iostream>
AssetManager::AssetManager() {
}

AssetManager::~AssetManager() {
}

void AssetManager::LoadTexture(const std::string & filename) {
	sf::Texture texture;
	if(texture.loadFromFile(filename)) textures[filename] = texture;
	else std::cout << "Couldn't load texture - " + filename << std::endl;
}

sf::Texture & AssetManager::GetTexture(const std::string & filename) {
	if(textures.count(filename) == 0)  LoadTexture(filename);
	return textures[filename];
}

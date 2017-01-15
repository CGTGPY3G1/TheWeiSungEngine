#include "AssetManager.h"
#include <iostream>
#include "Debug.h"
AssetManager::AssetManager() {
}

AssetManager & AssetManager::GetInstance() {
	static AssetManager instance;
	return instance;
}

AssetManager::~AssetManager() {
	
}

void AssetManager::LoadTexture(const std::string & filename, const bool & generateMipMaps, const bool & setSmooth, const bool & setSRGB) {
	sf::Texture texture;
	
	if(texture.loadFromFile(filename)) {
		if(setSmooth) texture.setSmooth(setSmooth);
		if(setSRGB) texture.setSrgb(setSRGB);
		if(generateMipMaps) texture.generateMipmap();
		textures[filename] = texture;
	}
	else Debug::GetInstance().PrintImmediately("Couldn't load texture - " + filename, DebugMessageType::DEBUG_TYPE_FAILURE_MEDIUM);
}

sf::Texture & AssetManager::GetTexture(const std::string & filename, const bool & generateMipMaps, const bool & setSmooth, const bool & setSRGB) {
	if(textures.count(filename) == 0)  LoadTexture(filename, generateMipMaps, setSmooth, setSRGB);
	return textures[filename];
}

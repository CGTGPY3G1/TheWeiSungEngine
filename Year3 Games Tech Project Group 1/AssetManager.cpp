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
		if(filename.compare("Images/Buildings.png") == 0) texture.setSmooth(false);
		else if (setSmooth) texture.setSmooth(setSmooth);
		if(setSRGB) texture.setSrgb(setSRGB);
		if(generateMipMaps) texture.generateMipmap();
		textures[filename] = texture;
	}
	else Debug::GetInstance().PrintImmediately("Couldn't load texture - " + filename, DebugMessageType::DEBUG_TYPE_FAILURE_MEDIUM);
}

void AssetManager::LoadSoundBuffer(const std::string & filename) {
	sf::SoundBuffer buffer;

	if(buffer.loadFromFile(filename)) {
		soundBuffers[filename] = buffer;
	}
	else Debug::GetInstance().PrintImmediately("Couldn't load Audio - " + filename, DebugMessageType::DEBUG_TYPE_FAILURE_MEDIUM);
}

sf::Texture & AssetManager::GetTexture(const std::string & filename, const bool & generateMipMaps, const bool & setSmooth, const bool & setSRGB) {
	if(textures.count(filename) == 0)  LoadTexture(filename, generateMipMaps, setSmooth, setSRGB);
	return textures[filename];
}

sf::SoundBuffer & AssetManager::GetSoundBuffer(const std::string & filename) {
	if(soundBuffers.count(filename) == 0)  LoadSoundBuffer(filename);
	return soundBuffers[filename];
}

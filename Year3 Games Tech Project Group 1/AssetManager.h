#pragma once
#ifndef WS_ASSET_MANAGER_H
#define WS_ASSET_MANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <memory>
class AssetManager {
public:
	static AssetManager & GetInstance();
	~AssetManager();
	void LoadTexture(const std::string & filename, const bool & generateMipMaps = true, const bool & setSmooth = false, const bool & setSRGB = false);
	sf::Texture & GetTexture(const std::string & filename, const bool & generateMipMaps = true, const bool & setSmooth = false, const bool & setSRGB = false);
protected:
	std::map<std::string, sf::Texture> textures;
	AssetManager();
};

#endif // !WS_ASSET_MANAGER_H
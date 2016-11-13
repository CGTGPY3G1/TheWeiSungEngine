#pragma once
#ifndef WS_ASSET_MANAGER_H
#define WS_ASSET_MANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <memory>
class AssetManager : public std::enable_shared_from_this<AssetManager> {
public:
	AssetManager();
	~AssetManager();
	void LoadTexture(const std::string & filename);
	sf::Texture & GetTexture(const std::string & filename);
private:
	std::weak_ptr<AssetManager> GetWeak() { return shared_from_this(); }
	std::map<std::string, sf::Texture> textures;
};

#endif // !WS_ASSET_MANAGER_H
#pragma once
#ifndef WS_TILE_MAPPER_H
#define WS_TILE_MAPPER_H
#include "ParsedTMX.h"
#include "Component.h"
#include "Graphics.h"

struct Tile {
	unsigned int id = -1;
	sf::Sprite sprite;
	std::vector<sf::Vertex> verts;
};

class Transform2D;
class TileMapper : public Component {
public:
	TileMapper();
	TileMapper(std::weak_ptr<GameObject> gameObject);
	~TileMapper();
	const ComponentType Type() const override { return COMPONENT_TILE_MAPPER; }
	void Init(const std::string & fileName, const std::string & TilesetName);
	const std::string GetName() const override { return "TileMapper"; }

	template <class Archive>
	void load(Archive & ar) {

	}

	template <class Archive>
	void save(Archive & ar) const {
		Component::save(ar);
		
	}
	bool LoadTmxMap(const std::string & xml, const std::string & tilesetName);
	void ProcessTmxBuildingGroup(std::shared_ptr<TmxGroup> group);
	void ProcessTmxTileLayer(std::shared_ptr<TmxTileset> tileset, std::shared_ptr<TmxLayer> layer, sf::Texture & texture);
	void ProcessCharacters(std::shared_ptr<TmxGroup> group);
	bool IsValid();
	void Draw();
private:

	std::shared_ptr<TmxMap> map;
	std::vector<std::vector<Tile>> tiles;
	std::weak_ptr<Transform2D> myTransform;
	sf::RenderStates renderStates;
};
#endif // !WS_TILE_MAPPER_H

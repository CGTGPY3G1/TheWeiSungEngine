#pragma once
#ifndef WS_TILE_MAPPER_H
#define WS_TILE_MAPPER_H
#include "ParsedTMX.h"
#include "Component.h"
#include "Graphics.h"

struct NavInfo {

	unsigned int x = 0, y = 0;
	int cost = 1;
	int priority = 0;
	std::vector<NavInfo *> neighbours;
	bool operator == (const NavInfo& other) const { return x == other.x && y == other.y; }
	bool operator == (const NavInfo* other) const { return x == other->x && y == other->y; }
	bool operator()(const NavInfo& lhs, const NavInfo& rhs) const { return lhs.priority < rhs.priority; }
};

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
	std::vector<std::pair<int, int>> GetPath(NavInfo * current, NavInfo * destination);

	static int GetHeuristic(NavInfo * current, NavInfo * destination);
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
	void ProcessVehicles(std::shared_ptr<TmxGroup> group);
	bool IsValid();
	void Draw();
private:
	Vector2 GetBuildingScale(const unsigned int & buildingType, const float & width, const float & height);
	std::shared_ptr<TmxMap> map;
	std::vector<std::vector<Tile>> tiles;
	std::weak_ptr<Transform2D> myTransform;
	sf::RenderStates renderStates;
};
#endif // !WS_TILE_MAPPER_H

#pragma once
#ifndef WS_TILE_MAPPER_H
#define WS_TILE_MAPPER_H
#include "ParsedTMX.h"
#include "Component.h"
#include "Graphics.h"
enum TileType {
	BLOCKED = -1000,
	WALKABLE = -500,
	TILE_TYPE_NULL = -1,
	TILE_TYPE_WATER = 0,
	TILE_TYPE_GRASS = 1,
	TILE_TYPE_SAND = 2,
	TILE_TYPE_PAVEMENT = 3,
	TILE_TYPE_ROAD = 4,
	TILE_TYPE_WATER_SAND = 5,
	TILE_TYPE_WATER_GRASS = 6,
	TILE_TYPE_SAND_GRASS = 7,
	TILE_TYPE_ROAD_DIVIDER = 8,
	TILE_TYPE_PAVEMENT_ROAD = 9,
	TILE_TYPE_PAVEMENT_GRASS = 10,
	TILE_TYPE_PAVEMENT_WATER = 11,
	MAX_TILE_TYPES
};


static const std::string tileTypeNames [] = { "Water", "Grass", "Sand", "Pavement", "Road", "Water and Sand", 
											"Water and Grass", "Sand and Grass", "Road Divider", "Pavement and Road", 
											"Pavement and Grass", "Pavement and Water" };

static const float forceScaleTable [] = { 0.0f, 0.8f, 0.7f, 0.95f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.9f, 0.875f, 0.5f };
struct GridLocation {
	GridLocation(const int & nX = 0, const int & nY = 0) : x(nX), y(nY) {}
	int x = 0, y = 0;
	bool operator == (const GridLocation& other) const { return x == other.x && y == other.y; }
	void operator = (const GridLocation& other) { x = other.x; y = other.y; }
	bool operator()(const GridLocation& other) const { return x == other.x && y == other.y; }
};

struct Edge;
struct IsAtLocation;
struct NavInfo {
	NavInfo(const Vector2 & position, const GridLocation & location) : worldPosition(position), gridLocation(location), processed(false){}
	Vector2 worldPosition;
	GridLocation gridLocation;
	std::vector<Edge> edges;
	bool processed = false;
	bool AddEdge(const Edge & edge) {
		bool toReturn = true;
		if(std::find(edges.begin(), edges.end(), edge) == edges.end()) edges.push_back(edge);
		else toReturn = false;
		return toReturn;
	}

	bool operator == (const NavInfo & other) {
		return gridLocation == other.gridLocation;
	}

	bool operator == (const std::shared_ptr<NavInfo> & other) {
		return gridLocation == other->gridLocation;
	}
};

struct Edge {
	std::shared_ptr<NavInfo> linked;
	bool operator == (const Edge & other) {
		return linked == other.linked;
	}

};

struct Tile {
	unsigned int id = -1;
	sf::Sprite sprite;
	std::vector<sf::Vertex> verts;
	TileType type = TileType::TILE_TYPE_NULL;
	float forceScale = 1.0f;
	Vector2 worldPosition;
	std::shared_ptr<NavInfo> navInfo;
	GridLocation closestNode = GridLocation(-1, -1);
	Tile() : closestNode(GridLocation(-1, -1)) {}
};

struct IsAtLocation {
	IsAtLocation(const GridLocation & i) : location(i) {}
	bool operator()(std::shared_ptr<NavInfo> l) { return l->gridLocation == location; }
private:
	GridLocation location;
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
	void ProcessTmxBarrierGroup(std::shared_ptr<TmxGroup> group);
	void ProcessTmxTileLayer(std::shared_ptr<TmxTileset> tileset, std::shared_ptr<TmxLayer> layer, sf::Texture & texture);
	void ProcessCharacters(std::shared_ptr<TmxGroup> group);
	void ProcessVehicles(std::shared_ptr<TmxGroup> group);
	void ProcessNavigationLayer(std::shared_ptr<TmxGroup> group);
	Vector2 IndexToWorld(const int & index);
	const GridLocation IndexToGrid(const int & index);
	const Vector2 GridToWorld(const GridLocation & gridLocation);
	bool IsOnGrid(const GridLocation & gridLocation);
	bool IsOnGrid(const int & x, const int & y);
	const GridLocation WorldToGrid(const Vector2 & worldPosition);
	const TileType GetTileType(const Vector2 & worldPosition);
	const std::string GetTileTypeAsString(const Vector2 & worldPosition);
	const float GetTileForceScale(const Vector2 & worldPosition);
	const Tile & GetTile(const Vector2 & worldPosition) const;
	void PrintTile(const Vector2 & worldPosition);
	bool IsValid();
	void Draw();
	bool GetShowNavLinks() { return showNavNodes; }
	void SetShowNavLinks(const bool & shouldShow) { showNavNodes = shouldShow; }
	bool GetGridLinks() { return showGridLinks; }
	void SetGridLinks(const bool & shouldShow) { showGridLinks = shouldShow; }
private:
	TileType TypeFromGID(unsigned int gid);
	Vector2 GetObjectScale(const unsigned int & layerTyoe, const unsigned int & objectType, const float & width, const float & height);
	std::shared_ptr<TmxMap> map;
	std::vector<std::vector<Tile>> tiles;
	std::weak_ptr<Transform2D> myTransform;
	sf::RenderStates renderStates;
	unsigned int tileWidth = 32, tileHeight = 32, width = 0, height = 0;
	float halfWidth = 0, halfHeight = 0;
	Vector2 worldScale = Vector2::One;
	bool showGridLinks = false, showNavNodes = false;
};
#endif // !WS_TILE_MAPPER_H

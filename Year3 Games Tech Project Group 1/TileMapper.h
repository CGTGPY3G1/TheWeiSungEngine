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
static const std::string tileTypeNames [] = {"Water", "Grass", "Sand", "Pavement", "Road", "Water and Sand", 
											"Water and Grass", "Sand and Grass", "Road Divider", "Pavement and Road", 
											"Pavement and Grass", "Pavement and Water"};
static const float forceScaleTable [] = {0.05f, 0.8f, 0.7f, 0.95f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.9f, 0.875f, 0.5f };
struct GridLocation {
	GridLocation(const int & nX = 0, const int & nY = 0) : x(nX), y(nY) {}
	int x = 0, y = 0;
	bool operator == (const GridLocation& other) const { return x == other.x && y == other.y; }
	bool operator()(const GridLocation& other) const { return x == other.x && y == other.y; }
};

namespace std {
	template <>
	struct hash<GridLocation> : public unary_function<GridLocation, int> {
		size_t operator()(const GridLocation& value) const {
			return value.x ^ (value.y << 1);
		}
	};
	template <>
	struct equal_to<GridLocation> : public unary_function<GridLocation, bool> {
		bool operator()(const GridLocation& lhs, const GridLocation& rhs) const {
			return lhs.x == rhs.x && lhs.y == rhs.y;
		}
	};

} // namespace std

struct NavInfo {
	Vector2 WorldPosition;
	unsigned int x = 0, y = 0;
	GridLocation gridLocation;
	float g = 1, h = 1, c = 0;
	float GetF() { return g + h; }
	const float GetF() const { return g + h; } ;
	int priority = 0;
	bool blocked = false;
	TileType type;
	NavInfo* parent;
	bool inClosed = false, inOpen;

	bool operator == (const NavInfo& other) const { return x == other.x && y == other.y; }
	bool operator == (const NavInfo* other) const { return x == other->x && y == other->y; }

	bool operator()(const NavInfo& lhs, const NavInfo& rhs) const { 
		/*const float dif = (lhs.g + lhs.h) - (rhs.g + rhs.h);
		if(dif > -0.00001f && dif < 0.00001f) return lhs.h < rhs.h;
		return dif > 0;*/
		return (lhs.GetF() < rhs.GetF());
	}
	bool operator()(const NavInfo* lhs, const NavInfo* rhs) const { 
		return (lhs->GetF() < rhs->GetF());
	}
	bool operator < (const NavInfo& rhs) const {
		return (GetF() < rhs.GetF());
	}
	bool operator > (const NavInfo& rhs) const { 
		return (GetF() > rhs.GetF());
	}
	bool operator < (const NavInfo* rhs) const {
		return (GetF() < rhs->GetF());
	}
	bool operator > (const NavInfo* rhs) const {
		return (GetF() > rhs->GetF());
	}
};

struct Tile {
	unsigned int id = -1;
	sf::Sprite sprite;
	std::vector<sf::Vertex> verts;
	TileType type = TileType::TILE_TYPE_NULL;
	float forceScale = 1.0f;
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
	void ProcessVehicles(std::shared_ptr<TmxGroup> group);
	Vector2 IndexToWorld(const int & index);
	const GridLocation IndexToGrid(const int & index);
	const GridLocation WorldToGrid(const Vector2 & worldPosition);
	const TileType GetTileType(const Vector2 & worldPosition);
	const std::string GetTileTypeAsString(const Vector2 & worldPosition);
	const float GetTileForceScale(const Vector2 & worldPosition);
	const Tile & GetTile(const Vector2 & worldPosition) const;
	bool IsValid();
	void Draw();
private:
	TileType TypeFromGID(unsigned int gid);
	Vector2 GetBuildingScale(const unsigned int & buildingType, const float & width, const float & height);
	std::shared_ptr<TmxMap> map;
	std::vector<std::vector<Tile>> tiles;
	std::weak_ptr<Transform2D> myTransform;
	sf::RenderStates renderStates;
	unsigned int tileWidth = 32, tileHeight = 32, width = 0, height = 0;
	float halfWidth = 0, halfHeight = 0;
	Vector2 worldScale = Vector2::One;
};
#endif // !WS_TILE_MAPPER_H

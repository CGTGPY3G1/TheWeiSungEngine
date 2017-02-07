#pragma once
#ifndef WS_TILE_MAPPER_H
#define WS_TILE_MAPPER_H
#include "ParsedTMX.h"
#include "Component.h"
#include "Graphics.h"
enum TileType {
	WALKABLE = 1,
	GRAVEL = 2,
	BLOCKED = 1000
};

struct GridLocation {
	unsigned int x = 0, y = 0;
	bool operator == (const GridLocation& other) const { return x == other.x && y == other.y; }
	bool operator < (const GridLocation& other) const { 
		const unsigned int dif = other.x - x;
		if(dif == 0) return (other.y - y < 0);
		return dif < 0;
	}
	bool operator > (const GridLocation& other) const {
		const unsigned int dif = other.x - x;
		if(dif == 0) return (other.y - y > 0);
		return dif > 0;
	}
	bool operator()(const GridLocation& other) const { 
		const unsigned int dif = other.x - x;
		if(dif == 0) return (other.y - y < 0);
		return dif < 0;
	}
	//bool operator()(const GridLocation& lhs, const GridLocation& rhs) const { return lhs.x == rhs.x && lhs.y == rhs.y; }
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
			return lhs.x == rhs.x && lhs.y == rhs.y;;
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
		const float dif = (lhs.g + lhs.h) - (rhs.g + rhs.h);
		if(dif > -0.00001f && dif < 0.00001f) return lhs.h < rhs.h;
		return dif > 0;
	}
	bool operator()(const NavInfo* lhs, const NavInfo* rhs) const { 
		return (lhs->x + lhs->y) < (rhs->x + rhs->y);
	}
	bool operator < (const NavInfo& rhs) const {
		return (x + y) < (rhs.x + rhs.y);
	}
	bool operator > (const NavInfo& rhs) const { 
		return (x + y) < (rhs.x + rhs.y);
	}
	bool operator < (const NavInfo* rhs) const {
		return (x + y) < (rhs->x + rhs->y);
	}
	bool operator > (const NavInfo* rhs) const {
		return (x + y) > (rhs->x + rhs->y);
	}
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

#pragma once
#ifndef WS_PARSED_TMX_H
#define WS_PARSED_TMX_H

#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <cereal\external\rapidxml\rapidxml.hpp>
struct TmxTileset {
	unsigned int firstGid, tileWidth, tileHeight, sourceWidth, sourceHeight;
	std::string name, filename;
};


struct TmxLayer {
	std::string name;
	unsigned int width, height;
	std::vector<int> data;
};


struct TmxProperty {
	std::string name;
	std::string value;
};


struct TmxObject {
	std::string name;
	unsigned int gid, type;
	float x, y, width, height, rotation;
	std::vector<std::shared_ptr<TmxProperty>> properties;
};


struct TmxGroup {
	std::string name;
	unsigned int width, height;
	bool visible;
	std::vector<std::shared_ptr<TmxObject>> objects;
};

class TmxMap {
public:
	TmxMap(std::string xml);
	bool Load(std::string xml);
	bool isValid = false;
	unsigned int width, height, tileWidth, tileHeight;
	std::vector<std::shared_ptr<TmxTileset>> tilesets;
	std::vector<std::shared_ptr<TmxLayer>> layers;
	std::vector<std::shared_ptr<TmxGroup>> groups;
};

#endif // !WS_PARSED_TMX_H
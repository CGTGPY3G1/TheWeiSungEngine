#pragma once
#include <memory>

struct TileInfo {

	bool blocked = false;
	int x = 0, y = 0;
	int baseCost = 1;
	std::shared_ptr<TileInfo> neighbours;
	bool operator () (const TileInfo & other){
		return baseCost < other.baseCost;
	}
};
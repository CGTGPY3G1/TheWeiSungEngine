#ifndef WS_TILE_INFO_H
#define WS_TILE_INFO_H
#include <memory>
namespace WeiSungEngine {
	struct TileInfo {

		bool blocked = false;
		int x = 0, y = 0;
		int baseCost = 1;
		std::shared_ptr<TileInfo> neighbours;
		bool operator () (const TileInfo & other) {
			return baseCost < other.baseCost;
		}
	};
}

#endif // !WS_TILE_INFO_H

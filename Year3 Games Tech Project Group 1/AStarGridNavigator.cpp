#include "AStarGridNavigator.h"

const GridLocation AStarGridNavigator::LEFT = GridLocation(-1, 0), AStarGridNavigator::RIGHT = GridLocation(1, 0),
					AStarGridNavigator::UP = GridLocation(0, -1), AStarGridNavigator::DOWN = GridLocation(0, 1),
					AStarGridNavigator::UP_LEFT = GridLocation(-1, -1), AStarGridNavigator::UP_RIGHT = GridLocation(1, -1),
					AStarGridNavigator::DOWN_LEFT = GridLocation(-1, 1), AStarGridNavigator::DOWN_RIGHT = GridLocation(1, 1);


AStarGridNavigator::AStarGridNavigator() {
}

AStarGridNavigator::~AStarGridNavigator() {
}

NavigationPath AStarGridNavigator::GetPathToTarget(const std::vector<std::vector<Tile>>& grid, const GridLocation & start, const GridLocation & end) {
	return NavigationPath();
}

#pragma once
#ifndef WS_ASTAR_GRID_NAVIGATOR_H
#define WS_ASTAR_GRID_NAVIGATOR_H
#include <vector>
#include <queue>
#include <tuple>
struct GridLocation {
	GridLocation(const int & x, const int & y) { this->x = x; this->y = y; }
	int x = 0, y = 0;
	GridLocation operator +(const GridLocation & other) {
		return GridLocation(this->x + other.x, this->y + other.y);
	}
	GridLocation operator =(const GridLocation & other) {
		this->x = other.x; this->y = other.y;
		return *this;
	}
	bool operator ==(const GridLocation & other) {
		return (this->x == other.x && this->y == other.y);
	}
};

struct NavAgent {
	std::vector<GridLocation> directions;

};

struct NavigationPath {
	std::vector<GridLocation> path;
};
class Tile;
class AStarGridNavigator {
public:
	const static GridLocation LEFT, RIGHT, UP, DOWN, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT;
	AStarGridNavigator();
	~AStarGridNavigator();
	NavigationPath GetPathToTarget(const std::vector<std::vector<Tile>> & grid, const GridLocation & start, const GridLocation & end);
private:
	const std::vector<GridLocation> directions = {UP_LEFT, UP, UP_RIGHT, LEFT, RIGHT, DOWN_LEFT, DOWN, DOWN_RIGHT};
};

#endif // !WS_ASTAR_GRID_NAVIGATOR_H



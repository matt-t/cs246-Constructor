#ifndef TILE_H
#define TILE_H
#include "enums.h"
#include <memory>
#include <vector>
#include <map>

class Vertex;
class Road;

class Tile {
	int locationNum;
	int rollNum;
	Resource resource;
	std::vector<std::weak_ptr<Vertex>> vertices;
	std::vector<std::weak_ptr<Road>> roads;

	public:
	bool geese = false; //this is made public because we would need to check the geese/change th geese
	std::map<Color, int> produceResources() noexcept;
	void setGeese();//this deals with the consequences of losses of resources when a geese is set on a tile
};

#endif

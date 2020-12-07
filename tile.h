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

	Tile(int locationNum); //should add vertices and roads here
	std::map<Color, int> produceResources() noexcept;
	Resource getResource() const noexcept;//this gets the resource type produced at this tile
	std::vector<Color> getLocationPlayers() const noexcept; //returns a list of players who have residence surrounding the tile
	void addRoad(std::shared_ptr<Road> road) noexcept;
	void addVertex(std::shared_ptr<Vertex> vertex) noexcept;
};

#endif

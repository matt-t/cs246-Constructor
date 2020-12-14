#ifndef ROAD_H
#define ROAD_H
#include "enums.h"
#include <memory>
#include <vector>

class Vertex;

class Road {
	// friend class Vertex;
	int location;
	Color owner;
	std::vector<std::weak_ptr<Vertex>> vertices;
	
	public:
	Road(int location);
	Road(int location, Color owner);
	Color getOwner() const noexcept;
	int getLocation() const noexcept;
	void build(Color player);
	void addVertex(std::shared_ptr<Vertex> vertex) noexcept;
	std::vector<std::shared_ptr<Vertex>> getVertices() const noexcept;
};

class RoadExistsException{};
class InvalidRoadLocationException{};
#endif

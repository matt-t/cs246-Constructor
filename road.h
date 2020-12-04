#ifndef ROAD_H
#define ROAD_H
#include "enums.h"
#include <memory>
#include <vector>

class Vertex;

class Road {
	int location;
	Color owner;
	
	public:
	std::vector<std::weak_ptr<Vertex>> vertices;
	Road(int location);
	Color getOwner() const noexcept;
	void build(Color player);
};
#endif

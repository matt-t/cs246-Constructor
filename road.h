#ifndef ROAD_H
#define ROAD_H
#include "enums.h"
#include <memory>
#include <vector>

class Vertex;

class Road {
	friend class Vertex;
	int location;
	Color owner;
	std::vector<std::weak_ptr<Vertex>> vertices;
	
	public:
	Road(int location);
	Color getOwner() const noexcept;
	void build(Color player);
};
#endif

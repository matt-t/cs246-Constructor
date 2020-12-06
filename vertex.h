#ifndef VERTEX_H
#define VERTEX_H
#include "enums.h"
#include <vector>
#include <memory>

class Road;

class Vertex {
	friend class Road;
	int location;
	Residence typeOfBuilding;
	Color owner;
	std::vector<std::weak_ptr<Road>> roads;
	
	public:
	Vertex(int location);
	Color getOwner() const noexcept;
	Residence getResidence() const noexcept;
	void build(Color player, bool gameStart = false); //if true is passed in, player can build anywhere without adjacent buiding (beginning of a turn). elsee, by default regular rules of building apply
	void upgrade(Color player);
};
#endif
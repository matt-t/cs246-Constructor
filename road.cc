#include "road.h"
#include "vertex.h" //high coupling?! vetex needs road and road needs vertex

//constructors
Road::Road(int location):
	location{ location }, owner{ Color::None }
{}

Road::Road(int location, Color owner):
	location{ location }, owner{ owner }
{}

//getters
//retrieves the owner
Color Road::getOwner() const noexcept {
	return owner;
}

int Road::getLocation() const noexcept {
	return location;
}


void Road::build(Color player) {
	if (owner != Color::None) {
		throw;//with custom message
	}
	bool validBuild = false;
	for (int i = 0; i < vertices.size(); ++i) {
		auto getVertex = vertices[i].lock();
		if (getVertex->getOwner() == player){
			validBuild = true;
			break;
		} 
		for (int j = 0; j < getVertex->roads.size(); ++j) {
			auto getRoad = getVertex->roads[j].lock();
			if(getRoad->owner == player) {
				validBuild = true;
				break;
			}
		}
	}
	if (validBuild == false) {
		throw; //another custom message
	}
	owner = player;
}

void Road::addVertex(std::shared_ptr<Vertex> vertex) noexcept{
	std::weak_ptr<Vertex> wp = vertex;
	vertices.push_back(wp);
}
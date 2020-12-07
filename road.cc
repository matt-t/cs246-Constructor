#include "road.h"
#include "vertex.h" //high coupling?! vetex needs road and road needs vertex


Road::Road(int location):
	location{ location }, owner{ Color::None }
{}//should we establish vertices at initialization

//retrieves the owner
Color Road::getOwner() const noexcept {
	return owner;
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
		throw; //another custome message
	}
	owner = player;
}

void Road::addVertex(std::shared_ptr<Vertex> vertex) noexcept{
	std::weak_ptr<Vertex> wp = vertex;
	vertices.push_back(wp);
}
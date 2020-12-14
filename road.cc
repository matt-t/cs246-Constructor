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


std::vector<std::shared_ptr<Vertex>> Road::getVertices() const noexcept {
	std::vector<std::shared_ptr<Vertex>> connected_vertices;
	for (const auto &vertex: vertices) {
		connected_vertices.emplace_back(std::move(vertex.lock()));
	}
	return connected_vertices;
}

void Road::build(Color player) {
	if (owner != Color::None) {
		throw RoadExistsException{};//with custom message
	}
	bool validBuild = false;
	for (const std::weak_ptr<Vertex> &vertex : vertices) {
		std::shared_ptr<Vertex> vertex_ptr = vertex.lock();
		if (vertex_ptr->getOwner() == player){
			validBuild = true;
			break;
		} 
		if (vertex_ptr->getOwner() == Color::None) {
			std::vector<std::shared_ptr<Road>> adjacentRoads = vertex_ptr->getRoads();
			for (const auto &adjacentRoad : adjacentRoads) {
				if(adjacentRoad->getOwner() == player) {
					validBuild = true;
					break;
				}
			}
		}
	}
	if (validBuild == false) {
		throw InvalidRoadLocationException{}; //another custom message
	}
	owner = player;
}

void Road::addVertex(std::shared_ptr<Vertex> vertex) noexcept{
	std::weak_ptr<Vertex> wp = vertex;
	vertices.push_back(wp);
}
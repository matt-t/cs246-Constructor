#include "vertex.h"
#include "road.h"

//constructors
Vertex::Vertex(int location):
	location{ location }, owner { Color::None }, typeOfBuilding{ Residence::None }
{} // maybe we should do pass in vector of roads here

Vertex::Vertex(int location, Color owner, Residence typeOfBuilding):
	location{ location }, owner { owner }, typeOfBuilding{ typeOfBuilding }
{}

//retrieves
Color Vertex::getOwner() const noexcept {
	return owner;
}

Residence Vertex::getResidence() const noexcept {
	return typeOfBuilding;
}

int Vertex::getLocation() const noexcept {
	return location;
}

std::vector<std::shared_ptr<Road>> Vertex::getRoads() const noexcept {
	std::vector<std::shared_ptr<Road>> connected_roads;
	for (const auto &road: roads) {
		connected_roads.emplace_back(std::move(road.lock()));
	}
	return connected_roads;
}


//builds residence
void Vertex::build(Color player, bool gameStart ) {
	//is already built
	if (owner != Color::None) {	
		throw BuildingExistsException(); //already a building here
	}
	//has adjacent building connected to it
	for (const std::weak_ptr<Road> &road : roads) {
		std::shared_ptr<Road> road_ptr = road.lock();
		std::vector<std::shared_ptr<Vertex>> adjacentVertices = road_ptr->getVertices();
		for (const auto &adjacentVertex : adjacentVertices) {
			if (adjacentVertex->getLocation() != location && adjacentVertex->getOwner() != Color::None) {
				throw InvalidLocationException();//there's a building adjacent to it
			}
		}
	}
	std::cout << "what happened" << std::endl;
	//is not connected by a road
	bool validBuild = false;
	if (!gameStart){
		for (const std::weak_ptr<Road> &road : roads) {
			std::shared_ptr<Road> road_ptr = road.lock();
			if (road_ptr->getOwner() == player) {
				validBuild = true;
				break;
			}
		}
	} else {
		validBuild = true;
	}
	if (validBuild){
		owner = player;
		typeOfBuilding = Residence::Basement;
	} else {
		throw InvalidLocationException();//not connected by a road of their own
	}
}
void Vertex::upgrade(Color player){
	if (owner != player) {
		throw BuidingNotOwnedException();// aha u don't live here
	} else if (typeOfBuilding== Residence::Tower) {
		throw AlreadyTowerException();//aha the house is already boojee enough
	} else if (typeOfBuilding== Residence::Basement) {
		typeOfBuilding= Residence::House;
	} else if (typeOfBuilding== Residence::House) {
		typeOfBuilding= Residence::Tower;
	}
}

void Vertex::addRoad(std::shared_ptr<Road> road) noexcept{
	std::weak_ptr<Road> wp = road;
	roads.push_back(wp);
}
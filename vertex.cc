#include "vertex.h"
#include "road.h"

Vertex::Vertex(int location):
	location{ location }, owner { Color::None }
{} // maybe we should do pass in vector of roads here

//retrieves
Color Vertex::getOwner() const noexcept {
	return owner;
}

Residence Vertex::getResidence() const noexcept {
	return typeOfBuilding;
}

//builds residence
void Vertex::build(Color player, bool gameStart ) {
	//is already built
	if (owner != Color::None) {	
		throw;//already a building here
	}
	//has adjacent building connected to it
	for (int i = 0; i < roads.size(); ++i) {
		auto getRoad = roads[i].lock();
		for (int j = 0; j < getRoad->vertices.size(); ++j) {
			auto getVertex = getRoad->vertices[j].lock();
			if (getVertex->location != location && getVertex->owner != Color::None) {
				throw;//there's a building adjacent to it
			}
		}
	}
	//is not connected by a road
	bool validBuild = false;
	if (!gameStart){
		for (int i = 0; i < roads.size(); ++i) {
			auto getRoad = roads[i].lock();
			if (getRoad->getOwner() == player) {
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
		throw;//not connected by a road of their own
	}
}
void Vertex::upgrade(Color player){
	if (owner != player) {
		throw;// aha u don't live here
	} else if (typeOfBuilding== Residence::Tower) {
		throw;//aha the house is already boojee enough
	} else if (typeOfBuilding== Residence::Basement) {
		typeOfBuilding= Residence::House;
	} else if (typeOfBuilding== Residence::House) {
		typeOfBuilding= Residence::Tower;
	}
}

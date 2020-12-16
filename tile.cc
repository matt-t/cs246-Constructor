#include "tile.h"
#include "vertex.h" //this coupling is kinda bad

Tile::Tile(int locationNum, Resource resource, int rollNum, 
std::vector<std::weak_ptr<Vertex>> vertices, std::vector<std::weak_ptr<Road>> roads):
    locationNum{ locationNum }, resource{ resource }, rollNum{ rollNum }, geese{ false }, vertices{ vertices }, roads{ roads }
{}

std::shared_ptr<Vertex> Tile::getVertex(int tileCorner) const noexcept {
    return vertices[tileCorner].lock();
}

std::shared_ptr<Road> Tile::getRoad(int tileEdge) const noexcept {
    return roads[tileEdge].lock();
}

int Tile::getLocation() const noexcept {
    return locationNum;
}


std::map<Color, int> Tile::produceResources() noexcept{
    std::map<Color, int> list;
    for (int i = 0; i < vertices.size(); ++i){
        auto getVertex = vertices[i].lock();
        if (getVertex->getOwner() != Color::None){
            switch (getVertex->getResidence()){
                case Residence::Basement:
                    list[getVertex->getOwner()]++;
                break;
                case Residence::House:
                    list[getVertex->getOwner()] += 2;
                break;
                case Residence::Tower:
                    list[getVertex->getOwner()] += 3;
                break;
            }
        }
    }
    return list;
}
Resource Tile::getResource() const noexcept{
    return resource;
}

int Tile::getRollNum() const noexcept{
    return rollNum;
}

std::set<Color> Tile::getLocationPlayers() const noexcept {
    std::set<Color> list;
    for (int i = 0; i < vertices.size(); ++i) {
        auto getVertex = vertices[i].lock();
        if (getVertex->getOwner() != Color::None){
            list.insert(getVertex->getOwner());
        }
    }
    return list;
}

void Tile::addRoad(std::shared_ptr<Road> road) noexcept{
	std::weak_ptr<Road> wp = road;
	roads.push_back(wp);
}

void Tile::addVertex(std::shared_ptr<Vertex> vertex) noexcept{
	std::weak_ptr<Vertex> wp = vertex;
	vertices.push_back(wp);
}

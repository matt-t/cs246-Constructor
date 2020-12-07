#include "tile.h"
#include "vertex.h" //this coupling is kinda bad

Tile::Tile(int locationNum):
    locationNum{ locationNum }, geese{ false }
{}

std::map<Color, int> Tile::produceResources() noexcept{
    std::map<Color, int> list;
    list[Color::Blue] = 0;
    list[Color::Orange] = 0;
    list[Color::Red] = 0;
    list[Color::Yellow] = 0;
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
std::vector<Color> Tile::getLocationPlayers() const noexcept {
    std::vector<Color> list;
    for (int i = 0; i < vertices.size(); ++i) {
        auto getVertex = vertices[i].lock();
        if (getVertex->getOwner() != Color::None){
            list.push_back(getVertex->getOwner());
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
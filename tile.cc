#include "tile.h"
#include "vertex.h" //this coupling is kinda bad

Tile::Tile(int locationNum, int rollNum, Resource resource):
    locationNum{ locationNum }, rollNum{ rollNum }, resource{ resource }, geese{ false }
{}

std::map<Color, int> Tile::produceResources() noexcept{

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
#include "board.h"

Board::Board(){

}

void Board::buildResidence(Color color, int location){
    vertices[location]->build(color);//error check
}

void Board::upgradeResidence(Color color, int location){
    vertices[location]->upgrade(color);//error check
}

void Board::buildRoad(Color color, int location){
    roads[location]->build(color);//error check
}

void Board::changeGeese(int location){
    if (location == geese) {
        throw; //geese cannot be placed on the same
    }
    tiles[geese]->geese = false;
    tiles[location]->geese = true;
    geese = location;
}

std::map<Color, std::map<Resource, int>> Board::getRollResources(int rollNumber) noexcept{

}
std::vector<Color> Board::getLocationPlayers(int location){
    
}

std::ostream &operator<<(std::ostream &out, const Board &board) {

}






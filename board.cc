#include "board.h"
#include <fstream>
#include <string>
#include <sstream>
#include "constants.h"

using namespace std;

Board::Board(vector<pair<Resource, int>> boardInfo){
    for  (int i = 0; i <= 53; ++i){
        auto p = std::make_shared<Vertex>(i);
        vertices.push_back(p);
    }
    for (int i = 0; i <= 71; ++i){
        auto p = std::make_shared<Road>(i);
        for (int j =0; j < RoadsVertices.size(); ++j){
            p->addVertex(vertices[RoadsVertices[i][j]]);
        }
        roads.push_back(p);
    }
    for (int i = 0; i <= 53; ++i){
        for (int j = 0; j < VerticesRoads.size(); ++j){
            vertices[i]->addRoad(roads[VerticesRoads[i][j]]);
        }
    }
    for (int i = 0; i <= 18; ++i){
        std::vector<std::weak_ptr<Vertex>>tileVertices;
        std::vector<std::weak_ptr<Road>> tileRoads;
        for (int j = 0; j < TilesVertices[i].size(); ++j){
            std::weak_ptr<Vertex> wp = vertices[TilesVertices[i][j]];
	        tileVertices.push_back(wp);
        }
        for (int j = 0; j < TilesRoads[i].size(); ++j){
            std::weak_ptr<Road> wp = roads[TilesRoads[i][j]];
	        tileRoads.push_back(wp);
        }
        auto p = std::make_shared<Tile>(i, boardInfo[i].first, boardInfo[i].second, tileVertices, tileRoads);
        tiles.push_back(p);
    }
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
        throw GeeseExistsHereException(); //geese cannot be placed on the same
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






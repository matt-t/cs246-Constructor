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
        roads.push_back(p);
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
    string fileName = "VerticesAndRoads.txt";
    string line;
    ifstream file(fileName);
    int vertex, road, tile;
    if (file.is_open()){
        getline(file, line);
        while (getline(file, line)){
            if (line == "attach vertices") { break; }
            istringstream num(line);
            num >> vertex;
            while (num >> road) {
                vertices[vertex]->addRoad(roads[road]);
            }
        }
        while (getline(file, line)){
            istringstream num(line);
            num >> road;
            while (num >> vertex) {
                roads[road]->addVertex(vertices[vertex]);
            }
        }
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






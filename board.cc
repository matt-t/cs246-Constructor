#include "board.h"
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

#include "constants.h"

using namespace std;

Board::Board(vector<pair<Resource, int>> boardInfo){
    for  (int i = 0; i <= 53; ++i){
        auto p = std::make_shared<Vertex>(i);
        vertices.push_back(p);
    }
    for (int i = 0; i <= 71; ++i){
        auto p = std::make_shared<Road>(i);
        for (int j =0; j < RoadsVertices[i].size(); ++j){
            p->addVertex(vertices[RoadsVertices[i][j]]);
        }
        roads.push_back(p);
    }
    for (int i = 0; i <= 53; ++i){
        for (int j = 0; j < VerticesRoads[i].size(); ++j){
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

Board::Board(std::vector<std::pair<Resource, int>> tileInfo, std::vector<Color> roadInfo, std::vector<std::pair<Color, Residence>> buildInfo, int geese):
    geese{ geese }
{
    cerr << "succ" << endl;
    for  (int i = 0; i <= 53; ++i){
        auto p = std::make_shared<Vertex>(i, buildInfo[i].first, buildInfo[i].second);
        vertices.push_back(p);
    }
    for (int i = 0; i <= 71; ++i){
        auto p = std::make_shared<Road>(i, roadInfo[i]);
        for (int j =0; j < RoadsVertices[i].size(); ++j){
            p->addVertex(vertices[RoadsVertices[i][j]]);
        }
        roads.push_back(p);
    }
    for (int i = 0; i <= 53; ++i){
        for (int j = 0; j < VerticesRoads[i].size(); ++j){
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
        auto p = std::make_shared<Tile>(i, tileInfo[i].first, tileInfo[i].second, tileVertices, tileRoads);
        tiles.push_back(p);
    }
    tiles[geese]->geese = true;
    cerr << "ess" << endl;
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
char getChar(Color color) {
    char c = '@';
    switch(color) {
        case Color::Blue:
            c = 'B';
            break;
        case Color::Red:
            c = 'R';
            break;
        case Color::Yellow:
            c = 'Y';
            break;
        case Color::Orange:
            c = 'O';
            break;
        case Color::None:
            c = 'N';
            break;
    }
    return c;
}

char getChar(Residence residence) {
    char c = '@';
    switch(residence) {
        case Residence::Basement:
            c = 'B';
            break;
        case Residence::House:
            c = 'H';
            break;
        case Residence::Tower:
            c = 'T';
            break;
        case Residence::None:
            c = '?';
            break;
    }
    return c;
}

std::string getMargin(int numSpaces) {
    std::string s = "";
    for (int i = 0; i < numSpaces; i++) {
        s += ' ';
    }
    return s;
}


std::string getTCString(int num) {
    std::string s = "";
    s += num >= 10 ? "" : " ";
    s += std::to_string(num);
    return s;
}

std::string getTCString(Color color, Residence residence) {
    std::string s = "";
    s += getChar(color);
    s += getChar(residence);
    return s;
}

std::string getTCString(Color color) {
    std::string s = "";
    s += getChar(color);
    s += 'R';
    return s;
}



std::string getRoadString(Road &road) {
    std::string str = "";
    str += "--";
    if (road.getOwner() == Color::None) {
        str += getTCString(road.getLocation());
    } else {
        str += getTCString(road.getOwner());
    }
    str += "--";
    return str;
}


std::string getVertexString(Vertex &vertex) {
    std::string str = "";
    str += '|';
    if (vertex.getOwner() == Color::None) {
        str += getTCString(vertex.getLocation());
    } else {
        str += getTCString(vertex.getOwner(), vertex.getResidence());
    }
    str += '|';
    return str;
}

std::string getDividerString(std::string s) {
    std::string str = "";
    int whiteSpace = 4 - s.length();
    int rightMargin = whiteSpace/2;
    int leftMargin = (s.length() % 2 == 0) ? whiteSpace/2 : (whiteSpace/2) + 1;
    str += getMargin(leftMargin) + s + getMargin(rightMargin);
    cout << str << endl;
    cout << "grief ^" << endl;
    return str;
}

std::string getTileSpan(std::string s = "") {
    std::string str = "";
    int whiteSpace = 6 - s.length();
    int leftMargin = whiteSpace/2;
    int rightMargin = (s.length() % 2 == 0) ? whiteSpace/2 : (whiteSpace/2) + 1;
    str += getMargin(leftMargin);
    str += s;
    str += getMargin(rightMargin);
    return str;
}

std::string getTileString(Tile &tile, bool resource = true) {
    std::string s = "";
    if (resource) {
        s = getResourceString(tile.getResource());
    } else {
        s = getTileSpan(std::to_string(tile.getRollNum()));
    }
    std::string str = "";
    int whiteSpace = 6 - s.length();
    int leftMargin = whiteSpace/2;
    int rightMargin = (s.length() % 2 == 0) ? whiteSpace/2 : whiteSpace/2 + 1;
    str += getMargin(leftMargin);
    str += s;
    str += getMargin(rightMargin);
    return str;
}

std::string Board::getVertexLine(std::vector<int> tileOrRoadLocations, std::vector<int> vertexLocations, int numTiles, bool roadFirst = true) const {
    std::string s = "";
    int curVertexIdx = 0;
    int tileOrRoadIdx = 0;
    s += getMargin(6);
    bool isRoad = roadFirst;
    while (curVertexIdx < vertexLocations.size()) {
        s += getVertexString(*vertices[vertexLocations[curVertexIdx++]]);
        if (tileOrRoadIdx != tileOrRoadLocations.size()) {
            if (isRoad) {
                s += getRoadString(*roads[tileOrRoadLocations[tileOrRoadIdx++]]);
            } else {
                s += getTileString(*tiles[tileOrRoadLocations[tileOrRoadIdx++]]);
            }
        }
        isRoad = !isRoad;
    }
    return s;
}


std::string Board::getDividerLine(std::vector<int> tileLocations, int numTiles, bool dividers = true, std::vector<std::string> dividerStrings = defaultDividers) const {
    std::string s = "";
    int curTileIdx = 0;
    int curDividerIdx = 0;
    s += getMargin(6);
    cout << "huh" << endl;
    s += getDividerString(numTiles == 5 ? dividerStrings[curDividerIdx++] : "  ");
    cout << s << endl;
    if (tileLocations.size() == 3) {
        s += getTileString(*tiles[tileLocations[curTileIdx++]], dividers);
    } else {
        s += getMargin(6);
    }
    cout << s << endl;
    s += getDividerString(numTiles >= 3 ? dividerStrings[curDividerIdx++] : "  ");

    if (tileLocations.size() == 2) {
        s += getTileString(*tiles[tileLocations[curTileIdx++]], dividers);
    } else {
        s += getMargin(6);
    }

    s += getDividerString(numTiles >= 1 ? dividerStrings[curDividerIdx++] : "  ");
    if (tileLocations.size() != 2) {
        s += getTileString(*tiles[tileLocations[curTileIdx++]], dividers);
    } else {
        s += getMargin(6);
    }
    s += getDividerString(numTiles >= 1 ? dividerStrings[curDividerIdx++] : "  ");

    if (tileLocations.size() == 2) {
        s += getTileString(*tiles[tileLocations[curTileIdx++]], dividers);
    } else {
        s += getMargin(6);
    }
    
    s += getDividerString(numTiles >= 3 ? dividerStrings[curDividerIdx++] : "  ");
    if (tileLocations.size() == 3) {
        s += getTileString(*tiles[tileLocations[curTileIdx++]], dividers);
    } else {
        s += getMargin(6);
    }
    s += getDividerString(numTiles == 5 ? dividerStrings[curDividerIdx++] : "  ");
    cout << "huh" << endl;

    return s;
}

std::vector<int> getVectorBetween(int leftBound, int rightBound) {
    std::vector<int> v;
    for (int i = leftBound; i <= rightBound; i++) {
        v.emplace_back(i);
    }
    return v;
}

std::ostream &operator<<(std::ostream &out, const Board &board) {
    int roadIdx = 0;
    int vertexIdx = 0;
    int tileIdx = 0;
    int rollIdx = 0;
    out << board.getVertexLine(getVectorBetween(0, 0), getVectorBetween(0, 1), 1) << std::endl;
    out << board.getDividerLine(getVectorBetween(0, -1), 1) << std::endl;
    return out;
}

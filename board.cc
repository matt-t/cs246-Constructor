#include "board.h"
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

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
}
    

void Board::buildResidence(Color color, int location){
    if (location > 53 || location < 0) {
        throw InvalidLocationException();
    }
    try{
    vertices[location]->build(color);
    } catch(BuildingExistsException& e){
        cerr << "ERROR: A residence already exists here." << endl;
    } catch (InvalidLocationException& e){
        cerr << "ERROR: You cannot build here." << endl;
    }
}

void Board::upgradeResidence(Color color, int location){
    try{ 
        vertices[location]->upgrade(color);
    } catch(BuidingNotOwnedException& e){
        cerr << "ERROR: You do not own this residence." << endl;
    } catch (AlreadyTowerException& e){
        cerr << "ERROR: Your residence is already a Tower." << endl;
    }
}


void Board::buildRoad(Color color, int location){
    try{
        roads[location]->build(color);
    } catch (RoadExistsException& e){
        cerr << "ERROR: Someone has already built a road here." << endl;
    } catch(InvalidRoadLocationException& e) {
        cerr << "ERROR: Invalid location chosen. None of your road or residence connect to here." << endl;
    } //need to change player's information
}

void Board::changeGeese(int location){
    if (location == geese) {
        throw GeeseExistsHereException(); //geese cannot be placed on the same
    } else if (location < 0 || location > 18) {
        throw GeeseOutOfRange();
    }
    tiles[geese]->geese = false;
    tiles[location]->geese = true;
    geese = location;
}

int Board::getGeese(){
    return geese;
}

std::map<Color, std::map<Resource, int>> Board::getRollResources(int rollNumber) noexcept{

}
std::set<Color> Board::getLocationPlayers(int location){
    return tiles[location]->getLocationPlayers();
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
    s += COLOR_TO_CHAR.at(color);
    s += RESIDENCE_TO_CHAR.at(residence);
    return s;
}

std::string getTCString(Color color) {
    std::string s = "";
    s += COLOR_TO_CHAR.at(color);
    s += 'R';
    return s;
}



std::string getRoadString(Road &road, bool horizontal = true) {
    std::string str = "";
    if (horizontal) {
        str += "--";
    } else {
        str += " ";
    }
    
    if (road.getOwner() == Color::None) {
        str += getTCString(road.getLocation());
    } else {
        str += getTCString(road.getOwner());
    }
    if (horizontal) {
        str += "--";
    } else {
        str += " ";
    }
    
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
    return str;
}

std::string getTileString(std::string s = "") {
    std::string str = "";
    int whiteSpace = 6 - s.length();
    int rightMargin = whiteSpace/2;
    int leftMargin = (s.length() % 2 == 0) ? whiteSpace/2 : (whiteSpace/2) + 1;
    str += getMargin(leftMargin);
    str += s;
    str += getMargin(rightMargin);
    return str;
}

std::string Board::printLine(Tile &tile, int line, bool includeLeft = false) const noexcept {
    std::string s = "";
    switch(line) {
        case 0:
            if (includeLeft) {
                s += getVertexString(*tile.getVertex(TOP_LEFT_VERTEX));
            }
            s += getRoadString(*tile.getRoad(TOP_MIDDLE_ROAD));
            s += getVertexString(*tile.getVertex(TOP_RIGHT_VERTEX));
            break;
        case 1:
            if (includeLeft) {
                s += getDividerString("|");
            }
            s += getMargin(6);
            s += getDividerString("|");
            break;
        case 2:
            if (includeLeft) {
                s += getRoadString(*tile.getRoad(TOP_LEFT_ROAD), false);
            }
            s += getTileString(std::to_string(tile.getLocation()));
            s += getRoadString(*tile.getRoad(TOP_RIGHT_ROAD), false);
            break;
        case 3:
            if (includeLeft) {
                s += getDividerString("|");
            }
            s += getTileString(getResourceString(tile.getResource()));
            s += getDividerString("|");
            break;
        case 4:
            if (includeLeft) {
                s += getVertexString(*tile.getVertex(MIDDLE_LEFT_VERTEX));
            }
            s += getTileString(std::to_string(tile.getRollNum()));
            s += getVertexString(*tile.getVertex(MIDDLE_RIGHT_VERTEX));
            break;
        case 5:
            if (includeLeft) {
                s += getDividerString("|");
            }
            if (tile.geese) {
                s += getTileString(GOOSE_STRING);
            } else {
                s += getMargin(6);
            }
            s += getDividerString("|");
            break;
        case 6:
            if (includeLeft) {
                s += getRoadString(*tile.getRoad(BOTTOM_LEFT_ROAD), false);
            }
            s += getMargin(6);
            s += getRoadString(*tile.getRoad(BOTTOM_RIGHT_ROAD), false);
            break;
        case 7:
            if (includeLeft) {
                s += getDividerString("|");
            }
            s += getMargin(6);
            s += getDividerString("|");
            break;
        case 8:
            if (includeLeft) {
                s += getVertexString(*tile.getVertex(BOTTOM_LEFT_VERTEX));
            }
            s += getRoadString(*tile.getRoad(BOTTOM_MIDDLE_ROAD));
            s += getVertexString(*tile.getVertex(BOTTOM_RIGHT_VERTEX));
            break;
    }
    return s;
}


std::vector<std::string> Board::printChunk(std::vector<int> tileLocations, bool top, int topChange = 0, int bottomChange = 0) const noexcept {
    std::vector<std::string> chunks;
    int oddLine = 0;
    int evenLine = 4;
    if (!top) {
        oddLine = 4;
        evenLine = 0;
    }
    for (int lineNum = 0 + topChange; lineNum < 4 + bottomChange; lineNum++) {
        bool odd = true;
        std::string chunk = "";
        bool leftMost = true;
        int curLine = 0;
        for (const int &tileLocation: tileLocations) {
            curLine = odd ? oddLine + lineNum : evenLine + lineNum;
            chunk += printLine(*tiles[tileLocation], curLine, leftMost);
            odd = !odd;
            if (leftMost == true) {
                leftMost = false;
            }
        }
        chunks.push_back(chunk);
    }
    return chunks;
}

std::ostream &operator<<(std::ostream &out, const Board &board) {

    std::vector<vector<int>> chunks{
        {3, 1, 4, 2, 5}, {3, 6, 4, 7, 5}, {8, 6, 9, 7, 10}, {8, 11, 9, 12, 10}, {8, 11, 9, 12, 10}, {13, 16, 14, 17, 15}
    };
    std::vector<int> top{0};
    std::vector<int> top2{1, 0, 2};
    std::vector<int> bottom2{16, 18, 17};
    std::vector<int> bottom{18};

    std::vector<std::string> topStrings = board.printChunk(top, true);
    for (const std::string &line: topStrings) {
        out << getMargin(26) << line << std::endl;
    }
    
    
    std::vector<std::string> topStrings2 = board.printChunk(top2, true);
    for (const std::string &line: topStrings2) {
        out << getMargin(16) << line << std::endl;
    }

    bool odd = true;
    int curChunk = 0;
    for(std::vector<int> &chunk: chunks) {
        curChunk++;
        std::vector<std::string> chunkStrings = board.printChunk(chunk, odd, 0, curChunk == chunks.size() ? 1 : 0);
        for (const std::string &line: chunkStrings) {
            out << getMargin(6) << line << endl;
        }
        odd = !odd;
    }

    std::vector<std::string> botStrings2 = board.printChunk(bottom2, false, 1, 1);
    for (const std::string &line: botStrings2) {
        out << getMargin(16) << line << std::endl;
    }
    
    std::vector<std::string> botStrings = board.printChunk(bottom, false, 1, 1);
    for (const std::string &line: botStrings) {
        out << getMargin(26) << line << std::endl;
    }
    
    return out;
}

#ifndef BOARD_H
#define BOARD_H
#include "enums.h"
#include "tile.h"
#include "road.h"
#include "vertex.h"
#include <memory>
#include <vector>
#include <map>
#include <utility>

class Board{
    std::vector<std::shared_ptr<Tile>> tiles;
    std::vector<std::shared_ptr<Vertex>> vertices;
    std::vector<std::shared_ptr<Road>> roads;
    int geese;

    std::string printLine(Tile &tile, int line, bool includeLeft) const noexcept;
    std::vector<std::string> printChunk(std::vector<int> tileLocations, bool top, int topChange, int bottomChange) const noexcept;

    public:
    //constructors
    Board(std::vector<std::pair<Resource, int>> tileInfo);
    Board(std::vector<std::pair<Resource, int>> tileInfo, std::vector<Color> roadInfo, std::vector<std::pair<Color, Residence>> buildInfo, int geese);
    //building, upgrading, changing geese
    void buildResidence(Color color, int location, bool gameStart = false);
    void upgradeResidence(Color color, int location);
    void buildRoad(Color color, int location);
    void changeGeese(int location);
    Resource getTileResource(int location) const;
    int getTileRollNum(int location) const;
    int getGeese() const;
    //getting roll and location information
    std::map<Color, std::map<Resource, int>> getRollResources(int rollNumber) noexcept;
    std::set<Color> getLocationPlayers(int location);

    friend std::ostream &operator<<(std::ostream &out, const Board &board);    
};

std::ostream &operator<<(std::ostream &out, const Board &board);

class GeeseExistsHereException{};
class GeeseOutOfRange{};

#endif

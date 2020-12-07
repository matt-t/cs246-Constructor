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

    public:
    Board(std::vector<std::pair<Resource, int>> boardInfo);
    void buildResidence(Color color, int location);
    void upgradeResidence(Color color, int location);
    void buildRoad(Color color, int location);
    void changeGeese(int location);
    std::map<Color, std::map<Resource, int>> getRollResources(int rollNumber) noexcept;
    std::vector<Color> getLocationPlayers(int location);

    friend std::ostream &operator<<(std::ostream &out, const Board &board);    
};

std::ostream &operator<<(std::ostream &out, const Board &board);

#endif

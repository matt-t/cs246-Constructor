#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include <string>
#include <utility>
#include "player.h"
#include "board.h"

class Game {
    /* Private Variables */
    unsigned int seed;
    std::map<Color, std::unique_ptr<Player>> players;
    std::unique_ptr<Board> board;
    int turn;
    int winner;

    /* Private Methods */
    //
    void save();
    
    //
    void status() noexcept;

    //
    void residences(Player &player);
    
    //
    void help(int movePhase) noexcept;
    
    //
    void next() noexcept;

    //
    void handleRollMove(Player &player, std::string move, int &movePhase);
    
    //
    void handleActionMove(Player &player, std::string move, int &movePhase);


    public:
    // Constructor
    Game(unsigned int seed, std::vector<std::pair<Resource, int>> tileInfo);
    Game(unsigned int seed, std::vector<std::pair<Resource, int>> tileInfo, int turn, int geese, std::vector<Color> roadInfo, std::vector<std::pair<Color, Residence>> buildInfo, 
            std::map<Color, int> playerPoints, std::map<Color, std::map<Resource, int>> playerResources, std::map<Color, std::map<int, Residence>> playerResidences, std::map<Color, std::vector<int>> playerRoads);

    //
    void playGame();   

    //
    void setBasements();

};

#endif

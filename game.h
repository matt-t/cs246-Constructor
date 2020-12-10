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
    int seed;
    std::vector<std::unique_ptr<Player>> players;
    std::unique_ptr<Board> board;
    int turn;
    int winner;

    /* Private methods */
    
    //
    void handleRollPhase(Player &player, std::string move, int &movePhase);
    
    //
    void handleActionPhase(Player &player, std::string move, int &movePhase);

    public:
    
    // Constructor
    Game(int seed, std::vector<std::pair<Resource, int>> tileInfo);
    Game(int seed, std::vector<std::pair<Resource, int>> tileInfo, int turn, int geese, std::vector<Color> roadInfo, std::vector<std::pair<Color, Residence>> buildInfo, 
            std::map<Color, int> playerPoints, std::map<Color, std::map<Resource, int>> playerResources, std::map<Color, std::map<int, Residence>> playerResidences, std::map<Color, std::vector<int>> playerRoads);


    //
    void save();
    
    //
    void status();

    //
    void residences(Player &player);
    
    //
    void help(int movePhase) noexcept;
    
    //
    void printBoard();
    
    //
    void next() noexcept;
    
    //
    void playGame();   

};

#endif

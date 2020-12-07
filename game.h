#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include "player.h"
#include "board.h"

class Game {
    /* Private Variables */
    std::vector<std::unique_ptr<Player>> players;
    Board board;
    int turn;
    int winner;

    /* Private methods */
    
    //
    void handleRollPhase(Player &player);
    
    //
    void handleActionPhase(Player &player);

    public:
    
    // Constructor
    Game();

    //
    void save();
    
    //
    void status(Player &player);

    //
    void residences(Player &player);
    
    //
    void help() noexcept;
    
    //
    void printBoard();
    
    //
    void next() noexcept;
    
    //
    void playGame();   

};

#endif

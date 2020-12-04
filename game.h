#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
//#include "player.h"
//#include "board.h"

class Game {
    /* Private Variables */
    std::vector<std::unique_ptr<Player>> players;
    Board board;
    int turn = 0;
    int winner = -1;

    /* Private methods */
    
    //
    void handleRollPhase(Player player);
    
    //
    void handleActionPhase(Player player);

    public:
    
    //
    void save();
    
    //
    void status();
    
    //
    void help() noexcept;
    
    //
    void printBoard();
    
    //
    void next();
    
    //
    void playGame();   

}

#endif

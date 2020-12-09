#include <iostream>
#include "game.h"
#include "enums.h"

using namespace std;

Game::Game(std::vector<std::pair<Resource, int>> tileInfo): board{make_unique<Board>(tileInfo)}, turn{0}, winner{-1} 
{
    // for (int i = 0; i < 4; i++) {
    //     std::unique_ptr<Player> plyr = make_unique<Player>();
    //     players.emplace_back(std::move(plyr));
    // }
}
    

Game::Game(std::vector<std::pair<Resource, int>> tileInfo, int turn, int geese, std::vector<Color> roadInfo, std::vector<std::pair<Color, Residence>> buildInfo):
    board{make_unique<Board>(tileInfo, roadInfo, buildInfo, geese)}, turn{turn}, winner{-1}
{
    // cerr << "yikers" << endl;
    // for (int i = 0; i < 4; i++) {
    //     std::unique_ptr<Player> plyr = make_unique<Player>();
    //     players.emplace_back(std::move(plyr));
    // }
}

void Game::save() {

}

void Game::status(Player &player) {
    cout << "player.getColor()" << " has " << player.getPoints() << " building points, ";
    auto resources = player.getResources();
    cout << resources[Resource::Brick];
    // <numBrick> brick, <numEnergy> energy, <numGlass> glass, ;
    // <numHeat> heat, and <numWifi> wifi << endl;
}

void Game::residences(Player &player) {
    auto residences = player.getResidences();
    for (auto &res: residences) {
        cout << res.first << ": "<<  res.second << endl;
    }
}
    
void Game::help() noexcept {
    cout << "Valid Commands:" << endl;
    cout << "board" << endl;
    cout << "status" << endl;
    cout << "residences" << endl;
    cout << "build-road <edge#>" << endl;
    cout << "build-res <housing#>" << endl;
    cout << "improve-res <houseing#>" << endl;
    cout << "trade <colour> <give> <take>" << endl;
    cout << "next" << endl;
    cout << "save <file>" << endl;
    cout << "help" << endl;
}
    
void Game::printBoard() {

}

void Game::next() noexcept {
    if (turn == 3) {
        turn = 0;
    } else {
        ++turn;
    }
}

void Game::handleRollPhase(Player &player, string move, int &movePhase) {
    if (move == "roll") {
        cout << "roll" << endl;
        ++movePhase;
    } else if (move == "load") {
        cout << "load" << endl;
    } else if (move == "fair") {
        cout <<  "fair" << endl;
    } else {
        cout << "Invalid command." << endl;
    }
}
    

void Game::handleActionPhase(Player &player, string move, int &movePhase) {
    if (move == "board") {
        cout << *board << endl;
        cout << "board" << endl;
    } else if (move == "status") {
        cout << "status" << endl;
    } else if (move == "residences") {
        cout << "residences" << endl;
    } else if (move == "build-road") {
        try {
            int edge;
            cin >> edge;
            cout << "build-road" << endl;
        } catch (invalid_argument & e) {
            cerr << e.what() << endl;       //whatever function error gives
        }
    } else if (move == "build-res") {
        try {
            int vertex;
            cin >> vertex;
            cout << "build-res" << endl;
        } catch (invalid_argument & e) {
            cerr << e.what() << endl;       //whatever function error gives
        }
    } else if (move == "improve") {
        try {
            int vertex;
            cin >> vertex;
            cout << "improve" << endl;
        } catch (invalid_argument & e) {
            cerr << e.what() << endl;       //whatever function error gives
        }
    } else if (move == "trade") {
        try {
            string color, resourceGive, resourceTake;
            cin >> color >> resourceGive >> resourceTake;
            cout << "trade" << endl;
            //function to call trade
        } catch (invalid_argument & e) {
            //whatever function error gives
        }
    } else if (move == "next") {
        // CHECK IF WINNER
        if (player.getPoints() == 10) {
            winner = turn;
        }
        next();
        --movePhase;
    } else if (move == "save") {
        cout << "save" << endl;
    } else if (move == "help") {
        help();
        cout << "help" << endl;
    } else {
        cout << "Invalid command." << endl;
    }
}

void Game::playGame() {
    
    // setting up of basements  --> take arg to determine if u need to set up basement or not
    cout << "IN PLAY GAME" << endl;
    cout << *board << endl;
    int movePhase = 0;
    string move;
    while(cin >> move && winner == -1) {
        if (movePhase == 0) {
            cout << "0" << endl;
            movePhase++;
            //handleRollPhase(*players[turn], move, movePhase);
        } else {
            cout << "1" << endl;
            movePhase--;
            //handleActionPhase(*players[turn], move, movePhase);
        }
    }

    if (winner == -1) {         // if while loop ended cause EOF auto save game
        save();
    } 
} 


#include <iostream>
#include "game.h"

using namespace std;

// need to set up constructor

void Game::save() {

}

void Game::status() {

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

void Game::next() {
    if (turn == 3) {
        turn = 0;
    } else {
        ++turn;
    }
}

void Game::handleRollPhase() {
    cout << "Builder <colour>’s turn" << endl;
    string move;
    while(cin >> move) {
        try {
            if (move == "roll") {
                cout << "roll" << endl;
                break;
            } else if (move == "load") {
                cout << "load" << endl;
            } else if (move == "fair") {
                cout <<  "fair" << endl;
            } else {
                cout << "Invalid command." << endl;
            }
        } catch (invalid_argument & e) {
            cerr << e.what() << endl;       // return exception thrown by function called
        }
    }
}
    

void Game::handleActionPhase() {
    string move;
    while(cin >> move) {
        try {
            if (move == "board") {
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
                    //whatever function error gives
                }
            } else if (move == "build-res") {
                try {
                    int vertex;
                    cin >> vertex;
                    cout << "build-res" << endl;
                } catch (invalid_argument & e) {
                    //whatever function error gives
                }
            } else if (move == "improve") {
                int vertex;
                cin >> vertex;
                cout << "improve" << endl;
            } else if (move == "trade") {
                try {
                    string color, resourceGive, resourceTake;
                    cin >> color >> resourceGive >> resourceTake;
                    //function to call trade
                } catch (invalid_argument & e) {
                    //whatever function error gives
                }
                cout << "trade" << endl;
            } else if (move == "next") {
                next();
                cout << "next" << endl;
                break;
            } else if (move == "save") {
                cout << "save" << endl;
            } else if (move == "help") {
                help();
                cout << "help" << endl;
            } else {
                cout << "Invalid command." << endl;
            }
        } catch (invalid_argument & e) {
            cerr << e.what() << endl;       // return exception thrown by function called
        }
    }
}



void Game::playGame() {
    cin.exceptions(ios::eofbit|ios::failbit);
    
    // setting up of basements  --> take arg to determine if u need to set up basement or not

    // NEED MORE THOUGHT/INPUT OF CATCHING EOF FOR INPUT AND WHEN TO SAVE

    while(winner == -1) {
        try {
            //handleRollPhase(players[turn]);
            //handleActionPhase(players[turn]);
        } catch (ios::failure &) { save(); } 
    } 
} 

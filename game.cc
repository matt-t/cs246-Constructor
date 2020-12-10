#include <iostream>
#include "game.h"
#include "enums.h"

using namespace std;

Game::Game(int seed, vector<pair<Resource, int>> tileInfo): 
    seed{ seed }, board{make_unique<Board>(tileInfo)}, turn{0}, winner{-1} 
{
    auto b = make_unique<Player>(Color::Blue);
    players.push_back(move(b));
    auto r = make_unique<Player>(Color::Red);
    players.push_back(move(r));
    auto o = make_unique<Player>(Color::Orange);
    players.push_back(move(o));
    auto y = make_unique<Player>(Color::Yellow);
    players.push_back(move(y));
    // for (int i = 0; i < 4; i++) {
    //     std::unique_ptr<Player> plyr = make_unique<Player>();
    //     players.emplace_back(std::move(plyr));
    // }
}
    

Game::Game(int seed, vector<pair<Resource, int>> tileInfo, int turn, int geese, vector<Color> roadInfo, vector<pair<Color, Residence>> buildInfo, 
            vector<int>playerPoints, vector<map<Resource, int>>playerResources, vector<map<int, Residence>>playerResidences, vector<vector<int>>playerRoads):
    seed{ seed }, board{make_unique<Board>(tileInfo, roadInfo, buildInfo, geese)}, turn{turn}, winner{-1}
{
    auto b = make_unique<Player>(Color::Blue,  playerPoints[0], playerResources[0], playerResidences[0], playerRoads[0]);
    players.push_back(move(b));
    auto r = make_unique<Player>(Color::Red, playerPoints[1],  playerResources[1], playerResidences[1], playerRoads[1]);
    players.push_back(move(r));
    auto o = make_unique<Player>(Color::Orange, playerPoints[2], playerResources[2], playerResidences[2], playerRoads[2]);
    players.push_back(move(o));
    auto y = make_unique<Player>(Color::Yellow, playerPoints[3], playerResources[3], playerResidences[3], playerRoads[3]);
    players.push_back(move(y));
    // cerr << "yikers" << endl;
    // for (int i = 0; i < 4; i++) {
    //     std::unique_ptr<Player> plyr = make_unique<Player>();
    //     players.emplace_back(std::move(plyr));
    // }
}

void Game::save() {

}

void Game::status(Player &player) {
    cout << player.getColor() << " has " << player.getPoints() << " building points, ";
    auto resources = player.getResources();
    cout << resources[Resource::Brick] << " brick, ";
    cout << resources[Resource::Energy] << " energy, ";
    cout << resources[Resource::Glass] << " glass, ";
    cout << resources[Resource::Heat] << " heat, and ";
    cout << resources[Resource::Wifi] << " wifi" << endl;
}

void Game::residences(Player &player) {
    cout << player.getPoints() << endl;
    auto residences = player.getResidences();
    for (auto res: residences) {
        cout << res.first << ": "<<  res.second << endl;
    }
}
    
void Game::help(int movePhase) noexcept {
    cout << "Valid Commands:" << endl;
    if (movePhase) {
        cout << "~ board" << endl;
        cout << "~ status : prints the current status of all builders in order from builder 0 to 3." << endl;
        cout << "~ residences" << endl;
        cout << "~ build-road <road#> : attempts to builds the road at <road#>." << endl;
        cout << "~ build-res <housing#> : attempts to builds a basement at <housing#>." << endl;
        cout << "~ improve-res <housing#> : attempts to improve the residence at <housing#>." << endl;
        cout << "~ trade <colour> <give> <take> : attempts to trade with builder <colour>, giving one resource of type <give> and receiving one resource of type <take>." << endl;
        cout << "~ next : passes control onto the next builder in the game." << endl;
        cout << "~ save <file> : saves the current game state to <file>." << endl;
    } else {
        cout << "~ load : changes current builder's dice type to 'loaded'" << endl;
        cout << "~ fair : changes current builder's dice type to 'fair'" << endl;
        cout << "~ roll : rolls the dice and distributes resources." << endl;
        cout << "~ status : prints the current status of all builders in order from builder 0 to 3." << endl;
    }
    cout << "~ help : prints out the list of commands." << endl;
}
    
void Game::printBoard() {

}

void Game::next() noexcept {
    cout << *board;
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
    } else if (move == "help") {
        help(movePhase);
    } else {
        cout << "Invalid command." << endl;
    }
}
    

void Game::handleActionPhase(Player &player, string move, int &movePhase) {
    if (move == "board") {
        cout << *board << endl;
        cout << "board" << endl;
    } else if (move == "status") {
        status(player);
    } else if (move == "residences") {
        residences(player);
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
            /*
            auto playerReceive = COPY(*player)
            auto playerLose = COPY(players.find())
            playerReceive
            */
        } catch (invalid_argument & e) {
            //whatever function error gives
        }
    } else if (move == "next") {
        if (player.getPoints() >= 10) {         // CHECK IF WINNER
            winner = turn;
        }
        next();
        --movePhase;
        cout << turn << " " << winner << endl;
    } else if (move == "save") {
        cout << "save" << endl;
    } else if (move == "help") {
        help(movePhase);
    } else {
        cout << "Invalid command." << endl;
    }
}

void Game::playGame() {
    
    // setting up of basements  --> take arg to determine if u need to set up basement or not
    cout << *board << endl;
    int movePhase = 0;
    string move;
    cout << "Builder " << players[turn]->getColor() << "'s turn." << endl;
    while (winner == -1) {
        if (movePhase) {
            cout << "Enter a command:" << endl;
        }
        cout << "> ";

        if (cin >> move) {
            if (movePhase == 0) {
                handleRollPhase(*players[turn], move, movePhase);
            } else {
                int temp = turn;
                handleActionPhase(*players[turn], move, movePhase);
                if (temp != turn && winner == -1) {
                    // PRINT BOARD
                    cout << "Builder " << players[turn]->getColor() << "'s turn." << endl;
                } else if (winner != -1) {
                    turn = temp;        // undo the next turn move cause we ending the game 
                }
            }
        } else {
            break;
        }
    }

    if (winner == -1) {         // if while loop ended cause EOF auto save game
        save();
    } else {                    // if while loop ended cause player won 
        cout << "Congratulations!! " << "<COLOR" << " wins!!" << endl;
    }
} 


#include <iostream>
#include <fstream>
#include <algorithm>
#include "game.h"
#include "enums.h"
#include "constants.h"


using namespace std;

Game::Game(int seed, vector<pair<Resource, int>> tileInfo): 
    seed{ seed }, board{make_unique<Board>(tileInfo)}, turn{0}, winner{-1} 
{
    for (const Color &color: COLOR_ORDER) {
        // std::unique_ptr<Player> temp = std::make_unique<Player>(color);
        players[color] = std::make_unique<Player>(color);
    }
    // auto b = make_unique<Player>(Color::Blue);
    // players.push_back(move(b));
    // auto r = make_unique<Player>(Color::Red);
    // players.push_back(move(r));
    // auto o = make_unique<Player>(Color::Orange);
    // players.push_back(move(o));
    // auto y = make_unique<Player>(Color::Yellow);
    // players.push_back(move(y));
    // for (int i = 0; i < 4; i++) {
    //     std::unique_ptr<Player> plyr = make_unique<Player>();
    //     players.emplace_back(std::move(plyr));
    // }
}
    

Game::Game(int seed, vector<pair<Resource, int>> tileInfo, int turn, int geese, vector<Color> roadInfo, vector<pair<Color, Residence>> buildInfo, 
            std::map<Color, int> playerPoints, map<Color, map<Resource, int>> playerResources, map<Color, map<int, Residence>> playerResidences, map<Color, vector<int>> playerRoads):
    seed{ seed }, board{make_unique<Board>(tileInfo, roadInfo, buildInfo, geese)}, turn{turn}, winner{-1}
{
    for (const Color &color: COLOR_ORDER) {
        players[color] = std::make_unique<Player>(color, playerPoints[color], playerResources[color], playerResidences[color], playerRoads[color]);
        // players.push_back(std::move(temp));
    }
}

void Game::save() {

}

void Game::status() {
    for (const Color &color: COLOR_ORDER) {
        cout << color << " has " << players[color]->getPoints() << " building points, ";
        auto resources = players[color]->getResources();
        cout << resources[Resource::Brick] << " " << RESOURCE_BRICK_STRING << ", ";
        cout << resources[Resource::Energy] << " " << RESOURCE_ENERGY_STRING << ", ";
        cout << resources[Resource::Glass] << " " << RESOURCE_GLASS_STRING << ", ";
        cout << resources[Resource::Heat] << " " << RESOURCE_HEAT_STRING << " , and ";
        cout << resources[Resource::Wifi] << " " << RESOURCE_WIFI_STRING << "." << endl;
    }
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
    
void Game::printBoard() { //might not need this 

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
        
        player.changeDice(DiceType::Fair);//for testing
        int getRoll = player.rollDice(seed);
        cout << getRoll << endl;

        if (getRoll == 4) {
            //players with 10 or more resource lose half resources
            set<Color> unluckyPlayers = board->getLocationPlayers(board->getGeese());
            for (auto p : unluckyPlayers) {//check if more than 10 resource
                if (players[p]->totalResource() >= 10){
                    int half = players[p]->totalResource() / 2;
                    cout << "Builder " << COLOR_TO_STRING.at(p) << " loses " << half << " resources to the geese. They lose:" << endl;
                    for (int i = half; i > 0; --i){
                        Resource stolen = players[p]->generateRandomResource();
                        players[p]->takeResource(stolen, 1);
                        cout << "1 " << RESOURCE_TO_STRING.at(stolen) << endl;
                    }//need to cout message correctly
                }
            }
            //roller chooses position and notify board 
            cout << "Choose where to place the GEESE." << endl;
            int newGeeseTile;
            bool changed = false;
            while (changed == false){
                cin >> newGeeseTile;
                if (cin.fail()){
                    cin.clear();
                    cin.ignore(256,'\n');
                    cout << "ERROR: Choose a valid integer." << endl; 
                } else {
                    try {
                        board->changeGeese(newGeeseTile);
                        changed = true;
                    } catch(GeeseExistsHereException& e) {
                        cout << "ERROR: The geese already exists here. Choose somewhere else." << endl;
                    } catch(GeeseOutOfRange& e) {
                        cout << "ERROR: Tile selection is invalid." << endl;
                    }
                }
            }
            //cout who roller can steal from
            set<Color> stealAvailable = board->getLocationPlayers(newGeeseTile);
            for (auto p : stealAvailable) { //check if have resources, if not removed from the set
                if (players[p]->totalResource() == 0 || p == player.getColor()){
                    stealAvailable.erase(p);
                }
            }
            if (stealAvailable.size() != 0){
                cout << "Builder " << player.getColor() << " can choose to steal from ";
                for (auto p : stealAvailable){
                    cout << COLOR_TO_STRING.at(p) << " ";
                } cout << endl;
                //choose who to steal from
                string stealFrom;
                while (cin >> stealFrom) {
                    transform(stealFrom.begin(), stealFrom.end(), stealFrom.begin(), ::toupper);
                    if (STRING_TO_COLOR.count(stealFrom) == 0){
                        cout << "ERROR: Choose a valid player." << endl;
                    } else {
                        break;
                    }
                }
                Color stealing = STRING_TO_COLOR.at(stealFrom);
                //steals random resource 
                Resource stolen = players[stealing]->generateRandomResource();
                players[stealing]->takeResource(stolen, 1);
                player.addResource(stolen, 1);
                cout << "Builder " << COLOR_TO_STRING.at(player.getColor()) << " steals " << RESOURCE_TO_STRING.at(stolen) << " from builder " << COLOR_TO_STRING.at(stealing) << endl;
            }
            } else {
            //produce resource from the tiles rolled
            
        }
        ++movePhase;
    } else if (move == "load") {
        cout << "load" << endl;
    } else if (move == "fair") {
        cout <<  "fair" << endl;
    } else if (move == "help") {
        help(movePhase);
    } else if (move == "status") {
        status();
    } else {
        cout << "Invalid command." << endl;
    }
}
    

void Game::handleActionPhase(Player &player, string move, int &movePhase) {
    if (move == "board") {
        cout << *board << endl;
        cout << "board" << endl;
    } else if (move == "status") {
        status();
    } else if (move == "residences") {
        residences(player);
    } else if (move == "build-road") {
        try {
            int edge;
            cin >> edge;
            unique_ptr<Player> tempSelf = make_unique<Player>(player);
            tempSelf->buildRoad(edge);
            board->buildRoad(player.getColor(), edge);
            std::swap(players[player.getColor()], tempSelf);
            cout << "build-road at " << edge << " for " << player.getColor() << endl;
        } catch (invalid_argument & e) {
            cerr << e.what() << endl;       //whatever function error gives
        }
    } else if (move == "build-res") {
        try {
            int vertex;
            cin >> vertex;
            unique_ptr<Player> tempSelf = make_unique<Player>(player);
            tempSelf->buildResidence(vertex);
            board->buildResidence(player.getColor(), vertex);
            std::swap(players[player.getColor()], tempSelf);
            cout << "build-res at " << vertex << " for " << player.getColor() << endl;
        } catch (invalid_argument & e) {
            cerr << e.what() << endl;       //whatever function error gives
        }
    } else if (move == "improve") {
        try {
            int vertex;
            cin >> vertex;
            unique_ptr<Player> tempSelf = make_unique<Player>(player);
            tempSelf->upgradeResidence(vertex);
            board->upgradeResidence(player.getColor(), vertex);
            std::swap(players[player.getColor()], tempSelf);
            cout << "upgrading residence at " << vertex << " for " << player.getColor() << endl;
        } catch (invalid_argument & e) {
            cerr << e.what() << endl;       //whatever function error gives
        }
    } else if (move == "trade") {
        try {
            string color, resourceGive, resourceTake;
            while(true) {
                cin >> color;
                std::transform(color.begin(), color.end(), color.begin(), [](char c) {return std::toupper(c);});
                if (STRING_TO_COLOR.count(color) == 0) {
                    cout << "ur a troll, give me a real colour" << endl;
                } else {
                    break;
                }
            }
            while(true) {
                cin >> resourceGive;
                std::transform(resourceGive.begin(), resourceGive.end(), resourceGive.begin(), [](char c) {return std::toupper(c);});
                if (STRING_TO_RESOURCE.count(resourceGive) == 0) {
                    cout << "ur a troll, give me a real resource" << endl;
                } else {
                    break;
                }
            }
            while(cin >> resourceTake) {
                std::transform(resourceTake.begin(), resourceTake.end(), resourceTake.begin(), [](char c) {return std::toupper(c);});
                if (STRING_TO_RESOURCE.count(resourceTake) == 0) {
                    cout << "ur a troll, give me a real resource" << endl;
                } else {
                    break;
                }
            }
            cout << "trading between " << player.getColor() << " and " << color << " with " << resourceGive << " + " << resourceTake << endl;
            
            unique_ptr<Player> tempOther = make_unique<Player>(*players[STRING_TO_COLOR.at(color)]);
            unique_ptr<Player> tempSelf = make_unique<Player>(player);
            tempOther->takeResource(STRING_TO_RESOURCE.at(resourceTake), 1);
            tempSelf->addResource(STRING_TO_RESOURCE.at(resourceGive), 1);
            std::swap(players[STRING_TO_COLOR.at(color)], tempOther);
            std::swap(players[player.getColor()], tempSelf);
            
            /*
            auto playerReceive = COPY(*player)
            auto playerLose = COPY(players.find())
            playerReceive.addResources()
            */
        } catch (invalid_argument & e) {
            //whatever function error gives
        } catch (...) {
            throw "wtf";
        }
    } else if (move == "next") {
        if (player.getPoints() >= 10) {         // CHECK IF WINNER
            winner = turn;
        }
        next();
        --movePhase;
        cout << turn << " " << winner << endl;
    } else if (move == "save") {
        string save_file_name;
        cin >> save_file_name;
        ofstream save_file {save_file_name};
        if (!save_file) {
            cerr << "can't open output file" << endl;
            exit(1);
        }
        save_file << turn << endl;
        for (const Color &color: COLOR_ORDER) {
            save_file << players[color]->getResources()[Resource::Brick] << " ";
            save_file << players[color]->getResources()[Resource::Energy] << " ";
            save_file << players[color]->getResources()[Resource::Glass] << " ";
            save_file << players[color]->getResources()[Resource::Heat] << " ";
            save_file << players[color]->getResources()[Resource::Wifi] << " ";
            save_file << 'r' << " ";
            for (const int &road : players[color]->getRoads()) {
                save_file << road << " ";
            }
            save_file << 'h';
            for (const auto &residence : players[color]->getResidences()) {
                save_file << " " << residence.first << " " << RESIDENCE_TO_CHAR.at(residence.second);
            }
            save_file << endl;
        }
        for (int tileNum = 0; tileNum < NUM_TILES; tileNum++) {
            save_file << RESOURCE_TO_SAVE_NUM.at(board->getTileResource(tileNum)) << " " << board->getTileRollNum(tileNum);
            if (tileNum != NUM_TILES - 1) {
                save_file << " ";
            }
        }
        save_file << endl;
        save_file << board->getGeese() << endl;
        
        cout << "Saving to " << save_file_name << "..."<< endl;
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
    cout << "Builder " << COLOR_ORDER.at(turn) << "'s turn." << endl;
    while (winner == -1) {
        if (movePhase) {
            cout << "Enter a command:" << endl;
        }
        cout << "> ";

        if (cin >> move) {
            if (movePhase == 0) {
                handleRollPhase(*players[COLOR_ORDER.at(turn)], move, movePhase);
            } else {
                int temp = turn;
                handleActionPhase(*players[COLOR_ORDER.at(turn)], move, movePhase);
                if (temp != turn && winner == -1) {
                    // PRINT BOARD
                    cout << "Builder " << COLOR_ORDER.at(turn) << "'s turn." << endl;
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
        cout << "Congratulations!! " << COLOR_ORDER.at(turn) << " wins!!" << endl;
    }
} 


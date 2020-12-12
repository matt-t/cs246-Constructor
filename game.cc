#include <iostream>
#include <fstream>
#include <algorithm>
#include "game.h"
#include "enums.h"
#include "constants.h"
#include "eofException.h"

using namespace std;

Game::Game(unsigned int seed, vector<pair<Resource, int>> tileInfo): 
    seed{ seed }, board{make_unique<Board>(tileInfo)}, turn{0}, winner{-1} 
{
    for (const Color &color: COLOR_ORDER) {
        // std::unique_ptr<Player> temp = std::make_unique<Player>(color);
        players[color] = std::make_unique<Player>(color);
    }
}
    

Game::Game(unsigned int seed, vector<pair<Resource, int>> tileInfo, int turn, int geese, vector<Color> roadInfo, vector<pair<Color, Residence>> buildInfo, 
            std::map<Color, int> playerPoints, map<Color, map<Resource, int>> playerResources, map<Color, map<int, Residence>> playerResidences, map<Color, vector<int>> playerRoads):
    seed{ seed }, board{make_unique<Board>(tileInfo, roadInfo, buildInfo, geese)}, turn{turn}, winner{-1}
{
    for (const Color &color: COLOR_ORDER) {
        players[color] = std::make_unique<Player>(color, playerPoints[color], playerResources[color], playerResidences[color], playerRoads[color]);
        // players.push_back(std::move(temp));
    }
}

void Game::save(bool exitGame) {
    string save_file_name = DEFAULT_SAVE_FILE;
    if (!exitGame){
        cin >> save_file_name;
        if (cin.eof()) {
            throw EOFException();
        }
    }

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
        save_file << RESOURCE_TO_INT.at(board->getTileResource(tileNum)) << " " << board->getTileRollNum(tileNum);
        if (tileNum != NUM_TILES - 1) {
            save_file << " ";
        }
    }
    save_file << endl;
    save_file << board->getGeese() << endl;
    
    cout << "Saving to " << save_file_name << "..."<< endl;
}

void Game::status() noexcept {
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
    cout << "Builder " << player.getColor() << " has " << player.getPoints() << " building points." << endl;
    cout << "Builder " << player.getColor()<< " has built:"  << endl;
    auto residences = player.getResidences();
    for (const auto &res: residences) {
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

void Game::next() noexcept {
    cout << *board;
    ++turn;
}

void Game::handleGoose(Player &player){
//players with 10 or more resource lose half resources
    set<Color> unluckyPlayers = board->getLocationPlayers(board->getGeese());
    for (auto p : unluckyPlayers) {//check if more than 10 resource
        if (players[p]->totalResource() >= 10){
            map<Resource, int> coutStolen;
            int half = players[p]->totalResource() / 2;
            cout << "Builder " << COLOR_TO_STRING.at(p) << " loses " << half << " resources to the goose. They lose:" << endl;
            for (int i = half; i > 0; --i){
                Resource stolen = players[p]->generateRandomResource();
                players[p]->takeResource(stolen, 1);
                coutStolen[stolen]++;
            }
            for (auto stolenResource : coutStolen){
                cout << stolenResource.second << " " << RESOURCE_TO_STRING.at(stolenResource.first) << endl;
            }
        }
    }
    //roller chooses position and notify board 
    cout << "Choose where to place the GOOSE." << endl;
    int newGeeseTile;
    bool changed = false;
    while (changed == false){
        cin >> newGeeseTile;
        if (cin.eof()) {
            throw EOFException();
        }
        if (cin.fail()){
            cin.clear();
            cin.ignore(256,'\n');
            cout << "ERROR: Choose a valid integer." << endl; 
        } else {
            try {
                board->changeGeese(newGeeseTile);
                changed = true;
            } catch(GeeseExistsHereException& e) {
                cout << "ERROR: The goose already exists here. Choose somewhere else." << endl;
            } catch(GeeseOutOfRange& e) {
                cout << "ERROR: Tile selection is invalid." << endl;
            }
        }
    }
    //cout who roller can steal from
    set<Color> stealAvailable = board->getLocationPlayers(newGeeseTile);
    for (const auto &color : COLOR_ORDER) { //check if have resources, if not removed from the set
        if (players[color]->totalResource() == 0 || color == player.getColor()){
            stealAvailable.erase(color);
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
        if (cin.eof()) {
            throw EOFException();
        }
        Color stealing = STRING_TO_COLOR.at(stealFrom);
        //steals random resource 
        Resource stolen = players[stealing]->generateRandomResource();
        players[stealing]->takeResource(stolen, 1);
        player.addResource(stolen, 1);
        cout << "Builder " << COLOR_TO_STRING.at(player.getColor()) << " steals " << RESOURCE_TO_STRING.at(stolen) << " from builder " << COLOR_TO_STRING.at(stealing) << endl;
    }
}

void Game::handleRollMove(Player &player, string move, int &movePhase) {
    if (move == "roll") {
        //cout << "roll" << endl;
        int getRoll = player.rollDice();
        cout << getRoll << " is rolled."<< endl;
        if (getRoll == 7) {
            handleGoose(player);
        } else {
            //produce resource from the tiles rolled
            map<Color, map<Resource, int>> playersGet = board->getRollResources(getRoll);
            for (const auto &color : playersGet) {
                cout << "Builder "<<color.first << " got:"<< endl;
                for (const auto resource : color.second) {
                    cout << resource.second << " "<<  resource.first  << endl;
                }
            }
        }
        ++movePhase;
    } else if (move == "load") {
        player.changeDice(DiceType::Loaded);
        cout << "Dice changed to loaded dice." << endl;
    } else if (move == "fair") {
        player.changeDice(DiceType::Fair);
        cout <<  "Dice changed to fair dice." << endl;
    } else if (move == "help") {
        help(movePhase);
    } else if (move == "status") {
        status();
    } else {
        cout << "Invalid command." << endl;
    }
}
    

void Game::handleActionMove(Player &player, string move, int &movePhase) {
    if (move == "board") {
        cout << *board << endl;
        cout << "board" << endl;
    } else if (move == "status") {
        status();
    } else if (move == "residences") {
        residences(player);
    } else if (move == "build-road") {
        bool roadChosen = false;
        int edge;
        while (roadChosen == false){
            cin >> edge;
            if (cin.eof()) {
                throw EOFException();
            }
            if (cin.fail()){
                cin.clear();
                cin.ignore(256,'\n');
                cout << "ERROR: Choose a valid location." << endl; 
                continue;
            } else {roadChosen = true;}
            try {
                unique_ptr<Player> tempSelf = make_unique<Player>(player);
                tempSelf->buildRoad(edge);
                board->buildRoad(player.getColor(), edge);
                std::swap(players[player.getColor()], tempSelf);
                cout << "build-road at " << edge << " for " << player.getColor() << endl; // for testing
            } catch (RoadExistsException& e){
                cout << "ERROR: Someone has already built a road here." << endl;
            } catch(InvalidRoadLocationException& e) {
                cout << "ERROR: Cannot build road here. None of your road or residence connect to here." << endl;
            } catch (InsufficientResourceException &e) {
                cout << "ERROR: You do not have enough resources." << endl;
            } catch (InvalidLocationException& e){
                cout << "ERROR: Enter a valid location." << endl;
            }
        }
    } else if (move == "build-res") {
        int vertex;
        bool vertexChosen = false;
        while (vertexChosen == false){
            cin >> vertex;
            if (cin.eof()) {
                throw EOFException();
            }
            if (cin.fail()){
                cin.clear();
                cin.ignore(256,'\n');
                cout << "ERROR: Choose a valid location." << endl; 
                continue;
            } else {vertexChosen = true;}
            try {
                unique_ptr<Player> tempSelf = make_unique<Player>(player);
                tempSelf->buildResidence(vertex);
                board->buildResidence(player.getColor(), vertex);
                std::swap(players[player.getColor()], tempSelf);
                cout << "build-res at " << vertex << " for " << player.getColor() << endl; // for testing
            } catch (BuildingExistsException& e){
                cout << "ERROR: A residence already exists here." << endl;
            } catch (InvalidLocationException& e){
                cout << "ERROR: You cannot build here." << endl;
            } catch (InsufficientResourceException & e) {
                cout << "You do not have enough resources." << endl;
            }
        }
    } else if (move == "improve-res") {
        int vertex;
        bool vertexChosen = false;
        while (vertexChosen == false){
            cin >> vertex;
            if (cin.eof()) {
                throw EOFException();
            }
            if (cin.fail()){
                cin.clear();
                cin.ignore(256,'\n');
                cout << "ERROR: Choose a valid location." << endl; 
                continue;
            } else {vertexChosen = true;}
            try {
                int vertex;
                unique_ptr<Player> tempSelf = make_unique<Player>(player);
                tempSelf->upgradeResidence(vertex);
                board->upgradeResidence(player.getColor(), vertex);
                std::swap(players[player.getColor()], tempSelf);
                cout << "upgrading residence at " << vertex << " for " << player.getColor() << endl; // for testing
            } catch(BuidingNotOwnedException& e){
                cout << "ERROR: You do not own this residence." << endl;
            } catch(InvalidLocationException& e){
                cout << "ERROR: Enter a valid location." << endl;
            } catch (AlreadyTowerException& e){
                cout << "ERROR: Your residence is already a Tower." << endl;
            } catch (InsufficientResourceException & e) {
                cout << "You do not have enough resources." << endl;
            }
        }
    } else if (move == "trade") {
        try {
            string color, resourceGive, resourceTake;
            while(cin >> color) {
                std::transform(color.begin(), color.end(), color.begin(), ::toupper);
                if (STRING_TO_COLOR.count(color) == 0 || STRING_TO_COLOR.at(color) == player.getColor()) {
                    cout << "Enter a VALID player color to trade with." << endl;
                } else {
                    break;
                }
            }
            while(cin >> resourceGive) {
                std::transform(resourceGive.begin(), resourceGive.end(), resourceGive.begin(), ::toupper);
                if (STRING_TO_RESOURCE.count(resourceGive) == 0) {
                    cout << "Enter a VALID resource you want to trade with." << endl;
                } else {
                    break;
                }
            } 
            while(cin >> resourceTake) {
                std::transform(resourceTake.begin(), resourceTake.end(), resourceTake.begin(), ::toupper);
                if (STRING_TO_RESOURCE.count(resourceTake) == 0) {
                    cout << "Enter a VALID resource you want to trade for." << endl;
                } else {
                    break;
                }
            }
            if (cin.eof()) {
                throw EOFException();
            }
            cout << "trading between " << player.getColor() << " and " << color << " with " << resourceGive << " for " << resourceTake << endl; // for testing
            
            unique_ptr<Player> tempOther = make_unique<Player>(*players[STRING_TO_COLOR.at(color)]);
            unique_ptr<Player> tempSelf = make_unique<Player>(player);
            tempOther->takeResource(STRING_TO_RESOURCE.at(resourceTake), 1);
            tempSelf->addResource(STRING_TO_RESOURCE.at(resourceGive), 1);
            std::swap(players[STRING_TO_COLOR.at(color)], tempOther);
            std::swap(players[player.getColor()], tempSelf);
            
        } catch (InsufficientResourceException & e) {
            cout << "You do not have enough resources." << endl;
        }
    } else if (move == "next") {
        if (player.getPoints() >= 10) {         // CHECK IF WINNER
            winner = turn % 4;
        }
        next();
        --movePhase;
        cout << turn % 4 << " " << winner << endl;
    } else if (move == "save") {
        save();
    } else if (move == "help") {
        help(movePhase);
    } else {
        cout << "Invalid command." << endl;
    }
}

void Game::playGame() {
    cout << *board << endl;
    int movePhase = 0;
    string move;
    cout << "Builder " << COLOR_ORDER.at(turn % 4) << "'s turn." << endl;
    while (winner == -1) {
        if (movePhase) {
            cout << "Enter a command:" << endl;
        }
        cout << "> ";
        try {
            if (cin >> move) {
                if (movePhase == 0) {
                    handleRollMove(*players[COLOR_ORDER.at(turn % 4)], move, movePhase);
                } else {
                    int temp = turn % 4;
                    handleActionMove(*players[COLOR_ORDER.at(turn % 4)], move, movePhase);
                    if (temp != turn % 4 && winner == -1) {
                        // PRINT BOARD
                        cout << "Builder " << COLOR_ORDER.at(turn % 4) << "'s turn." << endl;
                    } else if (winner != -1) {
                        --turn;        // undo the next turn move cause we ending the game 
                    }
                }
            } else {
                break;
            }
        } catch (EOFException &e) {
            cout << "End of file reached." << endl;
            break;
        }
    }

    if (winner == -1) {         // if while loop ended cause EOF auto save game
        save(true);
    } else {                    // if while loop ended cause player won 
        cout << "Congratulations!! " << COLOR_ORDER.at(turn % 4) << " wins!!" << endl;
    }
} 

void Game::setBasement(Player &player, vector<int> &locations) {
    int vertex;
    while(true) {
        cout << "Builder " << player.getColor() << " where do you want to build a basement?" << endl;
        cin >> vertex;
        if (cin.eof()) {
            throw EOFException();
        }
        if (cin.fail()){
                cin.clear();
                cin.ignore(256,'\n');
                cout << "ERROR: Choose a valid integer." << endl; 
        } else {
            try {
                unique_ptr<Player> tempSelf = make_unique<Player>(player);
                tempSelf->buildResidence(vertex, true);
                board->buildResidence(player.getColor(), vertex, true);
                std::swap(players[player.getColor()], tempSelf);
                cout << "Builder " << player.getColor() << " successfully built a basement at " << vertex << "." << endl;
                locations.push_back(vertex);
                break;
            } catch (BuildingExistsException& e) {
                cout << "ERROR: A residence already exists here." << endl;
                cout << "Basements already exist as locations: ";
                for (const auto& val: locations) {
                    cout << val << " ";
                }
                cout << endl;
            } catch (InvalidLocationException& e) {
                cout << "ERROR: You cannot build here." << endl;
                cout << "Basements already exist as locations: ";
                for (const auto& val: locations) {
                    cout << val << " ";
                }
                cout << endl;
            }
        }
    }
}

//
void Game::initBasements() {
    cout << *board << endl;
    vector<int> locations;
    try {
        for (auto iter = players.begin(); iter != players.end(); ++iter) {
            setBasement(*iter->second, locations);
        }
        for (auto iter = players.rbegin(); iter != players.rend(); ++iter) {
            setBasement(*iter->second, locations);
        }
    } catch (EOFException &e) {
        cout << "End of file reached." << endl;
        throw e;
    }
}


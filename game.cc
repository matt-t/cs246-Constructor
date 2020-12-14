#include <iostream>
#include <fstream>
#include <algorithm>
#include "game.h"
#include "enums.h"
#include "constants.h"
#include "eofException.h"
#include "insufficientResourceException.h"

using namespace std;

Game::Game(unsigned int seed, vector<pair<Resource, int>> tileInfo): 
    seed{ seed }, board{make_unique<Board>(tileInfo)}, turn{0}, winner{false} 
{
    for (const Color &color: COLOR_ORDER) {
        // std::unique_ptr<Player> temp = std::make_unique<Player>(color);
        players[color] = std::make_unique<Player>(color);
    }
}
    

Game::Game(unsigned int seed, vector<pair<Resource, int>> tileInfo, int turn, int geese, vector<Color> roadInfo, vector<pair<Color, Residence>> buildInfo, 
            std::map<Color, int> playerPoints, map<Color, map<Resource, int>> playerResources, map<Color, map<int, Residence>> playerResidences, map<Color, vector<int>> playerRoads):
    seed{ seed }, board{make_unique<Board>(tileInfo, roadInfo, buildInfo, geese)}, turn{turn}, winner{false}
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
    if (board->getGeese() != UNINITIALIZED_GEESE) {
        save_file << board->getGeese() << endl;
    }
    
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
        cout << "~ multi-trade <colour> <give> <#given> <take> <#taken> : attempts to trade with builder <colour>, giving <#given> resources of type <give> and taking <#taken> resources of type <take>." << endl;
        cout << "~ market-trade <give> <take> : attempts to trade with the market, giving four resources of type <give> and receiving one resource of type <take>." << endl;
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

void Game::checkWinner() noexcept {
    for (const Color &color: COLOR_ORDER) {
        if (players[color]->getPoints() >= WINNING_POINTS) {
            winner = true;
            cout << "Congratulations!! " << players[color]->getColor() << " wins!!" << endl;
        }
    }
}

void Game::handleGoose(Player &player){
//players with 10 or more resource lose half resources
    for (const Color &color : COLOR_ORDER) {//check if more than 10 resource
        if (players[color]->totalResource() >= GEESE_STEAL_FROM){
            map<Resource, int> coutStolen;
            int half = players[color]->totalResource() / 2;
            cout << "Builder " << COLOR_TO_STRING.at(color) << " loses " << half << " resources to the goose. They lose:" << endl;
            for (int i = half; i > 0; --i){
                Resource stolen = players[color]->generateRandomResource();
                players[color]->takeResource(stolen, 1);
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
            cerr << "ERROR: Choose a valid integer." << endl; 
        } else {
            try {
                board->changeGeese(newGeeseTile);
                changed = true;
            } catch(GeeseExistsHereException& e) {
                cerr << "ERROR: The goose already exists here. Choose somewhere else." << endl;
            } catch(GeeseOutOfRange& e) {
                cerr << "ERROR: Tile selection is invalid." << endl;
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
                cerr << "ERROR: Choose a valid player." << endl;
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
    } else {
        cout << "There are no players to steal from. " << endl;
    }
}

void Game::handleRollMove(Player &player, string move, int &movePhase) {
    if (move == "roll") {
        //cout << "roll" << endl;
        int getRoll = player.rollDice();
        cout << getRoll << " is rolled."<< endl;
        if (getRoll == GEESE_ROLL) {
            handleGoose(player);
        } else {
            //produce resource from the tiles rolled
            map<Color, map<Resource, int>> playersGet = board->getRollResources(getRoll);
            for (const auto &color : playersGet) {
                cout << "Builder "<<color.first << " got:"<< endl;
                for (const auto resource : color.second) {
                    cout << resource.second << " "<<  resource.first  << endl;
                    players[color.first]->addResource(resource.first, resource.second);
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
        cerr << "Invalid command." << endl;
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
                cerr << "ERROR: Choose a valid location." << endl; 
                continue;
            } else {roadChosen = true;}
            try {
                unique_ptr<Player> tempSelf = make_unique<Player>(player);
                tempSelf->buildRoad(edge);
                board->buildRoad(player.getColor(), edge);
                std::swap(players[player.getColor()], tempSelf);
                cout << "Builder " << player.getColor() << " successfully built a road at " << edge << "." << endl;
            } catch (RoadExistsException& e){
                cerr << "ERROR: Someone has already built a road here." << endl;
            } catch(InvalidRoadLocationException& e) {
                cerr << "ERROR: Cannot build road here. None of your road or residence connect to here." << endl;
            } catch (InsufficientResourceException &e) {
                cerr << "ERROR: You do not have enough resources." << endl;
            } catch (InvalidLocationException& e){
                cerr << "ERROR: Enter a valid location." << endl;
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
                cerr << "ERROR: Choose a valid location." << endl; 
                continue;
            } else {vertexChosen = true;}
            try {
                unique_ptr<Player> tempSelf = make_unique<Player>(player);
                tempSelf->buildResidence(vertex);
                board->buildResidence(player.getColor(), vertex);
                std::swap(players[player.getColor()], tempSelf);
                cout << "Builder " << player.getColor() << " successfully built a " << player.getResidences().at(vertex) << " at " << vertex << "." << endl;
            } catch (BuildingExistsException& e){
                cerr << "ERROR: A residence already exists here." << endl;
            } catch (InvalidLocationException& e){
                cerr << "ERROR: You cannot build here." << endl;
            } catch (InsufficientResourceException & e) {
                cerr << "You do not have enough resources." << endl;
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
                cerr << "ERROR: Choose a valid location." << endl; 
                continue;
            } else {vertexChosen = true;}
            try {
                int vertex;
                unique_ptr<Player> tempSelf = make_unique<Player>(player);
                tempSelf->upgradeResidence(vertex);
                board->upgradeResidence(player.getColor(), vertex);
                std::swap(players[player.getColor()], tempSelf);
                cout << "Builder " << player.getColor() << " successfully built a " << player.getResidences().at(vertex) << " at " << vertex << "." << endl;
            } catch(BuidingNotOwnedException& e){
                cerr << "ERROR: You do not own this residence." << endl;
            } catch(InvalidLocationException& e){
                cerr << "ERROR: Enter a valid location." << endl;
            } catch (AlreadyTowerException& e){
                cerr << "ERROR: Your residence is already a Tower." << endl;
            } catch (InsufficientResourceException & e) {
                cerr << "You do not have enough resources." << endl;
            }
        }
    } else if (move == "trade") {
        try {
            string color, resourceGive, resourceTake, acceptance;
            while(cin >> color) {
                std::transform(color.begin(), color.end(), color.begin(), ::toupper);
                if (STRING_TO_COLOR.count(color) == 0 || STRING_TO_COLOR.at(color) == player.getColor()) {
                    cerr << "Enter a VALID player color to trade with." << endl;
                } else {
                    break;
                }
            }
            while(cin >> resourceGive) {
                std::transform(resourceGive.begin(), resourceGive.end(), resourceGive.begin(), ::toupper);
                if (STRING_TO_RESOURCE.count(resourceGive) == 0) {
                    cerr << "Enter a VALID resource you want to trade with." << endl;
                } else {
                    break;
                }
            } 
            while(cin >> resourceTake) {
                std::transform(resourceTake.begin(), resourceTake.end(), resourceTake.begin(), ::toupper);
                if (STRING_TO_RESOURCE.count(resourceTake) == 0) {
                    cerr << "Enter a VALID resource you want to trade for." << endl;
                } else {
                    break;
                }
            }
            if (cin.eof()) {
                throw EOFException();
            }            
            unique_ptr<Player> tempOther = make_unique<Player>(*players[STRING_TO_COLOR.at(color)]);
            unique_ptr<Player> tempSelf = make_unique<Player>(player);
            tempOther->takeResource(STRING_TO_RESOURCE.at(resourceTake), 1);
            tempOther->addResource(STRING_TO_RESOURCE.at(resourceGive), 1);
            tempSelf->takeResource(STRING_TO_RESOURCE.at(resourceGive), 1);
            tempSelf->addResource(STRING_TO_RESOURCE.at(resourceTake), 1);

            while(true) {
                cout << "Does " << color << " accept the trade?" << endl;
                cin >> acceptance;
                if (cin.eof()) {
                    throw EOFException();
                }       
                std::transform(acceptance.begin(), acceptance.end(), acceptance.begin(), ::toupper);
                if (acceptance == "YES") {
                    std::swap(players[STRING_TO_COLOR.at(color)], tempOther);
                    std::swap(players[player.getColor()], tempSelf);
                    cout << "Builder " << player.getColor() << " successfully traded " << resourceGive << " for " << resourceTake << "." << endl;
                    break;
                } else if (acceptance == "NO") {
                    cout << color << " has declined the trade." << endl;
                    break;
                } else {
                    cerr << "Enter a VALID response." << endl;
                }
            }
            
        } catch (InsufficientResourceException & e) {
            if (e.getColor() == player.getColor()) {
                cerr << "You don't have enough " << e.getResource() << "." << endl;
            } else {
                cerr << e.getColor() << " doesn't have enough " << e.getResource() << "." << endl;
            }
        }
    } else if (move == "market-trade") {
        try {
            string resourceGive, resourceTake;
            while(cin >> resourceGive) {
                std::transform(resourceGive.begin(), resourceGive.end(), resourceGive.begin(), ::toupper);
                if (STRING_TO_RESOURCE.count(resourceGive) == 0) {
                    cerr << "Enter a VALID resource you want to trade with." << endl;
                } else {
                    break;
                }
            } 
            while(cin >> resourceTake) {
                std::transform(resourceTake.begin(), resourceTake.end(), resourceTake.begin(), ::toupper);
                if (STRING_TO_RESOURCE.count(resourceTake) == 0) {
                    cerr << "Enter a VALID resource you want to trade for." << endl;
                } else {
                    break;
                }
            }
            if (cin.eof()) {
                throw EOFException();
            }            
            unique_ptr<Player> tempSelf = make_unique<Player>(player);
            tempSelf->takeResource(STRING_TO_RESOURCE.at(resourceGive), MARKET_TRADE_RATE);
            tempSelf->addResource(STRING_TO_RESOURCE.at(resourceTake), 1);
            std::swap(players[player.getColor()], tempSelf);
            cout << "Builder " << player.getColor() << " successfully traded " << MARKET_TRADE_RATE << " " << resourceGive << " for " << "1" << " " << resourceTake << "." << endl;

            
        } catch (InsufficientResourceException & e) {
            cerr << "You do not have enough resources." << endl;
        }
    } else if (move == "multi-trade") {
        try {
            string color, resourceGive, resourceTake, acceptance;
            int resourceGiveNum, resourceTakeNum;
            while(cin >> color) {
                std::transform(color.begin(), color.end(), color.begin(), ::toupper);
                if (STRING_TO_COLOR.count(color) == 0 || STRING_TO_COLOR.at(color) == player.getColor()) {
                    cerr << "Enter a VALID player color to trade with." << endl;
                } else {
                    break;
                }
            }
            while(cin >> resourceGive) {
                std::transform(resourceGive.begin(), resourceGive.end(), resourceGive.begin(), ::toupper);
                if (STRING_TO_RESOURCE.count(resourceGive) == 0) {
                    cerr << "Enter a VALID resource you want to trade with." << endl;
                } else {
                    break;
                }
            } 
            while(cin >> resourceGiveNum) {
                if (resourceGiveNum < 1) {
                    cerr << "Enter a VALID number of resources you want to trade with." << endl;
                } else {
                    break;
                }
            }
            while(cin >> resourceTake) {
                std::transform(resourceTake.begin(), resourceTake.end(), resourceTake.begin(), ::toupper);
                if (STRING_TO_RESOURCE.count(resourceTake) == 0) {
                    cerr << "Enter a VALID resource you want to trade for." << endl;
                } else {
                    break;
                }
            }
            while(cin >> resourceTakeNum) {
                if (resourceGiveNum < 1) {
                    cerr << "Enter a VALID number of resources you want to trade for." << endl;
                } else {
                    break;
                }
            }
            if (cin.eof()) {
                throw EOFException();
            }            
            unique_ptr<Player> tempOther = make_unique<Player>(*players[STRING_TO_COLOR.at(color)]);
            unique_ptr<Player> tempSelf = make_unique<Player>(player);
            tempOther->takeResource(STRING_TO_RESOURCE.at(resourceTake), resourceTakeNum);
            tempOther->addResource(STRING_TO_RESOURCE.at(resourceGive), resourceGiveNum);
            tempSelf->takeResource(STRING_TO_RESOURCE.at(resourceGive), resourceGiveNum);
            tempSelf->addResource(STRING_TO_RESOURCE.at(resourceTake), resourceTakeNum);

            while(true) {
                cout << "Does " << color << " accept the trade?" << endl;
                cin >> acceptance;
                if (cin.eof()) {
                    throw EOFException();
                }       
                std::transform(acceptance.begin(), acceptance.end(), acceptance.begin(), ::toupper);
                if (acceptance == "YES") {
                    std::swap(players[STRING_TO_COLOR.at(color)], tempOther);
                    std::swap(players[player.getColor()], tempSelf);
                    cout << "Builder " << player.getColor() << " successfully traded " << resourceGiveNum << " " << resourceGive << " for " << resourceTakeNum << " " << resourceTake << "." << endl;
                    break;
                } else if (acceptance == "NO") {
                    cout << color << " has declined the trade." << endl;
                    break;
                } else {
                    cerr << "Enter a VALID response." << endl;
                }
            }
        } catch (InsufficientResourceException & e) {
            if (e.getColor() == player.getColor()) {
                cerr << "You don't have enough " << e.getResource() << "." << endl;
            } else {
                cerr << e.getColor() << " doesn't have enough " << e.getResource() << "." << endl;
            }
        }
    } else if (move == "next") {
        checkWinner();
        next();
        --movePhase;
        //cout << turn % NUM_PLAYERS << " " << winner << endl;
    } else if (move == "save") {
        save();
    } else if (move == "help") {
        help(movePhase);
    } else {
        cerr << "Invalid command." << endl;
    }
}

void Game::playGame() {
    cout << *board << endl;
    checkWinner();
    if (winner) {
        return;
    }
    int movePhase = 0;
    string move;
    cout << "Builder " << COLOR_ORDER.at(turn % NUM_PLAYERS) << "'s turn." << endl;
    while (!winner) {
        if (movePhase) {
            cout << "Enter a command:" << endl;
        }
        cout << "> ";
        try {
            if (cin >> move) {
                if (movePhase == 0) {
                    handleRollMove(*players[COLOR_ORDER.at(turn % NUM_PLAYERS)], move, movePhase);
                } else {
                    int temp = turn % NUM_PLAYERS;
                    handleActionMove(*players[COLOR_ORDER.at(turn % NUM_PLAYERS)], move, movePhase);
                    if (temp != turn % NUM_PLAYERS && !winner) {
                        cout << "Builder " << COLOR_ORDER.at(turn % NUM_PLAYERS) << "'s turn." << endl;
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

    if (!winner) {         // if while loop ended cause EOF auto save game
        save(true);
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
                cerr << "ERROR: Choose a valid integer." << endl; 
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
                cerr << "ERROR: A residence already exists here." << endl;
                cerr << "Basements already exist as locations: ";
                for (const auto& val: locations) {
                    cerr << val << " ";
                }
                cerr << endl;
            } catch (InvalidLocationException& e) {
                cerr << "ERROR: You cannot build here." << endl;
                cerr << "Basements already exist as locations: ";
                for (const auto& val: locations) {
                    cerr << val << " ";
                }
                cerr << endl;
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


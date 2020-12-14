#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>
#include "enums.h"
#include "game.h"
#include "constants.h"
#include <random>
#include <chrono>
#include <stdlib.h>
#include "eofException.h"
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]){
	//processing of command line arguments
	unsigned int seed = 0;
	string game_file;
	string board_file;
	bool game_loaded = false;
	bool board_loaded = false;
	bool board_randomized = false;
	bool seed_set = false;
	vector<pair<Resource, int>> boardInfo;
	vector<Color> roadInfo(NUM_ROADS, Color::None);
	vector<pair<Color, Residence>> buildInfo(NUM_VERTICES, {Color::None, Residence::None});
	int geese = UNINITIALIZED_GEESE;
	int turn = 0;
	map<Color, int> playerPoints;
	map<Color, map<Resource, int>> playerResources;
	map<Color, map<int, Residence>> playerResidences;
	map<Color, vector<int>> playerRoads;

	for (int i = 1 ; i < argc; ++i) {
		string s = argv[i];
		stringstream ss;
		if (s == "-seed"){
			if (seed_set == true) { //seed has already been set
				cerr << "ERROR: already specified -seed once before" << endl;
				return 1;
			}
			++i;
			if (i >= argc) {
				cerr << "ERROR: -seed missing seed argument" << endl;
				return 1;
			} else {
				ss << argv[i];
				if (! (ss >> seed)) {
					cerr << "ERROR: the seed must be an integer" << endl;
					return 1;
				}
				seed_set = true;
				//cout << "The seed is set to: " << seed << endl;
			}
		} else if (s == "-load") {
			if (board_randomized == true){
				board_randomized = false;
			} else if (board_loaded == true){
				cerr << "ERROR: already specified -board, can't also specify -load" << endl;
				return 1;
			} else if (game_loaded == true) {
				cerr << "ERROR: already specified -load once before" << endl;
				return 1;
			}
			++i;
			if (i >= argc) {
				cerr << "ERROR: -load missing filename argument" << endl;
				return 1;
			} else {
				ss << argv[i];
				ss >> game_file;
				ifstream f(game_file);
				if (f.fail()) {
					cerr << "ERROR: Invalid file " << game_file << endl;
					return 1;
				}
				if (f.is_open()) {
					string line;
					getline(f, line);
					istringstream read(line);
					read >> turn;
					if (read.fail()){
							cerr << "ERROR: Unsupported game format in file " << game_file <<". There must be a turn integer."<< endl;
							return 1;
						}
					for (const Color &color: COLOR_ORDER) {
						playerPoints[color] = 0;
						getline(f, line);
						read.clear();
						read.str(line);
						int num;
						read >> num;
						playerResources[color].emplace(Resource::Brick, num);
						read >> num;
						playerResources[color].emplace(Resource::Energy, num);
						read >> num;
						playerResources[color].emplace(Resource::Glass, num);
						read >> num;
						playerResources[color].emplace(Resource::Heat, num);
						read >> num;
						playerResources[color].emplace(Resource::Wifi, num);
						if (read.fail() || read.eof()){
							cerr << "ERROR: Unsupported game format in file " << game_file <<". There must be sufficient player information."<< endl;
							return 1;
						}
						string type;
						read >> type;
						while(read >> num) {
							playerRoads[color].emplace_back(num);
							roadInfo[num] = color;
						}
						read.clear();
						read >> type;
						char residenceType;
						while (read >> num){ // im not sure if we need to reset failbit here
							read >> residenceType;
							if (read.fail() || read.eof() || !(residenceType == 'B'||residenceType == 'T'||residenceType == 'H')){
								cerr << "ERROR: Unsupported game format in file " << board_file <<". Building information must be a pair of integer and B/H/T."<< endl;
								return 1;
							}
							playerResidences[color].emplace(num, CHAR_TO_RESIDENCE.at(residenceType));
							playerPoints[color] += RESIDENCE_TO_POINTS.at(CHAR_TO_RESIDENCE.at(residenceType));
							buildInfo[num] = make_pair(color, CHAR_TO_RESIDENCE.at(residenceType));
						}
						read.clear();
					}
					getline(f, line);
					read.clear();
					read.str(line); // sus about how this syntax works
					int resourceNum, rollNum;
					Resource resource = Resource::Park; // need some better way to deal with resource if unitialized 
					while(read >> resourceNum){
						if (INT_TO_RESOURCE.count(resourceNum) == 0){
							cerr << "ERROR: Unsupported board format in file " << board_file << ". Tile resource must be between 0 and 5."<< endl;
							return 1;
						}
						resource = INT_TO_RESOURCE.at(resourceNum);
						read >> rollNum;
						if (rollNum < (MIN_DICE_ROLL*2) || rollNum > (MAX_DICE_ROLL*2)) {
							cerr << "ERROR: Unsupported board format in file " << board_file << ". Tile number must be between 0 and 18."<< endl;
							return 1;
						}
						if (resource == Resource::Park && rollNum != GEESE_ROLL) {
							cerr << "ERROR: Unsupported board format in file " << game_file << ". Parks must be followed by the geese number of " << GEESE_ROLL << "." << endl;
							return 1;
						}
						if (resource != Resource::Park && rollNum == GEESE_ROLL) {
							cerr << "ERROR: Unsupported board format in file " << game_file << ". The resource of a tile with roll number " << GEESE_ROLL << " must be park." << endl;
							return 1;
						}
						if (read.fail()){
							cerr << "ERROR: Unsupported board format in file " << board_file <<". File must contain 19 pairs of integers."<< endl;
							return 1;
						}
						boardInfo.push_back(std::make_pair(resource, rollNum));
					}
					if (boardInfo.size() != 19) {
						cerr << "ERROR: Unsupported board format in file " << game_file << ". There must be a total of 19 tiles."<< endl;
						return 1;
					}
					getline(f, line);
					read.clear();
					read.ignore(256,'\n');
					read.str(line);
					read >> geese; // geese can also be omitted and the program still runs
				} else {
					cerr << "ERROR: Cannot open file " << game_file << endl;
					return 1;
				}
				game_loaded = true;
				//cout << "The game from " << game_file << "is loaded." << endl;
			}		
		} else if (s == "-board") {
			if (board_randomized == true){
				board_randomized = false;
			} else if (game_loaded == true){
					cerr << "ERROR: already specified -load, can't also specify -board" << endl;
					return 1;
			} else if (board_loaded == true) {
					cerr << "ERROR: already specified -board once before" << endl;
					return 1;
			}
			++i;
			if (i >= argc) {
				cerr << "ERROR: -board missing filename argument" << endl;
					return 1;
			} else {
				ss << argv[i];
				ss >> board_file;
				ifstream f(board_file);
				if (f.fail()) {
						cerr << "ERROR: Invalid file" << board_file << endl;
						return 1;
				}
				if (f.is_open()){
					string line;
					getline(f, line);
					istringstream read(line);
					int resourceNum, rollNum;
					Resource resource = Resource::Park; // need some better way to deal with resource if unitialized 
					while(read >> resourceNum){
						if (INT_TO_RESOURCE.count(resourceNum) == 0){
							cerr << "ERROR: Unsupported board format in file " << board_file <<". Tile resource must be between 0 and 5."<< endl;
							return 1;
						}
						resource = INT_TO_RESOURCE.at(resourceNum);
						read >> rollNum;
						if (rollNum < (MIN_DICE_ROLL*2) || rollNum > (MAX_DICE_ROLL*2)) {
							cerr << "ERROR: Unsupported board format in file " << board_file <<". Tile number must be between 0 and 18."<< endl;
							return 1;
						}
						if (resource == Resource::Park && rollNum != GEESE_ROLL) {
							cerr << "ERROR: Unsupported board format in file " << game_file << ". Parks must be followed by the geese number of " << GEESE_ROLL << "." << endl;
							return 1;
						}
						if (resource != Resource::Park && rollNum == GEESE_ROLL) {
							cerr << "ERROR: Unsupported board format in file " << game_file << ". The resource of a tile with roll number " << GEESE_ROLL << " must be park." << endl;
							return 1;
						}
						if (read.fail()){
							cerr << "ERROR: Unsupported board format in file " << board_file <<". File must contain 19 pairs of integers."<< endl;
							return 1;
						}
						boardInfo.push_back(std::make_pair(resource, rollNum));
					}
					if (boardInfo.size() != 19) {
						cerr << "ERROR: Unsupported board format in file " << board_file <<". There must be a total of 19 tiles."<< endl;
						return 1;
					}
				} else {
					cerr << "ERROR: Cannot open file " << board_file << endl;
					return 1;
				}
				board_loaded = true;
				//cout << "The board from " << board_file << " is loaded." << endl;
			}
		} else if (s == "-random-board"){
			if (board_loaded != true && game_loaded != true){
				board_randomized = true;
			} //else if (board_randomized == true) {
			// 	cerr << "ERROR: already specified -random-board once before" << endl;
			// 	return 1;
			// }
			//cout << "The board is randomized" << endl;
		} else {
			cerr << "ERROR: unrecognized argument" << endl;
			return 1;
		}
	}
	//if there is no seed passed in, we generate one randomly
	if (!seed_set){	
		seed = std::chrono::system_clock::now().time_since_epoch().count();
	}
	srand(seed);
	//process initializing game
	if (game_loaded) {
		cout << "Loading game from previously saved game file..." << endl;
		//feed in boardInfo AND gameInfo;
		Game game{seed, boardInfo, turn, geese, roadInfo, buildInfo, playerPoints, playerResources, playerResidences, playerRoads};
		game.playGame();
	} else if (board_loaded) {
		cout << "Loading game from specific board layout..." << endl;
		//feed in boardInfo
		Game game{seed, boardInfo};
		game.initBasements();
		game.playGame();
	} else if (board_randomized){
		//generate a random board by building boardInfo
		int rollNum;
		Resource resource;
		vector<int> rollNumList = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
		vector<int> resourceList = {0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5};
		for(int i = 18; i > 0; --i){
			int randomNum = rand() % (i);
			int randomResource = rand() % (i + 1);
			resource = INT_TO_RESOURCE.at(resourceList[randomResource]);
			resourceList.erase(resourceList.begin() + randomResource);
			if (resource == Resource::Park) {
				rollNum = 7;
			} else {
				rollNum = rollNumList[randomNum];
				rollNumList.erase(rollNumList.begin() + randomNum);
			} 
			boardInfo.push_back(std::make_pair(resource, rollNum));
		} 
		resource = INT_TO_RESOURCE.at(resourceList[0]);
		if (resource == Resource::Park) {
			rollNum = 7;
		} else {
			rollNum = rollNumList[0];
		} boardInfo.push_back(std::make_pair(resource, rollNum));
		Game game{seed, boardInfo};
		cout << "Loading game from randomized board..." << endl;
		game.initBasements();
		game.playGame();
	} else {
		//looks for layout.txt
		ifstream f("layout.txt");
		if (f.fail()) {
				cerr << "ERROR: Unable to open file layout.txt for default board layout." << endl;
				return 1;
		} if (f.is_open()){
			string line;
			getline(f, line);
			istringstream read(line);
			int resourceNum, rollNum;
			Resource resource = Resource::Park; // need some better way to deal with resource if unitialized 
			while(read >> resourceNum){
				resource = INT_TO_RESOURCE.at(resourceNum);
				read >> rollNum;
				boardInfo.push_back(std::make_pair(resource, rollNum));
			}
		} else {
			cerr << "ERROR: Cannot open file " << game_file << endl;
			return 1;
		} Game game{seed, boardInfo};
		cout << "Loading game from default board layout..." << endl;
		try {
			game.initBasements();
		} catch (EOFException &e) {
			return 1;
		}
		game.playGame();
	}

	string playAgain;
	cout << "Would you like to play again? (yes/y to play again)" << endl;				// Currently only loads layout.txt
	cin >> playAgain;
	transform(playAgain.begin(), playAgain.end(), playAgain.begin(), ::toupper);
	while (playAgain == "YES" || playAgain == "Y") {
		ifstream f("layout.txt");
		if (f.fail()) {
				cerr << "ERROR: Unable to open file layout.txt for default board layout." << endl;
				return 1;
		} if (f.is_open()){
			string line;
			getline(f, line);
			istringstream read(line);
			int resourceNum, rollNum;
			Resource resource = Resource::Park; // need some better way to deal with resource if unitialized 
			while(read >> resourceNum){
				switch(resourceNum){
					case 0: resource = Resource::Brick; break;
					case 1: resource = Resource::Energy; break;
					case 2: resource = Resource::Glass; break;
					case 3: resource = Resource::Heat; break;
					case 4: resource = Resource::Wifi; break;
					case 5: resource = Resource::Park; break;
				}
				read >> rollNum;
				boardInfo.push_back(std::make_pair(resource, rollNum));
			}
		} else {
			cerr << "ERROR: Cannot open file " << game_file << endl;
			return 1;
		} Game game{seed, boardInfo};
		try {
			game.initBasements();
		} catch (EOFException &e) {
			return 1;
		}
		game.playGame();

		cout << "Would you like to play again?" << endl;
		cin >> playAgain;
		transform(playAgain.begin(), playAgain.end(), playAgain.begin(), ::toupper);
	}
			
	return 0;
}

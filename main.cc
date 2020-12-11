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
	int r = 72;
	int v = 54;
	vector<Color> roadInfo(r, Color::None);
	vector<pair<Color, Residence>> buildInfo(v, {Color::None, Residence::None});
	int geese, turn;
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
				cout << "The seed is set to: " << seed << endl;
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
					for (const Color color: COLOR_ORDER) {
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
					getline(f, line);
					read.clear();
					read.str(line);
					read >> geese;
				} else {
					cerr << "ERROR: Cannot open file " << game_file << endl;
					return 1;
				}
				game_loaded = true;
				cout << "The game from " << game_file << "is loaded." << endl;
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
						cerr << "ERROR: Invalid file " << game_file << endl;
						return 1;
				}
				if (f.is_open()){
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
				}
				board_loaded = true;
				cout << "The board from " << board_file << "is loaded." << endl;
			}
		} else if (s == "-random-board"){
			if (board_loaded != true && game_loaded != true){
				board_randomized = true;
			} //else if (board_randomized == true) {
			// 	cerr << "ERROR: already specified -random-board once before" << endl;
			// 	return 1;
			// }
			cout << "The board is randomized" << endl;
		} else {
			cerr << "ERROR: unrecognized argument" << endl;
			return 1;
		}
	}
	//if there is no seed passed in, we generate one randomly
	if (!seed_set){	
		seed = std::chrono::system_clock::now().time_since_epoch().count();
	}
	cout << "the seed is: "<<seed << endl;
	srand(seed);
	//process initializing game
	if (game_loaded) {
		cout << "The game constructor is run with loaded game." << endl;
		//feed in boardInfo AND gameInfo;
		Game game{seed, boardInfo, turn, geese, roadInfo, buildInfo, playerPoints, playerResources, playerResidences, playerRoads};
		game.playGame();
	} else if (board_loaded) {
		cout << "The game constructor is run with loaded board." << endl;
		//feed in boardInfo
		Game game{seed, boardInfo};
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
		
		cout << "random board!! generated with seed" << endl;
		Game game{seed, boardInfo};
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
		game.playGame();
	}
	

	// Handles gameplay:
	// game.playGame();
	

	// Check to play again
	/*
	string playAgain;
	cout << "Would you like to play again?" << endl;
	cin >> playAgain;
	if (playAgain == "yes") {
			// can we wrap everything in whie loop? 
			// only thing is that the aboec stuff checks cmd line args 
			// but we probs wanna restart the game from scrath and there is no input
			// so we start the game from random board?!?
	}
	*/

	return 0;
}

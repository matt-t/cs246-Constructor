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

using namespace std;

int main(int argc, char* argv[]){
	
	//processing of command line arguments
	int seed = 0;
	string game_file;
	string board_file;
	bool game_loaded = false;
	bool board_loaded = false;
	bool board_randomized = false;
	bool seed_set = false;
	std::vector<std::pair<Resource, int>> boardInfo;

	for (int i = 1 ; i < argc; ++i) {
		string s = argv[i];
		stringstream ss;
		if (s == "-seed"){
			if (seed_set == true) {
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
				cerr << "ERROR: already specified -random, can't also specify -load" << endl;
				return 1;
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
				game_loaded = true;
				cout << "The game from " << game_file << "is loaded." << endl;
				//do something with f;
			}		
		} else if (s == "-board") {
			if (board_randomized == true){
                                cerr << "ERROR: already specified -random, can't also specify -board" << endl;
                                return 1;
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
					Resource resource;
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
			if (board_loaded == true){
                                cerr << "ERROR: already specified -board, can't also specify -random-board" << endl;
                                return 1;
                        } else if (game_loaded == true){
                                cerr << "ERROR: already specified -load, can't also specify -random-board" << endl;
                                return 1;
                        } else if (board_randomized == true) {
                                cerr << "ERROR: already specified -random-board once before" << endl;
                                return 1;
                        }
			board_randomized = true;
			cout << "The board is randomized" << endl;
			// do something to the board
		} else {
			cerr << "ERROR: unrecognized argument" << endl;
			return 1;
		}
	}

	if (board_loaded == false && game_loaded == false){
		//generate boardInfo vector for the randomized board
	} 
	if (game_loaded == true){
		//feed in boardInfo AND gameInfo;
	} else {
		cout << "The game constructor is run." << endl;
		Game game{boardInfo};
	}
	

	// Loop for actual gameplay:
	// game.playGame( bool needtoSEtbasement);

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

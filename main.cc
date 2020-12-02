#include <iostream>
#include <string>
#include<memory>
#include<fstream>
#include<sstream>
#include  <stdexcept>
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
	//init Game here;
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
				board_loaded = true;
				cout << "The board from " << board_file << "is loaded." << endl;
				//do something with f
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
	return 0;
}

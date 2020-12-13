#include "loadedDice.h"
#include <iostream>
#include "eofException.h"

int LoadedDice::rollDice() const {
    int loadedNum;
    while (true) {
        std::cout << "Input a roll between 2 and 12:" << std::endl;
        if (std::cin >> loadedNum) {
            if (loadedNum > 12 || loadedNum < 2) {
                std::cerr << "ERROR: Invalid roll " << loadedNum << std::endl;
            } else {
                return loadedNum;
            } 
        } else {
            if (std::cin.eof()) {
                throw EOFException();
            }
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cerr << "ERROR:  isn't a valid integer." << std::endl;
        }
    }
}

std::unique_ptr<Dice> LoadedDice::clone() const {
    return std::make_unique<LoadedDice>(*this);
}


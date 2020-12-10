#include "loadedDice.h"

int LoadedDice::rollDice() const {
    if (loadedNumber == 0) {
        throw UnloadedException();
    }
    return loadedNumber;
}
void LoadedDice::loadDice(int num) {
    if (num > 12 || num < 2) {
        throw InvalidRollException();
    }
    loadedNumber = num;
}

std::unique_ptr<Dice> LoadedDice::clone() const {
    return std::make_unique<LoadedDice>(*this);
}


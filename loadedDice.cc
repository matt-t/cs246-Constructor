#include "loadedDice.h"

int LoadedDice::rollDice(int seed) const {
    if (loadedNumber == 0) {
        throw UnloadedDiceException();
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


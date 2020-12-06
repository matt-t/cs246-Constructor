#include "loadedDice.h"

int LoadedDice::rollDice(diceType dice) const override {
    if (loadedNumber == 0) {
        throw UnloadedException();
    }
    return loadedNumber;
}
void LoadedDice::loadDice(int num) {
    loadedNumber = num;
}

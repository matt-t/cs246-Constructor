#include "loadedDice.h"

int LoadedDice::rollDice() const {
    if (loadedNumber == 0) {
        throw UnloadedException();
    }
    return loadedNumber;
}
void LoadedDice::loadDice(int num) {
    loadedNumber = num;
}

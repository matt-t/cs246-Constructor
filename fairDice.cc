#include "fairDice.h"
#include <algorithm>

int LoadedDice::rollDice(diceType dice) const override {
    vector<int> diceNums = [1, 2, 3, 4, 5, 6];
    return std::shuffle(diceNums)[0];
}

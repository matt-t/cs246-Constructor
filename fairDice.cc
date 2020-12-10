#include "fairDice.h"
#include <algorithm>
#include <vector>
#include <random>

int FairDice::rollDice() const {
    std::vector<int> diceNums{1, 2, 3, 4, 5, 6};
    std::default_random_engine rng{1};
    std::shuffle(diceNums.begin(), diceNums.end(), rng);
    int firstRoll = diceNums[0];
    std::shuffle(diceNums.begin(), diceNums.end(), rng);
    int secondRoll = diceNums[0];
    return firstRoll + secondRoll;
}

std::unique_ptr<Dice> FairDice::clone() const {
    return std::make_unique<FairDice>(*this);
}
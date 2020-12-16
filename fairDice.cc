#include "fairDice.h"
#include <algorithm>
#include <vector>
#include <random>
#include <stdlib.h>

int FairDice::rollDice() const {
    //std::vector<int> diceNums{1, 2, 3, 4, 5, 6};
    //std::default_random_engine rng{seed};
    //std::shuffle(diceNums.begin(), diceNums.end(), rng);
    int firstRoll = rand() % 6 + 1;
    //std::shuffle(diceNums.begin(), diceNums.end(), rng);
    int secondRoll = rand() % 6 + 1;
    return firstRoll + secondRoll;
}

std::unique_ptr<Dice> FairDice::clone() const {
    return std::make_unique<FairDice>(*this);
}

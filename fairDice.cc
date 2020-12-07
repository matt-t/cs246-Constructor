#include "fairDice.h"
#include <algorithm>
#include <vector>
#include <random>

int FairDice::rollDice() const {
    std::vector<int> diceNums{1, 2, 3, 4, 5, 6};
    std::default_random_engine rng{1};
    std::shuffle(diceNums.begin(), diceNums.end(), rng);
    return diceNums[0];
}

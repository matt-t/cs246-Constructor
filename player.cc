#include "player.h"
#include "loadedDice.h"
#include "fairDice.h"

void Player::changeDice(DiceType newDice) {
    if (newDice == DiceType::Fair) {
        playerDice = std::make_unique<FairDice>();
    } else if (newDice == DiceType::Loaded) {
        playerDice = std::make_unique<LoadedDice>();
    }
}

std::map<Residence, std::vector<int>> Player::getResidences() const {
    return residences;
}

void Player::addResources(Resource resource, int amount) {
    resources[resource] += amount;
}

void Player::takeResources(Resource resource, int amount) {
    if (resources[resource] < amount) {
        throw InsufficientResourceException();
    }
    resources[resource] -= amount;
}

void Player::generateRandomResource() {
    
}
int Player::totalResource() const {
    int sum = 0;
    for (auto resource : resources) {
        sum += resource.second;
    }
    return sum;
}

void Player::buildResidence(Color color, int location) {
    residences[Residence::Basement].emplace_back(location);
}

void Player::buildRoad(int location) {
    roads.emplace_back(location);
}

int Player::rollDice() {
    return playerDice->rollDice();
}

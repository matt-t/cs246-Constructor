#include "player.h"
#include "loadedDice.h"
#include "fairDice.h"
#include <memory>

Player::Player() {
    playerDice = std::make_unique<LoadedDice>();
};


void Player::changeDice(DiceType newDice) {
    if (newDice == DiceType::Fair) {
        playerDice = std::make_unique<FairDice>();
    } else if (newDice == DiceType::Loaded) {
        playerDice = std::make_unique<LoadedDice>();
    }
}

std::map<int, Residence> Player::getResidences() const {
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

void Player::buildResidence(int location) {
    if (residences.count(location) != 0) {
        throw PlayerResidenceTypeException();
    }
    residences[location] = Residence::Basement;
}

void Player::upgradeResidence(int location) {
    if (residences.count(location) == 0) {
        throw PlayerResidenceTypeException();
    }
    if (residences[location] == Residence::Tower) {
        throw PlayerResidenceTypeException();
    }
    residences[location] = residences[location] == Residence::Basement ? Residence::House : Residence::Tower;
}


void Player::buildRoad(int location) {
    roads.emplace_back(location);
}

int Player::rollDice() {
    return playerDice->rollDice();
}

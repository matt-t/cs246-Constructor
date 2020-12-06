#include "player.h"

void player::changeDice(DiceType newDice) {
    dice = newDice == DiceType::Fair ? fairDice{} : loadedDice{};
}

std::map<Residence, std::vector<int>> player::getResidences() const {
    return residences;
}

void player::addResources(resource: Resource, amount: int) {
    resources[resource] += amount;
}

void player::takeResources(resource: Resource, amount: int) {
    if (resources[resource] < amount) {
        throw InsufficientResourceException();
    }
    resources[resource] -= amount;
}

void player::generateRandomResource() {
    // ???
}
int player::totalResource() const {
    int sum = 0;
    for (auto resource : resources) {
        sum += resource.second;
    }
    return;
}

void player::buildResidence(color: Color, location: int) {
    residences[Residence::basement].emplace_back(location);
}

void player::buildRoad(location: int) {
    roads.emplace_back(location);
}

int player::rollDice() {
    return diceType.rollDice();
}

int player::handleGooseRoll() {
    // ??
}
void player::transfer(to: Color, resource: Resource) {
    // ??
}

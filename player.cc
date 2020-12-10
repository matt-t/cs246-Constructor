#include "player.h"
#include "loadedDice.h"
#include "fairDice.h"
#include <memory>
#include "constants.h"

using namespace std;

//Ctor
Player::Player(Color player):
    color{ player }, points{ 0 }
{
    playerDice = std::make_unique<LoadedDice>();
};

Player::Player(Color player, int points, map<Resource, int> resources, map<int, Residence> residences, vector<int> roads):
    color{ player }, points{ points }, resources{ resources }, residences{ residences}, roads{ roads }
{
    playerDice = std::make_unique<LoadedDice>();
}

// Copy constructor
Player::Player(const Player &other): 
color{other.color},
resources{other.resources},
residences{other.residences},
roads{other.roads},
playerDice{other.playerDice->clone()}
{}

// Move constructor
Player::Player(Player &&other) : 
color{other.color},
resources{other.resources},
residences{other.residences},
roads{other.roads},
playerDice{nullptr}
{
    std::swap(playerDice, other.playerDice);
}

// Copy assignment operator
Player & Player::operator=(const Player &other) {
    Player temp = other;
    std::swap(color, temp.color);
    std::swap(resources, temp.resources);
    std::swap(residences, temp.residences);
    std::swap(roads, temp.roads);
    std::swap(playerDice, temp.playerDice);
    return *this;
}
// Move assignment operator
Player & Player::operator=(Player &&other) {
    std::swap(color, other.color);
    std::swap(resources, other.resources);
    std::swap(residences, other.residences);
    std::swap(roads, other.roads);
    std::swap(playerDice, other.playerDice);
    return *this;
}

// Destructor
Player::~Player() {}

        
//methods
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
    // for (Resource resourceCost: BASEMENT_COST) {
    //     if (player.resources[resourceCost.first] < resourceCost.second) {}
    // }
    residences[location] = Residence::Basement;
    points += RESIDENCE_TO_POINTS.at(Residence::Basement);
}

void Player::upgradeResidence(int location) {
    if (residences.count(location) == 0) {
        throw PlayerResidenceTypeException();
    }
    if (residences[location] == Residence::Tower) {
        throw PlayerResidenceTypeException();
    }
    residences[location] == Residence::Basement 
        ? (residences[location] = Residence::House, points = points + RESIDENCE_TO_POINTS.at(Residence::House) - RESIDENCE_TO_POINTS.at(Residence::Basement))
        : (residences[location] = Residence::Tower, points = points + RESIDENCE_TO_POINTS.at(Residence::Tower) - RESIDENCE_TO_POINTS.at(Residence::House));
}


void Player::buildRoad(int location) {
    roads.emplace_back(location);
}

int Player::rollDice(int seed) const {
    return playerDice->rollDice(seed);
}

int Player::getPoints() const {
    return points;
}

std::map<Resource, int> Player::getResources() const {
    return resources;
}

Color Player::getColor() {
    return color;
}




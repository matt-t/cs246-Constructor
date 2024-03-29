#include "player.h"
#include "loadedDice.h"
#include "fairDice.h"
#include "constants.h"
#include <cstdlib>
#include "insufficientResourceException.h"
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
points{other.points},
resources{other.resources},
residences{other.residences},
roads{other.roads},
playerDice{other.playerDice->clone()}
{}

// Move constructor
Player::Player(Player &&other) : 
color{other.color},
points{other.points},
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
    std::swap(points, temp.points);
    std::swap(resources, temp.resources);
    std::swap(residences, temp.residences);
    std::swap(roads, temp.roads);
    std::swap(playerDice, temp.playerDice);
    return *this;
}
// Move assignment operator
Player & Player::operator=(Player &&other) {
    std::swap(color, other.color);
    std::swap(points, other.points);
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

void Player::addResource(Resource resource, int amount) {
    if (amount < 0) {
        throw InvalidResourceAmount{};
    }
    resources[resource] += amount;
}

void Player::takeResource(Resource resource, int amount) {
    if (amount < 0) {
        throw InvalidResourceAmount{};
    }
    if (resources[resource] < amount) {
        throw InsufficientResourceException{color, resource};
    }
    resources[resource] -= amount;
}

Resource Player::generateRandomResource() {
    int playerSeed = (int) rand() % totalResource();
    for (auto r : resources){
        if (r.second != 0){
            playerSeed -= r.second;
            if (playerSeed <= 0){return r.first;}
        }
        
    }
}

int Player::totalResource() const {
    int sum = 0;
    for (auto resource : resources) {
        sum += resource.second;
    }
    return sum;
}

void Player::buildResidence(int location, bool isFree) {
    if (!isFree) {
        for (const auto resourceCost: BASEMENT_COST) {
            if (resources[resourceCost.first] < resourceCost.second) {
                throw InsufficientResourceException{color, resourceCost.first};
            }
        }
        for (const auto resourceCost: BASEMENT_COST) {
            takeResource(resourceCost.first, resourceCost.second);
        }
    }
    residences[location] = Residence::Basement;
    points += RESIDENCE_TO_POINTS.at(Residence::Basement);
}

void Player::upgradeResidence(int location) {
    if (residences.count(location) == 0) {
        throw PlayerOwnershipException();
    }
    if (residences[location] == Residence::Tower) {
        throw PlayerResidenceTypeException();
    }
    std::map<Resource, int> resourcesRequired;

    if (residences[location] == Residence::Basement) {
        resourcesRequired = HOUSE_COST;
    } else if (residences[location] == Residence::House) {
        resourcesRequired = TOWER_COST;
    }

    for (const auto resourceCost: resourcesRequired) {
        if (resources[resourceCost.first] < resourceCost.second) {
            throw InsufficientResourceException{color, resourceCost.first};
        }
    }

    for (const auto resourceCost: resourcesRequired) {
        takeResource(resourceCost.first, resourceCost.second);
    }
    residences[location] == Residence::Basement 
        ? (residences[location] = Residence::House, points = points + RESIDENCE_TO_POINTS.at(Residence::House) - RESIDENCE_TO_POINTS.at(Residence::Basement))
        : (residences[location] = Residence::Tower, points = points + RESIDENCE_TO_POINTS.at(Residence::Tower) - RESIDENCE_TO_POINTS.at(Residence::House));
}


void Player::buildRoad(int location, bool isFree) {
    if (!isFree) {
        for (const auto resourceCost: ROAD_COST) {
            if (resources[resourceCost.first] < resourceCost.second) {
                throw InsufficientResourceException{color, resourceCost.first};
            }
        }
        for (const auto resourceCost: ROAD_COST) {
            takeResource(resourceCost.first, resourceCost.second);
        }
    }

    roads.emplace_back(location);
}

int Player::rollDice() const {
    return playerDice->rollDice();
}

int Player::getPoints() const {
    return points;
}

std::map<Resource, int> Player::getResources() const {
    return resources;
}

std::map<int, Residence> Player::getResidences() const {
    return residences;
}

std::vector<int> Player::getRoads() const {
    return roads;
}

Color Player::getColor() {
    return color;
}




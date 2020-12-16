#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <map>
#include <vector>
#include "enums.h"
#include <memory>
#include "dice.h"
//class Dice;

class Player {
    public:
        Player(Color player);
        Player(Color player, int points, std::map<Resource, int> resources, std::map<int, Residence> residences, std::vector<int> roads);
        void changeDice(DiceType dice);
        void addResource(Resource resource, int amount);
        void takeResource(Resource resource, int amount);
        Resource generateRandomResource();
        int totalResource() const;
        void buildResidence(int location, bool isFree = false);
        void upgradeResidence(int location);
        void buildRoad(int location, bool isFree = false);
        int rollDice() const;
        int getPoints() const;
        std::map<Resource, int> getResources() const;
        std::map<int, Residence> getResidences() const;
        std::vector<int> getRoads() const;
        Color getColor();

        // Copy constructor
        Player(const Player &other);
        // Move constructor
        Player(Player &&other);
        // Copy assignment operator
        Player &operator=(const Player &other);
        // Move assignment operator
        Player &operator=(Player &&other);
        // Destructor
        ~Player();

    private:
        Color color;
        int points;
        std::map<Resource, int> resources;
        std::map<int, Residence> residences;
        std::vector<int> roads;
        std::unique_ptr<Dice> playerDice;
};

class PlayerResidenceTypeException{};
class PlayerOwnershipException{};
class InvalidResourceAmount{};

#endif

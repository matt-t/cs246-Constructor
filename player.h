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
        std::map<int, Residence> getResidences() const;
        void addResource(Resource resource, int amount);
        void takeResource(Resource resource, int amount);
        void generateRandomResource();
        int totalResource() const;
        void buildResidence(int location);
        void upgradeResidence(int location);
        void buildRoad(int location);
        int rollDice(int seed) const;
        int handleGooseRoll();
        int getPoints() const;
        std::map<Resource, int> getResources() const;
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

class InsufficientResourceException{};
class PlayerResidenceTypeException{};

#endif

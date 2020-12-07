#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <map>
#include <vector>
#include "enums.h"
#include <memory>

class Dice;

class Player {
    public:
        void changeDice(DiceType dice);
        std::map<int, Residence> getResidences() const;
        void addResources(Resource resource, int amount);
        void takeResources(Resource resource, int amount);
        void generateRandomResource();
        int totalResource() const;
        void buildResidence(int location);
        void upgradeResidence(int location);
        void buildRoad(int location);
        int rollDice();
        int handleGooseRoll();

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

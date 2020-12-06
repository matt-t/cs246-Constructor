#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <map>
#include <vector>
#include "enums.h"

class Player {
    public:
        void changeDice(DiceType dice);
        std::map<Residence, std::vector<int>> getResidences() const;
        void addResources(resource: Resource, amount: int);
        void takeResources(resource: Resource, amount: int);
        void generateRandomResource();
        int totalResource() const;
        void buildResidence(color: Color, location: int);
        void buildRoad(location: int);
        int rollDice();
        int handleGooseRoll();
        void transfer(to: Color, resource: Resource);

    private:
        Color color;
        int points;
        std::map<Resource, int> resources;
        std::map<Residence, std::vector<int>> residences;
        std::vector<int> roads;
        Dice diceType;
        
};

class InsufficientResourceException {};

#endif

#ifndef __DICE_H__
#define __DICE_H__

#include "enums.h"

class Dice {
    public:
        virtual int rollDice(diceType dice) = 0;
};

#endif

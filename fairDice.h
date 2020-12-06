#ifndef __DICE_H__
#define __DICE_H__

#include "enums.h"

class FairDice: public Dice {
    public:
        int rollDice(diceType dice) const override;
};

#endif

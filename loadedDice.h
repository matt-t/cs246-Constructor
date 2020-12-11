#ifndef __LOADEDDICE_H__
#define __LOADEDDICE_H__

#include "dice.h"

class LoadedDice: public Dice {
    public:
        int rollDice() const override;
        std::unique_ptr<Dice> clone() const override;
};

class UnloadedDiceException {};
class InvalidRollException {};

#endif

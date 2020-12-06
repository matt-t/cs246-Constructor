#ifndef __LOADEDDICE_H__
#define __LOADEDDICE_H__

#include "dice.h"
#include "enums.h"

class LoadedDice: public Dice {
        int loadedNumber;
    public:
        int rollDice(diceType dice) const override;
        void loadDice(int num);
};

class UnloadedException {};


#endif

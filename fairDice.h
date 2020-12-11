#ifndef __FAIRDICE_H__
#define __FAIRDICE_H__

#include "dice.h"

class FairDice: public Dice {
    public:
        int rollDice() const override;
        std::unique_ptr<Dice> clone() const override;
};

#endif

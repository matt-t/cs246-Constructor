#ifndef __FAIRDICE_H__
#define __FAIRDICE_H__

#include "dice.h"

class FairDice: public Dice {
    public:
        int rollDice(int seed) const override;
        std::unique_ptr<Dice> clone() const override;
};

#endif

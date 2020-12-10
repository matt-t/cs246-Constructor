#ifndef __DICE_H__
#define __DICE_H__

#include <memory>

class Dice {
    public:
        virtual int rollDice(int seed) const = 0;
        virtual ~Dice() = default;
        virtual std::unique_ptr<Dice> clone() const = 0;
};

#endif

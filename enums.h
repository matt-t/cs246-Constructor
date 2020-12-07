#ifndef _ENUMS_H
#define _ENUMS_H

#include <iostream>

// possible states of a vertex
enum class Residence { Basement, House, Tower };

// five resources in the game, obtaineed by the players and allocated by their tiles
enum class Resource { Brick, Energy, Glass, Heat, Wifi, Park };

enum class Color { Blue, Red, Orange, Yellow, None };

enum class DiceType { Fair, Loaded };

std::ostream& operator<<(std::ostream& out, const Residence residence);
std::ostream& operator<<(std::ostream& out, const Resource resource);
std::ostream& operator<<(std::ostream& out, const Color color);

#endif

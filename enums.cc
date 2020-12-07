#include "enums.h"
#include "constants.h"
#include <iostream>

std::ostream& operator<<(std::ostream& out, const Residence residence) {
    if (residence == Residence::Basement) {
        out <<  RESIDENCE_BASEMENT_STRING;
    } else if (residence == Residence::House) {
        out << RESIDENCE_HOUSE_STRING;
    } else if (residence == Residence::Tower) {
        out <<  RESIDENCE_TOWER_STRING;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Resource resource) {
    if (resource == Resource::Brick) {
        out <<  RESOURCE_BRICK_STRING;
    } else if (resource == Resource::Energy) {
        out << RESOURCE_ENERGY_STRING;
    } else if (resource == Resource::Glass) {
        out <<  RESOURCE_GLASS_STRING;
    } else if (resource == Resource::Heat) {
        out <<  RESOURCE_HEAT_STRING;
    } else if (resource == Resource::Wifi) {
        out <<  RESOURCE_WIFI_STRING;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Color color) {
    if (color == Color::Blue) {
        out <<  COLOR_BLUE_STRING;
    } else if (color == Color::Red) {
        out << COLOR_RED_STRING;
    } else if (color == Color::Orange) {
        out <<  COLOR_ORANGE_STRING;
    } else if (color == Color::Yellow) {
        out <<  COLOR_YELLOW_STRING;
    } else if (color == Color::None) {
        out <<  COLOR_NONE_STRING;
    }
    return out;
}

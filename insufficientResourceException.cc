#include "insufficientResourceException.h"

InsufficientResourceException::InsufficientResourceException(Color color, Resource resource) : color{color}, resource{resource} {}

Color InsufficientResourceException::getColor() {
    return color;
};

Resource InsufficientResourceException::getResource() {
    return resource;
};

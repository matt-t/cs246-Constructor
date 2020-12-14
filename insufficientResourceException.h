#ifndef __INSUFFICIENTRESOURCEEXCEPTION_H__
#define __INSUFFICIENTRESOURCEEXCEPTION_H__

#include "enums.h"

class InsufficientResourceException {
    Color color;
    Resource resource;
    public:
        Color getColor();
        Resource getResource();
        InsufficientResourceException(Color color, Resource resource);
};

#endif

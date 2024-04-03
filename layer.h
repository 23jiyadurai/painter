#pragma once
#include "pixel.h"
#include <random>
#include <numbers>
class layer {
    sf::Uint8* pixels;
    unsigned int width;
    unsigned int height;
public:
    layer(unsigned int w, unsigned int h);
    layer(unsigned int w, unsigned int h, const pixel& p);
    sf::Uint8* as_u8();

    void applyVerticalGradient(const pixel &p);
    void randomize();
    void ring();
};

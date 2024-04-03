#include "pixel.h"

pixel::pixel() {
    red = 0;
    green = 0;
    blue = 0;
    alpha = 0;
}

pixel::pixel(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a) {
    red = r;
    green = g;
    blue = b;
    alpha = a;
}

// TODO:: Make this a constexpr?
pixel pixel::operator+(const pixel &other) const {
    sf::Uint8 max = std::min((sf::Uint8)(255-alpha), other.alpha);
    pixel sum(u8_add(red, std::min(max, other.red)),
              u8_add(red, std::min(max, other.red)),
              u8_add(red, std::min(max, other.red)),
              u8_add(alpha, max));
    return sum;
}

pixel::pixel(const pixel &other) : red(other.red), green(other.green), blue(other.blue), alpha(other.alpha){}


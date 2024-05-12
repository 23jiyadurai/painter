#include "Pixel.h"

Pixel::Pixel() {
    red = 0;
    green = 0;
    blue = 0;
    alpha = 0;
}

Pixel::Pixel(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a) {
    red = r;
    green = g;
    blue = b;
    alpha = a;
}

// TODO:: Make this a constexpr?

Pixel Pixel::operator+(const Pixel &other) const {
    sf::Uint8 max = std::min((sf::Uint8)(255-alpha), other.alpha);
    Pixel sum(u8_add(red, std::min(max, other.red)),
              u8_add(red, std::min(max, other.red)),
              u8_add(red, std::min(max, other.red)),
              u8_add(alpha, max));
    return sum;
}

Pixel::Pixel(const Pixel &other) = default;


#pragma once
#include <SFML/Graphics.hpp>

constexpr unsigned char u8_add(unsigned short a, unsigned short b){
    if (a + b > 255) return 255;
    return a + b;
}

class Pixel {
public:
    sf::Uint8 red;
    sf::Uint8 green;
    sf::Uint8 blue;
    sf::Uint8 alpha;
    Pixel();
    Pixel(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);
    Pixel(const Pixel& other);
    Pixel operator+(const Pixel& other) const;
    explicit Pixel(unsigned int i);
    explicit operator unsigned int() const;

};

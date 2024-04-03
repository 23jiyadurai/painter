#pragma once
#include <SFML/Graphics.hpp>

constexpr unsigned char u8_add(unsigned short a, unsigned short b){
    if (a + b > 255) return 255;
    return a + b;
}

class pixel {
public:
    sf::Uint8 red;
    sf::Uint8 green;
    sf::Uint8 blue;
    sf::Uint8 alpha;
    pixel();
    pixel(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);
    pixel(const pixel& other);
    pixel operator+(const pixel& other) const;

};

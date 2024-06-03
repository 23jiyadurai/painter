#pragma once
#include "Pixel.h"
#include <random>
#include <numbers>

struct Brush{
    std::vector<std::pair<int,int>> offsets;
    double r;
};
class Layer {
    unsigned int width;
    unsigned int height;
    sf::Uint8* pixels;
    sf::Texture* texture;
public:
    Layer();
    Layer(unsigned int w, unsigned int h);
    Layer(unsigned int w, unsigned int h, const Pixel& p);
    sf::Uint8* as_u8();
    sf::Sprite* as_Sprite();

    void applyVerticalGradient(const Pixel &p);
    void randomize();
    void ring();
    void drawPixel(const Pixel& p, int x, int y);
    void clear(const Pixel& p);

    void drawWithBrush(const Pixel &p, int x, int y, Brush &brush);
    void drawLine(const Pixel &p, int x1, int y1, int x2, int y2, Brush &brush);

    void bucket(const Pixel &p, int x, int y);
};

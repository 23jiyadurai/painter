#pragma once
#include "Layer.h"

class Canvas : public sf::Drawable {
public:
    std::vector<Layer*> layers;
    Layer *curLayer;
    size_t curLayerIndex;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    explicit Canvas(std::vector<Layer*>& ls);
    Canvas() = default;
    void moveUpALayer();
    void moveDownALayer();
};
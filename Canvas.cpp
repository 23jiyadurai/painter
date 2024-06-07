#include "Canvas.h"

#include <ranges>

void Canvas::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto & layer : std::ranges::reverse_view(layers)){
        target.draw(*layer);
    }
}

Canvas::Canvas(std::vector<Layer*> &ls) {
    layers = ls;
    curLayer = ls[0];
    curLayerIndex = 0;
}

void Canvas::moveUpALayer() {
    if (curLayerIndex <= 0) return;
    curLayerIndex--;
    curLayer = layers[curLayerIndex];
}

void Canvas::moveDownALayer() {
    if (curLayerIndex + 1 >= layers.size()) return;
    curLayerIndex++;
    curLayer = layers[curLayerIndex];

}

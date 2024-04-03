#include <SFML/Graphics.hpp>
#include "layer.h"
#include <iostream>
void drawCircle(sf::RenderWindow *window){
    sf::CircleShape circle(150);
    circle.setFillColor(sf::Color::Cyan);
    window->draw(circle);
}
int main(){
    auto window = new sf::RenderWindow(sf::VideoMode(800,800), "painter.exe");
    pixel p(0xFF, 0x00, 0x00, 255);
    layer l(800,800, p);
    pixel q(0xFF, 0x92, 0x00, 255);
    l.applyVerticalGradient(q);
//    l.randomize();
//    l.ring();
    sf::Image image;
    image.create(800, 800, l.as_u8());
    sf::Texture texture;
    texture.create(800, 800);
    texture.update(image);
    sf::Sprite sprite(texture);
    while (window->isOpen()){
        sf::Event event{};
        while (window->pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window->close();
        }
        window->clear(sf::Color::White);
        window->draw(sprite);
        window->display();
    }
}
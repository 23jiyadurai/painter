#include <SFML/Graphics.hpp>
#include "Layer.h"
#include <iostream>
#include <chrono>


int main(){
    auto window = new sf::RenderWindow(sf::VideoMode(800,800), "painter");
    Pixel p(0xFF, 0xFF, 0xFF, 255);
    Layer l(800, 800, p);
    Pixel q(0x00, 0x00, 0x00, 255);
//    l.applyVerticalGradient(q);
//    l.randomize();
//    l.ring();
//    sf::Image image;
//    image.create(800, 800, l.as_u8());
//    sf::Texture texture;
//    texture.create(800, 800);
//    texture.update(image);
//    sf::Sprite sprite(texture);
    auto lSprite = l.as_Sprite();
    auto start = std::chrono::system_clock::now();
    int frames = 0;
    bool clicking = false;
    while (window->isOpen()){
        sf::Event event{};
        while (window->pollEvent(event)){
            switch (event.type){
                case sf::Event::Closed:
                    window->close();
                    break;
                case sf::Event::MouseButtonPressed:
                    clicking = true;
                    l.drawPixel(q, event.mouseButton.x, event.mouseButton.y);
                    break;
                case sf::Event::MouseButtonReleased:
                    clicking = false;
                    break;
                case sf::Event::MouseMoved:
                    if (clicking) l.drawPixel(q, event.mouseMove.x, event.mouseMove.y);
                    break;
            }
        }
        lSprite = l.as_Sprite();

        window->clear(sf::Color::White);
        window->draw(*lSprite);
        frames++;
        window->display();
    }
    std::cout << "Made " << frames << " frames in " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start) << " milliseconds." << std::endl;


}
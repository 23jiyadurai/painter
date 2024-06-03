#include <SFML/Graphics.hpp>
#include "Layer.h"
#include <iostream>
#include <chrono>

int main(){
    auto window = new sf::RenderWindow(sf::VideoMode(800,800), "painter");
    window->setVerticalSyncEnabled(true);
    Pixel p(0xFF, 0xFF, 0xFF, 255);
    Layer l(800, 800, p);
    Pixel q(0x000000ff);
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
    int events = 0;
    sf::Font font;
    if (!font.loadFromFile("JetBrainsMono-Light.ttf")) {
        return 1;
    }
    sf::Text eventsText("", font, 24);
    sf::Text fpsText("", font, 24);
    sf::Text rgbText("", font, 24);
    rgbText.setFillColor(sf::Color::Magenta);
    fpsText.setFillColor(sf::Color::Blue);
    eventsText.setFillColor(sf::Color::Green);

    Brush brush2 = {{{-1,0}, {0,-1}, {0,0}, {1,0}, {0,1}}, 1};
    Brush brush3 = {{{-1,-1}, {-1,1}, {0,0}, {1,-1}, {1,1}}, 1};
    Brush brush1 = {{{0,0}}, 0.5};
    auto brushes = {brush1, brush2, brush3};
    bool clicking = false;
    Brush* currentBrush = &brush1;
    std::pair<int,int> previousPosition = {-1,-1};
    while (window->isOpen()){
        sf::Event event{};
        while (window->pollEvent(event)){
            events++;
            switch (event.type){

                case sf::Event::Closed:
                    window->close();
                    break;
                case sf::Event::MouseButtonPressed:
                    clicking = true;
//                    l.drawWithBrush(q, event.mouseButton.x, event.mouseButton.y, *currentBrush);
                    l.drawLine(q, previousPosition.first, previousPosition.second, event.mouseButton.x, event.mouseButton.y, *currentBrush);
                    break;
                case sf::Event::MouseButtonReleased:
                    clicking = false;
                    break;
                case sf::Event::MouseMoved:
//                    if (clicking) l.drawWithBrush(q, event.mouseMove.x, event.mouseMove.y, *currentBrush);
                    if (clicking) {
                        l.drawLine(q, previousPosition.first, previousPosition.second, event.mouseMove.x, event.mouseMove.y, *currentBrush);
                    }
                    previousPosition = {event.mouseMove.x, event.mouseMove.y};
                    break;
                case sf::Event::KeyPressed:
                    switch(event.key.code){
                        case sf::Keyboard::Num1:
                            currentBrush = &brush1;
                            break;
                        case sf::Keyboard::Num2:
                            currentBrush = &brush2;
                            break;
                        case sf::Keyboard::Num3:
                            currentBrush = &brush3;
                            break;
                        case sf::Keyboard::Z:
                            q = Pixel((unsigned int)(q)+256+256*256+256*256*256);
                            break;
                        case sf::Keyboard::C:
                            l.clear(p);
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        lSprite = l.as_Sprite();
        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start) >= std::chrono::seconds(1)){
            start = std::chrono::system_clock::now();
            fpsText.setString(std::to_string(frames));
            frames = 0;
            events = 0;
        }
        rgbText.setPosition(800-rgbText.getLocalBounds().width, 0);
        rgbText.setString(std::format("0x{:x}", (unsigned int)q));
        eventsText.setString(std::format("{}", events));
        eventsText.setPosition(800 - eventsText.getLocalBounds().width, 800 - eventsText.getLocalBounds().height);

        window->clear(sf::Color::White);
        window->draw(*lSprite);
        window->draw(fpsText);
        window->draw(rgbText);
        window->draw(eventsText);
        frames++;
        window->display();
    }
//    std::cout << "Made " << frames << " frames in " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start) << " milliseconds." << std::endl;


}
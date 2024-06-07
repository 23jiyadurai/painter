#include <SFML/Graphics.hpp>
#include "Layer.h"
#include "Canvas.h"
#include <iostream>
#include <chrono>

int taxicabDistance(std::pair<int,int>, std::pair<int,int>);

enum class Tool { Line, Bucket, Brush, LinesOut, Triangle };
void useTool(Tool t, std::vector<std::pair<int,int>>& pastPositions, std::pair<int,int> cur, Layer& layer, Brush& brush, Pixel& p){
    switch (t){
        case Tool::Line:
            if (pastPositions.empty()) break;
            layer.drawLine(p, pastPositions[0].first, pastPositions[0].second, cur.first, cur.second, brush);
            break;
        case Tool::Bucket:
            layer.bucket(p, cur.first, cur.second);
            break;
        case Tool::Brush:
            layer.drawWithBrush(p, cur.first, cur.second, brush);
            break;
        case Tool::LinesOut:
            layer.drawLinesOutOfPoint(p, cur.first, cur.second, brush);
            break;
        case Tool::Triangle:
            if (pastPositions.size() > 2){
                layer.drawPolygon(p, pastPositions, cur.first, cur.second, brush);
                pastPositions.clear();
            }
            break;
        default:
            break;
    }
}

int main(){
    int WINDOW_WIDTH = 1200;
    int WINDOW_HEIGHT = 800;
    auto window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "painter");
    window->setVerticalSyncEnabled(true);
    Pixel p(0xFF, 0xFF, 0xFF, 255);
    Pixel transparent(0, 0, 0, 0);
    Layer l(WINDOW_WIDTH, WINDOW_HEIGHT, transparent);
    Layer l2(WINDOW_WIDTH, WINDOW_HEIGHT, p);
    std::vector<Layer*> ls = {&l, &l2};
    Canvas canvas(ls);

    Pixel q(0x000000ff);
//    l.applyVerticalGradient(q);
//    l.randomize();
//    sf::Image image;
//    image.create(800, 800, l.as_u8());
//    sf::Texture texture;
//    texture.create(800, 800);
//    texture.update(image);
//    sf::Sprite sprite(texture);
//    auto lSprite = l.as_Sprite();
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
    Brush brush4 = {{{-1,-1}, {-1,0}, {0,-1}, {0,0}}, 1};
    auto brushes = {brush1, brush2, brush3};
    bool clicking = false;
    Brush* currentBrush = &brush1;
//    std::pair<int,int> previousPosition = {-1,-1};
//    std::pair<int,int> prevPrevPos = {-1, -1};
    std::vector<std::pair<int,int>> pastPositions({{-1,-1}, {-1,-1}});
    Tool currentTool = Tool::Brush;
    while (window->isOpen()){
        auto curLayer = canvas.curLayer;
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
//                    curLayer->drawLine(q, previousPosition.first, previousPosition.second, event.mouseButton.x, event.mouseButton.y, *currentBrush);
                    pastPositions.insert(pastPositions.begin(), {event.mouseButton.x, event.mouseButton.y});
                    useTool(currentTool, pastPositions, {event.mouseButton.x, event.mouseButton.y}, *curLayer, *currentBrush, q);
                    if (currentTool != Tool::Triangle && pastPositions.size() > 2) pastPositions.pop_back();
                    //prevPrevPos = previousPosition;
//                    previousPosition = {event.mouseButton.x, event.mouseButton.y};
//                    l.drawLinesOutOfPoint(q, event.mouseButton.x, event.mouseButton.y, *currentBrush);
                    break;
                case sf::Event::MouseButtonReleased:
                    clicking = false;
                    break;
                case sf::Event::MouseMoved:
//                    if (clicking) l.drawWithBrush(q, event.mouseMove.x, event.mouseMove.y, *currentBrush);
                    if (clicking && currentTool == Tool::Brush) {
//                        curLayer->drawLine(q, previousPosition.first, previousPosition.second, event.mouseMove.x, event.mouseMove.y, *currentBrush);
                        useTool(Tool::Line, pastPositions, {event.mouseMove.x, event.mouseMove.y}, *curLayer, *currentBrush, q);

                        if (taxicabDistance(pastPositions[1], {event.mouseMove.x, event.mouseMove.y}) < 20)
                            useTool(Tool::Line, pastPositions, {event.mouseMove.x, event.mouseMove.y}, *curLayer, *currentBrush, q);

//                            curLayer->drawLine(q, prevPrevPos.first, prevPrevPos.second, event.mouseMove.x, event.mouseMove.y, *currentBrush);
                    }
                    if (currentTool == Tool::Brush){
                        pastPositions.insert(pastPositions.begin(), {event.mouseMove.x, event.mouseMove.y});
                        if (pastPositions.size() > 2) pastPositions.pop_back();

//                        prevPrevPos = previousPosition;
//                        previousPosition = {event.mouseMove.x, event.mouseMove.y};
                    }
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
                        case sf::Keyboard::Num4:
                            currentBrush = &brush4;
                            break;
                        case sf::Keyboard::Z:
                            q = Pixel((unsigned int)(q)+256+256*256+256*256*256);
                            break;
                        case sf::Keyboard::C:
                            curLayer->clear(transparent);
                            break;
                        case sf::Keyboard::L:
//                            curLayer->drawLinesOutOfPoint(q, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, *currentBrush);
                            currentTool = Tool::Line;
                            pastPositions.clear();
                            break;
//                        case sf::Keyboard::M:
//                            curLayer->drawLine(q, WINDOW_WIDTH / 2, 400, 800, 800, *currentBrush);
//                            break;
                        case sf::Keyboard::R:
                            q.red++;
                            break;
                        case sf::Keyboard::G:
                            q.green++;
                            break;
                        case sf::Keyboard::B:
                            q.blue++;
                            break;
                        case sf::Keyboard::O:
                            curLayer->ring();
                            break;
                        case sf::Keyboard::Up:
                            canvas.moveUpALayer();
                            break;
                        case sf::Keyboard::Down:
                            canvas.moveDownALayer();
                            break;
                        case sf::Keyboard::X:
                            currentTool = Tool::LinesOut;
                            break;
                        case sf::Keyboard::P:
                            currentTool = Tool::Triangle;
                            pastPositions.clear();
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
//        lSprite = l.as_Sprite();
        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start) >= std::chrono::seconds(1)){
            start = std::chrono::system_clock::now();
            fpsText.setString(std::to_string(frames));
            frames = 0;
            events = 0;
        }
        rgbText.setPosition(static_cast<float>(WINDOW_WIDTH) - rgbText.getLocalBounds().width, 0);
        rgbText.setString(std::format("0x{:x}", (unsigned int)q));
        eventsText.setString(std::format("{}", events));
        eventsText.setPosition(static_cast<float>(WINDOW_WIDTH) - eventsText.getLocalBounds().width,
                               static_cast<float>(WINDOW_HEIGHT) - eventsText.getLocalBounds().height);

        window->clear(sf::Color::White);
//        window->draw(*lSprite);
//        window->draw(l);
        window->draw(canvas);
        window->draw(fpsText);
        window->draw(rgbText);
        window->draw(eventsText);
        frames++;
        window->display();
    }
//    std::cout << "Made " << frames << " frames in " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start) << " milliseconds." << std::endl;


}

int taxicabDistance(std::pair<int,int> a, std::pair<int,int> b){
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}
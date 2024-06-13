#include <SFML/Graphics.hpp>
#include "Layer.h"
#include "Canvas.h"
#include <iostream>
#include <chrono>

int taxicabDistance(std::pair<int,int>, std::pair<int,int>);
std::pair<int,int> convertCoordinates(std::pair<int,int> coords, sf::Vector2<unsigned int> windowSize, int width, int height);

enum class Tool { Line, Bucket, Brush, LinesOut, Triangle, IntermediateLine, Polygon };
void useTool(Tool t, std::vector<std::pair<int,int>>& pastPositions, std::pair<int,int> cur, Layer& layer, Brush& brush, Pixel& p){
    switch (t){
        case Tool::Line:
            if (pastPositions.size() < 2) break;
            layer.drawLine(p, pastPositions[1].first, pastPositions[1].second, cur.first, cur.second, brush);
            pastPositions.clear();
//            if (pastPositions.size() > 1 && taxicabDistance(pastPositions[1], cur) < 20)
//                layer.drawLine(p, pastPositions[1].first, pastPositions[1].second, cur.first, cur.second, brush);
            break;
        case Tool::IntermediateLine:
            if (pastPositions.size() < 2) break;
            layer.drawLine(p, pastPositions[1].first, pastPositions[1].second, cur.first, cur.second, brush);
//            layer.drawLine(p, pastPositions[2].first, pastPositions[2].second, cur.first, cur.second, brush);
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
        case Tool::Polygon:
            if (pastPositions.size() < 3) break;
            if (!(cur.first == -1 && cur.second == -1)) break;
            layer.drawPolygon(p, pastPositions, pastPositions[0].first, pastPositions[0].second, brush);
            break;
        default:
            break;
    }
}

int main(){
    int WINDOW_WIDTH = 1200;
    int WINDOW_HEIGHT = 800;
    auto window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "painter");
//    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(60);
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
    sf::View view = window->getView();
    while (window->isOpen()){
        auto curLayer = canvas.curLayer;
        sf::Event event{};
        while (window->pollEvent(event)){
            events++;
            switch (event.type){

                case sf::Event::Closed:
                    window->close();
                    break;
                case sf::Event::Resized:
                    std::cout << "hello\n";
//                    view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
//                    window->setView(view);
                    break;
                case sf::Event::MouseButtonPressed:
                    clicking = true;
//                    l.drawWithBrush(q, event.mouseButton.x, event.mouseButton.y, *currentBrush);
//                    curLayer->drawLine(q, previousPosition.first, previousPosition.second, event.mouseButton.x, event.mouseButton.y, *currentBrush);

                    std::cout << "(" <<event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
                    pastPositions.insert(pastPositions.begin(), convertCoordinates({event.mouseButton.x, event.mouseButton.y}, window->getSize(), WINDOW_WIDTH, WINDOW_HEIGHT));
                    useTool(currentTool, pastPositions, convertCoordinates({event.mouseButton.x, event.mouseButton.y}, window->getSize(), WINDOW_WIDTH, WINDOW_HEIGHT), *curLayer, *currentBrush, q);
                    if ((currentTool != Tool::Triangle && currentTool != Tool::Polygon) && pastPositions.size() > 2) pastPositions.pop_back();
                    //prevPrevPos = previousPosition;
//                    previousPosition = {event.mouseButton.x, event.mouseButton.y};
//                    l.drawLinesOutOfPoint(q, event.mouseButton.x, event.mouseButton.y, *currentBrush);
                    break;
                case sf::Event::MouseButtonReleased:
                    clicking = false;
                    break;
                case sf::Event::MouseMoved:
//                    if (clicking) l.drawWithBrush(q, event.mouseMove.x, event.mouseMove.y, *currentBrush);
                    if (currentTool == Tool::Brush){
                        pastPositions.insert(pastPositions.begin(), convertCoordinates({event.mouseMove.x, event.mouseMove.y}, window->getSize(), WINDOW_WIDTH, WINDOW_HEIGHT));
                        if (pastPositions.size() > 2) pastPositions.pop_back();

//                        prevPrevPos = previousPosition;
//                        previousPosition = {event.mouseMove.x, event.mouseMove.y};
                    }
                    if (clicking && currentTool == Tool::Brush) {
//                        curLayer->drawLine(q, previousPosition.first, previousPosition.second, event.mouseMove.x, event.mouseMove.y, *currentBrush);
                        useTool(Tool::IntermediateLine, pastPositions, convertCoordinates({event.mouseMove.x, event.mouseMove.y}, window->getSize(), WINDOW_WIDTH, WINDOW_HEIGHT), *curLayer, *currentBrush, q);

//                        if (taxicabDistance(pastPositions[1], {event.mouseMove.x, event.mouseMove.y}) < 20)
//                            useTool(Tool::Line, pastPositions, {event.mouseMove.x, event.mouseMove.y}, *curLayer, *currentBrush, q);

//                            curLayer->drawLine(q, prevPrevPos.first, prevPrevPos.second, event.mouseMove.x, event.mouseMove.y, *currentBrush);
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
                        case sf::Keyboard::T:
                            currentTool = Tool::Triangle;
                            pastPositions.clear();
                            break;
                        case sf::Keyboard::P:
                            if (currentTool == Tool::Polygon)
                                useTool(currentTool, pastPositions, {-1,-1}, *curLayer, *currentBrush, q);
                            currentTool = Tool::Polygon;
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

std::pair<int,int> convertCoordinates(std::pair<int,int> coords, sf::Vector2<unsigned int> windowSize, int width, int height){
    float newX = (float)coords.first / (float)windowSize.x * (float)width;
    float newY = (float)coords.second / (float)windowSize.y * (float)height;
    return {newX, newY};
}

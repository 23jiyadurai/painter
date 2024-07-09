#include <iostream>
#include <unordered_set>
#include "Layer.h"

sf::Uint8 *Layer::as_u8() {
    return pixels;
}

Layer::Layer(unsigned int w, unsigned int h) {
    width = w;
    height = h;
    pixels = new sf::Uint8[4*w*h]();
    texture = new sf::Texture();
    texture->create(width, height);
}

Layer::Layer(unsigned int w, unsigned int h, const Pixel &p) {
    width = w;
    height = h;
    pixels = new sf::Uint8[4*w*h];
    for (int i = 0; i < 4 * w * h; i += 4){
        pixels[i] = p.red;
        pixels[i+1] = p.green;
        pixels[i+2] = p.blue;
        pixels[i+3] = p.alpha;
    }
    texture = new sf::Texture();
    texture->create(width, height);
}

// this method doesn't quite work with some alpha values but looks ok
void Layer::applyVerticalGradient(const Pixel& p) {
    float normalR = (float)p.red/255.0f;
    float normalG = (float)p.green/255.0f;
    float normalB = (float)p.blue/255.0f;
    float normalA = (float)p.alpha/255.0f;

    for (int i = 0; i < 4 * width * height; i += 4){
        unsigned char strength = i / ((int)width * 4.0f / 255.0f * (int)height);
        pixels[i] = u8_add((strength * normalR), pixels[i]);
        pixels[i+1] = u8_add((strength * normalG), pixels[i+1]);
        pixels[i+2] = u8_add((strength * normalB), pixels[i+2]);
        pixels[i+3] = u8_add((strength * normalA), pixels[i+3]);
    }
}

void Layer::randomize() {
    std::random_device rd;
    std::mt19937 rand(rd());
    std::uniform_int_distribution<unsigned char> dist(0,255);
    for (int i = 0; i < 4 * width * height; i += 4){
        pixels[i] = dist(rand);
        pixels[i+1] = dist(rand);
        pixels[i+2] = dist(rand);
        pixels[i+3] = 255;
    }
}

void Layer::ring() {
    // color of a Pixel is based on (h/2-y) / (w/2 - x) for x != w/2, (h/2-y) otherwise?
    int x = 0;
    int y = 0;
    for (int i = 0; i < 4 * width * height; i += 4){
        if (x == width){
            x = 0;
            y++;
        }
        double posX = (double)x / (double)width;
        double posY = (double)y / (double)height;
        bool left = posX < 0.5;
//        int divisor = (int)width/2 - x;
//        if (divisor == 0) divisor = 1;
//        auto color = (unsigned char)(255.0f * (((float)height/2-y) / (float)(width/2-x)));
        double theta = std::atan((0.5-posY)/ (posX-0.5));
        if (left) theta+=std::numbers::pi;
//        unsigned int color = (unsigned int)((std::acos(((double)width/2-(double)x)/(double)(std::sqrt((width/2.0 - x)*(width/2.0 -x) + (height/2.0 - y)*(height/2.0-y))))*(8.0*16777216.0 * std::numbers::inv_pi)));
//        unsigned int color = (unsigned int)((std::numbers::pi / 2.0 + std::atan((height/2.0-y)/std::abs((width/2.0-x))))*(16777216.0 * std::numbers::inv_pi));

//        unsigned char color = (x*(width-x)/400 + y*(height-y)/400)%256;
//        unsigned char color = (x*x/200 + y*y/200)%256;
//        unsigned char color = (x*x+y*y)%256;
//        unsigned char color = (x+y)%256;
//        unsigned char color = (int)((255*(y/(float)x)))%256;
//        unsigned char red = (unsigned char)(127.0 + std::sin(theta)*127.0);
//        unsigned char green = (unsigned char)(127.0 + std::sin(theta+2.0*std::numbers::pi/3.0)*127.0);
//        unsigned char blue = (unsigned char)(127.0 + std::sin(theta+4.0*std::numbers::pi/3.0)*127.0);

        double colR = 0.5 + 0.5*std::sin(theta);
        double colG = 0.5 + 0.5*std::sin(theta+2.0*std::numbers::pi/3.0);
        double colB = 0.5 + 0.5*std::sin(theta+4.0*std::numbers::pi/3.0);
//        double colR = std::max(std::sin(theta), 0.0);
//        double colG = std::max(std::sin(theta + 2.0*std::numbers::pi/3.0), 0.0);
//        double colB = std::max(std::sin(theta + 4.0*std::numbers::pi/3.0), 0.0);
        unsigned char red = (unsigned char)(255*colR);
        unsigned char green = (unsigned char)(255*colG);
        unsigned char blue = (unsigned char)(255*colB);

        pixels[i] = red;
        pixels[i+1] = green;
        pixels[i+2] = blue;
        pixels[i+3] = 255;
        x++;

    }

}

std::unique_ptr<sf::Sprite> Layer::as_Sprite() {
//    sf::Image image;
//    image.create(800, 800, pixels);
    texture->update(pixels);
    auto sprite = std::make_unique<sf::Sprite>(sf::Sprite(*texture));
    return sprite;
}

Layer::Layer() {
    width = 800;
    height = 800;
    pixels = new sf::Uint8[4*width*height];
    texture = new sf::Texture();
    texture->create(width, height);
}

void Layer::drawPixel(const Pixel &p, int x, int y) {
    if (x >= 0 && y >= 0 && x < width && y < height){
        int offset = 4*((int)width*y+x);
        pixels[offset] = p.red;
        pixels[offset+1] = p.green;
        pixels[offset+2] = p.blue;
        pixels[offset+3] = p.alpha;
    }
}

void Layer::drawWithBrush(const Pixel &p, int x, int y, Brush &brush){
    for (auto v : brush.offsets){
        drawPixel(p, x+v.first, y+v.second);
    }
}

void Layer::bucket(const Pixel &p, int x, int y){
    Pixel replacedColor(reinterpret_cast<int*>(pixels)[width*y + x]);
    std::cout << (int)replacedColor.red << " " << (int)replacedColor.green << " " << (int)replacedColor.blue << " " << (int)replacedColor.alpha << "\n";
    std::cout << (int)p.red << " " << (int)p.green << " " << (int)p.blue << " " << (int)p.alpha << "\n";
//    std::cout << *(unsigned int*)&replacedColor << " " << *(unsigned int*)&p << "\n";
    if (replacedColor == p) return;
    std::queue<int> q;
    std::unordered_set<int> set;
    q.emplace(y*width + x);
    while (!q.empty()){
        int position = q.front();
        q.pop();
        if (set.contains(position)) continue;
        set.insert(position);
        int offset = position * 4;
        if (position > 0  && position < width*height && Pixel(reinterpret_cast<int*>(pixels)[position]) == replacedColor){
            pixels[offset] = p.red;
            pixels[offset+1] = p.green;
            pixels[offset+2] = p.blue;
            pixels[offset+3] = p.alpha;
            q.emplace(position + 1);
            q.emplace(position - 1);
            q.emplace(position + width);
            q.emplace(position - width);
        }
    }

}

void Layer::drawLine(const Pixel &p, int x1, int y1, int x2, int y2, Brush &brush) {
//    std::cout << "Drew line from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ").\n";

    // if the calculation of slope will result in a division by 0
    if (x1 == x2){
        for (int i = std::min(y1,y2); i <= std::max(y1,y2); i++){
            drawWithBrush(p, x1, i, brush);
        }
        return;
    }
    // find the slope required to draw the line
    double slope = (double)(y2 - y1) / (x2 - x1);
    if (std::abs(slope) <= 1.0){
        int xPos = x1 < x2 ? x1 : x2;
        int xEnd = x1 < x2 ? x2 : x1;
        double yPos = x1 < x2 ? y1 : y2;

        while (xPos < xEnd){
            xPos++;
            yPos += slope;
            drawWithBrush(p, (int)xPos, (int)yPos, brush);
        }
    }
        // otherwise, treat y as the independent variable
    else {
        slope = 1.0/slope;
//        if (yPos > y2){
//            yPos = y2;
//            y2 = y1;
//        }
        int yPos = y1 < y2 ? y1 : y2;
        int yEnd = y1 < y2 ? y2 : y1;
        double xPos = y1 < y2 ? x1 : x2;
        while (yPos < yEnd){
            yPos += 1;
            xPos += slope;
            drawWithBrush(p, (int)xPos, (int)yPos, brush);
        }
    }
    // if going backwards, start from the leftmost point
//    if (x2 < x1){
//        std::swap(x1, x2);
//        std::swap(y1,y2);
//    }
//
//    double xPos = x1;
//    double yPos = y1;
    // if the slope is less than or equal to one, treat x as the independent variable


}

void Layer::clear(const Pixel &p) {
    delete[] pixels;
    pixels = new sf::Uint8[4*width*height];
    for (int i = 0; i < 4 * width * height; i += 4){
        pixels[i] = p.red;
        pixels[i+1] = p.green;
        pixels[i+2] = p.blue;
        pixels[i+3] = p.alpha;
    }
    delete texture;
    texture = new sf::Texture();
    texture->create(width, height);
}

Layer::~Layer() {
    delete[] pixels;
    delete texture;
}

void Layer::drawLinesOutOfPoint(const Pixel &p, int x, int y, Brush &brush) {
    for (int xPos = 0; xPos <= width; xPos += 50){
        drawLine(p, x, y, xPos, 0, brush);
        drawLine(p, x, y, xPos, height, brush);
    }
    for (int yPos = 0; yPos <= height; yPos += 50){
        drawLine(p, x, y, 0, yPos, brush);
        drawLine(p, x, y, width, yPos, brush);
    }


}

void Layer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    texture->update(pixels);
    target.draw(sf::Sprite(*texture));
}

void Layer::drawPolygon(Pixel &p, std::vector<std::pair<int, int>> &pastPositions, int x, int y, Brush &brush) {
    for (int i = 0; i < pastPositions.size() - 1; i++){
        drawLine(p, pastPositions[i].first, pastPositions[i].second, pastPositions[i+1].first, pastPositions[i+1].second, brush);
    }
    drawLine(p, pastPositions.back().first, pastPositions.back().second, x, y, brush);
    drawLine(p, pastPositions.front().first, pastPositions.front().second, x, y, brush);
}

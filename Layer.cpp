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

sf::Sprite* Layer::as_Sprite() {
//    sf::Image image;
//    image.create(800, 800, pixels);
    texture->update(pixels);
    auto* sprite = new sf::Sprite(*texture);
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
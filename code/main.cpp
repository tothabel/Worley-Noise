#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <math.h>
#include <time.h>
#include "quick_sort.hpp"
using namespace std;

float Remap (float value, float from1, float to1, float from2, float to2) {
    return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}

int main(){
    srand(time(0));
    
    sf::VideoMode videoMode = {sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height};
    vector<pair<int, int>> points;
    sf::VertexArray pixels(sf::Points, videoMode.width * videoMode.height);
    
    int pointNum;
    int n = 0;
    bool colored;
    cout << "Enter the number of points: ";
    cin >> pointNum;
    cout << endl;
    cout << "Should the noise be funky?: ";
    cin >> colored;
    cout << endl;
    
    //init variables
    for (int i = 0; i < pointNum; i++){
        pair<int, int> tempPoint = {rand() % videoMode.width, rand() % videoMode.height};
        points.push_back(tempPoint);
    }
    
    for (float x = 0; x < videoMode.width; x++){
        for (float y = 0; y < videoMode.height; y++){
            int index = y * videoMode.width + x;
            pixels[index].position = sf::Vector2f(x, y);
            pixels[index].color = sf::Color(x/(videoMode.width/250), y/(videoMode.height/250), 0);
        }
    }
    
    //calculate pixel colors
    for (int i = 0; i < videoMode.width * videoMode.height; i++){
        vector<float> distances_;
        for (pair<int, int> p : points){
            int dist = sqrt(pow(pixels[i].position.x - p.first, 2) +
                            pow(pixels[i].position.y - p.second, 2) * 1.0);
            distances_.push_back(dist);
        }
        float *distances = distances_.data();
        quick_sort(distances, pointNum);
        
        //set pixel colors based on if the noise is funky
        if (!colored)
            pixels[i].color = sf::Color(255, 255, 255, min(255.f, distances[n]));
        else {
            int r, g, b;
            r = Remap(distances[0], 0, 150, 0, 255);
            g = Remap(distances[1], 0, 50, 255, 0);
            b = Remap(distances[2], 0, 200, 255, 0);
            
            //r = Remap(distances[0], 0, 100, 0, 255);
            //g = Remap(distances[0], 0, 0, 0, 0);
            //b = Remap(distances[1], 0, 200, 0, 255);
            
            pixels[i].color = sf::Color(r, g, b);
        }

    }
    
    sf::RenderWindow window(videoMode, "Worley Noise Simulation");

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                    
                default:
                    break;
            }
        }
        
        //calculate distances if in gameloop

        window.clear(sf::Color::Black);
        
        //draw
        window.draw(pixels);
        
        window.display();
    }

    return 0;
}

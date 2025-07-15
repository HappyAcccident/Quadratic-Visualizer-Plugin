#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "Shape.h"
#include "Display.h"
#include "Animation.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);

    Display& display = Display::getInstance();

    display.setResolution(2048);
    int res = display.getResolution();

    Shape* A = new Shape(3, (13.0/3.0), M_PI/8.0 + 0.2, 3, "Regular"); //8 pointed star with steepness 3
    Shape* B = new Shape(1, res, M_PI/res, -2, "Regular"); //circle
    Shape* C = new Shape(1, 4, M_PI/4, 1, "Regular"); //square
    // Animation* CScale = new ScaleAnimation(144*1, 1, 2);
    Animation* CRotate = new RotateAnimation(144*10);
    // C->addAnimation(CScale);
    C->addAnimation(CRotate);
    display.addShape(C);

    // Shape* square = new Shape(1, 4, M_PI/4, 1, "Regular"); //square
    // Animation squareScale = Animation(144, 0.5, 1.5, "scale");
    // square->addAnimation(&squareScale);
    // display.addShape(square);
    // Shape* sum = new Shape(*A + *B);
    // display.addShape(sum);

    auto quadraticRoot = Shape::quadratic(*A, *B, *C);
    Shape* pos = new Shape(quadraticRoot.first);
    Shape* neg = new Shape(quadraticRoot.second);
    // display.addShape(pos);
    // display.addShape(neg);

    // Shape* square = new Shape(1, 4, 0, 1, "Regular");
    // Shape* root = new Shape(Shape::sqrt(*square));
    // display.addShape(root);
    display.setCurrentFrame(0);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::seconds(10);

    while (window.isOpen())
    {
        auto now = std::chrono::high_resolution_clock::now();
        // if (now - startTime >= duration) {
        //     break;
        // }

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        A->rotate(0.005);
        // B->rotate(-0.01);
        quadraticRoot = Shape::quadratic(*A, *B, *C);
        *pos = quadraticRoot.first;
        *neg = quadraticRoot.second;
        C->updateAnimations();

        // *sum = *A + *B;

        window.clear();

        float scale = display.getMaxRadius();

        for (auto shape : display.getShapes())
        {
            auto shapePts = shape->getPts();
            int res = display.getResolution();

            sf::VertexArray triangles(sf::PrimitiveType::Triangles, 3 * shapePts.size());
                    
            float radius = 2.0f;
                    
            for (int i = 0; i < shapePts.size(); i++) {
                float x = (500 / 1 /*scale*/) * shapePts[i].real();
                float y = (500 / 1 /*scale*/) * shapePts[i].imag();
            
                float xPrint = window.getSize().x / 2.f + x;
                float yPrint = window.getSize().y / 2.f - y;

                sf::Vector2f vertex1(xPrint - radius, yPrint - radius);
                sf::Vector2f vertex2(xPrint + radius, yPrint - radius);
                sf::Vector2f vertex3(xPrint, yPrint + radius);         
            
                triangles[3*i + 0].position = vertex1;
                triangles[3*i + 1].position = vertex2;
                triangles[3*i + 2].position = vertex3;
            
                for (int j = 0; j < 3; ++j) {
                    triangles[3*i + j].color = sf::Color::White;
                }
            }
            
            window.draw(triangles);
        }


        window.display();
        display.incrementCurrentFrame();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    double elapsedSeconds = std::chrono::duration<double>(endTime - startTime).count();
    std::cout << "Frames: " << display.getCurrentFrame() << "\n";
    std::cout << "Elapsed time: " << elapsedSeconds << " seconds\n";
    std::cout << "Average FPS: " << display.getCurrentFrame() / elapsedSeconds << "\n";

    return 0;
}

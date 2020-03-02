#include <iostream>
#include "Game/Globals.h"
#include "Game/Planet.h"
#include "Game/Unit.h"

sf::Font Global::g_font;
sf::Vector2f Global::g_mousePos = sf::Vector2f(0, 0);

int main()
{
    using namespace Container::Vector;

    Global::g_font.loadFromFile("AGENCYB.TTF");

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Simple RTS");

    Planet::MAX_INCREMENT_LEVEL = 5;
    Planet p;
    p.SetPosition(400, 400);
    p.SetMaxLevel(4);
    p.SetTeam("Player");
    p.SetColor(sf::Color::Green);
    p.SetCurrentLevel(4);
    Timer deltaTimer;
    deltaTimer.Start();

    Vector<Unit> unitVector(30, 30);

    for (size_t i = 0; i < unitVector.Size(); i++)
    {
        unitVector[i].SetPosition(rand() % 1280, 0);
        unitVector[i].SetDestination(&p);
        unitVector[i].SetInOrbit(true);
        unitVector[i].SetColor(sf::Color::Green);
    }


    bool leftMousePressed = false;
    bool rightMousePressed = false;
    while (window.isOpen())
    {
        float dt = (float)deltaTimer.Stop();
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
        }
        Global::g_mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

        bool leftMousePress = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        bool rightMousePress = sf::Mouse::isButtonPressed(sf::Mouse::Right);
        bool inc = false;
        bool dec = false;

        if (leftMousePress && !leftMousePressed)
            inc = true;
        if (rightMousePress && !rightMousePressed)
            dec = true;

        leftMousePressed = leftMousePress;
        rightMousePressed = rightMousePress;

        float x = -100.0f * dt * sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        x += 100.0f * dt * sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        float y = -100.0f * dt * sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        y += 100.0f * dt * sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        p.Move(x, y);

        p.Update(dt);


        for (size_t i = 0; i < unitVector.Size(); i++)
        {
            unitVector[i].Update(dt);
        }

        window.clear();

        p.Draw(&window);

        if (p.Contains(Global::g_mousePos))
        {
            if (inc)
            {
                int level = p.GetCurrentLevel();
                p.IncrementToNextLevel(1);
                if (p.GetCurrentLevel() > level)
                {
                    p.SetTeam("Player");
                    p.SetColor(sf::Color::Green);
                }
            }
            if (dec)
            {
                p.IncrementDestruction(1);
            }
            p.DrawInfo(&window);
        }
        
        for (size_t i = 0; i < unitVector.Size(); i++)
        {
            unitVector[i].Draw(&window);
        }

        window.display();
    }

    return 0;
}
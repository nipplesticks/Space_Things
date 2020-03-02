#include <iostream>
#include "Game/Globals.h"
#include "Game/Planet.h"
#include "Game/Unit.h"

sf::Font Global::g_font;
sf::Vector2f Global::g_mousePos = sf::Vector2f(0, 0);
QuadTree<Unit*> Global::g_unitQuadtree;

int main()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    using namespace Container::Vector;

    Global::g_font.loadFromFile("AGENCYB.TTF");

    Global::g_unitQuadtree.Init(sf::Vector2f(-5000, -5000), sf::Vector2f(5000, 5000), 10U);

    sf::RenderWindow window(sf::VideoMode(1280, 720, desktop.bitsPerPixel), "Planet_stuff");

    Planet::MAX_INCREMENT_LEVEL = 5;
    Planet p;
    p.SetPosition(400, 400);
    p.SetMaxLevel(4);
    p.SetTeam("Player");
    p.SetColor(sf::Color::Green);
    p.SetCurrentLevel(4);

    Vector<Unit> unitVector(500, 500);
    Vector<Unit*> unitVectorSelection;
    for (size_t i = 0; i < unitVector.Size(); i++)
    {
        unitVector[i].SetPosition(rand() % 1280, 0);
        unitVector[i].SetDestination(&p);
        unitVector[i].SetInOrbit(true);
        unitVector[i].SetColor(sf::Color::Green);
    }

    sf::RectangleShape selection;
    selection.setFillColor(sf::Color::Transparent);
    selection.setOutlineThickness(1);
    selection.setOutlineColor(sf::Color::White);

    bool leftMousePressed = false;
    bool rightMousePressed = false;

    double totalTime = 0;
    size_t frameCounter = 0;

    Timer deltaTimer;
    deltaTimer.Start();
    while (window.isOpen())
    {
        float dt = (float)deltaTimer.Stop();
        frameCounter++;
        totalTime += dt;
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
        {
            selection.setPosition(Global::g_mousePos);
            inc = true;
        }
        else if (leftMousePress)
            selection.setSize(Global::g_mousePos - selection.getPosition());

        if (rightMousePress && !rightMousePressed)
        {
            dec = true;


            Planet *pp = nullptr;
            sf::Vector2f planetPos = p.GetPosition();
            sf::Vector2f planetToMouse = Global::g_mousePos - planetPos;
            float lSqr = planetToMouse.x * planetToMouse.x + planetToMouse.y * planetToMouse.y;
            if (lSqr < (p.GetRadius() * p.GetRadius()))
                pp = &p;
            size_t selectionSize = unitVectorSelection.Size();
            for (size_t i = 0; i < selectionSize; i++)
            {
                if (pp)
                {
                    unitVectorSelection[i]->SetDestination(pp);
                    unitVectorSelection[i]->SetInOrbit(true);
                }
                else
                    unitVectorSelection[i]->SetDestination(Global::g_mousePos);
            }
        }

        if (!leftMousePress && leftMousePressed)
            unitVectorSelection = Global::g_unitQuadtree.GetObjectsFromQuad(selection.getPosition(), selection.getPosition() + selection.getSize());

        bool drawSelection = leftMousePress && leftMousePressed;

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
        
        Global::g_unitQuadtree.Clear();
        for (size_t i = 0; i < unitVector.Size(); i++)
            unitVector[i].DrawAndUpdateQt(&window);

        if (drawSelection)
            window.draw(selection);
        window.display();

        
        if (totalTime > 1.0f)
        {
            window.setTitle("FPS: " + std::to_string(frameCounter) + " | " + std::to_string(totalTime / frameCounter) + " ms/frame");
            totalTime -= 1.0f;
            frameCounter = 0;
        }
    }

    Global::g_unitQuadtree.Release();

    return 0;

}
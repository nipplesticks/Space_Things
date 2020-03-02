#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include "Globals.h"
#include "Planet.h"
#include "Unit.h"
#include "TeamHandler.h"
#include <string>

class Game
{
public:
    Game();

    ~Game();

    void Init();

    void Run(sf::RenderWindow * wnd);

    void Terminate();

    bool IsRunning();

    void Release();

private:
    void _loadMap();
    void _handleInput();
    void _update();
    void _updateGameTimer();
    void _draw();
    std::string _vecTosTring(const sf::Vector2f& vec)
    {
        std::string str = "X: ";
        str += std::to_string(vec.x);
        str += " Y: " + std::to_string(vec.y);
        return str;
    }
private:
    bool m_isRunning;
    bool m_terminated;

    sf::RenderWindow* m_wndPtr;

    float m_deltaTime;
    size_t m_frameCounter;
    float m_timer;

    sf::RectangleShape m_selection;
    bool m_drawSelection;

    TeamHandler m_player;
    Container::Vector::Vector<Planet> m_planets;
    Planet* m_planetHover;

    sf::Text m_fps;
    sf::Text m_frameTime;
    sf::Text m_otherInfo;
};


#endif
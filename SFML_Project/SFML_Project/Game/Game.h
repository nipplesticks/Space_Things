#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <string>
#include "Globals.h"
#include "Planet.h"
#include "Unit.h"
#include "TeamHandler.h"
#include "Button.h"

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
    void _setupButtons();
    void _loadMap();
    void _handleInput();
    void _update();
    void _updateGameTimer();
    void _draw();

    void _setHalfSpeed();
    void _setNormalSpeed();
    void _setDoubleSpeed();
    void _setTrippleSpeed();

private:
    bool m_isRunning;
    bool m_terminated;

    sf::RenderWindow* m_wndPtr;

    float m_deltaTime;
    size_t m_frameCounter;
    float m_timer;
    float m_timeChanger;
    bool m_hasSelection;

    sf::RectangleShape m_selection;
    bool m_drawSelection;

    TeamHandler m_player;
    TeamHandler m_enemy;
    Container::Vector::Vector<Planet> m_planets;
    Planet* m_planetHover;

    sf::Text m_fps;
    sf::Text m_frameTime;
    sf::Text m_otherInfo;

    Container::Vector::Vector<Button<void(void)>> m_speedButtons;
};


#endif
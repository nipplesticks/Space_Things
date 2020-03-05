#ifndef  _GAME_STATE_H_
#define _GAME_STATE_H_

#include "State.h"
#include <iostream>
#include <string>
#include "Globals.h"
#include "Planet.h"
#include "Unit.h"
#include "TeamHandler.h"
#include "Button.h"

class GameState : public State
{
public:
    GameState();
    ~GameState();

    virtual void Init() override;
    virtual void Update(float dt, State::Event* e) override;
    virtual void Release() override;
    virtual void Draw(sf::RenderWindow* wnd) override;

private:
    void _setupButtons();
    void _loadMap();
    void _handleInput();
    void _update();

    void _setHalfSpeed();
    void _setNormalSpeed();
    void _setDoubleSpeed();
    void _setTrippleSpeed();

private:
    float m_deltaTime;

    float m_timeChanger;
    bool m_hasSelection;

    sf::RectangleShape m_selection;
    bool m_drawSelection;

    TeamHandler m_player;
    TeamHandler m_enemy;
    Container::Vector::Vector<Planet> m_planets;
    Planet* m_planetHover;

    Container::Vector::Vector<Button<void(void)>> m_speedButtons;

};


#endif // ! _GAME_STATE_H_

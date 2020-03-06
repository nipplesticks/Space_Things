#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <string>
#include "Structures/Globals.h"
#include "States/GameState.h"
#include "States/MenuState.h"
#include "States/PauseState.h"
#include "States/LevelEditorState.h"
#include "Structures/Camera.h"

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
    void _updateGameTimer();
    void _handleStateEvent(State::Event* se);
    void _pushNewState(State::Event* se);
private:
    bool m_isRunning;
    bool m_terminated;

    sf::RenderWindow* m_wndPtr;

    float m_deltaTime;
    size_t m_frameCounter;
    float m_timer;

    Button<void()> m_fps;
    Button<void()> m_frameTime;
    Button<void()> m_otherInfo;

    Container::Vector::Vector<State*> m_stateStack;
};

#endif
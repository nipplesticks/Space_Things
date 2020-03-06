#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include "State.h"
#include "../GUI/Button.h"
#include "../../Utility/vector.h"

class MenuState : public State
{
public:
    MenuState();
    ~MenuState();

    void Init();
    void Update(float dt, Event* e);
    void Release();
    void Draw(sf::RenderWindow* wnd);

private:
    void _startGame();
    void _selectLevel();
    void _levelEditor();
    void _options();
    void _quit();

private:
    Event* m_eventPtr;
    Container::Vector::Vector<Button<void(void)>> m_buttons;
};


#endif
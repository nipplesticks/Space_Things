#ifndef _PAUSE_STATE_H_
#define _PAUSE_STATE_H_

#include "State.h"
#include "../../Utility/vector.h"
#include "../GUI/Button.h"

class PauseState : public State
{
public:
    PauseState();
    ~PauseState();
    void Init();
    void Update(float dt, Event* e);
    void Release();
    void Draw(sf::RenderWindow* wnd);

private:
    void _continue();
    void _options();
    void _quit();
private:
    bool m_EscapePressed;
    Event* m_eventPtr;
    Button<void()> m_background;
    Container::Vector::Vector<Button<void(void)>> m_buttons;
};


#endif
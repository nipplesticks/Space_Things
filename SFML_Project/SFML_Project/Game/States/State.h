#ifndef _STATE_H_
#define _STATE_H_

#include <SFML/Graphics.hpp>

class State
{
public:
    enum StackEvent
    {
        None = 0,
        Game,
        Menu,
        Pause,
        LevelEditor,
        Pop,
        PopPush,
        Push
    };

    struct Event
    {
        State::StackEvent newState = StackEvent::None;
        State::StackEvent stackEvent = StackEvent::None;
        size_t nrOfPops = 1;
    };

public:
    State() {}
    virtual ~State() {};

    virtual void Init() = 0;
    virtual void Update(float dt, Event* e) = 0;
    virtual void Release() = 0;
    virtual void Draw(sf::RenderWindow * wnd) = 0;
    bool DrawStateBelow();

protected:
    bool m_drawBelow = false;
};

#endif
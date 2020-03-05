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
        Pop,
        PopPush,
        Push
    };

    struct Event
    {
        State::StackEvent newState = StackEvent::None;
        State::StackEvent stackEvent = StackEvent::None;
    };

public:
    State() {}
    virtual ~State() {};

    virtual void Init() = 0;
    virtual void Update(float dt, Event* e) = 0;
    virtual void Release() = 0;
    virtual void Draw(sf::RenderWindow * wnd) = 0;

};

#endif
#ifndef _LEVEL_EDITOR_STATE_H_
#define _LEVEL_EDITOR_STATE_H_

#include "State.h"

class LevelEditorState : public State
{
public:
    LevelEditorState();
    ~LevelEditorState();

    void Init() override;
    void Update(float dt, Event* e) override;
    void Release() override;
    void Draw(sf::RenderWindow* wnd) override;
private:

};

#endif
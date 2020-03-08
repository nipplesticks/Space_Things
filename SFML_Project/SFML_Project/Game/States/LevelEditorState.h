#ifndef _LEVEL_EDITOR_STATE_H_
#define _LEVEL_EDITOR_STATE_H_

#include "State.h"
#include "../GameObjects/Planet.h"
#include "../Structures/Camera.h"
#include "../GUI/Button.h"
#include <unordered_map>

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
    void _setupButtons();
    void _handleInput();
    void _save();
    void _load();
    void _quit();

    struct PlanetInfo
    {
        bool IsStartPlanet = false;
        unsigned int UnitCount = 0u;
    };

private:
    Event* m_eventPtr;
    Camera m_camera;
    Container::Vector::Vector<Planet> m_planets;
    std::unordered_map<Planet*, PlanetInfo> m_planetInfo;
    Container::Vector::Vector<Button<void()>> m_buttons;
    sf::RectangleShape m_selection;
    Planet* m_planetPtr;
    bool m_LeftMousePressed;
};

#endif
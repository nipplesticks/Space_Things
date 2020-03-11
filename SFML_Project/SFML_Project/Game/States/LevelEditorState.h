#ifndef _LEVEL_EDITOR_STATE_H_
#define _LEVEL_EDITOR_STATE_H_

#include "State.h"
#include "../GameObjects/Planet.h"
#include "../Structures/Camera.h"
#include "../GUI/Button.h"
#include "../Structures/Console.h"
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
    void _selectionMode();
    void _createPlanetMode();

    void _select();

    struct PlanetInfo
    {
        bool IsStartPlanet = false;
        int UnitCount = 0;
        sf::Vector2f MouseOffset = sf::Vector2f(0.0f,0.0f);
    };

    enum Mode
    {
        SelectionMode,
        CreatePlanetMode,
    };

private:
    void _makePlayer();
    void _makeE1();
    void _makeE2();
    void _makeE3();
    void _makeNetural();

    void _increaseStartUnit();
    void _decreaseStartUnit();

    void _incrementMaxLevel();
    void _decreaseMaxLevel();
    void _increaseLevel();
    void _decreaseLevel();

    void _setStartPlanet();

    void _setSelectionTeam(const std::string& str, const sf::Color& col);
    void _addUnits(int inc);
    void _maxLevel(int inc);
    void _incLevel(int inc);

private:
    Event* m_eventPtr;
    float m_deltaTime;
    Camera m_camera;
    Container::Vector::Vector<Planet> m_planets;
    std::unordered_map<Planet*, PlanetInfo> m_planetInfo;
    Container::Vector::Vector<Button<void()>> m_buttons;
    Container::Vector::Vector<Button<void()>> m_tools;
    Container::Vector::Vector<sf::RectangleShape> m_selections;
    Container::Vector::Vector<Planet*> m_selectedPlanets;
    sf::RectangleShape m_selection;
    Planet* m_planetPtr;
    float m_camSpeed;

    Console m_console;
    bool m_drawSelection;
    bool m_DelPressed;
    bool m_LeftMousePressed;
    Mode m_editMode;
};

#endif
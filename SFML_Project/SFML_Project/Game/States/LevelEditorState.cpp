#include "LevelEditorState.h"
#include "../Structures/Globals.h"
#include <fstream>

using namespace Container::Vector;

LevelEditorState::LevelEditorState()
{
    m_eventPtr = nullptr;
    m_planetPtr = nullptr;
    m_LeftMousePressed = true;
}

LevelEditorState::~LevelEditorState()
{
    m_eventPtr = nullptr;
    m_planetPtr = nullptr;
}

void LevelEditorState::Init()
{
    m_eventPtr = nullptr;
    m_LeftMousePressed = true;
    m_camera.SetPosition(Global::g_windowSize * 0.5f);
    m_planetPtr = nullptr;
    m_planets = Vector<Planet>(100, 0);
    _setupButtons();
}

void LevelEditorState::Update(float dt, Event* e)
{
    m_eventPtr = e;
    _handleInput();
    for (size_t i = 0; i < m_planets.Size(); i++)
        m_planets[i].Update(dt);
    for (size_t i = 0; i < m_buttons.Size(); i++)
        m_buttons[i].Update(dt);
}

void LevelEditorState::Release()
{
    m_eventPtr = nullptr;
}


void LevelEditorState::Draw(sf::RenderWindow* wnd)
{
    for (size_t i = 0; i < m_planets.Size(); i++)
    {
        m_planets[i].Draw(wnd);
        m_planets[i].DrawInfo(wnd);
    }
    for (size_t i = 0; i < m_buttons.Size(); i++)
    {
        m_buttons[i].Draw(wnd);
    }
}

void LevelEditorState::_setupButtons()
{
    m_buttons = Vector<Button<void(void)>>(3, 3);
    float size = 32.0f;
    std::function<void(void)> funcs[] = {
        std::bind(&LevelEditorState::_save, this),
        std::bind(&LevelEditorState::_load, this),
        std::bind(&LevelEditorState::_quit, this)
    };

    std::string str[] = {
        "S",
        "L",
        "Q"
    };

    for (size_t i = 0; i < m_buttons.Size(); i++)
    {
        m_buttons[i].SetPosition(Global::g_windowSize.x - (size + 2) * i, Global::g_windowSize.y);
        m_buttons[i].SetSize(size, size);
        m_buttons[i].SetOrigin(1.0f, 1.0f);
        m_buttons[i].RegisterFunction(funcs[i]);
        m_buttons[i].SetTextString(str[i]);
        m_buttons[i].SetTextOrigin(1.5f, 1.5f);
    }
}

void LevelEditorState::_handleInput()
{
    bool LeftMousePress = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (LeftMousePress && !m_LeftMousePressed)
    {
        Planet p;
        p.SetPosition(Global::g_mousePos);
        m_planets.PushBack(p);
        m_planetInfo.insert(std::make_pair(&m_planets.Back(), PlanetInfo()));
    }

    m_LeftMousePressed = LeftMousePress;
}

void LevelEditorState::_save()
{
    std::ofstream f;
    f.open("save.map");
    for (size_t i = 0; i < m_planets.Size(); i++)
    {
        f << "[Planet]\n";
        f << "Position " << m_planets[i].GetPosition().x << " " << m_planets[i].GetPosition().y << "\n";
        f << "Team " << m_planets[i].GetTeam() << "\n";
        f << "MaxLevel " << m_planets[i].GetMaxLevel();
        f << "StartLevel " << m_planets[i].GetCurrentLevel();
        f << "StartPlanet " << m_planetInfo[&m_planets[i]].IsStartPlanet;
        f << "UnitCount " << m_planetInfo[&m_planets[i]].UnitCount;
    }
    f.close();
}

void LevelEditorState::_load()
{
}

void LevelEditorState::_quit()
{
    m_eventPtr->stackEvent = Pop;
}

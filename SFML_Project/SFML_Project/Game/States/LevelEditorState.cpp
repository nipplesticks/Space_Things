#include "LevelEditorState.h"
#include "../Structures/Globals.h"
#include <fstream>

using namespace Container::Vector;

LevelEditorState::LevelEditorState()
{
    m_eventPtr = nullptr;
    m_planetPtr = nullptr;
    m_LeftMousePressed = true;
    m_drawSelection = false;
    m_DelPressed = true;
    m_deltaTime = 0.0f;
    m_camSpeed = 128.0f;
}

LevelEditorState::~LevelEditorState()
{
    m_eventPtr = nullptr;
    m_planetPtr = nullptr;
}

void LevelEditorState::Init()
{
    m_console.SetPosition(0.0f, 0.0f);
    m_console.SetSize(320.0f, 100.0f);
    m_console.SetCharacterSize(16);
    m_drawSelection = false;
    m_eventPtr = nullptr;
    m_LeftMousePressed = true;
    m_DelPressed = true;
    m_camera.SetPosition(Global::g_windowSize * 0.5f);
    m_planetPtr = nullptr;
    m_planets = Vector<Planet>(999, 0);
    m_selections = Vector<sf::RectangleShape>(999, 0);
    m_selectedPlanets = Vector<Planet*>(999, 0);
    m_camSpeed = 128.0f;
    m_camera.SetAsActive();
    m_selection.setFillColor(sf::Color::Transparent);
    m_selection.setOutlineThickness(2);
    m_selection.setOutlineColor(sf::Color::White);
    _setupButtons();
}

void LevelEditorState::Update(float dt, Event* e)
{
    m_eventPtr = e;
    m_deltaTime = dt;
    
    bool buttonPress = false;

    for (size_t i = 0; i < m_buttons.Size(); i++)
        buttonPress = m_buttons[i].Update(dt) || buttonPress;
    for (size_t i = 0; i < m_tools.Size(); i++)
        buttonPress = m_tools[i].Update(dt) || buttonPress;

    if (!buttonPress)
        _handleInput();

    for (size_t i = 0; i < m_planets.Size(); i++)
        m_planets[i].Update(dt);

    for (size_t j = 0; j < m_selectedPlanets.Size(); j++)
    {
        sf::Vector2f pos = m_selectedPlanets[j]->GetPosition();
        float r = m_selectedPlanets[j]->GetRadius();
        m_selections[j].setPosition(pos);
        m_selections[j].setSize(sf::Vector2f(r * 2.0f, r * 2.0f));
        m_selections[j].setOrigin(sf::Vector2f(r, r));
    }

    m_console.Update(m_deltaTime);
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
    for (size_t i = 0; i < m_tools.Size(); i++)
    {
        m_tools[i].Draw(wnd);
    }
    if (m_drawSelection)
        wnd->draw(m_selection);

    for (size_t i = 0; i < m_selections.Size(); i++)
    {
        m_selections[i].setOutlineThickness(m_camera.GetZoom());
        wnd->draw(m_selections[i]);
    }

    m_console.Draw(wnd);
}

void LevelEditorState::_setupButtons()
{
    {
        m_buttons = Vector<Button<void(void)>>(5, 5);
        float size = 32.0f;
        std::function<void(void)> funcs[] = {
            std::bind(&LevelEditorState::_save, this),
            std::bind(&LevelEditorState::_load, this),
            std::bind(&LevelEditorState::_quit, this),
            std::bind(&LevelEditorState::_selectionMode, this),
            std::bind(&LevelEditorState::_createPlanetMode, this)
        };

        std::string str[] = {
            "Sa",
            "Lo",
            "Qu",
            "Se",
            "Cr"
        };

        for (size_t i = 0; i < m_buttons.Size(); i++)
        {
            m_buttons[i].SetPosition(Global::g_windowSize.x - (size + 2) * i, Global::g_windowSize.y);
            m_buttons[i].SetSize(size, size);
            m_buttons[i].SetOrigin(1.0f, 1.0f);
            m_buttons[i].RegisterFunction(funcs[i]);
            m_buttons[i].SetTextString(str[i]);
            m_buttons[i].SetTextOrigin(1.25f, 1.25f);
        }
    }
    {
        m_tools = Vector<Button<void(void)>>(12, 12);
        float size = 32.0f;
        std::function<void(void)> funcs[] = {
            std::bind(&LevelEditorState::_makePlayer, this),
            std::bind(&LevelEditorState::_makeE1, this),
            std::bind(&LevelEditorState::_makeE2, this),
            std::bind(&LevelEditorState::_makeE3, this),
            std::bind(&LevelEditorState::_makeNetural, this),
            std::bind(&LevelEditorState::_increaseStartUnit, this),
            std::bind(&LevelEditorState::_decreaseStartUnit, this),
            std::bind(&LevelEditorState::_incrementMaxLevel, this),
            std::bind(&LevelEditorState::_decreaseMaxLevel, this),
            std::bind(&LevelEditorState::_increaseLevel, this),
            std::bind(&LevelEditorState::_decreaseLevel, this),
            std::bind(&LevelEditorState::_setStartPlanet, this),
        };

        std::string str[] = {
            "mPl",
            "mE1",
            "mE2",
            "mE3",
            "mNe",
            "+Un",
            "-Un",
            "+ML",
            "-ML",
            "+CL",
            "-CL",
            "set"
        };

        for (size_t i = 0; i < m_tools.Size(); i++)
        {
            m_tools[i].SetPosition(Global::g_windowSize.x - (size * 1.5f + 2) * i, 0.0f);
            m_tools[i].SetSize(size * 1.5f, size);
            m_tools[i].SetOrigin(1.0f, 0.0f);
            m_tools[i].RegisterFunction(funcs[i]);
            m_tools[i].SetTextString(str[i]);
            m_tools[i].SetTextOrigin(1.0f, 0.0f);
        }
    }
}

void LevelEditorState::_handleInput()
{
    bool LeftMousePress = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::S);

    bool zoomIn = sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp);
    bool zoomOut = sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown);

    if (zoomIn)
        m_camera.MultiplyZoom(1.0 - m_deltaTime);
    if (zoomOut)
        m_camera.MultiplyZoom(1.0 + m_deltaTime);

    bool delPress = sf::Keyboard::isKeyPressed(sf::Keyboard::Delete);

    sf::Vector2f translation(0, 0);
    translation.x -= (float)moveLeft * m_camSpeed * m_deltaTime;
    translation.x += (float)moveRight * m_camSpeed * m_deltaTime;
    translation.y -= (float)moveUp * m_camSpeed * m_deltaTime;
    translation.y += (float)moveDown * m_camSpeed * m_deltaTime;

    m_camera.Move(translation, true);
    m_drawSelection = false;
    
    switch (m_editMode)
    {
    case LevelEditorState::SelectionMode:
    {
        if (LeftMousePress && !m_LeftMousePressed)
        {
            m_console.Clear();
            m_planetPtr = nullptr;
            m_selection.setPosition(Global::g_mousePos);
            for (size_t i = 0; i < m_planets.Size(); i++)
            {
                if (m_planets[i].Contains(Global::g_mousePos))
                {
                    m_planetPtr = &m_planets[i];
                    
                    bool isSelected = false;
                    for (size_t j = 0; j < m_selectedPlanets.Size(); j++)
                    {
                        if (m_planetPtr == m_selectedPlanets[j])
                        {
                            isSelected = true;
                            break;
                        }
                    }

                    if (!isSelected)
                    {
                        m_selectedPlanets.Clear();
                        m_selectedPlanets.PushBack(m_planetPtr);
                    }
                    
                    for (size_t j = 0; j < m_selectedPlanets.Size(); j++)
                    {
                        sf::Vector2f offset = m_selectedPlanets[j]->GetPosition() - Global::g_mousePos;
                        m_planetInfo[m_selectedPlanets[j]].MouseOffset = offset;
                    }

                    break;
                }
            }
            if (m_planetPtr)
            {
                m_console.Clear();
                m_console.PushString("Team: " + m_planetPtr->GetTeam());
                m_console.PushString("CurrentLevel: " + std::to_string(m_planetPtr->GetCurrentLevel()));
                m_console.PushString("MaxLevel: " + std::to_string(m_planetPtr->GetMaxLevel()));
                m_console.PushString("StartPlanet: " + std::to_string(m_planetInfo[m_planetPtr].IsStartPlanet));
                m_console.PushString("StartUnitCount: " + std::to_string(m_planetInfo[m_planetPtr].UnitCount));
            }
        }
        if (LeftMousePress)
        {
            if (!m_planetPtr)
            {
                m_selection.setSize(Global::g_mousePos - m_selection.getPosition());
                m_selection.setOutlineThickness(m_camera.GetZoom());
                m_drawSelection = true;
            }
            else
            {
                for (size_t j = 0; j < m_selectedPlanets.Size(); j++)
                {
                    sf::Vector2f offset = m_planetInfo[m_selectedPlanets[j]].MouseOffset;
                    m_selectedPlanets[j]->SetPosition(Global::g_mousePos + offset);
                }
            }
        }
        if (!LeftMousePress && m_LeftMousePressed)
        {
            if (!m_planetPtr)
                _select();
        }
        break;
    }
    case LevelEditorState::CreatePlanetMode:
    {
        if (LeftMousePress && !m_LeftMousePressed)
        {
            m_console.Clear();
            m_planetPtr = nullptr;
            Planet p;
            p.SetPosition(Global::g_mousePos);
            m_planets.PushBack(p);
            m_planetInfo.insert(std::make_pair(&m_planets.Back(), PlanetInfo()));
            m_planetPtr = &m_planets.Back();
            m_planetPtr->Update(m_deltaTime);
            if (m_planetPtr)
            {
                m_console.PushString("Team: " + m_planetPtr->GetTeam());
                m_console.PushString("CurrentLevel: " + std::to_string(m_planetPtr->GetCurrentLevel()));
                m_console.PushString("MaxLevel: " + std::to_string(m_planetPtr->GetMaxLevel()));
                m_console.PushString("StartPlanet: " + std::to_string(m_planetInfo[m_planetPtr].IsStartPlanet));
                m_console.PushString("StartUnitCount: " + std::to_string(m_planetInfo[m_planetPtr].UnitCount));
            }
        }
        if (m_planetPtr && LeftMousePress)
        {
            m_planetPtr->SetPosition(Global::g_mousePos + m_planetInfo[m_planetPtr].MouseOffset);
        }
        break;
    }
    default:
        break;
    }

    
    if (delPress && !m_DelPressed && !m_selectedPlanets.Empty())
    {
        int size = m_selectedPlanets.Size() - 1;
        for (int i = size; i >= 0; i--)
        {
            m_planetInfo.erase(m_selectedPlanets[i]);
            for (size_t j = 0; j < m_planets.Size(); j++)
            {
                if (&m_planets[j] == m_selectedPlanets[i])
                {
                    m_planets.Erase(j);
                    break;
                }
            }
            m_selectedPlanets.Erase(i);
            m_selections.Erase(i);
        }

        m_planetPtr = nullptr;
    }

    if (m_planetPtr)
    {
        m_console.Clear();
        m_console.PushString("Team: " + m_planetPtr->GetTeam());
        m_console.PushString("CurrentLevel: " + std::to_string(m_planetPtr->GetCurrentLevel()));
        m_console.PushString("MaxLevel: " + std::to_string(m_planetPtr->GetMaxLevel()));
        m_console.PushString("StartPlanet: " + std::to_string(m_planetInfo[m_planetPtr].IsStartPlanet));
        m_console.PushString("StartUnitCount: " + std::to_string(m_planetInfo[m_planetPtr].UnitCount));
    }

    m_DelPressed = delPress;
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
        f << "MaxLevel " << m_planets[i].GetMaxLevel() << "\n";
        f << "CurrentLevel " << m_planets[i].GetCurrentLevel() << "\n";
        f << "StartLevel " << m_planets[i].GetCurrentLevel() << "\n";
        f << "StartPlanet " << m_planetInfo[&m_planets[i]].IsStartPlanet << "\n";
        f << "UnitCount " << m_planetInfo[&m_planets[i]].UnitCount << "\n";
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

void LevelEditorState::_selectionMode()
{
    m_editMode = SelectionMode;
}

void LevelEditorState::_createPlanetMode()
{
    m_editMode = CreatePlanetMode;
}

void LevelEditorState::_select()
{
    m_selectedPlanets.Clear();
    m_selections.Clear();
    sf::Vector2f aabbPos = m_selection.getPosition();
    sf::Vector2f aabbSize = m_selection.getSize();
    
    if (aabbSize.x < 0.0f)
    {
        aabbSize.x *= -1.0f;
        aabbPos.x -= aabbSize.x;
    }
    if (aabbSize.y < 0.0f)
    {
        aabbSize.y *= -1.0f;
        aabbPos.y -= aabbSize.y;
    }

    sf::Vector2f center;
    float rad;
    float dx, dy;

    for (size_t i = 0; i < m_planets.Size(); i++)
    {
        rad = m_planets[i].GetRadius();
        center = m_planets[i].GetPosition();
        dx = center.x - std::max(aabbPos.x, std::min(center.x, aabbPos.x + aabbSize.x));
        dy = center.y - std::max(aabbPos.y, std::min(center.y, aabbPos.y + aabbSize.y));
        if ((dx * dx + dy * dy) < (rad * rad))
        {
            m_selectedPlanets.PushBack(&m_planets[i]);
            m_selections.PushBack(m_selection);

            sf::Vector2f pos = m_selectedPlanets.Back()->GetPosition();
            float r = m_selectedPlanets.Back()->GetRadius();
            m_selections.Back().setPosition(pos);
            m_selections.Back().setSize(sf::Vector2f(r * 2.0f, r * 2.0f));
            m_selections.Back().setOrigin(sf::Vector2f(r, r));
        }
    }
}

void LevelEditorState::_makePlayer()
{
    _setSelectionTeam("Player", sf::Color::Green);
}

void LevelEditorState::_makeE1()
{
    _setSelectionTeam("Enemy1", sf::Color::Red);
}

void LevelEditorState::_makeE2()
{
    _setSelectionTeam("Enemy2", sf::Color::Magenta);
}

void LevelEditorState::_makeE3()
{
    _setSelectionTeam("Enemy2", sf::Color::Yellow);
}

void LevelEditorState::_makeNetural()
{
    _setSelectionTeam("", sf::Color(128,128,128));
}

void LevelEditorState::_increaseStartUnit()
{
    _addUnits(10);
}

void LevelEditorState::_decreaseStartUnit()
{
    _addUnits(-10);
}

void LevelEditorState::_incrementMaxLevel()
{
    _maxLevel(1);
}

void LevelEditorState::_decreaseMaxLevel()
{
    _maxLevel(-1);
}

void LevelEditorState::_increaseLevel()
{
    _incLevel(1);
}

void LevelEditorState::_decreaseLevel()
{
    _incLevel(-1);
}

void LevelEditorState::_setStartPlanet()
{
    if (m_planetPtr)
    {
        m_planetInfo[m_planetPtr].IsStartPlanet = true;
    }
}

void LevelEditorState::_setSelectionTeam(const std::string& str, const sf::Color& col)
{
    for (size_t i = 0; i < m_selectedPlanets.Size(); i++)
    {
        if (m_selectedPlanets[i]->GetCurrentLevel() < 1)
            m_selectedPlanets[i]->SetCurrentLevel(1);
        m_selectedPlanets[i]->SetColor(col);
        m_selectedPlanets[i]->SetTeam(str);
    }
}

void LevelEditorState::_addUnits(int inc)
{
    for (size_t i = 0; i < m_selectedPlanets.Size(); i++)
    {
        Planet* key = m_selectedPlanets[i];
        m_planetInfo[key].UnitCount += inc;
        if (m_planetInfo[key].UnitCount < 0)
            m_planetInfo[key].UnitCount = 0;
    }
}

void LevelEditorState::_maxLevel(int inc)
{
    for (size_t i = 0; i < m_selectedPlanets.Size(); i++)
    {
        Planet* key = m_selectedPlanets[i];
        key->SetMaxLevel(key->GetMaxLevel() + inc);
        if (key->GetMaxLevel() < 1)
            key->SetMaxLevel(1);
    }
}

void LevelEditorState::_incLevel(int inc)
{
    for (size_t i = 0; i < m_selectedPlanets.Size(); i++)
    {
        Planet* key = m_selectedPlanets[i];
        key->SetCurrentLevel(key->GetCurrentLevel() + inc);
    }

    if (!m_selectedPlanets.Empty() && m_planetPtr)
    {
        Planet* key = m_planetPtr;
        key->SetCurrentLevel(key->GetCurrentLevel() + inc);
    }
}

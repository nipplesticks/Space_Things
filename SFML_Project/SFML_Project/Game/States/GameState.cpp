#include "GameState.h"

using namespace Container::Vector;

GameState::GameState()
{
    m_timeChanger = 1.0f;
    m_drawSelection = false;
    m_planetHover = nullptr;
    m_deltaTime = 0.0f;
}

GameState::~GameState()
{
}

void GameState::Init()
{
    m_drawSelection = false;
    m_deltaTime = 0.0f;
    m_selection.setFillColor(sf::Color::Transparent);
    m_selection.setOutlineThickness(1);
    m_selection.setOutlineColor(sf::Color::White);
    m_planetHover = nullptr;
    m_timeChanger = 1.0f;
    m_hasSelection = false;

    _setupButtons();
    _loadMap();
}

void GameState::Update(float dt, State::Event* e)
{
    m_gameCamera.SetAsActive();
    bool pop = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
    if (pop)
    {
        e->stackEvent = State::Push;
        e->newState = State::Pause;
    }

    m_deltaTime = dt;
    m_deltaTime *= m_timeChanger;
    _handleInput();
    _update();
}

void GameState::Release()
{
}

void GameState::Draw(sf::RenderWindow* wnd)
{
    for (size_t i = 0; i < m_planets.Size(); i++)
        m_planets[i].Draw(wnd);

    m_player.Draw(wnd);
    m_enemy.Draw(wnd);

    if (m_planetHover)
        m_planetHover->DrawInfo(wnd);

    if (m_drawSelection)
        wnd->draw(m_selection);

    //m_enemy.DrawInfo(wnd);
    m_player.DrawInfo(wnd);

    for (size_t i = 0; i < m_speedButtons.Size(); i++)
        m_speedButtons[i].Draw(wnd);
}

void GameState::_setupButtons()
{
    m_speedButtons = Vector<Button<void(void)>>(4, 4);
    float size = 32.0f;
    float pos = Global::g_windowSize.y - size;

    std::function<void(void)> funcs[] = {
        std::bind(&GameState::_setHalfSpeed, this),
        std::bind(&GameState::_setNormalSpeed, this),
        std::bind(&GameState::_setDoubleSpeed, this),
        std::bind(&GameState::_setTrippleSpeed, this)
    };
    std::string str[] = { "H", "N", "D", "T" };
    for (size_t i = 0; i < m_speedButtons.Size(); i++)
    {
        m_speedButtons[i].SetSize(size, size);
        m_speedButtons[i].SetPosition(i * size + 1, pos);
        m_speedButtons[i].RegisterFunction(funcs[i]);
        m_speedButtons[i].SetTextString(str[i]);
        m_speedButtons[i].SetTextOrigin(-8.0f, 0.0f);
    }
}

void GameState::_loadMap()
{
    m_planets = Vector<Planet>(5, 5);
    Vector<Planet*> planetsPtr(m_planets.Size());

    for (size_t i = 0; i < m_planets.Size(); i++)
    {
        m_planets[i].SetMaxLevel(4);
        m_planets[i].SetCurrentLevel(0);
        sf::Vector2f position =
            sf::Vector2f((float)(rand() % ((int)Global::g_windowSize.x - 200) + 100),
            (float)(rand() % ((int)Global::g_windowSize.y - 200) + 100));
        m_planets[i].SetPosition(position);
        planetsPtr.PushBack(&m_planets[i]);
    }
    m_planets[0].SetTeam("Player");
    m_planets[0].SetColor(sf::Color::Green);
    m_planets[0].SetCurrentLevel(1);

    m_planets[planetsPtr.Size() - 1].SetTeam("Enemy");
    m_planets[planetsPtr.Size() - 1].SetColor(sf::Color::Red);
    m_planets[planetsPtr.Size() - 1].SetCurrentLevel(1);


    m_player.SetPlanetPointers(planetsPtr);
    m_player.SetNameAndTeamColor("Player", sf::Color::Green);
    m_player.SetStartPlanet(0);

    m_enemy.SetPlanetPointers(planetsPtr);
    m_enemy.SetNameAndTeamColor("Enemy", sf::Color::Red);
    m_enemy.SetStartPlanet(planetsPtr.Size() - 1);
}

void GameState::_handleInput()
{
    static bool Left_Mouse_Pressed = false;
    static bool Right_Mouse_Pressed = false;

    bool lMousePress = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool rMousePress = sf::Mouse::isButtonPressed(sf::Mouse::Right);

    bool lPress = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool rPress = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    bool uPress = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool dPress = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

    bool pguPress = sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp);
    bool pgdPress = sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown);

    bool homePress = sf::Keyboard::isKeyPressed(sf::Keyboard::Home);
    bool endPress = sf::Keyboard::isKeyPressed(sf::Keyboard::End);

    sf::Vector2f offset(.0f, .0f);
    float rot = 0.0f;
    float zoom = 1.0f;

    if (lPress)
        offset.x -= m_deltaTime * m_cameraSpeed;
    if (rPress)
        offset.x += m_deltaTime * m_cameraSpeed;
    if (uPress)
        offset.y -= m_deltaTime * m_cameraSpeed;
    if (dPress)
        offset.y += m_deltaTime * m_cameraSpeed;

    if (pguPress)
        zoom += m_zoomSpeed * m_deltaTime;
    if (pgdPress)
        zoom -= m_zoomSpeed * m_deltaTime;

    if (homePress)
        rot += m_rotSpeed * m_deltaTime;
    if (endPress)
        rot -= m_rotSpeed * m_deltaTime;

    m_gameCamera.Move(offset, true);
    m_gameCamera.Rotate(rot);
    m_gameCamera.MultiplyZoom(zoom);

    if (lMousePress && !Left_Mouse_Pressed)
    {
        m_drawSelection = false;
        m_hasSelection = false;
        m_selection.setPosition(Global::g_mousePos);
    }
    else if (lMousePress)
    {
        m_selection.setSize(Global::g_mousePos - m_selection.getPosition());
        m_drawSelection = true;
        m_hasSelection = true;
    }

    m_planetHover = nullptr;
    for (size_t i = 0; i < m_planets.Size(); i++)
    {
        if (m_planets[i].Contains(Global::g_mousePos))
        {
            m_planetHover = &m_planets[i];
            break;
        }
    }

    if (rMousePress)
    {
        if (m_hasSelection)
        {
            m_player.Select(m_selection.getPosition(), m_selection.getPosition() + m_selection.getSize());
            m_hasSelection = false;
            m_drawSelection = false;
        }
        m_player.SetDestination(Global::g_mousePos, m_planetHover);
    }

    Left_Mouse_Pressed = lMousePress;
    Right_Mouse_Pressed = rMousePress;
}

void GameState::_update()
{
    for (size_t i = 0; i < m_speedButtons.Size(); i++)
        m_speedButtons[i].Update(m_deltaTime);

    m_player.Update(m_deltaTime);
    m_enemy.Update(m_deltaTime);
    for (size_t i = 0; i < m_planets.Size(); i++)
        m_planets[i].Update(m_deltaTime);

    Global::g_unitQuadtree.Clear(); // This must be at the end!
}

void GameState::_setHalfSpeed()
{
    m_timeChanger = 0.5f;
}

void GameState::_setNormalSpeed()
{
    m_timeChanger = 1.0f;
}

void GameState::_setDoubleSpeed()
{
    m_timeChanger = 2.0f;
}

void GameState::_setTrippleSpeed()
{
    m_timeChanger = 3.0f;
}

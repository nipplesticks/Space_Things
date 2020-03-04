#include "Game.h"
#include "Globals.h"

sf::Font Global::g_font;
sf::Vector2f Global::g_mousePos = sf::Vector2f(0, 0);
QuadTree<Unit*> Global::g_unitQuadtree;

using namespace Container::Vector;

Game::Game()
{
    m_isRunning = false;
    m_terminated = false;
    m_wndPtr = nullptr;
    m_deltaTime = 0.0f;
    m_frameCounter = 0;
    m_timer = 0.0f;
    m_timeChanger = 1.0f;
    m_drawSelection = false;
    m_planetHover = nullptr;
    m_fps.setCharacterSize(16);
    m_otherInfo.setCharacterSize(16);
    m_frameTime.setCharacterSize(16);
}

Game::~Game()
{
    
}

void Game::Init()
{
    Global::g_font.loadFromFile("AGENCYB.TTF");
    Global::g_unitQuadtree.Init(sf::Vector2f(-5000, -5000), sf::Vector2f(5000, 5000), 10U);
    Planet::MAX_INCREMENT_LEVEL = 100;

    m_fps.setFont(Global::g_font);
    m_fps.setFillColor(sf::Color::White);
    m_frameTime.setFont(Global::g_font);
    m_frameTime.setFillColor(sf::Color::White);
    m_frameTime.setPosition(0.0f, m_fps.getCharacterSize() + 2);
    m_otherInfo = m_frameTime;
    m_isRunning = false;
    m_terminated = false;
    m_drawSelection = false;
    m_deltaTime = 0.0f;
    m_frameCounter = 0;
    m_timer = 0.0f;
    m_selection.setFillColor(sf::Color::Transparent);
    m_selection.setOutlineThickness(1);
    m_selection.setOutlineColor(sf::Color::White);
    m_planetHover = nullptr;
    m_timeChanger = 1.0f;

    _setupButtons();
}

void Game::Run(sf::RenderWindow* wnd)
{
    srand(static_cast<unsigned>(time(NULL)));
    wnd->setActive(true);
    m_wndPtr = wnd;

    _loadMap();

    m_isRunning = true;
    Timer gameTimer;
    gameTimer.Start();
    while (m_isRunning && !m_terminated)
    {
        m_frameCounter++;
        m_deltaTime = (float)gameTimer.Stop();
        m_timer += m_deltaTime;
        m_deltaTime *= m_timeChanger;

        if (m_wndPtr->isOpen() && m_wndPtr->hasFocus())
        {
            _handleInput();
            _update();
            _updateGameTimer();
            _draw();
        }
    }

    m_isRunning = false;
}

void Game::Terminate()
{
    m_terminated = true;
}

bool Game::IsRunning()
{
    return m_isRunning;
}

void Game::Release()
{
    Global::g_unitQuadtree.Release();
}

void Game::_setupButtons()
{
    m_speedButtons = Vector<Button<void(void)>>(4, 4);
    float size = 32.0f;
    float pos = Global::g_windowSize.y - size;

    std::function<void(void)> funcs[] = {
        std::bind(&Game::_setHalfSpeed, this),
        std::bind(&Game::_setNormalSpeed, this),
        std::bind(&Game::_setDoubleSpeed, this),
        std::bind(&Game::_setTrippleSpeed, this)
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

void Game::_loadMap()
{
    m_planets = Vector<Planet>(5, 5);
    Vector<Planet*> planetsPtr(m_planets.Size());

    for (size_t i = 0; i < m_planets.Size(); i++)
    {
        m_planets[i].SetMaxLevel(4);
        m_planets[i].SetCurrentLevel(0);
        sf::Vector2f position = sf::Vector2f((float)(rand() % 1080 + 100), (float)(rand() % 620 + 100));
        m_planets[i].SetPosition(position);
        planetsPtr.PushBack(&m_planets[i]);
    }
    m_planets[0].SetTeam("Player");
    m_planets[0].SetColor(sf::Color::Green);
    m_planets[0].SetCurrentLevel(1);

    m_player.SetPlanetPointers(planetsPtr);
    m_player.SetNameAndTeamColor("Player", sf::Color::Green);
    m_player.SetStartPlanet(0);
}

void Game::_handleInput()
{
    static bool Left_Mouse_Pressed = false;
    static bool Right_Mouse_Pressed = false;
    
    Global::g_mousePos = (sf::Vector2f)sf::Mouse::getPosition(*m_wndPtr); // Should be at the top section


    bool lMousePress = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool rMousePress = sf::Mouse::isButtonPressed(sf::Mouse::Right);


    m_drawSelection = false;
    if (lMousePress && !Left_Mouse_Pressed)
    {
        m_selection.setPosition(Global::g_mousePos);
    }
    else if (lMousePress)
    {
        m_selection.setSize(Global::g_mousePos - m_selection.getPosition());
        m_drawSelection = true;
    }
    else if (!lMousePress && Left_Mouse_Pressed)
    {
        m_player.Select(m_selection.getPosition(), Global::g_mousePos);
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
        m_player.SetDestination(Global::g_mousePos, m_planetHover);
    }

    Left_Mouse_Pressed = lMousePress;
    Right_Mouse_Pressed = rMousePress;
}

void Game::_update()
{
    for (size_t i = 0; i < m_speedButtons.Size(); i++)
        m_speedButtons[i].Update(m_deltaTime);

    m_player.Update(m_deltaTime);
    for (size_t i = 0; i < m_planets.Size(); i++)
        m_planets[i].Update(m_deltaTime);

    Global::g_unitQuadtree.Clear(); // This must be at the end!
}

void Game::_updateGameTimer()
{
    if (m_timer >= 1.0f)
    {
        m_fps.setString("FPS: " + std::to_string(m_frameCounter));
        m_frameTime.setString(std::to_string((m_timer * 1000.0f) / (float)m_frameCounter) + " ms/frame");
        m_timer -= 1.0f;
        m_frameCounter = 0;
    }
}

void Game::_draw()
{
    m_wndPtr->clear();

    for (size_t i = 0; i < m_planets.Size(); i++)
        m_planets[i].Draw(m_wndPtr);

    m_player.Draw(m_wndPtr);

    if (m_planetHover)
        m_planetHover->DrawInfo(m_wndPtr);

    if (m_drawSelection)
        m_wndPtr->draw(m_selection);

    m_player.DrawInfo(m_wndPtr);

    for (size_t i = 0; i < m_speedButtons.Size(); i++)
        m_speedButtons[i].Draw(m_wndPtr);

    m_wndPtr->draw(m_fps);
    m_wndPtr->draw(m_frameTime);
    m_wndPtr->draw(m_otherInfo);

    m_wndPtr->display();
}

void Game::_setHalfSpeed()
{
    m_timeChanger = 0.5f;
}

void Game::_setNormalSpeed()
{
    m_timeChanger = 1.0f;
}

void Game::_setDoubleSpeed()
{
    m_timeChanger = 2.0f;
}

void Game::_setTrippleSpeed()
{
    m_timeChanger = 3.0f;
}

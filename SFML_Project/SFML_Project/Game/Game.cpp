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
    m_deltaTime = 0.0f;
    m_frameCounter = 0;
    m_timer = 0.0f;

    m_stateStack.PushBack(new GameState());
    m_stateStack.Back()->Init();
}

void Game::Run(sf::RenderWindow* wnd)
{
    srand(static_cast<unsigned>(time(NULL)));
    wnd->setActive(true);
    m_wndPtr = wnd;

    m_isRunning = true;
    Timer gameTimer;
    gameTimer.Start();
    while (m_isRunning && !m_terminated)
    {
        m_frameCounter++;
        m_deltaTime = (float)gameTimer.Stop();
        m_timer += m_deltaTime;

        if (m_wndPtr->isOpen() && m_wndPtr->hasFocus())
        {
            State::Event se;
            Global::g_mousePos = (sf::Vector2f)sf::Mouse::getPosition(*m_wndPtr); // Should be at the top section
            if (!m_stateStack.Empty())
            {
                m_stateStack.Back()->Update(m_deltaTime, &se);
                _handleStateEvent(&se);
                if (!m_stateStack.Empty())
                {
                    m_wndPtr->clear();
                    m_stateStack.Back()->Draw(m_wndPtr);
                    m_wndPtr->draw(m_fps);
                    m_wndPtr->draw(m_frameTime);
                    m_wndPtr->draw(m_otherInfo);
                    m_wndPtr->display();
                }
                _updateGameTimer();
            }
            else
                m_terminated = true;
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
    for (size_t i = 0; i < m_stateStack.Size(); i++)
    {
        m_stateStack[i]->Release();
        delete m_stateStack[i];
        m_stateStack[i] = nullptr;
    }
    m_stateStack.Clear(true);
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

void Game::_handleStateEvent(State::Event* se)
{
    switch (se->stackEvent)
    {
    case State::Pop:
        m_stateStack.Back()->Release();
        delete m_stateStack.Back();
        m_stateStack.Back() = nullptr;
        m_stateStack.PopBack();
        break;
    case State::PopPush:
        m_stateStack.Back()->Release();
        delete m_stateStack.Back();
        m_stateStack.Back() = nullptr;
        m_stateStack.PopBack();
    case State::Push:
        _pushNewState(se);
        break;
    default:

        break;
    }
}

void Game::_pushNewState(State::Event* se)
{
    State* newState = nullptr;

    switch (se->newState)
    {
    case State::Game:
        newState = new GameState();
        newState->Init();
        m_stateStack.PushBack(newState);
        break;
    case State::Menu:
        //newState = new MenuState();
        //newState->Init();
        //m_stateStack.PushBack(newState);
        break;
    default:
        break;
    }
}

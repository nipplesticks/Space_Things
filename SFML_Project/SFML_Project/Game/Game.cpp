#include "Game.h"

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
    m_fps.SetTextSize(16);
    m_otherInfo.SetTextSize(16);
    m_frameTime.SetTextSize(16);
}

Game::~Game()
{
    
}

void Game::Init()
{
    Global::g_unitQuadtree.Init(sf::Vector2f(-5000, -5000), sf::Vector2f(5000, 5000), 10U);
    Planet::MAX_INCREMENT_LEVEL = 100;
    ButtonColor col;
    col.Hover = sf::Color::Transparent;
    col.Idle = sf::Color::Transparent;
    col.Press = sf::Color::Transparent;

    m_fps.SetBackgroundColor(col);
    m_fps.SetForegroundColor(col);
    m_fps.SetTextColor(sf::Color::White);

    m_frameTime.SetBackgroundColor(col);
    m_frameTime.SetForegroundColor(col);
    m_frameTime.SetTextColor(sf::Color::White);
    m_frameTime.SetPosition(0, 20);

    m_otherInfo.SetBackgroundColor(col);
    m_otherInfo.SetForegroundColor(col);
    m_otherInfo.SetTextColor(sf::Color::White);
    m_otherInfo.SetPosition(0, 20 * 2);

    
    m_isRunning = false;
    m_terminated = false;
    m_deltaTime = 0.0f;
    m_frameCounter = 0;
    m_timer = 0.0f;

    m_stateStack.PushBack(new MenuState());
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
            Global::g_mousePos = (sf::Vector2f)sf::Mouse::getPosition(*m_wndPtr); // Should be at the top section

            Camera* c = Camera::GetActiveCamera();

            if (c)
            {
                sf::Vector2f wSize = Global::g_windowSize;
                sf::Vector2f wCenter = wSize * 0.5f;
                Global::g_mousePos = Global::g_mousePos - wCenter; // Screen position from center to mp;
                float rot = Global::DegreeseToRadians(c->GetRotation());
                float x = Global::g_mousePos.x * std::cos(rot) - Global::g_mousePos.y * std::sin(rot);
                float y = Global::g_mousePos.x * std::sin(rot) + Global::g_mousePos.y * std::cos(rot);
                Global::g_mousePos.x = x;
                Global::g_mousePos.y = y;

                sf::Vector2f size = c->GetSize();
                sf::Vector2f pos = c->GetPosition();
                float zoom = c->GetZoom();
                Global::g_mousePos.x = (Global::g_mousePos.x / wSize.x);
                Global::g_mousePos.y = (Global::g_mousePos.y / wSize.y);
                // Rotate
                Global::g_mousePos.x = Global::g_mousePos.x * size.x * zoom;
                Global::g_mousePos.y = Global::g_mousePos.y * size.y * zoom;
                Global::g_mousePos = Global::g_mousePos + pos;

            }

            State::Event se;
            if (!m_stateStack.Empty())
            {
                m_stateStack.Back()->Update(m_deltaTime, &se);
                _handleStateEvent(&se);
                if (!m_stateStack.Empty())
                {
                    m_wndPtr->clear();

                    c = Camera::GetActiveCamera();
                    if (c)
                    {
                        sf::View v = m_wndPtr->getView();
                        sf::Vector2f cPos = c->GetPosition();
                        sf::Vector2f cSiz = c->GetSize();
                        float r = c->GetRotation();
                        float z = c->GetZoom();

                        sf::FloatRect fr;
                        fr.left = cPos.x;
                        fr.top = cPos.y;
                        fr.width = cSiz.x;
                        fr.height = cSiz.y;
                        v.reset(fr);
                        v.setRotation(r);
                        v.zoom(z);
                        v.setCenter(cPos);
                        m_wndPtr->setView(v);
                    }
                    size_t i = m_stateStack.Size() - 1;

                    if (i > 1)
                    {
                        for (int j = (int)i; j > 0; j--)
                        {
                            if (!m_stateStack[j]->DrawStateBelow())
                                break;
                            i--;
                        }
                    }
                    for (;i < m_stateStack.Size(); i++)
                        m_stateStack[i]->Draw(m_wndPtr);

                    m_fps.Update(m_deltaTime);
                    m_frameTime.Update(m_deltaTime);
                    m_otherInfo.Update(m_deltaTime);
                    m_fps.Draw(m_wndPtr);
                    m_frameTime.Draw(m_wndPtr);
                    m_otherInfo.Draw(m_wndPtr);
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
    m_stateStack.Clear();
}

void Game::_updateGameTimer()
{
    if (m_timer >= 1.0f)
    {
        m_fps.SetTextString("FPS: " + std::to_string(m_frameCounter));
        m_frameTime.SetTextString(std::to_string((m_timer * 1000.0f) / (float)m_frameCounter) + " ms/frame");
        m_timer -= 1.0f;
        m_frameCounter = 0;
    }
}

void Game::_handleStateEvent(State::Event* se)
{
    switch (se->stackEvent)
    {
    case State::Pop:
        for (size_t i = 0; i < se->nrOfPops; i++)
        {
            m_stateStack.Back()->Release();
            delete m_stateStack.Back();
            m_stateStack.Back() = nullptr;
            m_stateStack.PopBack();
        }
        break;
    case State::PopPush:
        for (size_t i = 0; i < se->nrOfPops; i++)
        {
            m_stateStack.Back()->Release();
            delete m_stateStack.Back();
            m_stateStack.Back() = nullptr;
            m_stateStack.PopBack();
        }
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
        break;
    case State::Menu:
        newState = new MenuState();
        break;
    case State::Pause:
        newState = new PauseState();
        break;
    default:
        return;
        break;
    }

    newState->Init();
    m_stateStack.PushBack(newState);
}

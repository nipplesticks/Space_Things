#include "PauseState.h"
#include "../Structures/Globals.h"
using namespace Container::Vector;
PauseState::PauseState()
{
    m_eventPtr = nullptr;
    m_drawBelow = true;
    m_EscapePressed = true;
}

PauseState::~PauseState()
{
    m_eventPtr = nullptr;
}

void PauseState::Init()
{
    m_eventPtr = nullptr;
    m_buttons = Vector<Button<void()>>(3, 3);

    float centerX = Global::g_windowSize.x * 0.5f;
    float centerY = Global::g_windowSize.y * 0.5f;

    m_background.SetSize(Global::g_windowSize * 0.5f);
    sf::Color c = sf::Color::Magenta;
    c.a = 64;
    ButtonColor col;
    col.Hover = c;
    col.Idle = c;
    col.Press = c;

    m_background.SetBackgroundColor(col);
    m_background.SetForegroundColor(col);
    m_background.SetPosition(centerX, centerY);
    m_background.SetOrigin(0.5f, 0.5f);
    m_background.SetTextString("Pause");
    m_background.SetTextColor(sf::Color::White);
    m_background.SetTextSize(64);
    m_background.SetTextOrigin(0.5f, 3.0f);
    float startPoint =
        m_background.GetPosition().y - m_background.GetBackgroundSize().y * 0.5f;
    float endPoint = 
        m_background.GetPosition().y + m_background.GetBackgroundSize().y * 0.5f;

    startPoint = startPoint + 150.0f;

    float sizeX = m_background.GetBackgroundSize().x * 0.5f;
    float sizeY = (endPoint - startPoint) / (float)m_buttons.Size();
    float offset = 32.0f * 0.25f;

    std::function<void(void)> funcs[] = {
        std::bind(&PauseState::_continue, this),
        std::bind(&PauseState::_options, this),
        std::bind(&PauseState::_quit, this)
    };

    std::string str[] = {
        "Continue",
        "Options",
        "Quit"
    };

    for (size_t i = 0; i < m_buttons.Size(); i++)
    {
        m_buttons[i].SetPosition(centerX, startPoint + (sizeY + offset) * (float)i);
        m_buttons[i].SetSize(sizeX, sizeY);
        m_buttons[i].SetOrigin(0.5f, 0.5f);
        m_buttons[i].RegisterFunction(funcs[i]);
        m_buttons[i].SetTextString(str[i]);
        m_buttons[i].SetTextOrigin(0.5f, 0.5f);
    }
}

void PauseState::Update(float dt, Event* e)
{
    m_eventPtr = e;
    bool escPress = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

    if (escPress && !m_EscapePressed)
        e->stackEvent = Pop;

    m_EscapePressed = escPress;

    for (size_t i = 0; i < m_buttons.Size(); i++)
        m_buttons[i].Update(dt);
    m_background.Update(dt);
}

void PauseState::Release()
{
    m_eventPtr = nullptr;
}

void PauseState::Draw(sf::RenderWindow* wnd)
{
    m_background.Draw(wnd);

    for (size_t i = 0; i < m_buttons.Size(); i++)
        m_buttons[i].Draw(wnd);
}

void PauseState::_continue()
{
    m_eventPtr->stackEvent = State::Pop;
}

void PauseState::_options()
{
    // TODO
}

void PauseState::_quit()
{
    m_eventPtr->stackEvent = State::Pop;
    m_eventPtr->nrOfPops = 2;
}

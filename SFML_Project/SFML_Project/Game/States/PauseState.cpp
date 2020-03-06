#include "PauseState.h"
#include "../Structures/Globals.h"
using namespace Container::Vector;
PauseState::PauseState()
{
    m_eventPtr = nullptr;
    m_drawBelow = true;
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

    m_background.setSize(Global::g_windowSize * 0.5f);
    sf::Color c = sf::Color::Magenta;
    c.a = 64;
    m_background.setFillColor(c);
    m_background.setPosition(centerX, centerY);
    m_background.setOrigin(Global::g_windowSize * 0.25f);

    m_pause.setFont(Global::g_font);
    m_pause.setString("Pause");
    m_pause.setFillColor(sf::Color::White);
    m_pause.setCharacterSize(64.0f);
    auto fr = m_pause.getGlobalBounds();
    m_pause.setOrigin(sf::Vector2f(
        fr.width * 0.5f, fr.height * 0.5f
    ));
    float startPoint =
        m_background.getPosition().y - m_background.getSize().y * 0.5f;
    float endPoint = 
        m_background.getPosition().y + m_background.getSize().y * 0.5f;

    m_pause.setPosition(centerX,
        startPoint + fr.height
    );

    startPoint = startPoint + fr.height * 3.5f;

    float sizeX = m_background.getSize().x * 0.5f;
    float sizeY = (endPoint - startPoint) / (float)m_buttons.Size();
    float offset = fr.height * 0.25f;

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
        m_buttons[i].SetOrigin(sizeX * 0.5f, sizeY * 0.5f);
        m_buttons[i].RegisterFunction(funcs[i]);
        m_buttons[i].SetTextString(str[i]);
        auto fr = m_buttons[i].GetText().getGlobalBounds();
        m_buttons[i].SetTextOrigin(fr.width * 0.5f, fr.height * 0.5f);
    }
}

void PauseState::Update(float dt, Event* e)
{
    m_eventPtr = e;

    for (size_t i = 0; i < m_buttons.Size(); i++)
        m_buttons[i].Update(dt);
}

void PauseState::Release()
{
    m_eventPtr = nullptr;
}

void PauseState::Draw(sf::RenderWindow* wnd)
{
    wnd->draw(m_background);
    wnd->draw(m_pause);

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

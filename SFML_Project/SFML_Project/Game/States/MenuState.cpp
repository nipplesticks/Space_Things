#include "MenuState.h"
#include "../Structures/Globals.h"

using namespace Container::Vector;

MenuState::MenuState()
{
    m_eventPtr = nullptr;
}

MenuState::~MenuState()
{
}

void MenuState::Init()
{
    m_eventPtr = nullptr;
    m_buttons = Vector<Button<void(void)>>(5, 5);

    float sizeX = 0.5f * Global::g_windowSize.x;
    float sizeY = (0.8f * Global::g_windowSize.y) / (float)m_buttons.Size();
    float xPosition = Global::g_windowSize.x * 0.5f;
    float offset = (0.2 * Global::g_windowSize.y) / (float)m_buttons.Size();

    std::function<void(void)> funcs[] = {
        std::bind(&MenuState::_startGame, this),
        std::bind(&MenuState::_selectLevel, this),
        std::bind(&MenuState::_levelEditor, this),
        std::bind(&MenuState::_options, this),
        std::bind(&MenuState::_quit, this)
    };

    std::string str[] = {
        "Start Game",
        "Select Level",
        "Level Editor",
        "Options",
        "Quit"
    };

    for (size_t i = 0; i < m_buttons.Size(); i++)
    {
        m_buttons[i].SetPosition(xPosition, offset * 2.5f + (sizeY + offset) * (float)i);
        m_buttons[i].SetSize(sizeX, sizeY);
        m_buttons[i].SetOrigin(0.5f, 0.5f);
        m_buttons[i].RegisterFunction(funcs[i]);
        m_buttons[i].SetTextString(str[i]);
        m_buttons[i].SetTextOrigin(0.5f, 0.5f);
    }
}

void MenuState::Update(float dt, Event* e)
{
    m_eventPtr = e;
    for (size_t i = 0; i < m_buttons.Size(); i++)
        m_buttons[i].Update(dt);
}

void MenuState::Release()
{
    m_eventPtr = nullptr;
    m_buttons.Clear();
}

void MenuState::Draw(sf::RenderWindow* wnd)
{
    for (size_t i = 0; i < m_buttons.Size(); i++)
        m_buttons[i].Draw(wnd);
}

void MenuState::_startGame()
{
    m_eventPtr->newState = State::Game;
    m_eventPtr->stackEvent = State::Push;
}

void MenuState::_selectLevel()
{
    
}

void MenuState::_levelEditor()
{

}

void MenuState::_options()
{
}

void MenuState::_quit()
{
    m_eventPtr->stackEvent = State::Pop;
}

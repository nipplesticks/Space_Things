#include "LevelEditorState.h"

LevelEditorState::LevelEditorState()
{
    m_eventPtr = nullptr;
}

LevelEditorState::~LevelEditorState()
{
    m_eventPtr = nullptr;
}

void LevelEditorState::Init()
{
    m_eventPtr = nullptr;
}

void LevelEditorState::Update(float dt, Event* e)
{
    m_eventPtr = e;
}

void LevelEditorState::Release()
{
    m_eventPtr = nullptr;
}


void LevelEditorState::Draw(sf::RenderWindow* wnd)
{

}

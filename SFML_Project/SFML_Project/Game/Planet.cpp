#include "Planet.h"
#include "Globals.h"

int Planet::MAX_INCREMENT_LEVEL = 100;
sf::Color Planet::NEUTRAL_COLOR = sf::Color(128, 128, 128);
float Planet::START_RADIUS = 16.0f;
float Planet::INDICATOR_RADIUS = 8.0f;
unsigned int Planet::FONT_SIZE = 16;
using namespace Container::Vector;

Planet::Planet()
{
    m_col = NEUTRAL_COLOR;
    m_shape.setFillColor(NEUTRAL_COLOR);
    m_shape.setRadius(START_RADIUS);
    m_shape.setOrigin(sf::Vector2f(START_RADIUS, START_RADIUS));

    m_team = "Neutral";
    m_text.setFont(Global::g_font);
    m_text.setString(m_team);
    m_text.setFillColor(NEUTRAL_COLOR);
    m_text.setOutlineThickness(1);
    m_text.setOutlineColor(sf::Color::Black);
    m_text.setCharacterSize(FONT_SIZE);
    
    _updateText();

    m_levelUpCounter = 0;
    m_destructionCounter = 0;
    m_currentLevel = 0;
    m_maxLevel = 1;

    m_levelUpIndicator.SetVertical(true);
    m_destructionIndicator.SetVertical(true);
    m_levelUpIndicator.UseMinMaxColors(true);
    m_destructionIndicator.UseMinMaxColors(true);

    m_destructionIndicator.SetMaxValue((float)MAX_INCREMENT_LEVEL);
    m_destructionIndicator.SetCurrentValue(0);

    m_levelUpIndicator.SetMaxValue((float)MAX_INCREMENT_LEVEL);
    m_levelUpIndicator.SetCurrentValue(0);

    m_destructionIndicator.SetForegroundMinColor(sf::Color::Green);
    m_destructionIndicator.SetForegroundMaxColor(sf::Color::Red);

    _updateIndicators();
}

Planet::~Planet()
{
}

void Planet::SetPosition(float x, float y)
{
    SetPosition({ x, y });
}

void Planet::SetPosition(const sf::Vector2f& position)
{
    m_shape.setPosition(position);
    _updateText();
    _updateLevelIndicator();
    _updateIndicators();
}

void Planet::Move(float x, float y)
{
    Move({ x, y });
}

void Planet::Move(const sf::Vector2f& offset)
{
    SetPosition(GetPosition() + offset);
}

void Planet::SetRadius(float r)
{
    m_shape.setRadius(r);
    m_shape.setOrigin(sf::Vector2f(r, r));

    _updateText();
    _updateLevelIndicator();
    _updateIndicators();
}

void Planet::SetColor(int r, int g, int b, int a)
{
    SetColor(sf::Color(r, g, b, a));
}

void Planet::SetColor(const sf::Color& color)
{
    m_col = color;
    m_shape.setFillColor(color);
    m_text.setFillColor(color);
    for (int i = 1; i < m_currentLevel; i++)
    {
        m_levelIndicator[i - 1].setFillColor(color);
    }
    _updateFromLevel();
    _updateText();
    _updateLevelIndicator();
}

void Planet::SetTeam(const std::string& team)
{
    if (team == "")
        m_team = "Neutral";
    else
        m_team = team;
    m_text.setString(m_team);
    _updateFromLevel();
    _updateText();
    _updateLevelIndicator();
}

void Planet::SetMaxLevel(int maxLevel)
{
    m_maxLevel = maxLevel;
    m_levelIndicator = Vector<sf::CircleShape>(m_maxLevel - 1, m_maxLevel - 1);
    _updateFromLevel();
    _updateText();
    _updateLevelIndicator();
}

void Planet::IncrementToNextLevel(int inc)
{
    if (m_destructionCounter > 0)
        m_destructionCounter -= inc;
    else if (m_levelUpCounter < MAX_INCREMENT_LEVEL)
        m_levelUpCounter += inc;

    if (m_destructionCounter < 0)
    {
        int delta = m_destructionCounter * -1;
        m_levelUpCounter += delta;
        m_destructionCounter = 0;
    }

    if (m_levelUpCounter >= MAX_INCREMENT_LEVEL)
    {
        int delta = m_levelUpCounter - MAX_INCREMENT_LEVEL;
        if (m_currentLevel < m_maxLevel)
            m_currentLevel++;
        _updateFromLevel();
        _updateText();
        _updateLevelIndicator();
        m_levelUpCounter = 0;
        m_levelUpCounter += delta;
    }
    if (m_maxLevel == m_currentLevel)
        m_levelUpCounter = 0;
    _updateIndicators();
}

void Planet::SetCurrentLevel(int level)
{
    m_currentLevel = level;
    m_currentLevel = m_currentLevel <= m_maxLevel
        ? m_currentLevel : m_maxLevel;

    if (m_currentLevel == 0)
    {
        SetTeam("");
        SetColor(NEUTRAL_COLOR);
    }

    _updateFromLevel();
    _updateText();
    _updateLevelIndicator();
    _updateIndicators();
}

void Planet::IncrementDestruction(int inc)
{
    if (m_currentLevel == 0)
    {
        if (m_levelUpCounter > 0)
        {
            m_levelUpCounter -= inc;
            m_levelUpCounter = m_levelUpCounter < 0 ? 0 : m_levelUpCounter;
        }
    }
    else
    {
        m_destructionCounter += inc;
        if (m_destructionCounter >= MAX_INCREMENT_LEVEL)
        {
            m_currentLevel = 0;
            m_levelUpCounter = 0;
            m_destructionCounter = 0;
            SetTeam("");
            SetColor(NEUTRAL_COLOR);
            _updateFromLevel();
            _updateText();
            _updateLevelIndicator();
        }
    }
    
    _updateIndicators();
}

bool Planet::Contains(float x, float y)
{
    return Contains({ x, y });
}

bool Planet::Contains(const sf::Vector2f& point) const
{
    sf::Vector2f pos = GetPosition();
    sf::Vector2f dir = point - pos;
    float l = sqrt(dir.x * dir.x + dir.y * dir.y);

    return l <= (GetRadius() * 1.1f);
}

const sf::Vector2f& Planet::GetPosition() const
{
    return m_shape.getPosition();
}

float Planet::GetRadius() const
{
    return m_shape.getRadius();
}

const sf::Color& Planet::GetColor() const
{
    return m_shape.getFillColor();
}

const std::string Planet::GetTeam() const
{
    return m_team;
}

int Planet::GetMaxLevel() const
{
    return m_maxLevel;
}

int Planet::GetNextLevelIncrement() const
{
    return m_levelUpCounter;
}

int Planet::GetCurrentLevel() const
{
    return m_currentLevel;
}

int Planet::GetDestructionIncrement() const
{
    return m_destructionCounter;
}

void Planet::Update(float dt)
{
    m_levelUpIndicator.Update(dt);
    m_destructionIndicator.Update(dt);

    if (m_currentLevel == 0)
    {
        m_shape.setFillColor(NEUTRAL_COLOR);
        m_shape.setOutlineThickness(-3);
        m_shape.setOutlineColor(m_col);
    }
    else
    {
        m_shape.setFillColor(m_col);
        m_shape.setOutlineThickness(0);
    }
}

void Planet::Draw(sf::RenderWindow* wnd)
{
    wnd->draw(m_shape);
    m_levelUpIndicator.Draw(wnd);
    m_destructionIndicator.Draw(wnd);
}

void Planet::DrawInfo(sf::RenderWindow* wnd)
{
    size_t size = m_levelIndicator.Size();
    for (size_t i = 0; i < size; i++)
        wnd->draw(m_levelIndicator[i]);
    wnd->draw(m_text);
}

void Planet::_updateFromLevel()
{
    m_shape.setRadius(START_RADIUS + m_currentLevel * 0.5f * START_RADIUS);
    m_shape.setOrigin(sf::Vector2f(GetRadius(), GetRadius()));
}

void Planet::_updateText()
{
    auto gb = m_text.getGlobalBounds();
    m_text.setOrigin(sf::Vector2f(gb.width * 0.5f, gb.height * 0.5f));
    sf::Vector2f pos = GetPosition();
    m_text.setPosition(pos - sf::Vector2f(0, GetRadius() + gb.height));
}

void Planet::_updateLevelIndicator()
{
    sf::Color col = GetColor();
    float r = INDICATOR_RADIUS;
    float width = ((m_maxLevel - 2) * r * 2) * 0.5f;
    for (int i = 0; i < m_maxLevel - 1; i++)
    {
        m_levelIndicator[i].setFillColor(NEUTRAL_COLOR);
        m_levelIndicator[i].setOutlineThickness(-2);
        m_levelIndicator[i].setOutlineColor(sf::Color::Black);
        m_levelIndicator[i].setRadius(r);
        m_levelIndicator[i].setOrigin(sf::Vector2f(r, r));
        m_levelIndicator[i].setPosition(GetPosition() + sf::Vector2f((i * r * 2) - width, GetRadius() + r * 2));
    }

    for (int i = 1; i < m_currentLevel; i++)
    {
        int index = i - 1;
        m_levelIndicator[index].setFillColor(GetColor());
    }
}

void Planet::_updateIndicators()
{
    float r = GetRadius();
    sf::Vector2f center = GetPosition() + sf::Vector2f(0.0f, r);
    sf::Vector2f size(INDICATOR_RADIUS, -r * 2);

    m_levelUpIndicator.SetPosition(center.x + r + 4.0f, center.y);
    m_destructionIndicator.SetPosition(center.x - (r + 4.0f + size.x), center.y);

    m_levelUpIndicator.SetSize(size);
    m_destructionIndicator.SetSize(size);

    m_levelUpIndicator.SetCurrentValue((float)m_levelUpCounter);
    m_destructionIndicator.SetCurrentValue((float)m_destructionCounter);
}
